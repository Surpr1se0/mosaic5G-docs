#define MAX_UES 64

#include "../../../../src/xApp/e42_xapp_api.h"
#include "../../../../src/util/alg_ds/alg/defer.h"
#include "../../../../src/util/time_now_us.h"
#include "../../../../src/util/alg_ds/ds/lock_guard/lock_guard.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>
#include <assert.h>

typedef struct {
  uint32_t prev_bytes;
  int64_t  prev_ts;
} mac_state_t;

static mac_state_t mac_state[MAX_UES];
static uint64_t cnt_mac  = 0;
static uint64_t cnt_pdcp = 0;
static int64_t  start_time_us = 0;

static void sm_cb_mac(sm_ag_if_rd_t const* rd)
{
  int64_t now = time_now_us();
  int64_t elapsed_time_ms = (now - start_time_us) / 1000;

  assert(rd != NULL);
  assert(rd->type == INDICATION_MSG_AGENT_IF_ANS_V0);
  assert(rd->ind.type == MAC_STATS_V0);

  for (size_t i = 0; i < rd->ind.mac.msg.len_ue_stats; ++i) {
    mac_ue_stats_impl_t* s = &rd->ind.mac.msg.ue_stats[i];
    uint32_t rnti = s->rnti;

    mac_state_t* st = &mac_state[rnti % MAX_UES];

    if (st->prev_ts != 0) {
      int64_t delta_t_us = now - st->prev_ts;
      if (delta_t_us >= 100000) { // 0.1s

        double delta_t_s = delta_t_us / 1e6;

        uint32_t ul_bytes = s->ul_aggr_tbs; 
        double throughput_mbps = ((ul_bytes  - st->prev_bytes) * 8.0) / (1e6 * delta_t_s);
        printf("%" PRId64 " ms [UE %" PRIu32 "] UL Throughput = %.5f Mbps\n",
               elapsed_time_ms, rnti, throughput_mbps);

        st->prev_ts   = now;
        st->prev_bytes = s->ul_aggr_tbs;
      }
    } else {
      st->prev_ts   = now;
      st->prev_bytes = s->ul_aggr_tbs;
    }
  }

  cnt_mac++;
}

static void sm_cb_pdcp(sm_ag_if_rd_t const* rd)
{
  assert(rd != NULL);
  assert(rd->type == INDICATION_MSG_AGENT_IF_ANS_V0);
  assert(rd->ind.type == PDCP_STATS_V0);

  int64_t now = time_now_us();
  int64_t elapsed_ms = (now - start_time_us) / 1000;

  int64_t ind_latency_us = now - rd->ind.pdcp.msg.tstamp;

  if ((cnt_pdcp % 1024) == 0) {
    printf("%" PRId64 " ms | now=%" PRId64 " us | PDCP ind_msg latency=%" PRId64 " us\n",
           elapsed_ms, now, ind_latency_us);
  }

  cnt_pdcp++;
}

int main(int argc, char *argv[])
{
  fr_args_t args = init_fr_args(argc, argv);
  init_xapp_api(&args);
  sleep(1);

  start_time_us = time_now_us();

  e2_node_arr_xapp_t nodes = e2_nodes_xapp_api();
  defer({ free_e2_node_arr_xapp(&nodes); });
  assert(nodes.len > 0);

  printf("Connected E2 nodes = %d\n", nodes.len);

  const char* mac_interval  = "10_ms";
  const char* pdcp_interval = "1_ms";

  sm_ans_xapp_t* mac_handle  = NULL;
  sm_ans_xapp_t* pdcp_handle = NULL;

  if (nodes.len > 0) {
    mac_handle  = calloc(nodes.len,  sizeof(sm_ans_xapp_t));
    pdcp_handle = calloc(nodes.len,  sizeof(sm_ans_xapp_t));
    assert(mac_handle  != NULL);
    assert(pdcp_handle != NULL);
  }

  for (int i = 0; i < nodes.len; i++) {
    e2_node_connected_xapp_t* n = &nodes.n[i];

    for (size_t j = 0; j < n->len_rf; j++)
      printf("[xAPP] Registered node %d ran func id = %d\n", i, n->rf[j].id);

    // PDCP é no CU-UP
    if (n->id.type == ngran_gNB_CUUP) {
      pdcp_handle[i] = report_sm_xapp_api(&n->id, 144, (void*)pdcp_interval, sm_cb_pdcp);
      assert(pdcp_handle[i].success == true);
    }

    if (n->id.type == ngran_gNB_DU) {
      mac_handle[i] = report_sm_xapp_api(&n->id, 142, (void*)mac_interval, sm_cb_mac);
      assert(mac_handle[i].success == true);
    }
  }

  sleep(90);

  for (int i = 0; i < nodes.len; ++i) {
    if (pdcp_handle && pdcp_handle[i].u.handle != 0) rm_report_sm_xapp_api(pdcp_handle[i].u.handle);
    if (mac_handle  && mac_handle[i].u.handle  != 0) rm_report_sm_xapp_api(mac_handle[i].u.handle);
  }

  free(pdcp_handle);
  free(mac_handle);

  while (try_stop_xapp_api() == false)
    usleep(1000);

  printf("MAC/PDCP xApp run SUCCESSFULLY\n");
  return 0;
}

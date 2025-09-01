/*
 * Adapted version of the original PDCP throughput xApp
 * Now fetches MAC metrics and prints per-RNTI (no throughput, just active RNTIs over time).
 */

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

// Basic MAC UE state to track timestamps (not used for throughput)
typedef struct {
    int64_t prev_ts;
    uint32_t prev_bytes;
} mac_state_t;

mac_state_t mac_state[MAX_UES];
static uint64_t cnt_mac = 0;
static int64_t start_time_us = 0;

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
        printf("%" PRId64 " [UE %u]\n", elapsed_time_ms, rnti);
        st->prev_ts = now;
        st->prev_bytes = s->ul_aggr_bytes_sdus;
      }
    } else {
      st->prev_ts = now;
      st->prev_bytes = s->ul_aggr_bytes_sdus;
    }
  }

  cnt_mac++;
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

  const char* interval = "10_ms";
  sm_ans_xapp_t* mac_handle = NULL;

  if (nodes.len > 0) {
    mac_handle = calloc(nodes.len, sizeof(sm_ans_xapp_t));
    assert(mac_handle != NULL);
  }

  for (int i = 0; i < nodes.len; i++) {
    e2_node_connected_xapp_t* n = &nodes.n[i];
    for (size_t j = 0; j < n->len_rf; j++)
      printf("[xAPP] Registered node %d ran func id = %d\n", i, n->rf[j].id);

    if (n->id.type == ngran_gNB_DU || n->id.type == ngran_gNB_CUUP) {
      mac_handle[i] = report_sm_xapp_api(&nodes.n[i].id, 142, (void*)interval, sm_cb_mac);
      assert(mac_handle[i].success == true);
    }
  }

  sleep(90);

  for (int i = 0; i < nodes.len; ++i) {
    if (mac_handle[i].u.handle != 0)
      rm_report_sm_xapp_api(mac_handle[i].u.handle);
  }

  if (nodes.len > 0) {
    free(mac_handle);
  }

  while (try_stop_xapp_api() == false)
    usleep(1000);

  printf("MAC Stats xApp run SUCCESSFULLY\n");
}

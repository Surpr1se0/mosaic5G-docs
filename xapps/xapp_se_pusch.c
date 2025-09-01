/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements...
 */

#define MAX_UES 1024
#define CARRIER_BANDWIDTH_MHZ 38.16

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
#include <math.h>
#include <assert.h>
#include <pthread.h>


typedef struct {
  int64_t prev_ts_us;  
} ue_state_t;

static ue_state_t state[MAX_UES];   
static int64_t start_time_us = 0;  
static pthread_mutex_t state_mu = PTHREAD_MUTEX_INITIALIZER;

static const double Tp = 3.0;   
static const double Tc = 200.0; 
static const int64_t PRINT_INTERVAL_US = 100000;

static void sm_cb_mac(sm_ag_if_rd_t const* rd)
{
  int64_t now = time_now_us();

  int64_t elapsed_time_ms = (now - start_time_us) / 1000;

  assert(rd != NULL);
  assert(rd->type == INDICATION_MSG_AGENT_IF_ANS_V0);
  assert(rd->ind.type == MAC_STATS_V0);

  pthread_mutex_lock(&state_mu);

  for (size_t i = 0; i < rd->ind.mac.msg.len_ue_stats; ++i) {
    mac_ue_stats_impl_t* ue = &rd->ind.mac.msg.ue_stats[i];
    uint32_t rnti = ue->rnti;

    ue_state_t* st = &state[rnti % MAX_UES];

    if (st->prev_ts_us == 0 || (now - st->prev_ts_us) >= PRINT_INTERVAL_US) {
      double overhead = 1.0 - (Tp / Tc);

      // UPLINK (PUSCH)
      double pusch_snr_db = ue->pusch_snr;
      double pusch_snr_linear = pow(10.0, pusch_snr_db / 10.0);
      double se_ul = overhead * log2(1.0 + pusch_snr_linear);

      printf("[%5" PRId64 " ms] [UE %u] UL: SNR = %.2f dB | SE = %.3f bit/s/Hz\n",
             elapsed_time_ms, rnti, pusch_snr_db, se_ul);

      st->prev_ts_us = now;
    }
  }

  pthread_mutex_unlock(&state_mu);
}

int main(int argc, char *argv[])
{
  fr_args_t args = init_fr_args(argc, argv);

  // Init the xApp
  init_xapp_api(&args);
  sleep(1);

  // Store the start time of the xApp
  start_time_us = time_now_us();

  for (int i = 0; i < MAX_UES; ++i) state[i].prev_ts_us = 0;

  e2_node_arr_xapp_t nodes = e2_nodes_xapp_api();
  defer({ free_e2_node_arr_xapp(&nodes); });

  assert(nodes.len > 0);
  printf("Connected E2 nodes = %d\n", nodes.len);

  const char* i_2 = "10_ms"; 
  sm_ans_xapp_t* mac_handle = NULL;

  if (nodes.len > 0){
    mac_handle = calloc(nodes.len, sizeof(sm_ans_xapp_t));
    assert(mac_handle != NULL);
  }

  for (int i = 0; i < nodes.len; i++) {
    e2_node_connected_xapp_t* n = &nodes.n[i];

    for (size_t j = 0; j < n->len_rf; j++){
      printf("[xAPP] Registered node %d ran func id = %d \n", i, n->rf[j].id);
    }

    if (n->id.type == ngran_gNB || n->id.type == ngran_eNB){
      // MAC Control is not yet implemented in OAI RAN
      printf("[xAPP] gNB or eNB, don't do anything\n");

    } else if (n->id.type == ngran_gNB_CU || n->id.type == ngran_gNB_CUUP){
      printf("[xAPP] CU or CUUP, don't do anything\n");

    } else if (n->id.type == ngran_gNB_DU){
      printf("[xApp] DU detected\n");
      mac_handle[i] = report_sm_xapp_api(&nodes.n[i].id, 142, (void*)i_2, sm_cb_mac);
      assert(mac_handle[i].success == true);
    }
  }
  sleep(90);

  for (int i = 0; i < nodes.len; ++i){
    if (mac_handle[i].u.handle != 0)
      rm_report_sm_xapp_api(mac_handle[i].u.handle);
  }

  if (nodes.len > 0){
    free(mac_handle);
  }

  while (try_stop_xapp_api() == false)
    usleep(1000);

  printf("Test xApp run SUCCESSFULLY\n");
}

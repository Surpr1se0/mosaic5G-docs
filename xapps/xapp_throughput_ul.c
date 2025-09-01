/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
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

typedef struct {
    uint32_t prev_bytes;
    int64_t prev_ts;
} pdcp_state_t;

pdcp_state_t state[MAX_UES][32]; // 32 bearers/ UE
static uint64_t cnt_pdcp;
static int64_t start_time_us = 0;  // Global time

static void sm_cb_pdcp(sm_ag_if_rd_t const* rd)
{
  int64_t now = time_now_us();

  // Elapsed time since xApp start
  int64_t elapsed_time_ms = (now - start_time_us) / 1000;

  assert(rd != NULL);
  assert(rd->type == INDICATION_MSG_AGENT_IF_ANS_V0);
  assert(rd->ind.type == PDCP_STATS_V0);

  for (size_t i = 0; i < rd->ind.pdcp.msg.len; ++i) {
    pdcp_radio_bearer_stats_t* s = &rd->ind.pdcp.msg.rb[i];
    uint32_t rnti = s->rnti;
    uint8_t rbid = s->rbid;

    pdcp_state_t* st = &state[rnti % MAX_UES][rbid];

    if (st->prev_ts != 0) {
      int64_t delta_t_us = now - st->prev_ts;
      double delta_t_s = delta_t_us / 1e6;

      //printf("[delta_t_us] %" PRId64 " µs\n", delta_t_us);
      //printf("[delta_t_s] %.2f s\n", delta_t_s);
      //printf("[txsdu_bytes] %u\n", s->txsdu_bytes);
      //printf("[prev_bytes] %u\n", st->prev_bytes);

      if (delta_t_us >= 100000) { // 0.1s = 100 ms = 100000 µs
        double throughput_mbps = ((s->txsdu_bytes - st->prev_bytes) * 8.0) / (1e6 * delta_t_s);
        printf("%" PRId64 "[UE %u | RB %u] Throughput = %.5f Mbps\n", elapsed_time_ms, rnti, rbid, throughput_mbps);

        st->prev_ts = now;
        st->prev_bytes = s->txsdu_bytes;
      }
    } else {
      st->prev_ts = now;
      st->prev_bytes = s->txsdu_bytes;
    }
  }

  cnt_pdcp++;
}

int main(int argc, char *argv[])
{
  fr_args_t args = init_fr_args(argc, argv);

  //Init the xApp
  init_xapp_api(&args);
  sleep(1);

  // Store the start time of the xApp
  start_time_us = time_now_us();

  e2_node_arr_xapp_t nodes = e2_nodes_xapp_api();
  defer({ free_e2_node_arr_xapp(&nodes); });

  assert(nodes.len > 0);

  printf("Connected E2 nodes = %d\n", nodes.len);

  // PDCP indication
  const char* i_2 = "10_ms"; // 1, 2, 5, 10, no more
  sm_ans_xapp_t* pdcp_handle = NULL;

  if(nodes.len > 0){
    pdcp_handle = calloc( nodes.len, sizeof(sm_ans_xapp_t) ); 
    assert(pdcp_handle  != NULL);
  }

  for (int i = 0; i < nodes.len; i++) {
    e2_node_connected_xapp_t* n = &nodes.n[i];
    for (size_t j = 0; j < n->len_rf; j++)
      printf("[xAPP] Registered node %d ran func id = %d \n ", i, n->rf[j].id);

      if(n->id.type ==  ngran_gNB_CU || n->id.type ==  ngran_gNB_CUUP){
        pdcp_handle[i] = report_sm_xapp_api(&nodes.n[i].id, 144, (void*)i_2, sm_cb_pdcp);
        assert(pdcp_handle[i].success == true);
      }
  }

  // TIME - time for experiment - add 5 seconds to iperf to allow for the xapp to start
  sleep(90);


  for(int i = 0; i < nodes.len; ++i){
    // Remove the handle previously returned
    if(pdcp_handle[i].u.handle != 0)
      rm_report_sm_xapp_api(pdcp_handle[i].u.handle);
  }

  if(nodes.len > 0){
    free(pdcp_handle);
  }

  //Stop the xApp
  while(try_stop_xapp_api() == false)
    usleep(1000);

  printf("Test xApp run SUCCESSFULLY\n");
}

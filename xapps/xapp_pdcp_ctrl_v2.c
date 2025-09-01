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
#define E2AP_V3 1
#define KPM_V3_00 1


#include "../../../../src/xApp/e42_xapp_api.h"
#include "../../../../src/util/alg_ds/alg/defer.h"
#include "../../../../src/util/time_now_us.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>


static volatile int trigger_mac_ctrl = 0;  // global flag
static pthread_mutex_t mac_ctrl_mutex = PTHREAD_MUTEX_INITIALIZER;
static uint64_t cnt_mac;
static uint64_t cnt_pdcp;
static int64_t latency_threshold_us = 38503000;  // default value


static void sm_cb_mac(sm_ag_if_rd_t const* rd)
{
  assert(rd != NULL);
  assert(rd->type ==INDICATION_MSG_AGENT_IF_ANS_V0);
  assert(rd->ind.type == MAC_STATS_V0);
 
  int64_t now = time_now_us();
  if(cnt_mac % 1024 == 0)
    printf("MAC ind_msg latency = %ld μs\n", now - rd->ind.mac.msg.tstamp);
  cnt_mac++;
}


static void sm_cb_pdcp(sm_ag_if_rd_t const* rd)
{
  assert(rd != NULL);
  assert(rd->type == INDICATION_MSG_AGENT_IF_ANS_V0);
  assert(rd->ind.type == PDCP_STATS_V0);

  int64_t now = time_now_us();

  if(cnt_pdcp % 1024 == 0)
  {
    printf("PDCP ind_msg latency = %ld μs\n", now - rd->ind.pdcp.msg.tstamp);

    if(now - rd->ind.pdcp.msg.tstamp > latency_threshold_us)
    {
      pthread_mutex_lock(&mac_ctrl_mutex);
      printf("[xApp] DETECTED HIGH LATENCY\n");
      trigger_mac_ctrl = 1;
      pthread_mutex_unlock(&mac_ctrl_mutex);
    }
  }
  cnt_pdcp++;
}

void* control_thread(void* arg){
  e2_node_arr_xapp_t* nodes = (e2_node_arr_xapp_t*)arg;
  while(1){
    sleep(1);
    pthread_mutex_lock(&mac_ctrl_mutex);
    if(trigger_mac_ctrl == 1){
      trigger_mac_ctrl = 0; // reset the flag
      pthread_mutex_unlock(&mac_ctrl_mutex);
      for(int i = 0; i < nodes->len; i++){
        if(nodes->n[i].id.type == ngran_gNB_DU){
          mac_ctrl_req_data_t wr = {.hdr.dummy = 1, .msg.action = 42 };
          sm_ans_xapp_t a = control_sm_xapp_api(&nodes->n[i].id, 142, &wr);
          if(a.success){
            printf("[xApp] Control sent to MAC on node %d\n", i);
          } else {
            printf("[xApp] Failed to send control to MAC on node %d\n", i);
          }
        } 
      }
    } else {
      pthread_mutex_unlock(&mac_ctrl_mutex);
    }
  }
}

int main(int argc, char *argv[])
{
  if(argc >= 2) {
    latency_threshold_us = atoll(argv[1]) * 1000;  
    printf("\nLatency threshold set to %ld μs\n", latency_threshold_us);
  } else {
    printf("Using default latency threshold: %ld μs\n", latency_threshold_us);
  }

  fr_args_t args = init_fr_args(argc, argv);

  //Init the xApp
  init_xapp_api(&args);
  sleep(1);

  e2_node_arr_xapp_t nodes = e2_nodes_xapp_api();
  defer({ free_e2_node_arr_xapp(&nodes); });

  assert(nodes.len > 0);

  printf("Connected E2 nodes = %d\n", nodes.len);

  // MAC indication
  const char* i_0 = "1_ms";
  sm_ans_xapp_t* mac_handle = NULL;
  
  // PDCP indication
  const char* i_2 = "1_ms";
  sm_ans_xapp_t* pdcp_handle = NULL;

  if(nodes.len > 0){
    mac_handle = calloc( nodes.len, sizeof(sm_ans_xapp_t) ); 
    assert(mac_handle  != NULL);
    pdcp_handle = calloc( nodes.len, sizeof(sm_ans_xapp_t) ); 
    assert(pdcp_handle  != NULL);
  }

  for (int i = 0; i < nodes.len; i++) {
    e2_node_connected_xapp_t* n = &nodes.n[i];
    for (size_t j = 0; j < n->len_rf; j++){
      printf("[xAPP] Registered node %d ran func id = %d \n", i, n->rf[j].id);
    }
    if(n->id.type == ngran_gNB || n->id.type == ngran_eNB){
      // MAC Control is not yet implemented in OAI RAN
      printf("[xAPP] gNB or eNB, don't do anything\n");

      pdcp_handle[i] = report_sm_xapp_api(&nodes.n[i].id, 144, (void*)i_2, sm_cb_pdcp);
      assert(pdcp_handle[i].success == true);

    } else if(n->id.type ==  ngran_gNB_CU || n->id.type ==  ngran_gNB_CUUP){
      printf("[xAPP] CU or CUUP, don't do anything\n");
      
      pdcp_handle[i] = report_sm_xapp_api(&nodes.n[i].id, 144, (void*)i_2, sm_cb_pdcp);
      assert(pdcp_handle[i].success == true);

    } else if(n->id.type == ngran_gNB_DU){
      printf("[xApp] DU, will send control to MAC\n");
      mac_ctrl_req_data_t wr = {.hdr.dummy = 1, .msg.action = 42 };
      sm_ans_xapp_t const a = control_sm_xapp_api(&nodes.n[i].id, 142, &wr);
      assert(a.success == true);

      mac_handle[i] = report_sm_xapp_api(&nodes.n[i].id, 142, (void*)i_0, sm_cb_mac);
      assert(mac_handle[i].success == true);
    }
  }

  pthread_t ctrl_thread;
  pthread_create(&ctrl_thread, NULL, control_thread, &nodes);

  sleep(30);


  for(int i = 0; i < nodes.len; ++i){
    // Remove the handle previously returned
    if(mac_handle[i].u.handle != 0 )
      rm_report_sm_xapp_api(mac_handle[i].u.handle);
    if(pdcp_handle[i].u.handle != 0)
      rm_report_sm_xapp_api(pdcp_handle[i].u.handle);
  }

  if(nodes.len > 0){
    free(mac_handle);
    free(pdcp_handle);
  }

  //Stop the xApp
  while(try_stop_xapp_api() == false)
    usleep(1000);

  printf("Test xApp run SUCCESSFULLY\n");
}




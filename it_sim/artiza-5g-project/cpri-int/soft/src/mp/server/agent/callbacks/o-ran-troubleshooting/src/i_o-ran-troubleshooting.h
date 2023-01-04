/*
 * @file    i_o-ran-troubleshooting.h
 */

#ifndef _H_i_o_ran_troubleshooting
#define _H_i_o_ran_troubleshooting
/*!
 * @addtogroup  SAA_MOD_O_RAN_TROUBLESHOOTING
 * @brief       SAA o-ran-troubleshooting
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <pthread.h>
#include "u_o-ran-troubleshooting.h"

#include "f_mpsw_msg_o-ran_troubleshooting_def.h"
#include "f_mpsw_msg_o-ran_troubleshooting_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_ntf_lib.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_o_ran_troubleshooting_N_status_success     "SUCCESS"   /* rpc応答 成功 */
#define i_o_ran_troubleshooting_N_status_failure     "FAILURE"   /* rpc応答 失敗 */

#define D_O_RAN_TROUBLESHOOTING_FAILURE_STR_MAX  32  /* failure_reason最大文字数 */
#define D_O_RAN_TROUBLESHOOTING_LOG_FILE_STR_MAX 128 /* ログファイル最大文字数   */
/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/
typedef enum
{
    E_MPSW_TROUBLESHOOTING_STOP = 0,    /**< トラブルシューティングログ収集 停止状態    */
    E_MPSW_TROUBLESHOOTING_START        /**< トラブルシューティングログ収集 開始状態    */
}E_MPSW_TROUBLESHOOTING;

/* struct of higher-layer-if and low_layer_if */
typedef struct troubleshooting_status_t_{
    xmlChar status[D_O_RAN_TROUBLESHOOTING_FAILURE_STR_MAX];             /**< rpc 失敗要因           */
    xmlChar failure_reason[D_O_RAN_TROUBLESHOOTING_FAILURE_STR_MAX];     /**< rpc 失敗要因           */
}troubleshooting_status_t;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/* rpc start-troubleshooting-logs */
extern status_t i_o_ran_troubleshooting_start_troubleshooting_logs_invoke(troubleshooting_status_t *reply);

/* rpc stop-troubleshooting-logs */
extern status_t i_o_ran_troubleshooting_stop_troubleshooting_logs_invoke(troubleshooting_status_t *reply);

/* notification troubleshooting-log-generated */
extern void i_o_ran_troubleshooting_troubleshooting_log_generated_handler(void *data, uint32_t size);

#endif

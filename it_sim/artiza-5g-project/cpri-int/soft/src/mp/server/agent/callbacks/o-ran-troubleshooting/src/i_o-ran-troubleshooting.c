
/*
 * @file    i_o-ran-troubleshooting.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_TROUBLESHOOTING
 * @brief       SAA o-ran-troubleshooting
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "i_o-ran-troubleshooting.h"

static E_MPSW_TROUBLESHOOTING  troubleshooting_state = E_MPSW_TROUBLESHOOTING_STOP;  /**< トラブルシューティングログ収集状態 */
static pthread_mutex_t mutex_for_troubleshooting_state = PTHREAD_MUTEX_INITIALIZER;
/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   MSI_O_RAN_TROUBLESHOOTING_START_LOGS送信関数
 * @note    MSI_O_RAN_TROUBLESHOOTING_START_LOGSを送信する
 * @param   [in] reply
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/1/28 KCN)南條
 */
status_t i_o_ran_troubleshooting_start_troubleshooting_logs_invoke(troubleshooting_status_t *reply)
{
    status_t res = NO_ERR;
    void *req_data;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ  reply_data;
    int ret = D_MSG_BUFFER_GET_OK;
    (void)reply;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_troubleshooting_state);

    /* 既にトラブルシューティングログ収集が開始している場合MSIは送信しない */
    if(troubleshooting_state == E_MPSW_TROUBLESHOOTING_START)
    {
        if (LOGINFO) {
            log_info("\nAlready troubleshooting started.");
        }
        xml_strcpy(reply->status, (const xmlChar *)i_o_ran_troubleshooting_N_status_success);

        /* unlock */
        pthread_mutex_unlock(&mutex_for_troubleshooting_state);
        return res;
    }

    /* トラブルシューティングログ収集を開始状態に設定 */
    troubleshooting_state = E_MPSW_TROUBLESHOOTING_START;

    /* unlock */
    pthread_mutex_unlock(&mutex_for_troubleshooting_state);

    /* MSI_O_RAN_TROUBLESHOOTING_START_LOGS送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REQ, 0, &req_data);
    if(response_id == 0) {
        /* lock */
        pthread_mutex_lock(&mutex_for_troubleshooting_state);

        /* トラブルシューティングログ収集を停止状態に設定 */
        troubleshooting_state = E_MPSW_TROUBLESHOOTING_STOP;

        /* unlock */
        pthread_mutex_unlock(&mutex_for_troubleshooting_state);

        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_TROUBLESHOOTING_START_LOGSレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ),
        &message_id,
        &read_size,
        &reply_data,
        D_MSG_WAIT_TIME );

    char v_reply_reason_str[D_O_RAN_TROUBLESHOOTING_FAILURE_STR_MAX] = "";
    /* MSI_O_RAN_TROUBLESHOOTING_START_LOGSレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id == D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_CFM) {
            xml_strcpy(reply->status, (const xmlChar *)i_o_ran_troubleshooting_N_status_success);
            if(LOGINFO) {
                log_info("\n%s: MSI success: wrong message id. (%0x).", __func__, message_id);
            }
        } else if(message_id == D_MSG_MSI_O_RAN_TROUBLESHOOTING_START_LOGS_REJ) {
            /* lock */
            pthread_mutex_lock(&mutex_for_troubleshooting_state);

            /* トラブルシューティングログ収集を停止状態に設定 */
            troubleshooting_state = E_MPSW_TROUBLESHOOTING_STOP;

            /* unlock */
            pthread_mutex_unlock(&mutex_for_troubleshooting_state);

            xml_strcpy(reply->status, (const xmlChar *)i_o_ran_troubleshooting_N_status_failure);
            snprintf(v_reply_reason_str, D_O_RAN_TROUBLESHOOTING_FAILURE_STR_MAX, "%s", reply_data.msgErr[0].errorDescription);
            xml_strcpy(reply->failure_reason, (const xmlChar *)v_reply_reason_str);
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x).", __func__, message_id);
            }
        } else {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x).", __func__, message_id);
            }
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: too small buffer size.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_TIMEOUT:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: time out.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_OTHER:
    default:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: other.", __func__);
        }
    }

    if(res != NO_ERR)
    {
        /* lock */
        pthread_mutex_lock(&mutex_for_troubleshooting_state);

        /* トラブルシューティングログ収集を停止状態に設定 */
        troubleshooting_state = E_MPSW_TROUBLESHOOTING_STOP;

        /* unlock */
        pthread_mutex_unlock(&mutex_for_troubleshooting_state);
    }

    return res;
} /* i_o_ran_troubleshooting_start_troubleshooting_logs_invoke */

/*!
 * @brief   MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS送信関数
 * @note    MSI_O_RAN_TROUBLESHOOTING_STOP_LOGSを送信する
 * @param   [in] reply
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/1/28 KCN)南條
 */
status_t i_o_ran_troubleshooting_stop_troubleshooting_logs_invoke(troubleshooting_status_t *reply)
{
    status_t res = NO_ERR;
    void *req_data;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ  reply_data;
    int ret = D_MSG_BUFFER_GET_OK;
    (void)reply;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_troubleshooting_state);

    /* トラブルシューティングログ収集が開始していない場合MSIは送信しない */
    if(troubleshooting_state == E_MPSW_TROUBLESHOOTING_STOP)
    {
        if (LOGINFO) {
            log_info("\ntroubleshooting isn't started.");
        }
        xml_strcpy(reply->status, (const xmlChar *)i_o_ran_troubleshooting_N_status_success);

        /* unlock */
        pthread_mutex_unlock(&mutex_for_troubleshooting_state);

        return res;
    }

    /* トラブルシューティングログ収集を停止状態に設定 */
    troubleshooting_state = E_MPSW_TROUBLESHOOTING_STOP;

    /* unlock */
    pthread_mutex_unlock(&mutex_for_troubleshooting_state);

    /* MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REQ, 0, &req_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_TROUBLESHOOTING_STOP_LOGSレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        sizeof(T_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ),
        &message_id,
        &read_size,
        &reply_data,
        D_MSG_WAIT_TIME );

    char v_reply_reason_str[D_O_RAN_TROUBLESHOOTING_FAILURE_STR_MAX] = "";
    /* MSI_O_RAN_TROUBLESHOOTING_STOP_LOGSレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id == D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_CFM) {
            xml_strcpy(reply->status, (const xmlChar *)i_o_ran_troubleshooting_N_status_success);
            if(LOGINFO) {
                log_info("\n%s: MSI success: wrong message id. (%0x).", __func__, message_id);
            }
        } else if(message_id == D_MSG_MSI_O_RAN_TROUBLESHOOTING_STOP_LOGS_REJ) {
            xml_strcpy(reply->status, (const xmlChar *)i_o_ran_troubleshooting_N_status_failure);
            snprintf(v_reply_reason_str, D_O_RAN_TROUBLESHOOTING_FAILURE_STR_MAX, "%s", reply_data.msgErr[0].errorDescription);
            xml_strcpy(reply->failure_reason, (const xmlChar *)v_reply_reason_str);
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x).", __func__, message_id);
            }
        } else {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x).", __func__, message_id);
            }
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: too small buffer size.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_TIMEOUT:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: time out.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_OTHER:
    default:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: other.", __func__);
        }
    }

    return res;
} /* i_o_ran_troubleshooting_stop_troubleshooting_logs_invoke */

/*!
 * @brief   MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND受信関数
 * @note    MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_INDを受信する
 * @param   [in] reply
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/1/28 KCN)南條
 */
void i_o_ran_troubleshooting_troubleshooting_log_generated_handler(void *data, uint32_t size)
{
    T_MPSW_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND  *ntf_datap;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_troubleshooting_state);

    /* トラブルシューティングログ収集が停止状態の場合notificationは読み捨て */
    if(troubleshooting_state == E_MPSW_TROUBLESHOOTING_STOP)
    {
        if (LOGINFO) {
            log_info("\n troubleshooting is stoped.(notification don't send.)");
        }

        /* unlock */
        pthread_mutex_unlock(&mutex_for_troubleshooting_state);
        return;
    }
    /* トラブルシューティングログ収集を停止状態に設定 */
    troubleshooting_state = E_MPSW_TROUBLESHOOTING_STOP;

    /* unlock */
    pthread_mutex_unlock(&mutex_for_troubleshooting_state);

    if(size < sizeof(T_MPSW_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND))
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND is failed: size below.(size:%d)", __func__, size);
        }
        return;
    }
    ntf_datap = (T_MPSW_MSI_O_RAN_TROUBLESHOOTING_LOGS_GENERATED_NTF_IND*)data;

    char v_log_file_name_str[D_O_RAN_TROUBLESHOOTING_LOG_FILE_STR_MAX] = "";
    xmlChar v_log_file_name[D_O_RAN_TROUBLESHOOTING_LOG_FILE_STR_MAX] = "";
    snprintf(v_log_file_name_str, D_O_RAN_TROUBLESHOOTING_LOG_FILE_STR_MAX, "%s", ntf_datap->log_file_name[0]);
    xml_strcpy(v_log_file_name, (xmlChar *)v_log_file_name_str);

    /* notification troubleshooting-log-generated送信 */
    u_o_ran_troubleshooting_troubleshooting_log_generated_send(v_log_file_name);

    return;
} /* i_o_ran_troubleshooting_troubleshooting_log_generated_handler */

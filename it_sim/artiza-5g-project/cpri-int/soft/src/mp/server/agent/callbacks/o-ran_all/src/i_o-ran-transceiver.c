/*
 * @file    i_o-ran-transceiver.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_TRANSCEIVER
 * @brief       SAA o-ran-transceiver
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "i_o-ran-transceiver.h"


/********************************************************************************************************************/
/*  static valuable                                                                                                 */
/********************************************************************************************************************/


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   MSI_O_RAN_TRANSCEIVER_GET送信関数
 * @note    
 * @param   [in] void
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_NOT_SUPPORTED
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/05 KCN)吉木
 */
status_t i_o_ran_transceiver_send_msi (
    void  *get_data,
    const xmlChar *k_port_transceivers_port_transceiver_data_interface_name,
    uint32 *port_num)
{
    status_t res = NO_ERR;
    uint32_t request_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    int ret = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI_O_RAN_TRANSCEIVER_GET送信 */
    request_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_TRANSCEIVER_GET_REQ, 0, NULL);
    if(request_id == 0) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSI_O_RAN_TRANSCEIVER_GETレスポンス待ち */
    ret = f_saa_com_get_response(request_id,
        max(sizeof(T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_CFM), sizeof(T_MPSW_MSI_O_RAN_TRANSCEIVER_GET_REJ)),
        &message_id,
        &read_size,
        (void *)get_data,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_TRANSCEIVER_GETレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_TRANSCEIVER_GET_CFM) {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x).", __func__, message_id);
            }
        }
        else {
            if(!xml_strcmp(k_port_transceivers_port_transceiver_data_interface_name, i_ietf_interface_N_name_fheth0)){
                *port_num = SAA_TRANS_PORT_NUM_0;
            }
            else {
                if(LOGWARN) {
                    log_warn("\n%s: invalid interface name, %s", __func__, k_port_transceivers_port_transceiver_data_interface_name);
                }
                return ERR_NCX_INVALID_VALUE;
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
} /* i_o_ran_transceiver_send_msi */

/*!
 * @brief   出力する文字列を成形する
 * @note    
 * @param   [in] source_str
 * @param   [in] length
 * @param   [out] target_str
 * @return  void
 * @date    2019/03/21 KCN)吉木
 */
void i_o_ran_transceiver_string_correction (
    xmlChar *target_str,
    xmlChar *source_str,
    unsigned int length)
{

    memset(target_str, '\0', (length+1));
    for(unsigned int i = 0, j = 0; i < length; i++) {
        if(source_str[i] == 0x3c) {
            target_str[j] = 0x7b;
            j++;
        }
        else if(source_str[i] == 0x3e) {
            target_str[j] = 0x7d;
            j++;
        }
        else if(source_str[i] !=  0x20 && source_str[i] != 0x00) {
                target_str[j] = source_str[i];
                j++;
        }
    }

    return;

} /* i_o_ran_transceiver_send_msi */

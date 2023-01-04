/*!
 *  @skip	$Id:$
 *  @file	f_saa_msi_lib.c
 *  @brief	SAA MSI library
 *  @author	KCN) Nakai Hiroyuki
 *  @date	2019/3/8 KCN)Nakai create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */


/*!
 * @addtogroup	SAA
 * @brief		NETCONF sub-agent.
 * @note		Works with netconfd-pro
 * @{
 */

#include <stdint.h>

/* YumaWorks SDK */
#include "log.h"

#include "f_saa_msg_buffer.h"
#include "f_saa_msi_lib.h"

/*!
 * @brief       PFにリクエストを送信し、レスポンスを得る。
 * @note        
 * @param       func_name       関数名(ログ用)
 * @param       message_id      送信メッセージID
 * @param       data_size       送信データサイズ
 * @param       request_buffer  送信データバッファ
 * @param       cfm_id          正常レスポンスのメッセージID
 * @param       rej_id          拒否レスポンスのメッセージID
 * @param       max_size        受信可能最大データサイズ
 * @param       response_buffer 受信データバッファ
 * @param       read_size       受信サイズ
 * @return      D_MSI_CFM:正常
 * @return      D_MSI_REJ:拒否
 * @return      D_MSI_WRONG_RESPONSE:不正なレスポンス
 * @return      D_MSI_REQUEST_ERR:リクエスト送信異常
 * @return      D_MSI_RESPONSE_ERR_SIZE:レスポンス受信異常(サイズ不足)
 * @return      D_MSI_RESPONSE_ERR_TIMEOUT:レスポンス受信異常(タイムアウト)
 * @return      D_MSI_RESPONSE_ERR_OTHER:レスポンス受信異常(その他)
 * @date        2019/3/1 KCN)Nakai create
 */
int f_saa_com_do_msi_request(uint32_t message_id, uint32_t data_size, void *request_buffer, uint32_t cfm_id, uint32_t rej_id, uint32_t max_size, void *response_buffer, uint32_t *read_size){
    int rc = D_MSI_CFM;
    int response_rc = -1;
    uint32_t request_id = 0;
    uint32_t response_id = 0;

    log_dev1("\n%s: message_id=%x, data_size=%d, request_buffer(addr)=%p, cfm_id=%x, rej_id=%x, max_size=%d, request_buffer(addr)=%p, reas_size(addr)=%p",
        __func__, message_id, data_size, request_buffer, cfm_id, rej_id, max_size, response_buffer, read_size);

    request_id = f_saa_com_add_request(message_id, data_size, request_buffer);
    if(request_id != 0){
        response_rc = f_saa_com_get_response(request_id, max_size, &response_id, read_size, response_buffer, D_MSG_WAIT_TIME);
        switch(response_rc){
        case D_MSG_BUFFER_GET_OK:
            if(response_id == cfm_id){
                rc = D_MSI_CFM;
            }
            else if(response_id == rej_id){
                rc = D_MSI_REJ;
            }
            else{
                log_dev0("\n%s: wrong response: rc=%d: request=%x, cfm_id=%x, rej_id=%x => response_rc=%d, response_id=%x, read_size=%d",
                    __func__, rc, message_id, cfm_id, rej_id, response_rc, response_id, *read_size);
                rc = D_MSI_WRONG_RESPONSE;
            }
            break;
        case D_MSG_BUFFER_GET_ERR_SIZE:
            rc = D_MSI_RESPONSE_ERR_SIZE;
            break;
        case D_MSG_BUFFER_GET_ERR_TIMEOUT:
            rc = D_MSI_RESPONSE_ERR_TIMEOUT;
            break;
        case D_MSG_BUFFER_GET_ERR_OTHER:
        /* no break */
        default:
            rc = D_MSI_RESPONSE_ERR_OTHER;
            break;
        }
    }
    else{
        rc = D_MSI_REQUEST_ERR;
    }

    log_dev1("\n%s: rc=%d: response_rc=%d, response_id=%x, read_size=%d",
        __func__, rc, response_rc, response_id, *read_size);

    return rc;
}


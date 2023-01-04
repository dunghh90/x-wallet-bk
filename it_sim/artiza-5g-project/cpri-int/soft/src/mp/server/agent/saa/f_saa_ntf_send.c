/*!
 *  @skip	$Id:$
 *  @file	f_saa_ntf_sender.c
 *  @brief	SAA inter-thread communication with buffers.
 *  @author	KCN) Nakai Hiroyuki
 *  @date	2019/1/24 KCN)Nakai create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup	SAA
 * @brief		NETCONF sub-agent.
 * @note		Works with netconfd-pro
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* YumaWorks SDK */
#include "log.h"

#include "f_saa_msg_buffer.h"
#include "f_saa_ntf_lib.h"
#include "f_saa_ntf_send.h"

/*!
 * @brief	f_saa_nft_sender
 * @note	notificationを送信する。
 * @param	
 * @return	
 * @date	2019/1/24 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */

#define BUFFER_SIZE (1024*10)

void f_saa_ntf_send(void){
    char     buffer[BUFFER_SIZE];
    uint32_t message_id = 0;
    uint32_t      size = 0;
    int      rc = D_MSG_BUFFER_GET_OK;
    void (*handler)(void *, uint32_t) = NULL;

    f_saa_com_notification_lock();

    while(1){
        rc = f_saa_com_get_notification(BUFFER_SIZE, &message_id, &size, buffer);
        if(rc == D_MSG_BUFFER_GET_EMPTY){
            break;
        }
        else if(rc == D_MSG_BUFFER_GET_OK){
            handler = f_saa_com_get_notification_handler(message_id);
            f_saa_com_log_notification(message_id, size, buffer, handler);
            if(handler != NULL){
                handler(buffer, size);
            }
            else{
                log_dev0("\nNotification handler is not registerd for %x", message_id);
            }
        }
        else {
             log_dev0("\nFailed to get notification message. (%d)", rc);
        }
    }

    f_saa_com_notification_unlock();

}

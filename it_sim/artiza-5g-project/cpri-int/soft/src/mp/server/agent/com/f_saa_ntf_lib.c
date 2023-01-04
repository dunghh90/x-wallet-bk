/*!
 *  @skip	$Id:$
 *  @file	f_saa_ntf_lib.c
 *  @brief	SAA manage notification handler functions.
 *  @author	KCN) Nakai Hiroyuki
 *  @date	2019/01/24 KCN)Nakai create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup	SAA
 * @brief		NETCONF sub-agent.
 * @note		Works with netconfd-pro
 * @{
 */


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

/* YumaWorks SDK */
#include "log.h"

#include "f_saa_ntf_lib.h"


#define MAX_REC 256
#define TYPE_NOTIFICATION 0x0705
#define D_COLUMNS 16

struct handler_record {
    uint32_t message_id;
    void (* handler)(void *data, uint32_t size);
};

	
static struct handler_record handler_table[MAX_REC];
static pthread_mutex_t handler_mutex = PTHREAD_MUTEX_INITIALIZER;

/*!
 * @brief	notification送信handler管理テーブルを初期化する。
 * @note	
 * @param	
 * @return	
 * @date	2019/1/24 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
void f_saa_com_init_notification_table(void){
	memset(handler_table, 0, sizeof(struct handler_record) * MAX_REC);
}

/*!
 * @brief	notification送信依頼MSIに対するhandlerを登録する。
 * @note	notification送信を行うSIL-SA関数は、本関数でhandlerを登録すること。
 * @param	message_id [in]メッセージID
 * @param	handler [in] notification送信関数(handler)
 * @return	
 * @date	2019/1/24 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
void f_saa_com_reg_notification_handler(uint32_t message_id, void (* handler)(void *data, uint32_t size)){
    int i = 0;
    struct handler_record *record;
    bool set = false;

    if((message_id & TYPE_NOTIFICATION) != TYPE_NOTIFICATION){
        log_dev0("\nmessage is not a notification indication. (%x)", message_id);
        return;
    }

    if(handler == NULL){
        log_dev0("\nmessage handler is NULL.");
        return;
    }

    pthread_mutex_lock(&handler_mutex);
    for(i=0; i< MAX_REC; i++){
        record = &handler_table[i];
        if(record->message_id == 0){
            record->message_id = message_id;
            record->handler = handler;
            set = true;
            break;
        }
    }
    pthread_mutex_unlock(&handler_mutex);

    if(!set){
        log_dev0("\nmessage handler table is full.");
    }

}

/*!
 * @brief	notification送信依頼を行うhandlerを取得する。
 * @note	notification送信処理は本関数でhandlerを取得すること。
 * @param	message_id [in]メッセージID
 * @return	handlerのアドレス
 * @date	2019/1/24 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
void(*f_saa_com_get_notification_handler(uint32_t message_id))(void *data, uint32_t size){

    void (* handler)(void *data, uint32_t size) = NULL;
    int i = 0;
    struct handler_record *record;
    bool set = false;

    if((message_id & TYPE_NOTIFICATION) != TYPE_NOTIFICATION){
        log_dev0("\nmessage is not a notification indication. (%x)", message_id);
        return NULL;
    }

    pthread_mutex_lock(&handler_mutex);
    for(i=0; i< MAX_REC; i++){
        record = &handler_table[i];
        if(record->message_id == message_id){
            handler = record->handler;
            set = true;
            break;
        }
        else if(record->message_id == 0){
            break;
        }
    }
    pthread_mutex_unlock(&handler_mutex);

    if(!set){
        log_dev0("\nhandler is not registered for %x", message_id);
    }
    
    return handler;
}

/*!
 * @brief	notification送信依頼をログに出力する。
 * @note	notification送信処理は本関数でhandlerを取得すること。
 * @param	message_id [in]メッセージID
 * @param	size [in]メッセージサイズ
 * @param	data [in]メッセージデータ
 * @param	handler [in] notification送信ハンドラのアドレス
 * @return	なし
 * @date	2019/7/5 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
void f_saa_com_log_notification(uint32_t message_id, uint32_t size, void *data, void (*handler)(void *, uint32_t)){
    uint32_t i = 0;
    int index = 0;
    int offset = 0;
    char buffer[64];

    memset(buffer, 0, sizeof(buffer));

    log_info("\n%s: message_id=%08x, size=%d, handler=%p", __func__, message_id, size, handler);
    for(i=0; i< size; i++){
        index = i % D_COLUMNS;
        if(index == 0){
            offset = i;
        }
        snprintf(buffer + (index * 2), 3, "%02x", *((char *)data + i));
        if(i % D_COLUMNS == (D_COLUMNS -1)){
            log_info("\n%s %08X: %s", __func__, offset, buffer);
            memset(buffer, 0, sizeof(buffer));
        }
    }
    if(strlen(buffer) > 0){
        log_info("\n%s %08X: %s", __func__, offset, buffer);
    }
}


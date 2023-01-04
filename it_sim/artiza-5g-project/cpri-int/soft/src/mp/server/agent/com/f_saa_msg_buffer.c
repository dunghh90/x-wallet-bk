/*!
 *  @skip	$Id:$
 *  @file	saa_msg_buffer.c
 *  @brief	SAA inter-thread communication with buffers.
 *  @author	KCN) Nakai Hiroyuki
 *  @date	2018/11/16 KCN)Nakai create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */


/*!
 * @addtogroup	SAA
 * @brief		NETCONF sub-agent.
 * @note		Works with netconfd-pro
 * @{
 */


#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "f_saa_msg_buffer.h"

//millisecond
#define DELAY_FOR_REQUEST 10L
#define DELAY_FOR_RESPONSE 10L
#define DELAY_FOR_NOTIFICATION 10L


typedef struct  {
	time_t added_time;
	uint32_t request_id; //if notification, ignore this.
	uint32_t message_id;
	uint32_t size;
	void *data;
} MSG_RECORD;

struct  msg_node {
	MSG_RECORD record;
	struct msg_node *previous;
	struct msg_node *next;
};
typedef struct msg_node MSG_NODE;

static uint32_t lib_request_id = 0;

static MSG_NODE *lib_buffer_for_request = NULL;
static MSG_NODE *lib_buffer_for_response = NULL;
static MSG_NODE *lib_buffer_for_notification = NULL;

static pthread_mutex_t lib_mutex_for_request = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lib_mutex_for_response = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lib_mutex_for_notification = PTHREAD_MUTEX_INITIALIZER;

static int get_record_data(MSG_RECORD *record, uint32_t max_size, uint32_t *request_id, uint32_t *message_id, uint32_t *read_size, void *data);
static MSG_NODE * create_node(uint32_t request_id, uint32_t message_id, uint32_t data_size, void *data);
static void add_node_to_list(MSG_NODE **list, MSG_NODE *node);
static void remove_node_from_list(MSG_NODE **list, MSG_NODE *node);
static int wait_for_message(long msec);

/*!
 * @brief	Userに対するRequestメッセージをバッファに追加する。
 * @note	本関数で送信依頼したRequestに対するResponseを待つ場合には本関数の戻り値をリクエストIDとしてf_saa_com_get_request()を呼ぶこと。
 * @param	message_id [in]メッセージID
 * @param	data_size [in] メッセージデータ本文のサイズ
 * @param	data [in] メッセージデータ本文のバッファへのアドレス
 * @return	0以上: 成功.リクエストID, 0:失敗
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
uint32_t f_saa_com_add_request(uint32_t message_id, uint32_t data_size, void *data){
	uint32_t rc = 0;
	MSG_NODE *node = NULL;
	uint32_t temp_request_id = 0;
	
	//lock
	pthread_mutex_lock(&lib_mutex_for_request);
	temp_request_id = lib_request_id + 1;
	if(temp_request_id == 0){
		temp_request_id ++;
	}

	node = create_node(temp_request_id, message_id, data_size, data);
	if(node != NULL){
		add_node_to_list(&lib_buffer_for_request, node);
		rc = lib_request_id = temp_request_id;
	}
	//unlock
	pthread_mutex_unlock(&lib_mutex_for_request);

	return rc;
}

/*!
 * @brief	Userから受信したResponseメッセージをバッファに追加する。
 * @note	N/A
 * @param	request_id [in]リクエストID
 * @param	message_id [in]メッセージID
 * @param	data_size [in] メッセージデータ本文のサイズ
 * @param	data [in] メッセージデータ本文のバッファへのアドレス
 * @return	D_MSG_BUFFER_ADD_OK: 成功, D_MSG_BUFFER_ADD_ERR:失敗
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
int f_saa_com_add_response(uint32_t request_id, uint32_t message_id, uint32_t data_size, void *data){
	uint32_t rc = D_MSG_BUFFER_ADD_ERR;
	MSG_NODE *node = NULL;

	//lock
	pthread_mutex_lock(&lib_mutex_for_response);
	node = create_node(request_id, message_id, data_size, data);
	if(node != NULL){
		add_node_to_list(&lib_buffer_for_response, node);
		rc = D_MSG_BUFFER_ADD_OK;
	}

	//unlock
	pthread_mutex_unlock(&lib_mutex_for_response);

	return rc;
}

/*!
 * @brief	Userから受信したNotificationメッセージをバッファに追加する。
 * @note	N/A
 * @param	message_id [in]メッセージID
 * @param	data_size [in] メッセージデータ本文のサイズ
 * @param	data [in] メッセージデータ本文のバッファへのアドレス
 * @return	D_MSG_BUFFER_ADD_OK: 成功, D_MSG_BUFFER_ADD_ERR:失敗
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
int f_saa_com_add_notification(uint32_t message_id, uint32_t data_size, void *data){
	uint32_t rc = D_MSG_BUFFER_ADD_ERR;
	MSG_NODE *node = NULL;

	//lock
	pthread_mutex_lock(&lib_mutex_for_notification);
	node = create_node(0, message_id, data_size, data);
	if(node != NULL){
		add_node_to_list(&lib_buffer_for_notification, node);
		rc = D_MSG_BUFFER_ADD_OK;
	}

	//unlock
	pthread_mutex_unlock(&lib_mutex_for_notification);

	return rc;
}


/*!
 * @brief	Userに対して送信するRequestメッセージをバッファから取得する。
 * @note	呼び元で用意した読み込みバッファのサイズが不足する場合エラーになる。
 * @param	max_size [in] メッセージデータ本文の最大読み込み可能サイズ
 * @param	request_id [out]リクエストID
 * @param	message_id [out]メッセージID
 * @param	read_size [out] 実際に読み込んだデータサイズ
 * @param	data [out] メッセージデータ本文を格納するバッファのアドレス
 * @return	D_MSG_BUFFER_GET_OK: 成功, D_MSG_BUFFER_GET_ERR_SIZE:バッファサイズ不足, D_MSG_BUFFER_GET_ERR_OTHER: その他のエラー
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
int f_saa_com_get_request(uint32_t max_size, uint32_t *request_id, uint32_t *message_id, uint32_t *read_size, void *data){
	uint32_t rc = D_MSG_BUFFER_GET_OK;
	MSG_NODE **head_addr = NULL;
	MSG_NODE *node = NULL;

	head_addr = &lib_buffer_for_request;


	while(1){
		//lock
		pthread_mutex_lock(&lib_mutex_for_request);
		node = *head_addr;
		if(node != NULL){
			rc = get_record_data(&(*head_addr)->record, max_size, request_id, message_id, read_size, data);
			remove_node_from_list(head_addr, node);
			//unlock
			pthread_mutex_unlock(&lib_mutex_for_request);
			break;
		}

		//unlock
		pthread_mutex_unlock(&lib_mutex_for_request);
		wait_for_message(DELAY_FOR_REQUEST);
	}

	return rc;
}
	

/*!
 * @brief	Userから受信したResponseメッセージをバッファから取得する。
 * @note	呼び元で用意した読み込みバッファのサイズが不足する場合エラーになる。
 * @param	request_id [out]リクエストID
 * @param	message_id [out]メッセージID
 * @param	read_size [out] 実際に読み込んだデータサイズ
 * @param	data [out] メッセージデータ本文を格納するバッファのアドレス
 * @return	D_MSG_BUFFER_GET_OK: 成功, D_MSG_BUFFER_GET_ERR_SIZE:バッファサイズ不足, D_MSG_BUFFER_ERR_TIMEOUT:タイムアウト, D_MSG_BUFFER_GET_ERR_OTHER: その他のエラー
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
int f_saa_com_get_response(uint32_t request_id, uint32_t max_size, uint32_t *message_id, uint32_t *read_size, void *data, uint32_t wait_time){
	uint32_t rc = D_MSG_BUFFER_GET_ERR_TIMEOUT;
	int found = 0; //0: not found, 1: found
	MSG_NODE **head_addr = NULL;
	MSG_NODE *node = NULL;

	struct timespec wk_time ;
	time_t current_time = 0;
	time_t max_time = 0;

	head_addr = &lib_buffer_for_response;

	(void)clock_gettime(CLOCK_MONOTONIC_RAW, &wk_time);
	current_time = wk_time.tv_sec;
	max_time = current_time + wait_time;

	while(current_time <= max_time){
		//lock
		pthread_mutex_lock(&lib_mutex_for_response);
		node = *head_addr;
		if(node != NULL){
			while(1){
				if(node->record.request_id == request_id){
					found = 1;
					break;
				}
				else if(node->next == NULL){
					break;
				}
				else{
					node = node->next;
				}
			}
			
			if(found == 1){
				rc = get_record_data(&node->record, max_size, &request_id, message_id, read_size, data);
				remove_node_from_list(head_addr, node);
				//unlock
				pthread_mutex_unlock(&lib_mutex_for_response);
				break;
			}
		}

		//unlock
		pthread_mutex_unlock(&lib_mutex_for_response);
		wait_for_message(DELAY_FOR_RESPONSE);
		(void)clock_gettime(CLOCK_MONOTONIC_RAW, &wk_time);
		current_time = wk_time.tv_sec;
	}

	return rc;
}

/*!
 * @brief	Userから受信したNotificationメッセージをバッファから取得する。
 * @note	待ち合わせはしない。バッファが空の場合は呼び元に返る。
 * @note        排他制御は呼び元で行うこと。
 * @param	message_id [out]メッセージID
 * @param	read_size [out] 実際に読み込んだデータサイズ
 * @param	data [out] メッセージデータ本文を格納するバッファのアドレス
 * @return	D_MSG_BUFFER_GET_EMPTY: 空, D_MSG_BUFFER_GET_OK: 成功, D_MSG_BUFFER_GET_ERR_SIZE:バッファサイズ不足, D_MSG_BUFFER_GET_ERR_OTHER: その他のエラー
 * @date	2018/11/16 KCN)Nakai create
 * @date	2019/04/03 KCN)Nakai modify
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
int f_saa_com_get_notification(uint32_t max_size, uint32_t *message_id, uint32_t *read_size, void *data){
	uint32_t rc = D_MSG_BUFFER_GET_EMPTY;
	MSG_NODE **head_addr = NULL;
	MSG_NODE *node = NULL;
	uint32_t request_id = 0;

	head_addr = &lib_buffer_for_notification;

	node = *head_addr;
	if(node != NULL){
		rc = get_record_data(&node->record, max_size, &request_id, message_id, read_size, data);
		remove_node_from_list(head_addr, node);

	}

	return rc;
}

/*!
 * @brief	Notificationバッファをロックする。
 * @note	N/A
 * @param	N/A
 * @return	N/A
 * @date	2019/04/03 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
void f_saa_com_notification_lock(void){
    pthread_mutex_lock(&lib_mutex_for_notification);
}

/*!
 * @brief	Notificationバッファをアンロックする。
 * @note	N/A
 * @param	N/A
 * @return	N/A
 * @date	2019/04/03 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
void f_saa_com_notification_unlock(void){
    pthread_mutex_unlock(&lib_mutex_for_notification);
}

/*!
 * @brief	メッセージを格納するためのノードを作成する。
 * @note	N/A
 * @param	request_id [in]リクエストID
 * @param	message_id [in]メッセージID
 * @param	data_size [in] メッセージデータ本文のサイズ
 * @param	data [in] メッセージデータ本文のバッファへのアドレス
 * @return	0以外: 成功. ノードのアドレス, 0:失敗
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static MSG_NODE * create_node(uint32_t request_id, uint32_t message_id, uint32_t data_size, void *data){
	MSG_NODE *node = NULL;
        time_t wk_time = 0;

	node = (MSG_NODE *)malloc(sizeof(MSG_NODE));
	if(node == NULL){
		return NULL;
	}
	
	node->record.data = (MSG_NODE *)malloc(data_size);
	if(node->record.data == NULL){
		free((void *)node);
		return NULL;
	}

	node->record.added_time = time(&wk_time);
	node->record.request_id = request_id;
	node->record.message_id = message_id;
	node->record.size = data_size;
	memcpy((void *)node->record.data, data, data_size);

	return node;
}

/*!
 * @brief	ノード内のレコードからメッセージを取得する。
 * @note	N/A
 * @param	request_id [out]リクエストID
 * @param	message_id [out]メッセージID
 * @param	read_size [out] 実際に読み込んだデータサイズ
 * @param	data [out] メッセージデータ本文を格納するバッファのアドレス
 * @return	D_MSG_BUFFER_GET_OK: 成功, D_MSG_BUFFER_GET_ERR_SIZE:バッファサイズ不足, D_MSG_BUFFER_GET_ERR_OTHER: その他のエラー
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static int get_record_data(MSG_RECORD *record, uint32_t max_size, uint32_t *request_id, uint32_t *message_id, uint32_t *read_size, void *data){
	uint32_t rc = D_MSG_BUFFER_GET_OK;

	if(record->size <= max_size){
		*request_id = record->request_id;
		*message_id = record->message_id;
		*read_size = record->size;
		memcpy((void *)data, (void *)record->data, record->size);
		rc = D_MSG_BUFFER_GET_OK;
	}
	else{
		*request_id = 0;
		*message_id = 0;
		*read_size = 0;
		rc = D_MSG_BUFFER_GET_ERR_SIZE;
	}

	return rc;
}

/*!
 * @brief	ノードをリストに追加する。
 * @note	N/A
 * @param	list [in] 追加先リストの先頭ノードのアドレス 
 * @param	node[in] 追加するノードのアドレス
 * @return	N/A
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static void add_node_to_list(MSG_NODE **list, MSG_NODE *node){
	MSG_NODE *tmp;

	if(*list == NULL){
		node->previous = node->next = NULL;
		*list = node;
	}
	else{
		tmp = *list;
		while(tmp->next != NULL){
			tmp = tmp->next;
		}

		tmp->next = node;
		node->previous = tmp;
		node->next = NULL;
	}
}

/*!
 * @brief	ノードをリストから削除する。
 * @note	N/A
 * @param	list [in] 削除対象リストの先頭ノードのアドレス 
 * @param	node[in] 削除するノードのアドレス
 * @return	N/A
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static void remove_node_from_list(MSG_NODE **list, MSG_NODE *node){

	if(*list == NULL || node == NULL){
		return;
	}

	if(node->previous != NULL){
		node->previous->next = node->next;
	}
	else{
		*list = node->next;
	}
	
	if(node->next != NULL){
		node->next->previous = node->previous;
	}

	free((void *)node->record.data);
	free((void *)node);
}

/*!
 * @brief	指定した時間待ち合わせを行う。
 * @note	N/A
 * @param	msec [in]待ち時間(msec)
 * @return	0:成功, -1:失敗
 * @date	2018/11/16 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static int wait_for_message(long msec){
	struct timespec req;

	req.tv_sec = 0;
	req.tv_nsec = (time_t)(msec * 1000 * 1000);

	return nanosleep(&req, 0);
}

/* @} */

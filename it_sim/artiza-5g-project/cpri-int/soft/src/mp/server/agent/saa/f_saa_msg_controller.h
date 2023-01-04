/*!
 * @skip  $ld:$
 * @file  f_saa_msg_controller.h
 * @brief message controller for Sub-Agent 
 * @date   2018/11/22 FJT)Taniguchi create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup SAA_MSG_CONTROLLER
 * @{
 */

#ifndef F_SAA_MSG_CONTROLLER_H
#define F_SAA_MSG_CONTROLLER_H

#include <netinet/in.h>
#include <arpa/inet.h>

/*!
 * @brief	T_SAA_MSG_CTRL_THREAD_ARG
 * @note	Thread start informations
 * 
 */
typedef struct {
	in_addr_t src_addr;				/*!< 受信用IPアドレス */
	in_port_t src_port;				/*!< 受信用ポート番号 */
	in_addr_t dst_addr;				/*!< 送信先IPアドレス */
	in_port_t dst_port;				/*!< 送信先ポート番号 */
}T_SAA_MSG_CTRL_THREAD_ARG;


void* f_saa_msg_sender(void* arg);		/*!< message sender thread function		 */
void* f_saa_msg_receiver(void* arg);	/*!< message receiver thread function	 */

/** @} */

#endif	/* F_SAA_MSG_CONTROLLER_H */

/*!
 * @skip  $ld:$
 * @file  f_mpsw_msi.h
 * @brief M-plane Server access function Infterface
 * @date  2018/10/24 Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup MPSW_MSI
 * @{
 */

#ifndef F_MPSW_MSI_H
#define F_MPSW_MSI_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include "f_mpsw_msg_subscribe_typ.h"

/*!
 * @brief	Function result code
 * @note	Function result code is ..
 * 
 */
typedef enum {
	MSI_FUNCIF_OK = 0,			/*!< result OK */
	MSI_FUNCIF_NG = 1			/*!< result NG */
} E_MSI_FUNCIF_RESULT;
/** @} */


/*!
 * @brief	MSI handle
 * @note	MSI handle is ..
 * 
 */
typedef struct{
	INT			socketFd;
	INT			subscriberId;
	INT			receiverId;
	INT			senderId;
	in_port_t	port;
	in_addr_t	addr;
} T_MSI_HANDLE;
/** @} */


/*!
 * @brief	starting APP List
 * @note	starting APP List is ..
 * 
 */
typedef struct{
	const CHAR*		startPrg;			/*!< start program name  */
	CHAR**			startOpt;			/*!< start program option */
	INT				cpus;				/*!< CPU core bit of program */
	INT				policy;				/*!< scheduling policy of program  */
	INT				priority;			/*!< priority of program */
} T_MSI_APP_LIST;
/** @} */

/*!
 * @brief	Subscribe module information
 * @note	Subscribe module information is ..
 * 
 */
typedef struct{
	UINT		modId;				/*!< module ID */
	UINT		modOptBit;			/*!< module option bit */
} T_MSI_SUBSCRIBE_MOD_INFO;
/** @} */



/*!
 * @brief	MSI Function Interfaces
 */
INT msi_app_initialize(
	T_MSI_APP_LIST* NpList,
	T_MSI_APP_LIST* SpList,
	INT	readyIndWaitTime,
	VOID* logFunc,
	pid_t*	NPid,
	pid_t*	SPid
);

INT msi_subscribe(
	in_port_t					portNo,
	CHAR*						serverIpAdder,
	UINT						modNum,
	T_MSI_SUBSCRIBE_MOD_INFO**	modInfo,
	VOID*						logfunc,
	VOID*						sndLogFunc,
	VOID*						rcvlogFunc,
	T_MSI_HANDLE*				msiHandle
);

INT msi_desubscribe(T_MSI_HANDLE* msiHandle);
INT msi_send_to_agent(T_MSI_HANDLE* msiHandle ,INT msiLen, VOID* msiMsg);
INT msi_receive_from_agent(T_MSI_HANDLE* msiHandle, INT msiMaxLen ,INT* msiLen,VOID* msiMsg);
INT msi_receive_timed_from_agent(T_MSI_HANDLE* msiHandle,INT msiMaxLen ,INT  msiWaitTime ,INT*  msiLen,VOID* msiMsg);
/** @} */


#endif
/** @} */

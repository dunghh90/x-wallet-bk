/*!
 * @skip  $ld:$
 * @file  f_mpsw_msg_MSI.h
 * @brief M-plane Server access Infterface
 * @date  2018/10/24 Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */


#include "f_mpsw_types.h"
#include "f_mpsw_msg_enum.h"

/*!
 * @addtogroup MPSW_MSG_MSI
 * @{
 */

#ifndef F_MPSW_MSG_MSI_H
#define F_MPSW_MSG_MSI_H

/**
 * @brief	MSI MSG HEADER
 * @note	MSI MSG HEADER
 * 
 */
typedef struct{
	UINT	msgId;							/**< message Id			*/
	UINT	requestId;						/**< request Id			*/
	UINT	reserve[3];						/**< reserve			*/
	INT		subscriberId;					/**< subscriber Id		*/
	INT		senderId;						/**< sender Id			*/
	INT		msgLength;						/**< message Length		*/
}T_MPSW_SRV_MSGHEAD;


/**
 * @brief	MSI MSG ERROR
 * @note	MSI MSG ERROR
 * 
 */
typedef struct{
	INT		errorCode;						/**< message Id			*/
	CHAR	errorDescription[32];			/**< request Id			*/
}T_MPSW_SRV_MSGERROR;


/**
 * @brief	MSI MSG TYPE
 * @note	MSI MSG TYPE
 * 
 */
typedef enum{
	E_MSI_MSGTYPE_REQ = 1,					/**< Requset */
	E_MSI_MSGTYPE_CFM,						/**< Confirm */
	E_MSI_MSGTYPE_REJ,						/**< Reject */
	E_MSI_MSGTYPE_FWD,						/**< Forward */
	E_MSI_MSGTYPE_IND						/**< Indication */
}E_MPSW_SRV_MSGTYPE;


/**
 * @brief	MSI MSG OPE
 * @note	MSI MSG OPE
 * 
 */
typedef enum{
	E_MSI_MSGOPE_INTNL = 0,					/**< internal */
	E_MSI_MSGOPE_GET,						/**< get */
	E_MSI_MSGOPE_EDIT,						/**< edit-config */
	E_MSI_MSGOPE_INIT,						/**< init */
	E_MSI_MSGOPE_INIT2,						/**< init2 */
	E_MSI_MSGOPE_CLEANUP,					/**< cleanup */
	E_MSI_MSGOPE_INVOKE,					/**< invoke */
	E_MSI_MSGOPE_SEND,						/**< send */
	E_MSI_MSGOPE_VALIDATE,					/**< validate */
	E_MSI_MSGOPE_ALL = 0xFF					/**< all */
}E_MSI_MSGOPE;




#define D_MSG_MSI_OFFSET			0x81000000U


#endif
/** @} */

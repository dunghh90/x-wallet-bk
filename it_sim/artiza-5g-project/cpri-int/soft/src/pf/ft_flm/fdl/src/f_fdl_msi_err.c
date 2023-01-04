/*!
 * @skip  $ld:$
 * @file  f_fdl_msi_err.c
 * @brief MSI要求処理（インストール中のMSI受信)
 * @date  2019/02/23 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_fdl_inc.h"

/*!
 * @brief 関数機能概要:M-Planeに対し、REJ応答を返却する
 * @note  関数処理内容.
 *		-# REJ応答処理を実施。
 *		-# 固定で異常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/02/23 KCN)上本 Create
 */
INT	f_fdl_msi_err(VOID* rcvMsg_p)
{
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ	sndMsg_inv ;
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ		sndMsg_dld ;
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ			sndMsg_ins ;
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ		sndMsg_act ;

	INT			rtn = D_SYS_OK ;

	UINT					r_messageId;
	UINT					w_requestId;
	INT						w_payloadSizePtr;
	VOID *					w_payloadPtr;

	char					err_loading[32] = "file Installing now!" ;


	/* MSIデータの取得 */
	f_com_MSIgetPayload(rcvMsg_p ,				/* [in]  Receive Message pointer			*/
						&r_messageId ,			/* [out] Receive Message ID					*/
						&w_requestId ,			/* [out] Receive Message Request ID			*/
						&w_payloadSizePtr ,		/* [out] Receive Message payload size		*/
						&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/
	switch( r_messageId )
	{
		case	D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ :
			/* パラメータの初期化 */
			memset(&sndMsg_inv , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ)) ;
			/* 異常応答メッセージの返却 */
			sndMsg_inv.msgErr.errorCode = 0 ;
			memcpy(&sndMsg_inv.msgErr.errorDescription[0] , &err_loading , sizeof(err_loading));

			/* MSI送信 */
			(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ ,					/* 送信するメッセージID		*/
								w_requestId ,														/* リクエストID				*/
								sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ) ,		/* 送信するデータサイズ		*/
								&sndMsg_inv ,														/* 送信データのポインタ		*/
								D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
								D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/
			break ;
		case	D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ :
			/* パラメータの初期化 */
			memset(&sndMsg_dld , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ)) ;
			/* 異常応答メッセージの返却 */
			sndMsg_dld.msgErr.errorCode = 0 ;
			memcpy(&sndMsg_dld.msgErr.errorDescription[0] , &err_loading , sizeof(err_loading));

			/* MSI送信 */
			(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ ,					/* 送信するメッセージID		*/
								w_requestId ,														/* リクエストID				*/
								sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ) ,			/* 送信するデータサイズ		*/
								&sndMsg_dld ,														/* 送信データのポインタ		*/
								D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
								D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/
			break ;
		case	D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ :
			/* パラメータの初期化 */
			memset(&sndMsg_ins , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ)) ;
			/* 異常応答メッセージの返却 */
			sndMsg_ins.msgErr.errorCode = 0 ;
			memcpy(&sndMsg_ins.msgErr.errorDescription[0] , &err_loading , sizeof(err_loading));

			/* MSI送信 */
			(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ ,					/* 送信するメッセージID		*/
								w_requestId ,														/* リクエストID				*/
								sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ) ,			/* 送信するデータサイズ		*/
								&sndMsg_ins ,														/* 送信データのポインタ		*/
								D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
								D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/
			break ;
		case	D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ :

			/* パラメータの初期化 */
			memset(&sndMsg_act , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ)) ;
			/* 異常応答メッセージの返却 */
			sndMsg_act.msgErr.errorCode = 0 ;
			memcpy(&sndMsg_act.msgErr.errorDescription[0] , &err_loading , sizeof(err_loading));

			/* MSI送信 */
			(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ ,					/* 送信するメッセージID		*/
								w_requestId ,														/* リクエストID				*/
								sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ) ,			/* 送信するデータサイズ		*/
								&sndMsg_act ,														/* 送信データのポインタ		*/
								D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
								D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/
			break ;
		default	:
			rtn = D_SYS_NG ;
			break ;
	}

	return rtn ;
}
/* @} */



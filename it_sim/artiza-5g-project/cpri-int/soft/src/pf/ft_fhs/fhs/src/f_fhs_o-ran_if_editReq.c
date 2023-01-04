/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_fhs_o-ran_if_editReq.c
 *  @brief  MSI_O_RAN_INTERFACES_EDIT_REQ (o-ran-interfaces EDIT Request) Recieved function
 *  @date   2018/11/26 FJT)Yoshida  create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_FHS
* @{ */

#include "f_fhs_inc.h"
#include "BPF_COM_CMD.h"

/* @{ */
/**
* @brief      MSI_O_RAN_INTERFACES_EDIT_REQ (ietf-interfaces EDIT Request) Recieved function
* @note       o-ran-interfaces EDIT Request factor judge, and table data read.\n
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     result of procedure
* @date       2018/11/26  FJT)Yoshida  new create
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_fhs_o_ran_if_editReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ		*reqBody;						/* 要求データ部  */
	T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REJ		rejBody;						/* 応答データ部  */

	INT										ret = D_RRH_OK;					/* 終了コード  */

	UINT									filtvid_regAddr;				/* レジスタアドレス  */
	UINT									captag_regAddr;					/* レジスタアドレス  */
	UINT									regwData = 0;					/* レジスタ書込み値  */
	UINT									regrData = 0;					/* レジスタ読出し値  */


	reqBody = (T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ *)req;

	/* 設定情報表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit : plane      = %08x", reqBody->plane );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit : port       = %08x", reqBody->port );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit : marking[0] = %04x", reqBody->marking[0] );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit : marking[1] = %04x", reqBody->marking[1] );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-usermgmt edit : vlan_id    = %04x", reqBody->vlan_id );

	while(1) {

		if( reqBody->plane == D_FHS_M_PLANE ) {	/* M-Plane  */

			/*** Req Port番号チェック  ***/
			if( reqBody->port != f_comw_du_connection_tbl->mp.port_no ) {

				/* 不一致 → Reject  */
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Not suppoted Port No, port=%08x",
									reqBody->port);

				rejBody.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR;
				memset( rejBody.msgErr.errorDescription, 0, 32 );
				strcpy( rejBody.msgErr.errorDescription, D_FHS_MSIERR_MSG_MP_PORT );

				ret = D_RRH_NG;
				break;
			}
			/*** vlan-idチェック  ***/
			if( (reqBody->vlan_id != 0xFFFF)
			 && (reqBody->vlan_id != f_comw_du_connection_tbl->mp.mac.vid) ) {

				/* 不一致 → Reject  */
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "vlan_id unmatch, cu_set=%04x, du_set=%04x",
									reqBody->vlan_id, f_comw_du_connection_tbl->mp.mac.vid);

				rejBody.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR;
				memset( rejBody.msgErr.errorDescription, 0, 32 );
				strcpy( rejBody.msgErr.errorDescription, D_FHS_MSIERR_MSG_MP_VID );

				ret = D_RRH_NG;
				break;
			}
			/*** pcp設定  ***/
			if( reqBody->marking[0] != 0xFFFF ) {

				BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_MPLANE_PCP, (UCHAR *)&reqBody->marking[0]);
				break;
			}

		} else if( reqBody->plane == D_FHS_UC_PLANE ) {	/* UC-Plane  */

			/* 送受信用レジスタアドレス算出  */
			if( reqBody->port == D_RRH_FHPORT0 ) {
				filtvid_regAddr = M_DU_REG_ETH_PORT0(D_DU_REG_ETH_FILTVID1);
				captag_regAddr  = M_DU_REG_ETH_PORT0(D_DU_REG_ETH_CAPTAG);
			} else {
				filtvid_regAddr = M_DU_REG_ETH_PORT1(D_DU_REG_ETH_FILTVID1);
				captag_regAddr  = M_DU_REG_ETH_PORT1(D_DU_REG_ETH_CAPTAG);
			}

			/*** vlan-id設定  ***/
			if( reqBody->vlan_id != 0xFFFF ) {

				/* U/FC-Plane受信判定用レジスタ設定  */
				regwData = 0;
				(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, filtvid_regAddr, &regwData);
				regwData = D_DU_ETH_MASK_FILTVID_EN | (reqBody->vlan_id & D_DU_ETH_MASK_FILTVID_VID);
				(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, filtvid_regAddr, &regwData);

				/* U-Plane 送信カプセル用TAG設定  */
				(VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, captag_regAddr, &regrData);
				regwData = (D_DU_ETH_MASK_CAPTAG_PCP & regrData);
				regwData = regwData | (reqBody->vlan_id & D_DU_ETH_MASK_CAPTAG_VID);
				(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, captag_regAddr, &regwData);

				f_comw_du_connection_tbl->ucp[reqBody->port].umac.vid = reqBody->vlan_id;
			}

			/*** pcp設定  ***/
			if( reqBody->marking[0] != 0xFFFF ) {

				/* U-Plane 送信カプセル用TAG設定  */
				(VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, captag_regAddr, &regrData);
				regwData = (regrData & D_DU_ETH_MASK_CAPTAG_VID);
				regwData = regwData | (D_DU_ETH_MASK_CAPTAG_PCP & (reqBody->marking[0] << D_DU_ETH_SHFT_CAPTAG_PCP));
				(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, captag_regAddr, &regwData);

				f_comw_du_connection_tbl->ucp[reqBody->port].umac.pcp = reqBody->marking[0];
			}
			if( reqBody->marking[1] != 0xFFFF ) {

				f_comw_du_connection_tbl->ucp[reqBody->port].cmac.pcp = reqBody->marking[1];
			}

			/* VLAN TAG */
			BPF_COM_CMD_SET_VLANTAG(reqBody->port ,
								(UINT)(f_comw_du_connection_tbl->ucp[reqBody->port].umac.vid),
								(UINT)(f_comw_du_connection_tbl->ucp[reqBody->port].umac.vid),
								(UINT)(f_comw_du_connection_tbl->ucp[reqBody->port].umac.pcp)
			);

		} else {	/* S-Plane  */

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "vlan not supported plane" );

			rejBody.msgErr.errorCode = E_MPSW_ERR_CODE_PARAMETER_ERROR;
			memset( rejBody.msgErr.errorDescription, 0, 32 );
			strcpy( rejBody.msgErr.errorDescription, D_FHS_MSIERR_MSG_MP_VID );

			ret = D_RRH_NG;
		}
		break;
	}

	if( ret == D_RRH_OK ) {

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_INTERFACE_EDIT_CFM,				/* 送信するメッセージID		*/
							 w_requestId,									/* リクエストID				*/
							 D_RRH_ZERO,									/* 送信するデータサイズ		*/
							 (VOID *)D_RRH_NULL,							/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,								/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_FHS ) ;						/* pf_fhsのスレッドID		*/
		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-interfaces edit OK!" );

	} else {

		/* MSI送信処理 */
		(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_INTERFACE_EDIT_REJ,				/* 送信するメッセージID		*/
							 w_requestId,									/* リクエストID				*/
							 sizeof(T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REJ),	/* 送信するデータサイズ		*/
							 (VOID *)&rejBody,								/* 送信データのポインタ		*/
							 D_RRH_PROCQUE_PF ,								/* PFのプロセスID			*/
							 D_SYS_THDQID_PF_FHS ) ;						/* pf_fhsのスレッドID		*/
		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-interfaces edit NG(%08x) (%s)", 
							rejBody.msgErr.errorCode, rejBody.msgErr.errorDescription );
	}
	return	D_RRH_OK;
}
/* @} */



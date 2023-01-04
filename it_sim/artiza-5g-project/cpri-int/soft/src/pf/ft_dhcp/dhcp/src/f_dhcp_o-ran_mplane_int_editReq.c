/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_dhcp_o-ran_mplane_int_editReq.c
 *  @brief  MSI_O_RAN_MPLANE_INT_EDIT_REQ (o-ran-mplane-int EDIT Request) Recieved function
 *  @date   2019/02/18 FJT)Yoshida  create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_DHCP
* @{ */

#include "f_dhcp_inc.h"


/* @{ */
/**
* @brief      MSI_O_RAN_MPLANE_INT_EDIT_REQ (o-ran-mplane-int EDIT Request) Recieved function
* @note       o-ran-mplane-int EDIT Request factor judge, and table data read.\n
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     result of procedure
* @date       2019/02/18  FJT)Yoshida  new create
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_dhcp_o_ran_mplane_int_editReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REQ		*reqBody;						/* 応答データ部		*/

	T_SYS_NMA_VLAN_WRITE_REQ				*sendMsg;

	UCHAR									regw_data;						/* レジスタ読出し値	*/
	INT										portno_chg_flag = D_RRH_OFF;
	INT										flash_write_flag = D_RRH_OFF;

	INT										i;
	INT										endcd;
	INT										errcd;

	struct bpf_timespec						dtime;							/* sem waittime		*/


	dtime.tv_sec  = 0;
	dtime.tv_nsec = BPF_RU_IPCM_WAITTIME_LOG;

	reqBody = (T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REQ *)req;

	/* 設定情報表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-mplane-int edit : call_home_ssh_port = %08x", reqBody->mplane_ssh_port.call_home_ssh_port );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-mplane-int edit : server_ssh_port    = %08x", reqBody->mplane_ssh_port.server_ssh_port );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-mplane-int edit : lowest_vlan_id     = %04x", reqBody->access_vlan_info.lowest_vlan_id );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-mplane-int edit : highest_vlan_id    = %04x", reqBody->access_vlan_info.highest_vlan_id );

	for( i = 0; i < reqBody->access_vlan_info.num_of_vlans; i++ ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-mplane-int edit : searchable_vlan_id[%04d] = %04x",
							i, reqBody->access_vlan_info.searchable_vlan_id[i] );
	}

	/* ==================================================================== */
	/* renewal check of m-plane-ssh-port									*/
	/* ==================================================================== */
	if( (reqBody->mplane_ssh_port.call_home_ssh_port != 0xFFFFFFFF)
	 && (reqBody->mplane_ssh_port.call_home_ssh_port != f_comw_netconf_tbl->callhome_ssh_port) ) {

		/* callhome-ssh-port更新	*/
		f_comw_netconf_tbl->callhome_ssh_port = (USHORT)reqBody->mplane_ssh_port.call_home_ssh_port;
		portno_chg_flag = D_RRH_ON;
	}
	if( (reqBody->mplane_ssh_port.server_ssh_port != 0xFFFFFFFF)
	 && (reqBody->mplane_ssh_port.server_ssh_port != f_comw_netconf_tbl->server_ssh_port) ) {

		/* ssh-port更新	*/
		f_comw_netconf_tbl->server_ssh_port = (USHORT)reqBody->mplane_ssh_port.server_ssh_port;
		portno_chg_flag = D_RRH_ON;
	}

	/* ssh-port更新有無チェック	*/
	if( portno_chg_flag == D_RRH_ON ) {

		/* EEPROM更新	*/
		regw_data = (UCHAR)( (f_comw_netconf_tbl->callhome_ssh_port & 0xFF00) >> 8 );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_CALLHOME_SSH_PORT_H, &regw_data);

		regw_data = (UCHAR)( f_comw_netconf_tbl->callhome_ssh_port & 0x00FF );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_CALLHOME_SSH_PORT_L, &regw_data);

		regw_data = (UCHAR)( (f_comw_netconf_tbl->server_ssh_port & 0xFF00) >> 8 );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SERVER_SSH_PORT_H, &regw_data);

		regw_data = (UCHAR)( f_comw_netconf_tbl->server_ssh_port & 0x00FF );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SERVER_SSH_PORT_L, &regw_data);

		regw_data = D_RRH_ON;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SSH_USEABLE_FLAG, &regw_data);
	}

	/* ==================================================================== */
	/* renewal check of dhcpv4												*/
	/* ==================================================================== */
	if( (reqBody->access_vlan_info.lowest_vlan_id != 0xFFFF)
	 && (reqBody->access_vlan_info.lowest_vlan_id != f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid) ) {

		/* lowest-vlan-id更新	*/
		f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid = reqBody->access_vlan_info.lowest_vlan_id;

		/* EEPROM更新	*/
		regw_data = (UCHAR)( (f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid & 0xFF00) >> 8 );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_LOW_PORT1_H, &regw_data);

		regw_data = (UCHAR)( f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid & 0x00FF );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_LOW_PORT1_L, &regw_data);
	}
	if( (reqBody->access_vlan_info.highest_vlan_id != 0xFFFF)
	 && (reqBody->access_vlan_info.highest_vlan_id != f_comw_du_netinfo_eep_tbl->vid_search_info.highVid) ) {

		/* highest-vlan-id更新	*/
		f_comw_du_netinfo_eep_tbl->vid_search_info.highVid = reqBody->access_vlan_info.highest_vlan_id;

		/* EEPROM更新	*/
		regw_data = (UCHAR)( (f_comw_du_netinfo_eep_tbl->vid_search_info.highVid & 0xFF00) >> 8 );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_HIGH_PORT1_H, &regw_data);

		regw_data = (UCHAR)( f_comw_du_netinfo_eep_tbl->vid_search_info.highVid & 0x00FF );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_HIGH_PORT1_L, &regw_data);
	}
	/* highest-vlan-id or lowest-vlan-idの更新あり	*/
	if( (reqBody->access_vlan_info.lowest_vlan_id  != 0xFFFF)
	 || (reqBody->access_vlan_info.highest_vlan_id != 0xFFFF) ) {

		regw_data = D_RRH_ON;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_SCAN_MODE, &regw_data);
	}
	if( reqBody->access_vlan_info.num_of_vlans != 0 ) {

		/* Take semphore of VLAN information	*/
		endcd = BPF_RU_IPCM_PROCSEM_TAKE( E_RRH_SEMID_APL_VLAN_INFO, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd );
		if(endcd != BPF_RU_IPCM_OK) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "searchable-access-vlan info sem take error rtn_bpf=0x%08x errcd=0x%08x", endcd, errcd );
		}

		for( i = 0; i < reqBody->access_vlan_info.num_of_vlans; i++ ) {

			f_com_search_vid_set( f_comw_searchable_vlan_info->seachable_vlan_id, 
								 reqBody->access_vlan_info.searchable_vlan_id[i], 
								 D_RRH_OFF );
			flash_write_flag = D_RRH_ON;
		}

		/* Give semphore of VLAN information	*/
		endcd =  BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_VLAN_INFO, BPF_RU_IPCM_LOCK_RW, &errcd);
		if(endcd != BPF_RU_IPCM_OK) {
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "searchable-access-vlan info sem give error rtn_bpf=0x%08x errcd=0x%08x", endcd, errcd);
		}
	}

	/* ==================================================================== */
	/* send to MSI															*/
	/* ==================================================================== */
	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_CFM,					/* 送信するメッセージID		*/
						 w_requestId,											/* リクエストID				*/
						 D_RRH_ZERO,											/* 送信するデータサイズ		*/
						 (VOID *)D_RRH_NULL,									/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,										/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_DHC ) ;								/* pf_dhcpのスレッドID		*/
	/* 処理結果表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-mplane-int edit OK!" );

	if( flash_write_flag == D_RRH_ON ) {

		/* ================================================================ */
		/* send to interface for pf_nma										*/
		/* ================================================================ */
		/* Thread間IF送信	*/
		/* 通信用共有メモリを取得する	*/
		endcd = BPF_RU_IPCM_PROCMSG_ADDRGET( E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別			*/
											 sizeof(T_SYS_NMA_VLAN_WRITE_REQ),		/* Size					*/
											 (VOID **)&sendMsg,						/* msgP					*/
											 &errcd );
		/* 処理結果チェック	*/
		if( endcd != D_SYS_OK ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed get process message address %d", errcd);
			return D_SYS_NG;
		}
		/* 初期化完了通知msg編集	*/
		sendMsg->thdIf.uiEventNo	 = D_SYS_MSGID_NMA_VLAN_WRITE_REQ;			/* 初期化完了通知			*/
		sendMsg->thdIf.uiDstPQueueID = D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID	*/
		sendMsg->thdIf.uiDstTQueueID = D_SYS_THDQID_PF_NMA;						/* 送信先Thread Queue ID	*/
		sendMsg->thdIf.uiSrcPQueueID = D_RRH_PROCQUE_PF;						/* 送信元Process Queue ID	*/
		sendMsg->thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_DHC;						/* 送信元Thread Queue ID	*/
		sendMsg->thdIf.uiLength		 = sizeof(T_SYS_NMA_VLAN_WRITE_REQ);		/* Length(Header部 + Data部)*/
		sendMsg->requestID			 = w_requestId;								/* Request ID				*/
																				/* searchable-access-vlans	*/
		memcpy( sendMsg->searchable_vlans, f_comw_searchable_vlan_info->seachable_vlan_id, D_DU_SEARCHABLE_VLAN_MAX );

		/* pf_nmaにT_SYS_NMA_NETCONF_WRITE_REQを送信	*/
		endcd = BPF_RU_IPCM_MSGQ_SEND( D_SYS_THDQID_PF_NMA, (void *)sendMsg );
		if( endcd != D_SYS_OK ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", endcd);
			return D_SYS_NG;
		}
	}
	return	D_RRH_OK;
}
/* @} */

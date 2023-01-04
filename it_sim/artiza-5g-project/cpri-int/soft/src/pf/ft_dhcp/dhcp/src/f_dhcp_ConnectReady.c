/*!
 * @skip 		$ld:$
 * @file 		f_dhcp_ConnectReady.c
 * @brief		pf_dhcpスレッド M-Plane接続準備要求処理
 * @date		2018/02/20 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "m_cm_header.h"
#include "f_dhcp_inc.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h> 
#include <arpa/inet.h>
#include <net/if.h>
#include <net/route.h>
#include <net/if_arp.h>

/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_discoverCheck
 * @note		DHCP discoverを開始してよいかチェックする
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2020/2/21 FJT)Taniguchi create
 */
/****************************************************************************************************************************/
UINT f_dhcp_discoverCheck(UINT firstFlg){
	UINT	clockClassState;
	UINT	clockClassCnt;
	UINT	clockClassMaxCnt = 5;	/* 5sec */
	UINT	checkMax = 10;			/* 10sec */
	UINT	checkCnt;
	UINT	linkState;

	if(firstFlg == D_RRH_OFF){
		/* 初回以外は、skip */
		return D_RRH_OK;
	}

	/* 最大10秒ぐらい待ちうけし、10秒まってもダメなら、強制的にDHCP Goする						*/
	clockClassCnt = 0;
	for(checkCnt = 0; checkCnt < checkMax ; checkCnt++){
		(void)BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, D_DU_SVCTL_WAVE_STOP_ALL_PTP, &clockClassState);
		if(clockClassState == D_RRH_OFF){
			clockClassCnt++;
		}else{
			clockClassCnt = 0;
		}
		/* firstFlgがONの場合																	*/
		/*   5回連続でaccept可能なclock classを受けていたらDHCP discoverをGoする				*/
		/*   5回連続の理由は即OKにすると、PTP側でRTC時刻が全く設定されていない可能性があり、	*/
		/*   最低でも1度はRTC設定(settime時 or steptime時)が走ることを期待するため				*/
		if(clockClassCnt >= clockClassMaxCnt){
			(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, &clockClassState);
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Received acceptable ClockClass latest ptp state:0x%08x",clockClassState);
			break;
		}
		f_com_taskDelay(1000,0,0,0);
		/* 念のためLink状態をチェック */
		BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV6INF,D_DU_SVCTL_PORT1_DISCONNECT ,&linkState);
		if(linkState != D_RRH_OFF){
			(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, &clockClassState);
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Detected FH Link down(0x%08x) latest ptp state:0x%08x count:0x%08x",linkState,clockClassState,clockClassCnt);
			return D_RRH_NG;
		}
	}
	if(checkCnt >= checkMax){
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, &clockClassState);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "NOT received acceptable ClockClass latest sv16:0x%08x count:0x%08x",clockClassState,clockClassCnt);
	}
	return D_RRH_OK;
}

/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_ConnectReadyWrap
 * @note		DHCP Client処理を実行する
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT	f_dhcp_ConnectReadyWrap(VOID	*msgP)
{
	T_API_MPLANE_READY_REQ			*apiReq_p;							/**< M-Plane接続準備要求*/
	T_API_MPLANE_READY_RSP			*apiRsp_p;							/**< M-Plane接続準備応答*/

	UINT							reg_data;							/* レジスタ読出し用		*/
	UINT							reg_data2;							/* レジスタ読出し用		*/
	UINT							state_flag;
	UINT							msg_flag = D_RRH_OFF;

	INT								rtn = D_SYS_OK;
	INT								ret;								/* サブ関数処理結果		*/
	INT								errcd;								/* エラーコード			*/

	CHAR							cmd_str[D_SYS_NUM128];				/* コマンド文字列		*/
	CHAR							file_path[D_SYS_NUM64];				/* ファイルパス文字列	*/
	CHAR							file_name[D_SYS_NUM64];				/* ファイル名文字列		*/


	/********************************************************************************************/
	/* leasesファイル削除																		*/
	/********************************************************************************************/
	/* leasesファイル名初期設定	*/
	memset ( file_name, 0, sizeof(file_name) );
	sprintf( file_name, D_DHCP_FNAME_LEASES );

	/* leasesファイルパス初期設定	*/
	memset ( file_path, 0, sizeof(file_path) );
	sprintf( file_path, D_DHCP_PATH_DHCP );

	/* "/var/lib/dhclient/dhclient.leases"ファイル削除	*/
	memset ( cmd_str, 0, sizeof(cmd_str) );
	sprintf( cmd_str, D_DHCP_CMD_DEL_LEASES, file_path, file_name);

	/********************************************************************************************/
	/* DHCP Client処理開始																		*/
	/********************************************************************************************/
	/* M-Plane接続準備要求ポインタ取得	*/
	apiReq_p = (T_API_MPLANE_READY_REQ *)msgP;

	/* 起動元チェック	*/
	if( (apiReq_p->header.uiSrcPQueueID & 0xFFFFFF00 ) == D_RRH_PROCID_Q_TP ) {

		return	D_SYS_NG;	/* TPからのDHCP起動時は、NG応答	*/
	}

	/* VLANありの場合、パラメータ	*/
	if( (apiReq_p->data.Flag & D_API_FLAG_MPN_MPLANE_READY_V) == D_API_FLAG_MPN_MPLANE_READY_V ) {

		if( (apiReq_p->data.lowVid < D_RRH_VID_MIN) || (apiReq_p->data.lowVid > D_RRH_VID_MAX) ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR,  "API parameter Range Error, lowVid=%d", 
								(UINT)apiReq_p->data.lowVid );
			apiReq_p->data.lowVid  = D_RRH_VID_MIN;
			apiReq_p->data.highVid = D_RRH_VID_MAX;
		}
		if( (apiReq_p->data.highVid < D_RRH_VID_MIN) || (apiReq_p->data.highVid > D_RRH_VID_MAX) ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR,  "API parameter Range Error, highVid=%d", 
								(UINT)apiReq_p->data.highVid );
			apiReq_p->data.lowVid  = D_RRH_VID_MIN;
			apiReq_p->data.highVid = D_RRH_VID_MAX;
		}
		if( apiReq_p->data.highVid < apiReq_p->data.lowVid ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR,  "API parameter Relation Error, lowVid=%d / highVid=%d",
								(UINT)apiReq_p->data.lowVid, (UINT)apiReq_p->data.highVid );
			apiReq_p->data.lowVid  = D_RRH_VID_MIN;
			apiReq_p->data.highVid = D_RRH_VID_MAX;
		}
		if( (apiReq_p->data.lastUsedVid < apiReq_p->data.lowVid) || (apiReq_p->data.highVid < apiReq_p->data.lastUsedVid) ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR,  "API parameter lastUsedVID Error, lowVid=%d / highVid=%d / lastUsedVid=%d",
								(UINT)apiReq_p->data.lowVid, (UINT)apiReq_p->data.highVid, (UINT)apiReq_p->data.lastUsedVid );
			apiReq_p->data.lastUsedVid = apiReq_p->data.lowVid;
		}
	}

	if( apiReq_p->data.Interface != 0xFFFF ) {

		ret = f_dhcp_ConnectReady(msgP);

	} else {
		UINT	firstFlg = D_RRH_ON;
		UINT	dhcpCnt = 0;
		UINT	dhcpCheck = D_RRH_NG;

		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV6INF, &reg_data);
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, &reg_data2);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING,  "Starting DHCP discover sv6info = %08x sv16info = %08x", reg_data,reg_data2);

		while(1) {

			ret = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV6INF, &reg_data);
			if(ret != BPF_HM_DEVC_COMPLETE) {

				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_REG_READ error=0x%08x", ret );
				return	D_SYS_NG;
			}

			state_flag = D_RRH_OFF;
			if( (reg_data & D_DU_SVCTL_PORT1_DISCONNECT) == D_RRH_OFF) {
				dhcpCnt++;

				if(msg_flag == D_RRH_ON) {
					(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, &reg_data2);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING,  "ReStarting DHCP discover(%d) sv6info = %08x sv16info = %08x", dhcpCnt,reg_data,reg_data2);
					msg_flag = D_RRH_OFF;
				}

				/* discover開始してよいかチェックする */
				if(( dhcpCheck = f_dhcp_discoverCheck(firstFlg)) == D_RRH_NG ){
					continue;
				}

				/* fheth0を実行する	*/
				apiReq_p->data.Interface = D_API_INTERFACE_MPN_MPLANE_READY_FHPORT0;

				/* OS時刻更新を抑止する */
				BPF_HM_DEVC_SETTIME_OS_SUPPRESS();

				ret = f_dhcp_ConnectReady(msgP);

				/* OS時刻更新を抑止解除する */
				BPF_HM_DEVC_SETTIME_OS_CORRECTION();

				state_flag = D_RRH_ON;
				firstFlg = D_RRH_OFF;

				/* DHCP Procedure完了チェック	*/
				if( ret == D_SYS_ON ) {
					break;	/* DHCP Procedure完了	*/
				} else {
					ret = system ( cmd_str );
				}
			}
			if( state_flag != D_RRH_ON ) {
				if(msg_flag == D_RRH_OFF) {
					(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, &reg_data2);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING,  "Stopping DHCP discover(%d) sv6info = %08x sv16info = %08x", dhcpCnt,reg_data,reg_data2);
					msg_flag = D_RRH_ON;
				}
				/* wait 1000ms */
				f_com_taskDelay(1000,0,0,0);
				firstFlg = D_RRH_ON;	/* リンク断したら、再度first */
			}
		}
	}

	/********************************************************************************************/
	/* M-Plane接続開始応答 送信																	*/
	/********************************************************************************************/
	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer種別			*/
										sizeof(T_API_MPLANE_READY_RSP),	/* Size					*/
										(VOID **)&apiRsp_p,				/* msgP					*/
										&errcd );
	/* 処理結果チェック	*/
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed get process message address %d", errcd);
		return D_SYS_NG;
	}
	/* M-Plane接続開始応答msg編集	*/
	apiRsp_p->header.uiEventNo	   = D_API_MSGID_MPN_MPLANE_READY_RSP;	/* M-Plane接続準備応答	*/
	apiRsp_p->header.uiDstPQueueID = apiReq_p->header.uiSrcPQueueID;/* 送信先Process Queue ID	*/
	apiRsp_p->header.uiDstTQueueID = apiReq_p->header.uiSrcTQueueID;/* 送信先Thread Queue ID	*/
	apiRsp_p->header.uiSrcPQueueID = D_RRH_PROCQUE_PF;				/* 送信元Process Queue ID	*/
	apiRsp_p->header.uiSrcTQueueID = D_SYS_THDQID_PF_DHC;			/* 送信元Thread Queue ID	*/
	apiRsp_p->header.uiLength	   = sizeof(T_API_MPLANE_READY_RSP);/* Length(Header部 + Data部)*/

	/* VID scan結果チェック	*/
	if( f_comw_du_connection_tbl->mp.mac.vid == f_comw_du_netinfo_eep_tbl->vid_search_info.lastUsedVid ) {

		/* Method = "LastUsedVid"を設定	*/
		apiRsp_p->data.vidAcquiringMethod = D_DHCP_METHOD_LAST_USED_VID;
	} else {

		/* Method = "VidRangeScan"を設定	*/
		apiRsp_p->data.vidAcquiringMethod = D_DHCP_METHOD_VID_RANGE_SCAN;
	}

	/* CU Network情報設定	*/
	memcpy( (VOID*)apiRsp_p->data.ifname, (VOID*)f_com_ifname, D_RRH_IF_NAME_SMAX );	/* InterfaceName  */
	memset( (VOID*)apiRsp_p->data.remoteMacAddress, 0, D_RRH_MAC_ADDR_SIZE );	/* Remote MAC	*/

	apiRsp_p->data.dhcpFlag		  = f_comw_du_connection_tbl->mp.dhcpFlag ;		/* DHCP状態		*/
	apiRsp_p->data.vid			  = f_comw_du_connection_tbl->mp.mac.vid ;		/* VID			*/
	apiRsp_p->data.pcp			  = f_comw_du_connection_tbl->mp.mac.pcp ;		/* PCP			*/
	apiRsp_p->data.localIpAddress = f_comw_du_connection_tbl->mp.ip.ip_addr_du;	/* IP Addr (DU)	*/
	apiRsp_p->data.remoteIpAdress = f_comw_du_connection_tbl->mp.ip.ip_addr_cu;	/* IP Addr (CU)	*/
	apiRsp_p->data.defaultGateway = f_comw_du_connection_tbl->mp.ip.default_gateway;/* DefaultGw*/
	apiRsp_p->data.subnetMask	  = f_comw_du_connection_tbl->mp.ip.subnet_mask;	/* SubnetMsk*/

	/* pf_mpagへM-Plane接続開始応答を送信	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( apiRsp_p->header.uiDstPQueueID,	/* 送信先Thread Queue ID	*/
								 (void *)apiRsp_p );				/* 送信msg					*/
	/* 処理結果チェック	*/
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", ret);
		return D_SYS_NG;
	}

	f_com_dumpMsgEventSend("VLAN Scanning was completed(M-plane VLAN-ID:%d PCP:%d llsCU-IP:%d.%d.%d.%d RU-IP:%d.%d.%d.%d)",
			f_comw_du_connection_tbl->mp.mac.vid,
			f_comw_du_connection_tbl->mp.mac.pcp,
			( f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0xFF000000) >> 24,
			( f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x00FF0000) >> 16,
			( f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x0000FF00) >> 8,
			( f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x000000FF),
			( f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0xFF000000) >> 24,
			( f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x00FF0000) >> 16,
			( f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x0000FF00) >> 8,
			( f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x000000FF)
	);

	return	rtn;
}


/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_ConnectReady
 * @note		DHCP Client処理を実行する
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT	f_dhcp_ConnectReady(VOID	*msgP)
{
	T_API_MPLANE_READY_REQ			*apiReq_p;							/**< M-Plane接続準備要求*/

	INT								ret;								/* サブ関数処理結果		*/
	USHORT							*vid_adr;							/* 検索VIDアドレス		*/

	USHORT							lowVid;								/* Lowest VLAN-ID		*/
	USHORT							highVid;							/* Highest VLAN-ID		*/
	USHORT							lastUsedVid;						/* Last Used VLAN-ID	*/

	UINT							port_no;							/* PORT番号				*/
	UCHAR							wdata;								/* EEPROM書込み用		*/


	/********************************************************************************************/
	/* M-Plane接続準備要求パラメータチェック													*/
	/********************************************************************************************/
	/* M-Plane接続準備要求ポインタ取得	*/
	apiReq_p = (T_API_MPLANE_READY_REQ *)msgP;

	/* パラメータ初期化	*/
	lowVid      = apiReq_p->data.lowVid;
	highVid     = apiReq_p->data.highVid;
	lastUsedVid = apiReq_p->data.lastUsedVid;

	/********************************************************************************************/
	/* 内部変数初期化																			*/
	/********************************************************************************************/
	/* 共有メモリの検索VID現在値を設定	*/
	vid_adr = &f_comw_du_netinfo_eep_tbl->vid_search_info.nowVid;

	/* 検索開始VID値設定	*/
	*vid_adr = lastUsedVid;

	/********************************************************************************************/
	/* Interface判定																			*/
	/********************************************************************************************/
	if( apiReq_p->data.Interface == D_API_INTERFACE_MPN_MPLANE_READY_FHPORT0 ) {

		/* Interface設定	*/
		memset ( f_com_ifname, 0, sizeof(f_com_ifname) );
		sprintf( f_com_ifname, "%s", D_RRH_IF_NAME_0 );

		port_no = D_API_INTERFACE_MPN_MPLANE_READY_FHPORT0;

	} else {

		/* Interface設定	*/
		memset ( f_com_ifname, 0, sizeof(f_com_ifname) );
		sprintf( f_com_ifname, "%s", D_RRH_IF_NAME_DBG );

		port_no = D_API_INTERFACE_MPN_MPLANE_READY_DPORT;
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "[Interface : %s]", f_com_ifname) ;

	/********************************************************************************************/
	/* DHCP処理開始																				*/
	/********************************************************************************************/
	if( (apiReq_p->data.Flag & D_API_FLAG_MPN_MPLANE_READY_VD) == D_API_FLAG_MPN_MPLANE_READY_VD ) {

		ret = f_dhcp_get_ipaddr_vd( highVid, lowVid, vid_adr, port_no );

	} else if( (apiReq_p->data.Flag & D_API_FLAG_MPN_MPLANE_READY_VD) == D_API_FLAG_MPN_MPLANE_READY_D ) {

		ret = f_dhcp_get_ipaddr_d( highVid, lowVid, vid_adr, port_no );

	} else if( (apiReq_p->data.Flag & D_API_FLAG_MPN_MPLANE_READY_VD) == D_API_FLAG_MPN_MPLANE_READY_V ) {

		ret = f_dhcp_get_ipaddr_v( highVid, lowVid, vid_adr, port_no );
	} else {

		ret = f_dhcp_get_ipaddr( highVid, lowVid, vid_adr, port_no );
	}
	
	if( ret != D_SYS_ON ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO,   "DHCP Failed!, ret=%08x", ret );

	} else {

		lastUsedVid = *vid_adr; 
		f_comw_du_connection_tbl->mp.port_no      = port_no;
		f_comw_du_connection_tbl->mp.mac.vid      = *vid_adr;
		f_comw_du_connection_tbl->mp.mac.pcp      = f_comw_du_netinfo_eep_tbl->pcp;

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "** Result of IP Address Acquisition : %08x **", ret) ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "[interface]") ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => interface name         : %x (port#%d)", 
							f_com_ifname, 
							f_comw_du_connection_tbl->mp.port_no) ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "[MAC Layer]") ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => Local  MAC Address     : %02x:%02x:%02x:%02x:%02x:%02x",
							f_comw_du_netinfo_eep_tbl->localMacAddress[port_no][0],
							f_comw_du_netinfo_eep_tbl->localMacAddress[port_no][1],
							f_comw_du_netinfo_eep_tbl->localMacAddress[port_no][2],
							f_comw_du_netinfo_eep_tbl->localMacAddress[port_no][3],
							f_comw_du_netinfo_eep_tbl->localMacAddress[port_no][4],
							f_comw_du_netinfo_eep_tbl->localMacAddress[port_no][5] );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "[VLAN     ]") ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => VLAN ID                : %04x", f_comw_du_connection_tbl->mp.mac.vid);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => VLAN usr Priority      : %04x", f_comw_du_connection_tbl->mp.mac.pcp);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "[IP  Layer]") ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => Local  IP Address      : %d.%d.%d.%d",
							((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0xFF000000) >> 24), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x00FF0000) >> 16), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x0000FF00) >>  8), 
							 (f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x000000FF) );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => NETCONF Client Address : %d.%d.%d.%d",
							((f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0xFF000000) >> 24), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x00FF0000) >> 16), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x0000FF00) >>  8), 
							 (f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x000000FF) );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => DHCP Server Address    : %d.%d.%d.%d",
							((f_comw_du_connection_tbl->mp.ip.ip_addr_dhcps & 0xFF000000) >> 24), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_dhcps & 0x00FF0000) >> 16), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_dhcps & 0x0000FF00) >>  8), 
							 (f_comw_du_connection_tbl->mp.ip.ip_addr_dhcps & 0x000000FF) );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => Subnet mask            : %d.%d.%d.%d",
							((f_comw_du_connection_tbl->mp.ip.subnet_mask & 0xFF000000) >> 24), 
							((f_comw_du_connection_tbl->mp.ip.subnet_mask & 0x00FF0000) >> 16), 
							((f_comw_du_connection_tbl->mp.ip.subnet_mask & 0x0000FF00) >>  8), 
							 (f_comw_du_connection_tbl->mp.ip.subnet_mask & 0x000000FF) );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => Default Gateway        : %d.%d.%d.%d",
							((f_comw_du_connection_tbl->mp.ip.default_gateway & 0xFF000000) >> 24), 
							((f_comw_du_connection_tbl->mp.ip.default_gateway & 0x00FF0000) >> 16), 
							((f_comw_du_connection_tbl->mp.ip.default_gateway & 0x0000FF00) >>  8), 
							 (f_comw_du_connection_tbl->mp.ip.default_gateway & 0x000000FF) );

		/* VLAN-IDをEEPROMに保存	*/
		wdata = (UCHAR)((f_comw_du_connection_tbl->mp.mac.vid & 0xFF00) >> 8);
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_LAST_PORT1_H, &wdata);

		wdata = (UCHAR)(f_comw_du_connection_tbl->mp.mac.vid & 0x00FF);
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_LAST_PORT1_L, &wdata);


		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING,  "DHCP Complete(0%x)! FH-Eth%d DHCP UP CU-IP:%d.%d.%d.%d RU-IP:%d.%d.%d.%d VID:%d(0x%x) PCP:%d",
							ret,
							port_no+1,
							((f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0xFF000000) >> 24), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x00FF0000) >> 16), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x0000FF00) >>  8), 
							 (f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x000000FF) ,
							((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0xFF000000) >> 24), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x00FF0000) >> 16), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x0000FF00) >>  8), 
							 (f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x000000FF),
							lastUsedVid,lastUsedVid,
							f_comw_du_netinfo_eep_tbl->pcp);

		BPF_COM_LOG_DMESG("FH-Eth%d DHCP UP CU-IP:%d.%d.%d.%d RU-IP:%d.%d.%d.%d VID:%d(0x%x) PCP:%d \n",
							port_no+1,
							((f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0xFF000000) >> 24), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x00FF0000) >> 16), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x0000FF00) >>  8), 
							 (f_comw_du_connection_tbl->mp.ip.ip_addr_cu & 0x000000FF) ,
							((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0xFF000000) >> 24), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x00FF0000) >> 16), 
							((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x0000FF00) >>  8), 
							 (f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0x000000FF),
							lastUsedVid,lastUsedVid,
							f_comw_du_netinfo_eep_tbl->pcp);

	}
	return	f_comw_du_connection_tbl->mp.dhcpFlag;
}


/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_get_ipaddr_vd
 * @note		IPアドレス取得処理を実行する。(DHCPあり/VLAN scanあり)
 * @param		highVid			[in]USHORT	High VID
  * @param		lowVid			[in]USHORT	Low VID
 * @param		*vid_adr		[in]USHORT*	検索開始VID
 * @retval		D_SYS_ON		1:正常終了
 * @retval		D_SYS_OFF		0:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/04/xx FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT		f_dhcp_get_ipaddr_vd( USHORT	highVid, USHORT	lowVid, USHORT	*vid_adr, UINT	port_no )
{
	USHORT							lastUsedVid;						/* 前回値保存			*/

	CHAR							cmd_str[D_SYS_NUM128];				/* コマンド文字列		*/
	CHAR							ifname[D_SYS_NUM32];				/* interface			*/
	CHAR							file_path[D_SYS_NUM64];				/* ファイルパス文字列	*/
	CHAR							file_name[D_SYS_NUM64];				/* ファイル名文字列		*/
	LONG							file_size;

	INT								i;									/* ループ変数			*/
	INT								ret;								/* サブ関数処理結果		*/
	INT								errcd;								/* エラーコード			*/

	INT								valid_cnt;

	UINT							regAddr;							/* レジスタアドレス		*/
	UINT							regData = 0;						/* レジスタ書込み値		*/


	/********************************************************************************************/
	/* ローカル変数初期化																		*/
	/********************************************************************************************/
	/* 有効vlan-id数算出	*/
	valid_cnt = f_com_search_vid_cnt( f_comw_searchable_vlan_info->seachable_vlan_id, lowVid, highVid );
	if( valid_cnt != 0 ) {

		for( i = 0; i < ((highVid - lowVid) + 1); i++ ) {
			
			ret = f_com_search_vid_chk( f_comw_searchable_vlan_info->seachable_vlan_id, *vid_adr );
			if( ret == D_RRH_OK ) {
				break;
			} else {
				(*vid_adr)++;
				if( *vid_adr > highVid ) {
					*vid_adr = lowVid ;
				}
			}
		}
	}
	lastUsedVid = *vid_adr;

	/* FILTVID15 レジスタアドレス  */
	regAddr = D_DU_REG_ETH_FILTVID15;
	/* FILTVID15 レジスタ設定値  */
	regData = 0;
	(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_OFF, regAddr, &regData);

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "IP Address Get Method (DHCP[ON] / VLAN[ON])") ;
	/********************************************************************************************/
	/* leasesファイル削除																		*/
	/********************************************************************************************/
	/* leasesファイル名初期設定	*/
	memset ( file_name, 0, sizeof(file_name) );
	sprintf( file_name, D_DHCP_FNAME_LEASES );

	/* leasesファイルパス初期設定	*/
	memset ( file_path, 0, sizeof(file_path) );
	sprintf( file_path, D_DHCP_PATH_DHCP );

	/********************************************************************************************/
	/* VLAN初期設定																				*/
	/********************************************************************************************/
	memset ( cmd_str, 0, sizeof(cmd_str) );
	sprintf( cmd_str, D_DHCP_CMD_VLAN_NAME_TYPE );
	ret = system ( cmd_str );

	/********************************************************************************************/
	/* DHCP Client処理開始																		*/
	/********************************************************************************************/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "DHCP Client with VLAN scan Start") ;

	while(1) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "  => DHCP Client %s.%d start", f_com_ifname, *vid_adr ) ;

		/* FILTVID15 レジスタ設定値  */
		regData = (UINT)(D_DHCP_VAL_FILTVID15_EN | (*vid_adr & D_DHCP_VAL_FILTVID15_VID_MSK));
		(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_OFF, regAddr, &regData);

		/* interface設定	*/
		memset ( ifname, 0, sizeof(ifname) );
		sprintf( ifname, "%s.%d", f_com_ifname, *vid_adr );

		/* ethにvidを関連付け	*/
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_VLAN_ADD, f_com_ifname, *vid_adr );
		ret = system ( cmd_str );

#if 0
		/* ethにpcpを設定	*/
		for(i = 0; i < 8; i++) {
			memset ( cmd_str, 0, sizeof(cmd_str) );
			sprintf( cmd_str, D_DHCP_CMD_VLAN_INGRESS, ifname, i, f_comw_du_netinfo_eep_tbl->pcp );
			ret = system ( cmd_str );
		}
#endif

		/* ethにpcpを設定	*/
		for(i = 0; i < 8; i++) {
			memset ( cmd_str, 0, sizeof(cmd_str) );
			sprintf( cmd_str, D_DHCP_CMD_VLAN_EGRESS, ifname, i, f_comw_du_netinfo_eep_tbl->pcp );
			ret = system ( cmd_str );
		}

		/* eth有効化	*/
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_IFCONFIG_UP, ifname );
		ret = system ( cmd_str );

		/* dhcp client処理実行	*/
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_DHCLIENT, ifname );
		ret = system ( cmd_str );

		/* reasesファイル生成チェック ⇒ 生成されている場合は、DHCP正常終了	*/
		ret = BPF_RU_FACC_FSIZE( (const bpf_ru_facc_dpathname *)&file_path, 
								 (const bpf_ru_facc_fname *)&file_name, 
								 &file_size, &errcd );

		if( (ret == D_SYS_OK) && (file_size != D_SYS_NUM0) ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "create of leases file -> file size = %d byte",
								file_size ) ;

			ret = f_dhcp_fread_lease(  ) ;

			/* パラメータ取得結果チェック	*/
			if( ret == D_SYS_OK ) {

				/* テーブル設定	*/
				f_comw_du_connection_tbl->mp.dhcpFlag = D_SYS_ON;
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "DHCP Client with VLAN scan Complete") ;

				ret = D_SYS_OK ;
				break;
			}

			/* "/var/lib/dhclient/dhclient.leases"ファイル削除	*/
			memset ( cmd_str, 0, sizeof(cmd_str) );
			sprintf( cmd_str, D_DHCP_CMD_DEL_LEASES, file_path, file_name);
			ret = system ( cmd_str );
		}
		/* dhcp client処理実行	*/
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_DHCLIENTR, ifname );
		ret = system ( cmd_str );

		/* eth無効化	*/
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_IFCONFIG_DOWN, ifname );
		ret = system ( cmd_str );

		/* ethにvidを関連付け削除	*/
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_VLAN_DEL, f_com_ifname, *vid_adr );
		ret = system ( cmd_str );

		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_KILLALL );
		ret = system ( cmd_str );

		/* FILTVID15 レジスタ設定値  */
		regData = 0;
		(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_OFF, regAddr, &regData);

		/* search-vlan-id更新	*/
		(*vid_adr)++;
		if( *vid_adr > highVid ) {
			*vid_adr = lowVid ;
		}

		/* 次に使用するsearch-vlan-id更新	*/
		if( valid_cnt != 0 ) {

			while( *vid_adr != lastUsedVid) {

				ret = f_com_search_vid_chk( f_comw_searchable_vlan_info->seachable_vlan_id, *vid_adr );
				if( ret == D_RRH_OK ) {
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => DHCP Client %s.%d last:%d OK", f_com_ifname, *vid_adr ,lastUsedVid) ;
					break;
				} else {
					(*vid_adr)++;
					if( *vid_adr > highVid ) {
						*vid_adr = lowVid ;
					}
				}
			}
		}

		if( *vid_adr == lastUsedVid ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "  => DHCP Client %s.%d last:%d SAME OK", f_com_ifname, *vid_adr ,lastUsedVid) ;
			ret = D_SYS_NG ;
			break;
		}
	}
	if( f_comw_du_connection_tbl->mp.dhcpFlag == D_SYS_ON ) {
		/* interface設定(VID付き)	*/
		memset ( f_com_ifname, 0, sizeof(f_com_ifname) );
		sprintf( f_com_ifname, "%s", ifname );
	}
	return	f_comw_du_connection_tbl->mp.dhcpFlag;
}


/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_get_ipaddr_d
 * @note		IPアドレス取得処理を実行する。(DHCPあり/VLAN scanなし)
 * @param		highVid			[in]USHORT	High VID
  * @param		lowVid			[in]USHORT	Low VID
 * @param		*vid_adr		[in]USHORT*	検索開始VID
 * @retval		D_SYS_ON		1:正常終了
 * @retval		D_SYS_OFF		0:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/04/xx FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT		f_dhcp_get_ipaddr_d( USHORT	highVid, USHORT	lowVid, USHORT	*vid_adr, UINT	port_no )
{
	USHORT							lastUsedVid;						/* 前回値保存			*/

	CHAR							cmd_str[D_SYS_NUM128];				/* コマンド文字列		*/
	CHAR							file_path[D_SYS_NUM64];				/* ファイルパス文字列	*/
	CHAR							file_name[D_SYS_NUM64];				/* ファイル名文字列		*/
	LONG							file_size;

	INT								ret;								/* サブ関数処理結果		*/
	INT								errcd;								/* エラーコード			*/


	/********************************************************************************************/
	/* ローカル変数初期化																		*/
	/********************************************************************************************/
	lastUsedVid = *vid_adr;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "IP Address Get Method (DHCP[ON] / VLAN[OFF])") ;
	/********************************************************************************************/
	/* leasesファイル削除																		*/
	/********************************************************************************************/
	/* leasesファイル名初期設定	*/
	memset ( file_name, 0, sizeof(file_name) );
	sprintf( file_name, D_DHCP_FNAME_LEASES );

	/* leasesファイルパス初期設定	*/
	memset ( file_path, 0, sizeof(file_path) );
	sprintf( file_path, D_DHCP_PATH_DHCP );

	/********************************************************************************************/
	/* DHCP Client処理開始																		*/
	/********************************************************************************************/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "DHCP Client Start") ;

	while(1) {

		/* dhcp client処理実行	*/
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_DHCLIENT, f_com_ifname );
		ret = system ( cmd_str );

		/* reasesファイル生成チェック ⇒ 生成されている場合は、DHCP正常終了	*/
		ret = BPF_RU_FACC_FSIZE( (const bpf_ru_facc_dpathname *)&file_path, 
								 (const bpf_ru_facc_fname *)&file_name, 
								 &file_size, &errcd );

		if( (ret == D_SYS_OK) && (file_size != D_SYS_NUM0) ) {

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "create of leases file -> file size = %d byte",
								file_size ) ;
			ret = f_dhcp_fread_lease(  ) ;

			/* パラメータ取得結果チェック	*/
			if( ret == D_SYS_OK ) {

				/* テーブル設定	*/
				f_comw_du_connection_tbl->mp.dhcpFlag = D_SYS_ON;
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "DHCP Client with VLAN scan Complete") ;

				ret = D_SYS_OK ;
				break;
			}

			/* "/var/lib/dhclient/dhclient.leases"ファイル削除	*/
			memset ( cmd_str, 0, sizeof(cmd_str) );
			sprintf( cmd_str, D_DHCP_CMD_DEL_LEASES, file_path, file_name);
			ret = system ( cmd_str );
		}
		/* dhcp client処理実行	*/
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_DHCLIENTR, f_com_ifname );
		ret = system ( cmd_str );

		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_KILLALL );
		ret = system ( cmd_str );

		/***** VIDは、DHCP繰り返し回数として使用	*****/
		(*vid_adr)++;
		if( *vid_adr > highVid ) {
			*vid_adr = lowVid ;
		}
		if( *vid_adr == lastUsedVid ) {
			ret = D_SYS_NG ;
			break;
		}
	}
	return	f_comw_du_connection_tbl->mp.dhcpFlag;
}

/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_get_ipaddr_v
 * @note		IPアドレス取得処理を実行する。(DHCPなし/VLAN scanあり)
 * @param		highVid			[in]USHORT	High VID
  * @param		lowVid			[in]USHORT	Low VID
 * @param		*vid_adr		[in]USHORT*	検索開始VID
 * @retval		D_SYS_ON		1:正常終了
 * @retval		D_SYS_OFF		0:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/04/xx FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT		f_dhcp_get_ipaddr_v( USHORT	highVid, USHORT	lowVid, USHORT	*vid_adr, UINT	port_no )
{
	CHAR							cmd_str[D_SYS_NUM128];				/* コマンド文字列		*/
	CHAR							ifname[D_SYS_NUM32];				/* interface			*/

	INT								i;									/* ループ変数			*/
	INT								ret;								/* サブ関数処理結果		*/

	UINT							regAddr;							/* レジスタアドレス		*/
	UINT							regData = 0;						/* レジスタ書込み値		*/


	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "IP Address Get Method (DHCP[OFF] / VLAN[ON])") ;

	if( f_cmw_du_cnct_dbg_tbl.dFlag == D_SYS_ON ) {

		/* FILTVID15 レジスタアドレス  */
		regAddr = D_DU_REG_ETH_FILTVID15;
		/* FILTVID15 レジスタ設定値  */
		regData = 0;
		(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_OFF, regAddr, &regData);
		regData = (UINT)(D_DHCP_VAL_FILTVID15_EN | (*vid_adr & D_DHCP_VAL_FILTVID15_VID_MSK));
		(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_OFF, regAddr, &regData);

		/****************************************************************************************/
		/* ネットワーク情報設定(固定値)															*/
		/****************************************************************************************/
																		/* MAC address			*/
		f_comw_du_connection_tbl->mp.mac.vid		  = *vid_adr;		/* VID					*/
		f_comw_du_connection_tbl->mp.mac.pcp		  = D_RRH_MP_PCP;	/* PCP					*/
																		/* IPアドレス			*/
/*		f_comw_du_connection_tbl->mp.ip.ip_addr_du	  = f_cmw_du_cnct_dbg_tbl.src_addr;	*/
/*		f_comw_du_connection_tbl->mp.ip.ip_addr_cu	  = f_cmw_du_cnct_dbg_tbl.dst_addr;	*/
		f_comw_du_connection_tbl->mp.ip.default_gateway = ((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0xFFFFFF00) | 0x00000001) ;
		f_comw_du_connection_tbl->mp.ip.subnet_mask	  = 0xFFFFFF00 ;	/* SubnetMask			*/

		/* interface設定	*/
		memset ( ifname, 0, sizeof(ifname) );
		sprintf( ifname, "%s.%d", f_com_ifname, *vid_adr );

		/* ethにvidを関連付け	*/
		memset ( cmd_str, 0, sizeof(cmd_str) );
		sprintf( cmd_str, D_DHCP_CMD_VLAN_ADD, f_com_ifname, *vid_adr );
		ret = system ( cmd_str );

		if( ret == D_SYS_OK ) {
			/* eth有効化	*/
			memset ( cmd_str, 0, sizeof(cmd_str) );
			sprintf( cmd_str, D_DHCP_CMD_IFCONFIG_UP, ifname );
			ret = system ( cmd_str );

			/* interface設定(VID付き)	*/
			memset ( f_com_ifname, 0, sizeof(f_com_ifname) );
			sprintf( f_com_ifname, "%s", ifname );
		}

#if 0
		if( ret == D_SYS_OK ) {

			/* ethにpcpを設定	*/
			for(i = 0; i < 8; i++) {
				memset ( cmd_str, 0, sizeof(cmd_str) );
				sprintf( cmd_str, D_DHCP_CMD_VLAN_INGRESS, f_com_ifname, i, f_comw_du_netinfo_eep_tbl->pcp );
				ret = system ( cmd_str );
			}
		}
#endif
		if( ret == D_SYS_OK ) {

			/* ethにpcpを設定	*/
			for(i = 0; i < 8; i++) {
				memset ( cmd_str, 0, sizeof(cmd_str) );
				sprintf( cmd_str, D_DHCP_CMD_VLAN_EGRESS, f_com_ifname, i, f_comw_du_netinfo_eep_tbl->pcp );
				ret = system ( cmd_str );
			}
		}

		if( ret == D_SYS_OK ) {
			f_comw_du_connection_tbl->mp.dhcpFlag = D_SYS_ON;
		}

	} else {

		f_comw_du_connection_tbl->mp.dhcpFlag = D_SYS_OFF;
	}
	return	f_comw_du_connection_tbl->mp.dhcpFlag;
}


/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_get_ipaddr
 * @note		IPアドレス取得処理を実行する。(DHCPなし/VLAN scanなし)
 * @param		highVid			[in]USHORT	High VID
  * @param		lowVid			[in]USHORT	Low VID
 * @param		*vid_adr		[in]USHORT*	検索開始VID
 * @retval		D_SYS_ON		1:正常終了
 * @retval		D_SYS_OFF		0:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/04/xx FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT		f_dhcp_get_ipaddr( USHORT	highVid, USHORT	lowVid, USHORT	*vid_adr, UINT	port_no )
{
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "IP Address Get Method (DHCP[OFF] / VLAN[OFF])") ;

	if( f_cmw_du_cnct_dbg_tbl.dFlag == D_SYS_ON ) {

		/****************************************************************************************/
		/* ネットワーク情報設定(固定値)															*/
		/****************************************************************************************/
		f_comw_du_connection_tbl->mp.mac.vid		  = 0xFFFF;			/* VID					*/
		f_comw_du_connection_tbl->mp.mac.pcp		  = 0xFFFF;			/* PCP					*/
																		/* IPアドレス			*/
/*		f_comw_du_connection_tbl->mp.ip.ip_addr_du	  = f_cmw_du_cnct_dbg_tbl.src_addr;	*/
/*		f_comw_du_connection_tbl->mp.ip.ip_addr_cu	  = f_cmw_du_cnct_dbg_tbl.dst_addr;	*/
		f_comw_du_connection_tbl->mp.ip.default_gateway = ((f_comw_du_connection_tbl->mp.ip.ip_addr_du & 0xFFFFFF00) | 0x00000001) ;
		f_comw_du_connection_tbl->mp.ip.subnet_mask	  = 0xFFFFFF00 ;	/* SubnetMask			*/

		f_comw_du_connection_tbl->mp.dhcpFlag = D_SYS_ON;

	} else {

		f_comw_du_connection_tbl->mp.dhcpFlag = D_SYS_OFF;
	}
	return	f_comw_du_connection_tbl->mp.dhcpFlag;
}

/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_get_vendor_encapsulated_options
 * @note		vendor_encapsulated_optionsを取得する
 * @param		-
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/04/29 FJT)Taniguchi Create
 */
/****************************************************************************************************************************/
INT f_dhcp_get_vendor_encapsulated_options(CHAR* read_data, UINT* addr, CHAR* str){

	size_t 							value_len;
	size_t 							prefix_len;
	CHAR*							value_ptr;
	UINT							value_count;

	INT								type;
	INT								length;
	INT								value[4];
	INT								colon_count;
	

	prefix_len = strlen("  option vendor-encapsulated-options ");
	type = 0;
	length = 0;
	memset(value,0,sizeof(value));

	value_len = strlen(read_data) - prefix_len;
	value_ptr = read_data + prefix_len;
	colon_count = 0;
	for(value_count = 0 ; value_count < (UINT)value_len ; value_count++,value_ptr++){
		if( *value_ptr == ':' ){
			colon_count++;
			continue;
		}
		if( *value_ptr == ';' ){
			break;
		}
	}

	value_ptr = read_data + prefix_len;
	switch(colon_count){
		case 5:			/* TLV */
			sscanf( value_ptr, "%x:%x:%x:%x:%x:%x;", &type, &length, &value[0], &value[1], &value[2], &value[3] );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "vendor-encapsulated-options is TLV(%d) %s type:%d len:%d value:0x%x:0x%x:0x%x:0x%x:",colon_count , value_ptr , type , length , value[0], value[1], value[2], value[3] );
			break;
		case 3:			/* LV */
			sscanf( value_ptr, "%x:%x:%x:%x;", &value[0], &value[1], &value[2], &value[3] );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "vendor-encapsulated-options is LV(%d) %s value:0x%x:0x%x:0x%x:0x%x:",colon_count , value_ptr ,value[0], value[1], value[2], value[3] );
			break;
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "vendor-encapsulated-options is error %d %s",colon_count ,value_ptr) ;
			return D_SYS_NG;
			break;
	}
	*addr = (UINT)( ( (value[0] << 24) & 0xFF000000) | ( (value[1] << 16) & 0xFF0000) |  ( (value[2] << 8) & 0xFF00)  | (value[3]  & 0xFF ) );
	snprintf( str, (value_len - 1), "%s", value_ptr );
	return D_SYS_OK;
}

/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_fread_lease
 * @note		leaseファイルからIP Layer情報を取得する。
 * @param		-
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/04/xx FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT		f_dhcp_fread_lease( VOID )
{
	FILE							*fp;
	CHAR							file_full[D_SYS_NUM64];				/* ファイル名文字列		*/
	CHAR							read_data[D_SYS_NUM128];			/* 読出しデータ			*/
	CHAR							rdata1[D_SYS_NUM16];				/* 読出しデータ			*/
	CHAR							rdata2[D_SYS_NUM32];				/* 読出しデータ			*/

	INT								addr[4];
	INT								ret = D_SYS_OK;						/* サブ関数処理結果		*/

	UCHAR							Flag = 0x00;


	/********************************************************************************************/
	/* leasesファイルからの取得																	*/
	/********************************************************************************************/
	/* leasesファイル	*/
	memset( file_full, 0, sizeof(file_full) );
	strcpy( file_full, D_DHCP_PATH_DHCP);
	strcat( file_full, D_DHCP_FNAME_LEASES );

	/* leasesファイルオープン	*/
	fp = fopen( file_full, "r" );
	if( fp == NULL ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Leases File Open NG" );
		return	D_SYS_NG;
	}

	while(1) {

		/* ファイル読出し情報格納領域初期化	*/
		memset(read_data, 0, sizeof(read_data));
		memset(rdata1, 0, sizeof(rdata1));
		memset(rdata2, 0, sizeof(rdata2));

		/* 1行分読出し*/
		if( fgets(read_data, (D_SYS_NUM128 - 1), fp) == NULL ) {

			if( (Flag & 0x1F) != 0x1F ) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Network Parameter Get Error (leases file)" );
				ret = D_SYS_NG;
			}
			break;
		}

		if( strstr(read_data, "fixed-address") != NULL ) {

			/* Local IP Address取得	*/
			sscanf( read_data, "%s %d.%d.%d.%d", rdata1, &addr[0], &addr[1], &addr[2], &addr[3] );
			f_comw_du_connection_tbl->mp.ip.ip_addr_du
								 = (UINT)((addr[0] << 24) | (addr[1] << 16) | (addr[2] << 8) | addr[3]);
			Flag = Flag | 0x01;
			continue;
		}

		if( strstr(read_data, "subnet-mask") != NULL ) {

			/* Subnet Mask取得	*/
			sscanf( read_data, "%s %s %d.%d.%d.%d", rdata1, rdata2, &addr[0], &addr[1], &addr[2], &addr[3] );
			f_comw_du_connection_tbl->mp.ip.subnet_mask
							 = (UINT)((addr[0] << 24) | (addr[1] << 16) | (addr[2] << 8) | addr[3]);
			Flag = Flag | 0x02;
			continue;
		}

		if( strstr(read_data, "routers") != NULL ) {

			/* Default gateway取得	*/
			sscanf( read_data, "%s %s %d.%d.%d.%d", rdata1, rdata2, &addr[0], &addr[1], &addr[2], &addr[3] );
			f_comw_du_connection_tbl->mp.ip.default_gateway
							 = (UINT)((addr[0] << 24) | (addr[1] << 16) | (addr[2] << 8) | addr[3]);
			Flag = Flag | 0x04;
			continue;
		}

		if( strstr(read_data, "dhcp-server-identifier") != NULL ) {

			/* Remote IP Address取得	*/
			sscanf( read_data, "%s %s %d.%d.%d.%d", rdata1, rdata2, &addr[0], &addr[1], &addr[2], &addr[3] );
			f_comw_du_connection_tbl->mp.ip.ip_addr_dhcps
							 = (UINT)((addr[0] << 24) | (addr[1] << 16) | (addr[2] << 8) | addr[3]);
			Flag = Flag | 0x08;
			continue;
		}

		if( strstr(read_data, "vendor-encapsulated-options") != NULL ) {

			memset( &(f_comw_du_connection_tbl->mp.ip.vendor_specific_option[0]), 0, 20 );
			if(( ret = f_dhcp_get_vendor_encapsulated_options(read_data,
															  &(f_comw_du_connection_tbl->mp.ip.ip_addr_cu),
															  &(f_comw_du_connection_tbl->mp.ip.vendor_specific_option[0]) ) ) == D_SYS_NG){
				break;
			}
			Flag = Flag | 0x10;
			continue;
		}
	}
	/* leaseファイルclose	*/
	fclose( fp ) ;
	return	ret;
}
/* @} */

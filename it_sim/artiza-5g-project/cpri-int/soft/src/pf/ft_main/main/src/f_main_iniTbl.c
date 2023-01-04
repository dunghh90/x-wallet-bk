/*!
 * @skip		$ld:$
 * @file		f_main_iniTbl.c
 * @brief		pf_main テーブルの初期化処理
 * @author		alpha)中村
 * @date		2013/12/12 alpha)中村 Create
 * @date  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date		2015/09/24 ALPHA) ueda modify for TDD-RRE(Zynq)
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"
/*!
 * @brief		f_main_iniTbl
 * @note		PFプロセステーブルの初期化処理を行う.
 * @param		-
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/12/12 alpha)中村
 * @data  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date		2015/08/27 ALPHA) miyazaki modify to TDD-RRE-Zynq 
 * @date		2015/09/24 ALPHA) ueda modify for TDD-RRE(Zynq)
 */
#define BigLittleSwap16(V)  ((((USHORT)(V) & 0xff00) >> 8) | \
                            (((USHORT)(V) & 0x00ff) << 8))

VOID f_main_iniTbl()
{
	T_RRH_DU_INFORMATION_TBL		*du_info_tbl_adr;					/**< DU情報テーブル		*/

	INT								ret;
	INT								errcd;
	UCHAR							Flag = 0;							/**< PC接続フラグ from EEPROM	*/
	UCHAR							pcp;

	UCHAR							regdata_h;
	UCHAR							regdata_m;
	UCHAR							regdata_l;

	UCHAR							vid_valid_flag = D_RRH_OFF;

	int								i;									/**< loop回数  */


	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_iniTbl] ENTER" );

	/************************************************************************/
	/* システムパラメータTypical値設定										*/
	/************************************************************************/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY,(VOID**)&f_cmw_tra_inv_tbl,&errcd);	
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
	}
	f_com_iniSysParaTbl();

	/****************************************/
	/* Beamformingモードテーブル初期設定	*/
	/****************************************/
	/* DU情報テーブル 共有メモリアドレス取得	*/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_BF_MODE_TBL,					/* 共有メモリ番号		*/
						(VOID **)&f_comw_bf_mode,						/* 共有メモリアドレス	*/
						&errcd );										/* エラーコード			*/
	if(ret != BPF_RU_IPCM_OK) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
	}
	/* 装置種別がmmW、かつ、nonBeamformingモードの場合、起動条件アンマッチ	*/
	if( (f_comw_bf_mode->flag == D_RRH_ON) && (M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_RRH_EEP_DEV_KIND_5GDU_MMW) ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Startup condition Error : devkind=%02x / freq=%02x / beamforming_mode=%d",
							M_DU_RRHKIND_KIND_GET(f_comw_rrhKind), M_DU_RRHKIND_FREQ_GET(f_comw_rrhKind), f_comw_bf_mode->flag );
	}

	/****************************************/
	/* DUネットワーク情報テーブル初期設定	*/
	/****************************************/
	/* DU情報テーブル 共有メモリアドレス取得	*/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_DU_INFORMATION_TBL,				/* 共有メモリ番号		*/
						(VOID **)&du_info_tbl_adr,						/* 共有メモリアドレス	*/
						&errcd );										/* エラーコード			*/
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
	}
	f_comw_du_netinfo_eep_tbl  = &du_info_tbl_adr->eeprom_info;
	f_comw_searchable_vlan_info = &du_info_tbl_adr->flash_info;
	f_comw_du_connection_tbl   = &du_info_tbl_adr->du_connection_info;

	/*==== DUネットワーク情報テーブル -> EEPROM情報 初期設定	====*/
	/***** MAC Address取得		*****/
	/* EEPROMからMAC Address (Front Haul Port #0) 取得	*/
	f_main_iniTbl_eepread(D_RRH_EEP_MAC_ADDR_PORT1_H1, D_RRH_MAC_ADDR_SIZE, &f_comw_du_netinfo_eep_tbl->localMacAddress[0][0]);

	/***** VLAN ID取得			*****/
	f_main_iniTbl_eepread(D_DU_EEP_VID_SCAN_MODE, sizeof(UCHAR), &vid_valid_flag);
	if( vid_valid_flag == D_RRH_ON ) {
		/* EEPROMからlowVid情報取得	*/
		f_main_iniTbl_eepread(D_RRH_EEP_VID_LOW_PORT1_H, sizeof(USHORT),  (UCHAR *)&f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid);
		BPF_COM_END_BIGTOLITTLE16(f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid);

		/* EEPROMからhighVid情報取得	*/
		f_main_iniTbl_eepread(D_RRH_EEP_VID_HIGH_PORT1_H, sizeof(USHORT), (UCHAR *)&f_comw_du_netinfo_eep_tbl->vid_search_info.highVid);
		BPF_COM_END_BIGTOLITTLE16(f_comw_du_netinfo_eep_tbl->vid_search_info.highVid);
	} else {
		/* デフォルト値を設定	*/
		f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid  = D_RRH_VID_MIN;

		/* EEPROM更新	*/
		regdata_h = (UCHAR)( (f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid & 0xFF00) >> 8 );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_LOW_PORT1_H, &regdata_h);

		regdata_l = (UCHAR)( f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid & 0x00FF );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_LOW_PORT1_L, &regdata_l);

		f_comw_du_netinfo_eep_tbl->vid_search_info.highVid = D_RRH_VID_MAX;

		/* EEPROM更新	*/
		regdata_h = (UCHAR)( (f_comw_du_netinfo_eep_tbl->vid_search_info.highVid & 0xFF00) >> 8 );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_HIGH_PORT1_H, &regdata_h);

		regdata_l = (UCHAR)( f_comw_du_netinfo_eep_tbl->vid_search_info.highVid & 0x00FF );
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_HIGH_PORT1_L, &regdata_l);

		/* VLAN情報FLASH書込みフラグ 初期化	*/
		regdata_l = 0xFF;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_FLASH_WRITE_FLAG_VLAN, &regdata_l);
	}

	/* EEPROMからlastUsedVid情報取得	*/
	f_main_iniTbl_eepread(D_RRH_EEP_VID_LAST_PORT1_H, sizeof(USHORT), (UCHAR *)&f_comw_du_netinfo_eep_tbl->vid_search_info.lastUsedVid);
	BPF_COM_END_BIGTOLITTLE16(f_comw_du_netinfo_eep_tbl->vid_search_info.lastUsedVid);

	/* vlan scanを行うvlan-id範囲チェック	*/
	if( (f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid  < D_RRH_VID_MIN)
	 || (f_comw_du_netinfo_eep_tbl->vid_search_info.highVid < D_RRH_VID_MIN)
	 || (f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid  > D_RRH_VID_MAX)
	 || (f_comw_du_netinfo_eep_tbl->vid_search_info.highVid > D_RRH_VID_MAX)
	 || (f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid  > f_comw_du_netinfo_eep_tbl->vid_search_info.highVid) ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "search vlan-id range error. highest-vlan-id=%08x, lowest-vlan-id=%08x", 
							f_comw_du_netinfo_eep_tbl->vid_search_info.highVid, f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid);

		f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid  = D_RRH_VID_MIN;
		f_comw_du_netinfo_eep_tbl->vid_search_info.highVid = D_RRH_VID_MAX;
	}

	/* vlan scanを開始するvlan-idチェック	*/
	if( (f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid  > f_comw_du_netinfo_eep_tbl->vid_search_info.lastUsedVid)
	 || (f_comw_du_netinfo_eep_tbl->vid_search_info.highVid < f_comw_du_netinfo_eep_tbl->vid_search_info.lastUsedVid) ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "start vlan-id error. last-used-vlan-id=%08x", 
							f_comw_du_netinfo_eep_tbl->vid_search_info.lastUsedVid);

		f_comw_du_netinfo_eep_tbl->vid_search_info.lastUsedVid = f_comw_du_netinfo_eep_tbl->vid_search_info.lowVid;
	}
	/* VID現在値初期化	*/
	f_comw_du_netinfo_eep_tbl->vid_search_info.nowVid = 0x0000;

	f_main_iniTbl_eepread(D_DU_EEP_VID_MPLANE_PCP, sizeof(CHAR),  (UCHAR *)&pcp);
	if( pcp > D_RRH_PCP_MAX ) {
		f_comw_du_netinfo_eep_tbl->pcp = D_RRH_PCP_MPLANE;
	} else {
		f_comw_du_netinfo_eep_tbl->pcp = (USHORT)pcp;
	}


	/*==== DUネットワーク情報テーブル -> DUコネクション情報 初期設定	====*/
	/* M-Plane */
	f_comw_du_connection_tbl->mp.port_no  = 0xFFFFFFFF;
	f_comw_du_connection_tbl->mp.dhcpFlag = D_RRH_OFF;
	f_comw_du_connection_tbl->mp.mac.vid  = 0xFFFF;
	f_comw_du_connection_tbl->mp.mac.pcp  = 0xFFFF;

	/* U/FC-Plane	*/
	f_comw_du_connection_tbl->ucp[0].port_no  = 0;
	f_comw_du_connection_tbl->ucp[0].umac.vid = 0xFFFF;
	f_comw_du_connection_tbl->ucp[0].umac.pcp = D_RRH_PCP_UPLANE;
	f_comw_du_connection_tbl->ucp[0].cmac.vid = 0xFFFF;
	f_comw_du_connection_tbl->ucp[0].cmac.pcp = D_RRH_PCP_CPLANE;

	f_comw_du_connection_tbl->ucp[1].port_no  = 1;
	f_comw_du_connection_tbl->ucp[1].umac.vid = 0xFFFF;
	f_comw_du_connection_tbl->ucp[1].umac.pcp = D_RRH_PCP_UPLANE;
	f_comw_du_connection_tbl->ucp[1].cmac.vid = 0xFFFF;
	f_comw_du_connection_tbl->ucp[1].cmac.pcp = D_RRH_PCP_CPLANE;


	/****************************************/
	/* NETCONFユーザー情報テーブル初期設定	*/ /* TODO:pf_nmaで設定する予定(2019/3E)	*/
	/****************************************/
	/* NETCONFユーザー情報テーブル 共有メモリアドレス取得	*/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_T_RRH_NETCONF_INFO_TBL,			/* 共有メモリ番号		*/
						(VOID **)&f_comw_netconf_tbl,					/* 共有メモリアドレス	*/
						&errcd );										/* エラーコード			*/
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
	}
	f_comw_netconf_tbl->callhome_ssh_port = D_DU_DEFAULT_SSHPORTNO_CALLHOME;
	f_comw_netconf_tbl->server_ssh_port   = D_DU_DEFAULT_SSHPORTNO;

	/****************************************/
	/* TX CAL係数テーブル初期設定			*/
	/****************************************/
	/* TX CAL係数テーブル 共有メモリアドレス取得	*/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_TXCAL_COEF,						/* 共有メモリ番号		*/
						(VOID **)&f_comw_txcal_coef_tbl,				/* 共有メモリアドレス	*/
						&errcd);										/* 詳細NGコード			*/
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
	}
	for( i = 0; i < D_RRH_ANT_MAX; i++ ) {
		f_comw_txcal_coef_tbl->cal_coef[i] = D_RRH_ANT_CAL_COEF_INIT;
	}


	/****************************************/
	/* RX CAL係数テーブル初期設定			*/
	/****************************************/
	/* RX CAL係数テーブル 共有メモリアドレス取得	*/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_RXCAL_COEF,						/* 共有メモリ番号		*/
						(VOID **)&f_comw_rxcal_coef_tbl,				/* 共有メモリアドレス	*/
						&errcd);										/* 詳細NGコード			*/
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
	}
	for( i = 0; i < D_RRH_ANT_MAX; i++ ) {
		f_comw_rxcal_coef_tbl->cal_coef[i] = D_RRH_ANT_CAL_COEF_INIT;
	}


	/****************************************/
	/* Performanceログテーブル初期設定		*/
	/****************************************/
	/* RX CAL係数テーブル 共有メモリアドレス取得	*/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_PFM_PERFORMANCE_TBL,			/* 共有メモリ番号		*/
						(VOID **)&f_comw_pfmlog_tbl,					/* 共有メモリアドレス	*/
						&errcd);										/* 詳細NGコード			*/
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
	}
	memset( (VOID*)f_comw_pfmlog_tbl, 0, sizeof(T_RRH_PFM_PERFORMANCE_TBL) );
	f_comw_pfmlog_tbl->devkind = M_DU_RRHKIND_KIND_GET(f_comw_rrhKind);

	/****************************************/
	/* Performance情報管理テーブル初期設定	*/
	/****************************************/
	/* RX CAL係数テーブル 共有メモリアドレス取得	*/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_PFM_PERFORMANCE_MNG_TBL,		/* 共有メモリ番号		*/
						(VOID **)&f_comw_pfmmng_tbl,					/* 共有メモリアドレス	*/
						&errcd);										/* 詳細NGコード			*/
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
	}
	memset( (VOID*)f_comw_pfmmng_tbl, 0, sizeof(E_RRH_SHMID_APL_PFM_PERFORMANCE_MNG_TBL) );
	f_comw_pfmmng_tbl->measurement.init_interval   = 0xFFFFFFFF;
	f_comw_pfmmng_tbl->measurement.interval        = 0xFFFFFFFF;
	f_comw_pfmmng_tbl->notification.init_interval  = 0xFFFFFFFF;
	f_comw_pfmmng_tbl->notification.interval       = 0xFFFFFFFF;


	/****************************************/
	/* LBM/LBR管理テーブル初期設定			*/
	/****************************************/
	/* RX CAL係数テーブル 共有メモリアドレス取得	*/
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_LBM_MNGMENT_TBL,				/* 共有メモリ番号		*/
						(VOID **)&f_comw_lbm_mngment_tbl,				/* 共有メモリアドレス	*/
						&errcd);										/* 詳細NGコード			*/
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
	}
	memset( (VOID*)f_comw_lbm_mngment_tbl, 0, sizeof(T_RRH_LBM_MNGMENT_TBL) );

	for( i = 0; i < 8; i++ ) {
		f_comw_lbm_mngment_tbl->info[i].md_level = i;
	}
	for( i = 0; i < D_RRH_IF_FHMAX ; i++ ) {
		f_comw_lbm_mngment_tbl->port_info[i].port = i;
	}

	/****************************************/
	/* SFN/FRM補正用テーブル初期設定		*/
	/****************************************/
	/* SFN/FRM補正用テーブル 共有メモリのアドレス取得 */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET( 
			E_RRH_SHMID_APL_SFNFRM_CALB_TBL,				/* 共有メモリ番号	*/
			(VOID **)&f_comw_sfnfrm_tbl,					/* 共有メモリアドレス	*/
			&errcd );										/* エラーコード			*/
	if( ret != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x", ret, errcd);
		return ;
	}
	/* ns補正フラグ設定(OFF固定)	*/
	f_comw_sfnfrm_tbl->ns_flag = D_DU_SFNRFN_CORRECTION_STOP;

	/* offset_a設定(EEPROM読出し)	*/
	BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_OFFSET_A_H, (unsigned char *)&regdata_h );
	BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_OFFSET_A_M, (unsigned char *)&regdata_m );
	BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_OFFSET_A_L, (unsigned char *)&regdata_l );
	f_comw_sfnfrm_tbl->offset_a = ((regdata_h << 16) + (regdata_m << 8) + regdata_l);

	/* offset_b設定(EEPROM読出し)	*/
	BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_OFFSET_B_H, (unsigned char *)&regdata_h );
	BPF_HM_DEVC_EEPROM_READ( D_RRH_EEP_OFFSET_B_L, (unsigned char *)&regdata_l );
	f_comw_sfnfrm_tbl->offset_b =((regdata_h << 8) + regdata_l);

	/* offset_a範囲チェック	*/
	if( (f_comw_sfnfrm_tbl->offset_a < 0) || (f_comw_sfnfrm_tbl->offset_a > 12288000) )
	{
		/* SFN_RFM補正フラグ設定(OFF固定)	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR,"SFN/FRM offset_a range error : %d",
							f_comw_sfnfrm_tbl->offset_a );
		f_comw_sfnfrm_tbl->sfnfrm_flag = D_SYS_OFF;
	}
	else
	{
		f_comw_sfnfrm_tbl->sfnfrm_flag = D_SYS_ON;
	}


	/****************************************/
	/***** デバッグ情報テーブル初期設定	*****/
	/****************************************/
	/* デバッグ情報テーブル初期化	*/
	memset((VOID*)&f_cmw_du_cnct_dbg_tbl, 0, sizeof(T_COM_DU_CNCT_DBG_TBL));

	/* EEPROMからPC接続フラグ取得	*/
	f_main_iniTbl_eepread( D_RRH_EEP_PC_CONNECT_FLAG, sizeof(UCHAR), &Flag );
	f_cmw_du_cnct_dbg_tbl.dFlag = Flag;

	/* EEPROMからIP Address (Front Haul Port #0) 取得	*/
	f_main_iniTbl_eepread(D_RRH_EEP_IP_ADDR_PORT1_H1, sizeof(UINT), (UCHAR *)&f_cmw_du_cnct_dbg_tbl.src_addr[0]);
	f_cmw_du_cnct_dbg_tbl.src_addr[0] = ntohl(f_cmw_du_cnct_dbg_tbl.src_addr[0]);

	/* EEPROMからIP Address (Front Haul Port #1) 取得	*/
	f_main_iniTbl_eepread(D_RRH_EEP_IP_ADDR_PORT2_H1, sizeof(UINT), (UCHAR *)&f_cmw_du_cnct_dbg_tbl.src_addr[1]);
	f_cmw_du_cnct_dbg_tbl.src_addr[1] = ntohl(f_cmw_du_cnct_dbg_tbl.src_addr[1]);

	/* EEPROMからIP Address (Debug Port)取得	*/
	f_main_iniTbl_eepread(D_RRH_EEP_IP_ADDR_DBG_H1,   sizeof(UINT), (UCHAR *)&f_cmw_du_cnct_dbg_tbl.src_addr[2]);
	f_cmw_du_cnct_dbg_tbl.src_addr[2] = ntohl(f_cmw_du_cnct_dbg_tbl.src_addr[2]);

	/* EEPROMからIP Address (Debug Port)取得	*/
	f_main_iniTbl_eepread(D_RRH_EEP_IP_ADDR_ACCESS_H1, sizeof(UINT), (UCHAR *)&f_cmw_du_cnct_dbg_tbl.dst_addr);
	f_cmw_du_cnct_dbg_tbl.dst_addr    = ntohl(f_cmw_du_cnct_dbg_tbl.dst_addr);

	/****************************************/
	/* PTP Configurationテーブル初期設定	*/
	/****************************************/
	/* 共有メモリの取得 */
	ret = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_PTP_CONF,						/* 共有メモリ番号		*/
						(VOID **)&f_comw_ptpconf_tbl,					/* 共有メモリアドレス	*/
						&errcd );										/* エラーコード			*/

	/********************************/
	/*	SV制御テーブル初期化		*/
	/********************************/
	/*	装置種別がmmWの場合	*/
	if(M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_DU_EEP_DEV_KIND_5GDU_MMW)
	{
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_A].infop = (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptUseAmTbl;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_A].infoSize = sizeof(f_comr_svOptUseAmTbl);
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_A].mask = D_DU_SVCTL_BIT_USESLP_A_ALL_MMW;			/*	0x000000FF	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_B].infop = (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptUseBmTbl;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_B].infoSize = sizeof(f_comr_svOptUseBmTbl);
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_B].mask = D_DU_SVCTL_BIT_USESLP_B_ALL_MMW;			/*	0x0000FF00	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_C].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_C].mask = D_DU_SVCTL_BIT_USESLP_CD_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_D].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_USE_D].mask = D_DU_SVCTL_BIT_USESLP_CD_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID04_ALM].infop = (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid04mTbl;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID04_ALM].infoSize = sizeof(f_comr_svOptFid04mTbl);
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID04_ALM].mask = D_DU_ALMBIT_FLT04_ALL_MMW;				/*	0x00000030	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID07_ALM].infop = (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptFid07mTbl;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID07_ALM].infoSize = sizeof(f_comr_svOptFid07mTbl);
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID07_ALM].mask = D_DU_ALMBIT_FLT07_ALL_MMW;				/*	0xC5F71FFF	*/
//		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID08_ALM].infoSize = 0;
//		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID08_ALM].mask = D_DU_ALMBIT_FLT08_ALL_MMW;				/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_A].infop = (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF10AntAmTbl;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_A].infoSize = sizeof(f_comr_svOptF10AntAmTbl);
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_A].mask = D_DU_ALMBIT_FLT10_A_ALL_MMW;			/*	0x00000002	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_B].infop = (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF10AntBmTbl;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_B].infoSize = sizeof(f_comr_svOptF10AntBmTbl);
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_B].mask = D_DU_ALMBIT_FLT10_B_ALL_MMW;			/*	0x00020000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_C].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_C].mask = D_DU_ALMBIT_FLT10_CD_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_D].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_D].mask = D_DU_ALMBIT_FLT10_CD_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_A].infop = (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF11AntAmTbl;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_A].infoSize = sizeof(f_comr_svOptF11AntAmTbl);
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_A].mask = D_DU_ALMBIT_FLT11_A_ALL_MMW;			/*	0x00000100	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_B].infop = (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF11AntBmTbl;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_B].infoSize = sizeof(f_comr_svOptF11AntBmTbl);
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_B].mask = D_DU_ALMBIT_FLT11_B_ALL_MMW;			/*	0x01000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_C].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_C].mask = D_DU_ALMBIT_FLT11_CD_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_D].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID11_ALM_D].mask = D_DU_ALMBIT_FLT11_CD_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID12_ALM_A].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID12_ALM_A].mask = D_DU_ALMBIT_FLT12_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID12_ALM_B].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID12_ALM_B].mask = D_DU_ALMBIT_FLT12_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID12_ALM_C].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID12_ALM_C].mask = D_DU_ALMBIT_FLT12_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID12_ALM_D].infoSize = 0;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID12_ALM_D].mask = D_DU_ALMBIT_FLT12_ALL_MMW;			/*	0x00000000	*/
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_VH].infop = (T_COM_DU_SVBUS_CTRL_INFO_TBL *)&f_comr_svOptF10VHTbl;
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_VH].infoSize = sizeof(f_comr_svOptF10VHTbl);
		f_comw_svOptMapTbl[E_DU_SVCTL_OPE_FID10_ALM_VH].mask = D_DU_ALMBIT_FLT10_VH_ALL_MMW;		/*	0xFFFFFFFF	*/
	}

	/************************/
	/*	product-code初期化	*/
	/************************/
	if( (M_DU_RRHKIND_FREQ_GET(f_comw_rrhKind) == D_DU_EEP_DEV_FREQ_45G)
	 && (M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_DU_EEP_DEV_KIND_5GDU_SUB6) ) {			/* 4.5G-sub6	*/

		memset ( f_com_product_code, 0, 16 );
		sprintf( f_com_product_code, "%s", D_DU_PRODUCT_CODE_45G_SUB6 );

	} else if( (M_DU_RRHKIND_FREQ_GET(f_comw_rrhKind) == D_DU_EEP_DEV_FREQ_37G)
			&& (M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_DU_EEP_DEV_KIND_5GDU_SUB6) ) {	/* 3.7G-sub6	*/

		memset ( f_com_product_code, 0, 16 );
		sprintf( f_com_product_code, "%s", D_DU_PRODUCT_CODE_37G_SUB6 );

	} else if( (M_DU_RRHKIND_FREQ_GET(f_comw_rrhKind) == D_DU_EEP_DEV_FREQ_28G)
			&& (M_DU_RRHKIND_KIND_GET(f_comw_rrhKind) == D_DU_EEP_DEV_KIND_5GDU_MMW) ) {	/* 28G-mmW		*/

		memset ( f_com_product_code, 0, 16 );
		sprintf( f_com_product_code, "%s", D_DU_PRODUCT_CODE_280G_MMW );

	} else {	/* f_main_rrhKindGetでチェック済みのため、あり得ないルート	*/

		/* Abort処理	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DevFreq Error. DevKind=%02x, DevFreq=%02x", 
							M_DU_RRHKIND_KIND_GET(f_comw_rrhKind), M_DU_RRHKIND_FREQ_GET(f_comw_rrhKind) );
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_DEVKIND_ABNORMAL);
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_iniTbl] RETURN" );
	return;
}

VOID	f_main_iniTbl_eepread( USHORT	addr, UCHAR	size, UCHAR	*data_p )
{
	INT			i;
	INT			ret;

	for( i = 0; i < size; i++, addr++, data_p++ ) {

		ret = BPF_HM_DEVC_EEPROM_READ( addr, data_p );

		if(ret != BPF_RU_IPCM_OK)
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "BPF_HM_DEVC_EEPROM_READ error. Addr=%04x, ret=%08x", addr, ret);
		}
	}
}
/* @} */

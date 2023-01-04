/*!
 * @skip		$ld:$
 * @file		f_ini_main_con.c
 * @brief		NETCONF conection メイン処理
 * @author		FJT) Yoshida
 * @date		2018/11/23 FJT)吉田 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "f_ini_inc.h"
#include "f_mpsw_msi.h"
									/*	[0]			[1]				[2]									 [3]			[4]				[5]				[6]						[7]	*/
static	CHAR*	f_inir_Saa_pram[] = { "DU_agent",							/* [0]	*/
									  "",									/* [1]	*/
									  "/opt/agent/etc/saa_nvm_data.conf",	/* [2]	*/
									  "--startup=",							/* [3]	*/
									  "--config=",							/* [4]	*/
									  "--log-vendor",						/* [5]	*/
									  "--log-vendor-level=",				/* [6]	*/
									  "--runpath=/opt/agent/lib",			/* [7]	*/
									  NULL };								/* [8]	*/

static	CHAR*	f_inir_Nsa_pram[] = { "netconfd-pro",						/* [0]	*/
									  "--startup=",							/* [1]	*/
									  "--callhome-server=",					/* [2]	*/
									  "--config=",							/* [3]	*/
									  "--log-vendor",						/* [4]	*/
									  "--log-vendor-level=",				/* [5]	*/
									  "--callhome-retry-interval=",			/* [6]	*/
									  "--runpath=/opt/agent/lib",			/* [7]	*/
									  NULL };								/* [8]	*/

static	const	T_INI_CRT_EVENT_CTBL	f_inir_procCreMng_mp[2] = {
	 { "DU_agent",			  0,	D_RRH_PROCID_MPSW_SAA,		94,				3,		0	},	/* SCHED_FIFO */
	 { "netconfd-pro",		  0,	D_RRH_PROCID_MPSW_NSA,		94,				3,		0	}	/* SCHED_FIFO */
};

static CHAR		nsa_log_file_name[]     = "/var/log/netconfd-pro.log";
static CHAR		nsa_log_file_name_old[] = "/var/log/netconfd-pro.log.old";
static CHAR		nsa_log_file_name_stup[] = "/var/log/netconfd-pro.log.stup";
static CHAR		nsa_log_file_rotate_flag = D_RRH_OFF;
USHORT			retry_interval = D_RRH_CALLHOME_TIMER_DFAULT;


static CHAR*	f_iniw_ru_admin_state = "unlocked";

#define SAA_LOGLEVEL			0
#define NSA_LOGLEVEL			1
#define NSA_LOGFILE_ROTATE_SIZE	0x180000	/* 1.5M */
#define NSA_LOGFILE_ROTATE_TIME	10			/* 10sec */

/*!
 * @brief		MPLANE_SW_INIT_COMP_NTC送信処理 
 * @note		MPLANE_SW_INIT_COMP_NTC送信処理
 * @retval		none
 * @return		none
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/12/04 FJT)Taniguchi Create
 */
static 	void f_ini_main_con_send_mpstart(void)
{
	INT								cnt;
	INT								rtn_bpf = D_RRH_OK;
	T_SYS_MPLANE_SW_INIT_COMP_NTC	*sndMsg;		/* 送信MSG		*/
/* 2020/12/28 M&C) Implement device startup (start modify) */
	UINT							DstPQueueID[D_INI_PROCNO_MAX - 1] = {	D_RRH_PROCQUE_PF,
																			D_RRH_PROCQUE_PTP,
																			D_RRH_PROCQUE_F_PF,
																			D_RRH_PROCQUE_L2,
																			D_RRH_PROCQUE_L3,
																			D_RRH_PROCQUE_MT,
																			D_RRH_PROCQUE_RE};
/* 2020/12/28 M&C) Implement device startup (end modify) */
	INT								errcd;


	/* PF ProcessにM-PlaneSW起動完了通知を送信	*/
	for( cnt = 0; cnt < (procIdTbl->count - 1); cnt++)
	{
		/* バッファ取得 */
		rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別						*/
											sizeof(T_SYS_MPLANE_SW_INIT_COMP_NTC),	/* MessageSize						*/
											(VOID **)&sndMsg,						/* MessageBuffer					*/
											&errcd );								/* ErrorCode						*/
		if(rtn_bpf != BPF_RU_IPCM_OK)
		{
			/* BPF_RM_SVRM_INITEND_IND_RCV異常 */
			cmn_com_assert_th_get(f_iniw_assert_con_p,D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG return=%08x, error=%08x", rtn_bpf, errcd);
			break;
		}
		/* 各プロセスへの「T_SYS_MPLANE_SW_INIT_COMP_NTC」作成	*/
		sndMsg->head.uiEventNo		= D_SYS_MSGID_MPLANE_SW_INIT_COMP_NTC;			/* メッセージID						*/
		sndMsg->head.uiDstPQueueID	= DstPQueueID[cnt];								/* 送信先Process Queue ID			*/
		sndMsg->head.uiDstTQueueID	= 1;											/* 送信先Thread Queue ID(xx_Main)	*/
		sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_INIT;							/* 送信元Process Queue ID			*/
		sndMsg->head.uiSrcTQueueID	= 0;											/* 送信元Thread Queue ID			*/
		sndMsg->head.uiLength		= sizeof(T_SYS_MPLANE_SW_INIT_COMP_NTC);		/* データ部バイト長					*/
		/* 各プロセスへ「T_SYS_MPLANE_SW_INIT_COMP_NTC」を通知	*/
		rtn_bpf = BPF_RU_IPCM_MSGQ_SEND( DstPQueueID[cnt], (VOID *)sndMsg );
		if(rtn_bpf != BPF_RU_IPCM_OK){
			/* BPF_RU_IPCM_MSGQ_SEND異常 */
			cmn_com_assert_th_get(f_iniw_assert_con_p,D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG return=%08x", rtn_bpf);
			break;
		}
		else{
			cmn_com_assert_th_get(f_iniw_assert_con_p,D_RRH_LOG_AST_LV_RETURN, "M-Plane SW complete notice send OK DstPQueueID=%08x", DstPQueueID[cnt]);
		}
	}

	return;
}

/*!
 * @brief		LOGLevel処理 
 * @note		LOGLevel処理
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/12/04 FJT)Taniguchi Create
 */
static void f_ini_main_loglevel(INT kind ,int namesize, CHAR* loglvlname)
{
	CHAR*	env_name;
	CHAR*	lvl_name;
	INT		lvl_name_len;
	CHAR*	last_lvl_name;

	if(NSA_LOGLEVEL == kind){
		env_name="NSA_LOGLEVEL";
	}else{
		env_name="SAA_LOGLEVEL";
	}
	
	if( (lvl_name = getenv(env_name)) == NULL ){
		last_lvl_name = "debug4";
		strncpy(loglvlname,last_lvl_name,namesize);
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "%s Loglevel is %s",env_name,loglvlname );
		return;
	}
	last_lvl_name = "debug4";
	lvl_name_len = strlen(lvl_name);
	while(1){
		if(!strncmp(lvl_name,"off",lvl_name_len)){
			last_lvl_name = lvl_name;
			break;
		}
		else if(!strncmp(lvl_name,"write",lvl_name_len)){
			last_lvl_name = lvl_name;
			break;
		}
		else if(!strncmp(lvl_name,"error",lvl_name_len)){
			last_lvl_name = lvl_name;
			break;
		}
		else if(!strncmp(lvl_name,"warn",lvl_name_len)){
			last_lvl_name = lvl_name;
			break;
		}
		else if(!strncmp(lvl_name,"info",lvl_name_len)){
			last_lvl_name = lvl_name;
			break;
		}
		else if(!strncmp(lvl_name,"dev0",lvl_name_len)){
			last_lvl_name = lvl_name;
			break;
		}
		else if(!strncmp(lvl_name,"dev1",lvl_name_len)){
			last_lvl_name = lvl_name;
			break;
		}
		else if(!strncmp(lvl_name,"debug",strlen("debug"))){
			if(lvl_name_len > strlen("debug")){
				if(!strncmp(lvl_name,"debug2",lvl_name_len)){
					last_lvl_name = lvl_name;
					break;
				}
				else if(!strncmp(lvl_name,"debug3",lvl_name_len)){
					last_lvl_name = lvl_name;
					break;
				}
				else if(!strncmp(lvl_name,"debug4",lvl_name_len)){
					last_lvl_name = lvl_name;
					break;
				}
				else{
					;
				}
			}else{
				last_lvl_name = lvl_name;
			}
		}
		else{
			;
		}
		break;
	}
	strncpy(loglvlname,last_lvl_name,namesize);
	cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "%s Loglevel is %s",env_name,loglvlname );
	return;
}

/*!
 * @brief		map file create処理 
 * @note		map file create処理
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/12/04 FJT)Taniguchi Create
 */
static void f_ini_mp_log_create(CHAR* name , INT procNo)
{
	char cmd[256];
	int ret;
	
	memset(cmd,0,sizeof(cmd));
	snprintf(cmd,sizeof(cmd),"cat \"/proc/%d/status\" > /var/log/core.%s.%d.info.log",procNo,name,procNo);
	ret = system(cmd);
	memset(cmd,0,sizeof(cmd));
	snprintf(cmd,sizeof(cmd),"cat \"/proc/%d/maps\" >> /var/log/core.%s.%d.info.log",procNo,name,procNo);
	ret |= system(cmd);	/* pgr0539 */
	cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_INFO, "1st log create command result is %d",ret);
	return;
}



/*!
 * @brief		saa init2 file create処理 
 * @note		saa init2 file create処理
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/12/04 FJT)Taniguchi Create
 */
static void f_ini_mp_saa_nvm_data_create(CHAR* name,UINT ipadder)
{
	FILE*	fp;
	T_RRH_NETCONF_INFO_TBL*			netconf_info;
	T_RRH_DU_INFORMATION_TBL*		pf_info;
	T_RRH_DU_INFORMATION_EEPROM*	eep_tbl;
	T_RRH_DU_INFORMATION_FLASH*		vlan_info;
	T_RRH_DU_CONNECTION_INFO*		connection_tbl;
	int		ret;
	int		errcd;
	UINT	count;
	UINT	regdat, admin_state;
	struct in_addr my_ipadder;

	fp = fopen(name,"w");
	if(fp == NULL){
		cmn_com_assert_th_get(f_iniw_assert_main_p,D_RRH_LOG_AST_LV_ERROR, "saa_nvm_data file(%s) open error",name);
		return;
	}

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_T_RRH_NETCONF_INFO_TBL,			/* 共有メモリ番号		*/
						(VOID **)&netconf_info		,					/* 共有メモリアドレス	*/
						&errcd );										/* エラーコード			*/

	ret |= BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_DU_INFORMATION_TBL,				/* 共有メモリ番号		*/
						(VOID **)&pf_info		,						/* 共有メモリアドレス	*/
						&errcd );


	my_ipadder.s_addr = ipadder;
	if(ret != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x", errcd);

		fprintf(fp,"[o-ran-usermgmt]\n");
		fprintf(fp,"num_of_user=0\n");
		fprintf(fp,"\n");
		fprintf(fp,"[o-ran-mplane-int]\n");
		fprintf(fp,"num_of_seachable_vlan_id=0\n");
		fprintf(fp,"mplane-info.searchable-mplane-access-vlans-info.vlan-range.lowest-vlan-id=1\n");
		fprintf(fp,"mplane-info.searchable-mplane-access-vlans-info.vlan-range.highest-vlan-id=4094\n");
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-sub-interfaces.interface-name=0\n");
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-sub-interfaces.sub-interface=0\n");
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-sub-interfaces.client-info.mplane-ipv4-info.mplane-ipv4=%s\n",inet_ntoa(my_ipadder));
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-ssh-ports.call-home-ssh-port=%d\n",D_DU_DEFAULT_SSHPORTNO_CALLHOME);
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-ssh-ports.server-ssh-port=%d\n",D_DU_DEFAULT_SSHPORTNO);
	}
	else{
		UINT	count2;
		UINT	num_of_seachable_vlan_id;
		UCHAR	vlangrp;

		fprintf(fp,"[o-ran-usermgmt]\n");
		fprintf(fp,"num_of_user=%d\n",netconf_info->num_of_use);
		for(count=0;count < netconf_info->num_of_use;count++){
			fprintf(fp,"user.%d.name=%s\n",count,netconf_info->info[count].user);
			fprintf(fp,"user.%d.password=%s\n",count,netconf_info->info[count].password);
			fprintf(fp,"user.%d.enabled=%d\n",count,netconf_info->info[count].enabled);
		}
		fprintf(fp,"\n");

		eep_tbl = &pf_info->eeprom_info;
		vlan_info = &pf_info->flash_info;
		connection_tbl = &pf_info->du_connection_info;

		fprintf(fp,"[o-ran-mplane-int]\n");
		num_of_seachable_vlan_id=0;
		for(count=0;count < D_DU_SEARCHABLE_VLAN_MAX;count++){
			vlangrp = vlan_info->seachable_vlan_id[count];
			if(vlangrp == 0xFF){
				continue;
			}
			for(count2=0 ; count2 < 8 ;count2++){
				if(( vlangrp & ( 1 << (7 - count2) )) == 0){

					fprintf(fp,"mplane-info.searchable-mplane-access-vlans-info.searchable-access-vlans.%d=%d\n",
						num_of_seachable_vlan_id,
						(count*8) + count2
						);

					num_of_seachable_vlan_id++;
				}
			}
		}
		fprintf(fp,"num_of_seachable_vlan_id=%d\n",num_of_seachable_vlan_id);
		fprintf(fp,"mplane-info.searchable-mplane-access-vlans-info.vlan-range.lowest-vlan-id=%d\n",eep_tbl->vid_search_info.lowVid);
		fprintf(fp,"mplane-info.searchable-mplane-access-vlans-info.vlan-range.highest-vlan-id=%d\n",eep_tbl->vid_search_info.highVid);
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-sub-interfaces.interface-name=%d\n",connection_tbl->mp.port_no);
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-sub-interfaces.sub-interface=%d\n",connection_tbl->mp.mac.vid);
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-sub-interfaces.client-info.mplane-ipv4-info.mplane-ipv4=%s\n",inet_ntoa(my_ipadder));
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-ssh-ports.call-home-ssh-port=%d\n",netconf_info->callhome_ssh_port);
		fprintf(fp,"mplane-info.m-plane-interfaces.m-plane-ssh-ports.server-ssh-port=%d\n",netconf_info->server_ssh_port);
		fprintf(fp,"\n");

		fprintf(fp,"[o-ran-interfaces]\n");
		fprintf(fp,"interfaces.interface.mac-address=%02x:%02x:%02x:%02x:%02x:%02x\n", 
					eep_tbl->localMacAddress[0][0],
					eep_tbl->localMacAddress[0][1],
					eep_tbl->localMacAddress[0][2],
					eep_tbl->localMacAddress[0][3],
					eep_tbl->localMacAddress[0][4],
					eep_tbl->localMacAddress[0][5]);
		fprintf(fp,"\n");

	}
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV1INF, &regdat);
	switch( regdat & D_DU_SVCTL_BIT_ADM_ALL )
	{
		case D_DU_SVCTL_BIT_ADM_LOCKED :										/* admin状態がLock状態なら */
		case D_DU_SVCTL_BIT_ADM_LOCKED_LO :										/* admin状態がLock状態(local)なら */
			admin_state = E_MPSW_ADMIN_STATE_LOCKED ;
			f_iniw_ru_admin_state = "locked";
			break;
		case D_DU_SVCTL_BIT_ADM_DOWN:											/* admin状態がshutting-downなら */
			admin_state = E_MPSW_ADMIN_STATE_SHUTTINGDOWN ;
			f_iniw_ru_admin_state = "shutting-down";
			break;
		case D_DU_SVCTL_BIT_ADM_UNLOCKED:										/* admin状態がunlockedなら */
		case D_DU_SVCTL_BIT_ADM_UNLOCKED_LO:									/* admin状態がunlocked(local)なら */
			admin_state = E_MPSW_ADMIN_STATE_UNLOCKED ;
			f_iniw_ru_admin_state = "unlocked";
			break;
		default:																/* admin状態がその他なら */
			f_iniw_ru_admin_state = "unknown";
			admin_state = E_MPSW_ADMIN_STATE_UNKNOWN ;
			break;
	}

	UCHAR	hardware_name[D_DU_CARD_NAME_LEN];

	memset(hardware_name,0,sizeof(hardware_name));
	for(count=0;count < D_DU_CARD_NAME_LEN; count++){
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CARD_NAME + count, &hardware_name[count]);
		if( (hardware_name[count] < 0x21) || (hardware_name[count] > 0x7e) ){
			hardware_name[count] = 0;
			break;
		}
	}
	fprintf(fp,"[ietf-hardware]\n");
	fprintf(fp,"hardware.component.name=%s\n", hardware_name);
	fprintf(fp,"hardware.component.name.state.admin-state=%d\n", admin_state);
	fprintf(fp,"\n");

	fprintf(fp,"[o-ran-operations]\n");
	fprintf(fp,"operational-info.re-call-home-no-ssh-timer=%d\n", retry_interval);
	fprintf(fp,"\n");

	fflush(fp);
	fclose(fp);

	return;
}

/*!
 * @brief		startup-cfg file edit処理 
 * @note		startup-cfg file edit処理
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/07/12  FJT)Yoshida Create
 */
static void f_ini_mp_startup_cfg_edit( CHAR device_kind0,CHAR device_kind1 )
{
	FILE*							fp;
	char							*filename = NULL;

	T_RRH_DU_INFORMATION_TBL*		pf_info;
	T_RRH_DU_INFORMATION_EEPROM*	eep_tbl;
	T_RRH_DU_INFORMATION_FLASH*		vlan_info;
	T_RRH_DU_CONNECTION_INFO*		connection_tbl;

	USHORT							vlan_id;

	INT								endcd ;
	INT								errcd ;

	T_RRH_NETCONF_INFO_TBL*			netconf_info;
	UINT							count,count2;


	if(device_kind0 == D_DU_EEP_DEV_KIND_5GDU_SUB6){
		if(device_kind1 == D_DU_EEP_DEV_FREQ_45G){
			filename = "/etc/yumapro/startup-cfg_4.5G.xml";
		}else{
			filename = "/etc/yumapro/startup-cfg_3.7G.xml";
		}
	}else{
		filename = "/etc/yumapro/startup-cfg_28G.xml";
	}

	if( (fp = fopen( filename, "a+" )) == NULL ) {
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_ERROR, "%s file open error", filename);
		return;
	}

	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_T_RRH_NETCONF_INFO_TBL,			/* 共有メモリ番号		*/
						(VOID **)&netconf_info,						/* 共有メモリアドレス	*/
						&errcd );										/* エラーコード			*/

	endcd |= BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_DU_INFORMATION_TBL,	/* 共有メモリ番号		*/
										(VOID **)&pf_info		,				/* 共有メモリアドレス	*/
										&errcd );

	if( endcd != BPF_RU_IPCM_OK ) {
		cmn_com_assert_th_get( f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, 
							   "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x, errcd=%08x",
							   endcd, errcd);
		fclose(fp);
		return;
	}

	/** O-RAN MPLANE INT **************************************************************************/
	vlan_info = &pf_info->flash_info;
	eep_tbl = &pf_info->eeprom_info;
	connection_tbl = &pf_info->du_connection_info;

				fprintf(fp,"<!-- O-RAN MPLANE INT -->\n" );
				fprintf(fp,"<mplane-info xmlns=\"urn:o-ran:mplane-interfaces:1.0\">\n" );
				fprintf(fp," <searchable-mplane-access-vlans-info>\n" );

	vlan_id = 0;
	for( count = 0; count < D_DU_SEARCHABLE_VLAN_MAX; count++ ) {
		if(vlan_info->seachable_vlan_id[count] == 0xFF ){
			vlan_id += 8;
			continue;
		}
		for( count2 = 7; ; count2-- ) {
			if( ((vlan_info->seachable_vlan_id[count] >> count2) & 0x01) != 0x01 ) {
				fprintf(fp,"  <searchable-access-vlans>%d</searchable-access-vlans>\n", vlan_id );
			}
			vlan_id++ ;
			if(count2 == 0){
				break;
			}
		}
	}

				fprintf(fp,"  <vlan-range>\n" );
				fprintf(fp,"   <lowest-vlan-id>%u</lowest-vlan-id>\n"  ,(UINT)(eep_tbl->vid_search_info.lowVid));
				fprintf(fp,"   <highest-vlan-id>%u</highest-vlan-id>\n",(UINT)(eep_tbl->vid_search_info.highVid));
				fprintf(fp,"  </vlan-range>\n" );
				fprintf(fp," </searchable-mplane-access-vlans-info>\n" );
				fprintf(fp," <m-plane-interfaces>\n" );
				fprintf(fp,"  <m-plane-sub-interfaces>\n" );
				fprintf(fp,"   <interface-name>m-vlan%u</interface-name>\n",(UINT)(connection_tbl->mp.port_no));
				fprintf(fp,"   <sub-interface>%u</sub-interface>\n"        ,(UINT)(connection_tbl->mp.mac.vid));
				fprintf(fp,"  </m-plane-sub-interfaces>\n");
				fprintf(fp,"  <m-plane-ssh-ports>\n");
				fprintf(fp,"   <call-home-ssh-port>%u</call-home-ssh-port>\n",(USHORT)(netconf_info->callhome_ssh_port));
				fprintf(fp,"   <server-ssh-port>%u</server-ssh-port>\n",(USHORT)(netconf_info->server_ssh_port));
				fprintf(fp,"  </m-plane-ssh-ports>\n");
				fprintf(fp," </m-plane-interfaces>\n");
				fprintf(fp,"</mplane-info>\n");

	/** O-RAN OPERATIONS **************************************************************************/
				fprintf(fp,"<!-- O-RAN OPERATIONS -->\n");
				fprintf(fp,"<operational-info xmlns=\"urn:o-ran:operations:1.0\">\n");
				fprintf(fp," <re-call-home-no-ssh-timer>%u</re-call-home-no-ssh-timer>\n",(USHORT)retry_interval);
				fprintf(fp,"</operational-info>\n");

	/** O-RAN USER ********************************************************************************/
				fprintf(fp,"<!-- O-RAN USER -->\n");
				fprintf(fp,"<users xmlns=\"urn:o-ran:user-mgmt:1.0\">\n");
	if(netconf_info->num_of_use == 0){
				fprintf(fp," <user>\n");
				fprintf(fp,"  <name>oranuser</name>\n");
				fprintf(fp,"  <password>o-ran-password</password>\n");
				fprintf(fp,"  <enabled>true</enabled>\n");
				fprintf(fp," </user>\n");
	}
	else{
		for(count=0;count < netconf_info->num_of_use;count++){
				fprintf(fp," <user>\n");
				fprintf(fp,"  <name>%s</name>\n",netconf_info->info[count].user);
				fprintf(fp,"  <password>%s</password>\n",netconf_info->info[count].password);
			if(netconf_info->info[count].enabled == 0){
				fprintf(fp,"  <enabled>false</enabled>\n");
			}else{
				fprintf(fp,"  <enabled>true</enabled>\n");
			}
				fprintf(fp," </user>\n");
		}
	}
				fprintf(fp,"</users>\n");

	/***********************************************************************************************/
	fprintf(fp,"</config>\n" );
	fflush(fp);
	fclose(fp);

	return;
}

static void f_ini_mp_startup_cfg_sed(CHAR device_kind0,CHAR device_kind1)
{
	char							cmd[256];
	char*							cfg_name = NULL;
	T_RRH_DU_INFORMATION_TBL*		pf_info = NULL;
	T_RRH_DU_INFORMATION_EEPROM*	eep_tbl = NULL;
	T_RRH_DU_CONNECTION_INFO*		connection_tbl;
	int								ret;
	int								errcd;

	if(device_kind0 == D_DU_EEP_DEV_KIND_5GDU_SUB6){
		if(device_kind1 == D_DU_EEP_DEV_FREQ_45G){
			cfg_name = "/etc/yumapro/startup-cfg_4.5G.xml";
		}else{
			cfg_name = "/etc/yumapro/startup-cfg_3.7G.xml";
		}
	}else{
		cfg_name = "/etc/yumapro/startup-cfg_28G.xml";
	}

	ret = BPF_RU_IPCM_PROCSHM_ADDRGET( 
						E_RRH_SHMID_APL_DU_INFORMATION_TBL,				/* 共有メモリ番号		*/
						(VOID **)&pf_info		,						/* 共有メモリアドレス	*/
						&errcd );

	if(ret == BPF_RU_IPCM_OK){
		eep_tbl = &pf_info->eeprom_info;
		memset(cmd,0,sizeof(cmd));

		/* RU_MAC_ADDRを実体に置き換える */
		snprintf(cmd,sizeof(cmd)-1,"sed -i 's/RU_MAC_ADDR/%02x:%02x:%02x:%02x:%02x:%02x/' %s",
				eep_tbl->localMacAddress[0][0],
				eep_tbl->localMacAddress[0][1],
				eep_tbl->localMacAddress[0][2],
				eep_tbl->localMacAddress[0][3],
				eep_tbl->localMacAddress[0][4],
				eep_tbl->localMacAddress[0][5],
				cfg_name
		);

		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] %s",cmd);
		if((ret = system((const char *)cmd)) != 0 ){
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "[f_ini_main_con] sed warn %d %d",ret,errno);
		}

		memset(cmd,0,sizeof(cmd));

		/* RU_ADMINを実体に置き換える */
		snprintf(cmd,sizeof(cmd)-1,"sed -i 's/RU_ADMIN/%s/' %s",f_iniw_ru_admin_state,cfg_name);
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] %s",cmd);
		if((ret = system((const char *)cmd)) != 0 ){
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "[f_ini_main_con] sed warn %d %d",ret,errno);
		}


		connection_tbl = &pf_info->du_connection_info;
		memset(cmd,0,sizeof(cmd));
		/* RU_MP_VLAN_IDを実体に置き換える */
		snprintf(cmd,sizeof(cmd)-1,"sed -i 's/RU_MP_VLAN_ID/%u/' %s",connection_tbl->mp.mac.vid,cfg_name);
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] %s",cmd);
		if((ret = system((const char *)cmd)) != 0 ){
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "[f_ini_main_con] sed warn %d %d",ret,errno);
		}


		memset(cmd,0,sizeof(cmd));
		/* RU_MP_PCPを実体に置き換える */
		snprintf(cmd,sizeof(cmd)-1,"sed -i 's/RU_MP_PCP/%u/' %s",connection_tbl->mp.mac.pcp,cfg_name);
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] %s",cmd);
		if((ret = system((const char *)cmd)) != 0 ){
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "[f_ini_main_con] sed warn %d %d",ret,errno);
		}

		unsigned char	domain_num = 0;
		BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_VID_DOMAIN_NO, &domain_num );
		if( (domain_num < 0x18) || ( domain_num > 0x2B ) ) {
			domain_num = 0x18;
		}

		memset(cmd,0,sizeof(cmd));
		/* RU_PTP_DOMAINを実体に置き換える */
		snprintf(cmd,sizeof(cmd)-1,"sed -i 's/RU_PTP_DOMAIN/%u/' %s",domain_num,cfg_name);
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] %s",cmd);
		if((ret = system((const char *)cmd)) != 0 ){
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "[f_ini_main_con] sed warn %d %d",ret,errno);
		}

		unsigned char	mac_ptn = 0;
		char*	multicast_name;
		BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_VID_MULTI_MACADDR, &mac_ptn );
		if( mac_ptn == 1 ) {
			multicast_name = "NONFORWARDABLE";
		}else{
			multicast_name = "FORWARDABLE";
		}

		memset(cmd,0,sizeof(cmd));
		/* RU_PTP_MULTICAST_MACADDRを実体に置き換える */
		snprintf(cmd,sizeof(cmd)-1,"sed -i 's/RU_PTP_MULTICAST_MACADDR/%s/' %s",multicast_name,cfg_name);
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] %s",cmd);
		if((ret = system((const char *)cmd)) != 0 ){
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "[f_ini_main_con] sed warn %d %d",ret,errno);
		}

	}else{
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_ERROR, "[f_ini_main_con] sed error %d %d",ret,errcd);
	}
	return;
}


static void f_ini_mp_netconfd_cfg_sed(UINT ipadder)
{
	char							cmd[256];
	struct in_addr 					my_ipadder;
	int								ret;

	memset(cmd,0,sizeof(cmd));
	my_ipadder.s_addr = ipadder;

	/* RU_IP_ADDRESSを実体に置き換える */
	snprintf(cmd,sizeof(cmd)-1,"sed -i 's/RU_IP_ADDRESS/%s/' /etc/yumapro/netconfd-pro.conf",inet_ntoa(my_ipadder));
	cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] %s",cmd);
	if((ret = system((const char *)cmd)) != 0 ){
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "[f_ini_main_con] sed warn %d %d",ret,errno);
	}
	return;
}




/*!
 * @brief		関数機能概要:CONスレッドメイン。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note		関数処理内容.
 *				-# 状態を"初期状態"に設定する
 *				-# 初期化関数をCallする(f_log_init())
 *				-# MSG待ち無限Loop処理へ移行する
 *					-# MSG受信関数をCallする(BPF_RU_IPCM_MSGQ_RECV())
 *					-# 受信メッセージ解析関数をCallする(f_log_anlz())
 * @retval		none
 * @return		none
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/11/23 FJT)吉田 Create
 */
VOID f_ini_main_con(VOID)
{
	VOID							*pMsg;
	UINT							rcvSize;
	INT								ret;
	INT								errcd;

	UCHAR							Flag;
	T_RRH_DU_INFORMATION_TBL		*du_info_tbl_adr;				/**< DU情報テーブル		*/

	INT								rtn_bpf = D_RRH_OK;
	INT								rtn_api = D_RRH_OK;

	T_MSI_APP_LIST Nsa_pList;
	T_MSI_APP_LIST Saa_pList;

	CHAR	Nsa_pram[16][128];
	CHAR	Saa_pram[16][128];
	CHAR*	Nsa_prams[17];
	CHAR*	Saa_prams[17];

	pid_t	Nsa_Pid = -1;
	pid_t	Saa_Pid = -1;
	union sigval	value;

	UINT	arg_index;
	INT		arg_len;

	UCHAR	device_kind0;
	UCHAR	device_kind1;
	
	CHAR*	argv_name;

	struct in_addr ipaddr;

	USHORT	sshportNo;
	UCHAR	retry_interval_eep[2] = {0, 0};

	CHAR	loglevels[32];
	sigset_t sigSetVal;
	FILE* tmpfp = NULL;
	long filelen;
	CHAR	startup_conf_set[256];

	f_iniw_assert_con_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"Init con thread");
	
	cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_main_con] ENTER" );

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "ini_con", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_INI,				/* ProcessID		*/
									D_SYS_THDID_INIT_CON,			/* ThreadID			*/
									D_SYS_THDQID_INI_CON,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self());

	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_MP_CONNECT_FLAG, (UCHAR*)&Flag );
	if(Flag == D_RRH_ON ){
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "M-plane Skip, M-plane Mode was Disable." );
	}
	else{
		ret = system("cmd_planelogstart 1>/dev/null 2> /dev/null");	/* pgr0539 */
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "M-plane Packet capture start %d",ret);	/* pgr0539 */

		/* DHCP有無フラグチェック	*/
		BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_DHCP_ENT_FLAG, (UCHAR*)&Flag );
		if( Flag != D_RRH_ON ) {
	
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] DHCP Procedure Start" );
	
			/* DHCP起動パラメータ取得	*/
			rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET( 
								E_RRH_SHMID_APL_DU_INFORMATION_TBL,				/* 共有メモリ番号		*/
								(VOID **)&du_info_tbl_adr,						/* 共有メモリアドレス	*/
								&errcd );										/* エラーコード			*/
			if(rtn_bpf != BPF_RU_IPCM_OK)
			{
				cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "BPF_RU_IPCM_PROCSHM_ADDRGET error. endcd=%08x", errcd);
				cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "[f_ini_main_con] DHCP Procedure Failed!!!" );
			} else {
				rtn_api = duApi_Mpn_Mnr_MplaneReady( D_RRH_PROCQUE_API_MPLANE_READY,/* Process Que ID  */
													 0,									/* wait time  */
													 0,									/* buffer addr  */
													 0xFFFF,							/* port_no  */
													 0x0003,							/* Flag  */
													 du_info_tbl_adr->eeprom_info.vid_search_info.lowVid,							/* lowest-vlan-id  */
													 du_info_tbl_adr->eeprom_info.vid_search_info.highVid,							/* hoghest-vlan-id  */
													 du_info_tbl_adr->eeprom_info.vid_search_info.lastUsedVid);						/* scan start vlan-id  */
	
				if(rtn_api != D_RRH_OK)
				{
					cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "duApi_Mpn_Mnr_MplaneReady error. endcd=%08x", rtn_api);
					cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_WARNING, "[f_ini_main_con] DHCP Procedure Failed!!!" );
				}
	
				while(1)
				{
					/********************************************************************************/
					/* メッセージ受信処理															*/
					/********************************************************************************/
					rtn_bpf = BPF_RU_IPCM_MSGQ_RECV(
								D_RRH_PROCQUE_API_MPLANE_READY,	/* 受信MSGキューID			*/
								0,								/* 受信待ち時間				*/
								(VOID**)&pMsg,					/* メッセージ受信バッファ	*/
								&rcvSize	);					/* 受信Messageサイズ		*/
	
					cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] NETCONF Start");
	
					rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND0, &device_kind0);
					rtn_bpf += BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND1, &device_kind1);
					if(rtn_bpf != BPF_HM_DEVC_COMPLETE){
						cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_ERROR, "[f_ini_main_con] EEPROM read error");
						break;
					}

					/* パラメータ初期化	*/
					memset(&Nsa_pList,0,sizeof(Nsa_pList));
					memset(&Saa_pList,0,sizeof(Saa_pList));
					memset(Nsa_pram,0,sizeof(Nsa_pram));
					memset(Saa_pram,0,sizeof(Saa_pram));

					/* re-call-home-no-ssh-timer初期設定	*/
					rtn_bpf  = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CALLHOME_SSH_TIMER_H, &retry_interval_eep[0]);
					rtn_bpf |= BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CALLHOME_SSH_TIMER_L, &retry_interval_eep[1]);
					if(rtn_bpf != BPF_HM_DEVC_COMPLETE){
						cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_ERROR, "[f_ini_main_con] EEPROM read error");
					} else {
						retry_interval = (retry_interval_eep[0] << 8) | retry_interval_eep[1];
						if( (retry_interval == 0) || (retry_interval == 0xFFFF) ) {
							retry_interval = D_RRH_CALLHOME_TIMER_DFAULT;
						}
					}

					f_ini_mp_startup_cfg_sed(device_kind0,device_kind1);
					f_ini_mp_startup_cfg_edit(device_kind0,device_kind1);

					f_ini_mp_netconfd_cfg_sed(htonl(((T_API_MPLANE_READY_RSP*)pMsg)->data.localIpAddress));

					for(arg_index=0 ; f_inir_Saa_pram[arg_index] != NULL ; arg_index++){
						arg_len = strlen(f_inir_Saa_pram[arg_index]);
						if(arg_len > 0){
							memcpy(Saa_pram[arg_index],f_inir_Saa_pram[arg_index],arg_len);
						}
						switch(arg_index){
							case 1:
								if(device_kind0 == D_DU_EEP_DEV_KIND_5GDU_SUB6){
									if(device_kind1 == D_DU_EEP_DEV_FREQ_45G){
										argv_name = "4.5G";
									}
									else{
										argv_name ="3.7G";
									}
								}
								else{
										argv_name ="28G";
								}
								snprintf(&(Saa_pram[arg_index][0]),sizeof(Saa_pram[arg_index]),"%s",argv_name);
								break;
							case 2:
								f_ini_mp_saa_nvm_data_create(f_inir_Saa_pram[arg_index],htonl(((T_API_MPLANE_READY_RSP*)pMsg)->data.remoteIpAdress));
								break;
							case 3:
								if(device_kind0 == D_DU_EEP_DEV_KIND_5GDU_SUB6){
									if(device_kind1 == D_DU_EEP_DEV_FREQ_45G){
										argv_name = "/etc/yumapro/startup-cfg_4.5G.xml";
									}
									else{
										argv_name ="/etc/yumapro/startup-cfg_3.7G.xml";
									}
								}
								else{
										argv_name ="/etc/yumapro/startup-cfg_28G.xml";
								}
								snprintf(&(Saa_pram[arg_index][arg_len]),sizeof(Saa_pram[arg_index]),"%s",argv_name);
								break;
							case 4:
								argv_name ="/etc/yumapro/netconfd-pro.conf";
								snprintf(&(Saa_pram[arg_index][arg_len]),sizeof(Saa_pram[arg_index]),"%s",argv_name);
								break;
							case 6:
								memset(loglevels,0,sizeof(loglevels));
								f_ini_main_loglevel(SAA_LOGLEVEL,sizeof(loglevels)-1 ,loglevels );
								snprintf(&(Saa_pram[arg_index][arg_len]),sizeof(Saa_pram[arg_index]),"%s",loglevels);
								break;
							default:
								break;
						}
						Saa_prams[arg_index] = &Saa_pram[arg_index][0];
					}
					Saa_prams[arg_index] = NULL;
					Saa_pList.startPrg = (const char*)Saa_pram[0];
					Saa_pList.startOpt = Saa_prams;
					Saa_pList.cpus = f_inir_procCreMng_mp[0].procCpu;
					Saa_pList.policy = SCHED_FIFO;
					Saa_pList.priority = f_inir_procCreMng_mp[0].procPriority;
	
					rtn_bpf = BPF_HM_DEVC_COMPLETE;

					for(arg_index=0 ; f_inir_Nsa_pram[arg_index] != NULL ; arg_index++){
						arg_len = strlen(f_inir_Nsa_pram[arg_index]);
						memcpy(Nsa_pram[arg_index],f_inir_Nsa_pram[arg_index],arg_len);
						switch(arg_index){
							case 1:
								if(device_kind0 == D_DU_EEP_DEV_KIND_5GDU_SUB6){
									if(device_kind1 == D_DU_EEP_DEV_FREQ_45G){
										argv_name = "/etc/yumapro/startup-cfg_4.5G.xml";
									}
									else{
										argv_name ="/etc/yumapro/startup-cfg_3.7G.xml";
									}
								}
								else{
										argv_name ="/etc/yumapro/startup-cfg_28G.xml";
								}
								snprintf(&(Nsa_pram[arg_index][arg_len]),sizeof(Nsa_pram[arg_index])-arg_len,"%s",argv_name);

								memset(startup_conf_set,0,sizeof(startup_conf_set));
								snprintf(startup_conf_set,sizeof(startup_conf_set),"ln -s %s /var/log/startup-cfg.xml",argv_name);
								ret = system(startup_conf_set);
								cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "Set conf link(%d):%s",ret,startup_conf_set);	/* pgr0539 */

								break;
							case 2:
								sshportNo = D_DU_DEFAULT_SSHPORTNO_CALLHOME;	/* 固定値設定(EEPROM値は使用しない)	*/
								ipaddr.s_addr=htonl(((T_API_MPLANE_READY_RSP*)pMsg)->data.remoteIpAdress);
								snprintf(&(Nsa_pram[arg_index][arg_len]),sizeof(Nsa_pram[arg_index])-arg_len,"MplaneServer@%s:%d",inet_ntoa(ipaddr),sshportNo);
								break;
							case 3:
								argv_name ="/etc/yumapro/netconfd-pro.conf";
								snprintf(&(Nsa_pram[arg_index][arg_len]),sizeof(Nsa_pram[arg_index])-arg_len,"%s",argv_name);
								break;
							case 5:
								memset(loglevels,0,sizeof(loglevels));
								f_ini_main_loglevel(NSA_LOGLEVEL,sizeof(loglevels)-1 ,loglevels );
								snprintf(&(Nsa_pram[arg_index][arg_len]),sizeof(Nsa_pram[arg_index]),"%s",loglevels);
								break;
							case 6:
								snprintf( &(Nsa_pram[arg_index][arg_len]), sizeof(Nsa_pram[arg_index]), "%d", retry_interval );
								break;
							default:
								break;
						}
						Nsa_prams[arg_index] = &Nsa_pram[arg_index][0];
					}
					Nsa_prams[arg_index] = NULL;
					Nsa_pList.startPrg = (const char*)Nsa_pram[0];
					Nsa_pList.startOpt = Nsa_prams;
					Nsa_pList.cpus = f_inir_procCreMng_mp[1].procCpu;
					Nsa_pList.policy = SCHED_FIFO;
					Nsa_pList.priority = f_inir_procCreMng_mp[1].procPriority;
	
	
					sigemptyset( &sigSetVal );
					(void)sigaddset( &sigSetVal, SIGUSR1 );
					(void)sigaddset( &sigSetVal, SIGUSR2 );
					(void)sigaddset( &sigSetVal, SIGCHLD );
					(void)pthread_sigmask(SIG_UNBLOCK, &sigSetVal, NULL);

					rtn_bpf = msi_app_initialize(NULL,&Saa_pList,0,bpf_com_plog_trace_mp,NULL,&Saa_Pid);
					if(rtn_bpf != MSI_FUNCIF_OK){
						cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_ERROR, "MPlane SAA Error(msi_app_initialize error)");
						break;
					}
	
					procIdTbl->mpsw_count = 0;
					procIdTbl->mpsw_info[ procIdTbl->mpsw_count ].aplProcessID  = f_inir_procCreMng_mp[0].procNo;
					procIdTbl->mpsw_info[ procIdTbl->mpsw_count ].realProcessID = Saa_Pid;
					procIdTbl->mpsw_info[ procIdTbl->mpsw_count ].PQID = 0;

					f_ini_mp_log_create("SAA",Saa_Pid);
					procIdTbl->mpsw_count++;

					f_ini_main_con_send_mpstart();
					
					rtn_bpf = msi_app_initialize(&Nsa_pList,NULL,0,bpf_com_plog_trace_mp,&Nsa_Pid,NULL);
					if(rtn_bpf != MSI_FUNCIF_OK){
						cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_ERROR, "MPlane NSA Error(msi_app_initialize error)");
						break;
					}

					procIdTbl->mpsw_info[ procIdTbl->mpsw_count ].aplProcessID  = f_inir_procCreMng_mp[1].procNo;
					procIdTbl->mpsw_info[ procIdTbl->mpsw_count ].realProcessID = Nsa_Pid;
					procIdTbl->mpsw_info[ procIdTbl->mpsw_count ].PQID = 0;

					f_ini_mp_log_create("NSA",Nsa_Pid);

					procIdTbl->mpsw_count++;
	
					cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] NETCONF complete!! SAA:%d NSA:%d",Saa_Pid,Nsa_Pid);

					sigemptyset( &sigSetVal );
					(void)sigaddset( &sigSetVal, SIGUSR1 );
					(void)sigaddset( &sigSetVal, SIGUSR2 );
					(void)sigaddset( &sigSetVal, SIGCHLD );	
					(void)pthread_sigmask(SIG_BLOCK, &sigSetVal, NULL);
	
					break;
				}
			}
		} else {
	
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_main_con] DHCP Procedure not Start" );
		}
	}

	/********************************************************************************************/
	/* LOG rotate 監視																			*/
	/********************************************************************************************/
	value.sival_int = 0;
	if((procIdTbl->mpsw_count > 0) && (Nsa_Pid != -1)){
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "log rotate start" );
		while(1){
			sleep(NSA_LOGFILE_ROTATE_TIME);
			if((tmpfp = fopen(nsa_log_file_name,"r")) == NULL){
				cmn_com_assert_th_get(f_iniw_assert_con_p,D_RRH_LOG_AST_LV_INFO, "fopen NG(%d)",errno);
				continue;
			}
			fseek(tmpfp, 0, SEEK_END);
			filelen = ftell(tmpfp);
			fclose(tmpfp);
			if( filelen >= NSA_LOGFILE_ROTATE_SIZE ){
				if(nsa_log_file_rotate_flag == D_RRH_OFF){
					if( rename(nsa_log_file_name,nsa_log_file_name_stup) == 0 ){
						nsa_log_file_rotate_flag = D_RRH_ON;
					}else{
						cmn_com_assert_th_get(f_iniw_assert_con_p,D_RRH_LOG_AST_LV_ERROR, "start up rename NG(%d)",errno);
					}
				}else{
					if(nsa_log_file_rotate_flag == D_RRH_ON){
						nsa_log_file_rotate_flag++;
					}else{
						if( remove(nsa_log_file_name_old) != 0 ){
							cmn_com_assert_th_get(f_iniw_assert_con_p,D_RRH_LOG_AST_LV_ERROR, "remove NG(%d)",errno);
						}
					}
					if( rename(nsa_log_file_name,nsa_log_file_name_old) != 0 ){
						cmn_com_assert_th_get(f_iniw_assert_con_p,D_RRH_LOG_AST_LV_ERROR, "old rename NG(%d)",errno);
					}
				}
				rtn_bpf = sigqueue(Nsa_Pid,SIGUSR2,value);	/* log rotate */
				if(rtn_bpf != 0){
					cmn_com_assert_th_get(f_iniw_assert_con_p,D_RRH_LOG_AST_LV_ERROR, "SIGUSR2 NG(%d)",errno);
					break;
				}
			}
		}
	}else{
		cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_INFO, "log rotate skip" );
	}
	/********************************************************************************************/
	/* メッセージ受信待ち																		*/
	/********************************************************************************************/
	while(1)
	{
		/****************************************************************************************/
		/* メッセージ受信																		*/
		/****************************************************************************************/
		rtn_bpf = BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_INI_CON,			/* 受信MSGキューID			*/
					0,								/* 受信待ち時間				*/
					(VOID**)&pMsg,					/* メッセージ受信バッファ	*/
					&rcvSize	);					/* 受信Messageサイズ		*/

		if( rtn_bpf != BPF_RU_IPCM_OK )
		{
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_ERROR, "[f_ini_main_con] RECEIVE" );
			break;
		}

		/* 受信バッファ解放	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)pMsg );
		if( ret != 0 )
		{
			cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_ERROR, "[f_ini_main_con] FREE" );
			break;
		}
	}
	cmn_com_assert_th_get(f_iniw_assert_con_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_main_con] RETURN" );
	
	return;
}

/* @} */

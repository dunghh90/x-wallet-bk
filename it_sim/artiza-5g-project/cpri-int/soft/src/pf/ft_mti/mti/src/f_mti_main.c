/*!
 * @skip  $ld:$
 * @file  f_mti_main.c
 * @brief pf_mti メイン処理
 * @date  2019/05/13 Fujitsu)Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_MTI
 * @{
 */

#include <stdarg.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "f_mti_inc.h"


enum {
	E_MTI_PRODUCT_TYPE_37G = 0,
	E_MTI_PRODUCT_TYPE_45G,
	E_MTI_PRODUCT_TYPE_280G,
};

#define D_MTI_BEAMID_MAX_MMW		45
#define D_MTI_BEAMID_MAX_SUB6		7

T_RRH_MT_INFO*			f_mtiw_InfoTbl = NULL;
static UCHAR			f_mtiw_recvData[16*1024];		/* 16K */
static UCHAR			f_mtiw_sendData[16*1024];		/* 16K */
UCHAR					f_mtiw_productName[12];
UCHAR					f_mtiw_softVer[2];
UCHAR					f_mtiw_hardVer[2];
UINT					f_mtiw_productType;
UINT					f_mtiw_beamIdMax = 0;
INT						f_mtiw_sd_cli = -1;
UINT					f_mtiw_optdef_val = 0;
UINT					f_mtiw_opt_val = 0;

/*!
 * @brief 関数機能概要:pf_mti log
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
VOID f_mti_mtLog(const char* Message_p, ... )
{
	T_RRH_LOG_MT_HIS_RECORD		*logp;
	UINT						index;
	va_list args;
	
	/* Check message */
	if(Message_p == NULL)
	{
		return;
	}
	index = f_mtiw_InfoTbl->logInfo.index;
	logp = &(f_mtiw_InfoTbl->logInfo.tRecord[index]);
	
	logp->date.year = 0;
	/* set message 			*/
	memset(logp->message, 0, sizeof(logp->message));

	va_start(args, Message_p);	/* pgr0039 */
	(VOID)vsnprintf((char*)logp->message, sizeof(logp->message), Message_p, args);
	va_end(args);

	/* Get System time			*/
	(VOID)BPF_HM_DEVC_GETTIME(&(logp->date));

	/*	update index	*/
	f_mtiw_InfoTbl->logInfo.index = (UINT)((index + 1) % D_DU_MTLOG_RCD_NUM);
	
	return;
}


/*!
 * @brief 関数機能概要:pf_mti init
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT f_mti_init( VOID )
{
	INT					sd;
	struct ifreq		ifr;
	INT					ret = D_SYS_NG;
	UINT				count;
	UINT				count2;
	UINT				oper_state;
	UCHAR				eepVal;

	/* スレッド内情報初期化 */
	printf ("\nThis is file  %s  line %d f_mti_init f_cmw_tra_inv_tbl->cmx_device_BW = %x",__FILE__,__LINE__,f_cmw_tra_inv_tbl->cmx_device_BW);//C9 add code debug
	switch(f_cmw_tra_inv_tbl->cmx_device_BW){
		case D_DU_EEP_DEV_FREQ_37G:
			f_mtiw_productType  = (UINT)E_MTI_PRODUCT_TYPE_37G;
			f_mtiw_beamIdMax = D_MTI_BEAMID_MAX_SUB6;
			f_mtiw_optdef_val |= D_MTI_OPTION_FAN_SPIN_MODE_ON;
			break;
		case D_DU_EEP_DEV_FREQ_45G:
			f_mtiw_productType  = (UINT)E_MTI_PRODUCT_TYPE_45G;
			f_mtiw_beamIdMax = D_MTI_BEAMID_MAX_SUB6;
			f_mtiw_optdef_val |= D_MTI_OPTION_FAN_SPIN_MODE_ON;
			break;
		default:
			f_mtiw_productType  = (UINT)E_MTI_PRODUCT_TYPE_280G;
			f_mtiw_beamIdMax = D_MTI_BEAMID_MAX_MMW;
			f_mtiw_optdef_val |= D_MTI_OPTION_FAN_SPIN_MODE_ON;
			f_mtiw_optdef_val |= D_MTI_OPTION_BF_ENVELOPE_MODE_ON;
			break;
	}
	printf ("\nThis is file  %s  line %d f_mti_init f_cmw_tra_inv_tbl->cmx_device_BW = %x",__FILE__,__LINE__,f_cmw_tra_inv_tbl->cmx_device_BW);//C9 add code debug

	for(count=0 , count2=0 ; count < (sizeof(f_cmw_tra_inv_tbl->cmx_cardname) /sizeof(f_cmw_tra_inv_tbl->cmx_cardname[0])) ;count++){
		f_mtiw_productName[count2] = (UCHAR)((f_cmw_tra_inv_tbl->cmx_cardname[count] & 0xFF00) >> 8);
		count2++;
		f_mtiw_productName[count2] = (UCHAR)(f_cmw_tra_inv_tbl->cmx_cardname[count] & 0xFF);
		count2++;
	}
	printf ("\nThis is file  %s  line %d f_mti_init f_cmw_tra_inv_tbl->cmx_bootup_face = %x",__FILE__,__LINE__,f_cmw_tra_inv_tbl->cmx_bootup_face);//C9 add code debug

	if ( f_cmw_tra_inv_tbl->cmx_bootup_face == D_RRH_BOOT_ACT0 ){
		f_mtiw_softVer[0] = (UCHAR)((f_cmw_tra_inv_tbl->cmx_f0_sver[0] & 0xFF00) >> 8);
		f_mtiw_softVer[1] = (UCHAR)(f_cmw_tra_inv_tbl->cmx_f0_sver[0] & 0xFF);
	}else{
		f_mtiw_softVer[0] = (UCHAR)((f_cmw_tra_inv_tbl->cmx_f1_sver[0] & 0xFF00) >> 8 );
		f_mtiw_softVer[1] = (UCHAR)(f_cmw_tra_inv_tbl->cmx_f1_sver[0] & 0xFF);
	}

	f_mtiw_hardVer[0] = (UCHAR)(f_cmw_tra_inv_tbl->cmx_hver1 & 0xFF);			/* Major */
	f_mtiw_hardVer[1] = (UCHAR)((f_cmw_tra_inv_tbl->cmx_hver1 & 0xFF00) >> 8);	/* Minor */

	f_mti_mtLog("Option information set start... ");
	/* FAN回転数モード	*/
	if( (f_mtiw_optdef_val & D_MTI_OPTION_FAN_SPIN_MODE_ON) == D_MTI_OPTION_FAN_SPIN_MODE_ON ) {
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_FAN_SPIN_MODE, &eepVal);
		if( eepVal == D_RRH_ON ) {	/* 低速モード	*/
			f_mtiw_opt_val    |= D_MTI_OPTION_FAN_SPIN_MODE_ON;
		}
	}
	/* mmW BeamID 新旧モード	*/
	if( (f_mtiw_optdef_val & D_MTI_OPTION_BF_ENVELOPE_MODE_ON) == D_MTI_OPTION_BF_ENVELOPE_MODE_ON ) {
		BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_BF_ENVELOPE_MODE, &eepVal);
		if( eepVal == D_RRH_ON ) {	/* 新Beam動作モード	*/
			f_mtiw_optdef_val |= D_MTI_OPTION_BEAMID_POSITION_NEW;
			f_mtiw_opt_val    |= D_MTI_OPTION_BF_ENVELOPE_MODE_ON;
		}
	}
	f_mti_mtLog("   - Support option : %08X", f_mtiw_optdef_val);
	f_mti_mtLog("   - Initial Value  : %08X", f_mtiw_opt_val);

	/* Operational=enableまで待つ */
	f_mti_mtLog("Oper-state check start... ");
	count = 0;
	while(1){
		f_com_taskDelay(1000,0,0,0);
		count++;
		oper_state = 0;
		BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV1INF, D_DU_SVCTL_BIT_OPE_ENABLE , &oper_state); 
		if(oper_state != 0){
			f_mti_mtLog("Oper-state was enabled");
			break;
		}
		if(count >= 180){
			f_mti_mtLog("Oper-state check timeout.");
			break;
		}
	}

	/* Etherが活性化されるまで待つ */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Local MT port Ether check start... ");
	f_mti_mtLog("Local MT port Ether check start... ");
	while(1){
		errno = 0;
		if (ioctl(sd, SIOCGIFFLAGS, &ifr) < 0){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Local MT port Ether ioctl error(%d %s) ", errno,ifr.ifr_name);
			f_mti_mtLog("Local MT port Ether ioctl error... ");
			break;
		}

		if(( ifr.ifr_flags & IFF_UP ) != 0 ){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Local MT port Ether UP(0x%x) ", ifr.ifr_flags);
			f_mti_mtLog("Local MT port Ether UP ");
			ret = D_SYS_OK;
			break;
		}

		f_com_taskDelay(1000,0,0,0);
	}
	close(sd);
	return ret;
}

/*!
 * @brief 関数機能概要:pf_mti loop
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
INT f_mti_loop( VOID )
{
	int					sd;
	int					sd_cli;
	int					cli_len;
	struct sockaddr_in	cli_addr;
	int					onoff;
	struct sockaddr_in	ssaddr;
	int					rtn;
	ssize_t				rlen;
	ssize_t				slen;
	int					conn;


	f_mti_mtLog("MT loop start.... ");

	(VOID)BPF_HM_DEVC_GETTIME(&(f_mtiw_InfoTbl->comInfo.server.s_date));

	errno = 0;
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "MT socket error(e:%d) ", errno);
		f_mti_mtLog("MT socket error(e:%d) ", errno);
		return D_SYS_NG;
	}

	memset(&ssaddr,0,sizeof(ssaddr));
	ssaddr.sin_family      = AF_INET;
	ssaddr.sin_port        = htons(f_mtiw_InfoTbl->comInfo.server.portNo);
	ssaddr.sin_addr.s_addr = htonl(f_mtiw_InfoTbl->comInfo.server.ipAddr);
	onoff = 1;
	setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, (char *)&onoff, sizeof(onoff) );

	errno = 0;
	if(( rtn = bind(sd, (struct sockaddr *)&ssaddr, sizeof(ssaddr))) != 0){
		close(sd);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "MT bind error(e:%d sd:%d) ", errno,sd);
		f_mti_mtLog("MT bind error(e:%d sd:%d) ", errno,sd);
		return D_SYS_NG;
	}

	listen(sd, 1);

	f_mtiw_InfoTbl->comInfo.client.ipAddr = 0;
	f_mtiw_InfoTbl->comInfo.client.portNo = 0;

	while(1){

		cli_len = sizeof(cli_addr);
		memset(&cli_addr,0,sizeof(cli_addr));
		
		f_mtiw_InfoTbl->comInfo.mt_con_state = D_SYS_OFF;

		errno = 0;
		if(( sd_cli = accept(sd, (struct sockaddr *)&cli_addr , (socklen_t *)&cli_len)) == -1){
			f_mti_mtLog("MT accept error(e:%d sd:%d) ", errno,sd);
			break;
		}

		f_mtiw_InfoTbl->comInfo.mt_con_count++;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "MT link connected(%d) by add:0x%x port:%d ", f_mtiw_InfoTbl->comInfo.mt_con_count,ntohl(cli_addr.sin_addr.s_addr),ntohs(cli_addr.sin_port));
		f_mti_mtLog("MT link connected(%d) by add:0x%x port:%d ", f_mtiw_InfoTbl->comInfo.mt_con_count,ntohl(cli_addr.sin_addr.s_addr),ntohs(cli_addr.sin_port));

		onoff = 1;
		setsockopt( sd_cli, IPPROTO_TCP, TCP_NODELAY   ,(char*)&onoff,sizeof(onoff));
		onoff = 1;
		setsockopt( sd_cli, SOL_SOCKET , SO_KEEPALIVE  ,(char*)&onoff,sizeof(onoff));
		onoff = 5;
		setsockopt( sd_cli, SOL_TCP    , TCP_KEEPIDLE  ,(char*)&onoff,sizeof(onoff));
		onoff = 3;
		setsockopt( sd_cli, SOL_TCP    , TCP_KEEPINTVL ,(char*)&onoff,sizeof(onoff));
		onoff = 3;
		setsockopt( sd_cli, SOL_TCP    , TCP_KEEPCNT   ,(char*)&onoff,sizeof(onoff));

		f_mtiw_InfoTbl->comInfo.client.ipAddr =  ntohl(cli_addr.sin_addr.s_addr);
		f_mtiw_InfoTbl->comInfo.client.portNo = ntohs(cli_addr.sin_port);
		(VOID)BPF_HM_DEVC_GETTIME(&(f_mtiw_InfoTbl->comInfo.client.s_date));

		conn = D_SYS_NG;
		f_mtiw_sd_cli = -1;

		while(1){
			errno = 0;
			if((rlen = recv(sd_cli, f_mtiw_recvData, sizeof(f_mtiw_recvData), 0)) <= 0){
				f_mti_mtLog("MT recv error(e:%d len:%d )", errno , rlen);
				break;
			}
			f_mtiw_sd_cli = sd_cli;
			conn = f_mti_event(rlen,f_mtiw_recvData,f_mtiw_sendData,&slen);
			if(conn != D_SYS_MNG){
				errno = 0;
				if(( rlen = send(sd_cli,f_mtiw_sendData, slen , 0)) <= 0){
					f_mti_mtLog("MT send error(e:%d len:%d )", errno , rlen);
					f_mtiw_sd_cli = -1;
					break;
				}
			}

			if((conn == D_SYS_NG) || (conn == -2)){
				break;
			}
		}
		
		close(sd_cli);

		(VOID)BPF_HM_DEVC_GETTIME(&(f_mtiw_InfoTbl->comInfo.client.e_date));

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "MT link down(%d)",f_mtiw_InfoTbl->comInfo.mt_con_count);
		f_mti_mtLog("MT link down(%d)",f_mtiw_InfoTbl->comInfo.mt_con_count);

		/* Reset */
		if(conn == -2){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Reset by MT" );
			BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI, SIGUSR2, D_RRH_SIGUSR2_OPT_RSTCMD );
			break;
		}
	}
	close(sd);
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "MTI thread finish");
	(VOID)BPF_HM_DEVC_GETTIME(&(f_mtiw_InfoTbl->comInfo.server.e_date));
	f_mti_mtLog("MT loop end.... ");
	return D_SYS_NG;
}


/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
VOID f_mti_send(UCHAR* senddata_p ,ssize_t slen ){
	ssize_t				rlen;

	if(f_mtiw_sd_cli == -1){
		f_mti_mtLog("MT send skip error(len:%d )", slen);
		return;
	}
	if(( rlen = send(f_mtiw_sd_cli,senddata_p, slen , 0)) <= 0){
		f_mti_mtLog("MT send2 error(e:%d len:%d )", errno , rlen);
		f_mtiw_sd_cli = -1;
	}
	return;
}

/*!
 * @brief 関数機能概要:pf_mtiメインスレッド。
 * @note  関数処理内容.
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/05/13 Fujitsu)Taniguchi Create
 */
VOID f_mti_main( VOID )
{
	T_SYS_INIT_COMP_NTC*		sndMsg;							/* 送信メッセージ(初期化完了通知)	*/
	INT							endcd= 0;
	INT							errcd= 0;
	INT							ret;
	
	prctl(PR_SET_NAME, "pf_mti", 0, 0, 0);
	printf("\ninit f_mti_main ");//C9 add code debug
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,D_SYS_THDID_PF_MTI,D_SYS_THDQID_PF_MTI,(unsigned long int)pthread_self());

	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_MT_INFO, (VOID **)&f_mtiw_InfoTbl, &errcd);

	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_INIT_COMP_NTC),			/* MessageSize						*/
										(VOID **)&sndMsg,						/* MessageBuffer					*/
										&errcd );								/* ErrorCode						*/
	if( endcd != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " ADDRGET NG %d %d ", endcd ,errcd);
		return;
	}

	sndMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;					/* メッセージID						*/
	sndMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* 返信先PQID						*/
	sndMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* 送信先TQID						*/
	sndMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元PQID						*/
	sndMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_MTI;							/* 送信元TQID						*/
	endcd = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,(VOID *)sndMsg );
	if( endcd != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ_SEND NG %d ", endcd );
		return;
	}

	if((ret = f_mti_init()) != D_SYS_OK ){
		return;
	}

	if(( ret = f_mti_loop() ) != D_SYS_OK ){
		return;
	}
	
	return;
}

/* @} */

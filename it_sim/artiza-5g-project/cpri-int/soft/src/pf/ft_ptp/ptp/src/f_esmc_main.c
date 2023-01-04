/*!
 * @skip		$ld:$
 * @file		f_esmc_main.c
 * @brief		pf_esmc メイン処理
 * @author		KCN)Suehiro
 * @date		2018/08/28
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "f_esmc_typ.h"
#include "f_esmc_def.h"
#include "f_rrh_def.h"
#include "f_ptp_inc.h"

#include <sys/time.h>
#include <sys/msg.h>
#include <net/if.h> 
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> 

extern UINT					f_syncE_ssm_timer ;
extern UINT					f_syncE_ssm_timer_e ;
extern UINT					f_syncE_ssm_timeout ;
extern UINT					f_syncE_acceptance_ssm[];

extern pthread_mutex_t f_syncE_ssm_mutex; 

/* 関数プロトタイプ */

/*!
 * @brief		main
 * @note		main関数.
 *                  -# ソケットオープン
 *					-# MSG待ち無限Loop処理へ移行
 *					-# MSG受信時は解析処理を実施
 *                   # まずはfhethS0のみ版
 * @param		-
 * @return		N/A
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 */

void f_esmc_main(void)
{
	int						endcd, errcd;				/*  返り値 */
	struct ifreq			ifr0;
	int						sock0;						/* fhethS0用 */

	struct sockaddr_ll		sll0;
	
	struct sockaddr_storage sockaddr_in_src;
	socklen_t				srcLen = (int)sizeof(sockaddr_in_src);
	char					buf[2048];

	int						numFd;
	struct timeval			taskSelectTimeout, workingSelectTimeout;
	fd_set					taskFdSet, workingFdSet;
	
	char					s_ssmcode;
	unsigned char			val;
	struct esmc_header		*esmcform;					/* ESMC構造体 */
	signed int				nBytes;
	unsigned int			time_data ;
	char					lastval = 0;				/* 前回値 */
	int						errnum;
	T_RRH_PTPCONF_MNG		*f_ptp_tbl_adr;			/*  PTP状態テーブル	*/
	unsigned int			ifindex0;
	UINT					stopwave_raise_count;
	UINT					stopwave_cease_count;
	UINT					count;
	INT						ret = D_SYS_OK ;
	int						synce_state = D_DU_SVCTL_ON_SYNCE_STATE_DISABLE;


	/* FDSET 初期化 */
	FD_ZERO(&taskFdSet);


	/* スレッド名表示 */
	prctl(PR_SET_NAME, "ptp_esmcr", 0, 0, 0);

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PTP, 			/* ProcessID	:1	*/
								 D_SYS_THDID_PTP_ESMCRCV, 	/* ThreadID			*/
								 D_SYS_THDQID_PTP_ESMCRCV,	/* ThreadQueueID	*/
								 D_SYS_THDID_PTP_ESMCRCV);	/* ThreadID			*/


	/* 共有メモリの取得 */
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
        E_RRH_SHMID_APL_PTP_CONF,   		/* 共有メモリ番号               */
        (VOID **)&f_ptp_tbl_adr,       	   /* 共有メモリアドレス           */
        &errcd                              /* 詳細NGコード                 */
	);
	/* 共有メモリの取得失敗時 */
	if( endcd != BPF_RU_IPCM_OK )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,"shared mem get NG.NG:%x ERRCD:%x",endcd,errcd);
		return;
	}

	/* PTP側でポート確定後に処理を開始する(確定まで待ち合わせ) */
	while( f_ptp_tbl_adr->actport == PORT_NOUSE)				/* 0:使用ポート無 */
	{
		sleep(1);
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER," f_esmc_main(): Waiting judge useport.");
	}

	UINT	portNo= 0;
	/* ソケット生成		*/
	sock0 = socket(PF_PACKET, SOCK_RAW, ETH_P_SLOW);
    if(sock0 < 0){
		errnum = errno;
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," ESMC Socket Create error. socket= %d, err=%s", sock0,strerror(errnum));
        return;
    }

    /* インターフェース情報取得 (examplePtpSocketConfig802p3参考) */
	memset(&ifr0, 0, sizeof(struct ifreq));
	snprintf(ifr0.ifr_name,sizeof(ifr0.ifr_name),"fhethS%d",portNo);

	/* ifindexの番号が欲しいから問合せを実施 */
	ioctl(sock0, SIOCGIFINDEX, &ifr0);
	ifindex0 = ifr0.ifr_ifindex;
    
    /* ソケット情報設定 */
    memset(&sll0, 0, sizeof(struct sockaddr_ll));

    sll0.sll_family = AF_PACKET;
    sll0.sll_ifindex = ifindex0;
    sll0.sll_protocol = htons(ETH_P_SLOW);
    sll0.sll_halen = ETHER_ADDR_LEN;
	
	/* バインド */
    if(bind(sock0, (struct sockaddr *)&sll0, sizeof(sll0)) < 0)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," ESMC Socket bind error(0).");
        close(sock0);
        return;
    }
	
	FD_SET(sock0, &taskFdSet);						/* selectで待つ読み込みソケットとしてsock0を登録します */
	/* タイムアウト値設定 1.1秒 */
	taskSelectTimeout.tv_sec = 1;
	taskSelectTimeout.tv_usec = 100000;	/* 100msecはマージン(対向が1秒周期で送信するため、100msecは待ち時間とする) */

	stopwave_raise_count = D_RRH_OFF;
	stopwave_cease_count = D_RRH_OFF;
	
	/* メモリ初期設定 ITU-T G8264勧告上、5秒がデフォルト値 */
	f_syncE_ssm_timer = 5;
	f_syncE_ssm_timer_e = 5;
	f_syncE_ssm_timeout = SYNCE_SSM_TIMEOUT_FALSE;
	for(count = 0; count < SYNCE_ACCEPTANCE_SSM_NUM; count++)
	{
		f_syncE_acceptance_ssm[count] = PTP_EDIT_DATA_UNDEF_32;
	}
	f_syncE_acceptance_ssm[0] = ESMC_QL_PRC;
	f_syncE_acceptance_ssm[1] = ESMC_QL_SSU_A;

	time_data = D_PTP_REPORT_TIMER * f_syncE_ssm_timer ;
	/* ssmタイマ設定 */
	ret = BPF_RU_HRTM_REGISTER(	BPF_RU_HRTM_REG_ONCE,					/* Timer種別						*/
								E_RRH_TIMID_SYNCE_SSMTIME,				/* Timer ID							*/
								time_data,								/* Timer値							*/
								D_SYS_MSGID_SYNCESSMTO_NTC,				/* TimeOut時通知Event番号			*/
								D_SYS_THDQID_PTP_MAIN,					/* TimeOut時送信先Queue番号			*/
								&errcd );
	if( ret != D_SYS_OK )
	{
		/* タイマ設定エラーの表示 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," SSM timer set error.");
	}

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"Starting SyncE recvport%d(%s) if:%d soc:0x%x ",
		portNo,ifr0.ifr_name,ifindex0,sock0);
	
	/********************************************************************************/
	/* ESMCパケット受信処理の無限Loop												*/
	/********************************************************************************/
	while(1)
	{
		/* 格納エリアお掃除 */
		memset(buf, 0, sizeof(buf));
		memcpy(&workingFdSet, &taskFdSet, sizeof(workingFdSet));
		memcpy(&workingSelectTimeout, &taskSelectTimeout, sizeof(workingSelectTimeout));
		val = 0;

		pthread_mutex_lock(&f_syncE_ssm_mutex);
		if(f_syncE_ssm_timer != f_syncE_ssm_timer_e)
		{
			/* SSMタイマがeditされていたらタイマを再設定する */
			if(f_syncE_ssm_timer != 0)
			{
				ret = BPF_RU_HRTM_CANCEL( E_RRH_TIMID_SYNCE_SSMTIME , &errcd ) ;

				if( ret != D_SYS_OK )
				{
					/* タイマ設定エラーの表示 */
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," SSM timer cancel error.");
				}
			}
			if(f_syncE_ssm_timer_e != 0)
			{
				time_data = D_PTP_REPORT_TIMER * f_syncE_ssm_timer_e ;
				/* ssmタイマ設定 */
				ret = BPF_RU_HRTM_REGISTER(	BPF_RU_HRTM_REG_ONCE,					/* Timer種別						*/
											E_RRH_TIMID_SYNCE_SSMTIME,				/* Timer ID							*/
											time_data,								/* Timer値							*/
											D_SYS_MSGID_SYNCESSMTO_NTC,				/* TimeOut時通知Event番号			*/
											D_SYS_THDQID_PTP_MAIN,					/* TimeOut時送信先Queue番号			*/
											&errcd );
				if( ret != D_SYS_OK )
				{
					/* タイマ設定エラーの表示 */
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," SSM timer set error.");
				}
			}
			f_syncE_ssm_timer = f_syncE_ssm_timer_e;
			f_syncE_ssm_timeout = SYNCE_SSM_TIMEOUT_FALSE;
		}
		pthread_mutex_unlock(&f_syncE_ssm_mutex);

		numFd = select(sock0 + 1, &workingFdSet, NULL, NULL, &workingSelectTimeout);
		
		if (numFd > 0)
		{
			if (FD_ISSET(sock0, &workingFdSet)) 
			{
				nBytes = recvfrom( sock0, (char *)buf, sizeof(buf), 0, (struct sockaddr *)(void *)&sockaddr_in_src, &srcLen);
				/* nByte=0の時はソケットが切れているのでcloseする */
				if (nBytes == 0)
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," Socket disconnected(0).");
					break;
				}
				if (nBytes == -1)
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," ESMC Select error.");
					break;
				}
				
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER," ESMC recvfrom fhethS.");

				/* bufをESMC構造体にコピーしてからメンバーとして取得 */
				esmcform = (struct esmc_header *)buf;
				s_ssmcode = esmcform->ssmcode;
				
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "SSMCODE: 0x%02x ", s_ssmcode);

				val = (SSM_REV_MASK & s_ssmcode );
				f_ptp_ssmval = val;

				/* 受信したssm値がリストと一致する場合 */
				pthread_mutex_lock(&f_syncE_ssm_mutex);
				for(count = 0; count < SYNCE_ACCEPTANCE_SSM_NUM; count++)
				{
					/* 受信したssm値がリストと一致する場合 */
					if(val == f_syncE_acceptance_ssm[count]){
						if(stopwave_cease_count == D_RRH_OFF){
							stopwave_cease_count = D_RRH_ON;
							synce_state = D_DU_SVCTL_ON_SYNCE_STATE_PARENT;
						}
						/* 期待するSSM受信したのでSSMタイマ再設定 */
						if(f_syncE_ssm_timer != 0){
							time_data = D_PTP_REPORT_TIMER * f_syncE_ssm_timer ;
							/* ssmタイマ設定(本関数内で起動済タイマはキャンセルされる) */
							ret = BPF_RU_HRTM_REGISTER(	BPF_RU_HRTM_REG_ONCE,					/* Timer種別						*/
														E_RRH_TIMID_SYNCE_SSMTIME,				/* Timer ID							*/
														time_data,								/* Timer値							*/
														D_SYS_MSGID_SYNCESSMTO_NTC,				/* TimeOut時通知Event番号			*/
														D_SYS_THDQID_PTP_MAIN,					/* TimeOut時送信先Queue番号			*/
														&errcd );
							if( ret != D_SYS_OK ){
								/* タイマ設定エラーの表示 */
								BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," SSM timer reset error(%d).",errcd);
							}
						}
						f_syncE_ssm_timeout = SYNCE_SSM_TIMEOUT_FALSE;
						break;
					}
				}
				pthread_mutex_unlock(&f_syncE_ssm_mutex);
				/* 受信したssm値がリストと不一致の場合 */
				if(count == SYNCE_ACCEPTANCE_SSM_NUM && stopwave_raise_count == D_RRH_OFF)
				{
					stopwave_raise_count = D_RRH_ON;
					synce_state = D_DU_SVCTL_ON_SYNCE_STATE_NOK;
				}

			}else{
				if(stopwave_raise_count == D_RRH_OFF){
					stopwave_raise_count = D_RRH_ON;
					synce_state = D_DU_SVCTL_ON_SYNCE_STATE_DISABLE;
				}
				errnum = errno;
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," select internal 0x%02x 0x%02x error:%s", val ,lastval,strerror(errnum));
			}
		}
		else
		{
			if(f_syncE_ssm_timeout == SYNCE_SSM_TIMEOUT_TRUE && stopwave_raise_count == D_RRH_OFF){
				stopwave_raise_count = D_RRH_ON;
				f_ptp_ssmval = ESMC_QL_DNU;
				synce_state = D_DU_SVCTL_ON_SYNCE_STATE_DISABLE;
			}
			if(numFd == 0){
				/* タイムアウト エラーにはせずリトライする */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER," ESMC Select timeout.");
			}else{
				errnum = errno;
				/* EINTRの場合はリトライする */
				if (errno != EINTR )
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," ESMC select() error. err=%s", strerror(errnum));
					break;
				}
			}
		}

		if(stopwave_raise_count == D_RRH_ON){
			/* 停波(SVレジスタへの書込み)  */
			/* SV15レジスタ B24 のSyncE Lockedビットを落とす */
			/* SV15レジスタ B19-16 のSyncE StatusをNOKとする */
			f_com_SVCtl(E_DU_SVCTL_OPE_STP_WAV, D_DU_SVCTL_ON, D_DU_SVCTL_ON_WAVE_STOP_ALL);
			f_com_SVCtl(E_DU_SVCTL_OPE_SPLAN, D_DU_SVCTL_OFF, D_DU_SVCTL_ON_SYNCE_STATE_LOCKED);
			f_com_SVCtl(E_DU_SVCTL_OPE_SYNCE, D_DU_SVCTL_CHG, synce_state);
			stopwave_raise_count++;
			stopwave_cease_count = D_RRH_OFF;
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"Raise STOP Wave by SyncE packet(0x%x->0x%x)",lastval,val);
		}
		else if(stopwave_cease_count == D_RRH_ON){
			/* 停波解除(SVレジスタへの書込み)  */
			/* SV15レジスタ B24 のSyncE Lockedビットを立てる */
			/* SV15レジスタ B19-16 のSyncE StatusをOKとする */
			f_com_SVCtl(E_DU_SVCTL_OPE_STP_WAV, D_DU_SVCTL_OFF, D_DU_SVCTL_ON_WAVE_STOP_ALL);
			f_com_SVCtl(E_DU_SVCTL_OPE_SPLAN, D_DU_SVCTL_ON, D_DU_SVCTL_ON_SYNCE_STATE_LOCKED);
			f_com_SVCtl(E_DU_SVCTL_OPE_SYNCE, D_DU_SVCTL_CHG, synce_state);
			stopwave_cease_count++;
			stopwave_raise_count = D_RRH_OFF;
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"Cease STOP Wave by SyncE packet(0x%x->0x%x)",lastval,val);
		}
		else{
			/* Do nothing */	;
		}
		/* 前回値を上書き */
		lastval = val;
	}
	/* Close処理 */
	close(sock0);
	
	return;
}


/* @} */

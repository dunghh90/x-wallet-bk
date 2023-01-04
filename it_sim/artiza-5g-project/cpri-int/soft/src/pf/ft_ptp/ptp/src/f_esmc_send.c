/*!
 * @skip		$ld:$
 * @file		f_esmc_send.c
 * @brief		ptp_esmc 送信処理
 * @author		KCN)Suehiro
 * @date		2018/08/28
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "f_sys_def.h"
#include "f_esmc_typ.h"
#include "f_esmc_def.h"
#include "f_rrh_def.h"
#include "f_ptp_inc.h"
#include "f_du_reg_ctb.h"

#include <sys/time.h>
#include <sys/msg.h>

#include <net/if.h> 
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> 


/*****************   STATIC GLOBAL VARIABLES   ********************************/
long	ptplocksts = 0x02000000;			/* SVレジスタ	 */


/*************************** define **************************************/
#define ESMC_PDU_SIZE	60

/******************************************************************************/

/*!
 * @brief		main
 * @note		send関数.
 * @param		-
 * @return		N/A
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 */
void f_esmc_send( void)
{
	struct esmc_header		*destbuf;
	char					buf[64];
	struct sockaddr_ll		destsll;
	int						getsts;
	char					macaddr_s0[6];			/* 自分のMACアドレス格納用 */
	int						i;						/* ループ変数 */
	unsigned short			addr_s0 = 0x0220;		/* EEPROMアドレス(S0) */
	/* unsigned short	addr_s1 = 0x0260;	*/		/* EEPROMアドレス(S1) */
	unsigned char	data = 0;
	int	eveflag;
	int	bytes;
	int						errnum;					/* systemエラー戻り値格納用 */
	int iRtn;
	unsigned int temp;
	T_RRH_PTPCONF_MNG		*f_ptp_tbl_adr;			/*  PTP状態テーブル	*/
	UINT					info;

	/* スレッド名表示 */
	prctl(PR_SET_NAME, "ptp_esmcs", 0, 0, 0);

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PTP, 			/* ProcessID	:1	*/
								 D_SYS_THDID_PTP_ESMCSND, 	/* ThreadID			*/
								 D_SYS_THDQID_PTP_ESMCSND,	/* ThreadQueueID	*/
								 D_SYS_THDID_PTP_ESMCSND);	/* ThreadID			*/


	/* 共有メモリの取得 */
	int errcd;
	int endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
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
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER," f_esmc_send(): Waiting judge useport.");
	}
	UINT	portNo=0;
	addr_s0 = D_DU_EEP_MAC_ADDR_PORT1_H1;
	info = M_DU_REG_ETH_PORT0(D_DU_REG_ETH_IRQMACSTI);

	/* EEPROMから自分のMACアドレス情報を取得 */
	for( i = 0; i < 6; i++, addr_s0++ ) {
		BPF_HM_DEVC_EEPROM_READ( addr_s0, &data );
		macaddr_s0[i] = data;
	}

	/* ソケット生成処理 start */
	unsigned int			ifindex0;
	struct ifreq			ifr0;
	int						sock0;						/* fhethS0用 */
	struct sockaddr_ll		sll0;
	
	sock0 = socket(PF_PACKET, SOCK_RAW, ETH_P_SLOW);
    if(sock0 < 0){
		errnum = errno;
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," f_esmc_send() NG: ESMC Socket Create error. socket= %d, err=%s", sock0, strerror(errnum));
        return;
    }

    /* インターフェース情報取得  */
	memset(&ifr0, 0, sizeof(struct ifreq));
	snprintf(ifr0.ifr_name,sizeof(ifr0.ifr_name),"fhethS%d",portNo);

	/* ifindexの番号が欲しいから問合せを実施 */
	ioctl(sock0, SIOCGIFINDEX, &ifr0);
	ifindex0 = ifr0.ifr_ifindex;
    
    /* ソケット情報設定 */
    memset(&sll0, 0, sizeof(struct sockaddr_ll));

    sll0.sll_family = AF_PACKET;
    sll0.sll_ifindex = ifindex0;							/* ifindex0固定版 */
    sll0.sll_protocol = htons(ETH_P_SLOW);					/* 0x8809 */
    sll0.sll_halen = ETHER_ADDR_LEN;

	/* バインド */
    if(bind(sock0, (struct sockaddr *)&sll0, sizeof(sll0)) < 0)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," f_esmc_send() NG: ESMC Socket bind error(0).");
        close(sock0);
        return;
    }
	/* ソケット生成処理 end */
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"Starting SyncE sendport%d(%s) if:%d soc:0x%x smac:%02x:%02x%02x%02x%02x%02x ",
		portNo,ifr0.ifr_name,ifindex0,sock0,macaddr_s0[0],macaddr_s0[1],macaddr_s0[2],macaddr_s0[3],macaddr_s0[4],macaddr_s0[5]);
	/********************************************************************************/
	/* ESMCパケット送信処理のLoop													*/
	/********************************************************************************/
	while(1)
	{
		/* 1秒待つ */
		sleep(1);

		iRtn = BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, info, D_RRH_ON, &temp); 
		if(iRtn != BPF_HM_DEVC_COMPLETE){
			/* リードエラー */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , "register read error errcode = %d , addr = 0x%08x \n",iRtn,info);
			break;
		}
		/* LOS中は送信しない*/
		if((temp & D_RRH_ON) != 0){
			continue;
		}
		/* PTP同期状態をSV#15 B25から取得 */
		iRtn = BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, 0x02000000 , &temp); 
		if(iRtn != BPF_HM_DEVC_COMPLETE)
		{
			/* リードエラー */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , "register read error errcode = %d , addr = 0xa000018c \n",iRtn );
			break;
		}

		/* 25ビット目をbit ANDして変化検出 */
		getsts = temp & 0x02000000 ;

		/* 前回値と比較 */
		if (ptplocksts != getsts ){
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER," getsts :%08x", getsts);
			eveflag = 1;
			ptplocksts = getsts;		/* 状態上書き */
			if(ptplocksts == 0x02000000 ){
				BPF_COM_LOG_DMESG("FH-Eth%d SYNC Quality is GOOD(^=^) - Starting SyncE packet ESMC_QL_PRC(0x%x)\n",portNo+1,ESMC_QL_PRC);
			}else{
				BPF_COM_LOG_DMESG("FH-Eth%d SYNC Quality is BAD(T=T) - Starting SyncE packet ESMC_QL_DNU(0x%x)\n",portNo+1,ESMC_QL_DNU);
			}
		}
		else{
			eveflag = 0;
		}
		/* 送信するPKT作成 */
		destbuf = (struct esmc_header *)buf;
		memset(destbuf, 0, ESMC_PDU_SIZE);

		destbuf->destAddr[0] = 0x01;
		destbuf->destAddr[1] = 0x80;
		destbuf->destAddr[2] = 0xC2;
		destbuf->destAddr[3] = 0x00;
		destbuf->destAddr[4] = 0x00;
		destbuf->destAddr[5] = 0x02;
		
		/* 今はPort0側固定 */
		for( i = 0; i < 6; i++) {
			destbuf->sourceAddr[i]= macaddr_s0[i];
		}

		destbuf->slowprotocolEth[0] = 0x88;
		destbuf->slowprotocolEth[1] = 0x09;
		destbuf->slowprotocolSub = 0x0A;
		destbuf->itu_oui[0] =0x00;
		destbuf->itu_oui[1] =0x19;
		destbuf->itu_oui[2] =0xA7;
		destbuf->itu_subtype[0] = 0x00;
		destbuf->itu_subtype[1] = 0x01;

		if (eveflag == 1 ){
			destbuf->verflag = 0x18;
		}
		else{
			destbuf->verflag = 0x10;
		}
		destbuf->reserve1[0] = 0x00;
		destbuf->reserve1[1] = 0x00;
		destbuf->reserve1[2] = 0x00;
		destbuf->type = 0x01;		
		destbuf->length[0] = 0x00;
		destbuf->length[1] = 0x04;
		
		if (ptplocksts == 0x02000000 ){
			/* 同期状態 */
			destbuf->ssmcode = ESMC_QL_PRC;
		}
		else{
			/* 同期未確立 */
			destbuf->ssmcode = ESMC_QL_DNU;
		}
		
		for( i = 0; i < 8; i++) 
		{
			destbuf->datapad[i]= 0x00000000;
  		}
		
		/* FCS演算はハードが実施するのでここでは何もしない */
		

		/* 送信処理 examplePtpSocketTx参考 */
		memset(&destsll, 0, sizeof(struct sockaddr_ll));
		
		destsll.sll_halen    = ETHER_ADDR_LEN;
		destsll.sll_family   = AF_PACKET;
		destsll.sll_protocol = htons(ETH_P_SLOW);
		destsll.sll_ifindex  = ifindex0;
		
		bytes = sendto(sock0, (char *)destbuf, sizeof(struct esmc_header), 0, (struct sockaddr*)&destsll, sizeof(destsll));
		if (bytes == -1)
		{
			errnum = errno;
			/* EINTRの場合はリトライする */
			if (errno != EINTR )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR," f_esmc_send() NG: sendto error. err=%s", strerror(errnum));
				break;
			}
		}
		
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER," ESMC sendto fhethS.");
	}
	close(sock0);
	return;
}
/* @} */

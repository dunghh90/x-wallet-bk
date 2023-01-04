/*!
 * @skip	$ld:$
 * @file	f_eth_IPset.c
 * @brief	IPアドレスの設定処理
 * @date	2013/11/11 ALPHA)中村 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_ETH
 * @{
 */

#include "f_eth_inc.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>

/*!
 *	@brief	f_eth_init_IPset
 *	@note	関数処理内容.
 *			-# スレッド起動時にEEPROMから取得したIPアドレスの設定を行う。
 *				-# EEPROM情報を読み込む（IP-addr/サブネットマスク）
 *				-# 上記で取得した値を引数として、IPアドレス設定用の関数をコールする。
 *			-# 正常終了を戻り値として上位に返却する。
 * @retval		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-ETH-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/11/11 ALPHA)中村 Create
 */
VOID f_eth_init_IPset()
{
	INT					rtn;
	UINT				ipAdr;
	UINT				ipMask;
	T_RRH_TRAINVTBL		*invTbl = NULL;
	INT					rtn_bpf;
	INT					errcd;

	rtn	= D_SYS_OK;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_eth_init_IPset] ENTER" );

	/* 共有メモリからパラメータを取得 */
	rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
	if(BPF_RU_IPCM_NG == rtn_bpf)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Inventory sh memory address error=%08x", rtn_bpf);
		rtn = D_SYS_NG;
		return;
	}

	/************************************************************/
    /* EEPROMよりIP取得                                         */
    /************************************************************/
	ipAdr = ((invTbl->cmx_ipadd[0] << 24) |
			 (invTbl->cmx_ipadd[1] << 16) |
			 (invTbl->cmx_ipadd[2] <<  8) |
			  invTbl->cmx_ipadd[3]);
	ipMask= ((invTbl->cmx_netmask[0] << 24) |
			 (invTbl->cmx_netmask[1] << 16) |
			 (invTbl->cmx_netmask[2] <<  8) |
			  invTbl->cmx_netmask[3]);

	/************************************************************/
    /* IPアドレス設定用関数をコール                             */
    /************************************************************/
	rtn = f_eth_IPset(ipAdr, ipMask);
	if(rtn == D_SYS_NG)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "IP Setting error=%08x", rtn);
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_eth_init_IPset] ENTER" );
}

/*!
 *	@brief	f_eth_api_IPset
 *	@note	関数処理内容.
 *			-# APIコール時のIPアドレスの設定を行う。
 *				-# 受信メッセージから設定値を読み込む（IP-addr/サブネットマスク）
 *				-# 上記で取得した値を引数として、IPアドレス設定用の関数をコールする。
 *			-# 正常終了を戻り値として上位に返却する。
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常発生
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-ETH-001-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/11/19 ALPHA)中村 Create
 * @date 2015/08/11 TDI)satou EEPROMの参照アドレスをS3G→3Gに変更(DCM-FHM_EEPROM.xlsに合わせる)
 */
INT f_eth_api_IPset(VOID* msgP)
{
	INT							rtn;
	INT							rtn_bpf;
	INT							errcd;
	INT							result;
	T_API_DBG_IFCONFIG_REQ		*apiReqMsg;
	T_API_DBG_IFCONFIG_RSP	    *apiRspMsg;
	UCHAR						chValue;
	rtn	= D_SYS_OK;
	UINT						count;
	UINT						shifttable[4] = {24,16,8,0};
	INT							eepaddr_ipadd[4] = {D_RRH_EEP_IP_ADDER_3G_H1,D_RRH_EEP_IP_ADDER_3G_H2,
													D_RRH_EEP_IP_ADDER_3G_L1,D_RRH_EEP_IP_ADDER_3G_L2};
	INT							eepaddr_nmask[4] = {D_RRH_EEP_NETMASK_3G_H1,D_RRH_EEP_NETMASK_3G_H2,
													D_RRH_EEP_NETMASK_3G_L1,D_RRH_EEP_NETMASK_3G_L2};
	T_RRH_TRAINVTBL		*invTbl = NULL;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_eth_api_IPset] ENTER" );

	apiReqMsg = ( T_API_DBG_IFCONFIG_REQ* )msgP;

	/* 受信イベント番号からタスク内イベント番号へ変換 */
	switch( apiReqMsg->header.uiEventNo )
	{
		case D_API_MSGID_DBG_IFCONFIG_REQ:
			/*	IP設定設定			*/
			result = f_eth_IPset(apiReqMsg->data.ipAddr, apiReqMsg->data.ipMask);

			/* 応答(送信元へ設定)を送信*/
			rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別						*/
												sizeof(T_SYS_INIT_COMP_NTC),			/* MessageSize						*/
												(VOID **)&apiRspMsg,					/* MessageBuffer					*/
												&errcd );								/* ErrorCode						*/
			if( rtn_bpf != D_RRH_OK )
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Procmsg Addrgeterror=%08x", rtn_bpf );
				rtn	= D_SYS_NG;

				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_eth_api_IPset] RETURN" );
				return rtn;
			}
			/* 結果を設定 */
			apiRspMsg->data.result = result;

			/* 送信MSG作成 */
			apiRspMsg->header.uiEventNo		= D_API_MSGID_DBG_IFCONFIG_RSP;				/* メッセージID						*/
			apiRspMsg->header.uiDstPQueueID	= apiReqMsg->header.uiSrcPQueueID;			/* 返信先PQID						*/
			apiRspMsg->header.uiDstTQueueID	= apiReqMsg->header.uiSrcTQueueID;			/* 送信先TQID						*/
			apiRspMsg->header.uiSrcPQueueID	= apiReqMsg->header.uiDstPQueueID;			/* 送信元PQID						*/
			apiRspMsg->header.uiSrcTQueueID	= apiReqMsg->header.uiDstTQueueID;			/* 送信元TQID						*/
			apiRspMsg->header.uiLength		= sizeof(T_API_DBG_IFCONFIG_RSP);			/* データ部バイト長					*/

			BPF_RU_IPCM_MSGQ_SEND(	apiRspMsg->header.uiDstPQueueID	,					/* 送信MSGキューID					*/
									(VOID *)apiRspMsg );
			if(result == D_SYS_OK)
			{
				/* 共有メモリからパラメータを取得 */
				rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
				if(BPF_RU_IPCM_NG == rtn_bpf)
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Inventory sh memory address error=%08x", rtn_bpf);
					rtn = D_SYS_NG;
					return rtn;
				}
				
				for(count=0;count<4;count++)
				{
					/* ipadd更新処理 (EEP/共有メモリ)*/
					chValue = (UCHAR)(apiReqMsg->data.ipAddr >> shifttable[count]);
					if( chValue != (invTbl->cmx_ipadd[count]))
					{
						rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(eepaddr_ipadd[count],&chValue);
						if( BPF_HM_DEVC_COMPLETE != rtn_bpf )
						{
							BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_EEPROM_WRITE  ip error=%08x", rtn_bpf);
						}
						invTbl->cmx_ipadd[count] = chValue;
					}
					/* netmask更新処理 (EEP/共有メモリ)*/
					chValue = (UCHAR)(apiReqMsg->data.ipMask >> shifttable[count]);
					if( chValue != (invTbl->cmx_netmask[count]))
					{
						rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(eepaddr_nmask[count],&chValue);
						if( BPF_HM_DEVC_COMPLETE != rtn_bpf )
						{
							BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_EEPROM_WRITE netmask error=%08x", rtn_bpf);
						}
						invTbl->cmx_netmask[count] = chValue;
					}
				}
			}
			break;
		
		default:
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "EventNo error=%08x", (UINT)apiReqMsg->header.uiEventNo);
			rtn = D_SYS_NG;
			break;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_eth_api_IPset] RETURN" );
	return rtn;
}

/*!
 *	@brief	f_eth_IPset
 *	@note	関数処理内容.
 *			-# IPアドレスの設定を行う。
 *				-# 引数で渡されたパラメータIP-addr/サブネットマスク）でsocketを開き、IFupする
 *				-# 指定された値でsocketを開き、IFupする
 *				-# 指定値値が, 0xffffffffの場合、以下のデフォルト値を設定
 *			-# 正常終了を戻り値として上位に返却する。
 * @param  		ipAdr			[in]UINT IPアドレス
 * @param  		ipMask			[in]UINT サブネットマスク
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常発生
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-ETH-001-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/11/19 ALPHA)中村 Create
 */
INT f_eth_IPset(UINT ipAdr, UINT ipMask)
{
	INT					rtn;
	INT					sockfd;
	struct 				sockaddr_in sin;
	struct 				ifreq ifr;
	CHAR*				interface_name = "eth0";
	int					ret;

	rtn	= D_SYS_OK;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_eth_IPset] ENTER" );

	/* 異常値の場合はデフォルト値を設定する */
	if((ipAdr == 0) || (ipAdr == 0xffffffff))
	{
		ipAdr = D_ETH_IPADR;
	}
	if((ipMask == 0) || (ipMask == 0xffffffff))
	{
		ipMask = D_ETH_IPMSK;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Set IP Adr=%08x, Set IP Mask=%08x ", ipAdr, ipMask);

	/* ソケットをOpen */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	/************************************************************/
    /* IPアドレス設定                                           */
    /************************************************************/
	memset(&sin, 0, sizeof(struct sockaddr));

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(ipAdr);

	strncpy(ifr.ifr_name, interface_name, IFNAMSIZ);
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));

	ret = ioctl(sockfd, SIOCSIFADDR, &ifr);

	if(ret == -1) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "ioctl:ip set error.");
		perror("ioctl:SIOCSIFADDR");

		/* error発生の為close */
		close(sockfd);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_eth_IPset] RETURN" );
		return D_SYS_NG;
	}

	/************************************************************/
    /* マスク設定                                               */
    /************************************************************/
	memset(&sin, 0, sizeof(struct sockaddr));

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(ipMask);

	strncpy(ifr.ifr_name, interface_name, IFNAMSIZ);
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));

	ret = ioctl(sockfd, SIOCSIFNETMASK, &ifr);
	if(ret == -1) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "ioctl:mask error.");
		perror("ioctl:SIOCSIFNETMASK");

		/* error発生の為close */
		close(sockfd);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_eth_IPset] RETURN" );
		return D_SYS_NG;
	}

	/************************************************************/
    /* IFup処理                                                 */
    /************************************************************/
	strcpy( ifr.ifr_name, interface_name );
	if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "ioctl:Interface name error.");
		perror("ioctl:SIOCGIFFLAGS");

		/* error発生の為close */
		close(sockfd);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_eth_IPset] RETURN" );
		return D_SYS_NG;
	}

	strcpy( ifr.ifr_name, interface_name );
	ifr.ifr_flags |= IFF_UP;

	if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "ioctl:if up error.");
		perror("ioctl:SIOCGIFFLAGS");

		/* error発生の為close */
		close(sockfd);
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_eth_IPset] RETURN" );
		return D_SYS_NG;
	}

	/* 正常終了によるソケットのclose */
	close(sockfd);

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_eth_IPset] RETURN" );
	return rtn;
}
/* @} */

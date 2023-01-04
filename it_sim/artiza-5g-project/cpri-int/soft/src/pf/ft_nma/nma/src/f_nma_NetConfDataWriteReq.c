/*!
 * @skip  $ld:$
 * @file  f_nma_NetConfDataWriteReq.c
 * @brief Soft Parameter(NetConf) Filash Write Request
 * @date  2019/02/21 KCN)kamimoto Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_NMA
 * @{
 */
#include "f_nma_inc.h"

/*!
 * @brief Soft Parameter(NetConf) Filash Write Request
 * @note  This function is processed as follows.\n
 *       -# Write NetConf(User Information)
 * @param  *msgPtr    [in]VOID   received message
 * @return NON.
 * @date 2019/02/20 KCN)kamimoto Create\n
 * @warning	N/A
 * @FeatureID	PF_File-002-001-001
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
VOID f_nma_NetConfDataWriteReq(VOID* pRcvMsg)
{
	T_SYS_NMA_NETCONF_WRITE_REQ *		RCVMsg;
	T_SYS_NMA_NETCONF_WRITE_RSP *		sndMsg;

	int									ret;
	int									errcd ;
	unsigned char						wData ;

	/* 初期設定(返却バッファの確保) */
	RCVMsg = (T_SYS_NMA_NETCONF_WRITE_REQ *)pRcvMsg ;
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_NMA_FILE_WRITE_RSP),		/* Size								*/
										(VOID **)&sndMsg,						/* sndMsg							*/
										&errcd );
	if( ret != D_SYS_OK )
	{
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_NetConfDataWriteReq buffer error=0x%08x", errcd);
		return ;
	}
	else
	{
		sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMA_NETCONF_WRITE_RSP ;	/* NETCONFユーザ情報書込み応答		*/
		sndMsg->thdIf.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID			*/
		sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_FHS;					/* 送信先Thread Queue ID			*/
		sndMsg->thdIf.uiSrcPQueueID	= D_RRH_PROCQUE_PF;						/* 送信元Process Queue ID			*/
		sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_NMA;					/* 送信元Thread Queue ID			*/
		sndMsg->requestID			= RCVMsg->requestID;					/* リクエストID						*/
	}

	/* SSHユーザアカウント情報Flash書込みフラグ = ON */
	wData = D_RRH_EEP_WRITTING ;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_FLASH_WRITE_FLAG_SSH , &wData);

	/* 共有メモリ(netconf)の排他制御(Semtake) */
	ret = BPF_RU_IPCM_PROCSEM_TAKE( E_RRH_SEMID_APL_USER_INFO , BPF_RU_IPCM_LOCK_R , BPF_RU_IPCM_WAIT , NULL , &errcd );
	if( ret != BPF_RU_IPCM_OK )
	{
		/* セマフォ失敗 */
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_NetConfDataWriteReq Semapho(Take) Error ret=0x%08x errcd = 0x%08x", ret , errcd) ;
		sndMsg->result = D_SYS_SFPRSLT_SMPTER ;
		ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FHS ,			/* 送信先Thread Queue ID			*/
										(void*)sndMsg );				/* 送信Message						*/
		return ;
	}

	/* 共有メモリのread */
	memcpy( &f_nmaw_sftpdata_buf , &f_comw_netconf_tbl->num_of_use , ( sizeof(T_RRH_NETCONF_INFO_TBL) - D_SYS_NUM8 ) ) ;		/* pgr0548 */

	/* 共有メモリ(netconf)の排他制御(Semgive) */
	(VOID)BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_USER_INFO , BPF_RU_IPCM_LOCK_R , &errcd );

	/* Flashイレース */
	ret = f_nma_cm_EraseFlash(D_DU_QSPI_FLASH_FACE1 , D_RRH_QSPI_FLASH_OFFSET_NETCONF_USER_INFO , D_DU_QSPI_FLASH_SIZE_SOFT_PARA) ;
	if( ret != D_SYS_OK )
	{
		/* Assert */
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_NetConfDataWriteReq Flash Erase NG ret=%08x", ret);
		sndMsg->result = D_SYS_SFPRSLT_ERSERR ;
		ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FHS ,			/* 送信先Thread Queue ID			*/
										(void*)sndMsg );				/* 送信Message						*/
		return ;
	}

	/* SSHユーザアカウント情報のFlash書込み */
	ret = BPF_HM_DEVC_QSPI_FLASH_WRITE( D_DU_QSPI_FLASH_FACE1 , D_RRH_QSPI_FLASH_OFFSET_NETCONF_USER_INFO , ( sizeof(T_RRH_NETCONF_INFO_TBL) - D_SYS_NUM8 ) , (UCHAR *)&f_nmaw_sftpdata_buf );
	if( ret != BPF_HM_DEVC_COMPLETE )
	{
		/* 書込み失敗なら */
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_QSPI_FLASH_WRITE Error ret = 0x%08x", ret);
		sndMsg->result = D_SYS_SFPRSLT_WRTERR ;
		ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FHS ,			/* 送信先Thread Queue ID			*/
										(void*)sndMsg );				/* 送信Message						*/
		return ;
	}

	/* SSHユーザアカウント情報Flash書込みフラグ = OFF */
	wData = D_RRH_EEP_WRITE_OK ;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_FLASH_WRITE_FLAG_SSH , &wData);

	/* 書込み終了応答の送信 */
	sndMsg->result = D_SYS_SFPRSLT_OK ;
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_FHS ,			/* 送信先Thread Queue ID			*/
									(void*)sndMsg );				/* 送信Message						*/
	return ;
}
/* @} */



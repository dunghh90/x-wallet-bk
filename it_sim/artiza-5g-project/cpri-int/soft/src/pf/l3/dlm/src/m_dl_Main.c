/*!
 * @skip   $Id$
 * @file   m_dl_Main.c
 * @brief  DLM main thread function
 * @date   2008/07/15 FFCS)Wuh Create for eNB-008-001.
 * @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *                          stop writing FLASH after receiving CPRI message
 * @date   2015/08/15 FPT)Yen update for FHM
 * @date   2015/09/29 TDIPS)sasaki update 
 * @date   2015/10/13 TDIPS)sasaki update
 * @date   2015/10/14 TDIPS)sasaki update
 * @date   2015/11/19 TDIPS)sasaki update
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
*/
/********************************************************************************************************************/

/*
 * @addtogroup RRH_L3_DLM
 * @{
 */


#include "m_cm_header.h"								/* MDIF common head file				*/
#include "m_dl_header.h"								/* download management task head file*/
/* @{ */
/*!
 *  @brief  DLM thread main function
 *  @note   This function is processed as follows.
 *          1) Call the initialization function
 *          2) Receive message and analysis the message
 *		    3) Call the jump function
 *  @param  a_tskid      [in]  startup task id
 *  @param  a_event_no   [in]  startup event number
 *  @param  *a_bufaddr_p [in]  receving message address
 *  @return None.
 *  @date   2008/07/15 FFCS)Wuh Create for eNB-008-001.
 *  @date   2015/08/15 FPT)Yen update for FHM
 *  @date   2015/09/29 TDIPS)sasaki update ファイル系からのメッセージID誤りを修正
 *  @date   2015/10/14 TDIPS)sasaki update MKレビュー指摘No.299対応
 *  @date   2015/10/15 FPT)Duong update review comment 133,291
 */

VOID m_dl_main( UINT a_tskid, UINT a_event_no, VOID* a_bufaddr_p )
{
	USHORT 	lwEventId = 0;		/* event table index */
	USHORT	lwSignalKind = 0;
	USHORT	lwEventExist = CMD_OFF;
	USHORT	lwCpriNo = CMD_NUM0;
	USHORT	lwSysType = CMD_SYS_S3G;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	if( NULL == a_bufaddr_p )
	{
		cm_Assert(	CMD_ASL_USELOW, CMD_NUM0, "Buffer null");
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	if(( CMD_TSKIF_CPRIRCVNTC == a_event_no ) || ( CMD_TSKIF_REC_FWDNTC == a_event_no ))
	{
		/* CPRI信号種別取得 */
		lwSignalKind = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)a_bufaddr_p)->signal_kind & CMD_SYS_MSK_CPRID;
		lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)a_bufaddr_p)->signal_kind & CMD_SYS_CHK_CPRID;
	}

	while( dlr_event_ctbl[lwEventId].gbleventno != DLD_ECTBL_END )
	{
		if( a_event_no != dlr_event_ctbl[lwEventId].gbleventno )
		{
			/* Check next index */
			lwEventId++;
			continue;
		}

		switch( a_event_no )
		{
		/* スレッド起動 */
		case CMD_TSKIF_INIREQ:
			/* Event exist flag : ON */
			lwEventExist = CMD_ON;
			/* CpriNo: don't care */
			break;
		/* REシステムパラメータ更新要求 */
		case CMD_TSKIF_RESYSPARA_UPDATE_REQ:
			/* Event exist flag : ON */
			lwEventExist = CMD_ON;
			lwCpriNo = ((CMT_TSKIF_RESYSNEWREQ *)a_bufaddr_p)->cpri_no;
			lwSysType = ((CMT_TSKIF_RESYSNEWREQ *)a_bufaddr_p)->dummy;
			break;
		 /* REファイル更新要求 */
		case CMD_TSKIF_REFIRM_DL_REQ:
			/* Event exist flag : ON */
			lwEventExist = CMD_ON;
			lwCpriNo = ((CMT_TSKIF_REFILNEWREQ *)a_bufaddr_p)->cpri_no;
			lwSysType = ((CMT_TSKIF_REFILNEWREQ *)a_bufaddr_p)->sys_type;
			break;
		/* 運用中REファイル更新開始応答 */
		case CMD_TSKIF_USEREFILE_UPDATE_START_RSP:
			/* Event exist flag : ON */
			lwEventExist = CMD_ON;
			lwCpriNo = ((CMT_TSKIF_CPRINOGET *)a_bufaddr_p)->cpri_no;
			break;
		/* REファイル更新中止要求 */
		case CMD_TSKIF_REFIRM_DL_STOP_REQ:
			/* Event exist flag : ON */
			lwEventExist = CMD_ON;
			lwCpriNo = ((CMT_TSKIF_REFILNEWSTP *)a_bufaddr_p)->cpri_no;
			lwSysType = ((CMT_TSKIF_REFILNEWSTP *)a_bufaddr_p)->systerm;
			break;
		/* REファイル更新中止指示 */
		case CMD_TSKIF_REFIRM_DL_STOP_IND:
			/* Event exist flag : ON */
			lwEventExist = CMD_ON;
			lwCpriNo = DLD_ALL_RE;
			lwSysType = ((CMT_TSKIF_RENEWSTPNTC *)a_bufaddr_p)->systerm;
			break;
		/* FWファイル更新応答 */
		case D_API_MSGID_FILE_FIRM_UPDATE_RSP:
		/* REファイルデータ読込応答 */
		case D_API_MSGID_FILE_FIRM_READ_RSP:
		/* REファイルデータ読込中止応答 */
		case D_API_MSGID_FILE_FIRM_CANCEL_RSP:
			/* Event exist flag : ON */
			lwEventExist = CMD_ON;
			lwCpriNo = DLD_ALL_RE;
			break;
		/*  CPRI信号受信通知 */
		case CMD_TSKIF_CPRIRCVNTC:
			/* Check CPRI信号種別 */
			if( lwSignalKind == dlr_event_ctbl[lwEventId].addinfo1 )
			{
				/* Event exist flag : ON */
				lwEventExist = CMD_ON;
				lwCpriNo = DLD_ALL_RE;
			}
			else
			{
				lwEventId++;
			}
			break;
		/* タイムアウト発生通知 */
		case CMD_TSKIF_TIMOUTNTC:
			/* Check timer Id */
			if(	( dlr_event_ctbl[lwEventId].addinfo1 <= a_tskid ) && 
				( dlr_event_ctbl[lwEventId].addinfo2 >= a_tskid ))
			{
				/* Event exist flag : ON */
				lwEventExist = CMD_ON;
				m_dl_getCpriSys_from_timerid( a_tskid, dlr_event_ctbl[lwEventId].addinfo1, &lwCpriNo, &lwSysType );
			}
			else
			{
				lwEventId++;
			}
			break;
		/* REC Forwarding Notice */
		case CMD_TSKIF_REC_FWDNTC:
			/* Check CPRI信号種別 */
			if( lwSignalKind == dlr_event_ctbl[lwEventId].addinfo1 )
			{
				/* Event exist flag : ON */
				lwEventExist = CMD_ON;
				lwCpriNo = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)a_bufaddr_p)->cprircv_inf.link_num;
			}
			else
			{
				lwEventId++;
			}
			break;
		/* firmware erase response */
		case D_API_MSGID_FILE_FIRM_ERASE_RSP:
			return;
		/* その他 */
		default:
			cm_Assert(CMD_ASL_USELOW, a_event_no, "unknow event");
			return;
		}

		/* If event exist */
		if( CMD_ON == lwEventExist )
		{
			ge_dlw_main_event[lwSysType] = (EC_DLM_MAIN_EVT)lwEventId;
			break;
		}
	}

	if( DLD_ECTBL_END == dlr_event_ctbl[lwEventId].gbleventno )
	{
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s unknow event \n",__LINE__,__FUNCTION__);
		#endif
		cm_Assert(CMD_ASL_USELOW, a_event_no, "unknow event");
	}
	else
	{
		SAVE_LAST_CALL(dlr_mtrxtbl[lwEventId][(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta]);

		((CMT_TSKIF_HEAD*)a_bufaddr_p )->uiSrcPQueueID = lwSysType;
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s lwEventId = %d, lwSysType= %d, (*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = %d, lwCpriNo = %d\n",__LINE__,__FUNCTION__, lwEventId, lwSysType,
					(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta, lwCpriNo);
		#endif
		m_dl_cm_RunHisSet(DLD_RUNHIS_FLG_RCV, a_tskid, a_event_no, a_bufaddr_p);
		dlr_mtrxtbl[lwEventId][(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta]( a_bufaddr_p, lwCpriNo);
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s lwEventId = %d, lwSysType= %d, (*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = %d, lwCpriNo = %d\n",__LINE__,__FUNCTION__, lwEventId, lwSysType,
					(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta, lwCpriNo);
		#endif
	}

	return;
}
/* @} */
/*!
 *  @brief  Get CpriNo, Systerm type from timer Id
 *  @param  awTimerId [in]  Timer Id
 *  @param  awTimerIdMin [in]  Min Timer Id
 *  @param  awCpriNo_p [out] cpri link number
 *  @param  awSysType_p [out] system type
 *  @date   2015/08/22 FPT)Yen create for FHM
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.243対応
 *  @date   2015/10/13 TDIPS)sasaki Update MKレビュー指摘No.262対応
 */
VOID m_dl_getCpriSys_from_timerid( USHORT awTimerId, USHORT awTimerIdMin, USHORT* awCpriNo_p, USHORT* awSysType_p )
{
	USHORT	lwCpriNo = CMD_NUM0;
	USHORT	lwSysType = CMD_SYS_S3G;

	switch( awTimerIdMin )
	{
		case CMD_TIMID_FIRMWARESND_S3G:
		case CMD_TIMID_USEFIRMWARESND:
			lwCpriNo = DLD_ALL_RE;
			break;
		/* システム種別毎に独立 */
		case CMD_TIMID_SYSPRMSND_3G:
			lwCpriNo = DLD_ALL_RE;
			lwSysType = awTimerId - awTimerIdMin;
			break;
		/* 配下RE毎に独立 */
		case CMD_TIMID_FILEDATSND:
		case CMD_TIMID_FIRMFILE_INF:
		case CMD_TIMID_FIRMFILE_SNDFIN:
		case CMD_TIMID_UFIRMFILE_REP:
		case CMD_TIMID_UFIRMFILE_SNDFIN:
		case CMD_TIMID_UFIRMFILE_INF:
			lwCpriNo = awTimerId - awTimerIdMin + CMD_NUM1;
			break;
		/* 配下RE毎、さらに3G/LTEリンク毎に独立 */
		case CMD_TIMID_FIRMFILE_REP_3G:
		case CMD_TIMID_SYSPRM_INF:
		case CMD_TIMID_SYSPRM_SNDFIN:
			lwCpriNo = (awTimerId - awTimerIdMin) % D_RRH_CPRINO_RE_MAX + CMD_NUM1;
			lwSysType = (awTimerId - awTimerIdMin) / D_RRH_CPRINO_RE_MAX;
			break;
		default:
			break;
	}

	*awCpriNo_p = lwCpriNo;
	*awSysType_p = lwSysType;
	return;
}
/*!
 *  @brief  対RE状態遷移(FHM自律) main function
 *  @note   
 *  @param  *aBufaddr_p [in]  receving message address
 *  @param  awCpriNo [in]  Cpri No
 *  @return None.
 *  @date   2015/08/15 FPT)Yen create for FHM
 *  @date   2015/11/19 TDIPS)sasaki update REC CNT-N/E切替問題対応
 */

VOID m_dl_fhm_main( VOID* aBufaddr_p, USHORT awCpriNo )
{
	EC_DLM_MAIN_EVT		leEventTbl[EC_DLM_FHMDL_EVT_MAX] = 
							{ 	/* 0 */  EC_DLM_EVT_RESYSPARA_UPDATE_REQ         , /* 1: REシステムパラメータ更新要求 */
								/* 1 */  EC_DLM_EVT_REFILE_UPDATE_REQ            , /* 2: REファイル更新要求 */
								/* 2 */  EC_DLM_EVT_REFILE_UPDATE_STOP_REQ       , /* 3: REファイル更新中止要求 */
								/* 3 */  EC_DLM_EVT_REFILE_UPDATE_STOP_IND       , /* 4: REファイル更新中止指示 */
								/* 4 */  EC_DLM_EVT_LOADFIRM_RSP                 , /* 6: REファイルデータ読込応答 */
								/* 5 */  EC_DLM_EVT_CANCEL_LOADFIRM_RSP          , /* 7: REファイルデータ読込中止応答 */
								/* 6 */  EC_DLM_EVT_CPRI_FILEINFREQ              , /* 9: ファイル情報報告通知(対象データ:シスパラ) */
								/* 7 */  EC_DLM_EVT_CPRI_FILEINFRES              , /* 10: ファイル情報報告応答 */
								/* 8 */  EC_DLM_EVT_CPRI_FILSNDFINRES            , /* 13: ファイルデータ送信完了報告応答 */
								/* 9 */  EC_DLM_EVT_CPRI_FIRMFILEREQ             , /* 14: ファームウェアファイル報告要求 */
								/* 10 */ EC_DLM_EVT_CPRI_FIRMFILERES             , /* 15: ファームウェアファイル報告応答 */
								/* 11 */ EC_DLM_EVT_CPRI_UFIRMFILEREQ            , /* 21: 運用中ファームウェアファイル報告要求 */
								/* 12 */ EC_DLM_EVT_TO_FILEDATSND                , /* 23: ファイルデータ送信間隔タイマ */
								/* 13 */ EC_DLM_EVT_TO_SYSPRM_INF                , /* 24: システムパラメータ情報報告確認タイマ */
								/* 14 */ EC_DLM_EVT_TO_SYSPRM_SNDFIN             , /* 25: システムパラメータ送信完了確認タイマ */
								/* 15 */ EC_DLM_EVT_TO_FIRMFILE                  , /* 27: ファームウェアファイル報告確認タイマ */
								/* 16 */ EC_DLM_EVT_TO_FIRMFILE_INF              , /* 28: ファームウェアファイル情報報告確認タイマ */
								/* 17 */ EC_DLM_EVT_TO_FIRMFILE_SNDFIN             /* 29: ファームウェア送信完了確認タイマ */
							};

	USHORT				lwStsTbl[EC_DLM_FHMDL_STN_MAX] = 
							{	/* 0 */ DLD_RESTA_IDLE,							/* アイドル */
								/* 1 */ DLD_RESTA_FILEINF_SYSPRM_WAIT,			/* ファイル情報報告応答(シスパラ)待ち */
								/* 2 */ DLD_RESTA_FILE_COMP_SYSPRM_WAIT,		/* ファイルデータ送信完了報告応答(シスパラ)待ち */
								/* 3 */ DLD_RESTA_FWREP_WAIT,       			/* ファームウェアファイル報告応答待ち */
								/* 4 */ DLD_RESTA_FILEINF_WAIT,     			/* ファイル情報報告応答(FW)待ち */
								/* 5 */ DLD_RESTA_RE_FILEREAD,      			/* REファイルデータ読込中 */
								/* 6 */ DLD_RESTA_FILE_TRANS,       			/* ファイルデータ送信中(FW) */
								/* 7 */ DLD_RESTA_FILE_COMP_WAIT				/* ファイルデータ送信完了報告応答(FW)待ち */
							 };
	USHORT 				lwEventId;		/* event table index */
	USHORT 				lwStsId;		/* event table index */
	USHORT 				lwReNoMin;
	USHORT 				lwReNoMax;
	USHORT 				lwReCnt;
	USHORT				lwSysType = CMD_SYS_S3G;

	if( NULL == aBufaddr_p )
	{
		cm_Assert(	CMD_ASL_USELOW, CMD_NUM0, "Buffer null");
		return;
	}

	lwSysType = (( CMT_TSKIF_HEAD *)aBufaddr_p)->uiSrcPQueueID;

	/* Find Event Index */
	for(lwEventId = CMD_NUM0; lwEventId < EC_DLM_FHMDL_EVT_MAX; lwEventId++)
	{
		if( leEventTbl[lwEventId] == ge_dlw_main_event[lwSysType] )
		{
			break;
		}
	}

	if( DLD_ALL_RE == awCpriNo )
	{
		lwReNoMin = D_RRH_CPRINO_RE_MIN;
		lwReNoMax = D_RRH_CPRINO_RE_MAX;
	}
	else
	{
		lwReNoMin = awCpriNo;
		lwReNoMax = awCpriNo;
	}

	/* Loop all RE */
	for( lwReCnt = lwReNoMin; lwReCnt <= lwReNoMax; lwReCnt++ )
	{
		/* Find Status Index */
		for(lwStsId = CMD_NUM0; lwStsId < EC_DLM_FHMDL_STN_MAX; lwStsId++)
		{
			if( lwStsTbl[lwStsId] == gt_dlw_refw_all_tbl[lwSysType][lwReCnt - CMD_NUM1].re_sta )
			{
				break;
			}
		}

		if( ( lwEventId >= EC_DLM_FHMDL_EVT_MAX ) ||
			( lwStsId >= EC_DLM_FHMDL_STN_MAX ) )
		{
			cm_Assert(CMD_ASL_USELOW, lwEventId, "unknow event");
		}
		else
		{
			/* Call function */
			SAVE_LAST_CALL(dlr_fhm_mtrxtbl[lwEventId][lwStsId]);
			dlr_fhm_mtrxtbl[lwEventId][lwStsId]( aBufaddr_p, lwReCnt);
		}
	}
	return;
}

/*!
 *  @brief  対RE状態遷移(REC主導) main function
 *  @note   
 *  @param  *aBufaddr_p [in]  receving message address
 *  @param  awCpriNo [in]  Cpri No
 *  @return None.
 *  @date   2015/08/15 FPT)Yen create for FHM
 */

VOID m_dl_rec_main( VOID* aBufaddr_p, USHORT awCpriNo )
{
	EC_DLM_MAIN_EVT		leEventTbl[EC_DLM_RECDL_EVT_MAX] = 
							{	/* 0 */  EC_DLM_EVT_REFILE_UPDATE_STOP_REQ       , /* REファイル更新中止要求 */
								/* 1 */  EC_DLM_EVT_REFILE_UPDATE_STOP_IND       , /* REファイル更新中止指示 */
							 	/* 2 */  EC_DLM_EVT_USEREFILE_UPDATE_START_RSP   , /* 運用中REファイル更新開始応答 */
							 	/* 3 */  EC_DLM_EVT_CPRI_FILEINFREQ              , /* ファイル情報報告通知（対象データ：シスパラ） */
								/* 4 */  EC_DLM_EVT_CPRI_FILEINFRES              , /* ファイル情報報告応答 */
								/* 5 */  EC_DLM_EVT_CPRI_FILEDATSND				 , /* ファイルデータ送信 */
								/* 6 */  EC_DLM_EVT_CPRI_FILSNDFINNTC            , /* ファイルデータ送信完了報告通知 */
								/* 7 */  EC_DLM_EVT_CPRI_FILSNDFINRES            , /* ファイルデータ送信完了報告応答 */
								/* 8 */  EC_DLM_EVT_CPRI_FIRMFILEREQ             , /* ファームウェアファイル報告要求 */
								/* 9 */  EC_DLM_EVT_CPRI_FIRMFILERES             , /* ファームウェアファイル報告応答 */
							//	/* 10 */ EC_DLM_EVT_CPRI_UFILEINFREQ             , /* 運用中ファイル情報報告通知 */
								/* 11 */ EC_DLM_EVT_CPRI_UFILEINFRES             , /* 運用中ファイル情報報告応答 */
								/* 12 */ EC_DLM_EVT_CPRI_UFILEDATSND             , /* 運用中ファイルデータ送信 */
								/* 13 */ EC_DLM_EVT_CPRI_UFILSNDFINNTC           , /* 運用中ファイルデータ送信完了報告通知 */
								/* 14 */ EC_DLM_EVT_CPRI_UFILSNDFINRES           , /* 運用中ファイルデータ送信完了報告応答 */
								/* 15 */ EC_DLM_EVT_CPRI_UFIRMFILEREQ            , /* 運用中ファームウェアファイル報告要求 */
								/* 16 */ EC_DLM_EVT_CPRI_UFIRMFILERES            , /* 22: 運用中ファームウェアファイル報告応答 */
								/* 17 */ EC_DLM_EVT_TO_SYSPRM_INF                , /* システムパラメータ情報報告確認タイマ */
								/* 18 */ EC_DLM_EVT_TO_SYSPRM_SNDFIN             , /* システムパラメータ送信完了確認タイマ */
							//  /* 19 */ EC_DLM_EVT_TO_SYSPRMSND_S3G             , /* 26: システムパラメータ送信確認タイマ */
								/* 20 */ EC_DLM_EVT_TO_FIRMFILE                  , /* ファームウェアファイル報告確認タイマ */
								/* 21 */ EC_DLM_EVT_TO_FIRMFILE_INF              , /* ファームウェアファイル情報報告確認タイマ */
								/* 22 */ EC_DLM_EVT_TO_FIRMFILE_SNDFIN           , /* ファームウェア送信完了確認タイマ */
							//	/* 23 */ EC_DLM_EVT_TO_FIRMWARESND_S3G           , /* 30: ファームウェア送信確認タイマ */
								/* 24 */ EC_DLM_EVT_TO_UFIRMFILE                 , /* 31: ファームウェアファイル報告確認タイマ(更新用) */
								/* 25 */ EC_DLM_EVT_TO_UFIRMFILE_INF             , /* 32: ファームウェアファイル情報報告確認タイマ(更新用) */
								/* 26 */ EC_DLM_EVT_TO_UFIRMFILE_SNDFIN          , /* 33: ファームウェア送信完了確認タイマ(更新用) */
							//	/* 27 */ EC_DLM_EVT_TO_USEFIRMWARESND              /* 34: ファームウェア送信確認タイマ(更新用) */
							};
	USHORT				lwStsTbl[EC_DLM_RECDL_STN_MAX] = 
							{	/* 0 */ DLD_RESTA_IDLE,							/* アイドル */
								/* 1 */ DLD_RESTA_FILEINF_SYSPRM_WAIT,			/* ファイル情報報告応答(シスパラ)待ち */
								/* 2 */ DLD_RESTA_FILE_TRANS_WAIT,				/* ファイルデータ送信(シスパラ)待ち */
								/* 3 */ DLD_RESTA_FILE_COMP_SYSPRM_WAIT,		/* ファイルデータ送信完了報告応答(シスパラ)待ち */
								/* 4 */ DLD_RESTA_FWREP_WAIT,       			/* ファームウェアファイル報告応答待ち */
								/* 5 */ DLD_RESTA_FILEINF_WAIT,     			/* ファイル情報報告応答(FW)待ち */
								/* 6 */ DLD_RESTA_FILE_TRANS,       			/* ファイルデータ送信中(FW) */
								/* 7 */ DLD_RESTA_FILE_COMP_WAIT,				/* ファイルデータ送信完了報告応答(FW)待ち */
								/* 8 */ DLD_RESTA_USE_FWREP_WAIT,				/* 運用中ファームウェアファイル報告応答待ち */
								/* 9 */ DLD_RESTA_USE_FILEINF_WAIT,				/* 運用中ファイル情報報告応答(FW)待ち */
								/* 10*/ DLD_RESTA_USE_FILE_TRANS,				/* 運用中ファイルデータ送信中(FW) */
								/* 11*/ DLD_RESTA_USE_FILE_COMP_WAIT			/* 運用中ファイルデータ送信完了報告応答(FW)待ち */
							 };
	USHORT 				lwEventId;		/* event table index */
	USHORT 				lwStsId;		/* event table index */
	USHORT 				lwReNoMin;
	USHORT 				lwReNoMax;
	USHORT 				lwReCnt;
	USHORT				lwSysType = CMD_SYS_S3G;
#ifdef FHM_DLM_FOR_DEBUG_IT1
	USHORT				lwStsBefore;
#endif
	if( NULL == aBufaddr_p )
	{
		cm_Assert(	CMD_ASL_USELOW, CMD_NUM0, "Buffer null");
		return;
	}

	lwSysType = (( CMT_TSKIF_HEAD *)aBufaddr_p)->uiSrcPQueueID;
	
	/* Find Event Index */
	for(lwEventId = CMD_NUM0; lwEventId < EC_DLM_RECDL_EVT_MAX; lwEventId++)
	{
		if( leEventTbl[lwEventId] == ge_dlw_main_event[lwSysType] )
		{
			break;
		}
	}

	if( EC_DLM_RECDL_EVT_MAX == lwEventId )
	{
		cm_Assert(	CMD_ASL_USELOW, CMD_NUM0, "Event not found");
		return;
	}

	if( DLD_ALL_RE == awCpriNo )
	{
		lwReNoMin = D_RRH_CPRINO_RE_MIN;
		lwReNoMax = D_RRH_CPRINO_RE_MAX;
	}
	else
	{
		lwReNoMin = awCpriNo;
		lwReNoMax = awCpriNo;
	}

	/* Loop all RE */
	for( lwReCnt = lwReNoMin; lwReCnt <= lwReNoMax; lwReCnt++ )
	{
		/* Find Status Index */
		for(lwStsId = CMD_NUM0; lwStsId < EC_DLM_RECDL_STN_MAX; lwStsId++)
		{
			if( lwStsTbl[lwStsId] == gt_dlw_refw_all_tbl[lwSysType][lwReCnt - CMD_NUM1].re_sta )
			{
				break;
			}
		}

		if( ( lwEventId >= EC_DLM_RECDL_EVT_MAX ) ||
			( lwStsId >= EC_DLM_RECDL_STN_MAX ) )
		{
			cm_Assert(CMD_ASL_USELOW, lwEventId, "unknow event");
		}
		else
		{
			#ifdef FHM_DLM_FOR_DEBUG_IT1
			lwStsBefore = gt_dlw_refw_all_tbl[lwSysType][lwReCnt - CMD_NUM1].re_sta;
			#endif
			/* Call function */
			dlr_rec_mtrxtbl[lwEventId][lwStsId]( aBufaddr_p, lwReCnt);
			#ifdef FHM_DLM_FOR_DEBUG_IT1
			printf( "[%d]%s (lwEventId = %d, lwStsId = %d, awCpriNo = %d) ReSts : %d --> %d \n",
				__LINE__,__FUNCTION__, lwEventId, lwStsId, lwReCnt, lwStsBefore, gt_dlw_refw_all_tbl[lwSysType][lwReCnt - CMD_NUM1].re_sta);
			#endif
		}
	}
	return;
}

/* @} */


/********************************************************************************************************************/
/**
 *  @brief    file download sending and receiving running history		
 *  @note	    file download sending and receiving running history	
 *												
 *  @param  	sndrcv_flg				[in]		sending and recieving flag
 *  @param     task_id                               [in]         task ID
 *  @param     event_id                             [in]         event ID
 *  @param     *datap                               [in]        data pointer
 *  @return   Result code is returned
 *  @retval   -
 *  @date     2014/01/16  FFCS)linb Create
 */
/********************************************************************************************************************/
VOID m_dl_cm_RunHisSet(UINT sndrcv_flg, UINT task_id, UINT event_id, UCHAR *datap) 
{
	T_RRH_SYSTEM_TIME	datetime;
	UINT		sig_kind;		
	UINT		datas[10] = {0};
	CHAR 		*inf_p = NULL;
	
	sig_kind = CMD_ALLF_SHORT;
	/* 0927 breakをreturnに */
	switch(event_id)
	{
		case CMD_TSKIF_INIREQ:			
			return;

		/* REシステムパラメータ更新要求 */
		case CMD_TSKIF_RESYSPARA_UPDATE_REQ:
			datas[0] = ((CMT_TSKIF_RESYSNEWREQ *)datap)->cpri_no;	
			datas[1] = ((CMT_TSKIF_RESYSNEWREQ *)datap)->dummy;
			break;
		
		 /* REファイル更新要求 */
		case CMD_TSKIF_REFIRM_DL_REQ:
			datas[0] = ((CMT_TSKIF_REFILNEWREQ *)datap)->cpri_no;	
			datas[1] = ((CMT_TSKIF_REFILNEWREQ *)datap)->sys_type;
			break;
		
		/* 運用中REファイル更新開始応答 */
		case CMD_TSKIF_USEREFILE_UPDATE_START_RSP:
			datas[0] = ((CMT_TSKIF_CPRINOGET *)datap)->cpri_no;	
			break;
		
		/* REファイル更新中止要求 */
		case CMD_TSKIF_REFIRM_DL_STOP_REQ:
			datas[0] = ((CMT_TSKIF_REFILNEWSTP *)datap)->cpri_no;	
			datas[1] = ((CMT_TSKIF_REFILNEWSTP *)datap)->systerm;
			break;
		
		/* REファイル更新中止指示 */
		case CMD_TSKIF_REFIRM_DL_STOP_IND:
			datas[0] = DLD_ALL_RE;	
			datas[1] = ((CMT_TSKIF_REFILNEWSTP *)datap)->systerm;
			break;
		
		/* REファイルデータ読込応答 */
		case D_API_MSGID_FILE_FIRM_READ_RSP:
			datas[0] = DLD_ALL_RE;	
			datas[1] = ((T_API_FILE_LOAD_FW_DATA_RSP *)datap)->data.result;
			break;
		/* REファイルデータ読込中止応答 */
		case D_API_MSGID_FILE_FIRM_CANCEL_RSP:
			datas[0] = DLD_ALL_RE;	
			datas[1] = ((T_API_FILE_FIRM_CANCEL_RSP *)datap)->data.result;
			break;
		
		/* REC Forwarding Notice */
		case CMD_TSKIF_REC_FWDNTC:
			sig_kind = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)datap)->signal_kind;
			datas[0] = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)datap)->cprircv_inf.link_num;	
			break;
		
		case CMD_TSKIF_CPRIRCVNTC:
			sig_kind = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datap)->signal_kind;
			inf_p = (CHAR*)(&(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datap)->signal_kind));
			switch(sig_kind & CMD_SYS_MSK_CPRID)
			{
				case CMD_CPRID_FILEINFREQ:
					datas[0] = ((CMT_CPRIF_FILEINFREQ *)inf_p)->data;
					datas[1] = ((CMT_CPRIF_FILEINFREQ *)inf_p)->size;
					cm_MemCpy(&datas[2], ((CMT_CPRIF_FILEINFREQ *)inf_p)->filename, 20);
					break;
					
				case CMD_CPRID_FILEDATSND:
					datas[0] = ((CMT_CPRIF_FILEDATSND *)inf_p)->sgmtno1;
					datas[1] = ((CMT_CPRIF_FILEDATSND *)inf_p)->sgmtno2;
					datas[2] = ((CMT_CPRIF_FILEDATSND *)inf_p)->datsize1;
					datas[3] = ((CMT_CPRIF_FILEDATSND *)inf_p)->datsize2;
					return;		/*	走行履歴を取得しない	*/

				case CMD_CPRID_FILSNDFINNTC:
					datas[0] = ((CMT_CPRIF_FILSNDFINNTC *)inf_p)->tgtdat;
					datas[1] = ((CMT_CPRIF_FILSNDFINNTC *)inf_p)->chksum;					
					break;

				case CMD_CPRID_FIRMFILEREQ:
					return;

				case CMD_CPRID_UFILEINFREQ:
					datas[0] = ((CMT_CPRIF_UFILEINFREQ *)inf_p)->data;
					datas[1] = ((CMT_CPRIF_UFILEINFREQ *)inf_p)->size;					
					cm_MemCpy(&datas[2], ((CMT_CPRIF_UFILEINFREQ *)inf_p)->filename, 20);
					break;

				case CMD_CPRID_UFILEDATSND:
					datas[0] = ((CMT_CPRIF_UFILEDATSND *)inf_p)->sgmtno1;
					datas[1] = ((CMT_CPRIF_UFILEDATSND *)inf_p)->sgmtno2;
					datas[2] = ((CMT_CPRIF_UFILEDATSND *)inf_p)->datsize1;
					datas[3] = ((CMT_CPRIF_UFILEDATSND *)inf_p)->datsize2;					
					return;		/*	走行履歴を取得しない	*/

				case CMD_CPRID_UFILSNDFINNTC:
					datas[0] = ((CMT_CPRIF_UFILSNDFINNTC *)inf_p)->tgtdat;
					datas[1] = ((CMT_CPRIF_UFILSNDFINNTC *)inf_p)->chksum;										
					break;

				case CMD_CPRID_UFIRMFILEREQ:
					return;
					
				default:
					return;
			}
			break;
			
		case D_API_MSGID_FILE_FIRM_UPDATE_RSP:		
			datas[0] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.dlwType;	
			datas[1] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.dlwResult;
			datas[2] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.errMsg[0];
			datas[3] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.errMsg[1];
			datas[4] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.errMsg[2];
			datas[5] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.errMsg[3];
			datas[6] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.errMsg[4];
			datas[7] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.errMsg[5];
			datas[8] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.errMsg[6];
			datas[9] = ((T_API_FILE_UPDATE_FIRM_RSP *)datap)->data.errMsg[7];
			break;
			
		case D_API_MSGID_FILE_FIRM_ERASE_RSP:	
			return;
			
		case CMD_TSKIF_TIMOUTNTC:
			break;

		case CMD_TSKIF_SYSPARA_CHG_NTC:
			datas[0] = ((CMT_TSKIF_SYSPARA_CHG_NTC *)datap)->inf.tx_sys_num_old;
			datas[1] = ((CMT_TSKIF_SYSPARA_CHG_NTC *)datap)->inf.tx_sys_num_new;
			break;
			
		default:
			return;	
	}

	cm_RtcGet( &datetime );

	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.setFlg	 = CMD_ON;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.month      = datetime.month;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.day        = datetime.day;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.hour       = datetime.hour;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.min        = datetime.min;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.sec        = datetime.sec;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.msec       = datetime.msec;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.sndrcv_flg = (UCHAR)sndrcv_flg;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.event_id   = event_id;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.task_id    = (USHORT)task_id;
	dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].head.sig_kind   = (USHORT)sig_kind;
	cm_MemCpy(dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx].body,datas,sizeof(datas));

	dlw_runhis_tbl.write_idx++;
	if (dlw_runhis_tbl.write_idx >= DLD_RUNHIS_FACENUM_MAX)
	{
		dlw_runhis_tbl.write_idx = CMD_NUM0;
	}
	
	cm_MemPadc((UCHAR*)&dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx], CMD_NUM0,
				sizeof(dlw_runhis_tbl.inf[dlw_runhis_tbl.write_idx]));

	return;
}

VOID m_dl_logPrint(FILE *outf)
{
	UINT index;
	UINT bodyIdx;
	CHAR *strSndRcv[] = {"RCV","SND"};
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

	fprintf(outf,"L3 dlm running history----------------------------------------\n");
	
	for(index=0; index<DLD_RUNHIS_FACENUM_MAX; index++)
	{
		if(dlw_runhis_tbl.inf[index].head.setFlg == 0)
			continue;

		fprintf(outf,"No.%03d [%02x-%02x %02x:%02x:%02x.%03x] %s;%08x;%04x;%04x\n",
			index,
			dlw_runhis_tbl.inf[index].head.month,
			dlw_runhis_tbl.inf[index].head.day,
			dlw_runhis_tbl.inf[index].head.hour,
			dlw_runhis_tbl.inf[index].head.min,
			dlw_runhis_tbl.inf[index].head.sec,
			dlw_runhis_tbl.inf[index].head.msec,
			strSndRcv[dlw_runhis_tbl.inf[index].head.sndrcv_flg],
			dlw_runhis_tbl.inf[index].head.event_id,
			dlw_runhis_tbl.inf[index].head.task_id,
			dlw_runhis_tbl.inf[index].head.sig_kind);
		
		for(bodyIdx = 0;bodyIdx < 10;bodyIdx++)
		{
			fprintf(outf," %02x%02x %02x%02x",
				dlw_runhis_tbl.inf[index].body[bodyIdx * 4],
				dlw_runhis_tbl.inf[index].body[bodyIdx * 4 + 1],
				dlw_runhis_tbl.inf[index].body[bodyIdx * 4 + 2],
				dlw_runhis_tbl.inf[index].body[bodyIdx * 4 + 3]);

			if((bodyIdx % 5) == 4) fprintf(outf,"\n");
		}
	}
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

	return;
}

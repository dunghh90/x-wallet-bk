/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_syspsndcmp_ntc.c
 * @brief  the processing for "ファイルデータ送信完了報告通知" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/15 TDIPS)sasaki Update
 * @date   2015/10/26 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */
/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */
#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"
/*!
 *  @brief  the processing for "ファイルデータ送信完了報告通知" receiving at the REC主導ファイルデータ送信(シスパラ)待ち.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01824
 *  @Bug_No M-RRU-ZSYS-01862
 *  @Bug_No M-RRU-ZSYS-01906
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/08/24 FPT)Yen update
 *  @date   2015/10/08 FPT)Duong update MKレビュー指摘No.198対応
 *  @date   2015/10/12 TDIPS)Lay M-RRU-ZSYS-01862 Update value for 2 fields of variable
 *  @date   2015/10/15 TDIPS)sasaki update IT2問処No.90対応
 *  @date   2015/10/21 FPT)Yen M-RRU-ZSYS-01824 update ((3G)システムパラメータが範囲外: 応答結果 -> 3:情報NG(3G))
 *  @date   2015/10/26 TDIPS)sasaki M-RRU-ZSYS-01906 Update IT2問処No.139対処
 *  @date   2015/11/13 TDIPS)Quynh Update for その他未実装-015
 */
VOID m_dl_rl_fsnd_waiting_syspsndcmp_ntc(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILSNDFINNTC*			ltFilSndFin_p;
	USHORT			lwSysType;
	USHORT			lwTimerId;
	INT				errcd;
	INT				llResult;
	USHORT			lwTgtdat;
	USHORT			lwChksum;
	USHORT			lwChksumCalc;
	USHORT			lwChksumNG[CMD_MAX_SYSNUM] = {	CMD_CHKSUM_NG,
													CMD_CHKSUM_NG_S3G
								 				 };
	USHORT			lwParamNG[CMD_MAX_SYSNUM] = {	CMD_INF_NG,
													CMD_INF_NG_S3G
								 				 };
	UCHAR*			dlw_rcvsyspa_tbl[CMD_MAX_SYSNUM] =
							{ dlw_rcvsyspa_tbl_3g, dlw_rcvsyspa_tbl_s3g };
	USHORT			lwS3gBandwidth	= CMD_NUM0;				/* LTE帯域幅 	*/
	USHORT			lwTxSysNum	= CMD_NUM0;					/* 送信系統数 	*/

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_syspsndcmp_ntc] ENTER" );
	
	/* CPRI信号データ部取得 */
	ltFilSndFin_p = (CMT_TSKIF_CPRIRCV_FILSNDFINNTC*)bufp;

	lwSysType = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;

	/* タイマ停止システムパラメータ送信確認タイマ */
	lwTimerId = CMD_TIMID_SYSPRMSND_3G + lwSysType;
	llResult = cm_TStop(lwTimerId, &errcd);
	/* Stop timer NG */
	if( llResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, llResult, "システムパラメータ送信確認タイマ Stop NG" );
	}

	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);

	/* タイマ起動システムパラメータ送信完了確認タイマ */
	m_dl_fhm_sysp_filedatasendfin_timersta( CMD_NUM1, lwSysType ); /* All Re start 1 timer -> fix CpriNo = 1 */

	/* REC向けファイルデータ送信完了報告応答結果設定 */
	/* 対象データを取得 */
	lwTgtdat = ltFilSndFin_p->cpridat_filsndfinntc.tgtdat;
	/* チェックサムを取得 */
	lwChksum = ltFilSndFin_p->cpridat_filsndfinntc.chksum;
	/* チェックサム */
	cm_CheckSumCalcCpri((VOID*)(dlw_rcvsyspa_tbl[lwSysType]),
						(*gt_dlw_rec_all_tbl[lwSysType]).rcv_data_size, &lwChksumCalc);
	
	/* チェックサムNG */
	if(lwChksum != lwChksumCalc)
	{
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s lwChksum: %x, lwChksumCalc: %x\n",__LINE__,__FUNCTION__, lwChksum, lwChksumCalc);
#endif
		BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR, "check sum Error(lwChksum=0x%04x/lwChksumCalc=0x%04x )", lwChksum, lwChksumCalc);
		(*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt = lwChksumNG[lwSysType];
	}
	/* 分割番号不一致 or 受信済みデータサイズとファイル情報報告通知のトータルデータサイズ不一致 or 対象データ不一致  */
	else if(((gt_dlw_mnt_all_reinf[lwSysType][CMD_NUM0]).div_num != (*gt_dlw_rec_all_tbl[lwSysType]).rcv_div_no) ||
		    ((gt_dlw_mnt_all_reinf[lwSysType][CMD_NUM0]).file_size != (*gt_dlw_rec_all_tbl[lwSysType]).rcv_data_size) ||
		    ( CMD_SYSPAR != lwTgtdat))
	{
		BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR, "dwl file Error(date=%d/div_num=%d/rcv_div=%d )", 
								lwTgtdat, (gt_dlw_mnt_all_reinf[lwSysType][CMD_NUM0]).div_num, (*gt_dlw_rec_all_tbl[lwSysType]).rcv_div_no );
		BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR, "dwl file Error(file_size=%d/rcv_size=%d )", 
								(gt_dlw_mnt_all_reinf[lwSysType][CMD_NUM0]).file_size, (*gt_dlw_rec_all_tbl[lwSysType]).rcv_data_size );
		/* 221:ダウンロードファイル異常 */
		(*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt = CMD_REFW_NG;
	}
	else if( CMD_SYS_S3G == lwSysType )
	{
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s cmw_tra_sv_tbl.carr_onoff: %x \n",__LINE__,__FUNCTION__, cmw_tra_sv_tbl.carr_onoff );
#endif
		/* キャリア状態が使用中の場合*/
		if( 0 != (cmw_tra_sv_tbl.carr_onoff & CMD_CARDSV_S3G_USE_BIT) )
		{
			/* Get lte bandwidth (new)*/
			lwS3gBandwidth = ( dlw_rcvsyspa_tbl_s3g[CMD_NUM17 * CMD_NUM2] << DLD_BIT8 ) | 
							 ( dlw_rcvsyspa_tbl_s3g[CMD_NUM17 * CMD_NUM2 + CMD_NUM1] );
			/* Get tx system number (new)*/
			lwTxSysNum = ( dlw_rcvsyspa_tbl_s3g[CMD_NUM19 * CMD_NUM2] << DLD_BIT8 ) | 
					 	 ( dlw_rcvsyspa_tbl_s3g[CMD_NUM19 * CMD_NUM2 + CMD_NUM1] );
			/* 「変数名E：送信系統数」および「変数名E：LTE帯域幅」が変更された場合 */
			if( lwS3gBandwidth != cmw_sys_mng_tbl[CMD_SYS_S3G].s3g_bandwidth &&
				lwTxSysNum     != cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num )
			{
				BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR, "s3g_bandwidth change ( %d<-%d )! or tx_sys_num change(%d<-%d)", 
										cmw_sys_mng_tbl[CMD_SYS_S3G].s3g_bandwidth, lwS3gBandwidth, cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num, lwTxSysNum );
				/* 224：状態異常 */
				(*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt = CMD_SYSP_STA_NG;
			}
		}

		if( CMD_NML == (*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt ) 
		{
			/* CPRIレート上で対応できないLTE帯域/送信系統数指定によるNG 
			ハード設定でのガードも入れておりますが、対応できない指定の場合、NG応答する形とする。 */
			llResult = m_dl_CheckValidSyspara();
			if( CMD_RES_OK != llResult )
			{
				/* 225：パラメータ未対応 */
				(*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt = CMD_SYSP_PARA_INVALID;
			}
		}
	}

	if( CMD_NML == (*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt ) 
	{
		/* システムパラメータ設定処理を起動 */
		llResult = m_dl_SysParaSet(lwSysType);
		/* システムパラメータが範囲外 */
		/* REC向けファイルデータ送信完了報告応答結果設定 */
		/* 3:情報NG(3G), 62:情報NG(LTE) */
		/* else 0:正常 */
		if(llResult != CMD_RES_OK)
		{
			(*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt = lwParamNG[lwSysType];
		}
	}
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s (*gt_dlw_rec_all_tbl[%d]).filedata_trans_rslt: %d \n",__LINE__,__FUNCTION__, lwSysType, (*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt );
#endif
	/* 5: ファイルデータ送信完了報告応答(シスパラ)集約中 */
	(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_FINREP_SYSPARA_CON;

	m_dl_rl_fsndcmprsp_waiting_sysp_proc(lwSysType);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_syspsndcmp_ntc] RETURN" );
	
	return ;
}

/* @} */

/*!
 *  @brief  
 *  @note   CPRIレート上で対応できないLTE帯域/送信系統数指定によるNG
 *          ハード設定でのガードも入れておりますが、対応できない指定の場合、NG応答する形とする。
 *  
 *  @return UINT
 *  @retval CMD_RES_OK / CMD_RES_NG
 *  @date   2015/11/09 FPT)Quynh create
 */
UINT m_dl_CheckValidSyspara()
{
	USHORT			lwS3gBandwidth  = CMD_NUM0;					/* 変数名E：LTE帯域幅    */
	USHORT			lwTxSysNum	    = CMD_NUM0;					/* 変数名E：送信系統数   */
	USHORT			lwCarId;
    USHORT			lwCarS3gBandwidth[CMD_MAX_CARNUM] = {0};	/* キャリア#nのLTE帯域幅 */
	UINT			llResult        = CMD_RES_OK;

	/* Get lte bandwidth (new)*/
	lwS3gBandwidth = ( dlw_rcvsyspa_tbl_s3g[CMD_NUM17 * CMD_NUM2] << DLD_BIT8 ) | 
					 ( dlw_rcvsyspa_tbl_s3g[CMD_NUM17 * CMD_NUM2 + CMD_NUM1] );
	/* Get tx system number (new)*/
	lwTxSysNum = ( dlw_rcvsyspa_tbl_s3g[CMD_NUM19 * CMD_NUM2] << DLD_BIT8 ) | 
				 ( dlw_rcvsyspa_tbl_s3g[CMD_NUM19 * CMD_NUM2 + CMD_NUM1] );
    /* LTE帯域幅 */
    for( lwCarId = 0; lwCarId < CMD_MAX_CARNUM; lwCarId++ )
    {
        lwCarS3gBandwidth[lwCarId]  = (USHORT)(lwS3gBandwidth >> ( 3 * lwCarId ));
        lwCarS3gBandwidth[lwCarId] &= (USHORT)(BIT0|BIT1|BIT2);
    }

    switch( cmw_linebitrate )
	{
	/* CPRIレート4.9Gbpsの場合 */
	case CMD_LINE_BITRATE_49G:
		if( (CMD_NUM0 == lwTxSysNum) || (CMD_NUM1 == lwTxSysNum) )
		{
			/* 送信系統数が0(2以下)/ 1(4以下)の場合、キャリア#3以降のLTE帯域幅が、0以外の場合 */
			for (lwCarId = 2; lwCarId < CMD_MAX_CARNUM; lwCarId++)
			{
				if (0 != lwCarS3gBandwidth[lwCarId]) 
				{
					llResult = CMD_RES_NG;
					break;
				}
			}
			/* 送信系統数が1(4以下)の場合、LTE帯域幅が、2キャリア以上0以外が存在する場合 */
			if( (CMD_RES_NG != llResult)    &&
				(CMD_NUM1 == lwTxSysNum)    &&
				(0 != lwCarS3gBandwidth[0]) && (0 != lwCarS3gBandwidth[1]) )
			{
				llResult = CMD_RES_NG;
			}
		}
		/* 送信系統数が2(8以下)の場合 */
		else if( CMD_NUM2 == lwTxSysNum )
		{
			llResult = CMD_RES_NG;
		}
		break;
	/* CPRIレート9.8Gbpsの場合 */
	case CMD_LINE_BITRATE_98G:
		/* 送信系統数が0(2以下)/ 1(4以下)の場合、キャリア#3以降のLTE帯域幅が、0以外の場合 */
		if( (CMD_NUM0 == lwTxSysNum) || (CMD_NUM1 == lwTxSysNum) )
		{
			for (lwCarId = 2; lwCarId < CMD_MAX_CARNUM; lwCarId++)
			{
				if (0 != lwCarS3gBandwidth[lwCarId]) 
				{
					llResult = CMD_RES_NG;
					break;
				}
			}
		}
		/* 送信系統数が2(8以下)の場合、キャリア＃2以降のLTE帯域幅が、0以外の場合 */
		else if ((CMD_NUM2 == lwTxSysNum))
		{
			for (lwCarId = 1; lwCarId < CMD_MAX_CARNUM; lwCarId++)
			{
				if (0 != lwCarS3gBandwidth[lwCarId]) 
				{
					llResult = CMD_RES_NG;
					break;
				}
			}
		}
		break;
	default:
		break;
	}

	if( CMD_RES_OK != llResult )
	{
		BPF_COM_LOG_ASSERT_DWL( D_RRH_LOG_AST_LV_ERROR, "Invalid s3gBandWidth(%d) & TxSysNum(%d)", lwS3gBandwidth, lwTxSysNum );
	}

    return llResult;

}


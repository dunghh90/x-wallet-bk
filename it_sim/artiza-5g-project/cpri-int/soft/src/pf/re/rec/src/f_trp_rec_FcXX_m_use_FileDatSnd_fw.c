/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_FileDatSnd_fw.c
 * @brief  Forward ファイルデータ送信	l3/dlm -> re/rec -> l2/lpb
 * @date   2015/08/21 FPT)Yen create
 * @date   2015/09/30 FPT)sasaki update
 * @date   2015/10/01 TDIPS)sasaki update
 * @date   2015/10/02 TDIPS)sasaki update
 * @date   2015/10/16 TDIPS)sasaki update
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  
 * @note  
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01972
 * @date   2015/11/09 FPT)Yen M-RRU-ZSYS-01972 create ( bug IT2 No165) 配下RE強制停止状態の場合は配下REへ転送しない
 */
/****************************************************************************/
USHORT f_trp_rec_FcXX_m_frcstp_refilnewstp(                       /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
)
{
	USHORT						system;
	USHORT						cpriNo;
	USHORT						lwReSts;
	CMT_TSKIF_REFILNEWSTP		ltReFilNewStp;
	USHORT						lwLength;
    INT    						sit_ret;            /* 終了コード */
    CHAR  						errdata[CMD_NUM48];

	cpriNo = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->cprircv_inf.link_num;
	system = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind & CMD_SYS_CHK_CPRID;

	if( CMD_SYS_3G == system )
	{
		lwReSts = f_trp_rec_Fcom_t_c02stn_get_3g(cpriNo);
	}
	else
	{
		lwReSts = f_trp_rec_Fcom_t_c02stn_get(cpriNo);
	}

	if( EC_REC_C02_STN_FRCSTP != lwReSts )
	{
		return CMD_NG;
	}

	ltReFilNewStp.head.uiEventNo = CMD_TSKIF_REFIRM_DL_STOP_REQ;
    ltReFilNewStp.cpri_no = cpriNo;
	ltReFilNewStp.systerm = system;

    /******************/
    /* スレッド間送信 */
    /******************/
    lwLength = sizeof(CMT_TSKIF_REFILNEWSTP);

    /* REファイル更新中止要求を送信 */
    sit_ret = f_trp_rec_Fcom_t_msg_snd( &ltReFilNewStp,
                            CMD_TSKIF_REFIRM_DL_STOP_REQ,
                            D_RRH_PROCQUE_L3,
                            0, lwLength);

    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IT send NG.", cpriNo, sit_ret);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(&ltReFilNewStp, sizeof(ltReFilNewStp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
    }

	return CMD_OK;
}
/****************************************************************************/
/*!
 * @brief  Receive msg ファイルデータ送信 from l3/dlm
 * @note   Receive msg ファイルデータ送信 from l3/dlm
 *         - send [ファイルデータ送信] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01972
 * @date   2015/08/21 FPT)Yen create
 * @date   2015/10/09 FPT)Yen update メッセージサイズ
 * @date   2015/11/09 FPT)Yen M-RRU-ZSYS-01972 fix bug IT2 No165 配下RE強制停止状態の場合は配下REへ転送しない
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FileDatSnd_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	USHORT		lwRet;

	if( NULL == bufp )
	{
		return;
	}

	lwRet = f_trp_rec_FcXX_m_frcstp_refilnewstp(bufp);
	if( CMD_OK == lwRet )
	{
		return;
	}

	/* Send msg [ファイルデータ送信] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, ((CMT_TSKIF_CPRISND_FILEDATSND *)bufp)->head.uiLength);

	return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg 運用中ファイルデータ送信 from l3/dlm
 * @note   Receive msg 運用中ファイルデータ送信 from l3/dlm
 *         - send [運用中ファイルデータ送信] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create
 * @date   2015/10/09 FPT)Yen update メッセージサイズ
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_UFileDatSnd_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [運用中ファイルデータ送信] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, ((CMT_TSKIF_CPRISND_UFILEDATSND *)bufp)->head.uiLength );

    return;
}


/****************************************************************************/
/*!
 * @brief  Receive msg ファイルデータ送信転送指示 from l3/dlm
 * @note   Receive msg ファイルデータ送信転送指示 from l3/dlm
 *         - send [ファイルデータ送信] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/09/30 TDIPS)sasaki create
 * @date   2015/10/01 TDIPS)sasaki update ファイルデータ送信用バッファをスタックに配置するよう修正
 * @date   2015/10/02 TDIPS)sasaki update 運用中DLにも対応
 * @date   2015/10/16 TDIPS)sasaki update 10KB未満のファイルデータ送信時にメッセージ長不正となる問題対処
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FileDatSndInd_fw(                       /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	CMT_TSKIF_CPRIRCV_FILEDATSNDINT*	pFileDatSndInd = (CMT_TSKIF_CPRIRCV_FILEDATSNDINT *)bufp;
	CMT_TSKIF_CPRIRCV_FILEDATSND		ltFileDatSnd;
	ULONG								llSize;
	UCHAR*								pShmBuf;

	if( NULL == bufp )
	{
		return;
	}

	ltFileDatSnd.head.uiEventNo = CMD_TSKIF_RE_FWDNTC;
	ltFileDatSnd.head.uiSignalkind = pFileDatSndInd->cpridat_filedatsndint.signal_kind;
	ltFileDatSnd.cprircv_inf.link_num = pFileDatSndInd->cprircv_inf.link_num;

	ltFileDatSnd.cpridat_filedatsnd.signal_kind = pFileDatSndInd->cpridat_filedatsndint.signal_kind;
	ltFileDatSnd.cpridat_filedatsnd.sgmtno1 = pFileDatSndInd->cpridat_filedatsndint.sgmtno1;
	ltFileDatSnd.cpridat_filedatsnd.sgmtno2 = pFileDatSndInd->cpridat_filedatsndint.sgmtno2;
	ltFileDatSnd.cpridat_filedatsnd.datsize1 = pFileDatSndInd->cpridat_filedatsndint.datsize1;
	ltFileDatSnd.cpridat_filedatsnd.datsize2 = pFileDatSndInd->cpridat_filedatsndint.datsize2;

	llSize = (ltFileDatSnd.cpridat_filedatsnd.datsize1 << 16) + ltFileDatSnd.cpridat_filedatsnd.datsize2;
	pShmBuf = f_trp_rec_fwdata_area + (sizeof(UINT) * 8) + pFileDatSndInd->cpridat_filedatsndint.offset;
	memcpy(ltFileDatSnd.cpridat_filedatsnd.fildat, pShmBuf, llSize);

	f_trp_rec_Fcom_t_l2_msg_fw(&ltFileDatSnd, (sizeof(CMT_TSKIF_CPRISND_FILEDATSND) - CMD_FILEDAT_MAX + llSize));
	return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

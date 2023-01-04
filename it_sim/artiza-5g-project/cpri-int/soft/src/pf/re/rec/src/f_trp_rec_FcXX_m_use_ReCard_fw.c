/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ReCard_fw.c
 * @brief  Forward REカード制御
 * @date   2015/10/01 TDI)satou create
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"
// [16B] add start
//#include "m_cm_header.h"
// [16B] add end

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg [REカード制御要求] from l3/rct
 * @note   Receive msg [REカード制御要求] from l3/rct
 *         - send [REカード制御要求] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/10/01 TDI)satou create
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ReCardReq_fw(VOID *bufp) {
    CMT_TSKIF_HEAD *head;
#if 0  // [16B] del start
// [16B]
	CMT_TSKIF_CPRIRCV_RECARDCNTREQ *recardcntreq_p;
	USHORT slot_no;  // スロット番号(1-56)
	USHORT link_no;  // リンク番号
    USHORT sigsys_rslt;                     /*  system by checking signal */
    USHORT slot_idx; // スロット番号(index:0-15)
	UINT	sigchk_rslt;						/*  result of checking signal */
//    USHORT sig;
// [16B]
#endif // [16B] del end

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if( NULL == bufp )
    {
        return;
    }
#if 0  // [16B] del start
// [16B] add start
    /* validate signal */
	recardcntreq_p = (CMT_TSKIF_CPRIRCV_RECARDCNTREQ*)bufp;
//    sigchk_rslt = m_cm_chksig((CMT_TSKIF_CPRIRCV_RECARDCNTREQ*)bufp->cpridat_recardcntreq.signal_kind,&sigsys_rslt);
    sigchk_rslt = m_cm_chksig(recardcntreq_p->cpridat_recardcntreq.signal_kind,&sigsys_rslt);

    if(sigchk_rslt != CMD_RES_OK)
    {
        cm_Assert(	CMD_ASL_USELOW,
                    sigsys_rslt,
                    "validate signal NG");
    }

	// LTEの場合は自律リセットの情報クリアの処理実施
	if (CMD_SYS_S3G == sigchk_rslt) {

//		recardcntreq_p = (CMT_TSKIF_CPRIRCV_RECARDCNTREQ*)bufp;
		// リセットの場合のみクリア
		if (CMD_CARDRST == recardcntreq_p->cpridat_recardcntreq.cnt_kind) {

			slot_no = recardcntreq_p->cpridat_recardcntreq.slot_no; // スロット番号
			link_no = recardcntreq_p->cprircv_inf.link_num;         // リンク番号

            f_cmn_com_slotsrch_s3g(link_no, slot_no, &slot_idx);

			/***********************************************/
			/* タイマ取消 (REカード自律リセット待ちタイマ)**/
			/***********************************************/
			f_trp_rec_Fc02_t_timer_ccl(                            /* なし:R */
				EC_REC_TMK_REAUTRST_WAT,   /* RE Auto Reset 送信待ちタイマ:I */
		 		slot_no                                  /* スロット番号+1:I */
			);
			// 該当スロットの自律リセットの情報をクリアする
			 f_trp_com_AutoResetMngTbl[link_no - CMD_NUM1][slot_idx].ReAutoRstTimFlg = CMD_FLG_OFF;
			 f_trp_com_AutoResetMngTbl[link_no - CMD_NUM1][slot_idx].ReAutoRstCtlFlg = CMD_FLG_OFF;
			 f_trp_com_AutoResetMngTbl[link_no - CMD_NUM1][slot_idx].resetTime = D_TCM_DEFAULT_RSTTIM;
			 f_trp_com_AutoResetMngTbl[link_no - CMD_NUM1][slot_idx].ReAutoRstPreventFlg = CMD_FLG_OFF;

		}
	}
// [16B] add end
#endif // [16B] del end

    /* Send msg [REカード制御要求] to L2/lpb */
    head = (CMT_TSKIF_HEAD*)bufp;
    f_trp_rec_Fcom_t_l2_msg_fw( bufp, head->uiLength );

    return;
}

/*! @} */

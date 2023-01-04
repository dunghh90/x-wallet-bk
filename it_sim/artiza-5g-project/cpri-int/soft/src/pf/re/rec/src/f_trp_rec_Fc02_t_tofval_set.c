/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tofval_set.c
 * @brief  RE状態(セクタ部)管理テーブル(Toffset値)設定
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE状態(セクタ部)管理テーブル(Toffset値)設定
 * @note   RE状態(セクタ部)管理テーブル(Toffset値)を設定する
 *         - 受信値設定
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_tofval_set(                                 /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    USHORT cpr_no = D_REC_C02_CPR_NO();
                                                    /* CPRI番号インデックス */
    TOFFSET_INF*  rcvp;                            /* 受信Toffset値アドレス */
    T_RRH_TOFFSET_S3G  tblp;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    /* 受信Toffset値アドレス抽出 */
    rcvp = &(((CMT_CPRIF_TOFFSETINFRES*)l3_bufp)->toffset_inf);

    /**************/
    /* 受信値設定 */
    /**************/
	tblp.signal_kind   = ((CMT_CPRIF_TOFFSETINFRES*)l3_bufp)->signal_kind;
                                                                  /* 信号種別 */
    tblp.result        = ((CMT_CPRIF_TOFFSETINFRES*)l3_bufp)->result;
                                                                  /* 応答結果 */
    tblp.toffset  = rcvp->toffset;                             /* Toffset値 */
    tblp.updeleqp = rcvp->updeleqp;                     /* 上り装置内遅延値 */
    tblp.downdeleqp = rcvp->downdeleqp;                   /* 下り装置内遅延値 */
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s f_cmn_com_toffset_s3g_set(signal_kind:0x%x, result:%d, toffset:%d, updeleqp:%d, downdeleqp:%d) \n",
		__LINE__,__FUNCTION__,tblp.signal_kind,tblp.result, tblp.toffset, tblp.updeleqp, tblp.downdeleqp );
#endif
	/* 設定用 */
	f_cmn_com_toffset_s3g_set(cpr_no, &tblp);

    return CMD_ON;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/


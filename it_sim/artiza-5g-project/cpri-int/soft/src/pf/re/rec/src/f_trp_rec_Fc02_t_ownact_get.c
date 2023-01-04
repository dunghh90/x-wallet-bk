/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_ownact_get.c
 * @brief  自Active状態取得
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
 * @brief  自Active状態取得
 * @note   自リンクのActive状態を返却する
 * @param  -
 * @return Active状態
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_ownact_get(                         /* Active状態:R */
) {
    USHORT actsts;                                            /* Active状態 */
	USHORT actlnk_gr;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	actlnk_gr = D_REC_C02_ACTLNK_GR();
	
    /* RRE/LREの場合は無条件でActive */
    if (actlnk_gr == D_TCM_ACTIVE_LINK_GROUP_RRELRE) {
        actsts = D_REC_ACK_ACTCMP;
        return actsts;
    }

    /* 指定のCPRI番号と、該当ActiveリンクグループのActiveリンクのCPRI番号が一致 */
    if (D_REC_C02_CPR_NO() == f_trp_rec_Wcom_remcm_tbl.actsts[actlnk_gr - CMD_NUM1].cpr_no) {
        /* Active状態を設定 */
        actsts = f_trp_rec_Wcom_remcm_tbl.actsts[actlnk_gr - CMD_NUM1].sts;
    }
    /* CPRI番号が一致しない場合 */
    else {
        /* Negativeを設定 */
        actsts = D_REC_ACK_NEGCMP;
    }

    return actsts;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_ownact_set.c
 * @brief  自Active状態設定
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
 * @brief  自Active状態設定
 * @note   自Active状態を設定する
 * @param  actsts [in]  Active状態
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_ownact_set(                                 /* なし:R */
    USHORT actsts                                           /* Active状態:I */
) {
	USHORT actlnk_gr;
	USHORT tim_offset;
    USHORT index;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
	
	actlnk_gr = D_REC_C02_ACTLNK_GR();

    /* RE type is RRE/LRE */
	if (actlnk_gr == D_TCM_ACTIVE_LINK_GROUP_RRELRE) {
        return;
    }

    /************/
    /* 状態設定 */
    /************/
    f_trp_rec_Wcom_remcm_tbl.actsts[actlnk_gr - CMD_NUM1].sts = actsts;

    index = f_trp_rec_WobjMngTbl.actLnkGrpMngTbl.actLnkGrp2Obj[actlnk_gr];
    if(index != 0xFFFF)
    {
        f_trp_rec_WobjMngTbl.actLnkGrpMngTbl.actLnkGrpObj[index].actSetSts = actsts;
    }

    /****************/
    /* CPRI番号設定 */
    /****************/
    /* Negativeの場合 */
    if (actsts == D_REC_ACK_NEGCMP) {
        /* 無効 */
        f_trp_rec_Wcom_remcm_tbl.actsts[actlnk_gr - CMD_NUM1].cpr_no = D_REC_CPR_IV;
    }
    /* Negative以外の場合 */
    else {
        /* 有効 */
        f_trp_rec_Wcom_remcm_tbl.actsts[actlnk_gr - CMD_NUM1].cpr_no = D_REC_C02_CPR_NO();
    }

    index = f_trp_rec_WobjMngTbl.cpriLinkMngTbl.cpriLinkNo2Obj[f_trp_rec_Wcom_remcm_tbl.actsts[actlnk_gr - CMD_NUM1].cpr_no-1];
    if(index != 0xFFFF)
    {
        f_trp_rec_WobjMngTbl.cpriLinkMngTbl.cpriLinkObj[index].actSetSts = actsts;
    }

    /**************************************/
    /* Active切替指示送信待ちタイマ値設定 */
    /**************************************/
    /* Activeの場合 */
    if (actsts == D_REC_ACK_ACTCMP) {
        /* Active切替指示送信待ちタイマ値MIN設定 */
    	tim_offset = EC_REC_TMK_ACTCHG_WAT;
    	if(actlnk_gr != D_TCM_ACTIVE_LINK_GROUP_BRE)
    	{
    		tim_offset = EC_REC_TMK_ACTCHG_WAT2 + actlnk_gr - CMD_NUM2;
    	}
        f_trp_rec_Wcom_tmk_val_tbl.val[tim_offset].tim_val = D_REC_ACTCHG_TIMVAL_MIN;
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

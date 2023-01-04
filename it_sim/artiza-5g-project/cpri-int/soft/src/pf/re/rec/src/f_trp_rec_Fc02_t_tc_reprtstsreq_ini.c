/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_reprtstsreq_ini.c
 * @brief  残トライ回数(RE PORT状態報告要求)初期化
 * @date   2008/07/18 FFCS)Shihzh Create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2009
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  残トライ回数(RE PORT状態報告要求)初期化
 * @note   残トライ回数(RE PORT状態報告要求)を初期化する
 *         - 残トライ回数初期化
 * @param  -
 * @return None
 * @date   2008/07/18 FFCS)Shihzh Create.
 * @date   2009/03/04 FFCS)Shihzh CR-00031-005.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_reprtstsreq_ini(                         /* なし:R */
) {
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************/
    /* 残トライ回数初期化 */
    /**********************/
    if(f_trp_com_Wsysprmp_PF != NULL)
    {
        f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.reprtstsreq
          = f_trp_com_Wsysprmp_PF[D_SYSPARA_PF_C13_REPORTSTS_RTNUM] + CMD_NUM1;
    }
    else
    {
        /* set default value */
        f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.reprtstsreq
          = D_TCM_SYSDFT_REPORTSTS_RTNUM + CMD_NUM1;
    }
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s cpr_no:%d, tc.reprtstsreq:%d \n",__LINE__,__FUNCTION__,
		cpr_no_idx + 1, f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.reprtstsreq);
#endif
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

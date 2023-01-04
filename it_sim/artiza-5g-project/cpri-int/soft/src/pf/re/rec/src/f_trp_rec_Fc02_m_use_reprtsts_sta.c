/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_reprtsts_sta.c
 * @brief  運用中RE PORT状態報告開始処理
 * @date   2016/04/25 TDI)satou create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief	運用中RE PORT状態報告開始処理
 * @note	運用中RE PORT状態報告開始処理を行う
 * @param	bufp [in]  バッファポインタ
 * @return	None
 * @date	2016/04/25 TDI)satou create
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_reprtsts_sta(VOID* bufp) {
    T_RRH_REPORTSTA_S3G	shmRePort;
	USHORT cpr_idx = D_REC_C02_CPR_NO() - 1;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************************************************************/
    /*
     * 共有メモリから設定値を取得.
     *	PORT入力状態のbitが割り当てられていないREにRE PORT状態報告要求を送信するのは
     *	無駄であるため、RE PORT状態報告要求を送信しない。PORT入力状態のbitを
     *	割り当てられていないREの報告は、RECへの報告の集約対象外であるため無駄と判断
     */
    /**********************************************************************/
    f_cmn_com_RePortSta_s3g_allget(&shmRePort);

    if (D_RRH_ON == shmRePort.alloc_en[cpr_idx])
    {
		f_trp_rec_Fc02_t_l3_reprtstsreq_snd(D_REC_C02_CPR_NO());
    }
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_crg_wrt.c
 * @brief  レジスタ読み出し
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "f_rrh_reg_cnt.h"
#include "f_rrh_reg_dis.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  レジスタ書き込み
 * @note   レジスタを書き込む
 *         - メモリ書き込み
 * @param  crk    [in]  レジスタ種別
 * @param  cpr_no [in]  CPRI番号
 * @param  val    [in]  レジスタ値
 * @return 終了コード
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/09/08 FPT)Yen update for FHM
 * @date   2015/11/05 TDI)satou その他未実装-001 送信系切替制御解除を追加
 * @date   2015/12/03 TDI)satou IT3問処番号No.115
 * @date   2015/12/03 TDI)satou IT3問処番号No.117
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_crg_wrt(                              /* 終了コード:R */
    ET_REC_CRK crk,                                       /* レジスタ種別:I */
    USHORT  cpr_no,                                           /* CPRI番号:I */
    UINT    val                                             /* レジスタ値:I */
) {
    USHORT cpr_no_idx = cpr_no - CMD_NUM1;          /* CPRI番号インデックス */
    UINT   uit_ret = CMD_NG;                       			  /* 終了コード */
    UINT*   cra;                                         /* レジスタアドレス */
	UINT   regVal;
	UINT     llBitMask;
	USHORT   lwCnt;
	UINT     lwIrgCpmRegTbl[CMD_NUM8] = {
						D_RRH_REG_CNTS_IRQCPM12,
						D_RRH_REG_CNTS_IRQCPM34,
						D_RRH_REG_CNTS_IRQCPM56,
						D_RRH_REG_CNTS_IRQCPM78,
						D_RRH_REG_CNTS_IRQCPM9A,
						D_RRH_REG_CNTS_IRQCPMBC,
						D_RRH_REG_CNTS_IRQCPMDE,
						D_RRH_REG_CNTS_IRQCPMF10
					};

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* レジスタアドレス設定 */
    cra = f_trp_rec_Rcom_cra_tbl.adr[crk][cpr_no_idx];

    /**************/
    /* REGHIS追加 */
    /**************/
    f_trp_rec_Fcom_t_reghis_add(                                  /* なし:R */
        cra,                                          /* レジスタアドレス:I */
        val                                                 /* レジスタ値:I */
    );

	if((cpr_no_idx % 2) == 0 )
	{
		/* D12: STED1 */
		llBitMask = 0x00001000;
	}
	else
	{
		/* D28: STED2 */
		llBitMask = 0x10000000;
	}
	for (lwCnt = 0; lwCnt < D_RRH_L1INBAND_RETRY_MAX; lwCnt++)
	{
		/* L1 inband reset on */
		BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE_CPRI,
			(UINT)cra, 0x00008100);

		/* 1ms */
		usleep(D_RRH_L1INBAND_RESET_WAIT);

		/* L1 inband reset off */
		BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE_CPRI,
			(UINT)cra, 0x00008100);

		/* master State */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
			lwIrgCpmRegTbl[cpr_no_idx/2], &regVal);
		
		if ((regVal & llBitMask) != 0)
		{
			uit_ret = CMD_OK;
			break;
		}
	}

    return uit_ret;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

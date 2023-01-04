/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   m_rc_ReRstExe.c
 * @brief  レジスタ読み出し
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "f_rrh_reg_cnt.h"
#include "f_rrh_reg_cpri.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  レジスタ書き込み
 * @note   レジスタを書き込む
 *         - メモリ書き込み
 * @param  cpr_no [in]  CPRI番号
 * @return 終了コード
 * @date   2015/09/09 FFT)Quynh create.
 * @date   2015/11/05 TDI)satou その他未実装-001 送信系切替制御解除を追加
 * @date   2015/12/03 TDI)satou IT3問処番号No.117
 */
/****************************************************************************/
UINT m_rc_ReRstExe(                                         /* 終了コード:R */
    USHORT  cpr_no                                            /* CPRI番号:I */
) {
    USHORT cpr_no_idx = cpr_no - CMD_NUM1;          /* CPRI番号インデックス */
    UINT   uit_ret = CMD_NG;                       			  /* 終了コード */
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
						M_RRH_REG_CPRI_CPTXCW(cpr_no), 0x00008100);

		/* 1ms */
		usleep(D_RRH_L1INBAND_RESET_WAIT);

		/* L1 inband reset off */
		BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE_CPRI,
						M_RRH_REG_CPRI_CPTXCW(cpr_no), 0x00008100);

		/* master State */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
			lwIrgCpmRegTbl[cpr_no_idx/2], &regVal);
		
		if ((regVal & llBitMask) == 0)
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

/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_rsv_CardReset_zynq.c
 *  @brief  API Set datetime Request process function
 *  @date   2013/11/08 FFCS)Xut create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */
#include <signal.h>
#include "f_rsv_header.h"			/* REŠÄŽ‹ƒ^ƒXƒNƒwƒbƒ_ƒtƒ@ƒCƒ‹			*/

/**
* @brief API Card Reset Request process function
* @note  Send message to INIT process to request card reset.\n
* @param resetinf_p [in] the buffer address pointer of received message
* @return None
* @date   2013/11/08 FFCS)Xut create
* @warning	N/A
* @FeatureID	PF-Svp-002-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_CardReset(T_API_SVP_RESET_REQ *resetinf_p  )
{
//#ifndef OPT_RRH_IT1
    T_API_SVP_ALLRE_L1RESET_IND resetInd;

	/****************/
	/* リセット処理 */
	/****************/
	if(resetinf_p->rstFact == D_RRH_RSTFACT_CRDCNT)
	{
		/* INITプロセスへSignal送信(USR1, リセット要求(REカード制御要求(L3 MSG)) */
		BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI, SIGUSR1, 
						D_RRH_SIGUSR_OPT_RSET_L3_RMT_NTC|D_RRH_SIGUSR_OPT_RESET_DPDC_VAL);
	}
	else
	{
		/* INITプロセスへSignal送信(USR1, リセット要求(REリセット要求(L3 MSG)) */
		if(D_API_LCL_RST == resetinf_p->rstkind)
		{
		    /* 全配下REリセット */
		    memset(&resetInd, 0, sizeof resetInd);
		    f_rsv_AllReL1Reset(&resetInd);

            /* RE-MTによりローカルリセット要求 */
            BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI, SIGUSR1, D_RRH_SIGUSR_OPT_RSET_LOCSW_NTC);
		}
		else			
		{
            /* L3メッセージによりリセット要求 */
            BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI, SIGUSR1, D_RRH_SIGUSR_OPT_RSET_L3_RST_NTC);
		}
	}
//#endif
	return;
}

/**
* @brief  API 全配下RE L1 inband reset
* @param  inf_p 受信メッセージ
* @return None
* @date   2015/10/19 TDI)satou ハソ-QA-036 create
* @date   2015/12/03 TDI)satou IT3問処番号No.115
*/
VOID f_rsv_AllReL1Reset(T_API_SVP_ALLRE_L1RESET_IND *inf_p)
{

    /* 一斉 Inband RST送信設定 */
    BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_CNTS_L1RST2MST, BIT0);

    /* 1ms Wait */
    usleep(D_RRH_L1INBAND_RESET_WAIT);

    /* 一斉 Inband RST送信解除設定 */
    BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_CNTS_L1RST2MST, BIT0);
}

/**
* @brief  API 配下RE L1 inband reset
* @param  inf_p 受信メッセージ
* @return None
* @date   2015/10/19 TDI)satou ハソ-QA-036 create
* @date   2015/11/05 TDI)satou その他未実装-001 送信系切替制御解除を追加
* @date   2015/12/03 TDI)satou IT3問処番号No.115
* @date   2015/12/03 TDI)satou IT3問処番号No.117
* @date   2015/12/07 TDI)enoki IT3問処番号No.131
*/
VOID f_rsv_ReL1Reset(T_API_SVP_RE_L1RESET_IND *inf_p)
{
    UINT bit, address;

        /* CPM#n Inband RST送信設定 */
        address = M_RRH_REG_CPRI_CPTXCW(inf_p->link_num);
        bit     = BIT8 | BIT15;
        BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE_CPRI, address, bit);

        /* 1ms Wait */
        usleep(D_RRH_L1INBAND_RESET_WAIT);

        /* CPM#n Inband RST送信解除設定 */
        BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE_CPRI, address, bit);
}

/*! @} */

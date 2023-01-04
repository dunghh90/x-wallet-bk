/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_nego_AutoNegoReStartInd.c
 *  @brief  Auto Nego ReStart Indication function
 *  @date 2015/04/22 FFCS)fuxg create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_NEGO
* @{ */

#include "f_sys_inc.h"
#include "f_nego_inc.h"

/**
* @brief 	Auto Nego ReStart Indication function
* @note  	Auto Nego ReStart Indication function.\n
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2015/04/22 FFCS)fuxg create
* @date     2015/8/4 TDIPS)ikeda rev.27800 2.4Gビットレート対応
* @date     2015/8/4 TDIPS)ikeda rev.27819 オートネゴ修正
* @date     2015/09/05 TDI)satou レジスタアドレスのdefineを修正
* @warning	N/A
* @FeatureID	RRH-012-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_nego_AutoNegoReStartInd(UINT * buff_adr)
{
	T_SYS_AUTO_NEGO_RESTART_IND*		autonego_restart_p;
	INT			ret;
	INT			errcd;
	UINT 		regVal;
	CHAR *		bitratename[] = { "2.4G","4.9G","9.8G" };

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "EvtNo:0x%x", ((T_RRH_HEAD*)buff_adr)->uiEventNo);

	/* initial */
	f_nego_4s_count = 0;
	f_nego_hfn_count = 0;

	autonego_restart_p = (T_SYS_AUTO_NEGO_RESTART_IND*)buff_adr;
	f_nego_bitrate = autonego_restart_p->bit_rate;

	/* log */
	BPF_COM_PLOG_TRACE_CPRI(E_TRC_LV_INFO, "Auto Nego Resync!Bit Rate = %s(%d)", 
								bitratename[f_nego_bitrate],
								f_nego_bitrate);

	/* Hard set before CPP PHY LyncUP */
	f_nego_HardSetLyncUp_ReStart();

	/* hard set flag */
	f_nego_hard_set_flag = D_NEGO_HARD_SET_LYNCUP_RESTART;

	/* read and clear CPP PHY LyncUP ST */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, D_RRH_REG_CNTS_CDRST, &regVal);
	BPF_COM_PLOG_TRACE_CPRI( E_TRC_LV_INFO, "CNTS_CDRST:0x%x", regVal);

	/* CPP PHY LyncUP confirm */
	if((regVal & 0x00000001) == 0x00000001)
	{
		/* Hard set before Hfn Sync */
		f_nego_HardSetHfnSync_ReStart();

		/* hard set flag */
		f_nego_hard_set_flag = D_NEGO_HARD_SET_HFNSYNC_RESTART;

		/* HFN SYNC confirm */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
				D_RRH_REG_CPRI_CPST, &regVal);

		BPF_COM_PLOG_TRACE_CPRI( E_TRC_LV_INFO, "CPRI_CPST:0x%x", regVal);

		if((regVal & 0x00000001) == 0x00000001)
		{
			f_nego_hfn_count++;
		}
	}

	/* 10msec timer start */
	ret = BPF_RU_HRTM_REGISTER(
			BPF_RU_HRTM_REG_ONCE,
			E_RRH_TIMID_AUTO_NEGO,
			D_NEGO_ONE_SHOT_TIME_10MSEC,
			D_SYS_MSGID_AUTONEGO_TIMEOUT_NTC,
			D_SYS_THDQID_PF_NEGO,
			&errcd
		);

	if( ret != BPF_RU_HRTM_COMPLETE )
	{
		/* Assert log */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, 
							"Timer Register NG!ret = %d, errcd = %d", 
							ret, errcd);
	}

	f_nego_thrdState = D_NEGO_THRDSTA_RUN;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "EvtNo:0x%x", ((T_RRH_HEAD*)buff_adr)->uiEventNo);

	return;
}


/**
* @brief 	Hard Set before CPP PHY LyncUP in case of restart function
* @note  	Hard Set before CPP PHY LyncUP in case of restart function.\n
*
* @return 	None
* @date 	2015/04/22 FFCS)fuxg create
* @date 	2015/06/17 ALPHA)ueda RRH-007-000 TDD-Zynq対応
* @warning	N/A
* @FeatureID	RRH-012-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_nego_HardSetLyncUp_ReStart()
{
	/* EXT PLL Hold Over set */
	f_nego_pll_holdover_value |= 0x00000008;
	BPF_HM_DEVC_EXT_PLL_WRITE(D_RRH_REG_PLL_LMK4826_HOLDOVER_FORCE, f_nego_pll_holdover_value);

	/* 122.88 Mhz Output Stop */
	BPF_HM_DEVC_REG_WRITE_BITOFF(	D_RRH_LOG_REG_LV_READ_WRITE_CPRI, 
									D_RRH_REG_CNTS_CLKCNT, 
									0x00000001);

	return;
}


/**
* @brief 	Hard Set before CPP HfnSync in case of restart function
* @note  	Hard Set before CPP HfnSync in case of restart function.\n
*
* @return 	None
* @date 	2015/04/22 FFCS)fuxg create
* @date 	2015/06/17 ALPHA)ueda RRH-007-000 TDD-Zynq対応
* @warning	N/A
* @FeatureID	RRH-012-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_nego_HardSetHfnSync_ReStart()
{
	/* 122.88 Mhz Output ReStart */
	BPF_HM_DEVC_REG_WRITE_BITON(	D_RRH_LOG_REG_LV_READ_WRITE_CPRI, 
									D_RRH_REG_CNTS_CLKCNT, 
									0x00000001);

	/* EXT PLL Hold Over release */
	f_nego_pll_holdover_value &= 0xFFFFFFF7;
	BPF_HM_DEVC_EXT_PLL_WRITE(D_RRH_REG_PLL_LMK4826_HOLDOVER_FORCE, f_nego_pll_holdover_value);

	return;
}


/* @} */


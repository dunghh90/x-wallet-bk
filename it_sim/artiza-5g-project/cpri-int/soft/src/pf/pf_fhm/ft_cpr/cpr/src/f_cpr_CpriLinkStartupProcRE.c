/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_CpriLinkInitProcRE.c
 *  @brief  re  link start indication function
 *  @date 2015/08/05 TDIPS)ikeda create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "BPF_RU_ITCM.h"
#include "BPF_RU_HRTM.h"
#include "rrhApi_Cpri.h"

/**
* @brief 	re cpri link init indication function
* @note 	re cpri link init indication function.\n
* @param 	linkno [in] CPRI link number
* @return 	None
* @date 	2015/08/04 TDIPS)ikeda
* @warning	N/A
* @FeatureID	PF_Cpri-002-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_cpr_CpriLinkStartupProcRE(USHORT linkno)
{
	UINT regVal;

	/* CPM#n SFP & CPRI Enable設定 0:oFF 1:oN */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_SFPCNT(linkno), &regVal);
	regVal &= ~0x00000007;
	regVal |=  0x00000004;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, M_RRH_REG_CPRI_SFPCNT(linkno), &regVal);

	/* HFN SYNC confirm */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
			M_RRH_REG_CPRI_CPST(linkno), &regVal);

	f_sync_hfn_count[linkno] = 0;
	f_sync_hfn_linkup[linkno] = 0;
	if( (regVal & 0x00000001) == 0x00000001)
	{
		f_sync_hfn_count[linkno]++;
	}
#if 0
	/* 10msec timer start */
	f_cprLinkUpTimerCtrl(linkno, D_RRH_ON);
#endif
	return;
}
/* @} */
VOID f_cprLinkUpTimerCtrl(USHORT linkno, USHORT sFlg)
{
	INT		ret;
	INT		errcd;
	
	if((linkno < D_RRH_CPRINO_RE1) || (linkno > D_RRH_CPRINO_RE16))
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Timer stop link number NG! linkno = %d", linkno);
		return;
	}
	if(sFlg != D_RRH_OFF)
	{
		/*	該当CPRIリンクがタイマ停止中の場合	*/
		if(f_cprw_masterLinkUpTimerCnt[linkno-1] == D_RRH_OFF)
		{
			/*	全CPRIリンクがタイマ停止中の場合	*/
			if(f_cprw_masterLinkUpTimerNum == 0)
			{
				/* 10msec timer start */
				ret = BPF_RU_HRTM_REGISTER( BPF_RU_HRTM_REG_ONCE, E_RRH_TIMID_CPRI_LINKUP1,
						D_CPM_ONE_SHOT_TIME_10MSEC, D_SYS_MSGID_LINKUP_TIMEOUT_NTC,
						D_SYS_THDQID_PF_CPRI, &errcd
					);
				if ((BPF_RU_HRTM_COMPLETE != ret) && (BPF_RU_HRTM_ERR_NOENT != ret)) {
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Timer Register NG!ret = %d, errcd = %d", ret, errcd);
				}
			}
			f_cprw_masterLinkUpTimerNum++;
		}
		/*	該当CPRIリンクが既にタイマ生成中の場合	*/
		else
		{
			/*	nop	*/
		}
	}
	else
	{
		/*	該当CPRIリンクがタイマ生成中の場合	*/
		if(f_cprw_masterLinkUpTimerCnt[linkno-1] != D_RRH_OFF)
		{
			/*	タイマ生成CPRIリンクがある場合	*/
			if(f_cprw_masterLinkUpTimerNum != 0)
			{
				f_cprw_masterLinkUpTimerNum--;
			}
			if(f_cprw_masterLinkUpTimerNum == 0)
			{
				/* Stop timer */
				ret = BPF_RU_HRTM_CANCEL(E_RRH_TIMID_CPRI_LINKUP1, &errcd);
				if ((BPF_RU_HRTM_COMPLETE != ret) && (BPF_RU_HRTM_ERR_NOENT != ret)) {
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "BPF_RU_HRTM_CANCEL NG. ret=%d errcd=%d", ret, errcd);
				}
			}
		}
		/*	該当CPRIリンクが既にタイマ停止中の場合	*/
		else
		{
			/*	nop	*/
		}
	}
	f_cprw_masterLinkUpTimerCnt[linkno-1] = sFlg;
	return;
}


/* @} */


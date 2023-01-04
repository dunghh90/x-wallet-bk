/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_CpriLinkupTimeOutNtc.c
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
* @brief 	re cpri linkup timeout eventfunction
* @note 	re cpri linkup timeout eventfunction.\n
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2015/08/04 TDIPS)ikeda
* @date     2015/10/26 TDIPS)ikeda M-RRU-ZSYS-01802 CPM#n SFP & CPRI Disable設定削除
* @warning	N/A
* @FeatureID	PF_Cpri-002-001-001
* @Bug_No	M-RRU-ZSYS-01802
* @CR_No	N/A
* @TBD_No	N/A
*/
VOID f_cpr_CpriLinkupTimeOutNtc(UINT * buff_adr)
{
	T_API_COMMON_HEADER		*head_pt;					/**<  タスク共通ヘッダ		 */
	USHORT		linkno;
	UINT	regVal;
	INT		ret;
	INT		errcd;
	CHAR *	bitratename[] = { "2.4G","4.9G","9.8G"};

	/* タイマーIDからリンク番号と信号種別を計算 */
	head_pt = (T_API_COMMON_HEADER*) (buff_adr);
	linkno = M_RRH_CPRINO_CPRI_LINKUP(head_pt->uiSignalkind);
	/*	Master CPRIリンクの場合	*/
	if(linkno == D_RRH_CPRINO_RE1)
	{
		/*	Master CPRIリンク#1～16まで繰り返し	*/
		for(linkno=D_RRH_CPRINO_RE1; linkno<D_RRH_CPRINO_NUM; linkno++)
		{
			/*	停止中の場合はスキップ	*/
			if(f_cprw_masterLinkUpTimerCnt[linkno-1] == D_RRH_OFF)
			{
				continue;
			}
			/* HFN SYNC confirm */
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
					M_RRH_REG_CPRI_CPST(linkno), &regVal);

			if( (regVal & 0x00000001) == 0x00000001)
			{
				f_sync_hfn_count[linkno]++;

				/* hfn count confirm */
				if(f_sync_hfn_count[linkno] == D_CPR_HFNSYNC_TIMES)
				{
					/* log */
					BPF_COM_PLOG_TRACE_CPRI(E_TRC_LV_INFO,"CPRI Start Up!Bit Rate = %s(%d)",
												bitratename[f_cprw_bitrate_real],
												f_cprw_bitrate_real);
					
					f_cprLinkUpTimerCtrl(linkno, D_RRH_OFF);
					continue;
				}
			}
			else
			{
				/* hfn count clean */
				f_sync_hfn_count[linkno] = 0;

				/* link timer */
				if (++f_sync_hfn_linkup[linkno] > D_CPM_LINKUP_TIME_MAX )
				{
					BPF_COM_PLOG_TRACE_CPRI( E_TRC_LV_INFO, "CPRI_CPST:0x%x", regVal);
				}
			}
		}
		/* 10msec timer start */
		ret = BPF_RU_HRTM_REGISTER(
				BPF_RU_HRTM_REG_ONCE,
				head_pt->uiSignalkind,
				D_CPM_ONE_SHOT_TIME_10MSEC,
				D_SYS_MSGID_LINKUP_TIMEOUT_NTC,
				D_SYS_THDQID_PF_CPRI,
				&errcd
			);

		if( ret != BPF_RU_HRTM_COMPLETE )
		{
			/* Assert log */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO,
								"Timer Register NG!ret = %d, errcd = %d",
								ret, errcd);
		}
	}
	else
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Timer TO link number NG! linkno = %d", linkno);
	}

	return;
}
/* @} */

/* @} */


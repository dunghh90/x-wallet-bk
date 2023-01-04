/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_CpriParamUpdProc.c
 *  @brief  CPRI link system param updating function
 *  @date 2013/11/14 FFCS)niemsh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_CPR
* @{ */

#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "rrhApi_Cpri.h"
/**
* @brief 	CPRI link system param updating function
* @note  	CPRI link system param updating function.\n
* @param	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2013/11/14 FFCS)niemsh create
* @date     2015/08/08 TDI)satou Master側にも設定するように処理を追加
* @FeatureID	PF_Cpri-002-001-001
*/
VOID f_cpr_CpriParamUpdProc(UINT * buff_adr)
{
    USHORT linkno;  /* CPRIリンク番号 */

	/*write cpri stable timer regsiter*/
	T_API_CPRILINK_PARAM_UPDATE_IND *apiInd_pt;
	UINT paramValue;
	UINT re_paramValue;
	
	apiInd_pt = (T_API_CPRILINK_PARAM_UPDATE_IND*)buff_adr;
	paramValue = apiInd_pt->data.stableValue;
	paramValue &= 0xffff;
	re_paramValue = paramValue + 10;	/* RE-FHM間は+100msec 追加 */

	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++) {
	    if(linkno == D_RRH_CPRINO_REC ){
		    BPF_HM_DEVC_REG_WRITE(
		            D_RRH_LOG_REG_LV_WRITE_CPRI,        /* IN: ログレベル                     */
	    	        M_RRH_REG_CPRI_CPSTAB(linkno),      /* IN: レジスタアドレス(CPRI_CPSTAB)  */
	        	    &paramValue                         /* IN: レジスタ設定値(CPRI安定化時間) */
		    );
		}
		else{
		    BPF_HM_DEVC_REG_WRITE(
		            D_RRH_LOG_REG_LV_WRITE_CPRI,        /* IN: ログレベル                     */
	    	        M_RRH_REG_CPRI_CPSTAB(linkno),      /* IN: レジスタアドレス(CPRI_CPSTAB)  */
	        	    &re_paramValue                        /* IN: レジスタ設定値(CPRI安定化時間) */
		    );
		}
	}
	return;
 }
/* @} */

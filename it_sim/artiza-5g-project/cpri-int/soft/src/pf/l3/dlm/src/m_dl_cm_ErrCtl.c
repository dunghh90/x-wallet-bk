/*!
 * @skip   $Id$
 * @file   m_dl_cm_ErrCtl.c
 * @brief  the processing of Err control.
 * @date   2008/07/29 FFCS)Wuh Create for eNB-008-001.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{ 
 */

#include "m_cm_header.h"													/* MDIF common head file			*/
#include "m_dl_header.h"													/* download management task head file */
#include "f_rrh_reg_cnt.h"

/*!
 *  @brief  the function control the RED-LED ON/OFF.
 *  @note   the function control the RED-LED ON/OFF.
 *  @param  a_ctlcmd      [in]  control commond(0~1) 0:LED light dark  1:LED light bright
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *  @TBD_No ファームダウンロード、システムパラメータ配信失敗時のSV制御レジスタ設定値の見直しが必要
 */

void m_dl_cm_ErrCtl( UINT a_ctlcmd )
{
	/******************************************************/
	/* write the ERR code to ERR register                 */
	/******************************************************/
	if(a_ctlcmd == DLD_ERR_INF_ON) {
		/* Download info err on */
		m_cm_ErrCntRegSet( CMD_SET, D_RRH_REG_CNT_SV9INF_SYSPARA_DL_ERR, D_RRH_REG_CNT_SV9INF_L1ERR, CMD_CHKALM_NCY ) ;
	}
	else if(a_ctlcmd == DLD_ERR_CMP_ON){
		/* Download coplete err on */
		m_cm_ErrCntRegSet( CMD_SET, D_RRH_REG_CNT_SV9INF_FIRM_DL_ERR, D_RRH_REG_CNT_SV9INF_L1ERR, CMD_CHKALM_NCY ) ;
	}
	else if(a_ctlcmd == DLD_ERR_INF_OFF){
		/* Download info err off */
		m_cm_ErrCntRegSet( CMD_CLR, D_RRH_REG_CNT_SV9INF_SYSPARA_DL_ERR, D_RRH_REG_CNT_SV9INF_L1ERR, CMD_CHKALM_NCY ) ;
	}
	else if(a_ctlcmd == DLD_ERR_CMP_OFF){
		/* Download complete err off */
		m_cm_ErrCntRegSet( CMD_CLR, D_RRH_REG_CNT_SV9INF_FIRM_DL_ERR, D_RRH_REG_CNT_SV9INF_L1ERR, CMD_CHKALM_NCY ) ;
	}
#if 0 //TODO 3Gは対象外
	else if(a_ctlcmd == DLD_ERR_INF_S3G_ON) {
		/* Download info err on for S3G */
		m_cm_ErrCntRegSet( CMD_SET, CMD_ERR_DL_INF_S3G_ERR, CMD_CHKALM_NCY ) ;
	}
	else if(a_ctlcmd == DLD_ERR_CMP_S3G_ON){
		/* Download complete err on for S3G */
		m_cm_ErrCntRegSet( CMD_SET, CMD_ERR_DL_CMP_S3G_ERR, CMD_CHKALM_NCY ) ;
	}
	else if(a_ctlcmd == DLD_ERR_INF_S3G_OFF){
		/* Download info err off for S3G */
		m_cm_ErrCntRegSet( CMD_CLR, CMD_ERR_DL_INF_S3G_ERR_OFF, CMD_CHKALM_NCY ) ;
	}
	else if(a_ctlcmd == DLD_ERR_CMP_S3G_OFF){
		/* Download complete err off for S3G */
		m_cm_ErrCntRegSet( CMD_CLR, CMD_ERR_DL_CMP_S3G_ERR_OFF, CMD_CHKALM_NCY ) ;
	}
#endif
	return ;

}

/* @} */



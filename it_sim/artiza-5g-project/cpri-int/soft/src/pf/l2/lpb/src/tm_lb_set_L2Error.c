/*!f
 * @skip    $Id$
 * @file    tm_lb_set_L2Error.c
 * @brief   Set L2 error
 *  @date   2013/11/25 FFCS)hongj Create for zynq
 * 
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"			/* RRH common head file		       	*/
#include "l2_com_inc.h"
#include "f_rrh_reg_cnt.h"


/********************************************************************************************************************/
/*!
 *  @brief  Set L2 error
 *  @note   Set L2 error
 *  @param  linkno   [in]  CPRIリンク番号
 *  @param  flag     [in]  CMD_ON:BITON制御、CMD_OFF:BITOFF制御
 *  @retval -
 *  @date   2013/11/25 FFCS)hongj Create for zynq
 *  @date   2015/07/28 TDI)ikeda  FHMでの17リンク対応
 */
 /********************************************************************************************************************/
void tm_lb_set_L2Error(USHORT linkno, INT flag)
{
	UINT            svRegAdr;   /* SV制御レジスタアドレス */
	UINT            svRegDat;   /* SV制御レジスタ設定値 */

	svRegAdr = M_RRH_REG_CNT_L2_ERR_ADR(linkno);
	svRegDat = M_RRH_REG_CNT_L2_ERR(linkno);
	
	if(flag == CMD_ON)
	{
		/* 対RE Error ON */
		m_cm_ErrCntRegSetRE(linkno, CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
	}
	else
	{
		/* 対RE Error OFF */
		m_cm_ErrCntRegSetRE(linkno, CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);
	}
	return;
}
/* @} */


/*!
 * @skip    $Id$
 * @file    tm_lb_set_stateF.c
 * @brief   Set stateF
 *  @date   2013/11/25 FFCS)hongj Create for zynq
 * 
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "l2_com_inc.h"
#include "f_rrh_reg_cpri.h"

static UINT tm_lbw_set_stateF_flg[D_RRH_CPRINO_NUM] = { CMD_OFF };

/********************************************************************************************************************/
/*!
 *  @brief  Set stateF
 *  @note   Set stateF
 *  @note   STATE E状態で、CPRIステート制御レジスタのFSETビットをソフトウェアにて設定すると、
 *          CPRIステート表示レジスタの内容がSTETA Fに変化する。この時、STATE E未満に遷移した場合は、
 *          ハード自律でCPRIステート制御レジスタのFSETビットを"0"にし、CPRIステート表示レジスタの内容も
 *          実際のSTATE状態に応じて変化する。
 *  @note   CPRIステート制御レジスタのFSETビットは、STETA E未満の時に"1"を設定されても無効となる。
 *          (ハードで"0"クリアする)
 *  @param  linkno [in] CPRIリンク番号
 *  @param  flag   [in] CMD_ON:0x0000_00001設定、CMD_OFF:0x0000_0000設定
 *  @retval -
 *  @date   2013/11/25 FFCS)hongj Create for zynq
 *  @date   2015/07/28 TDI)ikeda  FHMでの17リンク対応
 *  @date   2015/10/30 TDI)ikeda  F OK cprino 表示追加
 */
 /********************************************************************************************************************/
void tm_lb_set_stateF(USHORT linkno, INT flag)
{
	UINT regVal;
	if (flag == CMD_ON)
	{
		regVal = 1;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,
			M_RRH_REG_CPRI_CPFSET(linkno),&regVal);

		/* log */
		BPF_COM_PLOG_TRACE_CPRI(E_TRC_LV_INFO, "CPRI %d State F Set(ON)!", linkno);
		tm_lbw_set_stateF_flg[linkno] = CMD_ON;
	}
	else
	{
		regVal = 0;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,
			M_RRH_REG_CPRI_CPFSET(linkno),&regVal);

		if(tm_lbw_set_stateF_flg[linkno] == CMD_ON){
			BPF_COM_PLOG_TRACE_CPRI(E_TRC_LV_INFO, "CPRI %d State F Set(OFF)!", linkno);
		}
		tm_lbw_set_stateF_flg[linkno] = CMD_OFF;
	}
	return;
}
/* @} */


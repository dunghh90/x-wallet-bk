/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReStr_Rsp_3g.c
 *  @brief  RE立ち上げ応答(3G)
 *  @date   2015/08/20 TDIPS) uchida create
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "f_rrh_reg_dis.h"
#include "f_rrh_reg_cpri.h"
#include "f_rrh_reg_cnt.h"
#include "f_rrh_reg_mix.h"
#include "f_sys_def.h"
#include "../../pf/ft_com/com/inc/f_com_ftyp.h"
#include "../../pf/pf_fhm/ft_cpr/cpr/inc/f_cpr_ftyp.h"
#ifndef CUNIT_STUB
#define calc_round round
#endif
/********************************************************************************************************************/
/**
 *  @brief    RE立ち上げ応答(3G)
 *  @param    parm_p  [in]  RE立ち上げ応答へのポインタ
 *  @return   None
 *  @date     2015/08/20 TDIPS) uchida create
 *  @date     2015/09/04 TDI)satou SV制御レジスタ設定関数を修正
 *  @date     2015/08/20 FPT) Yen update (IT2 Bug No 115)
 *  @date     2015/10/24 TDIPS)sasaki Warning対処
 *  @date     2015/10/28 TDI)satou FHM技説-QA-108
 *  @date     2016/02/25 TDIPS)enoki FHM技説1.0.5変更対応 不一致時に強制停止にしない
 */
/********************************************************************************************************************/
VOID m_rc_ReStr_Rsp_3g(CMT_TSKIF_RESTR_RSP *parm_p)
{
	T_RRH_EQP_3G			eqp_3g;


	/* 起動失敗した場合は処理終了。強制停止は別の通知がREプロセスから来るのでそこで処理する */
	if (parm_p->result != D_RRH_OK)
	{
	    return;
	}

	f_cmn_com_eqp_3g_get(parm_p->link_num, &eqp_3g);
   	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,
   		"REadd EquipParameter(3G): 800MFilTyp=0x%04x 2G=0x%04x 800M=0x%04x 1.7G=0x%04x 1.5G=0x%04x",
				eqp_3g.eqp_inf[15],
				eqp_3g.eqp_inf[16],
				eqp_3g.eqp_inf[17],
				eqp_3g.eqp_inf[18],
				eqp_3g.eqp_inf[20] );

	/* l3状態を運用中状態に変更 */
	f_cmn_com_layer3_set_each(parm_p->link_num, parm_p->system_type, (USHORT)E_RRH_LAYER3_RE_OPE);

	return;
}

/** @} */

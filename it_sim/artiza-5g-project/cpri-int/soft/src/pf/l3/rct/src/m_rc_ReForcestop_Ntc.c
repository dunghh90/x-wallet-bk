/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReForcestop_Ntc.c
 *  @brief  RE強制停止応答
 *  @date   2015/09/25 TDIPS) uchida create
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#include "f_rrh_reg_cnt.h"
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/********************************************************************************************************************/
/**
 *  @brief    RE強制終了応答
 *  @param    buff_adr [in] メッセージデータ
 *  @return   none
 *  @date     2015/09/25 TDIPS) uchida create
 *  @date     2015/10/22 TDI)satou その他未実装-007 LED設定処理を追加
 *  @date     2015/10/30 TDI)satou その他未実装-002 停波設定/解除
 *  @date     2016/11/22 FJT)ohashi 16B スロット数制限機能追加
 */
/********************************************************************************************************************/
VOID m_rc_ReForcestop_Ntc(VOID* buff_adr)
{
	CMT_TSKIF_RE_FORCESTOP_NTC* parm_p;
	UINT svRegAdr, svRegDat;

	parm_p = (CMT_TSKIF_RE_FORCESTOP_NTC*)buff_adr;

	/* layer3状態を強制停止状態へ */
	f_cmn_com_layer3_set_each(parm_p->link_num,parm_p->system_type, E_RRH_LAYER3_RE_STOP);

    if (CMD_SYS_S3G == parm_p->system_type)
    {
        switch (parm_p->stopFactor)
        {
        case E_RRH_RESTOP_FACTOR_MISMATCH_CARR:
            svRegAdr = D_RRH_REG_CNT_SV15INF;
            svRegDat = M_RRH_REG_CNT_S3G_CARRRESET_ERR(parm_p->link_num);
            m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
            break;
        case E_RRH_RESTOP_FACTOR_DL_NG:
            svRegAdr = D_RRH_REG_CNT_SV14INF;
            svRegDat = M_RRH_REG_CNT_DL_NG_ERR(parm_p->link_num);
            m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
            break;
        case E_RRH_RESTOP_FACTOR_DELAY_NG:
            svRegAdr = D_RRH_REG_CNT_SV13INF;
            svRegDat = M_RRH_REG_CNT_DELAY_ERR2(parm_p->link_num);
            m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
            break;
        case E_RRH_RESTOP_FACTOR_SLOT_NG:
            svRegAdr = D_RRH_REG_CNT_SV12INF;
            svRegDat = M_RRH_REG_CNT_SLOT_ERR(parm_p->link_num);
            m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
            break;
        default:
            svRegAdr = D_RRH_REG_CNT_SV16INF;
            svRegDat = M_RRH_REG_CNT_S3G_RESTARTUP_ERR(parm_p->link_num);
            m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
            break;
        }
    }
    else
    {
        switch (parm_p->stopFactor)
        {
        case E_RRH_RESTOP_FACTOR_MISMATCH_CARR:
            svRegAdr = D_RRH_REG_CNT_SV15INF;
            svRegDat = M_RRH_REG_CNT_3G_CARRRESET_ERR(parm_p->link_num);
            m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
            break;
        case E_RRH_RESTOP_FACTOR_DELAY_NG:
            svRegAdr = D_RRH_REG_CNT_SV13INF;
            svRegDat = M_RRH_REG_CNT_DELAY_ERR1(parm_p->link_num);
            m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
            break;
        case E_RRH_RESTOP_FACTOR_SLOT_NG:
            svRegAdr = D_RRH_REG_CNT_SV12INF;
            svRegDat = M_RRH_REG_CNT_SLOT_ERR(parm_p->link_num);
            m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
            break;
        default:
            svRegAdr = D_RRH_REG_CNT_SV14INF;
            svRegDat = M_RRH_REG_CNT_STARTUP_ERR1(parm_p->link_num);
            m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
            break;
        }
    }

    /* LED設定処理 */
    m_cm_WriteLEDReg();

    /* 停波設定/解除 */
    m_rc_SetWave();

	return ;
}
/** @} */

/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_Wave.c
 *  @brief  送波・停波処理
 *  @date   2015/10/30 TDI)satou create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited
 */
/********************************************************************************************************************/
/**
 * @addtogroup L3_RCT
 * @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "m_rc_def.h"
#include "m_rc_ftyp.h"
/********************************************************************************************************************/
/**
 *  @brief	送波・停波設定
 *  @return	None
 *  @date	2015/10/30 TDI)satou create
 */
/********************************************************************************************************************/
VOID m_rc_SetWave(VOID)
{
    static USHORT     dis_ldisen_old = 0;    /* HWの初期値と合わせる */
    static USHORT     mix_lmixen_old = 0;    /* HWの初期値と合わせる */
    static USHORT     dis_wdisen_old = 0;    /* HWの初期値と合わせる */
    static USHORT     mix_wmixen_old = 0;    /* HWの初期値と合わせる */
    T_RRH_TX_ONOFF    shmTxOnOff;
    T_RRH_LAYER3      shmLayer3[CMD_MAX_SYSNUM];
    T_TRXSET_PARA_LTE shmTrxParaLTE;
    T_TRXSET_PARA_3G  shmTrxPara3G;
    USHORT            isTrxSetLTE, isTrxSet3G;
    USHORT            dis_ldisen, mix_lmixen, dis_wdisen, mix_wmixen;
    USHORT            link_num, link_idx;
    USHORT            bit;
    USHORT            car_num;

    /***************************************************
     * 変数初期化
     **************************************************/
    dis_ldisen = mix_lmixen = dis_wdisen = mix_wmixen = 0xFFFF;
    f_cmn_com_tx_onoff_get(&shmTxOnOff);
    f_cmn_com_layer3_get(CMD_SYS_3G , &shmLayer3[CMD_SYS_3G ]);
    f_cmn_com_layer3_get(CMD_SYS_S3G, &shmLayer3[CMD_SYS_S3G]);

    isTrxSetLTE = CMD_OFF;
    for (car_num = CMD_MIN_CARNUM; car_num <= CMD_MAX_CARNUM_S3G; car_num++)
    {
        f_cmn_com_trxset_para_lte_get(car_num, &shmTrxParaLTE);
        if (shmTrxParaLTE.wSigKind != 0)
        {
            isTrxSetLTE = CMD_ON;
        }
    }

    isTrxSet3G = CMD_OFF;
    for (car_num = CMD_MIN_CARNUM; car_num <= CMD_MAX_CARNUM_3G; car_num++)
    {
        f_cmn_com_trxset_para_3g_get(car_num, &shmTrxPara3G);
        if (shmTrxPara3G.wSigKind != 0)
        {
            isTrxSet3G = CMD_ON;
        }
    }

    /***************************************************
     * 局ON/OFFがOFFの場合、全配下REについて「停波」
     * TRX設定中であれば、上りは停波設定にしない
     **************************************************/
    if (CMD_OFF == shmTxOnOff.onoff_fhm)
    {
        dis_ldisen = dis_wdisen = 0;

        if (CMD_OFF == isTrxSetLTE)
        {
            mix_lmixen = 0;
        }

        if (CMD_OFF == isTrxSet3G)
        {
            mix_wmixen = 0;
        }
    }

    /***************************************************
     * 配下RE単位に判定
     **************************************************/
    for (link_num = D_RRH_CPRINO_RE_MIN; link_num <= D_RRH_CPRINO_RE_MAX; link_num++)
    {
        /***************************************************
         * 変数初期化
         **************************************************/
        link_idx = link_num - 1;
        bit      = (USHORT)(BIT0 << link_idx);

        /***************************************************
         * 配下RE ON/OFFがOFFの場合、当該配下REについて「停波」
         * TRX設定中であれば、上りは停波設定にしない
         **************************************************/
        if (CMD_OFF == shmTxOnOff.onoff_re[link_idx])
        {
            dis_ldisen &= ~bit;
            dis_wdisen &= ~bit;

            if (CMD_OFF == isTrxSetLTE)
            {
                mix_lmixen &= ~bit;
            }

            if (CMD_OFF == isTrxSet3G)
            {
                mix_wmixen &= ~bit;
            }
        }

        /***************************************************
         * 強制停止(S3G)の場合、S3Gについて「停波」
         **************************************************/
        if (E_RRH_LAYER3_RE_STOP == shmLayer3[CMD_SYS_S3G].layer3_re[link_idx])
        {
            dis_ldisen &= ~bit;
            mix_lmixen &= ~bit;
        }

        /***************************************************
         * 強制停止(3G)の場合、3Gについて「停波」
         **************************************************/
        if (E_RRH_LAYER3_RE_STOP == shmLayer3[CMD_SYS_3G].layer3_re[link_idx])
        {
            dis_wdisen &= ~bit;
            mix_wmixen &= ~bit;
        }
    }

    /***************************************************
     * レジスタ設定をPFプロセスに依頼
     **************************************************/
    if ((dis_ldisen_old != dis_ldisen) || (mix_lmixen_old != mix_lmixen)
    ||  (dis_wdisen_old != dis_wdisen) || (mix_wmixen_old != mix_wmixen))
    {
        (VOID)rrhApi_Svp_Mnt_SetWave(D_RRH_PROCQUE_L3, dis_ldisen, mix_lmixen, dis_wdisen, mix_wmixen);
    }

    /***************************************************
     * 今回設定値を保存
     **************************************************/
    dis_ldisen_old = dis_ldisen;
    mix_lmixen_old = mix_lmixen;
    dis_wdisen_old = dis_wdisen;
    mix_wmixen_old = mix_wmixen;
}
/** @} */

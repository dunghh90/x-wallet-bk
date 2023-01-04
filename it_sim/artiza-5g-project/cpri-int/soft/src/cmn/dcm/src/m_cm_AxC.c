 /***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_cm_AxC.c
 *  @brief  AxC設定
 *  @date   2015/11/02 TDI)satou create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2015-
 */
/***************************************************************************/
/**
 * @addtogroup RRH_DCM
 * @{
 */
/***************************************************************************/
/* ヘッダインクルード                                                      */
/***************************************************************************/
#include "m_cm_header.h"
#include "f_rrh_def.h"
#include "rrhApi_Svp.h"
#include "l3_com_inc.h"
/**************************************************************************/
/**
 *  @brief  AxC設定(BRUSE設定値取得)
 *  @return BRUSE設定値
 *  @date   2015/11/02 TDI)satou その他未実装-003 create
 */
/**************************************************************************/
USHORT m_cmR_AxC_BRUSE(VOID)
{
    USHORT bruse;

    /* シスパラ/変数名E：送信系統数にて設定 */
    switch (cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num)
    {
    case 0:
        /* 送信系統数が0(2以下)なら2Br設定 */
        bruse = 0;
        break;
    case 1:
        /* 送信系統数が1(4以下)なら4Br設定 */
        bruse = 1;
        break;
    default:
        /* 送信系統数が2(8以下)なら8Br設定 */
        bruse = 2;
        break;
    }

    return bruse;
}
/**************************************************************************/
/**
 *  @brief  AxC設定(AXCBW設定値取得)
 *  @return AXCBW設定値
 *  @date   2015/11/02 TDI)satou その他未実装-003 create
 */
/**************************************************************************/
USHORT m_cmR_AxC_AXCBW(VOID)
{
    USHORT axcbw;
    USHORT car_idx;
    USHORT sysparaBandWidth;
    USHORT sysparaBandWidthSum = 0;
    USHORT bits;
    USHORT wcnum;

    for (car_idx = 0; car_idx < CMD_MAX_CARNUM; car_idx++)
    {
        sysparaBandWidth     = (USHORT)(cmw_sys_mng_tbl[CMD_SYS_S3G].s3g_bandwidth >> (3*car_idx));
        sysparaBandWidth    &= (USHORT)(BIT0|BIT1|BIT2);
        /*	最大帯域を設定する	*/
        if(sysparaBandWidthSum < sysparaBandWidth)
        {
        	sysparaBandWidthSum = sysparaBandWidth;
        }
    }

    bits = cmw_sys_mng_tbl[CMD_SYS_S3G].car_wk_flg;
    for (wcnum = 0; bits != 0; bits &= bits - 1) wcnum++;

    /* シスパラ/変数名E：LTE帯域幅にて設定 */
    if (CMD_LINE_BITRATE_24G == cmw_linebitrate)
    {
        if (CMD_CONN_CPRI_SHARED == cmw_connectinfo)
        {
            /*
             * CPRIレートが2.4G、HDLCレートが1920kbpsの場合 : LTE帯域幅
             * 但し、3Gキャリア運用有無の数とLTE帯域幅の合計が20Mを超える場合は、LTE帯域幅を減算
             */
            if (4 < (sysparaBandWidthSum + wcnum))
            {
                axcbw = 4 - wcnum;
            }
            else
            {
                axcbw = sysparaBandWidthSum;
            }
        }
        else
        {
            /* CPRIレートが2.4G、HDLCレートが960kbpsの場合 : 20M */
            axcbw = 4;
        }
    }
    else
    {
        /* CPRIレートが4.9G/9.8Gの場合、該当キャリアのLTE帯域幅が0以外の場合、20M */
        axcbw = (0 != sysparaBandWidthSum) ? 4 : 0;
    }

    return axcbw;
}
/**************************************************************************/
/**
 *  @brief  AxC設定(LTEBW設定値取得)
 *  @return LTEBW設定値
 *  @date   2015/11/02 TDI)satou その他未実装-003 create
 */
/**************************************************************************/
USHORT m_cmR_AxC_LTEBW(VOID)
{
    T_TRXSET_PARA_LTE trxpara;
    USHORT car_num;
    USHORT ltebw = 0;

    /* 実際にTRX設定されている帯域幅 */
    for (car_num = CMD_MIN_CARNUM; car_num <= CMD_MAX_CARNUM; car_num++)
    {
        f_cmn_com_trxset_para_lte_get(car_num, &trxpara);
        if (0 != trxpara.wSigKind)
        {
            /*	最大帯域を設定する	*/
            if(ltebw < trxpara.wBandWidth)
            {
	            ltebw = trxpara.wBandWidth;
	        }
        }
    }

    return ltebw;
}
/**************************************************************************/
/**
 *  @brief  AxC設定(LCNUM設定値取得)
 *  @return LCNUM設定値
 *  @date   2015/11/02 TDI)satou その他未実装-003 create
 */
/**************************************************************************/
USHORT m_cmR_AxC_LCNUM(VOID)
{
    USHORT car_idx;
    USHORT lcnum = 0;
    USHORT sysparaBandWidth[CMD_MAX_CARNUM] = {0};

    /* キャリア3,4,5のLTE帯域幅は無視する */
    for (car_idx = 0; car_idx < 2; car_idx++)
    {
        sysparaBandWidth[car_idx]  = (USHORT)(cmw_sys_mng_tbl[CMD_SYS_S3G].s3g_bandwidth >> (3*car_idx));
        sysparaBandWidth[car_idx] &= (USHORT)(BIT0|BIT1|BIT2);
    }

    switch (cmw_linebitrate)
    {
    case CMD_LINE_BITRATE_24G:
        if (CMD_CONN_CPRI_SHARED == cmw_connectinfo)
        {   /*
             * CPRIレートが2.4G、HDLCレートが1920kbpsの場合、
             * →キャリア1のLTE帯域幅が0以外なら1キャリア、0なら0キャリア
             */
            lcnum = (0 != sysparaBandWidth[0]) ? 1 : 0;
        }
        else
        {   /* CPRIレートが2.4G、HDLCレートが960kbpsの場合→1キャリア固定 */
            lcnum = 1;
        }
        break;
    case CMD_LINE_BITRATE_49G:
        if (cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num == 0)
        {   /*
             * CPRIレートが4.9G、送信系統数が0(2以下)の場合
             * →シスパラ/変数名E：キャリア1～キャリア2のLTE帯域幅が0以外であればキャリア数を加算する
             *   *TBD：0以外->4になる可能性あり
             */
            for (car_idx = 0; car_idx < CMD_MAX_CARNUM; car_idx++)
            {
                if (0 != sysparaBandWidth[car_idx]) lcnum++;
            }
        }
        else if (cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num == 1)
        {   /*
             * CPRIレートが4.9G、送信系統数が1(4以下)の場合
             * →シスパラ/変数名E：キャリア1のLTE帯域幅が0以外であればキャリア数を加算する
             *   *TBD：4の場合になる可能性あり
             */
            lcnum = (0 != sysparaBandWidth[0]) ? 1 : 0;
        }
        else
        {   /* CPRIレートが4.9G、送信系統数が2(8以下)の場合→キャリア数0固定 */
            lcnum = 0;
        }
        break;
    default:
        if (cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num <= 1)
        {   /*
             * CPRIレートが9.8G、送信系統数が0(2以下)の場合
             * CPRIレートが9.8G、送信系統数が1(4以下)の場合
             * →シスパラ/変数名E：キャリア1～キャリア2のLTE帯域幅が0以外であればキャリア数を加算する
             *   *TBD：0以外->4になる可能性あり
             */
            for (car_idx = 0, lcnum = 0; car_idx < CMD_MAX_CARNUM; car_idx++)
            {
                if (0 != sysparaBandWidth[car_idx]) lcnum++;
            }
        }
        else
        {
            if (0 != sysparaBandWidth[0]) lcnum++;
        }
        break;
    }

    return lcnum;
}
/**************************************************************************/
/**
 *  @brief  AxC設定(WCNUM設定値取得)
 *  @return WCNUM設定値
 *  @date   2015/11/02 TDI)satou その他未実装-003 create
 */
/**************************************************************************/
USHORT m_cmR_AxC_WCNUM(VOID)
{
    USHORT wcnum;
    USHORT bits = cmw_sys_mng_tbl[CMD_SYS_S3G].car_wk_flg;

    if (CMD_CONN_CPRI_SHARED == cmw_connectinfo)
    {   /* HDLCレートが1920kbps(CPRI共用)の場合→シスパラ/変数名E：3Gキャリア運用有無のONの個数 */
        for (wcnum = 0; bits != 0; bits &= bits - 1) wcnum++;
    }
    else
    {   /* HDLCレートが960kbps(LTE単独)の場合→0 */
        wcnum = 0;
    }

    return wcnum;
}
/**************************************************************************/
/**
 *  @brief  AxC設定(WCUSE設定値取得)
 *  @return WCUSE設定値
 *  @date   2015/11/02 TDI)satou その他未実装-003 create
 */
/**************************************************************************/
USHORT m_cmR_AxC_WCUSE(VOID)
{
    USHORT wcuse;

    if (CMD_CONN_CPRI_SHARED == cmw_connectinfo)
    {   /* HDLCレートが1920kbps(CPRI共用)の場合→シスパラ/変数名E：3Gキャリア運用有無そのもの */
        wcuse = cmw_sys_mng_tbl[CMD_SYS_S3G].car_wk_flg;
    }
    else
    {   /* HDLCレートが960kbps(LTE単独)の場合→0 */
        wcuse = 0;
    }

    return wcuse;
}
/**************************************************************************/
/**
 *  @brief  AxC設定
 *  @return None
 *  @date   2015/11/02 TDI)satou その他未実装-003 create
 */
/**************************************************************************/
VOID m_cmR_AxC(VOID)
{
    USHORT bruse, axcbw, ltebw, lcnum, wcnum, wcuse;

    bruse = m_cmR_AxC_BRUSE();
    axcbw = m_cmR_AxC_AXCBW();
    ltebw = m_cmR_AxC_LTEBW();
    lcnum = m_cmR_AxC_LCNUM();
    wcnum = m_cmR_AxC_WCNUM();
    wcuse = m_cmR_AxC_WCUSE();

    (VOID)rrhApi_Svp_Mnt_SetAxC(D_RRH_PROCQUE_L3, bruse, axcbw, ltebw, lcnum, wcnum, wcuse);
}

/** @} */

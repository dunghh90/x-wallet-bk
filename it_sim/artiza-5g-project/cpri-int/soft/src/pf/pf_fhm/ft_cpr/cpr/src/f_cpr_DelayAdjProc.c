/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file  f_cpr_DelayAdjProc.c
 *  @brief 遅延補正
 *  @date  2015/11/04 TDI)satou その他未実装-008 create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-2017
 */
/*********************************************************************************/
/**
 * @addtogroup RRH_PF_CPR
 * @{
 */
/*********************************************************************************/
/* include                                                                       */
/*********************************************************************************/
#include "f_sys_inc.h"
#include "f_cpr_inc.h"
#include "BPF_RU_ITCM.h"
#include "BPF_RU_HRTM.h"
#include "rrhApi_Cpri.h"
/*********************************************************************************/
/* define                                                                        */
/*********************************************************************************/
#define D_CPR_STEP10_ADJ_LTEDL_TRY_MAX (3)

/*********************************************************************************/
/* const                                                                         */
/*********************************************************************************/
/**
 * @brief   X_FHM
 * @date    2015/11/04 TDI)satou その他未実装-008 create
 */
const INT f_cprw_dly_X_FHM = 896;
/**
 * @brief   Y_FHM
 * @date    2015/11/04 TDI)satou その他未実装-008 create
 */
const INT f_cprw_dly_Y_FHM = 896;
/**
 * @brief   master port internal loop delay
 * @date    2015/11/04 TDI)satou その他未実装-008 create
 */
const INT f_cprw_dly_MasterPortInternal_loop_delay = 116;
/**
 * @brief   slave port internal loop delay
 * @date    2015/11/04 TDI)satou その他未実装-008 create
 */
const INT f_cprw_dly_SlavePortInternal_loop_delay  = -128;
/*********************************************************************************/
/* variables                                                                     */
/*********************************************************************************/
#define f_cprw_dly_alreadyAdjLteDL      f_cprw_dly_inf.f_cprw_dly_alreadyAdjLteDL
#define f_cprw_dly_cp_cprtdly           f_cprw_dly_inf.f_cprw_dly_cp_cprtdly
#define f_cprw_shm_conn                 f_cprw_dly_inf.f_cprw_shm_conn
#define f_cprw_shm_toffset_s3g          f_cprw_dly_inf.f_cprw_shm_toffset_s3g
#define f_cprw_shm_toffset_3g           f_cprw_dly_inf.f_cprw_shm_toffset_3g
#define f_cprw_dly_REn_T14              f_cprw_dly_inf.f_cprw_dly_REn_T14
#define f_cprw_dly_MAX_ReReport_Toffset f_cprw_dly_inf.f_cprw_dly_MAX_ReReport_Toffset
#define f_cprw_dly_MAX_ReReport_Dl      f_cprw_dly_inf.f_cprw_dly_MAX_ReReport_Dl
#define f_cprw_dly_MAX_ReReport_Ul      f_cprw_dly_inf.f_cprw_dly_MAX_ReReport_Ul
#define f_cprw_dly_MAX_T12_RE           f_cprw_dly_inf.f_cprw_dly_MAX_T12_RE
#define f_cprw_dly_MAX_Xn               f_cprw_dly_inf.f_cprw_dly_MAX_Xn
#define f_cprw_dly_MAX_Yn               f_cprw_dly_inf.f_cprw_dly_MAX_Yn
#define f_cprw_dly_MAX_Xn_plus_T12_RE   f_cprw_dly_inf.f_cprw_dly_MAX_Xn_plus_T12_RE
#define f_cprw_dly_MAX_Yn_plus_T12_RE   f_cprw_dly_inf.f_cprw_dly_MAX_Yn_plus_T12_RE
#define f_cprw_base_REn_T14             f_cprw_dly_inf.f_cprw_base_REn_T14
#define f_cprw_dly_adjLUDly             f_cprw_dly_inf.f_cprw_dly_adjLUDly
#define f_cprw_dly_adjWUDly             f_cprw_dly_inf.f_cprw_dly_adjWUDly
#define f_cprw_dly_adjLDDly             f_cprw_dly_inf.f_cprw_dly_adjLDDly
#define f_cprw_dly_adjWDDly             f_cprw_dly_inf.f_cprw_dly_adjWDDly
#define f_cprw_dly_failsafe_3G          f_cprw_dly_inf.f_cprw_dly_failsafe_3G
#define f_cprw_dly_failsafe_S3G         f_cprw_dly_inf.f_cprw_dly_failsafe_S3G

/**
 * @brief   遅延関連パラメータ
 * @date    2015/11/07 TDI)satou その他未実装-017 create
 */
struct
{
    T_RRH_TOFFSET_3G toffset_3g[D_RRH_CPRINO_RE_MAX];
    T_RRH_TOFFSET_S3G toffset_s3g[D_RRH_CPRINO_RE_MAX];
    INT REn_T14[D_RRH_CPRINO_RE_MAX];
    T_CPR_DLY_MAX MAX_ReReport_Toffset[CMD_MAX_SYSNUM];
    T_CPR_DLY_MAX MAX_ReReport_Dl[CMD_MAX_SYSNUM];
    T_CPR_DLY_MAX MAX_ReReport_Ul[CMD_MAX_SYSNUM];
    T_CPR_DLY_MAX MAX_T12_RE;
    T_CPR_DLY_MAX MAX_Xn[CMD_MAX_SYSNUM];
    T_CPR_DLY_MAX MAX_Yn[CMD_MAX_SYSNUM];
    T_CPR_DLY_MAX MAX_Xn_plus_T12_RE[CMD_MAX_SYSNUM];
    T_CPR_DLY_MAX MAX_Yn_plus_T12_RE[CMD_MAX_SYSNUM];
} f_cprw_backup_param;
/**
 * @brief   共有メモリ保存情報
 * @date    2015/11/04 TDI)satou その他未実装-008 create
 */
T_RRH_DELAY_INF f_cprw_dly_inf;

/**
 * @brief	Step10でLTE DL遅延補正時のT14
 * @note	初期値は、Step3において取得したT14値
 * 			(起動時から組み込まれているREについて、±8の範囲内にT14がおさまっているか判定に使用するT14)
 * @note	更新契機は、Step10においてLTE DL遅延補正実施時
 * @note	Step10でLTE DL遅延補正時のT14値を保存し、Step10で再度LTE DL遅延補正を実施しなおすか
 * 			どうか判定するために使用する. Step10において、Step3で決定したT14値と±8以内におさまっているが
 * 			前回Step10においてLTE DL遅延補正で測定したT14値と±8以内におさまらなかった場合、
 * 			LTE DL遅延補正を瞬断で実施したうえで、他の遅延補正を実施する.
 *
 * 			このような処理を行うのは、以下のような動作を行うREが存在し、
 * 			想定とは異なる値がLTE DL遅延補正に設定されてしまうのを防止するため.
 * 			- RECからToffset値報告要求を受信し、Step10を開始
 * 			- T14を測定した結果、Step7までに補正したT14測定結果と差分があることを検出
 *   			(例としてStep7:500, Step10:300)
 * 			- LTE DL遅延補正を実施
 * 			- CPRI再接続し、Step10でT14測定を行った結果、Step7で測定した結果に復帰
 * 				(Step7:500, Step10:300, Step10':500)
 *
 * 			上記のように動作すると、LTE DL補正の設定はT14:300の内容で補正し、
 * 			他の補正の設定はT14:500の内容で補正してしまう.
 *
 * @date	2016/04/17 TDI)satou 3201-15B-TS171
 */
static INT f_cprw_T14_Step10AdjLteDL[D_RRH_CPRINO_RE_MAX];

/**
 * @brief	Step10でのLTE DL遅延補正超過保護段数
 */
static USHORT f_cprw_Step10_TryCount_AdjLteDL[D_RRH_CPRINO_RE_MAX];

/**
 * @brief	Step10 LTE DL遅延補正、DL/UL位相差初期化実施有無
 */
static USHORT f_cprw_Step10_dly_AdjLteDL_f[D_RRH_CPRINO_RE_MAX];

/*********************************************************************************/
/* functions                                                                     */
/*********************************************************************************/
VOID f_cpr_dly_max_init(T_CPR_DLY_MAX *table)
{
    table->max = 0;
    table->max_re_num = D_RRH_CPRINO_RE_MIN;
    memset(table->REn, 0, sizeof table->REn);
}

VOID f_cpr_dly_max_get_max(T_CPR_DLY_MAX *table)
{
    USHORT cpr_idx;
    INT    first = D_RRH_ON;

    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    {
        if (1 == f_cprw_dly_cp_cprtdly[cpr_idx].valid)
        {
            if (D_RRH_ON == first)
            {
                first             = D_RRH_OFF;
                table->max        = table->REn[cpr_idx];
                table->max_re_num = cpr_idx + 1;
            }
            else
            {
                if (table->max < table->REn[cpr_idx])
                {
                    table->max        = table->REn[cpr_idx];
                    table->max_re_num = cpr_idx + 1;
                }
            }
        }
    }
}
/*********************************************************************************/
/**
* @brief    Step3パラメータ退避
* @return   None
* @date     2015/11/07 TDI)satou その他未実装-017 create
*/
/*********************************************************************************/
VOID f_cpr_backup_param(VOID)
{
    USHORT cpr_idx;

    for (cpr_idx = 0; cpr_idx < D_RRH_CPRINO_RE_MAX; cpr_idx++)
    {
        f_cprw_backup_param.toffset_3g [cpr_idx] = f_cprw_shm_toffset_3g [cpr_idx];
        f_cprw_backup_param.toffset_s3g[cpr_idx] = f_cprw_shm_toffset_s3g[cpr_idx];
        f_cprw_backup_param.REn_T14    [cpr_idx] = f_cprw_dly_REn_T14    [cpr_idx];
    }

    f_cprw_backup_param.MAX_ReReport_Toffset[CMD_SYS_3G ] = f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_3G ];
    f_cprw_backup_param.MAX_ReReport_Toffset[CMD_SYS_S3G] = f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_S3G];
    f_cprw_backup_param.MAX_ReReport_Dl     [CMD_SYS_3G ] = f_cprw_dly_MAX_ReReport_Dl     [CMD_SYS_3G ];
    f_cprw_backup_param.MAX_ReReport_Dl     [CMD_SYS_S3G] = f_cprw_dly_MAX_ReReport_Dl     [CMD_SYS_S3G];
    f_cprw_backup_param.MAX_ReReport_Ul     [CMD_SYS_3G ] = f_cprw_dly_MAX_ReReport_Ul     [CMD_SYS_3G ];
    f_cprw_backup_param.MAX_ReReport_Ul     [CMD_SYS_S3G] = f_cprw_dly_MAX_ReReport_Ul     [CMD_SYS_S3G];
    f_cprw_backup_param.MAX_T12_RE                        = f_cprw_dly_MAX_T12_RE;
    f_cprw_backup_param.MAX_Xn              [CMD_SYS_3G ] = f_cprw_dly_MAX_Xn              [CMD_SYS_3G ];
    f_cprw_backup_param.MAX_Xn              [CMD_SYS_S3G] = f_cprw_dly_MAX_Xn              [CMD_SYS_S3G];
    f_cprw_backup_param.MAX_Yn              [CMD_SYS_3G ] = f_cprw_dly_MAX_Yn              [CMD_SYS_3G ];
    f_cprw_backup_param.MAX_Yn              [CMD_SYS_S3G] = f_cprw_dly_MAX_Yn              [CMD_SYS_S3G];
    f_cprw_backup_param.MAX_Xn_plus_T12_RE  [CMD_SYS_3G ] = f_cprw_dly_MAX_Xn_plus_T12_RE  [CMD_SYS_3G ];
    f_cprw_backup_param.MAX_Xn_plus_T12_RE  [CMD_SYS_S3G] = f_cprw_dly_MAX_Xn_plus_T12_RE  [CMD_SYS_S3G];
    f_cprw_backup_param.MAX_Yn_plus_T12_RE  [CMD_SYS_3G ] = f_cprw_dly_MAX_Yn_plus_T12_RE  [CMD_SYS_3G ];
    f_cprw_backup_param.MAX_Yn_plus_T12_RE  [CMD_SYS_S3G] = f_cprw_dly_MAX_Yn_plus_T12_RE  [CMD_SYS_S3G];
}
/*********************************************************************************/
/**
* @brief    CP_CPRTDLY取得
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Get_CP_CPRTDLY(USHORT cpr_num)
{
    UINT regAdr, regDat;
    USHORT cpr_idx;
    UINT mask, shift;

    cpr_idx = cpr_num - 1;
    regAdr  = M_RRH_REG_CPRTDLY(cpr_num);
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regDat);

    f_cprw_dly_cp_cprtdly[cpr_idx].regDat = regDat;

    mask  = BIT31;
    shift = 31;
    f_cprw_dly_cp_cprtdly[cpr_idx].valid       = (regDat & mask) >> shift;

    mask  = (BIT21|BIT20|BIT19|BIT18|BIT17|BIT16|BIT15|BIT14);
    shift = 14;
    f_cprw_dly_cp_cprtdly[cpr_idx].hyper_frame = (regDat & mask) >> shift;

    mask  = (BIT13|BIT12|BIT11|BIT10|BIT9|BIT8|BIT7|BIT6);
    shift = 6;
    f_cprw_dly_cp_cprtdly[cpr_idx].basic_frame = (regDat & mask) >> shift;

    mask  = (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);
    f_cprw_dly_cp_cprtdly[cpr_idx].clock       = (regDat & mask);
}
/*********************************************************************************/
/**
* @brief    REn_T14取得
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Get_REn_T14(USHORT cpr_num)
{
    USHORT cpr_idx;

    cpr_idx = cpr_num - 1;

    f_cprw_dly_REn_T14[cpr_idx] = 0;

    /* ValidになっているMaster portについて測定する */
    if (1 == f_cprw_dly_cp_cprtdly[cpr_idx].valid)
    {
        /*
         * REn_T14 = CP_CPRTDLY[15:0] - {Master Internal loop delay}
         * @245.76MHz 計算結果が負の場合は0クリップ
         */
        f_cprw_dly_REn_T14[cpr_idx] = (f_cprw_dly_cp_cprtdly[cpr_idx].regDat & 0x3FFFFF) - f_cprw_dly_MasterPortInternal_loop_delay;
        if (0 > f_cprw_dly_REn_T14[cpr_idx])
        {
            f_cprw_dly_REn_T14[cpr_idx] = 0;
        }
    }
}
/*********************************************************************************/
/**
* @brief    Toffset_REn取得
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Get_Toffset_REn(USHORT cpr_num)
{
    USHORT cpr_idx;

    cpr_idx = cpr_num - 1;

    if ((0 != f_cprw_shm_toffset_3g[cpr_idx].signal_kind) &&  (0 == f_cprw_shm_toffset_3g[cpr_idx].result))
    {
        f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_3G ].REn[cpr_idx] = lround(f_cprw_shm_toffset_3g[cpr_idx].toffset    *16.0/25.0);
        f_cprw_dly_MAX_ReReport_Dl     [CMD_SYS_3G ].REn[cpr_idx] = lround(f_cprw_shm_toffset_3g[cpr_idx].downdeleqp *16.0/25.0);
        f_cprw_dly_MAX_ReReport_Ul     [CMD_SYS_3G ].REn[cpr_idx] = lround(f_cprw_shm_toffset_3g[cpr_idx].updeleqp   *16.0/25.0);
    }

    if ((0 != f_cprw_shm_toffset_s3g[cpr_idx].signal_kind) &&  (0 == f_cprw_shm_toffset_s3g[cpr_idx].result))
    {
        f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_S3G].REn[cpr_idx] = lround(f_cprw_shm_toffset_s3g[cpr_idx].toffset   *16.0/25.0);
        f_cprw_dly_MAX_ReReport_Dl     [CMD_SYS_S3G].REn[cpr_idx] = lround(f_cprw_shm_toffset_s3g[cpr_idx].downdeleqp*16.0/25.0);
        f_cprw_dly_MAX_ReReport_Ul     [CMD_SYS_S3G].REn[cpr_idx] = lround(f_cprw_shm_toffset_s3g[cpr_idx].updeleqp  *16.0/25.0);
    }
}
/*********************************************************************************/
/**
* @brief    T12_REn取得
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Get_T12_REn(USHORT cpr_num)
{
    USHORT cpr_idx;
    DL REn_T14;
    DL Toffset_REn;

    cpr_idx = cpr_num - 1;

    /*
     * T12_REn = T34_REn = (REn_T14 - Toffset_REn) / 2
     * 〈@245.76MHz、計算結果は四捨五入し、負の場合は0クリップ〉
     */
    REn_T14     = (DL)f_cprw_dly_REn_T14[cpr_idx];
    Toffset_REn = (DL)f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_S3G].REn[cpr_idx];

    f_cprw_dly_MAX_T12_RE.REn[cpr_idx] = lround((REn_T14 - Toffset_REn)/2);
    if (0 > f_cprw_dly_MAX_T12_RE.REn[cpr_idx])
    {
        f_cprw_dly_MAX_T12_RE.REn[cpr_idx] = 0;
    }
}
/*********************************************************************************/
/**
* @brief    Xn取得
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
* @date     2015/12/01 TDI)satou ハードソフト仕様書v1.11対応
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Get_Xn(USHORT sys_idx, USHORT cpr_num)
{
    USHORT cpr_idx;
    INT X_REn;

    cpr_idx = cpr_num - 1;

    X_REn  = f_cprw_dly_MAX_ReReport_Dl[sys_idx].REn[cpr_idx];

    f_cprw_dly_MAX_Xn[sys_idx].REn[cpr_idx] = X_REn;
}
/*********************************************************************************/
/**
* @brief    Yn取得
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
* @date     2015/12/01 TDI)satou ハードソフト仕様書v1.11対応
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Get_Yn(USHORT sys_idx, USHORT cpr_num)
{
    USHORT cpr_idx;
    INT Y_REn;

    cpr_idx = cpr_num - 1;

    Y_REn  = f_cprw_dly_MAX_ReReport_Ul[sys_idx].REn[cpr_idx];

    f_cprw_dly_MAX_Yn[sys_idx].REn[cpr_idx] = Y_REn;
}
/*********************************************************************************/
/**
* @brief    Xn+T12_RE最大値取得
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Get_Xn_plus_T12_RE(USHORT sys_idx, USHORT cpr_num)
{
    USHORT cpr_idx;
    INT Xn, T12_RE;

    cpr_idx = cpr_num - 1;

    Xn     = f_cprw_dly_MAX_Xn[sys_idx].REn[cpr_idx];
    T12_RE = f_cprw_dly_MAX_T12_RE.REn[cpr_idx];
    f_cprw_dly_MAX_Xn_plus_T12_RE[sys_idx].REn[cpr_idx] = Xn + T12_RE;
}
/*********************************************************************************/
/**
* @brief    Yn+T12_RE最大値取得
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Get_Yn_plus_T12_RE(USHORT sys_idx, USHORT cpr_num)
{
    USHORT cpr_idx;
    INT Yn, T12_RE;

    cpr_idx = cpr_num - 1;

    Yn     = f_cprw_dly_MAX_Yn[sys_idx].REn[cpr_idx];
    T12_RE = f_cprw_dly_MAX_T12_RE.REn[cpr_idx];
    f_cprw_dly_MAX_Yn_plus_T12_RE[sys_idx].REn[cpr_idx] = Yn + T12_RE;
}
/*********************************************************************************/
/**
* @brief    最大遅延取得
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
* @date     2015/11/07 TDI)satou 問処番号No.161
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Get_MAX_Delay(VOID)
{
    INT T12_RE;
    USHORT max_re_num;
    T_RRH_CPRI_STATE shmCpriSta;
    INT isMatch_MaxRE = D_RRH_FALSE;

    f_cmn_com_cpri_state_get(&shmCpriSta);

    if (CMD_CONN_S3G_SINGLE == shmCpriSta.cpri_linktype)
    {
        /* LTE下り遅延 + ファイバ遅延が最大となるRE、
         * LTE上り遅延 + ファイバ遅延が最大となるRE一致するとき */
        if (f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max_re_num == f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_S3G].max_re_num)
        {
            isMatch_MaxRE = D_RRH_TRUE;
            max_re_num = f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max_re_num;
        }
    }
    else
    {
        if ((f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max_re_num == f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_S3G].max_re_num)
        &&  (f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max_re_num == f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_3G ].max_re_num)
        &&  (f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max_re_num == f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_3G ].max_re_num))
        {
            isMatch_MaxRE = D_RRH_TRUE;
            max_re_num = f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max_re_num;
        }
    }

    f_cprw_dly_inf.MaxT12Re                         = f_cprw_dly_MAX_T12_RE.max_re_num;
    f_cprw_dly_inf.MaxDlEquipDelayReNo[CMD_SYS_3G ] = f_cprw_dly_MAX_ReReport_Dl   [CMD_SYS_3G ].max_re_num;
    f_cprw_dly_inf.MaxDlEquipDelayReNo[CMD_SYS_S3G] = f_cprw_dly_MAX_ReReport_Dl   [CMD_SYS_S3G].max_re_num;
    f_cprw_dly_inf.MaxUlEquipDelayReNo[CMD_SYS_3G ] = f_cprw_dly_MAX_ReReport_Ul   [CMD_SYS_3G ].max_re_num;
    f_cprw_dly_inf.MaxUlEquipDelayReNo[CMD_SYS_S3G] = f_cprw_dly_MAX_ReReport_Ul   [CMD_SYS_S3G].max_re_num;
    max_re_num                                      = f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_3G ].max_re_num;
    f_cprw_dly_inf.MaxDlDelayReNo[CMD_SYS_3G]       = max_re_num;
    f_cprw_dly_inf.MaxDlDelayRe[CMD_SYS_3G ]        = f_cprw_shm_toffset_3g [max_re_num - 1].downdeleqp;
    max_re_num                                      = f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max_re_num;
    f_cprw_dly_inf.MaxDlDelayReNo[CMD_SYS_S3G]      = max_re_num;
    f_cprw_dly_inf.MaxDlDelayRe[CMD_SYS_S3G]        = f_cprw_shm_toffset_s3g[max_re_num - 1].downdeleqp;
    max_re_num                                      = f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_3G ].max_re_num;
    f_cprw_dly_inf.MaxUlDelayReNo[CMD_SYS_3G]       = max_re_num;
    f_cprw_dly_inf.MaxUlDelayRe[CMD_SYS_3G ]        = f_cprw_shm_toffset_3g [max_re_num - 1].updeleqp;
    max_re_num                                      = f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_S3G].max_re_num;
    f_cprw_dly_inf.MaxUlDelayReNo[CMD_SYS_S3G]      = max_re_num;
    f_cprw_dly_inf.MaxUlDelayRe[CMD_SYS_S3G]        = f_cprw_shm_toffset_s3g[max_re_num - 1].updeleqp;

    if (D_RRH_TRUE == isMatch_MaxRE)
    {
        /* LTE下り遅延 + ファイバ遅延が最大となるRE、
         * LTE上り遅延 + ファイバ遅延が最大となるRE、
         * Wcdma下り遅延 + ファイバ遅延が最大となるRE、
         * Wcdma上り遅延 + ファイバ遅延が最大となるREが一致するとき
         * FHMの最大LTE下り遅延    ＝ MAX[LTE_X+T12_RE]
         * FHMの最大LTE上り遅延    ＝ MAX[LTE_Y+T12_RE]
         * FHMの最大Wcdma下り遅延  ＝ MAX[Wcdma_X+T12_RE]
         * FHMの最大Wcdma上り遅延  ＝ MAX[Wcdma_Y+T12_RE]
         * 最大ファイバ遅延　　　　＝ MAX[LTE_X+T12_RE]のT12_RE */
        f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G ]     = f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_3G ].max;
        f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G]     = f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max;
        f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G ]     = f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_3G ].max;
        f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G]     = f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_S3G].max;
        f_cprw_dly_inf.MaxFiberDelay                  = f_cprw_dly_MAX_T12_RE.REn[f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].max_re_num - 1];
    }
    else
    {
        /* FHMの最大LTE下り遅延   ＝ MAX[LTE_X] + MAX[T12_RE]
         * FHMの最大LTE上り遅延   ＝ MAX[LTE_Y] + MAX[T12_RE]
         * FHMの最大Wcdma下り遅延 ＝ MAX[Wcdma_X] + MAX[T12_RE]
         * FHMの最大Wcdma上り遅延 ＝ MAX[Wcdma_Y] + MAX[T12_RE]
         * 最大ファイバ遅延       ＝ MAX[T12_RE] */
        T12_RE = f_cprw_dly_MAX_T12_RE.max;
        f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G ]     = f_cprw_dly_MAX_Xn[CMD_SYS_3G ].max + T12_RE;
        f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G]     = f_cprw_dly_MAX_Xn[CMD_SYS_S3G].max + T12_RE;
        f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G ]     = f_cprw_dly_MAX_Yn[CMD_SYS_3G ].max + T12_RE;
        f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G]     = f_cprw_dly_MAX_Yn[CMD_SYS_S3G].max + T12_RE;
        f_cprw_dly_inf.MaxFiberDelay                  = T12_RE;
    }
}
/*********************************************************************************/
/**
* @brief    パラメータ収集
* @return   None
* @date     2015/11/17 TDI)satou
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_CollectParam_T14_toffset(USHORT cpr_num)
{
    USHORT cpr_idx = cpr_num - 1;

    f_cmn_com_toffset_3g_get(cpr_num, &f_cprw_shm_toffset_3g[cpr_idx]);
    f_cmn_com_toffset_s3g_get(cpr_num, &f_cprw_shm_toffset_s3g[cpr_idx]);

    /* CP_CPRTDLY取得       */
    f_cpr_DelayAdjustProc_Get_CP_CPRTDLY(cpr_num);
}
/*********************************************************************************/
/**
* @brief    パラメータ収集
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
* @date     2015/11/17 TDI)satou
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_CollectParam_REn(USHORT cpr_num)
{
    /* REn_T14取得          */
    f_cpr_DelayAdjustProc_Get_REn_T14(cpr_num);
    /* Toffset_REn取得      */
    f_cpr_DelayAdjustProc_Get_Toffset_REn(cpr_num);
    /* T12_REn取得          */
    f_cpr_DelayAdjustProc_Get_T12_REn(cpr_num);
    /* Xn取得(3G)           */
    f_cpr_DelayAdjustProc_Get_Xn(CMD_SYS_3G, cpr_num);
    /* Xn取得(S3G)          */
    f_cpr_DelayAdjustProc_Get_Xn(CMD_SYS_S3G, cpr_num);
    /* Yn取得(3G)           */
    f_cpr_DelayAdjustProc_Get_Yn(CMD_SYS_3G, cpr_num);
    /* Yn取得(S3G)          */
    f_cpr_DelayAdjustProc_Get_Yn(CMD_SYS_S3G, cpr_num);
    /* Xn+T12_RE取得(3G)    */
    f_cpr_DelayAdjustProc_Get_Xn_plus_T12_RE(CMD_SYS_3G, cpr_num);
    /* Xn+T12_RE取得(S3G)   */
    f_cpr_DelayAdjustProc_Get_Xn_plus_T12_RE(CMD_SYS_S3G, cpr_num);
    /* Yn+T12_RE取得(3G)    */
    f_cpr_DelayAdjustProc_Get_Yn_plus_T12_RE(CMD_SYS_3G, cpr_num);
    /* Yn+T12_RE取得(S3G)   */
    f_cpr_DelayAdjustProc_Get_Yn_plus_T12_RE(CMD_SYS_S3G, cpr_num);
}
/*********************************************************************************/
/**
* @brief    パラメータ収集
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_CollectParam_Step3(VOID)
{
    USHORT cpr_num, cpr_idx;

    f_cpr_dly_max_init(&f_cprw_dly_MAX_T12_RE                       );
    f_cpr_dly_max_init(&f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_3G ]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_S3G]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_ReReport_Dl     [CMD_SYS_3G ]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_ReReport_Dl     [CMD_SYS_S3G]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_ReReport_Ul     [CMD_SYS_3G ]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_ReReport_Ul     [CMD_SYS_S3G]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_Xn              [CMD_SYS_3G ]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_Xn              [CMD_SYS_S3G]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_Yn              [CMD_SYS_3G ]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_Yn              [CMD_SYS_S3G]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_Xn_plus_T12_RE  [CMD_SYS_3G ]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_Xn_plus_T12_RE  [CMD_SYS_S3G]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_Yn_plus_T12_RE  [CMD_SYS_3G ]);
    f_cpr_dly_max_init(&f_cprw_dly_MAX_Yn_plus_T12_RE  [CMD_SYS_S3G]);

    for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
    {
        f_cpr_DelayAdjustProc_CollectParam_REn(cpr_num);
        /*	最大超過補正値を初期化する	*/
        cpr_idx = cpr_num - 1;
        f_cprw_dly_adjLUDly[cpr_idx] = 0;
        f_cprw_dly_adjWUDly[cpr_idx] = 0;
        f_cprw_dly_adjLDDly[cpr_idx] = 0;
        f_cprw_dly_adjWDDly[cpr_idx] = 0;
        f_cprw_Step10_dly_AdjLteDL_f[cpr_idx] = 0;
    }

    /* 最大遅延取得. 最大値はStep3で決定する.
     * 以降はRECとのリンク断が発生してStep3をやり直さない限り変更しない.
     * つまり、f_cpr_dly_max_get_max()を呼ぶのはこの関数の場所のみのはず */
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_3G ]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_ReReport_Toffset[CMD_SYS_S3G]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_ReReport_Dl     [CMD_SYS_3G ]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_ReReport_Dl     [CMD_SYS_S3G]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_ReReport_Ul     [CMD_SYS_3G ]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_ReReport_Ul     [CMD_SYS_S3G]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_T12_RE                       );
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_Xn              [CMD_SYS_3G ]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_Xn              [CMD_SYS_S3G]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_Yn              [CMD_SYS_3G ]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_Yn              [CMD_SYS_S3G]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_Xn_plus_T12_RE  [CMD_SYS_3G ]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_Xn_plus_T12_RE  [CMD_SYS_S3G]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_Yn_plus_T12_RE  [CMD_SYS_3G ]);
    f_cpr_dly_max_get_max(&f_cprw_dly_MAX_Yn_plus_T12_RE  [CMD_SYS_S3G]);
    f_cpr_DelayAdjustProc_Get_MAX_Delay();

    /* step7で比較するT14を決定 */
    for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
    {
        cpr_idx = cpr_num - 1;
        f_cprw_base_REn_T14[cpr_idx] = f_cprw_dly_REn_T14[cpr_idx];
        f_cprw_T14_Step10AdjLteDL[cpr_idx] = f_cprw_dly_REn_T14[cpr_idx];
    }
}
/*********************************************************************************/
/**
* @brief    パラメータ収集
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
* @date     2015/11/17 TDI)satou
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_CollectParam_Step7(USHORT cpr_num)
{
    USHORT cpr_idx = cpr_num - 1;
    UINT regDat, regDat_sfpCnt;

    f_cpr_DelayAdjustProc_CollectParam_T14_toffset(cpr_num);
    f_cpr_DelayAdjustProc_CollectParam_REn(cpr_num);

    if (1 != f_cprw_dly_cp_cprtdly[cpr_idx].valid) {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPST(cpr_num), &regDat);
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_SFPCNT(cpr_num), &regDat_sfpCnt);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "[STEP7]cpri=%d valid=%d regDat=%x hfnsync=%x sfpcnt=%x",
                cpr_num, f_cprw_dly_cp_cprtdly[cpr_idx].valid, f_cprw_dly_cp_cprtdly[cpr_idx].regDat, regDat, regDat_sfpCnt);
    }
}
/*********************************************************************************/
/**
* @brief    パラメータ収集
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
* @date     2015/11/17 TDI)satou
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_CollectParam_Step10(USHORT cpr_num)
{
    USHORT cpr_idx = cpr_num - 1;
    UINT regDat, regDat_sfpCnt;

    f_cpr_DelayAdjustProc_CollectParam_T14_toffset(cpr_num);
    f_cpr_DelayAdjustProc_CollectParam_REn(cpr_num);

    if (1 != f_cprw_dly_cp_cprtdly[cpr_idx].valid) {
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPST(cpr_num), &regDat);
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_SFPCNT(cpr_num), &regDat_sfpCnt);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "[STEP10]cpri=%d valid=%d regDat=%x hfnsync=%x sfpcnt=%x",
                cpr_num, f_cprw_dly_cp_cprtdly[cpr_idx].valid, f_cprw_dly_cp_cprtdly[cpr_idx].regDat, regDat, regDat_sfpCnt);
    }
}
/*********************************************************************************/
/**
* @brief    C-Plane補正(FHM Toffset)
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Set_Toffset_FHM(VOID)
{
    UINT regVal;
    INT  Toffset_FHM;

    Toffset_FHM  = (f_cprw_dly_inf.MaxFiberDelay + f_cprw_dly_X_FHM)*2;
    Toffset_FHM += f_cprw_dly_SlavePortInternal_loop_delay;
    if (0 > Toffset_FHM) Toffset_FHM = 0;

    regVal = Toffset_FHM & 0xFFFF;
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, D_RRH_REG_MIX_FHMTOFST, &regVal);
}
/*********************************************************************************/
/**
* @brief    LTE下り遅延補正設定値チェック
* @return   チェック結果
* @retval   D_RRH_OK : 正常
* @retval   D_RRH_NG : レジスタ最大値超過
* @date     2016/01/05 TDI)satou create
*/
/*********************************************************************************/
INT f_cpr_DelayAdjustProc_CheckSetVal_Adjust_LTE_DL(USHORT cpr_num)
{
    UINT regVal;
    INT LTE_Xn, T12_REn;
    USHORT cpr_idx;

    /*
     * 最大のLTE下り遅延に、各REのLTE下り遅延を合わせる.
     * LTE下り遅延補正 = LD遅延オフセット + FHMの最大LTE下り遅延 - { LTE_Xn + T12_REn }
     */
    cpr_idx = cpr_num - 1;

    if (D_RRH_OFF == f_cprw_shm_conn.conn_disc[cpr_idx])
    {
        return D_RRH_OK;
    }

    LTE_Xn  = f_cprw_dly_MAX_Xn[CMD_SYS_S3G].REn[cpr_idx];
    T12_REn = f_cprw_dly_MAX_T12_RE.REn[cpr_idx];
    regVal  = D_CPR_LDDELAY_OFFSET + f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G] - (LTE_Xn + T12_REn);

    /*
     * LTE用設定値[DLY]の計算結果が、レジスタ設定上限値を超えた場合は、
     * 該当配下REを強制停止としRE強制停止(遅延調整NG)ERRとする。
     */
    if (D_CPR_DRYADJ_UPLIMIT_MAX < regVal)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "over threshold. cpr_num=%d DIS_LDL=%08x", cpr_num, regVal);
        return D_RRH_NG;
    }

    return D_RRH_OK;
}
/*********************************************************************************/
/**
* @brief    LTE下り遅延補正
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Adjust_LTE_DL(USHORT cpr_num)
{
    UINT regAdr, regVal;
    INT LTE_Xn, T12_REn;
    USHORT cpr_idx;

    /*
     * 最大のLTE下り遅延に、各REのLTE下り遅延を合わせる.
     * LTE下り遅延補正 = LD遅延オフセット + FHMの最大LTE下り遅延 - { LTE_Xn + T12_REn }
     */
    cpr_idx = cpr_num - 1;

    if (D_RRH_OFF == f_cprw_shm_conn.conn_disc[cpr_idx])
    {
        return;
    }

    LTE_Xn  = f_cprw_dly_MAX_Xn[CMD_SYS_S3G].REn[cpr_idx];
    T12_REn = f_cprw_dly_MAX_T12_RE.REn[cpr_idx];

    regAdr  = M_RRH_REG_DIS_LDL(cpr_num);
    regVal  = D_CPR_LDDELAY_OFFSET + f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G] - (LTE_Xn + T12_REn);
    regVal &= 0xFFFF;

    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);

    /* mod(LTE下り遅延補正, 64) ... Wcdma DL補正値計算時に使用する */
    f_cprw_dly_inf.Calc42n[cpr_idx] = regVal % 64;

    /* CPM#n LTE DL補正済み設定 */
    f_cprw_dly_alreadyAdjLteDL[cpr_idx] = D_RRH_ON;
}
/*********************************************************************************/
/**
* @brief    DL/UL位相差初期化
* @return   None
* @date     2015/11/04 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustProc_Init_MIX_LUL(USHORT cpr_num)
{
    UINT bitifno, regAdr;
    USHORT cpr_idx;

    bitifno = (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5);

    cpr_idx = cpr_num - 1;

    if (D_RRH_OFF == f_cprw_shm_conn.conn_disc[cpr_idx])
    {
        return;
    }

    regAdr = M_RRH_REG_MIX_LUL(cpr_num);
    (VOID)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE_CPRI, regAdr,  bitifno);
}
/*********************************************************************************/
/**
* @brief 	遅延補正(Step3)
* @param 	buff_adr [in] the buffer address pointer of received message
* @return 	None
* @date 	2015/11/04 TDI)satou その他未実装-008 create
* @date     2015/11/17 TDI)satou
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustStep3Proc(UINT *buff_adr)
{
    UINT regAdr, regVal, regDat, regDat_sfpCnt;
    UINT cpr_num, cpr_idx;

#ifdef OPT_RRH_SIM
    printf("%s start\n", __func__);
#endif
    /* --------------------------------------------------
     * 遅延調整パラメータ取得
     * -------------------------------------------------- */
    f_cpr_DelayAdjust_ClearState();

    /* 共有メモリからRE使用/未使用取得 */
    f_cmn_com_conn_disk_get(&f_cprw_shm_conn);

    for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
    {
        cpr_idx = cpr_num - 1;
        f_cpr_DelayAdjustProc_CollectParam_T14_toffset(cpr_num);

        if (1 != f_cprw_dly_cp_cprtdly[cpr_idx].valid) {
            (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_CPST(cpr_num), &regDat);
            (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRI_SFPCNT(cpr_num), &regDat_sfpCnt);
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "[STEP3]cpri=%d valid=%d regDat=%x hfnsync=%x sfpcnt=%x, cpriUse=%d",
                    cpr_num, f_cprw_dly_cp_cprtdly[cpr_idx].valid, f_cprw_dly_cp_cprtdly[cpr_idx].regDat, regDat, regDat_sfpCnt, f_cprw_shm_conn.conn_disc[cpr_idx]);
        }
    }

    /* RECへの応答結果を決定する */
    f_cprw_dly_inf.toffsetRslt[CMD_SYS_3G] = CMD_RPT_NG;
    for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
    {
        cpr_idx = cpr_num - 1;
        if (0 != f_cprw_shm_toffset_3g[cpr_idx].signal_kind)
        {
            f_cprw_dly_inf.toffsetRslt[CMD_SYS_3G] = CMD_NML;
            break;
        }
    }
    f_cprw_dly_inf.toffsetRslt[CMD_SYS_S3G] = CMD_HDL_NG;
    for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
    {
        cpr_idx = cpr_num - 1;
        if (0 != f_cprw_shm_toffset_s3g[cpr_idx].signal_kind)
        {
            f_cprw_dly_inf.toffsetRslt[CMD_SYS_S3G] = CMD_NML;
            break;
        }
    }

    /* --------------------------------------------------
     * 【Step4】
     * -------------------------------------------------- */
    /* CPS 光OFF & CPRI Disable設定   */
    /* CPM#n 光OFF & CPRI Disable設定 */
    for (cpr_num = D_RRH_CPRINO_REC; cpr_num < D_RRH_CPRINO_NUM; cpr_num++)
    {
        regAdr = M_RRH_REG_CPRI_SFPCNT(cpr_num);
        (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
        regVal &= ~(BIT0|BIT1|BIT2);
        regVal |= BIT0;
        (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);
    }

    /* --------------------------------------------------
     * 【Step5】
     * -------------------------------------------------- */
    /* MAX遅延 RE判定 FHM_Toffset値算出 */
    f_cpr_DelayAdjustProc_CollectParam_Step3();

    /* FHM_Toffset設定 */
    f_cpr_DelayAdjustProc_Set_Toffset_FHM();

    for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
    {   /*
         * Master Port(有効Port)のLTE DL遅延補正
         * レジスタ設定値の最大値を超過してしまう場合は設定をスキップする。
         * ここでMasterのCPRIリンクは切断されてしまうため、強制停止に遷移するのは
         * ReSync後になる。Step7の処理において、LTE DL遅延補正していない場合に強制停止を設定する。
         */
        if (D_RRH_OK != f_cpr_DelayAdjustProc_CheckSetVal_Adjust_LTE_DL(cpr_num))
        {
            continue;
        }

        f_cpr_DelayAdjustProc_Adjust_LTE_DL(cpr_num);

        /* Master Port(有効Port)のDL/UL位相差初期化 */
        f_cpr_DelayAdjustProc_Init_MIX_LUL(cpr_num);
    }

    /* Wait 1ms(光OFF時間1ms以上維持の為) */
    usleep(1000);

    /* CPS 光ON(AUTO) & CPRI Enable設定 */
    regAdr = M_RRH_REG_CPRI_SFPCNT(D_RRH_CPRINO_REC);
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
    regVal &= ~(BIT0|BIT1|BIT2);
    regVal |= BIT2;
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);

    /* パラメータ退避 */
    f_cpr_backup_param();
    f_cmn_com_delay_inf_set(&f_cprw_dly_inf);

    /* ログ */
    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "T14 in step3. #1:%d #2:%d #3:%d #4:%d #5:%d #6:%d #7:%d #8:%d",
            f_cprw_backup_param.REn_T14[ 0], f_cprw_backup_param.REn_T14[ 1],
            f_cprw_backup_param.REn_T14[ 2], f_cprw_backup_param.REn_T14[ 3],
            f_cprw_backup_param.REn_T14[ 4], f_cprw_backup_param.REn_T14[ 5],
            f_cprw_backup_param.REn_T14[ 6], f_cprw_backup_param.REn_T14[ 7]);
    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "T14 in step3. #9:%d #10:%d #11:%d #12:%d #13:%d #14:%d #15:%d #16:%d",
            f_cprw_backup_param.REn_T14[ 8], f_cprw_backup_param.REn_T14[ 9],
            f_cprw_backup_param.REn_T14[10], f_cprw_backup_param.REn_T14[11],
            f_cprw_backup_param.REn_T14[12], f_cprw_backup_param.REn_T14[13],
            f_cprw_backup_param.REn_T14[14], f_cprw_backup_param.REn_T14[15]);
}
/*********************************************************************************/
/**
* @brief    遅延補正関連パラメータ一致チェック
* @return   None
* @date     2015/11/07 TDI)satou その他未実装-017 create
* @date     2015/11/24 TDI)enoki ±4 ⇒ ±8 に変更(ハソ変更)
*/
/*********************************************************************************/
INT f_cpr_DelayAdjust_IsMatchParam(USHORT cpr_num)
{
    USHORT cpr_idx = cpr_num - 1;
    INT absVal;
    INT isMatch = D_RRH_NG;

    if (1 == f_cprw_dly_cp_cprtdly[cpr_idx].valid)
    {
        /* T14測定値が±8以上で差分ありと判断する */
        absVal = f_cprw_base_REn_T14[cpr_idx] - f_cprw_dly_REn_T14[cpr_idx];
        absVal = abs(absVal);
        if (8 > absVal)
        {
            isMatch = D_RRH_OK;
        }
        else
        {
            isMatch = D_RRH_NG;
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Over T14 threshold. cpri=%d old=%d new=%d",
                    cpr_num, f_cprw_base_REn_T14[cpr_idx], f_cprw_dly_REn_T14[cpr_idx]);
        }
    }

    return isMatch;
}
/*********************************************************************************/
/**
* @brief    情報収集済み全Master PortのDL/UL位相補正
* @return   None
* @date     2015/11/05 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjust_CPM_DLUL(USHORT cpr_num)
{
    UINT regAdr, regVal;
    INT  MIX_PMON;
    INT  adjust;
    UINT mask = (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);

    /*
     * UL/DL位相差n取得
     * 位相差表示レジスタ(Bit5:0 -32～31)
     */
    regAdr = M_RRH_REG_MIX_PMON(cpr_num);
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
    MIX_PMON = regVal & mask;
    if (0 != (MIX_PMON & BIT5))
    {
        MIX_PMON &= ~BIT5;
        MIX_PMON -= 32;
    }

    /* DL/UL位相差補正値n = 31 - UL/DL位相差n */
    adjust = 31 - MIX_PMON;

    /* DL/UL位相差補正値nを設定 */
    regAdr = M_RRH_REG_MIX_LUL(cpr_num);
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
    regVal &= ~mask;
    regVal |= (adjust & mask);
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);
}
/*********************************************************************************/
/**
* @brief    情報収集済み全Master PortのLTE UL遅延補正
* @return   Calc70nの最大値チェック結果
* @retval   D_RRH_OK : 正常終了
* @retval   D_RRH_NG : Calc70n最大値超過
* @date     2015/11/05 TDI)satou その他未実装-008 create
* @date     2016/01/05 TDI)satou レジスタ最大値チェック追加
*/
/*********************************************************************************/
INT f_cpr_DelayAdjust_CPM_S3G_UL(USHORT cpr_num)
{
    UINT regAdr, regVal;
    UINT adjust;
    UINT Calc70n, Calc71n, Calc72n;
    USHORT cpr_idx = cpr_num - 1;
    INT FHM_LTE_Yn_MAX, LTE_Yn, T12_REn;

    regAdr = M_RRH_REG_MIX_LUL(cpr_num);
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
    adjust = regVal & (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);

    /*
     * Calc70n = LU遅延オフセット(固定遅延245) + FHMの最大LTE上り遅延
     *           - {LTE_Yn + T12_REn} - DL位相差補正値n
     * Calc71n = int(Calc70n/64)
     * Calc72n = mod(Calc70n,64)
     */
    FHM_LTE_Yn_MAX = f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G];
    LTE_Yn         = f_cprw_dly_MAX_Yn[CMD_SYS_S3G].REn[cpr_idx];
    T12_REn        = f_cprw_dly_MAX_T12_RE.REn[cpr_idx];
    Calc70n        = D_CPR_LUDELAY_OFFSET + FHM_LTE_Yn_MAX - (LTE_Yn + T12_REn) - adjust;

    /*
     * Calc70nの計算結果が、レジスタ設定上限値を超えた場合は、
     * 該当配下REを強制停止としRE強制停止(遅延調整NG)ERRとする。
     */
    if (D_CPR_DRYADJ_UPLIMIT_MAX < Calc70n)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "over threshold. cpr_num=%d Calc70n=%08x", cpr_num, Calc70n);
        return D_RRH_NG;
    }

    Calc71n        = (INT)(Calc70n/64);
    Calc72n        = Calc70n % 64;

    regVal &= ~(BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|BIT9|BIT8|BIT7|BIT6);
    regVal |= (Calc71n << 6);

    regVal &= ~(BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|BIT16);
    regVal |= (Calc72n << 16);

    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);

    return D_RRH_OK;
}
/*********************************************************************************/
/**
* @brief    情報収集済み全Master Portの3G DL遅延補正
* @return   Calc80n最大値チェック結果
* @retval   D_RRH_OK : 正常終了
* @retval   D_RRH_NG : Calc80n最大値超過
* @date     2015/11/05 TDI)satou その他未実装-008 create
*/
/*********************************************************************************/
INT f_cpr_DelayAdjust_CPM_3G_DL(USHORT cpr_num)
{
    INT Calc80n, Calc81n, Calc82n;
    USHORT cpr_idx = cpr_num - 1;
    INT FHM_Wcdma_Xn_MAX, Wcdma_Xn, T12_REn, Calc42n;
    UINT regAdr, regVal;

    /*
     * Calc80n = WD遅延オフセット(固定遅延348) + FHMの最大Wcdma下り遅延
     *           - {Wcdma_Xn + T12_REn} - Calc42n
     * Calc81n = int(Calc80n/64)
     * Calc82n = mod(Calc80n,64)
     */
    FHM_Wcdma_Xn_MAX = f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G];
    Wcdma_Xn         = f_cprw_dly_MAX_Xn[CMD_SYS_3G].REn[cpr_idx];
    T12_REn          = f_cprw_dly_MAX_T12_RE.REn[cpr_idx];
    Calc42n          = f_cprw_dly_inf.Calc42n[cpr_idx];
    Calc80n          = D_CPR_WDDELAY_OFFSET + FHM_Wcdma_Xn_MAX - (Wcdma_Xn + T12_REn) - Calc42n;

    /*
     * Calc80nの計算結果が、レジスタ設定上限値を超えた場合は、
     * 該当配下REを強制停止としRE強制停止(遅延調整NG)ERRとする。
     */
    if (D_CPR_DRYADJ_UPLIMIT_MAX < Calc80n)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "over threshold. cpr_num=%d Calc80n=%08x", cpr_num, Calc80n);
        return D_RRH_NG;
    }

    Calc81n          = (INT)(Calc80n/64);
    Calc82n          = (Calc80n%64);

    regAdr = M_RRH_REG_DIS_WDL(cpr_num);
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
    regVal &= ~(BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|BIT9|BIT8|BIT7|BIT6);
    regVal |= (Calc81n << 6);
    regVal &= ~(BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|BIT16);
    regVal |= (Calc82n << 16);
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);

    return D_RRH_OK;
}
/*********************************************************************************/
/**
* @brief    情報収集済み全Master Portの3G UL遅延補正
* @return   Calc90n最大値チェック結果
* @retval   D_RRH_OK : 正常終了
* @retval   D_RRH_NG : Calc90n最大値超過
* @Bug_No   M-RRU-ZSYS-02000
* @date     2015/11/05 TDI)satou その他未実装-008 create
* @date     2015/11/19 TDI)satou M-RRU-ZSYS-02000 問処番号No.189 設定先アドレス誤りを修正
*/
/*********************************************************************************/
INT f_cpr_DelayAdjust_CPM_3G_UL(USHORT cpr_num)
{
    USHORT cpr_idx = cpr_num - 1;
    UINT regAdr, regVal;
    INT FHM_Wcdma_Yn_MAX, Wcdma_Yn, T12_REn;
    UINT adjust;
    INT Calc90n, Calc91n, Calc92n;
    /*
     * Calc90n = WU遅延オフセット(固定値円245) + FHMの最大Wcdma上り遅延
     *           - {Wcdma_Yn + T12_REn} - DL/UL位相差補正値n
     * Calc91n = int(Calc90n/64)
     * Calc92n = mod(Calc90n,64)
     */
    regAdr = M_RRH_REG_MIX_LUL(cpr_num);
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
    adjust = regVal & (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);

    FHM_Wcdma_Yn_MAX = f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G];
    Wcdma_Yn         = f_cprw_dly_MAX_Yn[CMD_SYS_3G].REn[cpr_idx];
    T12_REn          = f_cprw_dly_MAX_T12_RE.REn[cpr_idx];
    Calc90n          = D_CPR_WUDELAY_OFFSET + FHM_Wcdma_Yn_MAX - (Wcdma_Yn + T12_REn) - adjust;

    /*
     * Calc90nの計算結果が、レジスタ設定上限値を超えた場合は、
     * 該当配下REを強制停止としRE強制停止(遅延調整NG)ERRとする。
     */
    if (D_CPR_DRYADJ_UPLIMIT_MAX < Calc90n)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "over threshold. cpr_num=%d Calc90n=%08x", cpr_num, Calc90n);
        return D_RRH_NG;
    }

    Calc91n          = (INT)(Calc90n/64);
    Calc92n          = (Calc90n%64);

    regAdr = M_RRH_REG_MIX_WUL(cpr_num);
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
    regVal &= ~(BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|BIT9|BIT8|BIT7|BIT6);
    regVal |= (Calc91n << 6);
    regVal &= ~(BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|BIT16);
    regVal |= (Calc92n << 16);
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);

    return D_RRH_OK;
}
/*********************************************************************************/
/**
* @brief    遅延補正(Step7) 失敗REチェック
* @param    apiRsp [in] The response of D_API_MSGID_CPRI_DLY_ADJ_STEP7_RSP
* @return   None
* @date     2015/11/09 TDI)satou その他未実装-017 create
* @date     2016/01/06 TDI)satou レジスタ最大値超過のチェックを追加
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustStep7Proc_CheckFail(T_API_CPRILINK_ADJUST_DELAY_STEP7_RSP *apiRsp)
{
    USHORT cpr_num, cpr_idx;
    UINT regAdr, regVal;

    for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
    {
        cpr_idx = cpr_num - 1;

        if (E_API_RCD_NG == apiRsp->result_re[cpr_idx])
        {
			/* LTE DL遅延補正レジスタ最大値超過の場合、光OFFせずにl3/rctで強制停止に設定させる */
            if (D_RRH_OK != f_cpr_DelayAdjustProc_CheckSetVal_Adjust_LTE_DL(cpr_num))
            {
                apiRsp->result_re[cpr_idx] = E_API_RCD_OVER_MAX_DELAY;
                continue;
            }

			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "[STEP7]LTE DL delay adj1. cpr_num=%d",cpr_num);

            /* CPM#n 光OFF & CPRI Disable設定 */
            regAdr = M_RRH_REG_CPRI_SFPCNT(cpr_num);
            (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
            regVal &= ~(BIT0|BIT1|BIT2);
            regVal |= BIT0;
            (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);

            /* 個別Master portのLTE DL遅延補正、DL/UL位相差初期化 */
            f_cpr_DelayAdjustProc_Adjust_LTE_DL(cpr_num);
            f_cpr_DelayAdjustProc_Init_MIX_LUL(cpr_num);
        }
    }

    /* ハソでは「Wait 200ms (確実にEDWN発生させる為のWait)」とあるが以下理由より、Waitしない
     *
     * ・CPM#n 光OFF & CPRI Disable設定をしてから、
     *   CPM#n 光ON & CPRI Enable設定にもどすタイミングは、
     *   ハードからEDWNの割り込みを通知されたタイミングになっている
     *   (つまり、EDWN発生が保証できている)
     *
     * ・RECからToffset報告要求を受信したときにWait200msすると、
     *   RECへの応答性能を満たせない(メーカ提出資料で50msになっている) */
}
/*********************************************************************************/
/**
* @brief    遅延補正(Step7)
* @param    buff_adr [in] the buffer address pointer of received message
* @return   None
* @date     2015/11/05 TDI)satou その他未実装-008 create
* @date     2015/11/12 TDI)satou 問処番号No.174
* @date     2016/01/05 TDI)satou レジスタ最大値超過のチェックを追加
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustStep7Proc(UINT *buff_adr)
{
    T_API_CPRILINK_ADJUST_DELAY_STEP7 *apiInd;
    T_API_CPRILINK_ADJUST_DELAY_STEP7_RSP apiRsp;
    USHORT cpr_num, cpr_idx;
    INT isMatch;

#ifdef OPT_RRH_SIM
    printf("%s start\n", __func__);
#endif
    apiInd = (T_API_CPRILINK_ADJUST_DELAY_STEP7*)buff_adr;

    memset(&apiRsp, 0, sizeof apiRsp);
    apiRsp.header.uiEventNo = D_API_MSGID_CPRI_DLY_ADJ_STEP7_RSP;

    isMatch = D_RRH_OK;
    for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
    {
        cpr_idx = cpr_num - 1;
        if (D_RRH_ON == apiInd->tgt_re[cpr_idx])
        {   /*
             * Step3においてLTE DL遅延補正を行わなったREは遅延補正を行わず、
             * l3/rctで強制停止に設定させる。具体的にこのREは、Step3でLTE DL遅延補正の
             * レジスタ最大値を超過していたREである。
             */
            if (D_RRH_ON != f_cprw_dly_alreadyAdjLteDL[cpr_idx])
            {
                apiRsp.result_re[cpr_idx] = E_API_RCD_OVER_MAX_DELAY;
                apiInd->tgt_re[cpr_idx] = D_RRH_OFF;
                continue;
            }

            f_cpr_DelayAdjustProc_CollectParam_Step7(cpr_num);
            /* パラメータ差分がある場合はCPRIリンクを切ってやりなおし */
            if (D_RRH_OK != f_cpr_DelayAdjust_IsMatchParam(cpr_num))
            {
                apiRsp.result_re[cpr_idx] = E_API_RCD_NG;
                isMatch = D_RRH_NG;
            }
        }
    }

    /* パラメータ不一致のREがいても、360秒T.O後の場合は正常なREだけで遅延補正を実施 */
    if ((D_RRH_OK == isMatch) || (D_RRH_OFF == apiInd->param_check_en))
    {
        for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
        {
            cpr_idx = cpr_num - 1;

            if ((D_RRH_ON == apiInd->tgt_re[cpr_idx]) && (E_API_RCD_OK == apiRsp.result_re[cpr_idx]))
            {
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "[STEP7]LTE DL delay adj2. cpr_num=%d",cpr_num);
				
				/* 情報収集済みMaster PortのDL/UL位相補正 */
                f_cpr_DelayAdjust_CPM_DLUL(cpr_num);

                apiRsp.result_re[cpr_idx] = E_API_RCD_OK;

                /* 情報収集済みMaster PortのLTE UL遅延補正 */
                /* 情報収集済みMaster Portの3G DL遅延補正  */
                /* 情報収集済みMaster Portの3G UL遅延補正  */
                if ((D_RRH_OK != f_cpr_DelayAdjust_CPM_S3G_UL(cpr_num))
                ||  (D_RRH_OK != f_cpr_DelayAdjust_CPM_3G_DL (cpr_num))
                ||  (D_RRH_OK != f_cpr_DelayAdjust_CPM_3G_UL (cpr_num)))
                {   /* レジスタ最大値超過している場合は呼び出し元で強制停止に設定させる */
                    apiRsp.result_re[cpr_idx] = E_API_RCD_OVER_MAX_DELAY;
                }
                else
                {
					/*	フェールセーフERR発生中の場合	*/
					if(f_cprw_dly_failsafe_3G[cpr_idx] == D_SYS_ON)
					{
						/*	フェールセーフERR復旧	*/
						f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_FAILSAFE_ERR(cpr_num), D_SYS_OFF);
						f_cprw_dly_failsafe_3G[cpr_idx] = D_SYS_OFF;
					}
					/*	フェールセーフERR発生中の場合	*/
					if(f_cprw_dly_failsafe_S3G[cpr_idx] == D_SYS_ON)
					{
						/*	フェールセーフERR復旧	*/
						f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_FAILSAFE_ERR(cpr_num), D_SYS_OFF);
						f_cprw_dly_failsafe_S3G[cpr_idx] = D_SYS_OFF;
					}
				}
            }
        }
    }

    if (D_RRH_OK != isMatch)
    {
        /* 遅延補正に失敗したREが存在する場合はCPRI切断してやりなおし */
        f_cpr_DelayAdjustStep7Proc_CheckFail(&apiRsp);
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "step7 NG.");
    }

    /* 共有メモリに保存(360秒T.O、配下RE起動完了区別なく共有メモリに保存) */
    f_cmn_com_delay_inf_set(&f_cprw_dly_inf);

    /* 完了を報告 */
    f_com_sendMsgToAplFHM(apiInd->header.uiSrcPQueueID, &apiRsp, sizeof apiRsp, 0, D_SYS_THDQID_PF_CPRI);

    /* パラメータ退避 */
    f_cpr_backup_param();
}
/*********************************************************************************/
/**
* @brief    遅延補正(Step10) 最大遅延量以内かチェック
* @param    cpr_num [in] CPRI番号
* @return   None
* @date     2015/11/10 TDI)satou その他未実装-017 create
* @date     2015/11/24 TDI)enoki 最大値チェックは+8で実施
* @date     2019/01/08 FJT)koshida 最大値チェックは+8->+32に変更。+4～+32の場合は再補正実施。
*/
/*********************************************************************************/
INT f_cpr_DelayAdjustStep10Proc_IsWithinMax(USHORT cpr_num)
{
    INT ret = D_RRH_OK;
    INT max, REn, max32;
    USHORT cpr_idx = cpr_num - 1;

    /*	3G DL最大遅延超過チェック	*/
    max = f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G] + 4;
    max32 = f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G] + D_CPR_DELAY_MERGE_MAX;
    REn = f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_3G].REn[cpr_idx];
    /*	最大遅延量超過分が32CLKを超える場合	*/
    if (max32 < REn)
    {
        /*	最大遅延超過のためNG復帰する	*/
        f_cprw_dly_adjWDDly[cpr_idx] = (UINT)(REn - f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G]);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Over MaxFhmDlDelay(3G)+32. max=%d RE#%d=%d", f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G], cpr_num, REn);
        ret = D_RRH_NG;
    }
    /*	最大遅延量超過分が32CLK以下場合	*/
    else if (max < REn)
    {
        /*	最大超過補正値を決定する	*/
        f_cprw_dly_adjWDDly[cpr_idx] = (UINT)(REn - f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G]);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Over MaxFhmDlDelay(3G)+4. max=%d RE#%d=%d-%d", f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G], cpr_num, REn, f_cprw_dly_adjWDDly[cpr_idx]);
    }
    /*	最大遅延量超過なしの場合	*/
    else
    {
        /*	最大超過補正値を初期化する	*/
        f_cprw_dly_adjWDDly[cpr_idx] = 0;
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "DLDly init(3G). max=%d RE#%d=%d-%d", f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_3G], cpr_num, REn, f_cprw_dly_adjWDDly[cpr_idx]);
	}

    /*	S3G DL最大遅延超過チェック	*/
    max = f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G] + 4;
    max32 = f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G] + D_CPR_DELAY_MERGE_MAX;
    REn = f_cprw_dly_MAX_Xn_plus_T12_RE[CMD_SYS_S3G].REn[cpr_idx];
    /*	最大遅延量超過分が32CLKを超える場合	*/
    if (max32 < REn)
    {
        /*	最大遅延超過のためNG復帰する	*/
        f_cprw_dly_adjLDDly[cpr_idx] = (UINT)(REn - f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G]);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Over MaxFhmDlDelay(S3G)+32. max=%d RE#%d=%d", f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G], cpr_num, REn);
        ret = D_RRH_NG;
    }
    /*	最大遅延量超過分が32CLK以下場合	*/
    else if (max < REn)
    {
        /*	最大超過補正値を決定する	*/
        f_cprw_dly_adjLDDly[cpr_idx] = (UINT)(REn - f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G]);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Over MaxFhmDlDelay(S3G)+4. max=%d RE#%d=%d-%d", f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G], cpr_num, REn, f_cprw_dly_adjLDDly[cpr_idx]);
    }
    /*	最大遅延量超過なしの場合	*/
    else
    {
        /*	最大超過補正値を初期化する	*/
        f_cprw_dly_adjLDDly[cpr_idx] = 0;
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "DLDly init(S3G). max=%d RE#%d=%d-%d", max, cpr_num, f_cprw_dly_inf.MaxFhmDlDelay[CMD_SYS_S3G], f_cprw_dly_adjLDDly[cpr_idx]);
	}

    /*	3G UL最大遅延超過チェック	*/
    max = f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G] + 4;
    max32 = f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G] + D_CPR_DELAY_MERGE_MAX;
    REn = f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_3G].REn[cpr_idx];
    /*	最大遅延量超過分が32CLKを超える場合	*/
    if (max32 < REn)
    {
        /*	最大遅延超過のためNG復帰する	*/
        f_cprw_dly_adjWUDly[cpr_idx] = (UINT)(REn - f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G]);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Over MaxFhmUlDelay(3G)+32. max=%d RE#%d=%d", f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G], cpr_num, REn);
        ret = D_RRH_NG;
    }
    /*	最大遅延量超過分が32CLK以下場合	*/
    else if (max < REn)
    {
        /*	最大超過補正値を決定する	*/
        f_cprw_dly_adjWUDly[cpr_idx] = (UINT)(REn - f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G]);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Over MaxFhmUlDelay(3G)+4. max=%d RE#%d=%d-%d", f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G], cpr_num, REn, f_cprw_dly_adjWUDly[cpr_idx]);
    }
    /*	最大遅延量超過なしの場合	*/
    else
    {
        /*	最大超過補正値を初期化する	*/
        f_cprw_dly_adjWUDly[cpr_idx] = 0;
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "ULDly init(3G). max=%d RE#%d=%d-%d", f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_3G], cpr_num, REn, f_cprw_dly_adjWUDly[cpr_idx]);
	}

    /*	S3G UL最大遅延超過チェック	*/
    max = f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G] + 4;
    max32 = f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G] + D_CPR_DELAY_MERGE_MAX;
    REn = f_cprw_dly_MAX_Yn_plus_T12_RE[CMD_SYS_S3G].REn[cpr_idx];
    if (max32 < REn)
    {
        /*	最大遅延超過のためNG復帰する	*/
        f_cprw_dly_adjLUDly[cpr_idx] = (UINT)(REn - f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G]);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Over MaxFhmUlDelay(S3G)+32. max=%d RE#%d=%d", f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G], cpr_num, REn);
        ret = D_RRH_NG;
    }
    else if (max < REn)
    {
        /*	最大超過補正値を決定する	*/
        f_cprw_dly_adjLUDly[cpr_idx] = (UINT)(REn - f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G]);
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Over MaxFhmUlDelay(S3G)+4. max=%d RE#%d=%d-%d", f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G], cpr_num, REn, f_cprw_dly_adjLUDly[cpr_idx]);
    }
    /*	最大遅延量超過なしの場合	*/
    else
    {
        /*	最大超過補正値を初期化する	*/
        f_cprw_dly_adjLUDly[cpr_idx] = 0;
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "ULDly init(S3G). max=%d RE#%d=%d-%d", f_cprw_dly_inf.MaxFhmUlDelay[CMD_SYS_S3G], cpr_num, REn, f_cprw_dly_adjLUDly[cpr_idx]);
	}

    return ret;
}
/*********************************************************************************/
/**
* @brief    遅延補正(Step10) 強制停止
* @param    cpr_num			[in]	CPRIリンク番号
* @param	forceStopEn		[in]	強制停止への遷移有効(D_RRH_ON)/無効(D_RRH_OFF)
* @param	uiSrcPQueueID	[in]	メッセージ受信元プロセスのキューID
* @return   None
* @date     2016/04/20 TDI)satou create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustStep10_ForceStop(USHORT cpr_num, USHORT forceStopEn, UINT uiSrcPQueueID)
{
	T_API_CPRILINK_ADJUST_DELAY_STEP10_RSP apiRsp;
	USHORT cpr_idx = cpr_num - 1;
    UINT   regAdr,regVal;

	memset(&apiRsp, 0, sizeof apiRsp);
    apiRsp.header.uiEventNo = D_API_MSGID_CPRI_DLY_ADJ_STEP10_RSP;
    apiRsp.cpr_num          = cpr_num;

    /* 共有メモリ更新(デバッグ用途. 詳細ログ取得時に問題解析できるように) */
    f_cmn_com_delay_inf_set(&f_cprw_dly_inf);

	if (D_RRH_ON == forceStopEn)
	{	/*
		 * forceStopEnがD_RRH_ON(LTE-RECからのToffset値報告要求受信 若しくは CPRIリンク断復旧時)
		 * の場合、強制停止とする. ただし、保護段数以内におさまっている場合、強制停止せずに正常を実行元に返す.
		 */
		f_cprw_Step10_TryCount_AdjLteDL[cpr_idx]++;
		if (f_cprw_Step10_TryCount_AdjLteDL[cpr_idx] >= D_CPR_STEP10_ADJ_LTEDL_TRY_MAX)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "force stop. cpr_num=%d count=%d", cpr_num, f_cprw_Step10_TryCount_AdjLteDL[cpr_idx]);
			
			/* 強制停止へ遷移した場合、次回接続時にはLTE DL遅延補正から実施させる */
			/*	そして、フェールセーフERRも復旧させる	*/
			f_cprw_dly_alreadyAdjLteDL[cpr_idx]      = D_RRH_OFF;
			f_cprw_Step10_TryCount_AdjLteDL[cpr_idx] = 0;
			
			/*	フェールセーフERR発生中の場合	*/
			if(f_cprw_dly_failsafe_3G[cpr_idx] == D_SYS_ON )
			{
				/*	フェールセーフERR復旧	*/
				f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_FAILSAFE_ERR(cpr_num), D_SYS_OFF);
				f_cprw_dly_failsafe_3G[cpr_idx] = D_SYS_OFF;
			}
			/*	フェールセーフERR発生中の場合	*/
			if(f_cprw_dly_failsafe_S3G[cpr_idx] == D_SYS_ON )
			{
				/*	フェールセーフERR復旧	*/
				f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_FAILSAFE_ERR(cpr_num), D_SYS_OFF);
				f_cprw_dly_failsafe_S3G[cpr_idx] = D_SYS_OFF;
			}
			apiRsp.result = E_API_RCD_OVER_MAX_DELAY;
			f_com_sendMsgToAplFHM(uiSrcPQueueID, &apiRsp, sizeof apiRsp, 0, D_SYS_THDQID_PF_CPRI);
			return;
		}
		else
		{	/* 保護段数内におさまっている場合、強制停止せずに正常を実行元に返す */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "within protection. cpr_num=%d count=%d", cpr_num, f_cprw_Step10_TryCount_AdjLteDL[cpr_idx]);
			
			/*	最初のフェールセーフ実施の場合	*/
			if(f_cprw_Step10_TryCount_AdjLteDL[cpr_idx] == 1)
			{
				/*	フェールセーフERR発生	*/
				if((f_cprw_dly_adjWUDly[cpr_idx] > D_CPR_DELAY_MERGE_MAX) || (f_cprw_dly_adjWDDly[cpr_idx] > D_CPR_DELAY_MERGE_MAX))
				{
					f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_FAILSAFE_ERR(cpr_num), D_SYS_ON);
					f_cprw_dly_failsafe_3G[cpr_idx] = D_SYS_ON;
				}
				/*	フェールセーフERR発生	*/
				if((f_cprw_dly_adjLUDly[cpr_idx] > D_CPR_DELAY_MERGE_MAX) || (f_cprw_dly_adjLDDly[cpr_idx] > D_CPR_DELAY_MERGE_MAX))
				{
					f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_FAILSAFE_ERR(cpr_num), D_SYS_ON);
					f_cprw_dly_failsafe_S3G[cpr_idx] = D_SYS_ON;
				}
			}
			
			/*	CPM#n CPRI Disable設定	*/
			regAdr = M_RRH_REG_CPRI_SFPCNT(cpr_num);
			(VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
			regVal &= ~BIT2;
			(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);
			
			/* Wait 1ms */
			f_com_taskDelay(1, D_SYS_THDID_PF_CPRI, 0, 0);
			
			/*	CPM#n CPRI Disable設定	*/
			regVal |= BIT2;
			(VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);
		}
	}
	/*
	 * 保護段数内におさまっている場合 若しくは 3G-RECからのToffset値報告要求受信
	 * (forceStopEn : D_RRH_OFF)の場合、強制停止せずに正常を実行元に返す.
	 */
	apiRsp.result = E_API_RCD_OK;
	f_com_sendMsgToAplFHM(uiSrcPQueueID, &apiRsp, sizeof apiRsp, 0, D_SYS_THDQID_PF_CPRI);
}
/*********************************************************************************/
/**
* @brief    遅延補正(Step10)
* @param    buff_adr [in] the buffer address pointer of received message
* @return   None
* @date     2015/11/09 TDI)satou その他未実装-017 create
* @date     2015/12/02 TDI)satou IT3問処番号No.109
* @date     2015/12/07 TDI)enoki IT3問処番号No.131
* @date     2016/01/05 TDI)satou 遅延補正レジスタの最大値チェック追加
* @date     2016/01/29 TDI)satou eNB-F-15B-3201-00115 応答メッセージを汎用から専用のものに変更
* @date     2016/04/17 TDI)satou 3201-15B-TS171
* @date		2016/04/20 TDI)satou 強制停止への遷移条件を変更
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjustStep10Proc(UINT *buff_adr)
{
    T_API_CPRILINK_ADJUST_DELAY_STEP10 *apiInd;
    T_API_CPRILINK_ADJUST_DELAY_STEP10_RSP apiRsp;
    USHORT cpr_idx;
    UINT regAdr, regVal;
#if 0
    INT absVal;
#endif

#ifdef OPT_RRH_SIM
    printf("%s start\n", __func__);
#endif

    /* 変数初期化 */
    f_cmn_com_conn_disk_get(&f_cprw_shm_conn);

    apiInd = (T_API_CPRILINK_ADJUST_DELAY_STEP10*)buff_adr;
    cpr_idx = apiInd->cpr_num - 1;
    memset(&apiRsp, 0, sizeof apiRsp);
    apiRsp.header.uiEventNo = D_API_MSGID_CPRI_DLY_ADJ_STEP10_RSP;
    apiRsp.cpr_num          = apiInd->cpr_num;

    /* パラメータ取得 */
    f_cpr_DelayAdjustProc_CollectParam_Step10(apiInd->cpr_num);

	/* Valid=0のときはStep10をskip */
	if (1 != f_cprw_dly_cp_cprtdly[cpr_idx].valid)
	{
	    apiRsp.result = E_API_RCD_OK;
		f_com_sendMsgToAplFHM(apiInd->header.uiSrcPQueueID, &apiRsp, sizeof apiRsp, 0, D_SYS_THDQID_PF_CPRI);
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "step10 skip. cpr_num=%d, cprtdly=0x%08x", apiInd->cpr_num, f_cprw_dly_cp_cprtdly[cpr_idx].regDat);
		return;
    }
	
    if (D_RRH_ON == f_cprw_dly_alreadyAdjLteDL[cpr_idx])
    {   /* CPM#n LTE DL遅延補正済み */
        if (D_RRH_OK == f_cpr_DelayAdjust_IsMatchParam(apiInd->cpr_num))
        {   /* 前回収集遅延パラメータと差分がない */
            apiRsp.result = E_API_RCD_OK;
			/* Step10にてLTE DL遅延補正、DL/UL位相差初期化実施時は遅延補正再実施 */
            if (f_cprw_Step10_dly_AdjLteDL_f[cpr_idx] == D_SYS_ON)
            {
			    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "[STEP10]LTE DL delay adj2. cpr_num=%d",apiInd->cpr_num);
			    
                /* 情報収集済みMaster PortのDL/UL位相補正 */
                f_cpr_DelayAdjust_CPM_DLUL(apiInd->cpr_num);

                /* 情報収集済みMaster PortのLTE UL遅延補正 */
                /* 情報収集済みMaster Portの3G DL遅延補正 */
                /* 情報収集済みMaster Portの3G UL遅延補正 */
                if ((D_RRH_OK != f_cpr_DelayAdjust_CPM_S3G_UL(apiInd->cpr_num))
                ||  (D_RRH_OK != f_cpr_DelayAdjust_CPM_3G_DL (apiInd->cpr_num))
                ||  (D_RRH_OK != f_cpr_DelayAdjust_CPM_3G_UL (apiInd->cpr_num)))
                {
                    apiRsp.result = E_API_RCD_OVER_MAX_DELAY;
                }
                else
                {
					/*	Step10 LTE DL遅延補正、DL/UL位相差初期化実施なし設定	*/
					f_cprw_Step10_dly_AdjLteDL_f[cpr_idx] = D_SYS_OFF;
					
					/*	フェールセーフERR発生中の場合	*/
					if(f_cprw_dly_failsafe_3G[cpr_idx] == D_SYS_ON )
					{
						/*	フェールセーフERR復旧	*/
						f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_FAILSAFE_ERR(apiInd->cpr_num), D_SYS_OFF);
						f_cprw_dly_failsafe_3G[cpr_idx] = D_SYS_OFF;
					}
					/*	フェールセーフERR発生中の場合	*/
					if(f_cprw_dly_failsafe_S3G[cpr_idx] == D_SYS_ON )
					{
						/*	フェールセーフERR復旧	*/
						f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_FAILSAFE_ERR(apiInd->cpr_num), D_SYS_OFF);
						f_cprw_dly_failsafe_S3G[cpr_idx] = D_SYS_OFF;
					}
				}
            }
			/* Step10にてはLTE DL遅延補正、DL/UL位相差初期化が不要な場合 */
            else
            {
				/*	フェールセーフERR発生中の場合	*/
				if(f_cprw_dly_failsafe_3G[cpr_idx] == D_SYS_ON )
				{
					/*	フェールセーフERR復旧	*/
					f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_3G_FAILSAFE_ERR(apiInd->cpr_num), D_SYS_OFF);
					f_cprw_dly_failsafe_3G[cpr_idx] = D_SYS_OFF;
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "fail Safe err(3G) CLR. cpri=%d", apiInd->cpr_num);
				}
				/*	フェールセーフERR発生中の場合	*/
				if(f_cprw_dly_failsafe_S3G[cpr_idx] == D_SYS_ON )
				{
					/*	フェールセーフERR復旧	*/
					f_com_SVCtl(D_SYS_THDID_PF_CPRI, M_SYS_SVCTL_S3G_FAILSAFE_ERR(apiInd->cpr_num), D_SYS_OFF);
					f_cprw_dly_failsafe_S3G[cpr_idx] = D_SYS_OFF;
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "fail Safe err(S3G) CLR. cpri=%d", apiInd->cpr_num);
				}
            }
            /* 最大遅延量超過の保護段数をクリア */
			f_cprw_Step10_TryCount_AdjLteDL[cpr_idx] = 0;

            f_com_sendMsgToAplFHM(apiInd->header.uiSrcPQueueID, &apiRsp, sizeof apiRsp, 0, D_SYS_THDQID_PF_CPRI);
            f_cmn_com_delay_inf_set(&f_cprw_dly_inf);
            return;
        }
        else
        {
			/*	±8以上の差分がある場合はこれ以降で、LTE DL遅延補正およびDL/UL位相差初期化を実施	*/
		}
    }

    /* CPM#n LTE DL遅延補正未実施 または 前回収集遅延パラメータとの差分あり */
    if (D_RRH_OK != f_cpr_DelayAdjustStep10Proc_IsWithinMax(apiInd->cpr_num))
    {
    	f_cpr_DelayAdjustStep10_ForceStop(apiInd->cpr_num, apiInd->forceStopEn, apiInd->header.uiSrcPQueueID);
        return;
    }

    /* Master Port(有効Port)のLTE DL遅延補正設定値チェック */
    if (D_RRH_OK != f_cpr_DelayAdjustProc_CheckSetVal_Adjust_LTE_DL(apiInd->cpr_num))
    {
    	f_cpr_DelayAdjustStep10_ForceStop(apiInd->cpr_num, apiInd->forceStopEn, apiInd->header.uiSrcPQueueID);
		return;
    }
	/*	新T14に従って遅延補正をやり直すため、次回Step10比較用にT14値を更新	*/
    f_cprw_base_REn_T14[cpr_idx] = f_cprw_dly_REn_T14[cpr_idx];

    f_cmn_com_delay_inf_set(&f_cprw_dly_inf);

    /* ここでのT14を保存しておく. 理由は、f_cprw_T14_Step10AdjLteDLの説明を参照 */
    f_cprw_T14_Step10AdjLteDL[cpr_idx] = f_cprw_dly_REn_T14[cpr_idx];

    /* 最大遅延量超過の保護段数をクリア */
    f_cprw_Step10_TryCount_AdjLteDL[cpr_idx] = 0;

    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "[STEP10]LTE DL delay adj1. cpr_num=%d",apiInd->cpr_num);

    /* CPM#n 光OFF & CPRI Disable設定 */
    regAdr = M_RRH_REG_CPRI_SFPCNT(apiInd->cpr_num);
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, regAdr, &regVal);
    regVal &= ~(BIT0|BIT1|BIT2);
    regVal |= BIT0;
    (VOID)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_CPRI, regAdr, &regVal);

    /* Master Port(有効Port)のLTE DL遅延補正 */
    f_cpr_DelayAdjustProc_Adjust_LTE_DL(apiInd->cpr_num);

    /* Master Port(有効Port)のDL/UL位相差初期化 */
    f_cpr_DelayAdjustProc_Init_MIX_LUL(apiInd->cpr_num);

	/*	Step10 LTE DL遅延補正、DL/UL位相差初期化実施あり設定	*/
	f_cprw_Step10_dly_AdjLteDL_f[cpr_idx] = D_SYS_ON;
	
    /* ハソでは「Wait 200ms (確実にEDWN発生させる為のWait)」とあるが以下理由より、Waitしない
     *
     * ・CPM#n 光OFF & CPRI Disable設定をしてから、
     *   CPM#n 光ON & CPRI Enable設定にもどすタイミングは、
     *   ハードからEDWNの割り込みを通知されたタイミングになっている
     *   (つまり、EDWN発生が保証できている)
     *
     * ・RECからToffset報告要求を受信したときにWait200msすると、
     *   RECへの応答性能を満たせない(メーカ提出資料で50msになっている) */

    /* パラメータ退避 */
    f_cpr_backup_param();

    apiRsp.result = E_API_RCD_OK;
    f_com_sendMsgToAplFHM(apiInd->header.uiSrcPQueueID, &apiRsp, sizeof apiRsp, 0, D_SYS_THDQID_PF_CPRI);
}
/*********************************************************************************/
/**
* @brief    遅延補正 CPM#n 減設
* @param    cpr_num [in] CPRIリンク番号
* @return   None
* @date     2015/11/10 TDI)satou その他未実装-017 create
* @date     2015/11/11 TDI)satou その他未実装-006 最大遅延量減設の検出処理を追加
* @date		2016/04/19 TDI)satou Step10でのLTE DL遅延補正試行回数の初期化を追加
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjust_CPMnDisable(USHORT cpr_num)
{
    USHORT cpr_idx = cpr_num - 1;
    T_RRH_CPRI_STATE shmCpriSta;

    f_cprw_dly_alreadyAdjLteDL[cpr_idx] = D_RRH_OFF;
    f_cprw_Step10_TryCount_AdjLteDL[cpr_idx] = 0;

    f_cmn_com_cpri_state_get(&shmCpriSta);

    if (CMD_CONN_S3G_SINGLE == shmCpriSta.cpri_linktype)
    {
        if ((cpr_num == f_cprw_dly_inf.MaxDlDelayReNo[CMD_SYS_S3G])
        ||  (cpr_num == f_cprw_dly_inf.MaxUlDelayReNo[CMD_SYS_S3G]))
        {   /* 最大遅延量減設(S3G) */
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "disable maximum delay mount RE(S3G). cpri=%d", cpr_num);
            f_com_SVCtl(D_SYS_THDID_PF_CPRI, E_SYS_SVCTL_DELAY_ERR_S3G, D_SYS_ON);
        }
    }
    else
    {
        if ((cpr_num == f_cprw_dly_inf.MaxDlDelayReNo[CMD_SYS_3G])
        ||  (cpr_num == f_cprw_dly_inf.MaxUlDelayReNo[CMD_SYS_3G]))
        {   /* 最大遅延量減設(3G) */
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "disable maximum delay mount RE(3G). cpri=%d", cpr_num);
            f_com_SVCtl(D_SYS_THDID_PF_CPRI, E_SYS_SVCTL_DELAY_ERR, D_SYS_ON);
        }
        if ((cpr_num == f_cprw_dly_inf.MaxDlDelayReNo[CMD_SYS_S3G])
        ||  (cpr_num == f_cprw_dly_inf.MaxUlDelayReNo[CMD_SYS_S3G]))
        {   /* 最大遅延量減設(S3G) */
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "disable maximum delay mount RE(S3G). cpri=%d", cpr_num);
            f_com_SVCtl(D_SYS_THDID_PF_CPRI, E_SYS_SVCTL_DELAY_ERR_S3G, D_SYS_ON);
        }
    }

    f_cmn_com_delay_inf_set(&f_cprw_dly_inf);
}
/*********************************************************************************/
/**
* @brief    遅延補正状態クリア(他プロセスからの要求)
* @note     配下RE0台で起動時、状態初期化の用途で使用
* @return   None
* @date     2016/03/29 TDI)satou create
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjust_ClearStateProc(UINT *buff_adr)
{
	f_cpr_DelayAdjust_ClearState();
}
/*********************************************************************************/
/**
* @brief    遅延補正状態クリア
* @note     RECにEDWN発生し、そこからリンク復旧した時に遅延補正しなおす
* @return   None
* @date     2015/11/13 TDI)satou その他未実装-013 create
* @date		2016/04/19 TDI)satou Step10でのLTE DL遅延補正試行回数の初期化を追加
*/
/*********************************************************************************/
VOID f_cpr_DelayAdjust_ClearState(VOID)
{
    memset(&f_cprw_dly_inf, 0, sizeof f_cprw_dly_inf);
    memset(&f_cprw_Step10_TryCount_AdjLteDL[0], 0, sizeof f_cprw_Step10_TryCount_AdjLteDL);

    f_cmn_com_delay_inf_set(&f_cprw_dly_inf);
}
/* @} */

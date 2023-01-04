 /***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_cm_WriteLEDReg.c
 *  @brief  common function - LED Control Register Write function
 *  @date   2008/08/27   FFCS)zhengmh create
 *  @date   2010/01/07 QNET)Kabasima M-S3G-eNBSYS-00991 ST RRE ALM時のLED表示について
 *  @date   2010/01/09 QNET)Kabasima M-S3G-eNBPF-03109:RREで両系ブランチALM時にBSY-LED消灯しない
 *  @date   2010/05/20 FJT)Otani  CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *  @date   2010/06/23 FJT)Otani  M-S3G-eNBPF-03587 [800M-RRE SST]TRX設定後のリセットでUSE点灯すると同時にBSYが消灯
 *  @date   2010/08/03 CMS)Yorozuya  CR-00014-000 : ハソV2.19-02版
 *  @date   2010/08/19 CMS)Yorozuya  S3G-eNBPF-03701
 *  @date   2012/07/19 FFCS)niemsh modify for 1.5GVA
 *  @date   2013/11/25 FFCS)Xut modify for ZYNQ.
 *  @date   2015/04/21 FFCS)Yuzhh modify for TDD-SRE
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2012
 */
/***************************************************************************/

/** @addtogroup RRH_DCM
 *  @{
 */

/****************************************************************************/
/* ヘッダインクルード                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRA共通ヘッダファイル			*/
#include "f_rrh_def.h"
#include "rrhApi_Svp.h"
#include "l3_com_inc.h"
#include "m_dl_header.h"
/****************************************************************************/
/* 外部参照                                                                 */
/****************************************************************************/
extern UINT rec_Startup_First_flg[CMD_MAX_SYSNUM];
/****************************************************************************/
/* 定義                                                                     */
/****************************************************************************/
//bit 1   - Port En:Dis, En
#define PortEn_Dis    0
#define PortEn_En     1
#define PortEn_shif   0
#define PortEn_mask   1
//bit 2,3 - 光Mod: D.C, 無, 有(Fサポート品), 有(未サポート品)
#define LightMod_DC   0
#define LightMod_N    1
#define LightMod_EF   2
#define LightMod_EMi  3
#define LightMod_shif 1
#define LightMod_mask (3)
//bit 4,5 - CPRI State: F未満, F
#define CpriState_FMi 1
#define CpriState_F   2
#define CpriState_shif  3
#define CpriState_mask  (3)
//bit 6   - 強制停止: OFF, ON
#define ForceStop_OFF 0
#define ForceStop_ON  1
#define ForceStop_shif     5
#define ForceStop_mask     (1)
//bit 7   - L3(H.C): NG, OK
#define L3HC_NG       0
#define L3HC_OK       1
#define L3HC_OK_CMP   3
#define L3HC_shif     6
#define L3HC_mask     (1)
#define L3HC_CMP_mask (3)
#define RecStartup_Mi   0
#define RecStartup_Kan  1
//bit 8   - "遅延補正完&運用組込": 未, 完
#define Layer3_Mi      0
#define Layer3_Kan     1
#define Layer3_shif    8
#define Layer3_mask    (1)
//bit 9   - BDE-INF ACT
#define ACT_OFF	    0
#define ACT_ON	    1
#define ACT_shif    0
#define ACT_mask    (1)
//橙LED	緑LED
#define LED_OFF	0
#define LED_XX	1
#define	LED_ON	2

#define flags_combie(PortEn, LightMod, CpriState, ForceStop, L3HC, Layer3_v) \
	( (PortEn)\
	| (LightMod<<LightMod_shif)\
	| (CpriState<<CpriState_shif)\
	| (ForceStop<<ForceStop_shif)\
	| (L3HC<<L3HC_shif)\
	| (Layer3_v<<Layer3_shif) )

#define masks_combie flags_combie

#define HealthChk2Flag(value) ((value==CMD_ON)?L3HC_OK:L3HC_NG)
#define ConnDisc2Flag(value) ((value==D_RRH_ON)?PortEn_En:PortEn_Dis)
#define count_of(at) (sizeof(at)/sizeof(at[0]))
#define RecStartup2Flag(value) (((value==D_RRH_ON)?RecStartup_Kan:RecStartup_Mi)<<1)

#define LightMode_get(awCpriNo) (swLightModeData[awCpriNo])

//-------------static structures
static struct {
	USHORT value;
	USHORT mask;
	USHORT orgLed;
	USHORT grnLed;
} REInfoTbl[] = {
	//[0] Dis	-	-	-	-	-	消灯	消灯
	{flags_combie(PortEn_Dis, 0, 0, 0, 0, 0),
	masks_combie(PortEn_mask, 0, 0, 0, 0, 0),
	LED_OFF, LED_OFF},
	//[1] En	無	D.C	-	-	-	点灯	消灯
	{flags_combie(PortEn_En, LightMod_N, 0, 0, 0, 0),
	masks_combie(PortEn_mask, LightMod_mask, 0, 0, 0, 0),
	LED_ON, LED_OFF},
	//[2] En	有(Fサポート品)	F未満	-	-	-	点灯	消灯
	{flags_combie(PortEn_En, LightMod_EF, CpriState_FMi, 0, 0, 0),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, 0, 0, 0),
	LED_ON, LED_OFF},
	//[3] En	有(Fサポート品)	F	NG	OFF	未	点灯	消灯
	{flags_combie(PortEn_En, LightMod_EF, CpriState_F, ForceStop_OFF, L3HC_NG, Layer3_Mi),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, ForceStop_mask, L3HC_mask, Layer3_mask),
	LED_ON, LED_OFF},
	//[4] En	有(Fサポート品)	F	-	ON	-	点灯	消灯
	{flags_combie(PortEn_En, LightMod_EF, CpriState_F, ForceStop_ON, 0, 0),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, ForceStop_mask, 0, 0),
	LED_ON, LED_OFF},
	//[5] En	有(Fサポート品)	F	OK	OFF	未	消灯	点滅
	{flags_combie(PortEn_En, LightMod_EF, CpriState_F, ForceStop_OFF, L3HC_OK, Layer3_Mi),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, ForceStop_mask, L3HC_mask, Layer3_mask),
	LED_OFF, LED_XX},
	//[6] En	有(Fサポート品)	F	OK	OFF	完	消灯	点灯
	{flags_combie(PortEn_En, LightMod_EF, CpriState_F, ForceStop_OFF, L3HC_OK, Layer3_Kan),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, ForceStop_mask, L3HC_mask, Layer3_mask),
	LED_OFF, LED_ON},
	//[7] En	有(未サポート品)	F未満	-	-	-	点滅	消灯
	{flags_combie(PortEn_En, LightMod_EMi, CpriState_FMi, 0, 0, 0),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, 0, 0, 0),
	LED_XX, LED_OFF},
	//[8] En	有(未サポート品)	F	NG	OFF	未	点滅	消灯
	{flags_combie(PortEn_En, LightMod_EMi, CpriState_F, ForceStop_OFF, L3HC_NG, Layer3_Mi),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, ForceStop_mask, L3HC_mask, Layer3_mask),
	LED_XX, LED_OFF},
	//[9] En	有(未サポート品)	F	-	ON	-	点滅	消灯
	{flags_combie(PortEn_En, LightMod_EMi, CpriState_F, ForceStop_ON, 0, 0),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, ForceStop_mask, 0, 0),
	LED_XX, LED_OFF},
	//[10] En	有(未サポート品)	F	OK	OFF	未	点滅	点滅
	{flags_combie(PortEn_En, LightMod_EMi, CpriState_F, ForceStop_OFF, L3HC_OK, Layer3_Mi),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, ForceStop_mask, L3HC_mask, Layer3_mask),
	LED_XX, LED_XX},
	//[11] En	有(未サポート品)	F	OK	OFF	完	点滅	点灯
	{flags_combie(PortEn_En, LightMod_EMi, CpriState_F, ForceStop_OFF, L3HC_OK, Layer3_Kan),
	masks_combie(PortEn_mask, LightMod_mask, CpriState_mask, ForceStop_mask, L3HC_mask, Layer3_mask),
	LED_XX, LED_ON},
};

static struct {
	USHORT value;
	USHORT mask;
	USHORT orgLed;
	USHORT grnLed;
} BDEInfoTbl[] = {
	//[0] Not-ACT	-	-	-	消灯	消灯
	{flags_combie(ACT_OFF, 0, 0, 0, 0, 0),
	flags_combie(ACT_mask, 0, 0, 0, 0, 0),
	LED_OFF, LED_OFF},
	//[1] ACT	D.C	D.C	-	消灯	消灯
	//{flags_combie(ACT_ON, LightMod_DC, CpriState_DC, 0, 0, 0),
	//masks_combie(ACT_mask, LightMod_mask, CpriState_mask, 0, 0, 0),
	//LED_OFF, LED_OFF},
	//[2] ACT	無	D.C	-	点灯	消灯
	{flags_combie(ACT_ON, LightMod_N, 0, 0, 0, 0),
	masks_combie(ACT_mask, LightMod_mask, 0, 0, 0, 0),
	LED_ON, LED_OFF},
	//[3] ACT	有(Fサポート品)	F未満	-	点灯	消灯
	{flags_combie(ACT_ON, LightMod_EF, CpriState_FMi, 0, 0, 0),
	masks_combie(ACT_mask, LightMod_mask, CpriState_mask, 0, 0, 0),
	LED_ON, LED_OFF},
	//[4] ACT	有(Fサポート品)	F	NG	点灯	消灯
	{flags_combie(ACT_ON, LightMod_EF, CpriState_F, 0, L3HC_NG, 0),
	masks_combie(ACT_mask, LightMod_mask, CpriState_mask, 0, L3HC_mask, 0),
	LED_ON, LED_OFF},
	//[5] ACT	有(Fサポート品)	F	OK	消灯	点滅
	{flags_combie(ACT_ON, LightMod_EF, CpriState_F, 0, L3HC_OK, 0),
	masks_combie(ACT_mask, LightMod_mask, CpriState_mask, 0, L3HC_CMP_mask, 0),
	LED_OFF, LED_XX},
    //[6] ACT   有(Fサポート品)   F   OK  消灯  点灯
    {flags_combie(ACT_ON, LightMod_EF, CpriState_F, 0, L3HC_OK_CMP, 0),
    masks_combie(ACT_mask, LightMod_mask, CpriState_mask, 0, L3HC_CMP_mask, 0),
    LED_OFF, LED_ON},
	//[7] ACT	有(未サポート品)	F未満	-	点滅	消灯
	{flags_combie(ACT_ON, LightMod_EMi, CpriState_FMi, 0, 0, 0),
	masks_combie(ACT_mask, LightMod_mask, CpriState_mask, 0, 0, 0),
	LED_XX, LED_OFF},
	//[8] ACT	有(未サポート品)	F	NG	点滅	消灯
	{flags_combie(ACT_ON, LightMod_EMi, CpriState_F, 0, L3HC_NG, 0),
	masks_combie(ACT_mask, LightMod_mask, CpriState_mask, 0, L3HC_mask, 0),
	LED_XX, LED_OFF},
	//[9] ACT	有(未サポート品)	F	OK	点滅	点滅
	{flags_combie(ACT_ON, LightMod_EMi, CpriState_F, 0, L3HC_OK, 0),
	masks_combie(ACT_mask, LightMod_mask, CpriState_mask, 0, L3HC_CMP_mask, 0),
	LED_XX, LED_XX},
    //[10] ACT   有(未サポート品)   F   OK  点滅  点灯
    {flags_combie(ACT_ON, LightMod_EMi, CpriState_F, 0, L3HC_OK_CMP, 0),
    masks_combie(ACT_mask, LightMod_mask, CpriState_mask, 0, L3HC_CMP_mask, 0),
    LED_XX, LED_ON},
};

static USHORT swLightModeData[D_RRH_CPRINO_NUM];

//-------------functions
USHORT CpriState2Flag(USHORT awCpriState)
{
	USHORT lwCpriStateFlags = 0;
	switch(awCpriState) {
	case D_RRH_CPRISTAT_F:
		lwCpriStateFlags = CpriState_F;
		break;
	default:
		lwCpriStateFlags = CpriState_FMi;
		break;
	}
	return lwCpriStateFlags;
}

USHORT LightMode_getAll()
{
	INT rtn_bpf;                                            /* for BPF */
	INT errcd;                                              /* for BPF */
	const UINT sem_id = (UINT)E_RRH_SEMID_APL_SVCTL;        /* semaphore id */

	INT i;

	struct {
		UINT data;
		UINT addr;
	} regs[] = {
		{0, D_RRH_REG_CNT_SV9INF},
		{0, D_RRH_REG_CNT_SV11INF},
		{0, D_RRH_REG_CNT_SV12INF},
	};

	struct {
		UINT opt;
		UINT bit;
		UINT addr;
		USHORT mod_v;
		USHORT mod_i;
	} ltOptMapTbl[] = {
#if 0
		{E_SYS_SVCTL_SFP_ERR1,     D_RRH_REG_CNT_SV9INF_SFP_ERR1,            D_RRH_REG_CNT_SV9INF, LightMod_EMi,0},
#endif
		{E_SYS_SVCTL_SFP_ERR2,     D_RRH_REG_CNT_SV9INF_SFP_ERR2,            D_RRH_REG_CNT_SV9INF, LightMod_EMi,0},

#if 0
		{E_SYS_SVCTL_SFP_ERR1_01, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE1 ), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 1 },
		{E_SYS_SVCTL_SFP_ERR1_02, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE2 ), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 2 },
		{E_SYS_SVCTL_SFP_ERR1_03, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE3 ), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 3 },
		{E_SYS_SVCTL_SFP_ERR1_04, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE4 ), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 4 },
		{E_SYS_SVCTL_SFP_ERR1_05, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE5 ), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 5 },
		{E_SYS_SVCTL_SFP_ERR1_06, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE6 ), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 6 },
		{E_SYS_SVCTL_SFP_ERR1_07, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE7 ), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 7 },
		{E_SYS_SVCTL_SFP_ERR1_08, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE8 ), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 8 },
		{E_SYS_SVCTL_SFP_ERR1_09, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE9 ), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 9 },
		{E_SYS_SVCTL_SFP_ERR1_10, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE10), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 10},
		{E_SYS_SVCTL_SFP_ERR1_11, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE11), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 11},
		{E_SYS_SVCTL_SFP_ERR1_12, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE12), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 12},
		{E_SYS_SVCTL_SFP_ERR1_13, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE13), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 13},
		{E_SYS_SVCTL_SFP_ERR1_14, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE14), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 14},
		{E_SYS_SVCTL_SFP_ERR1_15, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE15), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 15},
		{E_SYS_SVCTL_SFP_ERR1_16, M_RRH_REG_CNT_SFP_ERR1(D_RRH_CPRINO_RE16), D_RRH_REG_CNT_SV11INF, LightMod_EMi, 16},
#endif

		{E_SYS_SVCTL_SFP_ERR2_01, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE1 ), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 1 },
		{E_SYS_SVCTL_SFP_ERR2_02, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE2 ), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 2 },
		{E_SYS_SVCTL_SFP_ERR2_03, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE3 ), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 3 },
		{E_SYS_SVCTL_SFP_ERR2_04, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE4 ), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 4 },
		{E_SYS_SVCTL_SFP_ERR2_05, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE5 ), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 5 },
		{E_SYS_SVCTL_SFP_ERR2_06, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE6 ), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 6 },
		{E_SYS_SVCTL_SFP_ERR2_07, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE7 ), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 7 },
		{E_SYS_SVCTL_SFP_ERR2_08, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE8 ), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 8 },
		{E_SYS_SVCTL_SFP_ERR2_09, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE9 ), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 9 },
		{E_SYS_SVCTL_SFP_ERR2_10, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE10), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 10},
		{E_SYS_SVCTL_SFP_ERR2_11, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE11), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 11},
		{E_SYS_SVCTL_SFP_ERR2_12, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE12), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 12},
		{E_SYS_SVCTL_SFP_ERR2_13, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE13), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 13},
		{E_SYS_SVCTL_SFP_ERR2_14, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE14), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 14},
		{E_SYS_SVCTL_SFP_ERR2_15, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE15), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 15},
		{E_SYS_SVCTL_SFP_ERR2_16, M_RRH_REG_CNT_SFP_ERR2(D_RRH_CPRINO_RE16), D_RRH_REG_CNT_SV12INF, LightMod_EMi, 16},
	};

	/* take semaphore of SV control */
	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(sem_id, BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, NULL, &errcd);
	if (BPF_RU_IPCM_OK != rtn_bpf) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "SV CTL sem take error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}

	/* read register */
	for(i = 0; i<(sizeof(regs)/sizeof(regs[0])); i++)
	{
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regs[i].addr, &regs[i].data);
	}

	/* give semaphore of SV control */
	rtn_bpf = BPF_RU_IPCM_PROCSEM_GIVE(sem_id, BPF_RU_IPCM_LOCK_RW, &errcd);
	if (BPF_RU_IPCM_OK != rtn_bpf) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "SV CTL sem give error rtn_bpf=0x%08x errcd=0x%08x", rtn_bpf, errcd);
	}

	//set all light mode is F
	for(i = 0; i < D_RRH_CPRINO_NUM; i++)
	{
		swLightModeData[i] = LightMod_EF;
	}
	//if E_SYS_SVCTL_SFP_ERR1 無
	//if E_SYS_SVCTL_SFP_ERR2 未サポート
	for(i = 0; i<(sizeof(ltOptMapTbl)/sizeof(ltOptMapTbl[0]));i++)
	{
		UINT regDat = 0;
		UINT regAddr = ltOptMapTbl[i].addr;
		//regDat = regs_search(regAddr);

		INT iReg;
		for (iReg = 0; iReg < count_of(regs); iReg++)
		{
			if (regs[iReg].addr == regAddr) break;
		}
		regDat = regs[iReg].data;

		if (regDat & ltOptMapTbl[i].bit)
		{
			USHORT index = ltOptMapTbl[i].mod_i;
			USHORT value = ltOptMapTbl[i].mod_v;

			if (LightMod_N != swLightModeData[index])
			{
			    swLightModeData[index] = value;
			}
		}
	}

	return 0;
}

USHORT get_RE_led_mode(
					 USHORT flag_PortEn,
					 USHORT flag_LightMod,
					 USHORT flag_CpriState,
					 USHORT flag_L3HC,
					 USHORT flag_ForceStop,
					 USHORT flag_Layer3,
					 USHORT *RE_led_orange_p,
					 USHORT *RE_led_green_p
					 )
{
	USHORT flag = 0;
	USHORT RE_led_orange = 0;
	USHORT RE_led_green = 0;

	//combine RE inf flag
	flag = flags_combie(flag_PortEn,
		flag_LightMod,
		flag_CpriState,
		flag_ForceStop,
		flag_L3HC,
		flag_Layer3);

	USHORT lwIndex = 0;

	//get RE inf LED mode
	for(lwIndex = 0; lwIndex <(sizeof(REInfoTbl)/sizeof(REInfoTbl[0]));lwIndex++)
	{
		if ((flag & REInfoTbl[lwIndex].mask) == REInfoTbl[lwIndex].value)
		{
			RE_led_orange = REInfoTbl[lwIndex].orgLed;
			RE_led_green = REInfoTbl[lwIndex].grnLed;
			break;
		}
	}

	*RE_led_orange_p = RE_led_orange;
	*RE_led_green_p = RE_led_green;

	return lwIndex;
}

USHORT get_BDE_led_mode(
					  USHORT flag_Act,
					  USHORT flag_LightMod,
					  USHORT flag_CpriState,
					  USHORT flag_L3HC,

					  USHORT *BDE_led_orange_p,
					  USHORT *BDE_led_green_p
					  )
{
	USHORT flag = 0;
	USHORT BDE_led_orange = 0;
	USHORT BDE_led_green = 0;

	USHORT lwIndex = 0;

	//combine BDE inf flag
	flag = flags_combie(flag_Act,
		flag_LightMod,
		flag_CpriState,
		0,
		flag_L3HC,
		0);

	//get BDE inf LED mode
	for(lwIndex = 0; lwIndex<(sizeof(BDEInfoTbl)/sizeof(BDEInfoTbl[0]));lwIndex++)
	{
		if ((flag & BDEInfoTbl[lwIndex].mask) == BDEInfoTbl[lwIndex].value)
		{
			BDE_led_orange = BDEInfoTbl[lwIndex].orgLed;
			BDE_led_green = BDEInfoTbl[lwIndex].grnLed;
			break;
		}
	}

	*BDE_led_orange_p = BDE_led_orange;
	*BDE_led_green_p = BDE_led_green;

	return lwIndex;
}
/**************************************************************************/
/**
 *  @brief  LED Control Register Write process
 *  @note   Write the data to the specified LED control Register \n
 *  
 *  
 *  @param  FileNamep       [in]    calling function file name
 *  @param  LineNo          [in]    calling function executing line
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01915
 *  @date   2008/08/27   FFCS)zhengmh create
 *  @date   2009/12/28 QNET)Kabasima M-S3G-eNBSYS-00991 ST RRE ALM時のLED表示について
 *  @date   2010/05/20 FJT)Otani  CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *  @date   2010/05/28 FJT)Otani  M-S3G-eNBPF-03498 [800M-RRE]ACT+USE状態からのBLK制御でUSE LEDが消灯しない
 *  @date   2010/06/23 FJT)Otani  M-S3G-eNBPF-03587 [800M-RRE SST]TRX設定後のリセットでUSE点灯すると同時にBSYが消灯
 *  @date   2010/08/03 CMS)Yorozuya  CR-00014-000 : ハソV2.19-02版
 *  @date   2010/08/19 CMS)Yorozuya  S3G-eNBPF-03701
 *  @date   2012/07/19 FFCS)niemsh modify for 1.5GVA
 *  @date   2013/11/25 FFCS)Xut modify for ZYNQ.
 *  @date   2015/04/27 FFCS)zhangxq modify for TDD-SRE
 *  @date   2015/09/02 TDI)satou カード状態がBLKの場合を削除(FHMにBLKは存在しない)
 *  @date   2015/09/04 TDI)satou LED1-3をFHM用に修正
 *  @date   2015/10/06 TDI)satou 「ダウンロード中」の判定をFHM用に修正
 *  @date   2015/10/27 TDI)satou M-RRU-ZSYS-01915 問処No.148 LED制御におけるMasterのLayer3状態の判定値を修正
 *  @date   2015/11/20 TDI)satou IT3問処No.96
 *  @date   2016/11/18 FJT)ohashi 16B スロット数制限機能追加
 */
/**************************************************************************/
VOID m_cmR_WriteLEDReg(
				   VOID*	FileNamep,
				   UINT		LineNo )
{
	INT		ret;
	USHORT	cardsta = 0;
	USHORT	multi_alm;
	UINT	dlm_sta;
	INT		wtime = 10;						/*  応答メッセージ待ち時間(msec単位)*/
	VOID *	data_p = 0;						/*  取得データ用ポインタ		*/
	UINT	led1Sta = 0;
	UINT	led2Sta = 0;
	UINT	led3Sta = 0;
	UINT    actLedSta[D_RRH_CPRINO_NUM] = {0};  /* CPRI LINK LED (ACT) 設定値 */
	UINT    dwnLedSta[D_RRH_CPRINO_NUM] = {0};  /* CPRI LINK LED (DWN) 設定値 */
	USHORT  lwOrgLed = 0;
	USHORT  lwGrnLed = 0;

	struct ltStateFlags {
		USHORT	flag_PortEn;
		USHORT	flag_LightMod;
		USHORT	flag_CpriState;
		USHORT	flag_L3HC;
		USHORT	flag_ForceStop;
		USHORT	flag_Layer3;
	};

	struct ltStateFlags ltBDEInf = {0};
	struct ltStateFlags ltREInf[D_RRH_CPRINO_NUM];

	T_RRH_CONN_DISC		shmConnDisc;
	T_RRH_CPRI_STATE	shmCpriState = {0};                   /* CPRI state (shared memory) */
	T_RRH_LAYER3        ltLayer3[CMD_MAX_SYSNUM];
	USHORT              lwindex;
	USHORT              lwL3HcFlag[CMD_MAX_SYSNUM];

	memset(&ltREInf, 0, sizeof(ltREInf));

	/***************** */
	/* カード状態取得  */
	/***************** */
	/* カード状態取得関数をコールする  */
	m_cm_GetCardSta2(&cardsta,&multi_alm);

	// Get condition info
	memset(&shmConnDisc, 0, sizeof(shmConnDisc));
	ret = f_cmn_com_conn_disk_get(&shmConnDisc);
	if (D_RRH_OK != ret)
	{
		/* アサート出力処理 */
		cm_Assert(CMD_ASL_USELOW, ret, "f_cmn_com_conn_disk_get NG");
		return;
	}

	ret = f_cmn_com_cpri_state_get(&shmCpriState);
	if (D_RRH_OK != ret)
	{
		/* アサート出力処理 */
		cm_Assert(CMD_ASL_USELOW, ret, "f_cmn_com_cpri_state_get NG");
		return;
	}
	/* Layer3状態取得 */
	ret  = f_cmn_com_layer3_get(CMD_SYS_S3G, &ltLayer3[CMD_SYS_S3G]);
	ret |= f_cmn_com_layer3_get(CMD_SYS_3G,  &ltLayer3[CMD_SYS_3G ]);
	if (D_RRH_OK != ret)
	{
		/* アサート出力処理 */
		cm_Assert(CMD_ASL_USELOW, ret, "f_cmn_com_layer3_get NG");
		return;
	}
	/*get all light mod*/
	LightMode_getAll();

	// Get info for BDE-IF (REC)
	/* カード状態がACTの場合 */
	if( 0 != ( cardsta & CMD_CHK_ACT ) )
	{
		//Port En/ACT
		ltBDEInf.flag_PortEn = ACT_ON;
	}
	else
	{
		//Port En/ACT
		ltBDEInf.flag_PortEn = ACT_OFF;
	}
	//+ get Light Mode
	//	NOTE: TBD
	ltBDEInf.flag_LightMod = LightMode_get(0);

	//+ get CpriState
	ltBDEInf.flag_CpriState = CpriState2Flag(shmCpriState.cprista_rec);

	//+ get L3(H.C)
	if (CMD_SYS_S3G == cmw_validsysinfo)
	{
	    ltBDEInf.flag_L3HC = HealthChk2Flag(shmCpriState.healthchk_rec[CMD_SYS_S3G]);
	    ltBDEInf.flag_L3HC |= RecStartup2Flag(rec_Startup_First_flg[CMD_SYS_S3G]);
	}
	else
	{
	    lwL3HcFlag[CMD_SYS_S3G] = HealthChk2Flag(shmCpriState.healthchk_rec[CMD_SYS_S3G]);
	    lwL3HcFlag[CMD_SYS_3G ] = HealthChk2Flag(shmCpriState.healthchk_rec[CMD_SYS_3G ]);
	    lwL3HcFlag[CMD_SYS_S3G] |= RecStartup2Flag(rec_Startup_First_flg[CMD_SYS_S3G]);
	    lwL3HcFlag[CMD_SYS_3G ] |= RecStartup2Flag(rec_Startup_First_flg[CMD_SYS_3G ]);
	    ltBDEInf.flag_L3HC      = (lwL3HcFlag[CMD_SYS_S3G] & lwL3HcFlag[CMD_SYS_3G ]);
	}

	// Get info for RE-If
	/* 設定値と異なるCPRIリンク番号について増設/減設を実施 */
	for (lwindex = D_RRH_CPRINO_RE_MIN; lwindex <= D_RRH_CPRINO_RE_MAX; lwindex++) 
	{
		//Get Port En
		ltREInf[lwindex - 1].flag_PortEn = ConnDisc2Flag(shmConnDisc.conn_disc[lwindex - 1]);

		//Get Light Mode
		ltREInf[lwindex - 1].flag_LightMod = LightMode_get(lwindex);

		//Get Cpri state
		ltREInf[lwindex - 1].flag_CpriState = CpriState2Flag(shmCpriState.cprista_re[lwindex - 1]);

		//Get L3HC
		if (CMD_SYS_S3G == cmw_validsysinfo)
		{
            ltREInf[lwindex - 1].flag_L3HC = HealthChk2Flag(shmCpriState.healthchk_re[CMD_SYS_S3G][lwindex - 1]);
		}
		else
		{
		    lwL3HcFlag[CMD_SYS_S3G] = HealthChk2Flag(shmCpriState.healthchk_re[CMD_SYS_S3G][lwindex - 1]);
		    lwL3HcFlag[CMD_SYS_3G ] = HealthChk2Flag(shmCpriState.healthchk_re[CMD_SYS_3G ][lwindex - 1]);
		    ltREInf[lwindex - 1].flag_L3HC = (lwL3HcFlag[CMD_SYS_S3G] & lwL3HcFlag[CMD_SYS_3G ]) ;
		}

		//Get 遅延補正完/運用組込
		if (CMD_SYS_S3G == cmw_validsysinfo)
		{
		    if ((E_RRH_LAYER3_RE_OPE == ltLayer3[CMD_SYS_S3G].layer3_re[lwindex - 1])
		    &&  (ltREInf[lwindex - 1].flag_L3HC != 0                                ))
		    {
                ltREInf[lwindex - 1].flag_Layer3 = Layer3_Kan;
		    }
		    else
		    {
		        ltREInf[lwindex - 1].flag_Layer3 = Layer3_Mi;
		    }
//16B スロット数制限機能追加 start
		    if (E_RRH_LAYER3_RE_STOP == ltLayer3[CMD_SYS_S3G].layer3_re[lwindex - 1]){
//				サポート状況に応じてLED制御内容は変える
//              ltREInf[lwindex - 1].flag_LightMod = LightMod_EF;
                ltREInf[lwindex - 1].flag_CpriState = CpriState_F;
                ltREInf[lwindex - 1].flag_ForceStop = ForceStop_ON;
                ltREInf[lwindex - 1].flag_L3HC = 0;
                ltREInf[lwindex - 1].flag_Layer3 = 0;
			}
			else{
                ltREInf[lwindex - 1].flag_ForceStop = ForceStop_OFF;
			}
//16B スロット数制限機能追加 end
		}
		else
		{
		    if ((E_RRH_LAYER3_RE_OPE == ltLayer3[CMD_SYS_S3G].layer3_re[lwindex - 1])
		    &&  (E_RRH_LAYER3_RE_OPE == ltLayer3[CMD_SYS_3G ].layer3_re[lwindex - 1]))
		    {
                ltREInf[lwindex - 1].flag_Layer3 = Layer3_Kan;		//=1
		    }
		    else
		    {
		        ltREInf[lwindex - 1].flag_Layer3 = Layer3_Mi;
		    }
//16B スロット数制限機能追加 start
		    if ((E_RRH_LAYER3_RE_STOP == ltLayer3[CMD_SYS_S3G].layer3_re[lwindex - 1])
		    &&  (E_RRH_LAYER3_RE_STOP == ltLayer3[CMD_SYS_3G ].layer3_re[lwindex - 1])){
//				サポート状況に応じてLED制御内容は変える
//              ltREInf[lwindex - 1].flag_LightMod = LightMod_EF;
                ltREInf[lwindex - 1].flag_CpriState = CpriState_F;
                ltREInf[lwindex - 1].flag_ForceStop = ForceStop_ON;
                ltREInf[lwindex - 1].flag_L3HC = 0;
                ltREInf[lwindex - 1].flag_Layer3 = 0;
			}
			else{
                ltREInf[lwindex - 1].flag_ForceStop = ForceStop_OFF;
			}
//16B スロット数制限機能追加 end
		}
	}


	//actLedSta [0] [1]...[16]
	//dwnLedSta [0] [1]...[16]
	//           ^   ^-for RE
	//           ^- for DBE-REC
	get_BDE_led_mode(ltBDEInf.flag_PortEn,
		ltBDEInf.flag_LightMod,
		ltBDEInf.flag_CpriState,
		ltBDEInf.flag_L3HC,
		&lwOrgLed,
		&lwGrnLed);
	actLedSta[0] = lwGrnLed;
	dwnLedSta[0] = lwOrgLed;
	for (lwindex = 0; lwindex < D_RRH_CPRINO_RE_MAX; lwindex++)
	{
		get_RE_led_mode(ltREInf[lwindex].flag_PortEn,
			ltREInf[lwindex].flag_LightMod,
			ltREInf[lwindex].flag_CpriState,
			ltREInf[lwindex].flag_L3HC,
			ltREInf[lwindex].flag_ForceStop,
			ltREInf[lwindex].flag_Layer3,
			&lwOrgLed,
			&lwGrnLed
			);
		actLedSta[lwindex+1] = lwGrnLed;
		dwnLedSta[lwindex+1] = lwOrgLed;
	}

	/* If RRH kind is RRE or SRE */
		/* カード状態がALMの場合 */
	if(CMD_CHK_ALM & cardsta)
	{
        /* Set LED1 to RED ON */
        led1Sta = D_RRH_LED1STATE_ON;
	}

	/* カード状態がACTの場合 */
	if(CMD_CHK_ACT & cardsta)
	{
		/* カード状態がUSEの場合 */
		if(CMD_CHK_USE & cardsta)
		{
			/* Set LED2 to Green ON */
			led2Sta = D_RRH_LED2STATE_ON;
		}
		else
		{
			/* Set LED1 to Green Blink */
			led2Sta = D_RRH_LED2STATE_BLNK;
		}
	}
	
	/* オート変数をダウンロード無しで初期化する */
	dlm_sta = CMD_OFF;
	
	/* ファームウェアのファイル情報報告通知を受信してからファイルデータ送信完了報告応答までの場合 */
	/* 運用中ダウンロードも含む */
	if ((gt_dlw_rec_tbl.rec_sta == EC_DLM_MAIN_STN_FILEDATATRANS_DES_WAIT       ) /* ファイルデータ転送先決定待ち */
	||  (gt_dlw_rec_tbl.rec_sta == EC_DLM_MAIN_STN_FILEDATATRANS_FW_WAIT        ) /* ファイルデータ送信(FW)待ち */
	||  (gt_dlw_rec_tbl.rec_sta == EC_DLM_MAIN_STN_FILEDATATRANS_FINREP_WAIT    ) /* ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	||  (gt_dlw_rec_tbl.rec_sta == EC_DLM_MAIN_STN_USE_FILEDATATRANS_DES_WAIT   ) /* 運用中ファイルデータ転送先決定待ち */
	||  (gt_dlw_rec_tbl.rec_sta == EC_DLM_MAIN_STN_USE_FILEDATATRANS_FW_WAIT    ) /* 運用中ファイルデータ送信(FW)待ち */
	||  (gt_dlw_rec_tbl.rec_sta == EC_DLM_MAIN_STN_USE_FILEDATATRANS_FINREP_WAIT) /* 運用中ファイルデータ送信完了報告応答(REC←FHM) 送信待ち */
	){
		/* ダウンロード中 */
		dlm_sta = CMD_ON;
	}

	/* ダウンロード中の場合 */
	if(dlm_sta == CMD_ON)
	{
		/* Set LED2 state to blink */
		led3Sta = D_RRH_LED3STATE_BLNK;
	}
	/* カード状態がERRの場合 */
	else if((CMD_CHK_ERR & cardsta) && (dlm_sta == CMD_OFF))
	{
        /* Set LED3 state to ON */
        led3Sta = D_RRH_LED3STATE_ON;
	}
	else
	{
		/* Set LED3 state to OFF */
		led3Sta = D_RRH_LED3STATE_OFF;
	}

	/* Call API Function:  LED setting Notification */
	ret = rrhApi_Svp_Mnt_LedSet(D_RRH_PROCQUE_L3, wtime, data_p, led1Sta, led2Sta, led3Sta, actLedSta, dwnLedSta);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_USELOW,ret,"rrhApi_Svp_Mnt_LedSet NG");
	}
	
	return;
}

/* @} */


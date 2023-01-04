/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_TraStaChgNtc.c
 *  @brief  TRA card status change interruption handle function
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  @date   2009/05/18 FFCS)Wangjuan M-S3G-eNBPF-02009 when several alarm code exist, all need report.
 *  @date   2009/08/05 FFCS)wangjuan M-S3G-eNBPF-02334 TRA no need stop wave and report carrier alarm when 3G ERROR control happen
 *  @date   2009/10/08 QNET)Kabasima CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2009/11/17 QNET)Kabasima M-S3G-eNBPF-02864:[品質改善]ハードALM状変受信時の処理誤り
 *  @date   2009/11/17 QNET)Kabasima M-S3G-eNBSYS-00828 ST USE状態のTRAへALM制御した時の前面LED表示
 *  @date   2010/01/09 QNET)Kabasima M-S3G-eNBPF-02830:[品質改善]再開時リソースALMした場合にEtherが繋がらない追加修正
 *  @date   2010/01/20 QNET)Kabasima CR-00085-000:[DCM向け]REハード・ソフトインタフェース仕様書v2.14版リリース
 *  @date   2010/02/19 QNET)Kabasima CR-xxxxx-xxx [DCM向け]TS-145-ALM制御時のRE動作の件
 *  @date   2010/03/02 QNET)Kabasima M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
 *  @date   2010/04/02 FJT)Taniguchi CR-XXXXX-XXX:F-971(DCM)対応
 *  @date   2010/05/10 FJT)Taniguchi M-S3G-eNBSYS-01567:☆ST_RE☆共用RREへのリモートALM時の動作対処
 *  @date   2010/05/06 FJT)Otani  CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *  @date   2010/07/07 FJT)Taniguchi modify CR-000XX RF共用時のハソ変更
 *  @date   2011/10/11 FFCS)Linlj  modify for 省電力対応
 *  @date   2011/10/21 FJT)Tokunaga M-S3G-eNBPF-04071対応
 *  @date   2011/10/23 COMAS)Uchida M-S3G-eNBPF-04074対応
 *  @date   2011/11/16 FJT)Tokunaga M-S3G-eNBPF-04083/M-S3G-eNBPF-04089対応
 *  @date   2011/11/22 FJT)Tokunaga M-S3G-eNBSYS-02540対応
 *  @date   2011/11/24 COMAS)Uchida M-S3G-eNBSYS-02539対応
 *  @date   2012/01/12 FJT)Tokunaga M-S3G-eNBPF-04092 CR-00054-000 15G-RREのPA OFF問題対応
 *  @date   2012/01/12 FJT)Tokunaga M-S3G-eNBPF-04091
 *                     CeNB-F-075-061(F)(F-1256(DCM)回答：BRE(RF共用)装置におけるUSE条件およびセクタ障害警報信号送信条件について)
 *  @date   2012/07/25 FFCS)niemsh create for 1.5GVA
 *  @date   2012/10/18 FFCS)niemsh modify for M-S3G-eNBSYS-02684
 *  @date	2012/10/22 FFCS)niemsh modify for M-S3G-eNBPF-04288
 *  @date	2012/11/06 FFCS)niemsh modify for M-S3G-eNBSYS-04303
 *  @date	2013/10/10 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
 *  @date   2013/11/18 FFCS)Xut modify for ZYNQ.
 *  @date   2015/04/21 FFCS)zhangxq modify for TDD-SRE
 *  @date   2015/06/22 ALPHA)ueda modify for TDD-SRE(Zynq)
 *  @date   2016/11/24 FJT)ohashi 16B スロット数制限機能追加
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */
#include "f_rrh_inc.h"
#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_rc_header.h"			/* RE監視タスクヘッダファイル			*/
#include "rrhApi_Svp.h"
#include "f_com_aplCom.h"

const USHORT rcr_errcode_cnv[] = {
    /* CNTM_SV9INF */
    0x0001, 0x0002, 0x0003, 0x0003, 0x0000, 0x0009, 0x0000, 0x0000,
    0x0006, 0x0006, 0x0000, 0x0004, 0x0005, 0x0000, 0x0000, 0x0000,
    0x000B, 0x0015, 0x001F, 0x0029, 0x0033, 0x003D, 0x0047, 0x0051,
    0x005B, 0x0065, 0x006F, 0x0079, 0x0083, 0x008D, 0x0097, 0x00A1,
    /* CNTM_SV10INF */
    0x000C, 0x0016, 0x0020, 0x002A, 0x0034, 0x003E, 0x0048, 0x0052,
    0x005C, 0x0066, 0x0070, 0x007A, 0x0084, 0x008E, 0x0098, 0x00A2,
    0x000D, 0x0017, 0x0021, 0x002B, 0x0035, 0x003F, 0x0049, 0x0053,
    0x005D, 0x0067, 0x0071, 0x007B, 0x0085, 0x008F, 0x0099, 0x00A3,
    /* CNTM_SV11INF */
    0x000D, 0x0017, 0x0021, 0x002B, 0x0035, 0x003F, 0x0049, 0x0053,
    0x005D, 0x0067, 0x0071, 0x007B, 0x0085, 0x008F, 0x0099, 0x00A3,
    0x0013, 0x001D, 0x0027, 0x0031, 0x003B, 0x0045, 0x004F, 0x0059,
    0x0063, 0x006D, 0x0077, 0x0081, 0x008B, 0x0095, 0x009F, 0x00A9,
    /* CNTM_SV12INF */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0012, 0x001C, 0x0026, 0x0030, 0x003A, 0x0044, 0x004E, 0x0058,
    0x0062, 0x006C, 0x0076, 0x0080, 0x008A, 0x0094, 0x009E, 0x00A8,
    /* CNTM_SV13INF */
    0x0010, 0x001A, 0x0024, 0x002E, 0x0038, 0x0042, 0x004C, 0x0056,
    0x0060, 0x006A, 0x0074, 0x007E, 0x0088, 0x0092, 0x009C, 0x00A6,
    0x0010, 0x001A, 0x0024, 0x002E, 0x0038, 0x0042, 0x004C, 0x0056,
    0x0060, 0x006A, 0x0074, 0x007E, 0x0088, 0x0092, 0x009C, 0x00A6,
    /* CNTM_SV14INF */
    0x0011, 0x001B, 0x0025, 0x002F, 0x0039, 0x0043, 0x004D, 0x0057,
    0x0061, 0x006B, 0x0075, 0x007F, 0x0089, 0x0093, 0x009D, 0x00A7,
    0x000F, 0x0019, 0x0023, 0x002D, 0x0037, 0x0041, 0x004B, 0x0055,
    0x005F, 0x0069, 0x0073, 0x007D, 0x0087, 0x0091, 0x009B, 0x00A5,
    /* CNTM_SV15INF */
    0x000E, 0x0018, 0x0022, 0x002C, 0x0036, 0x0040, 0x004A, 0x0054,
    0x005E, 0x0068, 0x0072, 0x007C, 0x0086, 0x0090, 0x009A, 0x00A4,
    0x000E, 0x0018, 0x0022, 0x002C, 0x0036, 0x0040, 0x004A, 0x0054,
    0x005E, 0x0068, 0x0072, 0x007C, 0x0086, 0x0090, 0x009A, 0x00A4,
    /* CNTM_SV16INF */
    0x0011, 0x001B, 0x0025, 0x002F, 0x0039, 0x0043, 0x004D, 0x0057,
    0x0061, 0x006B, 0x0075, 0x007F, 0x0089, 0x0093, 0x009D, 0x00A7,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* CNTM_SV5INF */
    0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0,
    0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8,
    0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0,
    0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8
};

const USHORT rcr_err_systyp[CMD_MAX_SYSNUM][18] = {
    {0x0127,0xFFFF,0xFFFF,0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0xFFFF,0x0000,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0xFFFF,0x0000},
    {0x1A2B,0xFFFF,0xFFFF,0x0000,0xFFFF,0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0xFFFF,0x0000,0x0000,0xFFFF}
};

/* @{ */
/**
* @brief TRA card control table update function.
* @note update TRA card control table according to TRA card status change factor.\n
* @param factor [in] TRA card status change factor
* @return  none
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2011/10/11 FFCS)Linlj  modify for 省電力対応
* @date 2012/05/01 FJT)Tokunaga M-S3G-eNBPF-04116;REが検出するERRの通知先システムについて
**/

VOID m_rc_cm_CardCntTblUpd( UINT factor )
{
#if 0 //TODO 障害監視の仕様提示待ち
	USHORT	a_remote_flag		=CMD_OFF;	/* remote control flag */
	//cnt_extitem_s3g init
	cmw_cardcnt_tbl.cnt_extitem_s3g = CMD_NUM0;
	switch (factor)
	{
		/*************************/
		/* 割込要因へ ALM"の場合 */
		/*************************/		
		case RCD_ALM:
			
			/* search card control table, to check remote ALM control exist or not */
			if ( cmw_cardcnt_tbl.exc_itm == CMD_CARDALM )
			{
				/* remote ALM control exist */
				a_remote_flag = CMD_ON;
			}
			
			/* リモート制御による割込の場合 */
			if( a_remote_flag == CMD_ON )
			{
				/* カード制御状態テーブルに"リモートALM"制御を保存*/
				cmw_cardcnt_tbl.cnt_kind = CMD_RMT_CNT;
				cmw_cardcnt_tbl.cnt_itm_s3g = CMD_REMOTE_ALM;
				cmw_cardcnt_tbl.exc_itm = CMD_NUM0;
				
				/* カード制御状態テーブルの制御コードを更新 */
				cmw_cardcnt_tbl.cnt_code = cmw_cardcnt_tbl.exc_code;
				cmw_cardcnt_tbl.exc_code = CMD_IVL_VAL;
				
				/* カード制御状態テーブルの強制閉塞フラグを更新 */
				cmw_cardcnt_tbl.cnt_cmpblk  = cmw_cardcnt_tbl.exc_cmpblk;

				/* set remote control flag to ON */
				rcw_remotecnt_flg = CMD_ON;
			}
			/* ローカル制御による割込の場合 */
			/* modify for M-S3G-eNBSYS-00009 */
			/* if remote control item already exist, do not update local control to card control table */
			/* in order to avoid the case when several remote control happened within 200ms, */
			/* local card control would be reported,it is not OK. 				*/
			else if((a_remote_flag == CMD_OFF) && (rcw_remotecnt_flg == CMD_OFF))
			{
				/* カード制御状態テーブルに"ローカルALM"制御を保存*/
				cmw_cardcnt_tbl.cnt_kind = CMD_LCL_CNT;
				cmw_cardcnt_tbl.cnt_itm_s3g = CMD_NUM0;
				cmw_cardcnt_tbl.cnt_code = CMD_IVL_VAL;		
			}
			else
			{
				/* 無処理*/
			}			
			
			break;

		/*********************************************************************/
		/* 割込要因へ "USE","UN-USE"の場合 */
		/*********************************************************************/
		/* modify for M-S3G-eNBSYS-00016 */
		case RCD_USE:
		case RCD_UNUSE:

			cmw_cardcnt_tbl.cnt_itm_s3g = CMD_NUM0;
			cmw_cardcnt_tbl.cnt_code = CMD_IVL_VAL;
			
			break;

		/*************************/
		/* 割込要因へ ERR"の場合 */
		/*************************/
		case RCD_ERR:
			/* リモート制御による割込の場合 */
			if( a_remote_flag == CMD_ON )
			{
				/* カード制御種別に"リモートERR"を設定 */
				/* S3Gのカード制御項目は更新しない(リモートERR制御へ Gのみに報告 */
				cmw_cardcnt_tbl.cnt_kind = CMD_RMT_CNT;
				cmw_cardcnt_tbl.exc_itm = CMD_NUM0;
				
				/* カード制御状態テーブルの制御コードを更新 */
				cmw_cardcnt_tbl.cnt_code = cmw_cardcnt_tbl.exc_code;
				cmw_cardcnt_tbl.exc_code  = CMD_IVL_VAL;

				/* カード制御状態テーブルの強制閉塞フラグを更新 */
				cmw_cardcnt_tbl.cnt_cmpblk  = cmw_cardcnt_tbl.exc_cmpblk;

				/* set remote control flag to ON */
				rcw_remotecnt_flg = CMD_ON;				
			}
			/* ローカル制御による割込の場合 */
			else if(( a_remote_flag == CMD_OFF ) && (rcw_remotecnt_flg == CMD_OFF))
			{
				/* カード制御状態テーブルに"ローカルERR"制御を保存*/
				cmw_cardcnt_tbl.cnt_kind = CMD_LCL_CNT;
				cmw_cardcnt_tbl.cnt_itm_s3g = CMD_NUM0;
				cmw_cardcnt_tbl.cnt_code = CMD_IVL_VAL;                        		
			}
			else
			{
				/* 無処理*/
			}			
			break;

		/*************************/
		/* 割込要因へ ERR OFF"の場合 */
		/*************************/
		case RCD_ERROFF:
			/* リモート制御による割込の場合 */
			if( a_remote_flag == CMD_ON )
			{
				/* カード制御種別に"リモートERR回復"を設定 */
				/* S3Gのカード制御項目は更新しない(リモートERR回復へ Gのみに報告 */
				cmw_cardcnt_tbl.cnt_kind = CMD_RMT_CNT;
				cmw_cardcnt_tbl.exc_itm = CMD_NUM0;

				/* カード制御状態テーブルの制御コードを更新 */
				cmw_cardcnt_tbl.cnt_code = cmw_cardcnt_tbl.exc_code;
				cmw_cardcnt_tbl.exc_code   = CMD_IVL_VAL;

				/* カード制御状態テーブルの強制閉塞フラグを更新 */
				cmw_cardcnt_tbl.cnt_cmpblk  = cmw_cardcnt_tbl.exc_cmpblk;

				/* set remote control flag to ON */
				rcw_remotecnt_flg = CMD_ON;								
			}
			/* ローカル制御による割込の場合 */
			else if(( a_remote_flag == CMD_OFF ) && (rcw_remotecnt_flg == CMD_OFF))
			{
				/* カード制御状態テーブルに"ローカルERR回復"制御を保存*/
				cmw_cardcnt_tbl.cnt_kind = CMD_LCL_CNT;
				cmw_cardcnt_tbl.cnt_itm_s3g = CMD_NUM0;
				cmw_cardcnt_tbl.cnt_code = CMD_IVL_VAL;                        		                            
			}
			else
			{
				/* 無処理*/
			}			
			break;

		default:
			break;
			
	}
#endif
}	

/* @{ */
/**
* @brief TRA card status change sub handle function for ACT on case
* @note Perform ACT on.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2015/04/21 FFCS)zhangxq modify for TDD-SRE
*/
VOID m_rc_sub_actOnProc()
{
	
	/* if TRA card first time ACT, start up CPRI link */
	if ( rcw_first_act_flg == CMD_OFF)
	{
		cmw_tra_sv_tbl.sv_inf[CMD_NUM0] |= D_RRH_ACT_BIT_ON;
		cmw_tra_sv_tbl.sv_inf[CMD_NUM0] |= D_RRH_INS_BIT;

		/* 初回ACT設定したのでONにする */
		rcw_first_act_flg  = CMD_ON;
		m_rc_CpriStartup();
	}
	return;
}

void m_rc_sub_actOffProc()
{}

/**
* @brief TRA card status change sub handle function for USE on case
* @note Perform USE on case.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2015/04/21 FFCS)zhangxq modify for TDD-SRE
*/
VOID m_rc_sub_useOnProc(VOID)
{
	/* update tra sv table info */
	cmw_tra_sv_tbl.sv_inf[CMD_NUM0] |= D_RRH_USE_BIT_ON;
	cmw_cardcnt_tbl.cnt_itm_s3g = CMD_NUM0;
	cmw_cardcnt_tbl.cnt_extitem_s3g = CMD_NUM0;

	return;
}


/**
* @brief TRA card status change sub handle function for UNUSE on case
* @note Perform UNUSE on case.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2012/10/18 FFCS)niemsh modify for M-S3G-eNBSYS-02684
* @date 2015/02/27 FFCS)zhangxq modify for TDD-SRE
*/
VOID m_rc_sub_useOffProc()
{
	
	/* update tra sv table info */
	cmw_tra_sv_tbl.sv_inf[CMD_NUM0] &= D_RRH_USE_BIT_OFF;
	/* LTE USE変化時、自システムの制御項目をクリア */
	cmw_cardcnt_tbl.cnt_itm_s3g = CMD_NUM0;
	cmw_cardcnt_tbl.cnt_extitem_s3g = CMD_NUM0;

	return;
}

/**
* @brief TRA card status change sub handle function for ERR on case
* @note Perform ERR on case.\n
* @param stainf_p [in] pointer of card status change notice
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2015/04/21 FFCS)zhangxq modify for TDD-SRE
* @date 2015/11/12 TDI)satou その他未実装-006 ERR CODE変換を追加
* @date 2015/12/04 TDI)satou IT3問処番号No.108
*/
VOID m_rc_sub_errOnProc(T_API_SVP_CARDSTACHG_NTC* stainf_p)
{
	UINT   a_factor = RCD_ERR;
	INT    idx_errInfo;
	INT    idx_errInfo_max;
	USHORT err_inf;
	UINT   bit;
	INT    idx_search;
	INT    hit;
	INT    idx_sys;

    /* No ERR -> ERRの場合 */
    if((cmw_tra_sv_tbl.sv_inf[CMD_NUM0] & D_RRH_ERR_BIT_ON ) == CMD_NUM0 )
    {
        /* Set SV-A0 (ERR bit) to ON */
        /* this is automatically done by hardware*/
        /* update sv table info */
        cmw_tra_sv_tbl.sv_inf[CMD_NUM0] |= D_RRH_ERR_BIT_ON;
    }

    cmw_tra_sv_tbl.err_num = 0;
    cmw_tra_sv_tbl.sys[CMD_SYS_3G ].err_num = 0;
    cmw_tra_sv_tbl.sys[CMD_SYS_S3G].err_num = 0;
    memset(cmw_tra_sv_tbl.err_inf, 0, sizeof cmw_tra_sv_tbl.err_inf);
    memset(cmw_tra_sv_tbl.sys[CMD_SYS_3G ].err_inf, 0, sizeof cmw_tra_sv_tbl.sys[CMD_SYS_3G ].err_inf);
    memset(cmw_tra_sv_tbl.sys[CMD_SYS_S3G].err_inf, 0, sizeof cmw_tra_sv_tbl.sys[CMD_SYS_S3G].err_inf);

    err_inf = 1;
    idx_errInfo_max = sizeof(stainf_p->cardSta.errInfo.err)/sizeof(stainf_p->cardSta.errInfo.err[0]);
    for (idx_errInfo = 0; idx_errInfo < idx_errInfo_max; idx_errInfo++)
    {
        for (bit = 1; bit != 0x10000; bit <<= 1, err_inf++)
        {
            if (0 != (stainf_p->cardSta.errInfo.err[idx_errInfo] & bit))
            {
                /* 同じERR CODEのものが存在する場合、格納しない */
                hit = 0;
                for (idx_search = 0; idx_search < cmw_tra_sv_tbl.err_num; idx_search++)
                {
                    if (rcr_errcode_cnv[err_inf - 1] == cmw_tra_sv_tbl.err_inf[idx_search])
                    {
                        hit = 1;
                        break;
                    }
                }

                if (hit == 0)
                {
                    cmw_tra_sv_tbl.err_inf[cmw_tra_sv_tbl.err_num] = rcr_errcode_cnv[err_inf - 1];
                    cmw_tra_sv_tbl.err_num++;
                }

                for (idx_sys = 0; idx_sys < CMD_MAX_SYSNUM; idx_sys++)
                {
                    if (0 != (rcr_err_systyp[idx_sys][idx_errInfo] & bit))
                    {
                        /* 同じERR CODEのものが存在する場合、格納しない */
                        hit = 0;
                        for (idx_search = 0; idx_search < cmw_tra_sv_tbl.sys[idx_sys].err_num; idx_search++)
                        {
                            if (rcr_errcode_cnv[err_inf - 1] == cmw_tra_sv_tbl.sys[idx_sys].err_inf[idx_search])
                            {
                                hit = 1;
                                break;
                            }
                        }

                        if (0 == hit)
                        {
                            cmw_tra_sv_tbl.sys[idx_sys].err_inf[cmw_tra_sv_tbl.sys[idx_sys].err_num] = rcr_errcode_cnv[err_inf - 1];
                            cmw_tra_sv_tbl.sys[idx_sys].err_num++;
                        }
                    }
                }
            }
        }
    }

    l3_com_ushortsort_asc(cmw_tra_sv_tbl.err_num, sizeof(USHORT)/sizeof(USHORT), cmw_tra_sv_tbl.err_inf);
    l3_com_ushortsort_asc(cmw_tra_sv_tbl.sys[CMD_SYS_3G ].err_num, sizeof(USHORT)/sizeof(USHORT), cmw_tra_sv_tbl.sys[CMD_SYS_3G ].err_inf);
    l3_com_ushortsort_asc(cmw_tra_sv_tbl.sys[CMD_SYS_S3G].err_num, sizeof(USHORT)/sizeof(USHORT), cmw_tra_sv_tbl.sys[CMD_SYS_S3G].err_inf);

    /* update TRA card control table */
    m_rc_cm_CardCntTblUpd(a_factor);
}


/**
* @brief TRA card status change sub handle function for ERR off case
* @note Perform ERR off case.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2015/04/21 FFCS)zhangxq modify for TDD-SRE
* @date 2015/12/04 TDI)satou IT3問処番号No.108
*/
VOID m_rc_sub_errOffProc(VOID)
{
    cmw_tra_sv_tbl.err_num = 0;
    cmw_tra_sv_tbl.sys[CMD_SYS_3G ].err_num = 0;
    cmw_tra_sv_tbl.sys[CMD_SYS_S3G].err_num = 0;

    /* Set SV-A0 (ERR bit) to OFF*/
    /* this is automatically done by hardware*/
    /* update sv table info */
    cmw_tra_sv_tbl.sv_inf[CMD_NUM0] &= D_RRH_ERR_BIT_OFF;

    /* update TRA card control table */
    m_rc_cm_CardCntTblUpd(RCD_ERROFF);
}

/**
* @brief TRA card status change sub handle function for single ALM on case
* @note Perform  single ALM on case.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2013/10/07 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
* @date 2015/04/21 FFCS)zhangxq modify for TDD-SRE
*/
VOID m_rc_sub_singleAlmProc()
{
	/*update ALM bit*/
	cmw_tra_sv_tbl.sv_inf[CMD_NUM0] |= D_RRH_ALM_BIT_ON;
#if 0 //TODO 障害監視の仕様提示待ち

	/* set SV-A2 USE off*/
	if(cmw_tra_sv_tbl.sv_inf[CMD_NUM1] & D_RRH_BR0ALM_BIT)
	{
		/* BR0停波(ON)を実施する*/
		m_rc_cm_APDStpWavSet(CMD_BR0_BIT, CMD_ON);
	}
	
//TDD-SRE update
	/* set SV-A2 USE off*/
	if(cmw_tra_sv_tbl.sv_inf[CMD_NUM1] & D_RRH_BR1ALM_BIT)
	{
		/* BR1停波(ON)を実施する*/
		m_rc_cm_APDStpWavSet(CMD_BR1_BIT, CMD_ON);
	}
	/* set SV-A2 USE off*/
	if(cmw_tra_sv_tbl.sv_inf[CMD_NUM1] & D_RRH_BR2ALM_BIT)
	{
		/* BR2停波(ON)を実施する*/
		m_rc_cm_APDStpWavSet(CMD_BR2_BIT, CMD_ON);
	}
	if(cmw_tra_sv_tbl.sv_inf[CMD_NUM1] & D_RRH_BR3ALM_BIT)
	{
		/* BR3停波(ON)を実施する*/
		m_rc_cm_APDStpWavSet(CMD_BR3_BIT, CMD_ON);
	}
//TD-SRE update
	/* update TRA card control table */
	m_rc_cm_CardCntTblUpd(RCD_ALM);
	
	/*LED_ALMを点灯 */
	m_cm_WriteLEDReg();

	return;
#endif
}


/**
* @brief TRA card status change sub handle function for multi ALM on case
* @note Perform  multi ALM on case.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2012/10/18 FFCS)niemsh modify for M-S3G-eNBSYS-02684
* @date 2012/11/06 FFCS)niemsh modify for M-S3G-eNBSYS-04303
* @date 2013/10/07 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
* @date 2015/04/21 FFCS)zhangxq modify for TDD-SRE
* @date 2015/06/22 ALPHA)ueda modify for TDD-SRE(Zynq)
*/
VOID m_rc_sub_multiAlmProc()
{
#if 0 //TODO 障害監視の仕様提示待ち
	UINT a_count;
	UINT abort_flg = CMD_FALSE;
	USHORT almcode = CMD_NUM0;
//	UINT trxrel_rslt = CMD_OK;				/* TRX解放結果	*/
//	USHORT br_bitmap;
//	USHORT car_no;

	/* update TRA card control table */
	m_rc_cm_CardCntTblUpd(RCD_ALM);

	/* 両系TRAの停波(ON)を実施する*/
//TDD-SRE
	if(cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num == CMD_DEV_TYPE_NT2R)
	{
		m_rc_cm_APDStpWavSet(CMD_NT2R_BR_BIT, CMD_ON);
	}
	else
	{
		m_rc_cm_APDStpWavSet(CMD_NT4R_BR_BIT, CMD_ON);
	}
//TDD-SRE
	/* ALMの被疑箇所がTRX-INFの場合、アラーム状態に遷移 */
	for(a_count = CMD_NUM0; a_count < cmw_tra_sv_tbl.alm_num; a_count++ )
	{
		almcode = cmw_tra_sv_tbl.alm_inf[a_count];
		/**********************************************************************************/
		/* ALMコードを判定してTRX-INF部のALMであればアボートさせる      				  */
		/* ただし、D_RRH_ALMCD_REMOTEALM(0x00AF)はアボートさせない       				  */
		/* D_RRH_ALMCD_RTOS(0x0001)      から D_RRH_ALMCD_CHKSTP(0x002B)				  */
		/* D_RRH_ALMCD_PLLNG(0x00C0)  													  */
		/* D_RRH_ALMCD_FPGAIF1(0x00CE)													  */
		/* D_RRH_ALMCD_FPGAIF2(0x00CF)													  */
		/* D_RRH_ALMCD_CPLDIF1(0x00D0)													  */
		/* D_RRH_ALMCD_FPGANG1(0x00C1)から D_RRH_ALMCD_FPGANG2(0x00C2)  				  */
		/**********************************************************************************/
		if(((almcode >= D_RRH_ALMCD_OS) && (almcode <= D_RRH_ALMCD_CSTP)) || 
			(almcode == D_RRH_ALMCD_PLLNG)  ||
			(almcode == D_RRH_ALMCD_FPGAIF1)||
			(almcode == D_RRH_ALMCD_FPGAIF2)||
			(almcode == D_RRH_ALMCD_CPLDIF1)||
			((almcode >= D_RRH_ALMCD_FPGANG1) && (almcode <= D_RRH_ALMCD_FPGANG2)))
		{
			abort_flg = CMD_ON;

			break;
		}
	}
	if(abort_flg == CMD_ON)
	{
		/* TRX解放を実施*/
//TD-SRE
		/*nt2r*/
		if(cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num == CMD_DEV_TYPE_NT2R)
		{
			br_bitmap = RCD_TRA_BR_NT2R;
		}
		/*nt4r*/
		else
		{
			br_bitmap = RCD_TRA_BR_NT4R;
		}
		for(car_no = CMD_CARNO_C1; car_no <CMD_CARNO_C2; car_no++)
		{
			trxrel_rslt = m_rc_cm_TrxRelExec_LTE( car_no ,br_bitmap, CMD_SYS_S3G );
			/* TRX解放結果がNGの場合 */
			if(trxrel_rslt != CMD_OK)
			{
				/* ASSERT */
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0, 
							"m_rc_TrxRelNtcRcv CARREL NG");
			}
		}
		
//TD-SRE
	}
	/* ALMの被疑箇所がTRX-INF以外の場合 , keep CPRI link connection */
	else
	{
		/* m_rc_TcpInitRspのIFLG設定直後の片系ブランチALMなのか両系ブランチALMなのかを */
		/* 判断するフラグ*/
//		rcw_restart_both_alm_flg = CMD_ON;
		
		/* Clear all software error */
//		m_cm_ErrCntRegSet(CMD_CLR, CMD_ERR_AMP_DISC_ERR, CMD_CHKALM_UNNCY);
		cmw_tra_sv_tbl.sv_inf[CMD_NUM4] &= CMD_AMPDISC_BIT;
		
		/* ハードERRがない場合はERR OFF */
		/*m_rc_HaveHardErrChk function  not err return 1 */
		if(m_rc_HaveHardErrChk())
		{	
			/* update tra sv table info */
			cmw_tra_sv_tbl.sv_inf[CMD_NUM0] &= D_RRH_ERR_BIT_OFF;
		}

		/*update ALM bit*/
		cmw_tra_sv_tbl.sv_inf[CMD_NUM0] |= D_RRH_ALM_BIT_ON;
	}

	return;
#endif
}

/* @} */

/* @{ */
/**
* @brief TRA card status change interruption handle function
* @note TRA card status change factor judge,table update, and status change handle.\n
* @param stainf_p [in] the buffer address pointer of received message
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2009/11/17 QNET)Kabasima M-S3G-eNBPF-02864:[品質改善]ハードALM状変受信時の処理誤り
* @date 2009/11/17 QNET)Kabasima M-S3G-eNBSYS-00828 ST USE状態のTRAへALM制御した時の前面LED表示
* @date 2010/01/09 QNET)Kabasima M-S3G-eNBPF-02830:[品質改善]再開時リソースALMした場合にEtherが繋がらない追加修正
* @date 2010/01/20 QNET)Kabasima CR-00085-000:[DCM向け]REハード・ソフトインタフェース仕様書v2.14版リリース
* @date 2010/02/19 QNET)Kabasima CR-xxxxx-xxx [DCM向け]TS-145-ALM制御時のRE動作の件
* @date 2010/03/02 QNET)Kabasima M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
* @date 2010/04/02 FJT)Taniguchi CR-XXXXX-XXX:F-971(DCM)対応
* @date 2010/05/10 FJT)Taniguchi M-S3G-eNBSYS-01567:☆ST_RE☆共用RREへのリモートALM時の動作対処
* @date 2010/05/06 FJT)Otani     CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
* @date 2010/07/07 FJT)Taniguchi modify CR-000XX RF共用時のハソ変更
* @date 2011/03/07 FJT)Koshida modify CeNB-F-072-018(DCM)BRE(RF共用)装置関連仕様について
* @date 2011/07/21 FJT)Tokunaga M-S3G-eNBPF-04059:[IT3]REリモート閉塞時の(3003)カード状態監視開始マクロメール通知について
* @date 2011/10/11 FFCS)Linlj  modify for 省電力対応
* @date 2012/02/29 FJT)Tokunaga modify for M-S3G-eNBPF-04109
* @date 2012/07/04 FFCS)Xut modify for CeNB-F-083-028
* @date 2012/10/18 FFCS)niemsh modify for M-S3G-eNBSYS-02684
* @date 2013/10/10 FJT)Matsumoto 13B-03-005 13B問題対応(TRA)
* @date 2015/04/16 FFCS)yuzhh modify for TDD-SRE
* @date 2015/11/27 TDI)satou IT3問処番号No.92
*/
VOID m_rc_TraStaChgNtc( T_API_SVP_CARDSTACHG_NTC* stainf_p )
{
    USHORT sys_idx;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif
	UINT	a_count		= CMD_NUM0;			/* ループカウント用変数			*/
	T_RRH_FHM_STATE		ltFhmState;
#ifdef OPT_RRH_SIM
	printf("\nSV State Control table before state change:\n");
	m_rc_dbg();

	printf("-------- RCT receives TRA state change notice ----------\n");
	printf("State change number = %d\n", stainf_p->cardSta.chgNum);
	for(a_count = CMD_NUM0; a_count < stainf_p->cardSta.chgNum; a_count++)
	{
		printf("ChangeType[%d] = %d, ChangeState[%d] = %d\n", a_count, stainf_p->cardSta.staChgInfo[a_count].chgType, a_count,
				stainf_p->cardSta.staChgInfo[a_count].chgSta);
	}
	printf("Card Control Item = %04x\n", stainf_p->cardSta.cardCntItem);
	printf("Card EXT Control Item = %04x\n", stainf_p->cardSta.cardExtItem);
	printf("Tx LTE Carrier State = %04x\n", stainf_p->cardSta.carrOnOff);
	printf("Alarm bit information = %04x\n", stainf_p->cardSta.almInfo.almFactor);
	printf("Soft alarm = %04x\n", stainf_p->cardSta.almInfo.softAlm);
	printf("PLL alarm = %04x\n", stainf_p->cardSta.almInfo.pllAlm);
	printf("APD COM alarm = %04x\n", stainf_p->cardSta.almInfo.apdComAlm);
#endif
	if(cmw_tra_sv_tbl.en_flg == CMD_FLG_OFF)
	{
		cmw_tra_sv_tbl.en_flg = CMD_FLG_ON;

		for(a_count = CMD_NUM0; a_count < stainf_p->cardSta.chgNum; a_count++)
		{
			/* Check INS state */
			if(stainf_p->cardSta.staChgInfo[a_count].chgType == D_API_SVP_STACHG_INS)
			{
				cmw_tra_sv_tbl.sv_inf[CMD_NUM0] |= D_RRH_INS_BIT;
			}
		}
	}

	cmw_tra_sv_tbl.carr_onoff = stainf_p->cardSta.carrOnOff;

//	if(stainf_p->cardSta.chgNum)
//	{
//		oldErrInfo = cmw_tra_sv_tbl.sv_inf[CMD_NUM4];
//		cmw_tra_sv_tbl.sv_inf[CMD_NUM3] = stainf_p->cardSta.errInfo.hardErr;
//		cmw_tra_sv_tbl.sv_inf[CMD_NUM4] = stainf_p->cardSta.errInfo.softErr;
//	}

	for(a_count = CMD_NUM0; a_count < stainf_p->cardSta.chgNum; a_count++)
	{
		/* If change type is ALM */
		if(stainf_p->cardSta.staChgInfo[a_count].chgType == D_API_SVP_STACHG_ALM)
		{
			/* Alarm code convertion  from sv alm bit convertion to alm code */
			m_rc_cm_AlmCodeCnv(stainf_p->cardSta.almInfo);
			
		    /* update ALM */
		    cmw_tra_sv_tbl.sv_inf[CMD_NUM0] |= D_RRH_ALM_BIT_ON;

		    /* カードALM(SOFTALM2以外がON)時には、ACT/USE/ERRは必ずOFFする */
		    if (((stainf_p->cardSta.almInfo.almFactor) & RCD_CARDALM_BIT) != 0)
		    {
		        cmw_tra_sv_tbl.sv_inf[CMD_NUM0] &= ~D_RRH_ACT_BIT_ON;
		        cmw_tra_sv_tbl.sv_inf[CMD_NUM0] &= ~D_RRH_USE_BIT_ON;
		        cmw_tra_sv_tbl.sv_inf[CMD_NUM0] &= ~D_RRH_ERR_BIT_ON;
		    }

            m_rc_sub_multiAlmProc();
		}
		
		/* If change type is ACT  */
		else if(stainf_p->cardSta.staChgInfo[a_count].chgType == D_API_SVP_STACHG_ACT)
		{
			if(stainf_p->cardSta.staChgInfo[a_count].chgSta == CMD_ON)
				m_rc_sub_actOnProc();
			else 
				m_rc_sub_actOffProc();
		}
		
		/* If change type is USE */
		else if(stainf_p->cardSta.staChgInfo[a_count].chgType == D_API_SVP_STACHG_USE)
		{
			if(stainf_p->cardSta.staChgInfo[a_count].chgSta == CMD_ON)
			{
				m_rc_sub_useOnProc();
			}
			else
			{
				m_rc_sub_useOffProc();	
			}
		}
		/* If change type is ERR */
		else if(stainf_p->cardSta.staChgInfo[a_count].chgType == D_API_SVP_STACHG_ERR)
		{
			if(stainf_p->cardSta.staChgInfo[a_count].chgSta == CMD_ON)
			{
				/* ERR ON process */
				m_rc_sub_errOnProc(stainf_p);
			}
			else
			{
				/* ERR OFF process */
				m_rc_sub_errOffProc();
			}
		}
		/*M-RRU-ZSYS-01601 */
		/* TRX Setting or Release with card USE state unchanged  */
		else if(stainf_p->cardSta.staChgInfo[a_count].chgType == D_API_SVP_STACHG_CARR)
		{
			if(stainf_p->cardSta.staChgInfo[a_count].chgSta == CMD_ON)
			{
				cmw_cardcnt_tbl.cnt_itm_s3g		= CMD_NUM0;
				cmw_cardcnt_tbl.cnt_extitem_s3g	= CMD_NUM0;
			}
		}
	}

	f_cmn_com_fhm_state_get(&ltFhmState);
	ltFhmState.FhmBase = (cmw_tra_sv_tbl.sv_inf[CMD_NUM0] << 8)  | ((cmw_tra_sv_tbl.sv_inf[CMD_NUM0] & 0xFF00) >> 8);
	ltFhmState.alm_num = cmw_tra_sv_tbl.alm_num;
	ltFhmState.err_num = cmw_tra_sv_tbl.err_num;
	cm_MemCpy( &ltFhmState.alm_inf[0], &cmw_tra_sv_tbl.alm_inf[0], sizeof(cmw_tra_sv_tbl.alm_inf[0]) * cmw_tra_sv_tbl.alm_num );
	cm_MemCpy( &ltFhmState.err_inf[0], &cmw_tra_sv_tbl.err_inf[0], sizeof(cmw_tra_sv_tbl.err_inf[0]) * cmw_tra_sv_tbl.err_num );

	for (sys_idx = 0; sys_idx < CMD_MAX_SYSNUM; sys_idx++)
	{
        ltFhmState.sys[sys_idx].FhmBase = ltFhmState.FhmBase;
        if (0 == cmw_tra_sv_tbl.sys[sys_idx].alm_num)
        {
            ltFhmState.sys[sys_idx].FhmBase &= ~CMD_CARDSV_ALM_BIT;
        }
        if (0 == cmw_tra_sv_tbl.sys[sys_idx].err_num)
        {
            ltFhmState.sys[sys_idx].FhmBase &= ~CMD_CARDSV_ERR_BIT;
        }
        ltFhmState.sys[sys_idx].alm_num = cmw_tra_sv_tbl.sys[sys_idx].alm_num;
        ltFhmState.sys[sys_idx].err_num = cmw_tra_sv_tbl.sys[sys_idx].err_num;
        cm_MemCpy(&ltFhmState.sys[sys_idx].alm_inf[0], &cmw_tra_sv_tbl.sys[sys_idx].alm_inf[0], sizeof(USHORT) * cmw_tra_sv_tbl.sys[sys_idx].alm_num);
        cm_MemCpy(&ltFhmState.sys[sys_idx].err_inf[0], &cmw_tra_sv_tbl.sys[sys_idx].err_inf[0], sizeof(USHORT) * cmw_tra_sv_tbl.sys[sys_idx].err_num);
	}

	/* 共用メモリFHM状態の呼び出し 設定用 */
	f_cmn_com_fhm_state_set(&ltFhmState);

    /* LED update */
    m_cm_WriteLEDReg();

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s FhmBase: 0x%x\n ", __LINE__, __FUNCTION__, ltFhmState.FhmBase);
#endif
#if 0 //TODO 障害監視の仕様提示待ち
	m_rc_ReCarStaRep_Renew(&rcw_carsts_tbl);
#endif

#ifdef OPT_RRH_SIM
	printf("\nSV State Control table after state change:\n");
	m_rc_dbg();
#endif
	
	return ;
}
/* @} */

/**
* @brief  have hard err or not
* @return RCD_HAVE_ERR means have err, RCD_NOT_ERR no err
* @date 2015/05/31 FFCS)create zhangxq  for TDD
*/
USHORT m_rc_HaveHardErrChk()
{
	USHORT hardErr = CMD_NUM0;

#if 0 //TODO 障害監視の仕様提示待ち
	switch(cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num)
	{
		case CMD_DEV_TYPE_NT2R:
			hardErr = cmw_tra_sv_tbl.sv_inf[CMD_NUM3] & ~(D_RRH_PAOFF_BR2|D_RRH_PAOFF_BR3);
			break;
		case CMD_DEV_TYPE_NT4R:
			hardErr = cmw_tra_sv_tbl.sv_inf[CMD_NUM3];
			break;

		case CMD_DEV_TYPE_NT8R:
			hardErr = cmw_tra_sv_tbl.sv_inf[CMD_NUM3];
			break;
			
		default:
		/* 0 and 1 and 2 and 3*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,
						"tx_sys_num NG %d",
						cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num);
			
			break;
	}
#endif

	/*No hard err*/
	if(hardErr == CMD_NUM0)
	{
		return RCD_NOT_ERR;
	}
	/*have hard err*/
	{
		return RCD_HAVE_ERR;
	}

}
/* @} */


/**
* @brief  have hard err or not
* @return RCD_HAVE_ERR means have err, RCD_NOT_ERR no err
* @date 2015/05/31 FFCS)create zhangxq  for TDD
*/
USHORT m_rc_HaveSoftErrChk()
{
	USHORT softErr = CMD_NUM0;

#if 0 //TODO 障害監視の仕様提示待ち
	switch(cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num)
	{
		case CMD_DEV_TYPE_NT2R:
			/*brach#2 branch#3 err need mask*/
			softErr = cmw_tra_sv_tbl.sv_inf[CMD_NUM4] & ~(CMD_SLP2_3_ERR_BIT);
			break;
		case CMD_DEV_TYPE_NT4R:
			softErr = cmw_tra_sv_tbl.sv_inf[CMD_NUM4];
			break;

		case CMD_DEV_TYPE_NT8R:
			softErr = cmw_tra_sv_tbl.sv_inf[CMD_NUM4];
			break;
			
		default:
		/* 0 and 1 and 2 and 3*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,
						"tx_sys_num NG %d",
						cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num);
			
			break;
	}
#endif

	/*No hard err*/
	if(softErr == CMD_NUM0)
	{
		return RCD_NOT_ERR;
	}
	/*have hard err*/
	{
		return RCD_HAVE_ERR;
	}
	
}
/* @} */

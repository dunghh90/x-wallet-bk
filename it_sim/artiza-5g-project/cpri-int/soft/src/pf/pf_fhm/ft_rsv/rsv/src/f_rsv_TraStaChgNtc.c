/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_rsv_TraStaChgNtc.c
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
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */

#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/

/**
* @brief TRA card status change sub handle function for ACT on case
* @TBD_No  f_com_SVCtlを呼び出す時引数未確認の為コメントアウト
* @note Perform ACT on case.\n
*
* @return None
* @date 2013/12/05 FFCS)Xut create
* @date 2015/11/27 TDI)satou IT3問処番号No.92
* @warning	N/A
* @FeatureID	PF-Svp-005-003-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_AlmInfChk()
{
    UINT almFlag = D_SYS_OFF;
    UINT logFlg = D_SYS_OFF;
    UINT hardAlm;
    UINT oldalm;

	/* Get hard alarm factor for f_com_alarmHitoryLogSet  */
	hardAlm = ( UINT ) ( (((UINT)tra_sv_newsta[D_SYS_NUM3]) << 16) | tra_sv_newsta[D_SYS_NUM2] );
	oldalm  = ( UINT ) ( (((UINT)tra_sv_oldsta[D_SYS_NUM3]) << 16) | tra_sv_oldsta[D_SYS_NUM2] );
	
	/* If PLL ALM is detected */
	if (((tra_sv_newsta[D_SYS_NUM3] & (D_RRH_PLLALM_BIT >> 16)) != D_SYS_NUM0)
	&&  ((tra_sv_newsta[D_SYS_NUM1] & D_RSV_MAJALM_PLL_BIT    ) == D_SYS_NUM0))
	{
		/* Set PLL ALM bit on in SV4 info */
		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_PLL_ALM, D_SYS_ON);
		almFlag = D_SYS_ON;
	}
	/* If CPRI ALM is detected */
	if (((tra_sv_newsta[D_SYS_NUM3] & (D_RRH_CPRIALM_BIT >> 16)) != D_SYS_NUM0)
	&&  ((tra_sv_newsta[D_SYS_NUM1] & D_RSV_MAJALM_CPRI_BIT    ) == D_SYS_NUM0))
	{
	    /* Set CPRI ALM bit on in SV4 info */
		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_CPRI_ALM, D_SYS_ON);
		almFlag = D_SYS_ON;
	}
    /* If APD COM ALM is detected */
    if (((hardAlm                    & D_RRH_COMALM_BIT       ) != D_SYS_NUM0)
    &&  ((tra_sv_newsta[D_SYS_NUM1]  & D_RSV_MAJALM_APDCOM_BIT) == D_SYS_NUM0))
    {
        /* Set APD COM ALM bit on in SV4 info */
        f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_COM_ALM, D_SYS_ON);
        almFlag = D_SYS_ON;
    }
	/* リソースALM検出 */
	if (((tra_sv_newsta[D_SYS_NUM1 ] & D_RSV_SOFTALM2_BIT    ) == D_SYS_NUM0)
	&&   (tra_sv_newsta[D_SYS_NUM13] & D_RSV_RESOURCE_ALM_BIT) != D_SYS_NUM0)
	{
	    f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_SOFT_ALM2, D_SYS_ON);
	    almFlag = D_SYS_ON;
	}

	/* If Soft ALM is detected */
	if (((tra_sv_newsta[D_SYS_NUM1]  & D_RSV_SOFTALM_BIT) == D_SYS_NUM0)
	&&  ((tra_sv_newsta[D_SYS_NUM12] & 0xFF             ) != D_SYS_NUM0))
	{
        /* Set Soft ALM bit on in SV1 info */
        f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_SOFT_ALM, D_SYS_ON );
        almFlag     = D_SYS_ON;
	}

	/* No ALM -> ALM */
	if((almFlag == D_SYS_ON) && (( tra_sv_newsta[D_SYS_NUM0] & D_RRH_ALM_BIT_ON ) == D_SYS_NUM0))
	{
		/* Set SV-A0(ALM bit)  to ON */						
		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_ALM, D_SYS_ON);
	}

	/* Mask PLL and CPRI alarm which would cause reset */
	hardAlm &= ~( D_RRH_PLLALM_BIT | D_RRH_CPRIALM_BIT |D_RRH_ALMBIT_CPLDIF1 );
	oldalm  &= ~( D_RRH_PLLALM_BIT | D_RRH_CPRIALM_BIT |D_RRH_ALMBIT_CPLDIF1 );
	/* COM alarm */
	if(hardAlm != oldalm )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL,
				"ALM HISTORY LOG GET by COM ALM o:%08X -> n:%08X",oldalm,hardAlm);
		logFlg = D_SYS_ON;
	}

	if(logFlg == D_SYS_ON )
	{
		f_com_alarmHitoryLogSet(D_RRH_ALARM_FACTOR_TYPE_HW, (UINT)tra_sv_newsta[D_SYS_NUM12], &hardAlm);
	}
	return;
}

/**
* @brief Check multi Alarm state
* @note Check whether alarm occurs according to state change .\n
* 
* @return 0: OFF; 1: ON
* @date 2015/04/27 FFCS)xutong create
* @warning	N/A
* @FeatureID	RRH-012-000
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	FHMではどうなる？
*
*/
UINT f_rsv_com_multiAlmChk()
{
	UINT		a_multi_alm_flg = D_SYS_OFF;  

#if 0 //TODO
	UINT		all_br_alm_flg = D_SYS_OFF;  
	
	/* In case of nT2R */
	if(f_cmw_sys_mng_tbl[D_SYS_S3G]->tx_sys_num == D_SYS_NUM0)
	{
		/* If BR0 and BR1 are both alarm */
		if(( tra_sv_newsta[D_SYS_NUM2]  & D_RRH_MAJALM_BR_BIT_NT2R ) == D_RRH_MAJALM_BR_BIT_NT2R)
		{
			all_br_alm_flg = D_SYS_ON; 
		}
	}
	/* In case of nT4R */
	else if((f_cmw_sys_mng_tbl[D_SYS_S3G]->tx_sys_num == D_SYS_NUM1) ||
		(f_cmw_sys_mng_tbl[D_SYS_S3G]->tx_sys_num == D_SYS_NUM2) )
	{
		/* If BR0 and BR1 are both alarm */
		if(( tra_sv_newsta[D_SYS_NUM2]  & D_RRH_MAJALM_BR_BIT_NT4R ) == D_RRH_MAJALM_BR_BIT_NT4R)
		{
			all_br_alm_flg = D_SYS_ON; 
		}
	}
	else
	{
		f_com_assert(f_cmw_sys_mng_tbl[D_SYS_S3G]->tx_sys_num, 	(UCHAR*)"tx system num ABL");
	}
	/* Check whether PLL, CPRI, COM, soft alarm occurs, or all BRs are in alarm state */
	if (((tra_sv_newsta[D_SYS_NUM2] & (D_RRH_HDALM_BIT |D_RRH_SOFTALM_BIT )) != D_SYS_NUM0)  ||
		(all_br_alm_flg == D_SYS_ON))
	{
		a_multi_alm_flg = D_SYS_ON;  
	}
#endif

	return a_multi_alm_flg;
}

/**
* @brief TRA card status change sub handle function for ACT on case
* @note Perform ACT on case.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_sub_actOnProc()
{
	/* if TRA card first time ACT  */
	if ( f_rsvw_first_act_flg == D_SYS_OFF)
	{		
		f_rsvw_first_act_flg  = D_SYS_ON;
	}

	return;
}

/**
* @brief TRA card status change sub handle function for USE on case
* @note Perform USE on case.\n
*
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_sub_useOnProc()
{
	/* 
	since ACT task only set BR#0/1 use, RSV need set
	SV-A0(USE bit) to ON
	*/
	f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_USE, D_SYS_ON);
	
	return;
}


/**
* @brief TRA card status change sub handle function for UNUSE on case
* @note Perform UNUSE on case.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2012/10/18 FFCS)niemsh modify for M-S3G-eNBSYS-02684
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_sub_useOffProc()
{
	/* 
	since ACT task only set BR#0/1 use, RSV need set
	SV-A0(USE bit) to OFF
	*/
	f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_USE,D_SYS_OFF);

	return;
}

/**
* @brief TRA card status change sub handle function for single ALM on case
* @TBD_No  f_com_SVCtlを呼び出す時引数未確認の為コメントアウト
* @note Perform  single ALM on case.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2013/10/07 FJT)Matsumoto 13B-03-005 13B–â‘è‘Î‰ž(TRA)
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_sub_singleAlmProc()
{
	UINT	assertCode;

	assertCode = ( (UINT)tra_sv_newsta[D_SYS_NUM8] ) | 
				((UINT)tra_sv_newsta[D_SYS_NUM9] << 16);

	f_com_assert(assertCode, (UCHAR*)"TRA CARD SINGLE ALM");

	/* set SV-A2 USE off*/
	if(tra_sv_newsta[D_SYS_NUM1] & D_RRH_BR0ALM_BIT)
	{
		/*USR br#0*/
//TODO 引数不明の為コメントアウト
//		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_BR0USE_C0, D_SYS_OFF);
//		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_BR0USE_C1, D_SYS_OFF);
	}
	if(tra_sv_newsta[D_SYS_NUM1] & D_RRH_BR1ALM_BIT)
	{
		/*USR br#1*/
//TODO 引数不明の為コメントアウト
//		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_BR1USE_C0, D_SYS_OFF);
//		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_BR1USE_C1, D_SYS_OFF);
	}
	if(tra_sv_newsta[D_SYS_NUM1] & D_RRH_BR2ALM_BIT)
	{
		/*USR br#2*/
//TODO 引数不明の為コメントアウト
//		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_BR2USE_C0, D_SYS_OFF);
//		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_BR2USE_C1, D_SYS_OFF);
	}
	if(tra_sv_newsta[D_SYS_NUM1] & D_RRH_BR3ALM_BIT)
	{
		/*USR br#3*/
//TODO 引数不明の為コメントアウト
//		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_BR3USE_C0, D_SYS_OFF);
//		f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_BR3USE_C1, D_SYS_OFF);
	}

	return;
}


/**
* @brief TRA card status change sub handle function for multi ALM on case
* @TBD_No  f_com_SVCtlを呼び出す時引数未確認の為コメントアウト
* @note Perform  multi ALM on case.\n
* 
* @return None
* @date 2012/06/20 FFCS)niemsh create
* @date 2012/10/18 FFCS)niemsh modify for M-S3G-eNBSYS-02684
* @date 2012/11/06 FFCS)niemsh modify for M-S3G-eNBSYS-04303
* @date 2013/10/07 FJT)Matsumoto 13B-03-005 13B–â‘è‘Î‰ž(TRA)
* @date 2015/06/22 ALP)Ueda modify for RRH-007-000
* @date 2015/11/27 TDI)satou IT3問処番号No.92
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_sub_multiAlmProc()
{
    if (0 != (tra_sv_newsta[D_SYS_NUM1] & (D_RSV_SOFTALM_BIT|D_RSV_MAJALM_PLL_BIT|D_RSV_MAJALM_CPRI_BIT|D_RSV_MAJALM_APDCOM_BIT)))
    {
        /* カードALM(SOFTALM2以外がON)時には、ACT/USE/ERRは必ずOFFする */
        f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_ACT, D_SYS_OFF);
        f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_USE, D_SYS_OFF);
        f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_ERR, D_SYS_OFF);
    }

	if ((tra_sv_newsta[D_SYS_NUM12] >= D_RRH_ALMCD_OS  )
	&&  (tra_sv_newsta[D_SYS_NUM12] <= D_RRH_ALMCD_CSTP))
	{
		/* Abort process */
		f_com_abort( D_RRH_PROCID_F_PF, tra_sv_newsta[D_SYS_NUM12] );
	}
	/* PLL UNLOCK ALM */
	else if((tra_sv_newsta[D_SYS_NUM3] & (D_RRH_ALMBIT_PLLUNLOCK >> 16)) != 0)
	{
		/* Abort process */
		f_com_abort( D_RRH_PROCID_F_PF, D_RRH_ALMCD_PLLNG);
	}
	/* FPGA-PLL-UNLOCK1 */
	else if((tra_sv_newsta[D_SYS_NUM3] & (D_RRH_ALMBIT_FPGAUNLK1 >> 16)) != 0)
	{
		/* Abort process */
		f_com_abort( D_RRH_PROCID_F_PF, D_RRH_ALMCD_FPGANG1);
	}
	/* CPLD_IF alarm */
	else if((tra_sv_newsta[D_SYS_NUM3]& (D_RRH_ALMBIT_CPLDIF1 >> 16)) != 0)
	{
		/* Abort process */
		f_com_abort( D_RRH_PROCID_F_PF, D_RRH_ALMCD_CPLDIF1);
	}
	/* TEMP_ALM */
	else if((tra_sv_newsta[D_SYS_NUM3]& (D_RRH_ALMBIT_TEMP >> 16)) != 0)
    {
        /* Abort process */
        f_com_abort( D_RRH_PROCID_F_PF, D_RRH_ALMCD_TEMP);
    }
	/* FPGA IF ALM1 */
	else if((tra_sv_newsta[D_SYS_NUM3]& (D_RRH_ALMBIT_FPGAIF1 >> 16) ) != 0)
	{
		/* Abort process */
		f_com_abort( D_RRH_PROCID_F_PF, D_RRH_ALMCD_FPGAIF1);
	}
	/* FPGA IF ALM2 */
	else if((tra_sv_newsta[D_SYS_NUM3]& (D_RRH_ALMBIT_FPGAIF2 >> 16) ) != 0)
	{
		/* Abort process */
		f_com_abort( D_RRH_PROCID_F_PF, D_RRH_ALMCD_FPGAIF2);
	}
	else
	{
		f_rsvw_restart_both_alm_flg = D_SYS_ON;
	}

	return;
}

/**
* @brief Check Alarm state
* @note Check whether alarm occurs according to state change .\n
* @param cardSta [in] The pointer of Card state
* 
* @return None
* @date 2015/01/06 FFCS)xutong create
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
UINT f_rsv_sub_almStaChk(T_API_SVP_CARDSTA_INFO* cardSta)
{
    UINT ret = D_SYS_ALLF_INT;
	
	/* ALM情報が前と違う場合 */
	if(( tra_sv_newsta[D_SYS_NUM1]  & D_RRH_ALM_BIT ) != ( tra_sv_oldsta[D_SYS_NUM1] & D_RRH_ALM_BIT ))
	{
		/* Set state change type to ALM */
		cardSta->staChgInfo[cardSta->chgNum].chgType = D_API_SVP_STACHG_ALM;

		/* State change number increment */
		cardSta->chgNum++;

		ret = cardSta->staChgInfo[cardSta->chgNum].chgSta = D_API_SVP_ALMSTA_MULTI;
	}
	else
	{
	}
	return ret;
}

/**
* @brief Check ACT state
* @note Check whether ACT state change according to state change .\n
* @param cardSta [in] The pointer of Card state
* @return None
* @date 2015/01/06 FFCS)xutong create
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
UINT f_rsv_sub_actStaChk(T_API_SVP_CARDSTA_INFO* cardSta)
{
	UINT ret = D_SYS_ALLF_INT;
	
	/* リソースALMの場合があるため、ALMビットが0かどうかの判定は削除する */
	/* f_rsvw_first_act_flgを見ているため、INSビットだけ判定する */
	/* フラグ判定の内側は1回しか通らないことになる */
	if(((( tra_sv_newsta[D_SYS_NUM0] & D_RRH_INS_BIT) != D_SYS_NUM0) && 
		( f_rsvw_first_act_flg == D_SYS_OFF)) ||
		(((tra_sv_oldsta[D_SYS_NUM0] & D_RRH_ACT_BIT ) == D_SYS_NUM0) &&
		(( tra_sv_newsta[D_SYS_NUM0] & D_RRH_ACT_BIT) != D_SYS_NUM0)))
	{
		/* Set state change type to ACT */
		cardSta->staChgInfo[cardSta->chgNum].chgType = D_API_SVP_STACHG_ACT;
		/* Set change state to ON */
		cardSta->staChgInfo[cardSta->chgNum].chgSta = D_SYS_ON;
		ret = D_SYS_ON;
		/* State change number increment */
		cardSta->chgNum++;
	}
	else if(((tra_sv_oldsta[D_SYS_NUM0] & D_RRH_ACT_BIT ) != D_SYS_NUM0) &&
		(( tra_sv_newsta[D_SYS_NUM0] & D_RRH_ACT_BIT) == D_SYS_NUM0))
	{
		/* UNSet state change type to ACT */
		cardSta->staChgInfo[cardSta->chgNum].chgType = D_API_SVP_STACHG_ACT;
		/* Set change state to Off */
		cardSta->staChgInfo[cardSta->chgNum].chgSta = D_SYS_OFF;
		ret = D_SYS_OFF;
		/* State change number increment */
		cardSta->chgNum++;
	}
	
	return ret;
}

/**
* @brief Check USE state
* @note Check whether USE state change according to state change .\n
* @param cardSta [in] The pointer of Card state
* @return None
* @date 2015/01/06 FFCS)xutong create
* @date 2015/10/23 TDI)satou FHM用に修正
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
UINT f_rsv_sub_useStaChk(T_API_SVP_CARDSTA_INFO* cardSta)
{
	UINT ret = D_SYS_ALLF_INT;
	
	/*************************/
	/* 割込要因が"USE"の場合 */
	/*************************/
	if((( tra_sv_newsta[D_SYS_NUM0] & D_RRH_INS_BIT   ) == D_RRH_INS_BIT ) &&
		((tra_sv_newsta[D_SYS_NUM1] & D_RRH_USE_BIT   ) != D_SYS_NUM0    ) &&
		((tra_sv_newsta[D_SYS_NUM0] & D_RRH_USE_BIT_ON) == D_SYS_NUM0    ))
	{
		/* Set state change type to USE */
		cardSta->staChgInfo[cardSta->chgNum].chgType = D_API_SVP_STACHG_USE;
		/* Set change state to ON */
		cardSta->staChgInfo[cardSta->chgNum].chgSta = D_SYS_ON;
		ret = D_SYS_ON;
		/* State change number increment */
		cardSta->chgNum++;
	}
	/****************************/
	/* 割込要因が"UN-USE"の場合 */
	/****************************/
	else
	if ((( tra_sv_newsta[D_SYS_NUM0] & D_RSV_INS_BIT    ) == D_RSV_INS_BIT    ) &&
		(( tra_sv_newsta[D_SYS_NUM1] & D_RRH_USE_BIT    ) == D_SYS_NUM0       ) &&
		(( tra_sv_newsta[D_SYS_NUM0] & D_RSV_USE_BIT_ON ) == D_RSV_USE_BIT_ON ))
	{
		/* Set state change type to USE */
		cardSta->staChgInfo[cardSta->chgNum].chgType = D_API_SVP_STACHG_USE;
		/* Set change state to ON */
		cardSta->staChgInfo[cardSta->chgNum].chgSta = D_SYS_OFF;
		ret = D_SYS_OFF;
		/* State change number increment */
		cardSta->chgNum++;
	}

	/* TRX Setting or Release with USE state unchanged  */
	else
	if ((( tra_sv_newsta[D_SYS_NUM0] & D_RSV_USE_BIT_ON ) == D_RSV_USE_BIT_ON ) &&
		(( tra_sv_newsta[D_SYS_NUM1] & D_RRH_USE_BIT    ) != ( tra_sv_oldsta[D_SYS_NUM1] & D_RRH_USE_BIT  ) ))
	{
		/* Set state change type to USE */
		cardSta->staChgInfo[cardSta->chgNum].chgType = D_API_SVP_STACHG_CARR;
		/* Set change state to ON */
		cardSta->staChgInfo[cardSta->chgNum].chgSta = D_SYS_ON;
		/* State change number increment */
		cardSta->chgNum++;
	}
	
	return ret;
}

/**
* @brief Check Error state
* @note Check whether Error state change according to state change .\n
* @param cardSta [in] The pointer of Card state
* @return None
* @date 2015/01/06 FFCS)xutong create
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_sub_errStaChk(T_API_SVP_CARDSTA_INFO* cardSta)
{
	if(( tra_sv_newsta[D_SYS_NUM0] & D_RSV_INS_BIT ) == D_RSV_INS_BIT)
	{
		/* ERR情報が前と違う場合 */
	    if ((tra_sv_newsta[D_SYS_NUM16] != tra_sv_oldsta[D_SYS_NUM16])
	    ||  (tra_sv_newsta[D_SYS_NUM17] != tra_sv_oldsta[D_SYS_NUM17])
	    ||  (tra_sv_newsta[D_SYS_NUM18] != tra_sv_oldsta[D_SYS_NUM18])
	    ||  (tra_sv_newsta[D_SYS_NUM19] != tra_sv_oldsta[D_SYS_NUM19])
	    ||  (tra_sv_newsta[D_SYS_NUM20] != tra_sv_oldsta[D_SYS_NUM20])
	    ||  (tra_sv_newsta[D_SYS_NUM21] != tra_sv_oldsta[D_SYS_NUM21])
	    ||  (tra_sv_newsta[D_SYS_NUM22] != tra_sv_oldsta[D_SYS_NUM22])
	    ||  (tra_sv_newsta[D_SYS_NUM23] != tra_sv_oldsta[D_SYS_NUM23])
	    ||  (tra_sv_newsta[D_SYS_NUM24] != tra_sv_oldsta[D_SYS_NUM24])
	    ||  (tra_sv_newsta[D_SYS_NUM25] != tra_sv_oldsta[D_SYS_NUM25])
	    ||  (tra_sv_newsta[D_SYS_NUM26] != tra_sv_oldsta[D_SYS_NUM26])
	    ||  (tra_sv_newsta[D_SYS_NUM27] != tra_sv_oldsta[D_SYS_NUM27])
	    ||  (tra_sv_newsta[D_SYS_NUM28] != tra_sv_oldsta[D_SYS_NUM28])
	    ||  (tra_sv_newsta[D_SYS_NUM29] != tra_sv_oldsta[D_SYS_NUM29])
	    ||  (tra_sv_newsta[D_SYS_NUM30] != tra_sv_oldsta[D_SYS_NUM30])
	    ||  (tra_sv_newsta[D_SYS_NUM31] != tra_sv_oldsta[D_SYS_NUM31])
	    ||  (tra_sv_newsta[D_SYS_NUM8] != tra_sv_oldsta[D_SYS_NUM8])
	    ||  (tra_sv_newsta[D_SYS_NUM9] != tra_sv_oldsta[D_SYS_NUM9]))
	    {
            cardSta->staChgInfo[cardSta->chgNum].chgType = D_API_SVP_STACHG_ERR;

            if ((0 == tra_sv_newsta[D_SYS_NUM16]) &&  (0 == tra_sv_newsta[D_SYS_NUM17])
            &&  (0 == tra_sv_newsta[D_SYS_NUM18]) &&  (0 == tra_sv_newsta[D_SYS_NUM19])
            &&  (0 == tra_sv_newsta[D_SYS_NUM20]) &&  (0 == tra_sv_newsta[D_SYS_NUM21])
            &&  (0 == tra_sv_newsta[D_SYS_NUM22]) &&  (0 == tra_sv_newsta[D_SYS_NUM23])
            &&  (0 == tra_sv_newsta[D_SYS_NUM24]) &&  (0 == tra_sv_newsta[D_SYS_NUM25])
            &&  (0 == tra_sv_newsta[D_SYS_NUM26]) &&  (0 == tra_sv_newsta[D_SYS_NUM27])
            &&  (0 == tra_sv_newsta[D_SYS_NUM28]) &&  (0 == tra_sv_newsta[D_SYS_NUM29])
            &&  (0 == tra_sv_newsta[D_SYS_NUM30]) &&  (0 == tra_sv_newsta[D_SYS_NUM31])
            &&  (0 == tra_sv_newsta[D_SYS_NUM8]) &&  (0 == tra_sv_newsta[D_SYS_NUM9]))
            {
                cardSta->staChgInfo[cardSta->chgNum].chgSta = D_SYS_OFF;

                /* Set ERR bit OFF */
                f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_ERR, D_SYS_OFF);
            }
            else
            {
                cardSta->staChgInfo[cardSta->chgNum].chgSta = D_SYS_ON;

                /* No ERR -> ERR Setting */
                if((tra_sv_newsta[D_SYS_NUM0] & D_RRH_ERR_BIT_ON) == D_SYS_NUM0)
                {
                    f_com_SVCtl(D_SYS_THDID_PF_RSV, E_SYS_SVCTL_ERR, D_SYS_ON);
                }
            }
            cardSta->chgNum++;
	    }
	}
	
	return;
}

/**
* @brief Send API: Card State change Notice to L3
* @note Send API: Card State change Notice to L3 .\n
* @param msgNtc [out] send message
* @return None
* @date 2015/01/06 FFCS)xutong create
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_sub_sndStaNtc(T_API_SVP_CARDSTACHG_NTC* msgNtc)
{
	UINT 	num;
	
	/* Edit and Send API: Card State Change Notification to L3-RCT */
	msgNtc->header.uiSrcPQueueID = D_RRH_PROCQUE_F_PF;
	msgNtc->header.uiSrcTQueueID = D_SYS_THDQID_PF_RSV;
	msgNtc->header.uiEventNo     = D_API_MSGID_SVP_STATE_CHG_NTC;
	msgNtc->header.uiSignalkind  = 0;
	msgNtc->header.uiDstTQueueID = 0;
	msgNtc->header.uiLength      = sizeof(T_API_SVP_CARDSTACHG_NTC);

	/* Set carrier onoff */
	msgNtc->cardSta.carrOnOff = tra_sv_newsta[D_SYS_NUM1] & D_RRH_USE_BIT;

	/* Set software alarm */
	msgNtc->cardSta.almInfo.softAlm  = tra_sv_newsta[D_SYS_NUM12];
	msgNtc->cardSta.almInfo.softAlm2 = tra_sv_newsta[D_SYS_NUM13];
	
	/* Error Information setting */
	msgNtc->cardSta.errInfo.err[ 0] = tra_sv_newsta[D_SYS_NUM16];
	msgNtc->cardSta.errInfo.err[ 1] = tra_sv_newsta[D_SYS_NUM17];
	msgNtc->cardSta.errInfo.err[ 2] = tra_sv_newsta[D_SYS_NUM18];
	msgNtc->cardSta.errInfo.err[ 3] = tra_sv_newsta[D_SYS_NUM19];
	msgNtc->cardSta.errInfo.err[ 4] = tra_sv_newsta[D_SYS_NUM20];
	msgNtc->cardSta.errInfo.err[ 5] = tra_sv_newsta[D_SYS_NUM21];
	msgNtc->cardSta.errInfo.err[ 6] = tra_sv_newsta[D_SYS_NUM22];
	msgNtc->cardSta.errInfo.err[ 7] = tra_sv_newsta[D_SYS_NUM23];
	msgNtc->cardSta.errInfo.err[ 8] = tra_sv_newsta[D_SYS_NUM24];
	msgNtc->cardSta.errInfo.err[ 9] = tra_sv_newsta[D_SYS_NUM25];
	msgNtc->cardSta.errInfo.err[10] = tra_sv_newsta[D_SYS_NUM26];
	msgNtc->cardSta.errInfo.err[11] = tra_sv_newsta[D_SYS_NUM27];
	msgNtc->cardSta.errInfo.err[12] = tra_sv_newsta[D_SYS_NUM28];
	msgNtc->cardSta.errInfo.err[13] = tra_sv_newsta[D_SYS_NUM29];
	msgNtc->cardSta.errInfo.err[14] = tra_sv_newsta[D_SYS_NUM30];
    msgNtc->cardSta.errInfo.err[15] = tra_sv_newsta[D_SYS_NUM31];
    msgNtc->cardSta.errInfo.err[16] = tra_sv_newsta[D_SYS_NUM8];
    msgNtc->cardSta.errInfo.err[17] = tra_sv_newsta[D_SYS_NUM9];

	/* Send message to the requested Process */
	for(num = D_SYS_NUM0; num < sizeof(f_rsvw_cardStaChgNtc_QID) / sizeof(UINT);num++)
	{
		if(f_rsvw_cardStaChgNtc_QID[ num] != D_SYS_NUM0)
		{
			msgNtc->header.uiDstPQueueID	= f_rsvw_cardStaChgNtc_QID[ num];
			/* Call common function to send API message */
			f_com_sendMsgToAplFHM(f_rsvw_cardStaChgNtc_QID[ num] , msgNtc,sizeof(T_API_SVP_CARDSTACHG_NTC),
						D_API_MSGID_SVP_STATE_CHG_NTC, D_SYS_THDQID_PF_RSV);
		}
	}
	
	return;
}

/**
* @brief Set SV register access log
* @note Set SV register access log according to state change .\n
* @param sv_inf    [in] The pointer of SV state in received message
* @return None
* @date 2015/01/06 FFCS)xutong create
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_svLogSet(UINT* sv_inf)
{
	USHORT	loopcnt			= D_SYS_NUM0;	
	
	/* Check whether SV state log table is set */
	for(loopcnt = D_SYS_NUM0; loopcnt < D_RRH_SV_INF_NUM; loopcnt++)
	{
		if(f_comw_svStateLog.svState[loopcnt] != D_SYS_ALLF_INT) 
		{
			if(f_comw_svStateLog.svState[loopcnt] != sv_inf[loopcnt])
			{
				break;
			}
		}
	}
	/* If SV state log table is set */
	if(loopcnt < D_RRH_SV_INF_NUM)
	{
		/* Set SV register access log */
		f_com_SVLogSet(D_SYS_THDID_PF_RSV);
	}
	return;
}

/* @{ */
/**
* @brief TRA card status change interruption handle function
* @TBD_No  BPF_RM_SVRM_SIGNAL_SENDを呼び出す時引数未確認の為コメントアウト
* @note TRA card status change factor judge,table update, and status change handle.\n
* @param a_trainf_p [in] the buffer address pointer of received message
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
* @date 2014/12/18 FFCS)Xut modify for ppcEOL
* @date 2015/07/28 TDI)satou PA ON/OFFの割り込み処理を削除
* @date 2015/07/29 TDI)satou BLKの割り込み処理を削除
* @date 2015/07/29 TDI)satou 「sleep state check」処理を削除
* @date 2015/08/27 TDI)satou 「f_rsvw_remotecnt_flg」設定を削除(設定のみで参照していない)
* @date 2015/09/04 TDI)satou TST State Check(診断チェック)を削除(FHMにカード診断機能は存在しない)
* @date 2016/11/22 FJT)ohashi 16B スロット数制限機能追加
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_TraStaChgNtc( T_SYS_TRA_IRQNTC *a_trainf_p )
{
    T_API_SVP_CARDSTACHG_NTC msgNtc;
    VOID *sv_inf;
    USHORT loopcnt = D_SYS_NUM0;        /* ループカウンタ       */
    UINT a_count   = D_SYS_NUM0;        /* ループカウント用変数 */
    UINT ret       = D_SYS_ALLF_INT;
	
	if( a_trainf_p == NULL)
	{
		return;
	}
	/************************************************************************/
	/* 初期化																*/
	/************************************************************************/
	memset( &tra_sv_oldsta, 0, sizeof( tra_sv_oldsta ));
	memset( &msgNtc, 0, sizeof( msgNtc ));
	
	/************************************************************************/
	/* 割込要因検出															*/
	/************************************************************************/
	for ( a_count = D_SYS_NUM0; a_count < D_RSV_SVTBL_NUM; a_count++ )
	{
		tra_sv_oldsta[a_count] = tra_sv_newsta[a_count];
	}
	/* Set tra_sv_newsta[D_SYS_NUM0] ~ tra_sv_newsta[D_SYS_NUM4] */
	for( a_count = D_SYS_NUM0 ; a_count < (D_RSV_SVTBL_NUM/2) ; a_count++ )
	{
		tra_sv_newsta[ D_SYS_NUM2 * a_count    ] = (USHORT)(  a_trainf_p->sv_inf[a_count]                 );
		tra_sv_newsta[(D_SYS_NUM2 * a_count) +1] = (USHORT)(( a_trainf_p->sv_inf[a_count] ) >> D_SYS_NUM16);
	}

	tra_sv_newsta[16] = tra_sv_newsta[16] & (~0x0040);
	tra_sv_newsta[22] = 0;
//	tra_sv_newsta[23] = 0;
//	for ( a_count = D_SYS_NUM0; a_count < D_RSV_SVTBL_NUM; a_count++ )
//	{
//		tra_sv_newsta[a_count] = tra_sv_newsta[a_count] &  f_rsvr_tra_msk_tbl[a_count] ;
//	}

	/****************************/
	/* SV状態変化履歴ログ取得   */
	/****************************/
	/*  Control code = IRQ */
	f_comw_svStateLog.chgType = D_SYS_SVSTATE_IRQ;
	/* Set SV State log data */
	sv_inf = (void*)(a_trainf_p->sv_inf);
	memcpy(f_comw_svStateLog.svState, sv_inf, sizeof(UINT)*D_RRH_SV_INF_NUM );
	/* SV状態変化履歴設定 (制御コード= IRQ) */
	f_com_SVLogSet(D_SYS_THDID_PF_RSV);
	
#ifdef OPT_RRH_SIM
	f_rsv_dbg();
#endif

	/*  Control code = CTL */
	f_comw_svStateLog.chgType = D_SYS_SVSTATE_CTL;
	/* Initialize SV state log table */
	for(loopcnt = D_SYS_NUM0; loopcnt < D_RRH_SV_INF_NUM; loopcnt++)
	{
		f_comw_svStateLog.svState[loopcnt] = D_SYS_ALLF_INT;
	}

	/* Check whether alarm occurs */
	f_rsv_AlmInfChk();

	/*************************/
	/* 割込要因が"ALM"の場合 */
	/*************************/
	if(( tra_sv_newsta[D_SYS_NUM1] & D_RRH_ALM_BIT ) != 0 )
	{
	    /* set alarm factor */
		msgNtc.cardSta.almInfo.almFactor = tra_sv_newsta[D_SYS_NUM1];

		/* Get alarm information */
		f_rsv_cm_GetAlmInf(tra_sv_newsta[D_SYS_NUM1] , &(msgNtc.cardSta.almInfo.softAlm));

        /* 前回から変化があるかチェック */
		ret = f_rsv_sub_almStaChk(&(msgNtc.cardSta));
		if (D_SYS_ALLF_INT != ret)
		{
            f_rsv_sub_multiAlmProc();

            /* rsc send signal to init for log get */
            BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_INI, SIGUSR1, D_RRH_SIGUSR_OPT_RESORCE_ALM_NTC );

            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL,
                "SV01-%08x,SV02-%08x,SV03-%08x,SV04-%08x,SV05-%08x,SV06-%08x,SV07-%08x,SV08-%08x",
                (tra_sv_newsta[D_SYS_NUM0 ]) | (tra_sv_newsta[D_SYS_NUM1 ] << 16),
                (tra_sv_newsta[D_SYS_NUM2 ]) | (tra_sv_newsta[D_SYS_NUM3 ] << 16),
                (tra_sv_newsta[D_SYS_NUM4 ]) | (tra_sv_newsta[D_SYS_NUM5 ] << 16),
                (tra_sv_newsta[D_SYS_NUM6 ]) | (tra_sv_newsta[D_SYS_NUM7 ] << 16),
                (tra_sv_newsta[D_SYS_NUM8 ]) | (tra_sv_newsta[D_SYS_NUM9 ] << 16),
                (tra_sv_newsta[D_SYS_NUM10]) | (tra_sv_newsta[D_SYS_NUM11] << 16),
                (tra_sv_newsta[D_SYS_NUM12]) | (tra_sv_newsta[D_SYS_NUM13] << 16),
                (tra_sv_newsta[D_SYS_NUM14]) | (tra_sv_newsta[D_SYS_NUM15] << 16)
            );
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL,
                "SV09-%08x,SV10-%08x,SV11-%08x,SV12-%08x,SV13-%08x,SV14-%08x,SV15-%08x,SV16-%08x (ALM DETECT)",
                (tra_sv_newsta[D_SYS_NUM16]) | (tra_sv_newsta[D_SYS_NUM17] << 16),
                (tra_sv_newsta[D_SYS_NUM18]) | (tra_sv_newsta[D_SYS_NUM19] << 16),
                (tra_sv_newsta[D_SYS_NUM20]) | (tra_sv_newsta[D_SYS_NUM21] << 16),
                (tra_sv_newsta[D_SYS_NUM22]) | (tra_sv_newsta[D_SYS_NUM23] << 16),
                (tra_sv_newsta[D_SYS_NUM24]) | (tra_sv_newsta[D_SYS_NUM25] << 16),
                (tra_sv_newsta[D_SYS_NUM26]) | (tra_sv_newsta[D_SYS_NUM27] << 16),
                (tra_sv_newsta[D_SYS_NUM28]) | (tra_sv_newsta[D_SYS_NUM29] << 16),
                (tra_sv_newsta[D_SYS_NUM30]) | (tra_sv_newsta[D_SYS_NUM31] << 16)
            );
		}
	}
	
	/* If RSV task state is not USE state, only handle card status is ALARM case */
	if(f_rsvw_thrdstate != D_RSV_TSK_STATE_USE )
	{
		/* Check whether SV state log table is set */
		for(loopcnt = D_SYS_NUM0; loopcnt < D_RRH_SV_INF_NUM; loopcnt++)
		{
			if(f_comw_svStateLog.svState[loopcnt] != D_SYS_ALLF_INT) 
			{
				break;
			}
		}
		/* If SV state log table is set */
		if(loopcnt < D_RRH_SV_INF_NUM)
		{
			/****************************/
			/* SV状態変化履歴ログ取得   */
			/****************************/
			f_com_SVLogSet(D_SYS_THDID_PF_RSV);
		}
		return;
	}

	/*************************/
	/* 割込要因が"ACT"の場合 */
	/*************************/
	ret = f_rsv_sub_actStaChk(&(msgNtc.cardSta));
	/* If ACT state change to ON */
	if(ret == D_SYS_ON)
	{
		/* ACT ON process */
		f_rsv_sub_actOnProc();
	}
	
	/********************************/
	/* 割込要因が"USE"の場合        */
	/********************************/
	ret = f_rsv_sub_useStaChk(&(msgNtc.cardSta));
	/* If state change is ON */
	if(ret == D_SYS_ON)
	{
		f_rsv_sub_useOnProc();
	}
	else if(ret == D_SYS_OFF)
	{
		f_rsv_sub_useOffProc();
	}
	else
	{
	}
	
	/***********************************/
	/* 割込要因が"ERR","ERR解除"の場合 */
	/***********************************/
	f_rsv_sub_errStaChk(&(msgNtc.cardSta));

	/***************************************************/
	/* Edit and Send Card State change Notice message  */
	/***************************************************/
	f_rsv_sub_sndStaNtc(&msgNtc);

	/***************************************************/
	/* Set SV access history Log                       */
	/***************************************************/
	f_rsv_svLogSet(a_trainf_p->sv_inf);
	
	return ;
}
/* @} */
/* @} */


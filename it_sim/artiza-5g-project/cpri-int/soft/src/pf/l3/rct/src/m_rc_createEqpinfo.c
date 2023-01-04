/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_createEqpinfo.c
 *  @brief  装置情報の集約
 *  @date   2015/08/27 TDIPS)maruyama create 立ち上がったREの情報をまとめる(m_rc_infoAllRE.cから分割）
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"
#include "m_rc_header.h"
#include "m_rc_ftyp.h"


/********************************************************************************************************************/
/**
 *  @brief  装置情報の集約
 *  
 *  
 *  
 *  @return None
 *  @note   参考ソース VOID m_rc_ReStrRep( VOID* aparm_p)
 *  @date   2015/08/07  TDIPS) maruyama create
 *  @date   2015/08/24  TDIPS) maruyama modify RE接続判定の修正
 *  @date   2015/08/27 TDIPS)maruyama create 立ち上がったREの情報をまとめる(m_rc_infoAllRE.cから分割）
 *  @date   2015/10/11 TDIPS)maruyama create 電源種別、TRX-INFタイプ(SFPモジュール)の修正
 */
/********************************************************************************************************************/

VOID m_rc_createEqpinfo()
{
	UINT		cnt			=0;				/* ループカウンタ	*/
	UINT		getInfo_flg	=CMD_TRUE;		/* 取得フラグ		*/
	UINT		existResource_flg[CMD_MAX_SYSNUM]	= {};	/* 装置情報取得有無フラグ	*/
	T_RRH_EQP_S3G	set_eqp_s3g						= {};	/* 共有メモリデータ（LTE)	*/
 	T_RRH_EQP_3G	set_eqp_3g						= {};	/* 共有メモリデータ（3G)	*/
 	USHORT		fhm_spfmod	= 0;				/* FHM SPF module */
 	UINT		ret			= 0;				/* リターン	*/



	/* 電源種別		*/
	/* initプロセスのBPF初期化でEEPROMの値からFHM装置構成の電源種別に設定されている	*/
	/*   共有メモリの電源種別を読み込む（退避させておく）*/
	f_cmn_com_booteqp_s3g_get(&set_eqp_s3g);
	f_cmn_com_booteqp_3g_get(&set_eqp_3g);

	/* TRX-INFタイプ(SFPモジュール)		*/
	ret = m_gt_FHM_spfmodule(&fhm_spfmod);

	/********************************************************/
	/* LTE側の情報											*/
	/* 参考：ldwExistResource = m_rc_GenResFromReSubLte();	*/
	/* すべてのREの情報を読みに行ってしまう					*/
	/********************************************************/
	m_rc_ReEqpStrInfInitTable_LTE();			/* 初期データを設定		*/
	/* #7 電源種別:*/
	gt_recw_ReEqpStrInfTbl_LTE.powerkind	= set_eqp_s3g.eqp_inf[6];
	/* #25 TRX-INFタイプ(SFPモジュール) */
	gt_recw_ReEqpStrInfTbl_LTE.trx_inf_type	= fhm_spfmod;

	getInfo_flg = CMD_TRUE;						/* 最初のデータ	*/
	existResource_flg[CMD_SYS_S3G] = CMD_RES_EMPTY;
	for( cnt = 1 ; cnt < D_RRH_CPRINO_NUM ; cnt++ )
	{
		if(re_startupRSP[cnt][CMD_SYS_S3G] == D_RRH_ON)
		{
			existResource_flg[CMD_SYS_S3G]=m_rc_getConnectEqpInfoLTETbl(cnt,&getInfo_flg);	/* 装置情報取得・集約	*/
			if(CMD_RES_EMPTY != existResource_flg[CMD_SYS_S3G])
			{
				getInfo_flg = CMD_FALSE;
               }
		}
	}

	if( CMD_RES_EMPTY != existResource_flg[CMD_SYS_S3G] )
	{
        gt_recw_ReEqpStrInfTbl_LTE.powerunit = RCD_PWR_UNIT_W;

		/**Update again the field that is constant*/
		/* #1メーカ名:  70 */
		gt_recw_ReEqpStrInfTbl_LTE.maker       = D_RRH_IND_MAKER_NAME;
		/* #2装置形番(BTSの形番号): 3201 */
		gt_recw_ReEqpStrInfTbl_LTE.bts_no      = CMD_BTS_SRE_NO;
		/* #3装置号番(REの番号):1 */
		gt_recw_ReEqpStrInfTbl_LTE.eqp_no      = 1;
		/* #4RE種別(3:RRE/LRE): 3 */
		gt_recw_ReEqpStrInfTbl_LTE.rekind      = 3;
		/* #6未規定項目:65535 */
		gt_recw_ReEqpStrInfTbl_LTE.dummy1      = CMD_IVL_VAL;
		/* #7 電源種別: */
		gt_recw_ReEqpStrInfTbl_LTE.powerkind	= set_eqp_s3g.eqp_inf[6];
		/* #15 未規定項目:65535 */
		gt_recw_ReEqpStrInfTbl_LTE.dummy2[0] = CMD_IVL_VAL;
		/* #16 未規定項目:65535 */
		gt_recw_ReEqpStrInfTbl_LTE.dummy2[1] =  CMD_IVL_VAL;
		/* #25 TRX-INFタイプ(SFPモジュール) */
		gt_recw_ReEqpStrInfTbl_LTE.trx_inf_type	= fhm_spfmod;
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_0 == 0)
		{
			gt_recw_ReEqpStrInfTbl_LTE.powerclass_0 = CMD_IVL_VAL;
		}
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_1 == 0)
		{
			gt_recw_ReEqpStrInfTbl_LTE.powerclass_1 = CMD_IVL_VAL;
		}
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_2 == 0)
		{
			gt_recw_ReEqpStrInfTbl_LTE.powerclass_2 = CMD_IVL_VAL;
		}
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_3 == 0)
		{
			gt_recw_ReEqpStrInfTbl_LTE.powerclass_3 = CMD_IVL_VAL;
		}
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_4 == 0)
		{
			gt_recw_ReEqpStrInfTbl_LTE.powerclass_4 = CMD_IVL_VAL;
		}
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_5 == 0)
		{
			gt_recw_ReEqpStrInfTbl_LTE.powerclass_5 = CMD_IVL_VAL;
		}
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_6 == 0)
		{
			gt_recw_ReEqpStrInfTbl_LTE.powerclass_6 = CMD_IVL_VAL;
		}
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_7 == 0)
		{
			gt_recw_ReEqpStrInfTbl_LTE.powerclass_7 = CMD_IVL_VAL;
		}
	}

	/********************************************************/
	/* 3G側の情報											*/
	/* 参考 ldwExistResource = m_rc_GenResFromReSub3g();	*/
	/********************************************************/
	m_rc_ReEqpStrInfInitTable_3G();			/* 初期データを設定	*/
	/* #11 電源種別:*/
	gt_recw_ReEqpStrInfTbl_3G.powerkind	= set_eqp_3g.eqp_inf[10];
	/* #20 TRX-INFタイプ(SFPモジュール) */
	gt_recw_ReEqpStrInfTbl_3G.trx_inf_type	= fhm_spfmod;

	getInfo_flg = CMD_TRUE;						/* 最初のデータ		*/
	existResource_flg[CMD_SYS_3G] = CMD_RES_EMPTY;
	for(cnt = 1 ; cnt < D_RRH_CPRINO_NUM ; cnt++)
	{
		if(re_startupRSP[cnt][CMD_SYS_3G] == D_RRH_ON)
		{
			existResource_flg[CMD_SYS_3G] = m_rc_getConnectEqpInfo3gTbl(cnt,&getInfo_flg);	/* 装置情報取得・集約	*/
			if(CMD_RES_EMPTY != existResource_flg[CMD_SYS_3G])
			{
				getInfo_flg = CMD_FALSE;
			}
		}
	}

	if( CMD_RES_EMPTY != existResource_flg[CMD_SYS_3G] )
	{
        gt_recw_ReEqpStrInfTbl_3G.powerunit = RCD_PWR_UNIT_W;

		/**Update again the field that is constant*/
		/* #1メーカ名:  70 */
		gt_recw_ReEqpStrInfTbl_3G.maker       =   D_RRH_IND_MAKER_NAME;
		/* #2BTS形番(BTSの形番号): 3001 */
		gt_recw_ReEqpStrInfTbl_3G.bts_no      =   CMD_BTS_NO;
		/* #3RE種別(2:RRE・LRE/OF-TRX):2 */
		gt_recw_ReEqpStrInfTbl_3G.rekind      =   2;
		/* #4装置号番:1 */
//TODO 設定値が不明
// 配下REの中で異なる値をもつ装置がある場合、FHMのメーカ製RREと同じ
		gt_recw_ReEqpStrInfTbl_3G.eqp_no      =   1;
		/* #5未規定項目:65535 */
		gt_recw_ReEqpStrInfTbl_3G.dummy1[0]   = CMD_IVL_VAL;
		/* #6未規定項目:65535 */
		gt_recw_ReEqpStrInfTbl_3G.dummy1[1]   = CMD_IVL_VAL;
		/* #7未規定項目:65535 */
		gt_recw_ReEqpStrInfTbl_3G.dummy1[2]   = CMD_IVL_VAL;
		/* #8未規定項目:65535 */
		gt_recw_ReEqpStrInfTbl_3G.dummy1[3]   = CMD_IVL_VAL;
		/* #9未規定項目:65535 */
		gt_recw_ReEqpStrInfTbl_3G.dummy1[4]   = CMD_IVL_VAL;
		/* #10未規定項目:65535 */
		gt_recw_ReEqpStrInfTbl_3G.dummy1[5]   = CMD_IVL_VAL;
		/* #11 電源種別: */
		gt_recw_ReEqpStrInfTbl_3G.powerkind	= set_eqp_3g.eqp_inf[10];
		/* #20 TRX-INFタイプ(SFPモジュール) */
		gt_recw_ReEqpStrInfTbl_3G.trx_inf_type	= fhm_spfmod;
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_3G.powerclass_0 == 0)
		{
			gt_recw_ReEqpStrInfTbl_3G.powerclass_0 = CMD_IVL_VAL;
		}
		/*	パワークラスの最大値未設定の場合	*/
		if(gt_recw_ReEqpStrInfTbl_3G.powerclass_1 == 0)
		{
			gt_recw_ReEqpStrInfTbl_3G.powerclass_1 = CMD_IVL_VAL;
		}
	}

	/* 共有メモリに書き込み		*/
	/* signal_kind	0: 未受信で以降のパラメータはDon't Care、  0以外: 受信済み	*/
	/* result		0固定														*/
	/* S3G	*/
	if( CMD_RES_EMPTY != existResource_flg[CMD_SYS_S3G])
	{
		set_eqp_s3g.signal_kind = 1;
	}
	else
	{
		set_eqp_s3g.signal_kind = 0;
	}
	set_eqp_s3g.result =0;
	memcpy( &(set_eqp_s3g.eqp_inf) , &gt_recw_ReEqpStrInfTbl_LTE , sizeof(gt_recw_ReEqpStrInfTbl_LTE) );
	f_cmn_com_booteqp_s3g_set(&set_eqp_s3g);

	/* 3G	*/
	if( CMD_RES_EMPTY != existResource_flg[CMD_SYS_3G])
	{
		set_eqp_3g.signal_kind = 1;
	}
	else
	{
		set_eqp_3g.signal_kind = 0;
	}
	set_eqp_3g.result =0;
	memcpy( &(set_eqp_3g.eqp_inf) , &gt_recw_ReEqpStrInfTbl_3G , sizeof(gt_recw_ReEqpStrInfTbl_3G) );
	f_cmn_com_booteqp_3g_set(&set_eqp_3g);

}


/********************************************************************************************************************/
/**
 *  @brief  FHM_spfmodule情報
 *  @param  fhm_spf    [out]    受信メッセージへのポインタ(CMT_TSKIF_RESTR_RSP)
 *  @return None
 *  @date   2015/10/11  TDIPS) maruyama create
 *  @note   pf/ft_cpr/cprのf_cpr_CpriLinkInitProc.cでも関数内でspfmodule情報の処理を行っている
 *          現状、ヘッダファイルもpf/ft_cpr/cprをL3/rct用が別にある
 *  @note   2015/11/30  TDIPS) watanabe modify TRX-INFタイプとSFPのIDとの対応付けを修正
 */
/********************************************************************************************************************/

UINT m_gt_FHM_spfmodule( USHORT *fhm_spf )
{
	UINT	num = 0;										/* ループカウンタ	*/
	UCHAR	sfpcode[D_CPR_EEPROM_SFP_LEN+1] = {};		/* SPF モジュール	*/
	UINT 	ret = D_RRH_OK;									/* 戻り値			*/
	UINT	sit_ret = BPF_HM_DEVC_COMPLETE;					/* 戻り値			*/
	INT		errcd;
	T_RRH_SFP_LOG_ALL	*shm_sfplog;
	
    sit_ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SFPLOG ,(VOID**)&shm_sfplog, &errcd);
    if(sit_ret != BPF_RU_IPCM_OK)
    {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCSHM_ADDRGET() NG. shm_num=%d errcd=0x%08x", E_RRH_SHMID_APL_SFPLOG, errcd);
		/*	誤実装(Aタイプ)とする	*/
		*fhm_spf = 0;
        return (D_RRH_NG);
    }

	/* SPFモジュール 取得	*/
	memcpy(sfpcode, &shm_sfplog->cpr_latest[D_RRH_CPRINO_REC].log.sfpInf[0][0x60], D_CPR_EEPROM_SFP_LEN);
	sfpcode[D_CPR_EEPROM_SFP_LEN] = 0;

	/* SPFモジュールがRECであるか確認して、報告値を設定	*/
	for ( num = 0; num < D_CPR_EEPROM_SFP_MAX; num++ )
	{
		if ( strncmp((const char*)sfpcode, f_cpr_sfp[num], D_CPR_EEPROM_SFP_LEN) == 0 )
		{
			/* sfpcodeがREC用(対RE用)は誤実装 */
			if ( f_sfp_type[num] == -1 )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "sfpcode data is not rec ret=%s", sfpcode);
				*fhm_spf = 0;
				ret = D_RRH_NG;
			}

			if( ( num % 2 ) == 0 ) 			/* RE用である（現状のf_cpreeprom_sfpテーブルでは偶数のときRE用）	*/
			{
				*fhm_spf = f_sfp_type[num];
				ret = D_RRH_OK;
				break;
			}
		}
	}

	if ( num >= D_CPR_EEPROM_SFP_MAX )		/* 一致するSPFがない	*/
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "rec sfpcode not match[%s]", sfpcode);
		/*	誤実装(Aタイプ)とする	*/
		*fhm_spf = 0;
		ret = D_RRH_NG;
	}

	return( ret );
}


/* @} */

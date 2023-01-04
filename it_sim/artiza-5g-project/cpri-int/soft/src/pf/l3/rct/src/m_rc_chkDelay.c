/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_chkDelay.c
 *  @brief  遅延量算出・設定
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
 *  @brief	遅延量算出・設定
 *  @return	None
 *  @date	2015/08/07	TDIPS) maruyama create
 *  @date	2015/08/24	TDIPS) maruyama modify RE接続判定の修正、Toffset値書き込みデータを修正
 *  @date	2015/08/26	TDIPS) maruyama modify RE接続判定の修正
 *  @date	2015/09/08	TDIPS) maruyama modify RE接続判定、T12の計算式（四捨五入）、最大遅延検索、FHM_Toffset計算式の修正
 */
/********************************************************************************************************************/

VOID m_rc_chkDelay()
{

	T_RRH_RE_DELAY_INF	re_delay_inf[D_RRH_CPRINO_NUM]	= {};	/* 共有メモリからの取得データ(RE単位遅延補正情報)	*/
	T_RRH_RE_DELAY_INF	set_re_delay_inf				= {};	/* 共有メモリへの設定データ(RE単位遅延補正情報)		*/
//	T_RRH_DELAY_INF		delay_inf						= {};	/* 共有メモリからの取得データ(遅延補正情報)			*/
	T_RRH_DELAY_INF		set_delay_inf					= {};	/* 共有メモリへの設定データ(遅延補正情報)			*/
	T_RRH_TOFFSET_S3G	toffset_s3g[D_RRH_CPRINO_NUM]						= {};	/* 共有メモリからの取得データ((LTE)Toffset値報告)	*/
	T_RRH_TOFFSET_3G	toffset_3g[D_RRH_CPRINO_NUM]						= {};	/* 共有メモリからの取得データ((3G)Toffset値報告)	*/
	UINT	cnt											= 0;	/* ループカウンタ									*/
	SHORT	clc_T12[D_RRH_CPRINO_NUM]					= {};	/* T12算出値										*/
	USHORT	clc_DlDelay[CMD_MAX_SYSNUM][D_RRH_CPRINO_NUM]		= {};	/* 下り遅延量算出値									*/
	USHORT	clc_UlDelay[CMD_MAX_SYSNUM][D_RRH_CPRINO_NUM]		= {};	/* 上り遅延量算出値									*/
	USHORT	clc_T12_MAX									= 0;	/* T12最大値										*/
	USHORT	clc_T12_MaxNo								= 0;	/* T12最大値のRE番号								*/
	USHORT	DlEquipDelay_MAX[CMD_MAX_SYSNUM]			= {};	/* 下り装置遅延最大値								*/
	USHORT	UlEquipDelay_MAX[CMD_MAX_SYSNUM]			= {};	/* 下り装置遅延最大値のRE番号						*/
	USHORT	DlEquipDelay_MaxNo[CMD_MAX_SYSNUM]			= {};	/* 上り装置遅延最大値								*/
	USHORT	UlEquipDelay_MaxNo[CMD_MAX_SYSNUM]			= {};	/* 上り装置遅延最大値のRE番号						*/
	USHORT	clc_DlDelay_MAX[CMD_MAX_SYSNUM]				= {};	/* 下り遅延量最大値									*/
	USHORT	clc_UlDelay_MAX[CMD_MAX_SYSNUM]				= {};	/* 上り遅延量最大値									*/
	USHORT	clc_DlDelay_MaxNo[CMD_MAX_SYSNUM]			= {};	/* 下り遅延量最大値のRE番号							*/
	USHORT	clc_UlDelay_MaxNo[CMD_MAX_SYSNUM]			= {};	/* 上り遅延量最大値のRE番号							*/
	USHORT	DlBaseDelay[CMD_MAX_SYSNUM]					= {};	/* 下り遅延基準値									*/
	USHORT	UlBaseDelay[CMD_MAX_SYSNUM]					= {};	/* 上り遅延基準値									*/
	USHORT	DlBaseDelayAlign[CMD_MAX_SYSNUM]			= {};	/* 下り遅延基準値アライン							*/
	USHORT	UlBaseDelayAlign[CMD_MAX_SYSNUM]			= {};	/* 上り遅延基準値アライン							*/
	USHORT	clc_FHM_Toffset								= 0;	/* FHM Toffset算出値								*/
	USHORT	tmp_data									= 0;	/* 計算用仮データ									*/
	UINT	re_conn[D_RRH_CPRINO_NUM]					= {};	/* RE接続状況										*/
	DL		work_clc_d									= 0.0;	/* 計算の途中データ									*/
	
	
	INT					Calc0[D_RRH_CPRINO_NUM]	= {};			/* RoundTripDelay(レジスタから取得*/
	INT 				RTDelay_offset 			= 116;			/* Master Port Internal loop delay(ハソ仕様より)*/
	INT					REn_T14					= 0;			/* REn_T14(= Calc0 - ( RTDelay_offset x 2)) */
	DL					Toffset_REn_d			= 0.0;			/* Toffset_REn計算途中	*/
	INT					Toffset_REn[D_RRH_CPRINO_NUM]	= {};	/* Toffset_REn(Toffset_REn x 16 / 25 )*/
	DL					T12_d					= 0.0;			/* T12計算途中 */
	UINT				get_Calc 				= 0;			/* RoundTripDelay(レジスタから取得値)*/
	UINT				X_Fhm					= 896;			/* FHM 下り装置内遅延(ハソ仕様より)*/
	UINT				Y_Fhm					= 896;			/* FHM 上り装置内遅延(ハソ仕様より)*/
	UINT				ToffsetReport			= 0;			/* RECに報告するToffset値(ハソ仕様より)*/
	UINT				toffset[D_RRH_CPRINO_NUM] = {};			/* REからのToffset値*/

	for( cnt = 1 ; cnt <= 1 ; cnt++ )
	{
		if( (re_startupREQ[cnt][CMD_SYS_3G] == D_RRH_OFF ) &&
			(re_startupREQ[cnt][CMD_SYS_S3G] == D_RRH_OFF ) )
		{
			/* 3G、S3Gとも要求なし→接続していない*/
			re_conn[cnt] = D_RRH_OFF;
		}
		else if( (re_startupREQ[cnt][CMD_SYS_3G] == re_startupRSP[cnt][CMD_SYS_3G]) &&
			(re_startupREQ[cnt][CMD_SYS_LTE] == re_startupRSP[cnt][CMD_SYS_LTE]) )
		{
			/* 要求と応答が一致（要求なしを除く）→接続している	*/
			re_conn[cnt] = D_RRH_ON;
		}
		else
		{
			/* 	要求と応答が不一致→接続していない*/
			re_conn[cnt] = D_RRH_OFF;
		}
	}
	/****************************************************************************/
	/* T12、下り遅延量、上り遅延量を算出										*/
	/****************************************************************************/
	for( cnt = 1 ; cnt <= 1 ; cnt++ )
	{
		if( re_conn[cnt] != D_RRH_ON )
		{
			// 応答がないREの遅延計算はしない	*/
			continue;
		}
		
		/* RE単位のToffset(toffset),上り装置遅延(updeleqp)、下り装置遅延(downdeleqp)	*/
		f_cmn_com_toffset_s3g_get(cnt, &toffset_s3g[cnt]);
		f_cmn_com_toffset_3g_get(cnt, &toffset_3g[cnt]);

		/***************************************************************/
		/* RE単位遅延補正情報テーブルで3G/S3Gと分けていない（Toffset,T12も）*/
		/***************************************************************/
		if((toffset_3g[cnt].signal_kind == CMD_CPRID_TOFFSETINFRES+CMD_SYS_3G) &&  /* 3Gでresult=0であれば*/
			(toffset_3g[cnt].result == 0) )
		{
			toffset[cnt]=toffset_3g[cnt].toffset;
		}
		else if((toffset_s3g[cnt].signal_kind == CMD_CPRID_TOFFSETINFRES+CMD_SYS_S3G) &&  /* S3Gでresult=0であれば*/
				(toffset_s3g[cnt].result == 0) )
		{
			toffset[cnt]=toffset_s3g[cnt].toffset;
		}
		else
		{
			toffset[cnt]= 0;
		}

		/* RoundTripDelay(Calc0) 取得 */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI, M_RRH_REG_CPRTDLY(cnt), &get_Calc);
		Calc0[cnt] = get_Calc & 0x0000FFFF;

		/* REn_T14の計算*/
		/* REn_T14 = Calc0 - ( RTDelay_offset x 2)	*/
		/* REn_T14 < 0 なら REn_T14 = 0	*/
		REn_T14 = Calc0[cnt] - (RTDelay_offset * 2);
		if( REn_T14 < 0 )
		{
			REn_T14 = 0;
		}

		/* Toffset_REnの計算*/
		/* Toffset_REn ＝ Toffset_REn x 16 / 25 */
		/* Toffset_REnは四捨五入*/
		Toffset_REn_d = toffset[cnt] * 16.0 / 25.0;
		Toffset_REn[cnt] = Toffset_REn_d + 0.5;

		/* T12_REnの計算 */
		/* T12_REn = (REn_T14-Toffset_REn) / 2	*/
		/*         = (Calc0 - ( RTDelay_offset x 2) - Toffset_REn x 16 / 25) / 2 */
		T12_d = ( REn_T14 - Toffset_REn[cnt] ) / 2.0;
		clc_T12[cnt] = T12_d + 0.5;
		if( clc_T12[cnt] < 0 )
		{
			clc_T12[cnt] = 0;
		}

		/* 下り遅延量																		*/
		/* DlDelay = T12 +downdeleqp 														*/
		clc_DlDelay[CMD_SYS_3G][cnt]	= clc_T12[cnt] + ( toffset_3g[cnt].downdeleqp  );
		clc_DlDelay[CMD_SYS_S3G][cnt]	= clc_T12[cnt] + ( toffset_s3g[cnt].downdeleqp  );
	
		/* 上り遅延量																		*/
		/* UlDelay = T12 + updeleqp														*/
		clc_UlDelay[CMD_SYS_3G][cnt]	= clc_T12[cnt] + ( toffset_3g[cnt].updeleqp );
		clc_UlDelay[CMD_SYS_S3G][cnt]	= clc_T12[cnt] + ( toffset_s3g[cnt].updeleqp );
		
	}

	
	/****************************************************************************/
	/* 最大遅延値と最大遅延のRE番号を検索										*/
	/****************************************************************************/
	/* 探索する最初のデータ			*/
	/* T12			*/
	clc_T12_MAX 	= 0;
	clc_T12_MaxNo	= 1;
	
	/* 装置内遅延量	*/
	DlEquipDelay_MAX[CMD_SYS_3G]	= 0;
	DlEquipDelay_MAX[CMD_SYS_S3G]	= 0;
	UlEquipDelay_MAX[CMD_SYS_3G]	= 0;
	UlEquipDelay_MAX[CMD_SYS_S3G]	= 0;
	DlEquipDelay_MaxNo[CMD_SYS_3G]	= 1;
	DlEquipDelay_MaxNo[CMD_SYS_S3G]	= 1;
	UlEquipDelay_MaxNo[CMD_SYS_3G]	= 1;
	UlEquipDelay_MaxNo[CMD_SYS_S3G]	= 1;
	
	/* 遅延量		*/
	clc_DlDelay_MAX[CMD_SYS_3G]		= 0;
	clc_DlDelay_MAX[CMD_SYS_S3G]	= 0;
	clc_UlDelay_MAX[CMD_SYS_3G]		= 0;
	clc_UlDelay_MAX[CMD_SYS_S3G]	= 0;
	clc_DlDelay_MaxNo[CMD_SYS_3G]	= 1;
	clc_DlDelay_MaxNo[CMD_SYS_S3G]	= 1;
	clc_UlDelay_MaxNo[CMD_SYS_3G]	= 1;
	clc_UlDelay_MaxNo[CMD_SYS_S3G]	= 1;
	
	/* 最大値の探索開始				*/
	for( cnt = 1 ; cnt <= 1 ; cnt++ )
	{
		if( re_conn[cnt] != D_RRH_ON )
		{
			continue;
		}

		/* T12の最大		*/
		if( clc_T12_MAX < clc_T12[cnt] )
		{
			clc_T12_MAX		= clc_T12[cnt];
			clc_T12_MaxNo	= cnt;
		}
		/* 装置内の最大(3G DL)	*/
		if( DlEquipDelay_MAX[CMD_SYS_3G] < toffset_3g[cnt].downdeleqp )
		{
			DlEquipDelay_MAX[CMD_SYS_3G]	= toffset_3g[cnt].downdeleqp;
			DlEquipDelay_MaxNo[CMD_SYS_3G]	= cnt;
		}
		/* 装置内の最大(LTE DL)	*/
		if( DlEquipDelay_MAX[CMD_SYS_S3G] < toffset_s3g[cnt].downdeleqp )
		{
			DlEquipDelay_MAX[CMD_SYS_S3G]	= toffset_s3g[cnt].downdeleqp;
			DlEquipDelay_MaxNo[CMD_SYS_S3G]	= cnt;
		}
		/* 装置内の最大(3G UL)	*/
		if( UlEquipDelay_MAX[CMD_SYS_3G] < toffset_3g[cnt].updeleqp )
		{
			UlEquipDelay_MAX[CMD_SYS_3G]	= toffset_3g[cnt].updeleqp;
			UlEquipDelay_MaxNo[CMD_SYS_3G]	= cnt;
		}
		/* 装置内の最大(LTE UL)	*/
		if( UlEquipDelay_MAX[CMD_SYS_S3G] < toffset_s3g[cnt].updeleqp )
		{
			UlEquipDelay_MAX[CMD_SYS_S3G]	= toffset_s3g[cnt].updeleqp;
			UlEquipDelay_MaxNo[CMD_SYS_S3G]	= cnt;
		}
		/* 下りの最大(3G DL)	*/
		if( clc_DlDelay_MAX[CMD_SYS_3G] < clc_DlDelay[CMD_SYS_3G][cnt] )
		{
			clc_DlDelay_MAX[CMD_SYS_3G]		= clc_DlDelay[CMD_SYS_3G][cnt];
			clc_DlDelay_MaxNo[CMD_SYS_3G]	= cnt;
		}
		/* 下りの最大(LTE DL)	*/
		if( clc_DlDelay_MAX[CMD_SYS_S3G] < clc_DlDelay[CMD_SYS_S3G][cnt] )
		{
			clc_DlDelay_MAX[CMD_SYS_S3G]	= clc_DlDelay[CMD_SYS_S3G][cnt];
			clc_DlDelay_MaxNo[CMD_SYS_S3G]	= cnt;
		}
		/* 下りの最大(3G UL)	*/
		if( clc_UlDelay_MAX[CMD_SYS_3G] < clc_UlDelay[CMD_SYS_3G][cnt] )
		{
			clc_UlDelay_MAX[CMD_SYS_3G]		= clc_UlDelay[CMD_SYS_3G][cnt];
			clc_UlDelay_MaxNo[CMD_SYS_3G]	= cnt;
		}
		/* 下りの最大(LTE UL)	*/
		if( clc_UlDelay_MAX[CMD_SYS_S3G] < clc_UlDelay[CMD_SYS_S3G][cnt] )
		{
			clc_UlDelay_MAX[CMD_SYS_S3G]	= clc_UlDelay[CMD_SYS_S3G][cnt];
			clc_UlDelay_MaxNo[CMD_SYS_S3G]	= cnt;
		}
	}

	/****************************************************************************************/
	/* 遅延基準値																			*/
	/* T12が最大のREと、遅延量が最大のREが等しい場合は、遅延量が最大なものを基準値とする。	*/
	/* それ以外の場合は、T12の最大値と装置内遅延を加算したものを基準値とする				*/
	/****************************************************************************************/
	/* 遅延基準値(3G DL)	*/
	if( clc_DlDelay_MaxNo[CMD_SYS_3G] == clc_T12_MaxNo )
	{
		DlBaseDelay[CMD_SYS_3G] = clc_DlDelay_MAX[CMD_SYS_3G];
	}
	else
	{
		DlBaseDelay[CMD_SYS_3G] = clc_T12_MAX+DlEquipDelay_MAX[CMD_SYS_3G];
	}
	/* 遅延基準値(3G UL)	*/
	if( clc_UlDelay_MaxNo[CMD_SYS_3G] == clc_T12_MaxNo )
	{
		UlBaseDelay[CMD_SYS_3G] = clc_UlDelay_MAX[CMD_SYS_3G];
	}
	else
	{
		UlBaseDelay[CMD_SYS_3G] = clc_T12_MAX+UlEquipDelay_MAX[CMD_SYS_3G];
	}
	/* 遅延基準値(LTE DL)	*/
	if( clc_DlDelay_MaxNo[CMD_SYS_S3G] == clc_T12_MaxNo )
	{
		DlBaseDelay[CMD_SYS_S3G] = clc_DlDelay_MAX[CMD_SYS_S3G];
	}
	else
	{
		DlBaseDelay[CMD_SYS_S3G] = clc_T12_MAX+DlEquipDelay_MAX[CMD_SYS_S3G];
	}
	/* 遅延基準値(LTE UL)	*/
	if( clc_UlDelay_MaxNo[CMD_SYS_S3G] == clc_T12_MaxNo )
	{
		UlBaseDelay[CMD_SYS_S3G] = clc_UlDelay_MAX[CMD_SYS_S3G];
	}
	else
	{
		UlBaseDelay[CMD_SYS_S3G] = clc_T12_MAX+UlEquipDelay_MAX[CMD_SYS_S3G];
	}
	
	/****************************************************************************/
	/* 遅延基準アライン															*/
	/****************************************************************************/
	tmp_data = DlBaseDelay[CMD_SYS_3G] % 64;
	DlBaseDelayAlign[CMD_SYS_3G] = DlBaseDelay[CMD_SYS_3G] + (64-tmp_data);
	tmp_data = DlBaseDelay[CMD_SYS_S3G] % 64;
	DlBaseDelayAlign[CMD_SYS_S3G] = DlBaseDelay[CMD_SYS_S3G] + (64-tmp_data);

	tmp_data = UlBaseDelay[CMD_SYS_3G] % 64;
	UlBaseDelayAlign[CMD_SYS_3G] = UlBaseDelay[CMD_SYS_3G] + (64-tmp_data);
	tmp_data = UlBaseDelay[CMD_SYS_S3G] % 64;
	UlBaseDelayAlign[CMD_SYS_S3G] = UlBaseDelay[CMD_SYS_S3G] + (64-tmp_data);

	/****************************************************************************/
	/* 遅延補正																	*/
	/****************************************************************************/
	for( cnt = 1 ; cnt <= 1 ; cnt++ )
	{
		if( re_conn[cnt] != D_RRH_ON )
		{
			continue;
		}

        //DL_DelayOffset[CMD_SYS_3G][cnt]	= DlBaseDelayAlign[CMD_SYS_3G]-clc_DlDelay[CMD_SYS_3G][cnt];
        //DL_DelayOffset[CMD_SYS_S3G][cnt]	= DlBaseDelayAlign[CMD_SYS_S3G]-clc_DlDelay[CMD_SYS_S3G][cnt];
	
        //UL_DelayOffset[CMD_SYS_3G][cnt]	= UlBaseDelayAlign[CMD_SYS_3G]-clc_UlDelay[CMD_SYS_3G][cnt];
        //UL_DelayOffset[CMD_SYS_S3G][cnt]	= UlBaseDelayAlign[CMD_SYS_S3G]-clc_UlDelay[CMD_SYS_S3G][cnt];
	}

	/****************************************************************************/
	/* FHM_Toffset算出															*/
	/* HM Toffset = (MAX[T12] × 2 + X_FHM + Y_FHM + Toffset_Report) 		*/
	/****************************************************************************/
	work_clc_d = clc_T12_MAX * 2 + X_Fhm + Y_Fhm + ToffsetReport ;
	clc_FHM_Toffset = work_clc_d;


	/****************************************************************************/
	/* Toffset_Report															*/
	/* Toffset_Report = FHM_Toffset + T34 - T12 - (T12_REn + X_FHM) x 2 		*/
	/****************************************************************************/
	ToffsetReport = clc_FHM_Toffset - (clc_T12[1] + X_Fhm) * 2;
	if( ToffsetReport < 0 )
	{
		ToffsetReport = 0;
	}

	/* 共有メモリに遅延量書き込み	*/
	/* RE単位遅延補正情報	*/
	for( cnt = 1 ; cnt <= 1 ; cnt++ )
	{
		f_cmn_com_re_delay_inf_get(cnt, &re_delay_inf[cnt]);		/* 共有メモリから遅延情報取得		*/
		if( re_conn[cnt] != D_RRH_ON )
		{
			set_re_delay_inf.UseKind  = 0;
		}
		else
		{
			set_re_delay_inf.UseKind = 1;
		}
		set_re_delay_inf.RoundTrip	= Calc0[cnt];
		set_re_delay_inf.RtDelay	= RTDelay_offset;
		set_re_delay_inf.Toffset	= toffset[cnt];
		set_re_delay_inf.T12		= clc_T12[cnt];
		set_re_delay_inf.DlEquipDelay[CMD_SYS_3G]	= toffset_3g[cnt].downdeleqp;
		set_re_delay_inf.DlEquipDelay[CMD_SYS_S3G]	= toffset_s3g[cnt].downdeleqp;
		set_re_delay_inf.UlEquipDelay[CMD_SYS_3G]	= toffset_3g[cnt].updeleqp;
		set_re_delay_inf.UlEquipDelay[CMD_SYS_S3G]	= toffset_s3g[cnt].updeleqp;
		set_re_delay_inf.DlDelay[CMD_SYS_3G]		= clc_DlDelay[CMD_SYS_3G][cnt];
		set_re_delay_inf.DlDelay[CMD_SYS_S3G]		= clc_DlDelay[CMD_SYS_S3G][cnt];
		set_re_delay_inf.UlDelay[CMD_SYS_3G]		= clc_UlDelay[CMD_SYS_3G][cnt];
		set_re_delay_inf.UlDelay[CMD_SYS_S3G]		= clc_UlDelay[CMD_SYS_S3G][cnt];
		
		/* 書き込み */
		f_cmn_com_re_delay_inf_set(cnt,&set_re_delay_inf);
	}
	
	/* 遅延補正情報 */
	set_delay_inf.MaxT12Re = clc_T12_MaxNo;
	set_delay_inf.MaxDlEquipDelayReNo[CMD_SYS_3G]		= DlEquipDelay_MaxNo[CMD_SYS_3G];
	set_delay_inf.MaxDlEquipDelayReNo[CMD_SYS_S3G]	= DlEquipDelay_MaxNo[CMD_SYS_S3G];
	set_delay_inf.MaxUlEquipDelayReNo[CMD_SYS_3G]		= UlEquipDelay_MaxNo[CMD_SYS_3G];
	set_delay_inf.MaxUlEquipDelayReNo[CMD_SYS_S3G]	= UlEquipDelay_MaxNo[CMD_SYS_S3G];
	set_delay_inf.MaxDlDelayRe[CMD_SYS_3G]			= clc_DlDelay_MaxNo[CMD_SYS_3G];
	set_delay_inf.MaxDlDelayRe[CMD_SYS_S3G]			= clc_DlDelay_MaxNo[CMD_SYS_S3G];
	set_delay_inf.MaxUlDelayRe[CMD_SYS_3G]			= clc_UlDelay_MaxNo[CMD_SYS_3G];
	set_delay_inf.MaxUlDelayRe[CMD_SYS_S3G]			= clc_UlDelay_MaxNo[CMD_SYS_S3G];
	set_delay_inf.DlDelayBase[CMD_SYS_3G]			= DlBaseDelay[CMD_SYS_3G];
	set_delay_inf.DlDelayBase[CMD_SYS_S3G]			= DlBaseDelay[CMD_SYS_S3G];
	set_delay_inf.UlDelayBase[CMD_SYS_3G]			= UlBaseDelay[CMD_SYS_3G];
	set_delay_inf.UlDelayBase[CMD_SYS_S3G]			= UlBaseDelay[CMD_SYS_S3G];
	set_delay_inf.DlDelayBaseAlign[CMD_SYS_3G]		= DlBaseDelayAlign[CMD_SYS_3G];
	set_delay_inf.DlDelayBaseAlign[CMD_SYS_S3G]		= DlBaseDelayAlign[CMD_SYS_S3G];
	set_delay_inf.UlDelayBaseAlign[CMD_SYS_3G]		= UlBaseDelayAlign[CMD_SYS_3G];
	set_delay_inf.UlDelayBaseAlign[CMD_SYS_S3G]		= UlBaseDelayAlign[CMD_SYS_S3G];	
	set_delay_inf.X_Fhm								= X_Fhm;
	set_delay_inf.Y_Fhm								= Y_Fhm;
	set_delay_inf.ToffsetReport						= ToffsetReport;
	set_delay_inf.FhmToffset						= clc_FHM_Toffset;
	
	/* 書き込み	*/
	f_cmn_com_delay_inf_set(&set_delay_inf);


}

/* @} */

/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_getConnectEqpInfo3gTbl.c
 *  @brief  装置情報取得して集約(3G)
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
 *  @brief  周波数帯(3G)カウント
 *  @param  awFreqBand          [in]    対応周波数帯
 *  @return None
 *  @date   2016/02/26 TDIPS)enoki create
 */
/********************************************************************************************************************/
VOID m_rc_FreqBandCounterSet3G( USHORT awFreqBand )
{
	switch( awFreqBand )
	{
	case D_RRH_CRP_FRQ_2G		:
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_2G]++;
		return;
	case D_RRH_CRP_FRQ_800M		:
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_800M]++;
		return;
	case D_RRH_CRP_FRQ_17G		:
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_17G]++;
		return;
	case D_RRH_CRP_FRQ_2G_800M	:
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_2G]++;
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_800M]++;
		return;
	case D_RRH_CRP_FRQ_2G_17G	:
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_2G]++;
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_17G]++;
		return;
	case D_RRH_CRP_FRQ_800M_17G	:
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_800M]++;
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_17G]++;
		return;
	case D_RRH_CRP_FRQ_2G_17G_800:
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_2G]++;
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_17G]++;
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_800M]++;
		return;
	case D_RRH_CRP_FRQ_15G		:
		gw_rcw_FreqBandCounter3G[D_RRH_CRP_FRQ_15G]++;
		return;
	default :
		return;
	}
}

/********************************************************************************************************************/
/**
 *  @brief  装置情報取得して集約(3G)
 * @TBD_No #4装置号番の設定値が不明
 * @TBD_No #7電源種別の判断不明
 * @TBD_No #12パワークラス単位の設定値の判別方法が不明
 * @TBD_No #25RE側TRX-INFタイプ情報の判断不明
 *  @param  lp_cnt              [in]    CPRIリンク番号
 *  @param  firstElement_flg    [out]   取得フラグ
 *  @return None
 *  @date   2015/08/07  TDIPS) maruyama create
 *  @date   2015/08/27 TDIPS)maruyama create 立ち上がったREの情報をまとめる(m_rc_infoAllRE.cから分割）
 *  @date   2015/10/11 TDIPS)maruyama  リターン時の判定処理修正
 *  @date   2015/10/17 TDI)satou FHM技説-QA-029
 *  @date   2015/10/28 TDI)satou FHM技説-QA-108
 *  @date   2015/12/01 FPT)Lay IT3問処No103の対応
 *  @date   2016/02/25 TDI)enoki FHM技説1.0.5仕様変更対応
 */
/********************************************************************************************************************/
UINT m_rc_getConnectEqpInfo3gTbl(UINT	lp_cnt,UINT* firstElement_flg)
{
	T_RRH_EQP_3G                ltReEqpInfBuf3g	={};
	CMT_CPRIF_EQPSTRRES*        ltReEqpInf3g_p	= NULL;
	USHORT                      lwTemp;
	USHORT						lwVal1;
	USHORT						lwVal2;
	const USHORT				FreqBandMixTable[9][9] =		/* 3G 対応周波数帯集約テーブル */
		{  /* 0  1  2  3  4  5  6  7  8 */
			{ 0, 3, 4, 3, 4, 6, 6, 0, 0}, 		/* 0:2G				D_RRH_CRP_FRQ_2G		*/
			{ 3, 1, 5, 3, 6, 5, 6, 1, 1},		/* 1:800M			D_RRH_CRP_FRQ_800M		*/
			{ 4, 5, 2, 6, 4, 5, 6, 2, 2},		/* 2:1.7G			D_RRH_CRP_FRQ_17G		*/
			{ 3, 3, 6, 3, 6, 6, 6, 3, 3},		/* 3:2G/800M		D_RRH_CRP_FRQ_2G_800M	*/
			{ 4, 6, 4, 6, 4, 6, 6, 4, 4},		/* 4:2G/1.7G		D_RRH_CRP_FRQ_2G_17G	*/
			{ 6, 5, 5, 6, 6, 5, 6, 5, 5},		/* 5:800M/1.7G		D_RRH_CRP_FRQ_800M_17G	*/
			{ 6, 6, 6, 6, 6, 6, 6, 6, 6},		/* 6:2G/800M/1.7G	D_RRH_CRP_FRQ_2G_17G_800 */
			{ 0, 1, 2, 3, 4, 5, 6, 7, 7},		/* 7:1.5G			D_RRH_CRP_FRQ_15G		*/
			{ 0, 1, 2, 3, 4, 5, 6, 7, 65535}	/* 8:範囲外			 						*/
		};


	f_cmn_com_eqp_3g_get( lp_cnt, &ltReEqpInfBuf3g );
	ltReEqpInf3g_p = (CMT_CPRIF_EQPSTRRES*)(&ltReEqpInfBuf3g);
			
	if((ltReEqpInf3g_p->signal_kind != 0))
	{
		if(CMD_TRUE == *firstElement_flg)
		{
			/* 指定周波数帯カウンター(3G)初期化 */
			memset( &gw_rcw_FreqBandCounter3G[0], 0, sizeof(gw_rcw_FreqBandCounter3G) );

			/**Mark the first element is not NULL*/
			*firstElement_flg = CMD_FALSE;
			memcpy(&gt_recw_ReEqpStrInfTbl_3G,&(ltReEqpInf3g_p->cpristr_inf),sizeof(ltReEqpInf3g_p->cpristr_inf));

			gt_recw_ReEqpStrInfTbl_3G.powerunit = RCD_PWR_UNIT_W;
			/*	パワークラスが無効値(65535)でないの場合は、初期値設定する	*/
			if(gt_recw_ReEqpStrInfTbl_3G.powerclass_0 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_3G.powerclass_0 = m_rc_RoundUp_Watt(ltReEqpInf3g_p->cpristr_inf.powerunit, ltReEqpInf3g_p->cpristr_inf.powerclass_0);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_3G.powerclass_0 = 0;
			}
			
			/*	パワークラスが無効値(65535)でないの場合は、初期値設定する	*/
			if(gt_recw_ReEqpStrInfTbl_3G.powerclass_1 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_3G.powerclass_1 = m_rc_RoundUp_Watt(ltReEqpInf3g_p->cpristr_inf.powerunit, ltReEqpInf3g_p->cpristr_inf.powerclass_1);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_3G.powerclass_1 = 0;
			}

			/* 対応周波数帯が範囲外の場合は非対応(65535)を設定 */
			if( D_RRH_CRP_FRQ_15G < gt_recw_ReEqpStrInfTbl_3G.frqband )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "FreqBand(3G) RangeOut. RE#%d:%d",
                        lp_cnt,
                        gt_recw_ReEqpStrInfTbl_3G.frqband);
				gt_recw_ReEqpStrInfTbl_3G.frqband = 65535;
			}

			/* 指定対応周波数帯をカウントアップ */
			/* REカード情報報告応答のカード名の決定に使用する */
			m_rc_FreqBandCounterSet3G( gt_recw_ReEqpStrInfTbl_3G.frqband );
		}
		else
		{
			/* RECへの応答を更新 */
			
			/* #4(装置号番):  */
//TODO 異なる装置番号の場合、１でよいのか
			/* 配下REの中で異なる値をもつ装置がある場合は:  */
			/* 装置号番に1を付与し、その値をそのまま保持する */
			/* 配下REの中で同じ値をもつ場合は:              */
			/* 装置号番を配下REと同じ値で更新する。         */
			if((ltReEqpInf3g_p->cpristr_inf.eqp_no == 1)
			||(gt_recw_ReEqpStrInfTbl_3G.eqp_no != ltReEqpInf3g_p->cpristr_inf.eqp_no))
            {
                gt_recw_ReEqpStrInfTbl_3G.eqp_no = 1;
            }
			/* #11(電源種別):  */
//TODO :0：DC、3：AC100V／AC200V 混在の場合の判断不明

			/* #12(パワークラス単位): パワークラス単位が最大のものの場合、更新する。 */
			gt_recw_ReEqpStrInfTbl_3G.powerunit = RCD_PWR_UNIT_W;

			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInf3g_p->cpristr_inf.powerclass_0 != CMD_IVL_VAL)
			{
				/* #13(パワークラス(0系)): が最大のものの場合、更新する。 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInf3g_p->cpristr_inf.powerunit, ltReEqpInf3g_p->cpristr_inf.powerclass_0);
				gt_recw_ReEqpStrInfTbl_3G.powerclass_0 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_3G.powerclass_0);
			}

			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInf3g_p->cpristr_inf.powerclass_1 != CMD_IVL_VAL)
			{
				/* #14(パワークラス(1系)):が最大のものの場合、更新する。 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInf3g_p->cpristr_inf.powerunit, ltReEqpInf3g_p->cpristr_inf.powerclass_1);
				gt_recw_ReEqpStrInfTbl_3G.powerclass_1 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_3G.powerclass_1);
			}

			/* #15(対応周波数帯): */
			/* １つ以上の配下REが対応する全ての周波数帯において対応とすること */
			lwVal1 = gt_recw_ReEqpStrInfTbl_3G.frqband;
			if( D_RRH_CRP_FRQ_15G < lwVal1 )
			{
				lwVal1 = D_RRH_CRP_FRQ_15G + 1;		/* 最大値＋１ */
			}
			lwVal2 = ltReEqpInf3g_p->cpristr_inf.frqband;
			if( D_RRH_CRP_FRQ_15G < lwVal2 )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "FreqBand(3G) RangeOut. RE#%d:%d",
                        lp_cnt,
                        ltReEqpInf3g_p->cpristr_inf.frqband);
				lwVal2 = D_RRH_CRP_FRQ_15G + 1;		/* 最大値＋１ */
			}
			gt_recw_ReEqpStrInfTbl_3G.frqband = FreqBandMixTable[lwVal1][lwVal2];

			/* 指定対応周波数帯をカウントアップ */
			/* REカード情報報告応答のカード名の決定に使用する */
			m_rc_FreqBandCounterSet3G( ltReEqpInf3g_p->cpristr_inf.frqband );

			/* #16(800MHz帯フィルタタイプ ): 項番#15と同様 */
			/* 配下REの中で異なる値をもつ装置がある場合は65535で報告 */
			if(gt_recw_ReEqpStrInfTbl_3G.us800m_fil_ytp != ltReEqpInf3g_p->cpristr_inf.us800m_fil_ytp)
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us800m_fil_ytp. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_3G.us800m_fil_ytp,
                        lp_cnt,
                        ltReEqpInf3g_p->cpristr_inf.us800m_fil_ytp);
				gt_recw_ReEqpStrInfTbl_3G.us800m_fil_ytp = 65535;
			}

			/* #17(2GHzキャリア数): */
			/* 配下REの中で最小のもの(但し0の場合を除く) */
			if(((0 == gt_recw_ReEqpStrInfTbl_3G.us2g_car_no)&&(0 < ltReEqpInf3g_p->cpristr_inf.us2g_car_no))||
			   ((gt_recw_ReEqpStrInfTbl_3G.us2g_car_no > ltReEqpInf3g_p->cpristr_inf.us2g_car_no)&&(0 < ltReEqpInf3g_p->cpristr_inf.us2g_car_no )))
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us2g_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_3G.us2g_car_no,
                        lp_cnt,
                        ltReEqpInf3g_p->cpristr_inf.us2g_car_no);
				gt_recw_ReEqpStrInfTbl_3G.us2g_car_no = ltReEqpInf3g_p->cpristr_inf.us2g_car_no;
			}

			/*  #18(800MHzキャリア数):項番#17と同様 */
			if(((0 == gt_recw_ReEqpStrInfTbl_3G.us800m_car_no)&&(0 < ltReEqpInf3g_p->cpristr_inf.us800m_car_no))||
			   ((gt_recw_ReEqpStrInfTbl_3G.us800m_car_no > ltReEqpInf3g_p->cpristr_inf.us800m_car_no)&&(0 < ltReEqpInf3g_p->cpristr_inf.us800m_car_no )))
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us800m_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_3G.us800m_car_no,
                        lp_cnt,
                        ltReEqpInf3g_p->cpristr_inf.us800m_car_no);
				gt_recw_ReEqpStrInfTbl_3G.us800m_car_no = ltReEqpInf3g_p->cpristr_inf.us800m_car_no;
			}

			/* #19(1.7GHzキャリア数):  項番#17と同様 */
			if(((0 == gt_recw_ReEqpStrInfTbl_3G.us17g_car_no)&&(0 < ltReEqpInf3g_p->cpristr_inf.us17g_car_no))||
			   ((gt_recw_ReEqpStrInfTbl_3G.us17g_car_no > ltReEqpInf3g_p->cpristr_inf.us17g_car_no)&&(0 < ltReEqpInf3g_p->cpristr_inf.us17g_car_no)))
			{	
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us17g_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_3G.us17g_car_no,
                        lp_cnt,
                        ltReEqpInf3g_p->cpristr_inf.us17g_car_no);
				gt_recw_ReEqpStrInfTbl_3G.us17g_car_no = ltReEqpInf3g_p->cpristr_inf.us17g_car_no;
			}

			/** #20(RE側TRX-INFタイプ情報):  */

			/* #21(1.5GHzキャリア数): 項番#17と同様 */
			if(((0 == gt_recw_ReEqpStrInfTbl_3G.us15g_car_no)&&(0 < ltReEqpInf3g_p->cpristr_inf.us15g_car_no))||
			   ((gt_recw_ReEqpStrInfTbl_3G.us15g_car_no > ltReEqpInf3g_p->cpristr_inf.us15g_car_no)&&(0 < ltReEqpInf3g_p->cpristr_inf.us15g_car_no)))
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us15g_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_3G.us15g_car_no,
                        lp_cnt,
                        ltReEqpInf3g_p->cpristr_inf.us15g_car_no);
				gt_recw_ReEqpStrInfTbl_3G.us15g_car_no = ltReEqpInf3g_p->cpristr_inf.us15g_car_no;
			}
		}
	}
	

	//return;
	if( CMD_FALSE == *firstElement_flg )
	{
		/*The synthesis is happened */
		return CMD_RES_OK;
	}
	else
	{
		/*The synthesis is not  happened */
		return CMD_RES_EMPTY;
	}

}


/* @} */


/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_getConnectEqpInfoLTETbl.c
 *  @brief  装置情報取得して集約(LTE)
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
 *  @brief  装置情報取得して集約(LTE)
 * @TBD_No #7電源種別の判断不明
 * @TBD_No #10パワークラス単位の設定値の判別方法が不明
 * @TBD_No #25RE側TRX-INFタイプ情報の判断不明
 *  @note   参考ソース UINT m_rc_GenResFromReSubLte()
 *  @param  lp_cnt              [in]    CPRIリンク番号
 *  @param  firstElement_flg    [out]   取得フラグ
 *  @return None
 *  @date   2015/08/07  TDIPS) maruyama create
 *  @date   2015/08/27 TDIPS)maruyama create 立ち上がったREの情報をまとめる(m_rc_infoAllRE.cから分割）
 *  @date   2015/10/11 TDIPS)maruyama  リターン時の判定処理修正
 *  @date   2015/10/17 TDI)satou FHM技説-QA-029
 *  @date   2015/10/28 TDI)satou FHM技説-QA-108
 *  @date   2015/12/01 FPT)Lay IT3問処No103の対応
 *  @date   2016/02/22 TDIPS)enoki FHM技説1.0.5変更対応
 *  @date   2016/02/24 TDIPS)enoki FHM技説1.0.5変更対応
 */
/********************************************************************************************************************/
UINT m_rc_getConnectEqpInfoLTETbl(UINT	lp_cnt,UINT* firstElement_flg)
{
	T_RRH_EQP_S3G				ltReEqpInfBufLte	= {};
	CMT_CPRIF_EQPSTRRES_S3G*    ltReEqpInfLte_p		= NULL;
	USHORT                      lwTemp;


	f_cmn_com_eqp_s3g_get( lp_cnt , &ltReEqpInfBufLte  );				/* 共有メモリから取得	*/
	ltReEqpInfLte_p = (CMT_CPRIF_EQPSTRRES_S3G*)(&ltReEqpInfBufLte);	/* LTE用バッファ	*/

	if(ltReEqpInfLte_p->signal_kind != 0)
	{
		if( CMD_TRUE == *firstElement_flg )
		{										/* 最初のデータなら比較せずそのままコピー	*/
			memcpy(&gt_recw_ReEqpStrInfTbl_LTE,&(ltReEqpInfLte_p->cpristr_inf),sizeof(ltReEqpInfLte_p->cpristr_inf));
			*firstElement_flg = CMD_FALSE;		/* フラグクリア	*/

			gt_recw_ReEqpStrInfTbl_LTE.powerunit = RCD_PWR_UNIT_W;
			/*	パワークラスが無効値(65535)でないの場合は、初期値設定する	*/
			if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_0 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_0 = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_0);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_0 = 0;
			}
			/*	パワークラスが無効値(65535)でないの場合は、初期値設定する	*/
			if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_1 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_1 = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_1);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_1 = 0;
			}
			/*	パワークラスが無効値(65535)でないの場合は、初期値設定する	*/
			if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_2 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_2 = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_2);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_2 = 0;
			}
			/*	パワークラスが無効値(65535)でないの場合は、初期値設定する	*/
			if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_3 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_3 = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_3);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_3 = 0;
			}
			/*	パワークラスが無効値(65535)でないの場合は、初期値設定する	*/
			if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_4 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_4 = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_4);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_4 = 0;
			}
			/*	パワークラスが無効値(65535)でないの場合は、初期値設定する	*/
			if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_5 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_5 = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_5);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_5 = 0;
			}
			/*	パワークラスが無効値(65535)でないの場合は、初期値設定する	*/
			if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_6 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_6 = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_6);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_6 = 0;
			}
			if(gt_recw_ReEqpStrInfTbl_LTE.powerclass_7 != CMD_IVL_VAL)
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_7 = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_7);
			}
			/*	パワークラスが無効値(65535)の場合は、仮の初期値を設定する	*/
			else
			{
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_7 = 0;
			}
		}
		else
		{
												/* 2回目以降なので比較する	*/
			/* #1-#4は固定値のため比較しない	*/
			/* #5(3GBTS共用方式) ： 配下REのなかで異なる値を持つ装置がある場合は、0(LTE専用)で報告 */
			if(gt_recw_ReEqpStrInfTbl_LTE.bts_share_type != ltReEqpInfLte_p->cpristr_inf.bts_share_type)
			{
				/* LTE専用とする */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch bts_share_type. FHM:0x%04x RE#%d:0x%04x",
				        gt_recw_ReEqpStrInfTbl_LTE.bts_share_type,
				        lp_cnt,
				        ltReEqpInfLte_p->cpristr_inf.bts_share_type);
				gt_recw_ReEqpStrInfTbl_LTE.bts_share_type = 0;
			}
			/* 配下REが接続されていない場合 */
			
			/* #6 予備のため比較しない	*/
			
			
			/* #7(電源種別)： */
//TODO:0：DC、3：AC100V／AC200V 混在の場合の判断不明
//→
/*FHMの電源種別で、BPF初期化内でEEPROMから取得値をもとに設定される 	*/
/* RE個別の電源種別は参照しない									*/
			/* #8(受信アンテナ数)：受信アンテナ数が最大のものの場合、更新する。 */
			gt_recw_ReEqpStrInfTbl_LTE.rcv_antena_no = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.rcv_antena_no, 
																			gt_recw_ReEqpStrInfTbl_LTE.rcv_antena_no);


			/* #9(送信アンテナ数): 送信アンテナ数が最大のものの場合、更新する。 */
			gt_recw_ReEqpStrInfTbl_LTE.snd_antena_no = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.snd_antena_no,
																			gt_recw_ReEqpStrInfTbl_LTE.snd_antena_no);


			/* #10(パワークラス単位): 「0:W」 */
			gt_recw_ReEqpStrInfTbl_LTE.powerunit = RCD_PWR_UNIT_W;


			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInfLte_p->cpristr_inf.powerclass_0 != CMD_IVL_VAL)
			{
				/* #11(パワークラス(0系)) : パワークラス(0系)が最大のものの場合、更新する。 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_0);
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_0 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_LTE.powerclass_0);
			}

			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInfLte_p->cpristr_inf.powerclass_1 != CMD_IVL_VAL)
			{
				/* #12(パワークラス(1系)) : 項番#11と同様 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_1);
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_1 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_LTE.powerclass_1);
			}

			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInfLte_p->cpristr_inf.powerclass_2 != CMD_IVL_VAL)
			{
				/* #13(パワークラス(2系)) : 項番#11と同様 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_2);
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_2 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_LTE.powerclass_2);
			}

			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInfLte_p->cpristr_inf.powerclass_3 != CMD_IVL_VAL)
			{
				/* #14(パワークラス(3系)) : 項番#11と同様 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_3);
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_3 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_LTE.powerclass_3);
			}

			/* #15,16は予備のため比較しない	*/

			/* #17(対応周波数帯) : 一つ以上の配下REが対応する全ての周波数帯において対応とする */
//			gt_recw_ReEqpStrInfTbl_LTE.frqband |= (ltReEqpInfLte_p->cpristr_inf.frqband & 0x007F);
			gt_recw_ReEqpStrInfTbl_LTE.frqband |= (ltReEqpInfLte_p->cpristr_inf.frqband & CMD_CRP_FRQ_ALL_S3G);/* 34G-SRE(CMD_CRP_FRQ_ALL_S3G = 0x00FF) */

			/* #18(最大周波数帯域幅)：配下REの中で最小のものを設定 */
			if( gt_recw_ReEqpStrInfTbl_LTE.max_bandwidth > ltReEqpInfLte_p->cpristr_inf.max_bandwidth )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch max_bandwidth. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_LTE.max_bandwidth,
                        lp_cnt,
                        ltReEqpInfLte_p->cpristr_inf.max_bandwidth);
				gt_recw_ReEqpStrInfTbl_LTE.max_bandwidth = ltReEqpInfLte_p->cpristr_inf.max_bandwidth;
			}

			/* #19(キャリア数(2GHz) ) */
			/* 配下REの中で最小のもの設定 */
			if( gt_recw_ReEqpStrInfTbl_LTE.us2g_car_no > ltReEqpInfLte_p->cpristr_inf.us2g_car_no )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us2g_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_LTE.us2g_car_no,
                        lp_cnt,
                        ltReEqpInfLte_p->cpristr_inf.us2g_car_no);
				gt_recw_ReEqpStrInfTbl_LTE.us2g_car_no = ltReEqpInfLte_p->cpristr_inf.us2g_car_no;
			}


			/* #20(キャリア数(1.5GHz)): 項番#19と同様 */
			if( gt_recw_ReEqpStrInfTbl_LTE.us15g_car_no > ltReEqpInfLte_p->cpristr_inf.us15g_car_no )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us15g_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_LTE.us15g_car_no,
                        lp_cnt,
                        ltReEqpInfLte_p->cpristr_inf.us15g_car_no);
				gt_recw_ReEqpStrInfTbl_LTE.us15g_car_no = ltReEqpInfLte_p->cpristr_inf.us15g_car_no;
			}


			/* #21(キャリア数(800MHz)):項番#19と同様 */
			if( gt_recw_ReEqpStrInfTbl_LTE.us800m_car_no > ltReEqpInfLte_p->cpristr_inf.us800m_car_no )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us800m_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_LTE.us800m_car_no,
                        lp_cnt,
                        ltReEqpInfLte_p->cpristr_inf.us800m_car_no);
				gt_recw_ReEqpStrInfTbl_LTE.us800m_car_no = ltReEqpInfLte_p->cpristr_inf.us800m_car_no;
			}


			/* #22(キャリア数(700MHz)): 項番#19と同様 */
			if( gt_recw_ReEqpStrInfTbl_LTE.us700m_car_no > ltReEqpInfLte_p->cpristr_inf.us700m_car_no )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us700m_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_LTE.us700m_car_no,
                        lp_cnt,
                        ltReEqpInfLte_p->cpristr_inf.us700m_car_no);
				gt_recw_ReEqpStrInfTbl_LTE.us700m_car_no = ltReEqpInfLte_p->cpristr_inf.us700m_car_no;
			}


			/* #23(キャリア数(1.7GHz)): 項番#19と同様 */
			if( gt_recw_ReEqpStrInfTbl_LTE.us17g_car_no > ltReEqpInfLte_p->cpristr_inf.us17g_car_no )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us17g_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_LTE.us17g_car_no,
                        lp_cnt,
                        ltReEqpInfLte_p->cpristr_inf.us17g_car_no);
				gt_recw_ReEqpStrInfTbl_LTE.us17g_car_no = ltReEqpInfLte_p->cpristr_inf.us17g_car_no;
			}


			/* #24(キャリア数(1.7GHz LowBand)): 項番#19と同様 (us17g_car_no_Low)*/
			if( gt_recw_ReEqpStrInfTbl_LTE.dummy3[0] > ltReEqpInfLte_p->cpristr_inf.dummy3[0] )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us17g_car_no_Low. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_LTE.dummy3[0],
                        lp_cnt,
                        ltReEqpInfLte_p->cpristr_inf.dummy3[0]);
				gt_recw_ReEqpStrInfTbl_LTE.dummy3[0] = ltReEqpInfLte_p->cpristr_inf.dummy3[0];
			}


			/* #25(TRX-INFタイプ):  */
//TODO：0：13dBタイプ  1：21dBタイプ  2：5～14dBタイプ  3：13～21dBタイプ 混在の場合の判定不明
//→FHMのTRX-INFタイプで、RE個別では参照しない

			/* #26(キャリア数(3.5GHz)): 項番#19と同様 */
			if( gt_recw_ReEqpStrInfTbl_LTE.us35g_car_no > ltReEqpInfLte_p->cpristr_inf.us35g_car_no )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us35g_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_LTE.us35g_car_no,
                        lp_cnt,
                        ltReEqpInfLte_p->cpristr_inf.us35g_car_no);
				gt_recw_ReEqpStrInfTbl_LTE.us35g_car_no = ltReEqpInfLte_p->cpristr_inf.us35g_car_no;
			}

			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInfLte_p->cpristr_inf.powerclass_4 != CMD_IVL_VAL)
			{
				/* #27(パワークラス(4系)):パワークラス(4系)が最大のものの場合、更新する。 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_4);
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_4 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_LTE.powerclass_4);
			}

			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInfLte_p->cpristr_inf.powerclass_5 != CMD_IVL_VAL)
			{
				/* #28(パワークラス(5系)):項番#27と同様 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_5);
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_5 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_LTE.powerclass_5);
			}

			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInfLte_p->cpristr_inf.powerclass_6 != CMD_IVL_VAL)
			{
				/* #29(パワークラス(6系)):項番#27と同様 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_6);
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_6 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_LTE.powerclass_6);
			}

			/*	パワークラスが無効値(65535)の場合は、設定をスキップする	*/
			if(ltReEqpInfLte_p->cpristr_inf.powerclass_7 != CMD_IVL_VAL)
			{
				/* #30(パワークラス(7系)):項番#27と同様 */
				lwTemp = m_rc_RoundUp_Watt(ltReEqpInfLte_p->cpristr_inf.powerunit, ltReEqpInfLte_p->cpristr_inf.powerclass_7);
				gt_recw_ReEqpStrInfTbl_LTE.powerclass_7 = m_rc_GetMaxValue(lwTemp, gt_recw_ReEqpStrInfTbl_LTE.powerclass_7);
			}
			
			/* #31(キャリア数(3.4GHz)): 項番#19と同様 */
			if( gt_recw_ReEqpStrInfTbl_LTE.us34g_car_no > ltReEqpInfLte_p->cpristr_inf.us34g_car_no )
			{
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, "mismatch us34g_car_no. FHM:0x%04x RE#%d:0x%04x",
                        gt_recw_ReEqpStrInfTbl_LTE.us34g_car_no,
                        lp_cnt,
                        ltReEqpInfLte_p->cpristr_inf.us34g_car_no);
				gt_recw_ReEqpStrInfTbl_LTE.us34g_car_no = ltReEqpInfLte_p->cpristr_inf.us34g_car_no;
			}

			/* #32-#63は予備のため比較しない	*/
		}
	}

	/*return*/
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

/********************************************************************************************************************/
/**
 *  @brief  パワークラス小数点切り上げ
 *  @param  powerunit      [in] パワークラス単位
 *  @param  powerclass_org [in] 変換元(パワークラス)
 *  @return watt変換後の値(小数点切り上げ)
 *  @date   2015/10/17 TDI)satou create
 */
/********************************************************************************************************************/
USHORT m_rc_RoundUp_Watt(USHORT powerunit, USHORT powerclass_org)
{
    USHORT powerclass_ret;

	/*	パワークラスが無効値(65535)でない場合	*/
	if(powerclass_org != CMD_IVL_VAL)
	{
	    switch (powerunit)
	    {
	    /* パワークラス単位が「0:W」の場合は、変換なし */
	    case RCD_PWR_UNIT_W:
	        powerclass_ret = powerclass_org;
	        break;
	    /*
	     * パワークラス単位が「1:mW」の場合は、Wに変換して小数点以下切り上げ.
	     * -     0mW →  0.000W → 0W
	     * -  1000mW →  1.000W → 1W
	     * -  1250mW →  1.250W → 2W
	     * - 65534mW → 65.534W → 66W
	     * - 65535mW → 無効のため変換しない
	     */
	    case RCD_PWR_UNIT_MW:
	        if (0 != (powerclass_org % 1000))
	        {
	            powerclass_ret = (powerclass_org / 1000) + 1;
	        }
	        else
	        {
	            powerclass_ret = (powerclass_org / 1000);
	        }
	        break;
	    /* パワークラス単位が「2:μW」の場合は、0μWの場合は0W、それ以外は1Wを返却 */
	    default:
	        powerclass_ret = (powerclass_org == 0) ? 0 : 1;
	        break;
	    }
	}
	/*	パワークラスが無効値(65535)の場合	*/
	else
	{
        /*	無効値(65535)の場合は変更しない	*/
        powerclass_ret = powerclass_org;
	}

    return powerclass_ret;
}

/* @} */


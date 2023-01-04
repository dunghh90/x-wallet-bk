/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReStrRep.c
 *  @brief  RE equipment composition info report process 
 *  @date   FPT)Quynh Create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 
 */
/********************************************************************************************************************/

/** @addtogroup L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"

/********************************************************************************************************************/
/**
 *  @brief  (3G)(0x5120)RE装置構成情報報告応答(0:正常)
 *  @brief	(LTE)(0x5111)RE装置構成情報報告応答(0:正常)
 *  @note 
 *  @param  aparm_p [in] parameter pointer
 *  @return 
 *  @date FPT) Quynh create
 */
/********************************************************************************************************************/

VOID m_rc_ReStrRep( VOID* aparm_p)
{

	USHORT	                lwSigsys_rslt;						/*  system by checking signal */
	UINT                    ldwSigchk_rslt;						/*  result of checking signal */
	//UINT                    ldwExistResource = CMD_RES_EMPTY; 	/*  notice that the resource is exist (have at least one response is collected from RE)*/
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
	/*In case buffer is NULL*/
	if( NULL == aparm_p )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		/*end processing*/
		return;
	}
	
	/* validate signal */
	ldwSigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)aparm_p)->signal_kind,&lwSigsys_rslt);
	if(ldwSigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
					lwSigsys_rslt, 
					"m_rc_ReStrRep validate signal NG");
	}
//		/*LTE*/
//	if( CMD_SYS_LTE == lwSigsys_rslt )
//	{
//		/*Init table that save information of all RE (LTE)*/
//		m_rec_ReEqpStrInfInitTable_LTE();
//		
//		ldwExistResource = m_rc_GenResFromReSubLte();
//		
//		if( CMD_RES_EMPTY != ldwExistResource )
//		{
//			/*TODO*/ 
//			/**additional check for CPRI link#12 (3G) or CPRI link#10 (LTE)*/
//			if(gt_recw_ReEqpStrInfTbl_LTE.powerunit == CMD_NUM1)
//			{
//				gt_recw_ReEqpStrInfTbl_LTE.powerunit = CMD_NUM0;
//			}
//			
//			/**Update again the field that is constant*/
//			/* メーカ名:  70 */
//			gt_recw_ReEqpStrInfTbl_LTE.maker       = D_RRH_IND_MAKER_NAME;
//			/* 装置形番(BTSの形番号): 3201 */
//			gt_recw_ReEqpStrInfTbl_LTE.bts_no      = CMD_BTS_SRE_NO;
//			/* 装置号番(REの番号):1 */
//			gt_recw_ReEqpStrInfTbl_LTE.eqp_no      = 1;
//			/* RE種別(3:RRE/LRE): 3 */
//			gt_recw_ReEqpStrInfTbl_LTE.rekind      = 3;
//			/* 未規定項目:65535 */
//			gt_recw_ReEqpStrInfTbl_LTE.dummy1      = CMD_IVL_VAL;
//			/* #15 未規定項目:65535 */
//			gt_recw_ReEqpStrInfTbl_LTE.dummy2[0] = CMD_IVL_VAL;
//			/* #16 未規定項目:65535 */
//			gt_recw_ReEqpStrInfTbl_LTE.dummy2[1] =  CMD_IVL_VAL;
//		}
//	}
//	else /**3G*/
//	{
//		/*Init table that save information of all RE (3G)*/
//		m_rec_ReEqpStrInfInitTable_3G();
//		
//		ldwExistResource = m_rc_GenResFromReSub3g();
//		
//		if( CMD_RES_EMPTY != ldwExistResource )
//		{
//			/*TODO*/ 
//			/**additional check for CPRI link#12 (3G) or CPRI link#10 (LTE)*/
//			if(gt_recw_ReEqpStrInfTbl_3G.powerunit == CMD_NUM1)
//			{
//				gt_recw_ReEqpStrInfTbl_3G.powerunit = CMD_NUM0;
//			}
//			/**Update again the field that is constant*/
//			/* メーカ名:  70 */
//			gt_recw_ReEqpStrInfTbl_3G.maker       =   D_RRH_IND_MAKER_NAME; 
//			/* BTS形番(BTSの形番号): 3201 */
//			gt_recw_ReEqpStrInfTbl_3G.bts_no      =   CMD_BTS_SRE_NO;
//			/* RE種別(2:RRE・LRE/OF-TRX): 3 */
//			gt_recw_ReEqpStrInfTbl_3G.rekind      =   3;
//			/* 装置号番:1 */
//			gt_recw_ReEqpStrInfTbl_3G.eqp_no      =   1;
//			/* 未規定項目:65535 */
//			gt_recw_ReEqpStrInfTbl_3G.dummy1[0]   = CMD_IVL_VAL;
//			/* 未規定項目:65535 */
//			gt_recw_ReEqpStrInfTbl_3G.dummy1[1]   = CMD_IVL_VAL; 
//			/* 未規定項目:65535 */
//			gt_recw_ReEqpStrInfTbl_3G.dummy1[2]   = CMD_IVL_VAL;
//			/* 未規定項目:65535 */
//			gt_recw_ReEqpStrInfTbl_3G.dummy1[3]   = CMD_IVL_VAL;
//			/* 未規定項目:65535 */
//			gt_recw_ReEqpStrInfTbl_3G.dummy1[4]   = CMD_IVL_VAL;
//			/* 未規定項目:65535 */
//			gt_recw_ReEqpStrInfTbl_3G.dummy1[5]   = CMD_IVL_VAL;
//			
//		}
//		
//	}
//
	// send response to REC here
	m_rc_ReStrRepResSnd( lwSigsys_rslt );
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	return ;
}

/********************************************************************************************************************/
/**
 *  @brief  RE device composition information report response
 *  @note
 *  @param  aparm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2021/01/19 M&C)DuongMX Create
 */
/********************************************************************************************************************/
VOID m_rc_ReStrRes( VOID* aparm_p)
{
	USHORT	                		  lwSigsys_rslt;			/*  system by checking signal */
	UINT                    		  ldwSigchk_rslt;			/*  result of checking signal */
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
	/*In case buffer is NULL*/
	if( NULL == aparm_p )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		/*end processing*/
		return;
	}
	
	/* validate signal */
	ldwSigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRISND_EQPSTRRES_S3G*)aparm_p)->cpridat_eqpstrres.signal_kind, &lwSigsys_rslt);
	if(ldwSigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_USELOW, lwSigsys_rslt, "m_rc_ReStrRes validate signal NG");
	}

	/*LTE*/
	if( CMD_SYS_LTE == lwSigsys_rslt )
	{
		/*  RE装置構成情報報告応答送信 */
		m_cm_L3MsgSend_REC(aparm_p);
	}
	else /**3G*/
	{
		cm_Assert(	CMD_ASL_USELOW, lwSigsys_rslt, "m_rc_ReStrRes 3G signal NG");
		return;		
	}
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	return ;
}

/********************************************************************************************************************/
/**
 *  @brief	(3G)(0x5110) the function that synthesize RE装置構成情報 (LTE)
 *  @note 
 *  @param -
 *  @return lwFirstElement [out] Notice that have at least one response from subordinate RE
 *  @date 
 *  @TBD_No 修正未完了
*/
/********************************************************************************************************************/
//UINT m_rc_GenResFromReSubLte()
//{
//	USHORT                      lwLoop;
//	USHORT                      lwFirstElement = CMD_TRUE; 
//	T_RRH_EQP_S3G               ltReEqpInfBufLte;								/*Buffer to get equipment information of each subordinate RE*/
//	CMT_CPRIF_EQPSTRRES_S3G*    ltReEqpInfLte_p;								/*The pointerSave equipment information of all RE (LTE) */
//	USHORT                      lwFhmAlmFlag = CMD_FALSE;						/* This flag true when have a case is ALM*/
//	
//		
///* RECへの応答を更新 */
//		for( lwLoop=0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
//		{
//			f_cmn_com_eqp_s3g_get( lwLoop + CMD_NUM1, &ltReEqpInfBufLte  );
//			ltReEqpInfLte_p = (CMT_CPRIF_EQPSTRRES_S3G*)(&ltReEqpInfBufLte);
//			
//			if((ltReEqpInfLte_p->signal_kind != 0))
//			{
//				if( CMD_TRUE == lwFirstElement )
//				{
//					
//					lwFirstElement = CMD_FALSE;
//					/** Cpy CPRIリンク装置構成情報 */
//					memcpy(&gt_recw_ReEqpStrInfTbl_LTE,&(ltReEqpInfLte_p->cpristr_inf),sizeof(ltReEqpInfLte_p->cpristr_inf));
//					
//				}
//				else
//				{
//					/* #5(3GBTS共用方式) ： */
//					if(gt_recw_ReEqpStrInfTbl_LTE.bts_share_type != ltReEqpInfLte_p->cpristr_inf.bts_share_type)
//					{
//						gt_recw_ReEqpStrInfTbl_LTE.bts_share_type = 0;
//					}
//					/* 配下REが接続されていない場合 */
//					
//					/* #7(電源種別)： */    //TBD; //SR
//					
//					/* #8(受信アンテナ数)：受信アンテナ数が最大のものの場合、更新する。 */
//					gt_recw_ReEqpStrInfTbl_LTE.rcv_antena_no = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.rcv_antena_no, 
//																					gt_recw_ReEqpStrInfTbl_LTE.rcv_antena_no);
//					
//					
//					/* #9(送信アンテナ数): 送信アンテナ数が最大のものの場合、更新する。 */
//					gt_recw_ReEqpStrInfTbl_LTE.snd_antena_no = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.snd_antena_no,
//																					gt_recw_ReEqpStrInfTbl_LTE.snd_antena_no);
//					
//					
//					/* #10(パワークラス単位): パワークラス単位が最大のものの場合、更新する。 */
//					gt_recw_ReEqpStrInfTbl_LTE.powerunit = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.powerunit,
//																				gt_recw_ReEqpStrInfTbl_LTE.powerunit);
//
//					/* #11(パワークラス(0系)): パワークラス(0系)が最大のものの場合、更新する。 */
//					gt_recw_ReEqpStrInfTbl_LTE.powerclass_0 = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.powerclass_0,
//																					gt_recw_ReEqpStrInfTbl_LTE.powerclass_0);
//					
//					/* #12(パワークラス(1系)) : 項番#11と同様 */
//					gt_recw_ReEqpStrInfTbl_LTE.powerclass_1 = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.powerclass_1,
//																					gt_recw_ReEqpStrInfTbl_LTE.powerclass_1);
//					/* #13パワークラス(2系): 項番#11と同様 */
//					gt_recw_ReEqpStrInfTbl_LTE.powerclass_2 = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.powerclass_2,
//																					gt_recw_ReEqpStrInfTbl_LTE.powerclass_2);
//					/* #14パワークラス(3系) : 項番#11と同様 */
//					gt_recw_ReEqpStrInfTbl_LTE.powerclass_3 = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.powerclass_3,
//																					gt_recw_ReEqpStrInfTbl_LTE.powerclass_3);
//					/* #17(対応周波数帯): */
//					if(gt_recw_ReEqpStrInfTbl_LTE.frqband != ltReEqpInfLte_p->cpristr_inf.frqband)
//					{	
//						gt_recw_ReEqpStrInfTbl_LTE.frqband = 0;
//						/* FHMのカード基本監視項目をALMにします */
//						lwFhmAlmFlag = CMD_TRUE;
//					}
//					/* #18(最大周波数帯域幅) */
//					if(gt_recw_ReEqpStrInfTbl_LTE.max_bandwidth != ltReEqpInfLte_p->cpristr_inf.max_bandwidth)
//					{	
//						gt_recw_ReEqpStrInfTbl_LTE.max_bandwidth = 4;
//						/* FHMのカード基本監視項目をALMにします */
//						lwFhmAlmFlag = CMD_TRUE;
//					}
//					
//					
//					/* #19(キャリア数(2GHz) ) */
//					/* 配下REの中で異なる値をもつ装置がある場合は: */
//					/* キャリア数(2GHz)を65535で更新し、その値をそのまま保持する。 */
//					/* 配下REの中で同じ値をもつ場合はキャリア数(2GHz)を配下REと同じ値で更新する。 */
//					if(gt_recw_ReEqpStrInfTbl_LTE.us2g_car_no != ltReEqpInfLte_p->cpristr_inf.us2g_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_LTE.us2g_car_no = CMD_IVL_VAL;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					
//					}
//					
//					/* #20(キャリア数(1.5GHz)): 項番#19と同様 */
//					if(gt_recw_ReEqpStrInfTbl_LTE.us15g_car_no != ltReEqpInfLte_p->cpristr_inf.us15g_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_LTE.us15g_car_no = CMD_IVL_VAL;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					
//					}
//					
//					/* #21(キャリア数(800MHz)):項番#19と同様 */
//					if(gt_recw_ReEqpStrInfTbl_LTE.us800m_car_no != ltReEqpInfLte_p->cpristr_inf.us800m_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_LTE.us800m_car_no = CMD_IVL_VAL;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					
//					}
//
//					/* #22(キャリア数(700MHz)): 項番#19と同様 */
//					if(gt_recw_ReEqpStrInfTbl_LTE.us700m_car_no != ltReEqpInfLte_p->cpristr_inf.us700m_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_LTE.us700m_car_no = CMD_IVL_VAL;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					
//					}
//
//					/* #23(キャリア数(1.7GHz)): 項番#19と同様 */
//					if(gt_recw_ReEqpStrInfTbl_LTE.us17g_car_no != ltReEqpInfLte_p->cpristr_inf.us17g_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_LTE.us17g_car_no = CMD_IVL_VAL;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					
//					}
//					/* #24(キャリア数(1.7GHz LowBand)): 項番#19と同様 (us17g_car_no_Low)*/
//					if(gt_recw_ReEqpStrInfTbl_LTE.dummy3[0] != ltReEqpInfLte_p->cpristr_inf.dummy3[0])
//					{	
//						gt_recw_ReEqpStrInfTbl_LTE.dummy3[0] = CMD_IVL_VAL;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					
//					}
//					/* #25(TRX-INFタイプ):  */
//					//TBD;
//					/* #26(キャリア数(3.5GHz)): 項番#19と同様 */
//					if(gt_recw_ReEqpStrInfTbl_LTE.us35g_car_no != ltReEqpInfLte_p->cpristr_inf.us35g_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_LTE.us35g_car_no = CMD_IVL_VAL;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					
//					}
//					/* #27(パワークラス(4系)):パワークラス(4系)が最大のものの場合、更新する。 */
//					gt_recw_ReEqpStrInfTbl_LTE.powerclass_4 = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.powerclass_4,
//																					gt_recw_ReEqpStrInfTbl_LTE.powerclass_4);
//					/* #28(パワークラス(5系)):項番#27と同様 */
//					gt_recw_ReEqpStrInfTbl_LTE.powerclass_5 = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.powerclass_5,
//																					gt_recw_ReEqpStrInfTbl_LTE.powerclass_5);
//
//					/* #29(パワークラス(6系)):項番#27と同様 */
//					gt_recw_ReEqpStrInfTbl_LTE.powerclass_6 = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.powerclass_6,
//																					gt_recw_ReEqpStrInfTbl_LTE.powerclass_6);
//					/* #30(パワークラス(7系)):項番#27と同様 */
//					gt_recw_ReEqpStrInfTbl_LTE.powerclass_7 = m_rc_GetMaxValue(ltReEqpInfLte_p->cpristr_inf.powerclass_7,
//																					gt_recw_ReEqpStrInfTbl_LTE.powerclass_7);
//				}
//			}
//		}
//	
//	/*Update FHM ALM bit*/
//	 m_rc_SetFhmAlm( lwFhmAlmFlag );
//	
//	/*return*/
//	if( CMD_FALSE == lwFirstElement )
//	{
//		/*The synthesis is happened */
//		return CMD_RES_OK;
//	}
//	else
//	{
//		/*The synthesis is not  happened */
//		return CMD_RES_EMPTY;
//	}
//}

/********************************************************************************************************************/
/**
 *  @brief	(3G)(0x5110) the function that synthesize RE装置構成情報 (3G)
 *  @note 
 *  @param -
 *  @return lwFirstElement [out] Notice that have at least one response from subordinate RE
 *  @date 
 *  @TBD_No 修正未完了
 */
/********************************************************************************************************************/
//UINT m_rc_GenResFromReSub3g()
//{
//	USHORT                      lwLoop;
//	USHORT                      lwFirstElement = CMD_TRUE; 
//	T_RRH_EQP_3G                ltReEqpInfBuf3g;						/* Buffer to get equipment information of each subordinate RE*/		
//	CMT_CPRIF_EQPSTRRES*        ltReEqpInf3g_p;							/* The pointerSave equipment information of all RE (3G) */
//	USHORT                      lwFhmAlmFlag = CMD_FALSE;				/* This flag true when have a case is ALM*/
//	
//	
///* RECへの応答を更新 */
//		for(lwLoop=0;lwLoop<D_RRH_CPRINO_RE_MAX;lwLoop++)
//		{
//			f_cmn_com_eqp_3g_get( lwLoop + CMD_NUM1, &ltReEqpInfBuf3g );
//			ltReEqpInf3g_p = (CMT_CPRIF_EQPSTRRES*)(&ltReEqpInfBuf3g);
//			
//			if((ltReEqpInf3g_p->signal_kind != 0))
//			{
//				if(CMD_TRUE == lwFirstElement)
//				{
//					/**Mark the first element is not NULL*/
//					lwFirstElement = CMD_FALSE;
//					memcpy(&gt_recw_ReEqpStrInfTbl_3G,&(ltReEqpInf3g_p->cpristr_inf),sizeof(ltReEqpInf3g_p->cpristr_inf));
//					
//				}
//				else
//				{
//	
//					/* RECへの応答を更新 */
//
//					/* #4(装置号番):  */
//					/* 配下REの中で異なる値をもつ装置がある場合は:  */
//					/* 装置号番に1を付与し、その値をそのまま保持する */
//					/* 配下REの中で同じ値をもつ場合は装置号番を配下REと同じ値で更新する。 */
//					if((ltReEqpInf3g_p->cpristr_inf.eqp_no == 1)||
//						(gt_recw_ReEqpStrInfTbl_3G.eqp_no != ltReEqpInf3g_p->cpristr_inf.eqp_no))
//						{
//						
//							gt_recw_ReEqpStrInfTbl_3G.eqp_no = 1;
//						}
//					/* #11(電源種別):  */
//					/*TODO*/ /*TBD*/ //SR
//					
//					/* #12(パワークラス単位): パワークラス単位が最大のものの場合、更新する。 */
//					gt_recw_ReEqpStrInfTbl_3G.powerunit = m_rc_GetMaxValue(ltReEqpInf3g_p->cpristr_inf.powerunit,
//																			gt_recw_ReEqpStrInfTbl_3G.powerunit);
//
//					/* #13(パワークラス(0系)): が最大のものの場合、更新する。 */
//					gt_recw_ReEqpStrInfTbl_3G.powerclass_0 = m_rc_GetMaxValue(ltReEqpInf3g_p->cpristr_inf.powerclass_0,
//																				gt_recw_ReEqpStrInfTbl_3G.powerclass_0);
//					/* #14(パワークラス(0系)):が最大のものの場合、更新する。 */
//					gt_recw_ReEqpStrInfTbl_3G.powerclass_1 = m_rc_GetMaxValue(ltReEqpInf3g_p->cpristr_inf.powerclass_1,
//																				gt_recw_ReEqpStrInfTbl_3G.powerclass_1);
//					/* #15(対応周波数帯): */
//					/* 配下REの中で異なる値をもつ装置がある場合は: */
//					/* 対応周波数帯に65535を付与し、その値をそのまま保持する */
//					if(gt_recw_ReEqpStrInfTbl_3G.frqband != ltReEqpInf3g_p->cpristr_inf.frqband)
//					{	
//						gt_recw_ReEqpStrInfTbl_3G.frqband = CMD_IVL_VAL;
//						
//					}
//					
//					/* #16(800MHz帯フィルタタイプ ): 項番#15と同様 */
//					if(gt_recw_ReEqpStrInfTbl_3G.us800m_fil_ytp != ltReEqpInf3g_p->cpristr_inf.us800m_fil_ytp)
//					{	
//						gt_recw_ReEqpStrInfTbl_3G.us800m_fil_ytp = CMD_IVL_VAL;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					}
//
//					/* #17(2GHzキャリア数): */
//					/* 対応周波数帯に0を付与し、その値をそのまま保持する */
//					if(gt_recw_ReEqpStrInfTbl_3G.us2g_car_no != ltReEqpInf3g_p->cpristr_inf.us2g_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_3G.us2g_car_no = 0;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					
//					}
//					/*  #18(800MHzキャリア数):項番#17と同様 */
//					if(gt_recw_ReEqpStrInfTbl_3G.us800m_car_no != ltReEqpInf3g_p->cpristr_inf.us800m_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_3G.us800m_car_no = 0;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//	
//					}
//					/* #19(1.7GHzキャリア数):  項番#17と同様 */
//					if(gt_recw_ReEqpStrInfTbl_3G.us800m_car_no != ltReEqpInf3g_p->cpristr_inf.us17g_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_3G.us17g_car_no = 0;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					
//					}
//					/** #20(RE側TRX-INFタイプ情報):  */
//					/*TODO*/ /*TBD*/
//					
//					/* #21(1.5GHzキャリア数): 項番#17と同様 */
//					if(gt_recw_ReEqpStrInfTbl_3G.us15g_car_no != ltReEqpInf3g_p->cpristr_inf.us15g_car_no)
//					{	
//						gt_recw_ReEqpStrInfTbl_3G.us15g_car_no = 0;
//						/* FHMのカード基本監視項目をALMにします。 */
//						lwFhmAlmFlag = CMD_TRUE;
//					}
//				}
//			}
//		}
//	
//	/*Update FHM ALM bit*/
//	 m_rc_SetFhmAlm( lwFhmAlmFlag );
//	
//	//return;
//	if( CMD_FALSE == lwFirstElement )
//	{
//		/*The synthesis is happened */
//		return CMD_RES_OK;
//	}
//	else
//	{
//		/*The synthesis is not  happened */
//		return CMD_RES_EMPTY;
//	}
//}

/********************************************************************************************************************/
/**
 *  @brief  Send response message to REC
 *  @note 
 *  @param adwSysType [in] System type
 *  @return None
 *  @date   2015/10/27 TDI)satou FHM技説-QA-113
 */
/********************************************************************************************************************/
VOID m_rc_ReStrRepResSnd( UINT adwSysType )
{
	CMT_TSKIF_CPRISND_EQPSTRRES_S3G*  lRspLTE_p;							/** Pointer of response message of LTE system*/ 
	CMT_TSKIF_CPRISND_EQPSTRRES*      lRsp3G_p;								/** Pointer of response message of 3G system*/
	USHORT                            lwBufget_rslt;						/** result of getting buffer*/
	T_RRH_EQP_S3G                     ltSynReEqpInfShmlte;					/** Synthesized Re equipment infromation that is get from shared memory (LTE)*/
	T_RRH_EQP_3G                      ltSynReEqpInfShm3g;					/** Synthesized Re equipment infromation that is get from shared memory (3G)*/
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start:  System type: %d\n",__LINE__,__FUNCTION__, adwSysType);
#endif
	
	/**Message for LTE */
	if( CMD_SYS_LTE == adwSysType)
	{
		//NR independent mode or LTE + NR mode
		if(cmw_connectoperatemode != CMD_OP_LTE)
		{
			return;
		}

		/* ポインタ初期化 */
		lRspLTE_p = NULL;
		/****************/
		/* バッファ獲得 */
		/****************/
		lwBufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
										sizeof(CMT_TSKIF_CPRISND_EQPSTRRES_S3G),
										CMD_NUM1,
										(VOID **)&lRspLTE_p	);
		/********************/
		/* バッファ獲得失敗 */
		/********************/
		if(lwBufget_rslt != CMD_RES_OK)
		{
			/* ASSERT */
			cm_Assert(	CMD_ASL_USELOW,
						lwBufget_rslt,
						"m_rc_ReStrRep cm_L2BfGet NG");

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
			
			return;
		}
	
		/* CPRI信号応答結果 */
	    lRspLTE_p->cpridat_eqpstrres.result = CMD_NML;
	
		/* CPRI信号種別設定 */
		lRspLTE_p->cpridat_eqpstrres.signal_kind = CMD_CPRID_EQPSTRRES + adwSysType;
		
		/* Get synthesized data from shared memory */
		if( CMD_OK != f_cmn_com_booteqp_s3g_get( &ltSynReEqpInfShmlte ) )
		{
			
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
			
			return;
		}
		
		/**copy data to the message*/
		memcpy( &(lRspLTE_p->cpridat_eqpstrres.cpristr_inf), &(ltSynReEqpInfShmlte.eqp_inf), sizeof(CPRIEQPSTR_INF_S3G) );
		/* CPRI信号種別設定 */
		/**********************/
		/* タスク間ヘッダ作成 */
		/**********************/
		lRspLTE_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)
									+	sizeof(CMT_CPRIF_EQPSTRRES_S3G);
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s SignalKind: %x\n",__LINE__,__FUNCTION__, lRspLTE_p->cpridat_eqpstrres.signal_kind);
#endif
		/*  RE装置構成情報報告応答送信 */
		m_cm_L3MsgSend_REC(lRspLTE_p);

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		return ;		
	}
	else
	{
		/* ポインタ初期化 */
		lRsp3G_p = NULL;
		/****************/
		/* バッファ獲得 */
			/****************/
		lwBufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
							sizeof(CMT_TSKIF_CPRISND_EQPSTRRES),
							CMD_NUM1,
							(VOID **)&lRsp3G_p	);
		/********************/
		/* バッファ獲得失敗 */
		/********************/
		if(lwBufget_rslt != CMD_RES_OK)
		{
			/* ASSERT */
			cm_Assert(	CMD_ASL_USELOW,
				lwBufget_rslt,
				"m_rc_ReStrRep cm_L2BfGet NG");
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
			
			return;
		}

		/* CPRI信号応答結果にOKを設定 */
	    lRsp3G_p->cpridat_eqpstrres.result = CMD_RPT_NG;

		/* CPRI信号種別設定 */
		lRsp3G_p->cpridat_eqpstrres.signal_kind = CMD_CPRID_EQPSTRRES;
		
		/* Get synthesized data from shared memory */
		if( CMD_OK != f_cmn_com_booteqp_3g_get( &ltSynReEqpInfShm3g ) )
		{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
			
			return;
		}
		
		/**copy data to the message*/
		memcpy( &(lRsp3G_p->cpridat_eqpstrres.cpristr_inf), &(ltSynReEqpInfShm3g.eqp_inf), sizeof(CPRIEQPSTR_INF) );

		/* CPRI信号種別設定 */
		/**********************/
		/* タスク間ヘッダ作成 */
		/**********************/
		lRsp3G_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)
									+	sizeof(CMT_CPRIF_EQPSTRRES);
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s SignalKind: %x\n",__LINE__,__FUNCTION__, lRsp3G_p->cpridat_eqpstrres.signal_kind);
#endif
		/*  RE装置構成情報報告応答送信 */
		m_cm_L3MsgSend_REC(lRsp3G_p);
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	return;
}

/********************************************************************************************************************/
/**
 *  @brief  init table that save information of all RE (LTE)
 *  @note 
 *  @param -
 *  @return None
 *  @date
 */
/********************************************************************************************************************/

VOID m_rc_ReEqpStrInfInitTable_LTE()
{
	USHORT lwLoop;
	/* 初期化 response to REC table */

	/* メーカ名:  F */
	gt_recw_ReEqpStrInfTbl_LTE.maker       = D_RRH_IND_MAKER_NAME;
	/* 装置形番(BTSの形番号): 3201 */
	gt_recw_ReEqpStrInfTbl_LTE.bts_no      = CMD_BTS_SRE_NO;
	/* 装置号番(REの番号):1 */
	gt_recw_ReEqpStrInfTbl_LTE.eqp_no      = 1;
	/* RE種別(3:RRE/LRE): 3 */
	gt_recw_ReEqpStrInfTbl_LTE.rekind      = 3;
	/* 3GBTS共用方式: 0 */
	gt_recw_ReEqpStrInfTbl_LTE.bts_share_type=0;
	/* 未規定項目:65535 */
	gt_recw_ReEqpStrInfTbl_LTE.dummy1      = CMD_IVL_VAL;
	/* 電源種別:0 */;
	gt_recw_ReEqpStrInfTbl_LTE.powerkind   = 0;
	/* 受信アンテナ数(CPRIリンク装置構成情報#8): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.rcv_antena_no= CMD_IVL_VAL;
	/* 送信アンテナ数(CPRIリンク装置構成情報#9): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.snd_antena_no = CMD_IVL_VAL;
	/* パワークラス単位: 0 */
	gt_recw_ReEqpStrInfTbl_LTE.powerunit = 0;
	/* パワークラス(0系): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.powerclass_0 = CMD_IVL_VAL;
	/* パワークラス(1系): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.powerclass_1 = CMD_IVL_VAL;
	/* パワークラス(2系): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.powerclass_2 = CMD_IVL_VAL;
	/* パワークラス(3系): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.powerclass_3 = CMD_IVL_VAL;
	/* 未規定項目:65535 */
	gt_recw_ReEqpStrInfTbl_LTE.dummy2[0] = CMD_IVL_VAL;
	/* 未規定項目:65535 */
	gt_recw_ReEqpStrInfTbl_LTE.dummy2[1] =  CMD_IVL_VAL;
	/* 対応周波数帯:0 */
	gt_recw_ReEqpStrInfTbl_LTE.frqband = 0;
	/* 最大周波数帯域幅: 4 */
	gt_recw_ReEqpStrInfTbl_LTE.max_bandwidth = 4;
	/* キャリア数(2GHz): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.us2g_car_no = CMD_IVL_VAL;
	/* (キャリア数(1.5GHz): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.us15g_car_no = CMD_IVL_VAL;
	/* (キャリア数(800MHz): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.us800m_car_no = CMD_IVL_VAL;
	/* (キャリア数(700MHz): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.us700m_car_no = CMD_IVL_VAL;
	/* (キャリア数(1.7GHz)): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.us17g_car_no = CMD_IVL_VAL;
	/* (キャリア数(1.7GHz LowBand)): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.dummy3[0] = CMD_IVL_VAL;
	/* TRX-INFタイプ: 0 */
	gt_recw_ReEqpStrInfTbl_LTE.trx_inf_type = 0;
	/* (キャリア数(3.5GHz)): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.us35g_car_no = CMD_IVL_VAL;
	/* (パワークラス(4系)): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.powerclass_4 = CMD_IVL_VAL;
	/* (パワークラス(5系)): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.powerclass_5 = CMD_IVL_VAL;
	/* (パワークラス(6系)): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.powerclass_6 = CMD_IVL_VAL;
	/* (パワークラス(7系)): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.powerclass_7 = CMD_IVL_VAL;
	/* (キャリア数(3.4GHz)): 65535 */
	gt_recw_ReEqpStrInfTbl_LTE.us34g_car_no = CMD_IVL_VAL;
	/* 未規定項目:65535 */
	/* 注意!!：エリアが追加された場合は、下の「D_RRH_EQP_INF_REM」のdefine定義を見直す必要あり */
	for(lwLoop = 0; lwLoop < D_RRH_EQP_INF_REM; lwLoop++)
	{
		gt_recw_ReEqpStrInfTbl_LTE.dummy4[lwLoop] = CMD_IVL_VAL;
	}
		
    return;
}

/********************************************************************************************************************/
/**
 *  @brief  init table that save information of all RE (3G)
 *  @note 
 *  @param -
 *  @return None
 *  @date   2015/10/22 TDI)satou FHM技説102-005, FHM技説-設計変更-024
 */
/********************************************************************************************************************/

VOID m_rc_ReEqpStrInfInitTable_3G()
{
    USHORT lwLoop;
    /* 初期化 response to REC table */

    /* メーカ名:  F */
    gt_recw_ReEqpStrInfTbl_3G.maker       =   D_RRH_IND_MAKER_NAME;
    /* BTS形番(BTSの形番号): 3001 */
    gt_recw_ReEqpStrInfTbl_3G.bts_no      =   CMD_BTS_NO;
    /* RE種別(2:RRE・LRE/OF-TRX): 3 */
    gt_recw_ReEqpStrInfTbl_3G.rekind      =   2;
    /* 装置号番:1 */
    gt_recw_ReEqpStrInfTbl_3G.eqp_no      =   1;
    /* 未規定項目:65535 */
    gt_recw_ReEqpStrInfTbl_3G.dummy1[0]   = CMD_IVL_VAL;
    /* 未規定項目:65535 */
    gt_recw_ReEqpStrInfTbl_3G.dummy1[1]   = CMD_IVL_VAL;
    /* 未規定項目:65535 */
    gt_recw_ReEqpStrInfTbl_3G.dummy1[2]   = CMD_IVL_VAL;
    /* 未規定項目:65535 */
    gt_recw_ReEqpStrInfTbl_3G.dummy1[3]   = CMD_IVL_VAL;
    /* 未規定項目:65535 */
    gt_recw_ReEqpStrInfTbl_3G.dummy1[4]   = CMD_IVL_VAL;
    /* 未規定項目:65535 */
    gt_recw_ReEqpStrInfTbl_3G.dummy1[5]   = CMD_IVL_VAL;
    /* 電源種別:0 ※報告時に共有メモリから取得し、再設定する */
    gt_recw_ReEqpStrInfTbl_3G.powerkind   = 0;
    /* パワークラス単位: 0  */
    gt_recw_ReEqpStrInfTbl_3G.powerunit   = 0;
    /* パワークラス(0系)):65535 */
    gt_recw_ReEqpStrInfTbl_3G.powerclass_0 = CMD_IVL_VAL;
    /* パワークラス(1系)):65535 */
    gt_recw_ReEqpStrInfTbl_3G.powerclass_1 = CMD_IVL_VAL;
    /* 対応周波数帯:65535 */
    gt_recw_ReEqpStrInfTbl_3G.frqband     = CMD_IVL_VAL;
    /* 800MHz帯フィルタタイプ: 65535  */
    gt_recw_ReEqpStrInfTbl_3G.us800m_fil_ytp = CMD_IVL_VAL;
    /* 2GHzキャリア数: 0 */
    gt_recw_ReEqpStrInfTbl_3G.us2g_car_no = 0;
    /* 800MHzキャリア数: 0 */
    gt_recw_ReEqpStrInfTbl_3G.us800m_car_no = 0;
    /* 1.7GHzキャリア数: 0 */
    gt_recw_ReEqpStrInfTbl_3G.us17g_car_no = 0;
    /* RE側TRX-INFタイプ情報: 0 */
    gt_recw_ReEqpStrInfTbl_3G.trx_inf_type = 0;
    /* 1.5GHzキャリア数: 0 */
    gt_recw_ReEqpStrInfTbl_3G.us15g_car_no = 0;
    /* 未規定項目: 65535 */
    for(lwLoop = 00; lwLoop<43;lwLoop++)
    {
		gt_recw_ReEqpStrInfTbl_3G.dummy2[lwLoop] = CMD_IVL_VAL;
    }

    return;
}

/********************************************************************************************************************/
/**
 *  @brief  Get max value
 *  @note 
 *  @param awVal1 [in] The first value need to compare
 *  @param awVal2 [in] The second value need to compare
 *  @return Maximum value 
 *  @date 
 */
/********************************************************************************************************************/

USHORT m_rc_GetMaxValue(USHORT awVal1, USHORT awVal2 )
{
	return (awVal1 > awVal2) ? awVal1 : awVal2;

}
	
/** @} */

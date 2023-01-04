/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ExtDataRcvBuildData.c
 *  @brief  
 *  @date   2015/08/03 FPT)VietCV Create new
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 
 */
/********************************************************************************************************************/

/** @addtogroup L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"

/********************************************************************************************************************/
/**
 *  @brief    get alm A
 *  @note     get alm A
 *  
 *  @return   RETURN almA
 *  @date     2015/08/03 FPT)VietCV Create new
 *  @date     2015/10/01 TDI)satou T.B.D解消
 *  @date     2015/10/01 TDI)satou 戻り値のエンディアン変換を追加
 *  @date     2015/10/03 FPT)Yen correct D12 value
 *  @date     2015/12/04 TDI)satou IT3問処番号No.108
 */
/********************************************************************************************************************/

USHORT m_rc_get_AlmA()
{
	T_RRH_FHM_STATE 	lt_fhmState;
	T_RRH_TX_ONOFF      lt_TxOnOff;
	USHORT 				lw_almA = 0;

	/* get fhm state from share memory */
	f_cmn_com_fhm_state_get(&lt_fhmState);

	/* get lw_rcw_FhmOnOffSta from share memory */
	f_cmn_com_tx_onoff_get(&lt_TxOnOff);
	if (lt_TxOnOff.onoff_fhm == CMD_ON)
	{
		lw_almA |= ALM_D15_BIT;
	}
	
	/* FHM ERR (FHMのカード基本監視項目(LTE)のERR状態。 　FHMの状態のみを対象とする） */
	if ((lt_fhmState.sys[CMD_SYS_S3G].FhmBase & RCD_CARDSV_ERR_BIT) == RCD_CARDSV_ERR_BIT)
	{
		lw_almA |= ALM_D14_BIT;
	}
	
	/* FHM ALM (FHMのカード基本監視項目(LTE)のALM状態。 　FHMの状態のみを対象とする） */
	if ((lt_fhmState.sys[CMD_SYS_S3G].FhmBase & RCD_CARDSV_ALM_BIT) == RCD_CARDSV_ALM_BIT)
	{
		lw_almA |= ALM_D13_BIT;
	}
	
	/* FHM ACT (FHMのカード基本監視項目(LTE)のACT状態。 　FHMの状態のみを対象とする） */
	/* 0：正常運用状態, 1：上記以外 */
	if ((lt_fhmState.sys[CMD_SYS_S3G].FhmBase & RCD_CARDSV_ACT_BIT) != RCD_CARDSV_ACT_BIT)
	{
		lw_almA |= ALM_D12_BIT;
	}
	
	/* FHM FAN異常 (FHMのRE状態報告応答(LTE)のFAN情報数が１以上のとき。 FHMの状態のみを対象とする） */
	if (lt_fhmState.fan_num)
	{
		lw_almA |= ALM_D4_BIT;
	}
	
	return BigLittleSwap16(lw_almA);
}
/********************************************************************************************************************/
/**
 *  @brief    get alm B
 *  @note     get alm B
 *  @param    index [in] the re number is used to get alm B
 *  @return   RETURN almB
 *  @Bug_No   M-RRU-ZSYS-01981
 *  @date     2015/08/03 FPT)VietCV Create new
 *  @date     2015/10/01 TDI)satou T.B.D解消
 *  @date     2015/10/03 FPT)Yen correct D0 value
 *  @date     2015/11/17 FPT)Yen M-RRU-ZSYS-01981 fix bug IT3 No84 大きいサイズの変数をstaticに
 *  @date     2016/01/27 TDI)satou 3001-15B-TS96
 */
/********************************************************************************************************************/
UCHAR m_rc_get_AlmB(UCHAR index)
{
	T_RRH_REST_S3G 					ltReSta;
	static T_RRH_CARDST_S3G 		ltCardSta;
	T_RRH_CPRI_STATE 				ltCpriSta;
	T_RRH_TX_ONOFF          		ltTxOnOff;
	UCHAR 							lc_almB = 0;
	T_RRH_CONN_DISC					ltShmConn;

	/* get re status from share memory */
	f_cmn_com_rest_s3g_get(index, &ltReSta);
	
	/* get re card status from share memory */
	f_cmn_com_cardst_s3g_get(index, &ltCardSta);
	
	/* get cpri state from share memory */
	f_cmn_com_cpri_state_get(&ltCpriSta);
	
	/* get from share memory */
	f_cmn_com_tx_onoff_get(&ltTxOnOff);

	/* 共有メモリからRE接続有無状態取得 */
	f_cmn_com_conn_disk_get(&ltShmConn);

	/*
	 * ※D7につきましては、TILT-INF信号での配下RE ON/OFFの制御状態に従う
	 *
	 * 1)該当配下REのCPRI使用/未使用が未使用の場合
	 *   =>ALM-B：0x00 or 0x80
	 *
	 * 2)該当配下REのCPRI使用/未使用が使用の場合
	 * 2-1)CPRIリンク(L1/L2/L3)断
	 *   =>ALM-B：0x61 or 0xE1
	 *
	 * 3)CPRIリンク正常
	 * 3-1)REカード状態報告確認タイマ(LTE)タイムアウトまたはREカード状態報告応答(LTE)のNG応答受信
	 *   =>D6：1、D5：1、D3：0、D2：0、D1：0、D0：1
	 *
	 * 3-2)REカード状態報告応答(LTE)の正常応答受信
	 *   =>D6：カード基本監視項目の実装情報の補数
	 *   =>D5：カード基本監視項目の実装情報の補数とERR情報の論理和
	 *   =>D3：カード拡張監視項目のAMP電源断情報
	 *   =>D2：カード拡張監視項目のWarmUP情報
	 *   =>D1：カード基本監視項目のALM情報
	 *   =>D0：カード基本監視項目のACT情報の補数
	 *
	 * 3-3)RE状態報告確認タイマ(LTE)タイムアウトまたはRE状態報告応答(LTE)のNG応答受信
	 *   =>D4：0
	 *
	 * 3-4) RE状態報告応答(LTE)の正常応答受信
	 *   =>D4：FAN情報数が0の場合0
	 *        FAN情報数が0以外の場合1
	 */

	/* ※D7につきましては、TILT-INF信号での配下RE ON/OFFの制御状態に従う */
	if (CMD_ON == ltTxOnOff.onoff_re[index - DEV_NUM_MIN])
	{
		lc_almB |= ALM_D7_BIT;
	}

	/*************************************************************************
	 * 1) 該当配下REのCPRI使用/未使用が未使用の場合
	 *   =>ALM-B：0x00 or 0x80
	 ************************************************************************/
	if (CMD_OFF == ltShmConn.conn_disc[index - DEV_NUM_MIN])
	{
		return lc_almB;
	}

	/*************************************************************************
	 * 2) 該当配下REのCPRI使用/未使用が使用の場合
	 * 2-1) CPRIリンク(L1/L2/L3)断
	 *     =>ALM-B：0x61 or 0xE1
	 *     ※LTEのL3断で上記条件をすべて確認できる. L1/L2断の場合、L3断も発生するため
	 ************************************************************************/
	if (CMD_OFF == ltCpriSta.healthchk_re[CMD_SYS_S3G][index - DEV_NUM_MIN])
	{
		lc_almB |= (ALM_D0_BIT | ALM_D5_BIT | ALM_D6_BIT);
		return lc_almB;
	}

	/*************************************************************************
	 * 3) CPRIリンク正常
	 * 3-1) ～ 3-2)
	 ************************************************************************/
	if ((0 == ltCardSta.signal_kind) || (CMD_NML != ltCardSta.result))
	{	/*
		 * 3-1) REカード状態報告確認タイマ(LTE)T.O または REカード状態報告応答(LTE)のNG応答受信
		 *     =>D6：1、D5：1、D3：0、D2：0、D1：0、D0：1
		 */
		lc_almB |= (ALM_D6_BIT | ALM_D5_BIT | ALM_D0_BIT);
	}
	else
	{	/* 3-2) REカード状態報告応答(LTE)の正常応答受信 */
		/*     =>D0：カード基本監視項目のACT情報の補数 */
		if ((ltCardSta.card_svitem & RCD_CARDSV_ACT_BIT) != RCD_CARDSV_ACT_BIT)
		{
			lc_almB |= ALM_D0_BIT;
		}
		
		/* =>D1：カード基本監視項目のALM情報 */
		if ((ltCardSta.card_svitem & RCD_CARDSV_ALM_BIT) == RCD_CARDSV_ALM_BIT)
		{
			lc_almB |= ALM_D1_BIT;
		}
		
		/* =>D2：カード拡張監視項目のWarmUP情報 */
		if ((ltCardSta.card_extsvitem & RCD_TRA_WARMUP) == RCD_TRA_WARMUP)
		{
			lc_almB |= ALM_D2_BIT;
		}
		
		/* =>D3：カード拡張監視項目のAMP電源断情報 */
		if ((ltCardSta.card_extsvitem & RCD_TRA_AMPDWN_S3G) == RCD_TRA_AMPDWN_S3G)
		{
			lc_almB |= ALM_D3_BIT;
		}

		/* =>D5：カード基本監視項目の実装情報の補数とERR情報の論理和 */
		if (((ltCardSta.card_svitem & RCD_CARDSV_IMP_BIT) != RCD_CARDSV_IMP_BIT)
		||  ((ltCardSta.card_svitem & RCD_CARDSV_ERR_BIT) == RCD_CARDSV_ERR_BIT))
		{
			lc_almB |= ALM_D5_BIT;
		}

		/* =>D6：カード基本監視項目の実装情報の補数 */
		if ((ltCardSta.card_svitem & RCD_CARDSV_IMP_BIT) != RCD_CARDSV_IMP_BIT)
		{
			lc_almB |= ALM_D6_BIT;
		}
	}

	/*************************************************************************
	 * 3)CPRIリンク正常
	 * 3-3) ～ 3-4)
	 ************************************************************************/
	if ((0 == ltReSta.signal_kind) || (CMD_NML != ltReSta.result))
	{	/*
		 * 3-3) RE状態報告確認タイマ(LTE)タイムアウトまたはRE状態報告応答(LTE)のNG応答受信
		 *     =>D4：0
		 */
		;
	}
	else
	{	/*
		 * 3-4) RE状態報告応答(LTE)の正常応答受信
		 *     =>D4：FAN情報数が0の場合0, FAN情報数が0以外の場合1
		 */
		if (0 != ltReSta.fan_num)
		{
			lc_almB |= ALM_D4_BIT;
		}
	}
	
	return lc_almB;
}

/********************************************************************************************************************/
/**
 *  @brief    Build 外部装置データ受信要求送信
 *  @note     Build 外部装置データ受信要求送信
 *  @param    parm_p [in] the buffer of 外部装置データ送信要求
 *  @param    response_p [out] the buffer of 外部装置データ受信要求
 *  @param    awProcessSta [in] 信号種別
 *  @return   None
 *  @Bug_No   M-RRU-ZSYS-01738
 *  @date     2015/08/03 FPT)VietCV Create new
 *  @date     2015/09/30 TDI)satou M-RRU-ZSYS-01738 信号長のエンディアン変換処理を追加
 *  @date     2015/10/06 FPT)Hai Fix bug IT1 (make almC in case FULL_STATE_RES_SIG)
 *  @date     2015/11/10 FPT)Yen fix bug IT3 No 69
 *  @date     2016/04/25 FJT)koshida 3001-15B-TS123対応(3G/LTE個別にTILT-INF信号の変化情報を作成する)
 */
/********************************************************************************************************************/
VOID m_rc_ExtDataRcvBuildData(CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p, CMT_TSKIF_CPRIRCV_EXTDATRCVREQ *response_p, UCHAR awProcessSta, USHORT awSysType)
{
	USHORT						lw_sndbyte_num;
	USHORT						lw_private_length;
	UCHAR						lw_device_number;
	USHORT						lw_almA;
	UCHAR						lc_almB;
	UCHAR						lc_almC;
	UCHAR						lc_enable_almB;
	T_RRH_ALM_ABC 				lt_alm_abc;

	if( ( NULL == parm_p ) || ( NULL == response_p ) )
	{
		return;
	}

	parm_p->cprircv_inf.link_num = D_RRH_CPRINO_REC;

	/* 共有メモリALM_ABCの呼び出し 取得用 */
	f_cmn_com_alm_abc_get(awSysType, &lt_alm_abc);
	
	/* 最新の装置状態構築 */
	/* 共通部 */
	/* port number */
	response_p->cpridat_extdatrcvreq.sndmsg[CMD_NUM0] = parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM0];
	
	/* device type */
	response_p->cpridat_extdatrcvreq.sndmsg[CMD_NUM3] = DEV_TYPE_FHM;
	
	/* 信号種別 */
	response_p->cpridat_extdatrcvreq.sndmsg[CMD_NUM4] = awProcessSta;
	if( FULL_STATE_RES_SIG == awProcessSta )
	{
		response_p->cpridat_extdatrcvreq.sndmsg[CMD_NUM4] = CHANGE_STATE_RES_SIG;
	}

	lw_sndbyte_num = CMD_NUM5;
	
	/* 個別部 */
	/* ヘルスチェック報告信号以外の場合 */
	if( HEAL_CHK_RES_SIG != awProcessSta )
	{
		/* (ALM-A) */
		lw_almA = m_rc_get_AlmA();
		lt_alm_abc.alm_a = lw_almA;
		memcpy(&(response_p->cpridat_extdatrcvreq.sndmsg[lw_sndbyte_num]), &lw_almA, sizeof(USHORT));
		lw_sndbyte_num += sizeof(USHORT);

		/* (E=1～16のALM-B) */
		for (lw_device_number = DEV_NUM_MIN; lw_device_number <= DEV_NUM_MAX; lw_device_number++)
		{
			lc_almB = m_rc_get_AlmB(lw_device_number);
			lc_enable_almB = CMD_OFF;
			
			/* 信号種別が状態報告信号か局リセット報告信号か状態変化信号（1st health check) */
			if ((STATE_RES_SIG == awProcessSta) || (RESET_RES_SIG == awProcessSta) || (FULL_STATE_RES_SIG == awProcessSta))
			{
				/* need make ALM-B */
				lc_enable_almB = CMD_ON;
			}
			else if (lt_alm_abc.alm_b[lw_device_number - DEV_NUM_MIN] != lc_almB)
			{
				/* need make ALM-B */
				lc_enable_almB = CMD_ON;
			}
			/* 配下REリセット報告信号/配下REON/OFF報告信号の場合、制御対象REのALM-Bを必ず報告 */
			else if ((SUBRE_RESET_RES_SIG == awProcessSta) || (SUBRE_ONOFF_RES_SIG == awProcessSta))
			{
			    if (lw_device_number == D_L3_CNVBCD_DEC(parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM5]))
			    {
			        lc_enable_almB = CMD_ON;
			    }
			}
			
			lt_alm_abc.alm_b[lw_device_number - DEV_NUM_MIN] = lc_almB;
			
			/* need to build ALM-B */
			if( CMD_ON == lc_enable_almB )
			{
				response_p->cpridat_extdatrcvreq.sndmsg[lw_sndbyte_num] = D_L3_CNVBCD(lw_device_number);
				lw_sndbyte_num += sizeof(UCHAR);
				
				response_p->cpridat_extdatrcvreq.sndmsg[lw_sndbyte_num] = lc_almB;
				lw_sndbyte_num += sizeof(UCHAR);
			}
		}

		/* (E=99のALM-C) */
		/* 信号種別が状態報告信号か局リセット報告信号の場合 */
		if ((STATE_RES_SIG == awProcessSta) || (RESET_RES_SIG == awProcessSta) || (FULL_STATE_RES_SIG == awProcessSta))
		{
			lw_device_number = 99;
			response_p->cpridat_extdatrcvreq.sndmsg[lw_sndbyte_num] = D_L3_CNVBCD(lw_device_number);
			lw_sndbyte_num += sizeof(UCHAR);

			lc_almC = CMD_NUM0;
			lt_alm_abc.alm_c = lc_almC;
			response_p->cpridat_extdatrcvreq.sndmsg[lw_sndbyte_num] = lc_almC;
			lw_sndbyte_num += sizeof(UCHAR);
		}
	}
	
	/* length of private message */
	lw_private_length = 0xD000 + CMD_NUM2 * (lw_sndbyte_num - CMD_NUM5);
	lw_private_length = BigLittleSwap16(lw_private_length);
	memcpy(&(response_p->cpridat_extdatrcvreq.sndmsg[CMD_NUM1]), (void*)&lw_private_length, sizeof(USHORT));

	response_p->cpridat_extdatrcvreq.sndbyte_num = lw_sndbyte_num;

	/* 共有メモリALM_ABCの呼び出し 設定用 */
	f_cmn_com_alm_abc_set(awSysType, &lt_alm_abc);

	/* タスク間フレームのデータ作成 */
	response_p->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
	response_p->head.uiSrcTQueueID = CMD_TSKID_RCT;
	response_p->head.uiLength      = sizeof(CMT_TSKIF_CPRIRCV_EXTDATSNDREQ)
	                               - sizeof(response_p->cpridat_extdatrcvreq.sndmsg)
	                               + response_p->cpridat_extdatrcvreq.sndbyte_num;

	return;
}

/** @} */

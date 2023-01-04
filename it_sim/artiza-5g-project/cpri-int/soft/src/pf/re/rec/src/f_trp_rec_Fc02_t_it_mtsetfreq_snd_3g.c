/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_it_mtsetfreq_snd.c
 * @brief  スレッド間(MTアドレス設定(FLD-MT)要求)送信
 * @date   2009/01/19 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  スレッド間(MTアドレス設定(FLD-MT)要求)送信
 * @note   スレッド間(MTアドレス設定(FLD-MT)要求)を送信する
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  -
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/19 TDI)satou 共有メモリから設定値を取得して設定するように
 * @date   2015/10/05 TDI)satou LTEのCPRI番号を参照していた不具合を修正
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_mtsetfreq_snd_3g(                           /* なし:R */
) {
	CMT_TSKIF_CPRISND_MTADDSETREQ	ltReq;
	INT ldwRet;
	T_RRH_MTADDSET shm_mtaddr_set;

	/* 共有メモリから設定値を取得(RECから受信したデータ。未受信の場合は初期値 */
    if (D_RRH_OK != f_cmn_com_mtaddset_get(CMD_SYS_3G, CMD_FLD_MT, &shm_mtaddr_set)) {
        cm_Assert(CMD_ASL_USEHIGH, CMD_NUM0, "f_cmn_com_mtaddset_get NG");
        return;
    }
	
	/*data seting*/
	ltReq.head.uiLength = sizeof( CMT_TSKIF_HEAD ) + sizeof( CMT_CPRISIGSND_INF ) + sizeof( CMT_CPRIF_MTADDSETREQ );
	ltReq.cprisnd_inf.link_num = D_REC_C02_3G_CPR_NO();  /* CPRI番号 */
	
	ltReq.cpridat_mtaddsetreq.signal_kind = CMD_CPRID_MTADDSETREQ;
	ltReq.cpridat_mtaddsetreq.tgtmt       = CMD_FLD_MT;                 /* 対象MT(FLD-MT)           */
	ltReq.cpridat_mtaddsetreq.ipadd_1     = shm_mtaddr_set.ipadd_1;     /* IPアドレス#1             */
	ltReq.cpridat_mtaddsetreq.ipadd_2     = shm_mtaddr_set.ipadd_2;     /* IPアドレス#2             */
	ltReq.cpridat_mtaddsetreq.ipadd_3     = shm_mtaddr_set.ipadd_3;     /* IPアドレス#3             */
	ltReq.cpridat_mtaddsetreq.ipadd_4     = shm_mtaddr_set.ipadd_4;     /* IPアドレス#4             */
	ltReq.cpridat_mtaddsetreq.sbnetmsk_1  = shm_mtaddr_set.sbnetmsk_1;  /* サブネットマスク#1       */
	ltReq.cpridat_mtaddsetreq.sbnetmsk_2  = shm_mtaddr_set.sbnetmsk_2;  /* サブネットマスク#2       */
	ltReq.cpridat_mtaddsetreq.sbnetmsk_3  = shm_mtaddr_set.sbnetmsk_3;  /* サブネットマスク#3       */
	ltReq.cpridat_mtaddsetreq.sbnetmsk_4  = shm_mtaddr_set.sbnetmsk_4;  /* サブネットマスク#4       */
	ltReq.cpridat_mtaddsetreq.cmdtrsport  = shm_mtaddr_set.cmdtrsport;  /* コマンド転送用ポート番号 */
	ltReq.cpridat_mtaddsetreq.filtrsport  = shm_mtaddr_set.filtrsport;  /* ファイル転送用ポート番号 */
	ltReq.cpridat_mtaddsetreq.portno      = shm_mtaddr_set.portno;      /* ポート番号               */
	
	/* Send msg [MTアドレス設定(SV-MT)要求] to l2/lpb */
	ldwRet = f_trp_rec_Fcom_t_l2_msg_fw( &ltReq, sizeof(CMT_TSKIF_CPRISND_MTADDSETREQ) );
	
	/*Send msg NG*/
	if( ldwRet == D_RRH_NG )
	{
		cm_Assert(	CMD_ASL_USELOW,
						CMD_NUM0, 
						"f_trp_rec_Fc02_t_it_mtsetfreq_snd send msg NG");
	}
	
	
    return;
}

/****************************************************************************/
/*!
 * @brief  スレッド間(MTアドレス設定(FLD-MT)要求)送信
 * @note   スレッド間(MTアドレス設定(FLD-MT)要求)を送信する
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  -
 * @return None
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/19 TDI)satou 共有メモリから設定値を取得して設定するように
 * @date   2015/10/05 TDI)satou LTEのCPRI番号を参照していた不具合を修正
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_mtsetsreq_snd_3g(                           /* なし:R */
) {
	CMT_TSKIF_CPRISND_MTADDSETREQ	ltReq;
	INT ldwRet;
    T_RRH_MTADDSET shm_mtaddr_set;

    /* 共有メモリから設定値を取得(RECから受信したデータ。未受信の場合は初期値 */
    if (D_RRH_OK != f_cmn_com_mtaddset_get(CMD_SYS_3G, CMD_SV_MT, &shm_mtaddr_set)) {
        cm_Assert(CMD_ASL_USEHIGH, CMD_NUM0, "f_cmn_com_mtaddset_get NG");
        return;
    }

	/*data seting*/
	ltReq.head.uiLength = sizeof( CMT_TSKIF_HEAD ) + sizeof( CMT_CPRISIGSND_INF ) + sizeof( CMT_CPRIF_MTADDSETREQ );
	ltReq.cprisnd_inf.link_num = D_REC_C02_3G_CPR_NO();  /* CPRI番号 */
	
    ltReq.cpridat_mtaddsetreq.signal_kind = CMD_CPRID_MTADDSETREQ;
    ltReq.cpridat_mtaddsetreq.tgtmt       = CMD_SV_MT;                  /* 対象MT(FLD-MT)           */
    ltReq.cpridat_mtaddsetreq.ipadd_1     = shm_mtaddr_set.ipadd_1;     /* IPアドレス#1             */
    ltReq.cpridat_mtaddsetreq.ipadd_2     = shm_mtaddr_set.ipadd_2;     /* IPアドレス#2             */
    ltReq.cpridat_mtaddsetreq.ipadd_3     = shm_mtaddr_set.ipadd_3;     /* IPアドレス#3             */
    ltReq.cpridat_mtaddsetreq.ipadd_4     = shm_mtaddr_set.ipadd_4;     /* IPアドレス#4             */
    ltReq.cpridat_mtaddsetreq.sbnetmsk_1  = shm_mtaddr_set.sbnetmsk_1;  /* サブネットマスク#1       */
    ltReq.cpridat_mtaddsetreq.sbnetmsk_2  = shm_mtaddr_set.sbnetmsk_2;  /* サブネットマスク#2       */
    ltReq.cpridat_mtaddsetreq.sbnetmsk_3  = shm_mtaddr_set.sbnetmsk_3;  /* サブネットマスク#3       */
    ltReq.cpridat_mtaddsetreq.sbnetmsk_4  = shm_mtaddr_set.sbnetmsk_4;  /* サブネットマスク#4       */
    ltReq.cpridat_mtaddsetreq.cmdtrsport  = shm_mtaddr_set.cmdtrsport;  /* コマンド転送用ポート番号 */
    ltReq.cpridat_mtaddsetreq.filtrsport  = shm_mtaddr_set.filtrsport;  /* ファイル転送用ポート番号 */
    ltReq.cpridat_mtaddsetreq.portno      = shm_mtaddr_set.portno;      /* ポート番号               */

	/* Send msg [MTアドレス設定(SV-MT)要求] to l2/lpb */
	ldwRet = f_trp_rec_Fcom_t_l2_msg_fw( &ltReq, sizeof(CMT_TSKIF_CPRISND_MTADDSETREQ) );
	
	/*Send msg NG*/
	if( ldwRet == D_RRH_NG )
	{
		cm_Assert(	CMD_ASL_USELOW,
						CMD_NUM0, 
						"f_trp_rec_Fc02_t_it_mtsetfreq_snd send msg NG");
	}
	
	
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

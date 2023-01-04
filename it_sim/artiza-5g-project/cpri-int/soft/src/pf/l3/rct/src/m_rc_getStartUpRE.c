/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_getStartUpRE.c
 *  @brief  立ち上げ対象RE特定
 *  @date   2015/08/07 TDIPS)maruyama create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"
#include "m_rc_header.h"
#include "f_rrh_def.h"
/*********************************************************************************/
/**
 *  @brief  立ち上げ対象RE特定
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01778
 *  @Bug_No M-RRU-ZSYS-01967
 *  @date   2015/08/07 TDIPS)maruyama create
 *  @date   2015/08/24 TDIPS)maruyama modify RE起動待ちタイマの起動フラグの設定判定を見直し、変数の型変更
 *  @date   2015/08/26 TDIPS)maruyama modify REの取得を１からに変更、全REが対象外の場合の処理追加、
 *  @date   2015/08/26 TDIPS)maruyama modify 起動対象REがある場合の関数呼び出し位置変更
 *  @date   2015/09/02 TDIPS)maruyama modify 起動対象REがない場合の関数変更（m_rc_infoAllRE()に変更）
 *  @date   2015/10/15 TDI)satou M-RRU-ZSYS-01778 問処番号No.83対応
 *  @date   2015/11/07 TDIPS)sasaki M-RRU-ZSYS-01967 IT2問処No.160対処 起動確認待ちタイマ起動条件変更
 *  @date   2015/11/13 TDI)satou その他未実装-013
 *  @date   2016/03/29 TDI)satou 配下RE0台起動の場合、遅延補正状態クリアをPFプロセスに依頼するように
 */
/*********************************************************************************/
VOID m_rc_getStartUpRE()
{
	UINT	re_cnt			= 1;				/* ループカウンタ	*/
	USHORT	re_infoStartup	= D_RRH_OFF;		/* 立ち上げRE(D_RRH_ON(=1)=立ち上げ対象、D_RRH_OFF(=0)=対象外）	*/
	UINT	re_stup_flg		= D_RRH_OFF;		/* RE起動待ちタイマの起動フラグ(D_RRH_ON(=1)=起動する、D_RRH_OFF(=0)=起動しない）*/
	T_RRH_EQP_3G      shmEqp3G;
	T_RRH_EQP_S3G     shmEqpS3G;
// [16B] chg start
//	T_RRH_SLOT_3G     shmSlot3G;
//	T_RRH_SLOT_S3G    shmSlotS3G;
	T_RRH_SLOT_3G_RE     shmSlot3G;
	T_RRH_SLOT_S3G_RE    shmSlotS3G;
// [16B] chg end
	T_RRH_TOFFSET_3G  shmReDelayInf3G;
	T_RRH_TOFFSET_S3G shmReDelayInfS3G;
	static T_RRH_DELAY_INF shmDelayInf;

	memset(&shmEqp3G,         0, sizeof shmEqp3G        );
	memset(&shmEqpS3G,        0, sizeof shmEqpS3G       );
	memset(&shmSlot3G,        0, sizeof shmSlot3G       );
	memset(&shmSlotS3G,       0, sizeof shmSlotS3G      );
	memset(&shmReDelayInf3G,  0, sizeof shmReDelayInf3G );
	memset(&shmReDelayInfS3G, 0, sizeof shmReDelayInfS3G);
	memset(re_startupREQ,     0, sizeof re_startupREQ   );
	memset(re_startupRSP,     0, sizeof re_startupRSP   );
	memset(re_startup,        0, sizeof re_startup      );
		
	/* 共有メモリから立ち上げREの取得	*/
	for( re_cnt = D_RRH_CPRINO_RE_MIN ; re_cnt <= D_RRH_CPRINO_RE_MAX ; re_cnt++ )
	{
	    /* RECへの報告を作るのに必要な共有メモリ初期化 */
	    f_cmn_com_eqp_3g_set(re_cnt, &shmEqp3G);
	    f_cmn_com_eqp_s3g_set(re_cnt, &shmEqpS3G);
	    f_cmn_com_slot_3g_set(re_cnt, &shmSlot3G);
	    f_cmn_com_slot_s3g_set(re_cnt, &shmSlotS3G);
	    f_cmn_com_toffset_3g_set(re_cnt, &shmReDelayInf3G);
	    f_cmn_com_toffset_s3g_set(re_cnt, &shmReDelayInfS3G);

		re_infoStartup 		= m_rc_get_startupREinfo(re_cnt);	/* 共有メモリから取得	*/
		re_startup[re_cnt]	= re_infoStartup;					/* 立ち上げ情報データを格納	*/

		if( re_infoStartup == D_RRH_ON )
		{
			/* RE起動待ちタイマの起動フラグ設定		*/
			re_stup_flg = D_RRH_ON;			/* フラグをセットする	*/

			if(fhm_StartUp_flg == D_RRH_ON)
			{
				/* RE CPRI Link Start NoticeをcprにLOGなし(SFP誤実装判定不要)通知		*/
				m_rc_SendMsg_reCpriLinkSat(re_cnt, 0);
			}
			else
			{
				/* RE CPRI Link Start NoticeをcprにLOGあり(SFP誤実装判定要)通知		*/
				m_rc_SendMsg_reCpriLinkSat(re_cnt, E_RRH_SFPLOG_TRIGGER_CARD_START);
			}
			if (CMD_SYS_S3G == cmw_validsysinfo) {
			    re_startupREQ[re_cnt][CMD_SYS_S3G] = D_RRH_ON;
			} else {
			    re_startupREQ[re_cnt][CMD_SYS_3G ] = D_RRH_ON;
			    re_startupREQ[re_cnt][CMD_SYS_S3G] = D_RRH_ON;
			}
		}
	}
	/*	FHM起動済み(SFP実装判定不要)	*/
	fhm_StartUp_flg = D_RRH_ON;
	
	/* １つでも起動対象のREあり */
	if(re_stup_flg == D_RRH_ON)	
	{
		/* 遅延未補正 */
		if(delay_adjust_step3 == D_RRH_OFF)
		{
			/* 起動確認待ちタイマ開始	*/
			m_rc_StartUpRE_timerset();
		}
	}
	else
	{	/*
		 * 遅延補正情報初期化.
		 * 配下RE接続ありの状態で遅延補正が完了し、その後に全ての配下REを減設してから
		 * RECをつなげなおすと、遅延補正実施せずにRECと接続され、前の遅延補正の結果が残ってしまう.
		 * 配下RE接続なしの状態で電源ONした時と同じ動作にするため、初期化しておく.
		 *
		 * ないと思われるが、PFプロセスで共有メモリ初期化する前にRECからToffset値報告要求を受信して
		 * しまうと、前の結果がRECに通知されてしまう可能性がある. 念のためここで共有メモリはクリアしておく.
		 */
		memset(&shmDelayInf, 0, sizeof shmDelayInf);
		f_cmn_com_delay_inf_set(&shmDelayInf);
		(VOID)rrhApi_Cpri_Mnt_DelayAdjustClear(D_RRH_PROCQUE_L3);

		/* 立ち上げ対象REが存在しない		*/
		/* 全REの起動完了処理までスキップ	*/
        /* 装置情報 */
        m_rc_createEqpinfo();

        /* スロット情報 */
        m_rc_createREslotinfo();

        /* REC CPRI Link Start Notice通知 */
        m_rc_SendMsg_recCPRILinkSta();

        /* RE起動完了 */
        re_StartUp_CPRI_Conn_flg = D_RRH_ON;
	}

	return;
}

/*********************************************************************************/
/**
 *  @brief  立ち上げRE特定
 *  @date   2015/08/07 TDIPS)maruyama create
 *  @date   2015/08/24 TDIPS)maruyama modify 型の変更
 *  @param [in] re_no	接続確認したいRE番号
 *  @note   D_RRH_CPRINO_RE1は1始まりなので注意が必要
 *  @return 0:未接続 1:接続
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/
USHORT m_rc_get_startupREinfo(UINT re_no)
{
	T_RRH_CONN_DISC		conn_disc	= {};					/* RE接続有無状態データ		*/
	USHORT				ret			= 0;					/* 戻り値					*/
	
	/* 共有メモリから立ち上げREの取得	*/
	f_cmn_com_conn_disk_get(&conn_disc);
	ret = conn_disc.conn_disc[re_no-1];
	return(ret);
}


/* @} */

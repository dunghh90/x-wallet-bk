/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_infoAllRE.c
 *  @brief  立ち上がったREの情報をまとめる
 *           周波数
 *           共用方式
 *           遅延量
 *           re装置構成
 *           reスロット情報
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
#include "m_rc_ftyp.h"
/*********************************************************************************/
/**
 *  @brief  立ち上がったREの情報まとめ
 *  @return none
 *  @Bug_No M-RRU-ZSYS-01965
 *  @Bug_No M-RRU-ZSYS-01967
 *  @Bug_No M-RRU-ZSYS-01982
 *  @Bug_No M-RRU-ZSYS-01988
 *  @date   2015/08/07 TDIPS)maruyama create
 *  @date   2015/11/06 TDIPS)sasaki M-RRU-ZSYS-01965 update IT2問処No.158対処
 *  @date   2015/11/07 TDIPS)sasaki M-RRU-ZSYS-01967 update IT2問処No.160対処 static変数をグローバルに変更
 *  @date   2015/11/12 TDIPS)sasaki M-RRU-ZSYS-01982 update IT2問処No.176対処 Step7正常終了ルートで10秒タイマを停止するよう修正
 *  @date   2015/11/13 TDIPS)sasaki M-RRU-ZSYS-01988 update IT2問処No.181対処 強制停止処理追加
 *  @date   2015/11/13 TDIPS)sasaki update IT2問処No.186対処
 *  @date   2016/01/06 TDI)satou レジスタ最大値超過した場合の強制停止を追加
 *  @date   2016/02/24 TDI)enoki RE装置構成情報報告とREスロット情報報告の集約タイミングをSTEP7完了からSTEP3直前に変更
 *  @date   2016/11/16 FJT)ohashi 16B スロット数制限機能追加
 *  @date   2017/04/07 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 */
/*********************************************************************************/
VOID m_rc_infoAllRE(INT delayPrmChkEn)
{
    T_API_CPRILINK_ADJUST_DELAY_STEP7_RSP	apiRsp;
	T_RRH_CPRI_STATE						ltCpriState;
    INT										errcd;
	INT										ldwRet;
    INT										isContinueStep7 = D_RRH_FALSE;
    USHORT									cpr_num, cpr_idx;
    USHORT									tgt_re[D_RRH_CPRINO_RE_MAX] = {0};

//16B スロット数制限機能追加 start
	T_RRH_SLOT_S3G_RE	buf_LTEslotInfo_re	= {};           /* 共有メモリ取得用(配下REの報告内容 LTE) */
	T_RRH_SLOT_3G_RE	buf_3GslotInfo_re	= {};           /* 共有メモリ取得用(配下REの報告内容 3G) */
	USHORT slot_no_max; 		/* スロット情報N */
	UINT userCode;
//16B スロット数制限機能追加 end

#ifdef FHM_RCT_DEBUG_FOR_IT1
    static USHORT m_rc_infoAllRE_ncall = 0;
	printf("m_rc_infoAllRE called %d times, delay_adjust_step3[%d]\n", ++m_rc_infoAllRE_ncall, delay_adjust_step3);
#endif

    /* 遅延量算出・設定 */
    if (delay_adjust_step3 == D_RRH_OFF)
    {

        /* 装置情報 */
        m_rc_createEqpinfo();

        /* スロット情報 */
        m_rc_createREslotinfo();

        rrhApi_Cpri_Mnt_DelayAdjustStep3(D_RRH_PROCQUE_L3);

        delay_adjust_step3 = D_RRH_ON;

		cm_Assert(CMD_ASL_USELOW, CMD_NUM0, "Complete step3");

        for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
        {
            re_startup[cpr_num] = m_rc_get_startupREinfo(cpr_num);
            if (D_RRH_ON == re_startup[cpr_num])
            {
                if (CMD_SYS_S3G == cmw_validsysinfo)
                {
                    re_startupREQ[cpr_num][CMD_SYS_S3G] = D_RRH_ON;
                    re_startupRSP[cpr_num][CMD_SYS_S3G] = D_RRH_OFF;
					rec_Startup_First_flg[CMD_SYS_S3G] = D_RRH_OFF;
                }
                else
                {
                    re_startupREQ[cpr_num][CMD_SYS_3G ] = D_RRH_ON;
                    re_startupREQ[cpr_num][CMD_SYS_S3G] = D_RRH_ON;
                    re_startupRSP[cpr_num][CMD_SYS_3G ] = D_RRH_OFF;
                    re_startupRSP[cpr_num][CMD_SYS_S3G] = D_RRH_OFF;
					rec_Startup_First_flg[CMD_SYS_3G ] = D_RRH_OFF;
					rec_Startup_First_flg[CMD_SYS_S3G] = D_RRH_OFF;
                }
            }
        }

        /* Step7完了待ちタイマ起動 */
        m_rc_Step7_timerset();
    }
    else
    {
        for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
        {
            cpr_idx = cpr_num - 1;
            if ((D_RRH_ON == re_startupRSP[cpr_num][CMD_SYS_3G ])
            ||  (D_RRH_ON == re_startupRSP[cpr_num][CMD_SYS_S3G]))
            {
                tgt_re[cpr_idx] = D_RRH_ON;
            }
        }

        rrhApi_Cpri_Mnt_DelayAdjustStep7(D_RRH_PROCQUE_L3_ADJ_DLY_STEP7, delayPrmChkEn, tgt_re, &apiRsp);

        for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
        {
            cpr_idx = cpr_num - 1;
            if (E_API_RCD_NG == apiRsp.result_re[cpr_idx])
            {
                re_startupRSP[cpr_num][CMD_SYS_3G ] = D_RRH_OFF;
                re_startupRSP[cpr_num][CMD_SYS_S3G] = D_RRH_OFF;
                isContinueStep7 = D_RRH_TRUE;
            }
            else if (E_API_RCD_OVER_MAX_DELAY == apiRsp.result_re[cpr_idx])
            {   /* レジスタ設定値を超過した場合は強制停止に設定する */
                if (D_RRH_ON == re_startupRSP[cpr_num][CMD_SYS_3G])
                {
                    m_rc_ReHCStop(cpr_num, CMD_SYS_3G, E_RRH_RESTOP_FACTOR_DELAY_NG);
                }
                if (D_RRH_ON == re_startupRSP[cpr_num][CMD_SYS_S3G])
                {
                    m_rc_ReHCStop(cpr_num, CMD_SYS_S3G, E_RRH_RESTOP_FACTOR_DELAY_NG);
                }
            }
            else
            {}
        }

        if ((D_RRH_ON == delayPrmChkEn) && (D_RRH_TRUE == isContinueStep7))
        {   /* 失敗REがいる場合、該当REのCPRIは切断される.
             * この場合、Step7を継続し、起動シーケンスを完了を再度待つ. */
            return;
        }

		cm_TStop(CMD_TIMID_WAIT_COMPLETE_STEP7, &errcd);

        delay_adjust_step3 = D_RRH_OFF;

		/* Step7完了待ちタイマ タイムアウト済 */
		if(re_StartUp_CPRI_Conn_flg == D_RRH_ON)
		{
			ldwRet = f_cmn_com_cpri_state_get(&ltCpriState);	
			if(ldwRet != D_RRH_OK)
			{
				cm_Assert(CMD_ASL_USELOW, CMD_NUM0, "Get CPRI State from shared memory NG");
			}

			/* REC-FHM間のCPRIリンク断 */
			if(ltCpriState.cprista_rec != D_RRH_CPRISTAT_E)
			{
				/* RE起動処理終了フラグOFF */
				re_StartUp_CPRI_Conn_flg = D_RRH_OFF;
				return;
			}

			/* ReSync対象外で、かつ起動未完了のREを強制停止 */
			m_rc_NoRSP_HSTOP(tgt_re, &ltCpriState);
		}

//16B スロット数制限機能追加 start

		/*全リンクスロット数読み出し*/
		slot_no_max = 0;
        for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++){
			f_cmn_com_slot_s3g_get(cpr_num, &buf_LTEslotInfo_re);
			slot_no_max += buf_LTEslotInfo_re.slot_num;
			f_cmn_com_slot_3g_get(cpr_num, &buf_3GslotInfo_re);
			slot_no_max += buf_3GslotInfo_re.slot_num;
		}
		/*スロット数規定値超えで全リンク強制停止*/
		if(slot_no_max > RCD_OVER_MAX_SLOT){
			cm_Assert(CMD_ASL_USEHIGH, slot_no_max, "Over Slot Count !!" );
	        for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++){
				/* 全リンク強制停止であるため、要求したREは強制停止する */
                if (D_RRH_ON == re_startupREQ[cpr_num][CMD_SYS_3G]){
					/*3Gリンク強制停止*/
					userCode = (CMD_SYS_3G << 16) | cpr_num;
                    m_rc_ReHCStop(cpr_num, CMD_SYS_3G, E_RRH_RESTOP_FACTOR_SLOT_NG);
                }
				/* 全リンク強制停止であるため、要求したREは強制停止する */
                if (D_RRH_ON == re_startupREQ[cpr_num][CMD_SYS_S3G]){
 					/*S3Gリンク強制停止*/
					userCode = (CMD_SYS_S3G << 16) | cpr_num;
                    m_rc_ReHCStop(cpr_num, CMD_SYS_S3G, E_RRH_RESTOP_FACTOR_SLOT_NG);
                }
			}
		}
		else{
			cm_Assert(CMD_ASL_USELOW, slot_no_max, "Slot Count OK !!" );
		}

//16B スロット数制限機能追加 end
        /*初期起動が完了したREを検索*/
        for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
        {
			/*	Step7完了時にRE起動要求(LTE)を送信している場合	*/
			if( re_startupREQ[cpr_num][CMD_SYS_S3G] == D_RRH_ON )
				{
					/*	CPRI使用が確定したため、対象REのCPRI異常をREカード状態報告の故障状態に反映する	*/
					re_funcFailValid[cpr_num] = D_RRH_ON;
				}
		}

        /* REC CPRI Link Start Notice通知 */
        m_rc_SendMsg_recCPRILinkSta();

        /* RE起動完了 */
        re_StartUp_CPRI_Conn_flg = D_RRH_ON;
    }
}

/********************************************************************************************************************/
/**
 *  @brief	REC CPRI Link Start Noticeをcprに通知
 *  @return	None
 *  @date	2015/08/07	TDIPS) maruyama create
 *  @date	2015/08/26	TDIPS) maruyama modify バッファ取得
 */
/********************************************************************************************************************/
VOID m_rc_SendMsg_recCPRILinkSta()
{
	CMT_TSKIF_REC_CPRILNKSTART_NTC*	a_msg	= NULL;		/* 送信MGS		*/
	UINT	a_rtn = 0;									/* 関数戻り値	*/
	

	/* バッファ取得 */
	a_rtn = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_REC_CPRILNKSTART_NTC), (VOID **)&a_msg);
	/* 戻り値判定 */
	if( a_rtn != CMD_RES_OK ){
		/* バッファ取得失敗時はアボート処理 */
		cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_SendMsg_recCPRILinkSta",
						   "cm_BReq NG",
						   a_rtn, sizeof(CMT_TSKIF_REC_CPRILNKSTART_NTC), CMD_NUM0);
		return;
	}
	
	/* 立ち上げREなのでMSG送信			*/
	/* タスク間フレームのデータ作成 */
	a_msg->head.uiEventNo 		= CMD_TSKIF_REC_CPRI_START_NTC;		/* イベント番号			*/
	a_msg->head.uiDstPQueueID	= D_RRH_PROCQUE_L3;					/* 送信先プロセスQID	*/
	a_msg->head.uiSrcPQueueID	= D_RRH_PROCQUE_L3;					/* 送信元プロセスQID	*/
	a_msg->head.uiDstTQueueID	= CMD_TSKID_CPR;					/* 送信先スレッドQID	*/
	a_msg->head.uiSrcTQueueID	= CMD_TSKID_RCT;					/* 送信元スレッドQID	*/
	a_msg->head.uiLength 		= sizeof(CMT_TSKIF_REC_CPRILNKSTART_NTC);	/* 送信MSGサイズ	*/

	/* Send Log */
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, CMD_TSKID_RCT, CMD_TSKIF_REC_CPRI_START_NTC, (UCHAR*)a_msg);

	/* RE CPRI Link Start Noticeを送信 */
	/* cm_Enter(tskNo, tskSts, evtNo, dat_p);		*/
	/* tskNo		:送信先							*/
	/* tskSts		:タスクsts（起動事象）			*/
	/* evtNo		:イベント番号					*/
	/* dat_p		：送信MSG						*/
	a_rtn = cm_Enter(CMD_TSKID_CPR, CMD_QRB_NORMAL,
			CMD_TSKIF_REC_CPRI_START_NTC, a_msg);
	if( a_rtn != CMD_RES_OK )
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
	}
}


/* @} */

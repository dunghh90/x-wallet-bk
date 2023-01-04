/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReStr_Rsp.c
 *  @brief  RE立ち上げ応答
 *  @date   
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"

/********************************************************************************************************************/
/**
 *  @brief    RE立ち上げ応答
 *  @note     
 *  @param    buff_adr [in] メッセージデータ
 *  @date 2015/08/13 TDIPS)ikeda create
 *  @date 2016/01/29 TDI)satou eNB-F-15B-3201-00115 APIの応答をメインルートで受信するように
 *  @date 2016/03/04 TDI)satou 共用構成で一方のREC起動未完了の状態で、REの増設が動いてしまうことがあるのを動かないように \n
 *                             LTE-RECからDL中にHC(3G)再送満了でリセット→強制停止になった場合に発生する
 *  @date 2016/04/20 TDI)satou rrhApi_Cpri_Mnt_DelayAdjustStep10() の引数変更
 *  @date 2016/11/16 FJT)ohashi 16B スロット数制限機能追加
 *  @date 2017/01/26 FJT)ohashi 3201-16B-TS96対処 キューイングREのCPRI接続通知条件を追加
 *  @date 2017/04/07 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 */
/********************************************************************************************************************/

VOID m_rc_ReStr_Rsp(VOID* buff_adr)
{
	CMT_TSKIF_RESTR_RSP *parm_p;
	T_RRH_LAYER3 shmLayer3[CMD_MAX_SYSNUM];
	USHORT cpr_idx;
	E_RRHAPI_RCODE rcode;
	USHORT isStartup = D_RRH_ON;
	USHORT forceStopEn = D_RRH_ON;		/* 最大遅延量超過による強制停止への遷移 : 有効 */
	UINT rec_startup_cmp = D_RRH_OFF;
//16B スロット数制限機能追加 start
	T_RRH_SLOT_S3G_RE	buf_LTEslotInfo_re	= {};           /* 共有メモリ取得用(配下REの報告内容 LTE) */
	T_RRH_SLOT_3G_RE	buf_3GslotInfo_re	= {};           /* 共有メモリ取得用(配下REの報告内容 3G) */
    USHORT	cpr_num;
	USHORT slot_no_max; 		/* スロット情報N */
	UINT userCode;
//16B スロット数制限機能追加 end
	T_RRH_LAYER3 	layer3;

	parm_p = (CMT_TSKIF_RESTR_RSP*)buff_adr;

	/* REから3GかLTEか情報をもらいここで判断する */
	if(parm_p->system_type == CMD_SYS_S3G)
	{
		/* LTEの場合*/
		/* RE立ち上げ応答関数(LTE)をコール */
		m_rc_ReStr_Rsp_lte(parm_p);
		/*	最初のStep10後の起動完了の場合	*/
		if(re_funcFailValid[parm_p->link_num] == 2)
		{
			/*	CPRI使用が確定したため、対象REのCPRI異常をREカード状態報告の故障状態に反映する	*/
			re_funcFailValid[parm_p->link_num] = D_RRH_ON;
		}
	}
	else
	{
		/* 3Gの場合 */
		/* RE立ち上げ応答関数(3G)をコール */
		m_rc_ReStr_Rsp_3g(parm_p);
	}

	cm_Assert( CMD_ASL_USELOW, 
		(((parm_p->system_type << 24 ) & 0xFF000000 ) | ((parm_p->link_num << 16 ) & 0x00FF0000 )), 
		"RE Start Ack" );

	cpr_idx = parm_p->link_num - 1;
	f_cmn_com_layer3_get(CMD_SYS_3G,  &shmLayer3[CMD_SYS_3G ]);
	f_cmn_com_layer3_get(CMD_SYS_S3G, &shmLayer3[CMD_SYS_S3G]);

	if (CMD_SYS_S3G == cmw_validsysinfo)
	{   /* LTE単独の場合、LTEのLayer3が運用中状態なら起動完了とみなす */
	    if (E_RRH_LAYER3_RE_OPE == shmLayer3[CMD_SYS_S3G].layer3_re[cpr_idx])
	    {
//16B スロット数制限機能追加 start
			/*全リンクスロット数読み出し*/
			slot_no_max = 0;
	        for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++){
				f_cmn_com_slot_s3g_get(cpr_num, &buf_LTEslotInfo_re);
				slot_no_max += buf_LTEslotInfo_re.slot_num;
				f_cmn_com_slot_3g_get(cpr_num, &buf_3GslotInfo_re);
				slot_no_max += buf_3GslotInfo_re.slot_num;
			}
			/*スロット数規定値超えで強制停止*/
			if(slot_no_max > RCD_OVER_MAX_SLOT){
				cm_Assert(CMD_ASL_DBGHIGH, slot_no_max, "Over Slot Count !!" );
				/*S3Gリンク強制停止*/
				m_rc_ReHCStop(parm_p->link_num, CMD_SYS_S3G, E_RRH_RESTOP_FACTOR_SLOT_NG);
			}
			else{
				cm_Assert(CMD_ASL_USELOW, slot_no_max, "Add Slot Count OK !!" );
				/*スロット数規定内であれば遅延補正STEP10を実施*/
		        rcode = rrhApi_Cpri_Mnt_DelayAdjustStep10(D_RRH_PROCQUE_L3, parm_p->link_num, isStartup, forceStopEn);
		        if (E_API_RCD_OK != rcode)
		        {
		            cm_Assert(CMD_ASL_USEHIGH, rcode, "rrhApi_Cpri_Mnt_DelayAdjustStep10 NG");
		        }
		    }
//16B スロット数制限機能追加 end
	    }
	}
	else
	{   /* 共用構成の場合、3GとLTEの両方のLayer3が運用中状態なら起動完了とみなす */
	    if ((E_RRH_LAYER3_RE_OPE == shmLayer3[CMD_SYS_3G ].layer3_re[cpr_idx])
	    &&  (E_RRH_LAYER3_RE_OPE == shmLayer3[CMD_SYS_S3G].layer3_re[cpr_idx]))
	    {
//16B スロット数制限機能追加 start
			/*全リンクスロット数読み出し*/
			slot_no_max = 0;
	        for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++){
				f_cmn_com_slot_s3g_get(cpr_num, &buf_LTEslotInfo_re);
				slot_no_max += buf_LTEslotInfo_re.slot_num;
				f_cmn_com_slot_3g_get(cpr_num, &buf_3GslotInfo_re);
				slot_no_max += buf_3GslotInfo_re.slot_num;
			}
			/*スロット数規定値超えで強制停止*/
			if(slot_no_max > RCD_OVER_MAX_SLOT){
				cm_Assert(CMD_ASL_USEHIGH, slot_no_max, "Slot Count Over !!" );
				/*3Gリンク強制停止*/
				userCode = (CMD_SYS_3G << 16) | (parm_p->link_num);
				m_rc_ReHCStop(parm_p->link_num, CMD_SYS_3G, E_RRH_RESTOP_FACTOR_SLOT_NG);
				/*S3Gリンク強制停止*/
				userCode = (CMD_SYS_S3G << 16) | (parm_p->link_num);
				m_rc_ReHCStop(parm_p->link_num, CMD_SYS_S3G, E_RRH_RESTOP_FACTOR_SLOT_NG);
			}
			else{
				cm_Assert(CMD_ASL_USELOW, slot_no_max, "Add Slot OK !!" );
				/*スロット数規定内であれば遅延補正STEP10を実施*/
//16B スロット数制限機能追加 end
		        rcode = rrhApi_Cpri_Mnt_DelayAdjustStep10(D_RRH_PROCQUE_L3, parm_p->link_num, isStartup, forceStopEn);
		        if (E_API_RCD_OK != rcode)
		        {
		            cm_Assert(CMD_ASL_USEHIGH, rcode, "rrhApi_Cpri_Mnt_DelayAdjustStep10 NG");
		        }
			}
	    }
	}

	/* バッファリングされた要求がある場合 */
	rec_startup_cmp = (cmw_validsysinfo == CMD_SYS_S3G) ? (rec_Startup_First_flg[CMD_SYS_S3G]) : (rec_Startup_First_flg[CMD_SYS_S3G] & rec_Startup_First_flg[CMD_SYS_3G]);

	/* REC-FHM間起動完了してれば、キューイングの先頭から処理開始 */
	if((rcw_buffcnt != CMD_NUM0) && (D_RRH_OFF != rec_startup_cmp))
	{
		/* キューイング数が0となったら抜ける */
		while(rcw_buffcnt > CMD_NUM0)
		{
			/* 該当systemのL3状態取得 */
			f_cmn_com_layer3_get(rcw_system_type[0], &layer3);

			/* CPRI index算出 */
			cpr_idx = rcw_cpri[0] -1;

			/* 起動待ちアイドル状態のRE */
			if(layer3.layer3_re[cpr_idx] == E_RRH_LAYER3_RE_IDLE)
			{
				/* 先頭のバッファをRE立ち上げ要求する */
				m_rc_ReStr_Req(rcw_cpri[0], rcw_system_type[0]);
				cm_Assert( CMD_ASL_USELOW, 
					(((rcw_system_type[0] << 24 ) & 0xFF000000 ) | ((rcw_cpri[0] << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
					"RE Start" );

				/* 処理したCPRI接続通知を削除 */
				memcpy(&rcw_cpri[0],		&rcw_cpri[1],			sizeof(rcw_cpri[0])			* (rcw_buffcnt - 1));
				memcpy(&rcw_system_type[0],	&rcw_system_type[1],	sizeof(rcw_system_type[0])	* (rcw_buffcnt - 1));
				rcw_buffcnt--;

				/* 実施したらLOOPを抜ける */
				break;
			}
			else
			{
				/* 起動不要であるため、CPRI接続通知を削除し、次のキューを検索 */
				memcpy(&rcw_cpri[0],		&rcw_cpri[1],			sizeof(rcw_cpri[0])			* (rcw_buffcnt - 1));
				memcpy(&rcw_system_type[0],	&rcw_system_type[1],	sizeof(rcw_system_type[0])	* (rcw_buffcnt - 1));
				rcw_buffcnt--;
				cm_Assert( CMD_ASL_USELOW, 
					(((rcw_system_type[0] << 24 ) & 0xFF000000 ) | ((rcw_cpri[0] << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
					"Skip RE Start" );
			}
		}
	}
	return;
}

/** @} */


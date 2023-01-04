/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReConn_buffchk.c
 *  @brief  CPRI接続通知
 *  @date   2015/08/13 TDIPS)uchida create
 */
/********************************************************************************************************************/

/*****************************************************************************************************************//**
 *
 * @addtogroup RRH_L3_RCT
 * @{
 *
 ********************************************************************************************************************/

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
/********************************************************************************************************************/
/**
 *  @brief    CPRI接続通知
 *  @param    buff_adr [in] メッセージデータ
 *  @return   None
 *  @Bug_No   M-RRU-ZSYS-01990
 *  @date     2015/08/13 TDIPS uchida create
 *  @date     2015/08/13 TDIPS)ikeda RE増減設コーディングミス修正
 *  @date     2015/08/20 TDIPS)uchida 不適切だったファイル名、関数名を変更
 *  @date     2015/11/13 TDIPS)sasaki M-RRU-ZSYS-01990 update IT2問処No.182対処 増設REのL3確立通知をキューイングする条件を修正
 *  @date     2017/01/25 FJT)ohashi   3201-16B-TS96対処 L3確立通知をキューイングする条件を修正
 *  @date     2017/02/16 FJT)ohashi   3201-16B-TS96対処 L3確立通知をキューイングする条件を見直し(運用中増設時対応)
 */
/********************************************************************************************************************/
VOID m_rc_ReConn_buffchk(VOID* buff_adr)
{
	UINT	cnt1;
	UINT	cnt2;
	USHORT	rec_startup_cmp;
	USHORT cpr_idx;
	T_RRH_LAYER3 layer3;
	CMT_TSKIF_CPRICONN_NTC* parm_p = (CMT_TSKIF_CPRICONN_NTC*)buff_adr;
	
	rec_startup_cmp = (cmw_validsysinfo == CMD_SYS_S3G) ? (rec_Startup_First_flg[CMD_SYS_S3G]) : (rec_Startup_First_flg[CMD_SYS_S3G] & rec_Startup_First_flg[CMD_SYS_3G]);

	/* REC の場合 */
	if (parm_p->link_num < D_RRH_CPRINO_RE_MIN)
	{
		/* REC は処理無し */
	}
	else
	{
		cpr_idx = parm_p->link_num -1;

		f_cmn_com_layer3_get(parm_p->system_type, &layer3);

		/* 起動待ちアイドル状態のRE */
		if(layer3.layer3_re[cpr_idx] == E_RRH_LAYER3_RE_IDLE)
		{
			/* REC起動シーケンス未完了であれば */
			if(rec_startup_cmp == D_RRH_OFF)
			{
				/*情報をバッファに追加*/
				rcw_cpri[rcw_buffcnt] = parm_p->link_num;
				rcw_system_type[rcw_buffcnt] = parm_p->system_type;
				rcw_thread_qid[rcw_buffcnt] = parm_p->head.uiSrcTQueueID;
				rcw_buffcnt++;
				cm_Assert( CMD_ASL_USELOW, 
					(((parm_p->system_type << 24 ) & 0xFF000000 ) | ((parm_p->link_num << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
					"REC-FHM Startup state CPRI Connect Queueing" );
			}
			else
			{
				/* REの状態を全て確認 */
				for ( cnt1=CMD_NUM0; cnt1 < CMD_SYS_BOTH_3G_S3G	; cnt1++ )
				{
					f_cmn_com_layer3_get(cnt1, &layer3);

					for ( cnt2=CMD_NUM0; cnt2 < D_RRH_CPRINO_RE_MAX; cnt2++ )
					{
						/* RE起動中状態のREがあれば */
						if(layer3.layer3_re[cnt2] == E_RRH_LAYER3_RE_START)
						{
							/*情報をバッファに追加*/
							rcw_cpri[rcw_buffcnt] = parm_p->link_num;
							rcw_system_type[rcw_buffcnt] = parm_p->system_type;
							rcw_thread_qid[rcw_buffcnt] = parm_p->head.uiSrcTQueueID;
							rcw_buffcnt++;
							cm_Assert( CMD_ASL_USELOW, 
								(((parm_p->system_type << 24 ) & 0xFF000000 ) | ((parm_p->link_num << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
								"CPRI Connect Queueing" );
							return;
						}
					}
				}
				/* RE起動中状態のREが存在しなければRE立ち上げ開始 */
				m_rc_ReStr_Req(parm_p->link_num, parm_p->system_type);
				cm_Assert( CMD_ASL_USELOW, 
					(((parm_p->system_type << 24 ) & 0xFF000000 ) | ((parm_p->link_num << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
					"CPRI Connect RE Start" );
			}
		}
		else
		{
			/* 起動待ちアイドル状態以外のREは起動中であり、RE立ち上げ不要 */
			cm_Assert( CMD_ASL_USELOW, 
				(((parm_p->system_type << 24 ) & 0xFF000000 ) | ((parm_p->link_num << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
				"CPRI Connect Skip RE Start " );
		}
	}
	return;
}
	
/*****************************************************************************************************************//**
 *
 * @} addtogroup RRH_L3_RCT
 *
 ********************************************************************************************************************/

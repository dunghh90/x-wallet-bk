/*!
 * @skip    $Id$
 * @file    tm_lb_RunHisSet.c
 * @brief   LAPB走行履歴書込み
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.22 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"
#include "tm_lb_header.h"





/*!
 *  @brief   LAPB走行履歴書込み
 *  @note    タスク起動された履歴を、走行履歴テーブルに残す
 *  @param   sndrcv_flg		[in]  sndrcv_flg
 *  @param   a_taskid		[in]  a_taskid
 *  @param   a_inf_p		[in]  a_inf_p
 *  @param   a_event		[in]  a_event
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.22 FFCS)hongj modify for zynq.
 *  @date   2015/8/10 TDIPS)ikeda rev.27999 LPB 17リンク修正
 *  @date   2015/8/12 TDIPS)ikeda rev.28117 レビュー42～45反映
 */
void	tm_lb_RunHisSet(UINT sndrcv_flg, UINT a_taskid,UINT a_event,UINT * a_inf_p)
{
//	T_RRH_SYSTEM_TIME	datetime;									/*!< 時刻情報				*/
	UCHAR		in_cpri_no = 0xFF;							/*!< 内部CPRI番号			*/
	UCHAR		inter_event_no = 0xFF;						/*!< 内部event番号			*/
	UCHAR		link_sta = 0xFF;							/*!< LAPB Link Status			*/
	/****************************/
	/* 収集データテーブル書込み */
	/****************************/
	switch(a_event)
	{
														/* L1データ受信通知		*/
		case CMD_TSKIF_L1DATRCVNTC:
			tm_lb_RunHisMngInfGet(&link_sta, &in_cpri_no, &inter_event_no, a_event, a_inf_p);
//
//			lblap_tx_rv_log(a_event, inter_event_no, a_inf_p, in_cpri_no);
//			/* L2/L3送受信ログ処理 */
			if(inter_event_no == LBD_ICTL)
			{
				tm_lb_big_to_little(a_inf_p);
			}
//			/*Receiving I frame will not be reserved log*/
//			if(((inter_event_no == LBD_ICTL) || (inter_event_no == LBD_RRCTL) )
//												&& (lbw_allrunhis_flg == CMD_OFF))
//			{
//				return;
//			}
			break;
														/* L1データ送信要求		*/
		case CMD_TSKIF_L1DATTXREQ:
														/* Layer 1 Data CNF		*/
		case CMD_TSKIF_L1DATCNF:
//			tm_lb_RunHisMngInfGet(&link_sta, &in_cpri_no, &inter_event_no, a_event, a_inf_p);
//
//			/* L2/L3送受信ログ処理 */
//			lblap_tx_rv_log(a_event, inter_event_no, a_inf_p, in_cpri_no);
//			
//			/*Receiving I frame will not be reserved log*/
//			if((inter_event_no == LBD_RRCTL) && (lbw_allrunhis_flg == CMD_OFF))
//			{
//				return;
//			}
			break;
														/* CPRI信号送信通知		*/
		case CMD_TSKIF_CPRISNDNTC:

			/* エンディアン変換 */
			tm_lb_little_to_big(a_inf_p);

			return;
														/* CPRI信号受信通知		*/
		case CMD_TSKIF_CPRIRCVNTC:
			return;
														/* L2 DEACT依頼			*/
		case CMD_TSKIF_L2DEACTREQ:
														/* データリンク設定要求	*/
		case CMD_TSKIF_ESTREQ:
														/* データリンク解放要求	*/
		case CMD_TSKIF_RELREQ:
														/* 自受信ビジー			*/
		case CMD_TSKIF_L2BSY:
														/* 自受信ビジーの解除	*/
		case CMD_TSKIF_L2BSYCLR:
														/* L2 STOP依頼			*/
		case CMD_TSKIF_L2STPREQ:
														/* L3 test 開始通知		*/
		case CMD_TSKIF_L3MODESTRNTC:
														/* L3 test 停止通知		*/
		case CMD_TSKIF_L3MODESTPNTC:
														/* データリンク設定確認	*/
		case CMD_TSKIF_ESTCNF:
														/* データリンク設定表示	*/
		case CMD_TSKIF_ESTIND:
														/* データリンク設定中表示	*/
		case CMD_TSKIF_ESTINGIND:
														/* データリンク解放確認	*/
		case CMD_TSKIF_RELCNF:
														/* データリンク解放表示	*/
		case CMD_TSKIF_RELIND:
//			tm_lb_RunHisMngInfGet(&link_sta, &in_cpri_no, &inter_event_no, a_event, a_inf_p);
			break;
														/* タイムアウト発生通知	*/
		case CMD_TSKIF_TIMOUTNTC :
//			tm_lb_RunHisTmrInfGet(&link_sta, &in_cpri_no, &inter_event_no, a_taskid, a_inf_p);
			break;
														/* 初期化要求			*/
		case CMD_TSKIF_INIREQ:
														/* 初期化応答			*/
		case CMD_TSKIF_INIRSP:
			return;
														/*	N/E間データ引継ぎ開始通知		*/
														/*	N/E間データ引継ぎ完了通知		*/
														/*	LAPB N/E切替開始要求			*/
														/*	LAPB N/E切替開始応答			*/
														/*	N/E切替完了通知					*/
														/*	Other							*/
		default:
			return;
	}
//	/* 時刻情報収集 */
//	cm_RtcGet( &datetime );
//	/* 走行履歴書込み */
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].setFlg = CMD_ON;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].month		= datetime.month;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].day		= datetime.day;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].hour		= datetime.hour;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].min		= datetime.min;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].sec		= datetime.sec;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].msec		= datetime.msec;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].sndrcv_flg = (UCHAR)sndrcv_flg;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].task_id = (USHORT)a_taskid;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].ext_event_id = a_event;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].task_sta = (UCHAR)lbw_tskstt;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].inter_event_id= inter_event_no;
//	lbw_runhis_tbl[in_cpri_no].inf[lbw_runhis_tbl[in_cpri_no].write_idx].link_sta = link_sta;
//	/**********************/
//	/* 次走行履歴書込準備 */
//	/**********************/
//	/* 書込位置更新 */
//	lbw_runhis_tbl[in_cpri_no].write_idx++;
//	if (lbw_runhis_tbl[in_cpri_no].write_idx >= LBD_RUNHIS_FACENUM_MAX)
//	{
//		lbw_runhis_tbl[in_cpri_no].write_idx = CMD_NUM0;
//	}

	return;
}

void	tm_lb_logPrint(FILE *outf)
{
	UINT index;
	UINT index2;
	UINT printIdx;
	USHORT linkno;
	CHAR *strSndRcv[] = {"RCV","SND"};
	
	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++ )
	{
		fprintf(outf,"L2 lpb running history CPRI no %03d----------------------------------------\n", linkno);
		printIdx = 0;

		for(index = 0; index < LBD_RUNHIS_FACENUM_MAX;index++)
		{
			if(lbw_runhis_tbl[linkno].inf[index].setFlg == 0)
				continue;

			fprintf(outf,"No.%03d [%02x-%02x %02x:%02x:%02x.%03x] %s;%08x;%04x;%04x\n",printIdx,
				lbw_runhis_tbl[linkno].inf[index].month,
				lbw_runhis_tbl[linkno].inf[index].day,
				lbw_runhis_tbl[linkno].inf[index].hour,
				lbw_runhis_tbl[linkno].inf[index].min,
				lbw_runhis_tbl[linkno].inf[index].sec,
				lbw_runhis_tbl[linkno].inf[index].msec,
				strSndRcv[lbw_runhis_tbl[linkno].inf[index].sndrcv_flg],
				lbw_runhis_tbl[linkno].inf[index].ext_event_id,
				lbw_runhis_tbl[linkno].inf[index].inter_event_id,
				lbw_runhis_tbl[linkno].inf[index].task_sta);

			printIdx++;
		}
	}

	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++ )
	{
		fprintf(outf,"\nL2 lpb linkstate history CPRI no %03d----------------------------------------\n", linkno);
		printIdx = 0;

		for(index = 0; index < 128;index++)
		{
			if(lbw_lnkStatLog[linkno].tbl[index].setflg == 0)
				continue;

			fprintf(outf,"No.%03d [%02x-%02x %02x:%02x:%02x.%02x] %d,%d,%d\n",printIdx++,
				lbw_lnkStatLog[linkno].tbl[index].datetime.month,
				lbw_lnkStatLog[linkno].tbl[index].datetime.day,
				lbw_lnkStatLog[linkno].tbl[index].datetime.hour,
				lbw_lnkStatLog[linkno].tbl[index].datetime.min,
				lbw_lnkStatLog[linkno].tbl[index].datetime.sec,
				lbw_lnkStatLog[linkno].tbl[index].datetime.msec,
				lbw_lnkStatLog[linkno].tbl[index].linkstate_prev,
				lbw_lnkStatLog[linkno].tbl[index].linkstate_now,
				lbw_lnkStatLog[linkno].tbl[index].linkevent);

			if(lbw_lnkStatLog[linkno].tbl[index].func) fprintf(outf,"  %s\n",lbw_lnkStatLog[linkno].tbl[index].func);

			for(index2 = 0; index2 < 6;index2++)
			{
				fprintf(outf,"  %02x %02x %02x %02x %02x %02x %02x %02x\n",
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_prev[index2 * 8],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_prev[index2 * 8 + 1],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_prev[index2 * 8 + 2],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_prev[index2 * 8 + 3],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_prev[index2 * 8 + 4],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_prev[index2 * 8 + 5],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_prev[index2 * 8 + 6],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_prev[index2 * 8 + 7]);
			}
			for(index2 = 0; index2 < 6;index2++)
			{
				fprintf(outf,"  %02x %02x %02x %02x %02x %02x %02x %02x\n",
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_prev[index2 * 8],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_prev[index2 * 8 + 1],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_prev[index2 * 8 + 2],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_prev[index2 * 8 + 3],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_prev[index2 * 8 + 4],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_prev[index2 * 8 + 5],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_prev[index2 * 8 + 6],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_prev[index2 * 8 + 7]);
			}
			for(index2 = 0; index2 < 6;index2++)
			{
				fprintf(outf,"  %02x %02x %02x %02x %02x %02x %02x %02x\n",
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_now[index2 * 8],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_now[index2 * 8 + 1],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_now[index2 * 8 + 2],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_now[index2 * 8 + 3],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_now[index2 * 8 + 4],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_now[index2 * 8 + 5],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_now[index2 * 8 + 6],
							lbw_lnkStatLog[linkno].tbl[index].lpbdat_now[index2 * 8 + 7]);
			}
			for(index2 = 0; index2 < 6;index2++)
			{
				fprintf(outf,"  %02x %02x %02x %02x %02x %02x %02x %02x\n",
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_now[index2 * 8],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_now[index2 * 8 + 1],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_now[index2 * 8 + 2],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_now[index2 * 8 + 3],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_now[index2 * 8 + 4],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_now[index2 * 8 + 5],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_now[index2 * 8 + 6],
							lbw_lnkStatLog[linkno].tbl[index].lpbmng_now[index2 * 8 + 7]);
			}
		}
	}
	/* internal assert */
	outf = f_cmn_com_assert_th_print_all(outf);
	return;
}

/* @} */



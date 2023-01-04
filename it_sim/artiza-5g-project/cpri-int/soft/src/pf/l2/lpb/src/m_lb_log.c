/*!
 * @skip    $Id$
 * @file    m_lb_log.c
 * @brief   CPRI link 制御機能ブロック
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.18 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"
#include "tm_lb_header.h"

#include "m_lb_header.h"
#include "l2_com_inc.h"

/********************************************************************************************************/

/*!
 *  @brief   L2統計情報処理 (lblap_statis)
 *  @note    該当する統計カウンタをインクリメントする処理を行う
 *  @param   link_num		[in]  リンク番号
 *  @param   cnt_area		[in]  統計タイプ
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2013.11.22 FFCS)hongj  modify for zynq.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void lblap_statis(USHORT link_num,UINT cnt_area)
{
	T_RRH_SYSTEM_TIME time;
	UINT       index;
	USHORT     flag;
	USHORT     direction;

    switch(cnt_area)
    {
//	case LBD_L2_DOV:
//	case LBD_L2_DRV:
	case LBD_L2_INS:
//	case LBD_L2_ILR:
//	case LBD_L2_LTO:
//	case LBD_L2_LRO:
//	case LBD_L2_RST:
//	case LBD_L2_RXF:
//	case LBD_L2_RXO:
//	case LBD_L2_LRST:
	case LBD_L2_RBSY:
//	case LBD_L2_RDTO:
//	case LBD_L2_RI:
//	case LBD_L2_RS:
//	case LBD_L2_RU:
	case LBD_L2_RRNR:
	case LBD_L2_RREJ:
	case LBD_L2_RDM:
	case LBD_L2_RDSC:
//	case LBD_L2_RFMR:
	case LBD_L2_RSBM:
	case LBD_L2_RUA:
//	case LBD_L2_RC:
	case LBD_L2_DSC_I:
	    direction = D_L2_COM_DIRE_RECV;
		break;
	case LBD_L2_ESTIND_N:
	case LBD_L2_ESTIND_U:
	case LBD_L2_ESTCNF:
	case LBD_L2_RELIND_N:	
	case LBD_L2_RELIND_U:	
	case LBD_L2_RELCNF:	
	case LBD_L2_INGIND_N:
	case LBD_L2_INGIND_U:
    case LBD_L2_DSR2T1O:
    case LBD_L2_WAI2T1O:
    case LBD_L2_T4O:
    case LBD_L2_RDE:
	case LBD_L2_RSTO:
	case LBD_L2_DSRT1O:
//	case LBD_L2_WAITIO:
	case LBD_L2_IDSC:
	case LBD_L2_BIDSC:
	case LBD_L2_RSEND:
	case LBD_L2_RFRMW:
	case LBD_L2_RFRMX:
	case LBD_L2_RFRMY:
	case LBD_L2_RFRMZ:
	case LBD_L2_SFRMW:
	case LBD_L2_SFRMX:
	case LBD_L2_SFRMY:
	case LBD_L2_SFRMZ:
//	case LBD_L2_SRRP1:
//	case LBD_L2_SRRF1:
//	case LBD_L2_SRRF0:
	case LBD_L2_QUENUM:
	case LBD_L2_BSRNR:
	case LBD_L2_BCSRR:
	case LBD_L2_OBRRNR:
	case LBD_L2_OBCRRR:
//    case LBD_L2_NS2END:
	    direction = D_L2_COM_DIRE_EVNT;
		break;
	default :
	    direction = D_L2_COM_DIRE_SEND;
	    break;
	}
    

	/* L2統計情報処理 */
	index = l2_comw_stati_L2Tbl[link_num].mng.index;

    /* L2信号種別設定 */
	l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].singal_kind = cnt_area;
	/* 初回フラグ取得 */
	flag = l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].firstTime_flag;
	/* 時刻取得 */
	cm_RtcGet(&time);

	if (flag != CMD_FLG_ON)
	{
		/* 初回フラグ設定 */
		l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].firstTime_flag = CMD_FLG_ON;
		/* 初回L2送受時刻 */
		memcpy(&(l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].firstTime),  &time, sizeof(time));
	}

	/* 最終L2発生時刻 */
	memcpy(&(l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].lastTime),  &time, sizeof(time));

	if (l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].occur_counter != D_L2_STATI_OCCOUR_MAX)
	{
		/* 発生回数設定 */
		l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].occur_counter++;
	}
	else
	{
		/* 発生回数設定 */
		l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].occur_counter = 0;
		/* オーバーフロー回数設定 */
		l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].overflow_counter++;
	}

	/* 送受信フラグ */
	l2_comw_stati_L2Tbl[link_num].blks[index].sigs[cnt_area].opt.l2_opt.l2_tx_rv_flag = direction;

	return;
}												/*  END OF  "lblap_statis"   */
/********************************************************************************************************/

/*!
 *  @brief   L3統計情報処理 (lblap_statis_for_l3)
 *  @note    該当する統計カウンタをインクリメントする処理を行う
 *  @param   linkno         [in]  CPRIリンク番号
 *  @param   system_type    [in]  システム種別
 *  @param   cnt_area		[in]  統計タイプ
 *  @param   rsp_code		[in]  応答結果
 *  @return  None
 *  @date    2013.11.22 FFCS)hongj create for zynq.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void lblap_statis_for_l3(USHORT linkno, USHORT system_type, UINT cnt_area, UINT rsp_code)
{
	T_RRH_SYSTEM_TIME	time;
	UINT				index;
	USHORT				flag;
	T_RRH_L2_STATISTICS_L3TABLE * l2_comw_stati_L3Tbl_workp;

	/* 引数チェック */
	if(cnt_area >= D_RRH_L2_STATI_L3SIG_NUM)
	{
		cm_Assert( CMD_ASL_DBGLOW, cnt_area,
			"[lblap_statis_for_l3] Parameter cnt_area is wrong" );
		return;
	}
	l2_comw_stati_L3Tbl_workp = l2_comw_stati_L3Tbl+(D_RRH_CPRINO_NUM*system_type+linkno);

	/* L3統計情報処理 */
	index = l2_comw_stati_L3Tbl_workp->mng.index;

	/* L3信号種別設定 */
	l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].singal_kind = (l2_comr_stati_l3sig[cnt_area].signal_kind | system_type);
	/* 初回フラグ取得 */
	flag = l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].firstTime_flag;
	/* 時刻取得 */
	cm_RtcGet(&time);

	if (flag != CMD_FLG_ON)
	{
		/* 初回フラグ設定 */
		l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].firstTime_flag = CMD_FLG_ON;
		/* 初回L3送受時刻 */
		memcpy(&(l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].firstTime),  &time, sizeof(time));
	}

	/* 最終L3発生時刻 */
	memcpy(&(l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].lastTime),  &time, sizeof(time));

	if (l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].occur_counter != D_L2_STATI_OCCOUR_MAX)
	{
		/* 発生回数設定 */
		l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].occur_counter++;
	}
	else
	{
		/* 発生回数設定 */
		l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].occur_counter = 0;
		/* オーバーフロー回数設定 */
		l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].overflow_counter++;
	}

	/* 送受信フラグ */
	switch(cnt_area)
	{
	case D_L2_STATI_HCRES:
		/* OK回数 */
		l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].opt.l3_opt.l3_ok_counter++;
		break;

	case D_L2_STATI_LINKSTSRES:
	case D_L2_STATI_FIRMFILERES:
	case D_L2_STATI_FIRMFILERES2:
	case D_L2_STATI_FILEINFRES:
	case D_L2_STATI_FILSNDFINRES:
	case D_L2_STATI_UFIRMFILERES:
	case D_L2_STATI_UFIRMFILERES2:
	case D_L2_STATI_UFILEINFRES:
	case D_L2_STATI_UFILSNDFINRES:
	case D_L2_STATI_TRXSETRES:
	case D_L2_STATI_TRXRELRES:
	case D_L2_STATI_TOFFSETINFRES:
	case D_L2_STATI_CARONOFFRES:
	case D_L2_STATI_EQPSTRRES:
	case D_L2_STATI_SLOTINFRES:
	case D_L2_STATI_RESTSRES:
	case D_L2_STATI_CARDINFRES:
	case D_L2_STATI_REVERRES:
	case D_L2_STATI_RECARDCNTRES:
	case D_L2_STATI_CARDSTSRES:
	case D_L2_STATI_CARLSTSRES:
	case D_L2_STATI_REPORTCNTRES:
	case D_L2_STATI_REPORTSTSRES:
	case D_L2_STATI_RECRDDIARES:
	case D_L2_STATI_REOBSLOGRES:
	case D_L2_STATI_REOBLOGSTPRES:
	case D_L2_STATI_EXTDATSNDRES:
	case D_L2_STATI_EXTDATRCVRES:
	case D_L2_STATI_MTADDSETRES:
	case D_L2_STATI_FLDMTCNCRES:
	case D_L2_STATI_FLDCOMSNDRES:
	case D_L2_STATI_FLDCOMRCVRES:
	case D_L2_STATI_FLDFILSNDRES:
	case D_L2_STATI_FLDFILRCVRES:
	case D_L2_STATI_FLMTCNCRELRES:
	case D_L2_STATI_FLMTCMPRELRES:
	case D_L2_STATI_SVMTCNTRES:
	case D_L2_STATI_SVMTDATSNDRES:
	case D_L2_STATI_SVMTDATRCVRES:
	case D_L2_STATI_SVMTCNTRELRES:
	case D_L2_STATI_SVMTFORRELRES:
	case D_L2_STATI_MKMTCNTRES:
	case D_L2_STATI_MKMTCNTRELRES:
	case D_L2_STATI_MKMTFORRELRES:
	case D_L2_STATI_FTPPORTSETRES:
		if (rsp_code == CMD_NML)
		{
			/* OK回数 */
			l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].opt.l3_opt.l3_ok_counter++;
		}
		else
		{
			/* NG回数 */
			l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].opt.l3_opt.l3_ng_counter++;
			/* 最終NG要因 */
			l2_comw_stati_L3Tbl_workp->blks[index].sigs[cnt_area].opt.l3_opt.l3_last_ng_code = rsp_code;
		}
		break;

	default :
		break;
	}

	return;
}												/*	END OF	"lblap_statis_for_l3"	*/


/********************************************************************************************************/
#if 0
/*!
 *  @brief   統計カウンタ処理 1(lblap_stats1)
 *  @note    該当する統計カウンタにパラメータ分の値を加える処理を行う
 *  @param   link_num		[in]  リンク番号
 *  @param   cnt_area		[in]  統計数
 *  @param   add_val		[in]  統計タイプ
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void lblap_stats1(USHORT link_num, UINT cnt_area,USHORT add_val)
{
												/*-------------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*-------------------------------*/
	UINT work;                                              /*!< 値を判定するためのエリア */
													/*-----------------------*/
													/*  統計カウンタ値 MAX ? */
													/* (YES時は統計終了)     */
													/*-----------------------*/

	if(lbw_stslog[link_num][lbw_stscnt[link_num]].sts_data[cnt_area] != 0xFFFFFFFF)
	{
													/*-----------------------*/
													/* NO (統計可能)         */
													/*    Counter値セーブ    */
													/*-----------------------*/
		work = lbw_stslog[link_num][lbw_stscnt[link_num]].sts_data[cnt_area];

													/*-----------------------*/
													/*  Work Counter +追加値 */
													/*-----------------------*/
		work += (UINT)add_val;

											/*-------------------------------*/
											/* Counter Overflow Check処理    */
											/*-------------------------------*/
													/*-----------------------*/
													/* Work Counter > Counter*/
													/*-----------------------*/
		if(work >= lbw_stslog[link_num][lbw_stscnt[link_num]].sts_data[cnt_area])
		{
													/*-----------------------*/
													/* YES (Count UP OK!!)   */
													/*  Counter値更新        */
													/*-----------------------*/
			lbw_stslog[link_num][lbw_stscnt[link_num]].sts_data[cnt_area] = work;
		}
		else
		{
													/*-----------------------*/
													/* NO (Counter Overflow) */
													/* Counter = MAX 値 Set  */
													/*-----------------------*/
			lbw_stslog[link_num][lbw_stscnt[link_num]].sts_data[cnt_area] = 0xFFFFFFFF;
		}
	}
	else
	{
		/* Statistics log count adds 1*/
		lbw_stscnt[link_num]++;
		/* Statistics log count is greater than 2*/
		if(lbw_stscnt[link_num] >= CMD_NUM2)
		{
		/*Set Statistics log count to 0*/
			lbw_stscnt[link_num] = CMD_NUM0;
		}
		/*Clear old data*/
		cm_MemClr(&lbw_stslog[link_num][lbw_stscnt[link_num]],sizeof(struct LBT_STSLOG_DAT));
		/*Set current time to Statistics log count*/
		cm_RtcGet(&lbw_stslog[link_num][lbw_stscnt[link_num]].datetime);
													/*    Counter値更新      */
		lbw_stslog[link_num][lbw_stscnt[link_num]].sts_data[cnt_area] += add_val;
	}

	return;
}												/*  END OF  "lblap_stats1"   */
#endif

/********************************************************************************************************/

/*!
 *  @brief   上書きされるログ(I frame)は個別領域にコピーする (lblap_move_log)
 *  @note    LASHからのログ展開処理
 *  @param   a_log		[in]  log info pointer
 *  @return  None
 *  @date    2013.11.28 FFCS)hongj create for zynq.
 */
void lblap_move_log(T_RRH_L2_LOG_INFO * a_log)
{
	T_RRH_L2_LOG_INFO *			 copy_to;
	USHORT						 *singal_kind;
	USHORT						 sigkind;
	USHORT						 spec_no;
	USHORT						 spec_index;

	/* In case of "not I farm"  */
	if(a_log->lapb_type != LBD_ICTL)
	{
		/*	In case "RR signal"	*/
		if(a_log->lapb_type == D_L2_RRCTL)
		{
			/* 個別ログ無しとする */
			spec_no = 0xFFFF;
		}
		/*	In case "not RR signal"	*/
		else
		{
			/* I frame以外は個別領域0にコピーする */
			spec_no = 0;
		}
	}
	/* In case of "I farm"  */
	else
	{
		singal_kind = (USHORT *)(&a_log->data[0]);
		sigkind = l2_com_swap_USHORT(*singal_kind);
		switch(sigkind & 0xFFFE)
		{
			case CMD_CPRID_FIRMFILEREQ:
			case CMD_CPRID_FIRMFILERES:
			case CMD_CPRID_FILEINFREQ:
			case CMD_CPRID_FILEINFRES:
			case CMD_CPRID_FILEDATSND:
			case CMD_CPRID_FILSNDFINNTC:
			case CMD_CPRID_FILSNDFINRES:
			case CMD_CPRID_UFIRMFILEREQ:
			case CMD_CPRID_UFIRMFILERES:
			case CMD_CPRID_UFILEINFREQ:
			case CMD_CPRID_UFILEINFRES:
			case CMD_CPRID_UFILEDATSND:
			case CMD_CPRID_UFILSNDFINNTC:
			case CMD_CPRID_UFILSNDFINRES:
				spec_no = 1;
				break;

			case CMD_CPRID_TRXSETREQ:
			case CMD_CPRID_TRXSETRES:
			case CMD_CPRID_TRXRELREQ:
			case CMD_CPRID_TRXRELRES:
			case CMD_CPRID_CARONOFFREQ:
			case CMD_CPRID_CARONOFFRES:
				spec_no = 2;
				break;

			case CMD_CPRID_SLOTINFREQ:
			case CMD_CPRID_SLOTINFRES:
				spec_no = 3;
				break;
			case CMD_CPRID_CARDINFREQ:
			case CMD_CPRID_CARDINFRES:
			case CMD_CPRID_REVERREQ:
			case CMD_CPRID_REVERRES:
				spec_no = 3;
				break;

			case CMD_CPRID_RECARDCNTREQ:
			case CMD_CPRID_RECARDCNTRES:
				spec_no = 4;
				break;

			case CMD_CPRID_REPORTCNTREQ:
			case CMD_CPRID_REPORTCNTRES:
			case CMD_CPRID_RECRDDIAREQ:
			case CMD_CPRID_RECRDDIARES:
			case CMD_CPRID_REOBSLOGREQ:
			case CMD_CPRID_REOBSLOGRES:
			case CMD_CPRID_REOBLOGSTPREQ:
			case CMD_CPRID_REOBLOGSTPRES:
				spec_no = 5;
				break;

			case CMD_CPRID_EXTDATSNDREQ:
			case CMD_CPRID_EXTDATSNDRES:
			case CMD_CPRID_EXTDATRCVREQ:
			case CMD_CPRID_EXTDATRCVRES:
				spec_no = 6;
				break;

			case CMD_CPRID_MTADDSETREQ:
			case CMD_CPRID_MTADDSETRES:
			case CMD_CPRID_FLDMTCNCREQ:
			case CMD_CPRID_FLDMTCNCRES:
			case CMD_CPRID_FLDCOMSNDREQ:
			case CMD_CPRID_FLDCOMSNDRES:
			case CMD_CPRID_FLDCOMRCVREQ:
			case CMD_CPRID_FLDCOMRCVRES:
			case CMD_CPRID_FLDFILSNDREQ:
			case CMD_CPRID_FLDFILSNDRES:
			case CMD_CPRID_FLDFILRCVREQ:
			case CMD_CPRID_FLDFILRCVRES:
			case CMD_CPRID_FLMTCNCRELREQ:
			case CMD_CPRID_FLMTCNCRELRES:
			case CMD_CPRID_FLMTCMPRELREQ:
			case CMD_CPRID_FLMTCMPRELRES:
			case CMD_CPRID_SVMTCNTREQ:
			case CMD_CPRID_SVMTCNTRES:
			case CMD_CPRID_SVMTDATSNDREQ:
			case CMD_CPRID_SVMTDATSNDRES:
			case CMD_CPRID_SVMTDATRCVREQ:
			case CMD_CPRID_SVMTDATRCVRES:
			case CMD_CPRID_SVMTCNTRELREQ:
			case CMD_CPRID_SVMTCNTRELRES:
			case CMD_CPRID_SVMTFORRELREQ:
			case CMD_CPRID_SVMTFORRELRES:
				spec_no = 7;
				break;

			case CMD_CPRID_HCREQ:
			case CMD_CPRID_HCRES:
			case CMD_CPRID_TOFFSETINFREQ:
			case CMD_CPRID_TOFFSETINFRES:
			case CMD_CPRID_EQPSTRREQ:
			case CMD_CPRID_EQPSTRRES:
			case CMD_CPRID_RESTSREQ:
			case CMD_CPRID_RESTSRES:
			case CMD_CPRID_CARDSTSREQ:
			case CMD_CPRID_CARDSTSRES:
			case CMD_CPRID_CARLSTSREQ:
			case CMD_CPRID_CARLSTSRES:
			case CMD_CPRID_REPORTSTSREQ:
			case CMD_CPRID_REPORTSTSRES:
			case CMD_CPRID_MKMTDATSND:
			case CMD_CPRID_MKMTCNTREQ:
			case CMD_CPRID_MKMTCNTRES:
			case CMD_CPRID_MKMTCNTRELREQ:
			case CMD_CPRID_MKMTCNTRELRES:
			case CMD_CPRID_MKMTFORRELREQ:
			case CMD_CPRID_MKMTFORRELRES:
				/* 個別ログ無しとする */
				spec_no = 0xFFFF;
				break;

			default :
				/* 他のシグナルはBLK7とする */
				spec_no = 7;
				break;
		}
	}
	/*	In case of "spec_no less 8"	*/
	if(spec_no < D_RRH_L2_TXRV_LOG_BLKS)
	{
		spec_index = l2_comw_logHead->cntSpec[spec_no];
		copy_to = &(l2_comw_logSpec->spec[spec_no][spec_index]);
		memcpy(copy_to, a_log, sizeof(T_RRH_L2_LOG_INFO));
		if(l2_comw_logHead->cntSpec[spec_no] >= (D_RRH_L2_TXRV_LOG_SPEC_INF_NUM - 1))
		{
			/* reset specific log index to start position of non-cyclical area */
			l2_comw_logHead->cntSpec[spec_no] = 0;
		}
		else
		{
			/* point next record */
			l2_comw_logHead->cntSpec[spec_no]++;
		}
	}
	return;
}												/*	END OF	"lblap_move_log"   */

/********************************************************************************************************/

/*!
 *  @brief   singal kindチェック (lblap_check_singalKind)
 *  @note    singal kindチェック
 *  @param   singal_kind	[in]  singal kind
 *  @return  check result
 *  @date    2013.11.28 FFCS)hongj create for zynq.
 */
INT lblap_check_singalKind(USHORT singal_kind)
{
	INT      ret;

	switch(singal_kind & 0xfffe)
	{
		case CMD_CPRID_HCREQ:
		case CMD_CPRID_HCRES:
		case CMD_CPRID_RERSTREQ:
		case CMD_CPRID_RETIMSETREQ:
		case CMD_CPRID_LINKSTSREQ:
		case CMD_CPRID_LINKSTSRES:
		case CMD_CPRID_FIRMFILEREQ:
		case CMD_CPRID_FIRMFILERES:
		case CMD_CPRID_FIRMFILERES2:
		case CMD_CPRID_FILEINFREQ:
		case CMD_CPRID_FILEINFRES:
		case CMD_CPRID_FILEDATSND:
		case CMD_CPRID_FILSNDFINNTC:
		case CMD_CPRID_FILSNDFINRES:
		case CMD_CPRID_UFIRMFILEREQ:
		case CMD_CPRID_UFIRMFILERES:
		case CMD_CPRID_UFIRMFILERES2:
		case CMD_CPRID_UFILEINFREQ:
		case CMD_CPRID_UFILEINFRES:
		case CMD_CPRID_UFILEDATSND:
		case CMD_CPRID_UFILSNDFINNTC:
		case CMD_CPRID_UFILSNDFINRES:
		case CMD_CPRID_TRXSETREQ:
		case CMD_CPRID_TRXSETRES:
		case CMD_CPRID_TRXRELREQ:
		case CMD_CPRID_TRXRELRES:
		case CMD_CPRID_TOFFSETINFREQ:
		case CMD_CPRID_TOFFSETINFRES:
		case CMD_CPRID_CARONOFFREQ:
		case CMD_CPRID_CARONOFFRES:
		case CMD_CPRID_EQPSTRREQ:
		case CMD_CPRID_EQPSTRRES:
		case CMD_CPRID_SLOTINFREQ:
		case CMD_CPRID_SLOTINFRES:
		case CMD_CPRID_RESTSREQ:
		case CMD_CPRID_RESTSRES:
		case CMD_CPRID_CARDINFREQ:
		case CMD_CPRID_CARDINFRES:
		case CMD_CPRID_REVERREQ:
		case CMD_CPRID_REVERRES:
		case CMD_CPRID_RECARDCNTREQ:
		case CMD_CPRID_RECARDCNTRES:
		case CMD_CPRID_CARDSTSREQ:
		case CMD_CPRID_CARDSTSRES:
		case CMD_CPRID_CARLSTSREQ:
		case CMD_CPRID_CARLSTSRES:
		case CMD_CPRID_REPORTCNTREQ:
		case CMD_CPRID_REPORTCNTRES:
		case CMD_CPRID_REPORTSTSREQ:
		case CMD_CPRID_REPORTSTSRES:
		case CMD_CPRID_RECRDDIAREQ:
		case CMD_CPRID_RECRDDIARES:
		case CMD_CPRID_REOBSLOGREQ:
		case CMD_CPRID_REOBSLOGRES:
		case CMD_CPRID_REOBLOGSTPREQ:
		case CMD_CPRID_REOBLOGSTPRES:
		case CMD_CPRID_EXTDATSNDREQ:
		case CMD_CPRID_EXTDATSNDRES:
		case CMD_CPRID_EXTDATRCVREQ:
		case CMD_CPRID_EXTDATRCVRES:
		case CMD_CPRID_MTADDSETREQ:
		case CMD_CPRID_MTADDSETRES:
		case CMD_CPRID_FLDMTCNCREQ:
		case CMD_CPRID_FLDMTCNCRES:
		case CMD_CPRID_FLDCOMSNDREQ:
		case CMD_CPRID_FLDCOMSNDRES:
		case CMD_CPRID_FLDCOMRCVREQ:
		case CMD_CPRID_FLDCOMRCVRES:
		case CMD_CPRID_FLDFILSNDREQ:
		case CMD_CPRID_FLDFILSNDRES:
		case CMD_CPRID_FLDFILRCVREQ:
		case CMD_CPRID_FLDFILRCVRES:
		case CMD_CPRID_FLMTCNCRELREQ:
		case CMD_CPRID_FLMTCNCRELRES:
		case CMD_CPRID_FLMTCMPRELREQ:
		case CMD_CPRID_FLMTCMPRELRES:
		case CMD_CPRID_SVMTCNTREQ:
		case CMD_CPRID_SVMTCNTRES:
		case CMD_CPRID_SVMTDATSNDREQ:
		case CMD_CPRID_SVMTDATSNDRES:
		case CMD_CPRID_SVMTDATRCVREQ:
		case CMD_CPRID_SVMTDATRCVRES:
		case CMD_CPRID_SVMTCNTRELREQ:
		case CMD_CPRID_SVMTCNTRELRES:
		case CMD_CPRID_SVMTFORRELREQ:
		case CMD_CPRID_SVMTFORRELRES:
		case CMD_CPRID_MKMTDATSND:
		case CMD_CPRID_MKMTCNTREQ:
		case CMD_CPRID_MKMTCNTRES:
		case CMD_CPRID_MKMTCNTRELREQ:
		case CMD_CPRID_MKMTCNTRELRES:
		case CMD_CPRID_MKMTFORRELREQ:
		case CMD_CPRID_MKMTFORRELRES:
		case CMD_CPRID_FTPPORTSETREQ:
		case CMD_CPRID_FTPPORTSETRES:
		    ret = CMD_OK;
	        break;
		default:
	        ret = CMD_NG;
	        break;
	}
	
	return ret;
}												/*  END OF  "lblap_check_singalKind"   */


/********************************************************************************************************/
#if 0
/*!
 *  @brief   L2/L3送受信ログ処理 (lblap_tx_rv_log)
 *  @note    該当する統計カウンタをインクリメントする処理を行う
 *  @param   a_event		[in]  a_event
 *  @param   inter_event_no	[in]  interanl event no
 *  @param   a_inf_p		[in]  a_inf_p
 *  @return  None
 *  @date    2013.11.22 FFCS)hongj create for zynq.
 */
void lblap_tx_rv_log(UINT a_event,UINT inter_event_no, UINT * a_inf_p, USHORT link_num)
{
	CMT_TSKIF_CPRIRCV_SIGNALGET  *sig;
	T_RRH_L2_LOG_INFO 			*log_info;
	INT 						 ret;
	USHORT						 singal_kind;
	UINT						 pro_thd_no;
	UINT						 dataLen;
	UINT						 copyLen;
	CHAR						 direction;
	USHORT						 sgmtno1;
	USHORT						 sgmtno2;

	/* L2/L3送受信ログ処理 */
	if(a_event == CMD_TSKIF_L1DATRCVNTC)
	{
		direction = 'R';
	}
	else if(a_event == CMD_TSKIF_L1DATTXREQ)
	{
		direction = 'S';
	}
	else
	{
		/* 処理なし */
		return;
	}

	sig = (CMT_TSKIF_CPRIRCV_SIGNALGET *)a_inf_p;
	switch(inter_event_no)
	{
	case LBD_ICTL:
		singal_kind = l2_com_swap_USHORT(sig->signal_kind);
		if (direction == 'R')
		{
			pro_thd_no = sig->head.uiSrcPQueueID;
		}
		else
		{
			pro_thd_no = sig->head.uiDstPQueueID;
		}
		ret = lblap_check_singalKind(singal_kind);
		if(ret != CMD_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, singal_kind,
				"[lblap_tx_rv_log] signal_kind is wrong" );
			return;
		}
		if(((singal_kind & 0xFFFE) == CMD_CPRID_FILEDATSND) ||
					 ((singal_kind & 0xFFFE) == CMD_CPRID_UFILEDATSND))
		{
			sgmtno1 = l2_com_swap_USHORT(((CMT_TSKIF_CPRIRCV_FILEDATSND *)a_inf_p)->cpridat_filedatsnd.sgmtno1);
			sgmtno2 = l2_com_swap_USHORT(((CMT_TSKIF_CPRIRCV_FILEDATSND *)a_inf_p)->cpridat_filedatsnd.sgmtno2);
			/* 分割番号=1だけロギング */
			if((sgmtno1 == 0) && (sgmtno2 == 1))
			{
				/*	take log	*/
			}
			else
			{
				/*	don't take log	*/
				return;
			}
		}
		/*	SlavePortにてファイルデータ情報通知(システムパラメータ)を受信した場合	*/
		else if((singal_kind == CMD_CPRID_FILEINFREQ) && (link_num == D_RRH_CPRINO_REC))
		{
			sgmtno1 = l2_com_swap_USHORT(((CMT_TSKIF_CPRIRCV_FILEINFREQ *)a_inf_p)->cpridat_fileinfreq.data);
			if(sgmtno1 == CMD_SYSPAR)
			{
				l2_comw_logHead->cntTable = D_L2_TXRV_LOG_INF_NONCYC;
			}
		}
		dataLen = sig->cprircv_inf.length - 2;
		break;
	case LBD_RRCTL:
	case LBD_RNRCTL:
	case LBD_REJCTL:
	case LBD_SBMCTL:
	case LBD_DISCCTL:
	case LBD_DMCTL:
	case LBD_UACTL:
	case LBD_FRMRCTL:
		pro_thd_no = CMD_TSKID_LPB;
		dataLen = 0;
		break;
	default :
		/* 他のフレーム処理なし */
		return;
		break;
	}

	log_info = &(l2_comw_logTbl->records[l2_comw_logHead->cntTable]);
	if((log_info->dt.year != 0) && (l2_comw_logHead->cntTable >= D_L2_TXRV_LOG_INF_NONCYC2))
	{
		/* レコードに既にログがある */

		/* 個別ログにコピーする */
		lblap_move_log(log_info);
		memset(log_info, 0, sizeof(T_RRH_L2_LOG_INFO));
	}
	/* 時刻取得 */
	cm_RtcGet(&(log_info->dt));

	/* 時刻補助情報 */
	/* 送受信情報 */
	log_info->tx_rv_flag = direction;

	/* プロセス/スレッド識別情報 */
	log_info->pro_thd_no = pro_thd_no;
	
	/*	リンク番号情報	*/
	log_info->link_num = link_num;

	/* I frameデータ長 */
	log_info->date_len = (USHORT)dataLen;
	
	/* LAPB アドレス */
	log_info->lapb_addr = sig->cprircv_inf.Address;
	
	/* LAPB コマンド */
	log_info->lapb_cmd = sig->cprircv_inf.Control;

	/* LAPB frame*/
	log_info->lapb_type = inter_event_no;
	
	/* check data length */
	if(log_info->date_len > D_RRH_L2_TXRV_LOG_DATA_LEN)
	{
		copyLen = D_RRH_L2_TXRV_LOG_DATA_LEN;
	}
	else
	{
		copyLen = log_info->date_len;
	}
	/* there is data to copy */
	memcpy(log_info->data, &(sig->signal_kind), copyLen);

	if(l2_comw_logHead->cntTable >= (D_RRH_L2_TXRV_LOG_INF_NUM - 1))
	{
		/* reset log index to start position of non-cyclical area */
		l2_comw_logHead->cntTable = D_L2_TXRV_LOG_INF_NONCYC2;
	}
	else
	{
		/* point next record */
		l2_comw_logHead->cntTable++;
	}

	return;
}												/*	END OF	"lblap_tx_rv_log"	*/
#endif
/********************************************************************************************************/

/*!
 *  @brief   FLASHからのログ展開処理 (lblap_init_log)
 *  @note    FLASHからのログ展開処理
 *  @return  None
 *  @date    2013.11.22 FFCS)hongj create for zynq.
 */


void lblap_init_log()
{
	T_RRH_L2_LOG_INFO_TABLE logTbl_tmp;
	USHORT	   index;
	USHORT	   copyCounter;

	/* 立ち上げ領域をクリア  */
	memset(l2_comw_logTbl, 0, sizeof(T_RRH_L2_LOG_INFO) * D_L2_TXRV_LOG_INF_NONCYC2);
	
	/* 個別ログをクリア  */
	memset(l2_comw_logSpec, 0, sizeof(T_RRH_L2_LOG_INFO_SPEC));
	memset(l2_comw_logHead->cntSpec, 0, sizeof(l2_comw_logHead->cntSpec));
	
	/*	次書込み位置が立ち上げ領域または次書込み位置の年情報が0の場合	*/
	if((l2_comw_logHead->cntTable<D_L2_TXRV_LOG_INF_NONCYC2)||
		(l2_comw_logTbl->records[l2_comw_logHead->cntTable].dt.year == 0))
	{
		index = D_L2_TXRV_LOG_INF_NONCYC2;
	}
	else
	{
		index = l2_comw_logHead->cntTable;
	}
	if(index != D_L2_TXRV_LOG_INF_NONCYC2)
	{
		copyCounter = D_RRH_L2_TXRV_LOG_INF_NUM - index;
		/* save the old part log to logTbl_tmp */
		memcpy(&logTbl_tmp.records[D_L2_TXRV_LOG_INF_NONCYC2],
			&l2_comw_logTbl->records[index], sizeof(l2_comw_logTbl->records[0]) * copyCounter);

		/* save the new part log to logTbl_tmp */
		memcpy(&logTbl_tmp.records[D_L2_TXRV_LOG_INF_NONCYC2 + copyCounter],
			&l2_comw_logTbl->records[D_L2_TXRV_LOG_INF_NONCYC2],
			sizeof(l2_comw_logTbl->records[0]) * (index - D_L2_TXRV_LOG_INF_NONCYC2));

		/* restore log to l2_comw_logTbl */
		memcpy(&l2_comw_logTbl->records[D_L2_TXRV_LOG_INF_NONCYC2],
			&logTbl_tmp.records[D_L2_TXRV_LOG_INF_NONCYC2],
			sizeof(l2_comw_logTbl->records[0]) * (D_RRH_L2_TXRV_LOG_INF_NUM - D_L2_TXRV_LOG_INF_NONCYC2));
	}

	/* set l2_comw_log_index to start postion */
	l2_comw_logHead->cntTable = 0;
	return;
}												/*	END OF	"lblap_init_log"   */

/********************************************************************************************************/

/*!
 *  @brief   異常プロトコル統計処理 (lblap_ablog)
 *  @note    該当する異常統計カウンタをインクリメントする処理を行う。異常多発時は､ABLOG-INDを通知する処理を行う。
 *  @param   link_num		[in]  リンク番号
 *  @param   cnt_area		[in]  統計タイプ
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
void lblap_ablog(USHORT link_num, UINT cnt_area)
{
												/*-------------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*-------------------------------*/
													/*-----------------------*/
													/*  統計カウンタ値 MAX ? */
													/*-----------------------*/
	if(lbw_ablog[link_num][lbw_abcnt[link_num]].ab_data[cnt_area] != 0xFFFFFFFF)
	{
													/*-----------------------*/
													/* NO  (統計可能)        */
													/*    Counter値更新      */
													/*-----------------------*/
		lbw_ablog[link_num][lbw_abcnt[link_num]].ab_data[cnt_area] += CMD_NUM1;
	}
	else
	{
		/* abort log count adds 1*/
		lbw_abcnt[link_num]++;
		/* abort log count is greater than 10*/
		if(lbw_abcnt[link_num] >= CMD_NUM10)
		{
		/*Set abort log count to 0*/
			lbw_abcnt[link_num] = CMD_NUM0;
		}
		/*Clear old data*/
		cm_MemClr(&lbw_ablog[link_num][lbw_abcnt[link_num]],sizeof(struct LBT_ABTLOG_DAT));
		/*Set current time to abort log count*/
		cm_RtcGet(&lbw_ablog[link_num][lbw_abcnt[link_num]].datetime);
													/*    Counter値更新      */
		lbw_ablog[link_num][lbw_abcnt[link_num]].ab_data[cnt_area] += CMD_NUM1;
	}
	return;
}												/*  END OF  "lblap_ablog"    */

/* @} */



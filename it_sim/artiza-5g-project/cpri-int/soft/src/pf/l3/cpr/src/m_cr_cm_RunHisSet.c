/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_cm_RunHisSet.c
 *  @brief  CPRI management task runhistory set function
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  /* CPRI 管理 task header file */

/**
* @brief CPRI management task runhistory set function
* @note save task start history and runhistory to table.\n
* @param sndrcv_flg [in] sending/receiving flag
* @param task_id [in]source task id/destination task id
* @param event_id [in] sending event id/receiving event id
* @param datap [in] buffer address pointer of sending/receiving message
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date   2015/7/30 TDIPS)ikeda rev.27665 L3 17リンク対応
*/
VOID m_cr_cm_RunHisSet(UINT sndrcv_flg, UINT task_id, UINT event_id, UCHAR* datap)
{
	T_RRH_SYSTEM_TIME	datetime;				/* 時刻情報			*/
	UINT		sig_kind;						/* 信号種別(保存用)	*/
	UINT		datas[8] = {0};
	USHORT linkno;
	
	if (crw_hissetflag == CMD_OFF)
	{
		return;
	}
	
	/************************/
	/* 収集データ初期値設定 */
	/************************/
	sig_kind = CMD_ALLF_SHORT;						/* 信号種別(保存用)	  */

	/******************/
	/* 収集データ設定 */
	/******************/
	switch (event_id)
	{
	/* 受信イベント */
	case CMD_TSKIF_CPRISTACHGNTC: /* CPRI State変化通知 */
		linkno = ((T_API_CPRILINK_STATE_NTC *)datap)->link_num;
		datas[0] = ((T_API_CPRILINK_STATE_NTC *)datap)->cpriHDLCBitrate;
		datas[1] = ((T_API_CPRILINK_STATE_NTC *)datap)->cpriLineBitrate;
		datas[2] = ((T_API_CPRILINK_STATE_NTC *)datap)->cpriHdlc;
		datas[3] = ((T_API_CPRILINK_STATE_NTC *)datap)->cpriEDown;
		datas[4] = ((T_API_CPRILINK_STATE_NTC *)datap)->hfnSync;
		datas[5] = ((T_API_CPRILINK_STATE_NTC *)datap)->cpriState;
		break;

	/* 受信イベント(ヘルスチェックなど定期的に動作するため) */
	case CMD_TSKIF_CPRIRCVNTC: /* CPRI信号受信通知 */
//		linkno = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)datap)->cprircv_inf.link_num;
//		sig_kind = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datap)->signal_kind;
		return;

	/* 送信イベント(ヘルスチェックなど定期的に動作するため) */
	case CMD_TSKIF_CPRISNDNTC: /* CPRI信号送信通知 */
//		linkno = ((CMT_TSKIF_CPRISND_SIGNALGET *)datap)->cprisnd_inf.link_num;
//		sig_kind = ((CMT_TSKIF_CPRISND_SIGNALGET*)datap)->signal_kind;
		return;

	/* 受信イベント */
	case CMD_TSKIF_L2STANTC:
		linkno = ((T_API_L2_LINKSTATE_CHG_NTC *)datap)->data.link_num;
		datas[0] = ((T_API_L2_LINKSTATE_CHG_NTC *)datap)->data.state;
		break;
		
	/* 送信イベント */
	case CMD_TSKIF_L3STANTC:
		linkno = ((CMT_TSKIF_L3STATENTC *)datap)->link_num;
		datas[0] = ((CMT_TSKIF_L3STATENTC *)datap)->system_type;
		datas[1] = ((CMT_TSKIF_L3STATENTC *)datap)->onOff;
		break;

	/* 送信イベント */
	case CMD_TSKIF_CPRI_DISCONNECT_NTC:
		linkno = ((CMT_TSKIF_CPRIDISCONN_NTC *)datap)->link_num;
		datas[0] = ((CMT_TSKIF_CPRIDISCONN_NTC *)datap)->system_type;
		datas[1] = ((CMT_TSKIF_CPRIDISCONN_NTC *)datap)->layer_kind;
		break;

	/* 送信イベント */
	case CMD_TSKIF_CPRI_CONNECT_NTC:
		linkno = ((CMT_TSKIF_CPRICONN_NTC *)datap)->link_num;
		datas[0] = ((CMT_TSKIF_CPRICONN_NTC *)datap)->system_type;
		break;

	/* 送信イベント */
	case CMD_TSKIF_REC_CPRI_PRE_RSP:
		linkno = D_RRH_CPRINO_REC;
		break;

	/* 受信イベント */
	case CMD_TSKIF_REC_CPRI_PRE_REQ:
		linkno = D_RRH_CPRINO_REC;
		break;

	/* 受信イベント */
	case CMD_TSKIF_RE_CPRI_START_NTC:
		linkno = ((CMT_TSKIF_RE_CPRILNKSTART_NTC *)datap)->link_num;
		datas[0] = ((CMT_TSKIF_RE_CPRILNKSTART_NTC *)datap)->sfpLogTrigger;
		break;

	/* 受信イベント */
	case CMD_TSKIF_RECMPSTPNTC:
		linkno = ((CMT_TSKIF_RECMPSTPNTC *)datap)->link_num;
		datas[0] = ((CMT_TSKIF_RECMPSTPNTC *)datap)->system_type;
		datas[1] = ((CMT_TSKIF_RECMPSTPNTC *)datap)->stopFactor;
		break;

	/* 受信イベント */
	case CMD_TSKIF_REC_CPRI_START_NTC:
		linkno = D_RRH_CPRINO_REC;
		break;

	/* 受信イベント */
	case CMD_TSKIF_RE_CPRI_STOP_NTC:
		linkno = ((CMT_TSKIF_RE_CPRILNKSTOP_NTC *)datap)->link_num;
		break;
		
	default:
		return;
	}

	/****************************/
	/* 収集データテーブル書込み */
	/****************************/
	/* 時刻情報収集 */
	cm_RtcGet( &datetime );
	/* ヘッダデータ設定 */
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.setFlg	 = CMD_ON;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.month      = datetime.month;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.day        = datetime.day;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.hour       = datetime.hour;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.min        = datetime.min;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.sec        = datetime.sec;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.msec       = datetime.msec;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.sndrcvFlg = (UCHAR)sndrcv_flg;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.msgId   =  event_id;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.taskId    = (USHORT)task_id;
	crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].head.sig_kind   = (USHORT)sig_kind;
	cm_MemCpy(crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx].body,datas,sizeof(datas));

	/**********************/
	/* 次走行履歴書込準備 */
	/**********************/
	/* 書込位置更新 */
	crw_runhis_tbl[linkno].write_idx++;
	if (crw_runhis_tbl[linkno].write_idx >= CRD_RUNHIS_FACENUM_MAX)
	{
		crw_runhis_tbl[linkno].write_idx = CMD_NUM0;
	}
	/* 次情報書込みエリア0クリア                                */
	/* (ログが一周したときに、どの面が最新かわかりやすいように) */
	cm_MemPadc((UCHAR*)&crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx], CMD_NUM0,
				sizeof(crw_runhis_tbl[linkno].inf[crw_runhis_tbl[linkno].write_idx]));

	return;
}

VOID m_cr_cm_RunHisNG(USHORT linkno)
{
	UINT	write_idx;

	if (crw_hissetflag == CMD_OFF)
	{
		return;
	}

	if (crw_runhis_tbl[linkno].write_idx == CMD_NUM0)
	{
		write_idx = CRD_RUNHIS_FACENUM_MAX-CMD_NUM1;
	}
	else
	{
		write_idx = crw_runhis_tbl[linkno].write_idx-CMD_NUM1;
	}

	crw_runhis_tbl[linkno].inf[write_idx].head.sndrcvFlg = CRD_RUNHIS_FLG_SND_NG;
}

void m_cr_logPrint(FILE *outf)
{
	UINT index;
	UINT printIdx;
	UINT bodyIdx;
	CHAR *strSndRcv[] = {"RCV","SND"};
	CHAR *bitratename[] = { "2.4G","4.9G","9.8G" };
	CHAR *typename[] = { "3G", "S3G" };

	USHORT linkno;
	USHORT system_type;
	
	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++ )
	{
		fprintf(outf,"L3 cpr running history CPRINO %03d----------------------------------------\n", linkno);

		printIdx = 0;
		for(index = 0; index < CRD_RUNHIS_FACENUM_MAX;index++)
		{
			if(crw_runhis_tbl[linkno].inf[index].head.setFlg == 0)
				continue;

			fprintf(outf,"No.%03d [%02x-%02x %02x:%02x:%02x.%03x] %s;%08x;%04x;%04x\n",printIdx,
				crw_runhis_tbl[linkno].inf[index].head.month,
				crw_runhis_tbl[linkno].inf[index].head.day,
				crw_runhis_tbl[linkno].inf[index].head.hour,
				crw_runhis_tbl[linkno].inf[index].head.min,
				crw_runhis_tbl[linkno].inf[index].head.sec,
				crw_runhis_tbl[linkno].inf[index].head.msec,
				strSndRcv[crw_runhis_tbl[linkno].inf[index].head.sndrcvFlg],
				crw_runhis_tbl[linkno].inf[index].head.msgId,
				crw_runhis_tbl[linkno].inf[index].head.taskId,
				crw_runhis_tbl[linkno].inf[index].head.sig_kind);

				for(bodyIdx = 0;bodyIdx < 8;bodyIdx++)
				{
					fprintf(outf," %02x%02x %02x%02x",
						crw_runhis_tbl[linkno].inf[index].body[bodyIdx * 4],
						crw_runhis_tbl[linkno].inf[index].body[bodyIdx * 4 + 1],
						crw_runhis_tbl[linkno].inf[index].body[bodyIdx * 4 + 2],
						crw_runhis_tbl[linkno].inf[index].body[bodyIdx * 4 + 3]);

					if((bodyIdx % 4) == 3) fprintf(outf,"\n");
				}

			printIdx++;
		}

		for ( system_type = CMD_SYS_3G; system_type < CMD_MAX_SYSNUM; system_type++ )
		{
			fprintf(outf,"L3 cpr cpri state type %s ----------------------------------------\n", typename[system_type]);
			fprintf(outf,"cmw_cprilnkstatbl.cprilnkstano: %d\n",cmw_cprilnkstatbl[linkno].cprilnkstano[system_type]);
			fprintf(outf,"cmw_cprilnkstatbl.cprista: %d \n",cmw_cprilnkstatbl[linkno].cprista);
			fprintf(outf,"cmw_cprilnkstatbl.hdlcsta: %d \n",cmw_cprilnkstatbl[linkno].hdlcsta);
			fprintf(outf,"cmw_cprilnkstatbl.stafflg: %d \n",cmw_cprilnkstatbl[linkno].stafflg);
			fprintf(outf,"cmw_cprilnkstatbl.L2abnminfo: %d \n",cmw_cprilnkstatbl[linkno].L2abnminfo);
			fprintf(outf,"cmw_cprilnkstatbl.hcsta[%s]: %d \n",typename[system_type], cmw_cprilnkstatbl[linkno].hcsta[system_type]);

			if (linkno < D_RRH_CPRINO_RE_MIN)
			{
				fprintf(outf,"-----------The information of crw_hc_ctl_tbl(%s)-----------\n", typename[system_type]);
				fprintf(outf,"crw_hc_ctl_tbl[%d]\n", system_type);
				fprintf(outf,".re_state           :%d\n", crw_hc_ctl_tbl[system_type].re_state);
				fprintf(outf,".to_count           :%d\n", crw_hc_ctl_tbl[system_type].to_count);

				fprintf(outf,"-----------The information of cmw_toffset_tbl(%s)-----------\n", typename[system_type]);
				fprintf(outf,"cmw_toffset_tbl[%d]\n", system_type);
				fprintf(outf,".toffset            :%d\n", cmw_toffset_tbl[system_type].toffset);
				fprintf(outf,".ul_delay           :%d\n", cmw_toffset_tbl[system_type].ul_delay);
				fprintf(outf,".dl_delay           :%d\n", cmw_toffset_tbl[system_type].dl_delay);
			}
			else
			{
				fprintf(outf,"-----------The information of crw_hc_ctl_mst_tbl(%s)-----------\n", typename[system_type]);
				fprintf(outf,"crw_hc_ctl_mst_tbl[%d][%d]\n", linkno, system_type);
				fprintf(outf,".hlc_eststs        :%d\n", crw_hc_ctl_mst_tbl[linkno][system_type].hlc_eststs );
				fprintf(outf,".hlc_nores_cnt     :%d\n", crw_hc_ctl_mst_tbl[linkno][system_type].hlc_nores_cnt );
				fprintf(outf,".hlc_sndsts        :%d\n", crw_hc_ctl_mst_tbl[linkno][system_type].hlc_sndsts );
				fprintf(outf,".hlc_srtupsts      :%d\n", crw_hc_ctl_mst_tbl[linkno][system_type].hlc_srtupsts );
			}

			fprintf(outf,"valid system info:%d\n", cmw_validsysinfo);
			fprintf(outf,"system connect info:%d\n", cmw_connectinfo);
			fprintf(outf,"device kind:%d\n", cmw_device_kind);
			fprintf(outf,"RE Kind:%d\n", cmw_reKind);

			fprintf(outf,"-----------The information of cmw_hcsta(%s)-----------\n", typename[system_type]);
			fprintf(outf,"cmw_hcsta[%d]        :%d\n", system_type, cmw_hcsta[linkno][system_type]);

			fprintf(outf,"Line Bit Rate:%s(%d)\n", bitratename[cmw_linebitrate], cmw_linebitrate);
		}
	}

 	return;
}
/* @} */


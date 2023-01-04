/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReCntMain.c
 *  @brief  RE control task main process.
 *   - It receives messages from INI/LPB/RSV task.
 *   - And implement relative RE control functions .
 *  @date   2008/07/16 FFCS) Linlj create.
 *  @date   2013/11/05 FFCS)Xut modify for ZYNQ.
 *  @date   2015/04/25 FFCS)Zhangxq TDD
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/********************************************************************************************************************/
/** @addtogroup RRH_L3_RCT
 * @{ */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"
/* @{ */
/*****************************************************************************************************************//**
 *  @brief  RE control task main process.
 *  @param  tskid  [in] msg source task id
 *  @param  event_no  [in] msg id
 *  @param  buff_adr  [in]  the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01689
 *  @Bug_No M-RRU-ZSYS-01747
 *  @Bug_No M-RRU-ZSYS-01866
 *  @Bug_No M-RRU-ZSYS-01923
 *  @Bug_No M-RRU-ZSYS-01926
 *  @date 2011/07/21 FJT)Tokunaga M-S3G-eNBPF-04059:[IT3]REリモート閉塞時の(3003)カード状態監視開始マクロメール通知について
 *  @date 2015/05/31 FFCS)zhangxq update for nt2r<--->nt4r syspara update
 *  @date 2015/07/28 TDI)satou PORT状態変化通知の分岐を削除
 *  @date 2015/07/29 TDI)satou 「Local Block Control Request」を削除
 *  @date 2015/08/13 TDIPS)ikeda RE増減設コーディングミス修正
 *  @date 2015/08/24 TDIPS)maruyama CPRI接続通知受信時の処理見直し
 *  @date 2015/09/17 TDIPS)Lay M-RRU-ZSYS-01866 TRX設定応答のcaseを追加
 *  @date 2015/09/24 TDIPS)Lay M-RRU-ZSYS-01720 FLD-MT接続強制解放要求受信時にRECに応答が返らない不具合を修正
 *  @date 2015/09/30 TDIPS)maruyama M-RRU-ZSYS-01689 シーケンス見直しによる変更
 *                    CMD_TSKIF_ALLTSKININTC時（REC CPRI Link Preparation Request通知前)にREのLayer3状態を①起動待ちアイドル状態
 *                    CMD_TSKIF_CPRI_CONNECT_NTC時RECの確立であれば対象REのLayer3状態を運用中にし、re_StartUp_First_flgをON
 *                    フェールセーフ追加
 *                    CMD_TSKIF_RE_STARTUP_RSP時、全て立ち上げ済の場合re_StartUp_CPRI_Conn_flgをON
 *  @date 2015/10/05 TDI)satou 「RE use status set request」追加
 *  @date 2015/10/05 TDIPS)sasaki M-RRU-ZSYS-01747 リセットされて再起動した配下REの最新RE状態を取得していない不具合を修正
 *  @date 2015/10/07 TDIPS)sasaki update デバッグログ追加
 *  @date 2015/11/09 TDIPS)sasaki IT2問処No.163対処
 *  @date 2015/11/13 TDI)satou その他未実装-013 「CPRI restart indication」追加
 *  @date 2016/01/29 TDI)satou eNB-F-15B-3201-00115 D_API_MSGID_CPRI_DLY_ADJ_STEP10_RSPを追加
 *  @date 2016/03/03 TDI)enoki TILTによる配下REリセット後起動にてRE自律リセット抑止解除要求をREプロセスに送信
 *  @date 2016/11/22 FJT)ohashi TILTリセットによるスロット情報初期化処理抑止
 *  @date 2017/01/25 FJT)ohashi 3201-16B-TS96 LOG強化
 *  @date 2021/01/15 M&C)Huan.dh new intermediate RE Card control and status handle
 *  @date 2017/01/25 M&C)Huan.dh add case CARONOFF RE response handle
 *  @date 2021/02/09 M&C)Huan.dh add RE PORT status report judge
 *  @date 2021/02/19 M&C)Tri.hn Update based on No.67 (sequence 3-30) - Add case CMD_CPRID_RECRDDIARES
 *

 ********************************************************************************************************************/
VOID m_rc_ReCntMain( UINT tskid, UINT event_no, VOID *buff_adr )
{
	CMT_TSKIF_CPRIRCV_SIGNALGET *cpri_sig_p;/* CPRI信号種別					*/
	UINT	ret;		/* 関数戻り値	*/
	UINT	cnt;		/* ループカウンタ	*/
    CMT_TSKIF_CPRICONN_NTC      *p_cpriConnNtc;
    CMT_TSKIF_RESTR_RSP         *p_reStartupRsp;
    INT delayPrmChkEn;

	/* 初期化 */
	cpri_sig_p = NULL;

#ifdef FHM_RCT_DEBUG_FOR_IT1
    printf( "%s %s %d <event_no>0x%x\n", __FILE__, __FUNCTION__, __LINE__, event_no);
#endif

	/* 入力パラメータ(起動要因)をチェックし要因毎に関数コールを実施 */
	switch (event_no)
	{
		/* ■初期化要求時■	*/
		case CMD_TSKIF_INIREQ:
			/* タスク状態が初期状態の場合のみ処理を実施	*/
			if(rcw_tskstn == RCD_TSK_STN_INI)
			{
				/* 初期化関数をコール	*/
				m_rc_Ini();

			    /* LED設定処理 */
			    m_cm_WriteLEDReg();

			    /* 停波設定/解除 */
			    m_rc_SetWave();

				/* タスク状態更新(通常運用状態)	*/
				rcw_tskstn = RCD_TSK_STN_NORMAL;

				re_StartUp_CPRI_Conn_flg = 0;	/* RE	*/
			}
			else
			{
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0,
							"m_rc_ReCntMain TSK_STA_TRS_NG_INIREQ");
			}
			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );
			break ;

		/* ■CPRI信号受信通知受信時■						*/
		case CMD_TSKIF_CPRIRCVNTC :

			/* CPRI信号種別取得 */
			cpri_sig_p = (CMT_TSKIF_CPRIRCV_SIGNALGET *)buff_adr;

			/*  get cpri message system type */
			rcw_cpri_msg_flg = cpri_sig_p->signal_kind & CMD_SYS_CHK_CPRID;

			/* タスク状態が運用状態の場合のみ処理を実施 */
			if(rcw_tskstn == RCD_TSK_STN_NORMAL)
			{
				/* 受信メッセージアドレス内のCPRI信号要求内容から	*/
				/* 対象関数をコール									*/
				switch (cpri_sig_p->signal_kind & CMD_SYS_MSK_CPRID)
				{

					/* ■FLD-MT接続解放応答■							*/
					case CMD_CPRID_FLMTCNCRELRES:
						/* FLD-MT接続解放応答function is called					*/
						m_rc_ReFldDiscRspRcv
								((CMT_TSKIF_CPRISND_FLMTCNCRELRES *)buff_adr);
						break;
					/* ■RE時刻設定要求時■							*/
					case CMD_CPRID_RETIMSETREQ:
						/* RE時刻設定関数をコール					*/
						m_rc_ReTimSet
								((CMT_TSKIF_CPRISND_RETIMSETREQ *)buff_adr);
						break ;

					/* ■REリセット要求時■							*/
					case CMD_CPRID_RERSTREQ:
						/* REリセット関数をコール					*/
						m_rc_ReRst
								((CMT_TSKIF_CPRIRCV_RERSTREQ *)buff_adr);
						break ;

					/* ■Toffset値情報報告要求時■					*/
					case CMD_CPRID_TOFFSETINFREQ:
						/* Toffset値情報報告関数をコール			*/
						m_rc_ToffsetRep(buff_adr);
						break ;

					/* ■無変調キャリア出力ON/OFF指定要求時■					*/
					case CMD_CPRID_CARONOFFREQ:
						/* 無変調キャリア出力ON/OFF指定関数をコール			*/
						m_rc_UnCarOptOnOffDesReq( (CMT_TSKIF_CPRISND_CARONOFFREQ*) buff_adr);
						break ;

					/* ■無変調キャリア出力ON/OFF指定応答■					*/
					case CMD_CPRID_CARONOFFRES:
						/* 無変調キャリア出力ON/OFF指定関数をコール			*/
						m_rc_CarOnOff_Rcv_Rsp( (CMT_TSKIF_CPRISND_CARONOFFRES_S3G*) buff_adr);
						break ;

					/* ■RE装置構成情報報告要求時■					*/
					case CMD_CPRID_EQPSTRREQ:
						/* RE装置構成情報報告関数をコール			*/
						m_rc_ReStrRep(buff_adr);

						break ;

					/* ■REスロット情報報告要求時■					*/
					case CMD_CPRID_SLOTINFREQ:
						/* REスロット情報報告関数をコール			*/
						m_rc_ReSlotRep
							(buff_adr);
						break ;

					/* ■REスロット情報報告応答■					*/
					case CMD_CPRID_SLOTINFRES:
						/* REスロット情報処理			        */
						m_rc_SlotInf_Rcv_Rsp
							(buff_adr);
						break ;

					/* ■RE状態報告要求時■							*/
					case CMD_CPRID_RESTSREQ:
						/* RE状態報告関数をコール					*/
						m_rc_ReStaRep
						((CMT_TSKIF_CPRIRCV_RESTSREQ* )buff_adr);
						break ;

					/* ■RE PORT状態報告要求時■					*/
					case CMD_CPRID_REPORTSTSREQ:
						/* RE PORT状態報告関数をコール				*/
						//No intermediate device station data or
						//when there is intermediate device station data (LTE independent mode)
						if (cmw_connectoperatemode < CMD_OP_NR)
							{
								/* Call forward request RE PORT function				*/
								m_rc_RePortStaReq_LTE(buff_adr);
							}
						else
							{
								m_rc_RePortStaRep(buff_adr);
							}
						break ;

					/* ■RE PORT status report response■					*/
					case CMD_CPRID_REPORTSTSRES:
						//No intermediate device station data or
						//when there is intermediate device station data (LTE independent mode)
						if (cmw_connectoperatemode < CMD_OP_NR)
							{
							/* Call RE PORT function				*/
							m_rc_RePortStaRep_LTE
									((CMT_TSKIF_CPRIRCV_REPORTSTSRES *)buff_adr);
							}
						break ;

					/* ■REカード状態報告要求時■					*/
					case CMD_CPRID_CARDSTSREQ:
						/* REカード状態報告関数をコール				*/
						m_rc_ReCardStaRep
								((CMT_TSKIF_CPRIRCV_CARDSTSREQ *)buff_adr);

						break ;
					/* ■REカード制御応答■					*/
					case CMD_CPRID_RECARDCNTRES:
						/* REカード制御関数をコール		*/
						m_rc_ReCardCntRes
								((CMT_TSKIF_CPRIRCV_RECARDCNTRES *)buff_adr);

						break ;

					/* ■REカード状態報告応答■					*/
					case CMD_CPRID_CARDSTSRES:
						//No intermediate device station data or
						//when there is intermediate device station data (LTE independent mode)
						if (cmw_connectoperatemode < CMD_OP_NR)
							{
							/* REカード状態報告関数をコール				*/
							m_rc_ReCardStaRep_LTE
									((CMT_TSKIF_CPRIRCV_CARDSTSRES *)buff_adr);
							}

						break ;

					/* ■REキャリア状態報告要求時■					*/
					case CMD_CPRID_CARLSTSREQ:
						/* REキャリア状態報告関数をコール			*/
						m_rc_ReCarStaRep(buff_adr);
						break ;

					/* ■TRX設定要求時■							*/
					case CMD_CPRID_TRXSETREQ:
						/* recスレッドにTRX設定要求を受信し、転送する */
						m_rc_TrxSet_Transfer_Req(buff_adr);
						break ;

					/* ■TRX解放要求時■							*/
					case CMD_CPRID_TRXRELREQ:
						/* recスレッドにTRX解放要求を受信し、転送する */
						m_rc_TrxRel_Transfer_Req
								((CMT_TSKIF_CPRIRCV_TRXRELREQ *)buff_adr);
						break ;

					/* ■REバージョン報告要求時■					*/
					case CMD_CPRID_REVERREQ:
						/* REバージョン報告関数をコール				*/
						m_rc_ReVerRep
								((CMT_TSKIF_CPRIRCV_REVERREQ *)buff_adr);
						break ;

					/* ■REカード制御要求時■						*/
					case CMD_CPRID_RECARDCNTREQ:
						/* REカード制御関数をコール					*/
						m_rc_ReCardCnt
								((CMT_TSKIF_CPRIRCV_RECARDCNTREQ *)buff_adr);
						break ;

					/* ■RE PORT制御要求時■						*/
					case CMD_CPRID_REPORTCNTREQ:
						/* RE PORT制御関数をコール					*/
						m_rc_RePortCntReq
								((CMT_TSKIF_CPRIRCV_REPORTCNTREQ *)buff_adr);
						break ;

					/* ■ SV-MT/eNB-MT接続強制解放要求■					*/
					case CMD_CPRID_SVMTFORRELREQ:
						/* SV-MT/eNB-MT接続強制解放要求蛛E逅・*/
						m_rc_SvMt_ForReleaseConnectReq
								((CMT_TSKIF_CPRIRCV_SVMTFORRELREQ *)buff_adr);
						break;
					/* ■FLD-MTデータコマンド転送受信応答■				*/
					case CMD_CPRID_FLDCOMRCVRES:
						/* FLD-MTデータコマンド転送受信応答関数をコール */
						m_rc_Fld_CmdRcvRes
								((CMT_TSKIF_CPRIRCV_FLDCOMRCVRES *)buff_adr);
						break;
					/* ■FLD-MTデータファイル転送送信要求■		*/
					case CMD_CPRID_FLDFILSNDREQ:
						/* FLD-MTデータファイル転送送信関数をコール */
						m_rc_Fld_FilSndReq
								(buff_adr);
						break;

					/* FLD-MTデータコマンド転送送信要求 */
					case CMD_CPRID_FLDCOMSNDREQ:
						/* FLD-MTデータコマンド転送送信要求*/
						m_rc_FldMt_DataCmdTransferSndReq
								((CMT_TSKIF_CPRIRCV_FLDCOMSNDREQ *)buff_adr);
						break;

					/* FLD-MTデータファイル転送受信応答*/
					case CMD_CPRID_FLDFILRCVRES:
						m_rc_FldMt_FileTransRes
								((CMT_TSKIF_CPRIRCV_FLDFILRCVRES *)buff_adr);
						break;
					/*  FLD-MT接続応答 */
					case CMD_CPRID_FLDMTCNCRES:
						/*  FLD-MT接続応答処理 */
						m_rc_FldMt_ConnectResp
								((CMT_TSKIF_CPRIRCV_FLDMTCNCRES*)buff_adr);
						break;

					/* ■SV-MT/eNB-MTータ受信応答■					*/
					case CMD_CPRID_SVMTDATRCVRES:
						m_rc_RcvSVMTDatRcvRes
								((CMT_TSKIF_CPRIRCV_SVMTDATRCVRES *)buff_adr);
						break;

					/* SV-MT/eNB-MT接続応答 */
					case CMD_CPRID_SVMTCNTRES:
						/* SV-MT/eNB-MT接続応答処理 */
						m_rc_SvMt_ConnectResp
								((CMT_TSKIF_CPRIRCV_SVMTCNTRES*)buff_adr);
						break;
					/* ■ FLD-MT接続強制解放要求■					*/
					case CMD_CPRID_FLMTCMPRELREQ:
						/* FLD-MT接続強制解放要求処理 */
						m_rc_Fld_Mt_ForReleaseConnectReq
								((CMT_TSKIF_CPRIRCV_FLMTCMPRELREQ *)buff_adr);
						break;
					/* ■SV-MT/eNBデータ送信要求■					*/
					case  CMD_CPRID_SVMTDATSNDREQ:
						/* SV-MT/eNBデータ送信要求処理 */
						  m_rc_SvMt_DatSndReq
								((CMT_TSKIF_CPRIRCV_SVMTDATSNDREQ *)buff_adr);
						break;

					/* ■外部装置データ送信要求■					*/
					case  CMD_CPRID_EXTDATSNDREQ:
						/* 外部装置データ送信要求処理 */
						 m_rc_ExtDataSndReq((CMT_TSKIF_CPRIRCV_EXTDATSNDREQ *)buff_adr);
						 break;

					/* 外部装置データ受信応答 */
					case CMD_CPRID_EXTDATRCVRES:
						/* 外部装置データ受信応答処理 */
						m_rc_ExtDataRcvRsp((CMT_TSKIF_CPRIRCV_EXTDATRCVRES *)buff_adr);
						break;
					/* MTアドレス設定要求（REC) */
					case CMD_CPRID_MTADDSETREQ:
						/* MTアドレス設定要求処理 */
						m_rc_Mt_AddressSetReq( (CMT_TSKIF_CPRIRCV_MTADDSETREQ*) buff_adr );
						break;

					/* FTPデータ送信用PORT番号設定要求 */
					case CMD_CPRID_FTPPORTSETREQ:
						/* FTPデータ送信用PORT番号設定処理 */
						m_rc_FtpPortSetReq( (CMT_TSKIF_CPRISND_PORTSETREQ*) buff_adr );
						break;

					/* REカード診断要求 */
					case CMD_CPRID_RECRDDIAREQ:
						/* REカード診断要求処理 */
						m_rc_ReCardDiaReq( (CMT_TSKIF_CPRIRCV_RECRDDIAREQ*) buff_adr );
						break;

					/* REカード情報報告要求 */
					case CMD_CPRID_CARDINFREQ:
						/* REカード情報報告処理 */
						m_rc_ReCardInfoRepReq( (VOID*) buff_adr );
						break;

					/* ■SV-MT/eNB-MT接続解放応答	■					*/
					case CMD_CPRID_SVMTCNTRELRES:
						/* SV-MT/eNB-MT接続解放応答処理 */
						m_rc_RcvSVMTCntRelRes
								( (CMT_TSKIF_CPRIRCV_SVMTCNTRELRES* )buff_adr );
					break;

					default :
						/* 想定外の要求を受信した場合はログ出力する	*/
						cm_Assert(	CMD_ASL_USELOW,
									cpri_sig_p->signal_kind,
									"m_rc_ReCntMain CPRI_Sig_ABNORMAL");
						break ;
				} /* CPRI信号要求内容による分岐END	*/

			} /* タスク状態が運用状態の分岐END */

			/* タスク状態が運用中以外にCPRI信号を受信した場合 */
			else
			{
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0,
							"m_rc_ReCntMain TSK_STA_TRS_NG_CPRIRCV_NTC");
			}
			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break ;
		/*(0x4003)REC Forwarding Notice (re/rec->L3)*/
		case CMD_TSKIF_REC_FWDNTC:
			/* CPRI信号種別取征E*/
			cpri_sig_p = (CMT_TSKIF_CPRIRCV_SIGNALGET *)buff_adr;

			/*  get cpri message system type */
			rcw_cpri_msg_flg = cpri_sig_p->signal_kind & CMD_SYS_CHK_CPRID;
#ifdef FHM_RCT_DEBUG_FOR_IT1
    printf( "%s %s %d <signal_kind>0x%x rcw_tskstn:%d\n", __FILE__, __FUNCTION__, __LINE__, cpri_sig_p->signal_kind, rcw_tskstn);
#endif

			/* タスク状態が運用状態縺E場合縺Eみ処逅・ｒ実施 */
			if(rcw_tskstn == RCD_TSK_STN_NORMAL)
			{
				/* 受信メチEそージアドレス蜀・・CPRI信号要求蛛E容から	*/
				/* 対象関数をコール									*/
				switch (cpri_sig_p->signal_kind & CMD_SYS_MSK_CPRID)
				{
					/* ■ SV-MT/FLD-MT接続強制解放応答■					*/
					case CMD_CPRID_SVMTFORRELRES:
						m_rc_SvMt_ForReleaseConnectRes
								((CMT_TSKIF_CPRIRCV_SVMTFORRELRES *)buff_adr);
						break;
					/* ■ SV-MT/FLD-MT接続強制解放応答■					*/
					case CMD_CPRID_FLDCOMSNDRES:
						m_rc_FldMt_DataCmdTransferSndResRcv
								((CMT_TSKIF_CPRISND_FLDCOMSNDRES *)buff_adr);
						break;
					/* ■ FLD-MT接続解放要求蛛E逅・蔓					*/
					case CMD_CPRID_FLMTCNCRELREQ:
						m_rc_ReFldMReDiscReqRcv
								((CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ *)buff_adr);
						break;
					/* ■FLD-MTデータコマンド転送受信要求時■		*/
					case CMD_CPRID_FLDCOMRCVREQ:
						/* FLD-MTデータコマンド転送受信関数をコール */
						m_rc_Fld_CmdRcvReq
								((CMT_TSKIF_CPRIRCV_FLDCOMRCVREQ *)buff_adr);
						break;
					/* ■FLD-MTデータファイル転送送信応答■				*/
					case CMD_CPRID_FLDFILSNDRES:
						/* FLD-MTデータファイル転送送信応答関数をコール */
						m_rc_Fld_FilSndRes(buff_adr);
						break;

					/* ■FLD-MTデータファイル転送受信要求■				*/
					case CMD_CPRID_FLDFILRCVREQ:
						/* FLD-MTデータファイル転送受信要求 */
						m_rc_FldMt_FileTransReq(buff_adr);
						break;

					/* ■SV-MT/eNB-MT接続解放要求	■					*/
					case CMD_CPRID_SVMTCNTRELREQ:
						/* SV-MT/eNB-MT接続解放要求処理 */
						m_rc_SndSVMTCntRelReq
								( (CMT_TSKIF_CPRIRCV_SVMTCNTRELREQ* )buff_adr );
						break;

					/* FLD-MT接続要求 */
					case CMD_CPRID_FLDMTCNCREQ:
						/* FLD-MT接続要求処理 */
						m_rc_FldMt_ConnectReq
								((CMT_TSKIF_CPRIRCV_FLDMTCNCREQ*)buff_adr);
						break;

					/* ■ SV-MT/eNB-MTータ受要求■					*/
					case CMD_CPRID_SVMTDATRCVREQ:
						/*SV-MT/eNB-MTータ受要求処理 */
						m_rc_SndSVMTDatRcvReq
								((CMT_TSKIF_CPRIRCV_SVMTDATRCVREQ *)buff_adr);
						break;

					/* SV-MT/eNB-MT接続要求 */
					case CMD_CPRID_SVMTCNTREQ:
						/* SV-MT/eNB-MT接続要求処理 */
						m_rc_SvMt_ConnectReq
								((CMT_TSKIF_CPRIRCV_SVMTCNTREQ*)buff_adr);
						break;
					/* ■ FLD-MT接続強制解放応答■					*/
					case CMD_CPRID_FLMTCMPRELRES:
						m_rc_Fld_Mt_ForReleaseConnectRes
								((CMT_TSKIF_CPRIRCV_FLMTCMPRELRES *)buff_adr);
						break;
					/* ■SV-MT/eNBデータ送信応答■					*/
					case CMD_CPRID_SVMTDATSNDRES:
					/* SV-MT/eNBデータ送信応答処理					*/
						  m_rc_SvMt_DatSndRes
								((CMT_TSKIF_CPRIRCV_SVMTDATSNDRES *)buff_adr);
						break;

					/* ■REバージョン報告応答■*/
					case CMD_CPRID_REVERRES:
						m_rc_ReVerRepRes( (CMT_TSKIF_CPRIRCV_REVERRES*) buff_adr );
						break;

					/* 外部装置データ送信応答 */
					case CMD_CPRID_EXTDATSNDRES:
						/* 外部装置データ送信応答処理 */
						m_rc_ExtDataSndRsp((CMT_TSKIF_CPRIRCV_EXTDATSNDRES*) buff_adr);
						break;
					/* 外部装置データ受信要求 */
					case CMD_CPRID_EXTDATRCVREQ:
						/* 外部装置データ受信要求処理 */
						m_rc_ExtDataRcvReq((CMT_TSKIF_CPRIRCV_EXTDATRCVREQ *)buff_adr);
						break;
					/* MTアドレス設定応答 */
					case CMD_CPRID_MTADDSETRES:
						/* MTアドレス設定応答 */
						 m_rc_Mt_AddressSetRsp( (CMT_TSKIF_CPRIRCV_MTADDSETRES*) buff_adr );
						break;
					/* ■TRX解放応答 ■*/
					case CMD_CPRID_TRXRELRES:
						m_rc_TrxRel_Rcv_Rsp
								((CMT_TSKIF_CPRISND_TRXRELRES *)buff_adr);
						break;
					/* ■Receive TRX設定応答 from re/rec ■*/
					case CMD_CPRID_TRXSETRES:
						m_rc_TrxSet_Rcv_Rsp
								((CMT_TSKIF_CPRISND_TRXSETRES *)buff_adr);
						break;
					/* REカード情報報告応答 */
					case CMD_CPRID_CARDINFRES:
						m_rc_ReCardInfoRepRsp( buff_adr );
						break;
					/* RE装置構成情報報告応答 */
					case CMD_CPRID_EQPSTRRES:
						m_rc_ReStrRes((CMT_TSKIF_CPRISND_EQPSTRRES_S3G*)buff_adr);
						break;
					/* REキャリア状態報告応答 */
					case CMD_CPRID_CARLSTSRES:
						/* REキャリア状態報告応答 */
						m_rc_ReCarStaRepRes_S3G( (CMT_TSKIF_CPRISND_CARLSTSRES*) buff_adr );
						break;
					/* REカード診断要求 */
					case CMD_CPRID_RECRDDIARES:
						/* REカード診断要求処理 */
						m_rc_ReCardDiaRes( (CMT_TSKIF_CPRIRCV_RECRDDIARES*) buff_adr );
						break;
					/* FTPデータ送信用PORT番号設定要求 */
					case CMD_CPRID_FTPPORTSETRES:
						/* FTPデータ送信用PORT番号設定処理 */
						m_rc_FtpPortSetRes( (CMT_TSKIF_CPRISND_PORTSETRES*) buff_adr );
						break;
					/* RE状態報告応答 */
					case CMD_CPRID_RESTSRES:
						m_rc_ReStaRes((CMT_TSKIF_CPRIRCV_RESTSRES*)buff_adr);
						break;
					/**< 無変調キャリアON/OFF指定応答		*/
					case CMD_CPRID_CARONOFFRES: //C9 adda
						m_rc_UnCarOptOnOffDes_3g_Res((CMT_TSKIF_CPRISND_CARONOFFRES_S3G*) buff_adr,0);
						break;

					default:
						/* 想定外縺E要求を受信した場合縺Eログ出力すめE*/
						cm_Assert(	CMD_ASL_USELOW,
									cpri_sig_p->signal_kind,
									"m_rc_ReCntMain CPRI_Sig_ABNORMAL");
						break ;

				}

			}
			/* タスク状態が運用中以外にCPRI信号を受信した場吁E*/
			else
			{
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0,
							"m_rc_ReCntMain CMD_REC_FORWARD_NOTICE");
			}
			/* RE制御個別ロギング処逅・*/
			m_rc_loging( tskid, event_no, buff_adr );

			break;

		case CMD_TSKIF_CARDSTANTC:
			/* タスク状態が運用状態の場合のみ処理を実施	*/
			if(rcw_tskstn == RCD_TSK_STN_NORMAL)
			{
				m_rc_TraStaChgNtc((T_API_SVP_CARDSTACHG_NTC *)buff_adr);
			}
			else
			{
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0,
							"m_rc_ReCntMain CMD_TSKIF_CARDSTANTC");
			}
			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break;

		/* subcribe get version response */
		case D_API_MSGID_FILE_FIRM_VERSION_RSP :
			if(rcw_tskstn == RCD_TSK_STN_NORMAL)
			{
				/* the processing after receiving subscribe get version response */
				m_rc_SubGetVerRsp(buff_adr);
			}
			else
			{
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0,
							"m_rc_ReCntMain D_SYS_MSGID_SUBGETVER_RSP");
			}
			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break;

		case CMD_TSKIF_ALLTSKININTC :
			if(rcw_tskstn == RCD_TSK_STN_NORMAL)
			{
				rcw_first_prc_act_flg = CMD_ON;
				/* REを起動待ちアイドル状態に設定	*/
				for( cnt = 1 ; cnt < D_RRH_CPRINO_NUM ; cnt++ )
				{
					f_cmn_com_layer3_set_each(cnt,CMD_SYS_3G,E_RRH_LAYER3_RE_IDLE);		/* REのLayer3 ①起動待ちアイドル状態	*/
					f_cmn_com_layer3_set_each(cnt,CMD_SYS_S3G,E_RRH_LAYER3_RE_IDLE);	/* REのLayer3 ①起動待ちアイドル状態	*/
				}
				m_rc_CpriStartup();		/* 0x4101		REC CPRI Link Preparation Request */

			}
			else
			{
				cm_Assert(	CMD_ASL_USELOW,
							rcw_tskstn,
							"m_rc_ReCntMain CMD_TSKIF_ALLTSKININTC");
			}
			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break;

		/* the startup event is timeout notification */
		case CMD_TSKIF_TIMOUTNTC :
			if(rcw_tskstn == RCD_TSK_STN_NORMAL)
			{
				m_rc_TimeOutNtc( tskid );
			}
			else
			{
				cm_Assert(	CMD_ASL_USELOW,
							rcw_tskstn,
							"m_rc_ReCntMain CMD_TSKIF_TIMOUTNTC");
			}
			break;

		/*■CPRI接続通知受信時			*/
		case CMD_TSKIF_CPRI_CONNECT_NTC:
            p_cpriConnNtc = (CMT_TSKIF_CPRICONN_NTC*) buff_adr;

            /* 対RE リンク確立／対REC リンク確立 判定	*/
            if( (p_cpriConnNtc->link_num < D_RRH_CPRINO_RE_MIN) ||
                (p_cpriConnNtc->link_num > D_RRH_CPRINO_RE_MAX) )
            {
                /* CPRIリンク番号が１より小さい or １６より大きい	*/
                /* （REからの通知ではない）							*/
                break;
            }
            /* フェールセーフ	*/
            if( (p_cpriConnNtc->system_type != CMD_SYS_3G) &&
                (p_cpriConnNtc->system_type != CMD_SYS_S3G) )
            {
                /* system_typeが3G/S3Gでない	*/
                break;
            }
#ifdef FHM_RCT_DEBUG_FOR_IT1
		    printf( "[%d]%s L3OK[cpri:%d][sys:%d]\n",__LINE__,__FUNCTION__,p_cpriConnNtc->link_num,p_cpriConnNtc->system_type);
#endif
			/********************************************************************/
			/* RE起動待ち合わせ(ReSync完了)後は増設として扱い、					*/
			/* REC起動完了まで増設REの起動シーケンス開始を待つ					*/
			/********************************************************************/
			if(re_StartUp_CPRI_Conn_flg == D_RRH_OFF)
			{
				/* 装置立ち上げ時	*/
				if(rcw_tskstn == RCD_TSK_STN_NORMAL)
				{
					cm_Assert(CMD_ASL_USELOW, p_cpriConnNtc->link_num, "CMD_TSKIF_CPRI_CONNECT_NTC FHM Startup");

					/* 対象配下REのLayer3状態を「起動中状態」に設定 */
					f_cmn_com_layer3_set_each( p_cpriConnNtc->link_num, p_cpriConnNtc->system_type, E_RRH_LAYER3_RE_START );

					/*対RE CPRIリンク確立 の通知	*/
					m_rc_CpriConnectNTC(buff_adr);	/* REのrecに(0x4005)RE Startup Request送信		*/
				}
				else
				{
					cm_Assert(CMD_ASL_USELOW, rcw_tskstn, "CMD_TSKIF_CPRI_CONNECT_NTC");
				}
			}
			else
			{
				/* CPRI接続通知をキューイング */
				m_rc_ReConn_buffchk(buff_adr);
			}

			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break;

		/* ■RE立ち上げ応答時 */
		case CMD_TSKIF_RE_STARTUP_RSP:
#ifdef FHM_RCT_DEBUG_FOR_IT1
    printf( "%s %s %d CMD_TSKIF_RE_STARTUP_RSP\n", __FILE__, __FUNCTION__, __LINE__);
#endif
            p_reStartupRsp = (CMT_TSKIF_RESTR_RSP*) buff_adr;

            /* フェールセーフ	*/
            if( (p_reStartupRsp->link_num < D_RRH_CPRINO_RE_MIN) ||
                (p_reStartupRsp->link_num > D_RRH_CPRINO_RE_MAX) )
            {
                /* REのlink番号が1-16でない	*/
                break;
            }

            if( (p_reStartupRsp->system_type != CMD_SYS_3G) &&
                (p_reStartupRsp->system_type != CMD_SYS_S3G) )
            {
                /* system_typeが3G/S3Gでない	*/
                break;
            }

            if(rec_Startup_First_flg[p_reStartupRsp->system_type] == 0)
            {
                /* 装置立ち上げ時	*/
                if(rcw_tskstn == RCD_TSK_STN_NORMAL)
                {
                    ret=m_rc_reStartupRSP(buff_adr);		/* 立ち上がったRE応答受信		*/
                    if(ret == D_RRH_OK)
                    {
                        /* 立ち上げ対象のREすべて応答あり		*/
                        delayPrmChkEn = D_RRH_ON;
                        m_rc_infoAllRE(delayPrmChkEn);		/* REの情報をまとめる		*/
                    }
                }
                else
                {
                    cm_Assert(	CMD_ASL_USELOW,
                                rcw_tskstn,
                                "m_rc_ReCntMain CMD_TSKIF_RE_STARTUP_RSP");
                }
            }
            else
            {
                /* 立ち上げ時以外	*/
                m_rc_ReStr_Rsp(buff_adr);

                /* LED設定処理 */
                m_cm_WriteLEDReg();
            }
			/* check 該当配下REリセット状態 */
			if( CMD_ON == gw_rcw_reRstSta[((CMT_TSKIF_RESTR_RSP *)buff_adr)->system_type][((CMT_TSKIF_RESTR_RSP *)buff_adr)->link_num - CMD_NUM1] )
			{
				/* リセット中の場合 */
				m_rc_UnderReResetRep(((CMT_TSKIF_RESTR_RSP *)buff_adr)->link_num,((CMT_TSKIF_RESTR_RSP *)buff_adr)->system_type);

    			/* RE自律リセット抑止解除要求送信 */
				m_rc_ReSelfReset_PreventOff_Req_Send(((CMT_TSKIF_RESTR_RSP *)buff_adr)->link_num, CMD_OFF);
			}

		    /* 停波設定/解除 */
		    m_rc_SetWave();

			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break;

		/*■CPRI切断通知受信時 			*/
		case CMD_TSKIF_CPRI_DISCONNECT_NTC:

			if(rcw_tskstn == RCD_TSK_STN_NORMAL)
			{
				/*  切断要求されたREがまだバッファにある場合はバッファから削除をして終了 */
				m_rc_ReDis_buffchk(buff_adr);
				/*  CPRIリンク断発生時の自律TRX解放 */
				m_rc_CPRIdis_TrxRel(buff_adr);
			}
			else
			{
				cm_Assert(	CMD_ASL_USELOW,
							rcw_tskstn,
							"m_rc_ReCntMain CMD_TSKIF_CPRI_DISCONNECT_NTC");
			}

			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break;

			/*■RE切断応答時 			*/
		case CMD_TSKIF_RE_DISCONNECT_RSP:

			m_rc_ReDis_Rsp(buff_adr);

			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break;

		case CMD_TSKIF_REC_CPRI_PRE_RSP :
#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf("[%d]%s CMD_TSKIF_REC_CPRI_PRE_RSP recv\n",__LINE__,__FUNCTION__);
#endif

			cm_Assert(CMD_ASL_USELOW, 0, "CMD_TSKIF_REC_CPRI_PRE_RSP");

			if(rcw_tskstn == RCD_TSK_STN_NORMAL)
			{
				m_rc_getStartUpRE();			/* 立ち上げ対象RE特定						*/
			}
			else
			{
				cm_Assert(	CMD_ASL_USELOW,
							rcw_tskstn,
							"m_rc_ReCntMain CMD_TSKIF_REC_CPRI_PRE_RSP");
			}

			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break;

			/* RE強制停止応答 */
		case CMD_TSKIF_RE_FORCESTOP_NTC :
			m_rc_ReForcestop_Ntc(buff_adr);

			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

			break;

		/* RE use status set request */
		case D_API_MSGID_CPRI_SET_REUSE_IND:
		    m_rc_ReUseSet((T_API_CPRILINK_SET_REUSE_IND*)buff_adr);
		    break;

		/* CPRI restart indication */
		case CMD_TSKIF_CPRIRESTARTIND:

			cm_Assert(CMD_ASL_USELOW, 0, "CMD_TSKIF_CPRIRESTARTIND");

		    m_rc_CpriRestartNtc();

			/* RE制御個別ロギング処理 */
			m_rc_loging( tskid, event_no, buff_adr );

		    break;

		/* 遅延補正応答(Step10) */
		// case D_API_MSGID_CPRI_DLY_ADJ_STEP10_RSP:
		// 	m_rc_DelayAdjStep10_Rsp(buff_adr);
		// 	break;

		/* ■想定蛛Eでの起動要因以外縺E場合■						*/
		default :
			/* 想定外の要求を受信した場合はログ出力する */
			cm_Assert(	CMD_ASL_USELOW,
						(UINT)((rcw_tskstn<<16)+event_no),
						"m_rc_ReCntMain REQ_Sig_ABNORMAL");
			break ;
	} /* 入力パラメータ(起動要因)での分岐END */

	return;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief     RE card control LOG collection
 *  @note     RE card control LOG collection
 *  @param    a_tskid [in] task ID
 *  @param    a_event_no [in] event number
 *  @param    a_bufaddr_p [out] event number
 *  @return NONE
 *  @retval -
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2015/04/25 FFCS)Zhangxq TDD
 *  @date     2015/07/28 TDI)satou PORT状態変化通知のログ出力処理を削除
 */
/********************************************************************************************************************/
VOID m_rc_loging( UINT a_tskid, UINT a_event_no, VOID *a_bufaddr_p )
{
	T_RRH_SYSTEM_TIME	a_rtc_date ;				/* RTC時刻情報			*/
	UINT				a_logcnt ;					/* 書込先ログカウンタ	*/
	UINT				a_len;
//	CMT_TSKIF_CPRIRCV_SIGNALGET *cpri_sig_p;		/* CPRI信号種別			*/

	/* ==================================================================== */
	/* ■自タスクのタスク状態確認											*/
	/* ==================================================================== */
	if( rcw_tskstn != RCD_TSK_STN_NORMAL ){
		return;
	}

	/* ==================================================================== */
	/* ■ログカウンタ値取得													*/
	/* ==================================================================== */
	a_logcnt = rcw_logcnt ;

	/* ==================================================================== */
	/* ■ログ領域クリア														*/
	/* ==================================================================== */
	cm_MemClr( &rcw_log_tbl[a_logcnt], sizeof(RCT_LOG_T) );

	/* ==================================================================== */
	/* ■ログ領域ヘッダ部書込処理											*/
	/* ==================================================================== */
	/* 起動元タスクID設定 */
	rcw_log_tbl[a_logcnt].loghead.taskId = a_tskid;
	/* 起動要因番号 */
	rcw_log_tbl[a_logcnt].loghead.msgId = a_event_no;

	/* 起動要因がCPRI信号の場合はイベント番号に信号種別を設定する */
	if( (a_event_no == CMD_TSKIF_CPRIRCVNTC) || (a_event_no == CMD_TSKIF_REC_FWDNTC) )
	{
//		/* CPRI信号種別設定 */
//		cpri_sig_p = (CMT_TSKIF_CPRIRCV_SIGNALGET *)a_bufaddr_p;
//		/* Filter the messages for cycling report */
//		switch (cpri_sig_p->signal_kind & CMD_SYS_MSK_CPRID)
//		{
//			case CMD_CPRID_RESTSREQ:
//			case CMD_CPRID_CARLSTSREQ:
				return;
//		}
//		rcw_log_tbl[a_logcnt].loghead.sig_kind = cpri_sig_p->signal_kind;
	}
	/* 時刻情報設定 */
	cm_RtcGet( &a_rtc_date );
	rcw_log_tbl[a_logcnt].loghead.month = a_rtc_date.month;
	rcw_log_tbl[a_logcnt].loghead.day   = a_rtc_date.day;
	rcw_log_tbl[a_logcnt].loghead.hour  = a_rtc_date.hour;
	rcw_log_tbl[a_logcnt].loghead.min   = a_rtc_date.min;
	rcw_log_tbl[a_logcnt].loghead.sec   = a_rtc_date.sec;
	rcw_log_tbl[a_logcnt].loghead.msec  = a_rtc_date.msec;
	rcw_log_tbl[a_logcnt].loghead.setFlg = CMD_ON;

	/* ==================================================================== */
	/* ■ログ領域ボディ部書込処理											*/
	/* ==================================================================== */
	/* 起動要因番号から処理の振分け */
	switch( a_event_no )
	{

		case CMD_TSKIF_TRXRLS_NTC:				/* TRX解放通知受信時 */
//		case CMD_TSKIF_CPRIRCVNTC :			/* CPRI信号受信通知受信時 */
		case CMD_TSKIF_CARDSTANTC:
//		case CMD_TSKIF_REC_FWDNTC:
		case CMD_TSKIF_CPRI_CONNECT_NTC:
		case CMD_TSKIF_RE_STARTUP_RSP:
		case CMD_TSKIF_CPRI_DISCONNECT_NTC:
		case CMD_TSKIF_RE_DISCONNECT_RSP:
		case CMD_TSKIF_REC_CPRI_PRE_RSP :
		case CMD_TSKIF_RE_FORCESTOP_NTC :
		case CMD_TSKIF_CPRIRESTARTIND:
			a_len = ((T_API_COMMON_HEADER*)a_bufaddr_p)->uiLength - sizeof(T_API_COMMON_HEADER);
			if(a_len > RCD_LOG_SV1_SIZE) a_len = RCD_LOG_SV1_SIZE;
			cm_MemCpy(	rcw_log_tbl[a_logcnt].sv_tbl1,
						(UCHAR*)a_bufaddr_p + sizeof(T_API_COMMON_HEADER),
						a_len);
		break;

		default:
			return;
		break;
	}
	/* ==================================================================== */
	/* ■ロギングカウンタカウントアップ										*/
	/* ==================================================================== */
	rcw_logcnt++;
	if (rcw_logcnt >= RCD_LOG_MAX)
	{
		rcw_logcnt = CMD_NUM0;
	}
}

/* @} */

void m_rc_logPrint(FILE *outf)
{
	UINT index;
	UINT printIdx = 1;
	UINT bodyIdx;
	UINT loop;
	USHORT carrier_no;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

	fprintf(outf,"L3 RCT running history----------------------------------------\n");

	for(index = 0; index < RCD_LOG_MAX; index++)
	{
		if(rcw_log_tbl[index].loghead.setFlg == 0)
			continue;

		fprintf(outf,"No.%03d [%02x-%02x %02x:%02x:%02x.%03x] %08x;%04x;%04x\n",printIdx,
			rcw_log_tbl[index].loghead.month,
			rcw_log_tbl[index].loghead.day,
			rcw_log_tbl[index].loghead.hour,
			rcw_log_tbl[index].loghead.min,
			rcw_log_tbl[index].loghead.sec,
			rcw_log_tbl[index].loghead.msec,
			rcw_log_tbl[index].loghead.msgId,
			rcw_log_tbl[index].loghead.taskId,
			rcw_log_tbl[index].loghead.sig_kind);

		for(bodyIdx = 0;bodyIdx < RCD_LOG_SV1_SIZE / 4;bodyIdx++)
		{
			fprintf(outf,"%02x%02x %02x%02x",
				rcw_log_tbl[index].sv_tbl1[bodyIdx * 4],
				rcw_log_tbl[index].sv_tbl1[bodyIdx * 4 + 1],
				rcw_log_tbl[index].sv_tbl1[bodyIdx * 4 + 2],
				rcw_log_tbl[index].sv_tbl1[bodyIdx * 4 + 3]);

			if((bodyIdx % 4) == 3) fprintf(outf,"\n");
		}

		printIdx++;
	}

	fprintf(outf, "----------- The information of cmw_sys_mng_tbl -----------\n");
	fprintf(outf,"cmw_sys_mng_tbl[CMD_SYS_S3G] \n");
	fprintf(outf,".cpri_sttimer      :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].cpri_sttimer);
	fprintf(outf,".hls_timer         :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].hls_timer);
	fprintf(outf,".hls_unconfcnt     :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].hls_unconfcnt);
	fprintf(outf,".sys_timer         :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].sys_timer);
	fprintf(outf,".firm_timer        :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].firm_timer);
	fprintf(outf,".sec_chip0         :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].sec_chip0);
	fprintf(outf,".sec_chip1         :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].sec_chip1);
	fprintf(outf,".oudevt_rcvtimer   :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].oudevt_rcvtimer);
	fprintf(outf,".fldmt_cttimer)    :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].fldmt_cttimer);
	fprintf(outf,".fldmt_rltimer)    :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].fldmt_rltimer);
	fprintf(outf,".fldmt_dtcd_timer  :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].fldmt_dtcd_timer);
	fprintf(outf,".fldmt_dtfl_timer  :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].fldmt_dtfl_timer);
	fprintf(outf,".svmt_cttimer      :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].svmt_cttimer);
	fprintf(outf,".svmt_rltimer      :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].svmt_rltimer);
	fprintf(outf,".svmt_rcvtimer     :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].svmt_rcvtimer);
	fprintf(outf,".tx_div_value_3G   :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].tx_div_value_3G);
	fprintf(outf,".pri_sys_info      :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].pri_sys_info);
	fprintf(outf,".s3g_bandwidth     :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].s3g_bandwidth);
	fprintf(outf,".car_wk_flg        :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].car_wk_flg);
	fprintf(outf,".tx_sys_num        :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num);
	fprintf(outf,".car1_wk_frq       :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].car1_wk_frq);
	fprintf(outf,".car2_wk_frq       :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].car2_wk_frq);
	fprintf(outf,".car3_wk_frq       :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].car3_wk_frq);
	fprintf(outf,".car4_wk_frq       :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].car4_wk_frq);
	fprintf(outf,".mtExt_data_3g_flg :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].mtExt_data_3g_flg);
	fprintf(outf,".syspara_wait_timer:%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].syspara_wait_timer);
	fprintf(outf,".firm_sndtimer     :%04X \n",cmw_sys_mng_tbl[CMD_SYS_S3G].firm_sndtimer);

	fprintf(outf, "----------- The information of cmw_tra_sv_tbl -----------\n");
	fprintf(outf, "cmw_tra_sv_tbl.en_flg = %4x\n", cmw_tra_sv_tbl.en_flg);
	for(loop = 0; loop < 5; loop++)
	{
		fprintf(outf, "cmw_tra_sv_tbl.sv_inf[%d] = %04x\n", loop, cmw_tra_sv_tbl.sv_inf[loop]);
	}
	fprintf(outf, "cmw_tra_sv_tbl.alm_num = %d\n", cmw_tra_sv_tbl.alm_num);

	for(loop = 0; loop < cmw_tra_sv_tbl.alm_num; loop++)
	{
		fprintf(outf, "cmw_tra_sv_tbl.alm_inf[%d] = %04x\n", loop, cmw_tra_sv_tbl.alm_inf[loop]);
	}

	fprintf(outf, "cmw_tra_sv_tbl.card_cntitem = %04x\n", cmw_tra_sv_tbl.card_cntitem);
	fprintf(outf, "cmw_tra_sv_tbl.card_extitem = %04x\n", cmw_tra_sv_tbl.card_extitem);
	fprintf(outf, "cmw_tra_sv_tbl.carr_onoff = %04x\n", cmw_tra_sv_tbl.carr_onoff);


	fprintf(outf, "----------- The information of cmw_cardcnt_tbl  -----------\n");
	fprintf(outf, ".cnt_kind        = 0x%04x\n", cmw_cardcnt_tbl.cnt_kind       );
	fprintf(outf, ".cnt_itm_s3g     = 0x%04x\n", cmw_cardcnt_tbl.cnt_itm_s3g    );
	fprintf(outf, ".cnt_code        = 0x%04x\n", cmw_cardcnt_tbl.cnt_code       );
	fprintf(outf, ".cnt_cmpblk      = 0x%04x\n", cmw_cardcnt_tbl.cnt_cmpblk     );
	fprintf(outf, ".blk_code        = 0x%04x\n", cmw_cardcnt_tbl.blk_code       );
	fprintf(outf, ".err_code        = 0x%04x\n", cmw_cardcnt_tbl.err_code       );
	fprintf(outf, ".cnt_extitem_s3g = 0x%04x\n", cmw_cardcnt_tbl.cnt_extitem_s3g);
	fprintf(outf, ".brInfo.exc_code = 0x%04x 0x%04x\n", cmw_cardcnt_tbl.brInfo[0].exc_code      , cmw_cardcnt_tbl.brInfo[1].exc_code      );
	fprintf(outf, ".brInfo.exc_itm  = 0x%04x 0x%04x\n", cmw_cardcnt_tbl.brInfo[0].exc_itm       , cmw_cardcnt_tbl.brInfo[1].exc_itm       );
	fprintf(outf, ".brInfo.slpInfo  = 0x%04x 0x%04x\n", cmw_cardcnt_tbl.brInfo[0].slpInfo.state , cmw_cardcnt_tbl.brInfo[1].slpInfo.state );
	fprintf(outf, ".brInfo.paInfo   = 0x%04x 0x%04x\n", cmw_cardcnt_tbl.brInfo[0].paInfo.pa_flg , cmw_cardcnt_tbl.brInfo[1].paInfo.pa_flg );

	fprintf(outf, "----------- The information of cmw_toffset_tbl  -----------\n");
	fprintf(outf, ".toffset         = 0x%04x\n", cmw_toffset_tbl[CMD_SYS_S3G].toffset   );
	fprintf(outf, ".ul_delay        = 0x%04x\n", cmw_toffset_tbl[CMD_SYS_S3G].ul_delay  );
	fprintf(outf, ".dl_delay        = 0x%04x\n", cmw_toffset_tbl[CMD_SYS_S3G].dl_delay  );

	fprintf(outf, "----------- The information of cmw_re_dev_tbl  -----------\n");
	fprintf(outf, ".mkr_nm          = 0x%04x\n", cmw_re_dev_tbl.mkr_nm         );
	fprintf(outf, ".bts_type        = 0x%04x\n", cmw_re_dev_tbl.bts_type       );
	fprintf(outf, ".re_type         = 0x%04x\n", cmw_re_dev_tbl.re_type        );
	fprintf(outf, ".dev_kind        = 0x%04x\n", cmw_re_dev_tbl.dev_kind       );
	fprintf(outf, ".dev_no          = 0x%04x\n", cmw_re_dev_tbl.dev_no         );
	fprintf(outf, ".div_type        = 0x%04x\n", cmw_re_dev_tbl.div_type       );
	fprintf(outf, ".pwr_type        = 0x%04x\n", cmw_re_dev_tbl.pwr_type       );
	fprintf(outf, ".pwr_class_unit  = 0x%04x\n", cmw_re_dev_tbl.pwr_class_unit );
	fprintf(outf, ".pwr_class0      = 0x%04x\n", cmw_re_dev_tbl.pwr_class0     );
	fprintf(outf, ".pwr_class1      = 0x%04x\n", cmw_re_dev_tbl.pwr_class1     );
	fprintf(outf, ".pwr_class2      = 0x%04x\n", cmw_re_dev_tbl.pwr_class2     );
	fprintf(outf, ".pwr_class3      = 0x%04x\n", cmw_re_dev_tbl.pwr_class3     );
	fprintf(outf, ".crp_frq         = 0x%04x\n", cmw_re_dev_tbl.crp_frq        );
	fprintf(outf, ".max_bandwidth   = 0x%04x\n", cmw_re_dev_tbl.max_bandwidth  );
	fprintf(outf, ".max_tx_ant_no   = 0x%04x\n", cmw_re_dev_tbl.max_tx_ant_no  );
	fprintf(outf, ".fii_type        = 0x%04x\n", cmw_re_dev_tbl.fii_type       );
	fprintf(outf, ".cmx_2gcarcnt    = 0x%04x\n", cmw_re_dev_tbl.cmx_2gcarcnt   );
	fprintf(outf, ".cmx_800mcarcnt  = 0x%04x\n", cmw_re_dev_tbl.cmx_800mcarcnt );
	fprintf(outf, ".cmx_17gcarcnt   = 0x%04x\n", cmw_re_dev_tbl.cmx_17gcarcnt  );
	fprintf(outf, ".cmx_trxinf_typ  = 0x%04x\n", cmw_re_dev_tbl.cmx_trxinf_typ );
	fprintf(outf, ".cmx_15gcarcnt   = 0x%04x\n", cmw_re_dev_tbl.cmx_15gcarcnt  );
	fprintf(outf, ".cmx_35gcarcnt   = 0x%04x\n", cmw_re_dev_tbl.cmx_35gcarcnt  );
	fprintf(outf, ".mca_filt        = 0x%04x\n", cmw_re_dev_tbl.mca_filt       );

	fprintf(outf, "----------- The information of cmw_file_inf_tbl  -----------\n");
	fprintf(outf, ".en_flg          = 0x%04x\n", cmw_file_inf_tbl[CMD_SYS_S3G].en_flg         );
	fprintf(outf, ".target_data     = 0x%04x\n", cmw_file_inf_tbl[CMD_SYS_S3G].target_data    );
	fprintf(outf, ".division_num    = 0x%04x\n", cmw_file_inf_tbl[CMD_SYS_S3G].division_num   );
	fprintf(outf, ".ng_fct          = 0x%04x\n", cmw_file_inf_tbl[CMD_SYS_S3G].ng_fct         );


	fprintf(outf, "----------- The information of rcw_carsts_tbl  -----------\n");
	fprintf(outf, ".car_num         = 0x%04x\n", rcw_carsts_tbl.res.car_num);
	for(carrier_no = 0; carrier_no<CMD_MAX_CARNUM_S3G; carrier_no++)
	{
		if(rcw_carsts_tbl.car_every[carrier_no].com.car_sta != CMD_OFF)
		{
			fprintf(outf, ".car_no          = 0x%04x\n", rcw_carsts_tbl.car_every[carrier_no].com.car_no);
			fprintf(outf, ".car_sta         = 0x%04x\n", rcw_carsts_tbl.car_every[carrier_no].com.car_sta);
			fprintf(outf, ".car_sta_ul      = 0x%04x\n", rcw_carsts_tbl.car_every[carrier_no].com.car_sta_ul);
			fprintf(outf, ".car_sta_dl      = 0x%04x\n", rcw_carsts_tbl.car_every[carrier_no].com.car_sta_dl);
			fprintf(outf, ".br_num          = 0x%04x\n", rcw_carsts_tbl.car_every[carrier_no].com.br_num);
			for(loop = 0; loop < rcw_carsts_tbl.car_every[carrier_no].com.br_num; loop++)
			{
				fprintf(outf, ".br_no           = 0x%04x\n", rcw_carsts_tbl.car_every[carrier_no].br_every[loop].br_no);
				fprintf(outf, ".br_car_sta      = 0x%04x\n", rcw_carsts_tbl.car_every[carrier_no].br_every[loop].br_car_sta);
				fprintf(outf, ".br_car_sta_ul   = 0x%04x\n", rcw_carsts_tbl.car_every[carrier_no].br_every[loop].br_car_sta_ul);
				fprintf(outf, ".br_car_sta_dl   = 0x%04x\n", rcw_carsts_tbl.car_every[carrier_no].br_every[loop].br_car_sta_dl);
			}
		}
	}

	fprintf(outf, ".alm_num         = 0x%04x\n", rcw_carsts_tbl.alm.alm_num);
	for(loop = 0; loop < rcw_carsts_tbl.alm.alm_num; loop++)
	{
		fprintf(outf, ".rcd_almcode     = 0x%04x\n", rcw_carsts_tbl.alm_every[loop].rcd_almcode);
	}
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif

	return;
}

/* @} */


/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_r.c
 * @brief  ルート
 * @date   2015/07/29 FPT)Hieu create
 * @date   2015/09/30 TDIPS)sasaki udpate
 * @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-48)
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "rrhApi_L2.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  ルート
 * @note   イベント受信時の処理を行う
 *         - イベント変換
 *         - 状態番号取得
 *         - マトリクス関数起動
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01777
 * @date   2015/07/29 FPT)Hieu create
 * @date   2015/09/30 TDIPS)sasaki udpate 以下のメッセージを受け付けるよう修正
 *											・ファイルデータ送信転送指示
 *											・運用中REファイル更新開始通知
 *											・運用中REファイル更新完了通知
 * @date   2015/09/29 FPT)Quynh fix bug IT1(FLD-MTデータファイル転送受信)
 * @date   2015/10/22 FPT)Quynh M-RRU-ZSYS-01777 Fix bug IT2 (BUG_082_IT2_20-06-04_20-06-03)
 * @date   2016/03/03 tdips)enoki RE自律リセット抑止解除要求 追加
 * @date   2021/02/19 M&C)Tri.hn Update based on No.67 (sequence 3-30) - Add case CMD_CPRID_RECRDDIAREQ and CMD_CPRID_RECRDDIARES
 * @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-48) - add case CMD_CPRID_FTPPORTSETREQ and CMD_CPRID_FTPPORTSETRES
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_r(                                            /* なし:R */
   VOID* bufp                                         /* バッファポインタ:I */
) {
    
    UINT                   msg_id;                          /* メッセージID */
	USHORT                 cpk = 0x0000;                    /* CPRI信号種別 */
    VOID*                  datp;
//    USHORT                 cpr_no;                              /* CPRI番号 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /********************/
    /* メッセージID取得 */
    /********************/
    msg_id = ((CMT_TSKIF_MSGID_GET*)bufp)->head.msgid;

	switch(msg_id)
	{
		/* CPRI信号受信通知 (L3->rec)*/
	    case CMD_TSKIF_RE_FWDNTC:
		/* CPRI信号受信通知 (L2->rec)*/
	    case D_API_MSGID_L2_CPRIMSG_RCV_NTC:
			/* CPRI信号種別取得 */
			datp = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    		cpk  = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind;
			break;
		/* RE Disconnecion Request */
		case CMD_TSKIF_RE_DISCONNECT_REQ:
		/* RE Force Reset Indication(l3_rct->re_rec) */
		case CMD_TSKIF_RE_FORCERESET_IND:
		/* RE Force Stop Indication(l3_rct->re_rec) */
		case CMD_TSKIF_RE_FORCESTOP_IND:
		case CMD_TSKIF_FILEDATSND_TRANS_IND:
		case CMD_TSKIF_REFILE_UPDATE_START_NTC:
		case CMD_TSKIF_REFILE_UPDATE_COMP_NTC:
		/* RE自律リセット抑止解除要求 */
		case CMD_TSKIF_RE_SLFRST_PRVTOFF_REQ:
			datp = &((T_REC_MSGRCV_TBL*)bufp)->datp;
			break;
		default:
			return;
	}

	/* メッセージIDで分岐 */
    switch (msg_id) 
	{
		/* RE Forwarding Notice (L3->rec)*/
	    case CMD_TSKIF_RE_FWDNTC:
	        /* CPRI信号種別で分岐 */
	        switch (cpk & CMD_SYS_MSK_CPRID) 
			{
	            /* ■RE時刻設定要求■ */
	            case CMD_CPRID_RETIMSETREQ:
	                /* forward RE時刻設定要求 */
	                f_trp_rec_FcXX_m_use_ReTimSet_fw( datp );
	                break;
		        /* ■ SV-MT/eNB-MT接続強制解放要求■					*/
		        case CMD_CPRID_SVMTFORRELREQ:
					/* foward SV-MT/eNB-MT接続強制解放要求 */
					f_trp_rec_FcXX_m_use_SvMtForceRelReq_fw( datp );
		            break;
				/* ■外部装置データ送信要求■					*/
				case  CMD_CPRID_EXTDATSNDREQ:
					/* foward 外部装置データ送信要求 */
					f_trp_rec_FcXX_m_use_ExtDatSndReq_fw(datp);
					break;
				/* ■ FLD-MT接続応答■					*/
		        case CMD_CPRID_FLDMTCNCRES:
					/* foward FLD-MT接続応答 */
					f_trp_rec_FcXX_m_use_refldcntrsp_fw( datp );
		            break;
				/* ■ FLD-MT接続強制解放要求■					*/
		        case CMD_CPRID_FLMTCMPRELREQ:
					/* foward FLD-MT接続強制解放要求 */
					f_trp_rec_FcXX_m_use_refldfrrelcntreq_fw( datp );
		            break;
				/* ■ SV-MT接続応答■					*/
		        case CMD_CPRID_SVMTCNTRES:
					/* foward SV-MT接続応答 */
					f_trp_rec_FcXX_m_use_resvmtcntrsp_fw( datp );
		            break;
				/* ■ SV-MTデータ送信要求■					*/
		        case CMD_CPRID_SVMTDATSNDREQ:
					/* foward SV-MTデータ送信要求 */
					f_trp_rec_FcXX_m_use_resvmtdatsndreq_fw( datp );
		            break;				
				/* ■ SV-MT/eNB-MTデータ受信応答■					*/
		        case CMD_CPRID_SVMTDATRCVRES:
					/* foward SV-MT/eNB-MTデータ受信応答 */
					f_trp_rec_FcXX_m_use_SvMtDatRcvRes_fw( datp );
		            break;
				/* ■ SV-MT/eNB-MT接続解放応答■					*/
		        case CMD_CPRID_SVMTCNTRELRES:
					/* foward SV-MT/eNB-MT接続解放応答 */
					f_trp_rec_FcXX_m_use_SvMtCntRelRes_fw( datp );
		            break;
				/* 外部装置データ受信応答 */
				case CMD_CPRID_EXTDATRCVRES:
					/* Forward 外部装置データ受信応答 */
					f_trp_rec_FcXX_m_use_ExtRcvtSndRes_fw( datp );
					break;
				/* FLD-MT接続解放応答 */
				case CMD_CPRID_FLMTCNCRELRES:
					/* Forward FLD-MT接続解放応答 */
					f_trp_rec_FcXX_m_use_FldMtRelRes_fw( datp );
					break;
				/* FLD-MTデータコマンド転送受信応答 */
				case CMD_CPRID_FLDCOMRCVRES:
					/* Forward FLD-MTデータコマンド転送受信応答 */
					f_trp_rec_FcXX_m_use_FldCmdRcvRes_fw( datp );
					break;
				/* FLD-MTデータファイル転送送信要求 */
				case CMD_CPRID_FLDFILSNDREQ:
					/* Forward FLD-MTデータファイル転送送信要求 */
					f_trp_rec_FcXX_m_use_FldFilSndReq_fw( datp );
					break;
				/* ファームウェアファイル報告要求 */
				case CMD_CPRID_FIRMFILEREQ:
					/* Forward ファームウェアファイル報告要求 to RE */
					f_trp_rec_FcXX_m_use_FirmFileReq_fw( datp );
					break;
				/* ファイル情報報告通知 */
				case CMD_CPRID_FILEINFREQ:
					/* Forward ファイル情報報告通知 to RE */
					f_trp_rec_FcXX_m_use_FileInfReq_fw( datp );
					break;
				/* ファイルデータ送信 */
				case CMD_CPRID_FILEDATSND:
					/* Forward ファイルデータ送信 to RE */
					f_trp_rec_FcXX_m_use_FileDatSnd_fw( datp );
					break;
				/* ファイルデータ送信完了報告通知 */
				case CMD_CPRID_FILSNDFINNTC:
					/* Forward ファイルデータ送信完了報告通知 to RE */
					f_trp_rec_FcXX_m_use_FilSndFinNtc_fw( datp );
					break;
				/* 運用中ファイル情報報告通知 */
				case CMD_CPRID_UFILEINFREQ:
					/* Forward 運用中ファイル情報報告通知 to RE */
					f_trp_rec_FcXX_m_use_UFileInfReq_fw( datp );
					break;
				/* 運用中ファイルデータ送信*/
				case CMD_CPRID_UFILEDATSND:
					/* Forward 運用中ファイルデータ送信 to RE */
					f_trp_rec_FcXX_m_use_UFileDatSnd_fw( datp );
					break;
				/* 運用中ファイルデータ送信完了報告通知 */
				case CMD_CPRID_UFILSNDFINNTC:
					/* Forward 運用中ファイルデータ送信完了報告通知 to RE */
					f_trp_rec_FcXX_m_use_UFilSndFinNtc_fw( datp );
					break;
				/* 運用中ファームウェアファイル報告要求 */
				case CMD_CPRID_UFIRMFILEREQ:
					/* Forward 運用中ファームウェアファイル報告要求 to RE */
					f_trp_rec_FcXX_m_use_UFirmFileReq_fw( datp );
					break;
				/* REカード情報報告要求 */
				case CMD_CPRID_CARDINFREQ:
					/* Forward REカード情報報告要求 to RE */
					f_trp_rec_FcXX_m_use_CardInfoReq_fw( datp );
					break;
				/* FLD-MTデータコマンド転送送信要求 */
				case CMD_CPRID_FLDCOMSNDREQ:
					/* Forward FLD-MTデータコマンド転送送信要求 to RE */
					f_trp_rec_FcXX_m_use_fldcomsndreq_fw( datp );
					break;
				/* FLD-MTデータファイル転送受信応答 */
				case CMD_CPRID_FLDFILRCVRES:
					/* Forward FLD-MTデータファイル転送受信応答 to RE */
					f_trp_rec_FcXX_m_use_fldfilrcvres_fw( datp );
					break;
				/* REリセット要求 */
				case CMD_CPRID_RERSTREQ:
					/* Forward REリセット要求 to RE */
					f_trp_rec_FcXX_m_use_rerstreq_fw( datp );
					break;
				/* REバージョン報告要求 */
	        	case CMD_CPRID_REVERREQ:
	        	    f_trp_rec_FcXX_m_use_reverreq_fw( datp );
	        	    break;
	        	/* MTアドレス設定要求(REC主導) */
	        	case CMD_CPRID_MTADDSETREQ:
					f_trp_rec_FcXX_m_mtsetf_req( datp );
					break;
                /* 「MTアドレス設定確認タイマ」タイムアウト(REC)*/
                case CMD_CPRID_MTADDSETTOREQ:
					f_trp_rec_FcXX_m_mtsetf_rec_to( datp );
					break;
                /* 「RE障害ログ取得送信要求」*/
                case CMD_CPRID_REOBSLOGREQ:
					f_trp_rec_FcXX_m_use_TroublelogGetReq_fw( datp );
					break;
                /* 「RE障害ログ取得中止送信要求」*/
                case CMD_CPRID_REOBLOGSTPREQ:
					f_trp_rec_FcXX_m_use_TroublelogGetStopReq_fw( datp );
					break;
				/* ファイルデータ送信完了報告応答 */
				case CMD_CPRID_FILSNDFINRES:
					/* Forward ファイルデータ送信完了報告応答 to L2 */
					f_trp_rec_FcXX_m_use_TroubleLogFilSndFinRes_fw( datp );
					break;
				/* REカード制御要求 */
                case CMD_CPRID_RECARDCNTREQ:
                    f_trp_rec_FcXX_m_use_ReCardReq_fw( datp );
                    break;
				/* RE card diagnostic request */
				case CMD_CPRID_RECRDDIAREQ:
					f_trp_rec_FcXX_m_use_recarddiareq_fw( datp );
					break;
				/* RE card diagnostic response */
				case CMD_CPRID_RECRDDIARES:
					f_trp_rec_FcXX_m_use_recarddiares_fw( datp );
					break;				/* PORT number setting request for FTP data transmission */
				case CMD_CPRID_FTPPORTSETREQ:
					f_trp_rec_FcXX_m_use_ftpportsetreq_fw( datp );
					break;
				// /* PORT number setting response for FTP data transmission */
				// case CMD_CPRID_FTPPORTSETRES:
				// 	f_trp_rec_FcXX_m_use_ftpportsetres_fw( datp );
				// 	break;
				/* REスロット情報報告要求 */
				case CMD_CPRID_SLOTINFREQ:
					/* Forward REスロット情報報告要求 to RE */
					f_trp_rec_Fcom_t_l2_msg_fw( datp, sizeof(CMT_TSKIF_CPRIRCV_SLOTINFREQ) );
					break;
				default:
					break;
			}
			break;
		/* CPRI信号受信通知 (L2->rec)*/
	    case D_API_MSGID_L2_CPRIMSG_RCV_NTC:
	        /* CPRI信号種別で分岐 */
	        switch (cpk & CMD_SYS_MSK_CPRID) 
			{
		       /* ■ SV-MT/FLD-MT接続強制解放応答■					*/
				case CMD_CPRID_SVMTFORRELRES:
					/* Foward SV-MT/FLD-MT接続強制解放応答 */
					f_trp_rec_FcXX_m_use_SvMtForceRelRes_fw( datp );
		            break;
				/* 外部装置データ送信応答 */
				case CMD_CPRID_EXTDATSNDRES:
					/*Forward 外部装置データ送信応答 */
					f_trp_rec_FcXX_m_use_ExtDatSndRes_fw( datp );
					break;
				/* ■ FLD-MT接続要求■					*/
		        case CMD_CPRID_FLDMTCNCREQ:
					/* foward FLD-MT接続要求 */
					f_trp_rec_FcXX_m_use_refldcntreq_fw( datp );
		            break;
				/* ■ FLD-MT接続強制解放応答■					*/
		        case CMD_CPRID_FLMTCMPRELRES:
					/* foward FLD-MT接続強制解放応答 */
					f_trp_rec_FcXX_m_use_refldfrrelcntrsp_fw( datp );
		            break;
				/* ■ SV-MT接続要求■					*/
		        case CMD_CPRID_SVMTCNTREQ:
					/* foward SV-MT接続要求 */
					f_trp_rec_FcXX_m_use_resvmtcntreq_fw( datp );
		            break;
				/* ■ SV-MTデータ送信応答■					*/
		        case CMD_CPRID_SVMTDATSNDRES:
					/* foward SV-MTデータ送信応答 */
					f_trp_rec_FcXX_m_use_resvmtdatsndrsp_fw( datp );
		            break;
				/* ■ SV-MT/eNB-MTデータ受信要求■					*/
		        case CMD_CPRID_SVMTDATRCVREQ:
					/* foward SV-MT/eNB-MTデータ受信要求 */
					f_trp_rec_FcXX_m_use_SvMtDatRcvReq_fw( datp );
		            break;
				/* ■ SV-MT/eNB-MT接続解放要求■					*/
		        case CMD_CPRID_SVMTCNTRELREQ:
					/* foward SV-MT/eNB-MT接続解放要求 */
					f_trp_rec_FcXX_m_use_SvMtCntRelReq_fw( datp );
		            break;
				/* 外部装置データ受信要求 */
				case CMD_CPRID_EXTDATRCVREQ:
					/* Forward 外部装置データ受信要求 */
					f_trp_rec_FcXX_m_use_ExtDatRcvReq_fw( datp );
					break;
				/* FLD-MT接続解放要求 */
				case CMD_CPRID_FLMTCNCRELREQ:
					/* Forward FLD-MT接続解放要求 */
					f_trp_rec_FcXX_m_use_FldMtRelReq_fw( datp );
					break;
				/* FLD-MTデータコマンド転送受信要求 */
				case CMD_CPRID_FLDCOMRCVREQ:
					/* Forward FLD-MTデータコマンド転送受信要求 */
					f_trp_rec_FcXX_m_use_FldCmdRcvReq_fw( datp );
					break;
				/* FLD-MTデータファイル転送送信応答 */
				case CMD_CPRID_FLDFILSNDRES:
					/* Forward FLD-MTデータファイル転送送信応答 */
					f_trp_rec_FcXX_m_use_FldFilSndRes_fw( datp );
					break;
				/* ファームウェアファイル報告応答 */
				case CMD_CPRID_FIRMFILERES:
					/* Forward ファームウェアファイル報告要求 to L3 */
					f_trp_rec_FcXX_m_use_FirmFileRes_fw( datp );
					break;
				/* ファイル情報報告応答 */
				case CMD_CPRID_FILEINFRES:
					/* Forward ファイル情報報告応答 to L3 */
					f_trp_rec_FcXX_m_use_FileInfRes_fw( datp );
					break;
				/* ファイルデータ送信完了報告応答 */
				case CMD_CPRID_FILSNDFINRES:
					/* Forward ファイルデータ送信完了報告応答 to L3 */
					f_trp_rec_FcXX_m_use_FilSndFinRes_fw( datp );
					break;
				/* 運用中ファイル情報報告応答 */
				case CMD_CPRID_UFILEINFRES:
					/* Forward 運用中ファイル情報報告応答 to L3 */
					f_trp_rec_FcXX_m_use_UFileInfRes_fw( datp );
					break;
				/* 運用中ファイルデータ送信完了報告応答 */
				case CMD_CPRID_UFILSNDFINRES:
					/* Forward 運用中ファイルデータ送信完了報告応答 to L3 */
					f_trp_rec_FcXX_m_use_UFilSndFinRes_fw( datp );
					break;
				/* 運用中ファームウェアファイル報告応答 */
				case CMD_CPRID_UFIRMFILERES:
					/* Forward 運用中ファームウェアファイル報告応答 to L3 */
					f_trp_rec_FcXX_m_use_UFirmFileRes_fw( datp );
					break;
				/* REカード情報報告応答 */
				case CMD_CPRID_CARDINFRES:
					/* Forward REカード情報報告要求 to L3 */
					f_trp_rec_FcXX_m_use_CardInfoRsp_fw( datp );
					break;
				/* FLD-MTデータコマンド転送送信応答 */
				case CMD_CPRID_FLDCOMSNDRES:
					/* Forward FLD-MTデータコマンド転送送信応答 to L3 */
					f_trp_rec_FcXX_m_use_fldcomsndres_fw( datp );
					break;
				/* FLD-MTデータファイル転送受信要求 */
				case CMD_CPRID_FLDFILRCVREQ:
					/* Forward FLD-MTデータファイル転送受信要求 to RE */
					f_trp_rec_FcXX_m_use_fldfilrcvreq_fw( datp );
					break;
                /* REバージョン報告応答 */
				case CMD_CPRID_REVERRES:
				    f_trp_rec_FcXX_m_use_reverres_fw( datp );
				    break;
	        	/* MTアドレス設定応答 */
	        	case CMD_CPRID_MTADDSETRES:
					f_trp_rec_FcXX_m_mtsetf_res( datp );
					break;	
		        /* 「RE障害ログ取得送信要求」*/
		        case CMD_CPRID_REOBSLOGRES:
					f_trp_rec_FcXX_m_use_TroublelogGetRes_fw( datp );
					break;
		        /* 「RE障害ログ取得中止送信要求」*/
		        case CMD_CPRID_REOBLOGSTPRES:
					f_trp_rec_FcXX_m_use_TroublelogGetStopRes_fw( datp );
					break;
		        /* 「RE障害ログデータ送信」*/
		        case CMD_CPRID_FILEDATSND:
					f_trp_rec_FcXX_m_use_TroubleLogFilSndNtc_fw( datp );
					break;
		        /* 「RE障害ログデータ送信完了」*/
		        case CMD_CPRID_FILSNDFINNTC:
					f_trp_rec_FcXX_m_use_TroubleLogFilSndFinNtc_fw( datp );
					break;
				/* PORT number setting response for FTP data transmission */
				case CMD_CPRID_FTPPORTSETRES:
					printf("\n[Hiep][RE-REC][%s:%d]  func: %s \n   ",__FILE__,__LINE__,__func__);
					f_trp_rec_FcXX_m_use_ftpportsetres_fw( datp );
					break;
		        /* その他 */
		        default:
		            break;
		     }
	        break;
		
		/* RE Disconnecion Request */
		case CMD_TSKIF_RE_DISCONNECT_REQ:
			/* RE Disconnecion Request Process */
			f_trp_rec_FcXX_m_use_ReDisconnectRes( datp );
			break;
		/* RE Force Reset Indication(l3_rct->re_rec) */
		case CMD_TSKIF_RE_FORCERESET_IND:
			/* RE Force Reset Indication Process */
			f_trp_rec_FcXX_m_use_ReForceRst_Ind( datp );
			break;
		/* RE Force Stop Indication(l3_rct->re_rec) */
		case CMD_TSKIF_RE_FORCESTOP_IND:
			/* RE Force Stop Indication Process */
			f_trp_rec_FcXX_m_use_ReForceStp_Ind( datp );
			break;
		/* ファイルデータ送信転送指示 */
		case CMD_TSKIF_FILEDATSND_TRANS_IND:
			/* Forward ファイルデータ送信 to RE */
			f_trp_rec_FcXX_m_use_FileDatSndInd_fw( datp );
			break;
		/* 運用中REファイル更新開始通知(l3_dlm->re_rec) */
		case CMD_TSKIF_REFILE_UPDATE_START_NTC:
			f_trp_rec_Fc02_m_use_refilup_start_req( datp );
			break;
		/* 運用中REファイル更新完了通知(l3_dlm->re_rec) */
		case CMD_TSKIF_REFILE_UPDATE_COMP_NTC:
			/* RE Force Stop Indication Process */
			f_trp_rec_Fc02_m_res_usrefil_end( datp );
			break;
		/* RE自律リセット抑止解除要求 */
		case CMD_TSKIF_RE_SLFRST_PRVTOFF_REQ:
			f_trp_rec_FcXX_m_ReSelfReset_PreventOff_Req( datp );
			break;
	    default:
	    	break;
	}
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

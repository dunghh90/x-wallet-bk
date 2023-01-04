/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	m_rc_ReVerRep.c
 *	@brief	RE version info report process
 *	@date	2008/07/25 FFCS)linlj create
 *	@date	2009/03/03 FFCS)Linlj (M-S3G-eNBPF-01470) ☆CPRI-L3信号受信におけるNegative判定処理
 *	@date	2009/06/09	FFCS)Wuh modify for M-S3G-eNBPF-02047
 *						RE version response incorrect
 *	@date	2009/07/08	FFCS)Tangj modify for CR-00054-005
 *						3G FW Minor version is fixed 0
 *	@date	2009/08/04	FJT)Taniguchi modify for M-S3G-eNBPF-02332
 *						Hw version response
 *	@date	2009/08/08	FFCS)Wangjuan modify for CR-00058-005
 *						CPRI spec V1.0.9
 *	@date	2009/11/04	FFCS)Niemsh modify for CR-00077-001
 *						RE FW version change
 *	@date	2010/05/10	FJT)Taniguchi modify for CR-000XXX:ファームウエア版数管理変更
 *						RE FW version change
 *	@date	2013/11/05 FFCS)Xut modify for ZYNQ.
 *
 *	@date	2015/08/19 TDIPS)Watanabe modify for CARD:RE転送集約変更
 *  @date   2021/25/01 M&C)Tri.hn Sends request to RE from BDE and responds to BDE from RE
 *
 *	ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2016
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "l3_com_inc.h"
#include "rrhApi_File.h"


/* @{ */
/********************************************************************************************************************/
/**
 *	@brief	   RE version info report process
 *	@note	  RE version info report process
 *	@param	  parm_p [in] the buffer address pointer of received message
 *	@return   None
 *	@retval   -
 *	@date	  2008/07/25 FFCS)linlj create
 *	@date	  2009/03/03 FFCS)Linlj (M-S3G-eNBPF-01470) ☆CPRI-L3信号受信におけるNegative判定処理
 *	@date	  2009/06/09  FFCS)Wuh modify for M-S3G-eNBPF-02047
 *						RE version response incorrect
 *	@date	  2009/07/08  FFCS)Tangj modify for CR-00054-005:[DCM向け]システム仕様書5.0.0版_090626発行
 *						3G FW Minor version is fixed 0
 *	@date	  2009/11/04  FFCS)Niemsh modify for CR-00077-001
 *						RE FW version change
 *	@date	2010/05/10	FJT)Taniguchi modify for CR-000XXX
 *						RE FW version change
 *	@date	  2013/11/05 FFCS)Xut modify for ZYNQ.
 *	@date     2015/10/03 TDI)satou gw_rcw_ReVerRep_res_tblの初期化処理見直し
 *	@date	  2015/10/03 TDIPS)watanabe 送信できる配下REがない場合のNG応答を変更
 *	@date	  2015/10/06 TDIPS)watanabe 架のハードウェアID情報識別取得を削除
 *	@date	  2015/10/06 TDIPS)watanabe 架のファームウェアID情報識別取得を削除
 *	@date	  2015/10/28 TDIPS)watanabe 応答元、REC報告内容テーブル初期化追加
 *										スロットナンバー設定、対象種別設定位置修正
 *	@date     2015/12/01 TDIPS)Watanabe REバージョン報告確認タイマのタイマ値を技説に合わせて修正
 *	@date     2016/03/18 TDI)satou 送信できる配下REが存在しない場合、タイマが動作したままになってしまうのを修正
 *  @date     2016/10/06 KCN)Takagi 16B機能追加
 *  @date     2021/01/25 M&C)Tri.hn Update based on No.23 (seq 3-23, 3-24)
 *  @date     2021/01/26 M&C)Huan.dh Specifying slot numbers 51 to 56 verion returning
 */
/********************************************************************************************************************/
VOID m_rc_ReVerRep(CMT_TSKIF_CPRIRCV_REVERREQ *parm_p)
{
	REVER_INF	rever_inf;					/* REバージョン詳細の実体		*/
	UCHAR		idinf[RCD_IDINF_MAX_NUM];	/* ID情報						*/
	CHAR		log_data[CMD_NUM64];		/* アサート情報収集用エリア		*/
	UINT		idinfcnt = CMD_NUM0;		/* ID情報配列数					*/
	UINT		name_ary = CMD_NUM0;		/* 配列数						*/
	UINT		sigchk_rslt;				/*	result of checking signal 	*/
	USHORT		sigsys_rslt;				/*	system by checking signal 	*/
	USHORT		lwTimerId;
	INT			ldwResult;
	INT			errcd;
	USHORT		lwLoop;
	USHORT		lwTimerVal;
	T_RRH_LAYER3 ltLayer3Sts;
	USHORT		lwRsp_rslt;					/*	応答結果					*/
	T_RRH_CONN_DISC pOutData;
	USHORT		chk_rslt;					/* 条件チェック結果				*/
// [16B] add start
	T_RRH_SLOT_S3G_RE	buf_LTEslotInfo_re	= {};           /* 共有メモリ取得用(配下REの報告内容 LTE) */
// [16B] add end
	UINT		llResult;

	/* スロットナンバー設定 */
	inf_slot_no = parm_p->cpridat_reverreq.slot_no;
	/* 対象種別設定 */
	inf_kind_val = parm_p->cpridat_reverreq.tgtkind;

	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_reverreq.signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt,
							"validate signal NG");
		return;
	}

	/**********/
	/* 初期化 */
	/**********/
	cm_MemClr(idinf, sizeof(idinf));
	cm_MemClr(&rever_inf, sizeof(rever_inf));
//	cm_MemClr(&gw_rcw_ReVerRecRepCnt_tbl, sizeof(gw_rcw_ReVerRecRepCnt_tbl));
//	cm_MemClr(&gw_rcw_ReVerRep_ResSrcRcvSts, sizeof(gw_rcw_ReVerRep_ResSrcRcvSts));

	/************************************/
	/* パラメータチェックと条件チェック */
	/************************************/
	chk_rslt = m_rc_VerRepChk(parm_p);

	 /* チェック結果がOK以外の場合 */
	if(chk_rslt != CMD_OK)
	{
		 /* CPRI信号応答結果にNGを設定 */
		m_rc_REVerRepRspSnd( chk_rslt, sigsys_rslt );
		return ;
	}

	/* 3G system*/
	if( CMD_SYS_3G == sigsys_rslt)
	{
		/* 処理NG(RE)(42)を返却 */
		lwRsp_rslt = CMD_OTHR_ERR_RE;
		/* 応答送信 */
		m_rc_REVerRepRspSnd( lwRsp_rslt, sigsys_rslt );
	}
	/* LTE system*/
	else
	{
		/* 正常応答(RE)(0)を返却 */
		lwRsp_rslt = CMD_RES_OK;

		/**********************/
		/* バージョン情報設定 */
		/**********************/

		/******************/
		/* 対象指定で分岐 */
		/******************/
		switch (inf_kind_val)
		{
			/********************************************************************/
			/* 対象指定が架(1)の場合											*/
			/********************************************************************/
			case CMD_FRAME :
				if (inf_slot_no > 50 && inf_slot_no < 57)
				{
					/********************************************************/
					/* ファームウェア指定									*/
					/********************************************************/
					if(parm_p->cpridat_reverreq.id_kind == CMD_FRM)
					{
						/*****************************************/
						/* LOCALエリアにREバージョン情報を格納	 */
						/*****************************************/
						/* REバージョン情報取得 */
						m_rc_FirmVerSet(&rever_inf);

						/* ID情報識別取得 */
						m_rc_FirmIdInfGet(	&rever_inf,
											idinf,
											&idinfcnt);
					}

                	/********************************************************/
                		/* ハードウェア指定 									*/
                	/********************************************************/
                	else
                	{
                    		/* ハードバージョン設定 */
                    	m_rc_HardVerSet(&cmw_tra_inv_tbl,
                                    	&rever_inf,
                                    	&name_ary);

                    	/* ID情報識別取得 */
                    	m_rc_HardIdInfGet(	&cmw_tra_inv_tbl,
                                        	name_ary,
                                        	&rever_inf,
                                        	idinf,
                                        	&idinfcnt);
                	}
					/**************************/
					/* バージョンを10進に変換 */
					/**************************/
                	rever_inf.mjr_ver = (USHORT)D_L3_CNVBCD_DEC(rever_inf.mjr_ver);
                	rever_inf.mir_ver = (USHORT)D_L3_CNVBCD_DEC(rever_inf.mir_ver);

					/*******************************************/
					/* REバージョン報告要求 (l3/rct->re/rec) 	*/
					/******************************************/
					llResult = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, &rever_inf, sizeof(CMT_TSKIF_CPRIRCV_REVERREQ));
					if(D_RRH_OK != llResult)
					{
						cm_Assert( CMD_ASL_USELOW, llResult,"[m_rc_ReVerRep] Send Msg NG" );
					}

					// /* 応答送信 */
                	// m_rc_FHMVerRepRspSnd( lwRsp_rslt, sigsys_rslt , rever_inf, idinf, idinfcnt);
                	break;
				}
				else
					{
						cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,
															"m_rc_ReVerRep intermedidate only" );
						return;
					}
				break;

			/********************************************************************/
			/* 対象指定がカード(3)の場合										*/
			/********************************************************************/
			case CMD_CARD :
				if (inf_slot_no < 51 || inf_slot_no > 56)
				{
// 				lwTimerId = CMD_TIMID_RE_VER_REP_LTE;
// 				/* タイマ起動REバージョン報告確認タイマ */
// 				lwTimerVal = 10000;
// 				ldwResult = cm_TStat(lwTimerId, lwTimerVal, CMD_TIMMD_SINGLE,
// 								CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
// 				/*Start timer NG*/
// 				if(ldwResult != CMD_RES_OK)
// 				{
// 					cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "REバージョン報告確認タイマ Start NG" );
// 					return;
// 				}

// 				/* Get layer3 status*/
// 				(VOID)f_cmn_com_layer3_get(sigsys_rslt, &ltLayer3Sts);
// 				/* RE接続有無状態取得 */
// 				f_cmn_com_conn_disk_get( &pOutData );

// 				/* REC報告内容初期化 */
// 				/* 全てのREをループする (16 RE)*/
// 				for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
// 				{
// 					/* 配下REの使用/未使用を決定するON/OFF設定がOFFになっているCPRIリンク*/
// 					if( pOutData.conn_disc[lwLoop] == D_RRH_OFF )
// 					{
// 						/* テーブル初期化 */
// 						m_rc_REVerInitSet(CMD_VER_OFF, lwLoop, sigsys_rslt);
// 					/* 対RE Layer3状態が「②RE起動中状態」または「④運用中状態」の場合 */
// 					} else if(( D_L3_STA_2 == ltLayer3Sts.layer3_re[ lwLoop ] ) || ( D_L3_STA_4 == ltLayer3Sts.layer3_re[ lwLoop ] ))
// 					{
// 						/* テーブル初期化 */
// 						m_rc_REVerInitSet(CMD_VER_NG, lwLoop, sigsys_rslt);
// 					}else{
// 						/* テーブル初期化 */
// 						m_rc_REVerInitSet(CMD_VER_UNLINK, lwLoop, sigsys_rslt);
// 					}

// 				}
// 				/* 全てのREをループする (16 RE)*/
// 				for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
// 				{
//                     /* テーブル初期化 REバージョン報告応答結果 */
// 				    gw_rcw_ReVerRep_res_tbl[sigsys_rslt][lwLoop] = CMD_RES_NG;

// 					/* 対RE Layer3状態が「②RE起動中状態」または「④運用中状態」の場合 */
// 					if(( D_L3_STA_2 == ltLayer3Sts.layer3_re[ lwLoop ] ) || ( D_L3_STA_4 == ltLayer3Sts.layer3_re[ lwLoop ] ))
// 					{
// 						/*Set link num*/
// 						parm_p->cprircv_inf.link_num = lwLoop + 1;

// // [16B] add start
//                         // 共有メモリから、LTE スロット情報(RE側)を取得
//                         f_cmn_com_slot_s3g_get(lwLoop+1,&buf_LTEslotInfo_re);

//                         // REスロット情報報告のスロット数が０でない場合
//                         if(buf_LTEslotInfo_re.slot_num != 0) {
//                             parm_p->cpridat_reverreq.slot_no = buf_LTEslotInfo_re.slot_no[0]; // スロット情報報告の最若番(スロット番号#1)のスロット番号を設定

//                         }
//                         else {
//                             // スロット数が０の場合は、REバージョン報告要求をre/recに送信しない
//                             continue;
//                             //parm_p->cpridat_reverreq.slot_no = inf_slot_no; // 退避したスロット番号を設定
//                         }
// // [16B] add end

// 						/* REバージョン報告要求をre/recに送信する */
// 						ldwResult = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, sizeof( CMT_TSKIF_CPRISND_REVERREQ ) );
// 						/*Sending success*/
// 						if( D_RRH_OK == ldwResult)
// 						{
// 							/* テーブル更新 REバージョン報告送信先←送信した配下RE */
// 							gw_rcw_ReVerRep_ReqDstSndSts[sigsys_rslt][lwLoop] = D_L3_ON;
// 							/* テーブル初期化 REバージョン報告応答元 */
// 							gw_rcw_ReVerRep_ResSrcRcvSts[sigsys_rslt][lwLoop] = D_L3_OFF;
// 						}else{
// 							cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,
// 															"m_rc_ReVerRep m_rc_ReVerRep NG" );
// 							return;
// 						}
// 					}
// 				}
// 				/* 送信できる配下REがない場合NG応答を返却する */
// 				for(lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++){
// 					if(gw_rcw_ReVerRep_ReqDstSndSts[sigsys_rslt][lwLoop] == D_L3_ON){
// 						break;
// 					}
// 					if(lwLoop == D_RRH_CPRINO_RE_MAX-1){
// 						lwRsp_rslt = CMD_HDL_NG;
// 						m_rc_REVerRepRspSnd( lwRsp_rslt, sigsys_rslt );

// 						/* REバージョン報告確認タイマ停止 */
// 						if (BPF_RU_HRTM_COMPLETE != cm_TStop(lwTimerId, &errcd))
// 						{
// 							cm_Assert(CMD_ASL_DBGLOW, lwTimerId, "cm_TStop NG");
// 						}
// 					}
// 				}

					/*******************************************/
					/* REバージョン報告要求 (l3/rct->re/rec) 	*/
					/******************************************/
					llResult = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, parm_p, sizeof(CMT_TSKIF_CPRIRCV_REVERREQ));
					if(D_RRH_OK != llResult)
					{
						cm_Assert( CMD_ASL_USELOW, llResult,"[m_rc_ReVerRep] Send Msg NG" );

						break;
					}
				}
				else
				{
					cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,"m_rc_ReVerRep RE only" );
					return;
				}
				break;
			/************************************************************/
			/* デフォルトルート											*/
			/************************************************************/
			default:

				/* 範囲チェックで保障しているため、無処理 */

				/* ログデータに要因をコピー */
				sprintf(	log_data,
							"m_rc_ReVerRep Default NG  %d %d %d",
							parm_p->cpridat_reverreq.id_kind,
							parm_p->cpridat_reverreq.slot_no,
							parm_p->cpridat_reverreq.tgtkind);

				/* アサート情報出力 */
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0,
							log_data);
		}
	}

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *	@brief	   RE version report parameter check
 *	@note	  RE version report parameter check
 *	@param	  parm_p [in] the buffer address pointer of received message
 *	@return   RETURN CODE
 *	@retval    CMD_OK							0: 	Normal end
 *	@retval    CMD_OTHR_ERR_RE		42: 	Others error
 *	@retval    CMD_READ_NG_RE			43: 	Read NG
 *	@retval    CMD_KIND_NG_RE			44: 	Target kind NG
 *	@retval    CMD_SET_NEGATIVE		45: 	Negative setting
 *	@date	  2008/07/25 FFCS)linlj create
 *	@date	  2009/03/03 FFCS)Linlj (M-S3G-eNBPF-01470) ☆CPRI-L3信号受信におけるNegative判定処理
 *	@date	  2009/08/08  FFCS)Wangjuan modify for CR-00058-005
 *						CPRI spec V1.0.9
 *	@date	  2013/11/05 FFCS)Xut modify for ZYNQ.
 *	@date	  2013/11/15 TDIPS)enoki 架指定時のSLOTチェックを削除
 */
/********************************************************************************************************************/
USHORT m_rc_VerRepChk(CMT_TSKIF_CPRIRCV_REVERREQ *parm_p)
{
	CHAR log_data[CMD_NUM64];		/* アサート情報収集用エリア		*/
	UINT slotchk_rslt;				/* スロット番号チェック結果		*/

	/****************/
	/* 範囲チェック */
	/****************/

	/* 対象指定チェック */
	if((parm_p->cpridat_reverreq.tgtkind != CMD_FRAME) &&
	   (parm_p->cpridat_reverreq.tgtkind != CMD_CARD)  &&
	   (parm_p->cpridat_reverreq.tgtkind != CMD_FAN))
	{
		/* ログデータに要因をコピー */
		sprintf(	log_data,
						"m_rc_VerRepChk ParmCHK NG  %d %d %d",
						parm_p->cpridat_reverreq.id_kind,
						parm_p->cpridat_reverreq.slot_no,
						parm_p->cpridat_reverreq.tgtkind);

		/* アサート情報出力 */
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0,
					log_data);

		/* 3Gの場合,その他エラー(42)を返却 */
		if (rcw_cpri_msg_flg == CMD_SYS_3G)
		{
			return CMD_OTHR_ERR_RE;
		}
		/* LTEの場合,処理NG(RE)(51)を返却 */
		else
		{
			return CMD_HDL_NG;
		}
	}

	/* ID識別情報チェック */
	if((parm_p->cpridat_reverreq.id_kind != CMD_HARD) &&
	   (parm_p->cpridat_reverreq.id_kind != CMD_FRM))
	{
		/* ログデータに要因をコピー */
		sprintf(	log_data,
				"m_rc_VerRepChk ParmCHK NG 1  %d %d %d",
				parm_p->cpridat_reverreq.id_kind,
				parm_p->cpridat_reverreq.slot_no,
				parm_p->cpridat_reverreq.tgtkind);

		/* アサート情報出力 */
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0,
					log_data);

		/* 3Gの場合,その他エラー(42)を返却 */
		if (rcw_cpri_msg_flg == CMD_SYS_3G)
		{
			return CMD_OTHR_ERR_RE;
		}
		/* LTEの場合,処理NG(RE)(51)を返却 */
		else
		{
			return CMD_HDL_NG;
		}
	}

	/* 対象指定により処理を分岐 */
	switch (parm_p->cpridat_reverreq.tgtkind)
	{
		/****************/
		/* 対象指定が架 */
		/****************/
		case CMD_FRAME:
			break;

		/********************/
		/* 対象指定が架またはカード */
		/********************/
		case CMD_CARD:
			/************************/
			/* スロット番号チェック */
			/************************/
			slotchk_rslt = m_rc_cm_SlotNoChk(parm_p->cpridat_reverreq.slot_no);

			/* 戻り値判定 */
			if(slotchk_rslt != CMD_OK)
			{
				/* ログデータに要因をコピー */
				sprintf(	log_data,
						"m_rc_VerRepChk ParmCHK NG 2  %d %d %d %d",
						parm_p->cpridat_reverreq.id_kind,
						parm_p->cpridat_reverreq.slot_no,
						parm_p->cpridat_reverreq.tgtkind,
						slotchk_rslt);

				/* アサート情報出力 */
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0,
							log_data);

				/* 3Gの場合,その他エラー(42)を返却 */
				if (rcw_cpri_msg_flg == CMD_SYS_3G)
				{
					return CMD_OTHR_ERR_RE;
				}
				/* LTEの場合,処理NG(RE)(51)を返却 */
				else
				{
					return CMD_HDL_NG;
				}
			}
			break;

		/*****************/
		/* 対象指定がFAN */
		/*****************/
		case CMD_FAN:

			/* ログデータに要因をコピー */
			sprintf(	log_data,
					"m_rc_VerRepChk ParmCHK NG 7  %d %d %d",
					parm_p->cpridat_reverreq.id_kind,
					parm_p->cpridat_reverreq.slot_no,
					parm_p->cpridat_reverreq.tgtkind);

			/* アサート情報出力 */
			cm_Assert(	CMD_ASL_USELOW,
						CMD_NUM0,
						log_data);

			/* 3Gの場合,その他エラー(42)を返却 */
			if (rcw_cpri_msg_flg == CMD_SYS_3G)
			{
				return CMD_OTHR_ERR_RE;
			}
			/* LTEの場合,対象指定NG(RE)(for LTE)(52)を返却 */
			else
			{
				return CMD_KIND_NG_RE_S3G;
			}

			break;
		/**********/
		/* その他 */
		/**********/
		default :

			/* ログデータに要因をコピー */
			sprintf(	log_data,
					"m_rc_VerRepChk ParmCHK NG 8  %d %d %d",
					parm_p->cpridat_reverreq.id_kind,
					parm_p->cpridat_reverreq.slot_no,
					parm_p->cpridat_reverreq.tgtkind);

			/* アサート情報出力 */
			cm_Assert(	CMD_ASL_USELOW,
						CMD_NUM0,
						log_data);

			/* 3Gの場合,その他エラー(42)を返却 */
			if (rcw_cpri_msg_flg == CMD_SYS_3G)
			{
				return CMD_OTHR_ERR_RE;
			}
			/* LTEの場合,処理NG(RE)(51)を返却 */
			else
			{
				return CMD_HDL_NG;
			}

			break;

	} /* 対象指定による処理の分岐END */

	return CMD_OK;
}

/* @} */



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief	REバージョン報告応答送信関数
 *  @note
 *  @param	awResult [in] Response result
 *  @param	awSystem [in] System type
 *  @return	None
 *  @date	2015/08/17 TDIPS)Shingo.W
 *  @date	2016/03/18 TDI)satou NG応答の場合、情報数を0で報告するように
 */
/********************************************************************************************************************/
VOID m_rc_REVerRepRspSnd( USHORT awResult, USHORT awSystem )
{
	UINT   bufget_rslt;						/* バッファ獲得結果				*/

	USHORT datalength;						/* データ長						*/
	UINT   idinfcnt 	= CMD_NUM0;			/* ID情報配列数					*/
	USHORT loopcnt;
	UCHAR  idinf[RCD_IDINF_MAX_NUM];		/* ID情報						*/
	UINT   ressnd_rslt __attribute__((unused));		/* 応答送信結果			*/
	CMT_TSKIF_CPRISND_REVERRES *rsp_p;
	REVER_INF rever_arry[D_RRH_CPRINO_RE_MAX];

	/**********/
	/* 初期化 */
	/**********/
	cm_MemClr(rever_arry, sizeof(rever_arry));

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 最大値のバッファを獲得 */
	rsp_p		 = NULL;

	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_REVERRES)	+	/* 信号種別～情報数まで	*/
								sizeof(rever_arry),	/* 名称～年月日			*/
								CMD_NUM1,
								(VOID **)&rsp_p	);
	/********************/
	/* バッファ獲得失敗 */
	/********************/
	if(bufget_rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					bufget_rslt,
					"m_rc_ReVerRep cm_L2BfGet NG");
		return;
	}

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	datalength = (USHORT)(	sizeof(CMT_CPRISIGSND_INF)
						+	sizeof(CMT_CPRIF_REVERRES)
						+	sizeof(rever_arry));

	/* 信号種別 		   2byte					*/
	/* 応答結果 		   2byte					*/
	/* メーカ名 		   1byte * 16	= 16byte	*/
	/* 対象指定 		   2byte					*/
	/* スロット番号 	   2byte					*/
	/* ID情報識別		   1byte * 1024 = 1024byte	*/
	/* 情報数			   2byte 合計1050byte	    */

	/* 名称 			   1byte * 10	= 10byte	 REVER_INF */
	/* メジャーバージョン  2byte					*/
	/* マイナーバージョン  2byte					*/
	/* 年				   2byte					*/
	/* 月日 			   2byte 合計18byte	    	*/

	/********************/
	/* CPRI信号種別設定 */
	/********************/
	rsp_p->cpridat_reverres.signal_kind = CMD_CPRID_REVERRES + awSystem;

	/********************/
	/* 応答結果設定 */
	/********************/
	/* CPRI信号応答結果を設定 */
	rsp_p->cpridat_reverres.result = awResult;

	/********************/
	/* メーカ名(FUJITSU) */
	/********************/
	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM0] = 'F';
	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM1] = 'U';
	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM2] = 'J';
	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM3] = 'I';
	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM4] = 'T';
	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM5] = 'S';
	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM6] = 'U';
	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM7] = 0;

	/************/
	/* 対象指定 */
	/************/
	rsp_p->cpridat_reverres.rests_inf.kind = inf_kind_val;

	/****************/
	/* スロット番号 */
	/****************/
	rsp_p->cpridat_reverres.rests_inf.slot_no = inf_slot_no;

	/******************/
	/* ID情報識別設定 */
	/******************/
	for (loopcnt = CMD_NUM0; loopcnt < idinfcnt; loopcnt++)
	{
		rsp_p->cpridat_reverres.rests_inf.id_kind[loopcnt] = idinf[loopcnt];
	}

	/****************/
	/* ID情報数設定 */
	/****************/
	if (CMD_NML == rsp_p->cpridat_reverres.result)
	{	/* 正常応答の場合、情報数は最大数固定 */
		rsp_p->cpridat_reverres.rests_inf.inf_num = D_RRH_CPRINO_RE_MAX;

		rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + datalength;

		/************************/
		/* バージョン情報数設定 */
		/************************/
		/* REC報告内容テーブルをrever_infに代入する */
		m_rc_REVerInfoSet(awSystem, rever_arry);

		/**********************/
		/* 応答データにコピー */
		/***l*******************/
		cm_MemCpy(&(rsp_p->cpridat_reverres.rests_inf.inf_num) + CMD_NUM1,
					  rever_arry		,
					  sizeof(rever_arry));
	}
	else
	{	/* NG応答の場合、情報数は0で報告する */
		rsp_p->cpridat_reverres.rests_inf.inf_num = 0;
		rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_REVERRES);
	}

	/************/
	/* 応答送信 */
	/************/

	ressnd_rslt = m_cm_L3MsgSend_REC(rsp_p);

	if( ressnd_rslt != CMD_L3_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,
								"m_rc_ReVerRep m_rc_REVerRepRspSnd NG" );
	}

	return ;
}
/* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief	FHMバージョン報告応答送信関数
//  *  @param	awResult  [in] Response result
//  *  @param	awSystem  [in] System type
//  *  @param  rever_inf [in] RE version information
//  *  @param  idinf     [in] ID情報識別
//  *  @param  idinfcnt  [in] ID情報識別数
//  *  @return	None
//  *  @date	2015/08/17 TDIPS)Shingo.W
//  *  @date   2015/10/01 TDI)satou ID情報識別を引数で渡すように
//  *  @date	2015/10/06 TDIPS)watanabe 10進変換削除
//  *  @date   2021/25/01 M&C)Tri.hn Remove this function
//  */
// /********************************************************************************************************************/
// VOID m_rc_FHMVerRepRspSnd( USHORT awResult, USHORT awSystem , REVER_INF	rever_inf, UCHAR *idinf, UINT idinfcnt)
// {
// 	UINT   bufget_rslt;						/* バッファ獲得結果				*/

// 	USHORT datalength;						/* データ長						*/
// 	USHORT loopcnt;
// 	UINT   ressnd_rslt __attribute__((unused));		/* 応答送信結果			*/
// 	CMT_TSKIF_CPRISND_REVERRES *rsp_p;

// 	/****************/
// 	/* バッファ獲得 */
// 	/****************/
// 	/* 最大値のバッファを獲得 */
// 	rsp_p		 = NULL;
// 	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
// 								sizeof(CMT_TSKIF_CPRISND_REVERRES)	+	/* 信号種別～情報数まで	*/
// 								sizeof(rever_inf),	/* 名称～年月日			*/
// 								CMD_NUM1,
// 								(VOID **)&rsp_p	);
// 	/********************/
// 	/* バッファ獲得失敗 */
// 	/********************/
// 	if(bufget_rslt != CMD_RES_OK)
// 	{
// 		/* ASSERT */
// 		cm_Assert(	CMD_ASL_USELOW,
// 					bufget_rslt,
// 					"m_rc_ReVerRep cm_L2BfGet NG");
// 		return;
// 	}

// 	/**********************/
// 	/* タスク間ヘッダ作成 */
// 	/**********************/
// 	datalength = (USHORT)(	sizeof(CMT_CPRISIGSND_INF)
// 						+	sizeof(CMT_CPRIF_REVERRES)
// 						+	sizeof(REVER_INF));

// 	/* 信号種別 		   2byte					*/
// 	/* 応答結果 		   2byte					*/
// 	/* メーカ名 		   1byte * 16	= 16byte	*/
// 	/* 対象指定 		   2byte					*/
// 	/* スロット番号 	   2byte					*/
// 	/* ID情報識別		   1byte * 1024 = 1024byte	*/
// 	/* 情報数			   2byte 合計1050byte	    */

// 	/* 名称 			   1byte * 10	= 10byte	 REVER_INF */
// 	/* メジャーバージョン  2byte					*/
// 	/* マイナーバージョン  2byte					*/
// 	/* 年				   2byte					*/
// 	/* 月日 			   2byte 合計18byte	    	*/

// 	/********************/
// 	/* CPRI信号種別設定 */
// 	/********************/
// 	rsp_p->cpridat_reverres.signal_kind = CMD_CPRID_REVERRES + awSystem;

// 	/********************/
// 	/* 応答結果設定 */
// 	/********************/
// 	/* CPRI信号応答結果を設定 */
// 	rsp_p->cpridat_reverres.result = awResult;

// 	/********************/
// 	/* メーカ名(FUJITSU) */
// 	/********************/
// 	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM0] = 'F';
// 	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM1] = 'U';
// 	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM2] = 'J';
// 	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM3] = 'I';
// 	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM4] = 'T';
// 	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM5] = 'S';
// 	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM6] = 'U';
// 	rsp_p->cpridat_reverres.rests_inf.maker_name[CMD_NUM7] = 0;

// 	/************/
// 	/* 対象指定 */
// 	/************/
// 	rsp_p->cpridat_reverres.rests_inf.kind = inf_kind_val;

// 	/****************/
// 	/* スロット番号 */
// 	/****************/
// 	rsp_p->cpridat_reverres.rests_inf.slot_no = inf_slot_no;

// 	/**********************/
// 	/* ID情報数初期値設定 */
// 	/**********************/
// 	rsp_p->cpridat_reverres.rests_inf.inf_num = RCD_VERREP_INFNUM;

// 	/******************/
// 	/* ID情報識別設定 */
// 	/******************/
// 	for (loopcnt = CMD_NUM0; loopcnt < idinfcnt; loopcnt++)
// 	{
// 		rsp_p->cpridat_reverres.rests_inf.id_kind[loopcnt] = idinf[loopcnt];
// 	}

// 	/****************/
// 	/* ID情報数設定 */
// 	/****************/
// 	rsp_p->cpridat_reverres.rests_inf.inf_num = RCD_VERREP_INFNUM;

// 	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + datalength;


// 	/**********************/
// 	/* 応答データにコピー */
// 	/**********************/
// 	cm_MemCpy(&(rsp_p->cpridat_reverres.rests_inf.inf_num) + CMD_NUM1,
// 				  &rever_inf,
// 				  sizeof(REVER_INF));

// 	/************/
// 	/* 応答送信 */
// 	/************/

// 	ressnd_rslt = m_cm_L3MsgSend_REC(rsp_p);

// 	if( ressnd_rslt != CMD_L3_OK )
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,
// 								"m_rc_ReVerRep m_rc_FHMVerRepRspSnd NG" );
// 	}

// 	return ;
// }
// /* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *	@brief	   RE Hardware version info set
 *	@note	  RE Hardware version info set
 *	@param	  ref_tbl [in] reference table
 *	@param	  rever_inf_p [out] RE hardware version info
 *	@param	  arynum [out] RE hardware version info list length
 *	@return   None
 *	@retval   -
 *	@date	  2008/07/25 FFCS)linlj create
 *	@date     2015/10/01 TDI)satou 参照テーブルの型をT_RRH_TRAINVTBLに変更(起動時にEEPROMの内容を全展開した情報)
 *	@date	  2015/10/22 TDIPS)watanabe マイナーバージョンとメジャーバージョンの設定値を逆に変更
 */
/********************************************************************************************************************/
VOID m_rc_HardVerSet(VOID*		ref_tbl,
					 REVER_INF*	rever_inf_p,
					 UINT*		arynum)

{
    T_RRH_TRAINVTBL *tbl;					/* 参照テーブル(ダミー型)		*/
	UCHAR	name[CMD_NUM24];				/* 名称格納配列					*/
	UINT	loopcnt;						/* ループカウンタ				*/
	UINT	setcnt;							/* カード名設定カウンタ			*/
	UINT	name_cnt	 = CMD_NUM0;		/* カウンタ						*/
	UINT	getname_rslt = CMD_NG;			/* 名称取得処理結果				*/


	/* 初期化 */
	cm_MemClr(name, sizeof(name));

	/* 架インベントリテーブルの型でかぶせる */
	/* (テーブルの構造が全て同一の為）		*/
	tbl = (T_RRH_TRAINVTBL *)ref_tbl;

	/**************************/
	/* バージョン情報詳細設定 */
	/**************************/

	/* ■名称にカード品名を設定■	*/
	/* MDIF,TRA,シェルフのカード品名をローカルに入れ替える */
	/* カード名設定カウンタ初期化 */
	setcnt = CMD_NUM0;
	for (loopcnt = CMD_NUM0; loopcnt < RCD_CARDNAME_MAX; loopcnt++)
	{
		name[setcnt] = (UCHAR)(( tbl->cmx_cardname[loopcnt] & RCD_HI_MSK) >> CMD_BIT8);
		setcnt++;
		name[setcnt] = (UCHAR)( tbl->cmx_cardname[loopcnt] & RCD_LW_MSK);
		setcnt++;
	}

	/* カード品名の最後は、ハード側でNULLを設定しているため、NULLを検索 */
	for (loopcnt = CMD_NUM0; loopcnt < RCD_MAX_NAME; loopcnt++)
	{
		/* NULLがあった場合、配列数を保持して	*/
		/* ループを抜ける						*/
		if(name[loopcnt] == 0)
		{
			*arynum  = loopcnt;
			name_cnt = loopcnt;
			getname_rslt = CMD_OK;
			break;
		}
	}

	/* 獲得結果がNGの場合カード名称最大数(10)に設定 */
	if (getname_rslt == CMD_NG)
	{
		*arynum  = RCD_MAX_NAME;
		name_cnt = RCD_MAX_NAME;
	}


	/* 応答データに取得名称を設定 */
	for (loopcnt = CMD_NUM0; loopcnt < name_cnt; loopcnt++)
	{
		rever_inf_p->name[loopcnt] = name[loopcnt];
	}

	/* メジャーバージョン設定 */
	rever_inf_p->mjr_ver   = (USHORT)(tbl->cmx_hver1 & RCD_LW_MSK);

	/* マイナーバージョン設定 */
	rever_inf_p->mir_ver   = (USHORT)((tbl->cmx_hver1 & RCD_HI_MSK) >> CMD_BIT8);

	/* 年月日設定 */
	/* 年下2桁 */
	/* (USHORT = USHORT & 0x00FF) */
	rever_inf_p->year	   =  (USHORT)(tbl->cmx_pcbbyer & RCD_LW_MSK);
	/* (USHORT |= 0x2000) */
	rever_inf_p->year	  |= RCD_YEAR2000_MSK;
	/* 月日 */
	/* (USHORT |= USHORT) */
	rever_inf_p->month_day = tbl->cmx_pcbbday;

	return ;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *	@brief	   RE version info set
 *	@note	   REC報告内容初期設定関数
 *
 *	@param	  priority 初期化優先度
 *	@param	  Loop ループ添え字
 *	@param	  lte3g 信号種別
 *	@return   None
 *	@retval   -
 *	@date	  2015/08/14 TDIPS)S.Watanabe create
 */
/********************************************************************************************************************/
VOID m_rc_REVerInitSet(INT priority,USHORT Loop, USHORT lte3g)
{
	REVER_INF rever_inf = {{0}};
	/* 名称設定 */
	rever_inf.name[CMD_NUM0] = 'C';
	rever_inf.name[CMD_NUM1] = 'P';
	rever_inf.name[CMD_NUM2] = 'R';
	rever_inf.name[CMD_NUM3] = 'I';

	switch (priority)
	{
		case CMD_VER_OFF:
			rever_inf.name[CMD_NUM4] = '_';
			rever_inf.name[CMD_NUM5] = '_';
			rever_inf.name[CMD_NUM6] = '_';
			rever_inf.name[CMD_NUM7] = 'O';
			rever_inf.name[CMD_NUM8] = 'F';
			rever_inf.name[CMD_NUM9] = 'F';
			break;
		case CMD_VER_UNLINK:
			rever_inf.name[CMD_NUM4] = 'U';
			rever_inf.name[CMD_NUM5] = 'N';
			rever_inf.name[CMD_NUM6] = 'L';
			rever_inf.name[CMD_NUM7] = 'I';
			rever_inf.name[CMD_NUM8] = 'N';
			rever_inf.name[CMD_NUM9] = 'K';
			break;
		case CMD_VER_NG:
			rever_inf.name[CMD_NUM4] = '_';
			rever_inf.name[CMD_NUM5] = '_';
			rever_inf.name[CMD_NUM6] = '_';
			rever_inf.name[CMD_NUM7] = '_';
			rever_inf.name[CMD_NUM8] = 'N';
			rever_inf.name[CMD_NUM9] = 'G';
			break;
		default :
			/* NOT LEACHED */
			break;
	}

	/* メジャーバージョン設定 */
	rever_inf.mjr_ver
		= (USHORT)CMD_VERNO_MAX;

	/* マイナーバージョン設定 */
	rever_inf.mir_ver
		= (USHORT)CMD_VERNO_MAX;

	/* 年月日設定 */
	/* 年 */
	rever_inf.year	= CMD_VERYERSET_INT;

	/* 月日 */
	rever_inf.month_day	= CMD_VERMON_DAYSET_INT;

	/* テーブルに格納 */
	gw_rcw_ReVerRecRepCnt_tbl[lte3g][Loop] = rever_inf;

	return ;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *	@brief	   Get Hardware ID info
 *	@note	  Get Hardware ID info
 *	@param	  ref_tbl [in] reference table
 *	@param	  namearynum [in] name array count
 *	@param	  rever_inf_p [in] version info
 *	@param	  idinf [out] Hardware version ID info
 *	@param	  idinfcnt [out] version ID info count
 *	@return   None
 *	@retval   -
 *	@date	  2008/07/25 FFCS)linlj create
 */
/********************************************************************************************************************/
VOID m_rc_HardIdInfGet(	VOID*		ref_tbl,
						UINT		namearynum,
						REVER_INF*	rever_inf_p,
						UCHAR*		idinf,
						UINT*		idinfcnt)
{
	UINT	nameloopcnt;					/* ループカウンタ				*/
	T_RRH_TRAINVTBL *tbl;					/* 参照テーブル(ダミー型)		*/

	/* ダミー(架)の型にかぶせる */
	tbl = (T_RRH_TRAINVTBL *)ref_tbl;

	/* ID情報カウンタを0クリアする */
	*idinfcnt = CMD_NUM0;

	/* ■カード名■			*/
	for(nameloopcnt = CMD_NUM0; nameloopcnt < namearynum; nameloopcnt++)
	{
		idinf[*idinfcnt] = rever_inf_p->name[nameloopcnt];
		(*idinfcnt)++;
	}

	idinf[*idinfcnt] = '/';
	(*idinfcnt)++;

	/* ■製造管理番号■	*/
	/* yy */
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (tbl->cmx_pcbno1 & RCD_MSK_F000) >> CMD_BIT12 ];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (tbl->cmx_pcbno1 & RCD_MSK_0F00) >> CMD_BIT8 ];
	(*idinfcnt)++;

	/* yymm */
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (tbl->cmx_pcbno1 & RCD_MSK_00F0) >> CMD_BIT4 ];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (tbl->cmx_pcbno1 & RCD_MSK_000F) ];
	(*idinfcnt)++;

	/* yymm_ */
	idinf[*idinfcnt] = '_';
	(*idinfcnt)++;

	/* ■製造管理番号■	*/
	/* シリアル上位 yymm_aa*/
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (tbl->cmx_pcbno2 & RCD_MSK_00F0) >> CMD_BIT4 ];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[	tbl->cmx_pcbno2 & RCD_MSK_000F ];
	(*idinfcnt)++;

	/* ■製造管理番号■	*/
	/* シリアル下位 yymm_aaaaaa*/
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (tbl->cmx_pcbno3 & RCD_MSK_F000) >> CMD_BIT12];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (tbl->cmx_pcbno3 & RCD_MSK_0F00) >> CMD_BIT8];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (tbl->cmx_pcbno3 & RCD_MSK_00F0) >> CMD_BIT4];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[	tbl->cmx_pcbno3 & RCD_MSK_000F ];
	(*idinfcnt)++;

	/* yymm_aaaaaa/ */
	idinf[*idinfcnt] = '/';
	(*idinfcnt)++;

	/* ■バージョン■ */
	/* メジャーバージョン yymm_aaaaaa/zz */
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->mjr_ver & RCD_MSK_00F0) >> CMD_BIT4 ];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ rever_inf_p->mjr_ver & RCD_MSK_000F ];
	(*idinfcnt)++;

	/* マイナーーバージョン yymm_aaaaaa/zzzz */
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->mir_ver & RCD_MSK_00F0) >> CMD_BIT4 ];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ rever_inf_p->mir_ver & RCD_MSK_000F ];
	(*idinfcnt)++;

	/* 最後にNULLを入れる	*/
	idinf[*idinfcnt] = 0;
	(*idinfcnt)++;

	return ;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *	@brief	   RE firmware version info set
 *	@note	  RE firmware version info set
 *	@param	  rever_inf_p [out] RE firmware version info
 *	@return   None
 *	@retval   -
 *	@date	  2008/07/25 FFCS)linlj create
 *	@date	  2013/11/05 FFCS)Xut modify for ZYNQ.
 *	@date     2015/10/03 TDI)satou 名称設定をFHMに変更
 */
/********************************************************************************************************************/
VOID m_rc_FirmVerSet(REVER_INF *rever_inf_p)
{

	/* 名称設定 */
	rever_inf_p->name[CMD_NUM0] = 'F';
	rever_inf_p->name[CMD_NUM1] = 'H';
	rever_inf_p->name[CMD_NUM2] = 'M';
	rever_inf_p->name[CMD_NUM3] = 0;

	/* メジャーバージョン設定 */
	rever_inf_p->mjr_ver
		= (USHORT)(cmw_versionData.usever >> CMD_BIT8);

	/* マイナーバージョン設定 */
	rever_inf_p->mir_ver
		= (USHORT)(cmw_versionData.usever & RCD_LW_MSK);

	/* 年月日設定 */
	/* 年 */
	rever_inf_p->year	= cmw_versionData.year;

	/* +月日 */
	rever_inf_p->month_day	= cmw_versionData.mon_day;

	return ;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *	@brief	   RE Firm ID info report process
 *	@note	  RE Firm ID info report process
 *	@param	  rever_inf_p [in] version info
 *	@param	  idinf [out] version ID info
 *	@param	  idinfcnt [out] version ID info count
 *	@return   None
 *	@retval   -
 *	@date	  2008/07/25 FFCS)linlj create
 */
/********************************************************************************************************************/
VOID m_rc_FirmIdInfGet(	REVER_INF*	rever_inf_p,
						UCHAR*		idinf,
						UINT*		idinfcnt)
{
	UINT	nameloopcnt;					/* ループカウンタ				*/
	UINT	arynum;							/* 配列数						*/

	/* RE/VERxxxx/yyyy/mm/dd									 */
	/* xxxx 	  : ファーム版数								 */
	/* yyyy/mm/dd : 日付										 */
	/* ID情報の区切りは"/"とし、日付の後の区切りはスペースとする */
	/* 半角1024文字以内、英数字で表現する						 */
	/* 文字列の最後はNULLとする 								 */

	/* 配列数 固定値(RE)*/
	arynum = CMD_NUM3;

	/* カード名 RE固定*/
	for(nameloopcnt = CMD_NUM0; nameloopcnt < arynum; nameloopcnt++)
	{
		idinf[*idinfcnt] = rever_inf_p->name[nameloopcnt];
		(*idinfcnt)++;
	}

	/* / */
	idinf[*idinfcnt] = '/';
	(*idinfcnt)++;

	/* VER */
	idinf[*idinfcnt] = 'V';
	(*idinfcnt)++;

	idinf[*idinfcnt] = 'E';
	(*idinfcnt)++;

	idinf[*idinfcnt] = 'R';
	(*idinfcnt)++;

	/* メジャーバージョン */
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->mjr_ver & RCD_MSK_00F0) >> CMD_BIT4 ];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ rever_inf_p->mjr_ver & RCD_MSK_000F ];
	(*idinfcnt)++;

	/* マイナーバージョン */
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->mir_ver & RCD_MSK_00F0) >> CMD_BIT4 ];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ rever_inf_p->mir_ver & RCD_MSK_000F ];
	(*idinfcnt)++;

	/* / */
	idinf[*idinfcnt] = '/';
	(*idinfcnt)++;

	/* 年 */
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->year & RCD_MSK_F000) >> CMD_BIT12];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->year & RCD_MSK_0F00) >> CMD_BIT8];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->year & RCD_MSK_00F0) >> CMD_BIT4];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[	rever_inf_p->year & RCD_MSK_000F ];
	(*idinfcnt)++;

	/* / */
	idinf[*idinfcnt] = '/';
	(*idinfcnt)++;

	/* 月 */
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->month_day & RCD_MSK_F000) >> CMD_BIT12];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->month_day & RCD_MSK_0F00) >> CMD_BIT8];
	(*idinfcnt)++;

	/* / */
	idinf[*idinfcnt] = '/';
	(*idinfcnt)++;

	/* 日 */
	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ (rever_inf_p->month_day & RCD_MSK_00F0) >> CMD_BIT4 ];
	(*idinfcnt)++;

	idinf[*idinfcnt] = (UCHAR)rcr_bcd_cvt_tbl[ rever_inf_p->month_day & RCD_MSK_000F ];
	(*idinfcnt)++;

	/* 最後にNULLを入れる	*/
	idinf[*idinfcnt] = 0;
	(*idinfcnt)++;

	return ;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief 配下REからのREバージョン報告応答受信関数
 *  @note
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/08 TDIPS)Watanabe
 *  @date   2021/25/01 M&C)Tri.hn Sends request to RE from BDE and responds to BDE from RE
 */
/********************************************************************************************************************/

VOID m_rc_ReVerRepRes( CMT_TSKIF_CPRIRCV_REVERRES* parm_p )
{
	UINT		lwSigchk_rslt;					  //result of checking signal
	USHORT		lwSigsys_rslt;					  //system by checking signal
//	USHORT		lwReNo;
//	USHORT		lwTimerId;
//	USHORT		lwRcvAllResFlag;
//	USHORT		lwResAllResFlag;
//	INT			ldwResult;
//	INT			errcd;
//	UINT   ressnd_rslt __attribute__((unused));		// 応答送信結果


	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* validate signal*/
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_reverres.signal_kind, &lwSigsys_rslt);
	 /*signalがNGの場合*/
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		 /*アサート出力処理*/
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");
		return;
	}

// 	 /*3G system*/
// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		return;
// 		//lwTimerId = CMD_TIMID_RE_VER_REP_3G;
// 	}
// 	 /*LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_RE_VER_REP_LTE;
// 	}

// 	 /*Check CPRI番号*/
// 	if(( parm_p->cprircv_inf.link_num > D_RRH_CPRINO_RE_MAX ) || ( parm_p->cprircv_inf.link_num < D_RRH_CPRINO_RE_MIN ))
// 	{
// 		/* アサート出力処理*/
// 		cm_Assert(	CMD_ASL_USELOW,
// 							parm_p->cprircv_inf.link_num,
// 							"CPRI番号 is invalid");
// 		return;
// 	}

// 	lwReNo = parm_p->cprircv_inf.link_num - 1;

// 	 /*配下REからの応答待ち中の場合*/
// 	if( gw_rcw_ReVerRep_ReqDstSndSts[lwSigsys_rslt][lwReNo] == D_L3_ON )
// 	{
// 		 /*テーブル更新 REバージョン報告応答元←受信した配下RE*/
// 		gw_rcw_ReVerRep_ResSrcRcvSts[lwSigsys_rslt][lwReNo] = D_L3_ON;
// 		 /*テーブル更新 REバージョン報告応答結果←応答結果*/
// 		gw_rcw_ReVerRep_res_tbl[lwSigsys_rslt][lwReNo] = parm_p->cpridat_reverres.result;
// 		/*応答結果が「0:正常」の場合*/
// 		if( CMD_RES_OK == parm_p->cpridat_reverres.result )
// 		{
// 			cm_MemCpy(&gw_rcw_ReVerRecRepCnt_tbl[lwSigsys_rslt][lwReNo],
// 					&(parm_p->cpridat_reverres.rests_inf.inf_num) + CMD_NUM1,
// 					sizeof(REVER_INF));
// 		}
// 		 /*要求を送信したすべての配下REから正常応答を受信チェック*/
// 		lwRcvAllResFlag = m_rc_ReVerRepRcvAllChk( lwSigsys_rslt );
// 		 /*要求を送信したすべての配下REから正常応答を受信場合*/
// 		if( CMD_TRUE == lwRcvAllResFlag )
// 		{
// 			 /*タイマ停止 REバージョン報告確認タイマ*/
// 			ldwResult = cm_TStop( lwTimerId, &errcd );
// 			/*Stop timer NG*/
// 			if(ldwResult != CMD_RES_OK)
// 			{
// 				cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "REバージョン報告確認タイマ Stop NG" );
// 				return;
// 			}
// 			/* 配下REからの応答結果をチェック*/
// 			lwResAllResFlag = m_rc_ReVerRepResAllChk( lwSigsys_rslt );
// 			 /*1台以上の配下REから応答結果「0:正常」を受信*/
// 			if(lwResAllResFlag == CMD_TRUE)
// 			{
// 				m_rc_REVerRepRspSnd( CMD_NML,lwSigsys_rslt );
// 			 /*すべての配下REから異常応答を受信*/
// 			}else{
// 				m_rc_REVerRepRspSnd( CMD_HDL_NG, lwSigsys_rslt );
// 			}
// 		}
// 	}

	/* REバージョン報告応答 */
	m_rc_REVerRepRspSnd( CMD_NML,lwSigsys_rslt );
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief バージョン情報設定関数
 *  @param  awSystem        [in ] system type
 *  @param  rever_p         [out] バージョン情報設定先
 *  @return none
 *  @date	2015/09/01 TDIPS)Watanabe
 *  @date   2015/10/03 TDI)satou エンディアン変換を削除(L2プロセスで送信する際に変換されるため)
 *  @date	2015/10/06 TDIPS)watanabe 10進変換削除
 *  @date	2015/10/17 TDIPS)watanabe バージョン情報#n名称設定追加
 *  @date	2015/10/22 TDIPS)watanabe メーカー名取得先変更
 *  @date	2015/10/28 TDIPS)watanabe バージョン情報名称設定条件修正
 */
/********************************************************************************************************************/
VOID m_rc_REVerInfoSet(USHORT awSystem, REVER_INF* rever_p) {

	UINT loop;								 //ループ変数
	REVER_INF tmp;						//REC報告内容用（1～16）
	T_RRH_EQP_S3G                     ltSynReEqpInfShmlte;					/** Synthesized Re equipment infromation that is get from shared memory (LTE)*/

	for(loop = 0;loop < D_RRH_CPRINO_RE_MAX; loop++){
		 /*REC報告内容テーブルをrever_infに代入する*/
		tmp = gw_rcw_ReVerRecRepCnt_tbl[awSystem][loop];
		if( (gw_rcw_ReVerRep_ResSrcRcvSts[awSystem][loop] == D_L3_ON) && (gw_rcw_ReVerRep_res_tbl[awSystem][loop] == CMD_RES_OK) )
		{
			/***********************
			 バージョン情報名称設定
			***********************/
			f_cmn_com_eqp_s3g_get( loop+1, &ltSynReEqpInfShmlte );
			/* 「”配下REのRE装置構成情報報告応答のCPRIリンク装置構成情報のメーカ名”+” “+”配下REのバージョン情報#n名称“」で通知 */
			tmp.name[9] = tmp.name[7];
			tmp.name[8] = tmp.name[6];
			tmp.name[7] = tmp.name[5];
			tmp.name[6] = tmp.name[4];
			tmp.name[5] = tmp.name[3];
			tmp.name[4] = tmp.name[2];
			tmp.name[3] = tmp.name[1];
			tmp.name[2] = tmp.name[0];
			tmp.name[1] = '_';
			tmp.name[0] = ltSynReEqpInfShmlte.eqp_inf[0];
		}
		rever_p[loop] = tmp;
	}
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief 要求を送信したすべての配下REからの受信チェック関数
 *  @note
 *  @param  adw_Sys_type [in] system type
 *  @return None
 *  @date	2015/07/08 Fpt)Hieu
 */
/********************************************************************************************************************/

USHORT m_rc_ReVerRepRcvAllChk( USHORT adw_Sys_type)
{
	USHORT	lwLoop;

	/* 全てのREをループする (16 RE)*/
	for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		/* REバージョン報告送信先が送信した場合 */
		if( D_L3_ON == gw_rcw_ReVerRep_ReqDstSndSts[adw_Sys_type][lwLoop] )
		{
			/* 更新REバージョン報告応答元が受信しない場合 */
			if( D_L3_OFF == gw_rcw_ReVerRep_ResSrcRcvSts[adw_Sys_type][lwLoop] )
			{
				return CMD_FALSE;
			}
		}

	}

	return CMD_TRUE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief 要求を送信したすべての配下REからの応答結果チェック関数
 *  @note
 *  @param  adw_Sys_type [in] system type
 *  @return None
 *  @date	2015/08/17 tdips)S.Watanabe create
 */
/********************************************************************************************************************/

USHORT m_rc_ReVerRepResAllChk( USHORT adw_Sys_type)
{
	USHORT	lwLoop;

	/* 全てのREをループする (16 RE)*/
	for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		/* REバージョン報告応答結果が正常（0）の場合 */
		if( CMD_RES_OK == gw_rcw_ReVerRep_res_tbl[adw_Sys_type][lwLoop] )
		{
			return CMD_TRUE;
		}
	}

	return CMD_FALSE;
}
/* @} */


// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief REバージョン報告確認タイマ タイムアウト関数
//  *  @note
//  *  @param  awTimerId [in] タイマ ID
//  *  @return None
//  *  @date	2015/08/18 TDIPS)S.Watanabe create
//  *  @date   2015/10/03 TDI)satou NG応答コードを修正
//  *  @date   2021/25/01 M&C)Tri.hn Remove this function
//  */
// /********************************************************************************************************************/

// VOID m_rc_ReVerRepTO( USHORT awTimerId )
// {
// 	USHORT			lwSystemType;
// 	USHORT			lwResAllResFlag;
// 	USHORT			lwRsp_rslt;

// 	lwSystemType = CMD_SYS_LTE;

// 	lwResAllResFlag = m_rc_ReVerRepResAllChk( lwSystemType );
// 	/* 1台以上の配下REから応答結果「0:正常」を受信 */
// 	if(lwResAllResFlag == CMD_TRUE)
// 	{
// 		lwRsp_rslt = CMD_NML;
// 	/* すべての配下REから異常応答を受信 */
// 	}else
// 	{
// 		lwRsp_rslt = CMD_HDL_NG;
// 	}
// 	m_rc_REVerRepRspSnd(lwRsp_rslt, lwSystemType);
// 	return ;
// }
// /* @} */

/* @} */
/*!
 *	@brief	the function processing for receiving subscribe get version response.
 *	@note	This function is processed as follows.
 *			1) set version values into shared memory
 *	@param	buf_p [in]	Pointer of buffer
 *	@param	- [out] -
 *	@return Void
 *	@retval -
 *	@date	2014/03/07 FFCS)linb Create.
 */
VOID m_rc_SubGetVerRsp(VOID* buf_p)
{
	T_API_FILE_FIRM_VERSION_RSP	 *verrsp_p = NULL;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_rc_SubGetVerRsp] ENTER" );

	verrsp_p = (T_API_FILE_FIRM_VERSION_RSP *)buf_p;

	/* save latest version information */
	memcpy((void*)&cmw_versionData, (void*)&verrsp_p->data, sizeof(T_API_FILE_FIRM_VERSION_DATA));

	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,	"[m_rc_SubGetVerRsp] RETURN" );
	return;
}
/* @} */


/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_def.h
 *  @brief  CPRI management const variable define file
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2015/07/27 TDIPS)ikeda * 
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/
#ifndef    M_CR_DEF
#define    M_CR_DEF

/** @addtogroup RRH_L3_CPR
* @{ */

/** @name main matrix table define */
/* @{ */

/****************************************************************************/
/*	main matrix table define											*/
/****************************************************************************/
#define CRD_MX_STATE		5			/*	maximum task state number		*/
#define CRD_MX_EVENT		17			/*	maximum event number			*/
/* @} */

/** @name task state define */
/* @{ */

/****************************************************************************/
/* task state define                                                        */                                                           
/****************************************************************************/
#define CRD_TASKSTA_INIT			0			/* 初期状態						*/
#define CRD_TASKSTA_REC_STARTUP	1			/* REC開始要求待ち				*/
#define CRD_TASKSTA_REC_ACTIVE		2			/* RECStateE待ち				*/
#define CRD_TASKSTA_RE_STARTUP		3			/* RE起動開始					*/
#define CRD_TASKSTA_RUN			4			/* 運用中状態					*/
/* @} */


#define CRD_MX_CPRILNKSTA    8              /* max CPRI link state*/


/** @name task internal event code define */
/* @{ */

/****************************************************************************/
/* task internal event code define                                          */                                                                             
/****************************************************************************/
#define CRD_INTEREV_INITREQ		0	/* 初期化要求 */
#define CRD_INTEREV_RECCPRIPREREQ	1
#define CRD_INTEREV_CPRISTACHG		2	/* CPRI State変化通知 */
#define CRD_INTEREV_RE_CPRI_START	3
#define CRD_INTEREV_RECMPSTP		4
#define CRD_INTEREV_LAPBLNKCHGNTC	5	/* LAPB link状態変化通知 */
#define CRD_INTEREV_RECCPRISTART	6
#define CRD_INTEREV_CPRIIFRCV		7	/* CPRI信号受信通知(RE共通部 */
#define CRD_INTEREV_TIMEOUT		8
#define CRD_INTEREV_TIMEOUT_RSP	9
#define CRD_INTEREV_TIMEOUT_SND	10
#define CRD_INTEREV_CPRIDBGSTART	11
#define CRD_INTEREV_CPRIDBGSTOP	12
#define CRD_INTEREV_CPRIMONISTART	13
#define CRD_INTEREV_CPRIMONISTOP	14
#define CRD_INTEREV_CPRIDSCSTNTC	15
#define CRD_INTEREV_RE_CPRI_STOP    16

#define CRD_ECTBL_END		0xFFFFFFFF	  /*	event conversion table end	*/
/* @} */



/** @name error code  define for TRA negative setting request */
/* @{ */

/****************************************************************************/
/* error code  define for TRA negative setting request			*/
/****************************************************************************/
#define CRD_NEGSET_ALREADYNEG	1  /* already TRA間negative設定 */
/* @} */


/** @name setting type define for state F control register */
/* @{ */

/****************************************************************************/ 
/* setting type define for state F control register 						*/
/****************************************************************************/
#define CRD_STAFREG_0		0			/*	0h: stateF->stateE*/
#define CRD_STAFREG_1		1			/*	0h: stateE->stateF*/
/* @} */

/** @name setting bit object define for CPRI link error control register */
/* @{ */

/****************************************************************************/ 
/* setting bit object define for CPRI link error control register			*/
/****************************************************************************/
#define CRD_LINKREG_BITL2		1			/*	L2 bit of error control register */
#define CRD_LINKREG_BITL3_3G	2		/*	3G L3 bit of error control register */
#define CRD_LINKREG_BITL3_S3G    3		/* S3G L3 bit of error control register*/
/* @} */

/** @name setting type define for CPRI link error control register */
/* @{ */

/****************************************************************************/ 
/* setting type define for CPRI link error control register					*/
/****************************************************************************/
#define CRD_LINKREG_ERROFF		0			/*	error off */
#define CRD_LINKREG_ERRON		1			/*	error on */
/* @} */

/** @name link state related event define for CPRI link state priority check  */
/* @{ */

/****************************************************************************/ 
/* link state related event define for CPRI link state priority check 		*/
/****************************************************************************/
#define CRD_LINKEV_STAEDWNTOE	0   /* CPRI state change (state E down -> state E) */
#define CRD_LINKEV_HDLCACT		1	/* CPRI state change (HDLC ACT detection)   */
#define CRD_LINKEV_STAEDWN		2   /* CPRI state change (state E over -> state E down)  */
#define CRD_LINKEV_HDLCIDLE		3	/* CPRI state change (HDLC IDLE detection) */
#define CRD_LINKEV_TXABNM		4	/* TX abnormal  */
#define CRD_LINKEV_L2ESTIND		5	/* L2 data link establishment indication  */
#define CRD_LINKEV_L2RELIND		6	/* L2 data link release indication */
#define CRD_LINKEV_L3HCOK		7	/* L3 health check OK	 */
#define CRD_LINKEV_L3HCDSC	   	8	/* L3 health check disconnection */

#define CRD_MX_LINKEV       9   /* max link related event */          
/* @} */

/** @name priority define for CPRI link state priority check   */
/* @{ */

/****************************************************************************/
/* priority define for CPRI link state priority check 							*/
/****************************************************************************/
#define CRD_STAPRIO_LOW		0  /* low priority	 */
#define CRD_STAPRIO_SAME		1  /* same priority	 */
#define CRD_STAPRIO_HIGH		2  /* high priority	 */
#define CRD_STAPRIO_HIGH_FROM_F   3 /*!< high priority, and current state is state F */
#define CRD_STAPRIO_HIGH_FROM_L3OK  4 /*!< high priority, and current state is L3 OK */
/* @} */

/** @name field define for RE common part info common management table   */
/* @{ */

/* ======================================================================== */
/* field define for RE共通部情報共通管理テーブル                          */
/* ======================================================================== */

/****************************************************************************/
/* RE共通部設定要因 define									*/
/****************************************************************************/
#define CRD_RECLS_TRIG_OFF	        0	/* no operation  */
#define CRD_RECLS_TRIG_RECACTCNT	1	/* REC主導Active制御  */
#define CRD_RECLS_TRIG_RECNEGCNT	2	/* REC主導Negative制御  */
#define CRD_RECLS_TRIG_L1DSC	    3	/* L1 断  */
#define CRD_RECLS_TRIG_TXABNM	    4	/* TX送信異常  */
#define CRD_RECLS_TRIG_HDLCIDLE     5	/* HDLC IDE  */
#define CRD_RECLS_TRIG_L2DSC	    6	/* L2 断 */
#define CRD_RECLS_TRIG_L3DSC	    7	/* ヘルスチェック断  */
#define CRD_RECLS_TRIG_MDIFBLK	    8	/* MDIF card block  */
#define CRD_RECLS_TRIG_MDIFINSALM	9	/* 実装+ALM */
#define CRD_RECLS_TRIG_TPACTCNT		CMD_RECLS_TRIG_TPACTCNT	/*	10:TPコマンドによるActive設定	*/
#define CRD_RECLS_TRIG_TPNEGCNT		CMD_RECLS_TRIG_TPNEGCNT	/*	11:TPコマンドによるNegative設定	*/

/****************************************************************************/
/* RE共通部設定 sequence define									*/
/****************************************************************************/
#define CRD_RECLS_SEQ_OFF	                        0	 /* no operation  */
#define CRD_RECLS_SEQ_MDIFACTSETRUN		1	/* MDIF間active設定進行中 */
#define CRD_RECLS_SEQ_MDIFNEGSETRUN		2	/* MDIF間negative設定進行中 */
#define CRD_RECLS_SEQ_TPACTSETRUN	CMD_RECLS_SEQ_TPACTSETRUN	/* 3:TPコマンドactive設定進行中		*/
#define CRD_RECLS_SEQ_TPNEGSETRUN	CMD_RECLS_SEQ_TPNEGSETRUN	/* 4:TPコマンドnegative設定進行中	*/

/****************************************************************************/
/* 走行履歴情報テーブル用													*/
/****************************************************************************/
#define CRD_RUNHIS_FACENUM_MAX	128	/* 面数MAX */
#define CRD_RUNHIS_FLG_RCV		0x00	/* 送受信フラグ(受信)   */
#define CRD_RUNHIS_FLG_SND		0x01	/* 送受信フラグ(送信)   */
#define CRD_RUNHIS_FLG_SND_NG	0xff	/* 送受信フラグ(送信NG) */
/* @} */

/****************************************************************************/
/* 3G/S3G ACT/NEG register mask */
/****************************************************************************/
#define CRD_MSK_ACTNEG			0x003f		/* ACT/NEG MASK */
/* @} */

/* ======================================================================== */
/* RE状態定義定数															*/
/* ======================================================================== */
#define CRD_RESTATE_IDLE		0			/* REアイドル状態				*/
#define CRD_RESTATE_OPERATE		1			/* RE運用中状態					*/

/* ======================================================================== */
/* RE ２次リセット定義定数															*/
/* ======================================================================== */

/* L1 inband reset wait 1sec */
#define D_CPR_L1INBAND_RESET_WAIT			1000
/* L1 inband reset retry counter */
#define D_CPR_L1INBAND_RETRY_MAX			3

/* ======================================================================== */
/* REC状態定義定数															*/
/* ======================================================================== */
#define CRD_RECSTATE_STOP			0			/* ヘルスチェック停止中			*/
#define CRD_RECSTATE_START			1			/* ヘルスチェック起動中			*/

#define CRD_RECSTATE_INI			0			/* ヘルスチェック未確立(初期値)	*/
#define CRD_RECSTATE_DISCONNT		1			/* ヘルスチェック断				*/
#define CRD_RECSTATE_EST			2			/* ヘルスチェック確立			*/

#define CRD_RECSTATE_NOSEND		0			/* ヘルスチェック要求未送信		*/
#define CRD_RECSTATE_SENDING		1			/* ヘルスチェック要求送信中		*/


#define D_TCM_SYSDFT_HCRSP_UNCNFNUM	5	 	/* Health check response unconfirm number (unit:1 ) */
#define D_TCM_SYSDFT_HCRSP_CNFTIM		20  	/* Health check response confirm timer (unit:10ms) */
#define D_TCM_SYSDFT_HCRSP_ITVTIM		100		/* Health check response interval timer (unit:10ms) */
#define D_TCM_SYSDFT_HCRSP_ITVTIM_MIN   60      /* ヘルスチェック応答待ちタイマ最小値 (unit:10ms) */
#define D_TCM_SYSDFT_HCRSP_ITVTIM_MINUS 20      /* ヘルスチェック応答待ちタイマ 変数Dからの減算量 */


/* @} */
/** @name CPRI link state number define */
/* @{ */

/* CPRI link state number define											*/
#define CRD_CPRILNKSTA_NO0		0			/**< CPRI stateE donw */
											/**< HDLC idle */
											/**< none-stateF */
											/**< TX abnormal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO1		1			/**< CPRI stateE */
											/**< HDLC idle */
											/**< none-stateF */
											/**< TX abnormal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO2		2			/**< CPRI stateE */
											/**< HDLC active */
											/**< none-stateF */
											/**< TX normal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO3		3			/**< CPRI stateE */
											/**< HDLC active */
											/**< none-stateF */
											/**< TX abnormal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO4		4			/**< CPRI stateE */
											/**< HDLC active */
											/**< stateF */
											/**< TX normal */
											/**< L3 disconnection */

#define CRD_CPRILNKSTA_NO5		5			/**< CPRI stateE */
											/**< HDLC active */
											/**< stateF */
											/**< TX normal */
											/**< L3 OK */
/* @} */

#endif/*M_CR_DEF*/ 


/*!
 * @skip   $Id$
 * @file   m_dl_init.c
 * @brief  the processing for initial request.
 * @date   2008/07/15 FFCS)Wuh Create for eNB-008-001.
 * @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *                          stop writing FLASH after receiving CPRI message
 * @date  2009/04/08  FFCS)Wuh
 *                      add a new BTI setting flag to judge whether need do BTI setting
 * @date  2009/04/13  FFCS)Wuh modify for S3G PRC CR-xxx-xxx (CPRI spec V1.07)
 * @date   2009/06/09  FFCS)Wuh modify for M-S3G-eNBPF-02047
 *                      RE version response incorrect
 * @date   2009/07/20 FFCS)Tangj modify for S3G PRC 7th release function system parameter mechanism change
 *                    delete dlw_sysbtiset_flg
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2009
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"			/* MDIF common head file				*/
#include "m_dl_header.h"			/* download management task head file  */

/*!
 *  @brief  the processing function for initial request.
 *  @note   This function is processed as follows.
 *          1) Initial the tables
 *          2) Send the response for task initial request 
 *  @param  bufp        [in]    pointer of received message
 *  @param  cpri_no     [in]    CPRI link number
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01841 
 *  @date   2008/07/15 FFCS)Wuh Create.
 *  @date   2015/08/31 FPT)DuongCD update for fhm.
 *  @date   2015/09/17 FPT)Yen M-RRU-ZSYS-01841 update メーカ識別子 -> 88. 変数初期化修正
 */

VOID    m_dl_init(VOID* bufp, USHORT cpri_no)
{

    USHORT	a_3g		= CMD_SYS_3G;       /*-----for 3G-----*/
	USHORT	a_s3g		= CMD_SYS_S3G;       /*-----for S3G-----*/
	CMT_TSKIF_HEAD 		*head_p;
	INT					buf_ret;
	USHORT				lwLoop;
		
	/**********************/
	/* Initial the tables */
	/**********************/
	/* initialize the object data area ,initial download ending processing,called table */
	memset((CHAR*)cmw_file_inf_tbl, 0, sizeof(cmw_file_inf_tbl));

	memset(&dlw_runhis_tbl,0,sizeof(dlw_runhis_tbl));
	
	/* For FHM */
	/* DLM管理テーブル */
	memset(&gt_dlw_dlm_tbl, 0, sizeof(gt_dlw_dlm_tbl));
	/*DL中REファイル情報: 0xFFFF(DL中ファイル無) */
	gt_dlw_dlm_tbl.dlrefile_inf = 0xFFFF;
	/* メーカ識別子: 88("X")固定 */
	gt_dlw_dlm_tbl.mk_jdg = 88;
	
	/* 対REC情報テーブル */
	memset(&gt_dlw_rec_tbl, 0, sizeof(gt_dlw_rec_tbl));
	/* 対REC状態: アイドル */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;
	gt_dlw_rec_3g_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;
	
	/* 対RE-FW情報テーブル */
	memset((CHAR*)gt_dlw_refw_tbl, 0, sizeof(gt_dlw_refw_tbl));
	/* 対RE-FW情報テーブル(3G) */
	memset((CHAR*)gt_dlw_refw_3g_tbl, 0, sizeof(gt_dlw_refw_3g_tbl));
	
	for(lwLoop = CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX ; lwLoop ++)
	{
		
		/* 対RE状態:アイドル */
		gt_dlw_refw_tbl[lwLoop].re_sta = DLD_RESTA_IDLE;
		gt_dlw_refw_3g_tbl[lwLoop].re_sta = DLD_RESTA_IDLE;
		
		/* FHM保持REファイル情報: 0xFFFF(未取得) */
		gt_dlw_refw_tbl[lwLoop].fhmrefile_inf = 0xFFFF;
		gt_dlw_refw_3g_tbl[lwLoop].fhmrefile_inf = 0xFFFF;
		
	}
	
	/* FHM保持REファイル情報 */
	memset((CHAR*)gt_dlw_mnt_reinf, 0, sizeof(gt_dlw_mnt_reinf));
	/* FHM保持REファイル情報(3G) */
	memset((CHAR*)gt_dlw_mnt_3g_reinf, 0, sizeof(gt_dlw_mnt_3g_reinf));
	
	cm_MemPads( &cmw_hwsetsys_tbl.cpri_sttimer, 
				CMD_IVL_VAL, 
				sizeof( CMT_HWSETSYSTBL ) / sizeof( USHORT ));

	/* set the first startup and use download flag OFF */
	dlw_1stdlm_flg = CMD_OFF;

	m_dl_cm_DlEnd(a_3g);

	m_dl_cm_DlEnd(a_s3g);

	dlw_syscolcmp_flg = CMD_OFF;

	dlw_abdata_flg[CMD_SYS_3G] = CMD_OFF;
	dlw_abdata_flg[CMD_SYS_S3G] = CMD_OFF;
	
	/* clear primary system kind table */
		/* ZYNQ RREの場合 */
	dlw_prisys_tbl = CMD_SYS_S3G;
	
	/* set the download type no download */
	dlw_downtyp_tbl = DLD_TSK_TYP_NONE;

	/* set the download handle */
	dlw_handle = CMD_NUM0;

	/* set the flash state flag */
	gt_dlw_dlm_tbl.write_flg = CMD_NUM0;

	/* Init systerm param */
	m_dl_init_sysp();

	/*************************************/
	/* the processing of response sending*/
	/*************************************/
	/* the sending for initial response*/
	/* バッファ取得 */
	if( cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_HEAD ), (VOID**)&head_p ) != CMD_RES_OK )
	{
		/* バッファ取得失敗時はアボート処理 */
		cm_MAbort( CMD_ALMCD_BUFGET, "m_dl_init", 
							   "cm_BReq NG", 
							   CMD_NG, sizeof( CMT_TSKIF_HEAD ), CMD_NUM0 );
	}
	
	/* send 初期化応答 to 初期化タスク */
	head_p->uiLength = sizeof(CMT_TSKIF_HEAD);
	head_p->uiEventNo = CMD_TSKIF_INIRSP;
	head_p->uiSrcTQueueID = CMD_TSKID_DLM;
	head_p->uiDstTQueueID = CMD_TSKID_INI;

	buf_ret = cm_Enter( CMD_TSKID_INI, CMD_QRB_NORMAL, CMD_TSKIF_INIRSP, head_p );
	if(buf_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_USELOW, buf_ret, "cm_Enter NG" );
	}
	
	return ;
}

/*!
 *  @brief  Init systerm param value.
 *  @note   
 *  @param  - [in]  -
 *  @param  - [out] -
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01699
 *  @date   2015/09/10 FPT)Yen create
 *  @date   2015/09/26 TDIPS)sasaki M-RRU-ZSYS-01699 Update MKレビュー指摘No.200対応
 *  @date   2015/12/04 TDIPS)sasaki Update IT3問処No.127対処
 *  @date	2016/03/25 TDI)satou M-RRU-ZSYS-02090 cmw_sys_mng_tblの初期化処理をコメント化
 */

VOID    m_dl_init_sysp()
{
	USHORT			lwSysType;
	USHORT			lwCnt;
	USHORT*			p_sysp = NULL;
	USHORT			lwSyspSize[CMD_MAX_SYSNUM] = {DLD_SYSPAR3G_SIZE, DLD_SYSPARS3G_SIZE};
	UCHAR*			dlw_rcvsyspa_tbl[CMD_MAX_SYSNUM] = {dlw_rcvsyspa_tbl_3g, dlw_rcvsyspa_tbl_s3g};

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_init_sysp] ENTER" );

	for( lwSysType = CMD_NUM0; lwSysType < CMD_MAX_SYSNUM; lwSysType++ )
	{	/* L3プロセス起動時に、l3_main_Init()においてINITプロセスが
		 * 共有メモリに展開したシステムパラメータの内容をcmw_sys_mng_tblに展開済みである.
		 * cmw_sys_mng_tblを改めて初期化するのは冗長であるため処理をコメント化. */
#if 0
		/* CPRI State安定化時間（RE）*/
		cmw_sys_mng_tbl[lwSysType].cpri_sttimer = 20;
		/* ヘルスチェック要求確認タイマ（RE）*/
		cmw_sys_mng_tbl[lwSysType].hls_timer = 120;
		/* ヘルスチェック要求未確認保護段数（RE）*/
		cmw_sys_mng_tbl[lwSysType].hls_unconfcnt = 5;
		/* システムパラメータ送信確認タイマ */
		cmw_sys_mng_tbl[lwSysType].sys_timer = 300;
		/* ファームウェア送信確認タイマ */
		cmw_sys_mng_tbl[lwSysType].firm_timer = 28000;
		/* セクタnチップ補正(0系) */
		cmw_sys_mng_tbl[lwSysType].sec_chip0 = 0;
		/* セクタnチップ補正(1系) */
		cmw_sys_mng_tbl[lwSysType].sec_chip1 = 0;
		/* 外部装置データ受信確認タイマ */
		cmw_sys_mng_tbl[lwSysType].oudevt_rcvtimer = 150;
		/* FLD-MT接続確認タイマ */
		cmw_sys_mng_tbl[lwSysType].fldmt_cttimer = 150;
		/* FLD-MT接続解放確認タイマ */
		cmw_sys_mng_tbl[lwSysType].fldmt_rltimer = 150;
		/* FLD-MTデータコマンド受信確認タイマ */
		cmw_sys_mng_tbl[lwSysType].fldmt_dtcd_timer = 150;
		/* FLD-MTデータファイル受信確認タイマ */
		cmw_sys_mng_tbl[lwSysType].fldmt_dtfl_timer = 150;
		/* SV-MT/eNB-MT接続確認タイマ */
		cmw_sys_mng_tbl[lwSysType].svmt_cttimer = 150;
		/* SV-MT/eNB-MT接続解放確認タイマ */
		cmw_sys_mng_tbl[lwSysType].svmt_rltimer = 150;
		/* SV-MT/eNB-MTデータ受信確認タイマ */
		cmw_sys_mng_tbl[lwSysType].svmt_rcvtimer = 150;
		/* 3G用送信ダイバーシチ設定 */
		cmw_sys_mng_tbl[lwSysType].tx_div_value_3G = 2;
		/* RE優先システム情報 */
		cmw_sys_mng_tbl[lwSysType].pri_sys_info = 1;
		/* LTE帯域幅 */
		cmw_sys_mng_tbl[lwSysType].s3g_bandwidth = 1;
		/* 3Gキャリア運用有無 */
		cmw_sys_mng_tbl[lwSysType].car_wk_flg = 1;
		/* 送信系統数 */
		cmw_sys_mng_tbl[lwSysType].tx_sys_num = 1;
		/* セクタnチップ補正(2、3、4系) */
		cmw_sys_mng_tbl[lwSysType].sec_chip234 = 0;
		/* セクタnチップ補正(5、6、7系)*/
		cmw_sys_mng_tbl[lwSysType].sec_chip567 = 0;
		/* reserve for system parameter E */
		cmw_sys_mng_tbl[lwSysType].reserveE[CMD_NUM0] = 65535;
		cmw_sys_mng_tbl[lwSysType].reserveE[CMD_NUM1] = 65535;
		/* 3Gキャリア#1運用周波数 */
		cmw_sys_mng_tbl[lwSysType].car1_wk_frq = 0;
		/* 3Gキャリア#2運用周波数 */
		cmw_sys_mng_tbl[lwSysType].car2_wk_frq = 0;
		/* 3Gキャリア#3運用周波数 */
		cmw_sys_mng_tbl[lwSysType].car3_wk_frq = 0;
		/* 3Gキャリア#4運用周波数 */
		cmw_sys_mng_tbl[lwSysType].car4_wk_frq = 0;
		/* 対3G用MT信号/外部装置データ対応有無 */
		cmw_sys_mng_tbl[lwSysType].mtExt_data_3g_flg = 1;
		/* システムパラメータ転送待ちタイマ */
		cmw_sys_mng_tbl[lwSysType].syspara_wait_timer = 6000;
		/* ファームウェア送信確認タイマ（更新用） */
		cmw_sys_mng_tbl[lwSysType].firm_sndtimer = 56000;
		/* reserve for system parameter F */
		cmw_sys_mng_tbl[lwSysType].reserveF = 65535;
#endif

		/* 配下REへの転送用シスパラ領域を暫定値で初期化 */
		memcpy(dlw_rcvsyspa_tbl[lwSysType], &cmw_sys_mng_tbl[lwSysType], lwSyspSize[lwSysType]);
		for(lwCnt = 0, p_sysp = (USHORT *)dlw_rcvsyspa_tbl[lwSysType]; lwCnt < (lwSyspSize[lwSysType]/sizeof(USHORT)); lwCnt++, p_sysp++)
		{
			*p_sysp = htons(*p_sysp);
		}
	}

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_init_sysp] EXIT" );
}
/* @} */


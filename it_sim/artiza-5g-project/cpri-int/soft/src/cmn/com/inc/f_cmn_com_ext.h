/*!
 * @skip  $ld:$
 * @file  f_cmn_com_ext.h
 * @brief 装置共通外部参照定義ヘッダ
 * @date  2013/11/26 ALPHA) 近藤 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */
#ifndef F_CMN_COM_EXT_H
#define F_CMN_COM_EXT_H

/****************************************/
/* common function						*/
/****************************************/
extern VOID f_cmn_com_abort(UINT processID, UINT almCode, const VOID * fileName, UINT lineNo );
extern VOID f_cmn_com_assert(UINT	logLevel , UINT code, const VOID *access , const VOID* fileName, UINT lineNo );
#define f_com_assert( code , access ) \
  do { \
	f_cmn_com_assert( D_RRH_LOG_AST_LV_INFO , code , access , __FILE__ , __LINE__ );\
	BPF_COM_PLOG_TRACE_PROC( D_RRH_LOG_AST_LV_INFO , "0x%08x,%s", code, access);\
  } while(0)

extern void* f_cmn_com_assert_th_create( unsigned int logLevel , char* name_p );
extern void f_cmn_com_assert_th_destroy( void );			/* for debug */
extern void f_cmn_com_assert_th_start( void* buf_p );	/* for debug */
extern void f_cmn_com_assert_th_stop( void* buf_p );	/* for debug */
extern void f_cmn_com_assert_th_restart(unsigned int logLevel , void* buf_p );	/* for debug */
extern FILE* f_cmn_com_assert_th_print( FILE* fp , void* buf_p );		/* ログ取得時のスレッド単体用 */
extern FILE* f_cmn_com_assert_th_print_all( FILE* fp );					/* ログ取得時のプロセス内全スレッド用 */		
extern void f_cmn_com_assert_th_get(void* buf_p, unsigned int logLevel , const char* funcname ,  const char* format, ...);

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern INT f_cmn_com_alm_abc_get(USHORT lte3g, T_RRH_ALM_ABC *pOutData);				/* 共有メモリALM_ABCの呼び出し 取得用 */
extern INT f_cmn_com_alm_abc_set(USHORT lte3g, T_RRH_ALM_ABC *pInData);					/* 設定用 */

extern INT f_cmn_com_cardst_3g_get(USHORT linkno, T_RRH_CARDST_3G *pOutData);			/* 共用メモリ(3G)REカード状態の呼び出し 取得用 */
extern INT f_cmn_com_cardst_3g_set(USHORT linkno, T_RRH_CARDST_3G *pInData);			/* 設定用 */

extern INT f_cmn_com_cardst_s3g_get(USHORT linkno, T_RRH_CARDST_S3G *pOutData);			/* 共用メモリ(LTE)REカード状態の呼び出し 取得用 */
extern INT f_cmn_com_cardst_s3g_set(USHORT linkno, T_RRH_CARDST_S3G *pInData);			/* 設定用 */

extern INT f_cmn_com_carst_3g_get(USHORT linkno, T_RRH_CARST_3G *pOutData);				/* 共有メモリ(3G)キャリア状態の呼び出し 取得用 */
extern INT f_cmn_com_carst_3g_set(USHORT linkno, T_RRH_CARST_3G *pInData);				/* 設定用 */

extern INT f_cmn_com_carst_s3g_get(USHORT linkno, T_RRH_CARST_S3G *pOutData);			/* 共有メモリ(LTE)キャリア状態の呼び出し 取得用 */
extern INT f_cmn_com_carst_s3g_set(USHORT linkno, T_RRH_CARST_S3G *pInData);			/* 設定用 */

extern INT f_cmn_com_cpri_state_get(T_RRH_CPRI_STATE *pOutData);						/* 共用メモリCPRI状態の呼び出し 取得用 */
extern INT f_cmn_com_cpri_state_set(T_RRH_CPRI_STATE *pInData);							/* 設定用 */
extern INT f_cmn_com_cpri_state_set_cprist(USHORT link_num, USHORT cpriState, USHORT cpriHdlc, USHORT lineBitrate, UINT cpri_linktype);
extern INT f_cmn_com_cpri_state_set_hc(USHORT link_num, USHORT link_type, USHORT hcst);

extern INT f_cmn_com_delay_inf_get(T_RRH_DELAY_INF *pOutData);							/* 共有メモリ遅延補正情報の呼び出し 取得用 */
extern INT f_cmn_com_delay_inf_set(T_RRH_DELAY_INF *pInData);							/* 設定用 */

extern INT f_cmn_com_eqp_3g_get(USHORT linkno, T_RRH_EQP_3G *pOutData);					/* 共有メモリ(3G)装置構成情報の呼び出し 取得用 */
extern INT f_cmn_com_eqp_3g_set(USHORT linkno, T_RRH_EQP_3G *pInData);					/* 設定用 */

extern INT f_cmn_com_eqp_s3g_get(USHORT linkno, T_RRH_EQP_S3G *pOutData);				/* 共有メモリ(LTE)装置構成情報の呼び出し 取得用 */
extern INT f_cmn_com_eqp_s3g_set(USHORT linkno, T_RRH_EQP_S3G *pInData);				/* 設定用 */

extern INT f_cmn_com_fhm_state_get(T_RRH_FHM_STATE *pOutData);							/* 共用メモリFHM状態の呼び出し 取得用 */
extern INT f_cmn_com_fhm_state_set(T_RRH_FHM_STATE *pInData);							/* 設定用 */

extern INT f_cmn_com_fw_info_fhm_get(USHORT side, T_RRH_FW_INFO *pOutData);				/* 共有メモリFHM保持FWファイル情報(0,1面)の呼び出し 取得用*/
extern INT f_cmn_com_fw_info_fhm_set(USHORT side, T_RRH_FW_INFO *pInData);				/* 設定用 */

extern INT f_cmn_com_fw_info_re_get(USHORT side, T_RRH_FW_INFO *pOutData);              /* 共用メモリFHM保持FWファイル情報(RE-FW:0～15面)の呼び出し 取得用 */
extern INT f_cmn_com_fw_info_re_set(USHORT side, T_RRH_FW_INFO *pInData);				/* 設定用 */

extern INT f_cmn_com_layer3_get(USHORT lte3g, T_RRH_LAYER3 *pOutData);					/* 共用メモリLayer3の呼び出し 取得用 */
extern INT f_cmn_com_layer3_set(USHORT lte3g, T_RRH_LAYER3 *pInData);					/* 設定用 */
extern INT f_cmn_com_layer3_set_each(USHORT link_num, USHORT lte3g, USHORT layer3);

extern INT f_cmn_com_mtaddset_get(USHORT lte3g, USHORT mt, T_RRH_MTADDSET *pOutData);	/* 共有メモリMTアドレス設定の呼び出し 取得用 */
extern INT f_cmn_com_mtaddset_set(USHORT lte3g, USHORT mt, T_RRH_MTADDSET *pInData);	/* 設定用 */

extern INT f_cmn_com_re_delay_inf_get(USHORT linkno, T_RRH_RE_DELAY_INF *pOutData);		/* 共有メモリRE単位遅延補正情報の呼び出し 取得用 */
extern INT f_cmn_com_re_delay_inf_set(USHORT linkno, T_RRH_RE_DELAY_INF *pInData);		/* 設定用 */

extern INT f_cmn_com_rest_3g_get(USHORT linkno, T_RRH_REST_3G *pOutData);				/* 共用メモリ(3G)RE状態の呼び出し 取得用 */
extern INT f_cmn_com_rest_3g_set(USHORT linkno, T_RRH_REST_3G *pInData);				/* 設定用 */

extern INT f_cmn_com_rest_s3g_get(USHORT linkno, T_RRH_REST_S3G *pOutData);				/* 共有メモリ(LTE)RE状態の呼び出し 取得用 */
extern INT f_cmn_com_rest_s3g_set(USHORT linkno, T_RRH_REST_S3G *pInData);				/* 設定用 */

extern INT f_cmn_com_side_use_get(T_RRH_SIDE_USE *pOutData);							/* 共有メモリFHM保持FWファイル運用面の呼び出し 取得用 */
extern INT f_cmn_com_side_use_set(T_RRH_SIDE_USE *pInData);								/* 設定用 */

extern INT f_cmn_com_slot_3g_get(USHORT linkno, T_RRH_SLOT_3G_RE *pOutData);			/* 共有メモリ(3G)スロット情報の呼び出し 取得用 */
extern INT f_cmn_com_slot_3g_set(USHORT linkno, T_RRH_SLOT_3G_RE *pInData);				/* 設定用 */

extern INT f_cmn_com_slot_s3g_get(USHORT linkno, T_RRH_SLOT_S3G_RE *pOutData);			/* 共用メモリ(LTE)スロット情報の呼び出し 取得用 */
extern INT f_cmn_com_slot_s3g_set(USHORT linkno, T_RRH_SLOT_S3G_RE *pInData);			/* 設定用 */

extern INT f_cmn_com_syspara_3g_get(T_RRH_SYSPAR_3G *pOutData);							/* 共用メモリシスパラ格納領域(3G)の呼び出し 取得用 */
extern INT f_cmn_com_syspara_3g_set(T_RRH_SYSPAR_3G *pInData);							/* 設定用 */

extern INT f_cmn_com_syspara_lte_get(T_RRH_SYSPAR_LTE *pOutData);						/* 共用メモリシスパラ格納領域(LTE)の呼び出し 取得用 */
extern INT f_cmn_com_syspara_lte_set(T_RRH_SYSPAR_LTE *pInData);						/* 設定用 */

extern INT f_cmn_com_toffset_3g_get(USHORT linkno, T_RRH_TOFFSET_3G *pOutData);			/* 共用メモリ(3G)Toffset値報告の呼び出し 取得用 */
extern INT f_cmn_com_toffset_3g_set(USHORT linkno, T_RRH_TOFFSET_3G *pInData);			/* 設定用 */

extern INT f_cmn_com_toffset_s3g_get(USHORT linkno, T_RRH_TOFFSET_S3G *pOutData);		/* 共有メモリ(LTE)Toffset値報告の呼び出し 取得用 */
extern INT f_cmn_com_toffset_s3g_set(USHORT linkno, T_RRH_TOFFSET_S3G *pInData);		/* 設定用 */

extern INT f_cmn_com_trxset_para_3g_get(USHORT carrier, T_TRXSET_PARA_3G *pOutData);	/* 共有メモリ(3G)TRX設定パラメータテーブルの呼び出し 取得用 */
extern INT f_cmn_com_trxset_para_3g_set(USHORT carrier, T_TRXSET_PARA_3G *pInData);		/* 設定用 */

extern INT f_cmn_com_trxset_para_lte_get(USHORT carrier, T_TRXSET_PARA_LTE *pOutData);	/* 共有メモリ(LTE)TRX設定パラメータテーブルの呼び出し 取得用 */
extern INT f_cmn_com_trxset_para_lte_set(USHORT carrier, T_TRXSET_PARA_LTE *pInData);	/* 設定用 */

extern INT f_cmn_com_tx_onoff_get(T_RRH_TX_ONOFF *pOutData);				/* 共用メモリ無線送信ONOFF状態の呼び出し取得用 */
extern INT f_cmn_com_tx_onoff_set(T_RRH_TX_ONOFF *pInData);				    /* 設定用 */

extern INT f_cmn_com_conn_disk_get(T_RRH_CONN_DISC *pOutData);			                /* 共用メモリRE接続有無状態の呼び出し取得用  */
extern INT f_cmn_com_conn_disk_set(T_RRH_CONN_DISC *pInData);				            /* 設定用 */

extern INT f_cmn_com_bootslot_s3g_get(T_RRH_SLOT_S3G *pOutData);
extern INT f_cmn_com_bootslot_s3g_set(T_RRH_SLOT_S3G *pInData);
extern INT f_cmn_com_bootslot_3g_get(T_RRH_SLOT_3G *pOutData);
extern INT f_cmn_com_bootslot_3g_set(T_RRH_SLOT_3G *pInData);

extern INT f_cmn_com_booteqp_s3g_get(T_RRH_EQP_S3G *pOutData);
extern INT f_cmn_com_booteqp_s3g_set(T_RRH_EQP_S3G *pInData);
extern INT f_cmn_com_booteqp_3g_get(T_RRH_EQP_3G *pOutData);
extern INT f_cmn_com_booteqp_3g_set(T_RRH_EQP_3G *pInData);
 	
extern INT f_cmn_com_portInputSetting_s3g(T_RRH_REPORTSTA_S3G *setting);
extern INT f_cmn_com_RePortSta_s3g_set(USHORT linkno, UCHAR received_rsp, USHORT rsp_result, USHORT rsp_portin);
extern INT f_cmn_com_RePortSta_s3g_allget(T_RRH_REPORTSTA_S3G *pOutData);

extern INT f_cmn_com_shm_get(UINT shm_num, UINT sem_num, UINT size, VOID *pOutData);
extern INT f_cmn_com_shm_get_offset(UINT shm_num, UINT sem_num, UINT size, UINT offset, VOID *pOutData);
extern INT f_cmn_com_shm_set(UINT shm_num, UINT sem_num, UINT size, VOID *pInData);
extern INT f_cmn_com_shm_set_offset(UINT shm_num, UINT sem_num, UINT size, UINT offset, VOID *pInData);

extern INT f_cmn_com_cardstslt_3g_get(USHORT linkno, USHORT slt_index, T_RRH_CARDST_3G *pOutData);		/* 共用メモリ(3G)REカード状態(Slot単位)の呼び出し 取得用 16B KCN add */
extern INT f_cmn_com_cardstslt_3g_set(USHORT linkno, USHORT slt_index, T_RRH_CARDST_3G *pInData);		/* 設定用 */

extern INT f_cmn_com_cardstslt_s3g_get(USHORT linkno, USHORT slt_index, T_RRH_CARDST_S3G *pOutData);	/* 共用メモリ(LTE)REカード状態の呼び出し(Slot単位) 取得用 16B KCN add */
extern INT f_cmn_com_cardstslt_s3g_set(USHORT linkno, USHORT slt_index, T_RRH_CARDST_S3G *pInData);		/* 設定用 */

extern INT f_cmn_com_slotsrch_3g(USHORT linkno, USHORT slotno, USHORT *slot_index);		/* 共有メモリ(3G)スロット情報のスロット番号から何番スロットか取得用 16B KCN add */
extern INT f_cmn_com_slotsrch_s3g(USHORT linkno, USHORT slotno, USHORT *slot_index);	/* 共用メモリ(LTE)スロット情報のスロット番号から何番スロットか取得用 16B KCN add */

extern INT f_cmn_com_print_offon_get(UCHAR *pOutData);			/* デバッグ用printf文のOff/On状態取得 16B KCN add */
extern INT f_cmn_com_print_offon_set(UCHAR *pInData);			/* デバッグ用printf文のOff/On状態設定用 16B KCN add */

extern VOID f_cmn_com_dbg_printf(CHAR *pOutData);				/* デバッグ用printf文 16B KCN add */

/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
// 擬似環境 20210316
//#ifdef SIM_DEBUG
//#define cmn_com_assert_th_get(buf_p,logLevel,...) 0 //C9 add code
//#else
#define cmn_com_assert_th_get(buf_p,logLevel,...) \
  do { \
	if((logLevel & ( D_RRH_LOG_AST_LV_WARNING | D_RRH_LOG_AST_LV_ERROR | D_RRH_LOG_AST_LV_CRITICAL)) != 0){ \
		bpf_com_log_assert_real( logLevel,  __FILE__, __LINE__, __VA_ARGS__ ); \
	} else{ \
		f_cmn_com_assert_th_get(buf_p,logLevel , __func__ , __VA_ARGS__ ); \
	} \
	BPF_COM_PLOG_TRACE_PROC(logLevel,__VA_ARGS__); \
  } while(0)
//#endif
// 擬似環境 20210316


extern VOID f_cmn_com_log_threadCreate( CHAR * procName, VOID (*logCreateFunc)(FILE *), UINT msgQid );
extern VOID f_cmn_com_log_create_send( UINT logKind, UINT msgQid );
extern VOID f_cmn_com_log_main( T_CMN_LOG_SAVE_INFO * logInfo );

/****************************************/
/* rom table							*/
/****************************************/
extern T_CMN_ALMLOG_FUNCTION_LIST f_cmnw_almlogfunc;
extern T_CMN_SV_FUNCTION_LIST     f_cmnw_svfunc;

#endif
/* @} */

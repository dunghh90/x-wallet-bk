/* RE-FW information table (REFW)
 * share & access
 */
#ifndef		M_DL_RAM_H
#define		M_DL_RAM_H

/*------- (1) REFW const define ---------*/
/*対RE-FW情報テーブル                                                                                                 */
/*RE FW info table                                                                                                 */
/*CPRI#											;0～16															;0:無効        */
/*対RE状態										; ファイル更新用 対RE状態一覧 参照								;0:アイドル      */
/*更新開始要因									;0:未実行,1:RE起動(REC未接続),2:RE起動(REC接続済),3:REC主導		;0:未実行      */
/*メーカ識別子									; 0,65～90														;0             */
/*メジャーバージョン(運用中)					;0～99															;0             */
/*マイナーバージョン(運用中)					;0～99															;0             */
/*メジャーバージョン(起動用)					;0～99															;0             */
/*マイナーバージョン(起動用)					;0～99															;0             */
/*使用中タイマ									;0:使用タイマなし												;0:使用タイマなし */
/*												;1:ファームウェアファイル報告確認タイマ							;              */
/*												;2:ファームウェアファイル情報報告確認タイマ						;              */
/*												;3:ファームウェア送信完了確認タイマ								;              */
/*												;11:ファームウェアファイル報告確認タイマ(更新用)				;              */
/*												;12:ファームウェアファイル情報報告確認タイマ(更新用)			;              */
/*												;13:ファームウェア送信完了確認タイマ(更新用)					;              */
/*FHM保持REファイル情報							; 0xFFFF:初期値,0～16:FHM保持REファイル情報配列INDEX			;0xFFFF:未取得 */
/*分割数(=ファイルサイズ/10240)					;0～1639														;0             */
/*送信済み分割番号								; 0～16777216(=16MB)											;0             */
/*送信済みデータサイズ							; 0～16777216(=16MB)											;0             */
/*ファームウェアファイル報告結果				;0:未実行,1:正常,2:NG,3:タイムアウト							;0:未実行      */
/*ファイル情報報告結果							;0:未実行,1:正常,2:NG,3:タイムアウト							;0:未実行      */
/*ファイルデータ送信完了報告結果				;0:未実行,1:正常,2:NG,3:タイムアウト							;0:未実行      */
/*再送回数[ファームウェアファイル報告要求]		;0～3															;0             */
/*再送回数[ファイル情報報告通知]				;0～3															;0             */
/*再送回数[ファイルデータ送信完了報告通知]		;0～3															;0             */

/* --REFW RE status */
//ref: m_dl_def.h
/* --update start factor */
//ref: m_dl_def.h
/* --using time */
//ref: m_dl_def.h
/* --report result type & value */
#define		REPORT_RESULT_FW_FILE					0
#define		REPORT_RESULT_FILE_INFO					1
#define		REPORT_RESULT_FILE_DATA_SEND_COMPLETE	2
/* --values */
#define		REPORT_RESULT_NOT_EXEUCTE	0
#define		REPORT_RESULT_NORMAL		1
#define		REPORT_RESULT_NG			2
#define		REPORT_RESULT_TIMEOUT		3
/* --init */
VOID REFW_init(DLT_REFW_TBL* at_REFW_p);
/* --get set methods */
USHORT REFW_get_CPRI_no(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_RE_status(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_update_start_factor(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_maker_id(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_major_version_using(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_minor_version_using(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_major_version_startup(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_minor_version_startup(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_using_timer(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_FHM_holding_RE_file_info(DLT_REFW_TBL* at_REFW_p);
ULONG  REFW_get_transfer_complete_part_no(DLT_REFW_TBL* at_REFW_p);
ULONG  REFW_get_transfer_complete_data_size(DLT_REFW_TBL* at_REFW_p);
USHORT REFW_get_report_result(DLT_REFW_TBL* at_REFW_p, USHORT aw_type);
USHORT REFW_get_resend_counter(DLT_REFW_TBL* at_REFW_p, USHORT aw_type);
/* REFW set methods */
USHORT REFW_set_CPRI_no(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
USHORT REFW_set_RE_status(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
USHORT REFW_set_update_start_factor(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
USHORT REFW_set_maker_id(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
USHORT REFW_set_major_version_using(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
USHORT REFW_set_minor_version_using(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
USHORT REFW_set_major_version_startup(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
USHORT REFW_set_minor_version_startup(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
USHORT REFW_set_using_timer(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
USHORT REFW_set_FHM_holding_RE_file_info(DLT_REFW_TBL* at_REFW_p, USHORT aw_value);
ULONG  REFW_set_transfer_complete_part_no(DLT_REFW_TBL* at_REFW_p, ULONG aw_value);
ULONG  REFW_set_transfer_complete_data_size(DLT_REFW_TBL* at_REFW_p, ULONG aw_value);
USHORT REFW_set_report_result(DLT_REFW_TBL* at_REFW_p, USHORT aw_type, USHORT aw_value);
USHORT REFW_set_resend_counter(DLT_REFW_TBL* at_REFW_p, USHORT aw_type, USHORT aw_value);

/* ------(1) RE-FW information table (REFW)----*/
VOID   init_all_REFW();
VOID   init_REFW(USHORT aw_index);
USHORT get_CPRI_no(USHORT aw_index);
USHORT get_RE_status(USHORT aw_index);
USHORT get_update_start_factor(USHORT aw_index);
USHORT get_REFW_maker_id(USHORT aw_index);
USHORT get_REFW_major_version_using(USHORT aw_index);
USHORT get_REFW_minor_version_using(USHORT aw_index);
USHORT get_REFW_major_version_startup(USHORT aw_index);
USHORT get_REFW_minor_version_startup(USHORT aw_index);
USHORT get_REFW_using_timer(USHORT aw_index);
USHORT get_FHMRE_index(USHORT aw_index);
ULONG  get_transfer_complete_part_no(USHORT aw_index);
ULONG  get_transfer_complete_data_size(USHORT aw_index);
USHORT get_report_result(USHORT aw_index, USHORT aw_type);
USHORT get_resend_counter(USHORT aw_index, USHORT aw_type);

VOID set_CPRI_no(USHORT aw_index, USHORT aw_value);
VOID set_RE_status(USHORT aw_index, USHORT aw_value);
VOID set_update_start_factor(USHORT aw_index, USHORT aw_value);
VOID set_REFW_maker_id(USHORT aw_index, USHORT aw_value);
VOID set_REFW_major_version_using(USHORT aw_index, USHORT aw_value);
VOID set_REFW_minor_version_using(USHORT aw_index, USHORT aw_value);
VOID set_REFW_major_version_startup(USHORT aw_index, USHORT aw_value);
VOID set_REFW_minor_version_startup(USHORT aw_index, USHORT aw_value);
VOID set_REFW_using_timer(USHORT aw_index, USHORT aw_value);
VOID set_FHMRE_index(USHORT aw_index, USHORT aw_value);
VOID set_transfer_complete_part_no(USHORT aw_index, ULONG aw_value);
VOID set_transfer_complete_data_size(USHORT aw_index, ULONG aw_value);
VOID set_report_result(USHORT aw_index, USHORT aw_type, USHORT aw_value);
VOID set_resend_counter(USHORT aw_index, USHORT aw_type, USHORT aw_value);
/*--index map*/
USHORT convert_CPRI_no_REFW_index(USHORT aw_CPRI_no);
/*--*/
USHORT search_REFW_maker_id(USHORT aw_maker_id);
/*--resend counter*/
USHORT can_retry(USHORT aw_index, USHORT aw_type);
/*--check all RE is idle */
USHORT all_RE_idle();
/*--timer - using timer*/
USHORT m_dl_StopTimer(ULONG al_timer_id, USHORT aw_CPRI_no);
USHORT m_dl_StartTimer(ULONG al_timer_id, USHORT aw_CPRI_no);

/* ------(2) DLM manager table (DLM)-----------*/
/* (2) DLM MNG TBL*/
/*実行中DL開始要因				;0:未実行,1:REC,2:RE								;0:未実行				;*/
//executing DL start factor
/*更新中リンク情報				;0x0000～0xFFFF(D1:CPRI#1～D16:CPRI#16)				;0x0000					;*/
//updating link info
/* フラッシュ書込フラグ			;0:未実行,1:書込中									;0:未実行				;*/
//flash write flag
/*DL中REファイル読込状態		;0:未実行,1:読込中,2:読込済,3:読込中止中,4:読込NG	;0:未実行				;*/
//downloading RE file read status
/* DL中REファイル情報			;0xFFFF:初期値,0～16:FHM保持REファイル情報配列INDEX	;0xFFFF:DL中ファイル無	;*/
//downloading RE file info
/*メーカ識別子					;0,65～90											;70固定？				;*/
//maker id
/* メジャーバージョン(運用中)	;0～99												;0						;*/
/* マイナーバージョン(運用中)	;0～99												;0						;*/
/* メジャーバージョン(起動用)	;0～99												;0						;*/
/* マイナーバージョン(起動用)	;0～99												;0						;*/
//executing_DL_start_factor
//ref: m_dl_def.h
//flash_write_flag
//ref: m_dl_def.h
//#define FLASH_WRITE_FLAG_NOT_EXECUTE	0
//#define FLASH_WRITE_FLAG_WRITING		1
//DownLoading_RE_file_read_status
//ref: m_dl_def.h
/* get set methods */
USHORT DLM_get_executing_DL_start_factor(DLT_DLM_TBL* at_DLM_p);
USHORT DLM_get_updating_link_info(DLT_DLM_TBL* at_DLM_p, USHORT aw_CPRI_no);
USHORT DLM_get_flash_write_flag(DLT_DLM_TBL* at_DLM_p);
USHORT DLM_get_DownLoading_RE_file_read_status(DLT_DLM_TBL* at_DLM_p);
USHORT DLM_get_DownLoading_RE_file_info(DLT_DLM_TBL* at_DLM_p);
USHORT DLM_get_maker_id(DLT_DLM_TBL* at_DLM_p);
USHORT DLM_get_major_version_using(DLT_DLM_TBL* at_DLM_p);
USHORT DLM_get_minor_version_using(DLT_DLM_TBL* at_DLM_p);
USHORT DLM_get_major_version_startup(DLT_DLM_TBL* at_DLM_p);
USHORT DLM_get_minor_version_startup(DLT_DLM_TBL* at_DLM_p);

USHORT DLM_set_executing_DL_start_factor(DLT_DLM_TBL* at_DLM_p, USHORT aw_value);
USHORT DLM_set_updating_link_info(DLT_DLM_TBL* at_DLM_p, USHORT aw_CPRI_no, USHORT aw_value);
USHORT DLM_set_flash_write_flag(DLT_DLM_TBL* at_DLM_p, USHORT aw_value);
USHORT DLM_set_DownLoading_RE_file_read_status(DLT_DLM_TBL* at_DLM_p, USHORT aw_value);
USHORT DLM_set_DownLoading_RE_file_info(DLT_DLM_TBL* at_DLM_p, USHORT aw_value);
USHORT DLM_set_maker_id(DLT_DLM_TBL* at_DLM_p, USHORT aw_value);
USHORT DLM_set_major_version_using(DLT_DLM_TBL* at_DLM_p, USHORT aw_value);
USHORT DLM_set_minor_version_using(DLT_DLM_TBL* at_DLM_p, USHORT aw_value);
USHORT DLM_set_major_version_startup(DLT_DLM_TBL* at_DLM_p, USHORT aw_value);
USHORT DLM_set_minor_version_startup(DLT_DLM_TBL* at_DLM_p, USHORT aw_value);

VOID   init_DLM();
USHORT get_executing_DL_start_factor();
USHORT get_updating_link_info(USHORT aw_CPRI_no);
USHORT get_flash_write_flag();
USHORT get_DownLoading_RE_file_read_status();
USHORT get_DownLoading_RE_file_info();
USHORT get_DLM_maker_id();
USHORT get_DLM_major_version_using();
USHORT get_DLM_minor_version_using();
USHORT get_DLM_major_version_startup();
USHORT get_DLM_minor_version_startup();

VOID set_executing_DL_start_factor(USHORT aw_value);
VOID set_updating_link_info(USHORT aw_CPRI_no, USHORT aw_value);
VOID set_flash_write_flag(USHORT aw_value);
VOID set_DownLoading_RE_file_read_status(USHORT aw_value);
VOID set_DownLoading_RE_file_info(USHORT aw_value);
VOID set_DLM_maker_id(USHORT aw_value);
VOID set_DLM_major_version_using(USHORT aw_value);
VOID set_DLM_minor_version_using(USHORT aw_value);
VOID set_DLM_major_version_startup(USHORT aw_value);
VOID set_DLM_minor_version_startup(USHORT aw_value);

/*-------(3)FHM保持REファイル情報------------*/
/* FHM holding RE file info */
/*メーカ識別子					;2byte	;0,65～90											;0:無効		*/
/*メジャーバージョン			;2byte	;0～99												;0				*/
/*マイナーバージョン			;2byte	;0～99												;0				*/
/*同一メーカRE接続有無			;2byte	;0:無,1:有											;0:無     */
/*ファイル名					;20byte	;"RE___FHM_EMMmm.FEX" or "RE_______EMMmm.FEX"[*1]	;"":無効  */
/*ファイルサイズ				;4byte	;0～16777216(=16MB)									;0        */
/*年							;2byte	;0～65535											;0:無効   */
/*月							;1byte	;1～12												;0:無効		*/
/*日							;1byte	;1～31												;0:無効		*/
/*分割数(=ファイルサイズ/10240)	;4byte	;0～1639											;0				*/
/*ファイルデータ展開先アドレス	;4byte	;共有メモリアドレス									;0				*/
USHORT FHMRE_get_maker_id(DLT_MTN_REINF* at_FHMRE_p);
USHORT FHMRE_get_major_version(DLT_MTN_REINF* at_FHMRE_p);
USHORT FHMRE_get_minor_version(DLT_MTN_REINF* at_FHMRE_p);
USHORT FHMRE_get_maker_id_RE_connection_exist(DLT_MTN_REINF* at_FHMRE_p);
UCHAR* FHMRE_get_file_name(DLT_MTN_REINF* at_FHMRE_p);
ULONG FHMRE_get_file_size(DLT_MTN_REINF* at_FHMRE_p);
ULONG FHMRE_get_file_date(DLT_MTN_REINF* at_FHMRE_p);
ULONG FHMRE_get_number_part(DLT_MTN_REINF* at_FHMRE_p);
ULONG FHMRE_get_file_data_expand_first_addr(DLT_MTN_REINF* at_FHMRE_p);

USHORT FHMRE_set_maker_id(DLT_MTN_REINF* at_FHMRE_p, USHORT aw_value);
USHORT FHMRE_set_major_version(DLT_MTN_REINF* at_FHMRE_p, USHORT aw_value);
USHORT FHMRE_set_minor_version(DLT_MTN_REINF* at_FHMRE_p, USHORT aw_value);
USHORT FHMRE_set_maker_id_RE_connection_exist(DLT_MTN_REINF* at_FHMRE_p, USHORT aw_value);
UCHAR* FHMRE_set_file_name(DLT_MTN_REINF* at_FHMRE_p, UCHAR* aw_value);
ULONG FHMRE_set_file_size(DLT_MTN_REINF* at_FHMRE_p, ULONG aw_value);
//ULONG FHMRE_set_file_date(DLT_MTN_REINF* at_FHMRE_p, ULONG aw_value);
ULONG FHMRE_set_number_part(DLT_MTN_REINF* at_FHMRE_p, ULONG aw_value);
ULONG FHMRE_set_file_data_expand_first_addr(DLT_MTN_REINF* at_FHMRE_p, ULONG aw_value);

/*-------(3)FHM保持REファイル情報------------*/
/* FHM holding RE file info */
VOID   init_all_FHMRE();
USHORT get_FHMRE_maker_id(USHORT aw_index);
USHORT get_FHMRE_major_version(USHORT aw_index);
USHORT get_FHMRE_minor_version(USHORT aw_index);
USHORT get_maker_id_RE_connection_exist(USHORT aw_index);
UCHAR* get_FHMRE_file_name(USHORT aw_index);
ULONG  get_FHMRE_file_size(USHORT aw_index);
ULONG  get_file_date(USHORT aw_index);
ULONG  get_number_part(USHORT aw_index);
ULONG  get_file_data_expand_first_addr(USHORT aw_index);

VOID set_FHMRE_maker_id(USHORT aw_index, USHORT aw_value);
VOID set_FHMRE_major_version(USHORT aw_index, USHORT aw_value);
VOID set_FHMRE_minor_version(USHORT aw_index, USHORT aw_value);
VOID set_maker_id_RE_connection_exist(USHORT aw_index, USHORT aw_value);
VOID set_FHMRE_file_name(USHORT aw_index, UCHAR* aw_value);
VOID set_FHMRE_file_size(USHORT aw_index, ULONG aw_value);
//VOID set_file_date(USHORT aw_index, ULONG aw_value);
VOID set_number_part(USHORT aw_index, ULONG aw_value);
VOID set_file_data_expand_first_addr(USHORT aw_index, ULONG aw_value);

USHORT convert_CPRI_no_FHMRE_index(USHORT aw_CPRI_no);
USHORT search_FHMRE_maker_id(USHORT aw_maker_id);

/* ------(4)---------------------------
対REC状態						;2byte				ファイル更新用 対REC状態一覧 参照	;0:アイドル	
使用中タイマ					;2byte				0:使用タイマなし					;0:使用タイマなし	
													4:ファームウェア送信確認タイマ							;
													14:ファームウェア送信確認タイマ(更新用)					;
FHM保持ファイル情報				;2byte				0:FHM保持REファイル情報配列INDEX	;0固定	
受信済み分割番号				;4byte				0～1639							;0	
受信済みデータサイズ			;4byte				0～16777216(=16MB)				;0	
ファームウェアファイル報告結果	;2byte				0:未実行,1:正常,2:NG				;0:未実行	
ファイル情報報告結果			;2byte				0:未実行,1:正常,2:NG				;0:未実行	
ファイルデータ送信完了報告結果	;2byte				0:未実行,1:正常,2:NG,3:タイムアウト	;0:未実行	
*/
/* ファイル更新用 対REC状態状態一覧
 * file update REC status
0:アイドル							;以下に該当しない状態
1:ファイルデータ送信(FW)待ち		;最初のファイルデータ送信受信(REC→FHM) ～ ファイルデータ送信完了報告通知受信(REC→FHM)まで
2:運用中ファイルデータ送信(FW)待ち	;最初の運用中ファイルデータ送信受信(REC→FHM) ～ 運用中ファイルデータ送信完了報告通知受信(REC→FHM)まで
3:ファイルデータ転送先決定待ち		;ファイル情報報告/運用中ファイル情報報告応答送信(REC←FHM) ～ 最初のファイルデータ送信/運用中ファイルデータ送信受信(REC→FHM)まで
*/
/*--get & set methods*/
USHORT REC_get_status(DLT_REC_TBL* at_REC_p);
USHORT REC_get_using_timer(DLT_REC_TBL* at_REC_p);
//USHORT REC_get_FHM_hodling_file_info(DLT_REC_TBL* at_REC_p);
ULONG REC_get_recived_part_no(DLT_REC_TBL* at_REC_p);
ULONG REC_get_recived_data_size(DLT_REC_TBL* at_REC_p);
//USHORT REC_get_FW_file_report_result(DLT_REC_TBL* at_REC_p);
USHORT REC_get_file_info_report_result(DLT_REC_TBL* at_REC_p);
USHORT REC_get_file_data_send_complete_report_result(DLT_REC_TBL* at_REC_p);
/*--set methods */
USHORT REC_set_status(DLT_REC_TBL* at_REC_p, USHORT aw_value);
USHORT REC_set_using_timer(DLT_REC_TBL* at_REC_p, USHORT aw_value);
//USHORT REC_set_FHM_hodling_file_info(DLT_REC_TBL* at_REC_p, USHORT aw_value)
ULONG REC_set_recived_part_no(DLT_REC_TBL* at_REC_p, ULONG aw_value);
ULONG REC_set_recived_data_size(DLT_REC_TBL* at_REC_p, ULONG aw_value);
//USHORT REC_set_FW_file_report_result(DLT_REC_TBL* at_REC_p, USHORT aw_value)
USHORT REC_set_file_info_report_result(DLT_REC_TBL* at_REC_p, USHORT aw_value);
USHORT REC_set_file_data_send_complete_report_result(DLT_REC_TBL* at_REC_p, USHORT aw_value);

/*-------(4)対REC情報テーブル---------------*/
VOID   init_REC();
USHORT get_REC_status();
USHORT get_REC_using_timer();
ULONG  get_REC_recived_part_no();
ULONG  get_REC_recived_data_size();
USHORT get_REC_file_info_report_result();
USHORT get_REC_file_data_send_complete_report_result();

VOID set_REC_status(USHORT aw_value);
VOID set_REC_using_timer(USHORT aw_value);
VOID set_REC_recived_part_no(ULONG aw_value);
VOID set_REC_recived_data_size(ULONG aw_value);
VOID set_REC_file_info_report_result(USHORT aw_value);
VOID set_REC_file_data_send_complete_report_result(USHORT aw_value);

/*-------common functions----------------------*/
/*--notice msg code*/
//ファームウェアファイル報告要求(CMT_CPRIF_FIRMFILEREQ)		0x2110
#define MSG_FW_FILE_REPORT_REQ						1
//ファイル情報報告通知(CMT_CPRIF_FILEINFREQ)				0x2210
#define MSG_FILE_INFO_REPORT_NOTICE					2
//ファイルデータ送信完了報告通知(CMT_CPRIF_FILSNDFINNTC)	0x2310
#define MSG_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE	3
//REファイル更新応答 CMD_TSKIF_REFIRM_DL_RSP				0x4202
#define MSG_RE_FILE_UPDATE_RESPOND					4
//ファイルデータ送信	0x2300
#define MSG_FILE_DATA_SEND							5

#if(SEND_MSG_UT)
USHORT get_last_msg_code(USHORT aw_backward);
VOID save_last_msg_code(USHORT aw_code);
#endif
#if(TIMER_CHECK_UT)
VOID save_last_start_timer(ULONG al_timer_id);
ULONG get_last_start_timer(USHORT aw_backward);
VOID save_last_stop_timer(ULONG al_timer_id);
ULONG get_last_stop_timer(USHORT aw_backward);
#endif

VOID RE_forwarding_notice(USHORT aw_code, VOID* at_msg_p);
INT send_msg_to_RE(USHORT aw_code, VOID* at_msg_p);

USHORT is_updating_link_exist();
VOID   reset_updating_link_info();

#if(CALL_STACK_UT)
#define SAVE_LAST_CALL	save_last_call
void save_last_call(void* al_func_p);
VOID* get_last_call(USHORT aw_backward);
#else
#define SAVE_LAST_CALL(...)
#endif

#endif /*M_DL_RAM_H*/
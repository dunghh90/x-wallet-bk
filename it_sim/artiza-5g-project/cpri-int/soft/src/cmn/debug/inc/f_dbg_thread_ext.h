/*!
 * @skip   $Id$
 * @file   f_dbg_thread_def.h
 * @brief  デバッグスレッド外部関数プロトタイプ宣言ファイル
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


#ifndef F_DBG_THREAD_EXT_H_
#define F_DBG_THREAD_EXT_H_

extern	int	dbgcmd_exec_pf_dpda(char *, FILE *fp) 	;
extern	int	dbgcmd_exec_pf_dia(char *, FILE *fp) 	;
extern	int	dbgcmd_exec_pf_log(char *, FILE *fp) 	;
extern	int	dbgcmd_exec_pf_main(char *, FILE *fp) 	;
extern	int	dbgcmd_exec_pf_cpr(char *, FILE *fp) 	;
extern	int	dbgcmd_exec_pf_wdt(char *, FILE *fp) 	;
extern	int	dbgcmd_exec_pf_ext(char *, FILE *fp)	;
extern	int	dbgcmd_exec_pf_rsv(char *, FILE *fp)	;
extern	int	dbgcmd_exec_l2_lbp(char *, FILE *fp)	;
extern	int	dbgcmd_exec_l3_cpr(char *, FILE *fp)	;


#endif /* F_DBG_THREAD_EXT_H_ */

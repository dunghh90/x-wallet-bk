/*!
 * @skip		$ld:$
 * @file		f_hdl_ftyp.h
 * @brief		HDLスレッド プロトタイプ宣言
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 * @date		2013/10/29 ALPHA)鮫島 docomo ZYNQ化 & 共通化対応
 * @date		2015/04/13 ALPHA)鎌田 DCN向け35G-SRE対応
 * @date		2015/05/27 ALPHA)村上 ハソ改版(M-RRU-ZSYS-01582)対応
 * @date		2015/09/16 ALPHA)鎌田 modify for RRE 3.5G TDD
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */
#ifndef F_HDL_FTYP_H
#define F_HDL_FTYP_H

typedef struct{
	CHAR				thdName[D_HDL_THDNAME_LEN];			/**< name			*/
	UINT				thdPriority;						/**< priority		*/
	FUNCPTR				thdFunc;							/**< Function		*/
	UINT				thdStackSize;						/**< stack size		*/
}T_HDL_CRT_EVENT_CTBL;

/*!
 * @name HDLスレッドプロトタイプ
 * @note HDLスレッドのプロトタイプ定義
 * @{
 */
extern	VOID	f_hdl_main(T_HDL_CRT_EVENT_CTBL f_hdlr_thdCreMng_TDD[]);
extern	UINT	f_hdl_init(VOID);
extern	VOID	f_hdl_main_1s(VOID);
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern	VOID	f_hdl_main_cpri(VOID);
extern	VOID	f_hdl_main_cpri_re(VOID);
extern UINT 	f_hdl_main_cpri_sub(USHORT linkno);
extern	VOID	f_hdl_main_l1(VOID);
extern	VOID	f_hdl_main_wdt(VOID);
extern	VOID	f_hdl_main_pcie(VOID);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
extern	VOID	f_hdl_main_1pps(VOID);
extern	VOID	f_hdl_main_state_fheth1(VOID);
extern	VOID	f_hdl_main_state_fheth2(VOID);
extern	VOID	f_hdl_main_sv(VOID);
extern	VOID	f_hdl_main_wdt(VOID);
extern	VOID	f_hdl_main_ald(VOID);
extern	VOID	f_hdai_main(VOID);
/* @} */


#endif
/* @} */

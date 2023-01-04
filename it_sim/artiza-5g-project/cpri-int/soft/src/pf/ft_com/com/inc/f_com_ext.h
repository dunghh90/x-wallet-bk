/*!
 * @skip  $ld:$
 * @file  f_com_ext.h
 * @brief 装置共通外部参照定義ヘッダ
 * @date  2008/09/01 ALPHA) 戸塚 Create
 * @date  2008/12/22 ALPHA) 藤井 M-RRU-ZSYS-00162 対処
 * @date  2009/04/14 FFCS)wugh modify for M-RRU-ZSYS-00287 - 1-tone improvement.
 * @date  2010/02/16 ALPHA) 横山 LTE-RRH対応
 * @date  2010/04/14 FFCS)Wangjuan Update for M-RRU-RRHFT-00031
 * @date  2010/04/22 ALPHA) 藤井 Update for M-RRU-RRHFT-00033
 * @date  2010/05/25 ALPHA) 藤井 MT-RRH対応
 * @date  2010/07/12 ALPHA) 藤井 M-RRU-ZSYS-00412 対処
 * @date  2010/07/17 FFCS)wangjuan modify for M-RRU-ZSYS-00418
 * @date  2010/08/16 ALPHA) 藤井 M-RRU-ZSYS-00440 対処
 * @date  2010/10/04 ALPHA) 横山 AISG対応
 * @date  2011/04/11 ALPHA) 横山 LTE Only Mode対応
 * @date  2011/12/15 ALPHA) 藤井 M-RRU-ZSYS-00755 対処
 * @date  2012/10/22 COMAS) 中島 MTCD 対応
 * @date  2013/04/21 ALPHA) 藤井 B25 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2015
 */

/*!
 * @addtogroup RRH_PF_COM
 * @{
 */
#ifndef F_COM_EXT_H
#define F_COM_EXT_H

/****************************************/
/* common function						*/
/****************************************/
VOID	f_com_getSysTim(T_RRH_SYSTEM_TIME* systim);
VOID	f_com_getSysTim_notMs(T_RRH_SYSTEM_TIME* systim);
VOID	f_com_taskDelay(INT ticks, UINT task_no, UINT state, UINT evt_no);
VOID	f_com_logSaveFlash( UINT wdtLogFlg );
VOID	f_com_logSaveFlashFHM(void); /* 2021/01/19 M&C) Merge 4G FHM src (add) */
/****************************************/
/* APD初期設定関連						*/
/****************************************/
/* 5GDU_chg */
VOID	f_com_dpda_getAlmHistLog( T_RRH_ALARM_HISTORYLOG_INFO *pAlmHistLogInfo ); /* 5GDU_chg */

#define  f_com_abort( processID, almCode ) f_cmn_com_abort( processID, almCode, __FILE__, __LINE__)

extern UINT f_com_swap32(UINT);
extern USHORT f_com_swap16(USHORT);

/* matsumoto modify start */
#ifdef D_RRH_MODE_REC
extern T_RRH_TRAINVTBL			cmw_tra_inv_tbl;		/**< TRA card inventory management table  */
#endif
/* matsumoto modify end */

extern	UINT	f_comw_default_user_disabled_flag;

extern	T_COM_DUMP_MNG	f_comw_dumpmsg;


#endif
/* @} */

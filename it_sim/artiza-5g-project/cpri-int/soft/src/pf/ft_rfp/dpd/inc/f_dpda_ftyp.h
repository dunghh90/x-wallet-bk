/*!
 * @skip  $ld:$
 * @file  f_dpda_ftyp.h
 * @brief DPDAタスク プロトタイプ宣言
 * @date  2011/07/26 ALPHA)横山 Create
 * @date  2011/09/20 ALPHA)藤井 SRRU-2.1G-EVDO対応
 * @date  2011/11/02 ALPHA)藤井 M-RRU-ZSYS-00703 対処
 * @date  2011/11/04 ALPHA)横山 M-RRU-ZSYS-00714 対処
 * @date  2011/11/04 ALPHA)横山 M-RRU-ZSYS-00703 対処
 * @date  2011/11/04 ALPHA)横山 M-RRU-ZSYS-00716 対処
 * @date  2011/11/08 ALPHA)横山 M-RRU-ZSYS-00721 対処
 * @date  2011/11/15 ALPHA)藤井 M-RRU-ZSYS-00721 対処(アラーム仕様変更)
 * @date  2011/11/16 ALPHA)横山 M-RRU-ZSYS-00721 対処(アラーム仕様変更)
 * @date  2011/11/18 ALPHA)横山 M-RRU-ZSYS-00721 対処(TSSI報告要否判定変更)
 * @date  2011/12/06 ALPHA)横山 SRRU-2.1G-LTE対応
 * @date  2011/12/14 ALPHA)高橋 SRRU-2.1G-LTE対応
 * @date  2012/01/10 ALPHA)高橋 M-RRU-ZSYS-00766対処
 * @date  2012/01/12 ALPHA)横山 M-RRU-ZSYS-00775対処
 * @date  2012/01/20 ALPHA)横山 M-RRU-ZSYS-00788対処
 * @date  2012/01/23 ALPHA)藤井 M-RRU-ZSYS-00792対処
 * @date  2012/01/26 ALPHA)藤井 M-RRU-ZSYS-00798対処
 * @date  2012/01/30 ALPHA)横山 M-RRU-ZSYS-00801対処
 * @date  2012/01/30 ALPHA)横山 M-RRU-ZSYS-00800対処
 * @date  2012/02/08 ALPHA)横山 M-RRU-ZSYS-00807対処
 * @date  2012/03/16 ALPHA)高橋 N21GLTE対応
 * @date  2012/05/10 ALPHA)高橋 N21GLTE M-RRU-ZSYS-00871対処
 * @date  2012/06/14 ALPHA)横山 M-RRU-ZSYS-00887対処
 * @date  2012/12/18 ALPHA)横山 M-RRU-ZSYS-00921対処
 * @date  2012/12/21 ALPHA)横山 M-RRU-ZSYS-00925対処
 * @date  2013/01/10 ALPHA)横山 M-RRU-ZSYS-00930対処
 * @date  2013/04/03 ALPHA)中村 ST800対応
 * @date  2013/11/19 ALPHA)加藤 ZYNQ対応
 * @date  2015/04/22 ALPHA)近間 RRH-013-000 DCM_3.5G対応
 * @date  2015/08/07 TDI)satou FHMで全面見直し
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2015
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#ifndef F_DPDA_FTYP_H
#define F_DPDA_FTYP_H

/*!
 * @name DPDAタスク
 * @note プロトタイプ宣言
 * @{
 */
extern	VOID f_dpda_main_fhm();
extern	INT  f_dpda_init_fhm(VOID*);
extern	INT  f_dpda_anlz(VOID*);
extern	INT  f_dpda_abort_fhm(VOID*);
extern	UINT f_dpda_IO_read(UINT uiRegAddr, UINT uiReadMode, UINT uiLogLvl);
extern	VOID f_dpda_init_taskTable();
extern	INT  f_dpda_act_powReportPeriodic_fhm(VOID*);
extern	VOID f_dpda_alm_logCollect(USHORT usAlarmNum);
extern	VOID f_dpda_peri_getHardRegInfo();
extern	VOID f_dpda_alm_factorJudge(UINT almJdgInfo1, UINT almJdgInfo2, UINT almOperator, UCHAR* almResult);
extern	VOID f_dpda_alm_detect(VOID);
extern	VOID f_dpda_alm_infoCollect(VOID);
extern	VOID f_dpda_alm_dataMake(USHORT *usNumOfAlarmOccur, USHORT *usNumOfAlarmClear);
extern	VOID f_dpda_alm_stateUpdate(USHORT *usNumOfAlarmOccur, USHORT *usNumOfAlarmClear);
extern	VOID f_dpda_alm_report_fhm(USHORT usNumOfAlarmOccur, USHORT usNumOfAlarmClear);
/*! @} */

#endif
/*! @} */

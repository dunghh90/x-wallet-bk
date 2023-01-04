/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_ext.h
 *  @brief  CPRI management extern table define file
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

#ifndef    M_CR_EXT
#define    M_CR_EXT

/** @addtogroup RRH_L3_CPR
* @{ */

#include	"m_cr_def.h"
#include	"m_cr_typ.h"

extern USHORT crw_taskstate;
extern UINT crw_taskevent;
extern UINT crw_poweron_reset_target;
extern CRT_EVENT_CTBL const crr_event_ctbl[];
extern void (* const crr_mtrxtbl[CRD_MX_STATE][CRD_MX_EVENT])(UINT *buff_adr);
extern CRT_CPRISTAPRIO const crr_cprilnksta_priotbl[CRD_MX_CPRILNKSTA][CRD_MX_LINKEV];
extern CRT_CPRILNKSTA const crr_cprilnksta_class_tbl[CRD_MX_CPRILNKSTA];
extern CRT_HC_CTL_TBL crw_hc_ctl_tbl[CMD_MAX_SYSNUM];
extern CRT_HC_CTL_MST_TBL crw_hc_ctl_mst_tbl[D_RRH_CPRINO_NUM][CMD_MAX_SYSNUM];
extern const USHORT crr_l3carriermax[CMD_MAX_SYSNUM];
extern const UINT crr_hctimno[CMD_MAX_SYSNUM];
extern CRT_RUNHIS_TBL crw_runhis_tbl[D_RRH_CPRINO_NUM];
extern USHORT crw_hissetflag;

#endif/*M_CR_EXT*/
/** @} */

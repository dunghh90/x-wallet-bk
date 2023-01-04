/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_l3Log.h
 *  @brief	RE L3 log Table
 *  @date   2015/05/15 FFCS)wuqy create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/******************************************************************************************************************************/

#ifndef M_CM_L3LOG_HEAD
#define M_CM_L3LOG_HEAD

#include "f_rrh_l3logTbl.h"
#include "m_cm_ReCardStaTbl.h"
#include "m_cm_ReCarStaTbl.h"


/****************************************************************************/
/* L3 Log Table for RCT and L2 */
/****************************************************************************/
extern T_RRH_L3_SR_LOG*						cmw_l3log_tbl_p;

extern VOID m_cm_CardStsResLogSave( CMT_CPRIF_CARDSTSRES_S3G *log_data, USHORT len);

extern VOID m_cm_CarStsResLogSave(CMT_CPRIF_CARLSTSRES *res, USHORT log_len);

extern VOID m_cm_L3LogPrint(FILE *outf);

extern VOID m_cm_L3LogTblInit();

#endif

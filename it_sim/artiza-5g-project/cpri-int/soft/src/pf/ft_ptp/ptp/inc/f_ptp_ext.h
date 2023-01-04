/*!
 * @skip 		$Id:$
 * @file 		f_ptp_ext.h
 * @brief		PTP thread extern header
 * @date		2018/06/27 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_PTP

 *  @{
 */
#ifndef F_PTP_EXT_H
#define F_PTP_EXT_H

/****************************************************************************************************************/
/*  Information of PTP Thread																					*/
/****************************************************************************************************************/
/*!
 * @name Information of PTP Thread
 * @note Information of PTP Thread
 * @{
 */
extern	UCHAR					*f_ptp_sfnfrm_flag;
extern	UCHAR					*f_ptp_ns_flag;
extern	INT						f_ptp_t3_adj_val;
extern	INT						f_ptp_t2_adj_val;

extern	UCHAR					*f_ptp_d_num;
extern	UCHAR					*f_ptp_m_macaddr;
extern	UCHAR					*f_ptp_accepted_clockclass;

extern	UCHAR					f_ptp_ssmval;

extern	UINT					f_ptp_status_data ;
extern	UINT					f_sync_status_data ;
extern	UINT					f_synce_status_data ;

extern	UCHAR					f_ptp_reporting_period ;
extern	UCHAR					f_syncE_reporting_period ;
extern	UCHAR					f_sync_reporting_period ;
extern	UINT					f_ptp_reporting_guarded ;
extern	UINT					f_syncE_reporting_guarded;
/* @} */

#endif
/** @} */

/*!
 * @skip 		$ld:$
 * @file 		f_ptp_ram.c
 * @brief		pf_ptpスレッド RAMテーブル
 * @date		2018/06/27 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	RAM Table
 *  @note	
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

#include	"f_ptp_inc.h"

/** @addtogroup RRH_PF_PTP
 *  @{
 */

/****************************************************************************************************************/
/*  Information of PTP Thread																					*/
/****************************************************************************************************************/
/*!
 * @name	Information of PTP Thread
 * @note	Information of PTP Thread
 * @{
 */
UCHAR					*f_ptp_sfnfrm_flag;
UCHAR					*f_ptp_ns_flag;

INT						f_ptp_t3_adj_val;
INT						f_ptp_t2_adj_val;

VOID*					f_ptpmw_assert_p;							/**< PTP main thread assert pointer */

UCHAR					*f_ptp_d_num;
UCHAR					*f_ptp_m_macaddr;
UCHAR					*f_ptp_accepted_clockclass;

UCHAR					f_ptp_ssmval = ESMC_QL_DNU;

UINT					f_ptp_status_data ;
UINT					f_sync_status_data ;
UINT					f_synce_status_data ;

UCHAR					f_ptp_reporting_period ;
UCHAR					f_syncE_reporting_period ;
UCHAR					f_sync_reporting_period ;

UINT					f_ptp_reporting_guarded ;
UINT					f_syncE_reporting_guarded;

UINT					f_syncE_ssm_timer = 5 ;
UINT					f_syncE_ssm_timer_e = 5; /* EDIT値書き込みエリア */
UINT					f_syncE_ssm_timeout ;
UINT					f_syncE_acceptance_ssm[SYNCE_ACCEPTANCE_SSM_NUM] ;

pthread_mutex_t f_syncE_ssm_mutex			= PTHREAD_MUTEX_INITIALIZER; /* グローバル変数用mutex */

/** @} */

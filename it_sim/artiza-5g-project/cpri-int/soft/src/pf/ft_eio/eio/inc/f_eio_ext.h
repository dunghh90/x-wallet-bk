/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eio_ext.h
 *  @brief  5GDU-LLS external IO supervision & cntrol external header file defination
 *  @date   2019/03/19 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/* 二重取込み防止 */
#ifndef		F_EIO_EXT
#define		F_EIO_EXT

/** @addtogroup RRH_PF_EIO
* @{ */

/*	ram	*/
extern	UINT						f_eiow_thrdstate;
extern	T_EIOW_EXTIO				f_eiow_extIo;
extern	T_RRH_DU_EIO_SVCTRL_MNG		*f_eiow_svctrl_mngp;
extern	T_RRH_DU_EIO_SVCTRL_LOG		*f_eiow_svctrl_logp;
extern	T_EIOW_FANMNG				f_eiow_fanMngTbl;
extern	T_EIO_ANT_INFO				f_eiow_antMngTbl[D_RRH_ANT_MAX];
extern  T_RRH_DU_PROCESS_INFO		*f_eiow_DuProcInfo;

/*	rom	*/
extern	VOID (*const f_eior_matrix[E_EIO_STAT_MAX][E_EIO_EVT_MAX])(VOID* );
extern	const	T_EIOR_FANUNIT_SV_INFO		f_eior_fanUnitSvInfo[E_EIO_FANITEM_MAXNO];
extern	const	UINT	f_eior_fanDacVal[E_DU_FANSPEEDSTS_MAXNO];
extern	const	T_EIOR_FANSPEED_INFO	f_eior_fanSpeedInfo[E_DU_FAN_MAXNO];

/* @} */

#endif	/* F_EIO_EXT */


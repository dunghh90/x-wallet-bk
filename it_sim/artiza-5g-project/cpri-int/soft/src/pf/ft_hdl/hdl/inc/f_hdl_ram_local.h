/******************************************************************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		f_hdl_ram_local.h
 *  @brief		External Definition
 *  @note		NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 				See also TableSpecifications(hdl)_ram.xls
 *  @date		2015/09/16 ALPHA)c modify for RRE 3.5G TDD
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/******************************************************************************************************************************/

#ifndef	F_hdl_RAM_LOCAL_H
#define	F_hdl_RAM_LOCAL_H

/** @addtogroup RRH_PF_HDL
 *  @{
 */
#include "f_hdl_inc.h"


extern	T_HDL_CRT_EVENT_CTBL	f_hdlw_thdCreMng[D_RRH_HDLTHDNO_MAX];	/**< thread create manage table 		*/
extern	USHORT					f_hdlw_thdState;						/**< thread state management table		*/

/* CascadeRRE	*/
extern	UINT					f_hdlw_oldstatus[D_RRH_SV_INF_NUM];		/* ©SVέΜσΤ					*/
extern	UINT					f_hdlw_oldstatus2[D_RRH_SV_INF_NUM];	/* JXP[hqREέΜσΤ			*/
extern	T_RRH_SVSTATELOG		f_hdlw_svvStatelog;		/* qSVσΤΟ»π ₯₯₯ ^ΝωΆΜSVσΤΟ»πΖ―κ 	*/
extern	T_RRH_SFPINFO			f_hdlw_sfp0mnglog;						/* SFP#0Ηξρπ					*/
extern	T_RRH_SFPINFO			f_hdlw_sfp1mnglog;						/* SFP#1Ηξρπ					*/
extern	T_RRH_SFPINFO_HIS		f_hdlw_sfpwork;							/* SFPπ‘Κwork						*/
extern	T_RRH_SFPINFO_HIS*		f_hdlw_sfp0mnginfo_his;					/* CascadeRRE SFP#0Ηξρπ			*/
extern	T_RRH_SFPINFO_HIS*		f_hdlw_sfp1mnginfo_his;					/* CascadeRRE SFP#1Ηξρπ			*/

#endif	/* F_hdl_RAM_LOCAL_H */
/** @} */

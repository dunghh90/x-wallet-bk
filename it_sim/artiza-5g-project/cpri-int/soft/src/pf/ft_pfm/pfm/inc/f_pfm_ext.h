/*!
 * @skip 		$Id:$
 * @file 		f_pfm_ext.h
 * @brief		PFM thread extern header
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_DHC

 *  @{
 */
#ifndef F_PFM_EXT_H
#define F_PFM_EXT_H

/****************************************************************************************************************/
/*  Information of PFM Thread																					*/
/****************************************************************************************************************/
/*!
 * @name Information of PFM Thread
 * @note Information of PFM Thread
 * @{
 */
																		/* Information of PFM Thread			*/
extern	T_PFM_INFO							f_pfmw_thdInfo __attribute__ ((aligned (16)));
/* @} */

/****************************************************************************************************************/
/*  Matrix function of PFM Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of PFM Thread
 * @note	Matrix function of PFM Thread
 * @{
 */
																		/* Matrix function of PFM Thread		*/
extern	const	T_PFM_CALLSETRELL_MTXTBL	f_pfmr_callSetRell_mtxtbl[E_PFM_STATE_MAX][E_PFM_EVT_MAX];
/* @} */

/****************************************************************************************************************/
/*  Performance Register Address																				*/
/****************************************************************************************************************/
/*!
 * @name	Performance Register Address
 * @note	Performance Register Address
 * @{
 */
extern	UINT						f_pfmw_rxwin_reg_addr_offset_antnum;
extern	UINT						f_pfmw_rxwin_reg_addr_offset_ant;
extern	UINT						f_pfmw_rxwin_reg_addr_offset_ccnum;
extern	UINT						f_pfmw_rxwin_reg_addr_offset_cc;
extern	T_PFM_REG_INFO				f_pfmw_rxwin_reg_addr[D_RRH_PFM_NUM_OF_REG];
extern	T_PFM_REG_INFO				f_pfmw_rxwin_reg_addr_sub6[D_RRH_PFM_NUM_OF_REG];
extern	T_PFM_REG_INFO				f_pfmw_rxwin_reg_addr_mmw[D_RRH_PFM_NUM_OF_REG];
extern	UINT f_pfmw_rxwin_reg_addr_com[D_RRH_PFM_NUM_OF_REG_COM];

/* @} */

#endif
/** @} */

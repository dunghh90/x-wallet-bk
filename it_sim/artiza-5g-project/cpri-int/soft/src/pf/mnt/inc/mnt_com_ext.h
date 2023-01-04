/*!
 * @skip  $ld:$
 * @file  mnt_com_ext.h
 * @brief 外部参照宣言
 * @date  2013/11/28
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_MNT
 * @{
 */
#ifndef MNT_COM_EXT_H
#define MNT_COM_EXT_H

/*!
 * @name 関数外部宣言
 * @note 関数外部宣言定義
 * @{
 */

extern T_RRH_TRAINVTBL		cmw_tra_inv_tbl;		/**< TRA card inventory management table  */
												
									/**< TRA card inventory management table  */
extern	CMT_TBLCLIENTINFO	cmw_tblClientInfo[CMD_MX_TCPNO];
							/**< CPRI-MT/RE-MT Client information table  */

extern 	CMT_MTSETMNG			cmw_mtsetmng;			/**<  MT address infomation table  */

extern 	CMT_MTADDR_INFO 		cmw_tserv_addr[CMD_MAX_SYSNUM];       	/* TCP server's MT address infomation	   */

extern UINT	const	 mnt_comr_cprimsg_subTbl[MBD_CPRIMSGNUM];

extern UINT cmw_reKind;									/**< RE Kind	*/

#ifdef OPT_RRH_ZYNQ_REC
extern UINT internal_retrun_on;
#endif
/* mastumoto modify end */

/* @} */

#endif
/* @} */

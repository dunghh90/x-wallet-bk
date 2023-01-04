/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_ext.h
 *  @brief	TRA process external common table declaration
 *  @date   2008/07/18 FFCS)Tangj create
 *  @date   2009/03/05  FFCS)Wuh modify for M-S3G-eNBPF-01471
 *					    stop writing FLASH after receiving CPRI message
 *  @date   2009/04/16  FFCS)Wuh modify for S3G PRC CR-xxx-xxx (CPRI spec V1.07)
 *  @date   2009/06/09  FFCS)Wuh modify for M-S3G-eNBPF-02047
 *                      RE version response incorrect
 *	@date	2009/06/10  FFCS)Wangjuan modify for M-S3G-eNBPF-02050
 *						BRE card can not connect in SV-MT/FLD-MT debug test
 *  @date   2009/06/16  FFCS)Wuh modify for M-S3G-eNBPF-02086, 
 *                      modify for FLASH erase in file no new case faild
 *  @date   2009/12/27 QNET)Kabasima MOD QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2010/01/04 QNET)Kabasima ADD M-S3G-eNBPF-02869:[品質改善]REデバッグコマンド機能拡充
 *  @date   2010/06/01 FJT)Taniguchi CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *  @date   2010/11/02 CMS)Takahashi CR-XXX
 *  @date   2012/10/18 FFCS)niemsh modify for M-S3G-eNBSYS-02684
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2010
 */
/******************************************************************************************************************************/

/* For multiple include */
#include "m_cm_header.h"
#include "f_rrh_reg_eep.h"
 /*!
 * @addtogroup RRH_MNT_COM
 * @{
 */

/****************************************************************************/
/*																			*/
/*	Common table declaration												*/
/*																			*/
/****************************************************************************/
CMT_SYSMNGTBL		cmw_sys_mng_tbl[CMD_MAX_SYSNUM];
								/**< System parameter management table  */
								
CMT_REDEVCOMPTBL	cmw_re_dev_tbl;
								/**< RE device management table  */
/** @name inventory management table */
/* @{ */

T_RRH_TRAINVTBL			cmw_tra_inv_tbl;		/**< TRA card inventory management table  */
/* @} */
						
CMT_TBLCLIENTINFO	cmw_tblClientInfo[CMD_TCPCONN_NUM_MAX];
							/**< CPRI-MT/RE-MT Client information table  */

CMT_MTSETMNG		cmw_mtsetmng;							/**<  MT address infomation table  */

CMT_MTADDR_INFO 	cmw_tserv_addr[CMD_MAX_SYSNUM];       	/* TCP server's MT address infomation	   */

/*name  MT address reset flag */
UINT					cmw_mtaddr_reset;			/**< MT address reset flag for MTM */

/*name  MT address reset flag */
UINT					cmw_mkaddr_reset;			/**< MT address reset flag for MKM */
/* valid system info */
UINT					cmw_validsysinfo;
/* @} */

/*name SV-MT/FLD-MT connect enable flag for BRE card debug */
UINT					cmw_dbg_bremtconnflg;
/*name FLD-MT ftp upload transfer stop flag*/
UINT					cmw_ftpulstpflg;

/*name FLD-MT ftp upload transfer max message limitation*/
INT						cmw_ftpulmaxmsgq;
/* @} */

/** @name TRA file information table  */
/* @{ */
CMT_TRAFILEINFTBL		cmw_trafile_inf_tbl;			/**< TRA file information table  */
/* @} */

UINT cmw_hcsta[D_RRH_CPRINO_NUM][2];

UINT cmw_device_kind;				/* device kind */

UINT cmw_reKind;					/* RE Kind	*/

/* M_CM_EXT */
/* @} */



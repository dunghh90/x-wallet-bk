/*!
 * @skip    $Id$
 * @file    m_lb_ram.c
 * @brief   LAPB writable variable 定義
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.18 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/


/* @addtogroup RRH_L2_LPB
 * @{
 */

#include "m_cm_header.h"			/*common header							*/
#include "tm_lb_header.h"			/*lapb task header						*/
#include "m_lb_def.h"			/*lapb task header						*/


struct LBT_MNGTBL	lbw_mngtbl[D_RRH_CPRINO_NUM];                             /*!< */

VOID  *lbw_mngTblForLog_off[D_RRH_CPRINO_NUM];
UINT  lbw_mngTblForLog_size[D_RRH_CPRINO_NUM];

struct	LBT_STSLOG_DAT		lbw_stslog[D_RRH_CPRINO_NUM][CMD_NUM2];           /*!< L2 statistics  log area*/
UINT		lbw_stscnt[D_RRH_CPRINO_NUM];                                     /*!< L2 statistics log area count*/

struct	LBT_ABTLOG_DAT		lbw_ablog[D_RRH_CPRINO_NUM][CMD_NUM10];           /*!< L2 abort log area*/
UINT		lbw_abcnt[D_RRH_CPRINO_NUM];                                      /*!< L2 abort log area count*/

UINT		lbw_keepbufflg;                                 /*!< LAPB task keep buffer flag*/
						 			/* LAPB task state                    	*/
UINT		lbw_tskstt;                                     /*!< LAPB task state */
						 			/* LAPB task event                    	*/
UINT		lbw_tskevent;                                   /*!< LAPB task event */

/************************/
/* 走行履歴情報テーブル */
/************************/
/* 走行履歴情報テーブル */
LBT_RUNHIS_TBL	lbw_runhis_tbl[D_RRH_CPRINO_NUM];                             /*!< 走行履歴情報テーブル */

/*********************************************************************************/
/*In case of lbw_allrunhis_flg is equal to CMD_OFF,								*/
/* sending I frame, receiving I frame and L1-DATA-CNF will not be logged.		*/
/*In case of lbw_allrunhis_flg is equal to CMD_ON, All task IF will be logged.	*/
/********************************************************************************/
UINT	lbw_allrunhis_flg;                                  /*!< All task IF log flag */

UINT lbw_L3DstrbtTbl_cur[D_RRH_CPRINO_NUM];

/*	L3 distribution table(MDIF)	*/
UINT lbw_L3DstrbtTbl[D_RRH_CPRINO_NUM][LBD_L3MSGNUM][2];

UINT lbw_L2StsNotiTbl_cur[D_RRH_CPRINO_NUM];

/*	L2 state change notification distribution table(MDIF)	*/
UINT lbw_L2StsNotiDstrbtTbl[D_RRH_CPRINO_NUM][LBD_SUBTASKNUM];

UINT  lbw_lnk_set_respQId[D_RRH_CPRINO_NUM];
UINT  lbw_lnk_rel_respQId[D_RRH_CPRINO_NUM];

UINT lbw_l1BusyKey[D_RRH_CPRINO_NUM] = { CMD_NUM0 };

LPBW_LINKSTAT_LOGTBL lbw_lnkStatLog[D_RRH_CPRINO_NUM];
/* @} */



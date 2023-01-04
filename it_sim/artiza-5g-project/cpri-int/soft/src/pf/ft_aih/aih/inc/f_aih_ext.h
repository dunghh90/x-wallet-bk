/********************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_aih_ext.h
 * @brief AIH  extern description header file
 * @date  2019/08/09 FJ)Takahashi Create
 * @date  2019/09/05 FJ)Takahashi modify
 *
 * All Rights Reserved, Copyright FUJITSU LIMITED 2019
 */
/********************************************************************************************************************/

/*!
 * @addtogroup RRH_PF_AIH
 * @{
 */

#ifndef F_AIH_EXT
#define F_AIH_EXT

/* Thread state matrix table */
extern void (* const f_aihr_matrix[E_AIH_STAT_NUM][E_AIH_EVT_NUM]) (VOID *);



extern UINT f_aih_thrdstate;            /* Thread status */

/* log table */
extern T_RRH_AIH_ALD_LOG_TBL		*f_aih_aldLogp;          /* ALD time stamp log tbl */
extern T_RRH_AIH_ALD_CTRL_TBL		*f_aih_aldCtrlInfoLogp;  /* ALD latest info log tbl */
extern T_RRH_AIH_ALD_CTRL_INFO		*f_aih_aldCtrlLogp;      /* ALD latest info log tbl */


#endif

/* @} */

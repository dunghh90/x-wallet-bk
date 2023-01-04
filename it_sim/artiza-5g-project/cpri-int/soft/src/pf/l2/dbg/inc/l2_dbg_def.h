/*!
 * @skip  $ld:$
 * @file  L2_dbg_def.h
 * @brief L2 dbg definition
 * @date  2013/12/03 FFCS)hongj create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_L2_DBG
 */
#ifndef L2_DBG_DEF_H
#define L2_DBG_DEF_H


 /*!
 * @note Task status defination
 */
#define L2_DBG_ST_INIT    		0      	/* Init state			*/               
#define L2_DBG_ST_USE 	     	1 		/* Use state			*/

/* @} */

/*!
 * @name Main matrix table definition
 * @{  
 */
#define L2_DBG_MX_EVENT 		9
/* @} */

/*!
 * @name dbg command
 * @{  
 */
#define L2_DBG_CMD_STR_SUM 		"cmd_cprdbg dsp l2 sum"
#define L2_DBG_CMD_STR_HIST 	"cmd_cprdbg dsp l2 hist"
/* @} */

/*!
 * @name event no
 * @{  
 */

#define L2_DBG_EVTNO_CPRIMSG_SUB_REQ         0x00
#define L2_DBG_EVTNO_CPRIMSG_UNSUB_IND       0x01
#define L2_DBG_EVTNO_LAPBSTATE_SUB_REQ       0x02
#define L2_DBG_EVTNO_LAPBSTATE_UNSUB_IND     0x03
#define L2_DBG_EVTNO_CPRIMON_START_IND       0x04
#define L2_DBG_EVTNO_CPRIMON_STOP_IND        0x05
#define L2_DBG_EVTNO_CPRISIMU_START_REQ      0x06
#define L2_DBG_EVTNO_CPRISIMU_STOP_REQ       0x07
#define L2_DBG_EVTNO_CPRIMSG_STATISTICS_IND  0x08
/* @} */

#endif	/*L2_MAIN_DEF_H*/

/*!
 * @skip  $ld:$
 * @file  L2_main_def.h
 * @brief L2 main definition
 * @date  2013/11/15 FFCS)hongj create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_L2
 */
#ifndef L2_MAIN_DEF_H
#define L2_MAIN_DEF_H


 /*!
 * @note Task status defination
 */
#define L2_MAIN_ST_INIT    		0      	/* Init state			*/               
#define L2_MAIN_ST_USE      	1 		/* Use state			*/

/* @} */

/*!
 * @name Main matrix table definition
 * @{  
 */
#define L2_MAIN_MX_STATE 		2
#define L2_MAIN_MX_EVENT 		3
/* @} */

											/*********************************/
											/*          L3 message number    */
											/*********************************/
											
/* matsumoto modify start */
/* make subscription cpri message‚É‚æ‚èóM‚·‚éCPRIM†í•Ê”‚ğ0x34İ’è‚·‚é */
#ifdef D_RRH_MODE_REC
#define			LBD_L3MSGNUM	0x34		/*!< L3 message number       */
#endif
/* matsumoto modify end */
#endif	/*L2_MAIN_DEF_H*/

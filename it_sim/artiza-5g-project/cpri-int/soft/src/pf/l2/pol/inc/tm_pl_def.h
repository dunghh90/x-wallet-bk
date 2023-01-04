/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_pl_def.h
 * @brief Polling constant definition.
 * @date  2008/07/25 FFCS)Zouw Create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef    TM_PL_DEF
#define    TM_PL_DEF


/*!
 * @name Main matrix table definition
 * @{  
 */
#define PLD_MX_STATE 		2
#define PLD_MX_EVENT 		2
/* @} */

/*!
 * @name Task status defination
 * @{  
 */
#define PLD_ST_INIT			0  	/* Init state		*/
#define PLD_ST_USE			1	/* Use state		*/
/* @} */


/*!
 * @name Task internal event code
 * @{  
 */
#define PLD_EV_INITREQ		0	/* initialization request		       */
#define PLD_EV_POLINTRNTC	1	/* Polling Period Interrupt Notice	*/
/* @} */


/*!
 * @name Return value definition
 * @{  
 */
#define PLD_RET_NORMAL		0	/* Normal return	*/
#define PLD_RET_ABNORMAL	1	/* Abnormal return	*/
/* @} */

/*!
 * @name Mask bits of registers defination
 * @{  
 */

#define PLD_MSKRXNTC_T 	0x003F	       /* Valid bits of LAPB receive notification register(TRIF) Only_Six_Links*/
#define PLD_MSKRXNTC_M 	0x0001	/* Valid bits of LAPB receive notification register(MDIF)*/


#define PLD_MSKTXEND_T 	0x003F	/* Valid bits of LAPB transmission end register(TRIF) Only_Six_Links	*/
#define PLD_MSKTXEND_M 	0x0001	/* Valid bits of LAPB transmission end register(MDIF)*/

#define PLD_MSK_ONE			1				/* One							*/
#define PLD_MSK_CID			0x0038			       /* Card ID bits					*/
/* @} */

/*!
 * @name register value
 * @{  
 */
#define PLD_REG_TMR1DAT			0xFF3C		/* Timer1 Mode Register configuration*/
											       /* TRR reg value= 4ms*1000000/65536	*/
#define PLD_REG_TRR1DAT			0x003D		/* Timer1 Reference Register configuration-- 4ms*/
#define PLD_REG_TER1DAT			0x02		/* Timer1 Event Register configuration*/
#define PLD_REG_TGCR1DAT		0x0900		       /* Timer1 Global Configuration Register configuration*/
/* @} */
#endif	/*TM_PL_DEF*/

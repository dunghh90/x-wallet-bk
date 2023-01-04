/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_tim.h
 * @brief レジスタアドレス定義ヘッダ(GP0_TIM)
 * @date  2015/06/19 ALPHA) tomioka
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_TIM_H
#define F_RRH_REG_TIM_H


/*!
 * @name レジスタアドレス(GP0_TIM)
 * @note レジスタアドレス(GP0_TIM)
 * @{
 */
#define D_RRH_REG_TIM_AXITIMERCLRPLS0	0xA0010000		/* AXI Timer IRQクリア		*/
#define D_RRH_TIM_WDT_AXITIMER_DIS		0x00000100		/* AXI Timer IRQクリア		*/

#define D_RRH_REG_TIM_AXI_TCR0 			0xA0010008		/* Timer/Counter Register	*/
/* @} */

#endif
/* @} */

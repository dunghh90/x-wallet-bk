/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_gpio.h
 * @brief レジスタアドレス定義ヘッダ(CPRI)
 * @date  2015/06/19 ALPHA) tomioka
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_DEF_GPIO_H
#define F_RRH_REG_DEF_GPIO_H

/*!
 * @name CPRIレジスタアドレス
 * @note CPRIレジスタアドレス
 * @{
 */

/************************/
/* MIOデータ			*/
/************************/
#define D_RRH_REG_MIO_DATA_0					0xE000A040						/*!< DATA_0								*/
#define D_RRH_MIO_OXBOOTBUSY					0x00000080						/*!< OXBOOTBUSY							*/

/************************/
/* 割込み(GPIO)			*/
/************************/
#define D_RRH_REG_GPIO_INT_STAT_0				0xE000A218						/*!< 割込み要因(GPIO MIOバンク0)		*/
#define D_RRH_REG_GPIO_INT_STAT_1				0xE000A258						/*!< 割込み要因(GPIO MIOバンク1)		*/
#define D_RRH_REG_GPIO_INT_STAT_2				0xE000A298						/*!< 割込み要因(GPIO EMIOバンク2)		*/
#define D_RRH_REG_GPIO_INT_STAT_3				0xE000A2D8						/*!< 割込み要因(GPIO EMIOバンク3)		*/

#endif
/* @} */

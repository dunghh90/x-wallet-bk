/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_soc.h
 * @brief レジスタアドレス定義ヘッダ(SOC)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_SOC_H
#define F_RRH_REG_SOC_H

/*!
 * @name SOCレジスタアドレス for arriaX
 * @note SOCレジスタアドレス for arriaX
 * @{
 */

#if defined (OPT_RRH_ARCH_A10)
#define D_RRH_REG_SOC_GPIO2_SWPORTA_DR		0xFFC02B00
/* watchdog reg */
#define	D_RRH_REG_SOC_WDT_CR				0xFFD00200		/*!< Control Register */
#define	D_RRH_REG_SOC_WDT_TORR				0xFFD00204		/*!< Timeout Range Register */
#define	D_RRH_REG_SOC_WDT_CCVR				0xFFD00208		/*!< Current Counter Value Register. */
#define	D_RRH_REG_SOC_WDT_CRR				0xFFD0020C		/*!< Counter Restart Register.  */
#define	D_RRH_REG_SOC_IMG_STAT				0xFFD03080		/*!< IMG_STAT */
#define	D_RRH_SOC_IMG_STAT_emr						D_RRH_VAL_SHIFT_L(1 , 29)
#define	D_RRH_SOC_IMG_STAT_jtagm					D_RRH_VAL_SHIFT_L(1 , 28)
#define	D_RRH_SOC_IMG_STAT_imgcfg_FifoFull			D_RRH_VAL_SHIFT_L(1 , 25)
#define	D_RRH_SOC_IMG_STAT_imgcfg_FifoEmpty			D_RRH_VAL_SHIFT_L(1 , 24)
#define	D_RRH_SOC_IMG_STAT_f2s_msel2				D_RRH_VAL_SHIFT_L(1 , 18)
#define	D_RRH_SOC_IMG_STAT_f2s_msel1				D_RRH_VAL_SHIFT_L(1 , 17)
#define	D_RRH_SOC_IMG_STAT_f2s_msel0				D_RRH_VAL_SHIFT_L(1 , 16)
#define	D_RRH_SOC_IMG_STAT_f2s_nceo_oe				D_RRH_VAL_SHIFT_L(1 , 13)
#define	D_RRH_SOC_IMG_STAT_f2s_nconfig_pin			D_RRH_VAL_SHIFT_L(1 , 12)
#define	D_RRH_SOC_IMG_STAT_f2s_pr_error				D_RRH_VAL_SHIFT_L(1 , 11)
#define	D_RRH_SOC_IMG_STAT_f2s_pr_done				D_RRH_VAL_SHIFT_L(1 , 10)
#define	D_RRH_SOC_IMG_STAT_f2s_pr_ready				D_RRH_VAL_SHIFT_L(1 , 9 )
#define	D_RRH_SOC_IMG_STAT_f2s_cvp_conf_done		D_RRH_VAL_SHIFT_L(1 , 8 )
#define	D_RRH_SOC_IMG_STAT_f2s_condone_oe			D_RRH_VAL_SHIFT_L(1 , 7 )
#define	D_RRH_SOC_IMG_STAT_f2s_condone_pin			D_RRH_VAL_SHIFT_L(1 , 6 )
#define	D_RRH_SOC_IMG_STAT_f2s_nstatus_oe			D_RRH_VAL_SHIFT_L(1 , 5 )
#define	D_RRH_SOC_IMG_STAT_f2s_nstatus_pin			D_RRH_VAL_SHIFT_L(1 , 4 )
#define	D_RRH_SOC_IMG_STAT_f2s_initdone_oe			D_RRH_VAL_SHIFT_L(1 , 3 )
#define	D_RRH_SOC_IMG_STAT_f2s_usermode				D_RRH_VAL_SHIFT_L(1 , 2 )
#define	D_RRH_SOC_IMG_STAT_f2s_early_usermode		D_RRH_VAL_SHIFT_L(1 , 1 )
#define	D_RRH_SOC_IMG_STAT_f2s_crc_error			D_RRH_VAL_SHIFT_L(1 , 0 )
#define	D_RRH_SOC_IMG_STAT_CHK_FPGA_CONFIG			D_RRH_SOC_IMG_STAT_f2s_condone_pin

#define D_RRH_REG_SOC_RSTMGR_PER1MODRST		0xFFD05028
#define D_RRH_REG_SOC_GPIO_BOARDVER		    0xFFC02A50

#elif defined (OPT_RRH_ARCH_ZYNQ)
/**********************************************************************/
/* PS System registers (F800_1000 to F880_FFFF)						  */
/**********************************************************************/
#define D_RRH_REG_SOC_CHE_ECC_CONTROL		0xF80060C4		/*!< CHE_ECC_CONTROL_REG_OFFSET		 */
#define D_RRH_REG_SOC_CHE_ECC_STATS			0xF80060F0		/*!< CHE_ECC_STATS_REG_OFFSET		 */

#define D_RRH_SOC_CHE_ECC_CONTROL_CLR			0x00000003	/*!< CHE_ECC_CONTROL(clear error)	 */

/**********************************************************************/
/* CPU Private registers (F890_0000 to F8F0_2FFF)					  */
/**********************************************************************/
/* ICDICFR(Interrupt Configuration Register) */
#define D_RRH_REG_SOC_ICDICFR0		0xF8F01C00		/*!< IRQ ID  0-15		 */
#define D_RRH_REG_SOC_ICDICFR1		0xF8F01C04		/*!< IRQ ID 16-31		 */
#define D_RRH_REG_SOC_ICDICFR2		0xF8F01C08		/*!< IRQ ID 32-47		 */
#define D_RRH_REG_SOC_ICDICFR3		0xF8F01C0C		/*!< IRQ ID 48-63		 */
#define D_RRH_REG_SOC_ICDICFR4		0xF8F01C10		/*!< IRQ ID 64-79		 */
#define D_RRH_REG_SOC_ICDICFR5		0xF8F01C14		/*!< IRQ ID 80-95		 */

/* 2bitずつ、01(bin)が active HIGH level sensitive, 11(bin)が rising-edge sensitive を表す						*/
#define D_RRH_SOC_ICDICFR3_INIT_VAL		0x75555555		/*!< ICDICFR3初期値  (61:level/62:edge)					*/
#define D_RRH_SOC_ICDICFR4_INIT_VAL		0x555557D5		/*!< ICDICFR4初期値  (64:level/65:level/67:edge/68:edge)		*/
#define D_RRH_SOC_ICDICFR5_INIT_VAL		0x55555555		/*!< ICDICFR5初期値  (84:level        )					*/




















#else
#endif
/* @} */
#endif
/* @} */

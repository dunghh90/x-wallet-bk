/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_tx_def.h
 * @brief TXS constant definition.
 * @date  2008/07/18 FFCS)Zouw Create
 * @date  2013.11.22 FFCS)hongj modify for zynq.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008
 */
 /****************************************************************************/
#ifndef TM_TX_DEF
#define TM_TX_DEF


/*!
 * @name 800M-VA/BD/LSI(bank reduce 16->8)
 * @{  
 */
 
 /*!< 2008/05/16 FJT) T.Yanagisawa CR08A-0008 */
#define TXD_BD_MDEIF_BANK 		1

/* @} */


/*!
 * @name Main matrix table definition
 * @{  
 */
#define TXD_MX_STATE 		2
#define TXD_MX_EVENT 		2
/* @} */


/*!
 * @name Task status defination
 * @{  
 */
#define TXD_ST_INIT    		0      	/* Init state			*/               
#define TXD_ST_USE      	1 		       /* Use state			*/
/* @} */


/*!
 * @name Task internal event code
 * @{  
 */
#define TXD_EV_INIREQ		0		/* Initialization Request	       */
#define TXD_EV_L1DATTXREQ	1		/* L1 Data Sending Notice		*/
/* @} */


/*!
 * @name Return value defination
 * @{  
 */
#define	TXD_RET_NORMAL		0		/*  Normal return		*/
#define	TXD_RET_ABNORMAL   	1		/*  Abnormal return		*/
/* @} */

/*!
 * @name LPB tx buffer character values
 * @{  
 */
#define  TXD_BNBUF_SIZE  	0x2A00          /* Size of each bank buffer(0d10752)*/
#define  TXD_TXBUF_BASE 	D_RRH_REG_CM_TX /* Base address of LAPB tx buffer	*/
#define  TXD_SIZ_LNTRXBUF 	0x00055000	/* Size of TX,RX,reserved buf per link*/

#define M_TXD_TXBUF_BASE(cprino)	D_RRH_REG_CM_TX + (cprino * 0x4000)
/* @} */


/*!
 * @name Buffer request result value defination	
 * @{  
 */
#define TXD_MAX_RETRY		10000	/* Max retry time of reading SET bit TBD*/

#define TXD_TRUE			1		/* TRUE					             */
#define TXD_FALSE			0		/* False				                    */
/* @} */


/*!
 * @name Mask bits of registers defination
 * @{  
 */
#define TXD_MSK_ZERO		0		/* Zero								*/
#define TXD_MSK_FFFF		0xFFFF	/* Mask 0xFFFF							*/
#define TXD_MSK_TXSET		0x00008000	/* SET bit of LAPB transmission buffer control reg*/

#define TXD_MSK_TXBLK		0x0007	/* SET bit of LAPB transmission buffer control reg*/
#define TXD_MSK_TXFIN		0x00010000	/* SET bit of LAPB transmission buffer control reg*/

#define TXD_MSK_CID			0x0038	/* Card ID bits							*/
/* @} */


/*!
 * @name Interval between register addresses 
 * @{  
 */
#define TXD_ITV_TXCONT		0x0100	/*  Interval between LAPB tx control registers */
#define TXD_ITV_TXDTL_LK	0x0100	/*  Interval between LAPB tx data length registers (By link)*/
#define TXD_ITV_TXDTL_BK	2		/*  Interval between LAPB tx data length registers (By bank)*/
/* @} */

/*!
 * @name simulator mode 
 * @{  
 */
#define TXD_SIMU_MODE_OFF	(0)
#define TXD_SIMU_MODE_ON_L3	(1)

/* @} */

#endif	/*TM_TX_DEF*/

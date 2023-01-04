/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  Tm_rx_def.h
 * @brief RXS constant definition.
 * @date  2008/07/22 FFCS)Zouw Create
 * @date  2013.11.22 FFCS)hongj modify for zynq.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2015
 */
 /****************************************************************************/
#ifndef    TM_RX_DEF
#define    TM_RX_DEF

/*!
 * @name Max number of frames that could be read during task's running period TBD
 * @{  
 */
#define  RXD_FRMLIM_TRIF 	12
#define  RXD_FRMLIM_MDIF  	5
/* @} */


/*!
 * @name Main matrix table definition
 * @{  
 */
#define  RXD_MX_STATE  		2
#define  RXD_MX_EVENT  		3
/* @} */


/*!
 * @name Task internal event code
 * @{  
 */
#define RXD_EV_INIREQ		0		/* Initialization Request			*/
#define RXD_EV_RCVNTC_POL	1		/* L1 Data Receiving Notice (from POLLING task)	*/
#define RXD_EV_RCVNTC_MKM	2		/* L1 Data Receiving Notice (from make MT management)	*/
/* @} */


/*!
 * @name Return value defination
 * @{  
 */
#define  RXD_RET_ABNORMAL	1		/* Return fault					*/
#define  RXD_RET_NORMAL  	0		/* Return normal				*/
/* @} */




/*!
 * @name Task status defination
 * @{  
 */
#define  RXD_ST_INIT  		0		/* Init state		*/ 
#define  RXD_ST_USE  		1		/* Use state		*/ 
/* @} */

/*!
 * @name LPB rx buffer character values 
 * @{  
 */
#define  RXD_BNBUF_SIZE  	0x2A00				/* Size of each bank buffer(0d10752) */
#define  RXD_RXBUF_BASE  	D_RRH_REG_DDR_RX	/* Base of LAPB rx buffer            */
#define  RXD_SIZ_LNTRXBUF  	0x00055000			/* Size of TX and RX buffer per link */

#define M_RXD_RXBUF_BASE(cprino)	D_RRH_REG_DDR_RX + (cprino * 0x20000)

/* @} */

/*!
 * @name boolean value
 * @{  
 */
#define RXD_TRUE			1		/* TRUE			*/
#define RXD_FALSE	 		0		/* FALSE		*/
/* @} */

/*!
 * @name BANK NUM
 * @{  
 */
#define RXD_BD_MDEIF_BANK 		8
/* @} */

/*!
 * @name Mask bits of registers defination	
 * @{  
 */
#define RXD_MSK_ONE			1		/* One			*/
#define RXD_MSK_ZERO		0		/* Zero			*/
#define RXD_MSK_RDYCLR		0x7FFF	/* Clear RDY bit of LAPB RX data control register*/
#define RXD_MSK_RXEN		0x00008000	/* En bit of bank no notification register	*/
#define RXD_MSK_BKNTC		0x00000007	/* Bank no bits of bank no notification register*/
#define RXD_MSK_DTL			0x3FFF	/* Data length bits of LAPB RX data control register value*/
#define RXD_MSK_CID			0x0038	/* Card ID bits							*/
#define RXD_MSKRXNTC_T 		0x003F	/* Valid bits of LAPB receive notification register(TRIF) Only_Six_Links */
#define RXD_MSKRXNTC_M 		0x0001	/* Valid bits of LAPB receive notification register(MDIF)*/
/* @} */


/*!
 * @name Interval between register addresses 	
 * @{  
 */
#define RXD_ITV_RXCONT_LK	0x0100	/* Interval between LAPB RX data control registers(By link)*/
#define RXD_ITV_RXCONT_BK	4		/* Interval between LAPB RX data control registers(By bank)*/
#define RXD_ITV_RXSTA		0x0100	/* Interval between LAPB RX status registers*/
#define RXD_ITV_BKNTC		0x0100	/* Interval between LAPB RX bank no notification registers*/
/* @} */

/*
 * @name DDR Reception timer
 * @{
 */
#define RXD_FLG_TIME_100US		100
#define RXD_FLG_TIME_POLLING	10
/* @} */

#endif	/* TM_RX_DEF	*/

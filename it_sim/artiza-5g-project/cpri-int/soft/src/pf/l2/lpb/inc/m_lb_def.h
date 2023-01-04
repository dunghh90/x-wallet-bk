/*!
 * @skip    $Id$
 * @file    m_lb_def.h
 * @brief   LAPB define定義
 * @date    2008.07.22 FFCS)Shihzh create.
 * @date    2013.11.18 FFCS)hongj modify for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

#ifndef		M_LB_DEF
#define		M_LB_DEF                                        /*!< */


/****************************************************************************/
/*	main matrix table definition(MDIF card)									*/
/****************************************************************************/
#define	LBD_MX_STATE		3                               /*!< maximum state number			*/
#define	LBD_MX_EVENT		5                               /*!< maximum event number			*/

#define			LBD_L2OWNADDR	0x01                  /*!< own address */
#define			LBD_L2PERADDR	0x03                  /*!< peer address */

											/*********************************/
											/*          L3 message number    */
											/*********************************/
#define			LBD_L3MSGNUM	D_L2_L3MSGNUM              /*!< L3 message number       */


#define			LBD_SUBTASKNUM	0x05                       /*!< L2 state subscriber number  */

#endif/*LB_DEF*/


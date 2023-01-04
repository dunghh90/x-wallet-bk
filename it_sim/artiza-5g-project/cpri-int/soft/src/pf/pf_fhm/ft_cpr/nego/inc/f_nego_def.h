/*!
* @skip $Id:$
* @file f_nego_def.h
* @brief pf_nego thread Define Header
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-
*/
/** @addtogroup RRH_PF_CPR
 *  @{
 */
#ifndef F_NEGO_DEF_H
#define F_NEGO_DEF_H

/** @name thread state define */
/* @{ */
#define D_NEGO_THRDSTA_INIT	0			/* Initial Status	*/
#define D_NEGO_THRDSTA_IDLE	1			/* Idle Status		*/
#define D_NEGO_THRDSTA_RUN	2			/* Run Status		*/
/* @} */

/** @name thread event define */
/* @{ */
#define D_NEGO_EVT_START_IND				0
#define D_NEGO_EVT_STOP_IND					1
#define D_NEGO_EVT_TIMEOUT_NTC 				2
#define D_NEGO_EVT_RESTART_IND	 			3
/* @} */

/** @name thread event define */
/* @{ */
#define D_NEGO_STATMAX		3
#define D_NEGO_EVTMAX		4
/* @} */

/** @name runnning history */
/* @{ */
#define D_NEGO_RECVIEVEFLG	0
#define D_NEGO_SENDIEVEFLG	1
#define D_NEGO_RUNHIS_FACENUM_MAX	512
/* @} */

/** @name hard set flag */
/* @{ */
#define D_NEGO_HARD_SET_LYNCUP_START		0
#define D_NEGO_HARD_SET_HFNSYNC_START		1
#define D_NEGO_HARD_SET_LYNCUP_RESTART 		2
#define D_NEGO_HARD_SET_HFNSYNC_RESTART	 	3
/* @} */

/* CPP HFNSYNC Wait Time */
#define D_NEGO_HFNSYNC_WAIT_TIME_150MSEC	150

/* NEGO Time 1shot */
#define D_NEGO_ONE_SHOT_TIME_10MSEC			10

/* NEGO Time for Each Bit Rate */
#define D_NEGO_BIT_RATE_TIME_4SEC			4

/* CPP HFNSYNC continuous times */
#define D_NEGO_HFNSYNC_TIMES				3

/* CPP PHY Done Time polling 10msec */
#define D_NEGO_PHYDONE_TIME_POLLING			10

/* CPP PHY Done Time 100msec */
#define D_NEGO_PHYDONE_TIME_100MSEC			100

#endif
/** @} */

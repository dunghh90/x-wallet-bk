/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	tm_ls_def.h
 *  @brief	LAN Message Sending task Common Definition  
 *  @date   2008/07/30 FFCS)zhengmh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/******************************************************************************************************************************/


#ifndef TM_LS_DEF
#define TM_LS_DEF

/****************************************************************************/
/*	Main matrix table definition											*/
/****************************************************************************/
#define LSD_MAX_STATE		2			/*	maximum state number*/
#define LSD_MAX_EVENT		3			/*	maximum event number*/

/****************************************************************************/
/*	State flag definition													*/
/****************************************************************************/
#define LSD_STA_INIT		0			/*	Init state*/
#define LSD_STA_RUN			1			/*	Running state	*/

/****************************************************************************/
/*	Task event number definition											*/
/****************************************************************************/
#define LSD_EVT_INIREQ		0			/*	Init request*/
#define LSD_EVT_SNDREQ	 	1	        /*	Tcp data transmission request*/
#define LSD_EVT_RELREQ  	2			/*	Tcp link disconnection request*/

#if 0
#define SOCK_SD_ZERO		0			/*socket send function return 0*/
#define SOCK_SHUT_ALL		2			/*shutdown socket read and send*/
#define SOCK_SHUT_ZERO		0			/*socket shutdown function return 0*/
#endif
#endif/*TM_LS_DEF*/


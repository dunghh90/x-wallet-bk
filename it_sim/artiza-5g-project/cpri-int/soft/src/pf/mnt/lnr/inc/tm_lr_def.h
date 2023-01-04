/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	tm_lr_def.h
 *  @brief	LAN Message Receiving task Common Definition
 *  @date   2008/07/29 FFCS)zhengmh create
 *  @date   2013/11/22 FFCS)zhaodx modify for zynq
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*****************************************************************************/



#ifndef TM_LR_DEF
#define TM_LR_DEF

/****************************************************************************/
/*	Main matrix table definition											*/
/****************************************************************************/
#define LRD_MAX_STATE		2			/*	maximum state number*/
#define LRD_MAX_EVENT		2			/*	maximum event number*/

/****************************************************************************/
/*	State flag definition													*/
/****************************************************************************/
#define LRD_STA_INIT			0			/*	init state	*/
#define LRD_STA_RUN			1			/*	running state*/

#ifndef OPT_RRH_ZYNQ_REC
#define LRD_SRVCOUNT		9			/*max server socket*/
#define LRD_SRVSIMULATOR	0			/*server server socket offet*/
#define LRD_SRVSIMULATOR_RE1	1			/*server_re1 server socket offet*/
#define LRD_SRVSIMULATOR_RE2	2			/*server_re2 server socket offet*/
#define LRD_SRVSIMULATOR_RE3	3			/*server_re3 server socket offet*/
#define LRD_SRVMONITOR		4			/*monitr server socket offet*/
#define LRD_SRVMONITOR_RE1	5			/*monitr_re1 server socket offet*/
#define LRD_SRVMONITOR_RE2	6			/*monitr_re2 server socket offet*/
#define LRD_SRVMONITOR_RE3	7			/*monitr_re3 server socket offet*/
#define LRD_SRVOPTMT		8			/*OPT MT server socket offet*/

#define LRD_MAXDURATION	10000		/*10000 misecond*/
#define LRD_MAXFD			10
#else
#define LRD_SRVCOUNT		35			/*max server socket*/
#define LRD_SRVSIMULATOR	0			/*server server socket offet*/
#define LRD_SRVSIMULATOR_RE1 	1 			/*server_re1  server socket offet*/
#define LRD_SRVSIMULATOR_RE2 	2 			/*server_re2  server socket offet*/
#define LRD_SRVSIMULATOR_RE3 	3 			/*server_re3  server socket offet*/
#define LRD_SRVSIMULATOR_RE4 	4 			/*server_re4  server socket offet*/
#define LRD_SRVSIMULATOR_RE5 	5 			/*server_re5  server socket offet*/
#define LRD_SRVSIMULATOR_RE6 	6 			/*server_re6  server socket offet*/
#define LRD_SRVSIMULATOR_RE7 	7 			/*server_re7  server socket offet*/
#define LRD_SRVSIMULATOR_RE8 	8 			/*server_re8  server socket offet*/
#define LRD_SRVSIMULATOR_RE9 	9 			/*server_re9  server socket offet*/
#define LRD_SRVSIMULATOR_RE10	10			/*server_re10 server socket offet*/
#define LRD_SRVSIMULATOR_RE11	11			/*server_re11 server socket offet*/
#define LRD_SRVSIMULATOR_RE12	12			/*server_re12 server socket offet*/
#define LRD_SRVSIMULATOR_RE13	13			/*server_re13 server socket offet*/
#define LRD_SRVSIMULATOR_RE14	14			/*server_re14 server socket offet*/
#define LRD_SRVSIMULATOR_RE15	15			/*server_re15 server socket offet*/
#define LRD_SRVSIMULATOR_RE16	16			/*server_re16 server socket offet*/
#define LRD_SRVMONITOR		17			/*monitr server socket offet*/
#define LRD_SRVMONITOR_RE1 	18 			/*monitr_re1  server socket offet*/
#define LRD_SRVMONITOR_RE2 	19 			/*monitr_re2  server socket offet*/
#define LRD_SRVMONITOR_RE3 	20 			/*monitr_re3  server socket offet*/
#define LRD_SRVMONITOR_RE4 	21 			/*monitr_re4  server socket offet*/
#define LRD_SRVMONITOR_RE5 	22 			/*monitr_re5  server socket offet*/
#define LRD_SRVMONITOR_RE6 	23 			/*monitr_re6  server socket offet*/
#define LRD_SRVMONITOR_RE7 	24 			/*monitr_re7  server socket offet*/
#define LRD_SRVMONITOR_RE8 	25 			/*monitr_re8  server socket offet*/
#define LRD_SRVMONITOR_RE9 	26 			/*monitr_re9  server socket offet*/
#define LRD_SRVMONITOR_RE10	27			/*monitr_re10 server socket offet*/
#define LRD_SRVMONITOR_RE11	28			/*monitr_re11 server socket offet*/
#define LRD_SRVMONITOR_RE12	29			/*monitr_re12 server socket offet*/
#define LRD_SRVMONITOR_RE13	30			/*monitr_re13 server socket offet*/
#define LRD_SRVMONITOR_RE14	31			/*monitr_re14 server socket offet*/
#define LRD_SRVMONITOR_RE15	32			/*monitr_re15 server socket offet*/
#define LRD_SRVMONITOR_RE16	33			/*monitr_re16 server socket offet*/
#define LRD_SRVOPTMT		34			/*OPT MT server socket offet*/

#define LRD_MAXDURATION	10000		/*10000 misecond*/
#define LRD_MAXFD			32
#endif
/****************************************************************************/
/*	Task event number definition											*/
/****************************************************************************/
#define LRD_GW_IPMASK		0xffffff00	/*GW IP address mask */
#define LRD_GW_DCLS			1			/*Gateway No.4 byte	*/ 
#define LRD_MAXUNIT			1500		/*MAX LAN unit*/
#define LRD_EVT_INIREQ		0			/*Init request*/
#define LRD_EVT_TCPSTRUP  	1			/*Tcp server Initialization request*/
#define LRD_TCP_INVALIDHANDLE -1 		/*Invalid TCP link handle*/

#define SOL_TCP					6
/****************************************************************************/
/*	Tcp data receiveing result												*/
/****************************************************************************/
#define LRD_TCPRD_ERROR 		-1  	/*Fail to receive tcp data*/
#define LRD_TCPRD_TIMEOUT 		0 		/*Timerout occurence */
#define LRD_TCPRD_OK			1		/*Success in receiving tcp data*/

#define LRD_FCC2_MASK			0x40000000	/*FCC2 interrupt mask*/

#define MII_STAT_REG	  		1			/* Status register*/
#define MII_LINKUP				0x0004	  /* The Link is up*/
#endif/*TM_LR_DEF*/


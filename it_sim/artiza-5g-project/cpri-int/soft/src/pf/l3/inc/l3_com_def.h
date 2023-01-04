/*!
 * @skip  $ld:$
 * @file  l3_com_def.h
 * @brief common definition
 * @date  2013/11/15 FFCS)linb create
 * @date  2015/08/10 TDIPS)maruyama modify 起動スレッド最大数(D_L3_THRDNO_MAX)を実際の起動数に合わせる
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_L3
 * @{
 */
#ifndef L3_COM_DEF_H
#define L3_COM_DEF_H

 /*!
 * @note Thread name
 */
#define D_L3_THRDNAME_MAIN		"L3_main"		/* L3_main name		*/
#define D_L3_THRDNAME_CPR		"L3_cpr" 		/* L3_cpr  name		*/
#define D_L3_THRDNAME_RCT		"L3_rct "		/* L3_rct  name		*/
#define D_L3_THRDNAME_DLM		"L3_dlm "		/* L3_dlm  name		*/
#define D_L3_THRDNAME_LOG		"L3_log "		/* L3_log  name		*/
#define D_L3_THRDNAME_DIA		"L3_dia "		/* L3_dia  name		*/
#define D_L3_THRDNAME_SEND		"L3_send"		/* L3_send name		*/
#define D_L3_THRDNAME_RECV		"L3_recv"		/* L3_recv name		*/


/* @} */

 /*!
 * @note Thread priority
 */
#define D_L3_PRIORITY_MAIN			95	/* L3_main priority		*/
#define D_L3_PRIORITY_CPR			65	/* L3_cpr priority		*/
#define D_L3_PRIORITY_RCT			60	/* L3_rct priority		*/
#define D_L3_PRIORITY_DLM			30	/* L3_dlm priority		*/
#define D_L3_PRIORITY_LOG			31	/* L3_log priority		*/
#define D_L3_PRIORITY_DIA			31	/* L3_dia priority		*/
#define D_L3_PRIORITY_SEND			79	/* L3_send priority		*/
#define D_L3_PRIORITY_RECV			79	/* L3_recv priority		*/
#define D_L3_PRIORITY_DEBG			20/* L3_recv priority		*/

/* @} */

 /*!
 * @note Thread stack
 */
#define D_L3_STACKSIZE_MAIN	 0xA000		/* L3_main stack		*/
#define D_L3_STACKSIZE_CPR	 0xA000		/* L3_cpr  stack		*/
#define D_L3_STACKSIZE_RCT	 0xA000		/* L3_rct  stack		*/
#define D_L3_STACKSIZE_DLM	 0xA000		/* L3_dlm  stack		*/
#define D_L3_STACKSIZE_LOG	 0xA000		/* L3_log  stack		*/
#define D_L3_STACKSIZE_DIA	 0xA000		/* L3_dia  stack		*/
#define D_L3_STACKSIZE_SEND	 0xA000		/* L3_send stack		*/
#define D_L3_STACKSIZE_RECV	 0xA000		/* L3_recv stack		*/
#define D_L3_STACKSIZE_DEBG	 0xA000		/* L3_debg stack		*/
/* @} */

 /*!
 * @note Event NO
 */

/* main event */
#define D_L3_EVTNO_PROCESS_CREATE          	0x00
#define D_L3_EVTNO_TASK_INIT_REP           	0x01
#define D_L3_EVTNO_SYSTEM_INIT_NOTI       	0x02

/* @} */

 /*!
 * @note Re-define Thread ID
 */

#define D_RRH_THDID_L3_MAIN		0x00000050
#define D_RRH_THDID_L3_CPRI		0x00000051
#define D_RRH_THDID_L3_RCT		0x00000052
#define D_RRH_THDID_L3_DLM		0x00000053
#define D_RRH_THDID_L3_LOG		0x00000055
#define D_RRH_THDID_L3_DIA		0x00000056
#define D_RRH_THDID_L3_SEND		0x00000057
#define D_RRH_THDID_L3_RECV		0x00000058
#define D_RRH_THDID_L3_DBG		0x00000059

#define D_RRH_THDQID_L3_MAIN	1
#define CMD_TSKID_INI			D_RRH_THDQID_L3_MAIN
#define D_RRH_THDQID_L3_CPR		2
#define CMD_TSKID_CPR			D_RRH_THDQID_L3_CPR
#define D_RRH_THDQID_L3_RCT		3
#define CMD_TSKID_RCT			D_RRH_THDQID_L3_RCT
#define D_RRH_THDQID_L3_DLM		4
#define CMD_TSKID_DLM			D_RRH_THDQID_L3_DLM
#define D_RRH_THDQID_L3_DIA		6
#define CMD_TSKID_DIA			D_RRH_THDQID_L3_DIA
#define D_RRH_THDQID_L3_LOG		7
#define CMD_TSKID_LOG			D_RRH_THDQID_L3_LOG
#define D_RRH_THDQID_L3_SND		8
#define CMD_TSKID_SND			D_RRH_THDQID_L3_SND
#define D_RRH_THDQID_L3_RCV		9
#define CMD_TSKID_RCV			D_RRH_THDQID_L3_RCV
#define D_RRH_THDQID_L3_DBG		10
#define CMD_TSKID_DBG			D_RRH_THDQID_L3_DBG
#define D_L3_THRDNO_MAX			8
#define D_RRH_THDQID_L3_LGMK	21
#define CMD_TSKID_LGMK			D_RRH_THDQID_L3_LGMK

#define CMD_TSKID_LPB			D_RRH_PROCQUE_L2

/* @} */


 /*!
 * @note Return code
 */
enum {
	E_L3_RCD_OK = 0,
	E_L3_RCD_NG,
	E_L3_RCD_EPARAM,
	E_L3_RCD_ESIZEOVER,
	E_L3_RCD_ENOQNUM,
	E_L3_RCD_ENOSPACE,
	E_L3_RCD_EADDR,
	E_L3_RCD_EINTR
};
/* @} */

 /*!
 * @note Process number
 */
#define D_L3_PROC_NUM 8
/* @} */

 /*!
 * @note Wrong Process ID
 */
#define D_L3_PROCID_GETNG 0xFFFFFFFF
/* @} */

 /*!
 * @note Wrong Thread ID
 */
#define D_L3_THRDID_GETNG 0xFFFFFFFF
/* @} */


 /*!
 * @note Link state
 */
#define D_L3_LINKSTATE_ON		1
#define D_L3_LINKSTATE_OFF		2
#define D_L3_LINKSTATE_ABNORMAL	3
/* @} */

 /*!
 * @note number of cpri message subscription
 */
#define D_L3_CPRIMSGNUM 47
/* @} */

/* USHORT swap */
#define BigLittleSwap16(V)  ((((USHORT)(V) & 0xff00) >> 8) | \
                            (((USHORT)(V) & 0x00ff) << 8))

#define D_SYS_MSGID_ALLTSKININTC							0x10010007			/**< All thread initialize Notification		*/

/**
*
* @name Layer3ｿｿ
*/
/**  */
#define D_L3_STA_4	4
#define D_L3_STA_2	2
#define D_L3_STA_REC_IDLE	6
#define D_L3_STA_REC_USING	7

#endif
/* @} */

/*!
 * @skip  $ld:$
 * @file  mnt_com_def.h
 * @brief common definition
 * @date  2013/11/20 FFCS)zhaodx create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_MNT
 * @{
 */
#ifndef MNT_COM_DEF_H
#define MNT_COM_DEF_H

 /*!
 * @note Thread name
 */
#define D_MNT_THRDNAME_MAIN		"MNT_main"	/*!< MNT_main name		*/
#define D_MNT_THRDNAME_MKM		"MNT_mkm"	/*!< MNT_mkm  name		*/
#define D_MNT_THRDNAME_MTM		"MNT_mtm"	/*!< MNT_mtm  name		*/
#define D_MNT_THRDNAME_MLR		"MNT_mlr"	/*!< MNT_mlr  name		*/
#define D_MNT_THRDNAME_MLS		"MNT_mls"	/*!< MNT_mls  name		*/
#define D_MNT_THRDNAME_LNR		"MNT_lnr"	/*!< MNT_lnr  name		*/
#define D_MNT_THRDNAME_LNS		"MNT_lns"	/*!< MNT_lns  name		*/
#define D_MNT_THRDNAME_DBG		"MNT_dbg"	/*!< MNT_dbg  name		*/
#define D_MNT_THRDNAME_SEND		"MNT_send"	/*!< MNT_send name		*/
#define D_MNT_THRDNAME_RECV		"MNT_recv"	/*!< MNT_recv name		*/

/* @} */

 /*!
 * @note Thread priority
 */
#define D_MNT_PRIORITY_MAIN		90			/*!< MNT_main priority	*/
#define D_MNT_PRIORITY_MKM		38			/*!< MNT_mkm  priority	*/
#define D_MNT_PRIORITY_MTM		39			/*!< MNT_mtm  priority	*/
#define D_MNT_PRIORITY_MLR		75			/*!< MNT_mlr  priority	*/
#define D_MNT_PRIORITY_MLS		76			/*!< MNT_mls  priority	*/
#define D_MNT_PRIORITY_LNR		75			/*!< MNT_lnr  priority	*/
#define D_MNT_PRIORITY_LNS		76			/*!< MNT_lns  priority	*/
#define D_MNT_PRIORITY_DBG		38			/*!< MNT_lns  priority	*/
#define D_MNT_PRIORITY_SEND		79			/*!< MNT_send priority	*/
#define D_MNT_PRIORITY_RECV		79			/*!< MNT_recv priority	*/
/* @} */

 /*!
 * @note Thread stack
 */
#define D_MNT_STACKSIZE_MAIN	0			/*!< MNT_main stack		*/
#define D_MNT_STACKSIZE_MKM		0			/*!< MNT_mkm  stack		*/
#define D_MNT_STACKSIZE_MTM		0			/*!< MNT_mtm  stack		*/
#define D_MNT_STACKSIZE_MLR		0			/*!< MNT_mlr  stack		*/
#define D_MNT_STACKSIZE_MLS		0			/*!< MNT_mls  stack		*/
#define D_MNT_STACKSIZE_LNR		0			/*!< MNT_lnr  stack		*/
#define D_MNT_STACKSIZE_LNS		0			/*!< MNT_lns  stack		*/
#define D_MNT_STACKSIZE_DBG		0			/*!< MNT_lns  stack		*/
#define D_MNT_STACKSIZE_SEND	0			/*!< MNT_send stack		*/
#define D_MNT_STACKSIZE_RECV	0			/*!< MNT_recv stack		*/
/* @} */                              

                                                    
 /*!
 * @note Event NO
 */

/* main event */
#define D_MNT_EVTNO_PROCESS_CREATE          0x00
#define D_MNT_EVTNO_TASK_INIT_REP           0x01
#define D_MNT_EVTNO_SYSTEM_INIT_NOTI        0x02

/* @} */

 /*!
 * @note Return code
 */
enum {
	E_MNT_RCD_OK = 0,
	E_MNT_RCD_NG,
	E_MNT_RCD_EPARAM,
	E_MNT_RCD_ESIZEOVER,
	E_MNT_RCD_ENOQNUM,
	E_MNT_RCD_ENOSPACE,
	E_MNT_RCD_EADDR,
	E_MNT_RCD_EINTR
};
/* @} */

 /*!
 * @note Process number
 */
#define D_MNT_PROC_NUM 6
/* @} */

 /*!
 * @note Wrong Process ID
 */
#define D_MNT_PROCID_GETNG 0xFFFFFFFF
/* @} */

 /*!
 * @note Wrong Thread ID
 */
#define D_MNT_THRDID_GETNG 0xFFFFFFFF
/* @} */

 /*!
 * @note number of cpri message subscription
 */
#ifndef OPT_RRH_ZYNQ_REC
#define MBD_CPRIMSGNUM 21
#else
#define MBD_CPRIMSGNUM 105
#endif
/* @} */



/*!
 * @note thread ID
 * @date 2015/09/14 TDI)satou D_RRH_THDID_MT_MAX 10→7
 */
#define D_RRH_THDID_MT_MAIN			0x00000060	/**< MNT Main Thread						*/
#define D_RRH_THDID_MT_MNG			0x00000061	/**< MT Management Thread					*/
#define D_RRH_THDID_MT_CTRL			0x00000062	/**< CPRI-MT and RE-MT Control Thread		*/
#define D_RRH_THDID_MT_LAN_MT_RECV	0x00000063	/**< MT LAN Message Sending Thread			*/
#define D_RRH_THDID_MT_LAN_MT_SEND	0x00000064	/**< MT LAN Message Receiving Thread		*/
#define D_RRH_THDID_MT_LAN_RECV		0x00000065	/**< LAN Message Sending Thread				*/
#define D_RRH_THDID_MT_LAN_SEND		0x00000066	/**< LAN Message Receiving Thread			*/
#define D_RRH_THDID_MT_RECV			0x00000067	/**< MNT Recv Thread						*/
#define D_RRH_THDID_MT_SEND			0x00000068	/**< MNT Send Thread						*/
#define D_RRH_THDID_MT_DBG			0x00000069	/**< MNT dbg Thread							*/
#define D_RRH_THDID_MT_MAX          7           /**< MNT dbg Thread                         */
/* @} */

/*!
 * @note define thread queue ID
 */
#define CMD_TSKID_INI			1        					/**< MNT Main Thread						*/
#define CMD_TSKID_MTM			2					        /**< MT Management Thread					*/
#define CMD_TSKID_MKM			3					        /**< CPRI-MT and RE-MT Control Thread		*/
#define CMD_TSKID_MLR			4							/**< MT LAN Message Sending Thread			*/
#define CMD_TSKID_MLS			5							/**< MT LAN Message Receiving Thread		*/
#define CMD_TSKID_LNR			6						    /**< LAN Message Sending Thread				*/
#define CMD_TSKID_LNS			7						    /**< LAN Message Receiving Thread			*/
#define CMD_TSKID_RCV			8							/**< MNT Recv Thread						*/
#define CMD_TSKID_SND			9							/**< MNT Send Thread						*/
#define CMD_TSKID_DBG			10							/**< MNT dbg Thread							*/
#define CMD_TSKID_LPB			D_RRH_PROCQUE_L2			/**< L2 Process								*/

#define CMD_TSKID_LGMK			21					        /**< Log make Thread						*/
/* @} */

#define D_SYS_MSGID_ALLTSKININTC			0x10010007			/**< All thread initialize Notification		*/

#endif
/* @} */

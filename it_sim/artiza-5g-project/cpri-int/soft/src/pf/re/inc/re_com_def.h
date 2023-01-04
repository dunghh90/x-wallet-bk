/*!
 * @skip  $ld:$
 * @file  re_com_def.h
 * @brief common definition
 * @date  2015/--/--
 * @date  2015/08/04 TDIPS)maruyama DBGスレッド追加、L2とのMSG定義追加
 * @date  2015/08/05 TDIPS)maruyama All thread initialize Notification定義追加
 * @date  2015/10/02 TDIPS)sasaki update
 * @date  2021/02/04 M&C)Huan.dh update
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
#ifndef RE_COM_DEF_H
#define RE_COM_DEF_H

/**
 * @addtogroup TRIF_COM
 * @{
 */

/**
 * @name Thread name
 * @note Thread name
 * @{
 */
#define D_RE_THRDNAME_MAIN          "RE_main"           /**< RE_main name */
#define D_RE_THRDNAME_REC           "RE_rec"            /**< RE_rec  name */
#define D_RE_THRDNAME_RCV           "RE_rcv"            /**< RE_rcv  name */
#define D_RE_THRDNAME_SEND          "RE_send"           /**< RE_send name */
#define D_RE_THRDNAME_DBG           "RE_dbg"            /**< RE_dbg name */
/** @} */

/**
 * @name Thread priority
 * @note Thread priority
 * @{
 */
#define D_RE_PRIORITY_MAIN          95                  /**< RE_main priority */
#define D_RE_PRIORITY_REC           60                  /**< RE_rec  priority */
#define D_RE_PRIORITY_RCV           79                  /**< RE_rcv  priority */
#define D_RE_PRIORITY_SEND          79                  /**< RE_send  priority */
#define D_RE_PRIORITY_DBG           20                  /**< RE_dbg  priority */
/** @} */

/**
 * @name Thread stack
 * @note Thread stack
 * @{
 */
#define D_RE_STACKSIZE_MAIN         0                   /**< RE_main stack  */
#define D_RE_STACKSIZE_REC          0                   /**< RE_rec  stack  */
#define D_RE_STACKSIZE_RCV          0                   /**< RE_rcv  stack  */
#define D_RE_STACKSIZE_SEND         0                   /**< RE_send  stack */
#define D_RE_STACKSIZE_DBG          0                   /**< RE_dbg  stack */
/** @} */

/**
 * @name Thread No
 * @note Thread No
 * @{
 */
#define D_RE_THRDNO_MAIN            0                   /**< RE_main thread NO  */
#define D_RE_THRDNO_REC             1                   /**< RE_rec  thread NO  */
#define D_RE_THRDNO_RCV             2                   /**< RE_rcv  thread NO  */
#define D_RE_THRDNO_SEND            3                   /**< RE_send  thread NO */
#define D_RE_THRDNO_DBG             4                   /**< RE_dbg  thread NO */
#define D_RE_THRDNO_MAX             5                   /**< RE thread MAX      */
/** @} */

/**
 * @name define thread ID
 * @note define thread ID
 * @{
 */
#define D_RRH_THDID_RE_MAIN         0x00000080          /**< RE Main Thread */
#define D_RRH_THDID_RE_REC          0x00000083          /**< RE rec Thread  */
#define D_RRH_THDID_RE_RCV          0x00000081          /**< RE rcv Thread  */
#define D_RRH_THDID_RE_SEND         0x00000082          /**< RE send Thread */
#define D_RRH_THDID_RE_DBG          0x00000084          /**< RE send Thread */

#define CMD_TSKID_INI               1                   /**< Main Thread            */
#define CMD_TSKID_REC               2                   /**< REC Management Thread  */
#define CMD_TSKID_RCV               3                   /**< RCV Management Thread  */
#define CMD_TSKID_SEND              4                   /**< SEND Management Thread */
#define CMD_TSKID_DBG               5                   /**< DBG Management Thread */

#define CMD_TSKID_LGMK             21                   /**< log Make Thread */
/** @} */



/**
 * @name L2とのMSG定義数
 * @note L2とのMSG定義数
 * @{
 */
#define MBD_REMSGNUM 39		// この値は暫定
/* @} */


#define D_SYS_MSGID_ALLTSKININTC			0x10010007			/**< All thread initialize Notification		*/

/** @} TRIF_COM */

#endif /* RE_COM_DEF_H */

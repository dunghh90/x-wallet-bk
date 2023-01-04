/*!
 * @skip		$ld:$
 * @file		f_cmn_cmu_typ.h
 * @brief		cmu 構造体宣言
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#ifndef F_CMU_TYP_H
#define F_CMU_TYP_H

/**
 * @brief	msg manage table
 * @note	MSG用テーブル
 * @{
 * 
 */
typedef struct{
	T_RRH_HEAD				thdif;								/**< send msg head					*/
	UCHAR					data[D_CMU_DATA_MAX];				/**< データ部						*/
}T_CMU_MSG_MNG;
/* @} */

/**
 * @brief	Thread manage table
 * @note	自Thread情報保持用テーブル
 * @{
 * 
 */
typedef struct{
	UINT				uiProcID;								/**< プロセスID						*/
	UINT				uiProcQID;								/**< プロセスQID					*/
	UINT				uiThreadID;								/**< スレッドID						*/
	UINT				uiThreadQID;							/**< スレッドQID					*/
}T_CMU_THREAD_MNG;
/* @} */

/**
 * @brief	route information of message table
 * @note	route information of message table is ..
 * 
 */
typedef struct{
	UINT				event_id;								/**< EventID									*/
	UINT				event_id_sub;							/**< SignalKind									*/
	UINT				dst_que_id;								/**< 送信先QID(プロセスQID or スレッドQID)		*/
	UINT				dummy;									/**< dummy 										*/
}T_CMU_ROUTE_INFO;


typedef struct {
	pthread_mutex_t		mutex;									/**< mutexオブジェクト							*/
	UINT				infoNum;								/**< テーブル登録情報数							*/
	T_CMU_ROUTE_INFO	info[D_CMU_TABLE_MAX];					/**< メッセージ送信先情報管理テーブル			*/
} T_CMU_ROUTE_MNG;


/*!
 * @name CMUスレッド起動用構造体
 * @note CMUスレッド起動時に渡す構造体
 * @{
 */
typedef struct{
	T_CMU_ROUTE_INFO	tRouteInfo[D_CMU_TABLE_MAX];			/**< メッセージ送信先情報管理テーブル			*/
	UINT				uiProcID;								/**< プロセスID									*/
	UINT				uiProcQID;								/**< プロセスQID								*/
	UINT				uiThreadID;								/**< スレッドID									*/
	UINT				uiThreadQID;							/**< スレッドQID								*/
	UINT				uidstThreadQID;							/**< スレッドQID(送信元)						*/
	UINT				uiThreadInfo;							/**< スレッド情報(0:recv, 1:send)				*/
	CHAR				uiThreadName[D_CMU_NAME_NUM];			/**< スレッド名(ex:"pf_send","l2_recv")			*/
}T_CMU_CREATE_THREAD;
/* @} */


/**
 * @brief	route information of message table
 * @note	route information of message table is ..
 * 
 */
typedef struct{
	UINT				msgId;									/**< msgId										*/
	UINT				dst_que_id;								/**< 送信先QID(プロセスQID or スレッドQID)		*/
}T_CMU_ROUTE_MPINFO;

/*!
 * @name CMUスレッド起動用構造体
 * @note CMUスレッド起動時に渡す構造体
 * @{
 */
typedef struct{
	UINT				tRouteInfoNum;
	T_CMU_ROUTE_MPINFO	tRouteInfo[D_CMU_MPTABLE_MAX];			/**< メッセージ送信先情報管理テーブル			*/
	USHORT				portNo;									/**< プロセスID									*/
	CHAR				ipAddr[16];								/**< プロセスID									*/
	UINT				uiProcID;								/**< プロセスID									*/
	UINT				uiProcQID;								/**< プロセスQID								*/
	UINT				uiThreadID_send;						/**< スレッドID									*/
	UINT				uiThreadID_recv;						/**< スレッドID									*/
	UINT				uiThreadQID_send;						/**< スレッドQID								*/
	UINT				uiThreadQID_recv;						/**< スレッドQID								*/
	UINT				uidstThreadQID;							/**< スレッドQID(送信元)						*/
	CHAR				uiThreadName_send[D_CMU_NAME_NUM];		/**< スレッド名(ex:"pf_send","l2_recv")			*/
	CHAR				uiThreadName_recv[D_CMU_NAME_NUM];		/**< スレッド名(ex:"pf_send","l2_recv")			*/
	VOID* 				msiHandle;								/**< スレッド名(ex:"pf_send","l2_recv")			*/			
}T_CMU_CREATE_THREAD_MP;
/* @} */




#endif
/* @} */

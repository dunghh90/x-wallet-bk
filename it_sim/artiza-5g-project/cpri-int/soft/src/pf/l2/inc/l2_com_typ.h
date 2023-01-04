/*!
 * @skip  $ld:$
 * @file  l2_com_typ.h
 * @brief common structure definition
 * @date  2013/03/04 FFCS)hongj create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_L2
 */
#ifndef L2_COM_TYP_H
#define L2_COM_TYP_H

/*!
 * @brief Clear register.
 */
typedef struct
{
	CMT_TSKIF_HEAD header;
}L2_SUPERVISION_NOTI;
/* @} */

/*!
 * @brief Make L3 test on.
 */
typedef struct
{
	CMT_TSKIF_HEAD header;
	USHORT link_num;
}L2_L3_TEST_NOTI;
/* @} */

/*!
 *  @brief   Little endian <-> Big endian
 *  @note    Little endian <-> Big endian
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**< タスク共通ヘッダ			 */
	union
	{
	    CMT_CPRISIGRCV_INF		cprircv_inf;		/**< CPRI信号受信情報部	 */
	    CMT_CPRISIGSND_INF		cprisnd_inf;		/**< CPRI信号送信情報部	 */
	}m;
	union
	{
		USHORT                  signal_kind;
		CHAR                    body[2];        /**< CPRI信号データ部		 */
	}m2;
}L2_COM_CMT_CPRIF_MAPPING;
/* @} */

/*!
 * @brief Maker MT mode.
 */
typedef struct
{
	USHORT 		monitor;	/* 1:Monitor mode is ON;    0:Monitor mode is OFF;	*/
	USHORT 		simulator;	/* 1:Simulator mode is ON;  0:Simulator mode is OFF;*/
	UINT        rspQID_moni; /* response queue id of start monitor mode	*/
	UINT        rspQID_simu; /* response queue id of start simulator mode	*/
}L2_MKMTMODE;
/* @} */

/*!
 * @brief fault log management information.
 */

typedef struct 
{
	CHAR tblName[16];
	INT tblAdd;
	INT tblSize;
}T_L2_LOGTBL;
/* @} */

/*!
 * @brief L3 signal information.
 */

typedef struct 
{
	USHORT signal_kind;
	UCHAR sigName[16];
}T_L2_L3SIGNAMETBL;
/* @} */

#endif
/* @} */


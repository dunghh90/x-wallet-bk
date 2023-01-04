/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Table Define
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			...
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013
 */
/******************************************************************************************************************************/

#ifndef	F_MAIN_ROM_H
#define	F_MAIN_ROM_H

#include "f_hdl_inc.h"

/** @addtogroup RRH_PF_MAIN
 *  @{
 */

/** @name RRH_PF_MAIN
 *  @{
 */
#define	D_MAIN_THDNAME_LEN	16								/**< thread name len */
/** @} */

/**
 * @brief	thread create manage table
 * @note	thread create manage table
 * 
 */
typedef struct{
	UINT				thdID;								/**< ID					*/
	UINT				SchPolicy;							/**< Scheduling polic	*/
	UINT				thdPriority;						/**< priority			*/
	FUNCPTR				thdFunc;							/**< Function			*/
	UINT				thdStackSize;						/**< stack size			*/
	VOID*				option;								/**< option				*/
	UINT				thdCreateFlag;						/**< 起動フラグ			*/
	CHAR				thdName[32];						/**< Thread Name		*/
}T_MAIN_CRT_EVENT_CTBL;

/** @} */


/**
 * @brief	thread create manage table
 * @note	thread create manage table
 * 
 */
typedef struct{
	CHAR				thdName[D_HDL_THDNAME_LEN];			/**< name			*/
	UINT				thdPriority;						/**< priority		*/
	FUNCPTR				thdFunc;							/**< Function		*/
	UINT				thdStackSize;						/**< stack size		*/
}T_MAIN_CRT_HDLEVENT_CTBL;

/** @} */

/****************************************************************************************************************************/
/** receive msg manage table
 *  @addtogroup RRH_MAIN
 *  @{
 */
/****************************************************************************************************************************/
/**
 * @brief	receive msg manage table
 * @note	受信MSG用テーブル
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF		head;							/**< receive msg head			*/
	UCHAR					data[D_MAIN_DATA_MAX];			/**< データ部					*/
}T_MAIN_RCV_MSG_MNG;

/** @} */

/** @} */


#endif	/* F_MAIN_ROM_H */
/** @} */

/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(RRH API common part)
 *  @note	Struct Definition(RRH API common part)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/******************************************************************************************************************************/

/*!
 * @addtogroup RRH_APL
 * @{
 */
#ifndef F_COM_APLCOM_H
#define F_COM_APLCOM_H

/**
 * @brief	Thread Managment Table
 * @note	Thread Managment Table is ..
 * 
 */
typedef struct{
	UINT	thrdId;
	CHAR	thrdName[16];			/* スレッド�?						*/
	UINT	thrPrio;				/* スレッド POL					*/
	UINT	thrStack;
	VOID*	thrFunc;
	UINT	schedule;			/* スレッドキュー番�?通常)	*/
}T_SYS_THRDINFO;

extern INT apl_main_main(UINT,UINT,const T_SYS_THRDINFO*,UINT);
extern INT apl_com_msgQReceive(UINT,INT,VOID*,INT*,INT*);
extern INT apl_com_msgQSend(UINT,UINT,VOID*,UINT);

extern VOID  apl_com_alm(int ,CHAR *);

#endif
/** @} */

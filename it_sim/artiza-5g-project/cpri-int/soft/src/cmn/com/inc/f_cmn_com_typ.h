/*!
 * @skip  $ld:$
 * @file  f_cmn_com_typ.h
 * @brief 装置共通構造体定義ヘッダ
 * @date  2013/11/26 ALPHA) 近藤 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */
#ifndef F_CMN_COM_TYP_H
#define F_CMN_COM_TYP_H

/****************************************/
/* ALM log file output function tbl		*/
/****************************************/
typedef struct{
	VOID (*func_addr)(UINT,UINT);
} T_CMN_ALMLOG_FUNCTION_TBL;

typedef struct
{
	UINT							func_num;					/**< func num					 */
	T_CMN_ALMLOG_FUNCTION_TBL		func_list[32];				/**< function list				 */
} T_CMN_ALMLOG_FUNCTION_LIST;

typedef struct{
	VOID (*func_addr)(UINT,UINT);
} T_CMN_SV_FUNCTION_TBL;

typedef struct
{
	T_CMN_SV_FUNCTION_TBL		svfunc;					/**< SV function 				 */
} T_CMN_SV_FUNCTION_LIST;

/****************************************/
/* プロセスごとのログ収集情報テーブル	*/
/****************************************/
typedef struct
{
	CHAR							procName[4];				/**< process name				 */
	VOID							(*logSaveFunc)(FILE *fpPtr);	/**< log save function 		 */
	UINT							msgQid;						/**< message queue id 			 */
} T_CMN_LOG_SAVE_INFO;

typedef struct{
	T_RRH_HEAD						thdif;						/**< send msg head					*/
	UINT							logKind;					/**< データ部						*/
}T_CMN_LOG_CREATE_REQ;
/* @} */

#endif
/* @} */

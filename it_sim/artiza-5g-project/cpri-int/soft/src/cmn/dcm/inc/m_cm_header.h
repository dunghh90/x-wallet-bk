/********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_header.h
 *  @brief	Common include head file
 *  @date   2008/07/18 FFCS)Tangj create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/* For multiple include */

#ifndef    M_CM_HEADER
#define    M_CM_HEADER

#include <errno.h>
#include "f_rrh_inc.h"			/* System head file							*/
#include "f_cmn_com_inc.h"		/* Common head file							*/
#include "m_cm_def.h"			/* TRA common definition file				*/
#include "m_cm_typ.h"			/* TRA common structure head file			*/
#include "BPF_RU_HRTM.h"
#include "m_cm_ftyp.h"			/* TRA common prototype head file			*/
#include "m_cm_ext.h"			/* TRA external reference head file			*/
#include "m_cm_Std.h"			/* TRA common standard definition head file	*/
#include "fldc4.h"				/* FA tool head file 						*/
#include "fldccom.h"			/* FA tool head file 						*/
#include "BPF_RM_SVRM.h"		/*!< サーバ運転管理							*/
#include "BPF_RU_IPCM.h"		/*!< プロセス間通信管理						*/
#include "BPF_HM_DEVC.h"		/*!< デバイス制御							*/
#include "BPF_COM_LOG.h"		/*!< LOG機能								*/
#include "BPF_RU_ITCM.h"		/*!< スレッド間通信管理						*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#include "BPF_RU_MBLM.h"	
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#ifdef TRASimu
#include "stub.h"				/* IT1用外部参照ヘッダファイル				*/
#endif /* TRASimu */

#endif	/*M_CM_HEADER*/

/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_header.h
 *  @brief  5GDU-LLS supervision include header file
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2018/08/21 KCN)Kamimoto 5GDU-LLS Update
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2018
 */
/*********************************************************************************/

/* 二重取込み防止 */
#ifndef		F_RSV_HEADER
#define		F_RSV_HEADER
#include "f_du_reg_ctb.h"
#include "f_sys_inc.h"
#include "f_com_inc.h"

#include "rrhApi_Com.h"
#include "rrhApi_Svp.h"
#include "rrhApi_Ext.h"
#include "rrhApi_Dbg.h"
#include "rrhApi_Rfp.h"

#include "f_mpsw_msg_ietf_hardware_def.h"
#include "f_mpsw_msg_ietf_hardware_typ.h"
#include "f_mpsw_msg_o-ran_hardware_def.h"
#include "f_mpsw_msg_o-ran_hardware_typ.h"
#include "f_mpsw_msg_o-ran_fm_def.h"
#include "f_mpsw_msg_o-ran_fm_typ.h"
#include "f_mpsw_msg_o-ran_operations_def.h"
#include "f_mpsw_msg_o-ran_operations_typ.h"

#include "f_eqs_def.h"			/* 5GDU-LLS監視タスク共通デファインヘッダファイル	*/
#include "f_eqs_typ.h"			/* 5GDU-LLS監視タスク共通構造体ヘッダファイル		*/
#include "f_eqs_ftyp.h"			/* 5GDU-LLS監視タスクプロトタイプヘッダファイル	*/
#include "f_eqs_ext.h"			/* 5GDU-LLS監視タスク外部参照ヘッダファイル		*/

#endif	/* M_RS_HEADER */

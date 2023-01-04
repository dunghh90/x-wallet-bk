/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eio_header.h
 *  @brief  5GDU-LLS external IO supervision & cntrol include header file
 *  @date   2019/03/18 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/* 二重取込み防止 */
#ifndef		F_EIO_HEADER
#define		F_EIO_HEADER
#include "f_du_reg_ctb.h"
#include "f_sys_inc.h"
#include "f_com_inc.h"
#include "BPF_RU_HRTM.h"

#include "rrhApi_Com.h"
#include "rrhApi_Svp.h"
#include "rrhApi_Ext.h"
#include "rrhApi_Dbg.h"
#include "rrhApi_Rfp.h"

#include "f_mpsw_msg_o-ran_externalio_def.h"
#include "f_mpsw_msg_o-ran_externalio_typ.h"
#include "f_mpsw_msg_o-ran_fan_def.h"
#include "f_mpsw_msg_o-ran_fan_typ.h"

#include "f_eio_def.h"			/* 5GDU-LLS外部IOタスク共通デファインヘッダファイル	*/
#include "f_eio_typ.h"			/* 5GDU-LLS外部IOタスク共通構造体ヘッダファイル		*/
#include "f_eio_ftyp.h"			/* 5GDU-LLS外部IOタスクプロトタイプヘッダファイル	*/
#include "f_eio_ext.h"			/* 5GDU-LLS外部IOタスク外部参照ヘッダファイル		*/

#endif	/* F_EIO_HEADER */

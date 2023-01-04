/****************************************************************************/
/* NAME		: ダウンロード管理インクルードヘッダファイル( m_dl_header.h )	*/
/*																			*/
/* FUNCTION	: ダウンロード管理で使用する全ヘッダファイルをインクルード		*/
/*																			*/
/* ITEM No	: MB01-002-004-811					ファイル転送				*/
/*																			*/
/* DATE		: 2005/12/12	α)A.Etou			新規作成					*/
/*																			*/
/* UPDATE 	: なし															*/
/*																			*/
/*					All Rights Reserved, Copyright (C) FUJITSU LIMITED 2005	*/
/****************************************************************************/

/* 二重取込み防止 */
#ifndef		M_DL_HEADER
#define		M_DL_HEADER


#include "l3_com_inc.h"

#include "m_dl_def.h"		/* ダウンロード管理タスク共通デファインヘッダ	*/
#include "m_dl_typ.h"		/* ダウンロード管理タスク共通構造体ヘッダ		*/
#include "m_dl_ftyp.h"		/* ダウンロード管理タスクプロトタイプヘッダ		*/
#include "m_dl_ext.h"		/* ダウンロード管理タスク外部参照ヘッダ			*/
#include "m_dl_ram.h"

#include "BPF_RU_HRTM.h"

#include "rrhApi_Inv.h"
#include "rrhApi_File.h"
#include "f_sys_inc.h"

#endif	/* M_DL_HEADER */

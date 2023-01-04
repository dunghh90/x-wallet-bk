/*!
 * @skip  $ld:$
 * @file  f_fdl_typ.h
 * @brief FDLタスク 構造体宣言
 * @date 2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLD
 * @{
 */
#ifndef F_FDL_TYP_H
#define F_FDL_TYP_H

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
typedef struct{
	UINT					thdState;												/* Thread State						*/
}T_FDL_INFO;

typedef struct{
	INT (*eventProcess_addr)(VOID *msgPtr);
}T_FDL_CALLSETRELL_MTXTBL;

typedef struct{
	int						install_State[E_FDL_FLSH_FILEMAX] ;
	char					name[E_FDL_FLSH_FILEMAX][D_SYS_NUM128];
	UINT					slot;
}T_FDL_INSTSTATE ;


typedef struct {
	USHORT					buf_size0 ;
	USHORT					buf_size1 ;
	USHORT					ver_num0 ;
	USHORT					ver_num1 ;
	CHAR					ver_name[D_SYS_NUM16] ;
}T_FDL_EEP_PNT ;

typedef struct {
	UCHAR					build_ver[D_SYS_NUM6] ;
	UCHAR					build_date[D_SYS_NUM4] ;
	UCHAR					build_size[D_SYS_NUM4] ;
	char					build_name[D_SYS_NUM32] ;
}T_FDL_BUILD_DATA ;


typedef struct {
	UINT					count ;
	char					file_name[D_SYS_NUM32] ;
	char					file_ver[D_SYS_NUM8] ;
	char					product_vendor[D_SYS_NUM8] ;
	char					code[D_SYS_NUM16] ;
	char					name[D_SYS_NUM32] ;
	char					buildId[D_SYS_NUM8] ;
	char					bldName[D_SYS_NUM32] ;
	char					bldVersion[D_SYS_NUM8] ;
	char					chksum[D_SYS_NUM128] ;
}T_FDL_MANIFEST_DATA ;

/* @} */

#endif
/* @} */

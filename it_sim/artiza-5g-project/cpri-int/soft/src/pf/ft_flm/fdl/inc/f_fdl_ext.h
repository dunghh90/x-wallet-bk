/*!
 * @skip  $ld:$
 * @file  f_fdl_ext.h
 * @brief FDLタスク 外部参照宣言
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#ifndef F_FDL_EXT_H
#define F_FDL_EXT_H

extern	UINT						f_fdl_swact_data[2];
extern	const	char	f_fdl_Slot_nametbl[D_SYS_NUM2][D_SYS_NUM16] ;
extern	const	char	f_fdl_install_errtbl[E_FDL_ERR_ERRMAX][D_SYS_NUM32] ;
extern	const	T_FDL_EEP_PNT	f_fdl_eepwrit_point[E_FDL_FLSH_FILEMAX] ;
extern	const	char			f_fdl_type_nametbl[D_SYS_NUM3][D_SYS_NUM10] ;
extern	T_FDL_INSTSTATE				f_fdl_inst_state_data ;
extern	T_FDL_BUILD_DATA			f_fdl_inst_build_data ;
extern	char						f_fdl_prdct_id[D_SYS_NUM16];
#endif
/* @} */

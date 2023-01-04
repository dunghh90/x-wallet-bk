/*!
 * @skip		$ld:$
 * @file		f_mti_ext.h
 * @brief		MTIスレッド 外部参照宣言
 * @date		2019/05/13 Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_MTI
 * @{
 */
#ifndef F_MTI_EXT_H
#define F_MTI_EXT_H

extern	T_RRH_MT_INFO*			f_mtiw_InfoTbl;
extern	UCHAR					f_mtiw_productName[12];
extern	UCHAR					f_mtiw_softVer[2];
extern	UCHAR					f_mtiw_hardVer[2];
extern	UINT					f_mtiw_productType;
extern	UINT					f_mtiw_beamIdMax;

extern	UINT					f_mtiw_optdef_val;
extern	UINT					f_mtiw_opt_val;

#endif
/* @} */

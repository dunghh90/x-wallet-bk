/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_com_serchable_vlan_id.c
 *  @brief	TRA common function - hardware alarm report & clear function 
 *  @date   2019/02/18	FJT) h.Yoshida
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*****************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */
/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "f_sys_inc.h"			/* TRA common head file				*/
#include "f_com_inc.h"			/* TRA common head file				*/

/****************************************************************************************************************************/
/*!
 * @brief		f_com_search_vid_chk
 * @note		inputのvlan-idが有効かどうかをチェックし、結果を返す。
 * @param		UCHAR*			[in]seachable_vlans
 * @param		USHORT			[in]vlan_id
 * @return		D_SYS_OK(該当vlan-id有効) / D_SYS_NG(該当vlan-id無効)
 * @retval		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 * {@
 */
/****************************************************************************************************************************/
INT	f_com_search_vid_chk( UCHAR*	searchable_vlans, USHORT	vlan_id )
{
	USHORT							pos;			/* byte位置		*/
	USHORT							mod;			/* bit位置		*/
	UCHAR							mask;			/* mask値		*/
	INT								rtn;			/* 戻り値		*/


	mod  = vlan_id % 8;
	pos  = (vlan_id - mod) / 8;
	mask = 0x80 >> mod;

	if( (*(searchable_vlans + pos) & mask) != mask ) {

		rtn = D_SYS_OK;	/* bitがOFFの場合、該当vlan-id＝有効	*/
	} else {

		rtn = D_SYS_NG;	/* bitがONの場合、該当vlan-id＝無効		*/
	}
	return	rtn;
}
/* @} */

/****************************************************************************************************************************/
/*!
 * @brief		f_com_search_vid_set
 * @note		inputのvlan-idを有効/無効に設定する。
 * @param		UCHAR*			[in]seachable_vlans
 * @param		USHORT			[in]vlan_id	vlan-id
 * @param		INT				[in]ON(無効)/OFF(有効)
 * @retval		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 * {@
 */
/****************************************************************************************************************************/
VOID	f_com_search_vid_set( UCHAR*	searchable_vlans, USHORT	vlan_id, INT	invalid_flag )
{
	USHORT							pos;			/* byte位置		*/
	USHORT							mod;			/* bit位置		*/
	UCHAR							mask;			/* mask値		*/


	mod  = vlan_id % 8;
	pos  = (vlan_id - mod) / 8;
	mask = 0x80 >> mod;

	if( invalid_flag == D_RRH_OFF ) {

		/* vlan-idが有効の場合、該当bitをOFFに設定	*/
		*(searchable_vlans + pos) = *(searchable_vlans + pos) & ~mask;
	} else {

		/* vlan-idが無効の場合、該当bitをONに設定	*/
		*(searchable_vlans + pos) = *(searchable_vlans + pos) | mask;
	}


	return;
}
/* @} */

/****************************************************************************************************************************/
/*!
 * @brief		f_com_search_vid_cnt
 * @note		指定範囲内で有効なvlan-id数を返す。
 * @param		UCHAR*			[in]seachable_vlans
 * @param		USHORT			[in]low_vlan_id
 * @param		USHORT			[in]high_vlan_id
 * @return		INT				[out]有効vlan-id数
 * @retval		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 * {@
 */
/****************************************************************************************************************************/
INT	f_com_search_vid_cnt( UCHAR*	searchable_vlans, USHORT	low_vlan_id, USHORT	high_vlan_id )
{
	INT								vlan_id;
	INT								valid_cnt = 0;


	if( low_vlan_id <= high_vlan_id ) {

		/* lowest-vlan-id から highest-vlan-idの範囲内で有効vlan-id数チェック	*/
		for( vlan_id = low_vlan_id; vlan_id < (high_vlan_id + 1); vlan_id++ ) {

			/* vlan-id有効チェック	*/
			if( f_com_search_vid_chk(searchable_vlans, vlan_id) == D_SYS_OK ) {

				/* vlan-id有効数更新	*/
				valid_cnt++;
			}
		}
	}
	return	valid_cnt;
}
/* @} */

/* @} */

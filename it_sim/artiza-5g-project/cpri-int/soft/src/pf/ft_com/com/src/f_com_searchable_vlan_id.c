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
 * @note		input��vlan-id���L�����ǂ������`�F�b�N���A���ʂ�Ԃ��B
 * @param		UCHAR*			[in]seachable_vlans
 * @param		USHORT			[in]vlan_id
 * @return		D_SYS_OK(�Y��vlan-id�L��) / D_SYS_NG(�Y��vlan-id����)
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
	USHORT							pos;			/* byte�ʒu		*/
	USHORT							mod;			/* bit�ʒu		*/
	UCHAR							mask;			/* mask�l		*/
	INT								rtn;			/* �߂�l		*/


	mod  = vlan_id % 8;
	pos  = (vlan_id - mod) / 8;
	mask = 0x80 >> mod;

	if( (*(searchable_vlans + pos) & mask) != mask ) {

		rtn = D_SYS_OK;	/* bit��OFF�̏ꍇ�A�Y��vlan-id���L��	*/
	} else {

		rtn = D_SYS_NG;	/* bit��ON�̏ꍇ�A�Y��vlan-id������		*/
	}
	return	rtn;
}
/* @} */

/****************************************************************************************************************************/
/*!
 * @brief		f_com_search_vid_set
 * @note		input��vlan-id��L��/�����ɐݒ肷��B
 * @param		UCHAR*			[in]seachable_vlans
 * @param		USHORT			[in]vlan_id	vlan-id
 * @param		INT				[in]ON(����)/OFF(�L��)
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
	USHORT							pos;			/* byte�ʒu		*/
	USHORT							mod;			/* bit�ʒu		*/
	UCHAR							mask;			/* mask�l		*/


	mod  = vlan_id % 8;
	pos  = (vlan_id - mod) / 8;
	mask = 0x80 >> mod;

	if( invalid_flag == D_RRH_OFF ) {

		/* vlan-id���L���̏ꍇ�A�Y��bit��OFF�ɐݒ�	*/
		*(searchable_vlans + pos) = *(searchable_vlans + pos) & ~mask;
	} else {

		/* vlan-id�������̏ꍇ�A�Y��bit��ON�ɐݒ�	*/
		*(searchable_vlans + pos) = *(searchable_vlans + pos) | mask;
	}


	return;
}
/* @} */

/****************************************************************************************************************************/
/*!
 * @brief		f_com_search_vid_cnt
 * @note		�w��͈͓��ŗL����vlan-id����Ԃ��B
 * @param		UCHAR*			[in]seachable_vlans
 * @param		USHORT			[in]low_vlan_id
 * @param		USHORT			[in]high_vlan_id
 * @return		INT				[out]�L��vlan-id��
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

		/* lowest-vlan-id ���� highest-vlan-id�͈͓̔��ŗL��vlan-id���`�F�b�N	*/
		for( vlan_id = low_vlan_id; vlan_id < (high_vlan_id + 1); vlan_id++ ) {

			/* vlan-id�L���`�F�b�N	*/
			if( f_com_search_vid_chk(searchable_vlans, vlan_id) == D_SYS_OK ) {

				/* vlan-id�L�����X�V	*/
				valid_cnt++;
			}
		}
	}
	return	valid_cnt;
}
/* @} */

/* @} */

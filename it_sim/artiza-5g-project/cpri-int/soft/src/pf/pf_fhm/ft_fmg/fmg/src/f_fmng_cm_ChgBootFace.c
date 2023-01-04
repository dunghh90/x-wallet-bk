/*!
 * @skip  $ld:$
 * @file  f_fmng_cm_ChgBootFace.c
 * @brief Process of changing boot face
 * @date  2013/11/15 FFCS)linb create
 * @date  2015/09/16 TDIPS)sasaki Update
 * @date  2015/10/21 TDIPS)sasaki Update
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"

/*!
 *  @brief  the processing function for changing the boot face.
 *  @note   This function is processed as follows.
 *          1) get the programe boot face
 *          2) set the other side of EEPROM programe boot face 
 *  @param  a_bootface_typ		 	[in]  bootface changing type
 *  @return Void.
 *  @retval -
 *  @date   2008/09/26 FFCS)zhengmh Create 
 *  @date   2013/11/15 FFCS)linb Modify for DCM Zynq 
 *  @date   2015/09/16 TDIPS)sasaki 起動面情報の共有メモリへの展開処理追加
 *  @date   2015/10/21 TDIPS)sasaki Update M-DCM-CBBUPF-03847対処
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_fmng_cm_ChgBootFace(USHORT a_bootface_typ )
{
	USHORT					a_bootface	= D_SYS_NUM0;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_cm_ChgBootFace] ENTER" );				

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_INFO, "f_fmngw_usebootface=%d, cmx_com_bootmen=%d", 
						f_fmngw_usebootface, f_cmw_tra_inv_tbl->cmx_com_bootmen );					
	
	a_bootface = f_cmw_tra_inv_tbl->cmx_com_bootmen;
	
	/****************************************************************/
	/* the bootface changing type is download to opposite using face */
	/****************************************************************/
	if(a_bootface_typ == D_SYS_BOOTFACE_OPP)
	{
		/* 起動面が0面のとき */
		if(a_bootface == D_RRH_BOOT_ACT0)
		{
			/* 起動面を1面に設定 */
			a_bootface = D_RRH_BOOT_ACT1;
		}
		/* 起動面が1面のとき */
		else{
			/* 起動面を0面に設定 */
			a_bootface = D_RRH_BOOT_ACT0;
		}
	}
	else
	{
		a_bootface = f_fmngw_usebootface;
	}

	/* if the faces are same, no need to set */
	if(a_bootface == f_cmw_tra_inv_tbl->cmx_com_bootmen)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_cm_ChgBootFace] RETURN(NO CHANGE)" );					
		return;
	}
	
	/******************/
	/* EEPROM書込処理 */
	/******************/
	/* EEPROMアクセス処理を起動しプログラム起動面を変更する */
	f_fmng_cm_WriteEeprom(D_RRH_EEP_START_FLG, a_bootface);

	/* change the commoon table boot face */
	f_cmw_tra_inv_tbl->cmx_com_bootmen  = a_bootface;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_cm_ChgBootFace] RETURN(CHANGE)" );					
	return ;
}

/*
 *  @brief  the processing function for changing the cprifpga boot face.
 *  @note   This function is processed as follows.
 *          1) get the programe boot face
 *          2) set the other side of EEPROM programe boot face 
 *  @param  a_bootface_typ		 	[in]  bootface changing type
 *  @return Void.
 *  @retval - 
 *  @date   2015/06/16 ALP)murakami Create
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_cm_ChgBootFace_cprifpga(USHORT a_bootface_typ )
{
	USHORT					a_bootface	= D_SYS_NUM0;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_cm_ChgBootFace_cprifpga] ENTER" );				

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_INFO, "f_fmngw_usebootface_cprifpga=%d, cmx_cpri_bootmen=%d", 
						f_fmngw_usebootface_cprifpga, f_cmw_tra_inv_tbl->cmx_cpri_bootmen );					
	
	a_bootface = f_cmw_tra_inv_tbl->cmx_cpri_bootmen;
	
	/****************************************************************/
	/* the bootface changing type is download to opposite using face */
	/****************************************************************/
	if(a_bootface_typ == D_SYS_BOOTFACE_OPP)
	{
		/* 起動面が0面のとき */
		if(a_bootface == D_RRH_BOOT_ACT0)
		{
			/* 起動面を1面に設定 */
			a_bootface = D_RRH_BOOT_ACT1;
		}
		/* 起動面が1面のとき */
		else{
			/* 起動面を0面に設定 */
			a_bootface = D_RRH_BOOT_ACT0;
		}
	}
	else
	{
		a_bootface = f_fmngw_usebootface_cprifpga;
	}

	/* if the faces are same, no need to set */
	if(a_bootface == f_cmw_tra_inv_tbl->cmx_cpri_bootmen)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_cm_ChgBootFace_cprifpga] RETURN(NO CHANGE)" );					
		return;
	}
	
	/******************/
	/* EEPROM書込処理 */
	/******************/
	/* EEPROMアクセス処理を起動しプログラム起動面を変更する */
	f_fmng_cm_WriteEeprom(D_RRH_EEP_CPRI_START_FLG0, a_bootface);

	/* change the commoon table boot face */
	f_cmw_tra_inv_tbl->cmx_cpri_bootmen  = a_bootface;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_cm_ChgBootFace_cprifpga] RETURN(CHANGE)" );					
	return ;
}
/* @} */


/*!
 * @skip  $ld:$
 * @file  f_frw_load_fwinfo.c
 * @brief Reception of Flash Read request
 * @date  2015/08/07 TDIPS)sasaki create
 * @date  2015/08/10 TDIPS)sasaki update レビュー指摘No.37対応
 * @date  2015/08/14 TDIPS)sasaki update REファイル情報読込領域をグローバル変数に変更
 * @date  2015/09/30 TDIPS)sasaki update
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

 /*!
 * @addtogroup RRH_PF_FRW
 * @{
 */

#include "f_frw_inc.h"


/*!
 *  @brief  Read FW-INFO from QSPI-FLASH
 *  @note   This function is processed as follows.
 *          - get information of FW which FHM have.
 *
 *  
 *  @retval none
 *  @date  2015/08/07 TDIPS)sasaki create
 *  @date  2015/08/10 TDIPS)sasaki update レビュー指摘No.37対応
 *  @date  2015/08/14 TDIPS)sasaki update REファイル情報読込領域をグローバル変数に変更
 *  @date  2015/09/30 TDIPS)sasaki update イレース済みのREファイル情報の場合、メーカ識別を0クリアするよう修正
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

INT f_frw_load_fwinfo( VOID )
{
	UINT			idx;
	INT				ret = BPF_HM_DEVC_COMPLETE;

	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_ENTER, "[f_frw_load_fwinfo] ENTER" );

	/********************************************************************************************/
	/* PS側QSPI-FLASH(0面:0x00F00000)からFW情報(REファイル×16)を読込							*/
	/********************************************************************************************/
	ret = BPF_HM_DEVC_QSPI_FLASH_READ( D_FRW_FWINFO_AREA_FACE, D_FRW_FWINFO_AREA_OFFSET, sizeof(f_frww_fwInfo), (UCHAR *)f_frww_fwInfo );
	if( ret != BPF_HM_DEVC_COMPLETE )
	{
		cmn_com_assert_th_get( f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_QSPI_FLASH_READ() NG" );
		memset((CHAR *)&f_frww_fwInfo, 0, sizeof(f_frww_fwInfo));
		return ret;
	}

	/********************************************************************************************/
	/* FLASHから読み出したREファイル情報を共有メモリテーブルへ展開								*/
	/********************************************************************************************/
	for ( idx = 0; idx < D_RRH_CPRINO_RE_MAX; idx++ )
	{
		if(f_frww_fwInfo[idx].maker_id == 0xFFFF)
		{
			f_frww_fwInfo[idx].maker_id = 0;
		}
		ret = f_cmn_com_fw_info_re_set(idx, &f_frww_fwInfo[idx]);
		if( ret != D_RRH_OK )
		{
			cmn_com_assert_th_get( f_frww_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_cmn_com_fw_info_re_set(%d) NG", idx );
			return ret;
		}
	}

	cmn_com_assert_th_get(f_frww_assert_p,  D_RRH_LOG_AST_LV_RETURN, "[f_frw_load_fwinfo] RETURN" );		
    return D_RRH_OK;
}
/* @} */


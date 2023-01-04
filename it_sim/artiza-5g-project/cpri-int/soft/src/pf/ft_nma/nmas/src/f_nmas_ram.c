/*!
 * @skip  $ld:$
 * @file  f_nmas_ram.c
 * @brief RAM table definition.
 *  @date 2019/02/21 KCN)kamimoto  Create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

#include "f_nmas_inc.h"
    
/*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */

/**
 * @brief	pf_nmas thread state
 * @note	pf_nmas thread state
 * 
 */
UINT	f_nmasw_thrState;

UINT	f_nmasw_thrID;

UINT 	f_nmasw_respQid;

UCHAR	f_nmasw_compressData[D_NMAS_COMPDATA];	/**< 圧縮情報					*/

T_SYS_FLASH_READ_RSP		flashSReadRsp;

UCHAR	f_nmasw_dldata_buf[D_DU_QSPI_FLASH_SIZE_WRITE];

VOID*	f_nmasw_assert_p;

T_RRH_LUT_LOG*	f_nmas_lut_log;


/*!
 * @skip  $ld:$
 * @file  f_nma_ram.c
 * @brief RAM table definition.
 *  @date 2013/11/14 FFCS)linb create
 *  @date 2018/08/27 KCN)若水  Create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_nma_inc.h"
    
/*!
 * @addtogroup RRH_PF_NMA
 * @{
 */

/**
 * @brief	pf_nma thread state
 * @note	pf_nma thread state
 * 
 */
UINT	f_nmaw_thrState;

UINT	f_nmaw_thrID;

UINT 	f_nmaw_respQid;

UCHAR	f_nmaw_compressData[D_NMA_COMPDATA];	/**< 圧縮情報					*/

T_SYS_FLASH_READ_RSP		flashReadRsp;

UCHAR	f_nmaw_dldata_buf[D_DU_QSPI_FLASH_SIZE_WRITE];

UCHAR	f_nmaw_sftpdata_buf[D_DU_QSPI_FLASH_SIZE_SOFT_PARA] ;

VOID*	f_nmaw_assert_p;


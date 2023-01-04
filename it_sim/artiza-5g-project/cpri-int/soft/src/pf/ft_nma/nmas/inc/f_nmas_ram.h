/*!
 * @skip  $ld:$
 * @file  f_nmas_ram.h
 * @brief RAM table declaration.
 * @date  2019/02/21 KCN)kamimoto  Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */
    
/*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */

/**
 * @brief	pf_nmas thread state
 * @note	pf_nmas thread state
 * 
 */
extern UINT	f_nmasw_thrState;

extern UINT	f_nmasw_thrID;

extern UINT f_nmasw_respQid;

extern UCHAR	f_nmasw_compressData[D_NMAS_COMPDATA];	/**< 圧縮情報					*/

extern T_SYS_FLASH_READ_RSP		flashReadRsp;

extern UCHAR	f_nmasw_dldata_buf[D_DU_QSPI_FLASH_SIZE_WRITE];

extern VOID*	f_nmasw_assert_p;

extern T_RRH_LUT_LOG*	f_nmas_lut_log;



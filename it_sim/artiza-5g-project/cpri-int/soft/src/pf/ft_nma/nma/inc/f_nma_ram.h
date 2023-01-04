/*!
 * @skip  $ld:$
 * @file  f_nma_ram.h
 * @brief RAM table declaration.
 * @date  2013/11/13 FFCS)LINB Create\n
 * @date  2018/08/27 KCN)若水  Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
    
/*!
 * @addtogroup RRH_PF_NMA
 * @{
 */

/**
 * @brief	pf_nma thread state
 * @note	pf_nma thread state
 * 
 */
extern UINT	f_nmaw_thrState;

extern UINT	f_nmaw_thrID;

extern UINT f_nmaw_respQid;

extern UCHAR	f_nmaw_compressData[D_NMA_COMPDATA];	/**< 圧縮情報					*/

extern T_SYS_FLASH_READ_RSP		flashReadRsp;

extern UCHAR	f_nmaw_dldata_buf[D_DU_QSPI_FLASH_SIZE_WRITE];
extern UCHAR	f_nmaw_sftpdata_buf[D_DU_QSPI_FLASH_SIZE_SOFT_PARA] ;

extern VOID*	f_nmaw_assert_p;


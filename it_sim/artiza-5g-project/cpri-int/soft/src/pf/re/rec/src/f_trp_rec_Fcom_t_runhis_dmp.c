/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_runhis_dmp.c
 * @brief  RUNHIS表示
 * @date   20140417
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2014
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  RUNHIS表示
 * @note   RUNHISを表示する
 * @param  stream [in]  ストリーム
 * @param  dmp_idx [in] dump index
 * @return None
 * @date   20140417
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_runhis_dmp(                                 /* なし:R */
    FILE* stream,                                            /* ストリーム:I */
    UINT dmp_idx
) {
	T_REC_LOGTBL*	rominfp;
	UINT				log_idx;
	UINT				idx_p_size;
	UCHAR*			offset;
	UINT			cnt;
	
	/* RUNHISインデックス取得 */
	rominfp = &f_trp_rec_Rcom_logtbl_tbl.inf[D_REC_LOGTBL_NUM_RUNHIS];
	log_idx = dmp_idx;
	idx_p_size = sizeof(f_trp_rec_Wcom_runhis_tbl.tnm)
				+sizeof(f_trp_rec_Wcom_runhis_tbl.col_flg)
				+sizeof(f_trp_rec_Wcom_runhis_tbl.wrt_idx);
    printf("## Dump Table ##\n");
    printf("%s:%s_%02d\n",rominfp->nic, rominfp->tbl_name, log_idx);
	
    offset = (UCHAR*)&(f_trp_rec_Wcom_runhis_tbl.tnm[0]);
    for(cnt=0; cnt<idx_p_size; cnt++)
	{
		printf("%02x", offset[cnt]);
	}
	offset = (UCHAR*)&(f_trp_rec_Wcom_runhis_tbl.inf[log_idx]);
	for(cnt=0; cnt<sizeof(f_trp_rec_Wcom_runhis_tbl.inf[log_idx]); cnt++)
	{
		if((cnt%64) == 0)
		{
			printf("\n");
		}
		printf("%02x", offset[cnt]);
	}
	printf("\n");
	
	return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

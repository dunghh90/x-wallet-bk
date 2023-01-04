/*!
 * @skip 		$ld:$
 * @file 		f_pfm_InitReq.c
 * @brief		pf_pfm ログ取得処理
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_PFM
 * @{
 */

#include "f_pfm_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_pfm_set_log
 * @note		共有メモリにパフォーマンスログを出力する。
 * @param		*time			[in]VOID*	レジスタ読出し時間
 * @param		*reg_data		[in]UINT*	レジスタ値
 * @retval		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
VOID	f_pfm_set_log( UINT	fact, VOID	*time, UINT reg_size , UINT	*reg_data , UINT reg_size_com , UINT	*reg_data_com )
{

	/* 取得要因保存	*/
	f_comw_pfmlog_tbl->info[f_comw_pfmlog_tbl->index].fact = fact;

	/* 取得時間保存	*/
	memcpy( (VOID *)&f_comw_pfmlog_tbl->info[f_comw_pfmlog_tbl->index].time, 
			time, sizeof(T_RRH_SYSTEM_TIME) );

	/* レジスタ値保存	*/
	memcpy( (VOID *)&f_comw_pfmlog_tbl->info[f_comw_pfmlog_tbl->index].reg_data[0][0][0],
			reg_data, reg_size );

	memcpy( (VOID *)&f_comw_pfmlog_tbl->info[f_comw_pfmlog_tbl->index].reg_data_com[0],
			reg_data_com, reg_size_com );

	/* index値・書込み回数保存	*/
	f_comw_pfmlog_tbl->index = (f_comw_pfmlog_tbl->index + 1) % D_RRH_PFM_NUM_OF_INFO;
	f_comw_pfmlog_tbl->counter++;
	if(f_comw_pfmlog_tbl->counter == 0){
		f_comw_pfmlog_tbl->counter = D_RRH_PFM_NUM_OF_INFO;
	}
	return;
}


/****************************************************************************************************************************/
/*!
 * @brief		f_pfm_set_log
 * @note		共有メモリにパフォーマンスログを出力する。
 * @param		*time			[in]VOID*	レジスタ読出し時間
 * @param		*reg_data		[in]UINT*	レジスタ値
 * @retval		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
VOID	f_pfm_set_log_peri( VOID	*time, UINT reg_size , UINT	*reg_data_addr ,  UINT	*reg_data  )
{
	if( f_comw_pfmlog_tbl->fault_peri != D_RRH_OFF ){
		/* skip */
		return;
	}

	/* 取得時間保存	*/
	memcpy( (VOID *)&f_comw_pfmlog_tbl->info_peri[f_comw_pfmlog_tbl->index_peri].time, 
			time, sizeof(T_RRH_SYSTEM_TIME) );

	/* レジスタ値保存	*/
	memcpy( (VOID *)&f_comw_pfmlog_tbl->info_peri[f_comw_pfmlog_tbl->index_peri].reg_data_addr,
			reg_data_addr, reg_size );

	memcpy( (VOID *)&f_comw_pfmlog_tbl->info_peri[f_comw_pfmlog_tbl->index_peri].reg_data,
			reg_data, reg_size );

	/* Fault? */
	UINT	faultIdbit = 0;
	UINT	faultICheckbit = (UINT)( D_DU_SVCTL_BIT_OCCUR_FID_10 | D_DU_SVCTL_BIT_OCCUR_FID_11 | D_DU_SVCTL_BIT_OCCUR_FID_12 ) ;

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV4INF, &faultIdbit);
	if( ( faultIdbit & faultICheckbit) != D_RRH_OFF ){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Stop performance periodic counter by Fault(bit:0x%08x total cnt:0x%08x index:0x%08x)",
				 faultIdbit ,f_comw_pfmlog_tbl->counter_peri ,f_comw_pfmlog_tbl->index_peri );
		f_comw_pfmlog_tbl->fault_peri = faultIdbit;
	}

	/* index値・書込み回数保存	*/
	f_comw_pfmlog_tbl->index_peri = (f_comw_pfmlog_tbl->index_peri + 1) % D_RRH_PFM_NUM_OF_INFO_PER;
	f_comw_pfmlog_tbl->counter_peri++;
	if(f_comw_pfmlog_tbl->counter_peri == 0){
		f_comw_pfmlog_tbl->counter_peri = D_RRH_PFM_NUM_OF_INFO_PER;
	}

	return;
}



/* @} */

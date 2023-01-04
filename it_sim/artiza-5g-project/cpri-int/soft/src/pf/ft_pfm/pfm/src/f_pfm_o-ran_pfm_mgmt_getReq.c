/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_pfm_o-ran_pfm_getReq.c
 *  @brief  MSI_O_RAN_PERFORMANCE_MANAGEMENT_GET_REQ (o-ran-performance-management GET Request) Recieved function
 *  @date   2019/02/21 FJT)Yoshida  create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_PFM
* @{ */

#include "f_pfm_inc.h"
#include "BPF_COM_CMD.h"

/* @{ */
/**
* @brief      MSI_O_RAN_PERFORMANCE_MANAGEMENT_GET_REQ (o-ran-performance-management GET Request) Recieved function
* @note       o-ran-interfaces EDIT Request factor judge, and table data read.\n
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     result of procedure
* @date       2018/11/26  FJT)Yoshida  new create
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_pfm_o_ran_pfm_mgmt_getReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_PFM_MNG_GET_CFM			cfmBody;			/* 応答データ部  */

	INT										i;


	/* ==================================================================== */
	/* MSI_O_RAN_PFM_MNG_GET_CFM編集											*/
	/* ==================================================================== */
	for( i = 0; i < E_PFM_OBJIDX_MAX; i++ ) {

		cfmBody.rx_window_measure_get_t[i].measure_obj = i;
		cfmBody.rx_window_measure_get_t[i].act_flag    = f_comw_pfmmng_tbl->activate_state[i];
		cfmBody.rx_window_measure_get_t[i].tr_count[0] = f_comw_pfmmng_tbl->measurement.object[i].count[0];
		cfmBody.rx_window_measure_get_t[i].ru_count    = cfmBody.rx_window_measure_get_t[i].tr_count[0];

		/* 報告情報表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management get  : [%d] measurement-object = %d", 
							i, cfmBody.rx_window_measure_get_t[i].measure_obj );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management get  : [%d] activate           = %d", 
							i, cfmBody.rx_window_measure_get_t[i].act_flag );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management get  : [%d] count [RU]         = %d", 
							i, cfmBody.rx_window_measure_get_t[i].ru_count );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management get  : [%d] count [Port0]      = %d", 
							i, cfmBody.rx_window_measure_get_t[i].tr_count[0] );
	}
	/* ==================================================================== */
	/* MSI_O_RAN_PFM_MNG_GET_CFM送信											*/
	/* ==================================================================== */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_PFM_MNG_GET_CFM,			/* 送信するメッセージID		*/
						 w_requestId,								/* リクエストID				*/
						 sizeof(T_MPSW_MSI_O_RAN_PFM_MNG_GET_CFM),	/* 送信するデータサイズ		*/
						 (VOID *)&cfmBody,							/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,							/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_PFM ) ;					/* pf_pfmのスレッドID		*/
	/* 処理結果表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management get  OK!" );

	return	D_RRH_OK;
}
/* @} */



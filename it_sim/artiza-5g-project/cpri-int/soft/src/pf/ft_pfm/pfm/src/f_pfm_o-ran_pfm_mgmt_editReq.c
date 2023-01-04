/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_pfm_o-ran_pfm_editReq.c
 *  @brief  MSI_O_RAN_PERFORMANCE_MANAGEMENT_EDIT_REQ (o-ran-performance-management EDIT Request) Recieved function
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
* @brief      MSI_O_RAN_PERFORMANCE_MANAGEMENT_EDIT_REQ (o-ran-performance-management EDIT Request) Recieved function
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
INT f_pfm_o_ran_pfm_mgmt_editReq( VOID *req, UINT w_requestId )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ		*reqBody;			/* 要求データ部  */
	T_MPSW_MSI_O_RAN_PFM_MNG_NTF_IND			indBody;			/* 応答データ部  */

	T_RRH_SYSTEM_TIME						now;
	UINT									now_sec;
	UINT									now_mod;

	UINT									reg_data[D_RRH_CAR_MAX][D_RRH_ANT_MAX][D_RRH_PFM_NUM_OF_REG];
	ULLINT									obj_data[E_PFM_OBJIDX_MAX];
	UINT									reg_data_com[D_RRH_PFM_NUM_OF_REG_COM];

	UINT									activate_cnt = 0;
	UINT									index;

	UINT									LatchSet = D_RRH_ON;

	INT										h,i, j;
	UINT									regAddr;
	INT										endcd = 0;
	INT										errcd = 0;

	T_MPSW_RX_WINDOW_MEASURE_T				rx_window_measurement[D_RRH_PFM_NUM_OF_RXWIN];


	reqBody  = (T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ *)req;

	/* 設定情報表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management edit : measurement-interval    = %04x", reqBody->mea_interval );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management edit : notification-interval   = %04x", reqBody->ntf_interval );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management edit : num-of_object           = %04x", reqBody->measure_t_num );
	for( i = 0; i < reqBody->measure_t_num; i++ ) {

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management edit : [%d] measurement-object = %08x", 
							i, reqBody->rx_window_measure_t[i].measure_obj );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management edit : [%d] activate           = %d",   
							i, reqBody->rx_window_measure_t[i].act_flag );
	}

	/* ==================================================================== */
	/* ローカル変数初期化													*/
	/* ==================================================================== */
	memset( (VOID *)reg_data, 0, sizeof(reg_data) );
	memset( (VOID *)obj_data, 0, sizeof(obj_data) );
	memset( (VOID *)&indBody,  0, sizeof(indBody) );
	memset( (VOID *)rx_window_measurement, 0, sizeof(rx_window_measurement) );

	/* レジスタ読出し	*/
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_POL, D_DUS_REG_DLP_PMONLTCRALL, &LatchSet);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_POL, D_DU_REG_ULP_PMONLTCRALL, &LatchSet);

	for(h= 0 ; h < f_pfmw_rxwin_reg_addr_offset_ccnum ; h++){
		for(i = 0; i < f_pfmw_rxwin_reg_addr_offset_antnum ; i++){
			for( j = 0; j < D_RRH_PFM_NUM_OF_REG; j++ ) {
				if(f_pfmw_rxwin_reg_addr[j].regAddr == 0){
					continue;
				}
				regAddr = f_pfmw_rxwin_reg_addr[j].regAddr + ( f_pfmw_rxwin_reg_addr_offset_cc * h) + (f_pfmw_rxwin_reg_addr_offset_ant * i);
				/* レジスタ読出し	*/
				BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, regAddr , &reg_data[h][i][j] );
				if( f_pfmw_rxwin_reg_addr[j].pfmType != E_PFM_OBJIDX_NOTHING ) {
					/* レジスタ読出し値をobject単位に集計	*/
					obj_data[f_pfmw_rxwin_reg_addr[j].pfmType] += reg_data[h][i][j];
					obj_data[E_PFM_OBJIDX_RX_TOTAL]            += reg_data[h][i][j];
				}
			}
		}
	}

	for(i = 0; i < D_RRH_PFM_NUM_OF_REG_COM ; i++){
		BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, f_pfmw_rxwin_reg_addr_com[i] , &reg_data_com[i] );
	}
	
	
	/* RX_WINDOW_MEASUREMENT分の状態を保持	*/
	for( i = 0; i < D_RRH_PFM_NUM_OF_RXWIN; i++ ) {
		rx_window_measurement[i].measure_obj = i;
		rx_window_measurement[i].act_flag    = f_comw_pfmmng_tbl->activate_state[i];
	}
	for( i = 0; i < reqBody->measure_t_num; i++ ) {
		index = reqBody->rx_window_measure_t[i].measure_obj;
		rx_window_measurement[index].act_flag = reqBody->rx_window_measure_t[i].act_flag;
	}

	/* 現在時刻設定	*/
	f_com_getSysTim_notMs( &now );
	now_sec = (now.hour * 60 * 60) + (now.min * 60) + now.sec;
	now_mod = reqBody->mea_interval - (now_sec % reqBody->mea_interval);

	/* ==================================================================== */
	/* 測定開始/停止制御													*/
	/* ==================================================================== */
	for( i = 0; i < D_RRH_PFM_NUM_OF_RXWIN; i++ ) {

		if( rx_window_measurement[i].act_flag == D_RRH_ON ) {	/* activate	*/
			
			switch( f_comw_pfmmng_tbl->pfm_state[i] ) {

				case	E_PFM_CORRECT_STATE_STOP:				/* 停止中状態			*/
					/* activateフラグ設定	*/
					f_comw_pfmmng_tbl->activate_state[i] = rx_window_measurement[i].act_flag;

					/* 集計用領域初期化	*/
					memcpy( &f_comw_pfmmng_tbl->info.object[i].start, &now, sizeof(T_RRH_SYSTEM_TIME) );
					f_comw_pfmmng_tbl->info.object[i].count[0] = 0;

					/* 集計領域初期化	*/
					memset( &f_comw_pfmmng_tbl->measurement.object[i],  0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );
					memset( &f_comw_pfmmng_tbl->notification.object[i], 0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );

					if( f_comw_pfmmng_tbl->timer_state == D_RRH_OFF ) {

						f_comw_pfmmng_tbl->measurement.interval       = reqBody->mea_interval;
						f_comw_pfmmng_tbl->measurement.init_interval  = now_mod;

						f_comw_pfmmng_tbl->notification.interval      = reqBody->ntf_interval;
						f_comw_pfmmng_tbl->notification.init_interval = 0xFFFFFFFF;
					}

					/* 統計状態更新	*/
					f_comw_pfmmng_tbl->pfm_state[i] = E_PFM_CORRECT_STATE_ACTIVE_INIT;
					break;

				case	E_PFM_CORRECT_STATE_ACTIVE_INIT:		/* 測定中状態(初期)		*/
				case	E_PFM_CORRECT_STATE_ACTIVE:				/* 測定中状態			*/
				case	E_PFM_CORRECT_STATE_WAIT_NOTIFICATION:	/* 通知待ち状態			*/
					f_comw_pfmmng_tbl->info.object[i].count[0] += obj_data[i];
					break;

				default:
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Performance state error, measurement-object = %04x, state = %04x",
										i, f_comw_pfmmng_tbl->pfm_state[i] ) ;
					break;
			}

		} else {	/* deactivate	*/

			switch( f_comw_pfmmng_tbl->pfm_state[i] ) {

				case	E_PFM_CORRECT_STATE_STOP:				/* 停止中状態			*/
					/* 無処理	*/
					break;
				case	E_PFM_CORRECT_STATE_ACTIVE_INIT:		/* 測定中状態(初期)		*/
					/* activateフラグ設定	*/
					f_comw_pfmmng_tbl->activate_state[i] = rx_window_measurement[i].act_flag;

					/*** notification編集	***/
					indBody.rx_window_result_t[indBody.measure_t_num].measure_obj = i;

					/*== start-time	==*/
					memcpy( &indBody.rx_window_result_t[indBody.measure_t_num].start_time, 
							&f_comw_pfmmng_tbl->info.object[i].start, sizeof(T_MPSW_START_TIME) );
					/*== end-time	==*/
					memcpy( &indBody.rx_window_result_t[indBody.measure_t_num].end_time, &now, sizeof(T_MPSW_START_TIME) );
					/*== tr_count	==*/
					indBody.rx_window_result_t[indBody.measure_t_num].tr_count[0] = obj_data[i] + f_comw_pfmmng_tbl->info.object[i].count[0];
					/*== ru_count	==*/
					indBody.rx_window_result_t[indBody.measure_t_num].ru_count    = indBody.rx_window_result_t[indBody.measure_t_num].tr_count[0];
					indBody.measure_t_num++;

					/* objectテーブル初期化	*/
					memset( &f_comw_pfmmng_tbl->info.object[i],         0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );
					memset( &f_comw_pfmmng_tbl->measurement.object[i],  0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );
					memset( &f_comw_pfmmng_tbl->notification.object[i], 0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );

					/* 統計状態更新	*/
					f_comw_pfmmng_tbl->pfm_state[i] = E_PFM_CORRECT_STATE_STOP;
					break;

				case	E_PFM_CORRECT_STATE_ACTIVE:				/* 測定中状態			*/
					/* activateフラグ設定	*/
					f_comw_pfmmng_tbl->activate_state[i] = rx_window_measurement[i].act_flag;

					/*** notification編集	***/
					indBody.rx_window_result_t[indBody.measure_t_num].measure_obj = i;

					/*== start-time	==*/
					memcpy( &indBody.rx_window_result_t[indBody.measure_t_num].start_time, 
							&f_comw_pfmmng_tbl->info.object[i].start, sizeof(T_MPSW_START_TIME) );
					/*== end-time	==*/
					memcpy( &indBody.rx_window_result_t[indBody.measure_t_num].end_time, &now, sizeof(T_MPSW_START_TIME) );
					/*== tr_count	==*/
					indBody.rx_window_result_t[indBody.measure_t_num].tr_count[0] = obj_data[i]
																				  + f_comw_pfmmng_tbl->info.object[i].count[0]
																				  + f_comw_pfmmng_tbl->notification.object[i].count[0];
					/*== ru_count	==*/
					indBody.rx_window_result_t[indBody.measure_t_num].ru_count    = indBody.rx_window_result_t[indBody.measure_t_num].tr_count[0];
					indBody.measure_t_num++;

					/* objectテーブル初期化	*/
					memset( &f_comw_pfmmng_tbl->info.object[i],         0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );
					memset( &f_comw_pfmmng_tbl->measurement.object[i],  0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );
					memset( &f_comw_pfmmng_tbl->notification.object[i], 0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );

					f_comw_pfmmng_tbl->pfm_state[i] = E_PFM_CORRECT_STATE_STOP;
					break;

				case	E_PFM_CORRECT_STATE_WAIT_NOTIFICATION:	/* 通知待ち状態			*/
					/* activateフラグ設定	*/
					f_comw_pfmmng_tbl->activate_state[i] = rx_window_measurement[i].act_flag;

					/*** notification編集	***/
					indBody.rx_window_result_t[indBody.measure_t_num].measure_obj = i;

					/*== start-time	==*/
					memcpy( &indBody.rx_window_result_t[indBody.measure_t_num].start_time, 
							&f_comw_pfmmng_tbl->notification.object[i].start, sizeof(T_MPSW_START_TIME) );
					/*== end-time	==*/
					memcpy( &indBody.rx_window_result_t[indBody.measure_t_num].end_time, &now, sizeof(T_MPSW_START_TIME) );
					/*== tr_count	==*/
					indBody.rx_window_result_t[indBody.measure_t_num].tr_count[0] = obj_data[i]
																				  + f_comw_pfmmng_tbl->info.object[i].count[0]
																				  + f_comw_pfmmng_tbl->notification.object[i].count[0];
					/*== ru_count	==*/
					indBody.rx_window_result_t[indBody.measure_t_num].ru_count    = indBody.rx_window_result_t[indBody.measure_t_num].tr_count[0];
					indBody.measure_t_num++;

					/* objectテーブル初期化	*/
					memset( &f_comw_pfmmng_tbl->info.object[i],         0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );
					memset( &f_comw_pfmmng_tbl->measurement.object[i],  0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );
					memset( &f_comw_pfmmng_tbl->notification.object[i], 0, sizeof(T_RRH_PFM_MEASUREMENT_INFO) );

					f_comw_pfmmng_tbl->pfm_state[i] = E_PFM_CORRECT_STATE_STOP;
					break;

				default:
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Performance state error, measurement-object = %04x, state = %04x",
										i, f_comw_pfmmng_tbl->pfm_state[i] ) ;
					break;
			}
		}
	}

	/* ==================================================================== */
	/* タイマ制御															*/
	/* ==================================================================== */
	/* Activate Item数カウント	*/
	for( i = 0; i < D_RRH_PFM_NUM_OF_RXWIN; i++ ) {
		if( f_comw_pfmmng_tbl->activate_state[i] == D_RRH_ON ) {
			activate_cnt++ ;
		}
	}

	/* activate状態遷移あり、かつ、タイマ停止中	*/
	if( (activate_cnt != 0) && (f_comw_pfmmng_tbl->timer_state == D_RRH_OFF) ) {

		/* タイマ起動	*/
		endcd = BPF_RU_HRTM_REGISTER( BPF_RU_HRTM_REG_PERIODIC,			/* Timer種別						*/
									  E_RRH_TIMID_PFM_INTERVAL_RPT,		/* Timer ID							*/
									  1000,								/* Timer値							*/
									  D_SYS_MSGID_PFM_1S_TIMEOUT_NTF,	/* TimeOut時通知Event番号			*/
									  D_SYS_THDQID_PF_PFM,				/* TimeOut時送信先Queue番号			*/
									  &errcd );
		if( endcd != 0 ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Timer Start NG, endcd = %08x, errcd = %08x", endcd, errcd ) ;
		}
		f_comw_pfmmng_tbl->timer_state = D_RRH_ON;
	}
	/* deactivate状態遷移あり、かつ、タイマ起動中	*/
	if( (activate_cnt == 0) && (f_comw_pfmmng_tbl->timer_state == D_RRH_ON) ) {

		/* タイマ停止	*/
		endcd = BPF_RU_HRTM_CANCEL( E_RRH_TIMID_PFM_INTERVAL_RPT, &errcd );
		if( endcd != 0 ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Timer Cancel NG, endcd = %08x, errcd = %08x", endcd, errcd ) ;
		}
		f_comw_pfmmng_tbl->timer_state = D_RRH_OFF;
		
		/* テーブル初期化	*/
		memset( &f_comw_pfmmng_tbl->info, 0, sizeof(T_RRH_PFM_INTERVAL_INFO) );
		f_comw_pfmmng_tbl->info.interval      = 0xFFFFFFFF;
		f_comw_pfmmng_tbl->info.init_interval = 0xFFFFFFFF;

		memset( &f_comw_pfmmng_tbl->measurement, 0, sizeof(T_RRH_PFM_INTERVAL_INFO) );
		f_comw_pfmmng_tbl->measurement.interval      = 0xFFFFFFFF;
		f_comw_pfmmng_tbl->measurement.init_interval = 0xFFFFFFFF;

		memset( &f_comw_pfmmng_tbl->notification, 0, sizeof(T_RRH_PFM_INTERVAL_INFO) );
		f_comw_pfmmng_tbl->notification.interval      = 0xFFFFFFFF;
		f_comw_pfmmng_tbl->notification.init_interval = 0xFFFFFFFF;
	}

	/* ==================================================================== */
	/* MSI_O_RAN_PFM_MNG_EDIT_CFM送信										*/
	/* ==================================================================== */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_PFM_MNG_EDIT_CFM,		/* 送信するメッセージID		*/
						 w_requestId,							/* リクエストID				*/
						 D_RRH_ZERO,							/* 送信するデータサイズ		*/
						 (VOID *)D_RRH_NULL,					/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,						/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_PFM ) ;				/* pf_pfmのスレッドID		*/
	/* 処理結果表示	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management edit OK!" );

	/* ==================================================================== */
	/* MSI_O_RAN_PFM_MNG_NTF_IND送信											*/
	/* ==================================================================== */
	if( indBody.measure_t_num != 0 ) {

		/* MSI送信処理 */
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_PFM_MNG_NTF_IND,		/* メッセージID				*/
							0x00000000,							/* リクエストID				*/
							sizeof(T_MPSW_MSI_O_RAN_PFM_MNG_NTF_IND),/* 送信するデータサイズ	*/
							&indBody,							/* 送信データのポインタ		*/
							D_RRH_PROCQUE_PF,					/* PFのプロセスID			*/
							D_SYS_THDQID_PF_PFM ) ;				/* pf_pfmのスレッドID		*/
		/* 処理結果表示	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management ntf OK!" );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management ntf  : measure_t_num          = %04x", indBody.measure_t_num );
		for( i = 0; i < indBody.measure_t_num; i++ ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management ntf  : [%d]measurement-object = %04d", i, indBody.rx_window_result_t[i].measure_obj );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management ntf  : [%d]start-time         = %04d/%02d/%02d %02d:%02d:%02d", 
														i,
														indBody.rx_window_result_t[i].start_time.year,
														indBody.rx_window_result_t[i].start_time.month,
														indBody.rx_window_result_t[i].start_time.day,
														indBody.rx_window_result_t[i].start_time.hour,
														indBody.rx_window_result_t[i].start_time.min,
														indBody.rx_window_result_t[i].start_time.sec );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management ntf  : [%d]end-time           = %04d/%02d/%02d %02d:%02d:%02d", 
														i,
														indBody.rx_window_result_t[i].end_time.year,
														indBody.rx_window_result_t[i].end_time.month,
														indBody.rx_window_result_t[i].end_time.day,
														indBody.rx_window_result_t[i].end_time.hour,
														indBody.rx_window_result_t[i].end_time.min,
														indBody.rx_window_result_t[i].end_time.sec );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management ntf  : [%d]ru_count           = %04x", i, indBody.rx_window_result_t[i].ru_count );
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management ntf  : [%d]tr_count[0]        = %04x", i, indBody.rx_window_result_t[i].tr_count[0] );
		}
	}
	/* ==================================================================== */
	/* Performanceログ収集													*/
	/* ==================================================================== */
	f_pfm_set_log( E_PFM_PFMLOG_FACT_MSI, (VOID*)&now, sizeof(reg_data) , &reg_data[0][0][0] , sizeof(reg_data_com) , reg_data_com) ;

	return	D_RRH_OK;
}
/* @} */



/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_pfm_timeout_ind.c
 *  @brief  1sec periodic timer T.O. indication
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
* @brief      1sec periodic timer T.O. indication
* @note       1sec periodic timer T.O. indication
* @param      recvMsg     [in] the buffer address pointer of received message
* @return     result of procedure
* @date       2018/11/26  FJT)Yoshida  new create
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_pfm_timeout_ind( VOID *recvMsg )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_PFM_MNG_NTF_IND			indBody;			/* 応答データ部  */

	T_RRH_SYSTEM_TIME						now;
	UINT									now_sec;
	UINT									now_mod;

	UINT									LatchSet = D_RRH_ON;

	UINT									reg_data[D_RRH_CAR_MAX][D_RRH_ANT_MAX][D_RRH_PFM_NUM_OF_REG];
	UINT									reg_data_com[D_RRH_PFM_NUM_OF_REG_COM];
	ULLINT									obj_data[E_PFM_OBJIDX_MAX];

	INT										h,i, j;
	UINT									regAddr;
	UINT									state_reg = 0;


	/* ==================================================================== */
	/* ローカル変数初期化													*/
	/* ==================================================================== */
	memset( (VOID *)reg_data, 0, sizeof(reg_data) );
	memset( (VOID *)obj_data, 0, sizeof(obj_data) );
	memset( (VOID *)&indBody,  0, sizeof(indBody) );

	/* ==================================================================== */
	/* テーブル更新															*/
	/* ==================================================================== */
	/* 1秒周期タイマT.O.回数更新 (measurement-interval)	*/
	if( f_comw_pfmmng_tbl->measurement.interval != 0xFFFFFFFF ) {
		f_comw_pfmmng_tbl->measurement.time_cnt++;
	}
	/* 1秒周期タイマT.O.回数更新 (notification-interval)	*/
	if( f_comw_pfmmng_tbl->notification.interval != 0xFFFFFFFF ) {
		f_comw_pfmmng_tbl->notification.time_cnt++;
	}

	/* 時刻情報取得	*/
	f_com_getSysTim_notMs( (T_RRH_SYSTEM_TIME *)&now );
	now_sec = (now.hour * 60 * 60) + (now.min * 60) + now.sec;
	now_mod = now_sec % f_comw_pfmmng_tbl->measurement.interval;

	/* ==================================================================== */
	/* measurement-interval処理												*/
	/* ==================================================================== */
	if( (now_mod == 0) && ((f_comw_pfmmng_tbl->measurement.time_cnt >= f_comw_pfmmng_tbl->measurement.interval)
						|| (f_comw_pfmmng_tbl->measurement.time_cnt >= f_comw_pfmmng_tbl->measurement.init_interval)) ) {

		/* タイマ関連の領域初期化	*/
		f_comw_pfmmng_tbl->measurement.init_interval = 0xFFFFFFFF;
		f_comw_pfmmng_tbl->measurement.time_cnt      = 0;

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

		for( i = 0; i < D_RRH_PFM_NUM_OF_RXWIN; i++ ) {

			switch( f_comw_pfmmng_tbl->pfm_state[i] ) {

				case	E_PFM_CORRECT_STATE_STOP:				/* 停止中状態			*/
					/* 無処理	*/
					break;

				case	E_PFM_CORRECT_STATE_ACTIVE_INIT:		/* 測定中状態(初期)		*/
				case	E_PFM_CORRECT_STATE_ACTIVE:				/* 測定中状態			*/

					/***** measurement-object用領域設定								*****/
					memcpy( &f_comw_pfmmng_tbl->measurement.object[i].start, 	/* start-time	*/
							&f_comw_pfmmng_tbl->info.object[i].start, 
							sizeof(T_RRH_SYSTEM_TIME) );
					memcpy( &f_comw_pfmmng_tbl->measurement.object[i].end, &now, sizeof(T_RRH_SYSTEM_TIME) );
					f_comw_pfmmng_tbl->measurement.object[i].count[0] = obj_data[i] + f_comw_pfmmng_tbl->info.object[i].count[0];	/* count[0]	*/

					/***** notification-object用領域設定							*****/
					memcpy( &f_comw_pfmmng_tbl->notification.object[i].start, 	/* start-time	*/
							&f_comw_pfmmng_tbl->measurement.object[i].start, 
							sizeof(T_RRH_SYSTEM_TIME) );
					memcpy( &f_comw_pfmmng_tbl->notification.object[i].end, 	/* end-time	*/
							&f_comw_pfmmng_tbl->measurement.object[i].end, 
							sizeof(T_RRH_SYSTEM_TIME) );
					f_comw_pfmmng_tbl->notification.object[i].count[0] = f_comw_pfmmng_tbl->measurement.object[i].count[0];	/* count[0]	*/
					
					/***** 集計用領域設定											*****/
					memcpy( &f_comw_pfmmng_tbl->info.object[i].start, &now, sizeof(T_RRH_SYSTEM_TIME) );	/* start-time	*/
					f_comw_pfmmng_tbl->info.object[i].count[0] = 0;

					/* 統計状態更新	*/
					f_comw_pfmmng_tbl->pfm_state[i] = E_PFM_CORRECT_STATE_WAIT_NOTIFICATION;
					break;

				case	E_PFM_CORRECT_STATE_WAIT_NOTIFICATION:	/* 通知待ち状態			*/
					/***** measurement-object用領域設定								*****/
					memcpy( &f_comw_pfmmng_tbl->measurement.object[i].start, 	/* start-time	*/
							&f_comw_pfmmng_tbl->info.object[i].start, 
							sizeof(T_RRH_SYSTEM_TIME) );
					memcpy( &f_comw_pfmmng_tbl->measurement.object[i].end, &now, sizeof(T_RRH_SYSTEM_TIME) );
					f_comw_pfmmng_tbl->measurement.object[i].count[0] = obj_data[i] + f_comw_pfmmng_tbl->info.object[i].count[0];	/* count[0]	*/

					/***** notification-object用領域設定							*****/
					memcpy( &f_comw_pfmmng_tbl->notification.object[i].end, 	/* end-time	*/
							&f_comw_pfmmng_tbl->measurement.object[i].end, 
							sizeof(T_RRH_SYSTEM_TIME) );
					f_comw_pfmmng_tbl->notification.object[i].count[0] += f_comw_pfmmng_tbl->measurement.object[i].count[0];	/* count[0]	*/

					/***** 集計用領域設定											*****/
					memcpy( &f_comw_pfmmng_tbl->info.object[i].start, &now, sizeof(T_RRH_SYSTEM_TIME) );	/* start-time	*/
					f_comw_pfmmng_tbl->info.object[i].count[0] = 0;
					break;

				default:
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Performance state error, measurement-object = %04x, state = %04x",
										i, f_comw_pfmmng_tbl->pfm_state[i] ) ;
					break;
			}
		}

		f_pfm_set_log( E_PFM_PFMLOG_FACT_TIMER, (VOID*)&now, sizeof(reg_data) , &reg_data[0][0][0] , sizeof(reg_data_com) , reg_data_com) ;
	}

	/* ==================================================================== */
	/* notification-interval処理												*/
	/* ==================================================================== */
	if( f_comw_pfmmng_tbl->notification.time_cnt >= f_comw_pfmmng_tbl->notification.interval ) {

		/* タイマ関連の領域初期化	*/
		f_comw_pfmmng_tbl->notification.time_cnt = 0;

		for( i = 0; i < D_RRH_PFM_NUM_OF_RXWIN; i++ ) {

			switch( f_comw_pfmmng_tbl->pfm_state[i] ) {

				case	E_PFM_CORRECT_STATE_STOP:				/* 停止中状態			*/
				case	E_PFM_CORRECT_STATE_ACTIVE_INIT:		/* 測定中状態(初期)		*/
				case	E_PFM_CORRECT_STATE_ACTIVE:				/* 測定中状態			*/
					/* 無処理	*/
					break;

				case	E_PFM_CORRECT_STATE_WAIT_NOTIFICATION:	/* 通知待ち状態			*/

					/***** notification編集											*****/
					indBody.rx_window_result_t[indBody.measure_t_num].measure_obj = i;	/* measurement-object	*/
					memcpy( &indBody.rx_window_result_t[indBody.measure_t_num].start_time,	/* start-time	*/
							&f_comw_pfmmng_tbl->notification.object[i].start,
							sizeof(T_MPSW_START_TIME) );
					memcpy( &indBody.rx_window_result_t[indBody.measure_t_num].end_time,	/* end-time		*/
							&f_comw_pfmmng_tbl->notification.object[i].end,
							sizeof(T_MPSW_END_TIME) );
					indBody.rx_window_result_t[indBody.measure_t_num].tr_count[0] = f_comw_pfmmng_tbl->notification.object[i].count[0];
					indBody.rx_window_result_t[indBody.measure_t_num].ru_count    = indBody.rx_window_result_t[indBody.measure_t_num].tr_count[0];
					indBody.measure_t_num++;
					/* 統計状態更新	*/
					f_comw_pfmmng_tbl->pfm_state[i] = E_PFM_CORRECT_STATE_ACTIVE;
					break;

				default:
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Performance state error, measurement-object = %04x, state = %04x",
										i, f_comw_pfmmng_tbl->pfm_state[i] ) ;
					break;
			}
		}
	}

	if( indBody.measure_t_num != 0 ) {

		/* キャリア設定情報	*/
		BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ , D_DU_REG_CTB_SV1INF , &state_reg );
		if( (state_reg & D_DU_SVCTL_BIT_USA_ALL) == D_DU_SVCTL_BIT_USA_IDLE ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "o-ran-performance-management ntf not send sv1info =0x%08x", state_reg );
		} else {
			/* 報告情報表示	*/
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

			/* MSI送信処理 */
			(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_PFM_MNG_NTF_IND,		/* メッセージID				*/
								0x00000000,							/* リクエストID				*/
																	/* 送信するデータサイズ		*/
								(sizeof(UINT16) + (sizeof(T_MPSW_RX_WINDOW_RESULT_T) * indBody.measure_t_num)),
								&indBody,							/* 送信データのポインタ		*/
								D_RRH_PROCQUE_PF,					/* PFのプロセスID			*/
								D_SYS_THDQID_PF_PFM ) ;				/* pf_pfmのスレッドID		*/
		}
	}
	return	D_RRH_OK;
}

/* @{ */
/**
* @brief      Log periodic timer T.O. indication
* @note       Log periodic timer T.O. indication
* @param      recvMsg     [in] the buffer address pointer of received message
* @return     result of procedure
* @date       2019/04/11 Taniguchi
* @warning    N/A
* @FeatureID  5GDU-002-001
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_pfm_log_timeout( VOID *recvMsg )
{
	UINT									LatchSet = D_RRH_ON;
	T_RRH_SYSTEM_TIME						now;
	UINT									reg_data_addr[D_RRH_PFM_NUM_OF_REG_ALL];
	UINT									reg_data[D_RRH_PFM_NUM_OF_REG_ALL];
	UINT									count;
	UINT									count2;
	UINT									count3;
	UINT									index = 0;
	UINT									regAddr;
	
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_POL, D_DUS_REG_DLP_PMONLTALL, &LatchSet);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE_POL, D_DU_REG_ULP_PMONLTALL, &LatchSet);
	
	f_com_getSysTim_notMs( (T_RRH_SYSTEM_TIME *)&now );

	for(count = 0 ; count < D_RRH_PFM_NUM_OF_REG_COM ; count++){
		BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, f_pfmw_rxwin_reg_addr_com[count] , &reg_data[index] );
		reg_data_addr[index] = f_pfmw_rxwin_reg_addr_com[count];
		index++;
	}
	for(count= 0 ; count < f_pfmw_rxwin_reg_addr_offset_ccnum ; count++){
		for(count2 = 0; count2 < f_pfmw_rxwin_reg_addr_offset_antnum ; count2++){
			for( count3 = 0; count3 < D_RRH_PFM_NUM_OF_REG; count3++ ) {
				if(f_pfmw_rxwin_reg_addr[count3].regAddr == 0){
					continue;
				}
				regAddr = f_pfmw_rxwin_reg_addr[count3].regAddr + ( f_pfmw_rxwin_reg_addr_offset_cc * count) + (f_pfmw_rxwin_reg_addr_offset_ant * count2);
				BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, regAddr , &reg_data[index] );
				reg_data_addr[index] = regAddr;
				index++;
			}
		}
	}

	f_pfm_set_log_peri( &now, index * sizeof(UINT) , reg_data_addr ,  reg_data  );

	return	D_RRH_OK;
}

/* @} */

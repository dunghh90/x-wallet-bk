/********************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_fastDpdMode.c
 * @brief ハード自律PA-OFF時のDPD高速モード処理
 * @date 2020/01/23 FJT)長谷川哲 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2020
 */
/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:ハード自律PA-OFF時のDPD高速モード処理
 * @note  関数処理内容.
 *		-# ハード自律PA-OFF時のDPD高速モード処理(Sub6用)
 * @param	N/A
 * @return  VOID
 * @warning		N/A
 * @date 2020/01/23 FJT)Hasegawa
 
 */
VOID f_dpda_peri_fastDpdMode()
{
	UINT				antCnt;
    INT                 ret;
    UINT                allTxCarOFF_flag = D_RRH_ON; /* 全てのANTがTX-OFFか示すフラグ */
    UINT                TxCarOFF_flag[D_DPDA_ANTMAX] = {}; /* 個々のANTがTX-OFFか示すフラグ */
 	UINT				uiCarCnt;
	T_DPDA_ANT_INFO		*antInfo;
	T_DPDA_CAR_INFO		*carInfo;

    /* 全てのANTがTX-OFFの場合は処理をスキップする */
    for( antCnt = 0; antCnt < f_dpdaw_ant_max_num; antCnt++ ){
        antInfo = &(f_dpdaw_antCarStateInfo.ant_info[ antCnt ]);
        TxCarOFF_flag[antCnt] = D_RRH_ON;
        for( uiCarCnt=0; uiCarCnt < f_dpdaw_carrier_max_num; uiCarCnt++ ){
			carInfo = &(antInfo->car_info[ D_SYS_ANT_DL ][ uiCarCnt ]);
			if( D_DPDA_CAR_ON == carInfo->CarOnOff ){
                allTxCarOFF_flag = D_RRH_OFF;
                TxCarOFF_flag[antCnt] = D_RRH_OFF;
            }
        }
    }

    if(allTxCarOFF_flag == D_RRH_ON) {
        return ;
    }

    /* TCB_TXOFFSTA[0]とTCB_TXOFFSTA[30]が1の場合にDPD停止設定、100ms Wait、DPD_ON設定を行う */
    if( (f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A200_0720 ] & 0x1) && 
        (f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A200_0720 ] & 0x40000000)) {

        /* DPD停止設定 */
        for( antCnt = 0; antCnt < f_dpdaw_ant_max_num; antCnt++ ){
            if(TxCarOFF_flag[antCnt] == D_RRH_OFF) {
                ret = f_dpda_com_setDpdStop(antCnt);
                if(ret != D_DPDA_RTN_COMPLETE) {
                    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStop error=0x%08x", ret);
                }
            }
        }

        f_com_taskDelay(100,0,0,0);

        /* DPD_ON設定 */
        for( antCnt = 0; antCnt < f_dpdaw_ant_max_num; antCnt++ ){
            if(TxCarOFF_flag[antCnt] == D_RRH_OFF) {
               ret = f_dpda_com_setDpdStart(antCnt, D_RRH_OFF);
                if(ret != D_DPDA_RTN_COMPLETE) {
                    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStart error=0x%08x", ret);
                }
            }
        }
        f_dpda_com_clearQEC();   

		/* BFモードチェック (BFモード時のみ処理実行)	*/
		if( f_cmw_tra_inv_tbl->cmx_bfmode != D_RRH_OFF ) {
            for( antCnt = 0; antCnt < f_dpdaw_ant_max_num; antCnt++ ){
                ret = f_dpda_car_TxantcarStop(antCnt);
                if(ret != D_DPDA_RTN_COMPLETE) {
                    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TxantcarStop error=0x%08x ant=$d", ret, antCnt);
                }
                ret = f_dpda_car_RxantcarStop(antCnt);
                if(ret != D_DPDA_RTN_COMPLETE) {
                    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RxantcarStop error=0x%08x ant=%d", ret, antCnt);
                }
            }

            f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_ON;
            f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_ON;
            f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_ON;
            f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_ON;
		}

    }
    else if(f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A200_0720 ] & 0x1) {
        /* TCB_TXOFFSTA[0]が1, TCB_TXOFFSTA[30]が0の場合にQECカウンタクリアを行う */
        f_dpda_com_clearQEC();
    }

}

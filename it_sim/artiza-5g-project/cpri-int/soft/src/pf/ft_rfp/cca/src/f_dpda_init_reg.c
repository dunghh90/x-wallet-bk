/*!
 * @skip  $ld:$
 * @file  f_dpda_init_reg.c
 * @brief DPDAタスク レジスタ初期設定
 * @date 2018/08/13 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018-2020
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"
#include <sys/types.h> 

/*!
 * @brief 関数機能概要:DPDAタスクの初期化処理を行う
 * @note  関数処理内容.
 *       -# レジスタ初期設定
 *       -# RxCAL(起動時)
 *       -# TDDスイッチ部のレジスタ初期設定
 * @param  none
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/08/13 KCN)takagi Create
 * @date 2018/09/08 KCN)吉木   APDハソV2.01対応
 * @date 2019/02/22 KCN)吉木   3rdハソ対応
 * @date 2020/01/06 FJT)吉田   DCM5GDU-053-191227 [19BD] : キャリアリーク改善対応
*/
VOID f_dpda_init_reg()
{
	unsigned int 			uiSetData = 0;
    unsigned char           eep_data = 0;
	INT						ret;
    const T_DPDA_INIT_REG_SET_TBL *init2_reg_table_sub6;
    const T_DPDA_INIT_REG_SET_TBL *init2_reg_table_mmW;

	/* DU種別に応じ処理を切り分ける。*/
	switch(f_dpdaw_reKind)
	{
		/* sub6の場合 */
		case E_DPDA_REKIND_5GDU_SUB6:

            /* レジスタ初期設定 sub6共通[1] */
            for(int i = 0; i < D_DPDA_INIT_REG_NUM_SUB6; i++)
            {
                f_dpda_IO_write(
                    f_dpdar_init_reg_set_sub6[i].regAddr,
                    f_dpdar_init_reg_set_sub6[i].regData,
                    E_DPDA_BPF_NOMAL_MODE,
                    D_RRH_LOG_REG_LV_WRITE);
            }

            /* レジスタ初期設定 37G,45G個別 */
            if(f_cmw_tra_inv_tbl->cmx_device_BW == D_DU_EEP_DEV_FREQ_37G){
	            init2_reg_table_sub6 = f_dpdar_init2_reg_set_sub6_37G;
			}
			else {
				init2_reg_table_sub6 = f_dpdar_init2_reg_set_sub6_45G;
			}
			for(int i = 0; i < D_DPDA_INIT2_REG_NUM_SUB6; i++) {
				f_dpda_IO_write(
					init2_reg_table_sub6[i].regAddr,
					init2_reg_table_sub6[i].regData,
					E_DPDA_BPF_NOMAL_MODE,
					D_RRH_LOG_REG_LV_WRITE);
            }

            /* Beamforming Mode */
            if( f_comw_bf_mode->flag == D_RRH_OFF )
            {
                /* BF係数スルー設定 */
                uiSetData = 0x00000000;
                BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2014388, &uiSetData);
                BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2204388, &uiSetData);
                BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2401388, &uiSetData);
			}
			
			uiSetData= 3;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000040, &uiSetData);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000044, &uiSetData);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000048, &uiSetData);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB100004C, &uiSetData);

			/* CAL */
			if((ret = BPF_HM_DEVC_GET_ENV()) == BPF_HM_DEVC_COMPLETE ){
				BPF_COM_LOG_DMESG("Setting TX CAL ...");
				if((ret = f_dpda_txcal_startup()) == D_SYS_OK){
					BPF_COM_LOG_DMESG("OK\nSetting RX CAL ...");
					if((ret = f_dpda_rxcal_startup()) == D_SYS_OK){
						BPF_COM_LOG_DMESG("OK\n");
					}else{
						BPF_COM_LOG_DMESG("ERROR\n");
					}
				}else{
					BPF_COM_LOG_DMESG("ERROR\n");
				}
			}else{
				BPF_COM_LOG_DMESG("Skip TX,RX CAL ...\n");
			}

			BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_PAONOFF, &eep_data );
			if(eep_data == 0){
				uiSetData = 0;
			}else{
				uiSetData= 3;

			}
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000040, &uiSetData);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000044, &uiSetData);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000048, &uiSetData);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB100004C, &uiSetData);


			/* TDDスイッチ部のレジスタ初期設定 */
			f_dpda_init_reg_tddsw_sub6();
			break;
		
		/* mmWの場合 */
		case E_DPDA_REKIND_5GDU_MMW:

            /* レジスタ初期設定 */
            for(int i = 0; i < D_DPDA_INIT_REG_NUM_MMW; i++)
            {
                f_dpda_IO_write(
                    f_dpdar_init_reg_set_mmW[i].regAddr,
                    f_dpdar_init_reg_set_mmW[i].regData,
                    E_DPDA_BPF_NOMAL_MODE,
                    D_RRH_LOG_REG_LV_WRITE);
            }

            /* レジスタ初期設定 mmW(28G) */
	        init2_reg_table_mmW = f_dpdar_init2_reg_set_mmW_28G;
			for(int i = 0; i < D_DPDA_INIT2_REG_NUM_MMW; i++) {
				f_dpda_IO_write(
					init2_reg_table_mmW[i].regAddr,
					init2_reg_table_mmW[i].regData,
					E_DPDA_BPF_NOMAL_MODE,
					D_RRH_LOG_REG_LV_WRITE);
            }

			BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_PAONOFF, &eep_data );
			if(eep_data == 0){
				uiSetData = 0;
			}else{
				uiSetData= 3;

			}
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000040, &uiSetData);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000044, &uiSetData);

			/* TDDスイッチ部のレジスタ初期設定 */
			f_dpda_init_reg_tddsw_mmW();
			break;
		
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Impossible transition route. reKind = %d.", f_dpdaw_reKind );
			break;
	}
	
	return;
}

/* @} */

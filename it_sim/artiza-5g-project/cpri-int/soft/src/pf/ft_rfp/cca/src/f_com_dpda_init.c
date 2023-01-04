/*!
 * @skip  $ld:$
 * @file  f_com_dpda_init.c(base:f_dpda_init.c)
 * @brief APD初期化処理
 * @date  2011/07/26 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2015
 */

/*!
 * @addtogroup RRH_PF_COM
 * @{
 */

//#include "f_com_inc.h"
#include "f_dpda_inc.h"

/************************************************************************************************************/
/*!
 * @brief 関数機能概要:APD部初期化処理を行う
 * @note  関数処理内容.
 *       -# APDレジスタ初期値設定
 *       -# TX部トレーニングデータ設定
 * @param  dev_kind				[in]UCHAR	装置種別
 * @param  mmw_pwr_mode			[in]UCHAR	mmW電力モード(01:低電力モード 01以外:通常モード
 * @return VOID
 * @warning   N/A
 * @FeatureID PF-Rfp-001-001-001
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2011/07/26 ALPHA)横山 Create
 * @date 2011/09/26 ALPHA)横山 SRRU-2.1G対応
 * @date 2011/11/08 ALPHA)横山 M-RRU-ZSYS-00721対処
 * @date 2011/12/14 ALPHA)高橋 SRRU-2.1G-LTE対応
 * @date 2012/03/14 ALPHA)横山 N21GLTE対応
 * @date 2012/04/10 ALPHA)高橋 N21GLTE M-RRU-ZSYS-00842対応(ハソ10版)
 * @date 2012/05/08 ALPHA)高橋 N21GLTE M-RRU-ZSYS-00868対応
 * @date 2012/10/05 ALPHA)横山 MTCD対応
 * @date 2013/04/05 ALPHA)中村 B25対応
 * @date 2013/12/10 ALPHA)藤井 DCM-Zynq対応
 * @date 2015/09/07 ALPHA)藤原 DCM_3.5G TDD-RRE(Zynq)対応
 
 */
/************************************************************************************************************/
VOID f_com_dpda_init(UCHAR dev_kind)
{
	INT							ret=0;
	
	/****************************************************************************************************************/
	/* TX部トレーニングデータ設定																					*/
	/****************************************************************************************************************/
	
	/* RE種別に応じ処理を切り分ける。*/
	switch(dev_kind)
	{
		case D_DU_EEP_DEV_KIND_5GDU_SUB6:
			BPF_HM_DEVC_REG_WRITE_BITON( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_PWR, 0x00000001 ); /* PAPS FET-SW ON bit0 */
			usleep(200000);	 /* 200ms wait */

			f_com_dpda_init_trainingDataRead_Tx_sub6();  /* EEPROMからデータを展開(0x00～0x6FF 共通部含む) */
			f_com_dpda_init_trainingDataRead_Rx_sub6();  /* EEPROMからデータを展開(0x700～0x7FF) */
			f_com_dpda_init_trainingDataSet_Tx_sub6();   /* トレーニングデータの展開処理(sub6用) */
			/* RFIC/JESD起動関数 call */
//			ret = RFIC_TAL_START();
			/* 初期設定失敗時 */
			if (ret != RFIC_TALISE_COMPLETE)
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_TAL_START ERR ret = 0x%08x", (UINT)ret );

				/* retが0x00800000以上の場合 */
				/* retの上位：Fault ID / 下位：詳細Fault Code */
				if( (UINT)ret & D_RRH_SIGUSR_OPT_RSET_INIALM_NTC )
				{
					/* アラーム通知 */
					f_com_abort( D_SYS_THDID_PF_CCA, ret );
				}
			}
			else
			{
				/*	RFIC起動完了	*/
				f_dpdaw_rficSetupCmp = D_RRH_ON;
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "RFIC_TAL_START CMP:%d", (UINT)f_dpdaw_rficSetupCmp );
			}
			break;
		
		case D_DU_EEP_DEV_KIND_5GDU_MMW:
			f_com_dpda_init_RFtrainingDataRead_Tx_mmW(); /* RF用 EEPROMからデータを展開(0x00～0x5FF 共通部含む) */
			f_com_dpda_init_RFtrainingDataRead_Rx_mmW(); /* RF用 EEPROMからデータを展開(0x600～0x7FF) */
			f_com_dpda_init_trainingDataSet_Tx_mmW();                /* トレーニングデータの展開処理(mmW用) */
			/* mmW RFデバイス初期設定 関数起動 */
//			ret = RFIC_mmW_START();
			/* 初期設定失敗時 */
			if (ret != RFIC_MMW_COMPLETE)
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_START ERR ret = 0x%08x", (UINT)ret );

				/* retが0x00800000以上の場合 */
				/* retの上位：Fault ID / 下位：詳細Fault Code */
				/* ↑現状mmWからは詳細Fault Codeが設定される障害はないがsub6と同じ作りにする */
				if( (UINT)ret & D_RRH_SIGUSR_OPT_RSET_INIALM_NTC )
				{
					f_com_abort( D_SYS_THDID_PF_CCA, ret );
				}
			}
			else
			{
				/*	RFIC起動完了	*/
				f_dpdaw_rficSetupCmp = D_RRH_ON;
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "RFIC_mmW_START CMP:%d", (UINT)f_dpdaw_rficSetupCmp );
			}
			break;
		
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Impossible transition route. devKind = %d.", dev_kind );
			break;
	}
	return;
}

/* @} */

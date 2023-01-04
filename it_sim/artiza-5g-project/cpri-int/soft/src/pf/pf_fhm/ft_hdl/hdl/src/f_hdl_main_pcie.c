/*!
 * @skip  $ld:$
 * @file  f_hdl_main_pcie.c
 * @brief pf_hdl 子スレッド メイン処理(f_hdl_main_pcie)
 * @date  2015/06/11 ALPHA)tomioka Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */

#include "f_hdl_inc.h"
#include "f_com_inc.h"

/*!
 * @brief 関数機能概要: pf_hdl子スレッドメイン(f_hdl_main_pcie)
 * @note  関数処理内容.
 *       -# 割り込み通知先管理テーブルより、割り込み待ち種別を取得
 *       -# 割り込み待ち無限Loop処理へ移行する
 *          -# 割り込み受信関数をCallする(BPF_HM_DEVC_RESERVE)
 *          -# CPU Parity ALMを発生させる
 *          -# CPRI-FPGAへのアクセスを抑止する(BPF_HM_DEVC_FPGA_ACCESS)
 *
 * @return		N/A
 * @warning		N/A
 * @FeatureID	
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2015/06/11 ALPHA)tomioka Create
 * @date  2015/10/06 TDI)satou 「RST-SW操作時のIRQ通知 MIO[9]」の処理を追加
 */
VOID f_hdl_main_pcie()
{
	UINT endcd ;
	UINT readFactVal ;
	UINT regVal;

	/* スレッド名表示	*/
	prctl( PR_SET_NAME, "pf_hdpcie", 0, 0, 0 ) ;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_hdl_main_pcie] ENTER" );

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_F_PF,				/* ProcessID						*/
									D_SYS_THDID_PF_HDPCIE,		/* ThreadID							*/
									D_SYS_THDQID_PF_HDPCIE, (unsigned long int)pthread_self()) ;	/* ThreadQueueID					*/

	/****************************************************************************************************/
	/* 割り込み受信待ち																					*/
	/****************************************************************************************************/
	while( 1 )
	{
		/* 割り込み受信		*/
		endcd = BPF_HM_DEVC_RESERVE( BPF_IRQ_GPIO ) ; /* IRQ割り込み待ち処理				*/

		/* エラーログ取得	*/
		if( endcd != BPF_HM_DEVC_COMPLETE ) {
			/* エラー記録	*/
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " error BPF_HM_DEVC_RESERVE(ret:%d)", endcd );
			break ;
		}
		
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_GPIO_INT_STAT_0, &regVal);
		if (0 != (regVal & 0x200)) {    /* RST-SW操作時のIRQ通知 MIO[9] */
		    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Reset switch");

		    /* 割り込み要因クリア */
		    regVal = 0x200;
		    BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_GPIO_INT_STAT_0, &regVal);

	        /* INITプロセスにリセット発生シグナルを送信  */
	        BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI, SIGUSR2, D_RRH_SIGUSR2_OPT_RSTSW);
		} else {                        /* PCIe error */
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "PCIe Link(rootpoint) disconnect" );

            /* 割り込み要因ビット読み込み	*/
            BPF_HM_DEVC_REG_READ_BIT( D_RRH_LOG_REG_LV_HDL_READ, D_RRH_REG_CNT_STA1, D_RRH_CNT_STA1_PCIEERR , &readFactVal ) ;
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "IRQ1 Factor 0x%08x", readFactVal ) ;

            /* FPGAアクセス抑止設定 */
            BPF_HM_DEVC_FPGA_ACCESS( D_SYS_OFF );

//TODO 書き込み無効のレジスタなので、意味がないのではないか
            /* 割り込みクリア	*/
            endcd = BPF_HM_DEVC_REG_WRITE_BITON( D_RRH_LOG_REG_LV_HDL_READ_WRITE, D_RRH_REG_CNT_STA1, D_RRH_CNT_STA1_PCIEERR ) ;

            /* アラーム設定(FPGA-IF ALM_1(PCIe-ALM)) */
            f_com_abort(D_SYS_THDID_PF_HDPCIE, D_RRH_ALMCD_FPGAIF1 );
		}
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_hdl_main_pcie] RETURN" );
	
	return ;
}

/* @} */

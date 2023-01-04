/*!
 * @skip		$ld:$
 * @file		f_main_PCIe.c
 * @brief		PCIe初期設定
 * @author		alpha)tomioka
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/*!
 * @brief		f_main_PCIe_Chkconnect
 * @note		PCIe接続確認
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 */
UINT f_main_PCIe_Chkconnect()
{
	UINT	regw_data = 0;
	UINT	regr_data = 0;
	
	/* FPGAレジスタWアクセス	*/
	regw_data=0x5555AAAA;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_CNTS_REGCHK, &regw_data );
	
	/* FPGAレジスタRアクセス	*/
	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNTS_REGCHK, &regr_data );

	/* FPGAレジスタW/Rチェック	*/
	if(regr_data != regw_data)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "PCIe_Chk_con NG reg=0x%08x",  regr_data );
		return( D_SYS_NG );
	}

	/* FPGAレジスタWアクセス	*/
	regw_data=0xAAAA5555;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_CNTS_REGCHK, &regw_data );

	/* FPGAレジスタRアクセス	*/
	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNTS_REGCHK, &regr_data );

	/* FPGAレジスタW/Rチェック	*/
	if(regr_data != regw_data)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "PCIe_Chk_con NG reg=0x%08x",  regr_data );
		return( D_SYS_NG );
	}

	/* CNTM/SNTS STA1マスク解除	*/
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_CNT_STA1M,  D_RRH_CNT_STA1M_PCIEERR );
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_CNTS_STA1M, D_RRH_CNTS_STA1M_PCIEERR );

	BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNT_STA1, D_RRH_CNT_STA1_PCIEERR, &regr_data);
	if( regr_data != 0 )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "PCIe_endpoint NG reg=0x%08x", regr_data);
		return( D_SYS_NG );
	}
	BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_RRH_REG_CNTS_STA1, D_RRH_CNTS_STA1_PCIEERR, &regr_data);
	if( regr_data != 0 )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "PCIe_endpoint NG reg=0x%08x", regr_data);
		return( D_SYS_NG );
	}

	return( D_SYS_OK );
}

/*!
 * @brief		f_main_PCIe_interrupt
 * @note		PCIe Interrupt マスク解除設定
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 */
UINT f_main_PCIe_interrupt()
{
	UINT	regw_data	= 0;
	
	/*初期化 全Interrupt Enable設定([D17:D16]を除く)	*/
	regw_data = D_RRH_BIT_PCI_INTRPT_MR2;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_PCI_INTRPT_MR, &regw_data);
	
	/* 初期化 全Interrupt クリア設定	*/
	regw_data = D_RRH_BIT_PCI_INTRPT_DR;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_PCI_INTRPT_DR, &regw_data);


	/*	PCIe障害監視開始はpf_hndl */
	/*  PCIeステータス情報ログ取得開始はpf_dpd(1s周期)で実施 */
	
	return( D_SYS_OK );
}

/*!
 * @brief		f_main_PCIe_endpoint
 * @note		Initialize Endpoint設定（PCIe Fabric）
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 */
UINT f_main_PCIe_endpoint()
{
	UINT	regw_data	= 0;
	UINT	regr_data	= 0;
	
	/* BIST/Header/Lat Timer/Cache Ln確認	*/
	BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ, D_RRH_REG_PCI_BIST_HEAD_LT_CL, D_RRH_BIT_PCI_BIST_HEAD_LT_CL, &regr_data);
	if( regr_data != 0 )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "PCIe_endpoint NG reg=0x%08x", regr_data);
		return( D_SYS_NG );
	}

	/*	Status/Command Read  Bus Master enable/Memory access enable Write	*/
	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_PCI_STATE_CMD, D_RRH_BIT_PCI_STATE_CMD);

	/* Base Address Register 0設定	*/
	regw_data = D_RRH_BIT_PCI_BASE_ADDREG;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_PCI_BASE_ADDREG, &regw_data);
	
	/*	Bridge Enable	*/
	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_PCI_ROOTPORT_STS_CTL, D_RRH_BIT_PCI_ROOTPORT_STS_CTL_BRGEN);
	
	return( D_SYS_OK );
}

/*!
 * @brief		f_main_PCIe_root_complex
 * @note		Initialize Root Complex設定
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 * @date        2015/08/06 ALPHA) fujiwara Modify RRH-007-000 M-RRU-ZSYS-01656対応
 */
UINT f_main_PCIe_root_complex()
{
	UINT	cnt;
	UINT	regw_data	= 0;
	UINT	regr_data	= 0;
    UINT    onoff_retry_cnt =0;
	
	/* 初期化 全Interrupt マスク設定 (全Interrupt Disable設定)	*/
	regw_data = D_RRH_BIT_PCI_INTRPT_MR;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_PCI_INTRPT_MR, &regw_data);
	
	/* 初期化 全Interrupt クリア設定	*/
	regw_data = D_RRH_BIT_PCI_INTRPT_DR;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_PCI_INTRPT_DR, &regw_data);
	
    /* LinkDown 5回リトライアウト時、PCIeリセットON/OFFを実施し再度トライ           */
    /* PCIeリセットON/OFFのリトライが3回以上となった場合、PCIe初期設定異常ALMとする */
    for( onoff_retry_cnt = 0; onoff_retry_cnt < 3; onoff_retry_cnt++ )
    {
        /* Root側のPCIe PHY Status Link Up確認          */
        /* Link Down 時は、5回リトライ(1回/10ms周期)   */
        for( cnt = 0; cnt < 5; cnt++ )
        {
            /*  10ms wait*/
            f_com_taskDelay(10, 0, 0, 0);

            /* Link Up  bit11：1[bin] Link Up    */
            BPF_HM_DEVC_REG_READ_BIT(D_RRH_LOG_REG_LV_READ ,D_RRH_REG_PCI_PHY_STS_CTL, D_RRH_BIT_PCI_PHY_STS_CTL_LINKUP, &regr_data);
            if( regr_data == D_RRH_BIT_PCI_PHY_STS_CTL_LINKUP )
            {
                break;
            }
        }
        /* LinkUPならLoopを抜け、LinkDwnならPCIeリセットON/OFFを実行 */
        if( regr_data == D_RRH_BIT_PCI_PHY_STS_CTL_LINKUP )
        {
            break;
        }
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "PCIe PHY Status LinkDwn RetryOut=%d, reg=0x%08x", onoff_retry_cnt, regr_data);
        /* 0xf800_0240 <= 0xE  (PCIeリセットON)  */
        regw_data = 0x0000000E;
        BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_SLCR_ZYNQ_FPGA_RST_CTRL, &regw_data);
        /* 0xf800_0240 <= 0xC  (PCIeリセットOFF) */
        regw_data = 0x0000000C;
        BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_SLCR_ZYNQ_FPGA_RST_CTRL, &regw_data);
    }

	if( regr_data != D_RRH_BIT_PCI_PHY_STS_CTL_LINKUP )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "PCIe_root_complex NG reg=0x%08x",  regr_data );
		return( D_SYS_NG );
	}

	/*	Command Register	*/
    /* M-RRU-ZSYS-01656対応 */
    BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_PCI_CMD_REG, D_RRH_BIT_PCI_CMD_REG);

	/*	Second Lat Timer/Sub Bus Number/Second Bus Number/Primary Bus Number	*/
	regw_data = D_RRH_BIT_PCI_SLT_BUSNUM;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_PCI_SLT_BUSNUM, &regw_data);

	return( D_SYS_OK );
}

/*!
 * @brief		f_main_PCIe
 * @note		PCIe初期設定
 * @param		-
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 */
UINT f_main_PCIe()
{
	INT		rtn = D_SYS_OK;
	UINT	regw_data	= 0;
	
	/* PCIeリセット解除				*/
	regw_data = D_RRH_BIT_SLCR_ZYNQ_FPGA_RST_CTRL_XRSTCNTS;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_SLCR_ZYNQ_FPGA_RST_CTRL, &regw_data);
	
	/* Initialize Root Complex設定 */
	rtn = f_main_PCIe_root_complex();
	if(rtn != D_SYS_OK)
	{
		return rtn;
	}
	
	/* Initialize Endpoint設定（PCIe Fabric）*/
	rtn = f_main_PCIe_endpoint();
	if(rtn != D_SYS_OK)
	{
		return rtn;
	}
	
	/* PCIe Interrupt マスク解除設定 */
	rtn = f_main_PCIe_interrupt();
	if(rtn != D_SYS_OK)
	{
		return rtn;
	}
	
	/* スレーブFPGAリセット解除 */
	regw_data = D_RRH_BIT_SLCR_ZYNQ_FPGA_RST_CTRL_DEAST_RST;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_SLCR_ZYNQ_FPGA_RST_CTRL, &regw_data);
	
	/* すべてOKとなったのでPCIe接続可能と判断する */
	BPF_HM_DEVC_FPGA_ACCESS(D_RRH_ON);
	
	/* PCIe接続確認 PCIe Endpoint Kintex FPGA レジスタR/Wアクセス */
	rtn = f_main_PCIe_Chkconnect();
	if(rtn != D_SYS_OK)
	{
		return rtn;
	}
	
	return( D_SYS_OK );
}

/* @} */

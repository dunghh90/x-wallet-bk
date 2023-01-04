
/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_ASSIGN_ONOFF.c
 *  @brief  
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <sys/syscall.h>
#include "bpf_i_hm_devc.h"


/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  アサインフラグONOFF機能
 *  @note   アサインフラグONOFF機能
 *  @param  assign_onoff  [out]   0:off 1:on
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_ASSIGN(unsigned int assign_onoff)
{

	/* 入力値判定 */
	if((assign_onoff != DEVC_FLG_ON) && (assign_onoff != DEVC_FLG_OFF))
	{
		return BPF_HM_DEVC_NG;
	}
	/* デバイスの初期化が完了する前はソフトタイマで実施する */
	di_devc_assign_flag = assign_onoff;
	return BPF_HM_DEVC_COMPLETE;

}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  デバイス強制アクセスモード設定
 *  @note   デバイス強制アクセスモード設定
 *  @param  force_access  [in]   0:変更なし 1～0x7FFFFFFF:on  -1:off
 *  @return 設定結果
 *  @date   2014/01/30 FJT)Tanigchi create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_FORCE_ACCESS( int force_access )
{
    unsigned char buf[D_RRH_BUFSIZE_4096];			/* PAGE_SIZE */
   /* sharedmem attached ? */
    if( mincore(di_devc_data_p,1,buf) )
    {
        /* already sharedmem dettach */
        return -2;
    }
    if (force_access > 0)
    {
        di_devc_data_p->force_access |= force_access;
        di_devc_data_p->force_pid = getpid();
        di_devc_data_p->force_tid = (int)(syscall(SYS_gettid));
    }
    else if(force_access == -1)
    {
        di_devc_data_p->force_access = 0;
        di_devc_data_p->force_pid = 0;
        di_devc_data_p->force_tid = 0;
    }
    else
    {
        ; /* get */
    }
    return ( di_devc_data_p->force_access );
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  FPGAアクセス可否設定
 *  @note   FPGAアクセス可否設定
 *  @param  checkFlg  [in]   0:アクセス不可設定  0以外:アクセス可能設定
 *  @return 固定値
 *  @date   2014/12/03 ALPHA)Kuwamura create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_FPGA_ACCESS( int checkFlg )
{
    di_devc_data_p->fpga_access = checkFlg;
    return 0;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL-CONFIG状態設定
 *  @note   PL-CONFIG状態設定
 *  @param  state  [in]   0:PL-CONFIG未完了  0以外:PL-CONFIG完了
 *  @return 固定値
 *  @date   2014/12/25 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PL_CONFIG_STATE_SET( int state )
{
    di_devc_data_p->pl_config = state;
    return 0;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  MUTEX LOCK強制解放設定
 *  @note   MUTEX LOCK強制解放設定
 *  @param  none
 *  @return none
 *  @date   2015/03/19 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
void BPF_HM_DEVC_MUTEX_LOCK_FREE()
{
	int			loop_cnt;
	
	/* プロセス死亡時、シグナルハンドラでmutexのみ解除するための関数							*/
	/* (ハンドラ内でBPF_RM_SVRM_UNASSIGNを使用するとSIGCHLDが発生してしまうため本関数を用意)	*/
	for( loop_cnt = 0; loop_cnt < (sizeof(di_devc_data_fd)/sizeof(t_bpf_hm_devc_fd_info)); loop_cnt++ )
	{
		if(di_devc_data_fd.info[loop_cnt].validflg==DEVC_FLG_ON){
			pthread_mutex_unlock( &(di_devc_data_p->info[loop_cnt].mutex) );
		}
	}
	
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  spi 初期処理(SPI用のINIT処理)
 *  @note   SPI用の初期処理を行う
 *  @param  -
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/07/02 ALPHA)nakajima create
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応
 *  @date   2015/06/03 ALPHA)Yokoyama    TDD(ZYNQ)対応 PPCから流用して改造
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
void BPF_HM_DEVC_SPI_INIT()
{
	pthread_mutex_t		*mutex;
	unsigned int		rData = 0;
	unsigned int		wData = 0;
	unsigned int		uiCnt = 0;
	unsigned int		uiSPI;
	unsigned int		uiMutex[2] = { BPF_HM_DEVC_SPI_0, BPF_HM_DEVC_SPI_1 };
	unsigned int		uiBaseAddr[2] = { BPF_HM_ZYNQ_SPI0_OFFSET, BPF_HM_ZYNQ_SPI1_OFFSET };
	unsigned int		uiConfigVal[2] = { 0x00027C1D, 0x00027C19 };
	
	/* SPI#0/SPI#1の初期設定をループして行う	*/
	for( uiSPI = 0; uiSPI < 2; uiSPI++ )
	{
		/************************************************************/
		/* SPI#X 初期設定                                           */
		/************************************************************/
		/* Mutex lock */
		mutex = &di_devc_data_p->info[ uiMutex[uiSPI] ].mutex;
		(VOID)bpf_hm_devc_mutex_lock(mutex);
		
		/* レジスタ名やBIT位置はug585-Zynq-7000-TRM.pdfから転記	*/
		
		/* En_reg0 に0x00000000をwrite */
		wData = 0x00000000;
		(VOID)BPF_HM_DEVC_GPIO_WRITE(D_RRH_LOG_REG_LV_WRITE, (uiBaseAddr[ uiSPI ] + BPF_HM_ZYNQ_SPI_EN), &wData);
		
		/* Config_reg0 にwrite(値はSPI#0/SPI1で異なる) */
		wData = uiConfigVal[ uiSPI ];
		(VOID)BPF_HM_DEVC_GPIO_WRITE(D_RRH_LOG_REG_LV_WRITE, (uiBaseAddr[ uiSPI ] + BPF_HM_ZYNQ_SPI_CONFIG), &wData);
		
		/* RX_FIFO_not_empty="0"の確認 */
		/* Rx_data_reg0 をreadした後 Intr_status_reg0 をreadしRX_FIFO_not_emptyのBITを判定 */
		for( uiCnt = 0; uiCnt < BPF_HM_DEVC_SPI_FIFO; uiCnt++ )
		{
			/* Rx_data_reg0 READ */
			(VOID)BPF_HM_DEVC_GPIO_READ(D_RRH_LOG_REG_LV_READ, (uiBaseAddr[ uiSPI ] + BPF_HM_ZYNQ_SPI_RX_DATA), &rData);
			
			/* Intr_status_reg0 READ */
			(VOID)BPF_HM_DEVC_GPIO_READ(D_RRH_LOG_REG_LV_READ, (uiBaseAddr[ uiSPI ] + BPF_HM_ZYNQ_SPI_INTR_STATUS), &rData);
			
			/* BITが立っていなかったら抜ける	*/
			if( (rData & BPF_HM_ZYNQ_SPI_INTR_STATUS_IXR_RXNEMPTY) == 0 )
			{
				break;
			}
		}
		
		/* Mutex unlock */
		pthread_mutex_unlock(mutex);
		
		/* 規定回数を超えていた場合はここで終了	*/
		if (uiCnt >= BPF_HM_DEVC_SPI_FIFO)
	   	{
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"SPI#%d Not empty RX fifo. Data = %d", uiSPI, rData);
			return;
		}
	}
	
	return;
}
/* @} */

/* FLASH EOL対応 add start */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL-FLASH-ID設定
 *  @note   PL-FLASH-ID設定
 *  @param  id  [in] Manufacturer ID 0x20:Micron/0xC2:Macronix
 *  @return 固定値
 *  @date   2017/07/01 FJT)a.oohashi create 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_PL_FLASH_ID_SET( unsigned char id )
{
	/* Manufacturer ID = 0xC2(Macronix) */
    if (id == 0xC2)
    {
		di_devc_data_p->flash_kind = 1;
	}
	/* 0xC2(Macronix)以外は従来FLASH指定(Micron) */
	else
	{
		di_devc_data_p->flash_kind = 0;
	}
    return 0;
}
/* @} */
/* FLASH EOL対応 add end */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  device環境取得
 *  @note   device環境取得
 *  @param  N/A
 *  @return BPF_HM_DEVC_COMPLETE
 *  @date   2017/07/01 FJT)Taniguchi
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_GET_ENV( void )
{
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */

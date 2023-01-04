/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_WDT.c
 *  @brief  
 *  @date   2015/04/27 ALPHA)yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  WDT設定
 *  @note   BPF_HM_DEVC_WDT_SET
 *  @param  uiSec_AXI   [in]   AXTタイマ設定値
 *  @param  uiSec_SWDT  [in]   SWDTタイマ設定値
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2015/04/27 ALPHA)yokoyama create
 *  @date   2015/06/03 ALPHA)yokoyama mRRUから流用 & 修正
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_WDT_SET(unsigned int uiSec_AXI, unsigned int uiSec_SWDT)
{
	int					rtn;
	unsigned int		wData = 0;
	pthread_mutex_t		*mutex;         /* mutexオブジェクト    */
	
	/* デバイスの初期化が完了する前は処理スキップする */
	if(di_devc_assign_flag != DEVC_FLG_ON){
		return BPF_HM_DEVC_NG;
	}
	
	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_WDT ].mutex;			/* mutexオブジェクト      */
	/* Mutex lock */
	rtn = pthread_mutex_lock(mutex);
	if( rtn != 0 ){
	
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"pthread_mutex_lock mutex = %08x, error = %d", mutex,errno);
	    return BPF_HM_DEVC_NG;
	}
	
	/* タイマ値が 0 の場合＝停止											*/
	/* タイマ値が 0xFFFFFFFFの場合＝何もしない								*/
	/* タイマ値が 上記以外の場合＝タイマ設定								*/
	
	/********************************************************************************************************/
	/*  AXI_Timer(IP Macro)設定																				*/
	/********************************************************************************************************/
	if( 0 == uiSec_AXI )
	{
		/* AXI停止	*/
		wData = 0x0;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_AXITIMERCLRPLS0, &wData);
		/* フラグOFF	*/
		di_devc_data_p->wdt_set_axi  = DEVC_FLG_OFF;
	}
	else if( 0xFFFFFFFF == uiSec_AXI )
	{
		/* 何もしない	*/
	}
	else
	{
		/*	AXI_Timer クリア				*/
		wData = BPF_HM_DEVC_DATA_AXITIMER_CLR;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_AXITIMERCLRPLS0, &wData);
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_AXITIMERCLRPLS1, &wData);
		
		/* Timer0 カウンタ設定	*/
		wData = BPF_HM_DEVC_DATA_AXITIMER_VAL;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_AXITIMERTLR0, &wData );
		
		/* Timer0 カウンタ設定Load	*/
		wData = BPF_HM_DEVC_DATA_AXITIMER_CNTLOAD;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_AXITIMERCLRPLS0, &wData );
		
		/* Timer0 EN設定	*/
		wData = BPF_HM_DEVC_DATA_AXITIMER_EN;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_AXITIMERCLRPLS0, &wData );
		
		/* フラグON	*/
		di_devc_data_p->wdt_set_axi  = DEVC_FLG_ON;
	}
	
	
	/********************************************************************************************************/
	/*  System Watchdog Timer (SWDT)設定																	*/
	/********************************************************************************************************/
	if( 0 == uiSec_SWDT )
	{
		/* SWDT停止	*/
		/* SWDT Disable */
		wData = BPF_HM_DEVC_DATA_SWDT_MODE_DIS;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_SWDT_MODE, &wData);
		
		/* フラグOFF	*/
		di_devc_data_p->wdt_set_swdt  = DEVC_FLG_OFF;
	}
	else if( 0xFFFFFFFF == uiSec_SWDT )
	{
		/* 何もしない	*/
	}
	else
	{
		/* SWDT Disable */
		wData = BPF_HM_DEVC_DATA_SWDT_MODE_DIS;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_SWDT_MODE, &wData);
		if(uiSec_SWDT == 0xEEEEEEEE){
			wData = BPF_HM_DEVC_DATA_SWDT_CONTROL_TIMER_EXT;
		}else{
			wData = BPF_HM_DEVC_DATA_SWDT_CONTROL_TIMER;
		}
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_SWDT_CONTROL, &wData);

		/* SWDT Enable */
		wData = BPF_HM_DEVC_DATA_SWDT_MODE_EN;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_SWDT_MODE, &wData);
		
		/* SWDTタイマクリア */
		wData = BPF_HM_DEVC_DATA_SWDT_CLEAR;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_SWDT_CLEAR, &wData );
		
		/* フラグON	*/
		di_devc_data_p->wdt_set_swdt  = DEVC_FLG_ON;
	}
	
	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  WDT clear
 *  @note   BPF_HM_DEVC_WDT_CLEAR
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2015/04/27 ALPHA)yokoyama create
 *  @date   2015/06/03 ALPHA)yokoyama mRRUから流用 & 修正
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_WDT_CLEAR( )
{
	unsigned int     wData = 0;
	int              rtn;
	pthread_mutex_t  *mutex;         /* mutexオブジェクト    */
	
	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_WDT ].mutex;			/* mutexオブジェクト      */
	/* Mutex lock */
	rtn = pthread_mutex_lock(mutex);
	if( rtn != 0 )
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"pthread_mutex_lock mutex = %08x, error = %d",mutex,errno);
	    return BPF_HM_DEVC_NG;
	}
	
	/* WDT起動前にCLEARが動作すると割り込みが上がってしまう	*/
	/* そのためWDT起動時にしかCLEAR処理は行わない			*/
	
	if( di_devc_data_p->wdt_set_axi == DEVC_FLG_ON )
	{
		/* AXI_TIMER(IP Macro)クリア設定 */
		/* Timer0 カウンタ設定Load	*/
		wData = BPF_HM_DEVC_DATA_AXITIMER_CNTLOAD;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_AXITIMERCLRPLS0, &wData );
		
		/* Timer0 EN設定	*/
		wData = BPF_HM_DEVC_DATA_AXITIMER_EN;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_AXITIMERCLRPLS0, &wData );
	}

	if( di_devc_data_p->wdt_set_swdt == DEVC_FLG_ON )
	{
		/* SWDT制御レジスタへクリア設定*/
		wData = BPF_HM_DEVC_DATA_SWDT_CLEAR;
		(VOID)BPF_HM_DEVC_REG_WRITE(0, BPF_HM_DEVC_REG_SWDT_CLEAR, &wData);
	}

	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)yokoyama add end */

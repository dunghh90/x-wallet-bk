/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_LED_cntl.c
 *  @brief  LED control function
 *  @date   2018/08/22 KCN)Fujita create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_EQS
* @{ */
#include "f_eqs_header.h"			/* 5GDU-LLS監視タスクヘッダファイル			*/

/* @{ */
/**
* @brief API LED Setting Request process function
* @note Update LED control regiseter.\n
* @param ledinf_p [in] the buffer address pointer of received message
* @return None
* @date   2018/08/22 KCN)Fujita create
* @warning	N/A
* @FeatureID	
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/

//VOID f_rsv_LedSet( T_API_SVP_LEDSET_NTC* ledinf_p )
VOID f_eqs_LED_cntl( UINT *ledSta )
{
	USHORT	usLoop, ledLoop;
	UINT	regdat;		/* register data */
	UINT	ledVal[E_EQS_LED_MAX] = { D_EQS_LED1SET_PREVAL, D_EQS_LED1SET_PREVAL, D_EQS_LED1SET_PREVAL, D_EQS_LED1SET_PREVAL, D_EQS_LED1SET_PREVAL };
	
	/*************************/
	/* Get LED setting value */
	/*************************/
	/*	LED数分LOOP	*/
	for( ledLoop=E_EQS_LED1; ledLoop<E_EQS_LED_MAX; ledLoop++ )
	{
		/*	該当するLED設定値を検索	*/
		for(usLoop = D_SYS_NUM0; usLoop < D_SYS_NUM8; usLoop++)
		{
			if(f_eqs_ledInfo[ledLoop][usLoop].ledSta == ledSta[ledLoop])
			{
				ledVal[ledLoop] = f_eqs_ledInfo[ledLoop][usLoop].ledVal;
				break;
			}
		}
	}
	/***********/
	/* Set LED */
	/***********/
	/*	LED1,2設定値が有効値の場合	*/
	if((ledVal[E_EQS_LED1] != D_EQS_LED1SET_PREVAL) && (ledVal[E_EQS_LED2] != D_EQS_LED2SET_PREVAL) &&
	   (ledVal[E_EQS_LED3] != D_EQS_LED2SET_PREVAL) && (ledVal[E_EQS_LED4] != D_EQS_LED2SET_PREVAL) &&
	   (ledVal[E_EQS_LED5] != D_EQS_LED2SET_PREVAL))
	{
		regdat = (ledVal[E_EQS_LED1] | ledVal[E_EQS_LED2] | ledVal[E_EQS_LED3] | ledVal[E_EQS_LED4] | ledVal[E_EQS_LED5]);
		
		/*	LEDに変化がある場合	*/
		if(regdat != f_eqs_ledRegVal)
		{
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DU_REG_CTB_SYSLED, &regdat);
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"CARD LED SET pre 0x%08x -> next 0x%08x",f_eqs_ledRegVal , regdat);
			f_eqs_ledRegVal = regdat;
		}
	}
	else
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"CARD LED VAL ERR pre LED1STA=0x%08x, LED2STA=0x%08x", ledSta[E_EQS_LED1], ledSta[E_EQS_LED2]);
	}
	return;
}

/* @} */

/* @} */


/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_rsv_LedSet_zynq.c
 *  @brief  API LED Setting Request process function
 *  @date   2013/11/08 FFCS)Xut create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */
#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/

/* @{ */
/**
* @brief API LED Setting Request process function
* @note Update LED control regiseter.\n
* @param ledinf_p [in] the buffer address pointer of received message
* @return None
* @date   2013/11/25 FFCS)Xut create
* @date   2015/09/04 TDI)satou LED1-3をFHM用に修正
* @date   2015/09/05 TDI)satou CPRI LINK LEDの処理を追加
* @warning	N/A
* @FeatureID	PF-Svp-003-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_rsv_LedSet( T_API_SVP_LEDSET_NTC* ledinf_p )
{
	USHORT usLoop;
	UINT	regdat;		                /* register data */
	UINT	led1Sta;		            /* LED1 state 	*/
	UINT	led2Sta;		            /* LED2 state	*/
	UINT    led3Sta;                    /* LED3 state   */
	UINT	led1Val = D_SYS_NUM0;
	UINT	led2Val = D_SYS_NUM0;
	UINT    led3Val = D_SYS_NUM0;
	UINT    ledVal_CpriAct;
	UINT    ledVal_CpriDwn;
	UINT	setVal;
	UINT    linkno;                     /* CPRIリンク番号 */

	/* Get requested LED1, LED2, LED3 state */
	led1Sta = ledinf_p->led1Sta;
	led2Sta = ledinf_p->led2Sta;
	led3Sta = ledinf_p->led3Sta;

	/* Get LED1 setting value */
	for(usLoop = 0; usLoop < (sizeof(f_rsvr_led1Info)/sizeof(f_rsvr_led1Info[0])); usLoop++)
	{
		if(f_rsvr_led1Info[usLoop].ledSta == led1Sta)
		{
			led1Val = f_rsvr_led1Info[usLoop].setVal;
			break;
		}
	}

	/* Get LED2 setting value */
	for(usLoop = 0; usLoop < (sizeof(f_rsvr_led2Info)/sizeof(f_rsvr_led2Info[0])); usLoop++)
	{
		if(f_rsvr_led2Info[usLoop].ledSta == led2Sta)
		{
			led2Val = f_rsvr_led2Info[usLoop].setVal;
			break;
		}
	}

	/* Get LED3 setting value */
	for(usLoop = 0; usLoop < (sizeof(f_rsvr_led3Info)/sizeof(f_rsvr_led3Info[0])); usLoop++)
	{
	    if(f_rsvr_led3Info[usLoop].ledSta == led3Sta)
	    {
	        led3Val = f_rsvr_led3Info[usLoop].setVal;
	        break;
	    }
	}

	/* Get LED setting value */
	setVal = led1Val|led2Val|led3Val;

	/* Get register data from LED control register */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, 
				D_RRH_REG_CNT_SYSLED, &regdat);

	/* If LED setting value is changed */
	if(setVal != regdat)
	{
		/* Update LED control register */
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,
					D_RRH_REG_CNT_SYSLED, &setVal);

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"CARD LED SET pre 0x%08x -> next 0x%08x",regdat,setVal);

#ifdef OPT_RRH_SIM
		{
		    char strLedSt1[20] = {0};
		    char strLedSt2[20] = {0};
		    char strLedSt3[20] = {0};
		    if     (0           == ((BIT1|BIT0) & setVal)) strcpy(strLedSt1, "OFF"  );
		    else if(BIT1        == ((BIT1|BIT0) & setVal)) strcpy(strLedSt1, "BLINK");
		    else if((BIT1|BIT0) == ((BIT1|BIT0) & setVal)) strcpy(strLedSt1, "ON"   );
		    else                                           strcpy(strLedSt1, "INVALID");
            if     (0           == ((BIT3|BIT2) & setVal)) strcpy(strLedSt2, "OFF"  );
            else if(BIT3        == ((BIT3|BIT2) & setVal)) strcpy(strLedSt2, "BLINK");
            else if((BIT3|BIT2) == ((BIT3|BIT2) & setVal)) strcpy(strLedSt2, "ON"   );
            else                                           strcpy(strLedSt2, "INVALID");
            if     (0           == ((BIT9|BIT8) & setVal)) strcpy(strLedSt3, "OFF"  );
            else if(BIT9        == ((BIT9|BIT8) & setVal)) strcpy(strLedSt3, "BLINK");
            else if((BIT9|BIT8) == ((BIT9|BIT8) & setVal)) strcpy(strLedSt3, "ON"   );
            else                                           strcpy(strLedSt3, "INVALID");
		    printf("Set LED: address:%08X value:%08X LED1:%s LED2:%s LED3:%s\n", D_RRH_REG_CNT_SYSLED, setVal, strLedSt1, strLedSt2, strLedSt3);
		}
#endif
	}


	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++) {
	    ledVal_CpriAct = ledVal_CpriDwn = 0;

	    /* Get CPRI LINK LED(ACT LED) setting value */
	    for (usLoop = 0; usLoop < (sizeof(f_rsvr_ledCpriActInfo)/sizeof(f_rsvr_ledCpriActInfo[0])); usLoop++) {
	        if (f_rsvr_ledCpriActInfo[usLoop].ledSta == ledinf_p->ledCpriAct[linkno]) {
	            ledVal_CpriAct = f_rsvr_ledCpriActInfo[usLoop].setVal;
	            break;
	        }
	    }

	    /* Get CPRI LINK LED(DWN LED) setting value */
	    for (usLoop = 0; usLoop < (sizeof(f_rsvr_ledCpriDwnInfo)/sizeof(f_rsvr_ledCpriDwnInfo[0])); usLoop++) {
	        if (f_rsvr_ledCpriDwnInfo[usLoop].ledSta == ledinf_p->ledCpriDwn[linkno]) {
	            ledVal_CpriDwn = f_rsvr_ledCpriDwnInfo[usLoop].setVal;
	            break;
	        }
	    }

	    /* Get LED setting value */
	    setVal = ledVal_CpriAct | ledVal_CpriDwn;

	    /* Get register data from LED control register */
	    BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, M_RRH_REG_CNT_CPSLED(linkno), &regdat);

	    /* If LED setting value is changed, update LED control register */
	    if (setVal != regdat) {
	        BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_RRH_REG_CNT_CPSLED(linkno), &setVal);
	        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"CPRI LED #%d SET pre 0x%08x -> next 0x%08x",linkno,regdat,setVal);

#ifdef OPT_RRH_SIM
	        {
	            char strLedStDwn[20] = {0};
	            char strLedStAct[20] = {0};
	            if     (0           == ((BIT1|BIT0) & setVal)) strcpy(strLedStAct, "OFF"  );
	            else if(BIT1        == ((BIT1|BIT0) & setVal)) strcpy(strLedStAct, "BLINK");
	            else if((BIT1|BIT0) == ((BIT1|BIT0) & setVal)) strcpy(strLedStAct, "ON"   );
	            else                                           strcpy(strLedStAct, "INVALID");
	            if     (0           == ((BIT3|BIT2) & setVal)) strcpy(strLedStDwn, "OFF"  );
	            else if(BIT3        == ((BIT3|BIT2) & setVal)) strcpy(strLedStDwn, "BLINK");
	            else if((BIT3|BIT2) == ((BIT3|BIT2) & setVal)) strcpy(strLedStDwn, "ON"   );
	            else                                           strcpy(strLedStDwn, "INVALID");
	            printf("Set LED: address:%08X value:%08X DWN:%s ACT:%s\n", M_RRH_REG_CNT_CPSLED(linkno), setVal, strLedStDwn, strLedStAct);
	        }
#endif
	    }
	}

	return;
}

/* @} */

/* @} */


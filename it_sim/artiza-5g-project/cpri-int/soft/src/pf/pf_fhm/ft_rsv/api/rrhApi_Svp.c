/*!
 * @skip  $ld:$
 * @file  rrhApi_Svp.c
 * @brief API : 監視関連
 * @date  2013/11/05 FFCS)Xut Create.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/** @addtogroup RRH_API_SVP
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "f_sys_def.h"

#include "BPF_RU_IPCM.h"

#include "rrhApi_Com.h"
#include "rrhApi_Svp.h"

/********************************************************************************************************************/
/**
 *  @brief  API : 時刻設定通知
 *  @note   PF EVENT ID : 0xA0040007
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @param  datetime    : 時刻情報
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Svp-001-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_SetDateTime(
							INT	qid, INT	wtime,VOID *data_p,
							T_RRH_SYSTEM_TIME		datetime )
{
	T_API_SVP_SETTIME_REQ		apiReq;
	INT							errcd;
	INT							ret;
	
	memset(&apiReq,0,sizeof(apiReq));

	/* Set API parameters */
	apiReq.header.uiEventNo		= D_API_MSGID_SVP_DATETIME_SET_IND;	/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_F_PF;			/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;		/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;						/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;							/* Source T QID      */
	apiReq.header.uiLength		=  sizeof(apiReq);	

	memcpy((VOID*)&(apiReq.datetime), (VOID*)&datetime, sizeof(T_RRH_DATETIME));
	
	/* pf_rsvへ「API: 時刻設定通知」を送信する */
	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_F_PF,					/* Message QID             */
				(void *)&apiReq,					/* The message to transmit */
				sizeof(apiReq),						/* The size of message     */
				&errcd								/* Error code              */
				);

	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__,errcd);

		return E_API_RCD_NG;
	}
	
	return E_API_RCD_OK;
}
/********************************************************************************************************************/
/**
 *  @brief  API : カードリセット要求
 *  @note   PF EVENT ID : 0xA004000B
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @param  rstkind     : リセット種別
 *  @param  rstFact     : reset factor
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Svp-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_ResetCard(	
							INT	qid, INT	wtime,VOID *data_p,
							UINT			rstkind,
							UINT			rstFact )
{
	T_API_SVP_RESET_REQ      	apiReq;
	INT							errcd;
	INT							ret;
	
	memset(&apiReq,0,sizeof(apiReq));

	/* Edit API message */
	apiReq.header.uiEventNo		= D_API_MSGID_SVP_CARD_RESET_IND;	/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_F_PF;				/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;				/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		=  sizeof(apiReq);	
	apiReq.rstkind = rstkind;
	apiReq.rstFact = rstFact;
	
	/* pf_rsvへ「API: カードリセット要求」を送信する */
	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_F_PF,					/* Message QID             */
				(void *)&apiReq,					/* The message to transmit */
				sizeof(apiReq),						/* The size of message     */
				&errcd								/* Error code              */
				);

	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__,errcd);

		return E_API_RCD_NG;
	}
	
	return E_API_RCD_OK;
}
/********************************************************************************************************************/
/**
 *  @brief  API : ERR設定通知
 *  @note   PF EVENT ID : 0xA004000D
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @param  act_flag    : 制御フラグ, 0: クリア, 1: 設定
 *  @param  writeAdr    : write address
 *  @param  writeDat    : 設定値
 *  @param  chkalm_flg  : check alarm flag
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Svp-001-001-001
 *  @Bug_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_ErrSet(	
						INT	qid, INT	wtime,VOID *data_p,
						UINT			act_flag, 
						UINT            writeAdr,
						UINT			writeDat,
						USHORT			chkalm_flg)
{
	T_API_SVP_ERRSET_NTC		apiNtc;
	INT							errcd;
	INT							ret;

#ifdef OPT_RRH_IT1
	T_API_SVP_ERRSET_NTC	errset;
	printf("-------- Error Setting API --------\n");
	printf("act_flag = %d, writeDat = %04x\n", act_flag, writeDat);
	memset(&errset,0,sizeof(errset));
	errset.act_flag = act_flag;
	errset.writeDat = writeDat;
#endif
	
	memset(&apiNtc,0,sizeof(apiNtc));

	/* Edit API message */
	apiNtc.header.uiEventNo		= D_API_MSGID_SVP_ERRSET_IND;	/* Event  ID         */
	apiNtc.header.uiSignalkind	= 0;							/* Signal Kind       */
	apiNtc.header.uiDstPQueueID	= D_RRH_PROCQUE_F_PF;			/* Destination P QID */
	apiNtc.header.uiDstTQueueID	= 0;			/* Destination T QID */
	apiNtc.header.uiSrcPQueueID	= qid;							/* Source P QID      */
	apiNtc.header.uiSrcTQueueID	= 0;							/* Source T QID      */
	apiNtc.header.uiLength		=  sizeof(apiNtc);	
	apiNtc.act_flag = act_flag;
	apiNtc.writeAdr = writeAdr;
	apiNtc.writeDat = (D_SYS_ON == act_flag) ? writeDat : ~writeDat;
	apiNtc.chkalm_flg = (UINT)chkalm_flg;
	
	/* pf_rsvへ「ERR設定通知」を送信する */
	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_F_PF,					/* Message QID             */
				(void *)&apiNtc,					/* The message to transmit */
				sizeof(apiNtc),						/* The size of message     */
				&errcd								/* Error code              */
				);

	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__,errcd);

		return E_API_RCD_NG;
	}
		
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : カード状態Subscribe要求
 *  @note   PF EVENT ID : 0xA004000F
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Svp-005-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mpr_SubscribeCardState(
								INT	qid, INT	wtime,VOID *data_p)
{
	T_API_SVP_SUBSCARDSTA_REQ   apiReq;
	INT							errcd;
	INT							ret;

	if(rrhApi_com_checkRespQ(qid) != E_API_RCD_OK)
		return E_API_RCD_EPARAM;

	/* Initialization */
	memset(&apiReq,0,sizeof(apiReq));
	
	/* Set API parameters */
	apiReq.header.uiEventNo		= D_API_MSGID_SVP_STATE_SUB_REQ;	/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_F_PF;				/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;				/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		=  sizeof(apiReq);	

	/* pf_rsvへ「API: カード状態Subscribe要求」を送信する */
	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_F_PF,					/* Message QID             */
				(void *)&apiReq,					/* The message to transmit */
				sizeof(apiReq),						/* The size of message     */
				&errcd								/* Error code              */
				);

	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__,errcd);

		return E_API_RCD_NG;
	}
	
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : D_API_MSGID_SVP_SVREG_SET_IND
 *  @note   ERR設定はrrhApi_Svp_Mnt_ErrSet()を使用すること。USE bitなどのALM/ERR以外の用途
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wrAdr       : 書き込みアドレス(SV制御レジスタのアドレス)
 *  @param  clrBit      : クリアするbit(クリアしないならば0を指定)
 *  @param  setBit      : セットするbit(セットしないならば0を指定)
 *  @return 処理結果
 *  @date   2015/10/23 TDI)satou create
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_SvRegSet(INT qid, UINT wrAdr, UINT clrBit, UINT setBit) {
    T_API_SVP_SVSET_NTC apiNtc;
    INT errcd, ret;

    memset(&apiNtc, 0, sizeof apiNtc);
    apiNtc.header.uiEventNo     = D_API_MSGID_SVP_SVREG_SET_IND;
    apiNtc.header.uiSignalkind  = 0;
    apiNtc.header.uiDstPQueueID = D_RRH_PROCQUE_F_PF;
    apiNtc.header.uiDstTQueueID = 0;
    apiNtc.header.uiSrcPQueueID = qid;
    apiNtc.header.uiSrcTQueueID = 0;
    apiNtc.header.uiLength      = sizeof apiNtc;
    apiNtc.wrAdr                = wrAdr;
    apiNtc.setBit               = setBit;
    apiNtc.clrBit               = clrBit;

    ret = rrhApi_com_msgQSend(D_RRH_PROCQUE_F_PF, &apiNtc, sizeof apiNtc, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        rrhApi_com_log(__func__,errcd);
        return E_API_RCD_NG;
    }
    return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : カード状態Subscribe要求
 *  @note   PF EVENT ID : 0xA004000F
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Svp-005-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_UnsubscribeCardState(
								INT	qid, INT	wtime,VOID *data_p)
{
	T_API_SVP_UNSUBSCARDSTA_IND   apiReq;
	INT							errcd;
	INT							ret;

	/* Initialization */
	memset(&apiReq,0,sizeof(apiReq));
	
	/* Set API parameters */
	apiReq.header.uiEventNo		= D_API_MSGID_SVP_STATE_UNSUB_IND;	/* Event  ID         */
	apiReq.header.uiSignalkind	= 0;								/* Signal Kind       */
	apiReq.header.uiDstPQueueID	= D_RRH_PROCQUE_F_PF;				/* Destination P QID */
	apiReq.header.uiDstTQueueID	= 0;				/* Destination T QID */
	apiReq.header.uiSrcPQueueID	= qid;								/* Source P QID      */
	apiReq.header.uiSrcTQueueID	= 0;								/* Source T QID      */
	apiReq.header.uiLength		=  sizeof(apiReq);	

	/* pf_rsvへ「API: カード状態Subscribe要求」を送信する */
	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_F_PF,					/* Message QID             */
				(void *)&apiReq,					/* The message to transmit */
				sizeof(apiReq),						/* The size of message     */
				&errcd								/* Error code              */
				);

	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__,errcd);

		return E_API_RCD_NG;
	}
	
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : LED設定通知
 *  @note   PF EVENT ID : 0xA0040011
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @param  led1Sta     : LED1設定状態
 *  @param  led2Sta     : LED2設定状態
 *  @param  led3Sta     : LED3設定状態
 *  @param  actLedSta   : CPRI LINK LED (ACT LED)設定状態
 *  @param  dwnLedSta   : CPRI LINK LED (DWN LED)設定状態
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @date   2015/09/05 TDI)satou FHM用に見直し
 *  @FeatureID	PF_Svp-005-002-001
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_LedSet(	
							INT	qid, INT	wtime,VOID *data_p,
							UINT			led1Sta, 
							UINT			led2Sta,
							UINT            led3Sta,
							UINT            *actLedSta,
							UINT            *dwnLedSta)
{
	T_API_SVP_LEDSET_NTC		apiNtc;
	INT							errcd;
	INT							ret;
	UINT                        linkno;

#ifdef OPT_RRH_IT1
	printf("-------- API LED Setting request --------\n");
	printf("LED1 state is ");
	switch(led1Sta)
	{
	    case D_RRH_LED1STATE_OFF:
			printf("OFF; \n");
			break;
	    case D_RRH_LED1STATE_BLNK:
	        printf("Blink; \n");
	        break;
	    case D_RRH_LED1STATE_ON:
	        printf("ON; \n");
	        break;
		default:
			printf("ERROR; \n");
			break;
	}
	printf("LED2 state is ");
	switch(led2Sta)
	{
		case D_RRH_LED2STATE_OFF:
			printf("OFF; \n");
			break;
		case D_RRH_LED2STATE_BLNK:
			printf("Blink; \n");
			break;
		case D_RRH_LED2STATE_ON:
			printf("ON; \n");
			break;
		default:
			printf("ERROR; \n");
			break;
	}
	printf("LED3 state is ");
	switch(led3Sta)
	{
	    case D_RRH_LED3STATE_OFF:
	        printf("OFF; \n");
	        break;
	    case D_RRH_LED3STATE_BLNK:
	        printf("Blink; \n");
	        break;
	    case D_RRH_LED3STATE_ON:
	        printf("ON; \n");
	        break;
	    default:
	        printf("ERROR; \n");
	        break;
	}
	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++) {
	    printf("CPRI LINK LED (ACT) #%02d state is ", linkno);
	    switch (actLedSta[linkno]) {
	        case D_RRH_CPRILEDSTATE_ACT_OFF:
	            printf("OFF; \n");
	            break;
	        case D_RRH_CPRILEDSTATE_ACT_BLNK:
	            printf("Blink; \n");
	            break;
	        case D_RRH_CPRILEDSTATE_ACT_ON:
	            printf("ON; \n");
	            break;
	        default:
	            printf("ERROR; \n");
	            break;
	    }
	}
	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++) {
	    printf("CPRI LINK LED (DWN) #%02d state is ", linkno);
	    switch (dwnLedSta[linkno]) {
	        case D_RRH_CPRILEDSTATE_DWN_OFF:
	            printf("OFF; \n");
	            break;
	        case D_RRH_CPRILEDSTATE_DWN_BLNK:
	            printf("Blink; \n");
	            break;
	        case D_RRH_CPRILEDSTATE_DWN_ON:
	            printf("On; \n");
	            break;
	        default:
	            printf("ERROR; \n");
	            break;
	    }
	}
#endif
	
	memset(&apiNtc,0,sizeof(apiNtc));

	/* Edit API message */
	apiNtc.header.uiEventNo		= D_API_MSGID_SVP_LED_SET_IND;	/* Event  ID         */
	apiNtc.header.uiSignalkind	= 0;							/* Signal Kind       */
	apiNtc.header.uiDstPQueueID	= D_RRH_PROCQUE_F_PF;			/* Destination P QID */
	apiNtc.header.uiDstTQueueID	= 0;			/* Destination T QID */
	apiNtc.header.uiSrcPQueueID	= qid;							/* Source P QID      */
	apiNtc.header.uiSrcTQueueID	= 0;							/* Source T QID      */
	apiNtc.header.uiLength		=  sizeof(apiNtc);	
	apiNtc.led1Sta = led1Sta;
	apiNtc.led2Sta = led2Sta;
	apiNtc.led3Sta = led3Sta;
	
	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++) {
	    apiNtc.ledCpriAct[linkno] = actLedSta[linkno];
	    apiNtc.ledCpriDwn[linkno] = dwnLedSta[linkno];
	}

	/* pf_rsvへ「LED設定通知」を送信する */
	/* call MW func */
	ret = rrhApi_com_msgQSend(
				D_RRH_PROCQUE_F_PF,					/* Message QID             */
				(void *)&apiNtc,					/* The message to transmit */
				sizeof(apiNtc),						/* The size of message     */
				&errcd								/* Error code              */
				);

	/* checking parameter */
	if( ret != BPF_RU_IPCM_OK ) {
		rrhApi_com_log(__func__,errcd);

		return E_API_RCD_NG;
	}
		
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : 全配下REリセット(L1 inband reset)
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @return 処理結果
 *  @date   2015/10/19 TDI)satou ハソ-QA-036 create
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_AllReL1Reset(INT qid) {
#ifndef OPT_RRH_ZYNQ_REC	/*	擬似REの場合はL1 inband resetを送信しない	*/
    T_API_SVP_ALLRE_L1RESET_IND apiNtc;
    INT errcd, ret;

    memset(&apiNtc, 0, sizeof apiNtc);
    apiNtc.header.uiEventNo     = D_API_MSGID_SVP_ALL_RE_L1RESET_IND;
    apiNtc.header.uiSignalkind  = 0;
    apiNtc.header.uiDstPQueueID = D_RRH_PROCQUE_F_PF;
    apiNtc.header.uiDstTQueueID = 0;
    apiNtc.header.uiSrcPQueueID = qid;
    apiNtc.header.uiSrcTQueueID = 0;
    apiNtc.header.uiLength      = sizeof apiNtc;

    ret = rrhApi_com_msgQSend(D_RRH_PROCQUE_F_PF, (void*)&apiNtc, sizeof apiNtc, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        rrhApi_com_log(__func__, errcd);
        return E_API_RCD_NG;
    }
#endif
    return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : 配下REリセット(L1 inband reset)
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  link_num    : リセット対象配下REのCPRIリンク番号
 *  @return 処理結果
 *  @date   2015/10/19 TDI)satou ハソ-QA-036 create
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_ReL1Reset(INT qid, USHORT link_num) {
    T_API_SVP_RE_L1RESET_IND apiNtc;
    INT errcd, ret;

    memset(&apiNtc, 0, sizeof apiNtc);
    apiNtc.header.uiEventNo     = D_API_MSGID_SVP_RE_L1RESET_IND;
    apiNtc.header.uiSignalkind  = 0;
    apiNtc.header.uiDstPQueueID = D_RRH_PROCQUE_F_PF;
    apiNtc.header.uiDstTQueueID = 0;
    apiNtc.header.uiSrcPQueueID = qid;
    apiNtc.header.uiSrcTQueueID = 0;
    apiNtc.header.uiLength      = sizeof apiNtc;
    apiNtc.link_num             = link_num;

    ret = rrhApi_com_msgQSend(D_RRH_PROCQUE_F_PF, (void*)&apiNtc, sizeof apiNtc, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        rrhApi_com_log(__func__, errcd);
        return E_API_RCD_NG;
    }

    return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : 送波・停波設定
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  dis_ldisen  : 下りLTE用停波設定/解除
 *  @param  mix_lmixen  : 上りLTE用停波設定/解除
 *  @param  dis_wdisen  : 下りWcdma用停波設定/解除
 *  @param  mix_wmixen  : 上りWcdma用停波設定/解除
 *  @return 処理結果
 *  @date   2015/10/30 TDI)satou その他未実装-002 create
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_SetWave(INT qid, USHORT dis_ldisen, USHORT mix_lmixen, USHORT dis_wdisen, USHORT mix_wmixen) {
    T_API_SVP_SET_WAVE_IND apiNtc;
    INT errcd, ret;

    memset(&apiNtc, 0, sizeof apiNtc);
    apiNtc.header.uiEventNo     = D_API_MSGID_SVP_SET_WAVE_IND;
    apiNtc.header.uiSignalkind  = 0;
    apiNtc.header.uiDstPQueueID = D_RRH_PROCQUE_F_PF;
    apiNtc.header.uiDstTQueueID = 0;
    apiNtc.header.uiSrcPQueueID = qid;
    apiNtc.header.uiSrcTQueueID = 0;
    apiNtc.header.uiLength      = sizeof apiNtc;
    apiNtc.dis_ldisen           = dis_ldisen;
    apiNtc.mix_lmixen           = mix_lmixen;
    apiNtc.dis_wdisen           = dis_wdisen;
    apiNtc.mix_wmixen           = mix_wmixen;

    ret = rrhApi_com_msgQSend(D_RRH_PROCQUE_F_PF, (void*)&apiNtc, sizeof apiNtc, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        rrhApi_com_log(__func__, errcd);
        return E_API_RCD_NG;
    }
    return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : AxC設定
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  bruse       : BRUSE
 *  @param  axcbw       : AXCBW
 *  @param  ltebw       : LTEBW
 *  @param  lcnum       : LCNUM
 *  @param  wcnum       : WCNUM
 *  @param  wcuse       : WCUSE
 *  @return 処理結果
 *  @date   2015/11/03 TDI)satou その他未実装-003 create
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_SetAxC(INT qid, USHORT bruse, USHORT axcbw, USHORT ltebw, USHORT lcnum, USHORT wcnum, USHORT wcuse) {
    T_API_SVP_SET_AxC_IND apiNtc;
    INT errcd, ret;

    memset(&apiNtc, 0, sizeof apiNtc);
    apiNtc.header.uiEventNo     = D_API_MSGID_SVP_SET_AxC_IND;
    apiNtc.header.uiSignalkind  = 0;
    apiNtc.header.uiDstPQueueID = D_RRH_PROCQUE_F_PF;
    apiNtc.header.uiDstTQueueID = 0;
    apiNtc.header.uiSrcPQueueID = qid;
    apiNtc.header.uiSrcTQueueID = 0;
    apiNtc.header.uiLength      = sizeof apiNtc;
    apiNtc.bruse                = bruse;
    apiNtc.axcbw                = axcbw;
    apiNtc.ltebw                = ltebw;
    apiNtc.lcnum                = lcnum;
    apiNtc.wcnum                = wcnum;
    apiNtc.wcuse                = wcuse;

    ret = rrhApi_com_msgQSend(D_RRH_PROCQUE_F_PF, (void*)&apiNtc, sizeof apiNtc, &errcd);
    if (BPF_RU_IPCM_OK != ret) {
        rrhApi_com_log(__func__, errcd);
        return E_API_RCD_NG;
    }
    return E_API_RCD_OK;
}
/** @} */


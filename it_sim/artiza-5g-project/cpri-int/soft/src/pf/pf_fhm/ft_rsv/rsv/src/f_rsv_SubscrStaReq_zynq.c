/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_rsv_SubscrStaReq_zynq.c
 *  @brief  TRA card status change interruption handle function
 *  @date   2013/11/21FFCS)Xut create
 * 
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_RSV
* @{ */

#include "f_rsv_header.h"


/**
* @brief API subscribe State Request
* @note Edit and send API Card state change notice.\n
* @param uiSrcPQueueID [in] source process queue id
* @return None
* @date   2013/11/21FFCS)Xut create
* @date   2015/07/29 TDI)satou 「Read intial PA ON/OFF information from EEPROM」を削除
* @warning	N/A
* @FeatureID	PF-Svp-005-001-001
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*/

VOID f_rsv_rptCurCardSta(UINT uiSrcPQueueID)
{
	T_API_SVP_CARDSTACHG_NTC	msgNtc;

	/************************************************************************/
	/* 初期化																*/
	/************************************************************************/
	memset( &msgNtc, 0,sizeof( msgNtc ));

	/* Get INS state */
	if(( tra_sv_newsta[D_SYS_NUM0] & D_RRH_REG_CNT_SV1INF_INS) != 0)
	{
		/* Set state change type to INS */
		msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgType = D_API_SVP_STACHG_INS;
		/* Set change state to ON */
		msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgSta = D_SYS_ON;
		/* State change number increment */
		msgNtc.cardSta.chgNum++;	
	}

	/* Get ACT state */
	if(( tra_sv_newsta[D_SYS_NUM0] & D_RRH_REG_CNT_SV1INF_ACT) != 0)
	{
		/* Set state change type to ACT */
		msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgType = D_API_SVP_STACHG_ACT;
		/* Set change state to ON */
		msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgSta = D_SYS_ON;
		/* State change number increment */
		msgNtc.cardSta.chgNum++;	
	}
	else
	{
		/* Set state change type to ACT */
		msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgType = D_API_SVP_STACHG_ACT;
		/* Set change state to OFF */
		msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgSta = D_SYS_OFF;
		/* State change number increment */
		msgNtc.cardSta.chgNum++;	
	}

	if(( tra_sv_newsta[D_SYS_NUM0] & D_RRH_REG_CNT_SV1INF_ALM) != 0)
	{
        msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgType = D_API_SVP_STACHG_ALM;
        msgNtc.cardSta.almInfo.almFactor = tra_sv_newsta[D_SYS_NUM1];
        msgNtc.cardSta.chgNum++;
	}

	if(( tra_sv_newsta[D_SYS_NUM0] & D_RRH_REG_CNT_SV1INF_USE ) != 0 )
	{
		/* Set state change type to BLK */
		msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgType = D_API_SVP_STACHG_USE;
		/* Set change state to ON */
		msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgSta = D_SYS_ON;
		/* State change number increment */
		msgNtc.cardSta.chgNum++;	
	}

	/* Get ERR state */
	if(( tra_sv_newsta[D_SYS_NUM0] & D_RSV_INS_BIT ) == D_RSV_INS_BIT)
	{
		/*************************/
		/* 割込要因が"ERR"の場合 */
		/*************************/
        if ((0 != tra_sv_newsta[D_SYS_NUM16]) ||  (0 != tra_sv_newsta[D_SYS_NUM17])
        ||  (0 != tra_sv_newsta[D_SYS_NUM18]) ||  (0 != tra_sv_newsta[D_SYS_NUM19])
        ||  (0 != tra_sv_newsta[D_SYS_NUM20]) ||  (0 != tra_sv_newsta[D_SYS_NUM21])
        ||  (0 != tra_sv_newsta[D_SYS_NUM22]) ||  (0 != tra_sv_newsta[D_SYS_NUM23])
        ||  (0 != tra_sv_newsta[D_SYS_NUM24]) ||  (0 != tra_sv_newsta[D_SYS_NUM25])
        ||  (0 != tra_sv_newsta[D_SYS_NUM26]) ||  (0 != tra_sv_newsta[D_SYS_NUM27])
        ||  (0 != tra_sv_newsta[D_SYS_NUM28]) ||  (0 != tra_sv_newsta[D_SYS_NUM29])
        ||  (0 != tra_sv_newsta[D_SYS_NUM30]) ||  (0 != tra_sv_newsta[D_SYS_NUM31]))
		{
			/* Set state change type to ERR */
			msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgType = D_API_SVP_STACHG_ERR;
			/* Set change state to ON */
			msgNtc.cardSta.staChgInfo[msgNtc.cardSta.chgNum].chgSta = D_SYS_ON;
			/* State change number increment */
			msgNtc.cardSta.chgNum++;	
		}
	}

	/* Edit and Send API: Card State Change Notification to L3-RCT */
	msgNtc.header.uiSrcPQueueID = D_RRH_PROCQUE_F_PF;
	msgNtc.header.uiSrcTQueueID = D_SYS_THDQID_PF_RSV;
	msgNtc.header.uiEventNo		= D_API_MSGID_SVP_STATE_CHG_NTC;	/*!< Event  ID         */
	msgNtc.header.uiSignalkind	= 0;									/*!< Signal Kind       */
	msgNtc.header.uiDstPQueueID	= uiSrcPQueueID;
	msgNtc.header.uiDstTQueueID	= 0;									/*!< Destination T QID */
	msgNtc.header.uiLength		= sizeof(msgNtc);						/*!< Length            */

    /* Set carrier onoff */
    msgNtc.cardSta.carrOnOff = tra_sv_newsta[D_SYS_NUM1] & D_RRH_USE_BIT;

    /* Set software alarm */
    msgNtc.cardSta.almInfo.softAlm  = tra_sv_newsta[D_SYS_NUM12];
    msgNtc.cardSta.almInfo.softAlm2 = tra_sv_newsta[D_SYS_NUM13];

    /* Error Information setting */
    msgNtc.cardSta.errInfo.err[ 0] = tra_sv_newsta[D_SYS_NUM16];
    msgNtc.cardSta.errInfo.err[ 1] = tra_sv_newsta[D_SYS_NUM17];
    msgNtc.cardSta.errInfo.err[ 2] = tra_sv_newsta[D_SYS_NUM18];
    msgNtc.cardSta.errInfo.err[ 3] = tra_sv_newsta[D_SYS_NUM19];
    msgNtc.cardSta.errInfo.err[ 4] = tra_sv_newsta[D_SYS_NUM20];
    msgNtc.cardSta.errInfo.err[ 5] = tra_sv_newsta[D_SYS_NUM21];
    msgNtc.cardSta.errInfo.err[ 6] = tra_sv_newsta[D_SYS_NUM22];
    msgNtc.cardSta.errInfo.err[ 7] = tra_sv_newsta[D_SYS_NUM23];
    msgNtc.cardSta.errInfo.err[ 8] = tra_sv_newsta[D_SYS_NUM24];
    msgNtc.cardSta.errInfo.err[ 9] = tra_sv_newsta[D_SYS_NUM25];
    msgNtc.cardSta.errInfo.err[10] = tra_sv_newsta[D_SYS_NUM26];
    msgNtc.cardSta.errInfo.err[11] = tra_sv_newsta[D_SYS_NUM27];
    msgNtc.cardSta.errInfo.err[12] = tra_sv_newsta[D_SYS_NUM28];
    msgNtc.cardSta.errInfo.err[13] = tra_sv_newsta[D_SYS_NUM29];
    msgNtc.cardSta.errInfo.err[14] = tra_sv_newsta[D_SYS_NUM30];
    msgNtc.cardSta.errInfo.err[15] = tra_sv_newsta[D_SYS_NUM31];

	/* Call common function to send API message */
	f_com_sendMsgToAplFHM(uiSrcPQueueID, &msgNtc,sizeof(msgNtc),D_API_MSGID_SVP_STATE_CHG_NTC,D_SYS_THDQID_PF_RSV );

	return;
}

VOID f_rsv_SubscrStaReq(T_API_SVP_SUBSCARDSTA_REQ* msgReq_p)
{
	UINT 	num;
	INT		blank ;
	T_API_SVP_CARDSTACHG_NTC	msgNtc;

	/************************************************************************/
	/* 初期化																*/
	/************************************************************************/
	memset( &msgNtc, 0,sizeof( msgNtc ));
	/* 
	update f_rsvw_cardStaChgNtc_QID if the qid is not found
	*/
	blank = -1;
	for(num = 0; num < sizeof(f_rsvw_cardStaChgNtc_QID) / sizeof(UINT);num++)
	{
		if(f_rsvw_cardStaChgNtc_QID[ num] == msgReq_p->header.uiSrcPQueueID)
		{
			break;
		}
		else if((f_rsvw_cardStaChgNtc_QID[ num] == 0) && (blank < 0))
		{
			blank = num;
		}
	}

	if(num >= sizeof(f_rsvw_cardStaChgNtc_QID) / sizeof(UINT))
	{
		if(blank >= 0)
		f_rsvw_cardStaChgNtc_QID[ blank] = msgReq_p->header.uiSrcPQueueID;
		else 
		{
			f_com_assert(msgReq_p->header.uiSrcPQueueID,"too many subscribe card state");
			return;
		}
	}

#ifdef OPT_RRH_SIM
	f_rsv_dbg();
#endif
	/* Send API Card state change Notification with current state */
	f_rsv_rptCurCardSta(msgReq_p->header.uiSrcPQueueID);

	return;
}

/*! @} */

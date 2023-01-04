/*!
 * @skip  $ld:$
 * @file  f_cpr_anlz.c
 * @brief Analyze and dispatch message.
 * @date 2013/11/14 FFCS)niemsh create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
#include "f_rsv_header.h"			/* RE監視タスクヘッダファイル			*/
    
/*!
 * @addtogroup RRH_PF_CPR
 * @{
 */

/*!
 *  @brief  Analyze and dispatch message
 *  @note   This function is processed as follows.\n
 *          - Get event number from task IF header of message
 *          - Transform the event number to internal event number
 *          - Call the corresponding function in matrix
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2013/11/14 FFCS)niemsh create
 *  @date 2015/07/28 TDI)satou PORT状態変化監視機能を削除
 *  @date 2015/07/28 TDI)satou PA-SW 状態変化割込通知受信時の処理を削除
 *  @date 2015/07/28 TDI)satou PORT状態 subscribe要求受信時の処理を削除
 *  @date 2015/07/28 TDI)satou PORT制御機能を削除
 *  @date 2015/07/29 TDI)satou カード制御機能を削除
 *  @date 2015/09/02 TDI)satou システムパラメータ更新通知を削除
 *  @date 2015/10/19 TDI)satou D_API_MSGID_SVP_ALL_RE_L1RESET_IND, D_API_MSGID_SVP_RE_L1RESET_INDを追加
 *  @date 2015/10/23 TDI)satou D_API_MSGID_SVP_SVREG_SET_INDを追加
 *  @date 2015/10/30 TDI)satou その他未実装-002 D_API_MSGID_SVP_SET_WAVE_INDを追加
 *  @warning	N/A
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_rsv_anlz( VOID *pRcvMsg )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT	a_tsk_state	= D_RSV_TSK_STATE_OK;	/* タスク状態格納変数			*/
	UINT 	a_event_no;
	T_SYS_COMMON_THDIF *ptTskif;
	T_API_SVP_SVSET_NTC *msgSvSet;
	
	/************************************************************/
	/* Get event number from task IF header of message			*/
	/************************************************************/
	ptTskif = (T_SYS_COMMON_THDIF*)pRcvMsg;
	a_event_no = ptTskif->uiEventNo;

	/************************************************************/
	/* Transform the event number to internal event number		*/
	/************************************************************/
	/* 起動要因番号から処理の振分け */
	switch( a_event_no )
	{
		/**************************/
		/* 全タスク初期化完了通知受信時 */
		/**************************/
		case D_SYS_MSGID_ALLTSKININTC :

				/************************************************************/
				/* 全タスク起動完了待ち										*/
				/************************************************************/
				if ( f_rsvw_thrdstate == D_RSV_TSK_STATE_ALLTSK_STA_WAIT )
				{
					/* 初期化処理 */
					f_rsv_InitNtc();
				}
				else
				{
					a_tsk_state = D_RSV_TSK_STATE_NG;
				}
				break;
			
		/***************************************/
		/* TRAカード状態変化割込通知受信時 */
		/***************************************/
		case D_SYS_MSGID_TRA_IRQNTC :
			/* 自タスクのタスク状態が"通常運用状態"の場合 */
			if( f_rsvw_thrdstate <= D_RSV_TSK_STATE_USE )
			{
				/* TRAカード状態変化処理を実施 */
				f_rsv_TraStaChgNtc(( T_SYS_TRA_IRQNTC*)pRcvMsg );
			}
			else
			{
				a_tsk_state = D_RSV_TSK_STATE_NG;
			}
			break;

		case D_API_MSGID_SVP_STATE_SUB_REQ:
			/* 自タスクのタスク状態が"通常運用状態"の場合 */
			if( f_rsvw_thrdstate <= D_RSV_TSK_STATE_USE )
			{
				/* TRA Card state subscribe request */
				f_rsv_SubscrStaReq((T_API_SVP_SUBSCARDSTA_REQ*) pRcvMsg);
			}
			else
			{
				a_tsk_state = D_RSV_TSK_STATE_NG;
			}
			break;

		case D_API_MSGID_SVP_STATE_UNSUB_IND:
			/* 自タスクのタスク状態が"通常運用状態"の場合 */
			if( f_rsvw_thrdstate <= D_RSV_TSK_STATE_USE )
			{
				/* TRA Card state unsubscribe request */
				f_rsv_unSubscrStaReq((T_API_SVP_UNSUBSCARDSTA_IND*) pRcvMsg );
			}
			else
			{
				a_tsk_state = D_RSV_TSK_STATE_NG;
			}
			break;

		case D_API_MSGID_SVP_DATETIME_SET_IND:
			/* 自タスクのタスク状態が"通常運用状態"の場合 */
			if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
			{
				/* API Set datetime Request process */
				f_rsv_TimeSet((T_API_SVP_SETTIME_REQ*) pRcvMsg);
			}
			else
			{
				a_tsk_state = D_RSV_TSK_STATE_NG;
			}
			break;

		case D_API_MSGID_SVP_CARD_RESET_IND:
			/* 自タスクのタスク状態が"通常運用状態"の場合 */
			if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
			{
				/* API Card Reset Request process */
				f_rsv_CardReset( (T_API_SVP_RESET_REQ*) pRcvMsg);
			}
			else
			{
				a_tsk_state = D_RSV_TSK_STATE_NG;
			}
			break;

		case D_API_MSGID_SVP_LED_SET_IND:
			/* 自タスクのタスク状態が"通常運用状態"の場合 */
			if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
			{
				/* API LED Setting Request process */
				f_rsv_LedSet((T_API_SVP_LEDSET_NTC*) pRcvMsg);
			}
			else
			{
				a_tsk_state = D_RSV_TSK_STATE_NG;
			}
			break;

		case D_API_MSGID_SVP_ERRSET_IND:
			/* 自タスクのタスク状態が"通常運用状態"の場合 */
			if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
			{
				/* API ERR Setting Request process */
				f_rsv_ErrSet((T_API_SVP_ERRSET_NTC*) pRcvMsg);
			}
			else
			{
				a_tsk_state = D_RSV_TSK_STATE_NG;
			}
			break;

		case D_API_MSGID_SVP_SVREG_SET_IND:
            /* 自タスクのタスク状態が"通常運用状態"の場合 */
            if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
            {
                msgSvSet = (T_API_SVP_SVSET_NTC*)pRcvMsg;
                f_com_SVCtl_OtherProc(D_SYS_THDID_PF_RSV, msgSvSet->wrAdr, msgSvSet->clrBit, msgSvSet->setBit);
            }
            else
            {
                a_tsk_state = D_RSV_TSK_STATE_NG;
            }
            break;

		case D_API_MSGID_SVP_ALL_RE_L1RESET_IND:
            /* 自タスクのタスク状態が"通常運用状態"の場合 */
            if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
            {
                f_rsv_AllReL1Reset((T_API_SVP_ALLRE_L1RESET_IND*)pRcvMsg);
            }
            else
            {
                a_tsk_state = D_RSV_TSK_STATE_NG;
            }
		    break;

		case D_API_MSGID_SVP_RE_L1RESET_IND:
            /* 自タスクのタスク状態が"通常運用状態"の場合 */
            if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
            {
                f_rsv_ReL1Reset((T_API_SVP_RE_L1RESET_IND*)pRcvMsg);
            }
            else
            {
                a_tsk_state = D_RSV_TSK_STATE_NG;
            }
		    break;

		case D_API_MSGID_SVP_SET_WAVE_IND:
            /* 自タスクのタスク状態が"通常運用状態"の場合 */
            if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
            {
                f_rsv_SetWave((T_API_SVP_SET_WAVE_IND*)pRcvMsg);
            }
            else
            {
                a_tsk_state = D_RSV_TSK_STATE_NG;
            }
		    break;

		case D_API_MSGID_SVP_SET_AxC_IND:
            /* 自タスクのタスク状態が"通常運用状態"の場合 */
            if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
            {
                f_rsv_SetAxC((T_API_SVP_SET_AxC_IND*)pRcvMsg);
            }
            else
            {
                a_tsk_state = D_RSV_TSK_STATE_NG;
            }
            break;

		/******************************/
		/* タイムアウト発生通知受信時 */
		/******************************/
		case D_SYS_MSGID_TIMOUTNTC :
			/* 自タスクのタスク状態が"通常運用状態"の場合 */
			if( f_rsvw_thrdstate == D_RSV_TSK_STATE_USE )
			{
				/* Alarm cycling detect */
				f_rsv_AlmCyclChk();
			}
			else
			{
				a_tsk_state = D_RSV_TSK_STATE_NG;
			}
			break;

		default :
			/* その他の要求を受信した場合はアサート出力 */
			f_com_assert(a_event_no, 
					    "Exception Event Rcv" );
			
			break;
	}

	/* タスク状態と受信した起動要因が不適だった場合アサート出力 */
	if( a_tsk_state != D_RSV_TSK_STATE_OK )
	{
		f_com_assert(a_event_no,  "Task State NG" );
	}
	
	return;    
}

/* @} */



#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"
#include "BPF_COM_CMD.h"

char	t_dpda_rxoff_err[32] = "failed get send msg addr.";

/*!
 * @brief 関数機能概要:RX-OFF(M-Plane)設定
 * @note  関数処理内容.
 * @param  *msgP        [in]void            受信メッセージ
 * @return int
 * @date 2018/11/22 KCN)上本 Create
 */
int f_dpda_act_rxoff_request(VOID* msgP)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxoff_request called");

    T_RFP_MPL_RX_OFF_SETTING_REQ*	msgOFF = NULL;
    T_RFP_MPL_RX_OFF_SETTING_CFM*	sndMsg;
    unsigned int uiAntNum;
    unsigned int uiCarrier;
    unsigned int uiUnitType;
    int ret = D_DPDA_RTN_COMPLETE;
	int			errcd;

    msgOFF       = (T_RFP_MPL_RX_OFF_SETTING_REQ*)msgP;
    uiAntNum     = msgOFF->body.rx_array_car_t.ant ;
    uiCarrier    = msgOFF->body.rx_array_car_t.car ;
    uiUnitType   = f_dpdaw_reKind;



	/* duコマンド用 */
	BPF_COM_CMD_SET_RXENDP(uiCarrier,uiAntNum,D_RRH_OFF,0,0,0,D_DPDA_CELL_INVALID,0,0);
	BPF_COM_CMD_SET_RXCC(uiCarrier ,uiAntNum ,0, 0, 0,0 ,0);

	ret = f_dpda_act_rxCarrierOffSettingRequest(uiAntNum,
												uiCarrier,
												uiUnitType);

	if(ret != D_DPDA_RTN_COMPLETE)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxoff_request error=0x%08x", ret);
		/* 通信用共有メモリを取得する	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
											sizeof(T_RFP_MPL_RX_OFF_SETTING_CFM),	/* Size								*/
											(VOID **)&sndMsg,						/* sndMsg								*/
											&errcd );
		if( ret != D_SYS_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxoff_request buffer error=0x%08x", ret);
		}
		else
		{
			sndMsg->thdif.uiEventNo		= D_SYS_MSGID_RXOFF_CFM ;		/* RX-OFF(M-Plane)応答				*/
			sndMsg->thdif.uiDstPQueueID	= D_RRH_PROCQUE_PF;				/* 送信先Process Queue ID			*/
			sndMsg->thdif.uiDstTQueueID	= D_SYS_THDQID_PF_RFA;			/* 送信先Thread Queue ID			*/
			sndMsg->thdif.uiSrcPQueueID	= D_RRH_PROCQUE_PF;				/* 送信元Process Queue ID			*/
			sndMsg->thdif.uiSrcTQueueID	= D_SYS_THDQID_PF_CCA;			/* 送信元Thread Queue ID			*/
			sndMsg->requestId			= msgOFF->requestId ;			/* リクエストID						*/
			sndMsg->ant					= uiAntNum ;					/* アンテナ番号						*/
			sndMsg->car					= uiCarrier ;					/* キャリア番号						*/
			sndMsg->msgErr.msgErr.errorCode	= D_DPDA_EDIT_ERR ;				/* エラーコード						*/
			memcpy( &( sndMsg->msgErr.msgErr.errorDescription ) , t_dpda_rxoff_err , D_DPDA_UPCNF_ERR_MSG_SIZE ) ;
			
            ret = f_dpda_act_rxCarrierOffSettingRequest(uiAntNum,
                                                        uiCarrier,
                                                        uiUnitType);
			/* RX-OFF(M-Plane)応答を送信する */
			ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_RFA ,		/* 送信先Thread Queue ID			*/
											(void *)sndMsg );			/* 送信Message						*/

			if( ret != D_SYS_OK )
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxoff_request failed send message. %d", ret);
			}
		}
		return D_DPDA_RTN_NG;
	}

	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_RFP_MPL_RX_OFF_SETTING_CFM),	/* Size								*/
										(VOID **)&sndMsg,						/* sndMsg								*/
										&errcd );
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxoff_request buffer error=0x%08x", ret);
		return D_DPDA_RTN_NG;
	}

	sndMsg->thdif.uiEventNo		= D_SYS_MSGID_RXOFF_CFM ;		/* RX-OFF(M-Plane)応答				*/
	sndMsg->thdif.uiDstPQueueID	= D_RRH_PROCQUE_PF;				/* 送信先Process Queue ID			*/
	sndMsg->thdif.uiDstTQueueID	= D_SYS_THDQID_PF_RFA;			/* 送信先Thread Queue ID			*/
	sndMsg->thdif.uiSrcPQueueID	= D_RRH_PROCQUE_PF;				/* 送信元Process Queue ID			*/
	sndMsg->thdif.uiSrcTQueueID	= D_SYS_THDQID_PF_CCA;			/* 送信元Thread Queue ID			*/
	sndMsg->requestId			= msgOFF->requestId ;			/* リクエストID						*/
	sndMsg->ant					= uiAntNum ;					/* アンテナ番号						*/
	sndMsg->car					= uiCarrier ;					/* キャリア番号						*/
	sndMsg->msgErr.msgErr.errorCode	= D_DPDA_EDIT_OK ;				/* エラーコード						*/
	memset( &( sndMsg->msgErr.msgErr.errorDescription ) , 0 , D_DPDA_UPCNF_ERR_MSG_SIZE ) ;

	/* RX-OFF(M-Plane)応答を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_RFA ,		/* 送信先Thread Queue ID			*/
									(void *)sndMsg );			/* 送信Message						*/

	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxoff_request failed send message. %d", ret);
		return D_DPDA_RTN_NG;
	}
    return D_DPDA_RTN_COMPLETE;
}


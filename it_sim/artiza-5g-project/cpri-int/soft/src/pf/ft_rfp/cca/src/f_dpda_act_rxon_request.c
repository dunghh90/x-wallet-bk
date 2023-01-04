
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"
#include "BPF_COM_CMD.h"

char	t_dpda_rxon_err[32]			= "failed get send msg addr.";
char	t_dpda_rxon_err_input[32]	= "(SCS)Input parameter Error.";

/*!
 * @brief 関数機能概要:RX-ON(M-Plane)設定
 * @note  関数処理内容.
 * @param  *msgP        [in]void            受信メッセージ
 * @return int
 * @date 2018/11/22 KCN)上本 Create
 * @date 2019/02/22 KCN)吉木 3rd対応
 */
int f_dpda_act_rxon_request( void* msgP )
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxon_request called");

    T_RFP_MPL_RX_ON_SETTING_REQ*	msgON = NULL;	/* RX-ONメッセージ		*/
	T_RFP_MPL_RX_ON_SETTING_CFM*	sndMsg ;

    unsigned int uiAntNum;
    unsigned int uiCarrier;
    unsigned int uiAccellId;
    unsigned int uiCcId;
    unsigned int uiDuPortId;
    unsigned int uiCrCentFreq;
    unsigned int uiUnitType;
    unsigned int uiBw = E_DPDA_BW_10MHZ ;
    unsigned int uiScs;  
    int ret = D_DPDA_RTN_COMPLETE;
	int			errcd;
	uint64_t	bwhz ;
	unsigned int	uifft ;
	unsigned int	uita ;
    unsigned int uiAccellId_ex;
    unsigned int uiCcId_ex;
    unsigned int uiDuPortId_ex;


    msgON        = (T_RFP_MPL_RX_ON_SETTING_REQ*)msgP;
    uiAntNum     = msgON->body.rx_array_car_t.ant ;
    uiCarrier    = msgON->body.rx_array_car_t.car;
    uiCrCentFreq = msgON->body.rx_array_car_t.abs_freq_center;
    uiUnitType   = f_dpdaw_reKind;
    bwhz    = msgON->body.rx_array_car_t.ch_bandwidth;

    /********************************************************************/
    /* o-ran-uplane-confのHardcoded-eaxc-id対応により                    */
    /* ACELL-ID, CC-ID, DU-PORT-IDは起動時に設定を行うこととなったが    */
    /* DUコマンド側の変更を抑制するため、non PRACH側のみ使用する。      */
    /* なお、f_dpda_car_setRxParameterはコメント化しているため          */
    /* キャリアON時にレジスタ設定は行わない。                           */
    /********************************************************************/
    uiAccellId   = ( msgON->body.eaxc_id[0] & D_DPDA_CELL_MSK ) >> D_DPDA_CELL_CNT ;
    uiCcId       = ( msgON->body.eaxc_id[0] & D_DPDA_CCID_MSK ) >> D_DPDA_CCID_CNT ;
    uiDuPortId   = msgON->body.eaxc_id[0] & D_DPDA_POR_MSK ;
    uiAccellId_ex = ( msgON->body.eaxc_id[1] & D_DPDA_CELL_MSK ) >> D_DPDA_CELL_CNT ;
    uiCcId_ex     = ( msgON->body.eaxc_id[1] & D_DPDA_CCID_MSK ) >> D_DPDA_CCID_CNT ;
    uiDuPortId_ex = msgON->body.eaxc_id[1] & D_DPDA_POR_MSK ;


		/* f_dpda_act_rxCarrierOnSettingRequest 入力パラメータ変換処理 */
	if(bwhz == D_DPDA_BW_10MHZ)						/* BWが10MHzなら */
	{
		uiBw = E_DPDA_BW_10MHZ ;
	}
	else if(bwhz == D_DPDA_BW_20MHZ)				/* BWが20MHzなら */
	{
		uiBw = E_DPDA_BW_20MHZ ;
	}
	else if(bwhz == D_DPDA_BW_30MHZ)				/* BWが30MHzなら */
	{
		uiBw = E_DPDA_BW_30MHZ ;
	}
	else if(bwhz == D_DPDA_BW_40MHZ)				/* BWが40MHzなら */
	{
		uiBw = E_DPDA_BW_40MHZ ;
	}
	else if(bwhz == D_DPDA_BW_50MHZ)				/* BWが50MHzなら */
	{
		uiBw = E_DPDA_BW_50MHZ ;
	}
	else if(bwhz == D_DPDA_BW_60MHZ)				/* BWが60MHzなら */
	{
		uiBw = E_DPDA_BW_60MHZ ;
	}
	else if(bwhz == D_DPDA_BW_70MHZ)				/* BWが70MHzなら */
	{
		uiBw = E_DPDA_BW_70MHZ ;
	}
	else if(bwhz == D_DPDA_BW_80MHZ)				/* BWが80MHzなら */
	{
		uiBw = E_DPDA_BW_80MHZ ;
	}
	else if(bwhz == D_DPDA_BW_90MHZ)				/* BWが90MHzなら */
	{
		uiBw = E_DPDA_BW_90MHZ ;
	}
	else if(bwhz == D_DPDA_BW_100MHZ)				/* BWが100MHzなら */
	{
		uiBw = E_DPDA_BW_100MHZ ;
	}
	else if(bwhz == D_DPDA_BW_200MHZ)				/* BWが200MHzなら */
	{
		uiBw = E_DPDA_BW_200MHZ ;
	}
	else if(bwhz == D_DPDA_BW_400MHZ)				/* BWが400MHzなら */
	{
		uiBw = E_DPDA_BW_400MHZ ;
	}

    uiScs = msgON->body.rx_array_car_t.default_scs;
	switch( uiScs )
	{
		case E_UP_CAR_DEF_SCS_KHZ_5 :						/**< DEF SCS 5kHz			*/
		case E_UP_CAR_DEF_SCS_KHZ_15 :						/**< DEF SCS 15kHZ			*/
			/* SCSに15KHz未満を設定 */
			uiScs = E_DPDA_SCS_15KHZ ;
			break ;
		case E_UP_CAR_DEF_SCS_KHZ_30 :						/**< DEF SCS 30kHz			*/
			/* SCSに30KHz未満を設定 */
			uiScs = E_DPDA_SCS_30KHZ ;
			break ;
		case E_UP_CAR_DEF_SCS_KHZ_60 :						/**< DEF SCS 60kHz			*/
			/* SCSに60KHz未満を設定 */
			uiScs = E_DPDA_SCS_60KHZ ;
			break ;
		case E_UP_CAR_DEF_SCS_KHZ_120 :						/**< DEF SCS 120kHz			*/
			/* SCSに120KHzを設定 */
			uiScs = E_DPDA_SCS_120KHZ ;
			break ;
		case E_UP_CAR_DEF_SCS_KHZ_1_25 :					/**< DEF SCS 125kHz			*/
		case E_UP_CAR_DEF_SCS_KHZ_240 :						/**< DEF SCS 240kHz			*/
			/* SCSに240KHxを設定 */
			uiScs = E_DPDA_SCS_240KHZ ;
			break ;
		default :											/**< その他					*/
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxon_request (SCS)parameter Error. SCS=0x%08x", uiScs);
			ret = D_DPDA_RTN_INPUTERR ;
			break ;
	}

	/* fft-sampling-offsetの設定 */
	if(msgON->body.rx_array_car_t.ul_fft_sampling_offset != D_DPDA_FFT_SAMPL_MAX)
	{
		if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6)	/* Sub6	*/
		{
			/* レジスタ設定値計算	*/
			uifft = (msgON->body.rx_array_car_t.ul_fft_sampling_offset * D_DPDA_FFT_SAMPL_SUB6) ;
		}
		else	/* mmW	*/
		{
			/* レジスタ設定値計算	*/
			uifft = (msgON->body.rx_array_car_t.ul_fft_sampling_offset * D_DPDA_FFT_SAMPL_MMW) ;
		}
		/* レジスタ設定	*/
		ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DPDA_ULCPRMVMODE, &uifft );
	}

	/* ta-offsetの設定 */
	if(msgON->body.rx_array_car_t.n_ta_offset != D_DPDA_TA_OFFSET_MAX)
	{
		uita = ( msgON->body.rx_array_car_t.n_ta_offset / D_DPDA_TA_OFFSET_DEV ) & D_DPDA_TA_OFFSET_MASK ;
		ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DPDA_TCDLFRMAJ_ADDR_A , &uita ) ;
		ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_DPDA_TCULFRMAJ_ADDR , &uita ) ;
	}

	if (ret == D_DPDA_RTN_COMPLETE)
	{

		/* duコマンド用 */
		BPF_COM_CMD_SET_RXENDP(uiCarrier,uiAntNum,D_RRH_ON,uiAccellId,uiCcId,uiDuPortId,uiAccellId_ex,uiCcId_ex,uiDuPortId_ex);
		BPF_COM_CMD_SET_RXCC(uiCarrier,uiAntNum ,uiCrCentFreq, uiBw, uiScs, msgON->body.rx_array_car_t.ul_fft_sampling_offset ,msgON->body.rx_array_car_t.n_ta_offset );

		/* RX-ON設定 */
		ret = f_dpda_act_rxCarrierOnSettingRequest(	uiAntNum,
													uiCarrier,
													uiAccellId,
													uiCcId,
													uiDuPortId,
													uiCrCentFreq,
													uiUnitType,
													uiBw,
													uiScs);
	}

	if(ret != D_DPDA_RTN_COMPLETE)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxon_request error=0x%08x", ret);
		/* 通信用共有メモリを取得する	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
											sizeof(T_RFP_MPL_RX_ON_SETTING_CFM),	/* Size								*/
											(VOID **)&sndMsg,						/* sndMsg								*/
											&errcd );
		if( ret != D_SYS_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxon_request buffer error=0x%08x", ret);
		}
		else
		{
			sndMsg->thdif.uiEventNo		= D_SYS_MSGID_RXON_CFM ;		/* RX-ON(M-Plane)応答				*/
			sndMsg->thdif.uiDstPQueueID	= D_RRH_PROCQUE_PF;				/* 送信先Process Queue ID			*/
			sndMsg->thdif.uiDstTQueueID	= D_SYS_THDQID_PF_RFA;			/* 送信先Thread Queue ID			*/
			sndMsg->thdif.uiSrcPQueueID	= D_RRH_PROCQUE_PF;				/* 送信元Process Queue ID			*/
			sndMsg->thdif.uiSrcTQueueID	= D_SYS_THDQID_PF_CCA;			/* 送信元Thread Queue ID			*/
			sndMsg->requestId			= msgON->requestId ;			/* リクエストID						*/
			sndMsg->ant					= uiAntNum ;					/* アンテナ番号						*/
			sndMsg->car					= uiCarrier ;					/* キャリア番号						*/
			if (ret == D_DPDA_RTN_INPUTERR)
			{
				sndMsg->msgErr.msgErr.errorCode	= D_DPDA_EDIT_INPUT_ERR ;				/* エラーコード						*/
				memcpy( &( sndMsg->msgErr.msgErr.errorDescription ) , t_dpda_rxon_err_input , D_DPDA_UPCNF_ERR_MSG_SIZE ) ;
			}
			else
			{
				sndMsg->msgErr.msgErr.errorCode	= D_DPDA_EDIT_ERR ;				/* エラーコード						*/
				memcpy( &( sndMsg->msgErr.msgErr.errorDescription ) , t_dpda_rxon_err , D_DPDA_UPCNF_ERR_MSG_SIZE ) ;
			}

			/* RX-ON(M-Plane)応答を送信する */
			ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_RFA ,		/* 送信先Thread Queue ID			*/
											(void *)sndMsg );			/* 送信Message						*/

			if( ret != D_SYS_OK )
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxon_request failed send message. %d", ret);
			}
		}
		return D_DPDA_RTN_NG;
	}
	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_RFP_MPL_RX_ON_SETTING_CFM),	/* Size								*/
										(VOID **)&sndMsg,						/* sndMsg								*/
										&errcd );
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxon_request buffer error=0x%08x", ret);
		return D_DPDA_RTN_NG;
	}

	sndMsg->thdif.uiEventNo		= D_SYS_MSGID_RXON_CFM ;		/* RX-ON(M-Plane)応答				*/
	sndMsg->thdif.uiDstPQueueID	= D_RRH_PROCQUE_PF;				/* 送信先Process Queue ID			*/
	sndMsg->thdif.uiDstTQueueID	= D_SYS_THDQID_PF_RFA;			/* 送信先Thread Queue ID			*/
	sndMsg->thdif.uiSrcPQueueID	= D_RRH_PROCQUE_PF;				/* 送信元Process Queue ID			*/
	sndMsg->thdif.uiSrcTQueueID	= D_SYS_THDQID_PF_CCA;			/* 送信元Thread Queue ID			*/
	sndMsg->requestId			= msgON->requestId ;			/* リクエストID						*/
	sndMsg->ant					= uiAntNum ;					/* アンテナ番号						*/
	sndMsg->car					= uiCarrier ;					/* キャリア番号						*/
	sndMsg->msgErr.msgErr.errorCode	= D_DPDA_EDIT_OK ;				/* エラーコード						*/
	memset( &( sndMsg->msgErr.msgErr.errorDescription ) , 0 , D_DPDA_UPCNF_ERR_MSG_SIZE ) ;

	/* RX-ON(M-Plane)応答を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_RFA ,		/* 送信先Thread Queue ID			*/
									(void *)sndMsg );			/* 送信Message						*/

	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxon_request failed send message. %d", ret);
		return D_DPDA_RTN_NG;
	}

    return D_DPDA_RTN_COMPLETE;
}

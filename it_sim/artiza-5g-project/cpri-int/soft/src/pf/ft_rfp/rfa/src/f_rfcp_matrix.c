/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Table definitions of state change matrix 
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also ...
 * @date 2018/08/14 KCN)takagi Create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

#include	"f_rfcp_inc.h"

/** @addtogroup RRH_PF_RFP
 *  @{
 */

/* RFCP State Mng */
const T_RFCP_CALLSETRELL_MTXTBL f_rfcpr_callSetRell_mtxtbl[D_RFCP_STATENUM][D_RFCP_EVENTNUM] __attribute__ ((aligned (16))) =
{
	{
		/*	init	*/
		{f_rfcp_init						},			/**< スレッド起動						*/
		{f_rfcp_abort						},			/**< System Parameter Update Request	*/
		{f_rfcp_abort						},			/**< Carrier ON Request					*/
		{f_rfcp_abort						},			/**< Carrier OFF Request				*/
		{f_rfcp_abort						},			/**< The request of TxCal RxCal in use	*/
		{f_rfcp_abort						},			/**< The request of TxCal RxCal Training*/
		{f_rfcp_abort						},			/**< TX-ON(M-Plane) Comfirm				*/
		{f_rfcp_abort						},			/**< RX-ON(M-Plane) Comfirm				*/
		{f_rfcp_abort						},			/**< TX-OFF(M-Plane) Comfirm			*/
		{f_rfcp_abort						},			/**< RX-OFF(M-Plane) Comfirm			*/
		{f_rfcp_abort						},			/**< Processing Element GET Request		*/
		{f_rfcp_abort						},			/**< u-plane conf. status GET Request	*/
		{f_rfcp_abort						},			/**< u-plane conf. TX-EDIT Request		*/
		{f_rfcp_abort						}			/**< u-plane conf. RX-EDIT Request		*/
	},
	{
		/*	運用状態	*/
		{f_rfcp_abort						},			/**< スレッド起動						*/
		{f_rfcp_act_systemParamUpdate		},			/**< System Parameter Update Request	*/
		{f_rfcp_act_carrierOn				},			/**< Carrier ON Request					*/
		{f_rfcp_act_carrierOff				},			/**< Carrier OFF Request				*/
		{f_rfcp_txrxcal_inuse				},			/**< The request of TxCal RxCal in use	*/
		{f_rfcp_txrxcal_training			},			/**< The request of TxCal RxCal Training*/
		{f_rfcp_txon_comfirm				},			/**< TX-ON(M-Plane) Comfirm				*/
		{f_rfcp_rxon_comfirm				},			/**< RX-ON(M-Plane) Comfirm				*/
		{f_rfcp_txoff_comfirm				},			/**< TX-OFF(M-Plane) Comfirm			*/
		{f_rfcp_rxoff_comfirm				},			/**< RX-OFF(M-Plane) Comfirm			*/
		{f_rfcp_prccelmt_EDIT				},			/**< Processing Element GET Request		*/
		{f_rfcp_upcnf_GET					},			/**< u-plane conf. status GET Request	*/
		{f_rfcp_upcnf_TXEDIT				},			/**< u-plane conf. TX-EDIT Request		*/
		{f_rfcp_upcnf_RXEDIT				}			/**< u-plane conf. RX-EDIT Request		*/
	}
};

/** @} */
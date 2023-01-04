/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Table definitions of state change matrix 
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also ...
 * @date 2018/08/13 KCN)takagi Create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

#include	"f_dpda_inc.h"

/** @addtogroup RRH_PF_RFP
 *  @{
 */

/* DPDA State Mng(sub6)用 */
const T_DPDA_CALLSETRELL_MTXTBL f_dpdar_callSetRell_mtxtbl_sub6[D_DPDA_STATENUM][E_DPDA_EVENTNUM] __attribute__ ((aligned (16))) =
{
	{
		/*	init	*/
		{f_dpda_init							},			/**< タスク起動								*/
		{f_dpda_abort							},			/**< System Parameter Set Request			*/
		{f_dpda_abort							},			/**< Carrier ON Setting Request				*/
		{f_dpda_abort							},			/**< Carrier OFF Setting Request			*/
		{f_dpda_abort							},			/**< The request of TxCal RxCal in use		*/
		{f_dpda_abort							},			/**< The request of TxCal RxCal Training	*/
		{f_dpda_abort							},			/**< 周期監視報告							*/
		{f_dpda_abort							},			/**< TX-ON(M-Plane) Request					*/
		{f_dpda_abort							},			/**< RX-ON(M-Plane) Request					*/
		{f_dpda_abort							},			/**< TX-OFF(M-Plane) Request				*/
		{f_dpda_abort							},			/**< RX-OFF(M-Plane) Request				*/
		{f_dpda_abort							},			/**< TDDEN_CHG_NTC							*/
		{f_dpda_abort							},			/**< MNT_POWER_ADJUST_REQ					*/
		{f_dpda_abort							},			/**< E_DPDA_EVE_BEAMID_SET_REQ				*/
		{f_dpda_abort							},			/**< TX-SLEEP(M-Plane) Request				*/
		{f_dpda_abort							}			/**< RX-SLEEP(M-Plane) Request				*/
	},
	{
		/*	運用状態	*/
		{f_dpda_abort							},			/**< タスク起動								*/
		{f_dpda_act_sysParaSetRequest			},			/**< System Parameter Set Request			*/
		{f_dpda_act_carrierOnSettingRequest		},			/**< Carrier ON Setting Request				*/
		{f_dpda_act_carrierOffSettingRequest	},			/**< Carrier OFF Setting Request			*/
		{f_dpda_act_txrxCal_inUse				},			/**< The request of TxCal RxCal in use		*/
		{f_dpda_act_txrxCalTraining				},			/**< The request of TxCal RxCal Training	*/
		{f_dpda_act_powReportPeriodic			},			/**< 周期監視報告							*/
		{f_dpda_act_txon_request				},			/**< TX-ON(M-Plane) Request					*/
		{f_dpda_act_txoff_request				},			/**< RX-ON(M-Plane) Request					*/
		{f_dpda_act_rxon_request				},			/**< TX-OFF(M-Plane) Request				*/
		{f_dpda_act_rxoff_request				},			/**< RX-OFF(M-Plane) Request				*/
		{f_dpda_act_tdden_ntc					},			/**< TDDEN_CHG_NTC							*/
		{f_dpda_act_mnt_txgainRequest			},			/**< MNT_POWER_ADJUST_REQ					*/
		{f_dpda_act_mnt_beamidSetReq_sub6		},			/**< E_DPDA_EVE_BEAMID_SET_REQ				*/
		{f_dpda_act_txsleep_request				},			/**< TX-SLEEP(M-Plane) Request				*/
		{f_dpda_act_rxsleep_request				}			/**< RX-SLEEP(M-Plane) Request				*/
	}
};

/* DPDA State Mng(mmW)用 */
const T_DPDA_CALLSETRELL_MTXTBL f_dpdar_callSetRell_mtxtbl_mmW[D_DPDA_STATENUM][E_DPDA_EVENTNUM] __attribute__ ((aligned (16))) =
{
	{
		/*	init	*/
		{f_dpda_init								},			/**< タスク起動								*/
		{f_dpda_abort								},			/**< System Parameter Set Request			*/
		{f_dpda_abort								},			/**< Carrier ON Setting Request				*/
		{f_dpda_abort								},			/**< Carrier OFF Setting Request			*/
		{f_dpda_abort								},			/**< The request of TxCal RxCal in use		*/
		{f_dpda_abort								},			/**< The request of TxCal RxCal Training	*/
		{f_dpda_abort								},			/**< 周期監視報告							*/
		{f_dpda_abort								},			/**< TX-ON(M-Plane) Request					*/
		{f_dpda_abort								},			/**< RX-ON(M-Plane) Request					*/
		{f_dpda_abort								},			/**< TX-OFF(M-Plane) Request				*/
		{f_dpda_abort								},			/**< RX-OFF(M-Plane) Request				*/
		{f_dpda_abort								},			/**< TDDEN_CHG_NTC							*/
		{f_dpda_abort								},			/**< MNT_POWER_ADJUST_REQ					*/
		{f_dpda_abort								},			/**< E_DPDA_EVE_BEAMID_SET_REQ				*/
		{f_dpda_abort								},			/**< TX-SLEEP(M-Plane) Request				*/
		{f_dpda_abort								}			/**< RX-SLEEP(M-Plane) Request				*/
	},
	{
		/*	運用状態	*/
		{f_dpda_abort								},			/**< タスク起動								*/
		{f_dpda_act_sysParaSetRequest_mmW			},			/**< System Parameter Set Request			*/
		{f_dpda_act_carrierOnSettingRequest			},			/**< Carrier ON Setting Request				*/
		{f_dpda_act_carrierOffSettingRequest		},			/**< Carrier OFF Setting Request			*/
		{f_dpda_abort								},			/**< The request of TxCal RxCal in use		*/
		{f_dpda_abort								},			/**< The request of TxCal RxCal Training	*/
		{f_dpda_act_powReportPeriodic_mmW			},			/**< 周期監視報告							*/
		{f_dpda_act_txon_request					},			/**< TX-ON(M-Plane) Request					*/
		{f_dpda_act_txoff_request					},			/**< RX-ON(M-Plane) Request					*/
		{f_dpda_act_rxon_request					},			/**< TX-OFF(M-Plane) Request				*/
		{f_dpda_act_rxoff_request					},			/**< RX-OFF(M-Plane) Request				*/
		{f_dpda_act_tdden_ntc						},			/**< TDDEN_CHG_NTC							*/
		{f_dpda_act_mnt_txgainRequest				},			/**< MNT_POWER_ADJUST_REQ					*/
		{f_dpda_act_mnt_beamidSetReq_mmW			},			/**< E_DPDA_EVE_BEAMID_SET_REQ				*/
		{f_dpda_act_txsleep_request					},			/**< TX-SLEEP(M-Plane) Request				*/
		{f_dpda_act_rxsleep_request					}			/**< RX-SLEEP(M-Plane) Request				*/
	}
};


/** @} */


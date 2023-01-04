/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_ptp_msi_function.c
 *  @brief  pf_ptp PTP-Status Reoprting-Period Timer T.O
 *  @date   2019/02/15 KCN)Kamimoto create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup 5GDU_PF_EQS
* @{ */
#include <ctype.h>
#include "f_ptp_inc.h"
#include "zl303xx_PtpApi.h"
#include "BPF_COM_CMD.h"

/* ptp関数参照 */
extern Uint32T zl303xx_GetExampleInterface(void);

/* 内部テーブル参照 */
/*****************   IMPORTED GLOBAL VARIABLES   ******************************/
extern exampleAppS zlExampleApp;
extern zl303xx_ClockStreamS *Zl303xx_PtpStreamArray[ZL303XX_PTP_NUM_STREAMS_MAX];
extern int accepted_clock_classes;
extern int keep_announce_clockclass;

/* M-plane Edit変化確認用グローバル変数 */
extern	unsigned char	*f_ptp_d_num;
extern	unsigned char	*f_ptp_m_macaddr;
extern	unsigned char	*f_ptp_accepted_clockclass;
extern UINT f_syncE_ssm_timer_e;
extern UINT f_syncE_acceptance_ssm[];
extern pthread_mutex_t f_syncE_ssm_mutex; 

/*****************   DEFINES     **********************************************/

/* Formats a PTP Clock Identity into a string for printing. */
#define ZL303XX_PTP_CLK_TO_STRING(clkId, string)                              \
            OS_SPRINTF(string, "0x%02hx%02hx%02hx%02hx%02hx%02hx%02hx%02hx", \
                    clkId[0], clkId[1], clkId[2], clkId[3],                    \
                    clkId[4], clkId[5], clkId[6], clkId[7])




/**
* @brief PTP-Status Reporting Period Timer Time Out function
* @note PTP-Status Change Notification factor MSI Send..\n
* @param msg_P [in] Message pointer
* @return None
* @date 2019/02/14 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID get_mplane_param( unsigned int reqid )
{
	unsigned int state;								/* SV読出し結果置き場 */	
	T_MPSW_MSI_O_RAN_SYNC_GET_CFM		sndMsg;		/* 送信メッセージ(o-ran-sync GET要求(OK応答)) */

	INT									port = (INT)zl303xx_GetExampleInterface();
	zl303xx_ClockStreamS				*pPtpStream = Zl303xx_PtpStreamArray[port] ;

	char clkIdStr[]  = PTP_IDENTITY ;
	char clkIdStr1[] = PTP_IDENTITY ;
	char clkIdStr2[] = PTP_IDENTITY ;


	/* 初期化 */
	memset(&sndMsg, D_RRH_ZERO, sizeof(sndMsg));

	/* SVレジスタから状態を取得 */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SV16INF, &state);

	/* Sync Locked Status(B30-B28) */	
	if ( (state & D_DU_SVCTL_SYNC_STATE_HOLDOVER) == D_DU_SVCTL_SYNC_STATE_HOLDOVER)
	{
		sndMsg.sync_t.sync_status_sync_t.sync_state = E_SYNC_STATE_HOLDOVER;
	}
	else if ((state & D_DU_SVCTL_SYNC_STATE_LOCKED) == D_DU_SVCTL_SYNC_STATE_LOCKED)
	{
		sndMsg.sync_t.sync_status_sync_t.sync_state = E_SYNC_STATE_LOCKED;
	}
	else if ((state & D_DU_SVCTL_SYNC_STATE_FREERUN) == D_DU_SVCTL_SYNC_STATE_FREERUN)
	{
		sndMsg.sync_t.sync_status_sync_t.sync_state = E_SYNC_STATE_FREERUN;
	}
	else
	{
		/* ないはずですが */
		ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "sync_state value =%d.", state,0,0,0,0,0);
		sndMsg.sync_t.sync_status_sync_t.sync_state = D_DU_SVCTL_SYNC_STATE_FREERUN;
	}

	/* PTP Locked Status(B25) */
	if ((state & D_DU_SVCTL_PTP_STATE_LOCKED) == D_DU_SVCTL_PTP_STATE_LOCKED)
	{
		sndMsg.sync_t.ptp_status_t.lock_state = E_PTP_STATE_LOCKED;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.state = E_PTP_STATE_PARENT;
	}
	else{
		sndMsg.sync_t.ptp_status_t.lock_state = E_PTP_STATE_UNLOCKED;
		if(keep_announce_clockclass == 0)
		{
			sndMsg.sync_t.ptp_status_t.ptp_source_t.state = E_PTP_STATE_DISABLE;
		}
		else if(keep_announce_clockclass <= accepted_clock_classes)
		{
			sndMsg.sync_t.ptp_status_t.ptp_source_t.state = E_PTP_STATE_OK;
		}
		else{
			sndMsg.sync_t.ptp_status_t.ptp_source_t.state = E_PTP_STATE_NOK;
		}
	}
	
	if(sndMsg.sync_t.ptp_status_t.ptp_source_t.state == E_PTP_STATE_DISABLE)
	{
		sndMsg.sync_t.ptp_status_t.clock_class    = zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass;		/* uint8 */
		snprintf(sndMsg.sync_t.ptp_status_t.clock_identity, PTP_MPLANE_ID_SIZE, "%s", clkIdStr1 ) ;

		/* ptp_config部 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.local_port_number          = port;				/* uint16 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.twostep_flag               = D_RRH_ZERO;		/* bool型 Disable */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.leap61                     = D_RRH_ZERO;		/* bool型 Disable */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.leap59                     = D_RRH_ZERO;		/* bool型 Disable */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.current_utc_offset_valid   = D_RRH_ZERO;		/* bool型 Disable */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.ptp_timescale              = D_RRH_ZERO;		/* bool型 Disable */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.time_traceable             = D_RRH_ZERO;		/* bool型 Disable */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.frequency_traceable        = D_RRH_ZERO;		/* bool型 Disable */
		snprintf(sndMsg.sync_t.ptp_status_t.ptp_source_t.source_clock_identity, PTP_MPLANE_ID_SIZE, "%s", clkIdStr1 ) ;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.source_port_number         = D_RRH_ZERO;		/* uint16 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.current_utc_offset         = D_RRH_ZERO;		/* int16 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.priority1                  = D_PTP_PRIORITY127;	/* int8 最悪値を設定 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.clock_class                = D_PTP_CLKCLASS127;	/* int8 最悪値を設定 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.clock_accuracy             = 0x30;				/* int8 最悪値を設定 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.offset_scaled_log_variance = 0xFFFF;			/* uint16 最悪値を設定 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.priority2                  = D_PTP_PRIORITY127;	/* int8 最悪値を設定 */
		snprintf(sndMsg.sync_t.ptp_status_t.ptp_source_t.grandmaster_clock_identity, PTP_MPLANE_ID_SIZE, "%s", clkIdStr ) ;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.steps_removed              = D_RRH_ZERO;		/* uint16 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.time_source                = 0xA0;				/* uint8 */		
		
	}else
	{

		sndMsg.sync_t.ptp_status_t.clock_class                             = zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockQuality.clockClass;

		ZL303XX_PTP_CLK_TO_STRING(zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.clockIdentity, clkIdStr1);
		snprintf(sndMsg.sync_t.ptp_status_t.clock_identity, PTP_MPLANE_ID_SIZE, "%s", clkIdStr1 ) ;

		/* ptp_config部 */
		sndMsg.sync_t.ptp_status_t.ptp_source_t.local_port_number          = port;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.twostep_flag               = exampleGetTwoStepFlag();
		sndMsg.sync_t.ptp_status_t.ptp_source_t.leap61                     = pPtpStream->farEndTimePropertiesDS.leap61;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.leap59                     = pPtpStream->farEndTimePropertiesDS.leap59;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.current_utc_offset_valid   = pPtpStream->farEndTimePropertiesDS.currentUtcOffsetValid;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.ptp_timescale              = pPtpStream->farEndTimePropertiesDS.ptpTimescale;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.time_traceable             = pPtpStream->farEndTimePropertiesDS.timeTraceable;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.frequency_traceable        = pPtpStream->farEndTimePropertiesDS.frequencyTraceable;
		ZL303XX_PTP_CLK_TO_STRING( pPtpStream->farEndParentDS.u.v2.parentPortIdentity.clockIdentity, clkIdStr2 );
		snprintf(sndMsg.sync_t.ptp_status_t.ptp_source_t.source_clock_identity, PTP_MPLANE_ID_SIZE, "%s", clkIdStr2 ) ;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.source_port_number         = pPtpStream->farEndParentDS.u.v2.parentPortIdentity.portNumber;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.current_utc_offset         = pPtpStream->farEndTimePropertiesDS.currentUtcOffset;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.priority1                  = pPtpStream->farEndParentDS.u.v2.grandmasterPriority1;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.clock_class                = pPtpStream->farEndParentDS.u.v2.grandmasterClockQuality.clockClass;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.clock_accuracy             = pPtpStream->farEndParentDS.u.v2.grandmasterClockQuality.clockAccuracy;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.offset_scaled_log_variance = pPtpStream->farEndParentDS.u.v2.grandmasterClockQuality.offsetScaledLogVariance;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.priority2                  = pPtpStream->farEndParentDS.u.v2.grandmasterPriority2;

		ZL303XX_PTP_CLK_TO_STRING( pPtpStream->farEndParentDS.u.v2.grandmasterIdentity, clkIdStr );
		snprintf(sndMsg.sync_t.ptp_status_t.ptp_source_t.grandmaster_clock_identity, PTP_MPLANE_ID_SIZE, "%s", clkIdStr ) ;

		sndMsg.sync_t.ptp_status_t.ptp_source_t.steps_removed              = pPtpStream->farEndCurrentDS.stepsRemoved;
		sndMsg.sync_t.ptp_status_t.ptp_source_t.time_source                = pPtpStream->farEndTimePropertiesDS.timeSource;
	}

	/* SyncE Status(B19-B16) */	
	if ((state & D_DU_SVCTL_SYNCE_STATE_LOCKED) == D_DU_SVCTL_SYNCE_STATE_LOCKED)
	{
		sndMsg.sync_t.synce_status_t.lockstate = E_SYNCE_LOCK_STATE_LOCKED;
		
		sndMsg.sync_t.synce_status_t.synce_source_t.local_port_number = port;
		sndMsg.sync_t.synce_status_t.synce_source_t.quality_level = f_ptp_ssmval;
		
		sndMsg.sync_t.synce_status_t.synce_source_t.state = E_SYNCE_STATE_PARENT;
	}else
	{
		sndMsg.sync_t.synce_status_t.lockstate = E_SYNCE_LOCK_STATE_UNLOCKED;
		
		sndMsg.sync_t.synce_status_t.synce_source_t.local_port_number = port;
		sndMsg.sync_t.synce_status_t.synce_source_t.quality_level = f_ptp_ssmval;
		
		if ((state & D_DU_SVCTL_SYNCE_STATE_NOK) == D_DU_SVCTL_SYNCE_STATE_NOK)
		{
			sndMsg.sync_t.synce_status_t.synce_source_t.state = E_SYNCE_STATE_NOK;
		}
		else
		{
			sndMsg.sync_t.synce_status_t.synce_source_t.state = E_SYNCE_STATE_DISABLE;
		}
	}

	/* CFM応答 */
	f_com_MSIsend(D_MSG_MSI_O_RAN_SYNC_GET_CFM, reqid, sizeof(T_MPSW_MSI_O_RAN_SYNC_GET_CFM), &sndMsg, D_RRH_PROCQUE_PTP, D_SYS_THDQID_PTP_MAIN);
	ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "get_mplane_param() f_com_MSIsend finish. reqid=%d.", reqid,0,0,0,0,0);
	
	return;
}
/* @} */


/**
* @brief PTP-Status Reporting Period Timer Time Out function
* @note PTP-Status Change Notification factor MSI Send..\n
* @param msg_P [in] Message pointer
* @return None
* @date 2019/02/14 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID edit_mplane_param( VOID * msgP , unsigned int reqid )
{
	/* 変数宣言 */
	Uint32T											count,count2;
	UINT											reg_data1   = 0;
	UINT											reg_data2   = 0;
	UINT											reg_data3   = 0;
	UINT											reg_ini_addr[2];
	UINT											reg_set_addr[2];
	UINT											port;
	UCHAR											multi_mac_address[2][6] = { {0x01, 0x1B, 0x19, 0x00, 0x00, 0x00}, {0x01, 0x80, 0xC2, 0x00, 0x00, 0x0E} };
	zlStatusE										status = ZL303XX_OK;
	Sint32T											delayNs ;
	short											delay_asymmetry ;
	T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ *					rcvMsg ;
	unsigned char									domain_num ;
	unsigned char									m_addr ;
	unsigned char									accept_class ;
	unsigned char									ptp_report_time ;
	unsigned char									synce_report_time ;
	unsigned int									synce_ssm_timer ;
	unsigned int									time_data ;
	INT												ret = D_SYS_OK ;
	INT												errcd ;
	zl303xx_PtpStreamHandleT						streamHandle ;
	examplePtpStreamCreateS *						pStream = &zlExampleApp.stream[0].ptp ;
	examplePtpSocketTblS *							entry = NULL ;
	UINT											ssm_val;

	/* プロセス */
	/* 入力パラメータの整理 */
	rcvMsg = (T_MPSW_MSI_O_RAN_SYNC_EDIT_REQ *)msgP ;
	domain_num			= (unsigned char)rcvMsg->ptp_config_t.domain_number;
	m_addr				= (unsigned char)rcvMsg->ptp_config_t.multicast_mac_address;
	accept_class		= (unsigned char)rcvMsg->ptp_config_t.clock_classes;
	ptp_report_time		= (unsigned char)rcvMsg->ptp_config_t.ptp_reporting_period ;
	synce_report_time	= (unsigned char)rcvMsg->ptp_config_t.syncE_reporting_period ;
	synce_ssm_timer		= (unsigned int)rcvMsg->ptp_config_t.syncE_ssm_timeout ;
	delay_asymmetry		= rcvMsg->ptp_config_t.delay_asymmetry ;

	ZL303XX_TRACE_ALWAYS("edit_mplane_param interface domain_num : %x  m_addr : %x  accept_class : %x", domain_num , m_addr , accept_class , 0,0,0);
	ZL303XX_TRACE_ALWAYS("edit_mplane_param interface ptp_report_time : %x  synce_report_time : %x  delay_asymmetry : %x", ptp_report_time , synce_report_time , delay_asymmetry , 0,0,0);

	/* 変化有無チェック(変化ありの場合、運用パラメータ変更＋EEPROM書替) */
	if ( rcvMsg->ptp_config_t.domain_number != PTP_EDIT_DATA_UNDEF_16 )
	{
		if (domain_num != *f_ptp_d_num) {

			ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "domain_number changes. value=%d", domain_num, 0,0,0,0,0);

			/* EEPROM書換え */
			BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_DOMAIN_NO, &domain_num);

			/* 運用中データ書込み */
			zlExampleApp.clock[TARGET_DPLL].ptp.config.defaultDS.domainNumber = domain_num;

			if(( status = zl303xx_PtpClockDomainNumberSet(zlExampleApp.clock[TARGET_DPLL].ptp.handle,domain_num)) != ZL303XX_OK){
				ZL303XX_TRACE_ALWAYS("zl303xx_PtpClockDomainNumberSet Error(%d) domain_num : old:%d new:%d", status,*f_ptp_d_num , domain_num , 0 , 0 , 0 );
			}

			/* グローバル変数置換 */
			*f_ptp_d_num = domain_num;

			BPF_COM_CMD_SET_PTPINVALID();	/* command情報初期化 */

		}
	}

	if ( rcvMsg->ptp_config_t.multicast_mac_address != E_SYNC_MULTICAST_ADDR_UNCAHNGE )
	{
		if (m_addr != *f_ptp_m_macaddr ) {

			ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "multicast_macaddr changes. value=%d", m_addr, 0,0,0,0,0);

			/* EEPROM書換え */
			BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_MULTI_MACADDR, &m_addr);

			/* グローバル変数置換 */
			*f_ptp_m_macaddr = m_addr;

			port = zl303xx_GetExampleInterface();
			zl303xx_ClockStreamS* pPtpStream = Zl303xx_PtpStreamArray[port] ;

			reg_data1	= D_RRH_ZERO ;
			reg_data2	= (multi_mac_address[m_addr][0] << D_PTP_SHIT_BYTE1) |  multi_mac_address[m_addr][1] ;
			reg_data3	= (multi_mac_address[m_addr][2] << D_PTP_SHIT_BYTE3) | (multi_mac_address[m_addr][3] << D_PTP_SHIT_BYTE2) |
						  (multi_mac_address[m_addr][4] << D_PTP_SHIT_BYTE1) |  multi_mac_address[m_addr][5] ;
			if(m_addr == E_SYNC_MULTICAST_ADDR_NONFORWARDABLE )
			{
				reg_ini_addr[0] = D_DU_REG_ETH_FILTDA31;
				reg_ini_addr[1] = D_DU_REG_ETH_FILTDA32;

				reg_set_addr[0] = D_DU_REG_ETH_FILTDA21;
				reg_set_addr[1] = D_DU_REG_ETH_FILTDA22;
			}
			else
			{
				reg_ini_addr[0] = D_DU_REG_ETH_FILTDA21;
				reg_ini_addr[1] = D_DU_REG_ETH_FILTDA22;

				reg_set_addr[0] = D_DU_REG_ETH_FILTDA31;
				reg_set_addr[1] = D_DU_REG_ETH_FILTDA32;
			}

			examplePtpSocketTblPrint();

			/* 使用中のmulti-mac-address初期化 */
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_ini_addr[0], &reg_data1);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_ini_addr[1], &reg_data1);

			/* multi-mac-address受信設定  */
			memcpy( &examplePtpDestAddr[port].addressField[0], &multi_mac_address[m_addr][0], 6 );
			memcpy( &pPtpStream->config.destAddr.addressField[0], &examplePtpDestAddr[port].addressField[0], 6 );
			memcpy( &pStream->config.destAddr.addressField[0], &examplePtpDestAddr[port].addressField[0], 6 );

			/* アクティブのExamplePtpSocketTblを検索 */
			for(count = 0; count < EXAMPLE_PTP_SOCKETS_MAX; count++)
			{
				entry = &ExamplePtpSocketTbl[count];
				if(entry->active)
				{
					if(m_addr == E_SYNC_MULTICAST_ADDR_NONFORWARDABLE)
					{
						if(memcmp(&multi_mac_address[E_SYNC_MULTICAST_ADDR_FORWARDABLE][0] , &entry->mcastAddr.addressField[0] , 6) == 0)
						{
							memcpy( &entry->mcastAddr.addressField[0], &pPtpStream->config.destAddr.addressField[0], 6);
						}
					}
					else
					{
						if(memcmp(&multi_mac_address[E_SYNC_MULTICAST_ADDR_NONFORWARDABLE][0] , &entry->mcastAddr.addressField[0] , 6) == 0)
						{
							memcpy( &entry->mcastAddr.addressField[0], &pPtpStream->config.destAddr.addressField[0], 6);
						}
					}
				}
			}

			/* 変更後のmulti-mac-address設定	*/
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_set_addr[0], &reg_data2);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_set_addr[1], &reg_data3);

			examplePtpSocketTblPrint();

			BPF_COM_CMD_SET_PTPINVALID();	/* command情報初期化 */

		}
	}

	if ( rcvMsg->ptp_config_t.clock_classes != PTP_EDIT_DATA_UNDEF_16 )
	{
		if (accept_class != *f_ptp_accepted_clockclass) {

			ZL303XX_TRACE(ZL303XX_MOD_ID_PTP_ENGINE, 2, "accept_class changes. value=%d", accept_class, 0,0,0,0,0);

			/* EEPROM書換え */
			BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_VID_ACPT_CLK_CLASS, &accept_class);

			/* グローバル変数置換 */
			*f_ptp_accepted_clockclass = accept_class;

			/* 運用中データ書込み */
			int pre_accepted_clock_classes = accepted_clock_classes;
			accepted_clock_classes = (int)accept_class;
			keep_announce_clockclass = 0;	/* keepは一旦初期化:これがないと停波解除されない可能性がある */

			if(( status = zl303xx_PtpStreamMaxClockClassSet(zlExampleApp.stream[0].ptp.handle,accept_class)) != ZL303XX_OK){
				ZL303XX_TRACE_ALWAYS("zl303xx_PtpStreamMaxClockClassSet Error(%d) accepted_clock_classes : old:%d new:%d", status,pre_accepted_clock_classes , accepted_clock_classes , 0 , 0 , 0 );
			}
			
			BPF_COM_CMD_SET_PTPINVALID();	/* command情報初期化 */

		}
	}

	if( rcvMsg->ptp_config_t.delay_asymmetry != PTP_EDIT_DATA_DEF_16 )
	{
		/* Delay-Asymmetryの設定 */
		delayNs = (Sint32T)(((Sint32T)delay_asymmetry) * -1);

		streamHandle = zlExampleApp.stream[0].ptp.handle ;

		/* 符号反転してingress,egress両方に設定する*/
		status = zl303xx_PtpStreamDelayAsymmetryNsSet( streamHandle , ZL303XX_TRUE , delayNs ) ;
		status |= zl303xx_PtpStreamDelayAsymmetryNsSet( streamHandle , ZL303XX_FALSE , delayNs ) ;
		if( status != ZL303XX_OK )
		{
			/* 異常終了なら */
			ZL303XX_TRACE_ALWAYS("zl303xx_PtpStreamDelayAsymmetrySet Error delayInterval : %x", delay_asymmetry , 0 , 0 , 0 , 0 , 0);
		}
	}

	/* Sync, SyncE, PTP状態取得	*/
	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ , D_DU_REG_CTB_SV16INF , &reg_data1 );

	/* Sync-状態周期タイマの設定(supervisionが未) */
	if( f_sync_reporting_period != D_PTP_SYNC_REPORT )
	{
		ZL303XX_TRACE_ALWAYS( "monitor start from Sync  initial-state : %08x", reg_data1 & D_DU_SVCTL_SYNC_STATE_ALL , 0 , 0 , 0 , 0 , 0 );

		/* タイマ設定(1秒周期) */
		time_data = D_PTP_REPORT_TIMER * D_PTP_SYNC_REPORT ;
		ret = BPF_RU_HRTM_REGISTER(	BPF_RU_HRTM_REG_PERIODIC,					/* Timer種別						*/
									E_RRH_TIMID_SYNC_STSREPRT,					/* Timer ID							*/
									time_data,									/* Timer値							*/
									D_SYS_MSGID_SYNCSTSTO_NTC,					/* TimeOut時通知Event番号			*/
									D_SYS_THDQID_PTP_MAIN,						/* TimeOut時送信先Queue番号			*/
									&errcd );
		if( ret != D_SYS_OK )
		{
			ZL303XX_TRACE_ALWAYS( "Sync-State reporting period Timer Set NG errcd : %08x", errcd , 0 , 0 , 0 , 0 , 0 );
		}
		f_sync_reporting_period = D_PTP_SYNC_REPORT ;
	}

	/* PTP-状態保護タイマの設定 */
	if( rcvMsg->ptp_config_t.ptp_reporting_period != PTP_EDIT_DATA_UNDEF_16 ){
		if( f_ptp_reporting_guarded == D_RRH_NUMFFFFFFFF ){

			ZL303XX_TRACE_ALWAYS( "monitor start from PTP   initial-state : %08x", reg_data1 & D_DU_SVCTL_PTP_STATE_LOCKED , 0 , 0 , 0 , 0 , 0 );

			time_data = D_PTP_REPORT_TIMER * D_PTP_PTP_REPORT ;
			/* タイマ設定 */
			ret = BPF_RU_HRTM_REGISTER(	BPF_RU_HRTM_REG_PERIODIC,				/* Timer種別						*/
										E_RRH_TIMID_PTP_LCKREPRT,				/* Timer ID							*/
										time_data,								/* Timer値							*/
										D_SYS_MSGID_PTPSTSTO_NTC,				/* TimeOut時通知Event番号			*/
										D_SYS_THDQID_PTP_MAIN,					/* TimeOut時送信先Queue番号			*/
										&errcd );
			if( ret != D_SYS_OK ){
				/* タイマ設定エラーの表示 */
				ZL303XX_TRACE_ALWAYS( "PTP-State reporting period Timer Set NG errcd : %08x", errcd , 0 , 0 , 0 , 0 , 0 );
			}else{
				f_ptp_reporting_guarded = D_RRH_ZERO;
			}
		}
		f_ptp_reporting_period = ptp_report_time ;
	}

	/* SyncE-状態保護タイマの設定 */
	if( rcvMsg->ptp_config_t.syncE_reporting_period != PTP_EDIT_DATA_UNDEF_16 )
	{
		if( f_syncE_reporting_guarded == D_RRH_NUMFFFFFFFF ){

			ZL303XX_TRACE_ALWAYS( "monitor start from SyncE initial-state : %08x", reg_data1 & D_DU_SVCTL_SYNCE_STATE_LOCKED , 0 , 0 , 0 , 0 , 0 );

			time_data = D_PTP_REPORT_TIMER * D_PTP_SYNCE_REPORT ;
			/* タイマ設定 */
			ret = BPF_RU_HRTM_REGISTER(	BPF_RU_HRTM_REG_PERIODIC,				/* Timer種別						*/
										E_RRH_TIMID_SYNCE_LCKREPRT,				/* Timer ID							*/
										time_data,								/* Timer値							*/
										D_SYS_MSGID_SYNCESTSTO_NTC,				/* TimeOut時通知Event番号			*/
										D_SYS_THDQID_PTP_MAIN,					/* TimeOut時送信先Queue番号			*/
										&errcd );
			if( ret != D_SYS_OK ){
				/* タイマ設定エラーの表示 */
				ZL303XX_TRACE_ALWAYS( "SyncE-State reporting period Timer Set NG errcd : %08x", errcd , 0 , 0 , 0 , 0 , 0 );
			}else{
				f_syncE_reporting_guarded = D_RRH_ZERO;
			}
		}
		f_syncE_reporting_period = synce_report_time ;
	}

	/* SyncE-acceptance_ssmの設定 */
	pthread_mutex_lock(&f_syncE_ssm_mutex);
	for(count = 0; count < SYNCE_ACCEPTANCE_SSM_NUM; count++)
	{
		if( rcvMsg->ptp_config_t.syncE_acceptance_ssm[count] != PTP_EDIT_DATA_UNDEF_32 )
		{
			switch (rcvMsg->ptp_config_t.syncE_acceptance_ssm[count]){
				case E_SYNC_ACCEPTANCE_SSM_PRC:
					ssm_val = ESMC_QL_PRC;
					break;
				case E_SYNC_ACCEPTANCE_SSM_PRS:
					ssm_val = ESMC_QL_PRS;
					break;
				case E_SYNC_ACCEPTANCE_SSM_SSU_A:
					ssm_val = ESMC_QL_SSU_A;
					break;
				case E_SYNC_ACCEPTANCE_SSM_SSU_B:
					ssm_val = ESMC_QL_SSU_B;
					break;
				case E_SYNC_ACCEPTANCE_SSM_ST2:
					ssm_val = ESMC_QL_ST2;
					break;
				case E_SYNC_ACCEPTANCE_SSM_ST3:
					ssm_val = ESMC_QL_ST3;
					break;
				case E_SYNC_ACCEPTANCE_SSM_ST3E:
					ssm_val = ESMC_QL_ST3E;
					break;
				case E_SYNC_ACCEPTANCE_SSM_EEC1:
					ssm_val = ESMC_QL_EEC1;
					break;
				case E_SYNC_ACCEPTANCE_SSM_EEC2:
					ssm_val = ESMC_QL_EEC2;
					break;
				case E_SYNC_ACCEPTANCE_SSM_DNU:
					ssm_val = ESMC_QL_DNU;
					break;
				case E_SYNC_ACCEPTANCE_SSM_NONE:
					ssm_val = ESMC_QL_NONE;
					break;
				default:
					ZL303XX_TRACE_ALWAYS("Error syncE_acceptance_ssm unknown valune : %x", rcvMsg->ptp_config_t.syncE_acceptance_ssm[count] , 0 , 0 , 0 , 0 , 0);
					ssm_val = PTP_EDIT_DATA_UNDEF_32;
					break;
				}
			
				for(count2 = 0; count2 < SYNCE_ACCEPTANCE_SSM_NUM; count2++)
				{
					if(ssm_val == f_syncE_acceptance_ssm[count2])
					{
						/* ssm登録済 */
						break;
					}
					else if(f_syncE_acceptance_ssm[count2] == PTP_EDIT_DATA_UNDEF_32){
						/* ssm登録 */
						f_syncE_acceptance_ssm[count2] = ssm_val;
						break;
					}
				}
			}
	}
	pthread_mutex_unlock(&f_syncE_ssm_mutex);

	/* SyncE-ssmタイマの設定 */
	if( rcvMsg->ptp_config_t.syncE_ssm_timeout != PTP_EDIT_DATA_UNDEF_32 )
	{
		if( synce_ssm_timer != f_syncE_ssm_timer_e )
		{
			/* タイマ再設定	*/
			pthread_mutex_lock(&f_syncE_ssm_mutex);
			f_syncE_ssm_timer_e = synce_ssm_timer ;
			pthread_mutex_unlock(&f_syncE_ssm_mutex);
		}
	}

	/* CFM応答送信 */
	f_com_MSIsend(D_MSG_MSI_O_RAN_SYNC_EDIT_CFM,reqid, 0, NULL, D_RRH_PROCQUE_PTP, D_SYS_THDQID_PTP_MAIN);

	ZL303XX_TRACE_ALWAYS("End edit_mplane_param interface domain_num : %x  m_addr : %x  accept_class : %x", *f_ptp_d_num , *f_ptp_m_macaddr , *f_ptp_accepted_clockclass , 0,0,0);

	return;
}

/**
* @brief Sync-Status Reporting Period Timer Start function
* @note Sync-Status Reporting Period Timer Start function
* @param N/A
* @return N/A
* @date 2019/08/01 FJT)Taniguchi
* @warning	N/A
* @FeatureID	5GDU-002-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID ntc_mplane_param( VOID )
{
	UINT	reg_data1;
	INT		ret;
	INT		errcd ;

	/* Sync, SyncE, PTP状態取得	*/
	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ , D_DU_REG_CTB_SV16INF , &reg_data1 );

	/* Sync-状態周期タイマの設定(supervisionが未) */
	if( f_sync_reporting_period != D_PTP_SYNC_REPORT ){

		ZL303XX_TRACE_ALWAYS( "Starting Sync status supervision by  M-plane start up current SYNC state : 0x%08x", reg_data1 & D_DU_SVCTL_SYNC_STATE_ALL , 0 , 0 , 0 , 0 , 0 );

		/* タイマ設定(1秒周期) */
		ret = BPF_RU_HRTM_REGISTER(	BPF_RU_HRTM_REG_PERIODIC,					/* Timer種別						*/
									E_RRH_TIMID_SYNC_STSREPRT,					/* Timer ID							*/
									(D_PTP_REPORT_TIMER * D_PTP_SYNC_REPORT),	/* Timer値							*/
									D_SYS_MSGID_SYNCSTSTO_NTC,					/* TimeOut時通知Event番号			*/
									D_SYS_THDQID_PTP_MAIN,						/* TimeOut時送信先Queue番号			*/
									&errcd );
		if( ret != D_SYS_OK ){
			ZL303XX_TRACE_ALWAYS( "Sync-State reporting period Timer Set NG errcd : %08x", errcd , 0 , 0 , 0 , 0 , 0 );
		}else{
			f_sync_reporting_period = D_PTP_SYNC_REPORT ;
		}
	}

	/* PTP-状態保護タイマの設定 */
	if( f_ptp_reporting_guarded == D_RRH_NUMFFFFFFFF )
	{
		ZL303XX_TRACE_ALWAYS( "Starting PTP status supervision by M-plane start up current PTP-state : %08x", reg_data1 & D_DU_SVCTL_PTP_STATE_LOCKED , 0 , 0 , 0 , 0 , 0 );

		/* タイマ設定 */
		ret = BPF_RU_HRTM_REGISTER(	BPF_RU_HRTM_REG_PERIODIC,				/* Timer種別						*/
									E_RRH_TIMID_PTP_LCKREPRT,				/* Timer ID							*/
									D_PTP_REPORT_TIMER * D_PTP_PTP_REPORT,	/* Timer値							*/
									D_SYS_MSGID_PTPSTSTO_NTC,				/* TimeOut時通知Event番号			*/
									D_SYS_THDQID_PTP_MAIN,					/* TimeOut時送信先Queue番号			*/
									&errcd );
		if( ret != D_SYS_OK )
		{
			/* タイマ設定エラーの表示 */
			ZL303XX_TRACE_ALWAYS( "PTP-State reporting period Timer Set NG errcd : %08x", errcd , 0 , 0 , 0 , 0 , 0 );
		}else{
			f_ptp_reporting_period = D_PTP_PTP_REPORT_DEF ;
			f_ptp_reporting_guarded = D_RRH_ZERO;
		}
	}

	/* SyncE-状態保護タイマの設定 */
	if( f_syncE_reporting_guarded == D_RRH_NUMFFFFFFFF )
	{
		ZL303XX_TRACE_ALWAYS( "Starting SyncE status supervision by M-plane start up current SyncE-state : %08x", reg_data1 & D_DU_SVCTL_SYNCE_STATE_LOCKED , 0 , 0 , 0 , 0 , 0 );

		/* タイマ設定 */
		ret = BPF_RU_HRTM_REGISTER(	BPF_RU_HRTM_REG_PERIODIC,				/* Timer種別						*/
									E_RRH_TIMID_SYNCE_LCKREPRT,				/* Timer ID							*/
									D_PTP_REPORT_TIMER * D_PTP_SYNCE_REPORT,	/* Timer値							*/
									D_SYS_MSGID_SYNCESTSTO_NTC,				/* TimeOut時通知Event番号			*/
									D_SYS_THDQID_PTP_MAIN,					/* TimeOut時送信先Queue番号			*/
									&errcd );
		if( ret != D_SYS_OK )
		{
			/* タイマ設定エラーの表示 */
			ZL303XX_TRACE_ALWAYS( "SyncE-State reporting period Timer Set NG errcd : %08x", errcd , 0 , 0 , 0 , 0 , 0 );
		}
		else {
			f_syncE_reporting_period = D_PTP_SYNCE_REPORT_DEF ;
			f_syncE_reporting_guarded = D_RRH_ZERO;
		}
	}
	return;
}

/* @} */



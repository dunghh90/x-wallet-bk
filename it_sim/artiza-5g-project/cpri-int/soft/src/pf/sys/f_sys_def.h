/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Constant Value Definition
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 *  @date	2015/05/27 ALPHA)murakami ハソ改版(M-RRU-ZSYS-01582)対応
 *  @date	2015/10/30 ALPHA)murakami modify for M-RRU-ZSYS-01812
 *  @date	2015/10/30 ALPHA)ueda modify for M-RRU-ZSYS-01812
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2015
 */
/******************************************************************************************************************************/

/*!
 * @addtogroup RRH_PF_SYS
 * @{
 */
#ifndef F_SYS_DEF_H
#define F_SYS_DEF_H

/*!
 * @name Thread-ID
 * @note スレッドID
 * @{
 */
/* INIT Process	*/
#define D_SYS_THDID_INIT									0x00000010			/**< INIT Main Thread						*/
#define D_SYS_THDID_INIT_ALM								0x00000011			/**< INIT ALM Thread						*/
#define D_SYS_THDID_INIT_HLT								0x00000012			/**< INIT HLT Thread						*/
#define D_SYS_THDID_INIT_RUN								0x00000013			/**< INIT RUN Thread						*/
#define D_SYS_THDID_INIT_CON								0x00000014			/**< INIT CON Thread						*/

/* PF Process	*/
#define D_SYS_THDID_PF_MAIN									0x00000020			/**< Main Thread							*/
#define D_SYS_THDID_PF_RECV									0x00000021			/**< CMU Receive Thread						*/
#define D_SYS_THDID_PF_SEND									0x00000022			/**< CMU Send Thread						*/
#define D_SYS_THDID_PF_DBG									0x00000023			/**< FrontHaul Surveillance handler			*/
#define D_SYS_THDID_PF_HDLX									0x00000024			/**< Interrupt handler						*/
#define D_SYS_THDID_PF_EQS									0x00000025			/**< Equipment Surveillance handler			*/
#define D_SYS_THDID_PF_WDT									0x00000026			/**< WDT handler							*/
#define D_SYS_THDID_PF_NMA									0x00000027			/**< Non-volatile Memory Access handler		*/
#define D_SYS_THDID_PF_NMAS									0x00000028			/**< Non-volatile Memory Access handler (Slave)	*/
#define D_SYS_THDID_PF_LGC									0x00000029			/**< LoG Collection handler					*/
#define D_SYS_THDID_PF_CCA									0x0000002A			/**< Component Carrier Access handler		*/
#define D_SYS_THDID_PF_RFA									0x0000002B			/**< RF Access handler						*/
#define D_SYS_THDID_PF_DHC									0x0000002C			/**< DHCP handler							*/
#define D_SYS_THDID_PF_SFP									0x0000002D			/**< SFP monitor handler					*/
#define D_SYS_THDID_PTP_MAIN								0x00000050			/**< PTP main Thread						*/
#define D_SYS_THDID_PF_FHS									0x0000002F			/**< FrontHaul Surveillance handler			*/
#define D_SYS_THDID_PF_MPRECV								0x00000030			/**< CMU M-plane proxy receive handler		*/
#define D_SYS_THDID_PF_MPSEND								0x00000031			/**< CMU M-plane proxy send handler			*/
#define D_SYS_THDID_PF_EOM									0x00000032			/**< EthernetOaM handler					*/
#define D_SYS_THDID_PF_EOM_0								0x00000033			/**< EthernetOaM handler (fheth0)			*/
#define D_SYS_THDID_PF_EOM_1								0x00000034			/**< EthernetOaM handler (fheth1)			*/
#define D_SYS_THDID_PF_FDL									0x00000035			/**< File DownLoad handler					*/
#define D_SYS_THDID_PF_FUL									0x00000036			/**< File UpLoad handler					*/
#define D_SYS_THDID_PF_PFM									0x00000037			/**< Performance Monitor handler			*/
#define D_SYS_THDID_PF_EIO									0x00000038			/**< External IO Thread	(5G-商用DU)			*/
#define D_SYS_THDID_PF_MTI									0x00000039			/**< MTI Thread	(5G-商用DU)					*/
#define D_SYS_THDID_PF_AIH									0x00000040	 		/**< ALD(AISG) ThreadID						*/


#define D_SYS_THDID_PF_MAX									28					/**< スレッド最大数							*/
/* ※ 上記で定義する値は通番にすること(番号が飛ばないこと)、D_SYS_THDQNO_PF_MAXは最後に定義した値+2とすること (PTP_MAIN_THREAD含む)  */

#define D_SYS_THDID_PF_HDWDT								0x00000040			/**< Handler Thread = IRQ_121:WDT(IP_Macro)	*/
#define D_SYS_THDID_PF_HD1S									0x00000041			/**< Handler Thread = IRQ_122:1s周期		*/
#define D_SYS_THDID_PF_HD1PPS								0x00000042			/**< Handler Thread = IRQ_123:1pps Timing	*/
#define D_SYS_THDID_PF_HDSV									0x00000043			/**< Handler Thread = IRQ_124:SV状変		*/
#define D_SYS_THDID_PF_HDMSTA1								0x00000044			/**< Handler Thread = IRQ_140:ETH1 MAC状変	*/
#define D_SYS_THDID_PF_HDMSTA2								0x00000045			/**< Handler Thread = IRQ_141:ETH2 MAC状変	*/
#define D_SYS_THDID_PF_HDLALD								0x00000047	 		/**< ALD IRQ handlerThreadID				*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_SYS_THDID_PF_F_MAIN								0x00000020			/**< PF-FHM Main Thread						*/
#define D_SYS_THDID_PF_HDLM									0x00000021			/**< handle Main Thread						*/
#define D_SYS_THDID_PF_ETH									0x00000029			/**< Ethernet Set Thread					*/
#define D_SYS_THDID_PF_CPRI									0x0000002A			/**< CPRI Management Thread					*/
#define D_SYS_THDID_PF_RSV									0x0000002B			/**< RE Supervision Thread					*/
#define D_SYS_THDID_PF_F_DPD								0x0000002D			/**< DPD thread								*/
#define D_SYS_THDID_PF_NEGO									0x0000002E			/**< CPRI Auto Nego Thread					*/
#define D_SYS_THDID_PF_LOG									0x00000030			/**< Trouble Log Thread						*/
#define D_SYS_THDID_PF_FRMG									0x00000032			/**< fmg Thread								*/
#define D_SYS_THDID_PF_FRQ									0x00000033			/**< flash(QSPI) writing Thread				*/
#define D_SYS_THDID_PF_F_WDT							    0x00000034			/**< Watch Dog Timer Thread					*/
#define D_SYS_THDID_PF_FRQS									0x00000036			/**< flash(QSPI) writing Thread(slave)		*/
#define D_SYS_THDID_PF_FRQRE								0x00000038			/**< flash(QSPI) writing(for RE) Thread		*/

#define D_SYS_THDID_PF_HDL1									0x00000045			/**< Handler Thread(IRQ65)					*/
#define D_SYS_THDID_PF_HDPCIE								0x0000004B			/**< Handler Thread(IRQ52)					*/
#define D_SYS_THDID_PF_HDCPRI								0x00000048			/**< Handler Thread(IRQ84)					*/
#define D_SYS_THDID_PF_HDCPRI_RE							0x00000049			/**< Handler Thread(IRQ85)					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @} */

/*!
 * @name ThreadQ-ID_PTP
 * @note スレッドQID_PTP
 * @{
 */
#define D_SYS_THDID_PTP_ESMCRCV								0x00000051			/**< PTP Recieve Thread						*/
#define D_SYS_THDID_PTP_ESMCSND								0x00000052			/**< PTP Send Thread						*/
/* @} */

/*!
 * @name ThreadQ-ID_INIT
 * @note スレッドQID_INIT
 * @{
 */
#define D_SYS_THDQID_INI_MAIN								1					/**< INIT Main Thread						*/
#define D_SYS_THDQID_INI_ALM								2					/**< INIT ALM Thread						*/
#define D_SYS_THDQID_INI_HLT								3					/**< INIT HLT Thread						*/
#define D_SYS_THDQID_INI_RUN								4					/**< INIT RUN Thread						*/
#define D_SYS_THDQID_INI_CON								5					/**< INIT RUN Thread						*/
/* @} */

/*!
 * @name ThreadQ-ID_PF
 * @note スレッドQID_PF
 * @{
 */
#define D_SYS_THDQID_PF_MAIN								1					/**< Main Thread							*/
#define D_SYS_THDQID_PF_RECV								2					/**< CMU Receive Thread						*/
#define D_SYS_THDQID_PF_SEND								3					/**< CMU Send Thread						*/
#define D_SYS_THDQID_PF_DBG									4					/**< FrontHaul Surveillance handler			*/
#define D_SYS_THDQID_PF_HDLX								5					/**< Interrupt handler						*/
#define D_SYS_THDQID_PF_EQS									6					/**< Equipment Surveillance handler			*/
#define D_SYS_THDQID_PF_WDT									7					/**< WDT handler							*/
#define D_SYS_THDQID_PF_NMA									8					/**< Non-volatile Memory Access handler		*/
#define D_SYS_THDQID_PF_NMAS								9					/**< Non-volatile Memory Access handler (Slave)	*/
#define D_SYS_THDQID_PF_LGC									10					/**< LoG Collection handler					*/
#define D_SYS_THDQID_PF_CCA									11					/**< Component Carrier Access handler		*/
#define D_SYS_THDQID_PF_RFA									12					/**< RF Access handler						*/
#define D_SYS_THDQID_PF_DHC									13					/**< DHCP handler							*/
#define D_SYS_THDQID_PF_SFP									14					/**< SFP monitor handler					*/
#define D_SYS_THDQID_PTP_MAIN								15					/**< PTP main Thread						*/
#define D_SYS_THDQID_PF_FHS									16					/**< FrontHaul Surveillance handler			*/
#define D_SYS_THDQID_PF_MPRECV								17					/**< CMU M-plane proxy receive handler		*/
#define D_SYS_THDQID_PF_MPSEND								18					/**< CMU M-plane proxy send handler			*/
#define D_SYS_THDQID_PF_EOM									19					/**< EthernetOaM handler					*/
#define D_SYS_THDQID_PF_EOM_0								20					/**< EthernetOaM handler (fheth0)			*/
#define D_SYS_THDQID_PF_EOM_1								21					/**< EthernetOaM handler (fheth1)			*/
#define D_SYS_THDQID_PF_FDL									22					/**< File DownLoad handler					*/
#define D_SYS_THDQID_PF_FUL									23					/**< File UpLoad handler					*/
#define D_SYS_THDQID_PF_PFM									24					/**< Performance Monitor handler			*/
#define D_SYS_THDQID_PF_EIO									25					/**< External IO Thread	(5G-商用DU)			*/
#define D_SYS_THDQID_PF_MTI									26					/**< MTI Thread	(5G-商用DU)			*/
#define D_SYS_THDQID_PF_AIH									27					/**< AIH ThreadQueueID						*/

#define D_SYS_THDQNO_PF_MAX									D_SYS_THDID_PF_MAX	/**< スレッドQID最大数						*/
/* ※ 上記で定義する値は通番にすること(番号が飛ばないこと)、D_SYS_THDQNO_PF_MAXは最後に定義した値+2とすること (PTP_MAIN_THREAD含む)  */

#define D_SYS_THDQID_PF_DIA									25					/**< Equipment Diagnosys					*//* 未対応	*/

#define D_SYS_THDQID_PF_HDWDT								33					/**< Handler Thread = IRQ_121:WDT(IP_Macro)	*/
#define D_SYS_THDQID_PF_HD1S								34					/**< Handler Thread = IRQ_122:1s周期		*/
#define D_SYS_THDQID_PF_HD1PPS								35					/**< Handler Thread = IRQ_123:1pps Timing	*/
#define D_SYS_THDQID_PF_HDSV								36					/**< Handler Thread = IRQ_124:SV状変		*/
#define D_SYS_THDQID_PF_HDMSTA1								37					/**< Handler Thread = IRQ_140:ETH1 MAC状変	*/
#define D_SYS_THDQID_PF_HDMSTA2								38					/**< Handler Thread = IRQ_141:ETH2 MAC状変	*/
#define D_SYS_THDQID_PF_HDLALD								39					/**< HDLALD ThreadQueueID					*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_SYS_THDQID_PF_F_MAIN								1					/**< PF Main Thread							*/
#define D_SYS_THDQID_PF_HDLM								2					/**< handle Main Thread						*/
#define D_SYS_THDQID_PF_ETH									10					/**< Ethernet Set Thread					*/
#define D_SYS_THDQID_PF_CPRI								11					/**< CPRI Management Thread					*/
#define D_SYS_THDQID_PF_RSV									12					/**< RE Supervision Thread					*/
#define D_SYS_THDQID_PF_F_DPD								14					/**< DPD thread								*/
#define D_SYS_THDQID_PF_NEGO								15					/**< CPRI Auto Nego Thread					*/
#define D_SYS_THDQID_PF_LOG									17					/**< Trouble Log Thread						*/
#define D_SYS_THDQID_PF_FRMG								19					/**< fmg Thread								*/
#define D_SYS_THDQID_PF_FRQ									20					/**< flash(QSPI) writing Thread				*/
#define D_SYS_THDQID_PF_F_WDT							    21					/**< Watch Dog Timer Thread					*/
#define D_SYS_THDQID_PF_FRQS								23					/**< flash(QSPI) writing Thread(slave)		*/
#define D_SYS_THDQID_PF_FRQRE								24					/**< flash(QSPI) writing （for RE)Thread	*/

#define D_SYS_THDQID_PF_HDL1								35					/**< Handler Thread(IRQ65)					*/
#define D_SYS_THDQID_PF_HDPCIE								41					/**< Handler Thread(IRQ52)					*/
#define D_SYS_THDQID_PF_HDCPRI								38					/**< Handler Thread(IRQ84)					*/
#define D_SYS_THDQID_PF_HDCPRI_RE							39					/**< Handler Thread(IRQ85)					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/* @} */

/*!
 * @name ThreadQ-ID_PF
 * @note スレッドQID_PF
 * @{
 */
#define D_SYS_THDQID_PTP_ESMCRCV							39					/**< PTP Recieve Thread						*/
#define D_SYS_THDQID_PTP_ESMCSND							40					/**< PTP Send Thread						*/
/* ※ PF ThreadのMax値+1とすること  */
/* @} */

/*!
 * @name PF Process Thread Name
 * @note スレッド名称
 * @{
 */
#define D_SYS_THDNAME_MAIN									"MAIN"			/**< Main Thread							*/
#define D_SYS_THDNAME_RECV									"RECV"			/**< CMU Receive Thread						*/
#define D_SYS_THDNAME_SEND									"SEND"			/**< CMU Send Thread						*/
#define D_SYS_THDNAME_DBG									"DBG"			/**< Debug Thread							*/
#define D_SYS_THDNAME_HDLX									"HDLX"			/**< Interrupt handler						*/
#define D_SYS_THDNAME_EQS									"EQS"			/**< Equipment Surveillance handler			*/
#define D_SYS_THDNAME_WDT									"WDT"			/**< WDT handler							*/
#define D_SYS_THDNAME_NMA									"NMA"			/**< Non-volatile Memory Access handler		*/
#define D_SYS_THDNAME_NMAS									"NMAS"			/**< Non-volatile Memory Access handler (Slave)	*/
#define D_SYS_THDNAME_LGC									"LGC"			/**< LoG Collection handler					*/
#define D_SYS_THDNAME_CCA									"CCA"			/**< Component Carrier Access handler		*/
#define D_SYS_THDNAME_RFA									"RFA"			/**< RF Access handler						*/
#define D_SYS_THDNAME_DHC									"DHC"			/**< DHCP handler							*/
#define D_SYS_THDNAME_SFP									"SFP"			/**< SFP monitor handler					*/
#define D_SYS_THDNAME_FHS									"FHS"			/**< FrontHaul Surveillance handler			*/
#define D_SYS_THDNAME_EOM									"EOM"			/**< EthernetOaM handler					*/
#define D_SYS_THDNAME_EOM_0									"EOM0"			/**< EthernetOaM handler (fheth0)			*/
#define D_SYS_THDNAME_EOM_1									"EOM1"			/**< EthernetOaM handler (fheth1)			*/
#define D_SYS_THDNAME_FDL									"FDL"			/**< File DownLoad handler					*/
#define D_SYS_THDNAME_FUL									"FUL"			/**< File UpLoad handler					*/
#define D_SYS_THDNAME_PFM									"PFM"			/**< Performance Monitor handler			*/
#define D_SYS_THDNAME_EIO									"EIO"			/**< External IO Thread	(5G-商用DU)			*/
#define D_SYS_THDNAME_MTI									"MTI"			/**< MTI Thread	(5G-商用DU)					*/

#define D_SYS_THDNAME_DIA									"DIA"			/**< Equipment Diagnosys					*//* 未対応	*/
/* @} */

/*!
 * @name PF Process Thread Name
 * @note スレッド名称
 * @{
 */
#define D_SYS_THDNAME_PTP									"PTP"			/**< PTP main Thread						*/
/* @} */

/* Message ID(common) */
#define D_SYS_MSGID_INIT_COMP_NTC							0x00000102			/**< 初期化応答								*/

/* Message ID(cmu) */
#define D_SYS_MSGID_MSI_RCV_IND								0x10020001			/**< MSI receive Indication					*/
#define D_SYS_MSGID_MSI_SND_IND								0x10030001			/**< MSI send Indication					*/

/* Message ID(pf_main) */
#define D_SYS_MSGID_TROUBLELOG_OUT_REQ						0x10010003			/**< 障害ログ出力要求						*/
#define D_SYS_MSGID_RUNLOG_OUT_NTC							0x10010005			/**< 運用ログ出力完了通知					*/
#define D_SYS_MSGID_ALLTSKININTC							0x10010007			/**< All thread initialize Notification		*/
#define D_SYS_MSGID_TROUBLELOG_WRITE_REQ					0x10010009			/**< 障害ログ書き込み要求					*/
#define D_SYS_MSGID_MPLANE_SW_INIT_COMP_NTC					0x1001000B			/**< M-Plane SW起動完了通知					*/

/* Message ID(pf_log) */
#define D_SYS_MSGID_FLSLOGWR_IND							0x100C0001			/**< Log-Flash Clear/Write Request			*/
#define D_SYS_MSGID_CYCLIC_LOG_SHELTER_TO					0x100C0003			/**< ログ退避用タイマTO						*/
#define D_SYS_MSGID_TROUBLE_LOG_WRITE_REQ					0x100C0005			/**< 障害ログ書き込み要求					*/

/* Message ID(pf_dia) */
#define D_SYS_MSGID_FLASH_READ_REQ							0x100D0001			/**< FLASH読み込み要求						*/
#define D_SYS_MSGID_FLASH_READ_RSP							0x100D0002			/**< FLASH読み込み応答						*/

/* Message ID(pf_rfcp) */
#define D_SYS_MSGID_SYSTEM_PARAM_UPDATE						0x100E0001			/**< System Parameter Update Request(S3G:接続構成:S3G単独)	*/
#define D_SYS_MSGID_PA_CONTROL								0x100E0002			/**< PA Control Request						*/
#define D_SYS_MSGID_WAVE_CONTROL							0x100E0003			/**< APD Wave Control Request				*/
#define D_SYS_MSGID_CARRIER_ON								0x100E0004			/**< Carrier ON Request						*/
#define D_SYS_MSGID_CARRIER_OFF								0x100E0005			/**< Carrier OFF Request					*/
#define D_SYS_MSGID_NO_MOD_CAR								0x100E0006			/**< No Modulation Carrier ON/OFF Request	*/
#define D_SYS_MSGID_CPRI_CONTROL							0x100E0007			/**< CPRI Control Request					*/
#define D_SYS_MSGID_GET_CAR_FREQ							0x100E0008			/**< Get Carrier Frequency Info Req			*/

#define D_SYS_MSGID_CARRIER_3G_ON							0x100E0009			/**< Carrier ON Request(3G)					*/
#define D_SYS_MSGID_CARRIER_3G_OFF							0x100E000A			/**< Carrier OFF Request(3G)				*/
#define D_SYS_MSGID_NO_MOD_3G_CAR							0x100E000B			/**< No Modulation Carrier ON/OFF Request(3G)	*/

/* Message ID(pf_dpda) */
#define	D_SYS_MSGID_SYS_PARAM_SET_REQ						0x100D0001			/**< System Parameter Set Request(S3G:接続構成:S3G単独)	*/ /* 5GDU_chg */
#define	D_SYS_MSGID_CARRIER_ON_SETTING_REQ					0x100D0002			/**< Carrier ON Setting Request				*/ /* 5GDU_chg */
#define	D_SYS_MSGID_CARRIER_OFF_SETTING_REQ					0x100D0003			/**< Carrier OFF Setting Request			*/ /* 5GDU_chg */
#define	D_SYS_MSGID_TXRXCAL_INUSE_REQ						0x100D0004			/**< The request of TxCal RxCal in use		*/ /* 5GDU_chg */
#define	D_SYS_MSGID_TXRXCAL_TRAINING_REQ					0x100D0005			/**< The request of TxCal RxCal Training	*/ /* 5GDU_chg */
#define D_SYS_MSGID_TXON_REQ								0x100D0011			/**< TX-ON(M-Plane) Request					*/
#define D_SYS_MSGID_TXON_CFM								0x100D0012			/**< TX-ON(M-Plane) Comfirm					*/
#define D_SYS_MSGID_RXON_REQ								0x100D0013			/**< RX-ON(M-Plane) Request					*/
#define D_SYS_MSGID_RXON_CFM								0x100D0014			/**< RX-ON(M-Plane) Comfirm					*/
#define D_SYS_MSGID_TXOFF_REQ								0x100D0015			/**< TX-OFF(M-Plane) Request					*/
#define D_SYS_MSGID_TXOFF_CFM								0x100D0016			/**< TX-OFF(M-Plane) Comfirm					*/
#define D_SYS_MSGID_RXOFF_REQ								0x100D0017			/**< RX-OFF(M-Plane) Request					*/
#define D_SYS_MSGID_RXOFF_CFM								0x100D0018			/**< RX-OFF(M-Plane) Comfirm					*/
#define D_SYS_MSGID_TDDEN_CHG_NTC							0x100D0019			/**< TDD Enable chenge notice					*/
#define D_SYS_MSGID_BEAMID_SET_REQ							0x100D001A			/**< BeamID setting request					*/
#define D_SYS_MSGID_BEAMID_SET_RSP							0x100D001B			/**< BeamID setting response				*/
#define D_SYS_MSGID_TXSLEEP_REQ								0x100D0020			/**< TX-SLEEP(M-Plane) Request					*/
#define D_SYS_MSGID_RXSLEEP_REQ								0x100D0021			/**< RX-SLEEP(M-Plane) Request					*/


#define	D_SYS_MSGID_SEND_POWER_ADJUST_REQ					0x100F0001			/**< Send Power Adjust Request				*/
#define	D_SYS_MSGID_SEND_POWER_SAVE_REQ						0x100F0002			/**< Send Power Save Request				*/
#define	D_SYS_MSGID_PA_CONTROL_REQ							0x100F0004			/**< PA Control Request						*/
#define	D_SYS_MSGID_APD_WAVE_ONOFF_REQ						0x100F0005			/**< APD Wave ON/OFF Request				*/
#define	D_SYS_MSGID_NO_MOD_CAR_ONOFF_REQ					0x100F0008			/**< No Modulation Carrier ON/OFF Request	*/
#define	D_SYS_MSGID_IQ_SETTING_REQ							0x100F000B			/**< IQ Setting Request						*/
#define	D_SYS_MSGID_CALIB_CTRL_REQ							0x100F000C			/**< Calibration Control Request			*/
#define	D_SYS_MSGID_CALIB_CTRL_RSP							0x100F000D			/**< Calibration Control Response			*/
#define	D_SYS_MSGID_CARRIER_ON_SETTING_3G_REQ				0x100F000E			/**< Carrier ON Setting Request(3G)			*/
#define	D_SYS_MSGID_CARRIER_OFF_SETTING_3G_REQ				0x100F000F			/**< Carrier OFF Setting Request(3G)		*/
#define	D_SYS_MSGID_NO_MOD_CAR_ONOFF_3G_REQ					0x100F0010			/**< No Modulation Carrier ON/OFF Request(3G)	*/
#define	D_SYS_MSGID_MON_TDD_HARD_OFF_REQ					0x100F0011			/**< Monitor TDHDOFFx						*/

/* Message ID(pf_cpr) */
#define D_SYS_MSGID_CPRILNK_STATE_CHG_NTC					0x100B0001
#define D_SYS_MSGID_AUTONEGO_COMPLETE_NTC					0x100B0002
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_SYS_MSGID_LINKUP_TIMEOUT_NTC						0x100B0004
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/* @} */

/* Message ID(pf_mcpr) */
#define D_SYS_MSGID_MCPRILNK_TIMEOUT_NTC					0x100B0003
#define D_SYS_MSGID_MCPRI_HFNSYNC_TIMEOUT_NTC				0x100B0005
/* @} */

/* Message ID(pf_nego) */
#define D_SYS_MSGID_AUTONEGO_START_IND						0x10140001
#define D_SYS_MSGID_AUTONEGO_STOP_IND						0x10140002
#define D_SYS_MSGID_AUTONEGO_TIMEOUT_NTC					0x10140003
#define D_SYS_MSGID_AUTONEGO_RESTART_IND					0x10140004

/* Message ID(pf_fmng) */
#define D_SYS_MSGID_FLASH_ERASE_REQ							0x10130001			/**< Flash Erase Request						*/
#define D_SYS_MSGID_FLASH_ERASE_RSP							0x10130002			/**< Flash Erase Response						*/
#define D_SYS_MSGID_FLASH_WRITE_REQ							0x10130003			/**< Flash Write Request						*/
#define D_SYS_MSGID_FLASH_WRITE_RSP							0x10130004			/**< Flash Write Response						*/
#define D_SYS_MSGID_FLASH_TIMEOUT_NTC						0x10130005			/**< Flash Write Timeout Response				*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_SYS_MSGID_TARPROCESS_RSP							0x10130006			/**< Tar Process Response						*/
#define D_SYS_MSGID_FLASH_FWINFO_SAVE_REQ					0x10130007			/**< FW-Info Save Request						*/
#define D_SYS_MSGID_FLASH_FWINFO_SAVE_RSP					0x10130008			/**< FW-Info Save Response						*/
#define D_SYS_MSGID_FLASH_RE_ERASE_REQ						0x10130009			/**< RE-Flash Erase Request						*/
#define D_SYS_MSGID_FLASH_RE_ERASE_RSP						0x1013000A			/**< RE-Flash Erase Response					*/
#define D_SYS_MSGID_FLASH_RE_WRITE_REQ						0x1013000B			/**< RE-Flash Write Request						*/
#define D_SYS_MSGID_FLASH_RE_WRITE_RSP						0x1013000C			/**< RE-Flash Write Response					*/
#define D_SYS_MSGID_FLASH_RE_READ_REQ						0x1013000D			/**< RE-Flash Read Request						*/
#define D_SYS_MSGID_FLASH_RE_READ_RSP						0x1013000E			/**< RE-Flash Read Response						*/
#define D_SYS_MSGID_RE_FLASH_TIMEOUT_NTC					0x1013000f			/**< RE-Flash Access Timeout Response			*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* Message ID(pf_hdl) */
#define D_SYS_MSGID_PASW_NTC								0x10020001			/**< PA switch change interruption notice	*/
#define D_SYS_MSGID_PORT_NTC								0x10020002			/**< PORT IRQ Notice						*/
#define D_SYS_MSGID_TIMOUTNTC								0x10150001			/**< タイムアウト発生通知					*/ /* 5GDU_chg */
#define D_SYS_MSGID_CPRISTACHGNTC							0x10020009			/**< CPRI State変化通知						*/
#define D_SYS_MSGID_TRA_IRQNTC								0x10020011			/**< カード状態変化割込通知					*/
#define D_SYS_MSGID_TX_HIGHSPEED							0x10020006			/**< 高速モード要求							*/
#define D_SYS_MSGID_FWFB_JESD_NTC							0x10020007			/**< Clemente FWFB IRQ(GPIO)				*/
#define D_SYS_MSGID_CPRISSTACHGNTC							0x10020031			/**< CPRIS State変化通知					*/

/* Message ID(pf_ptp) */
#define D_SYS_MSGID_PTPSTSTO_NTC							D_DU_MSGID_PTPSTSTO_NTC		/**< PTP-状態保護タイムアウト		*/
#define D_SYS_MSGID_SYNCESTSTO_NTC							D_DU_MSGID_SYNCESTSTO_NTC	/**< SyncE-状態保護タイムアウト		*/
#define D_SYS_MSGID_SYNCSTSTO_NTC							D_DU_MSGID_SYNCSTSTO_NTC	/**< Sync-状態保護タイムアウト		*/
#define D_SYS_MSGID_SYNCESSMTO_NTC							D_DU_MSGID_SYNCESSMTO_NTC	/**< SyncE-SSMタイムアウト			*/

/* @} */

/* Message ID(pf_extm) */
#define D_SYS_MSGID_EXT_RCV_NTC								0x10100001			/**< EXT data receiving notice					*/
#define D_SYS_MSGID_EXT_SND_RSP								0x10100002			/**< EXT data receiving notice					*/
/* @} */
/* Message ID(pf_extr) */
#define D_SYS_MSGID_EXT_START_NTC							0x10110001			/**< EXT data receiving notice					*/
/* @} */
/* Message ID(pf_extr) */
#define D_SYS_MSGID_EXT_SND_REQ								0x10120001			/**< EXT data receiving notice					*/

/* Message ID(pf_rsv) */
#define D_SYS_MSGID_INIREQ									0x100A0001			/**< Initialize Request						*/
#define D_SYS_MSGID_PASW_IRQNTC								0x100A0004			/**< PASW State change Notification			*/
#define D_SYS_MSGID_PORT_IRQNTC								0x100A0005			/**< Port State change Notification			*/
#define D_SYS_MSGID_STATE_SUB_REQ							0x100A0006			/**< Card state subscribe request			*/
#define D_SYS_MSGID_STATE_UNSUB_REQ							0x100A0007			/**< Card state unsubscribe request			*/
#define D_SYS_MSGID_DATETIME_SET_IND						0x100A0008			/**< Set datetime indication				*/
#define D_SYS_MSGID_PORTSTA_SUB_REQ							0x100A0009			/**< Get datetime request					*/
#define D_SYS_MSGID_CARD_CONCTL_IND							0x100A000A			/**< Card control indication				*/
#define D_SYS_MSGID_CARD_RESET_IND							0x100A000B			/**< Card reset indication					*/
#define D_SYS_MSGID_PORT_CONTRL_IND							0x100A000C			/**< Port control indication				*/
#define D_SYS_MSGID_LED_SET_IND								0x100A000D			/**< LED setting indication					*/
#define D_SYS_MSGID_ERRSET_IND								0x100A000E			/**< Error setting indication				*/

/* Message ID(pf_eio) *//*	5G-商用DU	*/
#define D_SYS_MSGID_EIO_EXTOUTSV_TO				D_DU_MSGID_EIO_EXTOUTSV_TO		/**< pf_eio : (0x10170001)外部IO入力状態監視タイムアウト通知	*/
#define D_SYS_MSGID_EIO_FANSVCTRL_TO			D_DU_MSGID_EIO_FANSVCTRL_TO		/**< pf_eio : (0x10170003)FAN監視制御タイムアウト通知			*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* Message ID(pf_aih) */
#define D_SYS_MSGID_AIH_ALD_TIMEOUT							0x10180001			/**< ALD receive timeout indication			*/
#define D_SYS_MSGID_AIH_ALDSND_END							0x10180003			/**< ALD transmission completion interrupt	*/
#define D_SYS_MSGID_AIH_ALDRCV_START						0x10180005			/**< ALD receive start interrupt 			*/
#define D_SYS_MSGID_AIH_ALDRCV_END							0x10180007			/**< ALD reception completion interrupt 	*/
#define D_SYS_MSGID_AIH_ALDRCV_ERR							0x10180009			/**< ALD receive error interrupt 			*/
#define D_SYS_MSGID_AIH_ALDRCV_NTF							0x1018000A			/**< ALD receive notification 			 	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#if 0
/* Message ID(pf_al2) */
#define D_SYS_MSGID_AISG_PF_BASE							(0x10170001)
#define D_SYS_EVTNO_ADEV_SCAN_IND							(D_SYS_MSGID_AISG_PF_BASE + 0)		/*!< Device scan indication	*/
#define D_SYS_EVTNO_ADEV_MOUNT_NTC							(D_SYS_MSGID_AISG_PF_BASE + 1)
#define D_SYS_EVTNO_ADEV_SCANCMP_NTC						(D_SYS_MSGID_AISG_PF_BASE + 2)
#define D_SYS_EVTNO_ADEV_SCANTO_NTC							(D_SYS_MSGID_AISG_PF_BASE + 3)
#define D_SYS_EVTNO_XID_SND_REQ								(D_SYS_MSGID_AISG_PF_BASE + 4)		/*!< XID sending request	*/
#define D_SYS_EVTNO_XID_SND_RES								(D_SYS_MSGID_AISG_PF_BASE + 5)		/*!< XID sending response	*/
#define D_SYS_EVTNO_SNRM_SND_REQ							(D_SYS_MSGID_AISG_PF_BASE + 6)		/*!< SNRM sending request	*/
#define D_SYS_EVTNO_SNRM_SND_RES							(D_SYS_MSGID_AISG_PF_BASE + 7)		/*!< SNRM sending response	*/
#define D_SYS_EVTNO_DISC_SND_REQ							(D_SYS_MSGID_AISG_PF_BASE + 8)		/*!< DISC sending request	*/
#define D_SYS_EVTNO_DISC_SND_RES							(D_SYS_MSGID_AISG_PF_BASE + 9)		/*!< DISC sending response	*/
#define D_SYS_EVTNO_ACMD_SND_REQ							(D_SYS_MSGID_AISG_PF_BASE + 10)		/*!< AISG signal sending request	*/
#define D_SYS_EVTNO_ACMD_SND_RES							(D_SYS_MSGID_AISG_PF_BASE + 11)		/*!< AISG signal sending response	*/
#define D_SYS_EVTNO_ACMD_RCV_NTC							(D_SYS_MSGID_AISG_PF_BASE + 12)		/*!< AISG signal receiving notification	*/
#define D_SYS_EVTNO_ALNK_REL_IND							(D_SYS_MSGID_AISG_PF_BASE + 13)		/*!< AISG link release indication	*/
#define D_SYS_EVTNO_ALNK_DWN_NTC							(D_SYS_MSGID_AISG_PF_BASE + 14)		/*!< AISG link down notification	*/

#define D_SYS_EVTNO_AISG_DELAY1STO_NTC						(D_SYS_MSGID_AISG_PF_BASE + 15)
#define D_SYS_EVTNO_AISG_AUTOCLSTO_NTC						(D_SYS_MSGID_AISG_PF_BASE + 16)
#define D_SYS_EVTNO_AISG_STARTUP_IND						(D_SYS_MSGID_AISG_PF_BASE + 17)		/*!< AISG startup indication	*/
#define D_SYS_EVTNO_AISG_SNDCMP_NTC							(D_SYS_MSGID_AISG_PF_BASE + 18)		/*!< AISG L1 sending complete notification	*/
#define D_SYS_EVTNO_AISG_RCV_DAT_NTC						(D_SYS_MSGID_AISG_PF_BASE + 19)		/*!< AISG L1 receiving data notification	*/
#define D_SYS_EVTNO_AISG_L2TO_NTC							(D_SYS_MSGID_AISG_PF_BASE + 20)	/*!< Timer Timeout notification	*/
#define D_SYS_EVTNO_AISG_RCV_INT_NTC						(D_SYS_MSGID_AISG_PF_BASE + 21)	/*!< AISG L1 receiving interrupt notification	*/
#define D_SYS_EVTNO_AISG_RCVING_INT_NTC						(D_SYS_MSGID_AISG_PF_BASE + 22)	/*!< AISG receiving interrupt notification	*/
#endif

/* Message ID(pf_nmas) @ 5GDU */
#define D_SYS_MSGID_NMA_LOG_WRITE_REQ						0x100B0001			/**< pf_nma : ログ情報書込み要求			*/
#define D_SYS_MSGID_NMA_FILE_WRITE_REQ						0x100B0003			/**< pf_nma : ファイル書込み要求			*/
#define D_SYS_MSGID_NMA_FILE_WRITE_RSP						0x100B0004			/**< pf_nma : ファイル書込み応答			*/
#define D_SYS_MSGID_NMA_NETCONF_WRITE_REQ					0x100B0005			/**< pf_nma : NETCONFユーザー情報書込み要求	*/
#define D_SYS_MSGID_NMA_NETCONF_WRITE_RSP					0x100B0006			/**< pf_nma : NETCONFユーザー情報書込み応答	*/
#define D_SYS_MSGID_NMA_VLAN_WRITE_REQ						0x100B0007			/**< pf_nma : serchable-vlans書込み要求		*/
#define D_SYS_MSGID_NMA_VLAN_WRITE_RSP						0x100B0008			/**< pf_nma : serchable-vlans書込み応答		*/

/* Message ID(pf_pfm) @ 5GDU */
																				/**< pf_pfm : 1secタイマ T.O.通知			*/
#define D_SYS_MSGID_PFM_1S_TIMEOUT_NTF						D_DU_MSGID_PFM_1S_TIMEOUT_NTF
#define D_SYS_MSGID_PFM_LOG_TIMEOUT_NTF						D_DU_MSGID_PFM_LOG_TIMEOUT_NTF


/* Message ID(pf_nmas) @ 5GDU */
#define D_SYS_MSGID_NMAS_FILE_WRITE_REQ						0x10160003			/**< pf_nmas : ファイル書込み要求			*/
#define D_SYS_MSGID_NMAS_FILE_WRITE_RSP						0x10160004			/**< pf_nmas : ファイル書込み応答			*/
#define D_SYS_MSGID_NMAS_LUTLOG_WRITE_NFT					0x10160005			/**< pf_nmas : LUT LOG書き込み通知			*/
/* @} */

/*!
 * @name common
 * @note common definition
 * @{
 */
/* ON/OFF */
#define	D_SYS_OFF											D_RRH_OFF			/**< OFF									*/
#define	D_SYS_ON											D_RRH_ON			/**< ON										*/
#define	D_SYS_ONOFF_NUM										2					/**< ON/OFF種別数							*/

/* result */
#define	D_SYS_OK											D_RRH_OK			/**< 正常終了								*/
#define	D_SYS_NG											D_RRH_NG			/**< 異常終了								*/
#define	D_SYS_MNG											D_RRH_MNG			/**< 異常終了(マイナス)						*/

/* @} */


/*!
 * @name アンテナキャリア関連定義
 * @note アンテナキャリア関連定義
 * @{
 */
/* アンテナ定義 */
#define D_SYS_ANTA											0					/* アンテナA								*/
#define D_SYS_ANTB											1					/* アンテナB								*/
#define D_SYS_ANTC											2					/* アンテナC								*/
#define D_SYS_ANTD											3					/* アンテナD								*/
#define D_SYS_ANT_BOTH										2					/* アンテナ(両系)							*/
#define D_SYS_ANT_MAX										D_RRH_ANT_MAX		/* アンテナ数								*/

/* アンテナ方向 */
#define D_SYS_ANT_UL										0					/* UL										*/
#define D_SYS_ANT_DL										1					/* DL										*/
#define D_SYS_ANT_DIRECTION_NUM								2					/* アンテナ方向数							*/

/* キャリア最大数*/
#define D_SYS_CAR_MAX										D_RRH_CAR_MAX		/* LTE Carrier MAX数						*/
#define D_SYS_CAR_3G_C0										0					/* 3G Carrier No(C0)						*/
#define D_SYS_CAR_3G_C1										1					/* 3G Carrier No(C1)						*/
#define D_SYS_CAR_3G_C2										2					/* 3G Carrier No(C2)						*/
#define D_SYS_CAR_3G_C3										3					/* 3G Carrier No(C3)						*/
#define D_SYS_CAR_3G_MAX									4					/* 3G Carrier MAX数							*/

/* bandwidth */
#define D_SYS_BW_NONE										0					/* bandwidth: 0M(SLC/1CHP用)				*/
#define D_SYS_BW_3M											3000				/* bandwidth: 3M							*/
#define D_SYS_BW_5M											5000				/* bandwidth: 5M							*/
#define D_SYS_BW_10M										10000				/* bandwidth: 10M							*/
#define D_SYS_BW_15M										15000				/* bandwidth: 15M							*/
#define D_SYS_BW_20M										20000				/* bandwidth: 20M							*/
#define D_SYS_BW_NUM										D_RRH_BW_NUM		/* bandwidth数								*/

/* 対応周波数番号 */
#define D_SYS_FREQ_15G_LTE_MIN								6475				/* 1.5G RRE LTE周波数番号MIN				*/
#define D_SYS_FREQ_15G_LTE_MAX								6575				/* 1.5G RRE LTE周波数番号MAX				*/
#define D_SYS_FREQ_17G_LTE_MIN								1774				/* 1.7G RRE LTE周波数番号MIN				*/
#define D_SYS_FREQ_17G_LTE_MAX								1924				/* 1.7G RRE LTE周波数番号MAX				*/
#define D_SYS_FREQ_700M_LTE_MIN								9485				/* 700M RRE LTE周波数番号MIN				*/
#define D_SYS_FREQ_700M_LTE_MAX								9535				/* 700M RRE LTE周波数番号MAX				*/

/* PA状態 */
#define D_SYS_PA_STATE_INI									0xFF				/* PA状態(初期状態)							*/

/* 位相補償 */
#define D_SYS_CAR_N_MAX_SUB6_30K                            27
#define D_SYS_CAR_N_MAX_MMW_120K                            55
#define D_SYS_CAR_N_MAX_MMW_240K                            111
    
#define D_SYS_CAR_SAMPLE_BASE0_SUB6_30K                     88
#define D_SYS_CAR_SAMPLE_BASE1_SUB6_30K                     15448
#define D_SYS_CAR_SAMPLE_INC_SUB6_30K                       1096
    
#define D_SYS_CAR_SAMPLE_BASE_MMW_120K                      34
#define D_SYS_CAR_SAMPLE_INC_MMW_120K                       274
    
#define D_SYS_CAR_SAMPLE_BASE_MMW_240K                      25
#define D_SYS_CAR_SAMPLE_INC_MMW_240K                       137

#define D_SYS_CAR_NCO_TABLE_SIZE_SUB6                       196608
#define D_SYS_CAR_NCO_TABLE_SIZE_MMW                        24576

#define D_SYS_CAR_TX_DPHASFTSEL_OFFSET                         0xa2014280
#define D_SYS_CAR_TX_DPHASFTENB_OFFSET                         0xa2014284
#define D_SYS_CAR_TX_DPCCOEF30KS0_OFFSET_SUB6                  0xa2015080
#define D_SYS_CAR_TX_DPCCOEF30KS1_OFFSET_SUB6                  0xa20150c0
#define D_SYS_CAR_TX_DPCCOEF30KS0_OFFSET_MMW_120K              0xa2015100
#define D_SYS_CAR_TX_DPCCOEF30KS1_OFFSET_MMW_120K              0xa2015140
#define D_SYS_CAR_TX_DPCCOEF30KS2_OFFSET_MMW_120K              0xa2015180
#define D_SYS_CAR_TX_DPCCOEF30KS3_OFFSET_MMW_120K              0xa20151c0
#define D_SYS_CAR_TX_DPCCOEF30KS0_OFFSET_MMW_240K              0xa2015200
#define D_SYS_CAR_TX_DPCCOEF30KS1_OFFSET_MMW_240K              0xa2015240
#define D_SYS_CAR_TX_DPCCOEF30KS2_OFFSET_MMW_240K              0xa2015280
#define D_SYS_CAR_TX_DPCCOEF30KS3_OFFSET_MMW_240K              0xa20152c0
#define D_SYS_CAR_TX_DPCCOEF30KS4_OFFSET_MMW_240K              0xa2015300
#define D_SYS_CAR_TX_DPCCOEF30KS5_OFFSET_MMW_240K              0xa2015340
#define D_SYS_CAR_TX_DPCCOEF30KS6_OFFSET_MMW_240K              0xa2015380
#define D_SYS_CAR_TX_DPCCOEF30KS7_OFFSET_MMW_240K              0xa20153c0

#define D_SYS_CAR_RX_DPHASFTSEL_OFFSET                         0xa2204280
#define D_SYS_CAR_RX_DPHASFTSEL_OFFSET_MMW                     0xa2204280
#define D_SYS_CAR_RX_DPHASFTENB_OFFSET                         0xa2204284
#define D_SYS_CAR_RX_DPHASFTENB_OFFSET_MMW                     0xa2204284
#define D_SYS_CAR_RX_DPCCOEF30KS0_OFFSET_SUB6                  0xa2205080
#define D_SYS_CAR_RX_DPCCOEF30KS1_OFFSET_SUB6                  0xa22050c0
#define D_SYS_CAR_RX_DPCCOEF30KS0_OFFSET_MMW_120K              0xa2204900
#define D_SYS_CAR_RX_DPCCOEF30KS1_OFFSET_MMW_120K              0xa2204940
#define D_SYS_CAR_RX_DPCCOEF30KS2_OFFSET_MMW_120K              0xa2204980
#define D_SYS_CAR_RX_DPCCOEF30KS3_OFFSET_MMW_120K              0xa22049C0
#define D_SYS_CAR_RX_DPCCOEF30KS0_OFFSET_MMW_240K              0xa2205200
#define D_SYS_CAR_RX_DPCCOEF30KS1_OFFSET_MMW_240K              0xa2205240
#define D_SYS_CAR_RX_DPCCOEF30KS2_OFFSET_MMW_240K              0xa2205280
#define D_SYS_CAR_RX_DPCCOEF30KS3_OFFSET_MMW_240K              0xa22052c0
#define D_SYS_CAR_RX_DPCCOEF30KS4_OFFSET_MMW_240K              0xa2205300
#define D_SYS_CAR_RX_DPCCOEF30KS5_OFFSET_MMW_240K              0xa2205340
#define D_SYS_CAR_RX_DPCCOEF30KS6_OFFSET_MMW_240K              0xa2205380
#define D_SYS_CAR_RX_DPCCOEF30KS7_OFFSET_MMW_240K              0xa22053c0

/* @} */

/*!
 * @name CPRI関連定義
 * @note CPRI関連定義
 * @{
 */
#define D_SYS_CPRI_PORT_NUM									D_RRH_CPRI_MAX		/* CPRI Port数								*/

/* @} */

/*!
 * @name 装置情報関連
 * @note 装置情報関連定義
 * @note baseとなる装置情報がenumであるため、ここも E_SYSで定義する
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
#define E_SYS_RRHKIND_15G_TRI                               E_RRH_RRHKIND_15G_TRI   /**< 1.5G対応 3セクタ */
#define E_SYS_RRHKIND_17G_TRI                               E_RRH_RRHKIND_17G_TRI   /**< 1.7G対応 3セクタ */
#define E_SYS_RRHKIND_700M                                  E_RRH_RRHKIND_700M      /**< 700M対応 */
#define E_SYS_RRHKIND_SRE_15G                               E_RRH_RRHKIND_SRE_15G   /**< 1.5G対応 SRE */
#define E_SYS_RRHKIND_SLC_2G                                E_RRH_RRHKIND_SLC_2G    /**< 2.0G対応 SLC-RRE */
#define E_SYS_RRHKIND_SLC_800M                              E_RRH_RRHKIND_SLC_800M  /**< 800M対応 SLC-RRE */
#define E_SYS_RRHKIND_1CHP_15G                              E_RRH_RRHKIND_1CHP_15G  /**< 1.5G対応 1chip-RRE */
#define E_SYS_RRHKIND_1CHP_17G                              E_RRH_RRHKIND_1CHP_17G  /**< 1.7G対応 1chip-RRE */
#define E_SYS_RRHKIND_SRE_35G                               E_RRH_RRHKIND_SRE_35G   /**< 3.5G対応 TDD-SRE */
#define E_SYS_RRHKIND_RRE_35G                               E_RRH_RRHKIND_RRE_35G   /**< 3.5G対応 TDD-RRE */
#define E_SYS_RRHKIND_NUM                                   E_RRH_RRHKIND_NUM       /**< 装置種別数 */
/* @} */

/*!
 * @name FLASH-READデータ配列数
 * @note FLASH-READデータ配列数
 * @{
 */
#define D_SYS_FLASH_READ_DATA								0x20000				/* FLASH-READ-data数						*/
/* @} */

/**
 * @name FPGA実装サイズ
 * @note FPGA実装サイズデフォルト値の定義
 */
/* @{ */
#define	D_SYS_FPGA_SIZE_1M					0x00100000				/*!< FPGA実装サイズ1M					*/
/* @} */

/*!
 * @name EXT receiving buffer size defintion
 * @note EXT receiving buffer size defintion
 * @{
 */
#define D_SYS_EXT_BUFSIZE1		960 		/**< Buffer size kind 1,change  (960)		*/
/* @} */

/** @name number definition */
/* @{ */

/****************************************************************************/
/* common item definition ( number definition )								*/
/****************************************************************************/
#define D_SYS_NUM_MINUS1		-1			/**< -1							*/
#define D_SYS_NUM0				0			/**< 0							*/
#define D_SYS_NUM1				1			/**< 1							*/
#define D_SYS_NUM2				2			/**< 2							*/
#define D_SYS_NUM3				3			/**< 3							*/
#define D_SYS_NUM4				4			/**< 4							*/
#define D_SYS_NUM5				5			/**< 5							*/
#define D_SYS_NUM6				6			/**< 6							*/
#define D_SYS_NUM7				7			/**< 7							*/
#define D_SYS_NUM8				8			/**< 8							*/
#define D_SYS_NUM9				9			/**< 9							*/
#define D_SYS_NUM10				10			/**< 10							*/
#define D_SYS_NUM11				11			/**< 11							*/
#define D_SYS_NUM12				12			/**< 12							*/
#define D_SYS_NUM13				13			/**< 13							*/
#define D_SYS_NUM14				14			/**< 14							*/
#define D_SYS_NUM15				15			/**< 15							*/
#define D_SYS_NUM16				16			/**< 16							*/
#define D_SYS_NUM17				17			/**< 17							*/
#define D_SYS_NUM18				18			/**< 18							*/
#define D_SYS_NUM19				19			/**< 19							*/
#define D_SYS_NUM20				20			/**< 20							*/
#define D_SYS_NUM21				21			/**< 21							*/
#define D_SYS_NUM22				22			/**< 22							*/
#define D_SYS_NUM23				23			/**< 23							*/
#define D_SYS_NUM24				24			/**< 24							*/
#define D_SYS_NUM25				25			/**< 25							*/
#define D_SYS_NUM26				26			/**< 26							*/
#define D_SYS_NUM27				27			/**< 27							*/
#define D_SYS_NUM28				28			/**< 28							*/
#define D_SYS_NUM29				29			/**< 29							*/
#define D_SYS_NUM30				30			/**< 30							*/
#define D_SYS_NUM31				31			/**< 31							*/
#define D_SYS_NUM32				32			/**< 32							*/
#define D_SYS_NUM33				33			/**< 33							*/
#define D_SYS_NUM36				36			/**< 36							*/
#define D_SYS_NUM48				48			/**< 48							*/
#define D_SYS_NUM50				50			/**< 50							*/
#define D_SYS_NUM64				64			/**< 64							*/
#define D_SYS_NUM60				60			/**< 60		@800M				*/
#define D_SYS_NUM74				74			/**< 74							*/
#define D_SYS_NUM76				76			/**< 76							*/
#define D_SYS_NUM100			100			/**< 100						*/
#define D_SYS_NUM128			128			/**< 128						*/
#define D_SYS_NUM152			152			/**< 152						*/
#define D_SYS_NUM200			200			/**< 200						*/
#define D_SYS_NUM255			255			/**< 255						*/
#define D_SYS_NUM256			256			/**< 256						*/
#define D_SYS_NUM300			300			/**< 300						*/
#define D_SYS_NUM480			480			/**< 480						*/
#define D_SYS_NUM512			512			/**< 512						*/
#define D_SYS_NUM1000			1000		/**< 1000						*/
#define D_SYS_NUM1024			1024		/**< 1024						*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_SYS_NUM1200			1200		/**< 1200						*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define D_SYS_NUM2048			2048		/**< 2048						*/
#define D_SYS_NUM4096			4096		/**< 4096						*/
#define D_SYS_NUM12500			12500		/**< 12500						*/
#define D_SYS_NUM_10MS			100000		/**< 100000						*/

#define D_SYS_NUM_0x104			0x104		/**< 0x104						*/
#define D_SYS_NUM_0x108			0x108		/**< 0x108						*/
#define D_SYS_NUM_0x119			0x119		/**< 0x119						*/
#define D_SYS_NUM_0x11C			0x11C		/**< 0x11C						*/
#define D_SYS_NUM_0x138			0x138		/**< 0x138						*/
#define D_SYS_NUM_0x13C			0x13C		/**< 0x13C						*/
#define D_SYS_NUM_0x170			0x170		/**< 0x170						*/
#define D_SYS_NUM_0x1FF			0x1FF		/**< 0x1FF						*/
#define D_SYS_NUM_0x1A0			0x1A0		/**< 0x1A0						*/
#define D_SYS_NUM_0x200			0x200		/**< 0x200						*/
#define D_SYS_NUM_0x2000		0x2000		/**< 0x2000						*/
#define D_SYS_NUM_0x10000		0x10000		/**< 0x10000					*/
#define D_SYS_NUM_0x80			0x80		/**< 0x80						*/
#define D_SYS_NUM_0x800			0x800		/**< 0x800						*/
#define D_SYS_NUM_0x8000		0x8000		/**< 0x8000						*/
#define D_SYS_NUMAA				0xAA		/**< 0xAA						*/
#define D_SYS_NUMFF				0xFF		/**< 0xFF                         */
#define D_SYS_ALLF_SHORT		0xFFFF		/**< 0x8000						*/
#define D_SYS_ALLF_INT			0xFFFFFFFF	/**< 0x8000						*/

/* @} */

/** @name NULL definition */
/* @{ */

#ifndef NULL
#define NULL					0			/**< NULL						*/
#endif

/* ======================================================================== */
/* Connected system number for shared-RE									*/
/* ======================================================================== */

#define D_SYS_MAX_SYSNUM			2		/**< Max connected system number for shared-RE */
#define D_SYS_3G					0		/**< connected system is 3G */
#define D_SYS_S3G					1		/**< connected system is S3G */
#define D_SYS_BOTH_3G_S3G			2		/**< connected system is both 3G and S3G */
       
#define D_SYS_SUB6					17		/**< connected system is SUB6 */
#define D_SYS_MMW					18		/**< connected system is MMW */
#define D_SYS_BOTH_SUB6_MMW			19		/**< connected system is both SUB6 and MMW */

#define D_SYS_MSK_CPRID				0xFFF0	/**< Mask the system information in CPRI Signal ID */
#define D_SYS_CHK_CPRID				0x0001	/**< Check the system infromn in CPRI signal ID */

/* @} */


/*!
 * @brief SV Ctrl operation type
 * @date  2015/09/03 TDI)satou FHM用の定義に全面見直し
 * @date  2020/12/28 M&C) Merge 4G FHM src
 */
enum
{
	E_SYS_SVCTL_ERR = 0,                    /**< CNTM_SV1INF, SV0情報, ERR */
	E_SYS_SVCTL_ALM,                        /**< CNTM_SV1INF, SV0情報, ALM */
	E_SYS_SVCTL_USE,                        /**< CNTM_SV1INF, SV0情報, USE */
	E_SYS_SVCTL_ACT,                        /**< CNTM_SV1INF, SV1情報, ACT */
	E_SYS_SVCTL_NOINS,                      /**< CNTM_SV1INF, SV1情報, NOINS */
	E_SYS_SVCTL_INS,                        /**< CNTM_SV1INF, SV1情報, INS */
	E_SYS_SVCTL_S3G_USE_01,                 /**< CNTM_SV1INF, FHMキャリア状態, S3G-USE(C#1) */
	E_SYS_SVCTL_S3G_USE_02,                 /**< CNTM_SV1INF, FHMキャリア状態, S3G-USE(C#2) */
	E_SYS_SVCTL_S3G_USE_03,                 /**< CNTM_SV1INF, FHMキャリア状態, S3G-USE(C#3) */
	E_SYS_SVCTL_S3G_USE_04,                 /**< CNTM_SV1INF, FHMキャリア状態, S3G-USE(C#4) */
	E_SYS_SVCTL_3G_USE_01,                  /**< CNTM_SV1INF, FHMキャリア状態, 3G-USE(C#1) */
	E_SYS_SVCTL_3G_USE_02,                  /**< CNTM_SV1INF, FHMキャリア状態, 3G-USE(C#2) */
	E_SYS_SVCTL_3G_USE_03,                  /**< CNTM_SV1INF, FHMキャリア状態, 3G-USE(C#3) */
	E_SYS_SVCTL_3G_USE_04,                  /**< CNTM_SV1INF, FHMキャリア状態, 3G-USE(C#4) */
	E_SYS_SVCTL_PLL_ALM,                    /**< CNTM_SV1INF, FHM ALM情報, PLL ALM */
	E_SYS_SVCTL_CPRI_ALM,                   /**< CNTM_SV1INF, FHM ALM情報, CPRI ALM */
	E_SYS_SVCTL_COM_ALM,                    /**< CNTM_SV1INF, FHM ALM情報, COM ALM */
	E_SYS_SVCTL_SOFT_ALM,                   /**< CNTM_SV1INF, FHM ALM情報, SOFT ALM */
	E_SYS_SVCTL_SOFT_ALM2,                  /**< CNTM_SV1INF, FHM ALM情報, SOFT ALM2 */

	E_SYS_SVCTL_CPRI_ALM_01,                /**< CNTM_SV2INF, Master CPRI ALM(＃01～08), CPRI ALM #01 */
	E_SYS_SVCTL_CPRI_ALM_02,                /**< CNTM_SV2INF, Master CPRI ALM(＃01～08), CPRI ALM #02 */
	E_SYS_SVCTL_CPRI_ALM_03,                /**< CNTM_SV2INF, Master CPRI ALM(＃01～08), CPRI ALM #03 */
	E_SYS_SVCTL_CPRI_ALM_04,                /**< CNTM_SV2INF, Master CPRI ALM(＃01～08), CPRI ALM #04 */
	E_SYS_SVCTL_CPRI_ALM_05,                /**< CNTM_SV2INF, Master CPRI ALM(＃01～08), CPRI ALM #05 */
	E_SYS_SVCTL_CPRI_ALM_06,                /**< CNTM_SV2INF, Master CPRI ALM(＃01～08), CPRI ALM #06 */
	E_SYS_SVCTL_CPRI_ALM_07,                /**< CNTM_SV2INF, Master CPRI ALM(＃01～08), CPRI ALM #07 */
	E_SYS_SVCTL_CPRI_ALM_08,                /**< CNTM_SV2INF, Master CPRI ALM(＃01～08), CPRI ALM #08 */
	E_SYS_SVCTL_CPRI_ALM_09,                /**< CNTM_SV2INF, Master CPRI ALM(＃09～16), CPRI ALM #09 */
	E_SYS_SVCTL_CPRI_ALM_10,                /**< CNTM_SV2INF, Master CPRI ALM(＃09～16), CPRI ALM #10 */
	E_SYS_SVCTL_CPRI_ALM_11,                /**< CNTM_SV2INF, Master CPRI ALM(＃09～16), CPRI ALM #11 */
	E_SYS_SVCTL_CPRI_ALM_12,                /**< CNTM_SV2INF, Master CPRI ALM(＃09～16), CPRI ALM #12 */
	E_SYS_SVCTL_CPRI_ALM_13,                /**< CNTM_SV2INF, Master CPRI ALM(＃09～16), CPRI ALM #13 */
	E_SYS_SVCTL_CPRI_ALM_14,                /**< CNTM_SV2INF, Master CPRI ALM(＃09～16), CPRI ALM #14 */
	E_SYS_SVCTL_CPRI_ALM_15,                /**< CNTM_SV2INF, Master CPRI ALM(＃09～16), CPRI ALM #15 */
	E_SYS_SVCTL_CPRI_ALM_16,                /**< CNTM_SV2INF, Master CPRI ALM(＃09～16), CPRI ALM #16 */

	E_SYS_SVCTL_3G_FREQ_CMPLX_ALM,			/**< CNTM_SV7INF, FHM Soft ALM2情報, 3G-FREQ COMPLX ALM */
	E_SYS_SVCTL_3G_CARR_NUM_ALM,			/**< CNTM_SV7INF, FHM Soft ALM2情報, 3G-CARR NUM ALM */
	E_SYS_SVCTL_3G_UP_RESRC_NUM_ALM,		/**< CNTM_SV7INF, FHM Soft ALM2情報, 3G-UP-RESRC NUM ALM */
	E_SYS_SVCTL_3G_DWN_RESRC_NUM_ALM,		/**< CNTM_SV7INF, FHM Soft ALM2情報, 3G-DWN-RESRC NUM ALM */
	E_SYS_SVCTL_S3G_SHARETYPE_ALM,			/**< CNTM_SV7INF, FHM Soft ALM2情報, S3G-SHARE TYPE ALM */
	E_SYS_SVCTL_S3G_FREQ_CMPLX_ALM,			/**< CNTM_SV7INF, FHM Soft ALM2情報, S3G-FREQ CMPLX ALM */
	E_SYS_SVCTL_S3G_BAND_WIDTH_ALM,			/**< CNTM_SV7INF, FHM Soft ALM2情報, S3G-BAND WIDTH ALM */
	E_SYS_SVCTL_S3G_CARR_NUMALM,			/**< CNTM_SV7INF, FHM Soft ALM2情報, S3G-CARR NUM ALM */

	E_SYS_SVCTL_RE01_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#01～04), RE#01 OPT STS */
	E_SYS_SVCTL_RE01_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#01～04), RE#01 WAVE STS */
	E_SYS_SVCTL_RE02_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#01～04), RE#02 OPT STS */
	E_SYS_SVCTL_RE02_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#01～04), RE#02 WAVE STS */
	E_SYS_SVCTL_RE03_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#01～04), RE#03 OPT STS */
	E_SYS_SVCTL_RE03_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#01～04), RE#03 WAVE STS */
	E_SYS_SVCTL_RE04_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#01～04), RE#04 OPT STS */
	E_SYS_SVCTL_RE04_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#01～04), RE#04 WAVE STS */
	E_SYS_SVCTL_RE05_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#05～08), RE#05 OPT STS */
	E_SYS_SVCTL_RE05_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#05～08), RE#05 WAVE STS */
	E_SYS_SVCTL_RE06_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#05～08), RE#06 OPT STS */
	E_SYS_SVCTL_RE06_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#05～08), RE#06 WAVE STS */
	E_SYS_SVCTL_RE07_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#05～08), RE#07 OPT STS */
	E_SYS_SVCTL_RE07_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#05～08), RE#07 WAVE STS */
	E_SYS_SVCTL_RE08_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#05～08), RE#08 OPT STS */
	E_SYS_SVCTL_RE08_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#05～08), RE#08 WAVE STS */
	E_SYS_SVCTL_RE09_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#09～12), RE#09 OPT STS */
	E_SYS_SVCTL_RE09_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#09～12), RE#09 WAVE STS */
	E_SYS_SVCTL_RE10_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#09～12), RE#10 OPT STS */
	E_SYS_SVCTL_RE10_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#09～12), RE#10 WAVE STS */
	E_SYS_SVCTL_RE11_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#09～12), RE#11 OPT STS */
	E_SYS_SVCTL_RE11_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#09～12), RE#11 WAVE STS */
	E_SYS_SVCTL_RE12_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#09～12), RE#12 OPT STS */
	E_SYS_SVCTL_RE12_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#09～12), RE#12 WAVE STS */
	E_SYS_SVCTL_RE13_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#13～16), RE#13 OPT STS */
	E_SYS_SVCTL_RE13_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#13～16), RE#13 WAVE STS */
	E_SYS_SVCTL_RE14_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#13～16), RE#14 OPT STS */
	E_SYS_SVCTL_RE14_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#13～16), RE#14 WAVE STS */
	E_SYS_SVCTL_RE15_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#13～16), RE#15 OPT STS */
	E_SYS_SVCTL_RE15_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#13～16), RE#15 WAVE STS */
	E_SYS_SVCTL_RE16_OPT_STS,               /**< CNTM_SV8INF, Master CPRI 状態(CPRI#13～16), RE#16 OPT STS */
	E_SYS_SVCTL_RE16_WAVE_STS,              /**< CNTM_SV8INF, Master CPRI 状態(CPRI#13～16), RE#16 WAVE STS */

	E_SYS_SVCTL_L1ERR,                      /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#01~08), L1ERR */
	E_SYS_SVCTL_L2ERR,                      /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#01~08), L2ERR */
	E_SYS_SVCTL_3G_L3ERR,                   /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#01~08), 3G-L3ERR */
	E_SYS_SVCTL_S3G_L3ERR,                  /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#01~08), S3G-L3ERR */
	E_SYS_SVCTL_SFP_ERR1,                   /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#01~08), SFP ERR1 */
	E_SYS_SVCTL_SFP_ERR2,                   /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#01~08), SFP ERR2 */
	E_SYS_SVCTL_DELAY_ERR,                  /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#09~16), DERAY ERR */
	E_SYS_SVCTL_DELAY_ERR_S3G,              /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#09~16), DELAY ERR */
	E_SYS_SVCTL_SYSPARA_DL_ERR,             /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#09~16), SYSPARA DL ERR */
	E_SYS_SVCTL_FIRM_DL_ERR,                /**< CNTM_SV9INF, FHM&SlaveCPRI ERR情報(ERR#09~16), FIRM DL ERR */
	E_SYS_SVCTL_L1_ERR_01,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#17-24), L1 ERR #01 */
	E_SYS_SVCTL_L1_ERR_02,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#17-24), L1 ERR #02 */
	E_SYS_SVCTL_L1_ERR_03,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#17-24), L1 ERR #03 */
	E_SYS_SVCTL_L1_ERR_04,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#17-24), L1 ERR #04 */
	E_SYS_SVCTL_L1_ERR_05,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#17-24), L1 ERR #05 */
	E_SYS_SVCTL_L1_ERR_06,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#17-24), L1 ERR #06 */
	E_SYS_SVCTL_L1_ERR_07,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#17-24), L1 ERR #07 */
	E_SYS_SVCTL_L1_ERR_08,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#17-24), L1 ERR #08 */
	E_SYS_SVCTL_L1_ERR_09,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#25-32), L1 ERR #09 */
	E_SYS_SVCTL_L1_ERR_10,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#25-32), L1 ERR #10 */
	E_SYS_SVCTL_L1_ERR_11,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#25-32), L1 ERR #11 */
	E_SYS_SVCTL_L1_ERR_12,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#25-32), L1 ERR #12 */
	E_SYS_SVCTL_L1_ERR_13,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#25-32), L1 ERR #13 */
	E_SYS_SVCTL_L1_ERR_14,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#25-32), L1 ERR #14 */
	E_SYS_SVCTL_L1_ERR_15,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#25-32), L1 ERR #15 */
	E_SYS_SVCTL_L1_ERR_16,                  /**< CNTM_SV9INF, RE&MasterCPRI ERR情報(ERR#25-32), L1 ERR #16 */

	E_SYS_SVCTL_L2_ERR_01,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#33~40), L2 ERR #01 */
	E_SYS_SVCTL_L2_ERR_02,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#33~40), L2 ERR #02 */
	E_SYS_SVCTL_L2_ERR_03,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#33~40), L2 ERR #03 */
	E_SYS_SVCTL_L2_ERR_04,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#33~40), L2 ERR #04 */
	E_SYS_SVCTL_L2_ERR_05,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#33~40), L2 ERR #05 */
	E_SYS_SVCTL_L2_ERR_06,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#33~40), L2 ERR #06 */
	E_SYS_SVCTL_L2_ERR_07,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#33~40), L2 ERR #07 */
	E_SYS_SVCTL_L2_ERR_08,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#33~40), L2 ERR #08 */
	E_SYS_SVCTL_L2_ERR_09,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#41~48), L2 ERR #09 */
	E_SYS_SVCTL_L2_ERR_10,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#41~48), L2 ERR #10 */
	E_SYS_SVCTL_L2_ERR_11,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#41~48), L2 ERR #11 */
	E_SYS_SVCTL_L2_ERR_12,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#41~48), L2 ERR #12 */
	E_SYS_SVCTL_L2_ERR_13,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#41~48), L2 ERR #13 */
	E_SYS_SVCTL_L2_ERR_14,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#41~48), L2 ERR #14 */
	E_SYS_SVCTL_L2_ERR_15,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#41~48), L2 ERR #15 */
	E_SYS_SVCTL_L2_ERR_16,                  /**< CNTM_SV10INF, RE&MasterCPRI ERR#02情報(ERR#41~48), L2 ERR #16 */
	E_SYS_SVCTL_3G_L3_ERR_01,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#49~56), 3G-L3 ERR #01 */
	E_SYS_SVCTL_3G_L3_ERR_02,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#49~56), 3G-L3 ERR #02 */
	E_SYS_SVCTL_3G_L3_ERR_03,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#49~56), 3G-L3 ERR #03 */
	E_SYS_SVCTL_3G_L3_ERR_04,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#49~56), 3G-L3 ERR #04 */
	E_SYS_SVCTL_3G_L3_ERR_05,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#49~56), 3G-L3 ERR #05 */
	E_SYS_SVCTL_3G_L3_ERR_06,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#49~56), 3G-L3 ERR #06 */
	E_SYS_SVCTL_3G_L3_ERR_07,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#49~56), 3G-L3 ERR #07 */
	E_SYS_SVCTL_3G_L3_ERR_08,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#49~56), 3G-L3 ERR #08 */
	E_SYS_SVCTL_3G_L3_ERR_09,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#57~64), 3G-L3 ERR #09 */
	E_SYS_SVCTL_3G_L3_ERR_10,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#57~64), 3G-L3 ERR #10 */
	E_SYS_SVCTL_3G_L3_ERR_11,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#57~64), 3G-L3 ERR #11 */
	E_SYS_SVCTL_3G_L3_ERR_12,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#57~64), 3G-L3 ERR #12 */
	E_SYS_SVCTL_3G_L3_ERR_13,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#57~64), 3G-L3 ERR #13 */
	E_SYS_SVCTL_3G_L3_ERR_14,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#57~64), 3G-L3 ERR #14 */
	E_SYS_SVCTL_3G_L3_ERR_15,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#57~64), 3G-L3 ERR #15 */
	E_SYS_SVCTL_3G_L3_ERR_16,               /**< CNTM_SV10INF, RE&MasterCPRI ERR#03情報(ERR#57~64), 3G-L3 ERR #16 */

	E_SYS_SVCTL_S3G_L3_ERR_01,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#65~72), S3G-L3 ERR #01 */
	E_SYS_SVCTL_S3G_L3_ERR_02,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#65~72), S3G-L3 ERR #02 */
	E_SYS_SVCTL_S3G_L3_ERR_03,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#65~72), S3G-L3 ERR #03 */
	E_SYS_SVCTL_S3G_L3_ERR_04,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#65~72), S3G-L3 ERR #04 */
	E_SYS_SVCTL_S3G_L3_ERR_05,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#65~72), S3G-L3 ERR #05 */
	E_SYS_SVCTL_S3G_L3_ERR_06,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#65~72), S3G-L3 ERR #06 */
	E_SYS_SVCTL_S3G_L3_ERR_07,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#65~72), S3G-L3 ERR #07 */
	E_SYS_SVCTL_S3G_L3_ERR_08,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#65~72), S3G-L3 ERR #08 */
	E_SYS_SVCTL_S3G_L3_ERR_09,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#73~80), S3G-L3 ERR #09 */
	E_SYS_SVCTL_S3G_L3_ERR_10,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#73~80), S3G-L3 ERR #10 */
	E_SYS_SVCTL_S3G_L3_ERR_11,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#73~80), S3G-L3 ERR #11 */
	E_SYS_SVCTL_S3G_L3_ERR_12,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#73~80), S3G-L3 ERR #12 */
	E_SYS_SVCTL_S3G_L3_ERR_13,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#73~80), S3G-L3 ERR #13 */
	E_SYS_SVCTL_S3G_L3_ERR_14,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#73~80), S3G-L3 ERR #14 */
	E_SYS_SVCTL_S3G_L3_ERR_15,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#73~80), S3G-L3 ERR #15 */
	E_SYS_SVCTL_S3G_L3_ERR_16,              /**< CNTM_SV11INF, RE&MasterCPRI ERR#04情報(ERR#73~80), S3G-L3 ERR #16 */
	E_SYS_SVCTL_SFP_ERR1_01,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#81~88), SFP ERR1 #01 */
	E_SYS_SVCTL_SFP_ERR1_02,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#81~88), SFP ERR1 #02 */
	E_SYS_SVCTL_SFP_ERR1_03,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#81~88), SFP ERR1 #03 */
	E_SYS_SVCTL_SFP_ERR1_04,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#81~88), SFP ERR1 #04 */
	E_SYS_SVCTL_SFP_ERR1_05,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#81~88), SFP ERR1 #05 */
	E_SYS_SVCTL_SFP_ERR1_06,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#81~88), SFP ERR1 #06 */
	E_SYS_SVCTL_SFP_ERR1_07,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#81~88), SFP ERR1 #07 */
	E_SYS_SVCTL_SFP_ERR1_08,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#81~88), SFP ERR1 #08 */
	E_SYS_SVCTL_SFP_ERR1_09,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#89~96), SFP ERR1 #09 */
	E_SYS_SVCTL_SFP_ERR1_10,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#89~96), SFP ERR1 #10 */
	E_SYS_SVCTL_SFP_ERR1_11,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#89~96), SFP ERR1 #11 */
	E_SYS_SVCTL_SFP_ERR1_12,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#89~96), SFP ERR1 #12 */
	E_SYS_SVCTL_SFP_ERR1_13,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#89~96), SFP ERR1 #13 */
	E_SYS_SVCTL_SFP_ERR1_14,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#89~96), SFP ERR1 #14 */
	E_SYS_SVCTL_SFP_ERR1_15,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#89~96), SFP ERR1 #15 */
	E_SYS_SVCTL_SFP_ERR1_16,                /**< CNTM_SV11INF, RE&MasterCPRI ERR#05情報(ERR#89~96), SFP ERR1 #16 */

	E_SYS_SVCTL_SFP_ERR2_01,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#97~104), SFP ERR2 #01 */
	E_SYS_SVCTL_SFP_ERR2_02,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#97~104), SFP ERR2 #02 */
	E_SYS_SVCTL_SFP_ERR2_03,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#97~104), SFP ERR2 #03 */
	E_SYS_SVCTL_SFP_ERR2_04,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#97~104), SFP ERR2 #04 */
	E_SYS_SVCTL_SFP_ERR2_05,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#97~104), SFP ERR2 #05 */
	E_SYS_SVCTL_SFP_ERR2_06,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#97~104), SFP ERR2 #06 */
	E_SYS_SVCTL_SFP_ERR2_07,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#97~104), SFP ERR2 #07 */
	E_SYS_SVCTL_SFP_ERR2_08,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#97~104), SFP ERR2 #08 */
	E_SYS_SVCTL_SFP_ERR2_09,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#105~112), SFP ERR2 #09 */
	E_SYS_SVCTL_SFP_ERR2_10,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#105~112), SFP ERR2 #10 */
	E_SYS_SVCTL_SFP_ERR2_11,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#105~112), SFP ERR2 #11 */
	E_SYS_SVCTL_SFP_ERR2_12,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#105~112), SFP ERR2 #12 */
	E_SYS_SVCTL_SFP_ERR2_13,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#105~112), SFP ERR2 #13 */
	E_SYS_SVCTL_SFP_ERR2_14,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#105~112), SFP ERR2 #14 */
	E_SYS_SVCTL_SFP_ERR2_15,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#105~112), SFP ERR2 #15 */
	E_SYS_SVCTL_SFP_ERR2_16,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#06情報(ERR#105~112), SFP ERR2 #16 */
//16B
	E_SYS_SVCTL_SLOT_ERR_01,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SLOT ERR #01 */
	E_SYS_SVCTL_SLOT_ERR_02,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SLOT ERR #02 */
	E_SYS_SVCTL_SLOT_ERR_03,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SLOT ERR #03 */
	E_SYS_SVCTL_SLOT_ERR_04,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SLOT ERR #04 */
	E_SYS_SVCTL_SLOT_ERR_05,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SLOT ERR #05 */
	E_SYS_SVCTL_SLOT_ERR_06,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SLOT ERR #06 */
	E_SYS_SVCTL_SLOT_ERR_07,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SLOT ERR #07 */
	E_SYS_SVCTL_SLOT_ERR_08,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SLOT ERR #08 */
	E_SYS_SVCTL_SLOT_ERR_09,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SLOT ERR #09 */
	E_SYS_SVCTL_SLOT_ERR_10,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SLOT ERR #10 */
	E_SYS_SVCTL_SLOT_ERR_11,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SLOT ERR #11 */
	E_SYS_SVCTL_SLOT_ERR_12,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SLOT ERR #12 */
	E_SYS_SVCTL_SLOT_ERR_13,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SLOT ERR #13 */
	E_SYS_SVCTL_SLOT_ERR_14,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SLOT ERR #14 */
	E_SYS_SVCTL_SLOT_ERR_15,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SLOT ERR #15 */
	E_SYS_SVCTL_SLOT_ERR_16,               /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SLOT ERR #16 */
//	E_SYS_SVCTL_SFP_ERR3_01,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SFP ERR3 #01 */
//	E_SYS_SVCTL_SFP_ERR3_02,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SFP ERR3 #02 */
//	E_SYS_SVCTL_SFP_ERR3_03,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SFP ERR3 #03 */
//	E_SYS_SVCTL_SFP_ERR3_04,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SFP ERR3 #04 */
//	E_SYS_SVCTL_SFP_ERR3_05,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SFP ERR3 #05 */
//	E_SYS_SVCTL_SFP_ERR3_06,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SFP ERR3 #06 */
//	E_SYS_SVCTL_SFP_ERR3_07,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SFP ERR3 #07 */
//	E_SYS_SVCTL_SFP_ERR3_08,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#113~120), SFP ERR3 #08 */
//	E_SYS_SVCTL_SFP_ERR3_09,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SFP ERR3 #09 */
//	E_SYS_SVCTL_SFP_ERR3_10,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SFP ERR3 #10 */
//	E_SYS_SVCTL_SFP_ERR3_11,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SFP ERR3 #11 */
//	E_SYS_SVCTL_SFP_ERR3_12,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SFP ERR3 #12 */
//	E_SYS_SVCTL_SFP_ERR3_13,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SFP ERR3 #13 */
//	E_SYS_SVCTL_SFP_ERR3_14,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SFP ERR3 #14 */
//	E_SYS_SVCTL_SFP_ERR3_15,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SFP ERR3 #15 */
//	E_SYS_SVCTL_SFP_ERR3_16,                /**< CNTM_SV12INF, RE&MasterCPRI ERR#07情報(ERR#121~128), SFP ERR3 #16 */
//16B
	E_SYS_SVCTL_DELAY_ERR1_01,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#129~136), DELAY ERR1 #01 */
	E_SYS_SVCTL_DELAY_ERR1_02,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#129~136), DELAY ERR1 #02 */
	E_SYS_SVCTL_DELAY_ERR1_03,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#129~136), DELAY ERR1 #03 */
	E_SYS_SVCTL_DELAY_ERR1_04,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#129~136), DELAY ERR1 #04 */
	E_SYS_SVCTL_DELAY_ERR1_05,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#129~136), DELAY ERR1 #05 */
	E_SYS_SVCTL_DELAY_ERR1_06,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#129~136), DELAY ERR1 #06 */
	E_SYS_SVCTL_DELAY_ERR1_07,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#129~136), DELAY ERR1 #07 */
	E_SYS_SVCTL_DELAY_ERR1_08,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#129~136), DELAY ERR1 #08 */
	E_SYS_SVCTL_DELAY_ERR1_09,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#137~144), DELAY ERR1 #09 */
	E_SYS_SVCTL_DELAY_ERR1_10,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#137~144), DELAY ERR1 #10 */
	E_SYS_SVCTL_DELAY_ERR1_11,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#137~144), DELAY ERR1 #11 */
	E_SYS_SVCTL_DELAY_ERR1_12,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#137~144), DELAY ERR1 #12 */
	E_SYS_SVCTL_DELAY_ERR1_13,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#137~144), DELAY ERR1 #13 */
	E_SYS_SVCTL_DELAY_ERR1_14,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#137~144), DELAY ERR1 #14 */
	E_SYS_SVCTL_DELAY_ERR1_15,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#137~144), DELAY ERR1 #15 */
	E_SYS_SVCTL_DELAY_ERR1_16,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#08情報(ERR#137~144), DELAY ERR1 #16 */
	E_SYS_SVCTL_DELAY_ERR2_01,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#145~152), DELAY ERR2 #01 */
	E_SYS_SVCTL_DELAY_ERR2_02,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#145~152), DELAY ERR2 #02 */
	E_SYS_SVCTL_DELAY_ERR2_03,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#145~152), DELAY ERR2 #03 */
	E_SYS_SVCTL_DELAY_ERR2_04,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#145~152), DELAY ERR2 #04 */
	E_SYS_SVCTL_DELAY_ERR2_05,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#145~152), DELAY ERR2 #05 */
	E_SYS_SVCTL_DELAY_ERR2_06,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#145~152), DELAY ERR2 #06 */
	E_SYS_SVCTL_DELAY_ERR2_07,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#145~152), DELAY ERR2 #07 */
	E_SYS_SVCTL_DELAY_ERR2_08,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#145~152), DELAY ERR2 #08 */
	E_SYS_SVCTL_DELAY_ERR2_09,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#153~160), DELAY ERR2 #09 */
	E_SYS_SVCTL_DELAY_ERR2_10,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#153~160), DELAY ERR2 #10 */
	E_SYS_SVCTL_DELAY_ERR2_11,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#153~160), DELAY ERR2 #11 */
	E_SYS_SVCTL_DELAY_ERR2_12,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#153~160), DELAY ERR2 #12 */
	E_SYS_SVCTL_DELAY_ERR2_13,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#153~160), DELAY ERR2 #13 */
	E_SYS_SVCTL_DELAY_ERR2_14,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#153~160), DELAY ERR2 #14 */
	E_SYS_SVCTL_DELAY_ERR2_15,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#153~160), DELAY ERR2 #15 */
	E_SYS_SVCTL_DELAY_ERR2_16,              /**< CNTM_SV13INF, RE&MasterCPRI ERR#09情報(ERR#153~160), DELAY ERR2 #16 */

	E_SYS_SVCTL_STARTUP_ERR1_01,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#161~168), RE STARTUP ERR1 #01 */
	E_SYS_SVCTL_STARTUP_ERR1_02,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#161~168), RE STARTUP ERR1 #02 */
	E_SYS_SVCTL_STARTUP_ERR1_03,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#161~168), RE STARTUP ERR1 #03 */
	E_SYS_SVCTL_STARTUP_ERR1_04,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#161~168), RE STARTUP ERR1 #04 */
	E_SYS_SVCTL_STARTUP_ERR1_05,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#161~168), RE STARTUP ERR1 #05 */
	E_SYS_SVCTL_STARTUP_ERR1_06,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#161~168), RE STARTUP ERR1 #06 */
	E_SYS_SVCTL_STARTUP_ERR1_07,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#161~168), RE STARTUP ERR1 #07 */
	E_SYS_SVCTL_STARTUP_ERR1_08,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#161~168), RE STARTUP ERR1 #08 */
	E_SYS_SVCTL_STARTUP_ERR1_09,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#169~176), RE STARTUP ERR1 #09 */
	E_SYS_SVCTL_STARTUP_ERR1_10,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#169~176), RE STARTUP ERR1 #10 */
	E_SYS_SVCTL_STARTUP_ERR1_11,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#169~176), RE STARTUP ERR1 #11 */
	E_SYS_SVCTL_STARTUP_ERR1_12,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#169~176), RE STARTUP ERR1 #12 */
	E_SYS_SVCTL_STARTUP_ERR1_13,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#169~176), RE STARTUP ERR1 #13 */
	E_SYS_SVCTL_STARTUP_ERR1_14,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#169~176), RE STARTUP ERR1 #14 */
	E_SYS_SVCTL_STARTUP_ERR1_15,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#169~176), RE STARTUP ERR1 #15 */
	E_SYS_SVCTL_STARTUP_ERR1_16,            /**< CNTM_SV14INF, RE&MasterCPRI ERR#10情報(ERR#169~176), RE STARTUP ERR1 #16 */
	E_SYS_SVCTL_DL_NG_ERR_01,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#177~184), DL NG ERR #01 */
	E_SYS_SVCTL_DL_NG_ERR_02,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#177~184), DL NG ERR #02 */
	E_SYS_SVCTL_DL_NG_ERR_03,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#177~184), DL NG ERR #03 */
	E_SYS_SVCTL_DL_NG_ERR_04,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#177~184), DL NG ERR #04 */
	E_SYS_SVCTL_DL_NG_ERR_05,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#177~184), DL NG ERR #05 */
	E_SYS_SVCTL_DL_NG_ERR_06,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#177~184), DL NG ERR #06 */
	E_SYS_SVCTL_DL_NG_ERR_07,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#177~184), DL NG ERR #07 */
	E_SYS_SVCTL_DL_NG_ERR_08,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#177~184), DL NG ERR #08 */
	E_SYS_SVCTL_DL_NG_ERR_09,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#185~192), DL NG ERR #09 */
	E_SYS_SVCTL_DL_NG_ERR_10,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#185~192), DL NG ERR #10 */
	E_SYS_SVCTL_DL_NG_ERR_11,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#185~192), DL NG ERR #11 */
	E_SYS_SVCTL_DL_NG_ERR_12,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#185~192), DL NG ERR #12 */
	E_SYS_SVCTL_DL_NG_ERR_13,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#185~192), DL NG ERR #13 */
	E_SYS_SVCTL_DL_NG_ERR_14,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#185~192), DL NG ERR #14 */
	E_SYS_SVCTL_DL_NG_ERR_15,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#185~192), DL NG ERR #15 */
	E_SYS_SVCTL_DL_NG_ERR_16,               /**< CNTM_SV14INF, RE&MasterCPRI ERR#11情報(ERR#185~192), DL NG ERR #16 */

	E_SYS_SVCTL_3G_CARRRESET_ERR_01,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#193~200), 3G-CARR RESET ERR #01 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_02,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#193~200), 3G-CARR RESET ERR #02 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_03,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#193~200), 3G-CARR RESET ERR #03 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_04,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#193~200), 3G-CARR RESET ERR #04 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_05,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#193~200), 3G-CARR RESET ERR #05 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_06,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#193~200), 3G-CARR RESET ERR #06 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_07,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#193~200), 3G-CARR RESET ERR #07 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_08,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#193~200), 3G-CARR RESET ERR #08 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_09,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#201~208), 3G-CARR RESET ERR #09 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_10,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#201~208), 3G-CARR RESET ERR #10 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_11,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#201~208), 3G-CARR RESET ERR #11 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_12,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#201~208), 3G-CARR RESET ERR #12 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_13,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#201~208), 3G-CARR RESET ERR #13 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_14,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#201~208), 3G-CARR RESET ERR #14 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_15,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#201~208), 3G-CARR RESET ERR #15 */
	E_SYS_SVCTL_3G_CARRRESET_ERR_16,        /**< CNTM_SV15INF, RE&MasterCPRI ERR#12情報(ERR#201~208), 3G-CARR RESET ERR #16 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_01,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#209~216), S3G-CARR RESET ERR #01 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_02,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#209~216), S3G-CARR RESET ERR #02 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_03,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#209~216), S3G-CARR RESET ERR #03 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_04,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#209~216), S3G-CARR RESET ERR #04 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_05,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#209~216), S3G-CARR RESET ERR #05 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_06,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#209~216), S3G-CARR RESET ERR #06 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_07,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#209~216), S3G-CARR RESET ERR #07 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_08,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#209~216), S3G-CARR RESET ERR #08 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_09,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#217~224), S3G-CARR RESET ERR #09 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_10,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#217~224), S3G-CARR RESET ERR #10 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_11,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#217~224), S3G-CARR RESET ERR #11 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_12,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#217~224), S3G-CARR RESET ERR #12 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_13,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#217~224), S3G-CARR RESET ERR #13 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_14,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#217~224), S3G-CARR RESET ERR #14 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_15,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#217~224), S3G-CARR RESET ERR #15 */
	E_SYS_SVCTL_S3G_CARRRESET_ERR_16,       /**< CNTM_SV15INF, RE&MasterCPRI ERR#13情報(ERR#217~224), S3G-CARR RESET ERR #16 */

    E_SYS_SVCTL_STARTUP_ERR_S3G_01,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_02,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_03,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_04,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_05,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_06,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_07,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_08,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_09,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_10,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_11,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_12,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_13,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_14,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_15,         /**< CNTM_SV16INF */
    E_SYS_SVCTL_STARTUP_ERR_S3G_16,         /**< CNTM_SV16INF */

    E_SYS_SVCTL_3G_FAILSAFE_ERR_01,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_02,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_03,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_04,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_05,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_06,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_07,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_08,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_09,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_10,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_11,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_12,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_13,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_14,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_15,         /**< CNTM_SV5INF */
    E_SYS_SVCTL_3G_FAILSAFE_ERR_16,         /**< CNTM_SV5INF */

    E_SYS_SVCTL_S3G_FAILSAFE_ERR_01,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_02,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_03,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_04,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_05,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_06,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_07,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_08,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_09,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_10,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_11,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_12,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_13,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_14,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_15,        /**< CNTM_SV5INF */
    E_SYS_SVCTL_S3G_FAILSAFE_ERR_16,        /**< CNTM_SV5INF */

	E_SYS_SVCTL_MAX
};

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/** SV control operation type (L1 ERR) */
#define M_SYS_SVCTL_L1ERR(cprino)	(UINT)((D_RRH_CPRINO_REC==cprino) \
		? E_SYS_SVCTL_L1ERR : (E_SYS_SVCTL_L1_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

#define M_SYS_SVCTL_L2ERR(cprino)   (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? E_SYS_SVCTL_L2ERR : (E_SYS_SVCTL_L2_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (3G L3 ERR) */
#define M_SYS_SVCTL_3G_L3ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? E_SYS_SVCTL_3G_L3ERR : (E_SYS_SVCTL_3G_L3_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (S3G L3 ERR) */
#define M_SYS_SVCTL_S3G_L3ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? E_SYS_SVCTL_S3G_L3ERR : (E_SYS_SVCTL_S3G_L3_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (SFP ERR1) */
#define M_SYS_SVCTL_SFPERR1(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? E_SYS_SVCTL_SFP_ERR1 : (E_SYS_SVCTL_SFP_ERR1_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (SFP ERR2) */
#define M_SYS_SVCTL_SFPERR2(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? E_SYS_SVCTL_SFP_ERR2 : (E_SYS_SVCTL_SFP_ERR2_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (3G-DELAY ERR) */
#define M_SYS_SVCTL_3G_DELAY_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? E_SYS_SVCTL_DELAY_ERR : (E_SYS_SVCTL_DELAY_ERR1_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (S3G-DELAY ERR) */
#define M_SYS_SVCTL_S3G_DELAY_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? E_SYS_SVCTL_DELAY_ERR_S3G : (E_SYS_SVCTL_DELAY_ERR2_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (3G-RESTARTUP ERR) */
#define M_SYS_SVCTL_3G_RESTARTUP_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (E_SYS_SVCTL_STARTUP_ERR1_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (DL NG ERR) */
#define M_SYS_SVCTL_DL_NG_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (E_SYS_SVCTL_DL_NG_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (3G-CARR RESET ERR) */
#define M_SYS_SVCTL_3G_CARR_RESET_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (E_SYS_SVCTL_3G_CARRRESET_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (S3G-CARR RESET ERR) */
#define M_SYS_SVCTL_S3G_CARR_RESET_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (E_SYS_SVCTL_S3G_CARRRESET_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (S3G-RESTARTUP ERR) */
#define M_SYS_SVCTL_S3G_RESTARTUP_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (E_SYS_SVCTL_STARTUP_ERR_S3G_01 + (cprino - D_RRH_CPRINO_RE_MIN)))
//16B
/** SV control operation type (SLOT ERR) */
#define M_SYS_SVCTL_SLOT_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (E_SYS_SVCTL_SLOT_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))
//16B

/** SV control operation type (3G FAIL SAFE ERR) */
#define M_SYS_SVCTL_3G_FAILSAFE_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (E_SYS_SVCTL_3G_FAILSAFE_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))

/** SV control operation type (S3G FAIL SAFE ERR) */
#define M_SYS_SVCTL_S3G_FAILSAFE_ERR(cprino) (UINT)((D_RRH_CPRINO_REC==cprino) \
        ? 0 : (E_SYS_SVCTL_S3G_FAILSAFE_ERR_01 + (cprino - D_RRH_CPRINO_RE_MIN)))


/*!
 * @brief hardware alarm type
 * @date  2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
#define D_SYS_HWALMTYPE_PLL			0
#define D_SYS_HWALMTYPE_CPRI		1
#define D_SYS_HWALMTYPE_APDCOM		2
#define D_SYS_HWALMTYPE_APD0		3
#define D_SYS_HWALMTYPE_APD1		4
#define D_SYS_HWALMTYPE_APD2		5
#define D_SYS_HWALMTYPE_APD3		6
/* @} */

/*!
 * @name SET/CLEAR action define
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
#define D_SYS_SET					1
#define D_SYS_CLR					0
/* @} */

/** @name 接続構成種別 for shared-RE */
/* @{ */
#define D_SYS_CONN_3G_SINGLE		0		/**< 接続構成 is 3G単独 */
#define D_SYS_CONN_S3G_SINGLE		1		/**< 接続構成 is S3G単独 */
#define D_SYS_CONN_CPRI_SHARED		2		/**< 接続構成 is CPRI共用 */
#define D_SYS_CONN_RF_SHARED		3		/**< 接続構成 is RF共用 */
/* @} */


/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* @{ */
/* ======================================================================== */
/* Connected system number for shared-RE									*/
/* ======================================================================== */

#define D_SYS_MAX_SYSNUM			2		/**< Max connected system number for shared-RE */
#define D_SYS_3G					0		/**< connected system is 3G */
#define D_SYS_S3G					1		/**< connected system is S3G */
#define D_SYS_BOTH_3G_S3G			2		/**< connected system is both 3G and S3G */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @} */


/** @name 外部装置番号 */
/* @{ */
#define D_SYS_OARA_INF_1			1			/**< OA-RA-INF1					*/
#define D_SYS_OARA_INF_2			2			/**< OA-RA-INF2					*/
#define D_SYS_MTT_INF				3			/**< MTT-INF					*/
#define D_SYS_TILT_INF				4			/**< TILT-INF					*/

#define D_SYS_OARA_INF_S3G			1			/**< OA-RA-INF	 S3G			*/
#define D_SYS_TILT_INF_S3G			2			/**< TILT-INF	 S3G			*/


/****************************************************************************/
/*	- No.3 -																*/
/*								タイマID定義								*/
/*																			*/
/****************************************************************************/
/** @name timer ID definition */
/* @{ */

/* ======================================================================== */
/* timer ID definition 														*/
/* ======================================================================== */
#define D_SYS_TIMID_MIN				0x02000001	/**< timer ID minimum value 	*/
#define D_SYS_TIMID_INI_TRAMOUNT	0x02000001	/**< INI：TRA実装確認タイマ		*/
#define D_SYS_TIMID_INI_TRABOOTUP	0x02000002	/**< INI：TRA起動確認タイマ		*/
#define D_SYS_TIMID_INI_EXT_RCV		0x02000003	/**< INI：EXT状変受信タイマ		*/
#define D_SYS_TIMID_INI_IRQ4WATCH	0x02000004	/**< INI：IRQ4監視タイマ		*/

#define D_SYS_TIMID_USECHG_3G		0x02000006	/**< RSV: Use change timer for 3G*/
#define D_SYS_TIMID_USECHG_S3G		0x02000007	/**< RSV: Use change timer for S3G*/
#define D_SYS_TIMID_HCREQ_CNF_3G	0x02000008	/**< RSV: Health check confirm timer for 3G*/
#define D_SYS_TIMID_HCREQ_CNF_S3G	0x02000009	/**< RSV: Health check confirm timer for S3G*/

#define D_SYS_TIMID_FANALM_DETECT	0x0200000B	/**< RSV: detect FAN ALM timer */
#define D_SYS_TIMID_RLA_REGLOG		0x0200000C	/**< RLA: correct and write ram reffered RAL register value */
#define D_SYS_TIMID_PASTATE_AVOID	0x0200000D	/**< RSV: PA-OFF状態回避処理 timer */

#define D_SYS_TIMID_WARMUP0			0x0200000E	/**< RSV: #0 Warm up timer */
#define D_SYS_TIMID_WARMUP1			0x0200000F	/**< RSV: #1 Warm up timer */

#define D_SYS_TIMID_LOGTRNS_3G		0x02000010	/**< LOG：障害ログ転送間隔タイマ for 3G 	*/
#define D_SYS_TIMID_LOGTRNS_S3G		0x02000011	/**< LOG：障害ログ転送間隔タイマ for S3G 	*/

#define D_SYS_TIMID_USEFIRMWARESND	0x02000012	/**< DLM：FIRMWARE sending in USE state confirm timer	*/
#define D_SYS_TIMID_SYSPRMSND_3G	0x02000013	/**< DLM：SYSTEMPARAMETER sending confirm timer for 3G	*/
#define D_SYS_TIMID_SYSPRMSND_S3G	0x02000014	/**< DLM：SYSTEMPARAMETER sending confirm timer for 3G	*/
#define D_SYS_TIMID_RFSYSWAT		0x02000015	/**< DLM：RF SYSTEMPARAMETER confirm timer				*/
#define D_SYS_TIMID_FIRMWARESND_3G	0x02000016	/**< DLM：FIRMWARE sending confirm timer for 3G			*/
#define D_SYS_TIMID_FIRMWARESND_S3G	0x02000017	/**< DLM：FIRMWARE sending confirm timer for S3G		*/

#define D_SYS_TIMID_LPB_T1			0x02000018	/**< LPB：応答確認用タイマT1(link 1)					*/
#define D_SYS_TIMID_LPB_T2			0x02000019	/**< LPB：応答確認用タイマT2(link 1)					*/
#define D_SYS_TIMID_LPB_T3			0x0200001A	/**< LPB：相手局ビジー監視タイマT3(link 1)				*/
#define D_SYS_TIMID_LPB_T4			0x0200001B	/**< LPB：LPB：自局相手局アイドル状態 監視タイマT4(link 1)	*/
#define D_SYS_TIMID_L2BSYDET		0x0200001C	/**< LPB：L2 busy detect間隔タイマ						*/
#define D_SYS_TIMID_L1BSYDET		0x0200001D	/**< LPB：L1 busy detect間隔タイマ						*/

#define D_SYS_TIMID_STBL_PA0		0x0200001E	/**< RSV: #0 PA安定化タイマ 							*/
#define D_SYS_TIMID_STBL_PA1		0x0200001F	/**< RSV: #1 PA安定化タイマ 							*/

#define D_SYS_TIMID_OA1LNOFFSV		0x02000020	/**< EXT：OARA1回線断監視								*/
#define D_SYS_TIMID_OA2LNOFFSV		0x02000021	/**< EXT：OARA2回線断監視								*/
#define D_SYS_TIMID_MTTLNOFFSV		0x02000022	/**< EXT：MTT回線断監視									*/

#define D_SYS_TIMID_OA2EXTDRCF		0x02000024	/**< EXT：外部装置(OARA2)	データ受信確認				*/
#define D_SYS_TIMID_MTTEXTDRCF		0x02000025	/**< EXT：外部装置(MTT)		データ受信確認				*/
#define D_SYS_TIMID_TILEXTDRCF_3G	0x02000026	/**< EXT：外部装置(TILT)	データ受信確認 for 3G		*/
#define D_SYS_TIMID_TILEXTDRCF_S3G	0x02000027	/**< EXT：外部装置(TILT)	データ受信確認 for S3G		*/
#define D_SYS_TIMID_OA1EXTDRCF_3G	0x02000028	/**< EXT：外部装置(OARA1)	データ受信確認 for 3G		*/
#define D_SYS_TIMID_OA1EXTDRCF_S3G	0x02000029	/**< EXT：外部装置(OARA1)	データ受信確認 for S3G		*/
#define D_SYS_TIMID_PARITYSVCF		0x0200002A	/**< EXT: parity error supervision confirm timer 		*/

#define D_SYS_TIMID_SMCONCF_S3G		0x0200002D	/**< SV-MT接続確認タイマ for S3G						*/
#define D_SYS_TIMID_SMRELCF_S3G		0x0200002E	/**< SV-MT接続解放確認タイマ for S3G					*/
#define D_SYS_TIMID_SMDATRXCF_S3G	0x0200002F	/**< SV-MTデータ受信確認タイマ for S3G					*/
												
#define D_SYS_TIMID_FMCONCF			0x02000030	/**< FLD-MT接続確認タイマ								*/
#define D_SYS_TIMID_FMRELCF			0x02000031	/**< FLD-MT接続解放確認タイマ							*/
#define D_SYS_TIMID_FMCOMRXCF		0x02000032	/**< FLD-MTデータコマンド転送受信応答確認タイマ			*/
#define D_SYS_TIMID_FMFILRXCF		0x02000033	/**< FLD-MTデータファイル受信確認タイマ					*/
#define D_SYS_TIMID_SMCONCF			0x02000034	/**< SV-MT接続確認タイマ								*/
#define D_SYS_TIMID_SMRELCF			0x02000035	/**< SV-MT接続解放確認タイマ							*/
#define D_SYS_TIMID_SMDATRXCF		0x02000036	/**< SV-MTデータ受信確認タイマ							*/
#define D_SYS_TIMID_FMCONCF_S3G		0x02000037	/**< FLD-MT接続確認タイマ for S3G						*/
#define D_SYS_TIMID_FMRELCF_S3G		0x02000038	/**< FLD-MT接続解放確認タイマ for S3G					*/
#define D_SYS_TIMID_FMCOMRXCF_S3G	0x02000039	/**< FLD-MTデータコマンド転送受信応答確認タイマ	for S3G	*/
#define D_SYS_TIMID_FMFILRXCF_S3G	0x0200003A	/**< FLD-MTデータファイル受信確認タイマ	for S3G			*/
#define D_SYS_TIMID_TCPCONNWT		0x0200003B	/**< TCP connection waiting timer						*/
#define D_SYS_TIMID_FTPFILUPWT		0x0200003C	/**< FTP file upload waiting timer						*/
#define D_SYS_TIMID_REFLDL			0x0200003D	/**< MKM : ダウンロード確認タイマ						*/
#define D_SYS_TIMID_TDLWRTSV		0x0200003E	/**< MKM : TRAファイルライトタイマ						*/
#define D_SYS_TIMID_MKM_MON			0x02000040	/**< MKM: Monitor data timer							*/
#define D_SYS_TIMID_SNSSPV_INT		0x02000042	/**< センサ監視周期タイマ @800M							*/

/* @} */

/************************************/
/* RE共通部リンク確立要求パラメータ */
/************************************/
/* ﾂ榛’ﾃ?*/
#define D_SYS_ACTIVE				0			/**< Active						*/
#define D_SYS_NEGATIVE				1			/**< Negative					*/

#define D_SYS_FLASH					0
#define D_SYS_FPGA					1


#define D_SYS_SWCOMP				6

#define D_SYS_SIZE_1M				0x100000
#define D_SYS_SIZE_VUPU				0x80000

/* the size of VUPU is 320KB */
#define D_SYS_DEVDAT_VUPU			0x50000

/* @} */

/* ======================================================================== */
/* ダウンロード関連定義				 										*/
/* ======================================================================== */
/* BOOT FLASH SA120 ~SA127													*/
#define D_SYS_FLSADR_CFPGA_RRE0		0xF8000000		/**< CPRI-FPGA(RRE)領域0面 (TBD)		*/
#define D_SYS_FLSADR_CFPGA_RRE1		0xFA000000		/**< CPRI-FPGA(RRE)領域1面 (TBD)		*/

#define D_SYS_FLADDR_BOOT_0			0xF9F00000		/**< BOOT領域0面			*/
#define D_SYS_FLADDR_BOOT_1			0xFBF00000		/**< BOOT領域1面			*/
#define D_SYS_FLSADR_TP0			0xF9700000		/**< TP  領域0面			*/
#define D_SYS_FLSADR_TP1			0xFB700000		/**< TP  領域1面			*/

#define D_SYS_EEPSIDINF0			0x0000			/**< EEPROM起動面０(0x0000)				*/
#define D_SYS_EEPSINFMSK1			0x0001			/**< EEPROM起動面情報マスク(0x0001)		*/

#define	D_SYS_FLSRSLT_OK			0x00000000		/**< File更新正常							*/
#define	D_SYS_FLSRSLT_PRAERR		0x00000001		/**< File更新失敗(パラメータチェックエラー)	*/
#define	D_SYS_FLSRSLT_OPNERR		0x00000002		/**< File更新失敗(オープンエラー)			*/
#define	D_SYS_FLSRSLT_ERSERR		0x00000003		/**< Flash消去失敗							*/
#define	D_SYS_FLSRSLT_HNTERR		0x00000004		/**< Writeバッファ獲得失敗					*/
#define	D_SYS_FLSRSLT_WRTERR		0x00000005		/**< Flash書込み失敗						*/

#define	D_SYS_SFPRSLT_OK			0x00000000		/**< Softパラメータ書込み正常				*/
#define	D_SYS_SFPRSLT_EEPERR		0x00000001		/**< EEP 書込み失敗							*/
#define	D_SYS_SFPRSLT_SMPTER		0x00000002		/**< 排他制御(セマフォ)失敗					*/
#define	D_SYS_SFPRSLT_ERSERR		0x00000003		/**< Flash消去失敗							*/
#define	D_SYS_SFPRSLT_HNTERR		0x00000004		/**< Writeバッファ獲得失敗					*/
#define	D_SYS_SFPRSLT_WRTERR		0x00000005		/**< Flash書込み失敗						*/
#define	D_SYS_SFPRSLT_SMPGER		0x00000006		/**< 排他制御(セマフォ)失敗					*/


/* ======================================================================== */
/* EEPROM制御レジスタ												(W)		*/
/* ======================================================================== */
/* オフセット	*/

#define D_SYS_REGADR_HARDVER		0x000B		/**< ハードバージョン上位		*/
  
#define D_SYS_REGADR_SOFTVER0		0x0014		/**< Flash 0面ソフトバージョン	*/
#define D_SYS_REGADR_SOFTYEAR0		0x0015		/**< Flash 0面ソフト年情報		*/
#define D_SYS_REGADR_SOFTDAY0		0x0016		/**< Flash 0面ソフト月日情報	*/

#define D_SYS_REGADR_SOFTVER1		0x0017		/**< Flash 1面ソフトバージョン	*/
#define D_SYS_REGADR_SOFTYEAR1		0x0018		/**< Flash 1面ソフト年情報		*/
#define D_SYS_REGADR_SOFTDAY1		0x0019		/**< Flash 1面ソフト月日情報	*/

#define D_SYS_REGADR_FPGAVER0		0x002E		/**< Flash 0面FPGAバージョン	*/
#define D_SYS_REGADR_FPGAYEAR0		0x002F		/**< Flash 0面FPGA年情報		*/
#define D_SYS_REGADR_FPGADAY0		0x0030		/**< Flash 0面FPGA月日情報		*/

#define D_SYS_REGADR_FPGAVER1		0x0031		/**< Flash 1面FPGAバージョン	*/
#define D_SYS_REGADR_FPGAYEAR1		0x0032		/**< Flash 1面FPGA年情報		*/
#define D_SYS_REGADR_FPGADAY1		0x0033		/**< Flash 1面FPGA月日情報		*/

#define D_SYS_REGADR_ERASEFLG		0x002A		/**< Firm Erase Flag			*/
#define D_SYS_REGADR_FPGAERASEFLG	0x002B		/**< FPGA Erase Flag			*/
#define D_SYS_REGADR_VUPUERASEFLG	0x002C		/**< VUPU Erase Flag			*/
#define D_SYS_REGADR_APDERASEFLG 	0x002D      /**< APD Erase Flag				*/

#define D_SYS_REGADR_FPGABOOT		0x0034		/**< FPGA起動面情報				*/
#define D_SYS_REGADR_PRGBOOT		0x0035		/**< プログラム起動面情報		*/
#define D_SYS_REGADR_APD3BOOT		0x0036		/**< APD-FPGA起動面情報			*/
#define D_SYS_REGADR_VUPUBOOT		0x0037		/**< VUPU firm起動面情報		*/
#define D_SYS_EEPOFS_PRGRMBOOT		CMD_REGADR_PRGBO

/* ======================================================================== */
/* EEPROM関連(起動面)														*/
/* ======================================================================== */
#define D_SYS_BOOT0				0				/**< 0面起動					*/
#define D_SYS_BOOT1				1				/**< 1面起動					*/
/* @} */

/*!  
 * @name bootface changing type define
 * @{  
 */
#define D_SYS_BOOTFACE_USE		1	/* change the bootface to the current using face	*/
#define D_SYS_BOOTFACE_OPP		2	/* change the bootface to the opposite using face	*/
/* @} */

/* the size of CPRIFPGA file is 12M */
#define D_SYS_DEVDAT_CPRIFPGA		0xC00000			/* MDIF-FPGA/OFMSDIF-FPGAサイズ				*/
/* the size of FIRM file is 2M */
#define D_SYS_DEVDAT_FIRM			0x200000			/* MDIFファームサイズ						*/

/** @name bit definition */
/* @{ */

/* Bit */
#define D_SYS_BIT1				1
#define D_SYS_BIT2				2
#define D_SYS_BIT3				3
#define D_SYS_BIT4				4
#define D_SYS_BIT5				5
#define D_SYS_BIT6				6
#define D_SYS_BIT7				7
#define D_SYS_BIT8				8
#define D_SYS_BIT9				9
#define D_SYS_BIT10				10
#define D_SYS_BIT11				11
#define D_SYS_BIT12				12
#define D_SYS_BIT13				13
#define D_SYS_BIT14				14
#define D_SYS_BIT15				15
#define D_SYS_BIT16				16

/* @} */

/*!  
 * @name define for soft version mask
 * @{  
 */
#define D_SYS_MJRVER_MSK		0xFF00				/* メジャーバージョン用マスク	*/
#define D_SYS_MINVER_MSK		0x00FF				/* メジャーバージョン用マスク	*/
#define D_SYS_VER_MSK			0x000F				/* ソフトバージョン用マスク		*/
#define D_SYS_BIT_MSK			0x0001				/* the mask of Bit Line Rate 	*/
/* @} */

#define D_SYS_DEVKND_RRE				0x0001	/**< device kind for RRE 				*/
#define D_SYS_DEVKND_SRE				0x0001  /**< device kind for SRE				*/
#define D_SYS_DEVKND_BRECPRI			0x0002	/**< device kind for BRE-CPRI			*/
#define D_SYS_DEVKND_BRERF				0x0003	/**< device kind for BRE-RF				*/
#define D_SYS_DEVKND_LRE2T2R			0x0004	/**< device kind for LRE-2T2R			*/
#define D_SYS_DEVKND_LRE1T2R			0x0005	/**< device kind for LRE-1T1R			*/
#define D_SYS_DEVKND_LRE2T2R_CRD		0xFFFE	/**< device kind for LRE-2T2R(card type)	*/
#define D_SYS_DEVKND_LRE1T2R_CRD		0xFFFF	/**< device kind for LRE-1T1R(card type)	*/
#define D_SYS_DEVKND_RRE_800M			0x0006	/**< device kind for 800M-RRE			*/
#define D_SYS_DEVKND_RRE_2GCD			0x0007	/**< device kind for 2G CD-RRE			*/
#define D_SYS_DEVKND_RRE_ECO			0x0008	/**< device kind for 800M-RRE			*/
#define D_SYS_DEVKND_RRE_15G			0x0009	/**< device kind for 1.5G-RRE			*/
#define D_SYS_DEVKND_RRE_800M_SLC		0x000A	/**< device kind for 800M-RRE(SLC)			*/
#define D_SYS_DEVKND_RRE_800M_SLCHIGH	0x000B	/**< device kind for 800M-RRE(SLC 高出力)	*/
#define D_SYS_DEVKND_RRE_2G_SLC			0x000C	/**< device kind for 2G-RRE(SLC)			*/
#define D_SYS_DEVKND_RRE_2G_SLCHIGH		0x000D	/**< device kind for 2G-RRE(SLC 高出力)		*/
#define D_SYS_DEVKND_TRA_CPRI_CD		0x000E	/**< device kind for TRA CPRIコストダウン	*/
#define D_SYS_DEVKND_RRE_15G_VA			0x0100	/**< device kind for 1.5G (SLC)	*/
#define D_SYS_DEVKND_RRE_17G			0x0101	/**< device kind for 1.7G-RRE	*/
#define D_SYS_DEVKND_RRE_700M			0x0102	/**< device kind for 700M-RRE	*/
#define D_SYS_DEVKND_XRE_RSV1			0x0103	/**< device kind for Reserve No1	*/
#define D_SYS_DEVKND_XRE_RSV2			0x0104	/**< device kind for Reserve No2	*/
#define D_SYS_DEVKND_XRE_RSV3			0x0105	/**< device kind for Reserve No3	*/
#define D_SYS_DEVKND_LRE_15G			0x0106	/**< device kind for 15G LRE	*/
#define D_SYS_DEVKND_LRE_17G			0x0107	/**< device kind for 17G LRE	*/
#define D_SYS_DEVKIND_OFFSET_1CHP	 	D_SYS_DEVKND_RRE_15G_VA		/**< offset for 1chp 	*/
#define D_SYS_DEVKND_MAX				(D_SYS_DEVKND_LRE_17G+1)	/**< device kind for MAX 	*/


#define D_SYS_RCVDAT_MKRSV_3G_FILNUM3		0x1e6		/**< メーカ固有領域サイズ486byte*/
#define D_SYS_RCVDAT_MKRSV_3G_FILNUM4		0x1B6		/**< メーカ固有領域サイズ438byte*/
#define D_FSYS_RCV_NUM         				16777216	/**< 受信データ数(REファイルチェック用MDEIF 16M) */
#define D_SYS_RCV_3G_NUM					8388608		/**< 受信データ数(REファイルチェック(from 3G)用MDEIF 8M) */
#define D_SYS_RCVDAT_MKCOM_RSV				458			/**< 予約領域サイズ	458byte		*/


/*!
 * @brief ERR bit
 * @{  
 */
#define D_SYS_AMPDISC_CHLD_BIT	0x2000		/**< SLP0 ERR bit in SV A6/A7		*/
#define D_SYS_AMPDISC_BIT		0x0010		/**< AMPｶﾏERR bit in SV A8/A9		*/
#define D_SYS_SLP0_ERR_BIT		0x0020		/**< SLP0 ERR bit in SV A8/A9		*/
#define D_SYS_SLP1_ERR_BIT		0x0040		/**< SLP1 ERR bit in SV A8/A9		*/
#define D_SYS_SLP2_ERR_BIT		0x0080		/**< SLP2 ERR bit in SV A8/A9		*/
#define D_SYS_SLP3_ERR_BIT		0x0100		/**< SLP3 ERR bit in SV A8/A9		*/
#define D_SYS_SLP0_1_ERR_BIT	(D_SYS_SLP0_ERR_BIT |D_SYS_SLP1_ERR_BIT)
#define D_SYS_SLPAMP_BIT		(D_SYS_AMPDISC_BIT | D_SYS_SLP0_ERR_BIT | D_SYS_SLP1_ERR_BIT \
								|D_SYS_SLP2_ERR_BIT |D_SYS_SLP3_ERR_BIT )
/* @} */

/** @name BR number definition */
/* @{ */

/* ======================================================================== */
/* BR番号定義																*/
/* ======================================================================== */
#define D_SYS_BR0				0		/**< BR0							*/
#define D_SYS_BR1				1		/**< BR1							*/
#define D_SYS_BR2				2		/**< BR2						*/
#define D_SYS_BR3				3		/**< BR3						*/
#define D_SYS_BR_BOTH			2		/**< BR both						*/
#define D_SYS_MAX_BRNUM			4		/**< AMP/OF時の最大BR数				*/
#define D_SYS_OFNODIV_MAX_BRNUM	1		/**< OF-TRX(Divなし)時の最大BR数	*/

/* @} */

/*TDD SRE  TRX setting S. */
#define D_SYS_MAX_BRNUM_NT4R		0x04		/**< NT4R BR数				*/		
#define D_SYS_MAX_BRNUM_NT2R		0x02		/**< NT2R BR数				*/	
/*TDD SRE  TRX setting E. */


/** @name compress type definition */
/* @{ */

/* ======================================================================== */
/* Compress Type Definition													*/
/* ======================================================================== */
#define D_SYS_UNZIP				0		/**< non-compress					*/
#define D_SYS_GZIP				1		/**< gzip-compress					*/
#define D_SYS_FLDC4				2		/**< fldc4-compress					*/

/* @} */

/* @} */

/** @name file name for tar.gz */
/* @{ */

/* ======================================================================== */
/* File Name Definition														*/
/* ======================================================================== */
#define D_SYS_TARGZ_FILENAME			"zynq.tar.gz"		/**< file name				*/

/* @} */

/* @} */

/** @name decompression path */
/* @{ */

/* ======================================================================== */
/* Decompression Path Definition											*/
/* ======================================================================== */
#define D_SYS_UNZIP_DIRNAME				"/tmp/"		/**< decompression path				*/

/* @} */

/** @name Port out register definition */
/* @{ */
#define D_SYS_REGMSK_PROT_OUT			0x0000000F	/**< PORT出力状態			*/
#define D_SYS_REGMSK_PROT_OUT_SRE		0x00000001	/**< PORT出力状態			*/
#define D_SYS_REGMSK_PROT_OUT_35G_SRE	0x00000001	/**< PORT出力状態			*/
/* @} */

/** @name Port in register mask definition */
/* @{ */
#define D_SYS_REGMSK_PROT_IN			0x000000FF	/**< PORTIN register mask for RRE	*/
#define D_SYS_REGMSK_PROT_IN_SRE		0x00000001	/**< PORTIN register mask for SRE	*/
#define D_SYS_REGMSK_PROT_IN_35G_SRE	0x01000000	/**< PORTIN register mask for 35G-SRE	*/
/* @} */

/** @name decompression path */
/* @{ */
#define D_RRH_SYS_3G					0
#define D_RRH_SYS_S3G					1
#define D_RRH_MAX_SYSNUM				2
/* @} */

/** @name SV State Control code definition */
/* @{ */
#define D_SYS_SVSTATE_UNKOWN			0x00000000
#define D_SYS_SVSTATE_IRQ				0x00000001
#define D_SYS_SVSTATE_SET				0x00000002
#define D_SYS_SVSTATE_CTL				0x00000003
#define D_SYS_SVSTATE_CLR				0x00000004
#define D_SYS_SVSTATE_MAX				0x00000005

/* @} */


/*!
 * @brief the bit mask defintion in SV state IRQ
 * @{  
 */
#define D_SYS_REGMASK_SV1							0x00077337
#define D_SYS_REGMASK_SV2							0x0303FFFF
#define D_SYS_REGMASK_SV3							0x03030303
#define D_SYS_REGMASK_SV4							0x0311FF07
#define D_SYS_REGMASK_SV5							0xFFFFFFFF
#define D_SYS_REGMASK_SV6							0x37FFFF31
#define D_SYS_REGMASK_SV7							0x3FFF1FFF
#define D_SYS_REGMASK_SV8							0x000F3FFF
#define D_SYS_REGMASK_SV9							0x01070107
#define D_SYS_REGMASK_SV10							0x01070107
#define D_SYS_REGMASK_SV11							0x01070107
#define D_SYS_REGMASK_SV12							0x01070107
#define D_SYS_REGMASK_SV13							0x00000101
#define D_SYS_REGMASK_SV16							0x730F0001

/* @} */

/*!
 * @brief Max number of system parameters
 * @{  
 */
#define D_SYS_SYSPARA_MAX				32		/*シスパラ数*/
/* @} */

/*!
 * @brief Default values of system parameters
 * @{  
 */
/*システムパラメータTypical情報管理テーブル*/
#define D_SYS_SYS_CSTATE				20		/*	CPRI State安定化時間(RE)			*/
#define D_SYS_SYS_HCHECK				120		/*	ヘルスチェック要求確認タイマ(RE)	*/
#define D_SYS_SYS_HREQUEST				5		/*	ヘルスチェック要求未確認保護段数(RE) */
#define D_SYS_SYS_SYSSEND				300		/*	システムパラメータ送信確認タイマ	*/
#define D_SYS_SYS_FIRMSEND				7000	/*	ファームウェア送信確認タイマ		*/
#define D_SYS_SYS_SECCHIP0				0		/*	セクタnチップ補正(0系)				*/
#define D_SYS_SYS_SECCHIP1				0		/*	セクタnチップ補正(1系)				*/
#define D_SYS_SYS_EXTDATA				150		/*	外部装置データ受信確認タイマ		*/

#define D_SYS_SYS_FLD_MT_CON_TIM		150		/*	FLD-MT接続確認タイマ				*/
#define D_SYS_SYS_FLD_MT_REL_TIM		150		/*	FLD-MT接続解放確認タイマ			*/
#define D_SYS_SYS_FLD_MT_DATA_C_RCV_TIM	150		/*	FLD-MTデータコマンド受信確認タイマ	*/
#define D_SYS_SYS_FLD_MT_DATA_F_RCV_TIM	150		/*	FLD-MTデータファイル受信確認タイマ	*/

#define D_SYS_SYS_SV_MT_CON_TIM			150		/*	SV-MT接続確認タイマ					*/
#define D_SYS_SYS_SV_MT_REL_TIM			150		/*	SV-MT接続解放確認タイマ				*/
#define D_SYS_SYS_SV_MT_DATA_RCV_TIM	150		/*	SV-MTデータ受信確認タイマ			*/
#define D_SYS_SYS_TX_DIV_VALUE_3G		2		/*	3G用送信ダイバーシチ設定			*/
#define D_SYS_SYS_DEFAULT				0
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_SYS_SYS_RESERVE				65535
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* TBD: the default system para value*/
#define D_SYS_SYS_PRISYSINFO			0		/* primary system info					*/
#define D_SYS_SYS_S3G_BANDWIDTH			1		/* S3G bandwidth						*/
#define D_SYS_SYS_3G_CAR_WORKFLG		1		/* 3G carrier#1~4 working flag			*/
#define D_SYS_SYS_TX_SYS_NUM			0		/* TX system number						*/
#define D_SYS_SYS_3G_CAR1_WORKFRQ		0		/* 3G carrier#1 working frequency		*/
#define D_SYS_SYS_3G_CAR2_WORKFRQ		0		/* 3G carrier#2 working frequency		*/
#define D_SYS_SYS_3G_CAR3_WORKFRQ		0		/* 3G carrier#3 working frequency		*/
#define D_SYS_SYS_3G_CAR4_WORKFRQ		0		/* 3G carrier#4 working frequency		*/
#define D_SYS_SYS_3G_MT_EXTD_FLG		1		/**< 3G MT signaling/External equipment data corresponse flag (TBD)  */
#define D_SYS_SYS_SYSPARA_WAIT_TIM		6000	/**< システムパラメータ転送待ちタイマ	*/
#define D_SYS_SYS_FIRM_SND_TIM			0		/**< ファームウェア送信確認タイマ（更新用）  */
/* @} */

/* SRE対応 - Start */
#define D_SYS_REG_SWDT_MODE				0xF8005000	/**< System Watchdog Timer Registers - WD zero mode register  */
#define D_SYS_REG_SWDT_MODE_ZKEY		0x00ABC000	/**< Zero access key  */
#define D_SYS_REG_SWDT_MODE_ZKEY_BIT	0x00FFF000	/**< Zero access key Bit */
#define D_SYS_REG_SWDT_MODE_WDEN_BIT	0x00000001	/**< Zero access key Bit */
/* SRE対応 - End */

/** @name RE装置構成情報 */
/* @{ */
#define D_SYS_BTS_NO					3001		/* BTS型番						*/
#define D_SYS_BTS_SRE_NO				3201		/* BTS型番						*/
#define D_SYS_REKIND_OFTRX				0x0001		/**< RE type is RRE/LRE(OFTRX)	*/
#define D_SYS_DIV_ON					0x0001		/**< 装置号番がDIV有り			*/
#define D_SYS_DIV_OFF					0x0000		/**< 装置号番がDIV無し			*/
#define D_SYS_PTYP_DC					0x0000		/**< DC電源						*/
#define D_SYS_PTYP_COM					0x0003		/**< AC100/200V					*/
#define D_SYS_PWR_CLASS_5				5			/**< power class 5W				*/
#define D_SYS_PWR_CLASS_6300			0x189C		/**< power class 6300MW			*/
#define D_SYS_PWR_CLASS_667				0x029B		/**< power class 667MW			*/
#define D_SYS_PWR_CLASS_120				0x0078		/**< power class 120MW			*/
#define D_SYS_PWR_CLASS_4750			4750
#define D_SYS_PWR_CLASS_65535			65535
#define D_SYS_POWERUNIT_W				0			/**< W							*/
#define D_SYS_POWERUNIT_MW				1			/**< mW							*/
#define D_SYS_CRP_FRQ_2G				0			/**< 対応周波数帯 2G				*/
#define D_SYS_CRP_FRQ_2G_S3G			1			/**< 対応周波数帯 2G(S3G報告値)		*/
#define D_SYS_CRP_FRQ_15G_S3G			2			/**< 対応周波数帯 1.5G(S3G報告値)	*/
#define D_SYS_CRP_FRQ_800M_S3G			4			/**< 対応周波数帯 800M(S3G報告値)	*/
#define D_SYS_CRP_FRQ_17G_S3G			16			/**< 対応周波数帯 1.7G(S3G報告値)	*/
#define D_SYS_CRP_FRQ_700M_S3G			8			/**< 対応周波数帯 700(S3G報告値)	*/
#define D_SYS_CRP_FRQ_35G_S3G			64			/**< 対応周波数帯 3.5G	*/
#define D_SYS_BANDW_MAX_10M				2			/**< Max bandwidth is 10M */
#define D_SYS_BANDW_MAX_15M				3			/**< Max bandwidth is 15M */
#define D_SYS_BANDW_MAX_20M				4			/**< Max bandwidth is 20M */
#define D_SYS_BANDW_MAX_40M				8			/**< Max bandwidth is 40M */
#define D_SYS_SND_ANTE_MAX_1			1			/**< Max sending antenna number is 1 */
#define D_SYS_SND_ANTE_MAX_2			2			/**< Max sending antenna number is 2 */
#define D_SYS_SND_ANTE_MAX_4			4			/**< Max sending antenna number is 4 */

#define D_SYS_TRX_INF_TYP_C				2			/**< TRX-INFタイプ(Cタイプ)				*/
#define D_SYS_TRX_INF_TYP_D				3			/**< TRX-INFタイプ(Dタイプ)				*/
#define D_SYS_TRX_INF_TYP_SHORT			2			/**< TRX-INFタイプ(短距離タイプ)		*/
#define D_SYS_TRX_INF_TYP_UNSUPT		2			/**< TRX-INFタイプ(未サポート)			*/
#define D_SYS_TRX_INF_TYP_ABNORMAL		2			/**< TRX-INFタイプ(図番確認不可(異常))	*/
/* @} */

/*!
 * @brief definition of diag bit
 * @{  
 */
#define D_SYS_DIA_BOOT_CHK					0x10000000		/* (BOOT)CHK							*/
#define D_SYS_DIA_LM_CHK					0x01000000		/* (LM)CHK								*/
#define D_SYS_DIA_FPGA_CONFIG_CHK			0x00400000		/* FPGA Config確認						*/
#define D_SYS_DIA_CPRIFPGA_CHKSUM_CHK		0x00200000		/* CPRI-FPGAチェックサム				*/
#define D_SYS_DIA_PLFPGA_CHK				0x00100000		/* (PLFPGA)CHK							*/
#define D_SYS_DIA_TXFPGA_CHK				0x00010000		/* (TXFPGA)CHK							*/
#define D_SYS_DIA_CLEMENTE_CHKSUM_CHK		0x00002000		/* クレメンテチェックサムCHK			*/
#define D_SYS_DIA_RFIC_CHK					0x00001000		/* RFICレジスタアクセスCHK				*/
#define D_SYS_DIA_MEM_CHK					0x00000100		/* (memory)CHK							*/
#define D_SYS_DIA_APDFPGA_REG_CHK			0x00000040		/* APD-FPGAレジスタアクセス				*/
#define D_SYS_DIA_CPRIFPGA_REG_CHK			0x00000020		/* CPRIFPGA CHK							*/
#define D_SYS_DIA_REG_CHK					0x00000010		/* (register)CHK						*/
#define D_SYS_DIA_HDLC_CHK					0x00000004		/* HDLCレジスタアクセスCHK				*/
#define D_SYS_DIA_CPLD_CHK					0x00000002		/* CPLDレジスタアクセスCHK				*/
#define D_SYS_DIA_SFP_CHK					0x00000001		/* (SFP)CHK								*/

/* 1次診断アイテム */
#define D_SYS_DIA_1ST_TDD					( D_SYS_DIA_RFIC_CHK | D_SYS_DIA_MEM_CHK | D_SYS_DIA_REG_CHK | \
												D_SYS_DIA_HDLC_CHK | D_SYS_DIA_CPLD_CHK )
#define D_SYS_DIA_MASK						0xFFFFEFFB		/* HDLC,RFICレジスタチェック除外mask data */
 	
/* @} */

/* IPアドレス（デフォルト）  */
#define D_SYS_IPADR_DEFAULT					0xc0a80183				/*  192.168.1.131	*/

/* WDT関連情報  */
#define D_SYS_AXI_TIMER_SET			0x00000008		/* AXI_TIMER開始			*/
#define D_SYS_SWDT_TIMER_SET		0x00000011		/* SWDT_TIMER開始			*/
#define D_SYS_SWDT_TIMER_SET_EXT	0xEEEEEEEE		/* SWDT_TIMER開始(50秒)		*/
#define D_SYS_AXI_TIMER_CLR			0x00000000		/* AXI_TIMER停止			*/
#define D_SYS_SWDT_TIMER_CLR		0x00000000		/* SWDT_TIMER停止			*/
#define D_SYS_AXI_TIMER_SKP			0xFFFFFFFF		/* AXI_TIMER設定SKIP		*/
#define D_SYS_SWDT_TIMER_SKP		0xFFFFFFFF		/* SWDT_TIMER設定SKIP		*/
#define D_SYS_SWDT_RESET_DIS		0x0000FFFF		/* SWDT_TIMER Enable, Reset Disable	*/

/******************************************************
			AISG part
*******************************************************/
/*!
* @name AISG Result
* @note AISG Result define
* @{
*/
#define D_SYS_AISG_RST_NORMAL      		0x00            	/*!< Normal						*/
#define D_SYS_AISG_RST_CONFLICT      	0x01            	/*!< Conflict					*/
#define D_SYS_AISG_RST_FRMRRCV       	0x02            	/*!< FRMR receive				*/
#define D_SYS_AISG_RST_TOOMANYRNR		0x03            	/*!< Too Many RNR				*/
#define D_SYS_AISG_RST_TO				0x04            	/*!< Timeout					*/
#define D_SYS_AISG_RST_DMRCV			0x05            	/*!< DM receive					*/
#define D_SYS_AISG_RST_TOOMANYCMD		0x06            	/*!< Too Many Command			*/
#define D_SYS_AISG_RST_OTHERS			0x07            	/*!< Others						*/
/* @} */

/*!
* @name AISG XID Send Response Type
* @note AISG Result define
* @{
*/
#define D_SYS_AISG_XIDTYPE_ADR			0x01            		/*!< XID Type: address assign	*/
#define D_SYS_AISG_XIDTYPE_PROT			0x02            		/*!< XID Type: protocol version	*/
#define D_SYS_AISG_XIDTYPE_HDLC			0x03            		/*!< XID Type: hdlc parameter	*/
#define D_SYS_AISG_XIDTYPE_RST			0x04            		/*!< XID Type: reset			*/
/* @} */

/*!
* @name Baudrate
* @note Baudrate
* @{
*/
#define D_SYS_AISG_BAUDRATE_AUTO        	0x0000            	/*!< Baudrate:auto baud			*/
#define D_SYS_AISG_BAUDRATE_9600    	    0x0001            	/*!< Baudrate:9600 baud			*/
#define D_SYS_AISG_BAUDRATE_38400        	0x0002            	/*!< Baudrate:38400 baud			*/
#define D_SYS_AISG_BAUDRATE_115200        	0x0003            	/*!< Baudrate:115200 baud			*/
/* @} */


/*!
* @name Endian type
* @note Endian type
* @{
*/
#define D_SYS_AISG_LITTLE_ENDIAN        	0x0000            	/*!< Little endian			*/
#define D_SYS_AISG_BIG_ENDIAN    	    	0x0001            	/*!< Big endian				*/
/* @} */

/*!
* @name AISG Max Device Number
* @note AISG Max Device Number define
* @{
*/
#define D_SYS_AISG_MAX_DEVNUM        	0x0C            	/*!< Max Device Number			*/
/* @} */

/*!
 * @name TILT接続有無情報定義
 * @note 
 * @{
 */
#define D_SYS_EEP_TILTEN							0x01						/* TILT接続有無情報					*/
/* @} */

#endif
/** @} */

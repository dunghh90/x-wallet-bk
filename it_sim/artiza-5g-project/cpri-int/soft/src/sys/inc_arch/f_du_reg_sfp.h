/*!
 * @skip  $ld:$
 * @file  f_du_reg_sfp.h
 * @brief レジスタアドレス定義ヘッダ(SFP)
 * @date  2018/08/15 FJT) Yoshida
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_SFP_H
#define F_DU_REG_SFP_H
/*!
 * @name SFP EEPROMレジスタアドレス
 * @note SFP EEPROMレジスタアドレス
 * @{
 */
/* SFP:A0h	*/
#define D_DU_REG_SFP_A0_IDENTIFIER					0x0000	/* Identifier						*/
#define D_DU_REG_SFP_A0_EXT_IDENTIFIER				0x0001	/* Ext. Identifier					*/
#define D_DU_REG_SFP_A0_CONNECTOR					0x0002	/* Connector						*/
#define D_DU_REG_SFP_A0_TRANSCEIVER					0x0003	/* Transceiver						*/
#define D_DU_REG_SFP_A0_ENCODING					0x000B	/* Encoding							*/
#define D_DU_REG_SFP_A0_BR_NOMINAL					0x000C	/* BR, Nominal						*/
#define D_DU_REG_SFP_A0_RATE_ID						0x000D	/* Rate Identifier					*/
#define D_DU_REG_SFP_A0_LEN_SMF_KM					0x000E	/* Length(SMF,km)					*/
#define D_DU_REG_SFP_A0_LEN_SMF						0x000F	/* Length (SMF)						*/
#define D_DU_REG_SFP_A0_LEN_50UM					0x0010	/* Length (50um)					*/
#define D_DU_REG_SFP_A0_LEN_62_5UM					0x0011	/* Length (62.5um)					*/
#define D_DU_REG_SFP_A0_LEN_CABLE					0x0012	/* Length (OM4 or copper cable)		*/
#define D_DU_REG_SFP_A0_LEN_OM3						0x0013	/* Length (OM3)						*/
#define D_DU_REG_SFP_A0_VENDOR_NAME					0x0014	/* Vendor name						*/
#define D_DU_REG_SFP_A0_TRANSCEIVER_2				0x0024	/* Transceiver						*/
#define D_DU_REG_SFP_A0_VENDOR_OUI					0x0025	/* Vendor OUI						*/
#define D_DU_REG_SFP_A0_VENDOR_PN					0x0028	/* Vendor PN						*/
#define D_DU_REG_SFP_A0_VENDOR_REV					0x0038	/* Vendor rev						*/
#define D_DU_REG_SFP_A0_WAVELENGTH					0x003C	/* Wavelength						*/
#define D_DU_REG_SFP_A0_CC_BASE						0x003F	/* CC_BASE							*/
#define D_DU_REG_SFP_A0_OPTIONS						0x0040	/* Options							*/
#define D_DU_REG_SFP_A0_BR_MAX						0x0042	/* BR, max							*/
#define D_DU_REG_SFP_A0_BR_MIN						0x0043	/* BR, min							*/
#define D_DU_REG_SFP_A0_VENDOR_SN					0x0044	/* Vendor SN						*/
#define D_DU_REG_SFP_A0_DATE_CODE					0x0054	/* Date code						*/
#define D_DU_REG_SFP_A0_DIAG_MON_TYPE				0x005C	/* Diagnostic Monitoring Type		*/
#define D_DU_REG_SFP_A0_ENHANCED_OPT				0x005D	/* Enhanced Options					*/
#define D_DU_REG_SFP_A0_SFF8472_COMP				0x005E	/* SFF-8472 Compliance				*/
#define D_DU_REG_SFP_A0_CC_EXT						0x005F	/* CC_EXT							*/
#define D_DU_REG_SFP_A0_VENDOR_SPECIFIC				0x0060	/* Vendor Specific					*/

/* SFP:A2h	*/
#define D_DU_REG_SFP_A2_AW_THR						0x0000	/* A/W Thresholds					*/
#define D_DU_REG_SFP_A2_OPT_AW_THR					0x0028	/* Optional A/W Thresholds			*/
#define D_DU_REG_SFP_A2_EXT_CAL_CONSTANTS			0x0038	/* Ext Cal Constants				*/
#define D_DU_REG_SFP_A2_CC_DMI						0x005F	/* CC_DMI							*/
#define D_DU_REG_SFP_A2_DIAGNOSTICS					0x0060	/* Diagnostics						*/
#define D_DU_REG_SFP_A2_OPT_DIAG					0x006A	/* Optional Diagnostics				*/
#define D_DU_REG_SFP_A2_STAT_CONT					0x006E	/* Status/Control					*/
#define D_DU_REG_SFP_A2_ALARM_FLAGS					0x0070	/* Alarm Flags						*/
#define D_DU_REG_SFP_A2_CDR_UNLOCKED				0x0073	/* CDR Unlocked						*/
#define D_DU_REG_SFP_A2_WARNING FLAGS				0x0074	/* Warning Flags					*/
#define D_DU_REG_SFP_A2_EXT_STAT_CONT				0x0076	/* Ext Status/Control				*/
#define D_DU_REG_SFP_A2_VENDOR_SPECIFIC				0x0078	/* Vendor Specific					*/
#define D_DU_REG_SFP_A2_TABLE_SELECT				0x007F	/* Table Select						*/
#define D_DU_REG_SFP_A2_USER_EEPROM					0x0080	/* User EEPROM						*/
#define D_DU_REG_SFP_A2_VENDOR_CONTROL				0x00F8	/* Vendor Control					*/
#define D_DU_REG_SFP_A2_RX_DECISION_THR				0x0083	/* Rx Decision Threshold			*/

/* A/W Thresholds	*/
#define D_DU_REG_SFP_A2_TEMP_HIGH_ALM				0x0000	/* Temp High Alarm					*/
#define D_DU_REG_SFP_A2_TEMP_LOW_ALM				0x0002	/* Temp Low Alarm					*/
#define D_DU_REG_SFP_A2_TEMP_HIGH_WARN				0x0004	/* Temp High Warning				*/
#define D_DU_REG_SFP_A2_TEMP_LOW_WARN				0x0006	/* Temp Low Warning					*/
#define D_DU_REG_SFP_A2_VOLT_HIGH_ALM				0x0008	/* Voltage High Alarm				*/
#define D_DU_REG_SFP_A2_VOLT_LOW_ALM				0x000A	/* Voltage Low Alarm				*/
#define D_DU_REG_SFP_A2_VOLT_HIGH_WARN				0x000C	/* Voltage High Warning				*/
#define D_DU_REG_SFP_A2_VOLT_LOW_WARN				0x000E	/* Voltage Low Warning				*/
#define D_DU_REG_SFP_A2_BIAS_HIGH_ALM				0x0010	/* Bias High Alarm					*/
#define D_DU_REG_SFP_A2_BIAS_LOW_ALM				0x0012	/* Bias Low Alarm					*/
#define D_DU_REG_SFP_A2_BIAS_HIGH_WARN				0x0014	/* Bias High Warning				*/
#define D_DU_REG_SFP_A2_BIAS_LOW_WARN				0x0016	/* Bias Low Warning					*/
#define D_DU_REG_SFP_A2_TX_POW_HIGH_ALM				0x0018	/* TX Power High Alarm				*/
#define D_DU_REG_SFP_A2_TX_POW_LOW_ALM				0x001A	/* TX Power Low Alarm				*/
#define D_DU_REG_SFP_A2_TX_POW_HIGH_WARN			0x001C	/* TX Power High Warning			*/
#define D_DU_REG_SFP_A2_TX_POW_LOW_WARN				0x001E	/* TX Power Low Warning				*/
#define D_DU_REG_SFP_A2_RX_POW_HIGH_ALM				0x0020	/* RX Power High Alarm				*/
#define D_DU_REG_SFP_A2_RX_POW_LOW_ALM				0x0022	/* RX Power Low Alarm				*/
#define D_DU_REG_SFP_A2_RX_POW_HIGH_WARN			0x0024	/* RX Power High Warning			*/
#define D_DU_REG_SFP_A2_RX_POW_LOW_WARN				0x0026	/* RX Power Low Warning				*/

/* Optional A/W Thresholds			*/
#define D_DU_REG_SFP_A2_OPT_LASER_TEMP_HIGH_ALM		0x0028	/* Optional Laser Temp High Alarm	*/
#define D_DU_REG_SFP_A2_OPT_LASER_TEMP_LOW_ALM		0x002A	/* Optional Laser Temp Low Alarm	*/
#define D_DU_REG_SFP_A2_OPT_LASER_TEMP_HIGH_WARN	0x002C	/* Optional Laser Temp High Warning	*/
#define D_DU_REG_SFP_A2_OPT_LASER_TEMP_LOW_WARN		0x002E	/* Optional Laser Temp Low Warning	*/
#define D_DU_REG_SFP_A2_OPT_TEC_CUR_HIGH_ALM		0x0030	/* Optional TEC Current High Alarm	*/
#define D_DU_REG_SFP_A2_OPT_TEC_CUR_LOW_ALM			0x0032	/* Optional TEC Current Low Alarm	*/
#define D_DU_REG_SFP_A2_OPT_TEC_CUR_HIGH_WARN		0x0034	/* Optional TEC Current High Warning*/
#define D_DU_REG_SFP_A2_OPT_TEC_CUR_LOW_WARN		0x0036	/* Optional TEC Current Low Warning	*/

/* Diagnostics						*/
#define D_DU_REG_SFP_A2_TEMP						0x0060	/* Temperature						*/
#define D_DU_REG_SFP_A2_VCC							0x0062	/* Vcc								*/
#define D_DU_REG_SFP_A2_TX_BIAS						0x0064	/* TX Bias							*/
#define D_DU_REG_SFP_A2_TX_POW						0x0066	/* TX Power							*/
#define D_DU_REG_SFP_A2_RX_POW						0x0068	/* RX Power							*/

/* Optional Diagnostics				*/
#define D_DU_REG_SFP_A2_OPT_LASER_TEMP_WAVELENG		0x006A	/* Optional Laser Temp/Wavelength	*/
#define D_DU_REG_SFP_A2_OPT_TEC_CUR					0x006C	/* Optional TEC current				*/
/* @} */

/*!
 * @name SFP EEPROMレジスタサイズ
 * @note SFP EEPROMレジスタサイズ
 * @{
 */
#define D_DU_SIZE_SFP_A0_IDENTIFIER					0x0001	/* Identifier						*/
#define D_DU_SIZE_SFP_A0_EXT_IDENTIFIER				0x0001	/* Ext. Identifier					*/
#define D_DU_SIZE_SFP_A0_CONNECTOR					0x0001	/* Connector						*/
#define D_DU_SIZE_SFP_A0_TRANSCEIVER				0x0008	/* Transceiver						*/
#define D_DU_SIZE_SFP_A0_ENCODING					0x0001	/* Encoding							*/
#define D_DU_SIZE_SFP_A0_BR_NOMINAL					0x0001	/* BR, Nominal						*/
#define D_DU_SIZE_SFP_A0_RATE_ID					0x0001	/* Rate Identifier					*/
#define D_DU_SIZE_SFP_A0_LEN_SMF_KM					0x0001	/* Length (SMF,km)					*/
#define D_DU_SIZE_SFP_A0_LEN_SMF					0x0001	/* Length (SMF)						*/
#define D_DU_SIZE_SFP_A0_LEN_50UM					0x0001	/* Length (50um)					*/
#define D_DU_SIZE_SFP_A0_LEN_62_5UM					0x0001	/* Length (62.5um)					*/
#define D_DU_SIZE_SFP_A0_LEN_CABLE					0x0001	/* Length (OM4 or copper cable)		*/
#define D_DU_SIZE_SFP_A0_LEN_OM3					0x0001	/* Length (OM3)						*/
#define D_DU_SIZE_SFP_A0_VENDOR_NAME				0x0010	/* Vendor name						*/
#define D_DU_SIZE_SFP_A0_TRANSCEIVER_2				0x0001	/* Transceiver						*/
#define D_DU_SIZE_SFP_A0_VENDOR_OUI					0x0003	/* Vendor OUI						*/
#define D_DU_SIZE_SFP_A0_VENDOR_PN					0x0010	/* Vendor PN						*/
#define D_DU_SIZE_SFP_A0_VENDOR_REV					0x0004	/* Vendor rev						*/
#define D_DU_SIZE_SFP_A0_WAVELENGTH					0x0002	/* Wavelength						*/
#define D_DU_SIZE_SFP_A0_CC_BASE					0x0001	/* CC_BASE							*/
#define D_DU_SIZE_SFP_A0_OPTIONS					0x0002	/* Options							*/
#define D_DU_SIZE_SFP_A0_BR_MAX						0x0001	/* BR, max							*/
#define D_DU_SIZE_SFP_A0_BR_MIN						0x0001	/* BR, min							*/
#define D_DU_SIZE_SFP_A0_VENDOR_SN					0x0010	/* Vendor SN						*/
#define D_DU_SIZE_SFP_A0_DATE_CODE					0x0008	/* Date code						*/
#define D_DU_SIZE_SFP_A0_DIAG_MON_TYPE				0x0001	/* Diagnostic Monitoring Type		*/
#define D_DU_SIZE_SFP_A0_ENHANCED_OPT				0x0001	/* Enhanced Options					*/
#define D_DU_SIZE_SFP_A0_SFF8472_COMP				0x0001	/* SFF-8472 Compliance				*/
#define D_DU_SIZE_SFP_A0_CC_EXT						0x0001	/* CC_EXT							*/
#define D_DU_SIZE_SFP_A0_VENDOR_SPECIFIC			0x0020	/* Vendor Specific					*/
#define D_DU_SIZE_SFP_A0_RESERVED					0x0080	/* Reserved							*/

#define D_DU_SIZE_SFP_A2_AW_THR						0x0028	/* A/W Thresholds					*/
#define D_DU_SIZE_SFP_A2_OPT_AW_THR					0x0010	/* Optional A/W Thresholds			*/
#define D_DU_SIZE_SFP_A2_EXT_CAL_CONSTANTS			0x0024	/* Ext Cal Constants				*/
#define D_DU_SIZE_SFP_A2_CC_DMI						0x0001	/* CC_DMI							*/
#define D_DU_SIZE_SFP_A2_DIAGNOSTICS				0x000A	/* Diagnostics						*/
#define D_DU_SIZE_SFP_A2_OPT_DIAG					0x0004	/* Optional Diagnostics				*/
#define D_DU_SIZE_SFP_A2_STAT_CONT					0x0001	/* Status/Control					*/
#define D_DU_SIZE_SFP_A2_ALARM_FLAGS				0x0002	/* Alarm Flags						*/
#define D_DU_SIZE_SFP_A2_CDR_UNLOCKED				0x0001	/* CDR Unlocked						*/
#define D_DU_SIZE_SFP_A2_WARNING FLAGS				0x0002	/* Warning Flags					*/
#define D_DU_SIZE_SFP_A2_EXT_STAT_CONT				0x0002	/* Ext Status/Control				*/
#define D_DU_SIZE_SFP_A2_VENDOR_SPECIFIC			0x0007	/* Vendor Specific					*/
#define D_DU_SIZE_SFP_A2_TABLE_SELECT				0x0001	/* Table Select						*/
#define D_DU_SIZE_SFP_A2_USER_EEPROM				0x0078	/* User EEPROM						*/
#define D_DU_SIZE_SFP_A2_VENDOR_CONTROL				0x0008	/* Vendor Control					*/
#define D_DU_SIZE_SFP_A2_RX_DECISION_THR			0x0001	/* Rx Decision Threshold			*/

#define D_DU_SIZE_SFP_A2_TEMP_HIGH_ALM				0x0002	/* Temp High Alarm					*/
#define D_DU_SIZE_SFP_A2_TEMP_LOW_ALM				0x0002	/* Temp Low Alarm					*/
#define D_DU_SIZE_SFP_A2_TEMP_HIGH_WARN				0x0002	/* Temp High Warning				*/
#define D_DU_SIZE_SFP_A2_TEMP_LOW_WARN				0x0002	/* Temp Low Warning					*/
#define D_DU_SIZE_SFP_A2_VOLT_HIGH_ALM				0x0002	/* Voltage High Alarm				*/
#define D_DU_SIZE_SFP_A2_VOLT_LOW_ALM				0x0002	/* Voltage Low Alarm				*/
#define D_DU_SIZE_SFP_A2_VOLT_HIGH_WARN				0x0002	/* Voltage High Warning				*/
#define D_DU_SIZE_SFP_A2_VOLT_LOW_WARN				0x0002	/* Voltage Low Warning				*/
#define D_DU_SIZE_SFP_A2_BIAS_HIGH_ALM				0x0002	/* Bias High Alarm					*/
#define D_DU_SIZE_SFP_A2_BIAS_LOW_ALM				0x0002	/* Bias Low Alarm					*/
#define D_DU_SIZE_SFP_A2_BIAS_HIGH_WARN				0x0002	/* Bias High Warning				*/
#define D_DU_SIZE_SFP_A2_BIAS_LOW_WARN				0x0002	/* Bias Low Warning					*/
#define D_DU_SIZE_SFP_A2_TX_POW_HIGH_ALM			0x0002	/* TX Power High Alarm				*/
#define D_DU_SIZE_SFP_A2_TX_POW_LOW_ALM				0x0002	/* TX Power Low Alarm				*/
#define D_DU_SIZE_SFP_A2_TX_POW_HIGH_WARN			0x0002	/* TX Power High Warning			*/
#define D_DU_SIZE_SFP_A2_TX_POW_LOW_WARN			0x0002	/* TX Power Low Warning				*/
#define D_DU_SIZE_SFP_A2_RX_POW_HIGH_ALM			0x0002	/* RX Power High Alarm				*/
#define D_DU_SIZE_SFP_A2_RX_POW_LOW_ALM				0x0002	/* RX Power Low Alarm				*/
#define D_DU_SIZE_SFP_A2_RX_POW_HIGH_WARN			0x0002	/* RX Power High Warning			*/
#define D_DU_SIZE_SFP_A2_RX_POW_LOW_WARN			0x0002	/* RX Power Low Warning				*/

#define D_DU_SIZE_SFP_A2_OPT_LASER_TEMP_HIGH_ALM	0x0002	/* Optional Laser Temp High Alarm	*/
#define D_DU_SIZE_SFP_A2_OPT_LASER_TEMP_LOW_ALM		0x0002	/* Optional Laser Temp Low Alarm	*/
#define D_DU_SIZE_SFP_A2_OPT_LASER_TEMP_HIGH_WARN	0x0002	/* Optional Laser Temp High Warning	*/
#define D_DU_SIZE_SFP_A2_OPT_LASER_TEMP_LOW_WARN	0x0002	/* Optional Laser Temp Low Warning	*/
#define D_DU_SIZE_SFP_A2_OPT_TEC_CUR_HIGH_ALM		0x0002	/* Optional TEC Current High Alarm	*/
#define D_DU_SIZE_SFP_A2_OPT_TEC_CUR_LOW_ALM		0x0002	/* Optional TEC Current Low Alarm	*/
#define D_DU_SIZE_SFP_A2_OPT_TEC_CUR_HIGH_WARN		0x0002	/* Optional TEC Current High Warning*/
#define D_DU_SIZE_SFP_A2_OPT_TEC_CUR_LOW_WARN		0x0002	/* Optional TEC Current Low Warning	*/

#define D_DU_SIZE_SFP_A2_TEMP						0x0002	/* Temperature						*/
#define D_DU_SIZE_SFP_A2_VCC						0x0002	/* Vcc								*/
#define D_DU_SIZE_SFP_A2_TX_BIAS					0x0002	/* TX Bias							*/
#define D_DU_SIZE_SFP_A2_TX_POW						0x0002	/* TX Power							*/
#define D_DU_SIZE_SFP_A2_RX_POW						0x0002	/* RX Power							*/
#define D_DU_SIZE_SFP_A2_OPT_LASER_TEMP_WAVELENG	0x0002	/* Optional Laser Temp/Wavelength	*/
#define D_DU_SIZE_SFP_A2_OPT_TEC_CUR				0x0002	/* Optional TEC current				*/
/* @} */
#endif
/* @} */

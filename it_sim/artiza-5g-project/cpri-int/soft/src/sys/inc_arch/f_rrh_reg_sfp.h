/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_sfp.h
* @brief System共通定義(SFP)
 * @date  2015/10/15 ALPHA) fujii Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_SFP_H
#define F_RRH_REG_SFP_H

/*!
 * @name SFPアドレスオフセット(共通部)
 * @note SFPアクセス用のアドレスオフセット
 * @{
 */
/* SFP A0 */
#define	D_RRH_SFP_A0_SIZE					0x100				/* SFP A0 サイズ			*/

#define	D_RRH_SFP_A0_ID						0x000				/*	Identifier					*/
#define	D_RRH_SFP_A0_EXT_ID					0x001				/*	Ext. Identifier				*/
#define	D_RRH_SFP_A0_CONNECTOR				0x002				/*	Connector					*/
#define	D_RRH_SFP_A0_TRANSCEIVER			0x003				/*	Transceiver					*/
#define	D_RRH_SFP_A0_ENCODING				0x00B				/*	Encoding					*/
#define	D_RRH_SFP_A0_BR_NOMINAL				0x00C				/*	BR, Nominal					*/
#define	D_RRH_SFP_A0_RATE_ID				0x00D				/*	Rate Identifier				*/
#define	D_RRH_SFP_A0_LEN_SMF_KM				0x00E				/*	Length(SMF,km)				*/
#define	D_RRH_SFP_A0_LEN_SMF				0x00F				/*	Length (SMF)				*/
#define	D_RRH_SFP_A0_LEN_50UM				0x010				/*	Length (50um)				*/
#define	D_RRH_SFP_A0_LEN_625UM				0x011				/*	Length (62.5um)				*/
#define	D_RRH_SFP_A0_LEN_CABLE				0x012				/*	Length (cable)				*/
#define	D_RRH_SFP_A0_LEN_OM3				0x013				/*	Length (OM3)				*/
#define	D_RRH_SFP_A0_VENDOR_NAME			0x014				/*	Vendor name					*/
#define	D_RRH_SFP_A0_TRANSCEIVER2			0x024				/*	Transceiver					*/
#define	D_RRH_SFP_A0_VENDOR_OUI				0x025				/*	Vendor OUI					*/
#define	D_RRH_SFP_A0_VENDOR_PN				0x028				/*	Vendor PN					*/
#define	D_RRH_SFP_A0_VENDOR_REV				0x038				/*	Vendor rev					*/
#define	D_RRH_SFP_A0_WAVE_LENGTH			0x03C				/*	Wavelength					*/
#define	D_RRH_SFP_A0_UNALLOCATED			0x03E				/*	Unallocated					*/
#define	D_RRH_SFP_A0_CC_BASE				0x03F				/*	CC_BASE						*/
#define	D_RRH_SFP_A0_OPTIONS				0x040				/*	Options						*/
#define	D_RRH_SFP_A0_BR_MAX					0x042				/*	BR, max						*/
#define	D_RRH_SFP_A0_BR_MIN					0x043				/*	BR, min						*/
#define	D_RRH_SFP_A0_VENDOR_SN				0x044				/*	Vendor SN					*/
#define	D_RRH_SFP_A0_DATE_CODE				0x054				/*	Date code					*/
#define	D_RRH_SFP_A0_DIAG_MON_TYPE			0x05C				/*	Diagnostic Monitoring Type	*/
#define	D_RRH_SFP_A0_ENHANCED_OPTION		0x05D				/*	Enhanced Options			*/
#define	D_RRH_SFP_A0_SFF8472_COMP			0x05E				/*	SFF-8472 Compliance			*/
#define	D_RRH_SFP_A0_CC_EXT					0x05F				/*	CC_EXT						*/
#define	D_RRH_SFP_A0_VENDOR_SPEC			0x060				/*	Vendor Specific				*/
#define	D_RRH_SFP_A0_RX_RATE_SEL_RS0		0x06E				/*	RX RATE select				*/
#define	D_RRH_SFP_A0_RX_RATE_SEL_RS1		0x076				/*	RX RATE select				*/

#define	D_RRH_A0_VENDOR_SPEC_SIZE			32					/*	Vendor Specific(サイズ)		*/

/* SFP A2 */
#define	D_RRH_SFP_A2_SIZE					0x100				/* SFP A2 サイズ				*/

#define	D_RRH_SFP_A2_TEMP_HIGH_ALM			0x000				/* Temp High Alarm				*/
#define	D_RRH_SFP_A2_TEMP_LOW_ALM			0x002				/* Temp Low Alarm				*/
#define	D_RRH_SFP_A2_TEMP_HIGH_WARN			0x004				/* Temp High Warning			*/
#define	D_RRH_SFP_A2_TEMP_LOW_WARN			0x006				/* Temp Low Warning				*/
#define	D_RRH_SFP_A2_VOLTAGE_HIGH_ALM		0x008				/* Voltage High Alarm			*/
#define	D_RRH_SFP_A2_VOLTAGE_LOW_ALM		0x00A				/* Voltage Low Alarm			*/
#define	D_RRH_SFP_A2_VOLTAGE_HIGH_WARN		0x00C				/* Voltage High Warning			*/
#define	D_RRH_SFP_A2_VOLTAGE_LOW_WARN		0x00E				/* Voltage Low Warning			*/
#define	D_RRH_SFP_A2_BIAS_HIGH_ALM			0x010				/* Bias High Alarm				*/
#define	D_RRH_SFP_A2_BIAS_LOW_ALM			0x012				/* Bias Low Alarm				*/
#define	D_RRH_SFP_A2_BIAS_HIGH_WARN			0x014				/* Bias High Warning			*/
#define	D_RRH_SFP_A2_BIAS_LOW_WARN			0x016				/* Bias Low Warning				*/
#define	D_RRH_SFP_A2_TX_POWER_HIGH_ALM		0x018				/* TX Power High Alarm			*/
#define	D_RRH_SFP_A2_TX_POWER_LOW_ALM		0x01A				/* TX Power Low Alarm			*/
#define	D_RRH_SFP_A2_TX_POWER_HIGH_WARN		0x01C				/* TX Power High Warning		*/
#define	D_RRH_SFP_A2_TX_POWER_LOW_WARN		0x01E				/* TX Power Low Warning			*/
#define	D_RRH_SFP_A2_RX_POWER_HIGH_ALM		0x020				/* RX Power High Alarm			*/
#define	D_RRH_SFP_A2_RX_POWER_LOW_ALM		0x022				/* RX Power Low Alarm			*/
#define	D_RRH_SFP_A2_RX_POWER_HIGH_WARN		0x024				/* RX Power High Warning		*/
#define	D_RRH_SFP_A2_RX_POWER_LOW_WARN		0x026				/* RX Power Low Warning			*/
#define	D_RRH_SFP_A2_RX_PWR4				0x038				/* Rx_PWR(4)					*/
#define	D_RRH_SFP_A2_RX_PWR3				0x03C				/* Rx_PWR(3)					*/
#define	D_RRH_SFP_A2_RX_PWR2				0x040				/* Rx_PWR(2)					*/
#define	D_RRH_SFP_A2_RX_PWR1				0x044				/* Rx_PWR(1)					*/
#define	D_RRH_SFP_A2_RX_PWR0				0x048				/* Rx_PWR(0)					*/
#define	D_RRH_SFP_A2_TX_I_SLOPE				0x04C				/* Tx_I(Slope)					*/
#define	D_RRH_SFP_A2_TX_I_OFFSET			0x04E				/* Tx_I(Offset)					*/
#define	D_RRH_SFP_A2_TX_PWR_SLOPE			0x050				/* Tx_PWR(Slope)				*/
#define	D_RRH_SFP_A2_TX_PWR_OFFSET			0x052				/* Tx_PWR(Offset)				*/
#define	D_RRH_SFP_A2_T_SLOPE				0x054				/* T (Slope)					*/
#define	D_RRH_SFP_A2_T_OFFSET				0x056				/* T (Offset)					*/
#define	D_RRH_SFP_A2_V_SLOPE				0x058				/* V (Slope)					*/
#define	D_RRH_SFP_A2_V_OFFSET				0x05A				/* V (Offset)					*/
#define	D_RRH_SFP_A2_CC_DMI					0x05F				/* Check code for Base Diagnostic Fields (addresses 0 to 94)	*/
#define	D_RRH_SFP_A2_TEMPERATURE_MSB		0x060				/* Temperature MSB				*/
#define	D_RRH_SFP_A2_TEMPERATURE_LSB		0x061				/* Temperature LSB				*/
#define	D_RRH_SFP_A2_VCC_MSB				0x062				/* Vcc MSB						*/
#define	D_RRH_SFP_A2_VCC_LSB				0x063				/* Vcc LSB						*/
#define	D_RRH_SFP_A2_TX_BIAS_MSB			0x064				/* TX Bias MSB					*/
#define	D_RRH_SFP_A2_TX_BIAS_LSB			0x065				/* TX Bias LSB					*/
#define	D_RRH_SFP_A2_TX_POWER_MSB			0x066				/* TX Power MSB					*/
#define	D_RRH_SFP_A2_TX_POWER_LSB			0x067				/* TX Power LSB					*/
#define	D_RRH_SFP_A2_RX_POWER_MSB			0x068				/* RX Power MSB					*/
#define	D_RRH_SFP_A2_RX_POWER_LSB			0x069				/* RX Power LSB					*/
#define	D_RRH_SFP_A2_STS_CTRL				0x06E				/* Status/Control				*/
#define	D_RRH_SFP_A2_ALM_FLAGS1				0x070				/* Alarm Flags					*/
#define	D_RRH_SFP_A2_ALM_FLAGS2				0x071				/* Alarm Flags					*/
#define	D_RRH_SFP_A2_WARN_FLAGS1			0x074				/* Warning Flags				*/
#define	D_RRH_SFP_A2_WARN_FLAGS2			0x075				/* Warning Flags				*/
#define	D_RRH_SFP_A2_EXT_STS_CTRL			0x076				/* Ext Status/Control			*/
#define	D_RRH_SFP_A2_VENDOR_SPEC			0x078				/* Vendor specific memory addresses		*/
#define	D_RRH_SFP_A2_USER_EEP				0x080				/* User writable non-volatile memory	*/
#define	D_RRH_SFP_A2_VENDOR_CTRL			0x0F8				/* Vendor specific control addresses	*/

/* @} */
#endif
/* @} */


/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Constant Value Definition(Message ID)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also DUソフトウェア仕様書_Appendix_DUMT_IF仕様書.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MNT_MSG_INTERFACES_DEF_H
#define	F_MNT_MSG_INTERFACES_DEF_H

/** @addtogroup Message0x_interfaces
 *  @{
 */

#define	D_MSG_BODY_OPT_CONNECT_STT_REQ			0x3001U		/**< DU-MT接続開始要求  */
#define	D_MSG_BODY_OPT_CONNECT_STT_RSP			0x3002U		/**< DU-MT接続開始応答  */
#define	D_MSG_BODY_OPT_CONNECT_STP_REQ			0x3004U		/**< DU-MT接続停止要求  */
#define	D_MSG_BODY_OPT_CONNECT_STP_RSP			0x3005U		/**< DU-MT接続停止応答  */
#define	D_MSG_BODY_EEPROM_REG_READ_REQ			0x3011U		/**< インベントリ情報取得要求  */
#define	D_MSG_BODY_EEPROM_REG_READ_RSP			0x3012U		/**< インベントリ情報取得応答  */
#define	D_MSG_BODY_DUSTATUS_GET_REQ				0x3021U		/**< 装置情報取得要求  */
#define	D_MSG_BODY_DUSTATUS_GET_RSP				0x3022U		/**< 装置情報取得応答  */
#define	D_MSG_BODY_DUSTATUS_GET_MRU_RSP			0x3022U		/**< 装置情報取得応答（MRU用）  */
#define	D_MSG_BODY_FORCEALM_REQ					0x3031U		/**< 強制ALM発生要求  */
#define	D_MSG_BODY_FORCEALM_RSP					0x3032U		/**< 強制ALM発生応答  */
#define	D_MSG_BODY_CARDBLK_REQ					0x3041U		/**< 閉塞制御要求  */
#define	D_MSG_BODY_CARDBLK_RSP					0x3042U		/**< 閉塞制御応答  */
#define	D_MSG_BODY_CARDRST_REQ					0x3051U		/**< リセット制御要求  */
#define	D_MSG_BODY_CARDRST_RSP					0x3052U		/**< リセット制御応答  */
#define	D_MSG_BODY_PAONOFF_REQ					0x3061U		/**< PA ONOFF制御要求  */
#define	D_MSG_BODY_PAONOFF_RSP					0x3062U		/**< PA ONOFF制御応答  */
#define	D_MSG_BODY_NETCONF_AUTH_RESET_REQ		0x3071U		/**< NETCONF Auth Reset要求  */
#define	D_MSG_BODY_NETCONF_AUTH_RESET_RSP		0x3072U		/**< NETCONF Auth Reset応答  */
#define	D_MSG_BODY_POWADJ_WRITE_REQ				0x3081U		/**< 送信出力設定要求  */
#define	D_MSG_BODY_POWADJ_WRITE_RSP				0x3082U		/**< 送信出力設定応答  */
#define	D_MSG_BODY_BEAMID_SET_REQ				0x3091U		/**< BEAM ID設定要求  */
#define	D_MSG_BODY_BEAMID_SET_RSP				0x3092U		/**< BEAM ID設定応答  */
#define	D_MSG_BODY_SW_INSTALL_REQ				0x30A1U		/**< SWインストール要求  */
#define	D_MSG_BODY_SW_INSTALL_RSP				0x30A2U		/**< SWインストール応答  */
#define	D_MSG_BODY_AISG_SET_REQ					0x30B1U		/**< AISG設定要求  */
#define	D_MSG_BODY_AISG_SET_RSP					0x30B2U		/**< AISG設定応答  */
#define	D_MSG_BODY_EEPWRITE_REQ					0x30C1U		/**< EEP書き換え要求  */
#define	D_MSG_BODY_EEPWRITE_RSP					0x30C2U		/**< EEP書き換え応答  */

/** @} */

#endif	/* F_MNT_MSG_INTERFACES_DEF_H */

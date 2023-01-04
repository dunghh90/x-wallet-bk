/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also DUソフトウェア仕様書_Appendix_DUMT_IF仕様書.xlsm
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2018
 */
/******************************************************************************************************************************/

#ifndef	F_MNT_MSG_INTERFACES_TYP_H
#define	F_MNT_MSG_INTERFACES_TYP_H


/** @addtogroup Message0x_interfaces
 *  @{
 */

/**
 * @brief	(0x3001)DU-MT接続開始要求
 * @note	DU-MT接続開始要求を行う
 * 
 */
typedef struct{
	UINT				uiConnType;							/**< 接続種別  */
	UINT				uiCPRILinkNo;						/**< CPRI Link番号  */
	UINT				uiCarrierNo;						/**< キャリア番号  */
	UINT				uiSectorNo;							/**< セクタ番号  */
}__attribute__ ((packed)) T_MNT_BODY_OPT_CONNECT_STT_REQ;

/**
 * @brief	(0x3001)DU-MT接続開始要求
 * @note	DU-MT接続開始要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_OPT_CONNECT_STT_REQ	data;					/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_OPT_CONNECT_STT_REQ;

/**
 * @brief	(0x3002)DU-MT接続開始応答
 * @note	DU-MT接続開始応答を行う
 * 
 */
typedef struct{
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code  */
}__attribute__ ((packed)) T_MNT_BODY_OPT_CONNECT_STT_RSP;

/**
 * @brief	(0x3002)DU-MT接続開始応答
 * @note	DU-MT接続開始応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_OPT_CONNECT_STT_RSP	data;					/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_OPT_CONNECT_STT_RSP;

/**
 * @brief	(0x3004)DU-MT接続停止要求
 * @note	DU-MT接続停止要求を行う
 * 
 */
typedef struct{
	UINT				uiCauseCode;						/**< Cause  */
}__attribute__ ((packed)) T_MNT_BODY_OPT_CONNECT_STP_REQ;

/**
 * @brief	(0x3004)DU-MT接続停止要求
 * @note	DU-MT接続停止要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_OPT_CONNECT_STP_REQ	data;					/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_OPT_CONNECT_STP_REQ;

/**
 * @brief	(0x3005)DU-MT接続停止応答
 * @note	DU-MT接続停止応答を行う
 * 
 */
typedef struct{
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code  */
}__attribute__ ((packed)) T_MNT_BODY_OPT_CONNECT_STP_RSP;

/**
 * @brief	(0x3005)DU-MT接続停止応答
 * @note	DU-MT接続停止応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_OPT_CONNECT_STP_RSP	data;					/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_OPT_CONNECT_STP_RSP;

/**
 * @brief	(0x3011)インベントリ情報取得要求
 * @note	装置種別、インベントリ情報とビームフォーミング設定機能の有効/無効種別の取得要求を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
}__attribute__ ((packed)) T_MNT_BODY_EEPROM_REG_READ_REQ;

/**
 * @brief	(0x3011)インベントリ情報取得要求
 * @note	インベントリ情報取得要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_EEPROM_REG_READ_REQ	data;					/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_EEPROM_REG_READ_REQ;

/**
 * @brief	(0x3012)インベントリ情報取得応答
 * @note	装置種別、インベントリ情報とビームフォーミング設定機能の有効/無効種別の取得応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiOption;							/**< Option  */
	UINT				uiProductType;						/**< 装置種別  */
	UCHAR				ucProductName[12];					/**< Product Name  */
	UCHAR				ucSwVersion[2];						/**< SW Version  */
	UCHAR				ucHwVersion[2];						/**< HW Version  */
	UINT				uiBeamFormEnable;					/**< BeamForming Enable/Disable  */
	USHORT				usFaultTotalNum;					/**< 障害要因数  */
	USHORT				ucFaultIndexNum[32];				/**< 障害要因FaultIDIndex番号  */
	UCHAR				ucFaultName[32][48];				/**< 障害要因の名称  */
}__attribute__ ((packed)) T_MNT_BODY_EEPROM_REG_READ_RSP;

/**
 * @brief	(0x3012)インベントリ情報取得応答
 * @note	インベントリ情報取得応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_EEPROM_REG_READ_RSP	data;					/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_EEPROM_REG_READ_RSP;

/**
 * @brief	(0x3021)装置情報取得要求
 * @note	装置情報(装置状態、キャリア状態、送信出力情報、BEAM ID)取得要求を行う。
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
}__attribute__ ((packed)) T_MNT_BODY_DUSTATUS_GET_REQ;

/**
 * @brief	(0x3021)装置情報取得要求
 * @note	装置情報取得要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_DUSTATUS_GET_REQ	data;						/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_DUSTATUS_GET_REQ;

/**
 * @brief	(0x3022)装置情報取得応答
 * @note	装置情報(装置状態、キャリア状態、送信出力情報、BEAM ID)取得応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
	UCHAR				ucLeD1;								/**< LED1色  */
	UCHAR				ucLeD2;								/**< LED2色  */
	UCHAR				ucOperState;						/**< Config Mnagementのoper state  */
	UCHAR				ucAblblState;						/**< Config Mnagementのavailability state  */
	UCHAR				ucAdminState;						/**< Config Mnagementのadmin state  */
	UCHAR				ucUseState;							/**< Config Mnagementのusage state  */
	UCHAR				ucPowerState;						/**< Config Mnagementのpower state  */
	UCHAR				ucFanState;							/**< FANの障害状態  */
	UCHAR				ucPortState;						/**< 局舎監視IF入力状態  */
	UCHAR				ucFaultFactorNum;					/**< 障害要因数  */
	USHORT				ucFaultFactor[32];					/**< 障害要因  */
	UCHAR				ucPtpState;							/**< PTP状態  */
	UCHAR				ucLinkState;						/**< PORTのLINK状態  */
	UCHAR				ucSfpState;							/**< SFP状態  */
	UCHAR				ucOption;							/**< Option  */
	UCHAR				ucEF_type[16];						/**< SFP情報(伝送路損失タイプ)  */
	UCHAR				ucVendor[16];						/**< SFP情報(ベンダ情報)  */
	UCHAR				ucSerialNo[16];						/**< SFP情報(シリアル番号)  */
	UCHAR				ucCarrierState[4][4];				/**< Carrier状態  */
	UINT				uiFreqSnd[4];						/**< 送信周波数  */
	UINT				uiFreqRcv[4];						/**< 受信周波数  */
	UINT				uiBandwidth[4];						/**< 運用帯域幅  */
	UCHAR				ucBeamId;							/**< アンテナビームID  */
	UCHAR				ucSendOutVal;						/**< 送信出力調整値  */
	UINT				uiConnectionState;					/**< Connection state  */
}__attribute__ ((packed)) T_MNT_BODY_DUSTATUS_GET_RSP;

/**
 * @brief	(0x3022)装置情報取得応答
 * @note	装置情報取得応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_DUSTATUS_GET_RSP	data;						/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_DUSTATUS_GET_RSP;

/**
 * @brief	(0x3022)装置情報取得応答（MRU用）
 * @note	装置情報(装置状態、キャリア状態、送信出力情報、BEAM ID、AISG状態、PORT-INF詳細(ポート毎))取得応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
	UCHAR				ucLeD1;								/**< LED1色  */
	UCHAR				ucLeD2;								/**< LED2色  */
	UCHAR				ucOperState;						/**< Config Mnagementのoper state  */
	UCHAR				ucAblblState;						/**< Config Mnagementのavailability state  */
	UCHAR				ucAdminState;						/**< Config Mnagementのadmin state  */
	UCHAR				ucUseState;							/**< Config Mnagementのusage state  */
	UCHAR				ucPowerState;						/**< Config Mnagementのpower state  */
	UCHAR				ucFanState;							/**< FANの障害状態  */
	UCHAR				ucPortState;						/**< 局舎監視IF入力状態  */
	UCHAR				ucFaultFactorNum;					/**< 障害要因数  */
	USHORT				ucFaultFactor[32];					/**< 障害要因  */
	UCHAR				ucPtpState;							/**< PTP状態  */
	UCHAR				ucLinkState;						/**< PORTのLINK状態  */
	UCHAR				ucSfpState;							/**< SFP状態  */
	UCHAR				ucReserve;							/**< リザーブ  */
	UCHAR				ucEF_type[16];						/**< SFP情報(伝送路損失タイプ)  */
	UCHAR				ucVendor[16];						/**< SFP情報(ベンダ情報)  */
	UCHAR				ucSerialNo[16];						/**< SFP情報(シリアル番号)  */
	UCHAR				ucCarrierState[4][4];				/**< Carrier状態  */
	UINT				uiFreqSnd[4];						/**< 送信周波数  */
	UINT				uiFreqRcv[4];						/**< 受信周波数  */
	UINT				uiBandwidth[4];						/**< 運用帯域幅  */
	UCHAR				ucBeamId;							/**< アンテナビームID  */
	UCHAR				ucSendOutVal;						/**< 送信出力調整値(Ant一括調整用)  */
	UINT				uiConnectionState;					/**< Connection state  */
	UCHAR				uiAisgState;						/**< AISG状態  */
	UCHAR				ucPortInfState;						/**< PORT-INF詳細（全8ポート分）  */
	UCHAR				ucSendOutVal_ant[4];				/**< 送信出力調整値(Ant個別調整用)  */
}__attribute__ ((packed)) T_MNT_BODY_DUSTATUS_GET_MRU_RSP;

/**
 * @brief	(0x3022)装置情報取得応答（MRU用）
 * @note	装置情報取得応答（MRU用）(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_DUSTATUS_GET_MRU_RSP	data;					/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_DUSTATUS_GET_MRU_RSP;

/**
 * @brief	(0x3031)強制ALM発生要求
 * @note	強制ALMの発生をDU装置に要求する
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
}__attribute__ ((packed)) T_MNT_BODY_FORCEALM_REQ;

/**
 * @brief	(0x3031)強制ALM発生要求
 * @note	強制ALM発生要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_FORCEALM_REQ	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_FORCEALM_REQ;

/**
 * @brief	(0x3032)強制ALM発生応答
 * @note	強制ALMの発生に対する応答を返す
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
}__attribute__ ((packed)) T_MNT_BODY_FORCEALM_RSP;

/**
 * @brief	(0x3032)強制ALM発生応答
 * @note	強制ALM発生応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_FORCEALM_RSP	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_FORCEALM_RSP;

/**
 * @brief	(0x3041)閉塞制御要求
 * @note	装置のBLK/UNBLKの制御要求を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiState;							/**< Block  */
}__attribute__ ((packed)) T_MNT_BODY_CARDBLK_REQ;

/**
 * @brief	(0x3041)閉塞制御要求
 * @note	閉塞制御要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_CARDBLK_REQ	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_CARDBLK_REQ;

/**
 * @brief	(0x3042)閉塞制御応答
 * @note	装置のBLK/UNBLKの制御応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
}__attribute__ ((packed)) T_MNT_BODY_CARDBLK_RSP;

/**
 * @brief	(0x3042)閉塞制御応答
 * @note	閉塞制御応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_CARDBLK_RSP	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_CARDBLK_RSP;

/**
 * @brief	(0x3051)リセット制御要求
 * @note	装置のリセット制御要求を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiExchange;							/**< Exchange有り/無し  */
}__attribute__ ((packed)) T_MNT_BODY_CARDRST_REQ;

/**
 * @brief	(0x3051)リセット制御要求
 * @note	リセット制御要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_CARDRST_REQ	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_CARDRST_REQ;

/**
 * @brief	(0x3052)リセット制御応答
 * @note	装置のリセット制御応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
}__attribute__ ((packed)) T_MNT_BODY_CARDRST_RSP;

/**
 * @brief	(0x3052)リセット制御応答
 * @note	リセット制御応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_CARDRST_RSP	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_CARDRST_RSP;

/**
 * @brief	(0x3061)PA ONOFF制御要求
 * @note	PA ON/OFFの制御要求を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiBranch;							/**< PA ON/OFF branch no  */
	UINT				uiOnOff;							/**< PA ON/OFF  */
}__attribute__ ((packed)) T_MNT_BODY_PAONOFF_REQ;

/**
 * @brief	(0x3061)PA ONOFF制御要求
 * @note	PA ONOFF制御要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_PAONOFF_REQ	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_PAONOFF_REQ;

/**
 * @brief	(0x3062)PA ONOFF制御応答
 * @note	PA ON/OFFの制御応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
}__attribute__ ((packed)) T_MNT_BODY_PAONOFF_RSP;

/**
 * @brief	(0x3062)PA ONOFF制御応答
 * @note	PA ONOFF制御応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_PAONOFF_RSP	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_PAONOFF_RSP;

/**
 * @brief	(0x3071)NETCONF Auth Reset要求
 * @note	NETCONF Authenticationのデフォルト値設定要求を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
}__attribute__ ((packed)) T_MNT_BODY_NETCONF_AUTH_RESET_REQ;

/**
 * @brief	(0x3071)NETCONF Auth Reset要求
 * @note	NETCONF Auth Reset要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_NETCONF_AUTH_RESET_REQ	data;				/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_NETCONF_AUTH_RESET_REQ;

/**
 * @brief	(0x3072)NETCONF Auth Reset応答
 * @note	NETCONF Authenticationのデフォルト値設定応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
}__attribute__ ((packed)) T_MNT_BODY_NETCONF_AUTH_RESET_RSP;

/**
 * @brief	(0x3072)NETCONF Auth Reset応答
 * @note	NETCONF Auth Reset応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_NETCONF_AUTH_RESET_RSP	data;				/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_NETCONF_AUTH_RESET_RSP;

/**
 * @brief	(0x3081)送信出力設定要求
 * @note	送信出力設定の要求を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiSetVal;							/**< set Value  */
	UINT				uiBranch;							/**< branch no  */
}__attribute__ ((packed)) T_MNT_BODY_POWADJ_WRITE_REQ;

/**
 * @brief	(0x3081)送信出力設定要求
 * @note	送信出力設定要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_POWADJ_WRITE_REQ	data;						/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_POWADJ_WRITE_REQ;

/**
 * @brief	(0x3082)送信出力設定応答
 * @note	送信出力設定の応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
}__attribute__ ((packed)) T_MNT_BODY_POWADJ_WRITE_RSP;

/**
 * @brief	(0x3082)送信出力設定応答
 * @note	送信出力設定応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_POWADJ_WRITE_RSP	data;						/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_POWADJ_WRITE_RSP;

/**
 * @brief	(0x3091)BEAM ID設定要求
 * @note	アンテナビームパターン(ID)の設定要求を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiBeamId;							/**< Beam ID  */
}__attribute__ ((packed)) T_MNT_BODY_BEAMID_SET_REQ;

/**
 * @brief	(0x3091)BEAM ID設定要求
 * @note	BEAM ID設定要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_BEAMID_SET_REQ	data;						/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_BEAMID_SET_REQ;

/**
 * @brief	(0x3092)BEAM ID設定応答
 * @note	アンテナビームパターン(ID)の設定応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
}__attribute__ ((packed)) T_MNT_BODY_BEAMID_SET_RSP;

/**
 * @brief	(0x3092)BEAM ID設定応答
 * @note	BEAM ID設定応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_BEAMID_SET_RSP	data;						/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_BEAMID_SET_RSP;

/**
 * @brief	(0x30A1)SWインストール要求
 * @note	転送した対象ファイルの解凍・インストールを要求する
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UCHAR				ucFileName[32];						/**< 対象ファイルの名称  */
	UINT				uiFileSize;							/**< 対象ファイルのサイズ  */
}__attribute__ ((packed)) T_MNT_BODY_SW_INSTALL_REQ;

/**
 * @brief	(0x30A1)SWインストール要求
 * @note	SWインストール要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_SW_INSTALL_REQ	data;						/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_SW_INSTALL_REQ;

/**
 * @brief	(0x30A2)SWインストール応答
 * @note	転送した対象ファイルの解凍・インストールの結果応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
}__attribute__ ((packed)) T_MNT_BODY_SW_INSTALL_RSP;

/**
 * @brief	(0x30A2)SWインストール応答
 * @note	SWインストール応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_SW_INSTALL_RSP	data;						/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_SW_INSTALL_RSP;

/**
 * @brief	(0x30B1)AISG設定要求
 * @note	AISG機能の設定要求を行う（マクロ機用）
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiAisgStatus;						/**< AISG状態  */
}__attribute__ ((packed)) T_MNT_BODY_AISG_SET_REQ;

/**
 * @brief	(0x30B1)AISG設定要求
 * @note	AISG設定要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_AISG_SET_REQ	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_AISG_SET_REQ;

/**
 * @brief	(0x30B2)AISG設定応答
 * @note	AISG機能の設定応答を行う（マクロ機用）
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
}__attribute__ ((packed)) T_MNT_BODY_AISG_SET_RSP;

/**
 * @brief	(0x30B2)AISG設定応答
 * @note	AISG設定応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_AISG_SET_RSP	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_AISG_SET_RSP;

/**
 * @brief	(0x30C1)EEP書き換え要求
 * @note	RU装置にEEPの書き換えの要求を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UCHAR				ucWrtOffset;						/**< 書き換えオフセット  */
	UCHAR				ucWrtSize;							/**< 書き換えサイズ  */
	UCHAR				ucWrtVal[16];						/**< 書き換え値  */
}__attribute__ ((packed)) T_MNT_BODY_EEPWRITE_REQ;

/**
 * @brief	(0x30C1)EEP書き換え要求
 * @note	EEP書き換え要求(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_EEPWRITE_REQ	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_EEPWRITE_REQ;

/**
 * @brief	(0x30C2)EEP書き換え応答
 * @note	RU装置からEEPの書き換えの設定応答を行う
 * 
 */
typedef struct{
	UINT				uiFuncID;							/**< Function ID  */
	UINT				uiSeqNo;							/**< Sequence NO  */
	UINT				uiOK_NG;							/**< OK/NG  */
	UINT				uiNG_Code;							/**< NG Code   */
	USHORT				usWrtOffset;						/**< 書き換えオフセット（操作種別）  */
}__attribute__ ((packed)) T_MNT_BODY_EEPWRITE_RSP;

/**
 * @brief	(0x30C2)EEP書き換え応答
 * @note	EEP書き換え応答(header + data)
 * 
 */
typedef struct{
	T_MSGHEAD			msgHead;							/**< Message header  */
	T_MNT_BODY_EEPWRITE_RSP	data;							/**< Message data  */
}__attribute__ ((packed)) T_MSG_BODY_EEPWRITE_RSP;


/** @} */

#endif	/* F_MNT_MSG_INTERFACES_TYP_H */

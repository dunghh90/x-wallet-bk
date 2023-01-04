/*!
 * @skip   $Id$
 * @file   cmd_eeprom_disp.c
 * @brief  EEPROM内容表示
 * @date   2013/11/21 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2016
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */


#include	"f_dbg_cmd_inc.h"									/*!< DEBGU共通ヘッダ	*/


/**
 * @brief EEPROM項目定義テーブル型
 * @note EEPROM項目定義テーブルの内容を定義する
 *
 */
typedef struct{
	const char					*eep_item;					/*!< EEPROM item				*/
	unsigned short				eep_type;					/*!< EEPROM type				*/
	unsigned short				eep_read_sta;				/*!< EEPROM read position(start)*/
	unsigned short				eep_read_end;				/*!< EEPROM read position(end)	*/
}T_DBG_EEP_DISP_DATA;

/*
 * @brief 変換種別定義
 * @note 変換方法を定義する
 */
enum
{
	DISP_STRING =0,											/**< 文字列表示形式				*/
	DISP_HEX,												/**< １６進数表示形式			*/
	DISP_MACADDR,											/**< MACアドレス表示形式		*/
	DISP_IPADDR,											/**< IPアドレス表示形式			*/
	DISP_SNO,												/**< シリアルNo(BCD)表示形式	*/
	DISP_VER,												/**< バージョン表示形式			*/
	DISP_DATE_BCD,											/**< 年月日(BCD)表示形式		*/
	DISP_DATE_HEX,											/**< 年月日(HEX)表示形式		*/
	DISP_TIME,												/**< 時分秒ms(HEX)表示形式		*/
	DISP_DEV,												/**< デバイス種別表示形式		*/
	DISP_SIGN10,											/**< 符号数値(10bit)表示形式	*/
	DISP_MAX												/**< 表示形式最大数				*/
};

/*
 * @name EEPROM 項目定義テーブル
 * @note EEPROMの項目名とアドレスの対応表
 */
const	T_DBG_EEP_DISP_DATA		f_dbgr_eepDisp[] =
{	/* Item name,											Display type,	(start),	(end) */
	{ "Card Name                                           : ",	DISP_STRING,	0x000, 0x00b	 },	/**< カード品名(ASCII)	*/
	{ "Card Revision(ASCII)                                : ",	DISP_STRING,	0x010, 0x01b	 },	/**< カード図番	*/
	{ "Manufacture Number(Year/month/Serial)(BCD)          : ",	DISP_SNO,		0x020, 0x027	 },	/**< 製造番号(年/月 シリアルNo)	*/
	{ "Test Date(Year/month/day)                           : ",	DISP_DATE_BCD,	0x028, 0x02b	 },	/**< 試験日(年月日)	*/
	{ "Hard Version(HEX)                                   : ",	DISP_HEX,		0x02c, 0x02f	 },	/**< ハードバージョン(総合版数1～3桁目)	*/
	{ "Hard Version(ASCII)                                 : ",	DISP_STRING,	0x030, 0x031	 },	/**< ハードバージョン(総合版数4～5桁目)	*/
	{ "Device kind                                         : ",	DISP_DEV,		0x040, 0x041	 },	/**< 装置種別	*/
	{ "Broad Version                                       : ",	DISP_HEX,		0x042, 0x042	 },	/**< ボードバージョン	*/
	{ "FLASH start up side1                                : ",	DISP_HEX,		0x050, 0x050	 },	/**< Flash起動面情報1	*/
	{ "FLASH start up side2(for soft Parameter)            : ",	DISP_HEX,		0x051, 0x051	 },	/**< Flash起動面情報2 Softパラメータ用	*/
	{ "Start up busy counter                               : ",	DISP_HEX,		0x060, 0x060	 },	/**< 起動Busyカウンタ	*/
	{ "Flash#0 File Set Version                            : ",	DISP_VER,		0x070, 0x075	 },	/**< FLASH 0面ファイルセット1 代表バージョン	*/
	{ "Flash#0 Soft Version                                : ",	DISP_HEX,		0x076, 0x077	 },	/**< FLASH 0面ソフトバージョン	*/
	{ "Flash#0 PL-FPGA Version                             : ",	DISP_HEX,		0x078, 0x079	 },	/**< FLASH 0面PL-FPGAバージョン	*/
	{ "Flash#0 TRX-FPGA Version                            : ",	DISP_HEX,		0x07a, 0x07b	 },	/**< FLASH 0面TRX-FPGAバージョン	*/
	{ "Flash#0 RFIC-boot Version                           : ",	DISP_HEX,		0x07c, 0x07d	 },	/**< FLASH 0面RFIC-bootバージョン	*/
	{ "Flash#0 RFIC-ARM Version                            : ",	DISP_HEX,		0x07e, 0x07f	 },	/**< FLASH 0面RFIC-ARMバージョン	*/
	{ "Flash#0 Soft Parameter Version                      : ",	DISP_HEX,		0x080, 0x081	 },	/**< FLASH 0面Soft パラメータバージョン	*/
	{ "Flash#0 PTP Parameter Version                       : ",	DISP_HEX,		0x082, 0x083	 },	/**< FLASH 0面PTPパラメータバージョン	*/
	{ "Flash#0 Soft Erase Flag                             : ",	DISP_HEX,		0x096, 0x096	 },	/**< FLASH 0面ソフトイレースフラグ	*/
	{ "Flash#0 PL-FPGA Erase Flag                          : ",	DISP_HEX,		0x097, 0x097	 },	/**< FLASH 0面PL-FPGAイレースフラグ	*/
	{ "Flash#0 TRX-FPGA Erase Flag                         : ",	DISP_HEX,		0x098, 0x098	 },	/**< FLASH 0面TRX-FPGAイレースフラグ	*/
	{ "Flash#0 RFIC-boot Erase Flag                        : ",	DISP_HEX,		0x099, 0x099	 },	/**< FLASH 0面RFIC-bootイレースフラグ	*/
	{ "Flash#0 RFIC-ARM Erase Flag                         : ",	DISP_HEX,		0x09a, 0x09a	 },	/**< FLASH 0面RFIC-ARMイレースフラグ	*/
	{ "Flash#0 Soft Parameter Erase Flag                   : ",	DISP_HEX,		0x09b, 0x09b	 },	/**< FLASH 0面Soft パラメータイレースフラグ	*/
	{ "Flash#0 PTP Parameter Erase Flag                    : ",	DISP_HEX,		0x09c, 0x09c	 },	/**< FLASH 0面PTPパラメータイレースフラグ	*/
	{ "Flash#1 File Set Version                            : ",	DISP_VER,		0x0b0, 0x0b5	 },	/**< FLASH 1面ファイルセット1 代表バージョン	*/
	{ "Flash#1 Soft Version                                : ",	DISP_HEX,		0x0b6, 0x0b7	 },	/**< FLASH 1面ソフトバージョン	*/
	{ "Flash#1 PL-FPGA Version                             : ",	DISP_HEX,		0x0b8, 0x0b9	 },	/**< FLASH 1面PL-FPGAバージョン	*/
	{ "Flash#1 TRX-FPGA Version                            : ",	DISP_HEX,		0x0ba, 0x0bb	 },	/**< FLASH 1面TRX-FPGAバージョン	*/
	{ "Flash#1 RFIC-boot Version                           : ",	DISP_HEX,		0x0bc, 0x0bd	 },	/**< FLASH 1面RFIC-bootバージョン	*/
	{ "Flash#1 RFIC-ARM Version                            : ",	DISP_HEX,		0x0be, 0x0bf	 },	/**< FLASH 1面RFIC-ARMバージョン	*/
	{ "Flash#1 Soft Parameter Version                      : ",	DISP_HEX,		0x0c0, 0x0c1	 },	/**< FLASH 1面Soft パラメータバージョン	*/
	{ "Flash#1 PTP Parameter Version                       : ",	DISP_HEX,		0x0c2, 0x0c3	 },	/**< FLASH 1面PTPパラメータバージョン	*/
	{ "Flash#1 Soft Erase Flag                             : ",	DISP_HEX,		0x0d6, 0x0d6	 },	/**< FLASH 1面ソフトイレースフラグ	*/
	{ "Flash#1 PL-FPGA Erase Flag                          : ",	DISP_HEX,		0x0d7, 0x0d7	 },	/**< FLASH 1面PL-FPGAイレースフラグ	*/
	{ "Flash#1 TRX-FPGA Erase Flag                         : ",	DISP_HEX,		0x0d8, 0x0d8	 },	/**< FLASH 1面TRX-FPGAイレースフラグ	*/
	{ "Flash#1 RFIC-boot Erase Flag                        : ",	DISP_HEX,		0x0d9, 0x0d9	 },	/**< FLASH 1面RFIC-bootイレースフラグ	*/
	{ "Flash#1 RFIC-ARM Erase Flag                         : ",	DISP_HEX,		0x0da, 0x0da	 },	/**< FLASH 1面RFIC-ARMイレースフラグ	*/
	{ "Flash#1 Soft Parameter Erase Flag                   : ",	DISP_HEX,		0x0db, 0x0db	 },	/**< FLASH 1面Soft パラメータイレースフラグ	*/
	{ "Flash#1 PTP Parameter Erase Flag                    : ",	DISP_HEX,		0x0dc, 0x0dc	 },	/**< FLASH 1面PTPパラメータイレースフラグ	*/
	{ "Flash#0 Soft File Size                              : ",	DISP_HEX,		0x130, 0x133	 },	/**< FLASH 0面ソフトファイルサイズ	*/
	{ "Flash#1 Soft File Size                              : ",	DISP_HEX,		0x134, 0x137	 },	/**< FLASH 1面ソフトファイルサイズ	*/
	{ "Flash#0 PL-FPGA File Size                           : ",	DISP_HEX,		0x13c, 0x13f	 },	/**< FLASH 0面PL-FPGAファイルサイズ	*/
	{ "Flash#1 PL-FPGA File Size                           : ",	DISP_HEX,		0x140, 0x143	 },	/**< FLASH 1面PL-FPGAファイルサイズ	*/
	{ "Flash#0 TRX-FPGA File Size                          : ",	DISP_HEX,		0x148, 0x14b	 },	/**< FLASH 0面TRX-FPGAファイルサイズ	*/
	{ "Flash#1 TRX-FPGA File Size                          : ",	DISP_HEX,		0x14c, 0x14f	 },	/**< FLASH 1面TRX-FPGAファイルサイズ	*/
	{ "Flash#0 RFIC-boot File Size                         : ",	DISP_HEX,		0x154, 0x157	 },	/**< FLASH 0面RFIC-bootファイルサイズ	*/
	{ "Flash#1 RFIC-boot File Size                         : ",	DISP_HEX,		0x158, 0x15b	 },	/**< FLASH 1面RFIC-bootファイルサイズ	*/
	{ "Flash#0 RFIC-ARM File Size                          : ",	DISP_HEX,		0x160, 0x163	 },	/**< FLASH 0面RFIC-ARMファイルサイズ	*/
	{ "Flash#1 RFIC-ARM File Size                          : ",	DISP_HEX,		0x164, 0x167	 },	/**< FLASH 1面RFIC-ARMファイルサイズ	*/
	{ "Flash#0 Soft Parameter File Size                    : ",	DISP_HEX,		0x16c, 0x16f	 },	/**< FLASH 0面Soft パラメータファイルサイズ	*/
	{ "Flash#1 Soft Parameter File Size                    : ",	DISP_HEX,		0x170, 0x173	 },	/**< FLASH 1面Soft パラメータファイルサイズ	*/
	{ "Slot#0 Status                                       : ",	DISP_HEX,		0x17c, 0x17c	 },	/**< Slot#0の状態(O_RAN spec Softwaremanagement用)	*/
	{ "Slot#1 Status                                       : ",	DISP_HEX,		0x17d, 0x17d	 },	/**< Slot#2の状態(O_RAN spec Softwaremanagement用)	*/
	{ "Slot#0 Build-name(ASCII)                            : ",	DISP_STRING,	0x180, 0x19f	 },	/**< Slot#0 Build-name	*/
	{ "Flash#0 Build File Size                             : ",	DISP_HEX,		0x1a2, 0x1a5	 },	/**< FLASH 0面PTP パラメータファイルサイズ	*/
	{ "Flash#1 Build File Size                             : ",	DISP_HEX,		0x1a6, 0x1a9	 },	/**< FLASH 1面PTP パラメータファイルサイズ	*/
	{ "Flash#0 PTP Parameter File Size                     : ",	DISP_HEX,		0x1ae, 0x1b1	 },	/**< FLASH 0面PTP パラメータファイルサイズ	*/
	{ "Flash#1 PTP Parameter File Size                     : ",	DISP_HEX,		0x1b2, 0x1b5	 },	/**< FLASH 1面PTP パラメータファイルサイズ	*/
	{ "Slot#1 Build-name(ASCII)                            : ",	DISP_STRING,	0x1c0, 0x1df	 },	/**< Slot#1 Build-name	*/
	{ "MAC Address(Ether(MT-PORT))                         : ",	DISP_MACADDR,	0x200, 0x205	 },	/**< Ether(MT-PORT)情報:MACアドレス	*/
	{ "IP Address(Ether(MT-PORT))                          : ",	DISP_IPADDR,	0x206, 0x209	 },	/**< Ether(MT-PORT)情報:IPアドレス	*/
	{ "MAC Address(eCPRI_Port#1)                           : ",	DISP_MACADDR,	0x220, 0x225	 },	/**< eCPRI_Port#1:MACアドレス	*/
	{ "IP Address(eCPRI_Port#1)                            : ",	DISP_IPADDR,	0x226, 0x229	 },	/**< eCPRI_Port#1:IPアドレス	*/
	{ "Low VID                                             : ",	DISP_HEX,		0x320, 0x321	 },	/**< システムパラメータ:LowVID	*/
	{ "High VID                                            : ",	DISP_HEX,		0x322, 0x323	 },	/**< システムパラメータ:HighVID	*/
	{ "Last Used VID                                       : ",	DISP_HEX,		0x324, 0x325	 },	/**< システムパラメータ:LastUsedVID	*/
	{ "PTP Config Pattern                                  : ",	DISP_HEX,		0x326, 0x326	 },	/**< システムパラメータ:PTPコンフィグパターン	*/
	{ "m-plane-marking (pcp)                               : ",	DISP_HEX,		0x327, 0x327	 },	/**< m-plane-marking	*/
	{ "VLAN scan excution Flag                             : ",	DISP_HEX,		0x328, 0x328	 },	/**< VLAN scan実行モード	*/
	{ "ssh-port area usable flag (1:usable)                : ",	DISP_HEX,		0x329, 0x329	 },	/**< ssh-port有効フラグ	*/
	{ "call-home-ssh-port                                  : ",	DISP_HEX,		0x32a, 0x32b	 },	/**< call-home-ssh-port	*/
	{ "server-ssh-port                                     : ",	DISP_HEX,		0x32c, 0x32d	 },	/**< server-ssh-port	*/
	{ "callhome retry timer                                : ",	DISP_HEX,		0x32e, 0x32f	 },	/**< callhome retry timer	*/
	{ "domain-number                                       : ",	DISP_HEX,		0x370, 0x370	 },	/**< システムパラメータ:domain-number	*/
	{ "multicast-mac-address                               : ",	DISP_HEX,		0x371, 0x371	 },	/**< システムパラメータ:multicast-mac-address	*/
	{ "accepted-clock-classes                              : ",	DISP_HEX,		0x372, 0x372	 },	/**< システムパラメータ:accepted-clock-classes	*/
	{ "TEMP ALM Threshold                                  : ",	DISP_HEX,		0x380, 0x380	 },	/**< 温度ALM閾値	*/
	{ "PA ON/OFF                                           : ",	DISP_HEX,		0x381, 0x381	 },	/**< PA ON/OFF制御Status	*/
	{ "GAIN adjust(ALL ANT)                                : ",	DISP_HEX,		0x382, 0x382	 },	/**< ゲイン微調設定値(Ant共通)	*/
	{ "RX CAL CMP flag                                     : ",	DISP_HEX,		0x383, 0x383	 },	/**< RxCalトレーニング完了フラグ	*/
	{ "Beamforming envelope mode (1:new BF mode)           : ",	DISP_HEX,		0x385, 0x385	 },	/**< mmW BeamID 新旧モード設定	*/
	{ "FAN spin mode (1:Low Speed)                         : ",	DISP_HEX,		0x387, 0x387	 },	/**< FAN回転数モード設定	*/
	{ "mmW training complete RF board Serial No            : ",	DISP_SNO,		0x391, 0x398	 },	/**< mmWトレーニング完了 RFボードシリアルNo	*/
	{ "T3 Adjustment value X                               : ",	DISP_SIGN10,	0x3a6, 0x3a7	 },	/**< T3補正値X	*/
	{ "T2 Adjustment value Y                               : ",	DISP_SIGN10,	0x3a8, 0x3a9	 },	/**< T2補正値Y	*/
	{ "SFP Failsafe AB Counter                             : ",	DISP_HEX,		0x3aa, 0x3aa	 },	/**< SFPフェールセーフ ABカウンター	*/
	{ "SFP Failsafe ABCD Counter                           : ",	DISP_HEX,		0x3ab, 0x3ab	 },	/**< SFPフェールセーフ ABCDカウンター	*/
	{ "DHCP option(61) identifier                          : ",	DISP_HEX,		0x3b0, 0x3b7	 },	/**< DHCP option(61) identifier	*/
	{ "DHCP vaild flag                                     : ",	DISP_HEX,		0x3c1, 0x3c1	 },	/**< DHCP有無フラグ	*/
	{ "PTP device access control flag                      : ",	DISP_HEX,		0x3c2, 0x3c2	 },	/**< PTPdeviceアクセス抑止フラグ	*/
	{ "PTP master mode flag                                : ",	DISP_HEX,		0x3c3, 0x3c3	 },	/**< PTP masterモード	*/
	{ "SyncE ESMC send valid flag                          : ",	DISP_HEX,		0x3cb, 0x3cb	 },	/**< SyncE ESMC送信フラグ	*/
	{ "SSH user account info. Flash write Flag             : ",	DISP_HEX,		0x3ce, 0x3ce	 },	/**< SSHユーザーアカウント情報FLASH書込みフラグ	*/
	{ "searchable-access-vlans Flash write Flag            : ",	DISP_HEX,		0x3cf, 0x3cf	 },	/**< VLAN情報FLASH書込みフラグ	*/
	{ "Soft reset mask                                     : ",	DISP_HEX,		0x3d0, 0x3d0	 },	/**< ソフトリセットマスク	*/
	{ "Soft reset flag                                     : ",	DISP_HEX,		0x3d1, 0x3d1	 },	/**< ソフトリセットフラグ	*/
	{ "ALM self reset flag                                 : ",	DISP_HEX,		0x3d2, 0x3d2	 },	/**< ALM自律リセット有無フラグ	*/
	{ "Trouble log save side                               : ",	DISP_HEX,		0x3d3, 0x3d3	 },	/**< 障害ログ退避面	*/
	{ "Cyclic log save side                                : ",	DISP_HEX,		0x3d4, 0x3d4	 },	/**< サイクリックログ退避面	*/
	{ "Cyclic log erase state#0                            : ",	DISP_HEX,		0x3d5, 0x3d5	 },	/**< サイクリックログClear状態#0	*/
	{ "Cyclic log erase state#1                            : ",	DISP_HEX,		0x3d6, 0x3d6	 },	/**< サイクリックログClear状態#1	*/
	{ "Save Time valid flag                                : ",	DISP_HEX,		0x3d7, 0x3d7	 },	/**< 時刻退避フラグ(1：有効、1以外無効) */
	{ "Save Time(Year/Month/Day)                           : ",	DISP_DATE_HEX,	0x3d8, 0x3db	 },	/**< 時刻退避領域(年/月/日) */
	{ "Save Time(Hour:Min:Sec.10ms)                        : ",	DISP_TIME,		0x3dc, 0x3df	 },	/**< 時刻退避領域(時:分:秒.10ms) */
	{ "PC Connection flag                                  : ",	DISP_HEX,		0x3e0, 0x3e0	 },	/**< PC接続フラグ	*/
	{ "M-plane connection valid flag                       : ",	DISP_HEX,		0x3e1, 0x3e1	 },	/**< M-plane接続抑止フラグ	*/
	{ "Destination IP address                              : ",	DISP_IPADDR,	0x3e2, 0x3e5	 },	/**< 接続先IPアドレス	*/
	{ "PGC Log Get Mode Flag                               : ",	DISP_HEX,		0x3e6, 0x3e6	 },	/**< PGC ログ取得モード	*/
	{ "Beamforming Mode                                    : ",	DISP_HEX,		0x3e7, 0x3e7	 },	/**< ビームフォーミングモード	*/
	{ "M-plane Dump Mode                                   : ",	DISP_HEX,		0x3e8, 0x3e8	 },	/**< ダンプモード	*/
	{ "S-plane Dump Mode                                   : ",	DISP_HEX,		0x3e9, 0x3e9	 },	/**< ダンプモード	*/
	{ "Debug Msg Dump Mode                                 : ",	DISP_HEX,		0x3ea, 0x3ea	 },	/**< ダンプモード	*/
	{ "AISG Dump Mode                                      : ",	DISP_HEX,		0x3eb, 0x3eb	 },	/**< ダンプモード	*/
	{ "SFN Offset Alpha                                    : ",	DISP_HEX,		0x3f1, 0x3f3	 },	/**< SFN_Offset_α */
	{ "SFN Offset Beta                                     : ",	DISP_HEX,		0x3f4, 0x3f5	 },	/**< SFN_Offset_β */
	{ "frequency set mode                                  : ",	DISP_HEX,		0x3f6, 0x3f6	 },	/**< 中心周波数設定モード */
	{ "frequency number                                    : ",	DISP_HEX,		0x3f7, 0x3fa	 },	/**< 中心周波数 */
	{ "mmW_RXPLL_0x24                                      : ",	DISP_HEX,		0x3fb, 0x3fb	 },	/**< mmW_RXPLL_0x24 */
	{ "mmW_RXPLL_0x2A                                      : ",	DISP_HEX,		0x3fc, 0x3fd	 },	/**< mmW_RXPLL_0x2A */
	{ "mmW_RXPLL_0x2B                                      : ",	DISP_HEX,		0x3fe, 0x3ff	 },	/**< mmW_RXPLL_0x2B */
	{ 0, 0, 0, 0 } /* */
};


/* Invent Info エンディアン変換情報 */
const UCHAR f_dbgr_invShortInfo[D_RRH_I2C_EEP_DATANUM/4] =
{
/* CHARエリア:0 SHORTエリア:1 */
/*  0  2  4  6  8  a  c  e  */ 
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0000 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0010 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0020 ～ */
	1, 0, 0, 0, 0, 0, 0, 0, /* addr 0030 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0040 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0050 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0060 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 0070 ～ */
	1, 1, 0, 0, 0, 0, 0, 0, /* addr 0080 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0090 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00a0 ～ */
	1, 1, 1, 1, 1, 1, 1, 1, /* addr 00b0 ～ */
	1, 1, 0, 0,	0, 0, 0, 0, /* addr 00c0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00d0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 00f0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0100 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0110 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0120 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0130 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0140 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0150 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0160 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0170 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0180 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0190 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01a0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01b0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01c0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01d0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 01f0 ～ */
	1, 1, 1, 1, 1, 0, 0, 0, /* addr 0200 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0210 ～ */
	1, 1, 1, 1, 1, 0, 0, 0, /* addr 0220 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0230 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0240 ～ */
	0, 0, 0, 0,	0, 0, 0, 0, /* addr 0250 ～ */
	1, 1, 1, 1, 1, 0, 0, 0, /* addr 0260 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0270 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0280 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0290 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02a0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02b0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02c0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02d0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 02f0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0300 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0310 ～ */
	1, 1, 1, 0, 0, 0, 0, 1, /* addr 0320 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0330 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0340 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0350 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0360 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0370 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0380 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 0390 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 03a0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 03b0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 03c0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0, /* addr 03d0 ～ */
	0, 1, 1, 0, 0, 0, 0, 0, /* addr 03e0 ～ */
	0, 0, 0, 0, 0, 0, 0, 0  /* addr 03f0 ～ */
};


/*!
 * @brief 関数機能概要:EEPROM内容表示メイン関数
 * @note  関数処理内容.
 *       -# EEPROMよりデータを読み込み表示する
 * @param  argc			[in]int		コマンドアーギュメント数
 * @param  argv			[in]char*[]	コマンドアーギュメント(パラメータ無し)
 * @param  outfp		[in]FILE*	出力先ファイルポインタ
 * @return INT
 * @retval RTC_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date 2013/11/22 ALPHA)宮崎 create
 */
int cmd_eeprom_disp_main( int argc, char *argv[], FILE *outfp )	{

	unsigned char		value;
	unsigned short		usCnt;
	unsigned short		eep_num;
	int					readst;
	unsigned char		temp[8];
	unsigned short		year;
	short int			adj;
	unsigned char		devNum[5] = {0x10, 0x11, 0x12, 0x13, 0x14};
	unsigned char		devName[6][8] = {"Proto", "sub6evl", "mmWevl", "sub6", "mmW", "unkown"};
	unsigned char		frqNum[3] = {0x10, 0x11, 0x12};
	unsigned char		frqName[4][7] = {"4.5G", "3.7G", "28G", "UNKOWN"};
	unsigned short		devLp, frqLp;
	unsigned short		shmF = 0;
	unsigned char		*shmem = NULL;
	int					iErrCode;

	if( argc > 1 )
	{
		shmF = atoi(argv[1]);
	}
	if(shmF == 1)
	{
		BPF_RU_IPCM_PROCSHM_ADDRGET( E_RRH_SHMID_APL_INVENTORY, (VOID **)&shmem, &iErrCode );
		fprintf(outfp, "#### INVENTORY[shared memory] display ####\n");
	}
	else
	{
		fprintf(outfp, "#### INVENTORY[eeprom] display ####\n");
	}

	/* EEPROM項目数分ループする */
	for( eep_num = 0;; eep_num++ )
	{
        if( f_dbgr_eepDisp[eep_num].eep_read_sta == 0 &&
             f_dbgr_eepDisp[eep_num].eep_read_end == 0    ) {
            break;
        }

		/* EEPROMの項目名を表示 */
		fprintf(outfp, "[%04x-%04x] %s",f_dbgr_eepDisp[eep_num].eep_read_sta, f_dbgr_eepDisp[eep_num].eep_read_end, f_dbgr_eepDisp[eep_num].eep_item);

		for( usCnt =  f_dbgr_eepDisp[eep_num].eep_read_sta;
			 usCnt <= f_dbgr_eepDisp[eep_num].eep_read_end;
			 usCnt ++ )
		{
			if(shmF == 1)
			{
				if(f_dbgr_invShortInfo[usCnt/2] != 0)
				{
					if(usCnt%2 == 0)
					{
						/*	偶数アドレスは+1のアドレスのデータを取得	*/
						value = *(shmem + (usCnt+1));
					}
					else
					{
						/*	奇数アドレスは-1のアドレスのデータを取得	*/
						value = *(shmem + (usCnt-1));
					}
				}
				else
				{
					value = *(shmem + usCnt);
				}
			}
			else
			{
				if((readst =  BPF_HM_DEVC_EEPROM_READ(usCnt, &value )) != BPF_HM_DEVC_COMPLETE ) {
					fprintf(stderr, "EEPROM READ ERROR:(code:%d)\n", readst );
				}
			}

			switch( f_dbgr_eepDisp[eep_num].eep_type )
			{
			case DISP_STRING:
				fprintf(outfp, "%c", value);
				if(value==0)
				{
					goto NEXTWORD;
				}
				break;

			case DISP_HEX:
				fprintf(outfp, "%02x",(unsigned short)value);
				break;

			case DISP_MACADDR:
				if(usCnt != f_dbgr_eepDisp[eep_num].eep_read_end)
				{
					fprintf(outfp, "%02x:",(unsigned short)value);
				}
				else
				{
					fprintf(outfp, "%02x",(unsigned short)value);
				}
				break;

			case DISP_IPADDR:
				if(usCnt != f_dbgr_eepDisp[eep_num].eep_read_end)
				{
					fprintf(outfp, "%d.",(unsigned short)value);
				}
				else
				{
					fprintf(outfp, "%d",(unsigned short)value);
				}
				break;

			case DISP_SNO:									/**< シリアルNo(BCD)表示形式		*/
				
				/*	読出し結果を一旦テンポラリに格納する	*/
				temp[usCnt-f_dbgr_eepDisp[eep_num].eep_read_sta] = value;
				
				/*	最後まで取得した場合	*/
				if(usCnt == f_dbgr_eepDisp[eep_num].eep_read_end)
				{
					fprintf(outfp, "%02x%02x/%02x/%02x %02x%02x%02x%02x", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
				}
				break;

			case DISP_VER:									/**< バージョン表示形式			*/
				
				/*	読出し結果を一旦テンポラリに格納する	*/
				temp[usCnt-f_dbgr_eepDisp[eep_num].eep_read_sta] = value;
				
				/*	最後まで取得した場合	*/
				if(usCnt == f_dbgr_eepDisp[eep_num].eep_read_end)
				{
					fprintf(outfp, "V=%02x%02x %02x%02x/%02x/%02x", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]);
				}
				break;

			case DISP_DATE_BCD:								/**< 年月日(BCD)表示形式		*/
				
				/*	読出し結果を一旦テンポラリに格納する	*/
				temp[usCnt-f_dbgr_eepDisp[eep_num].eep_read_sta] = value;
				
				/*	最後まで取得した場合	*/
				if(usCnt == f_dbgr_eepDisp[eep_num].eep_read_end)
				{
					fprintf(outfp, "%02x%02x/%02x/%02x", temp[0], temp[1], temp[2], temp[3]);
				}
				break;
			case DISP_DATE_HEX:								/**< 年月日(BCD)表示形式		*/
				
				/*	読出し結果を一旦テンポラリに格納する	*/
				temp[usCnt-f_dbgr_eepDisp[eep_num].eep_read_sta] = value;
				
				/*	最後まで取得した場合	*/
				if(usCnt == f_dbgr_eepDisp[eep_num].eep_read_end)
				{
					year = temp[0];
					year = (unsigned short)((year<<8) + temp[1]);
					fprintf(outfp, "%04d/%02d/%02d", year, temp[2], temp[3]);
				}
				break;

			case DISP_TIME:									/**< 時分秒ms(HEX)表示形式		*/
				
				/*	読出し結果を一旦テンポラリに格納する	*/
				temp[usCnt-f_dbgr_eepDisp[eep_num].eep_read_sta] = value;
				
				/*	最後まで取得した場合	*/
				if(usCnt == f_dbgr_eepDisp[eep_num].eep_read_end)
				{
					fprintf(outfp, "%02d:%02d:%02d.%02d", temp[0], temp[1], temp[2], temp[3]);
				}
				break;

			case DISP_DEV:									/**< デバイス種別表示形式		*/
			
				/*	読出し結果を一旦テンポラリに格納する	*/
				temp[usCnt-f_dbgr_eepDisp[eep_num].eep_read_sta] = value;
				/*	最後まで取得した場合	*/
				if(usCnt == f_dbgr_eepDisp[eep_num].eep_read_end)
				{
					for(devLp=0; devLp<5; devLp++)
					{
						if(temp[0] == devNum[devLp])
						{
							break;
						}
					}
					for(frqLp=0; frqLp<3; frqLp++)
					{
						if(temp[1] == frqNum[frqLp])
						{
							break;
						}
					}
					fprintf(outfp, "%s(0x%02x),%s(0x%02x)", devName[devLp], temp[0], frqName[frqLp], temp[1]);
				}
				break;

			case DISP_SIGN10:							/**< 符号数値(10bit)表示形式	*/
			
				/*	読出し結果を一旦テンポラリに格納する	*/
				temp[usCnt-f_dbgr_eepDisp[eep_num].eep_read_sta] = value;
				/*	最後まで取得した場合	*/
				if(usCnt == f_dbgr_eepDisp[eep_num].eep_read_end)
				{
					adj = temp[0];
					adj = (short int)((adj<<8) + temp[1]);
					/*	範囲外(0x0000~0x01ff, 0x0200~0x3ff以外)だったら	*/
					if(temp[0] > 0x03)
					{
						adj = 0;
					}
					/*	マイナス値だったら	*/
					else if(adj > D_DU_T23ADJVAL_MN)
					{
						adj |= 0xFC00;
					}
					fprintf(outfp, "%02x%02x(%d[DEC])", temp[0], temp[1], adj);
				}
				break;
			}
		}
NEXTWORD:
		fprintf(outfp, "\n");
	}

	return D_DBG_CMD_RST_SUCCESS;
}
/* @} */

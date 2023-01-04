/*!
 * @skip  $ld:$
 * @file  f_rrh_def.h
 * @brief System共通定義
 * @date  2013/11/11 ALPHA) yokoyama Create
 * @date  2015/05/27 ALPHA) murakami ハソ改版(M-RRU-ZSYS-01582)対応
 * @date  2015/09/09 ALPHA) fujiwara TDD-RRE(ZYNQ)対応
 * @date  2015/09/11 ALPHA) ueda TDD-RRE(Zynq)対応
 * @date  2015/10/26 ALPHA) ueda modify for TDD-RRE(Zynq)
 * @date  2015/10/30 ALPHA) murakami modify for M-RRU-ZSYS-01812
 * @date  2015/11/20 ALPHA) murakami TDD-RRE(ZYNQ)対応
 * @date  2015/12/07 ALPHA) ueda modify for M-RRU-ZSYS-01960
 * @date  2018/02/14 FJT)   吉田 add 5G-DU対応
 * @date  2018/02/23 FJT)   吉田 modify
 * @date  2018/06/22 Fujitsu)吉田 5G-DU割込み処理追加
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_DEF_H
#define F_RRH_DEF_H

/*!
 * @name RRH共通デファイン
 * @note RRH共通デファインの定義
 * @{
 */
#define D_RRH_OK							0							/*!< 正常終了				*/
#define D_RRH_NG							1							/*!< 異常終了				*/
#define D_RRH_MNG							-1							/*!< 異常終了(マイナス)		*/
#define D_RRH_NG_OTH						2							/*!< ng(other)				*/
#define D_RRH_NG_TO							3							/*!< ng(T.O)				*/
#define D_RRH_OFF							0							/*!< OFF					*/
#define D_RRH_ON							1							/*!< ON						*/
#define D_RRH_TRUE							0							/*!< TRUE					*/
#define D_RRH_FALSE							-1							/*!< FALSE					*/
#define D_RRH_NULL							0							/*!< NULL					*/
#define D_RRH_SND							0							/*!< send					*/
#define D_RRH_RCV							1							/*!< receive				*/
#define D_RRH_WRITE							0							/*!< write					*/
#define D_RRH_READ							1							/*!< read					*/
#define D_RRH_INI							0							/*!< 0						*/
#define D_RRH_ZERO							0							/*!< 0						*/
#define D_RRH_NUMFFFFFFFF					0xFFFFFFFF					/*!< 0xFFFFFFFF				*/
#define D_RRH_NUMFFFF						0xFFFF						/*!< 0xFFFF					*/
#define D_RRH_EMPTYSTR						""							/*!< Empty String			*/
/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */

/*!
 * @name ビット定義
 * @note ビットの値の定義
 */
#define BIT0    0x00000001                      /**< 00000000 00000000 00000000 00000001 */
#define BIT1    0x00000002                      /**< 00000000 00000000 00000000 00000010 */
#define BIT2    0x00000004                      /**< 00000000 00000000 00000000 00000100 */
#define BIT3    0x00000008                      /**< 00000000 00000000 00000000 00001000 */
#define BIT4    0x00000010                      /**< 00000000 00000000 00000000 00010000 */
#define BIT5    0x00000020                      /**< 00000000 00000000 00000000 00100000 */
#define BIT6    0x00000040                      /**< 00000000 00000000 00000000 01000000 */
#define BIT7    0x00000080                      /**< 00000000 00000000 00000000 10000000 */
#define BIT8    0x00000100                      /**< 00000000 00000000 00000001 00000000 */
#define BIT9    0x00000200                      /**< 00000000 00000000 00000010 00000000 */
#define BIT10   0x00000400                      /**< 00000000 00000000 00000100 00000000 */
#define BIT11   0x00000800                      /**< 00000000 00000000 00001000 00000000 */
#define BIT12   0x00001000                      /**< 00000000 00000000 00010000 00000000 */
#define BIT13   0x00002000                      /**< 00000000 00000000 00100000 00000000 */
#define BIT14   0x00004000                      /**< 00000000 00000000 01000000 00000000 */
#define BIT15   0x00008000                      /**< 00000000 00000000 10000000 00000000 */
#define BIT16   0x00010000                      /**< 00000000 00000001 00000000 00000000 */
#define BIT17   0x00020000                      /**< 00000000 00000010 00000000 00000000 */
#define BIT18   0x00040000                      /**< 00000000 00000100 00000000 00000000 */
#define BIT19   0x00080000                      /**< 00000000 00001000 00000000 00000000 */
#define BIT20   0x00100000                      /**< 00000000 00010000 00000000 00000000 */
#define BIT21   0x00200000                      /**< 00000000 00100000 00000000 00000000 */
#define BIT22   0x00400000                      /**< 00000000 01000000 00000000 00000000 */
#define BIT23   0x00800000                      /**< 00000000 10000000 00000000 00000000 */
#define BIT24   0x01000000                      /**< 00000001 00000000 00000000 00000000 */
#define BIT25   0x02000000                      /**< 00000010 00000000 00000000 00000000 */
#define BIT26   0x04000000                      /**< 00000100 00000000 00000000 00000000 */
#define BIT27   0x08000000                      /**< 00001000 00000000 00000000 00000000 */
#define BIT28   0x10000000                      /**< 00010000 00000000 00000000 00000000 */
#define BIT29   0x20000000                      /**< 00100000 00000000 00000000 00000000 */
#define BIT30   0x40000000                      /**< 01000000 00000000 00000000 00000000 */
#define BIT31   0x80000000                      /**< 10000000 00000000 00000000 00000000 */

#define BIT32   UINT64_C(0x0000000100000000)    /**< 00000000 00000000 00000000 00000001 00000000 00000000 00000000 00000000 */
#define BIT33   UINT64_C(0x0000000200000000)    /**< 00000000 00000000 00000000 00000010 00000000 00000000 00000000 00000000 */
#define BIT34   UINT64_C(0x0000000400000000)    /**< 00000000 00000000 00000000 00000100 00000000 00000000 00000000 00000000 */
#define BIT35   UINT64_C(0x0000000800000000)    /**< 00000000 00000000 00000000 00001000 00000000 00000000 00000000 00000000 */
#define BIT36   UINT64_C(0x0000001000000000)    /**< 00000000 00000000 00000000 00010000 00000000 00000000 00000000 00000000 */
#define BIT37   UINT64_C(0x0000002000000000)    /**< 00000000 00000000 00000000 00100000 00000000 00000000 00000000 00000000 */
#define BIT38   UINT64_C(0x0000004000000000)    /**< 00000000 00000000 00000000 01000000 00000000 00000000 00000000 00000000 */
#define BIT39   UINT64_C(0x0000008000000000)    /**< 00000000 00000000 00000000 10000000 00000000 00000000 00000000 00000000 */
#define BIT40   UINT64_C(0x0000010000000000)    /**< 00000000 00000000 00000001 00000000 00000000 00000000 00000000 00000000 */
#define BIT41   UINT64_C(0x0000020000000000)    /**< 00000000 00000000 00000010 00000000 00000000 00000000 00000000 00000000 */
#define BIT42   UINT64_C(0x0000040000000000)    /**< 00000000 00000000 00000100 00000000 00000000 00000000 00000000 00000000 */
#define BIT43   UINT64_C(0x0000080000000000)    /**< 00000000 00000000 00001000 00000000 00000000 00000000 00000000 00000000 */
#define BIT44   UINT64_C(0x0000100000000000)    /**< 00000000 00000000 00010000 00000000 00000000 00000000 00000000 00000000 */
#define BIT45   UINT64_C(0x0000200000000000)    /**< 00000000 00000000 00100000 00000000 00000000 00000000 00000000 00000000 */
#define BIT46   UINT64_C(0x0000400000000000)    /**< 00000000 00000000 01000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT47   UINT64_C(0x0000800000000000)    /**< 00000000 00000000 10000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT48   UINT64_C(0x0001000000000000)    /**< 00000000 00000001 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT49   UINT64_C(0x0002000000000000)    /**< 00000000 00000010 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT50   UINT64_C(0x0004000000000000)    /**< 00000000 00000100 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT51   UINT64_C(0x0008000000000000)    /**< 00000000 00001000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT52   UINT64_C(0x0010000000000000)    /**< 00000000 00010000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT53   UINT64_C(0x0020000000000000)    /**< 00000000 00100000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT54   UINT64_C(0x0040000000000000)    /**< 00000000 01000000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT55   UINT64_C(0x0080000000000000)    /**< 00000000 10000000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT56   UINT64_C(0x0100000000000000)    /**< 00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT57   UINT64_C(0x0200000000000000)    /**< 00000010 00000000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT58   UINT64_C(0x0400000000000000)    /**< 00000100 00000000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT59   UINT64_C(0x0800000000000000)    /**< 00001000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT60   UINT64_C(0x1000000000000000)    /**< 00010000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT61   UINT64_C(0x2000000000000000)    /**< 00100000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT62   UINT64_C(0x4000000000000000)    /**< 01000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 */
#define BIT63   UINT64_C(0x8000000000000000)    /**< 10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 */
/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (end add) */


/*!
 * @name プロセスID
 * @note プロセスIDの定義
 * @{
 */
/* プロセスID	*/
#define D_RRH_PROCID_INI 					0							/*!< プロセスID(INIT)			*/
#define D_RRH_PROCID_PF						1							/*!< プロセスID(PF)				*/
#define D_RRH_PROCID_DBG					2							/*!< プロセスID(DBG)			*/
#define D_RRH_PROCID_PTP					3							/*!< プロセスID(PTP)			*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_PROCID_F_PF					4							/*!< プロセスID(F_PF)			*/
#define D_RRH_PROCID_L2						5							/*!< プロセスID(L2)				*/
#define D_RRH_PROCID_L3						6							/*!< プロセスID(L3)				*/
#define D_RRH_PROCID_RE						7							/*!< プロセスI(RE)				*/ //C9
#define D_RRH_PROCID_MT						8							/*!< プロセスID(MT)				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define D_RRH_PROCID_DUCMD					10							/*!< プロセスID(DUCMD)			*/
#define D_RRH_PROCID_MAX					11							/*!< プロセスID数				*/
#define D_RRH_PROCID_MW						D_RRH_PROCID_MAX+1			/*!< プロセスID(MW)				*/
#define D_RRH_PROCID_DBG_PTP				D_RRH_PROCID_MW+1			/*!< プロセスID(PTPdebug)		*/
#define D_RRH_PROCID_MPSW_SAA				D_RRH_PROCID_DBG_PTP+1		/*!< プロセスID(MPSAA)			*/
#define D_RRH_PROCID_MPSW_NSA				D_RRH_PROCID_MPSW_SAA+1		/*!< プロセスID(MPNSA)			*/


/* プロセス内のスレッド数MAX値 */
#define D_RRH_THREAD_MAX					64							/*!< プロセス内のスレッド数MAX値*/

/* プロセス識別子 */
/* プロセス間通信用のMessageQueueIDはここにある値 + 連番とする	*/
/* 基本的にはプロセスINDEXと似るようにすること					*/
#define D_RRH_PROCID_Q_INIT					0x10000000					/*!< INITプロセス識別子				*/
#define D_RRH_PROCID_Q_PF					0x10010000					/*!< PFプロセス識別子				*/
#define D_RRH_PROCID_Q_PTP					0x10030000					/*!< PTPプロセス識別子				*/
#define D_RRH_PROCID_Q_TP					0x10FD0000					/*!< TP用識別子						*/
#define D_RRH_PROCID_Q_DBG					0x10FE0000					/*!< DBGプロセス識別子				*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_PROCID_Q_F_PF					0x10050000					/*!< F_PFプロセス識別子				*/
#define D_RRH_PROCID_Q_L2					0x10020000					/*!< L2プロセス識別子				*/
#define D_RRH_PROCID_Q_L3					0x10070000					/*!< L3プロセス識別子				*/
#define D_RRH_PROCID_Q_MT					0x10040000					/*!< MTプロセス識別子				*/
#define D_RRH_PROCID_Q_RE					0x10060000					/*!< REプロセス識別子				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/* @} */

/*!
 * @name プロセス間メッセージキュー番号
 * @note プロセス間メッセージキュー番号の定義
 * @{
 */
/* BPF側で下位2byteのみ使用するようにしています																	*/
/* 連番は絶対に被らないこと！！！！！！！！！！！！！！！！														*/
/* プロセス識別子 + 連番とする              プロセス識別子      + 連番(絶対に被らないこと!!!!!)					*/
#define D_RRH_PROCQUE_INIT					D_RRH_PROCID_Q_INIT	+ 0x00000001	/*!< INIT共通Queue番号			*/
#define D_RRH_PROCQUE_PF					D_RRH_PROCID_Q_PF	+ 0x00000002	/*!< PF共通Queue番号(CMU用)		*/
#define D_RRH_PROCQUE_PTP					D_RRH_PROCID_Q_PTP	+ 0x00000003	/*!< PTP共通Queue番号(CMU用)	*/
#define D_RRH_PROCQUE_DBG1					D_RRH_PROCID_Q_DBG	+ 0x0000000A	/*!< DBG共通Queue番号			*/
#define D_RRH_PROCQUE_TP					D_RRH_PROCID_Q_TP	+ 0x0000000E	/*!< TP用Queue番号				*/
#define D_RRH_PROCQUE_DBG2					D_RRH_PROCID_Q_DBG	+ 0x0000000F	/*!< DBG共通Queue番号			*/
#define D_RRH_PROCQUE_DBG3					D_RRH_PROCID_Q_DBG	+ 0x00000010	/*!< DBG共通Queue番号			*/
#define D_RRH_PROCQUE_TP_DHCP				D_RRH_PROCID_Q_TP	+ 0x00000011	/*!< TP用Queue番号(DHCP用)		*/
#define D_RRH_PROCQUE_API_MPLANE_READY		D_RRH_PROCID_Q_INIT	+ 0x00000012	/*!< 即時応答待ち用duApi_Mpn_Mnr_MplaneReady	*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_PROCQUE_F_PF					D_RRH_PROCID_Q_F_PF	+ 0x00000002	/*!< F_PF共通Queue番号(CMU用)		*/
#define D_RRH_PROCQUE_L2					D_RRH_PROCID_Q_L2	+ 0x00000003	/*!< L2共通Queue番号(CMU用)		*/
#define D_RRH_PROCQUE_API_SETUPLNK			D_RRH_PROCID_Q_L2	+ 0x00000004	/*!< 即時応答待ち用rrhApi_L2_Mqr_SetupLink	*/
#define D_RRH_PROCQUE_API_RELEASELNK		D_RRH_PROCID_Q_L2	+ 0x00000005	/*!< 即時応答待ち用rrhApi_L2_Mqr_ReleaseLink*/
#define D_RRH_PROCQUE_L3					D_RRH_PROCID_Q_L3	+ 0x00000006	/*!< L3共通Queue番号(CMU用)		*/
#define D_RRH_PROCQUE_MT					D_RRH_PROCID_Q_MT	+ 0x00000007	/*!< MT共通Queue番号(CMU用)		*/
#define D_RRH_PROCQUE_LCKBLK				D_RRH_PROCID_Q_MT	+ 0x00000008
#define D_RRH_PROCQUE_L3_DIAAPI_WAIT		D_RRH_PROCID_Q_L3	+ 0x00000009	/*!< 即時応答待ち用QueueID	DIA(診断)*/
#define D_RRH_PROCQUE_API_STARTSIMU			D_RRH_PROCID_Q_DBG	+ 0x0000000b	/*!< 即時応答待ち用rrhApi_Dbg_Mqr_StartCpriSimu	*/
#define D_RRH_PROCQUE_API_STOPSIMU			D_RRH_PROCID_Q_DBG	+ 0x0000000c	/*!< 即時応答待ち用rrhApi_Dbg_Mqr_StopCpriSimu	*/
#define D_RRH_PROCQUE_API_IFCONFIG			D_RRH_PROCID_Q_DBG	+ 0x0000000D	/*!< 即時応答待ち用rrhApi_Dbg_Mqr_IfConfig	*/
#define D_RRH_PROCQUE_API_TILTSWITCH		D_RRH_PROCID_Q_DBG	+ 0x00000011	/*!< 即時応答待ち用rrhApi_Tilt_Mqr_Switch	*/
#define D_RRH_PROCQUE_API_RECVDATA			D_RRH_PROCID_Q_L3	+ 0x00000012	/*!< 即時応答待ち用rrhApi_Tilt_Mqr_ReceiveData */
#define D_RRH_PROCQUE_API_SENDDATA			D_RRH_PROCID_Q_L3	+ 0x00000013	/*!< 即時応答待ち用rrhApi_Tilt_Mqr_SendData */
#define D_RRH_PROCQUE_API_GETCARR			D_RRH_PROCID_Q_F_PF	+ 0x00000014	/*!< 即時応答待ち用rrhApi_Tilt_Mqr_SendData */
#define D_RRH_PROCQUE_API_CARRSET			D_RRH_PROCID_Q_F_PF	+ 0x00000015	/*!< 即時応答待ち用rrhApi_Rfp_Mqr_CarSet	*/
#define D_RRH_PROCQUE_API_NOMODCAR			D_RRH_PROCID_Q_F_PF	+ 0x00000016	/*!< 即時応答待ち用rrhApi_Rfp_Mqr_NoModCar	*/
#define D_RRH_PROCQUE_API_PACONTRL			D_RRH_PROCID_Q_F_PF	+ 0x00000017	/*!< 即時応答待ち用rrhApi_Rfp_Mqr_PaControl	*/
#define D_RRH_PROCQUE_API_WAVECNTRL			D_RRH_PROCID_Q_F_PF	+ 0x00000018	/*!< 即時応答待ち用rrhApi_Rfp_Mqr_WaveControl	*/
#define D_RRH_PROCQUE_API_PORTCNTRL			D_RRH_PROCID_Q_F_PF	+ 0x00000019	/*!< 即時応答待ち用rrhApi_Ext_Mnt_PortControl	*/
#define D_RRH_PROCQUE_API_READGIPOBIT		D_RRH_PROCID_Q_L3	+ 0x0000001A	/*!< 即時応答待ち用rrhApi_File_Mqr_ReadGPIOBit */
#define D_RRH_PROCQUE_API_READEEPROM		D_RRH_PROCID_Q_L3	+ 0x0000001B	/*!< 即時応答待ち用rrhApi_Tilt_Mqr_ReadEeprom */
#define D_RRH_PROCQUE_API_WRITEEEPROM		D_RRH_PROCID_Q_L3	+ 0x0000001C	/*!< 即時応答待ち用rrhApi_Tilt_Mqr_WriteEeprom */
#define D_RRH_PROCQUE_API_TESTMOD			D_RRH_PROCID_Q_F_PF	+ 0x0000001D	/*!< 即時応答待ち用rrhApi_Rfp_Mqr_TestModeGet	*/
#define D_RRH_PROCQUE_RE					D_RRH_PROCID_Q_RE	+ 0x0000001E	/*!< RE共通Queue番号(CMU用)		*/
#define D_RRH_PROCQUE_L3_ADJ_DLY_STEP7      D_RRH_PROCID_Q_L3   + 0x0000001F
#define D_RRH_PROCQUE_L3_ADJ_DLY_STEP10     D_RRH_PROCID_Q_L3   + 0x00000020
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* @} */
/*!
 * @name キャリア番号
 * @{
 */
#define CMD_MAX_CAR_3G      4   /**< Max 3G Re Carrier Number  */
#define CMD_MAX_CAR_LTE     2   /**< Max LTE Re Carrier Number */
/* @} */

/*!
 * @name CPRI番号
 * @note
 * @{
 */
#define		D_RRH_CPRINO_RE1				1			/**< 対RE1 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE2				2			/**< 対RE2 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE3				3			/**< 対RE3 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE4				4			/**< 対RE4 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE5				5			/**< 対RE5 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE6				6			/**< 対RE6 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE7				7			/**< 対RE7 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE8				8			/**< 対RE8 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE9				9			/**< 対RE9 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE10				10			/**< 対RE10 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE11				11			/**< 対RE11 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE12				12			/**< 対RE12 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE13				13			/**< 対RE13 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE14				14			/**< 対RE14 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE15				15			/**< 対RE15 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE16				16			/**< 対RE16 CPRIリンク番号			*/
#define		D_RRH_CPRINO_REC				0			/**< 対REC CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE_MIN				1			/**< 最小 CPRIリンク番号			*/
#define		D_RRH_CPRINO_RE_MAX				16			/**< 最大 CPRIリンク番号			*/
#define		D_RRH_CPRINO_NUM				2			/**< CPRIリンク番号数				*/ //C9 17
/* @} */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/*!
 * @name Process間セマフォID
 * @note Process間セマフォIDの定義
 * @{
 */
enum
{
	E_RRH_SEMID_BPF_MNG = 0,										/*!< BPFでの共有メモリ管理用			*/
	E_RRH_SEMID_MNG_COM_MEMORY,										/*!< BPFでの通信用共有メモリ管理テーブル*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_RRH_SEMID_LOGLEVEL_MNG,										/*!< Log Level管理テーブル				*/
	E_RRH_SEMID_TIMER_MNG,											/*!< Timer管理テーブル					*/
	E_RRH_SEMID_APL_PROC_ID_MNG_INFO,								/*!< ProcessID管理テーブル				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_RRH_SEMID_APL_INVENTORY,										/*!< Inventory情報						*/
	E_RRH_SEMID_APL_THRD_ID_MNG_INFO,								/*!< ThreadID管理テーブル				*/
	E_RRH_SEMID_APL_RST_REASON_LOG,									/*!< RESET REASON LOG					*/
	E_RRH_SEMID_APL_ASSERT_LOG,										/*!< Assert Log							*/
	E_RRH_SEMID_APL_REG_ACCESS_LOG,									/*!< Register Access Log				*/
	E_RRH_SEMID_APL_ALM_HISTORY_LOG,								/*!< Alarm Hisotry Log					*/
	E_RRH_SEMID_APL_BUFFER_HISTORY,									/*!< MessageBuffer獲得履歴				*/
	E_RRH_SEMID_APL_TIMER_HISTORY,									/*!< Timer履歴							*/
	E_RRH_SEMID_APL_SEMAPHORE_HISTORY,								/*!< Semapore履歴						*/
	E_RRH_SEMID_APL_SYSPARA_INFO,									/*!< System Parameter情報				*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_RRH_SEMID_APL_RECV_MSG_MNG_TABLE,								/*!< recvメッセージ送信先情報管理テーブル	*/
	E_RRH_SEMID_APL_SEND_MSG_MNG_TABLE,								/*!< sendメッセージ送信先情報管理テーブル	*/
	E_RRH_SEMID_APL_FLASH_LOG_RW,									/*!< Flashログ読み書き用				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_RRH_SEMID_APL_FLASH_LOG_SPI,									/*!< SPIログ出力						*/
	E_RRH_SEMID_APL_SVCTL,											/*!< SV register access control			*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_RRH_SEMID_APL_ALM_ABC,										/*!< ALM_ABC							*/
	E_RRH_SEMID_APL_CPRI_STATE,										/*!< CPRI状態							*/
	E_RRH_SEMID_APL_FHM_STATE,										/*!< FHM状態							*/
	E_RRH_SEMID_APL_LAYER3,											/*!< Layer3								*/
	E_RRH_SEMID_APL_MTADDSET,										/*!< MTアドレス設定						*/
	E_RRH_SEMID_APL_CARDST_S3G,										/*!< (LTE)REカード状態					*/
	E_RRH_SEMID_APL_CARDST_3G,										/*!< (3G)REカード状態					*/
	E_RRH_SEMID_APL_SLOT_S3G,										/*!< (LTE)スロット情報					*/
	E_RRH_SEMID_APL_SLOT_3G,										/*!< (3G)スロット情報					*/
	E_RRH_SEMID_APL_SLOT_S3G_BOOT,									/*!< (LTE)スロット情報(起動時集約) */
	E_RRH_SEMID_APL_SLOT_3G_BOOT,									/*!< (3G)スロット情報(起動時集約) */
	E_RRH_SEMID_APL_SIDE_USE,										/*!< FHM保持FWファイル運用面			*/
	E_RRH_SEMID_APL_FWINFO_FHM,										/*!< FHM保持FWファイル情報(0.1面) 		*/
	E_RRH_SEMID_APL_FWINFO_RE,										/*!< FHM保持FWファイル情報 (RE-FW)		*/
	E_RRH_SEMID_APL_SYSPAR_LTE,										/*!< シスパラ格納領域(LTE)				*/
	E_RRH_SEMID_APL_SYSPAR_3G,										/*!< シスパラ格納領域(3G)				*/
    E_RRH_SEMID_APL_FW_DATA,										/*!< FWファイルデータ格納領域			*/
	E_RRH_SEMID_APL_REST_S3G,										/*!< (LTE)RE状態						*/
	E_RRH_SEMID_APL_REST_3G,										/*!< (3G)RE状態							*/
	E_RRH_SEMID_APL_TOFFSET_S3G,									/*!< (LTE)Toffset値報告					*/
	E_RRH_SEMID_APL_TOFFSET_3G,										/*!< (3G)Toffset値報告					*/
	E_RRH_SEMID_APL_TRXSET_PARA_LTE,								/*!< (LTE)TRX設定パラメータテーブル		*/
	E_RRH_SEMID_APL_TRXSET_PARA_3G,									/*!< (3G)TRX設定パラメータテーブル		*/
	E_RRH_SEMID_APL_CARST_S3G,										/*!< (LTE)キャリア状態					*/
	E_RRH_SEMID_APL_CARST_3G,										/*!< (3G)キャリア状態					*/
	E_RRH_SEMID_APL_EQP_S3G,										/*!< (LTE)装置構成情報					*/
	E_RRH_SEMID_APL_EQP_3G,											/*!< (3G)装置構成情報					*/
	E_RRH_SEMID_APL_EQP_S3G_BOOT,									/*!< (LTE)装置構成情報(起動時集約）		*/
	E_RRH_SEMID_APL_EQP_3G_BOOT,									/*!< (3G)装置構成情報(起動時集約）		*/
	E_RRH_SEMID_APL_RE_DELAY_INF,									/*!< RE単位遅延補正情報					*/
	E_RRH_SEMID_APL_DELAY_INF,										/*!< 遅延補正情報					*/
	E_RRH_SEMID_APL_RRH_TX_ONOFF,									/*!< 無線送信ONOFF状態					*/
	E_RRH_SEMID_APL_T_RRH_CONN_DISC,								/*!< RE接続有無状態					*/
	E_RRH_SEMID_APL_REPORTSTA_S3G,									/*!< (LTE)RE PORT状態報告 */
	E_RRH_SEMID_MNG_COM_MEMORY_LAPBS,								/*!< BPFでの通信用共有メモリ管理テーブル */
	E_RRH_SEMID_MNG_COM_MEMORY_LAPBR,								/*!< BPFでの通信用共有メモリ管理テーブル */
	E_RRH_SEMID_MNG_COM_MEMORY_MON,									/*!< BPFでの通信用共有メモリ管理テーブル */
	E_RRH_SEMID_MNG_COM_MEMORY_TSK,									/*!< BPFでの通信用共有メモリ管理テーブル */
	E_RRH_SEMID_MNG_COM_MEMORY_MNT,									/*!< BPFでの通信用共有メモリ管理テーブル */
	E_RRH_SEMID_MNG_COM_MEMORY_TSK_L,								/*!< BPFでの通信用共有メモリ管理テーブル */
	E_RRH_SEMID_MNG_COM_MEMORY_TSK_LL,								/*!< BPFでの通信用共有メモリ管理テーブル */
#ifdef OPT_RRH_ZYNQ_REC
	E_RRH_SEMID_APL_RETURN_MESSAGE,
#endif
	E_RRH_SEMID_APL_CARDST_SLOT_S3G,								/*!< (LTE)REカード状態(ポート集約)		*/
	E_RRH_SEMID_APL_CARDST_SLOT_3G,									/*!< (3G)REカード状態(ポート集約)		*/
	E_RRH_SEMID_DBG_PRINT,											/*!< デバッグ用printf文出力Off(=0)/On(=1)	*/
	E_RRH_SEMID_APL_ASSERT_DWL,										/*!< Assert Log for DWL					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_RRH_SEMID_APL_SVCTL_LOG,										/*!< SV register access control log		*/
	E_RRH_SEMID_APL_FLTLVL,											/*!< SV control fault level tbl			*/
	E_RRH_SEMID_APL_FLTID,											/*!< SV control fault ID tbl			*/
	E_RRH_SEMID_APL_DU_CMD_INFO,									/*!< DUコマンド管理情報					*/
	E_RRH_SEMID_APL_SFP_INFO,										/*!< Transceiver情報領域アクセス		*/
	E_RRH_SEMID_APL_USER_INFO,										/*!< USERアカウント情報領域アクセス		*/
	E_RRH_SEMID_APL_VLAN_INFO,										/*!< VLAN情報領域アクセス				*/
	E_RRH_SEMID_APL_TP_CMD_INFO,									/*!< TPコマンド管理情報					*/
	E_RRH_SEMID_APL_MT_INFO,										/*!< MT管理								*/
	E_RRH_SEMID_APL_LUT_LOG,										/*!< LUT_LOG							*/
	E_RRH_SEMID_APL_MAX												/*!< Semaphore情報数					*/
};
/* @} */

/*!
 * @name CMUスレッドID
 * @note CMUスレッドIDの定義
 * @{
 */
enum
{
	E_RRH_CMU_RECV_ID = 0,											/*!< CMU RECV							*/
	E_RRH_CMU_SEND_ID,												/*!< CMU SEND							*/
	E_RRH_CMU_THREAD_MAX											/*!< CMU スレッド数						*/
};
/* @} */

/*==========================================================================================================*/
/*	共有メモリアクセス定義																					*/
/*==========================================================================================================*/
/**
 * @name BPF Shared Memory Number(for APL)
 * @name BPF Shared Memory Number(for APL) define
 */
enum
{
	E_RRH_SHMID_BPF_MNG = 0,										/*!< 00 : 共有メモリ管理				*/
	E_RRH_SHMID_MNG_COM_MEMORY,										/*!< 01 : 通信用共有メモリ管理			*/
	E_RRH_SHMID_LOGLEVEL_MNG,										/*!< 02 : ログ取得レベル管理テーブル	*/
	E_RRH_SHMID_TIMER_MNG,											/*!< 03 : タイマ管理テーブル			*/
	E_RRH_SHMID_APL_PROC_ID_MNG_INFO,								/*!< 04 : プロセスID管理テーブル		*/
	E_RRH_SHMID_APL_THRD_ID_MNG_INFO,								/*!< 05 : スレッドID管理テーブル		*/
	E_RRH_SHMID_APL_RST_REASON_LOG,									/*!< 06 : reset reason log				*/
	E_RRH_SHMID_APL_ASSERT_LOG,										/*!< 07 : Assert Log					*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_RRH_SHMID_APL_REG_ACCESS_LOG,									/*!< Register Access Log				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_RRH_SHMID_APL_ALM_HISTORY_LOG,								/*!< 08 : Alarm Hisotry Log				*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_RRH_SHMID_APL_CPRI_L1_LOSLOF_HIS,								/*!< CPRI L1復旧断履歴					*/
	E_RRH_SHMID_APL_CPRI_L1_LINK_LOG,								/*!< CPRI L1リンク障害ログ				*/
	E_RRH_SHMID_APL_CPRI_L1_STAT_INFO,								/*!< CPRI L1統計情報					*/
	E_RRH_SHMID_APL_CPRI_L2_STAT_INFO,								/*!< CPRI L2統計情報					*/
	E_RRH_SHMID_APL_CPRI_L3_STAT_INFO,								/*!< CPRI L3統計情報					*/
	E_RRH_SHMID_APL_CPRI_L3_LOSLOF_HIS,								/*!< CPRI L3復旧断履歴					*/
	E_RRH_SHMID_APL_LAPB_L3_SR_LOG,									/*!< CPRI LAPB/L3送受信Log				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_RRH_SHMID_APL_BUFFER_HISTORY,									/*!< 09 : メッセージバッファ獲得履歴	*/
	E_RRH_SHMID_APL_TIMER_HISTORY,									/*!< 10 : タイマ履歴					*/
	E_RRH_SHMID_APL_SEMAPHORE_HISTORY,								/*!< 11 : セマフォ履歴					*/
	E_RRH_SHMID_APL_DPDA_ALM_LOG,									/*!< 12 : dpda alarm log				*/
	E_RRH_SHMID_APL_DPDA_MNG_TBL,									/*!< 13 : dpda情報管理テーブル			*/
	E_RRH_SHMID_APL_INVENTORY,										/*!< 14 : インベントリ情報				*/
	E_RRH_SHMID_APL_RRH_KIND,										/*!< 15 : DU種別						*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_RRH_SHMID_APL_SHELF_INVENTORY,								/*!< Shelf Inventory情報				*/
	E_RRH_SHMID_APL_SYSPARA_INFO,									/*!< System Parameter情報				*/
	E_RRH_SHMID_APL_DEVICE_MNG_TBL,									/*!< Device Manage Table				*/
	E_RRH_SHMID_APL_SOFTWARE_VERSION,								/*!< ソフトウェアバージョン				*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_RRH_SHMID_APL_ALM_HIS_PREPARE,								/*!< 16 : Alarm History log収集準備Table*/
	E_RRH_SHMID_APL_TEMP_LOG_TBL,									/*!< 17 : Temprature Log Table			*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	E_RRH_SHMID_APL_SPI_ACCESS_LOG,									/*!< SPI Access Log						*/
	E_RRH_SHMID_APL_RE_KIND,										/*!< RE種別								*/
	E_RRH_SHMID_APL_CPRI_L3_SR_LOG,									/*!< L3送受信ログ						*/
	E_RRH_SHMID_APL_ALM_ABC,										/*!< ALM_ABC							*/
	E_RRH_SHMID_APL_CPRI_STATE,										/*!< CPRI状態							*/
	E_RRH_SHMID_APL_FHM_STATE,										/*!< FHM状態							*/
	E_RRH_SHMID_APL_LAYER3,											/*!< Layer3								*/
	E_RRH_SHMID_APL_MTADDSET,										/*!< MTアドレス設定						*/
	E_RRH_SHMID_APL_CARDST_S3G,										/*!< (LTE)REカード状態					*/
	E_RRH_SHMID_APL_CARDST_3G,										/*!< (3G)REカード状態					*/
	E_RRH_SHMID_APL_SLOT_S3G,										/*!< (LTE)スロット情報					*/
	E_RRH_SHMID_APL_SLOT_3G,										/*!< (3G)スロット情報					*/
	E_RRH_SHMID_APL_SIDE_USE,										/*!< FHM保持FWファイル運用面			*/
	E_RRH_SHMID_APL_FWINFO_FHM,										/*!< FHM保持FWファイル情報(0.1面) 		*/
	E_RRH_SHMID_APL_FWINFO_RE,										/*!< FHM保持FWファイル情報 (RE-FW)		*/
	E_RRH_SHMID_APL_SYSPAR_LTE,										/*!< シスパラ格納領域(LTE)				*/
	E_RRH_SHMID_APL_SYSPAR_3G,										/*!< シスパラ格納領域(3G)				*/
    E_RRH_SHMID_APL_REST_S3G,										/*!< (LTE)RE状態						*/
	E_RRH_SHMID_APL_REST_3G,										/*!< (3G)RE状態							*/
	E_RRH_SHMID_APL_TOFFSET_S3G,									/*!< (LTE)Toffset値報告					*/
	E_RRH_SHMID_APL_TOFFSET_3G,										/*!< (3G)Toffset値報告					*/
	E_RRH_SHMID_APL_TRXSET_PARA_LTE,								/*!< (LTE)TRX設定パラメータテーブル		*/
	E_RRH_SHMID_APL_TRXSET_PARA_3G,									/*!< (3G)TRX設定パラメータテーブル		*/
	E_RRH_SHMID_APL_CARST_S3G,										/*!< (LTE)キャリア状態					*/
	E_RRH_SHMID_APL_CARST_3G,										/*!< (3G)キャリア状態					*/
	E_RRH_SHMID_APL_EQP_S3G,										/*!< (LTE)装置構成情報					*/
	E_RRH_SHMID_APL_EQP_3G,											/*!< (3G)装置構成情報					*/
	E_RRH_SHMID_APL_EQP_S3G_BOOT,									/*!< (LTE)装置構成情報(起動時集約）		*/
	E_RRH_SHMID_APL_EQP_3G_BOOT,									/*!< (3G)装置構成情報(起動時集約）		*/
	E_RRH_SHMID_APL_RE_DELAY_INF,									/*!< RE単位遅延補正情報					*/
	E_RRH_SHMID_APL_DELAY_INF,										/*!< 遅延補正情報						*/
	E_RRH_SHMID_APL_RRH_TX_ONOFF,									/*!< 無線送信ONOFF状態					*/
	E_RRH_SHMID_APL_T_RRH_CONN_DISC,								/*!< RE接続有無状態						*/
#ifdef OPT_RRH_ZYNQ_REC
	E_RRH_SHMID_APL_RETURN_MESSAGE,
#endif
	E_RRH_SHMID_APL_SFPLOG,                                         /*!< SFP履歴ログ                        */
	E_RRH_SHMID_APL_SLOT_S3G_BOOT,									/*!< (LTE)スロット情報(起動時集約) */
	E_RRH_SHMID_APL_SLOT_3G_BOOT,									/*!< (3G)スロット情報(起動時集約) */
	E_RRH_SHMID_APL_REPORTSTA_S3G,									/*!< (LTE)RE PORT状態報告 */
	E_RRH_SHMID_APL_CARDST_SLOT_S3G,								/*!< (LTE)REカード状態(ポート集約)		*/
	E_RRH_SHMID_APL_CARDST_SLOT_3G,									/*!< (3G)REカード状態(ポート集約)		*/
	E_RRH_SHMID_DBG_PRINT,											/*!< デバッグ用printf文出力Off(=0)/On(=1)	*/
	E_RRH_SHMID_APL_ASSERT_DWL,										/*!< Assert Log for DWL					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	E_RRH_SHMID_APL_SFP0_MNG_INFO_HIS,								/*!< 18 : SFP#0管理情報履歴				*/
	E_RRH_SHMID_APL_SFP1_MNG_INFO_HIS,								/*!< 19 : SFP#1管理情報履歴				*/
	E_RRH_SHMID_APL_CAL_HIS,										/*!< 20 : CAL履歴						*/
	E_RRH_SHMID_APL_ALM_NTC_HIS,									/*!< 21 : Alarm通知ログ					*/
	E_RRH_SHMID_APL_EQ_HIS,											/*!< 22 : 装置状態ログ					*/
	E_RRH_SHMID_APL_ACTIVE_ALM_LIST,								/*!< 23 : Active-Alarm-list				*/
	E_RRH_SHMID_APL_CAR_STAT_HIS,									/*!< 24 : キャリア状態ログ				*/
	E_RRH_SHMID_APL_PTP_CONF,										/*!< 25 : PTP起動時パラメータ			*/
	E_RRH_SHMID_APL_PTP_STAT_HIS,									/*!< 26 : PTP状態ログ					*/
	E_RRH_SHMID_APL_PTP_TIME_ADJ,									/*!< 27 : PTP時刻補正					*/
	E_RRH_SHMID_APL_UPSIGSTOP_HIS,									/*!< 28 : 主信号停波履歴				*/
	E_RRH_SHMID_APL_FH_L1_HIS,										/*!< 29 : FH L1履歴						*/
	E_RRH_SHMID_APL_SFNFRM_CALB_TBL,								/*!< 30 : SFN/FRM補正用テーブル			*/
	E_RRH_SHMID_APL_TXPOW_DETECT_TBL,								/*!< 31 : Tx power detector テーブル	*/
	E_RRH_SHMID_APL_TXPOW_DETECT_LOG,								/*!< 32 : Tx power detector LOG			*/
	E_RRH_SHMID_APL_LUT_LOG,										/*!< 33 : LUT LOG						*/
	E_RRH_SHMID_APL_FH_STAT,										/*!< 34 : FH統計情報					*/
	E_RRH_SHMID_APL_MAC_STAT_ETH0,									/*!< 35 : Ethernet リンク障害履歴ログ(ETH#1)	*/
	E_RRH_SHMID_APL_MAC_STAT_ETH1,									/*!< 36 : Ethernet リンク障害履歴ログ(ETH#2)	*/
	E_RRH_SHMID_APL_PKT_STAT_MPLANE,								/*!< 37 : ネットワーク統計(M-Plane)		*/
	E_RRH_SHMID_APL_SV_STATE_HISTORY,								/*!< 38 : SV状態変化履歴ログ			*/
	E_RRH_SHMID_APL_TRAINING_DATA,									/*!< 39 : TPコマンド仮想メモリデータ	*/
	E_RRH_SHMID_APL_DU_INFORMATION_TBL,								/*!< 40 : DUネットワーク情報テーブル	*/
	E_RRH_SHMID_APL_TALISE_CONF_INFO,								/*!< 41 : RFIC Configデータ				*/
	E_RRH_SHMID_APL_DU_PROCESS_INFO,								/*!< 42 : DU information Table			*/
	E_RRH_SHMID_APL_RF_TRAINING_DATA,								/*!< 43 : RF個別トレーニングデータ		*/
	E_RRH_SHMID_APL_DU_CMD_INFO,									/*!< 44 : DUコマンド管理情報			*/
	E_RRH_SHMID_APL_DU_FLT_MNG,										/*!< 45 : fault管理情報					*/
	E_RRH_SHMID_APL_T_RRH_NETCONF_INFO_TBL,							/*!< 46 : NETCONFユーザー情報テーブル	*/
	E_RRH_SHMID_APL_TXCAL_COEF,										/*!< 47 : Tx CAL係数					*/
	E_RRH_SHMID_APL_RXCAL_COEF,										/*!< 48 : Rx CAL係数					*/
	E_RRH_SHMID_APL_PFM_PERFORMANCE_TBL,							/*!< 49 : Performanceログテーブル		*/
	E_RRH_SHMID_APL_PFM_PERFORMANCE_MNG_TBL,						/*!< 50 : Performance情報管理テーブル	*/
	E_RRH_SHMID_APL_LBM_MNGMENT_TBL,								/*!< 51 : LBM/LBR管理テーブル			*/
	E_RRH_SHMID_EIO_SVCTRL_MNG_TBL,									/*!< 52 : 外部IO情報テーブル			*//*	5G-商用DU	*/
	E_RRH_SHMID_EIO_SVCTRL_LOG_TBL,									/*!< 53 : 外部IO制御履歴テーブル		*//*	5G-商用DU	*/
	E_RRH_SHMID_APL_TP_CMD_INFO,									/*!< 54 : TPコマンド管理情報			*/
	E_RRH_SHMID_APL_MT_INFO,										/*!< 55 : MT管理						*/
	E_RRH_SHMID_APL_BF_MODE_TBL,									/*!< 56 : Beamforming Mode Flag			*/
	E_RRH_SHMID_APL_ALD_LOG_TBL, 									/* ALD(AISG) message history		*/
	E_RRH_SHMID_APL_ALD_CTRL_TBL, 									/* ALD(AISG) Latest information	*/
	E_RRH_SHMID_APL_SFPFS_FLG,										/*!< 57 : SFP Fail Safe ON/OFF Flag		*/
	E_RRH_SHMID_APL_DLF_LOG,										/*!< 58 : DLF統計情報					*/
	E_RRH_SHMID_APL_MAX												/*!< 共有メモリ情報数					*/
};

/* @} */

/*!
 * @name TimerID
 * @note TimerIDの定義
 * @{
 */
enum
{
	E_RRH_TIMID_LGC_SHELTER = 1,									/*!< ログ情報退避用タイマ				*/
	E_RRH_TIMID_PTP_LCKREPRT,										/*!< PTP-状態保護タイマ					*/
	E_RRH_TIMID_SYNCE_LCKREPRT,										/*!< SyncE-状態保護タイマ				*/
	E_RRH_TIMID_SYNC_STSREPRT,										/*!< Sync-状態保護タイマ				*/
	E_RRH_TIMID_PFM_INTERVAL_RPT,									/*!< Performance Management測定タイマ	*/
	E_RRH_TIMID_EIO_EXTIOSV,										/*!< 外部IO監視周期タイマ				*//*	5G-商用DU	*/
	E_RRH_TIMID_EIO_FANSVCTRL,										/*!< FAN監視制御周期タイマ				*//*	5G-商用DU	*/
	E_RRH_TIMID_PFM_LOGTIME,										/*!< Peformance ログタイマ				*//*	5G-商用DU	*/
	E_RRH_TIMID_SYNCE_SSMTIME,										/*!< SyncE-ssmタイマ					*/
	E_RRH_TIMID_AIH_ALDRCV,											/*!< ALD(AISG) Rcv wait timer			*/
	E_RRH_TIMID_APL_MAX,												/*!< TimerID数											*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	D_RRH_SHMID_APL_LMC_DATA = E_RRH_SHMID_APL_MAX,						/*!< LMC data storage					*/
	E_RRH_SHMID_MAX
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
};
/* @} */

/*!
 * @name TimerID
 * @note TimerIDの定義
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
enum
{
	E_RRH_TIMID_LOG_SHELTER = 1,									/*!< ログ情報退避用タイマ				*/
	E_RRH_TIMID_LOG_SEND_3G,										/*!< 障害ログ転送開始タイマ for 3G		*/
	E_RRH_TIMID_LOG_SEND_S3G,										/*!< 障害ログ転送開始タイマ for S3G	*/
	E_RRH_TIMID_HCREQ_CNF_3G,										/*!< RSV: Health check rec confirm timer for 3G		*/
	E_RRH_TIMID_HCREQ_CNF_S3G,										/*!< RSV: Health check rec confirm timer for S3G	*/
    E_RRH_TIMID_LOG_GET_STOP_3G,									/*!< 障害ログ転送開始タイマ for 3G		*/
    E_RRH_TIMID_LOG_GET_STOP_LTE,									/*!< 障害ログ転送開始タイマ for LTE	*/
        E_RRH_TIMID_LOG_GET_LTE,									/*!< RE障害ログ取得確認タイマ for LTE	*/

	E_RRH_TIMID_HCRSP1_CNF_3G,														/*!< RSV: Health check re1 confirm timer for 3G */
	E_RRH_TIMID_HCRSP1_CNF_S3G	= E_RRH_TIMID_HCRSP1_CNF_3G  + D_RRH_CPRINO_RE_MAX,	/*!< RSV: Health check re1 confirm timer for 3G */
	E_RRH_TIMID_HCRSP1_CNF_MAX  = E_RRH_TIMID_HCRSP1_CNF_S3G + D_RRH_CPRINO_RE_MAX,

	E_RRH_TIMID_HCSND1_CNF_3G	= E_RRH_TIMID_HCRSP1_CNF_MAX,						/*!< RSV: Health check re1 send timer for 3G */
	E_RRH_TIMID_HCSND1_CNF_S3G	= E_RRH_TIMID_HCSND1_CNF_3G  + D_RRH_CPRINO_RE_MAX,	/*!< RSV: Health check re1 send timer for 3G */
	E_RRH_TIMID_HCSND1_CNF_MAX  = E_RRH_TIMID_HCSND1_CNF_S3G + D_RRH_CPRINO_RE_MAX,

	E_RRH_TIMID_WARMUP0 		= E_RRH_TIMID_HCSND1_CNF_MAX,		/*!< RSV: #0 Warm up timer				*/
	E_RRH_TIMID_WARMUP1,											/*!< RSV: #1 Warm up timer				*/
	E_RRH_TIMID_USEFIRMWARESND,										/*!< DLM：FIRMWARE sending in USE state confirm timer	*/
	E_RRH_TIMID_SYSPRMSND_3G,										/*!< DLM：SYSTEMPARAMETER sending confirm timer for 3G	*/
	E_RRH_TIMID_SYSPRMSND_S3G,										/*!< DLM：SYSTEMPARAMETER sending confirm timer for S3G	*/
	E_RRH_TIMID_FIRMWARESND_3G,										/*!< DLM：FIRMWARE sending confirm timer for 3G			*/
	E_RRH_TIMID_FIRMWARESND_S3G,									/*!< DLM：FIRMWARE sending confirm timer for S3G		*/

	E_RRH_TIMID_LPB_T1,												/*!< LPB：応答確認用タイマT1(link 1)					*/
	E_RRH_TIMID_LPB_T1_RE1,											/*!< LPB：応答確認用タイマT1(re 1)						*/
	E_RRH_TIMID_LPB_T1_MAX = E_RRH_TIMID_LPB_T1_RE1 + D_RRH_CPRINO_RE_MAX,

	E_RRH_TIMID_LPB_T2 = E_RRH_TIMID_LPB_T1_MAX,					/*!< LPB：応答送信用タイマT2(link 1)					*/
	E_RRH_TIMID_LPB_T2_RE1,											/*!< LPB：応答送信用タイマT2(re 1)						*/
	E_RRH_TIMID_LPB_T2_MAX = E_RRH_TIMID_LPB_T2_RE1 + D_RRH_CPRINO_RE_MAX,

	E_RRH_TIMID_LPB_T3 = E_RRH_TIMID_LPB_T2_MAX,					/*!< LPB：相手局ビジー監視タイマT3(link 1)				*/
	E_RRH_TIMID_LPB_T3_RE1,											/*!< LPB：相手局ビジー監視タイマT3(re 1)				*/
	E_RRH_TIMID_LPB_T3_MAX = E_RRH_TIMID_LPB_T3_RE1 + D_RRH_CPRINO_RE_MAX,

	E_RRH_TIMID_LPB_T4 = E_RRH_TIMID_LPB_T3_MAX,					/*!< LPB：自局相手局アイドル状態監視タイマT4(link 1)	*/
	E_RRH_TIMID_LPB_T4_RE1,											/*!< LPB：自局相手局アイドル状態監視タイマT4(re 1)		*/
	E_RRH_TIMID_LPB_T4_MAX = E_RRH_TIMID_LPB_T4_RE1 + D_RRH_CPRINO_RE_MAX,

	E_RRH_TIMID_L2BSYDET = E_RRH_TIMID_LPB_T4_MAX,					/*!< LPB：L2 busy detect間隔タイマ(link 1)				*/
	E_RRH_TIMID_L2BSYDET_RE1,										/*!< LPB：L2 busy detect間隔タイマ(re 1)				*/
	E_RRH_TIMID_L2BSYDET_MAX = E_RRH_TIMID_L2BSYDET_RE1 + D_RRH_CPRINO_RE_MAX,

	E_RRH_TIMID_L1BSYDET = E_RRH_TIMID_L2BSYDET_MAX,				/*!< LPB：L1 busy detect間隔タイマ(link 1)				*/
	E_RRH_TIMID_L1BSYDET_RE1,										/*!< LPB：L1 busy detect間隔タイマ(re 1)				*/
	E_RRH_TIMID_L1BSYDET_MAX = E_RRH_TIMID_L1BSYDET_RE1 + D_RRH_CPRINO_RE_MAX,

	E_RRH_TIMID_STBL_PA0 = E_RRH_TIMID_L1BSYDET_MAX,				/*!< RSV: #0 PA安定化タイマ 							*/
	E_RRH_TIMID_STBL_PA1,											/*!< RSV: #1 PA安定化タイマ 							*/
	E_RRH_TIMID_FMRELCF,											/*!< FLD-MT接続解放確認タイマ							*/
	E_RRH_TIMID_FMRELCF_S3G,										/*!< FLD-MT接続解放確認タイマ for S3G					*/
	E_RRH_TIMID_TCPCONNWT,											/*!< TCP connection waiting timer 						*/
	E_RRH_TIMID_FTPFILUPWT,											/*!< FTP file upload waiting timer 						*/
	E_RRH_TIMID_REFLDL,												/*!< MKM : ダウンロード確認タイマ						*/
	E_RRH_TIMID_TDLWRTSV,											/*!< MKM : TRAファイルライトタイマ 						*/
	E_RRH_TIMID_RE_FLSRWSV,											/*!< MKM : RE-FLASH READ/WRITEタイマ 					*/
	E_RRH_TIMID_MKM_MON,											/*!< MKM: Monitor data timer							*/
	E_RRH_TIMID_HDLC_POLL,											/*!< HDLC poll timer									*/
	E_RRH_TIMID_ALARM_POLL,											/*!< ALarm poll timer									*/
	E_RRH_TIMID_DPD_PERIODIC,										/*!< dpda 周期監視タイマ(SLC)							*/
	E_RRH_TIMID_USECHG_3G,											/*!< 3G Use change timer								*/
	E_RRH_TIMID_USECHG_S3G,											/*!< S3G Use change timer								*/
	E_RRH_TIMID_PASW_CYCL,											/*!< PASW Cycling timer (200msec)						*/
	E_RRH_TIMID_AUTO_NEGO,											/*!< CPRI Auto Nego timer (5msec)						*/

	E_RRH_TIMID_CPRI_LINKUP1,										/*!< CPRI link timer (10msec)						*/
	E_RRH_TIMID_CPRI_LINKUP_MAX = E_RRH_TIMID_CPRI_LINKUP1 + D_RRH_CPRINO_RE_MAX,
	E_RRH_TIMID_CPRI_LINKUP_LAST = E_RRH_TIMID_CPRI_LINKUP_MAX -1,
    E_RRH_TIMID_CPRI_LINKUP2,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP3,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP4,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP5,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP6,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP7,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP8,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP9,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP10,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP11,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP12,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP13,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP14,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP15,										/*!< CPRI link timer (10msec)						*/
    E_RRH_TIMID_CPRI_LINKUP16,										/*!< CPRI link timer (10msec)						*/

	E_RRH_TIMID_DPD_TDD_HDOFF_MON,									/*!< DPDA TDD 自律OFF周期監視タイマ(100ms)				*/
	/*From here is FHM timer*/
	E_RRH_TIMID_SVMT_CONN_REL_FORCE_3G,								/*!< SV-MT/eNB-MT接続強制解放確認タイマ(1000msec) for 3G*/
	E_RRH_TIMID_SVMT_CONN_REL_FORCE_LTE,								/*!< SV-MT/eNB-MT接続強制解放確認タイマ(1000msec) for LTE*/
	E_RRH_TIMID_FMCOMRXCF_3G,										/*!< FLD-MTデータコマンド転送受信応答確認タイマ(1000msec) for 3G */
	E_RRH_TIMID_FMCOMRXCF_LTE,										/*!< FLD-MTデータコマンド転送受信応答確認タイマ(1000msec) for LTE */
	E_RRH_TIMID_FLD_MT_CONN_CONF_3G,								/**< FLD-MT接続確認タイマ for 3G*/
	E_RRH_TIMID_FLD_MT_CONN_CONF_LTE,								/**< FLD-MT接続確認タイマ for LTE*/
	E_RRH_TIMID_SVeNB_MT_CONN_CONF_3G,								/**< SV-MT/eNB-MT接続確認タイマ for 3G*/
	E_RRH_TIMID_SVeNB_MT_CONN_CONF_LTE,								/**< SV-MT/eNB-MT接続確認タイマ for LTE*/
	E_RRH_TIMID_FLD_MT_CONN_REL_FORCE_3G,							/*!<  FLD-MT接続強制解放確認タイマ(1000msec) for 3G*/
	E_RRH_TIMID_FLD_MT_CONN_REL_FORCE_LTE,							/*!<  FLD-MT接続強制解放確認タイマ(1000msec) for LTE*/
	E_RRH_TIMID_SV_MT_CONN_3G,										/*!< SV-MT/eNB-MT接続強制解放確認タイマ(1000msec) for 3G*/
	E_RRH_TIMID_SV_MT_CONN_LTE,										/*!< SV-MT/eNB-MT接続強制解放確認タイマ(1000msec) for LTE*/
	E_RRH_TIMID_TRX_SET_3GMIN,														/*!< TRXSET for 3G Carrier 1*/
	E_RRH_TIMID_TRX_SET_3GMAX = E_RRH_TIMID_TRX_SET_3GMIN + CMD_MAX_CAR_3G - 1,		/*!< TRXSET for 3G Carrier 4*/
	E_RRH_TIMID_TRX_SET_LTEMIN,														/*!< TRXSET for LTE Carrier 1*/
	E_RRH_TIMID_TRX_SET_LTEMAX = E_RRH_TIMID_TRX_SET_LTEMIN + CMD_MAX_CAR_LTE - 1,	/*!< TRXSET for LTE Carrier 2*/

	E_RRH_TIMID_TRX_REL_3GMIN,														/*!< TRXREL for 3G Carrier 1*/
	E_RRH_TIMID_TRX_REL_3GMAX = E_RRH_TIMID_TRX_REL_3GMIN + CMD_MAX_CAR_3G - 1,		/*!< TRXREL for 3G Carrier 4*/
	E_RRH_TIMID_TRX_REL_LTEMIN,														/*!< TRXREL for LTE Carrier 1*/
	E_RRH_TIMID_TRX_REL_LTEMAX = E_RRH_TIMID_TRX_REL_LTEMIN + CMD_MAX_CAR_LTE - 1,	/*!< TRXREL for LTE Carrier 2*/

	/*FLD-MTデータコマンド転送送信確認タイマ(3G)*/
	E_RRH_TIMID_MT_DATA_TRANS_SND_3G,
	/*FLD-MTデータコマンド転送送信確認タイマ(LTE)*/
	E_RRH_TIMID_MT_DATA_TRANS_SND_LTE,

	/* SV-MT/eNB-MTデータ受信確認タイマ(10msec) for 3G */
	E_RRH_TIMID_SVMTDATRCV_3G,
	/* SV-MT/eNB-MTデータ受信確認タイマ(10msec) for LTE */
	E_RRH_TIMID_SVMTDATRCV_LTE,

	/*!< SV-MT/eNB-MT接続解放確認タイマ(10msec) for 3G		*/
	E_RRH_TIMID_SVMTCNTREL_3G,
	/*!< SV-MT/eNB-MT接続解放確認タイマ(10msec) for LTE		*/
	E_RRH_TIMID_SVMTCNTREL_LTE,
	/* FLD-MTデータファイル転送送信確認タイマ (3G)*/
	E_RRH_TIMID_FLD_FILE_SND_3G,
	/* FLD-MTデータファイル転送送信確認タイマ (LTE)*/
	E_RRH_TIMID_FLD_FILE_SND_LTE,
	/*SV-MT/eNB-MTデータ送信確認タイマ for 3G */
	E_RRH_TIMID_SVMTDATSND_3G,
	/*SV-MT/eNB-MTデータ送信確認タイマ for LTE */
	E_RRH_TIMID_SVMTDATSND_LTE,
	/*!< 外部装置データ送信確認タイマ(1000ms) for 3G */
	E_RRH_TIMID_EXT_DAT_SND_3G,
	/*!< 外部装置データ送信確認タイマ(1000ms) for LTE */
	E_RRH_TIMID_EXT_DAT_SND_LTE,
	/*FLD-MTデータファイル転送受信応答確認タイマ for 3G */
	E_RRH_TIMID_FLDMT_FILE_TRANS_3G,
	/*FLD-MTデータファイル転送受信応答確認タイマ for LTE */
	E_RRH_TIMID_FLDMT_FILE_TRANS_LTE,

	E_RRH_TIMID_EXT_DAT_RCV_3G,										/*!< 外部装置データ受信確認タイマ for 3G */
	E_RRH_TIMID_EXT_DAT_RCV_LTE,									/*!< 外部装置データ受信確認タイマ for LTE */
	/* MTアドレス設定確認タイマ for 3G*/
	E_RRH_TIMID_MTADDSET_3G,
	/* MTアドレス設定確認タイマ for LTE*/
	E_RRH_TIMID_MTADDSET_LTE,

	/* REカード情報報告応答確認待ちタイマ for 3G */
	E_RRH_TIMID_CARDINFOREP_3G,

    E_RRH_TIMID_FWFILE_REP_RE_LTEMIN,																		/*!< FWFILE_UPDATE for S3G*/
    E_RRH_TIMID_FWFILE_REP_RE_LTEMAX = E_RRH_TIMID_FWFILE_REP_RE_LTEMIN + D_RRH_CPRINO_RE_MAX -1,			/*!< FWFILE_UPDATE for S3G*/
    E_RRH_TIMID_USFWFILE_REP_RE_LTEMIN,																		/*!< FWFILE_UPDATE for S3G*/
    E_RRH_TIMID_USFWFILE_REP_RE_LTEMAX = E_RRH_TIMID_USFWFILE_REP_RE_LTEMIN + D_RRH_CPRINO_RE_MAX -1,		/*!< FWFILE_UPDATE for S3G*/

    E_RRH_TIMID_FILEDATSND,
    E_RRH_TIMID_FILEDATSND_MAX = E_RRH_TIMID_FILEDATSND + D_RRH_CPRINO_RE_MAX - 1,
	/* システムパラメータ情報報告確認タイマ */
    E_RRH_TIMID_SYSPRM_INF,
    E_RRH_TIMID_SYSPRM_INF_MAX = E_RRH_TIMID_SYSPRM_INF + D_RRH_CPRINO_RE_MAX * 2 - 1,
    E_RRH_TIMID_SYSPRM_SNDFIN,
    E_RRH_TIMID_SYSPRM_SNDFIN_MAX = E_RRH_TIMID_SYSPRM_SNDFIN + D_RRH_CPRINO_RE_MAX * 2 - 1,
	E_RRH_TIMID_FIRMFILE_REP_3G,
	E_RRH_TIMID_FIRMFILE_REP_3G_MAX = E_RRH_TIMID_FIRMFILE_REP_3G + D_RRH_CPRINO_RE_MAX - 1,
    E_RRH_TIMID_FIRMFILE_REP,
    E_RRH_TIMID_FIRMFILE_REP_MAX = E_RRH_TIMID_FIRMFILE_REP + D_RRH_CPRINO_RE_MAX - 1,
    E_RRH_TIMID_FIRMFILE_INF,
    E_RRH_TIMID_FIRMFILE_INF_MAX = E_RRH_TIMID_FIRMFILE_INF + D_RRH_CPRINO_RE_MAX - 1,
	E_RRH_TIMID_UFIRMFILE_INF,
    E_RRH_TIMID_UFIRMFILE_INF_MAX = E_RRH_TIMID_UFIRMFILE_INF + D_RRH_CPRINO_RE_MAX - 1,
    E_RRH_TIMID_FIRMFILE_SNDFIN,
    E_RRH_TIMID_FIRMFILE_SNDFIN_MAX = E_RRH_TIMID_FIRMFILE_SNDFIN + D_RRH_CPRINO_RE_MAX - 1,
    E_RRH_TIMID_UFIRMFILE_REP,
    E_RRH_TIMID_UFIRMFILE_REP_MAX = E_RRH_TIMID_UFIRMFILE_REP + D_RRH_CPRINO_RE_MAX - 1,
    E_RRH_TIMID_UFIRMFILE_SNDFIN,
    E_RRH_TIMID_UFIRMFILE_SNDFIN_MAX = E_RRH_TIMID_UFIRMFILE_SNDFIN + D_RRH_CPRINO_RE_MAX - 1,

	D_RRH_TIMID_CPRIDSCST_FLG,
	D_RRH_TIMID_CPRIDSCST_FLG_MAX = D_RRH_TIMID_CPRIDSCST_FLG + D_RRH_CPRINO_RE_MAX,
	D_RRH_TIMID_CPRIDSCST_FLG_LAST = D_RRH_TIMID_CPRIDSCST_FLG_MAX - 1,

	/* RE立ち上げタイマ	*/
	E_RRH_TIMID_STARTUPRE,
	/* REバージョン報告確認タイマ for 3G */
	E_RRH_TIMID_RE_VER_REP_3G,
	/* REバージョン報告確認タイマ for LTE */
	E_RRH_TIMID_RE_VER_REP_LTE,
	/* Step7完了待ちタイマ */
	E_RRH_TIMID_WAIT_COMPLETE_STEP7,
	E_RRH_TIMID_LPB_10MSEC_INT,
	E_RRH_TIMID_APL_MAX_FHM												/*!< TimerID数											*/
};

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define	M_RRH_TIMID_HCRSP_CNF(cprino, sysnum) ((CMD_SYS_3G==sysnum ? E_RRH_TIMID_HCRSP1_CNF_3G : E_RRH_TIMID_HCRSP1_CNF_S3G) + (cprino - D_RRH_CPRINO_RE_MIN))
#define	M_RRH_TIMID_HCSND_CNF(cprino, sysnum) ((CMD_SYS_3G==sysnum ? E_RRH_TIMID_HCSND1_CNF_3G : E_RRH_TIMID_HCSND1_CNF_S3G) + (cprino - D_RRH_CPRINO_RE_MIN))

#define	M_RRH_TIMID_CPRI_LINKUP(cprino) (E_RRH_TIMID_CPRI_LINKUP1 + (cprino - D_RRH_CPRINO_RE_MIN))
#define	M_RRH_CPRINO_CPRI_LINKUP(timeid) ((timeid - E_RRH_TIMID_CPRI_LINKUP1) + D_RRH_CPRINO_RE_MIN)

#define	M_RRH_TIMID_CPRIDSCST_FLG(cprino) (D_RRH_TIMID_CPRIDSCST_FLG + (cprino - D_RRH_CPRINO_RE_MIN))
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @} */

/*!
 * @name File Kind
 * @note File Kindの定義
 * @{
 */
enum
{
	E_RRH_FILE_KIND_PLTFRM0 = 0,							/*!< FPGAのファイル更新					*/
	E_RRH_FILE_KIND_KERNEL0,								/*!< カーネル＋OSのファイル				*/
	E_RRH_FILE_KIND_RFICARM0,								/*!< RFICのファイル(ARM)				*/
	E_RRH_FILE_KIND_RFICFRM0,								/*!< RFICのファイル(Firm)				*/
	E_RRH_FILE_KIND_PTPPLL0,								/*!< PTP-PLL初期設定ファイル			*/
	E_RRH_FILE_KIND_TXRPL0,									/*!< TRX-PL(Slave)のファイル更新		*/
	E_RRH_FILE_KIND_PLTFRM1 = 10,							/*!< FPGAのファイル更新					*/
	E_RRH_FILE_KIND_KERNEL1,								/*!< カーネル＋OSのファイル				*/
	E_RRH_FILE_KIND_RFICARM1,								/*!< RFICのファイル(ARM)				*/
	E_RRH_FILE_KIND_RFICFRM1,								/*!< RFICのファイル(Firm)				*/
	E_RRH_FILE_KIND_PTPPLL1,								/*!< PTP-PLL初期設定ファイル			*/
	E_RRH_FILE_KIND_TXRPL1,									/*!< TRX-PL(Slave)のファイル更新		*/
	E_RRH_FILE_KIND_MAX
};


/**
 * @name SIGUSR1オプション情報 リセット要求
 * @note SIGUSR1オプション情報 リセット要求 definition
 */
/* @{ */
#define	D_RRH_SIGUSR_OPT_RSET_LOCSW_NTC		0x00010000				/*!< ローカルリセット[ローカルRST SW]					*/
#define	D_RRH_SIGUSR_OPT_RSET_L3_RMT_NTC	0x00020000				/*!< リモートリセット[REカード制御要求(L3 MSG)]			*/
#define	D_RRH_SIGUSR_OPT_RSET_L1IND_NTC		0x00040000				/*!< RE強制リセット[CPRI L1 RST]						*/
#define	D_RRH_SIGUSR_OPT_RSET_L3_RST_NTC	0x00080000				/*!< REリセット[REリセット要求(L3 MSG)]					*/
#define	D_RRH_SIGUSR_OPT_RSET_SWALM_NTC		0x00100000				/*!< ソフト障害検知(ALM発生時)							*/
#define	D_RRH_SIGUSR_OPT_RSET_SWERR_NTC		0x00200000				/*!< ソフト障害検知(エラー発生時)						*/
#define	D_RRH_SIGUSR_OPT_RSET_INIALM_NTC	0x00800000				/*!< ソフト障害検知(ALM発生時)							*/

#define	D_RRH_SIGUSR_OPT_RSET_HLT_DISC		D_RRH_SIGUSR_OPT_RSET_SWERR_NTC + 0x43	/*!< ソフト障害検知(エラー発生時)_HealthCheck断	*/
#define	D_RRH_SIGUSR_OPT_RSET_RSC_DISC		D_RRH_SIGUSR_OPT_RSET_SWERR_NTC + 0x99	/*!< ソフト障害検知(エラー発生時)_リソースALM	*/

#define	D_RRH_SIGUSR_OPT_RESORCE_ALM_NTC	0x00400000				/*!< リソースALM検出									*/
#define D_RRH_SUGUSR_OPT_RESET_CU_RST_NTC	0x00080000				/*!< M-Planeからの装置リセット[CUからのリセット要求]	*/
#define D_RRH_SUGUSR_OPT_RESET_WDT_RST_NTC	0x00080001				/*!< M-Planeからの装置リセット[M-PlaneのWDT発生]		*/

/* @} */

/**
 * @name SIGUSR1オプション情報 障害ログ出力要求 INIT->各Process向け
 * @note SIGUSR1オプション情報 障害ログ出力要求 INIT->各Process definition
 */
/* @{ */
#define	D_RRH_SIGUSR_OPT_TROUBLE_REQ		0x01100000				/*!< 障害ログ出力要求(ALM発生)							*/
#define	D_RRH_SIGUSR_OPT_TROUBLE_REQ_RST	0x01100001				/*!< 障害ログ出力要求(ALM発生_RESET有)					*/
#define	D_RRH_SIGUSR_OPT_HLTCHK_REQ			0x01200043				/*!< 障害ログ出力要求(ヘルスチェック断)					*/
/* @} */

/**
 * @name SIGUSR1オプション情報 障害ログ出力応答 各Process->INIT
 * @note SIGUSR1オプション情報 障害ログ出力応答 各Process->INIT definition
 */
/* @{ */
#define	D_RRH_SIGUSR_OPT_TROUBLE_RES		0x02100000				/*!< 障害ログ出力応答(ALM発生)							*/
#define	D_RRH_SIGUSR_OPT_HLTCHK_RES			0x02200043				/*!< 障害ログ出力応答(ヘルスチェック断)					*/
/* @} */

/**
 * @name SIGUSR1オプション情報 運用ログ出力要求 INIT<->PF_MAIN向け
 * @note SIGUSR1オプション情報 運用ログ出力要求 INIT<->PF_MAIN向け definition
 */
/* @{ */
#define	D_RRH_SIGUSR_OPT_RUNLOG_REQ			0x11000000				/*!< 運用ログ出力要求									*/
#define	D_RRH_SIGUSR_OPT_RUNLOG_GET			0x12000000				/*!< 運用ログ取得										*/
#define	D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI		0x13000000				/*!< 運用ログ出力完了(INITへ)							*/
#define	D_RRH_SIGUSR_OPT_RUNLOG_FIN_PF		0x14000000				/*!< 運用ログ出力完了通知(PFへ)							*/
#define	D_RRH_SIGUSR_OPT_RUNLOG_REQ_3G		0x11000001				/*!< 運用ログ出力要求(3G)								*/
#define	D_RRH_SIGUSR_OPT_RUNLOG_GET_3G		0x12000001				/*!< 運用ログ取得(3G)									*/
#define	D_RRH_SIGUSR_OPT_RUNLOG_FIN_INI_3G	0x13000001				/*!< 運用ログ出力完了(INITへ)(3G)						*/
#define	D_RRH_SIGUSR_OPT_RUNLOG_FIN_PF_3G	0x14000001				/*!< 運用ログ出力完了通知(PFへ)(3G)						*/
/* @} */

/**
 * @name SIGUSR1オプション情報 障害ログ書き込み要求 INIT<->PF_MAIN向け
 * @note SIGUSR1オプション情報 障害ログ書き込み要求 INIT<->PF_MAIN向け definition
 */
/* @{ */
#define	D_RRH_SIGUSR_OPT_TROUBLE_FLASH0_REQ	0x15000000				/*!< 障害ログ書き込み要求(PFへ)							*/
#define	D_RRH_SIGUSR_OPT_TROUBLE_FLASH1_REQ	0x15000001				/*!< 障害ログ書き込み要求(PFへ)							*/
#define	D_RRH_SIGUSR_OPT_TROUBLE_FLASH2_REQ	0x15000002				/*!< 障害ログ書き込み要求(PFへ)							*/
#define	D_RRH_SIGUSR_OPT_HEALTH_FLASH_REQ	0x15000003				/*!< 障害ログ書き込み要求(PFへ)							*/
/* @} */

/**
 * @name SIGUSR1オプション情報 障害ログ書き込み要求 INIT->INIT向け
 * @note SIGUSR1オプション情報 障害ログ書き込み要求 INIT->INIT向け definition
 */
/* @{ */
#define	D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_ALM	0x16000000				/*!< ログ出力次処理要求(障害)							*/
#define	D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_HLT	0x16000001				/*!< ログ出力次処理要求(ヘルスチェック)					*/
#define	D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_RUN	0x16000002				/*!< ログ出力次処理要求(運用ログ)						*/
#define	D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_RUN_3G	0x16000003			/*!< ログ出力次処理要求(運用ログ)(3G)					*/
/* @} */

/**
 * @name SIGUSR1オプション情報 リセット範囲情報
 * @note SIGUSR1オプション情報 リセット範囲情報 definition
 */
/* @{ */
#define	D_RRH_SIGUSR_OPT_RESET_DPDC_VAL		0x00000001				/*!< DPDCのみ											*/
/* @} */

/*!
 * @name SIGUSR2オプション情報定義
 * @note SIGUSR2オプション情報定義 define
 * @{
 */
#define D_RRH_SIGUSR2_OPT_WDTTO				0x00000001				/*!< USR2 WDT タイムアウト								*/
#define D_RRH_SIGUSR2_OPT_CPRIL1RST			0x00000010				/*!< USR2 CPRI L1 リセット								*/
#define D_RRH_SIGUSR2_OPT_RSTSW				0x80000000				/*!< USR2 ローカルリセットスイッチ						*/
#define D_RRH_SIGUSR2_OPT_RLBK				0x00000100				/*!< USR2 ロールバック									*/
#define D_RRH_SIGUSR2_OPT_RSTCMD			0x00001000				/*!< USR2 DUコマンドリセット							*/
/* @} */

/**
 * @name MAXIMUM constant
 * @note MAXIMUM constant definition
 */
/* @{ */
#define	D_RRH_FLASH_WRT_MAX					2						/*!< Maximum of FLASH access info		*/
/* @} */

/**
 * @name The Maximum Assrt log record
 * @note The Maximum Assrt log record define
 */
/* @{ */
#define	D_RRH_LOG_MAX_ASSERTLOGNUM 			1023					/*!< Maximum assert log record			*/
#define	D_RRH_LOG_MAX_ASSERTLOGBODY			200						/*!< Maximum assert log body			*/
#define	D_RRH_LOG_MAX_ASSERTFILENAME		32						/*!< Maximum assert log filename		*/
/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/**
 * @name The Maximum Assrt log for DWL record
 * @note The Maximum Assrt log for DWL record define
 * @{
 */
#define	D_RRH_LOG_MAX_ASSERTLOGNUM_DWL 		256					/*!< Maximum assert log record			*/
/*! @} */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/**
 * @name The Assert log level
 * @note The Assert log level
 */
/* @{ */
#define	D_RRH_LOG_AST_LV_ENTER				0x00000001				/*!< Assert Log Level:ENTER				*/
#define	D_RRH_LOG_AST_LV_RETURN				0x00000002				/*!< Assert Log Level:RETURN			*/
#define	D_RRH_LOG_AST_LV_INFO				0x00000004				/*!< Assert Log Level:INFO				*/
#define	D_RRH_LOG_AST_LV_WARNING			0x00000008				/*!< Assert Log Level:WARNING			*/
#define	D_RRH_LOG_AST_LV_ERROR				0x00000010				/*!< Assert Log Level:ERROR				*/
#define	D_RRH_LOG_AST_LV_CRITICAL			0x00000020				/*!< Assert Log Level:CRITICAL			*/
#define D_RRH_LOG_AST_DEFAULT	D_RRH_LOG_AST_LV_INFO | D_RRH_LOG_AST_LV_WARNING | D_RRH_LOG_AST_LV_ERROR | D_RRH_LOG_AST_LV_CRITICAL
#define D_RRH_LOG_AST_DEFAULT_NOINFO					D_RRH_LOG_AST_LV_WARNING | D_RRH_LOG_AST_LV_ERROR | D_RRH_LOG_AST_LV_CRITICAL
/* @} */

/**
 * @name The Maximum Register Access History log record
 * @note The Maximum Register Access History log record define
 */
/* @{ */
#define	D_RRH_LOG_REGLOG_RCD_NUM			4095					/*!< register log record size			*/
/* @} */

/**
 * @name The SPI Access History log record
 * @note The SPI Access History log record define
 */
/* @{ */
#define	D_RRH_LOG_SPILOG_RCD_NUM			4095					/*!< SPI log record size				*/
/* @} */

/**
 * @name The Temprature log record
 * @note The Temprature log record define
 */
/* @{ */
#define	D_RRH_LOG_TEMP_LOG_NUM 				1440					/*!< Maximum assert log record			*/
#define	D_RRH_LOG_TEMP_MEAS_PERIOD 			60						/*!< temprature measurement period		*/

#define	D_RRH_SENSOR_SHT20_NUM_SLC 			4						/*!< SHT20数							*/
#define	D_RRH_SENSOR_INVALID_VAL 			-32768					/*!< sensor情報							*/
/* @} */
/*!
 * @note statistics
 */
#define D_RRH_L1_STATI_L1SIG_NUM		8    /* l1 statistics singal number */
#define D_RRH_L2_STATI_L2SIG_NUM		85   /* l2 statistics singal number(LBD_L2_STSNUMと連動しているもの) */
#define D_RRH_L2_STATI_L3SIG_NUM		100  /* l3 statistics singal number */
#define D_RRH_L1_STATI_L1BLK_NUM		32   /* l1 statistics block number */
#define D_RRH_L2_STATI_L2BLK_NUM		32   /* l2 statistics block number */
#define D_RRH_L2_STATI_L3BLK_NUM		16   /* l3 statistics block number */
#define D_RRH_L2_STATI_L3LOOP_NUM		50   /* l3 statistics loop number */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_L2_STATI_L3SIG_SYSTEM_TYPE    2 /*!< l3 statistics singal number */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/* @} */

/*!
 * @note L2 L3 Send and Receive message Log information number
 */
#define D_RRH_L2_TXRV_LOG_DATA_LEN		96
#define D_RRH_L2_TXRV_LOG_INF_NUM		320
#define D_RRH_L2_TXRV_LOG_BLKS			8
#define D_RRH_L2_TXRV_LOG_SPEC_INF_NUM	64
/* @} */

/*!
 * @note 通信用共有メモリ獲得履歴用
 */
#define D_RRH_LOG_BUFFLOG_RCD_NUM			512			/* 通信用共有メモリ獲得履歴	*/
#define D_RRH_LOG_BUFFLOG_BUFFKIND_NUM		2			/* バッファ種別の数			*/
/* @} */

/*!
 * @note タイマ履歴用
 */
#define D_RRH_LOG_TIMERLOG_RCD_NUM		128				/* TimerログMAX数	*/

#define D_RRH_LOG_TIMERKIND_START		0				/* Timer Start		*/
#define D_RRH_LOG_TIMERKIND_STOP		1				/* Timer Stop		*/
#define D_RRH_LOG_TIMERKIND_TIMEOUT		2				/* Timer Timeout	*/

#define D_RRH_LOG_TIMERKIND_NUM			3				/* Timer Kind Num	*/
/* @} */

/*!
 * @note セマフォ履歴用
 */
#define D_RRH_LOG_SEMAPHORE_RCD_NUM		128			/* セマフォ取得解放履歴ログMAX数 */
#define D_RRH_LOG_SEMAPHOREKIND_TAKE 	0			/* 種別：セマフォ取得			 */
#define D_RRH_LOG_SEMAPHOREKIND_GIVE 	1			/* 種別：セマフォ解放			 */
#define D_RRH_LOG_SEMAPHOREKIND_NUM 	2			/* セマフォの種別の数			 */
/* @} */

/*!
 * @note l1 log info
 */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_L1_ERR_COUNT_SIZE			144
#define D_RRH_L1_ERR_INFO_SIZE			600
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define D_RRH_L1_ERR_20MCOUNT_SIZE		252
#define D_RRH_L1_ERR_20MINFO_SIZE		256
#define D_RRH_L1_LOSLOF_SIZE			256
/* @} */

/** @name Max number of SV State Log record */
/* @{ */
#define D_RRH_SVLOG_MAX					1024
#define D_RRH_SVCLOG_MAX				64
/* @} */

/** @name Max number of SFP Info Log record */
/* @{ */
#define D_RRH_SFPLOG_MAX				(128+1)		/* 履歴128件＋配列[0]に最新を置くための領域を持たせ129件とする */
#define D_RRH_SFPINFO_LEN				148
/* @} */

/** @name SFP mount status */
/* @{ */
#define D_RRH_SFP_MOUNT					0
#define D_RRH_SFP_UNMOUNT				1
#define D_RRH_SFPINF_MAX				128

#define D_RRH_SFP_MAXNUM				3 /* 2021/03/03 M&C)DuongMX Item No.35 - SFP Management (modify) */
/* @} */

/**
 * @name Flashへ退避するログの種別数
 * @note Flashへ退避するログの種別数の定義
 */
/* @{ */
#define D_RRH_FLASH_LOG_MAX					10						/*!< ログの数							*/
#define D_RRH_FLASH_LOG_SIZE				0x100000				/*!< 1つのログのMAXサイズ				*/
#define D_RRH_FLASH_LOG_SIZE_BEFORE			0x300000				/*!< 解凍前サイズ						*/
#define D_RRH_FLASH_LOG_DECOMP_TEMP			3*1024*1024+2			/*!< 解凍用temporary領域サイズ			*/
/* @} */
 
/**
 * @name Flashへ退避する運用データ関連のdefine
 * @note Flashへ退避する運用データ関連のdefine定義
 */
/* @{ */
#define D_RRH_FLASH_INIT_DATA_SIZE			0x100000				/*!< FLASH sectorサイズ					*/
/* @} */

/**
 * @name The Register Access History log level
 * @note The Register Access History log level
 */
/* @{ */
#define	D_RRH_LOG_REG_LV_READ				0x00000001				/*!< Reg Log Level:READ					*/
#define	D_RRH_LOG_REG_LV_READ_BUF			0x00000002				/*!< Reg Log Level:READ BUFF			*/
#define	D_RRH_LOG_REG_LV_WRITE				0x00000010				/*!< Reg Log Level:WRITE				*/
#define	D_RRH_LOG_REG_LV_WRITE_BUF			0x00000020				/*!< Reg Log Level:WRITE BUFF			*/
#define	D_RRH_LOG_REG_LV_WRITE_POL			0x00000040				/*!< Reg Log Level:WRITE POLLING		*/
#define	D_RRH_LOG_REG_LV_READ_WRITE			0x00000100				/*!< Reg Log Level:READ_WRITE			*/
#define	D_RRH_LOG_REG_LV_READ_BUSY			0x00000200				/*!< Reg Log Level:READ_BUSY			*/
#define	D_RRH_LOG_REG_LV_HDL_READ			0x00000400				/*!< Reg Log Level:HDL READ				*/
#define	D_RRH_LOG_REG_LV_HDL_WRITE			0x00000800				/*!< Reg Log Level:HDL WRITE			*/
#define	D_RRH_LOG_REG_LV_HDL_READ_BUF		0x00001000				/*!< Reg Log Level:HDL READ_BUF			*/
#define	D_RRH_LOG_REG_LV_HDL_WRITE_BUF		0x00002000				/*!< Reg Log Level:HDL WRITE_BUF		*/
#define	D_RRH_LOG_REG_LV_HDL_READ_WRITE		0x00004000				/*!< Reg Log Level:HDL READ_WRITE		*/
#define	D_RRH_LOG_REG_LV_READ_CPRI			0x00010000				/*!< Reg Log Level:CPRI READ			*/
#define	D_RRH_LOG_REG_LV_WRITE_CPRI			0x00020000				/*!< Reg Log Level:CPRI WRITE			*/
#define	D_RRH_LOG_REG_LV_READ_WRITE_CPRI	0x00040000				/*!< Reg Log Level:CPRI READ WRITE		*/
/* @} */

/**
 * @name The Macimum Message Data log size
 * @note The Macimum Message Data log size define
 */
/* @{ */
#define	D_RRH_LOG_PROC_DEAD_LOG_DATA_SIZE	65528					/*!< Process deadログのデータサイズ		*/
/* @} */

/*!
 * Alarm History log用 レジスタ情報
 * @date 2015/08/06 TDI)satou FHM用に全面見直し
 * @date 2020/12/28 M&C) Merge 4G FHM src
 */
enum
{
    E_RRH_REG_CNT_SV1INF,                   /*!< SV情報1                          */
    E_RRH_REG_CNT_SV2INF,                   /*!< SV情報2                          */
    E_RRH_REG_CNT_SV3INF,                   /*!< SV情報3                          */
    E_RRH_REG_CNT_SV4INF,                   /*!< SV情報4                          */
    E_RRH_REG_CNT_SV5INF,                   /*!< SV情報5                          */
    E_RRH_REG_CNT_SV6INF,                   /*!< SV情報6                          */
    E_RRH_REG_CNT_SV7INF,                   /*!< SV情報7                          */
    E_RRH_REG_CNT_SV8INF,                   /*!< SV情報8                          */
    E_RRH_REG_CNT_SV9INF,                   /*!< SV情報9                          */
    E_RRH_REG_CNT_SV10INF,                  /*!< SV情報10                         */
    E_RRH_REG_CNT_SV11INF,                  /*!< SV情報11                         */
    E_RRH_REG_CNT_SV12INF,                  /*!< SV情報12                         */
    E_RRH_REG_CNT_SV13INF,                  /*!< SV情報13                         */
    E_RRH_REG_CNT_SV14INF,                  /*!< SV情報14                         */
    E_RRH_REG_CNT_SV15INF,                  /*!< SV情報15                         */
    E_RRH_REG_CNT_SV16INF,                  /*!< SV情報16                         */
    E_RRH_REG_CPRI_CPSTAT0,                 /*!< CPRIステート表示(port0)          */
    E_RRH_REG_CPRI_CPSTAT1,                 /*!< CPRIステート表示(port1)          */
    E_RRH_REG_CPRI_CPSTAT2,                 /*!< CPRIステート表示(port2)          */
    E_RRH_REG_CPRI_CPSTAT3,                 /*!< CPRIステート表示(port3)          */
    E_RRH_REG_CPRI_CPSTAT4,                 /*!< CPRIステート表示(port4)          */
    E_RRH_REG_CPRI_CPSTAT5,                 /*!< CPRIステート表示(port5)          */
    E_RRH_REG_CPRI_CPSTAT6,                 /*!< CPRIステート表示(port6)          */
    E_RRH_REG_CPRI_CPSTAT7,                 /*!< CPRIステート表示(port7)          */
    E_RRH_REG_CPRI_CPSTAT8,                 /*!< CPRIステート表示(port8)          */
    E_RRH_REG_CPRI_CPSTAT9,                 /*!< CPRIステート表示(port9)          */
    E_RRH_REG_CPRI_CPSTAT10,                /*!< CPRIステート表示(port10)         */
    E_RRH_REG_CPRI_CPSTAT11,                /*!< CPRIステート表示(port11)         */
    E_RRH_REG_CPRI_CPSTAT12,                /*!< CPRIステート表示(port12)         */
    E_RRH_REG_CPRI_CPSTAT13,                /*!< CPRIステート表示(port13)         */
    E_RRH_REG_CPRI_CPSTAT14,                /*!< CPRIステート表示(port14)         */
    E_RRH_REG_CPRI_CPSTAT15,                /*!< CPRIステート表示(port15)         */
    E_RRH_REG_CPRI_CPSTAT16,                /*!< CPRIステート表示(port16)         */
    E_RRH_REG_PCI_BRDG_STS_CTL,             /*!< Bridge Status and Control        */
    E_RRH_REG_PCI_PHY_STS_CTL,              /*!< PHY Status/Control               */
    E_RRH_REG_PCI_ROOTPORT_STS_CTL,         /*!< Root Port Status/Control         */
    E_RRH_REG_PCI_ROOTPORT_ERR_FIFO,        /*!< Root Port Error FIFO Read        */
    E_RRH_REG_PCI_ROOTPORT_INT_FIFO1,       /*!< Root Port Interrupt FIFO Read1   */
    E_RRH_REG_PCI_ROOTPORT_INT_FIFO2,       /*!< Root Port Interrupt FIFO Read2   */
	E_RRH_REG_NUM_MAX
};

#define	D_RRH_ALMHIS_LOG_RECORD_NUM			16						/*!< Alarm History Log Record Num		*/
#define	D_RRH_ALMHIS_LOG_MEAS_NUM			20						/*!< Measuerment Log num				*/
#define	D_RRH_ALMHIS_AST_LOG_NUM			32						/*!< Alarm History Assert Log Num		*/
#define	D_RRH_ALMHIS_REG_LOG_NUM			32						/*!< Alarm History Assert Log Num		*/
#define	D_RRH_ALMHIS_SVREG_INFO_NUM			16                      /*!< Alarm History Info SV Reg Num		*/ /* 5GDU_chg */

/* レジスタ情報の数(D_DPDA_REG_SUB6_NUM_MAX又はD_DPDA_REG_MMW_NUM_MAX)の大きい方に合わせる) */
#define	D_RRH_ALMHIS_REG_VAL_NUM			192						/*!< Alarm History Info Reg Num 		*/
#define	D_RRH_ALMHIS_REG_NUM_SUB6			154						/*!< Alarm History Info Reg Num(sub6)	*/
#define	D_RRH_ALMHIS_REG_NUM_MMW			98						/*!< Alarm History Info Reg Num(mmW)	*/

#define	D_RRH_ALMHIS_REG_EXT_NUM			5						/*!< Alarm History Info Reg Num extra	*/
#define	D_RRH_ALMHIS_EXT_NUM_SUB6			0						/*!< Alarm History Info Reg Num extra(sub6)	*/
#define	D_RRH_ALMHIS_EXT_NUM_MMW			5						/*!< Alarm History Info Reg Num extra(mmW)	*/


/* Alarm History log用 レジスタテーブルINDEX(SRE) */
enum
{
	E_RRH_ALMHIS_ALM_FACTOR_HW0 = 0,								/*!<  Alarm Factor HW0					*/
	E_RRH_ALMHIS_ALM_FACTOR_HW1,									/*!<  Alarm Factor HW1					*/
	E_RRH_ALMHIS_ALM_FACTOR_HW2,									/*!<  Alarm Factor HW2					*/
	E_RRH_ALMHIS_ALM_FACTOR_NUM=16									/*!<  Alarm Factor Num					*/
};

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define	D_RRH_REG_ADDR_LOG_ALM_NUM_SLC			127					/*!< アラーム発生時収集レジスタ数(SLC)	*/
#define	D_RRH_REG_ADDR_LOG_ALM_NUM_1CHP			63					/*!< アラーム発生時収集レジスタ数(1CHP)	*/
#define	D_RRH_ALMHIS_REG_ALM_NUM			D_RRH_REG_ADDR_LOG_ALM_NUM_SLC	/*!< Alarm History Info Reg Num(SLC)	*/
/* 下記 define値のうち、値の大きいほうを採用する */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define	D_RRH_ALMHIS_APD_BUF_LOG_ALM_NUM		6					/*!< Alarm History Info apd buf(ALM発生時)		*/
#define	D_RRH_ALMHIS_APD_BUS_LOG_ALM_NUM		6					/*!< Alarm History Info apd bus(ALM発生時)		*/
#define	D_RRH_ALMHIS_APD_BUF_30S_RCD_NUM_SLC	240					/*!< Alarm History Info apd buf record 30s(SLC)	*/
#define	D_RRH_ALMHIS_APD_BUF_ADR_NUM_SLC		20					/*!< Alarm History Info apd buf address(SLC)	*/

/* @} */

/**
 * @name 起動面情報
 * @note 起動面情報の定義
 */
/* @{ */
#define	D_RRH_BOOT_ACT0						0x0000					/*!< 0面ACT								*/
#define	D_RRH_BOOT_ACT1						0x0001					/*!< 1面ACT								*/
#define	D_RRH_BOOT_FACE_NUM					2						/*!< 面数								*/
/* @} */

/**
 * @name Register Access Log Mask
 * @note Register Access Log Mask define
 */
/* @{ */
#define	D_RRH_REGLOG_MASK_READ		 		1						/*!< read log only							*/
#define	D_RRH_REGLOG_MASK_WRITE		 		2						/*!< write log only							*/
/* @} */

/*!
 * @name antenna carrier definition
 * @note antenna carrier definition
 * @{
 */
#define D_RRH_ANT_A										0			/* アンテナA							*/
#define D_RRH_ANT_B										1			/* アンテナB							*/
#define D_RRH_ANT_C										2			/* アンテナC							*/
#define D_RRH_ANT_D										3			/* アンテナD							*/
#define D_RRH_ANT_E										4			/* アンテナE							*/ /* 5GDU_chg*/
#define D_RRH_ANT_F										5			/* アンテナF							*/ /* 5GDU_chg*/
#define D_RRH_ANT_G										6			/* アンテナG							*/ /* 5GDU_chg*/
#define D_RRH_ANT_H										7			/* アンテナH							*/ /* 5GDU_chg*/
#define D_RRH_ANT_MAX									4			/* アンテナ数(sub6用)					*/ /* 5GDU_chg*/
#define D_RRH_ANT_MAX_MMW								2			/* アンテナ数(mmW用)					*/ /* 5GDU_chg*/
#define D_RRH_CAR_1										0			/* Carrier1								*/
#define D_RRH_CAR_2										1			/* Carrier2								*/
#define D_RRH_CAR_3										2			/* Carrier3								*/ /* 5GDU_chg*/
#define D_RRH_CAR_4										3			/* Carrier4								*/ /* 5GDU_chg*/
#define D_RRH_CAR_5										4			/* Carrier5								*/ /* 5GDU_chg*/
#define D_RRH_CAR_6										5			/* Carrier6								*/ /* 5GDU_chg*/
#define D_RRH_CAR_7										6			/* Carrier7								*/ /* 5GDU_chg*/
#define D_RRH_CAR_8										7			/* Carrier8								*/ /* 5GDU_chg*/
#define D_RRH_CAR_MAX									4			/* Carrier MAX数						*/ /* 5GDU_chg*/
#define D_RRH_CAR_3G_MAX								4			/* 3G Carrier MAX数						*/
#define D_RRH_ANT_UL									0			/* UL(recv)								*/
#define D_RRH_ANT_DL									1			/* DL(trans)							*/
#define D_RRH_ANT_ULDL_MAX								2			/* アンテナ方向数						*/
#define D_RRH_ANT_UNIT_AB								0			/* アンテナUINT(AB)						*/
#define D_RRH_ANT_UNIT_CD								1			/* アンテナUINT(CD)						*/
#define D_RRH_ANT_UNIT_EF								2			/* アンテナUINT(EF)						*/ /* 5GDU_chg*/
#define D_RRH_ANT_UNIT_GH								3			/* アンテナUINT(GH)						*/ /* 5GDU_chg*/
#define D_RRH_ANT_UNIT_MAX								2			/* アンテナUINT数(sub6用)				*/ /* 5GDU_chg*/
#define D_RRH_ANT_UNIT_MAX_MMW							1			/* アンテナUINT数(mmW用)				*/ /* 5GDU_chg */

#define D_RRH_POLARIZATION_NUM							2			/* 偏波面数								*/

/* @} */

/*!
 * @name Performance definition
 * @note Performance definition
 * @{
 */
#define D_RRH_PFM_NUM_OF_RXWIN							6			/* RX_WINDOW数							*/
#define D_RRH_PFM_NUM_OF_INFO							480			/* Perfomance ログ面数					*/
#define D_RRH_PFM_NUM_OF_REG							36			/* Perfomance レジスタ数				*/
#define D_RRH_PFM_NUM_OF_REG_COM						14			/* Perfomance レジスタ数				*/
#define D_RRH_PFM_NUM_OF_REG_ALL						((D_RRH_PFM_NUM_OF_REG * 4 * 2) + D_RRH_PFM_NUM_OF_REG_COM )		/* Perfomance レジスタ数				*/
#define D_RRH_PFM_NUM_OF_INFO_PER						60			/* Perfomance ログ面数					*/

/* @} */

/*!
 * @name Calibration coefficient definition
 * @note Calibration coefficient definition
 * @{
 */
#define D_RRH_ANT_CAL_COEF_INIT							0x40000000	/* Calibration係数						*/
/* @} */

/*!
 * @name cpri definition
 * @note cpri definition
 * @{
 */
#define D_RRH_CPRI_MAX									1			/* CPRI definition						*/
/* @} */

/*!
 * @name rat type definition
 * @note rat type definition
 * @{
 */
#define D_RRH_TYPE_CDMA									0			/*!< CDMA 				*/
#define D_RRH_TYPE_LTE									1			/*!< LTE				*/
#define D_RRH_TYPE_SIG_KIND_NUM							2			/*!< 信号種別数			*/

#define D_RRH_RAT_TYPE_BBU_CDMA							4			/*!< CDMA 				*/
#define D_RRH_RAT_TYPE_BBU_LTE							0			/*!< LTE				*/

/****************************************************************************/
/* 周波数番号																*/
/****************************************************************************/
#define D_RRH_FREQ_17G_3G_MIN							8837		/* 1.7G RRE 3G周波数番号MIN			*/
#define D_RRH_FREQ_17G_3G_MAX							8912		/* 1.7G RRE 3G周波数番号MAX			*/
#define D_RRH_FREQ_17G_3G_2_MIN							8562		/* 1.7G RRE 3G周波数番号MIN (FJT spec)	*/
#define D_RRH_FREQ_17G_3G_2_MAX							8836		/* 1.7G RRE 3G周波数番号MAX (FJT spec)	*/
#define D_RRH_FREQ_17G_3G_3								8913		/* 1.7G RRE 3G周波数番号    (FJT spec)	*/
#define D_RRH_FREQ_15G_LTE_MIN							6475		/* 1.5G RRE LTE周波数番号MIN		*/
#define D_RRH_FREQ_15G_LTE_MAX							6575		/* 1.5G RRE LTE周波数番号MAX		*/
#define D_RRH_FREQ_17G_LTE_MIN							1774		/* 1.7G RRE LTE周波数番号MIN		*/
#define D_RRH_FREQ_17G_LTE_MAX							1924		/* 1.7G RRE LTE周波数番号MAX		*/
#define D_RRH_FREQ_700M_LTE_MIN							9485		/* 700M RRE LTE周波数番号MIN		*/
#define D_RRH_FREQ_700M_LTE_MAX							9535		/* 700M RRE LTE周波数番号MAX		*/
#define D_RRH_FREQ_35G_TDD_MIN							42415		/* 3.5G TDD LTE周波数番号MIN		*/
#define D_RRH_FREQ_35G_TDD_MAX							42765		/* 3.5G TDD LTE周波数番号MAX		*/

#define D_RRH_S3G_FREQ_MIN								225			/**< S3G freq no minimun		*/
#define D_RRH_S3G_FREQ_MAX								375			/**< S3G freq no maximun		*/
#define D_RRH_S3G_FREQ_800M_MIN							6025		/**< S3G-800MHz freq no minimun		*/
#define D_RRH_S3G_FREQ_800M_MAX							6125		/**< S3G-800MHz freq no maximun		*/
#define D_RRH_S3G_FREQ_15G_MIN							6475		/**< S3G-1.5GHz freq no minimun		*/
#define D_RRH_S3G_FREQ_15G_MAX							6575		/**< S3G-1.5GHz freq no maximun		*/

/* 対応周波数帯 */
#define D_RRH_CRP_FRQ_2G								0			/**< 対応周波数帯 2G				*/
#define D_RRH_CRP_FRQ_800M								1			/**< 対応周波数帯 800M				*/
#define D_RRH_CRP_FRQ_17G								2			/**< 対応周波数帯 1.7G				*/
#define D_RRH_CRP_FRQ_2G_800M							3			/**< 対応周波数帯 2G/800M			*/
#define D_RRH_CRP_FRQ_2G_17G							4			/**< 対応周波数帯 2G/1.7G			*/
#define D_RRH_CRP_FRQ_800M_17G							5			/**< 対応周波数帯 800M/1.7G			*/
#define D_RRH_CRP_FRQ_2G_17G_800						6			/**< 対応周波数帯 2G/800M/1.7G		*/
#define D_RRH_CRP_FRQ_15G								7			/**< 対応周波数帯 1.5G				*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_CRP_FRQ_MAXVAL							D_RRH_CRP_FRQ_15G	/**< 対応周波数帯 最大値	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define D_RRH_CRP_FRQ_2G_S3G							1			/**< 対応周波数帯 2G(S3G報告値)		*/
#define D_RRH_CRP_FRQ_15G_S3G							2			/**< 対応周波数帯 1.5G(S3G報告値)	*/
#define D_RRH_CRP_FRQ_800M_S3G							4			/**< 対応周波数帯 800M(S3G報告値)	*/
#define D_RRH_CRP_FRQ_17G_S3G							16			/**< 対応周波数帯 1.7G(S3G報告値)	*/
#define D_RRH_CRP_FRQ_700M_S3G							8			/**< 対応周波数帯 700(S3G報告値)	*/

#define D_RRH_BW_ID_NONE								0			/*!< bandwidth none		*/
#define D_RRH_BW_ID_5M									1			/*!< bandwidth 5M		*/
#define D_RRH_BW_ID_10M									2			/*!< bandwidth 10M		*/
#define D_RRH_BW_ID_15M									3			/*!< bandwidth 15M		*/
#define D_RRH_BW_ID_20M									4			/*!< bandwidth 20M		*/
#define D_RRH_BW_NUM									5			/*!< num of bandwidth	*/
/* @} */

/*!
 * @name 周波数番号 
 * @note 周波数番号 definition
 * @{  
 */
#define D_RRH_FRQ2G_MIN			9612	/**<  周波数番号 2G					*/
#define D_RRH_FRQ2G_MAX			9888	/**<  周波数番号 2G					*/
#define D_RRH_FRQ800M_1_MIN		4162	/**<  周波数番号 800M				*/
#define D_RRH_FRQ800M_1_MAX		4188	/**<  周波数番号 800M				*/
#define D_RRH_FRQ800M_2_MIN		339		/**<  周波数番号 800M				*/
#define D_RRH_FRQ800M_2_MAX		363		/**<  周波数番号 800M				*/
#define D_RRH_FRQ800M_3			812		/**<  周波数番号 800M				*/
#define D_RRH_FRQ800M_4			837		/**<  周波数番号 800M				*/
#define D_RRH_FRQ800M_5			437		/**<  周波数番号 800M				*/
/* @} */

/*!
 * @name buffer size definition
 * @note buffer size definition
 * @{
 */
#define D_RRH_BUFSIZE_256								256			/*!< buffer size 256	*/
#define D_RRH_BUFSIZE_4096								4096		/*!< buffer size 4096	*/
#define D_RRH_BUFSIZE_8192								8192		/*!< buffer size 8192	*/
/* @} */


/*!
 * @name DataSize definition
 * @note DataSize definition
 * @{
 */
#define D_RRH_DATA_SIZE_160K							0x28000		/*!< DataSize 160KK		*/
#define D_RRH_DATA_SIZE_256K							0x40000		/*!< DataSize 256KK		*/
#define D_RRH_DATA_SIZE_100K							0x19000		/*!< DataSize 100KK		*/
#define D_RRH_DATA_SIZE_1M								0x100000	/*!< DataSize 1000KK	*/
/* @} */

/**
 * @name The Maximum log record
 * @note The Maximum log record define
 */
/* @{ */
#define	D_RRH_LOG_RSTRSN_RCD_NUM			128						/*!< reset reason log record size		*/
/* @} */

/*!
 * @name LastResetReason
 * @note リセット要因
 * @{
 */
#define	D_RRH_RST_RSN_UNKNWN				0x00000000
#define	D_RRH_RST_RSN_POWER					0x00000001
#define	D_RRH_RST_RSN_REQ					0x00000002
#define	D_RRH_RST_RSN_MAN					0x00000003
#define	D_RRH_RST_RSN_MMI					0x00000004
#define	D_RRH_RST_RSN_CTLLINK				0x00000005
#define	D_RRH_RST_RSN_CPRI					0x00000006
#define	D_RRH_RST_RSN_WDT					0x00000007
#define D_RRH_RST_RSN_PRM_CHG				0x00000008
#define	D_RRH_RST_RSN_IMG_FAIL				0x00000009
#define	D_RRH_RST_RSN_HRD_FAIL				0x0000000a
#define	D_RRH_RST_RSN_SFT_FAIL				0x0000000b
#define	D_RRH_RST_RSN_OSE_SHELLD			0x0000000c
#define	D_RRH_RST_RSN_PROCESS_DEAD			0x0000000d
#define	D_RRH_RST_RSN_LINK_TIMEOUT			0x0000000e
#define	D_RRH_RST_RSN_EWDT					0x0000000f
#define	D_RRH_RST_RSN_RLBK					0x00000010
#define	D_RRH_RST_RSN_AUTORST				0x00000011
#define	D_RRH_RST_RSN_RSTSW					0x00000012
#define	D_RRH_RST_RSN_SWRST1				0x00000013
#define D_RRH_RST_RSN_SWRST2				0x00000014
#define	D_RRH_RST_RSN_ALMRST				0x00000015
#define	D_RRH_RST_RSN_L1RST1				0x00000016
#define	D_RRH_RST_RSN_L1RST2				0x00000017
#define	D_RRH_RST_RSN_L3_FAIL				0x00000018
#define	D_RRH_RST_RSN_NUM					0x00000019
/* @} */

/*!
 * @name Reset要因
 * @note リセット要因表示レジスタ内容
 * @{
 */
/* ハードレジスタは[GR1DP1L_CPLD_reg_20131112.xls/リセット要因]を参照 */
#define D_RRH_RST_FACT_SWRST				0x00000020				/*!< Software Reset					*/
#define	D_RRH_RST_FACT_RSTSW				0x00000010				/*!< リセットスイッチリセット		*/
#define	D_RRH_RST_FACT_WDT					0x00000002				/*!< WDT T.OによるRESET				*/
#define	D_RRH_RST_FACT_FPGAREQ				0x00000001				/*!< FPGAからのリセット要求			*/
#define D_MAIN_RST_RSN_MASK_TDD				0x00000033				/*!< リセット要因取得用マスク値   */
/* @} */

/*!
 * @name PF Process Priority
 * @note スレッド優先度
 * @{
 */
#define D_RRH_PRIORITY_MAIN					95					/**< Main Thread							*/
#define D_RRH_PRIORITY_RECV					79					/**< CMU Receive Thread						*/
#define D_RRH_PRIORITY_SEND					79					/**< CMU Send Thread						*/
#define D_RRH_PRIORITY_DBG					0					/**< FrontHaul Surveillance handler			*/
#define D_RRH_PRIORITY_HDLX					98					/**< Interrupt handler						*/
#define D_RRH_PRIORITY_EQS					81					/**< Equipment Surveillance handler			*/
#define D_RRH_PRIORITY_WDT					1					/**< WDT handler							*/
#define D_RRH_PRIORITY_NMA					20					/**< Non-volatile Memory Access handler		*/
#define D_RRH_PRIORITY_NMAS					20					/**< Non-volatile Memory Access handler (Slave)	*/
#define D_RRH_PRIORITY_LGC					10					/**< LoG Collection handler					*/
#define D_RRH_PRIORITY_CCA					49					/**< Component Carrier Access handler		*/
#define D_RRH_PRIORITY_RFA					59					/**< RF Access handler						*/
#define D_RRH_PRIORITY_DHC					50					/**< DHCP handler							*/
#define D_RRH_PRIORITY_SFP					10					/**< SFP monitor handler					*/
#define D_RRH_PRIORITY_FHS					80					/**< FrontHaul Surveillance handler			*/
#define D_RRH_PRIORITY_MPRECV				79					/**< CMU M-plane proxy receive handler		*/
#define D_RRH_PRIORITY_MPSEND				79					/**< CMU M-plane proxy send handler			*/
#define D_RRH_PRIORITY_EOM					80					/**< EthernetOaM handler					*/
#define D_RRH_PRIORITY_EOM_0				80					/**< EthernetOaM handler (fheth0)			*/
#define D_RRH_PRIORITY_EOM_1				80					/**< EthernetOaM handler (fheth1)			*/
#define D_RRH_PRIORITY_FDL					50					/**< File DownLoad handler					*/
#define D_RRH_PRIORITY_FUL					50					/**< File UpLoad handler					*/
#define D_RRH_PRIORITY_PFM					10					/**< Performance Monitor handler			*/
#define D_RRH_PRIORITY_EIO					40					/**< External IO Thread	(5G-商用DU)			*/
#define D_RRH_PRIORITY_MTI					0					/**< MTI Thread	(5G-商用DU)			*/
#define D_RRH_PRIORITY_AIH					40					/**< ALD(AISG) interface Thread				*/


#define D_RRH_PRIORITY_DIA					80					/**< Equipment Diagnosys					*//* 未対応	*/

#define D_RRH_PRIORITY_PTP					70					/**< PTP main Thread						*/
#define D_RRH_PRIORITY_PTPR					49					/**< PTP Recieve Thread						*/
#define D_RRH_PRIORITY_PTPS					49					/**< PTP Send Thread						*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_PRIORITY_HDL					98				/**< Handler Main Thread					*/
#define D_RRH_PRIORITY_ETHR					78				/**< Ethernet Set Thread					*/
#define D_RRH_PRIORITY_RSV					69				/**< RE Supervision Thread					*/
#define D_RRH_PRIORITY_CPRI					69				/**< CPRI Management Thread					*/
#define D_RRH_PRIORITY_DPDA					49				/**< DPD Assistance Thread					*/
#define D_RRH_PRIORITY_DPD					49				/**< DPD thread										*/
#define D_RRH_PRIORITY_NEGO					69				/**< CPRI Auto Nego Thread							*/
#define D_RRH_PRIORITY_FMNG					38				/**< File Management Thread					*/
#define D_RRH_PRIORITY_LOGM					0				/**< Trouble Log Thread								*/
#define D_RRH_PRIORITY_FRQS					20				/**< FrashROM Write(QSPI) Thread			*/
#define D_RRH_PRIORITY_WDTT					1				/**< WDT Thread								*/
#define D_RRH_PRIORITY_DEBG					0				/**< Debug Thread							*/
#define D_RRH_PRIORITY_FRQRE				20				/**< FrashROM Write(QSPI) (for RE)Thread	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/* @} */

/*!
 * @name PF Stack Size
 * @note スタックサイズ
 * @{
 */
#define D_RRH_STACKSIZE_MAIN				0xA000				/**< Main Thread							*/
#define D_RRH_STACKSIZE_RECV				0xA000				/**< CMU Receive Thread						*/
#define D_RRH_STACKSIZE_SEND				0xA000				/**< CMU Send Thread						*/
#define D_RRH_STACKSIZE_DBG					0xA000				/**< FrontHaul Surveillance handler			*/
#define D_RRH_STACKSIZE_HDLX				0xA000				/**< Interrupt handler						*/
#define D_RRH_STACKSIZE_EQS					0xA000				/**< Equipment Surveillance handler			*/
#define D_RRH_STACKSIZE_WDT					0xA000				/**< WDT handler							*/
#define D_RRH_STACKSIZE_NMA					0xA000				/**< Non-volatile Memory Access handler		*/
#define D_RRH_STACKSIZE_NMAS				0xA000				/**< Non-volatile Memory Access handler (Slave)	*/
#define D_RRH_STACKSIZE_LGC					0xA000				/**< LoG Collection handler					*/
#define D_RRH_STACKSIZE_CCA					0xA000				/**< Component Carrier Access handler		*/
#define D_RRH_STACKSIZE_RFA					0xA000				/**< RF Access handler						*/
#define D_RRH_STACKSIZE_DHC					0xA000				/**< DHCP handler							*/
#define D_RRH_STACKSIZE_SFP					0xA000				/**< SFP monitor handler					*/
#define D_RRH_STACKSIZE_FHS					0xA000				/**< FrontHaul Surveillance handler			*/

#define D_RRH_STACKSIZE_EOM					0xA000				/**< EthernetOaM handler					*/
#define D_RRH_STACKSIZE_FDL					0xA000				/**< File DownLoad handler					*/
#define D_RRH_STACKSIZE_FUL					0xA000				/**< File UpLoad handler					*/
#define D_RRH_STACKSIZE_DIA					0xA000				/**< Equipment Diagnosys					*/
#define D_RRH_STACKSIZE_PFM					0xA000				/**< Performance Monitor handler			*/

#define D_RRH_STACKSIZE_HDWDT				0xA000				/**< Handler Thread = IRQ_121:WDT(IP_Macro)	*/
#define D_RRH_STACKSIZE_HD1S				0xA000				/**< Handler Thread = IRQ_122:1s周期		*/
#define D_RRH_STACKSIZE_HD1PPS				0xA000				/**< Handler Thread = IRQ_123:1pps Timing	*/
#define D_RRH_STACKSIZE_HDSV				0xA000				/**< Handler Thread = IRQ_124:SV状変		*/
#define D_RRH_STACKSIZE_HDMSTA1				0xA000				/**< Handler Thread = IRQ_140:ETH1 MAC状変	*/
#define D_RRH_STACKSIZE_HDMSTA2				0xA000				/**< Handler Thread = IRQ_141:ETH2 MAC状変	*/
#define D_RRH_STACKSIZE_DEFAULT				0x0000				/**< default(2M)設定						*/
/* @} */

/*!
 * @name L3 Process ThreadQ-ID
 * @note HDLスレッド数
 * @{
 */
#define D_RRH_HDLTHDNO_MAX									5					/**< HDL 子Thread数							*/
/* @} */

/**
 * @name Training data size
 * @note Training data size define
 */
/* @{ */
#define	D_RRH_TRAING_DATA_SIZE								0x400				/**!< training data size					*/
/* @} */

 /* 5GDU_chg */
/**
 * @name RF用 Training data size
 * @note RF用 Training data size define
 */
/* @{ */
#define	D_RRH_RF_TRAING_DATA_SIZE							0x400				/**!< RF用 training data size					*/
/* @} */


/*!
 * @name HDL Process ThreadQ-ID
 * @note HDLコスレッド定義群
 * @{
 */
/***** 現時点で使用予定のないものも定義	*****/

/* 各スレッド名称		*/
#define D_RRH_THDNAME_HDWDT									"HDWDT"				/**< Handler Thread = IRQ_121:WDT(IP_Macro)	*/
#define D_RRH_THDNAME_HD1S									"HD1S"				/**< Handler Thread = IRQ_122:1s周期		*/
#define D_RRH_THDNAME_HD1PPS								"HD1PPS"			/**< Handler Thread = IRQ_123:1pps Timing	*/
#define D_RRH_THDNAME_HDSV									"HDSV"				/**< Handler Thread = IRQ_124:SV状変		*/
#define D_RRH_THDNAME_HDMSTA1								"HDMSTA1"			/**< Handler Thread = IRQ_140:ETH1 MAC状変	*/
#define D_RRH_THDNAME_HDMSTA2								"HDMSTA2"			/**< Handler Thread = IRQ_141:ETH2 MAC状変	*/
#define D_RRH_THDNAME_HDLALD								"HDLALD"			/**< Handler Thread = IRQ_127:ALD(AISG)		*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_THDNAME_HDL1									"HDL1"				/**< Handler Thread(IRQ65)					*/
#define D_RRH_THDNAME_HDCPRI								"HDCPRI"			/**< Handler Thread(IRQ84)					*/
#define D_RRH_THDNAME_HDCPRI_RE								"HDCPRI_RE"			/**< Handler Thread(IRQ84)					*/
#define D_RRH_THDNAME_HDPCIE								"HDPCIE"			/**< Handler Thread(IRQ52)					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* スレッド優先度	*/
#define D_RRH_PRIORITY_HDWDT								99					/**< Handler Thread = IRQ_121:WDT(IP_Macro)	*/
#define D_RRH_PRIORITY_HD1S									87					/**< Handler Thread = IRQ_122:1s周期		*/
#define D_RRH_PRIORITY_HD1PPS								96					/**< Handler Thread = IRQ_123:1pps Timing	*/
#define D_RRH_PRIORITY_HDSV									90					/**< Handler Thread = IRQ_124:SV状変		*/
#define D_RRH_PRIORITY_HDMSTA1								88					/**< Handler Thread = IRQ_140:ETH1 MAC状変	*/
#define D_RRH_PRIORITY_HDMSTA2								88					/**< Handler Thread = IRQ_141:ETH2 MAC状変	*/
#define D_RRH_PRIORITY_HDLALD								86					/**< Handler Thread = IRQ_127:ALD(AISG)		*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_PRIORITY_HDCPRI								84					/**< Handler Thread(IRQ84)					*/
#define D_RRH_PRIORITY_HDCPRI_RE							85					/**< Handler Thread(IRQ85)					*/
#define D_RRH_PRIORITY_HDPCIE								89					/**< Handler Thread(IRQ52)					*/
#define D_RRH_PRIORITY_HDL1									89					/**< Handler Thread(IRQ65)					*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* 各スレッドのスタックサイズ	*/
#define D_RRH_STACKSIZE_HDWDT								0xA000				/**< Handler Thread = IRQ_121:WDT(IP_Macro)	*/
#define D_RRH_STACKSIZE_HD1S								0xA000				/**< Handler Thread = IRQ_122:1s周期		*/
#define D_RRH_STACKSIZE_HD1PPS								0xA000				/**< Handler Thread = IRQ_123:1pps Timing	*/
#define D_RRH_STACKSIZE_HDSV								0xA000				/**< Handler Thread = IRQ_124:SV状変		*/
#define D_RRH_STACKSIZE_HDMSTA1								0xA000				/**< Handler Thread = IRQ_140:ETH1 MAC状変	*/
#define D_RRH_STACKSIZE_HDMSTA2								0xA000				/**< Handler Thread = IRQ_141:ETH2 MAC状変	*/
#define D_RRH_STACKSIZE_HDLALD								0xA000				/**< Handler Thread = IRQ_127:ALD(AISG)		*/
/* @} */

#define D_RRH_SV_INF_NUM									8					/**< TRA SVバス情報数						*/

/* Requset or Notice message is from DBG task	*/
#define		CMD_TSKIF_DBG_EXEC_CMD_REQ						0x9999				/**< デバッグコマンド実行要求			*/
#define		CMD_TSKIF_DBG_EXEC_CMD_RES						0x9998				/**< デバッグコマンド実行結果			*/
#define		CMD_TSKIF_DBG_EXEC_EXP_CMD_REQ					0x9997				/**< 拡張デバッグコマンド実行要求		*/
#define		CMD_TSKIF_DBG_EXEC_EXP_CMD_RES					0x9996				/**< 拡張デバッグコマンド実行結果		*/

#define		CMD_TSKIF_LOG_CREATE_REQ						0x9001				/**< ログ作成要求						*/


/**
 * @name FPGA実装アドレス
 * @note FPGA実装アドレスデフォルト値の定義
 */
/* @{ */
#define D_RRH_PL_FPGA_ADDR                  0x01000000              /*!< PL-FPGA実装アドレスデフォルト      */
/* @} */

/**
 * @name FPGA実装サイズ
 * @note FPGA実装サイズデフォルト値の定義
 */
/* @{ */
#define	D_RRH_PL_FPGA_SIZE					0x00D00000				/*!< PL-FPGA実装サイズデフォルト		*/
/* @} */


/**
 * @name メッセージ送信先情報管理テーブル
 * @note メッセージ送信先情報管理テーブルの最大数定義
 */
/* @{ */
#define D_RRH_CMU_TABLE_MAX					64						/*!< テーブルの最大値 */
/* @} */

/**
 * @name EMIO board version set use value define
 * @note EMIO board version set use value define
 */
#define D_RRH_BIT_MASK_23_00				0x00FFFFFF
#define D_RRH_BIT_MASK_13_06				0x00003FC0
/* SRE対応 - Start */
#define D_RRH_BIT_MASK_31_28				0xF0000000
/* SRE対応 - End */
#define D_RRH_BIT_SHIFT_18					18
/* SRE対応 - Start */
#define D_RRH_BIT_SHIFT_24					24
#define D_RRH_BIT_SHIFT_14					14
/* SRE対応 - End */
/* @} */


/**
 * @name ALM要因コード
 * @note ALM要因コード
 */

/***** Fault ID : 0x01 Soft ALM	*****/
#define	D_DU_FLTID_OS						0x0001	/**< OS要因 							*/
#define	D_DU_FLTID_EXC						0x0002	/**< マシンチェック例外 				*/
#define	D_DU_FLTID_DSI						0x0003	/**< DSI例外 							*/
#define	D_DU_FLTID_ISI						0x0004	/**< ISI例外 							*/
#define	D_DU_FLTID_ARAI						0x0005	/**< アライメント例外 					*/
#define	D_DU_FLTID_PRO						0x0006	/**< プログラム例外 					*/
#define	D_DU_FLTID_TLB						0x0007	/**< 命令TLBミス例外 					*/
#define	D_DU_FLTID_MIS						0x0008	/**< Data TLB Miss例外 					*/
#define	D_DU_FLTID_INS						0x0009	/**< Instruction TLB Miss例外 			*/
#define	D_DU_FLTID_FNC						0x000A	/**< 関数戻り値異常 					*/
#define	D_DU_FLTID_TSK						0x000B	/**< タスク起動異常 					*/
#define	D_DU_FLTID_BGET						0x000C	/**< バッファ獲得異常 					*/
#define	D_DU_FLTID_BFRE						0x000D	/**< バッファ解放異常 					*/
#define	D_DU_FLTID_TSTA						0x000E	/**< タイマ起動異常 					*/
#define	D_DU_FLTID_TEND						0x000F	/**< タイマ停止異常 					*/
#define	D_DU_FLTID_SGET						0x0010	/**< セマフォ獲得異常 					*/
#define	D_DU_FLTID_SFRE						0x0011	/**< セマフォ解放異常 					*/
#define	D_DU_FLTID_ANOT						0x0012	/**< その他の異常 						*/
#define	D_DU_FLTID_ARNG						0x0013	/**< 論理異常 							*/
#define	D_DU_FLTID_STNG						0x0014	/**< 状態異常 							*/
#define	D_DU_FLTID_TKST						0x0015	/**< タスク状態異常 					*/
#define	D_DU_FLTID_CSIF						0x0016	/**< カード状態異常 					*/
#define	D_DU_FLTID_TIF						0x0017	/**< タスク間メッセージ異常 			*/
#define	D_DU_FLTID_UBUF						0x0018	/**< ユーザ管理バッファ枯渇 			*/
#define	D_DU_FLTID_DNG						0x0019	/**< データ異常 						*/
#define	D_DU_FLTID_RNG						0x001A	/**< ルート異常 						*/
#define	D_DU_FLTID_FNG						0x001B	/**< 関数戻り値異常 					*/
#define	D_DU_FLTID_ANG						0x001C	/**< その他の異常 						*/
#define	D_DU_FLTID_CIF						0x001D	/**< カード間IF異常 					*/
#define	D_DU_FLTID_SUM						0x001E	/**< 受信データチェックサム異常 		*/
#define	D_DU_FLTID_ANNG						0x001F	/**< その他の異常 						*/
#define	D_DU_FLTID_FWNG						0x0020	/**< FLASH-ROM書込失敗 					*/
#define	D_DU_FLTID_EENG						0x0021	/**< EEPROMアクセス失敗 				*/
#define	D_DU_FLTID_BUSNG					0x0022	/**< シリアルバス制御失敗 				*/
#define	D_DU_FLTID_ETHNG					0x0023	/**< ethernet制御失敗 					*/
#define	D_DU_FLTID_ANNG2					0x0024	/**< その他の異常 						*/
#define	D_DU_FLTID_FPGACNF					0x0024	/**< FPGA CONFIG NG						*/
#define	D_DU_FLTID_DIA1						0x0025	/**< 一次診断NG1 						*/
#define	D_DU_FLTID_DIA2						0x0026	/**< 一次診断NG2 						*/
#define	D_DU_FLTID_DIA3						0x0027	/**< 一次診断NG3 						*/
#define	D_DU_FLTID_DIA4						0x0028	/**< 一次診断NG4 						*/
#define	D_DU_FLTID_DIA5						0x0029	/**< 一次診断NG5 						*/
#define	D_DU_FLTID_WDT						0x002A	/**< WDT T.O 							*/
#define	D_DU_FLTID_CSTP						0x002B	/**< Check STOP 						*/
#define	D_DU_FLTID_DUM1						0x002C	/**< 予備 								*/
#define	D_DU_FLTID_DUM2						0x002D	/**< 予備 								*/
#define	D_DU_FLTID_DUM3						0x002E	/**< 予備 								*/
#define	D_DU_FLTID_DUM4						0x002F	/**< 予備 								*/
#define	D_DU_FLTID_DUM5						0x0030	/**< 予備 								*/
#define D_DU_FLTID_REMOTEALM				0x00AF
#define D_DU_FLTID_01_MAX					0xFFF
#define D_DU_FLTID_01_NUM					1
#define D_DU_FLTID_01_SFT					0

/***** Alarm Code : 0x01 Soft ALM							*****/
/***** f_com_abortコール用、Faultコードとは異なるので注意！	*****/
#define	D_DU_ALMCD_OS					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_OS	)	/**< OS要因 							*/
#define	D_DU_ALMCD_EXC					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_EXC	)	/**< マシンチェック例外 				*/
#define	D_DU_ALMCD_DSI					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DSI	)	/**< DSI例外 							*/
#define	D_DU_ALMCD_ISI					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_ISI	)	/**< ISI例外 							*/
#define	D_DU_ALMCD_ARAI					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_ARAI	)	/**< アライメント例外 					*/
#define	D_DU_ALMCD_PRO					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_PRO	)	/**< プログラム例外 					*/
#define	D_DU_ALMCD_TLB					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_TLB	)	/**< 命令TLBミス例外 					*/
#define	D_DU_ALMCD_MIS					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_MIS	)	/**< Data TLB Miss例外 					*/
#define	D_DU_ALMCD_INS					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_INS	)	/**< Instruction TLB Miss例外 			*/
#define	D_DU_ALMCD_FNC					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_FNC	)	/**< 関数戻り値異常 					*/
#define	D_DU_ALMCD_TSK					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_TSK	)	/**< タスク起動異常 					*/
#define	D_DU_ALMCD_BGET					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_BGET	)	/**< バッファ獲得異常 					*/
#define	D_DU_ALMCD_BFRE					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_BFRE	)	/**< バッファ解放異常 					*/
#define	D_DU_ALMCD_TSTA					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_TSTA	)	/**< タイマ起動異常 					*/
#define	D_DU_ALMCD_TEND					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_TEND	)	/**< タイマ停止異常 					*/
#define	D_DU_ALMCD_SGET					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_SGET	)	/**< セマフォ獲得異常 					*/
#define	D_DU_ALMCD_SFRE					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_SFRE	)	/**< セマフォ解放異常 					*/
#define	D_DU_ALMCD_ANOT					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_ANOT	)	/**< その他の異常 						*/
#define	D_DU_ALMCD_ARNG					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_ARNG	)	/**< 論理異常 							*/
#define	D_DU_ALMCD_STNG					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_STNG	)	/**< 状態異常 							*/
#define	D_DU_ALMCD_TKST					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_TKST	)	/**< タスク状態異常 					*/
#define	D_DU_ALMCD_CSIF					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_CSIF	)	/**< カード状態異常 					*/
#define	D_DU_ALMCD_TIF					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_TIF	)	/**< タスク間メッセージ異常 			*/
#define	D_DU_ALMCD_UBUF					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_UBUF	)	/**< ユーザ管理バッファ枯渇 			*/
#define	D_DU_ALMCD_DNG					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DNG	)	/**< データ異常 						*/
#define	D_DU_ALMCD_RNG					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_RNG	)	/**< ルート異常 						*/
#define	D_DU_ALMCD_FNG					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_FNG	)	/**< 関数戻り値異常 					*/
#define	D_DU_ALMCD_ANG					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_ANG	)	/**< その他の異常 						*/
#define	D_DU_ALMCD_CIF					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_CIF	)	/**< カード間IF異常 					*/
#define	D_DU_ALMCD_SUM					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_SUM	)	/**< 受信データチェックサム異常 		*/
#define	D_DU_ALMCD_ANNG					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_ANNG	)	/**< その他の異常 						*/
#define	D_DU_ALMCD_FWNG					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_FWNG	)	/**< FLASH-ROM書込失敗 					*/
#define	D_DU_ALMCD_EENG					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_EENG	)	/**< EEPROMアクセス失敗 				*/
#define	D_DU_ALMCD_BUSNG				(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_BUSNG	)	/**< シリアルバス制御失敗 				*/
#define	D_DU_ALMCD_ETHNG				(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_ETHNG	)	/**< ethernet制御失敗 					*/
#define	D_DU_ALMCD_ANNG2				(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_ANNG2	)	/**< その他の異常 						*/
#define	D_DU_ALMCD_FPGACNF				(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_FPGACNF)	/**< FPGA CONFIG NG						*/
#define	D_DU_ALMCD_DIA1					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DIA1	)	/**< 一次診断NG1 						*/
#define	D_DU_ALMCD_DIA2					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DIA2	)	/**< 一次診断NG2 						*/
#define	D_DU_ALMCD_DIA3					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DIA3	)	/**< 一次診断NG3 						*/
#define	D_DU_ALMCD_DIA4					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DIA4	)	/**< 一次診断NG4 						*/
#define	D_DU_ALMCD_DIA5					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DIA5	)	/**< 一次診断NG5 						*/
#define	D_DU_ALMCD_WDT					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_WDT	)	/**< WDT T.O 							*/
#define	D_DU_ALMCD_CSTP					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_CSTP	)	/**< Check STOP 						*/
#define	D_DU_ALMCD_DUM1					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DUM1	)	/**< 予備 								*/
#define	D_DU_ALMCD_DUM2					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DUM2	)	/**< 予備 								*/
#define	D_DU_ALMCD_DUM3					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DUM3	)	/**< 予備 								*/
#define	D_DU_ALMCD_DUM4					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DUM4	)	/**< 予備 								*/
#define	D_DU_ALMCD_DUM5					(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_DUM5	)	/**< 予備 								*/
#define D_DU_ALMCD_REMOTEALM			(D_RRH_SIGUSR_OPT_RSET_SWALM_NTC | D_DU_FLTID_REMOTEALM)/**< remoto ALM							*/

/***** Fault ID : 0x02 Initialization failure	*****/
#define D_DU_FLTID_DIAG_NG_MEM					0x0001	/**> Work memory self test failed	*/
#define D_DU_FLTID_DIAG_NG_PL					0x0002	/**> PL FPGA self test failed		*/
#define D_DU_FLTID_CONF_NG_TRX1					0x0010	/**> TRX-FPGA#1 ConfigNG			*/
#define D_DU_FLTID_PCIE_EST_NG1_TRX1			0x0020	/**> PL-TRX#1 PCIe establish NG1	*/
#define D_DU_FLTID_PCIE_EST_NG2_TRX1			0x0021	/**> PL-TRX#1 PCIe establish NG2	*/
#define D_DU_FLTID_PCIE_EST_NG3_TRX1			0x0022	/**> PL-TRX#1 PCIe establish NG3	*/
#define D_DU_FLTID_PCIE_EST_NG4_TRX1			0x0023	/**> PL-TRX#1 PCIe establish NG4	*/
#define D_DU_FLTID_PCIE_EST_NG5_TRX1			0x0024	/**> PL-TRX#1 PCIe establish NG5	*/
#define D_DU_FLTID_PCIE_EST_NG6_TRX1			0x0025	/**> PL-TRX#1 PCIe establish NG6	*/
#define D_DU_FLTID_DIAG_NG_TRX1					0x0030	/**> TRX#1 FPGA self test failed	*/
#define D_DU_FLTID_CNT_INIT_NG_SRAM1			0x0032	/**> SRAM#1 CNT INIT NG				*/
#define D_DU_FLTID_DIAG_NG_SRAM1				0x0031	/**> SRAM#1 CNT self test failed	*/
#define D_DU_FLTID_TRX1_PL_SVIF_NG				0x0036	/**> PL_TRX#1 SVIF NG				*/
#define D_DU_FLTID_PL_TRX1_SVIF_NG				0x0038	/**> TRX#1_PL SVIF NG				*/
#define D_DU_FLTID_DEVKIND_ABNORMAL				0x003a	/**> 装置種別異常					*/
#define D_DU_FLTID_LLB_SYS_PLL_NG				0x003b	/**> LLB SYS PLL NG					*/
#define D_DU_FLTID_TRX1_SYS_PLL_NG				0x003c	/**> TRX#1 SYS PLL NG				*/
#define D_DU_FLTID_LLB_IC_PLL_NG				0x003d	/**> LLB IC PLL NG					*/
#define D_DU_FLTID_TRX1_IC_PLL_NG				0x003e	/**> TRX#1 IC PLL NG				*/
#define D_DU_FLTID_IC_EST_NG01					0x0050	/**> IC establish NG1				*/
#define D_DU_FLTID_IC_EST_NG02					0x0051	/**> IC establish NG2				*/
#define D_DU_FLTID_IC_EST_NG03					0x0052	/**> IC establish NG3				*/
#define D_DU_FLTID_IC_EST_NG04					0x0053	/**> IC establish NG4				*/
#define D_DU_FLTID_IC_EST_NG05					0x0054	/**> IC establish NG5				*/
#define D_DU_FLTID_IC_EST_NG06					0x0055	/**> IC establish NG6				*/
#define D_DU_FLTID_IC_EST_NG07					0x0056	/**> IC establish NG7				*/
#define D_DU_FLTID_IC_EST_NG08					0x0057	/**> IC establish NG8				*/
#define D_DU_FLTID_IC_EST_NG09					0x0058	/**> IC establish NG9				*/
#define D_DU_FLTID_IC_EST_NG10					0x0059	/**> IC establish NG10				*/
#define D_DU_FLTID_RF_UINIT_UNMOUNT				0x090	/**> RF UNIT Unmount				*//*	5G-商用DU	*/
#define D_DU_FLTID_RF_UNIT_SN_MIS				0x091	/**> RF UNIT SN mismatch			*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_NG01_TRX1			0x0100	/**> RFIC INIT NG1-1				*/
#define D_DU_FLTID_RFIC_INIT_NG02_TRX1			0x0101	/**> RFIC INIT NG2-1				*/
#define D_DU_FLTID_RFIC_INIT_NG03_TRX1			0x0102	/**> RFIC INIT NG3-1				*/
#define D_DU_FLTID_RFIC_INIT_NG04_RFIC1			0x0103	/**> RFIC INIT NG4-1				*/
#define D_DU_FLTID_RFIC_INIT_NG04_RFIC2			0x0104	/**> RFIC INIT NG4-2				*/
#define D_DU_FLTID_RFIC_INIT_NG05_RFIC1			0x0105	/**> RFIC INIT NG5-1				*/
#define D_DU_FLTID_RFIC_INIT_NG05_RFIC2			0x0106	/**> RFIC INIT NG5-2				*/
#define D_DU_FLTID_RFIC_INIT_NG06_RFIC1			0x0107	/**> RFIC INIT NG6-1				*/
#define D_DU_FLTID_RFIC_INIT_NG06_RFIC2			0x0108	/**> RFIC INIT NG6-2				*/
#define D_DU_FLTID_RFIC_INIT_NG07_RFIC1			0x0109	/**> RFIC INIT NG7-1				*/
#define D_DU_FLTID_RFIC_INIT_NG07_RFIC2			0x010a	/**> RFIC INIT NG7-2				*/
#define D_DU_FLTID_RFIC_INIT_NG08_RFIC1			0x010b	/**> RFIC INIT NG8-1				*/
#define D_DU_FLTID_RFIC_INIT_NG08_RFIC2			0x010c	/**> RFIC INIT NG8-2				*/
#define D_DU_FLTID_RFIC_INIT_NG09_RFIC1			0x010d	/**> RFIC INIT NG9-1				*/
#define D_DU_FLTID_RFIC_INIT_NG09_RFIC2			0x010e	/**> RFIC INIT NG9-2				*/
#define D_DU_FLTID_RFIC_INIT_NG10_RFIC1			0x010f	/**> RFIC INIT NG10-1				*/
#define D_DU_FLTID_RFIC_INIT_NG10_RFIC2			0x0110	/**> RFIC INIT NG10-2				*/
#define D_DU_FLTID_RFIC_INIT_NG11_RFIC1			0x0111	/**> RFIC INIT NG11-1				*/
#define D_DU_FLTID_RFIC_INIT_NG11_RFIC2			0x0112	/**> RFIC INIT NG11-2				*/
#define D_DU_FLTID_RFIC_INIT_NG12_RFIC1			0x0113	/**> RFIC INIT NG12-1				*/
#define D_DU_FLTID_RFIC_INIT_NG12_RFIC2			0x0114	/**> RFIC INIT NG12-2				*/
#define D_DU_FLTID_RFIC_INIT_NG13_RFIC1			0x115	/**> RFIC INIT NG13-1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_NG13_RFIC2			0x116	/**> RFIC INIT NG13-2				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_NG13_TRX1			D_DU_FLTID_RFIC_INIT_NG13_RFIC1	/**> RFIC INIT NG13-1					*//*	5G-評価DU	*/
#define D_DU_FLTID_RFIC_INIT_NG14_RFIC			0x117	/**> RFIC INIT NG14-1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_NG14_TRX1			D_DU_FLTID_RFIC_INIT_NG14_RFIC	/**> RFIC INIT NG14-1					*//*	5G-評価DU	*/
#define D_DU_FLTID_RFIC_INIT_NG15_RFIC			0x118	/**> RFIC INIT NG15-1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_NG15_RFIC1			D_DU_FLTID_RFIC_INIT_NG15_RFIC	/**> RFIC INIT NG15-1					*//*	5G-評価DU	*/
#define D_DU_FLTID_RFIC_INIT_NG16_RFIC1			0x119	/**> RFIC INIT NG16-1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_NG16_TRX1			D_DU_FLTID_RFIC_INIT_NG16_RFIC1	/**> RFIC INIT NG16-1					*//*	5G-評価DU	*/
#define D_DU_FLTID_RFIC_INIT_NG17_TRX1			0x011a	/**> RFIC INIT NG17-1				*/
#define D_DU_FLTID_RFIC_INIT_NG18_TRX1			0x011b	/**> RFIC INIT NG18-1				*/
#define D_DU_FLTID_RFIC_INIT_NG19_TRX1			0x11c	/**> RFIC INIT NG19					*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_NG20_TRX1			0x11d	/**> RFIC INIT NG20					*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_NG21_TRX1			0x11e	/**> RFIC INIT NG21					*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG1_MMW		0x100	/**> TX_DAC INIT NG1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG2_MMW		0x101	/**> TX_DAC INIT NG2				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG3_MMW		0x102	/**> TX_DAC INIT NG3				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG4_MMW		0x103	/**> TX_DAC INIT NG4				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG5_MMW		0x104	/**> TX_DAC INIT NG5				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG6_MMW		0x105	/**> TX_DAC INIT NG6				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG7_1_MMW	0x106	/**> TX_DAC INIT NG7-1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG7_2_MMW	0x107	/**> TX_DAC INIT NG7-2				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG7_3_MMW	0x108	/**> TX_DAC INIT NG7-3				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG8_MMW		0x109	/**> TX_DAC INIT NG8				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG09_1_MMW	0x10a	/**> RFIC INIT NG9-1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG09_2_MMW	0x10b	/**> RFIC INIT NG9-2				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG09_3_MMW	0x10c	/**> RFIC INIT NG9-3				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG09_4_MMW	0x10d	/**> RFIC INIT NG9-4				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG10_1_MMW	0x10e	/**> RFIC INIT NG10-1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG10_2_MMW	0x10f	/**> RFIC INIT NG10-2				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG10_3_MMW	0x110	/**> RFIC INIT NG10-3				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_NG10_4_MMW	0x111	/**> RFIC INIT NG10-4				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_TX_DAC_RSTSNG_MMW	0x112	/**> TX_DAC_Read Status NG			*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_RX_ADC_NG1_MMW		0x180	/**> RX_ADC INIT NG1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_RX_ADC_NG2_MMW		0x181	/**> RX_ADC INIT NG2				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_RX_ADC_NG3_MMW		0x182	/**> RX_ADC INIT NG3				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_RX_ADC_NG4_1_MMW	0x183	/**> RX_ADC INIT NG4-1				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_RX_ADC_NG4_2_MMW	0x184	/**> RX_ADC INIT NG4-2				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_RX_ADC_NG4_3_MMW	0x185	/**> RX_ADC INIT NG4-3				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_RX_ADC_NG5_MMW		0x186	/**> RX_ADC INIT NG5				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_RX_ADC_NG6_MMW		0x187	/**> RX_ADC INIT NG6				*//*	5G-商用DU	*/
#define D_DU_FLTID_RFIC_INIT_RX_ADC_RSTSNG_MMW	0x188	/**> RX_ADC_Read Status NG			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_TRA_PRM_NG1				0x0200	/**> Tx Trainig Parameter NG		*/
#define D_DU_FLTID_RX_TRA_PRM_NG1				0x0201	/**> Rx Trainig Parameter NG		*/
#define D_DU_FLTID_PGC_TRA_PRM_NG_MMW			0x202	/**> PGC Trainig Parameter NG		*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_CAL_FAIL_INIT_A			0x220	/**> Tx Cal failed#AntA				*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_CAL_FAIL_INIT_B			0x221	/**> Tx Cal failed#AntB				*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_CAL_FAIL_INIT_C			0x222	/**> Tx Cal failed#AntC				*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_CAL_FAIL_INIT_D			0x223	/**> Tx Cal failed#AntD				*//*	5G-商用DU	*/
#define D_DU_FLTID_RX_CAL_FAIL_INIT_A			0x230	/**> Rx Cal failed#AntA				*//*	5G-商用DU	*/
#define D_DU_FLTID_RX_CAL_FAIL_INIT_B			0x231	/**> Rx Cal failed#AntB				*//*	5G-商用DU	*/
#define D_DU_FLTID_RX_CAL_FAIL_INIT_C			0x232	/**> Rx Cal failed#AntC				*//*	5G-商用DU	*/
#define D_DU_FLTID_RX_CAL_FAIL_INIT_D			0x233	/**> Rx Cal failed#AntD				*//*	5G-商用DU	*/
#define D_DU_FLTID_LNA_ABNORMAL_INIT_A			0x240	/**> LNA Gain abnormal ANT#A		*//*	5G-商用DU	*/
#define D_DU_FLTID_LNA_ABNORMAL_INIT_B			0x241	/**> LNA Gain abnormal ANT#B		*//*	5G-商用DU	*/
#define D_DU_FLTID_LNA_ABNORMAL_INIT_C			0x242	/**> LNA Gain abnormal ANT#C		*//*	5G-商用DU	*/
#define D_DU_FLTID_LNA_ABNORMAL_INIT_D			0x243	/**> LNA Gain abnormal ANT#D		*//*	5G-商用DU	*/
#define D_DU_FLTID_ETH_PLL_NG_INIT				0x300	/**> ETH PLL NG						*//*	5G-商用DU	*/
#define D_DU_FLTID_FH1_TX_NOT_DONE				0x301	/**> FH1 Tx Not Done				*//*	5G-商用DU	*/
#define D_DU_FLTID_FH1_RX_NOT_DONE				0x302	/**> FH1 Rx Not Done				*//*	5G-商用DU	*/
#define D_DU_FLTID_02_MAX					0xFFF
#define D_DU_FLTID_02_NUM					1
#define D_DU_FLTID_02_SFT					16


/***** Alarm Code : 0x02 Initialization failure				*****/
/***** f_com_abortコール用、Faultコードとは異なるので注意！	*****/
#define D_DU_ALMCD_DIAG_NG_MEM				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_DIAG_NG_MEM				)	/**> Work memory self test failed		*/
#define D_DU_ALMCD_DIAG_NG_PL				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_DIAG_NG_PL				)	/**> PL FPGA self test failed			*/
#define D_DU_ALMCD_CONF_NG_TRX1				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_CONF_NG_TRX1				)	/**> TRX-FPGA#1 ConfigNG				*/
#define D_DU_ALMCD_PCIE_EST_NG1_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_PCIE_EST_NG1_TRX1		)	/**> PL-TRX#1 PCIe establish NG1		*/
#define D_DU_ALMCD_PCIE_EST_NG2_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_PCIE_EST_NG2_TRX1		)	/**> PL-TRX#1 PCIe establish NG2		*/
#define D_DU_ALMCD_PCIE_EST_NG3_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_PCIE_EST_NG3_TRX1		)	/**> PL-TRX#1 PCIe establish NG3		*/
#define D_DU_ALMCD_PCIE_EST_NG4_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_PCIE_EST_NG4_TRX1		)	/**> PL-TRX#1 PCIe establish NG4		*/
#define D_DU_ALMCD_PCIE_EST_NG5_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_PCIE_EST_NG5_TRX1		)	/**> PL-TRX#1 PCIe establish NG5		*/
#define D_DU_ALMCD_PCIE_EST_NG6_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_PCIE_EST_NG6_TRX1		)	/**> PL-TRX#1 PCIe establish NG6		*/
#define D_DU_ALMCD_DIAG_NG_TRX1				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_DIAG_NG_TRX1				)	/**> TRX#1 FPGA self test failed		*/
#define D_DU_ALMCD_CNT_INIT_NG_SRAM1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_CNT_INIT_NG_SRAM1		)	/**> SRAM#1 CNT INIT NG					*/
#define D_DU_ALMCD_DIAG_NG_SRAM1			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_DIAG_NG_SRAM1			)	/**> SRAM#1 CNT self test failed		*/
#define D_DU_ALMCD_TRX1_PL_SVIF_NG			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_TRX1_PL_SVIF_NG			)	/**> PL_TRX#1 SVIF NG					*/
#define D_DU_ALMCD_PL_TRX1_SVIF_NG			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_PL_TRX1_SVIF_NG			)	/**> TRX#1_PL SVIF NG					*/
#define D_DU_ALMCD_DEVKIND_ABNORMAL			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_DEVKIND_ABNORMAL			)	/**> 装置種別異常						*/
#define D_DU_ALMCD_LLB_SYS_PLL_NG			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_LLB_SYS_PLL_NG			)	/**> LLB SYS PLL NG						*/
#define D_DU_ALMCD_TRX1_SYS_PLL_NG			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_TRX1_SYS_PLL_NG			)	/**> TRX#1 SYS PLL NG					*/
#define D_DU_ALMCD_LLB_IC_PLL_NG			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_LLB_IC_PLL_NG			)	/**> LLB IC PLL NG						*/
#define D_DU_ALMCD_TRX1_IC_PLL_NG			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_TRX1_IC_PLL_NG			)	/**> TRX#1 IC PLL NG					*/
#define D_DU_ALMCD_IC_EST_NG01				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG01				)	/**> IC establish NG1					*/
#define D_DU_ALMCD_IC_EST_NG02				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG02				)	/**> IC establish NG2					*/
#define D_DU_ALMCD_IC_EST_NG03				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG03				)	/**> IC establish NG3					*/
#define D_DU_ALMCD_IC_EST_NG04				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG04				)	/**> IC establish NG4					*/
#define D_DU_ALMCD_IC_EST_NG05				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG05				)	/**> IC establish NG5					*/
#define D_DU_ALMCD_IC_EST_NG06				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG06				)	/**> IC establish NG6					*/
#define D_DU_ALMCD_IC_EST_NG07				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG07				)	/**> IC establish NG7					*/
#define D_DU_ALMCD_IC_EST_NG08				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG08				)	/**> IC establish NG8					*/
#define D_DU_ALMCD_IC_EST_NG09				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG09				)	/**> IC establish NG9					*/
#define D_DU_ALMCD_IC_EST_NG10				(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_IC_EST_NG10				)	/**> IC establish NG10					*/
#define D_DU_ALMCD_RF_UINIT_UNMOUNT			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC |D_DU_FLTID_RF_UINIT_UNMOUNT			)	/**> RF UNIT Unmount					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RF_UNIT_SN_MIS			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC |D_DU_FLTID_RF_UNIT_SN_MIS			)	/**> RF UNIT SN mismatch				*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG01_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG01_TRX1		)	/**> RFIC INIT NG1-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG02_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG02_TRX1		)	/**> RFIC INIT NG2-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG03_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG03_TRX1		)	/**> RFIC INIT NG3-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG04_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG04_RFIC1		)	/**> RFIC INIT NG4-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG04_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG04_RFIC2		)	/**> RFIC INIT NG4-2					*/
#define D_DU_ALMCD_RFIC_INIT_NG05_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG05_RFIC1		)	/**> RFIC INIT NG5-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG05_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG05_RFIC2		)	/**> RFIC INIT NG5-2					*/
#define D_DU_ALMCD_RFIC_INIT_NG06_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG06_RFIC1		)	/**> RFIC INIT NG6-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG06_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG06_RFIC2		)	/**> RFIC INIT NG6-2					*/
#define D_DU_ALMCD_RFIC_INIT_NG07_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG07_RFIC1		)	/**> RFIC INIT NG7-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG07_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG07_RFIC2		)	/**> RFIC INIT NG7-2					*/
#define D_DU_ALMCD_RFIC_INIT_NG08_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG08_RFIC1		)	/**> RFIC INIT NG8-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG08_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG08_RFIC2		)	/**> RFIC INIT NG8-2					*/
#define D_DU_ALMCD_RFIC_INIT_NG09_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG09_RFIC1		)	/**> RFIC INIT NG9-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG09_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG09_RFIC2		)	/**> RFIC INIT NG9-2					*/
#define D_DU_ALMCD_RFIC_INIT_NG10_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG10_RFIC1		)	/**> RFIC INIT NG10-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG10_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG10_RFIC2		)	/**> RFIC INIT NG10-2					*/
#define D_DU_ALMCD_RFIC_INIT_NG11_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG11_RFIC1		)	/**> RFIC INIT NG11-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG11_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG11_RFIC2		)	/**> RFIC INIT NG11-2					*/
#define D_DU_ALMCD_RFIC_INIT_NG12_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG12_RFIC1		)	/**> RFIC INIT NG12-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG12_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG12_RFIC2		)	/**> RFIC INIT NG12-2					*/
#define D_DU_ALMCD_RFIC_INIT_NG13_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG13_RFIC1		)	/**> RFIC INIT NG13-1					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG13_RFIC2		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG13_RFIC2		)	/**> RFIC INIT NG13-2					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG13_TRX1		D_DU_ALMCD_RFIC_INIT_NG13_RFIC1												/**> RFIC INIT NG13-1					*//*	5G-評価DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG14_RFIC		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG14_RFIC		)	/**> RFIC INIT NG14-1					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG14_TRX1		D_DU_ALMCD_RFIC_INIT_NG14_RFIC												/**> RFIC INIT NG14-1					*//*	5G-評価DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG15_RFIC		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG15_RFIC		)	/**> RFIC INIT NG15-1					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG15_RFIC1		D_DU_ALMCD_RFIC_INIT_NG15_RFIC												/**> RFIC INIT NG15-1					*//*	5G-評価DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG16_RFIC1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG16_RFIC1		)	/**> RFIC INIT NG16-1					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG16_TRX1		D_DU_ALMCD_RFIC_INIT_NG16_RFIC1												/**> RFIC INIT NG16-1					*//*	5G-評価DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG17_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG17_TRX1		)	/**> RFIC INIT NG17-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG18_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG18_TRX1		)	/**> RFIC INIT NG18-1					*/
#define D_DU_ALMCD_RFIC_INIT_NG19_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG19_TRX1		)	/**> RFIC INIT NG19						*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG20_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG20_TRX1		)	/**> RFIC INIT NG20						*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_NG21_TRX1		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_NG21_TRX1		)	/**> RFIC INIT NG21						*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG1_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG1_MMW		)	/**> TX_DAC INIT NG1			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG2_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG2_MMW		)	/**> TX_DAC INIT NG2			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG3_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG3_MMW		)	/**> TX_DAC INIT NG3			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG4_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG4_MMW		)	/**> TX_DAC INIT NG4			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG5_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG5_MMW		)	/**> TX_DAC INIT NG5			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG6_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG6_MMW		)	/**> TX_DAC INIT NG6			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG7_1_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG7_1_MMW	)	/**> TX_DAC INIT NG7-1			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG7_2_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG7_2_MMW	)	/**> TX_DAC INIT NG7-2			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG7_3_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG7_3_MMW	)	/**> TX_DAC INIT NG7-3			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG8_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG8_MMW		)	/**> TX_DAC INIT NG8			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG09_1_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG09_1_MMW	)	/**> RFIC INIT NG9-1			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG09_2_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG09_2_MMW	)	/**> RFIC INIT NG9-2			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG09_3_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG09_3_MMW	)	/**> RFIC INIT NG9-3			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG09_4_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG09_4_MMW	)	/**> RFIC INIT NG9-4			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG10_1_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG10_1_MMW	)	/**> RFIC INIT NG10-1			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG10_2_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG10_2_MMW	)	/**> RFIC INIT NG10-2			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG10_3_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG10_3_MMW	)	/**> RFIC INIT NG10-3			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_NG10_4_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_NG10_4_MMW	)	/**> RFIC INIT NG10-4			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_TX_DAC_RSTSNG_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_TX_DAC_RSTSNG_MMW	)	/**> TX_DAC_Read Status NG		*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_RX_ADC_NG1_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_RX_ADC_NG1_MMW		)	/**> RX_ADC INIT NG1			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_RX_ADC_NG2_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_RX_ADC_NG2_MMW		)	/**> RX_ADC INIT NG2			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_RX_ADC_NG3_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_RX_ADC_NG3_MMW		)	/**> RX_ADC INIT NG3			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_RX_ADC_NG4_1_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_RX_ADC_NG4_1_MMW	)	/**> RX_ADC INIT NG4-1			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_RX_ADC_NG4_2_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_RX_ADC_NG4_2_MMW	)	/**> RX_ADC INIT NG4-2			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_RX_ADC_NG4_3_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_RX_ADC_NG4_3_MMW	)	/**> RX_ADC INIT NG4-3			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_RX_ADC_NG5_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_RX_ADC_NG5_MMW		)	/**> RX_ADC INIT NG5			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_RX_ADC_NG6_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_RX_ADC_NG6_MMW		)	/**> RX_ADC INIT NG6			*//*	5G-商用DU	*/
#define D_DU_ALMCD_RFIC_INIT_RX_ADC_RSTSNG_MMW	(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RFIC_INIT_RX_ADC_RSTSNG_MMW	)	/**> RX_ADC_Read Status NG		*//*	5G-商用DU	*/
#define D_DU_ALMCD_TX_TRA_PRM_NG1			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_TX_TRA_PRM_NG1			)	/**> Tx Trainig Parameter NG			*/
#define D_DU_ALMCD_RX_TRA_PRM_NG1			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RX_TRA_PRM_NG1			)	/**> Rx Trainig Parameter NG			*/
#define D_DU_ALMCD_PGC_TRA_PRM_NG_MMW		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_PGC_TRA_PRM_NG_MMW		)	/**> PGC Trainig Parameter NG			*//*	5G-商用DU	*/
#define D_DU_ALMCD_TX_CAL_FAIL_INIT_A		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_TX_CAL_FAIL_INIT_A		)	/**> Tx Cal failed#AntA					*//*	5G-商用DU	*/
#define D_DU_ALMCD_TX_CAL_FAIL_INIT_B		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_TX_CAL_FAIL_INIT_B		)	/**> Tx Cal failed#AntB					*//*	5G-商用DU	*/
#define D_DU_ALMCD_TX_CAL_FAIL_INIT_C		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_TX_CAL_FAIL_INIT_C		)	/**> Tx Cal failed#AntC					*//*	5G-商用DU	*/
#define D_DU_ALMCD_TX_CAL_FAIL_INIT_D		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_TX_CAL_FAIL_INIT_D		)	/**> Tx Cal failed#AntD					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RX_CAL_FAIL_INIT_A		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RX_CAL_FAIL_INIT_A		)	/**> Rx Cal failed#AntA					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RX_CAL_FAIL_INIT_B		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RX_CAL_FAIL_INIT_B		)	/**> Rx Cal failed#AntB					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RX_CAL_FAIL_INIT_C		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RX_CAL_FAIL_INIT_C		)	/**> Rx Cal failed#AntC					*//*	5G-商用DU	*/
#define D_DU_ALMCD_RX_CAL_FAIL_INIT_D		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_RX_CAL_FAIL_INIT_D		)	/**> Rx Cal failed#AntD					*//*	5G-商用DU	*/
#define D_DU_ALMCD_LNA_ABNORMAL_INIT_A		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_LNA_ABNORMAL_INIT_A		)	/**> LNA Gain abnormal ANT#A			*//*	5G-商用DU	*/
#define D_DU_ALMCD_LNA_ABNORMAL_INIT_B		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_LNA_ABNORMAL_INIT_B		)	/**> LNA Gain abnormal ANT#B			*//*	5G-商用DU	*/
#define D_DU_ALMCD_LNA_ABNORMAL_INIT_C		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_LNA_ABNORMAL_INIT_C		)	/**> LNA Gain abnormal ANT#C			*//*	5G-商用DU	*/
#define D_DU_ALMCD_LNA_ABNORMAL_INIT_D		(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_LNA_ABNORMAL_INIT_D		)	/**> LNA Gain abnormal ANT#D			*//*	5G-商用DU	*/
#define D_DU_ALMCD_ETH_PLL_NG_INIT			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_ETH_PLL_NG_INIT			)	/**> ETH PLL NG							*//*	5G-商用DU	*/
#define D_DU_ALMCD_FH1_TX_NOT_DONE			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_FH1_TX_NOT_DONE			)	/**> FH1 Tx Not Done					*//*	5G-商用DU	*/
#define D_DU_ALMCD_FH1_RX_NOT_DONE			(D_RRH_SIGUSR_OPT_RSET_INIALM_NTC | D_DU_FLTID_FH1_RX_NOT_DONE			)	/**> FH1 Rx Not Done					*//*	5G-商用DU	*/

/***** Fault ID : 0x03 Fronthaul Synchronization Error	*****/
#define D_DU_FLTID_PTP_UNLOCK				0x0001	/**> PTP UnLock(HoldOver)				*/
#define D_DU_FLTID_03_NUM					1
#define D_DU_FLTID_03_SFT					0

/***** Fault ID : 0x04 Module Clock Faulty	*****/
#define D_DU_FLTID_JC_PLL_UNLOCK			0x0001	/**> JC-PLL Unlock						*/
#define D_DU_FLTID_RF_PLL_UNLOCK			0x0002	/**> RF-PLL Unlock						*/
#define D_DU_FLTID_04_NUM_SB6				1
#define D_DU_FLTID_04_NUM_MMW				2
#define D_DU_FLTID_04_SFT					4

/***** Fault ID : 0x05 Fronthaul faulty	*****/
#define D_DU_FLTID_L1_LDOWN_FH1				0x0001	/**> L1断								*/
#define D_DU_FLTID_BER_DET_FH1				0x0002	/**> Critical BER detected				*/
#define D_DU_FLTID_SFP_UNMOUNT_FH1			0x0010	/**> SFP unmount						*/
#define D_DU_FLTID_SPLANE_ABNL_FH1			0x0020	/**> S-plane abnormal					*/
#define D_DU_FLTID_UPLANE_ABNL_FH1			0x0030	/**> U-plane abnormal					*/
#define D_DU_FLTID_CPLANE_ABNL_FH1			0x0040	/**> FC-Plane abnormal					*/
#define D_DU_FLTID_05_NUM					6
#define D_DU_FLTID_05_SFT					8

/***** Fault ID : 0x06 PS ALM	*****/
#define D_DU_FLTID_PS_ALM					0x0001	/**> RF Module PS ALM					*/
#define D_DU_FLTID_POW_ERR1					0x0002	/**> Power (Sub6:GateBias/mmW:RXADC)	*/
#define D_DU_FLTID_POW_ERR2					0x0003	/**> Power (Sub6:RFIC/mmW:TXDAC)		*/
#define D_DU_FLTID_SFP_PDOWN				0x010	/**> SFP Power Down						*//*	5G-商用DU	*/
#define D_DU_FLTID_SFP_PDOWN_FH1			D_DU_FLTID_SFP_PDOWN	/**> FLT IF(0x05):SFP Power Down			*//*	5G-評価DU	*/
#define D_DU_FLTID_06_NUM					4
#define D_DU_FLTID_06_SFT					24

/***** Fault ID : 0x07 common hardware (module) fault	*****/
#define D_DU_FLTID_SYS_PLL_UNLOCK_LLB		0x0001	/**> LLB SYS PLL Unlock					*/
#define D_DU_FLTID_IC_PLL_UNLOCK_LLB		0x0002	/**> LLB ICPLL Unlock or LLB IC DIV CHK NG	*/
#define D_DU_FLTID_ETH_PLL_UNLOCK_LLB		0x0003	/**> LLB ETHPLL Unlock					*/
#define D_DU_FLTID_TRX1_PCIE_ERR1_LLB		0x0004	/**> LLB_TRX#1 PCIE Root Error			*/
#define D_DU_FLTID_TRX1_PCIE_ERR2_LLB		0x0005	/**> LLB_TRX#1 PCIE EndPoint Error		*/
#define D_DU_FLTID_TRX1_SV_FAIL_LLB			0x0006	/**> LLB_TRX#1 SV bus failure			*/
#define D_DU_FLTID_ETH_ALM					0x007	/**> Ether Alm							*//*	5G-商用DU	*/
#define D_DU_FLTID_SRAM_CORRECT_ERR			0x008	/**> SRAM Correct Error					*//*	5G-商用DU	*/
#define D_DU_FLTID_SRAM_UNCORRECT_ERR		0x009	/**> SRAM UnCorrect Error				*//*	5G-商用DU	*/
#define D_DU_FLTID_TRX1_IC_TX_ERR_LLB		0x0010	/**> LLB_TRX#1 IC TX ERR				*/
#define D_DU_FLTID_TRX1_IC_RX_ERR_LLB		0x0011	/**> LLB_TRX#1 IC RX ERR				*/
#define D_DU_FLTID_LLB_IC_TX_ERR_TRX1		0x012	/**> TRX_LLB IC TX ERR					*//*	5G-商用DU	*/
#define D_DU_FLTID_LLB_IC_RX_ERR_TRX1		0x013	/**> TRX_LLB IC RX ERR					*//*	5G-商用DU	*/
#define D_DU_FLTID_AUTO_CRUISE_HC_ERR		0x020	/**> Auto Cruise HC ERR					*//*	5G-商用DU	*/
#define D_DU_FLTID_SYS_PLL_UNLOCK_TRX1		0x0101	/**> TRX#1 SYS PLL Unlock				*/
#define D_DU_FLTID_IC_PLL_UNLOCK_TRX1		0x0102	/**> TRX#1 ICPLL Unlock					*/
#define D_DU_FLTID_PHYICPLLULCK1_TRX1		0x0103	/**> TRX#1 PHYICPLLULCK1				*/
#define D_DU_FLTID_PHYICPLLULCK2_TRX1		0x0104	/**> TRX#1 PHYICPLLULCK2				*/
#define D_DU_FLTID_PHY_POW_ALM_TRX1			0x105	/**> TRX PHYPOWALM						*//*	5G-商用DU	*/
#define D_DU_FLTID_PHYJS1PLLULCK_TRX1		0x0106	/**> TRX#1 PHYJS1PLLULCK				*/
#define D_DU_FLTID_PHYJS2PLLULCK_TRX1		0x0107	/**> TRX#1 PHYJS2PLLULCK				*/
#define D_DU_FLTID_TRX1_SV_FAIL_TRX1		0x0108	/**> TRX#1 SV bus failure				*/
#define D_DU_FLTID_R1_TX_JESD_ERR_TRX1		0x0109	/**> TRX#1/RFIC#1 TX_JESD IF ERR		*/
#define D_DU_FLTID_R1_FB_JESD_ERR_TRX1		0x010a	/**> TRX#1/RFIC#1 FB_JESD IF ERR		*/
#define D_DU_FLTID_R1_RX_JESD_ERR_TRX1		0x010b	/**> TRX#1/RFIC#1 RX_JESD IF ERR		*/
#define D_DU_FLTID_R2_TX_JESD_ERR_TRX1		0x010c	/**> TRX#1/RFIC#2 TX_JESD IF ERR		*/
#define D_DU_FLTID_R2_FB_JESD_ERR_TRX1		0x010d	/**> TRX#1/RFIC#2 FB_JESD IF ERR		*/
#define D_DU_FLTID_R2_RX_JESD_ERR_TRX1		0x010e	/**> TRX#1/RFIC#2 RX_JESD IF ERR		*/
#define D_DU_FLTID_LMB_CORRECT_ERR			0x10f	/**> LMB Correct Error					*//*	5G-商用DU	*/
#define D_DU_FLTID_LMB_UNCORRECT_ERR		0x110	/**> LMB UnCorrect Error				*//*	5G-商用DU	*/
#define D_DU_FLTID_07_NUM_SB6				30
#define D_DU_FLTID_07_NUM_MMW				24
#define D_DU_FLTID_07_SFT					0

/***** Fault ID : 0x08 RF module fault	*****/
#define D_DU_FLTID_RFIC1_ALM				0x0001	/**> RFIC#1 ALM							*/
#define D_DU_FLTID_RFIC2_ALM				0x0002	/**> RFIC#2 ALM							*/
#define D_DU_FLTID_TXDAC_ALM				D_DU_FLTID_RFIC1_ALM	/**> TXDAC ALM			*//*	5G-商用DU	*/
#define D_DU_FLTID_RXDAC_ALM				D_DU_FLTID_RFIC2_ALM	/**> RXDAC ALM			*//*	5G-商用DU	*/
#define D_DU_FLTID_08_NUM_SB6				2
#define D_DU_FLTID_08_NUM_MMW				0
#define D_DU_FLTID_08_SFT					16


/***** Fault ID : 0x10 TX out of order	*****/
#define D_DU_FLTID_TX_GAIN_ALM_ANT_A		0x0001	/**> TX Gain ALM#AntA					*/
#define D_DU_FLTID_TX_GAIN_ALM_ANT_B		0x0002	/**> TX Gain ALM#AntB					*/
#define D_DU_FLTID_TX_GAIN_ALM_ANT_C		0x0003	/**> TX Gain ALM#AntC					*/
#define D_DU_FLTID_TX_GAIN_ALM_ANT_D		0x0004	/**> TX Gain ALM#AntD					*/
#define D_DU_FLTID_TOVER_ALM_ANT_A			0x0010	/**> TOVER ALM#AntA						*/
#define D_DU_FLTID_TOVER_ALM_ANT_B			0x0011	/**> TOVER ALM#AntB						*/
#define D_DU_FLTID_TOVER_ALM_ANT_C			0x0012	/**> TOVER ALM#AntC						*/
#define D_DU_FLTID_TOVER_ALM_ANT_D			0x0013	/**> TOVER ALM#AntD						*/
#define D_DU_FLTID_TX_CAL_FAIL_ANT_A		0x030	/**> Tx Cal failed#AntA					*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_CAL_FAIL_ANT_B		0x031	/**> Tx Cal failed#AntB					*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_CAL_FAIL_ANT_C		0x032	/**> Tx Cal failed#AntC					*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_CAL_FAIL_ANT_D		0x033	/**> Tx Cal failed#AntD					*//*	5G-商用DU	*/
#define D_DU_FLTID_10_NUM_SB6				3
#define D_DU_FLTID_10_AB_NUM_MMW			1
#define D_DU_FLTID_10_CD_NUM_MMW			0
#define D_DU_FLTID_10_A_SFT					0
#define D_DU_FLTID_10_B_SFT					16
#define D_DU_FLTID_10_C_SFT					0
#define D_DU_FLTID_10_D_SFT					16

#define D_DU_FLTID_TX_PW_ALM_V1				0x100	/**> tx power detector ALM #V1			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V2				0x101	/**> tx power detector ALM #V2			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V3				0x102	/**> tx power detector ALM #V3			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V4				0x103	/**> tx power detector ALM #V4			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V5				0x104	/**> tx power detector ALM #V5			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V6				0x105	/**> tx power detector ALM #V6			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V7				0x106	/**> tx power detector ALM #V7			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V8				0x107	/**> tx power detector ALM #V8			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V9				0x108	/**> tx power detector ALM #V9			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V10			0x109	/**> tx power detector ALM #V10			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V11			0x10A	/**> tx power detector ALM #V11			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V12			0x10B	/**> tx power detector ALM #V12			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V13			0x10C	/**> tx power detector ALM #V13			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V14			0x10D	/**> tx power detector ALM #V14			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V15			0x10E	/**> tx power detector ALM #V15			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_V16			0x10F	/**> tx power detector ALM #V16			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H1				0x110	/**> tx power detector ALM #H1			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H2				0x111	/**> tx power detector ALM #H2			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H3				0x112	/**> tx power detector ALM #H3			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H4				0x113	/**> tx power detector ALM #H4			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H5				0x114	/**> tx power detector ALM #H5			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H6				0x115	/**> tx power detector ALM #H6			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H7				0x116	/**> tx power detector ALM #H7			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H8				0x117	/**> tx power detector ALM #H8			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H9				0x118	/**> tx power detector ALM #H9			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H10			0x119	/**> tx power detector ALM #H10			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H11			0x11A	/**> tx power detector ALM #H11			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H12			0x11B	/**> tx power detector ALM #H12			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H13			0x11C	/**> tx power detector ALM #H13			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H14			0x11D	/**> tx power detector ALM #H14			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H15			0x11E	/**> tx power detector ALM #H15			*//*	5G-商用DU	*/
#define D_DU_FLTID_TX_PW_ALM_H16			0x11F	/**> tx power detector ALM #H16			*//*	5G-商用DU	*/
#define D_DU_FLTID_10_H_NUM_SB6				0
#define D_DU_FLTID_10_H_NUM_MMW				16
#define D_DU_FLTID_10_H_SFT					16
#define D_DU_FLTID_10_V_NUM_SB6				0
#define D_DU_FLTID_10_V_NUM_MMW				16
#define D_DU_FLTID_10_V_SFT					0
#define D_DU_FLTID_10_VH_NUM_SB6			0
#define D_DU_FLTID_10_VH_NUM_MMW			32
#define D_DU_FLTID_10_VH_SFT				0

/***** Fault ID : 0x11 RX out of order	*****/
#define D_DU_FLTID_RX_GAIN_ALM_ANT_A		0x0001	/**> Rx Gain ALM#AntA					*/
#define D_DU_FLTID_RX_GAIN_ALM_ANT_B		0x0002	/**> Rx Gain ALM#AntB					*/
#define D_DU_FLTID_RX_GAIN_ALM_ANT_C		0x0003	/**> Rx Gain ALM#AntC					*/
#define D_DU_FLTID_RX_GAIN_ALM_ANT_D		0x0004	/**> Rx Gain ALM#AntD					*/
#define D_DU_FLTID_RX_CAL_FAIL_ANT_A		0x030	/**> Rx Cal failed#AntA					*//*	5G-商用DU	*/
#define D_DU_FLTID_RX_CAL_FAIL_ANT_B		0x031	/**> Rx Cal failed#AntB					*//*	5G-商用DU	*/
#define D_DU_FLTID_RX_CAL_FAIL_ANT_C		0x032	/**> Rx Cal failed#AntC					*//*	5G-商用DU	*/
#define D_DU_FLTID_RX_CAL_FAIL_ANT_D		0x033	/**> Rx Cal failed#AntD					*//*	5G-商用DU	*/
#define D_DU_FLTID_LNA_ABNORMAL_ANT_A		0x040	/**> LNA Gain abnormal ANT#A			*//*	5G-商用DU	*/
#define D_DU_FLTID_LNA_ABNORMAL_ANT_B		0x041	/**> LNA Gain abnormal ANT#B			*//*	5G-商用DU	*/
#define D_DU_FLTID_LNA_ABNORMAL_ANT_C		0x042	/**> LNA Gain abnormal ANT#C			*//*	5G-商用DU	*/
#define D_DU_FLTID_LNA_ABNORMAL_ANT_D		0x043	/**> LNA Gain abnormal ANT#D			*//*	5G-商用DU	*/
#define D_DU_FLTID_11_NUM_SB6				3
#define D_DU_FLTID_11_AB_NUM_MMW			1
#define D_DU_FLTID_11_CD_NUM_MMW			0
#define D_DU_FLTID_11_A_SFT					8
#define D_DU_FLTID_11_B_SFT					24
#define D_DU_FLTID_11_C_SFT					8
#define D_DU_FLTID_11_D_SFT					24

/***** Fault ID : 0x12 ANT port  abnormal	*****/
#define D_DU_FLTID_VSWR_ALM_ANT_A			0x001	/**> VSWR ALM Ant#A						*//*	5G-商用DU	*/
#define D_DU_FLTID_VSWR_ALM_ANT_B			0x002	/**> VSWR ALM Ant#B						*//*	5G-商用DU	*/
#define D_DU_FLTID_VSWR_ALM_ANT_C			0x003	/**> VSWR ALM Ant#C						*//*	5G-商用DU	*/
#define D_DU_FLTID_VSWR_ALM_ANT_D			0x004	/**> VSWR ALM Ant#D						*//*	5G-商用DU	*/
#define D_DU_FLTID_12_NUM_SB6				1
#define D_DU_FLTID_12_AB_NUM_MMW			0
#define D_DU_FLTID_12_CD_NUM_MMW			0
#define D_DU_FLTID_12_A_SFT					12
#define D_DU_FLTID_12_B_SFT					28
#define D_DU_FLTID_12_C_SFT					12
#define D_DU_FLTID_12_D_SFT					28

/***** Fault ID : 0x30 Temperature too low	*****/
#define D_DU_FLTID_TEMP_ALM_LOW				0x0001	/**> Temperature too low				*/
#define D_DU_FLTID_30_NUM					1
#define D_DU_FLTID_30_SFT					0

/***** Fault ID : 0x31 Temperature too high	*****/
#define D_DU_FLTID_TEMP_ALM_HIGH			0x0001	/**> Temperature too high				*/
#define D_DU_FLTID_31_NUM					1
#define D_DU_FLTID_31_SFT					8

/***** Fault ID : 0x4X fan 関連	*****/
#define D_DU_FLTID_NO_FAN_DETECT			0x001	/**> FLTID(40):FANユニット未実装		*//*	5G-商用DU	*/
#define D_DU_FLTID_40_NUM					1
#define D_DU_FLTID_40_SFT					16
#define D_DU_FLTID_FAN_PS_ALM				0x001	/**> FLTID(41):FAN電源異常				*//*	5G-商用DU	*/
#define D_DU_FLTID_41_NUM					1
#define D_DU_FLTID_41_SFT					20
#define D_DU_FLTID_COOL_FAN1_BROK			0x001	/**> FLTID(42):FAN#1 回転数低下			*//*	5G-商用DU	*/
#define D_DU_FLTID_42_NUM					1
#define D_DU_FLTID_42_SFT					22
#define D_DU_FLTID_COOL_FAN2_BROK			0x001	/**> FLTID(43):FAN#2 回転数低下			*//*	5G-商用DU	*/
#define D_DU_FLTID_43_NUM					1
#define D_DU_FLTID_43_SFT					24
#define D_DU_FLTID_COOL_FAN3_BROK			0x001	/**> FLTID(44):FAN#3 回転数低下			*//*	5G-商用DU	*/
#define D_DU_FLTID_44_NUM					1
#define D_DU_FLTID_44_SFT					26

/***** Fault ID : 0x50 SFP 関連	*****/
#define D_DU_FLTID_SFP_TYP_ERR				0x001	/**> FLTID(50):SFP Type ERR				*//*	5G-商用DU	*/
#define D_DU_FLTID_SFP_ACCESS_ERR			0x002	/**> FLTID(50):SFP ACCESS ERR				*//*	5G-商用DU	*/
#define D_DU_FLTID_50_NUM					2
#define D_DU_FLTID_50_SFT					28

/***** Fault ID : SFP未実装の設定	*****/
#define D_DU_SVCTL_OFF_SFP_NOT_IMPL  		0x00000000
#define D_DU_SVCTL_ON_SFP_NOT_IMPL  		0x00000001

/***** Fault ID : 0x51 SFP FAIL SAFE 関連	*****/
#define D_DU_FLTID_SFP_FAILSAFE				0x001	/**> FLTID(51):FH fail safe (通知なし)	*//*	5G-商用DU	*/
#define D_DU_FLTID_SFP_FAILSAFE_NTF			0x002	/**> FLTID(51):FH fail safe (通知あり)	*//*	5G-商用DU	*/
#define D_DU_FLTID_51_NUM					2
#define D_DU_FLTID_51_SFT					30
#define D_DU_FLTID_51ALM_NUM				1
#define D_DU_FLTID_51NO_NUM					1
#define D_DU_FLTID_51ALM_SFT				D_DU_FLTID_51_SFT
#define D_DU_FLTID_51NO_SFT					31

/***** Fault ID : 0xFX etc		*****/
#define D_DU_FLTID_TX_SLEEP					0x001	/**> FLTID(F0):TxSleep					*//*	5G-商用DU	*/
#define D_DU_FLTID_F0_NUM					1
#define D_DU_FLTID_F0_SFT					16
#define D_DU_FLTID_TX_OFF					0x001	/**> FLTID(F1):TxOFF					*//*	5G-商用DU	*/
#define D_DU_FLTID_F1_NUM					1
#define D_DU_FLTID_F1_SFT					17
#define D_DU_FLTID_BEAM_ID_FIX				0x001	/**> FLTID(F2):BeamID TEST Mode			*//*	5G-商用DU	*/
#define D_DU_FLTID_F2_NUM					1
#define D_DU_FLTID_F2_SFT					18
#define D_DU_FLTID_FORCE_ALM				0x001	/**> FLTID(FF):Forced alarm				*//*	5G-商用DU	*/
#define D_DU_FLTID_FF_NUM					1
#define D_DU_FLTID_FF_SFT					31

/* @} */


/** @name Alarm bit  define */
/* @{ */

/***** SV5INF	*****/
#define D_DU_ALMBIT_FLT01_ALL				0x00000FFF
#define D_DU_ALMBIT_FLT02_ALL				0x0FFF0000


/***** SV6INF	*****/
#define D_DU_ALMBIT_PTP_UNLOCK				0x00000001	/**> FID=3:PTP UnLock(HoldOver)			*//* Major		*/
#define D_DU_ALMBIT_FLT03_ALL				D_DU_ALMBIT_PTP_UNLOCK

#define D_DU_ALMBIT_JC_PLL_UNLOCK			0x00000010	/**> FID=4:JC-PLL Unlock				*//* Critical	*/
#define D_DU_ALMBIT_RF_PLL_UNLOCK			0x00000020	/**> FID=4:RF-PLL Unlock				*//* Critical	*/
#define D_DU_ALMBIT_FLT04_ALL				(	D_DU_ALMBIT_JC_PLL_UNLOCK | D_DU_ALMBIT_RF_PLL_UNLOCK )
#define D_DU_ALMBIT_FLT04_ALL_SB6			(	D_DU_ALMBIT_JC_PLL_UNLOCK )
#define D_DU_ALMBIT_FLT04_ALL_MMW			(	D_DU_ALMBIT_FLT04_ALL )

#define D_DU_ALMBIT_L1_LDOWN_FH1			0x00000100	/**> L1断								*//* Minor		*/
#define D_DU_ALMBIT_BER_DET_FH1				0x00000200	/**> Critical BER detected				*//* Minor		*/
#define D_DU_ALMBIT_SFP_UNMOUNT_FH1			0x00000400	/**> SFP　unmount						*//* Minor		*/
#define D_DU_ALMBIT_SPALNE_ABNL_FH1			0x00001000	/**> S-plane　abnormal					*//* Minor		*/
#define D_DU_ALMBIT_UPALNE_ABNL_FH1			0x00002000	/**> U-plane abnormal					*//* Minor		*/
#define D_DU_ALMBIT_CPALNE_ABNL_FH1			0x00004000	/**> FC-Plane abnormal					*//* Minor		*/
#define D_DU_ALMBIT_FLT05_ALL				(	D_DU_ALMBIT_L1_LDOWN_FH1 | D_DU_ALMBIT_BER_DET_FH1 |\
												D_DU_ALMBIT_SFP_UNMOUNT_FH1 | D_DU_ALMBIT_SPALNE_ABNL_FH1 |\
												D_DU_ALMBIT_UPALNE_ABNL_FH1 | D_DU_ALMBIT_CPALNE_ABNL_FH1 )

#define D_DU_ALMBIT_PS_ALM					0x01000000	/**> RF Module PS ALM					*//* Major		*/
#define D_DU_ALMBIT_POW_ERR1				0x02000000	/**> Sub6:GateBias Power				*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_POW_ERR2				0x04000000	/**> Sub6:RFIC Power					*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_POW_ERR1_SUB6			D_DU_ALMBIT_POW_ERR1	/**> Sub6:GateBias Power				*//* Major		*//*	5G-評価DU	*/
#define D_DU_ALMBIT_POW_ERR2_SUB6			D_DU_ALMBIT_POW_ERR2	/**> Sub6:RFIC Power					*//* Major		*//*	5G-評価DU	*/
#define D_DU_ALMBIT_SFP_PDOWN				0x10000000	/**> SFP Power Down						*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_POW_ERR1_MMW			D_DU_ALMBIT_POW_ERR1	/**> mmW:RXADC Power					*//* Critical	*//*	5G-評価DU	*/
#define D_DU_ALMBIT_POW_ERR2_MMW			D_DU_ALMBIT_POW_ERR2	/**> mmW:TXDAC Power					*//* Critical	*//*	5G-評価DU	*/
#define D_DU_ALMBIT_SFP_PDOWN_FH1			D_DU_ALMBIT_SFP_PDOWN	/**> SFP Power Down						*//* Major		*//*	5G-評価DU	*/
#define D_DU_SVCTL_SFP_POWERDOWN_1			D_DU_ALMBIT_SFP_PDOWN	/**> SFP Power Down (PORT#1)			*//*	5G-評価DU	*/
#define D_DU_ALMBIT_FLT06_ALL				(	D_DU_ALMBIT_PS_ALM | D_DU_ALMBIT_POW_ERR1 | D_DU_ALMBIT_POW_ERR2 | D_DU_ALMBIT_SFP_PDOWN )
#define	D_DU_ALMBIT_SV6INF_ALL				(	D_DU_ALMBIT_PTP_UNLOCK |\
												D_DU_ALMBIT_JC_PLL_UNLOCK |\
												D_DU_ALMBIT_RF_PLL_UNLOCK |\
												D_DU_ALMBIT_PS_ALM |\
												D_DU_ALMBIT_POW_ERR1 |\
												D_DU_ALMBIT_POW_ERR2 )
#define	D_DU_ALMBIT_NON_ALM					0x00000000

/***** SV7INF	*****/
#define D_DU_ALMBIT_SYS_PLL_UNLOCK_LLB		0x00000001	/**> LLB SYS PLL Unlock					*//* Critical	*/
#define D_DU_ALMBIT_IC_PLL_UNLOCK_LLB		0x00000002	/**> LLB ICPLL Unlock					*//* Critical	*/
#define D_DU_ALMBIT_ETH_PLL_UNLOCK_LLB		0x00000004	/**> LLB ETHPLL Unlock					*//* Critical	*/
#define D_DU_ALMBIT_TRX1_PCIE_ERR1_LLB		0x00000008	/**> LLB_TRX#1 PCIE Root Error			*//* Critical	*/
#define D_DU_ALMBIT_TRX1_PCIE_ERR2_LLB		0x00000010	/**> LLB_TRX#1 PCIE EndPoint Error		*//* Critical	*/
#define D_DU_ALMBIT_TRX1_SV_FAIL_LLB		0x00000020	/**> LLB_TRX#1 SV bus failure			*//* Critical	*/
#define D_DU_ALMBIT_ETH_ALM					0x00000040	/**> Ether Alm							*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_SRAM_CORRECT_ERR		0x00000080	/**> SRAM Correct Error					*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_SRAM_UNCORRECT_ERR		0x00000100	/**> SRAM UnCorrect Error				*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TRX1_IC_TX_ERR_LLB		0x00000200	/**> LLB_TRX#1 IC TX ERR				*//* Critical	*/
#define D_DU_ALMBIT_TRX1_IC_RX_ERR_LLB		0x00000400	/**> LLB_TRX#1 IC RX ERR				*//* Critical	*/
#define D_DU_ALMBIT_LLB_IC_TX_ERR_TRX1		0x00000800	/**> TRX_LLB IC TX ERR					*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_LLB_IC_RX_ERR_TRX1		0x00001000	/**> TRX_LLB IC RX ERR					*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_AUTO_CRUISE_HC_ERR		0x00008000	/**> Auto Cruise HC ERR					*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_SYS_PLL_UNLOCK_TRX1		0x00010000	/**> TRX#1 SYS PLL Unlock				*//* Critical	*/
#define D_DU_ALMBIT_IC_PLL_UNLOCK_TRX1		0x00020000	/**> TRX#1 ICPLL Unlock					*//* Critical	*/
#define D_DU_ALMBIT_PHYICPLLULCK1_TRX1		0x00040000	/**> TRX#1 PHYICPLLULCK1				*//* Critical	*/
#define D_DU_ALMBIT_PHYICPLLULCK2_TRX1		0x00080000	/**> TRX#1 PHYICPLLULCK2				*//* Critical	*/
#define D_DU_ALMBIT_PHY_POW_ALM_TRX1		0x00100000	/**> TRX PHYPOWALM						*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_PHYJS1PLLULCK_TRX1		0x00200000	/**> TRX#1 PHYJS1PLLULCK				*//* Critical	*/
#define D_DU_ALMBIT_PHYJS2PLLULCK_TRX1		0x00400000	/**> TRX#1 PHYJS2PLLULCK				*//* Critical	*/
#define D_DU_ALMBIT_TRX1_SV_FAIL_TRX1		0x00800000	/**> TRX#1 SV bus failure				*//* Critical	*/
#define D_DU_ALMBIT_R1_TX_JESD_ERR_TRX1		0x01000000	/**> TRX#1/RFIC#1 TX_JESD IF ERR		*//* Critical	*/
#define D_DU_ALMBIT_R1_FB_JESD_ERR_TRX1		0x02000000	/**> TRX#1/RFIC#1 FB_JESD IF ERR		*//* Critical	*/
#define D_DU_ALMBIT_R1_RX_JESD_ERR_TRX1		0x04000000	/**> TRX#1/RFIC#1 RX_JESD IF ERR		*//* Critical	*/
#define D_DU_ALMBIT_R2_TX_JESD_ERR_TRX1		0x08000000	/**> TRX#1/RFIC#2 TX_JESD IF ERR		*//* Critical	*/
#define D_DU_ALMBIT_R2_FB_JESD_ERR_TRX1		0x10000000	/**> TRX#1/RFIC#2 FB_JESD IF ERR		*//* Critical	*/
#define D_DU_ALMBIT_R2_RX_JESD_ERR_TRX1		0x20000000	/**> TRX#1/RFIC#2 RX_JESD IF ERR		*//* Critical	*/
#define D_DU_ALMBIT_LMB_CORRECT_ERR			0x40000000	/**> LMB Correct Error					*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_LMB_UNCORRECT_ERR		0x80000000	/**> LMB UnCorrect Error				*//* Critical	*//*	5G-商用DU	*/
#define	D_DU_ALMBIT_SV7INF_ALL				(	D_DU_ALMBIT_SYS_PLL_UNLOCK_LLB |\
												D_DU_ALMBIT_IC_PLL_UNLOCK_LLB |\
												D_DU_ALMBIT_ETH_PLL_UNLOCK_LLB |\
												D_DU_ALMBIT_TRX1_PCIE_ERR1_LLB |\
												D_DU_ALMBIT_TRX1_PCIE_ERR2_LLB |\
												D_DU_ALMBIT_TRX1_SV_FAIL_LLB |\
												D_DU_ALMBIT_ETH_ALM |\
												D_DU_ALMBIT_SRAM_CORRECT_ERR |\
												D_DU_ALMBIT_SRAM_UNCORRECT_ERR |\
												D_DU_ALMBIT_TRX1_IC_TX_ERR_LLB |\
												D_DU_ALMBIT_TRX1_IC_RX_ERR_LLB |\
												D_DU_ALMBIT_LLB_IC_TX_ERR_TRX1 |\
												D_DU_ALMBIT_LLB_IC_RX_ERR_TRX1 |\
												D_DU_ALMBIT_AUTO_CRUISE_HC_ERR |\
												D_DU_ALMBIT_SYS_PLL_UNLOCK_TRX1 |\
												D_DU_ALMBIT_IC_PLL_UNLOCK_TRX1 |\
												D_DU_ALMBIT_PHYICPLLULCK1_TRX1 |\
												D_DU_ALMBIT_PHYICPLLULCK2_TRX1 |\
												D_DU_ALMBIT_PHY_POW_ALM_TRX1 |\
												D_DU_ALMBIT_PHYJS1PLLULCK_TRX1 |\
												D_DU_ALMBIT_PHYJS2PLLULCK_TRX1 |\
												D_DU_ALMBIT_TRX1_SV_FAIL_TRX1 |\
												D_DU_ALMBIT_R1_TX_JESD_ERR_TRX1 |\
												D_DU_ALMBIT_R1_FB_JESD_ERR_TRX1 |\
												D_DU_ALMBIT_R1_RX_JESD_ERR_TRX1 |\
												D_DU_ALMBIT_R2_TX_JESD_ERR_TRX1 |\
												D_DU_ALMBIT_R2_FB_JESD_ERR_TRX1 |\
												D_DU_ALMBIT_R2_RX_JESD_ERR_TRX1 |\
												D_DU_ALMBIT_LMB_CORRECT_ERR |\
												D_DU_ALMBIT_LMB_UNCORRECT_ERR )
#define D_DU_ALMBIT_FLT07_ALL				(	D_DU_ALMBIT_SV7INF_ALL )
#define D_DU_ALMBIT_FLT07_ALL_SB6			(	D_DU_ALMBIT_SV7INF_ALL )
#define D_DU_ALMBIT_FLT07_ALL_MMW			(	D_DU_ALMBIT_SYS_PLL_UNLOCK_LLB |\
												D_DU_ALMBIT_IC_PLL_UNLOCK_LLB |\
												D_DU_ALMBIT_ETH_PLL_UNLOCK_LLB |\
												D_DU_ALMBIT_TRX1_PCIE_ERR1_LLB |\
												D_DU_ALMBIT_TRX1_PCIE_ERR2_LLB |\
												D_DU_ALMBIT_TRX1_SV_FAIL_LLB |\
												D_DU_ALMBIT_ETH_ALM |\
												D_DU_ALMBIT_SRAM_CORRECT_ERR |\
												D_DU_ALMBIT_SRAM_UNCORRECT_ERR |\
												D_DU_ALMBIT_TRX1_IC_TX_ERR_LLB |\
												D_DU_ALMBIT_TRX1_IC_RX_ERR_LLB |\
												D_DU_ALMBIT_LLB_IC_TX_ERR_TRX1 |\
												D_DU_ALMBIT_LLB_IC_RX_ERR_TRX1 |\
												D_DU_ALMBIT_SYS_PLL_UNLOCK_TRX1 |\
												D_DU_ALMBIT_IC_PLL_UNLOCK_TRX1 |\
												D_DU_ALMBIT_PHYICPLLULCK1_TRX1 |\
												D_DU_ALMBIT_PHY_POW_ALM_TRX1 |\
												D_DU_ALMBIT_PHYJS1PLLULCK_TRX1 |\
												D_DU_ALMBIT_PHYJS2PLLULCK_TRX1 |\
												D_DU_ALMBIT_TRX1_SV_FAIL_TRX1 |\
												D_DU_ALMBIT_R1_TX_JESD_ERR_TRX1 |\
												D_DU_ALMBIT_R1_RX_JESD_ERR_TRX1 |\
												D_DU_ALMBIT_LMB_CORRECT_ERR |\
												D_DU_ALMBIT_LMB_UNCORRECT_ERR )


/***** SV8INF	*****/
#define D_DU_ALMBIT_RFIC1_ALM				0x00010000	/**> RFIC#1 ALM							*//* Critical	*/
#define D_DU_ALMBIT_RFIC2_ALM				0x00020000	/**> RFIC#2 ALM							*//* Critical	*/
#define D_DU_ALMBIT_TXDAC_ALM				D_DU_ALMBIT_RFIC1_ALM	/**> TXDAC ALM				*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_RXADC_ALM				D_DU_ALMBIT_RFIC2_ALM	/**> RXADC ALM				*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT08_ALL				(	D_DU_ALMBIT_RFIC1_ALM | D_DU_ALMBIT_RFIC2_ALM )
#define D_DU_ALMBIT_FLT08_ALL_SB6			(	D_DU_ALMBIT_FLT08_ALL )
#define D_DU_ALMBIT_FLT08_ALL_MMW			(	D_DU_ALMBIT_TXDAC_ALM | D_DU_ALMBIT_RXADC_ALM )

/***** SV9INF	*****/
#define D_DU_ALMBIT_TX_GAIN_ALM_ANT_A		0x00000001	/**> TX Gain ALM #AntA					*//* Major		*/
#define D_DU_ALMBIT_TOVER_ALM_ANT_A			0x00000002	/**> TOVER ALM   #AntA					*//* Major		*/
#define D_DU_ALMBIT_TX_CAL_FAIL_ANT_A		0x00000008	/**> Tx Cal failed#AntA					*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_RX_GAIN_ALM_ANT_A		0x00000100	/**> Rx Gain ALM #AntA					*//* Major		*/
#define D_DU_ALMBIT_RX_CAL_FAIL_ANT_A		0x00000200	/**> Rx Cal failed#AntA					*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_LNA_ABNORMAL_ANT_A		0x00000400	/**> LNA Gain abnormal ANT#A			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_VSWR_ALM_ANT_A			0x00001000	/**> VSWR ALM Ant#A						*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT10_A_ALL				(	D_DU_ALMBIT_TX_GAIN_ALM_ANT_A | D_DU_ALMBIT_TOVER_ALM_ANT_A | D_DU_ALMBIT_TX_CAL_FAIL_ANT_A )
#define D_DU_ALMBIT_FLT11_A_ALL				(	D_DU_ALMBIT_RX_GAIN_ALM_ANT_A | D_DU_ALMBIT_RX_CAL_FAIL_ANT_A | D_DU_ALMBIT_LNA_ABNORMAL_ANT_A )
#define D_DU_ALMBIT_FLT12_A_ALL				(	D_DU_ALMBIT_VSWR_ALM_ANT_A )
#define D_DU_ALMBIT_FLT10_A_ALL_SB6			(	D_DU_ALMBIT_FLT10_A_ALL )
#define D_DU_ALMBIT_FLT11_A_ALL_SB6			(	D_DU_ALMBIT_FLT11_A_ALL )
#define D_DU_ALMBIT_FLT12_A_ALL_SB6			(	D_DU_ALMBIT_FLT12_A_ALL )
#define D_DU_ALMBIT_FLT10_A_ALL_MMW			(	D_DU_ALMBIT_TOVER_ALM_ANT_A )
#define D_DU_ALMBIT_FLT11_A_ALL_MMW			(	D_DU_ALMBIT_RX_GAIN_ALM_ANT_A )
#define D_DU_ALMBIT_FLT12_ALL_MMW			(	0x00000000 )
#define D_DU_ALMBIT_FLT12_A_ALL_MMW			(	D_DU_ALMBIT_FLT12_ALL_MMW )


#define D_DU_ALMBIT_TX_GAIN_ALM_ANT_B		0x00010000	/**> TX Gain ALM #AntB					*//* Major		*/
#define D_DU_ALMBIT_TOVER_ALM_ANT_B			0x00020000	/**> TOVER ALM   #AntB					*//* Major		*/
#define D_DU_ALMBIT_TX_CAL_FAIL_ANT_B		0x00080000	/**> Tx Cal failed#AntB					*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_RX_GAIN_ALM_ANT_B		0x01000000	/**> Rx Gain ALM #AntB					*//* Major		*/
#define D_DU_ALMBIT_RX_CAL_FAIL_ANT_B		0x02000000	/**> Rx Cal failed#AntA					*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_LNA_ABNORMAL_ANT_B		0x04000000	/**> LNA Gain abnormal ANT#A			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_VSWR_ALM_ANT_B			0x10000000	/**> VSWR ALM Ant#B						*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT10_B_ALL				(	D_DU_ALMBIT_TX_GAIN_ALM_ANT_B | D_DU_ALMBIT_TOVER_ALM_ANT_B | D_DU_ALMBIT_TX_CAL_FAIL_ANT_B )
#define D_DU_ALMBIT_FLT11_B_ALL				(	D_DU_ALMBIT_RX_GAIN_ALM_ANT_B | D_DU_ALMBIT_RX_CAL_FAIL_ANT_B | D_DU_ALMBIT_LNA_ABNORMAL_ANT_B )
#define D_DU_ALMBIT_FLT12_B_ALL				(	D_DU_ALMBIT_VSWR_ALM_ANT_B )
#define D_DU_ALMBIT_FLT10_B_ALL_SB6			(	D_DU_ALMBIT_FLT10_B_ALL )
#define D_DU_ALMBIT_FLT11_B_ALL_SB6			(	D_DU_ALMBIT_FLT11_B_ALL )
#define D_DU_ALMBIT_FLT12_B_ALL_SB6			(	D_DU_ALMBIT_FLT12_B_ALL )
#define D_DU_ALMBIT_FLT10_B_ALL_MMW			(	D_DU_ALMBIT_TOVER_ALM_ANT_B )
#define D_DU_ALMBIT_FLT11_B_ALL_MMW			(	D_DU_ALMBIT_RX_GAIN_ALM_ANT_B )
#define D_DU_ALMBIT_FLT12_B_ALL_MMW			(	D_DU_ALMBIT_FLT12_ALL_MMW )

/***** SV10INF	*****/
#define D_DU_ALMBIT_TX_GAIN_ALM_ANT_C		0x00000001	/**> TX Gain ALM #AntA					*//* Major		*/
#define D_DU_ALMBIT_TOVER_ALM_ANT_C			0x00000002	/**> TOVER ALM   #AntA					*//* Major		*/
#define D_DU_ALMBIT_TX_CAL_FAIL_ANT_C		0x00000008	/**> Tx Cal failed#AntA					*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_RX_GAIN_ALM_ANT_C		0x00000100	/**> Rx Gain ALM #AntA					*//* Major		*/
#define D_DU_ALMBIT_RX_CAL_FAIL_ANT_C		0x00000200	/**> Rx Cal failed#AntA					*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_LNA_ABNORMAL_ANT_C		0x00000400	/**> LNA Gain abnormal ANT#A			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_VSWR_ALM_ANT_C			0x00001000	/**> VSWR ALM Ant#C						*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT10_C_ALL				(	D_DU_ALMBIT_TX_GAIN_ALM_ANT_C | D_DU_ALMBIT_TOVER_ALM_ANT_C | D_DU_ALMBIT_TX_CAL_FAIL_ANT_C )
#define D_DU_ALMBIT_FLT11_C_ALL				(	D_DU_ALMBIT_RX_GAIN_ALM_ANT_C | D_DU_ALMBIT_RX_CAL_FAIL_ANT_C | D_DU_ALMBIT_LNA_ABNORMAL_ANT_C )
#define D_DU_ALMBIT_FLT12_C_ALL				(	D_DU_ALMBIT_VSWR_ALM_ANT_C )
#define D_DU_ALMBIT_FLT10_C_ALL_SB6			(	D_DU_ALMBIT_FLT10_C_ALL )
#define D_DU_ALMBIT_FLT11_C_ALL_SB6			(	D_DU_ALMBIT_FLT11_C_ALL )
#define D_DU_ALMBIT_FLT12_C_ALL_SB6			(	D_DU_ALMBIT_FLT12_C_ALL )
#define D_DU_ALMBIT_FLT10_CD_ALL_MMW		(	0x00000000 )
#define D_DU_ALMBIT_FLT11_CD_ALL_MMW		(	0x00000000 )
#define D_DU_ALMBIT_FLT12_CD_ALL_MMW		(	D_DU_ALMBIT_FLT12_ALL_MMW )


#define D_DU_ALMBIT_TX_GAIN_ALM_ANT_D		0x00010000	/**> TX Gain ALM #AntB					*//* Major		*/
#define D_DU_ALMBIT_TOVER_ALM_ANT_D			0x00020000	/**> TOVER ALM   #AntB					*//* Major		*/
#define D_DU_ALMBIT_TX_CAL_FAIL_ANT_D		0x00080000	/**> Tx Cal failed#AntB					*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_RX_GAIN_ALM_ANT_D		0x01000000	/**> Rx Gain ALM #AntB					*//* Major		*/
#define D_DU_ALMBIT_RX_CAL_FAIL_ANT_D		0x02000000	/**> Rx Cal failed#AntA					*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_LNA_ABNORMAL_ANT_D		0x04000000	/**> LNA Gain abnormal ANT#A			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_VSWR_ALM_ANT_D			0x10000000	/**> VSWR ALM Ant#D						*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT10_D_ALL				(	D_DU_ALMBIT_TX_GAIN_ALM_ANT_D | D_DU_ALMBIT_TOVER_ALM_ANT_D | D_DU_ALMBIT_TX_CAL_FAIL_ANT_D )
#define D_DU_ALMBIT_FLT11_D_ALL				(	D_DU_ALMBIT_RX_GAIN_ALM_ANT_D | D_DU_ALMBIT_RX_CAL_FAIL_ANT_D | D_DU_ALMBIT_LNA_ABNORMAL_ANT_D )
#define D_DU_ALMBIT_FLT12_D_ALL				(	D_DU_ALMBIT_VSWR_ALM_ANT_D )
#define D_DU_ALMBIT_FLT10_D_ALL_SB6			(	D_DU_ALMBIT_FLT10_D_ALL )
#define D_DU_ALMBIT_FLT11_D_ALL_SB6			(	D_DU_ALMBIT_FLT11_D_ALL )
#define D_DU_ALMBIT_FLT12_D_ALL_SB6			(	D_DU_ALMBIT_FLT12_D_ALL )

/***** SV11INF	*****/
#define D_DU_ALMBIT_TX_PW_ALM_V1			0x00000001	/**> tx power detector ALM #V1			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V2			0x00000002	/**> tx power detector ALM #V2			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V3			0x00000004	/**> tx power detector ALM #V3			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V4			0x00000008	/**> tx power detector ALM #V4			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V5			0x00000010	/**> tx power detector ALM #V5			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V6			0x00000020	/**> tx power detector ALM #V6			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V7			0x00000040	/**> tx power detector ALM #V7			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V8			0x00000080	/**> tx power detector ALM #V8			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V9			0x00000100	/**> tx power detector ALM #V9			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V10			0x00000200	/**> tx power detector ALM #V10			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V11			0x00000400	/**> tx power detector ALM #V11			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V12			0x00000800	/**> tx power detector ALM #V12			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V13			0x00001000	/**> tx power detector ALM #V13			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V14			0x00002000	/**> tx power detector ALM #V14			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V15			0x00004000	/**> tx power detector ALM #V15			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_V16			0x00008000	/**> tx power detector ALM #V16			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H1			0x00010000	/**> tx power detector ALM #H1			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H2			0x00020000	/**> tx power detector ALM #H2			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H3			0x00040000	/**> tx power detector ALM #H3			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H4			0x00080000	/**> tx power detector ALM #H4			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H5			0x00100000	/**> tx power detector ALM #H5			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H6			0x00200000	/**> tx power detector ALM #H6			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H7			0x00400000	/**> tx power detector ALM #H7			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H8			0x00800000	/**> tx power detector ALM #H8			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H9			0x01000000	/**> tx power detector ALM #H9			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H10			0x02000000	/**> tx power detector ALM #H10			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H11			0x04000000	/**> tx power detector ALM #H11			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H12			0x08000000	/**> tx power detector ALM #H12			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H13			0x10000000	/**> tx power detector ALM #H13			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H14			0x20000000	/**> tx power detector ALM #H14			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H15			0x40000000	/**> tx power detector ALM #H15			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_PW_ALM_H16			0x80000000	/**> tx power detector ALM #H16			*//* Major		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT10_VH_SB6			(	0x00000000 )
#define D_DU_ALMBIT_FLT10_VH_ALL_SB6		(	0x00000000 )
#define D_DU_ALMBIT_FLT10_H_ALL				(	D_DU_ALMBIT_TX_PW_ALM_H1  | D_DU_ALMBIT_TX_PW_ALM_H2  |\
												D_DU_ALMBIT_TX_PW_ALM_H3  | D_DU_ALMBIT_TX_PW_ALM_H4  |\
												D_DU_ALMBIT_TX_PW_ALM_H5  | D_DU_ALMBIT_TX_PW_ALM_H6  |\
												D_DU_ALMBIT_TX_PW_ALM_H7  | D_DU_ALMBIT_TX_PW_ALM_H8  |\
												D_DU_ALMBIT_TX_PW_ALM_H9  | D_DU_ALMBIT_TX_PW_ALM_H10 |\
												D_DU_ALMBIT_TX_PW_ALM_H11 | D_DU_ALMBIT_TX_PW_ALM_H12 |\
												D_DU_ALMBIT_TX_PW_ALM_H13 | D_DU_ALMBIT_TX_PW_ALM_H14 |\
												D_DU_ALMBIT_TX_PW_ALM_H15 | D_DU_ALMBIT_TX_PW_ALM_H16 )
#define D_DU_ALMBIT_FLT10_H_ALL_MMW			(	D_DU_ALMBIT_FLT10_H_ALL )
#define D_DU_ALMBIT_FLT10_V_ALL				(	D_DU_ALMBIT_TX_PW_ALM_V1  | D_DU_ALMBIT_TX_PW_ALM_V2  |\
												D_DU_ALMBIT_TX_PW_ALM_V3  | D_DU_ALMBIT_TX_PW_ALM_V4  |\
												D_DU_ALMBIT_TX_PW_ALM_V5  | D_DU_ALMBIT_TX_PW_ALM_V6  |\
												D_DU_ALMBIT_TX_PW_ALM_V7  | D_DU_ALMBIT_TX_PW_ALM_V8  |\
												D_DU_ALMBIT_TX_PW_ALM_V9  | D_DU_ALMBIT_TX_PW_ALM_V10 |\
												D_DU_ALMBIT_TX_PW_ALM_V11 | D_DU_ALMBIT_TX_PW_ALM_V12 |\
												D_DU_ALMBIT_TX_PW_ALM_V13 | D_DU_ALMBIT_TX_PW_ALM_V14 |\
												D_DU_ALMBIT_TX_PW_ALM_V15 | D_DU_ALMBIT_TX_PW_ALM_V16 )
#define D_DU_ALMBIT_FLT10_V_ALL_MMW			(	D_DU_ALMBIT_FLT10_V_ALL )
#define D_DU_ALMBIT_FLT10_VH_ALL			(	D_DU_ALMBIT_FLT10_H_ALL  | D_DU_ALMBIT_FLT10_V_ALL )
#define D_DU_ALMBIT_FLT10_VH_ALL_MMW		(	D_DU_ALMBIT_FLT10_VH_ALL )

#define	D_DU_ALMBIT_ALM_ANT_A_ALL		( 	D_DU_ALMBIT_FLT10_A_ALL_SB6 | D_DU_ALMBIT_FLT11_A_ALL_SB6 | D_DU_ALMBIT_FLT12_A_ALL_SB6 )
#define	D_DU_ALMBIT_ALM_ANT_B_ALL		( 	D_DU_ALMBIT_FLT10_B_ALL_SB6 | D_DU_ALMBIT_FLT11_B_ALL_SB6 | D_DU_ALMBIT_FLT12_B_ALL_SB6 )
#define	D_DU_ALMBIT_ALM_ANT_C_ALL		(	D_DU_ALMBIT_FLT10_C_ALL_SB6 | D_DU_ALMBIT_FLT11_C_ALL_SB6 | D_DU_ALMBIT_FLT12_C_ALL_SB6 )
#define	D_DU_ALMBIT_ALM_ANT_D_ALL		(	D_DU_ALMBIT_FLT10_D_ALL_SB6 | D_DU_ALMBIT_FLT11_D_ALL_SB6 | D_DU_ALMBIT_FLT12_D_ALL_SB6 )

#define	D_DU_ALMBIT_TX_ALL_ALM_ANT_A	( 	D_DU_ALMBIT_FLT10_A_ALL_SB6 )
#define	D_DU_ALMBIT_TX_ALL_ALM_ANT_B	( 	D_DU_ALMBIT_FLT10_B_ALL_SB6 )
#define	D_DU_ALMBIT_TX_ALL_ALM_ANT_C	( 	D_DU_ALMBIT_FLT10_C_ALL_SB6 )
#define	D_DU_ALMBIT_TX_ALL_ALM_ANT_D	( 	D_DU_ALMBIT_FLT10_D_ALL_SB6 )
#define	D_DU_ALMBIT_TX_ALMBUT_NON			0x00000000


/***** SV13INF	*****/
#define D_DU_ALMBIT_TEMP_ALM_LOW			0x00000001	/**> Temp ALM (low)						*//* Major		*/
#define D_DU_ALMBIT_FLT30_ALL			(	D_DU_ALMBIT_TEMP_ALM_LOW )

#define D_DU_ALMBIT_TEMP_ALM_HIGH			0x00000100	/**> Temp ALM (High)					*//* Critical	*/
#define D_DU_ALMBIT_FLT31_ALL			(	D_DU_ALMBIT_TEMP_ALM_HIGH )

#define D_DU_ALMBIT_NO_FAN_DETECT			0x00010000	/**> FLTID(40):FANユニット未実装		*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT40_ALL			(	D_DU_ALMBIT_NO_FAN_DETECT )

#define D_DU_ALMBIT_FAN_PS_ALM				0x00100000	/**> FLTID(41):FAN電源異常				*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT41_ALL			(	D_DU_ALMBIT_FAN_PS_ALM )

#define D_DU_ALMBIT_COOL_FAN1_BROK			0x00400000	/**> FLTID(42):FAN#1 回転数低下			*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT42_ALL			(	D_DU_ALMBIT_COOL_FAN1_BROK )

#define D_DU_ALMBIT_COOL_FAN2_BROK			0x01000000	/**> FLTID(43):FAN#2 回転数低下			*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT43_ALL			(	D_DU_ALMBIT_COOL_FAN2_BROK )

#define D_DU_ALMBIT_COOL_FAN3_BROK			0x04000000	/**> FLTID(44):FAN#3 回転数低下			*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLT44_ALL			(	D_DU_ALMBIT_COOL_FAN3_BROK )

#define D_DU_ALMBIT_SFP_TYP_ERR				0x10000000	/**> FLTID(50):SFP Type ERR				*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_SFP_ACCESS_ERR			0x20000000	/**> FLTID(50):SFP ACCESS ERR			*//* Minor		*/
#define D_DU_ALMBIT_FLT50_ALL			(	D_DU_ALMBIT_SFP_TYP_ERR | D_DU_ALMBIT_SFP_ACCESS_ERR )

#define D_DU_ALMBIT_SFP_FAILSAFE			0x40000000	/**> FLTID(51):FH fail safe (通知なし)	*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_SFP_FAILSAFE_NTF		0x80000000	/**> FLTID(51):FH fail safe (通知あり)	*//* Minor		*/
#define D_DU_ALMBIT_FLT51_ALL			(	D_DU_ALMBIT_SFP_FAILSAFE | D_DU_ALMBIT_SFP_FAILSAFE_NTF )
#define D_DU_ALMBIT_FLT51ALM_ALL			D_DU_ALMBIT_SFP_FAILSAFE_NTF
#define D_DU_ALMBIT_FLT51NO_ALL				D_DU_ALMBIT_SFP_FAILSAFE

/***** SV14INF	*****//*	5G-商用DU	*/
#define D_DU_ALMBIT_TX_SLEEP				0x00010000	/**> FLTID(F0):TxSleep					*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLTF0_ALL			(	D_DU_ALMBIT_TX_SLEEP )

#define D_DU_ALMBIT_TX_OFF					0x00020000	/**> FLTID(F1):TxOFF					*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLTF1_ALL			(	D_DU_ALMBIT_TX_OFF )

#define D_DU_ALMBIT_BEAM_ID_FIX				0x00040000	/**> FLTID(F2):BeamID TEST Mode			*//* Minor		*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLTF2_ALL			(	D_DU_ALMBIT_BEAM_ID_FIX )

#define D_DU_ALMBIT_FORCE_ALM				0x80000000	/**> FLTID(FF):Forced alarm				*//* Critical	*//*	5G-商用DU	*/
#define D_DU_ALMBIT_FLTFF_ALL			(	D_DU_ALMBIT_FORCE_ALM )

#define D_DU_ALMBIT_FLTEA60_ALL			D_DU_ALMBIT_FLT50_ALL /* 2021/03/03 M&C)DuongMX Item No.35 - SFP Management (Add) */

/***** Fault ID : 0xFX etc		*****/


/* @} */

/*!
 * @brief the bit defintion in SV control register
 * @{  
 */
/***** SV1INF	*****/
#define D_DU_SVCTL_BIT_ADM_LOCKED			0x00000001	/**> Admin State = locked				*/
#define D_DU_SVCTL_BIT_ADM_DOWN				0x00000002	/**> Admin State = shutting down		*/
#define D_DU_SVCTL_BIT_ADM_UNLOCKED			0x00000004	/**> Admin State = unlocked				*/
#define D_DU_SVCTL_BIT_ADM_LOCKED_LO		0x01000000	/**> Admin State = locked(local)		*/
#define D_DU_SVCTL_BIT_ADM_UNLOCKED_LO		0x04000000	/**> Admin State = unlocked(local)		*/
#define	D_DU_SVCTL_BIT_ADM_ALL				(	D_DU_SVCTL_BIT_ADM_LOCKED |\
												D_DU_SVCTL_BIT_ADM_DOWN |\
												D_DU_SVCTL_BIT_ADM_UNLOCKED |\
												D_DU_SVCTL_BIT_ADM_LOCKED_LO |\
												D_DU_SVCTL_BIT_ADM_UNLOCKED_LO )
#define	D_DU_SVCTL_BIT_ADM_LOCKED_ALL		(	D_DU_SVCTL_BIT_ADM_LOCKED | D_DU_SVCTL_BIT_ADM_LOCKED_LO	)
#define	D_DU_SVCTL_BIT_ADM_UNLOCKED_ALL		(	D_DU_SVCTL_BIT_ADM_UNLOCKED | D_DU_SVCTL_BIT_ADM_UNLOCKED_LO	)
#define	D_DU_SVCTL_BIT_ADM_LO_ALL			(	D_DU_SVCTL_BIT_ADM_LOCKED_LO | D_DU_SVCTL_BIT_ADM_UNLOCKED_LO	)

#define D_DU_SVCTL_BIT_ENG_SLEEP			0x00000010	/**> Power State = sleep				*/
#define D_DU_SVCTL_BIT_ENG_AWAKE			0x00000020	/**> Power State = awake				*/
#define	D_DU_SVCTL_BIT_ENG_ALL				(	D_DU_SVCTL_BIT_ENG_SLEEP |\
												D_DU_SVCTL_BIT_ENG_AWAKE )

#define D_DU_SVCTL_BIT_OPE_ENABLE			0x00000100	/**> Operational State = enable			*/
#define D_DU_SVCTL_BIT_OPE_DISABLE			0x00000200	/**> Operational State = disable		*/
#define	D_DU_SVCTL_BIT_OPE_ALL				(	D_DU_SVCTL_BIT_OPE_ENABLE |\
												D_DU_SVCTL_BIT_OPE_DISABLE )

#define D_DU_SVCTL_BIT_AVL_FAULTY			0x00001000	/**> availability State = faulty		*/
#define D_DU_SVCTL_BIT_AVL_DEGRADED			0x00002000	/**> availability State = degraded		*/
#define D_DU_SVCTL_BIT_AVL_NORMAL			0x00004000	/**> availability State = normal		*/
#define	D_DU_SVCTL_BIT_AVL_ALL				(	D_DU_SVCTL_BIT_AVL_FAULTY |\
												D_DU_SVCTL_BIT_AVL_DEGRADED |\
												D_DU_SVCTL_BIT_AVL_NORMAL )
#define	D_DU_SVCTL_BIT_OFF					0x00000000


#define D_DU_SVCTL_BIT_USA_BUSY				0x00010000	/**> usage State = busy					*/
#define D_DU_SVCTL_BIT_USA_ACTIVE			0x00020000	/**> usage State = active				*/
#define D_DU_SVCTL_BIT_USA_IDLE				0x00040000	/**> usage State = idle					*/
#define	D_DU_SVCTL_BIT_USA_ALL				(	D_DU_SVCTL_BIT_USA_BUSY |\
												D_DU_SVCTL_BIT_USA_ACTIVE |\
												D_DU_SVCTL_BIT_USA_IDLE )


/*	SVCTL用(BIT ON/OFF操作)	*/
#define D_DU_SVCTL_ON_ADM_LOCKED			0x00000001	/**> Admin State = locked				*/
#define D_DU_SVCTL_ON_ADM_DOWN				0x00000002	/**> Admin State = shutting down		*/
#define D_DU_SVCTL_ON_ADM_UNLOCKED			0x00000004	/**> Admin State = unlocked				*/
#define D_DU_SVCTL_ON_ADM_LOCKED_LO			0x01000000	/**> Admin State = locked(local)		*/
#define D_DU_SVCTL_ON_ADM_UNLOCKED_LO		0x04000000	/**> Admin State = unlocked(local)		*/
#define D_DU_SVCTL_ON_ADM_NUM				5
#define D_DU_SVCTL_ON_ADM_SFT				0

#define D_DU_SVCTL_ON_ENG_SLEEP				0x00000001	/**> Power State = sleep				*/
#define D_DU_SVCTL_ON_ENG_AWAKE				0x00000002	/**> Power State = awake				*/
#define D_DU_SVCTL_ON_ENG_NUM				2
#define D_DU_SVCTL_ON_ENG_SFT				4

#define D_DU_SVCTL_ON_OPE_ENABLE			0x00000001	/**> Operational State = enable			*/
#define D_DU_SVCTL_ON_OPE_DISABLE			0x00000002	/**> Operational State = disable		*/
#define D_DU_SVCTL_ON_OPE_NUM				2
#define D_DU_SVCTL_ON_OPE_SFT				8

#define D_DU_SVCTL_ON_AVL_FAULTY			0x00000001	/**> availability State = faulty		*/
#define D_DU_SVCTL_ON_AVL_DEGRADED			0x00000002	/**> availability State = degraded		*/
#define D_DU_SVCTL_ON_AVL_NORMAL			0x00000004	/**> availability State = normal		*/
#define D_DU_SVCTL_ON_AVL_NUM				3
#define D_DU_SVCTL_ON_AVL_SFT				12

#define D_DU_SVCTL_ON_USA_BUSY				0x00000001	/**> usage State = busy					*/
#define D_DU_SVCTL_ON_USA_ACTIVE			0x00000002	/**> usage State = active				*/
#define D_DU_SVCTL_ON_USA_IDLE				0x00000004	/**> usage State = idle					*/
#define D_DU_SVCTL_ON_USA_NUM				3
#define D_DU_SVCTL_ON_USA_SFT				16

/***** SV2INF	*****/
#define D_DU_SVCTL_BIT_NON_USE_ALL			0x00000000
#define D_DU_SVCTL_BIT_USE_A_C0				0x00000001	/**> キャリア状態 Ant#A C0 USE			*/
#define D_DU_SVCTL_BIT_USE_A_C1				0x00000002	/**> キャリア状態 Ant#A C1 USE			*/
#define D_DU_SVCTL_BIT_USE_A_C2				0x00000004	/**> キャリア状態 Ant#A C2 USE			*/
#define D_DU_SVCTL_BIT_USE_A_C3				0x00000008	/**> キャリア状態 Ant#A C3 USE			*/
#define D_DU_SVCTL_BIT_SLP_A_C0				0x00000010	/**> キャリア状態 Ant#A C4 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_SLP_A_C1				0x00000020	/**> キャリア状態 Ant#A C5 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_SLP_A_C2				0x00000040	/**> キャリア状態 Ant#A C6 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_SLP_A_C3				0x00000080	/**> キャリア状態 Ant#A C7 USE			*//*	5G-商用DU	*/
#define	D_DU_SVCTL_BIT_USE_A_ALL_SB6		( D_DU_SVCTL_BIT_USE_A_C0 )
#define	D_DU_SVCTL_BIT_USE_A_ALL_MMW		( D_DU_SVCTL_BIT_USE_A_C0 | D_DU_SVCTL_BIT_USE_A_C1 | D_DU_SVCTL_BIT_USE_A_C2 | D_DU_SVCTL_BIT_USE_A_C3 )
#define	D_DU_SVCTL_BIT_SLP_A_ALL_SB6		( D_DU_SVCTL_BIT_SLP_A_C0 )
#define	D_DU_SVCTL_BIT_SLP_A_ALL_MMW		( D_DU_SVCTL_BIT_SLP_A_C0 | D_DU_SVCTL_BIT_SLP_A_C1 | D_DU_SVCTL_BIT_SLP_A_C2 | D_DU_SVCTL_BIT_SLP_A_C3 )
#define	D_DU_SVCTL_BIT_USESLP_A_ALL_SB6		( D_DU_SVCTL_BIT_USE_A_ALL_SB6 | D_DU_SVCTL_BIT_SLP_A_ALL_SB6 )
#define	D_DU_SVCTL_BIT_USESLP_A_ALL_MMW		( D_DU_SVCTL_BIT_USE_A_ALL_MMW | D_DU_SVCTL_BIT_SLP_A_ALL_MMW )

#define D_DU_SVCTL_BIT_USE_B_C0				0x00000100	/**> キャリア状態 Ant#B C0 USE			*/
#define D_DU_SVCTL_BIT_USE_B_C1				0x00000200	/**> キャリア状態 Ant#B C1 USE			*/
#define D_DU_SVCTL_BIT_USE_B_C2				0x00000400	/**> キャリア状態 Ant#B C2 USE			*/
#define D_DU_SVCTL_BIT_USE_B_C3				0x00000800	/**> キャリア状態 Ant#B C3 USE			*/
#define D_DU_SVCTL_BIT_SLP_B_C0				0x00001000	/**> キャリア状態 Ant#B C4 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_SLP_B_C1				0x00002000	/**> キャリア状態 Ant#B C5 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_SLP_B_C2				0x00004000	/**> キャリア状態 Ant#B C6 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_SLP_B_C3				0x00008000	/**> キャリア状態 Ant#B C7 USE			*//*	5G-商用DU	*/
#define	D_DU_SVCTL_BIT_USE_B_ALL_SB6		( D_DU_SVCTL_BIT_USE_B_C0 )
#define	D_DU_SVCTL_BIT_USE_B_ALL_MMW		( D_DU_SVCTL_BIT_USE_B_C0 | D_DU_SVCTL_BIT_USE_B_C1 | D_DU_SVCTL_BIT_USE_B_C2 | D_DU_SVCTL_BIT_USE_B_C3 )
#define	D_DU_SVCTL_BIT_SLP_B_ALL_SB6		( D_DU_SVCTL_BIT_SLP_B_C0 )
#define	D_DU_SVCTL_BIT_SLP_B_ALL_MMW		( D_DU_SVCTL_BIT_SLP_B_C0 | D_DU_SVCTL_BIT_SLP_B_C1 | D_DU_SVCTL_BIT_SLP_B_C2 | D_DU_SVCTL_BIT_SLP_B_C3 )
#define	D_DU_SVCTL_BIT_USESLP_B_ALL_SB6		( D_DU_SVCTL_BIT_USE_B_ALL_SB6 | D_DU_SVCTL_BIT_SLP_B_ALL_SB6 )
#define	D_DU_SVCTL_BIT_USESLP_B_ALL_MMW		( D_DU_SVCTL_BIT_USE_B_ALL_MMW | D_DU_SVCTL_BIT_SLP_B_ALL_MMW )


#define D_DU_SVCTL_BIT_USE_C_C0				0x00010000	/**> キャリア状態 Ant#C C0 USE			*/
#define D_DU_SVCTL_BIT_USE_C_C1				0x00020000	/**> キャリア状態 Ant#C C1 USE			*/
#define D_DU_SVCTL_BIT_SLP_C_C0				0x00100000	/**> キャリア状態 Ant#C C0 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_SLP_C_C1				0x00200000	/**> キャリア状態 Ant#C C1 USE			*//*	5G-商用DU	*/
#define	D_DU_SVCTL_BIT_USE_C_ALL_SB6		( D_DU_SVCTL_BIT_USE_C_C0 )
#define	D_DU_SVCTL_BIT_SLP_C_ALL_SB6		( D_DU_SVCTL_BIT_SLP_C_C0 )
#define	D_DU_SVCTL_BIT_USESLP_C_ALL_SB6		( D_DU_SVCTL_BIT_USE_C_ALL_SB6 | D_DU_SVCTL_BIT_SLP_C_ALL_SB6 )
#define	D_DU_SVCTL_BIT_USESLP_CD_ALL_MMW	( 0x00000000 )

#define D_DU_SVCTL_BIT_USE_D_C0				0x01000000	/**> キャリア状態 Ant#D C0 USE			*/
#define D_DU_SVCTL_BIT_USE_D_C1				0x02000000	/**> キャリア状態 Ant#D C1 USE			*/
#define D_DU_SVCTL_BIT_SLP_D_C0				0x10000000	/**> キャリア状態 Ant#C C0 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_SLP_D_C1				0x20000000	/**> キャリア状態 Ant#C C1 USE			*//*	5G-商用DU	*/
#define	D_DU_SVCTL_BIT_USE_D_ALL_SB6		( D_DU_SVCTL_BIT_USE_D_C0 )
#define	D_DU_SVCTL_BIT_SLP_D_ALL_SB6		( D_DU_SVCTL_BIT_SLP_D_C0 )
#define	D_DU_SVCTL_BIT_USESLP_D_ALL_SB6		( D_DU_SVCTL_BIT_USE_D_ALL_SB6 | D_DU_SVCTL_BIT_SLP_D_ALL_SB6 )

#define	D_DU_SVCTL_BIT_USE_ALL_SUB6			( D_DU_SVCTL_BIT_USE_A_ALL_SB6 | D_DU_SVCTL_BIT_USE_B_ALL_SB6 | D_DU_SVCTL_BIT_USE_C_ALL_SB6 | D_DU_SVCTL_BIT_USE_D_ALL_SB6)
#define	D_DU_SVCTL_BIT_USE_ALL_MMW			( D_DU_SVCTL_BIT_USE_A_ALL_MMW | D_DU_SVCTL_BIT_USE_B_ALL_MMW )


/*	SVCTL用(BIT ON/OFF操作)	*/
#define D_DU_SVCTL_ON_USE_C0				0x00000001	/**> キャリア状態 C0 USE			*/
#define D_DU_SVCTL_ON_USE_C1				0x00000002	/**> キャリア状態 C1 USE			*/
#define D_DU_SVCTL_ON_USE_C2				0x00000004	/**> キャリア状態 C2 USE			*/
#define D_DU_SVCTL_ON_USE_C3				0x00000008	/**> キャリア状態 C3 USE			*/
#define D_DU_SVCTL_ON_SLP_C0				0x00000010	/**> キャリア状態 C4 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_SLP_C1				0x00000020	/**> キャリア状態 C5 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_SLP_C2				0x00000040	/**> キャリア状態 C6 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_SLP_C3				0x00000080	/**> キャリア状態 C7 USE			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_USE_NUM_SB6			2
#define D_DU_SVCTL_ON_USE_NUM_MMW			8
#define D_DU_SVCTL_ON_USE_A_SFT				0
#define D_DU_SVCTL_ON_USE_B_SFT				8
#define D_DU_SVCTL_ON_USE_C_SFT				16
#define D_DU_SVCTL_ON_USE_D_SFT				24

/***** SV3INF	*****//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_FLV_MINOR			0x00000001	/**> Fault Level = Minor				*/
#define D_DU_SVCTL_BIT_FLV_MAJOR			0x00000002	/**> Fault Level = Major				*/
#define D_DU_SVCTL_BIT_FLV_CRITICAL			0x00000004	/**> Fault Level = Critical				*/
#define D_DU_SVCTL_BIT_FLV_ALL				( D_DU_SVCTL_BIT_FLV_MINOR | D_DU_SVCTL_BIT_FLV_MAJOR | D_DU_SVCTL_BIT_FLV_CRITICAL )

/***** SV4INF	*****/
#define D_DU_SVCTL_BIT_OCCUR_FID_01			0x00000001	/**> Fault ID = 0x01  発生状況			*/
#define D_DU_SVCTL_BIT_OCCUR_FID_02			0x00000002	/**> Fault ID = 0x02  発生状況			*/
#define D_DU_SVCTL_BIT_OCCUR_FID_03			0x00000004	/**> Fault ID = 0x03  発生状況			*/
#define D_DU_SVCTL_BIT_OCCUR_FID_04			0x00000008	/**> Fault ID = 0x04  発生状況			*/
#define D_DU_SVCTL_BIT_OCCUR_FID_05			0x00000010	/**> Fault ID = 0x05  発生状況			*/
#define D_DU_SVCTL_BIT_OCCUR_FID_06			0x00000020	/**> Fault ID = 0x06  発生状況			*/
#define D_DU_SVCTL_BIT_OCCUR_FID_07			0x00000040	/**> Fault ID = 0x07  発生状況			*/
#define D_DU_SVCTL_BIT_OCCUR_FID_08			0x00000080	/**> Fault ID = 0x08  発生状況			*/

#define D_DU_SVCTL_BIT_OCCUR_FID_10			0x00000100	/**> Fault ID = 0x10  発生状況			*/
#define D_DU_SVCTL_BIT_OCCUR_FID_11			0x00000200	/**> Fault ID = 0x11  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_12			0x00000400	/**> Fault ID = 0x12  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_20			D_DU_SVCTL_BIT_OCCUR_FID_11	/**> Fault ID = 0x20  発生状況			*//*	5G-評価DU	*/

#define D_DU_SVCTL_BIT_OCCUR_FID_30			0x00001000	/**> Fault ID = 0x30  発生状況			*/
#define D_DU_SVCTL_BIT_OCCUR_FID_31			0x00002000	/**> Fault ID = 0x31  発生状況			*/

#define D_DU_SVCTL_BIT_OCCUR_FID_40			0x00010000	/**> Fault ID = 0x40  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_41			0x00020000	/**> Fault ID = 0x41  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_42			0x00040000	/**> Fault ID = 0x42  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_43			0x00080000	/**> Fault ID = 0x43  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_44			0x00100000	/**> Fault ID = 0x44  発生状況			*//*	5G-商用DU	*/

#define D_DU_SVCTL_BIT_OCCUR_FID_50			0x01000000	/**> Fault ID = 0x50  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_51			0x04000000	/**> Fault ID = 0x51  発生状況			*//*	5G-商用DU	*/

#define D_DU_SVCTL_BIT_OCCUR_FID_F0			0x10000000	/**> Fault ID = 0xF0  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_F1			0x20000000	/**> Fault ID = 0xF1  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_F2			0x40000000	/**> Fault ID = 0xF2  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_BIT_OCCUR_FID_FF			0x80000000	/**> Fault ID = 0xFF  発生状況			*//*	5G-商用DU	*/

#define	D_DU_SVCTL_BIT_OCCUR_FID_ALL		(	D_DU_SVCTL_BIT_OCCUR_FID_01 |\
												D_DU_SVCTL_BIT_OCCUR_FID_02 |\
												D_DU_SVCTL_BIT_OCCUR_FID_03 |\
												D_DU_SVCTL_BIT_OCCUR_FID_04 |\
												D_DU_SVCTL_BIT_OCCUR_FID_05 |\
												D_DU_SVCTL_BIT_OCCUR_FID_06 |\
												D_DU_SVCTL_BIT_OCCUR_FID_07 |\
												D_DU_SVCTL_BIT_OCCUR_FID_08 |\
												D_DU_SVCTL_BIT_OCCUR_FID_10 |\
												D_DU_SVCTL_BIT_OCCUR_FID_11 |\
												D_DU_SVCTL_BIT_OCCUR_FID_12 |\
												D_DU_SVCTL_BIT_OCCUR_FID_30 |\
												D_DU_SVCTL_BIT_OCCUR_FID_31 |\
												D_DU_SVCTL_BIT_OCCUR_FID_40 |\
												D_DU_SVCTL_BIT_OCCUR_FID_41 |\
												D_DU_SVCTL_BIT_OCCUR_FID_42 |\
												D_DU_SVCTL_BIT_OCCUR_FID_43 |\
												D_DU_SVCTL_BIT_OCCUR_FID_44 |\
												D_DU_SVCTL_BIT_OCCUR_FID_50 |\
												D_DU_SVCTL_BIT_OCCUR_FID_51 |\
												D_DU_SVCTL_BIT_OCCUR_FID_F0 |\
												D_DU_SVCTL_BIT_OCCUR_FID_F1 |\
												D_DU_SVCTL_BIT_OCCUR_FID_F2 |\
												D_DU_SVCTL_BIT_OCCUR_FID_FF )

/*	SVCTL用(BIT ON/OFF操作)	*/
#define D_DU_SVCTL_ON_FLV_MINOR				D_DU_SVCTL_BIT_FLV_MINOR	/**> Fault Level = Minor				*/
#define D_DU_SVCTL_ON_FLV_MAJOR				D_DU_SVCTL_BIT_FLV_MAJOR	/**> Fault Level = Major				*/
#define D_DU_SVCTL_ON_FLV_CRITICAL			D_DU_SVCTL_BIT_FLV_CRITICAL	/**> Fault Level = Critical				*/
#define D_DU_SVCTL_ON_FLV_SFT				0

/* SV Bus control mapping Table用 INDEX */
enum
{
	E_DU_SVCTL_FLVNO_MINOR = 0,							/**> (00)Fault Level=Minor				*/
	E_DU_SVCTL_FLVNO_MAJOR,								/**> (01)Fault Level=Major				*/
	E_DU_SVCTL_FLVNO_CRITICAL,							/**> (02)Fault Level=Critical			*/
	E_DU_SVCTL_FLVNO_MAX								/**> (03)Fault Level Number				*/
};

#define D_DU_SVCTL_ON_OCCUR_FID_01			D_DU_SVCTL_BIT_OCCUR_FID_01	/**> Fault ID = 0x01  発生状況			*/
#define D_DU_SVCTL_ON_OCCUR_FID_02			D_DU_SVCTL_BIT_OCCUR_FID_02	/**> Fault ID = 0x02  発生状況			*/
#define D_DU_SVCTL_ON_OCCUR_FID_03			D_DU_SVCTL_BIT_OCCUR_FID_03	/**> Fault ID = 0x03  発生状況			*/
#define D_DU_SVCTL_ON_OCCUR_FID_04			D_DU_SVCTL_BIT_OCCUR_FID_04	/**> Fault ID = 0x04  発生状況			*/
#define D_DU_SVCTL_ON_OCCUR_FID_05			D_DU_SVCTL_BIT_OCCUR_FID_05	/**> Fault ID = 0x05  発生状況			*/
#define D_DU_SVCTL_ON_OCCUR_FID_06			D_DU_SVCTL_BIT_OCCUR_FID_06	/**> Fault ID = 0x06  発生状況			*/
#define D_DU_SVCTL_ON_OCCUR_FID_07			D_DU_SVCTL_BIT_OCCUR_FID_07	/**> Fault ID = 0x07  発生状況			*/
#define D_DU_SVCTL_ON_OCCUR_FID_08			D_DU_SVCTL_BIT_OCCUR_FID_08	/**> Fault ID = 0x08  発生状況			*/

#define D_DU_SVCTL_ON_OCCUR_FID_10			D_DU_SVCTL_BIT_OCCUR_FID_10	/**> Fault ID = 0x10  発生状況			*/
#define D_DU_SVCTL_ON_OCCUR_FID_20			D_DU_SVCTL_BIT_OCCUR_FID_20	/**> Fault ID = 0x20  発生状況			*//*	5G-評価DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_11			D_DU_SVCTL_BIT_OCCUR_FID_11	/**> Fault ID = 0x11  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_12			D_DU_SVCTL_BIT_OCCUR_FID_12	/**> Fault ID = 0x12  発生状況			*//*	5G-商用DU	*/

#define D_DU_SVCTL_ON_OCCUR_FID_30			D_DU_SVCTL_BIT_OCCUR_FID_30	/**> Fault ID = 0x30  発生状況			*/
#define D_DU_SVCTL_ON_OCCUR_FID_31			D_DU_SVCTL_BIT_OCCUR_FID_31	/**> Fault ID = 0x31  発生状況			*/

#define D_DU_SVCTL_ON_OCCUR_FID_40			D_DU_SVCTL_BIT_OCCUR_FID_40	/**> Fault ID = 0x40  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_41			D_DU_SVCTL_BIT_OCCUR_FID_41	/**> Fault ID = 0x41  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_42			D_DU_SVCTL_BIT_OCCUR_FID_42	/**> Fault ID = 0x42  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_43			D_DU_SVCTL_BIT_OCCUR_FID_43	/**> Fault ID = 0x43  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_44			D_DU_SVCTL_BIT_OCCUR_FID_44	/**> Fault ID = 0x44  発生状況			*//*	5G-商用DU	*/

#define D_DU_SVCTL_ON_OCCUR_FID_50			D_DU_SVCTL_BIT_OCCUR_FID_50	/**> Fault ID = 0x50  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_51			D_DU_SVCTL_BIT_OCCUR_FID_51	/**> Fault ID = 0x50  発生状況			*//*	5G-商用DU	*/

#define D_DU_SVCTL_ON_OCCUR_FID_F0			D_DU_SVCTL_BIT_OCCUR_FID_F0	/**> Fault ID = 0xF0  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_F1			D_DU_SVCTL_BIT_OCCUR_FID_F1	/**> Fault ID = 0xF1  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_F2			D_DU_SVCTL_BIT_OCCUR_FID_F2	/**> Fault ID = 0xF2  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_FF			D_DU_SVCTL_BIT_OCCUR_FID_FF	/**> Fault ID = 0xFF  発生状況			*//*	5G-商用DU	*/
#define D_DU_SVCTL_ON_OCCUR_FID_SFT			0
/* SV Bus control mapping Table用 INDEX */
enum
{
	E_DU_SVCTL_OCCUR_FIDNO_01 = 0,						/**> (00)Fault ID=0x01 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_02,							/**> (01)Fault ID=0x02 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_03,							/**> (02)Fault ID=0x03 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_04,							/**> (03)Fault ID=0x04 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_05,							/**> (04)Fault ID=0x05 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_06,							/**> (05)Fault ID=0x06 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_07,							/**> (06)Fault ID=0x07 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_08,							/**> (07)Fault ID=0x08 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_10,							/**> (08)Fault ID=0x10 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_11,							/**> (09)Fault ID=0x11 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_12,							/**> (10)Fault ID=0x12 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_30,							/**> (11)Fault ID=0x30 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_31,							/**> (12)Fault ID=0x31 Index			*/
	E_DU_SVCTL_OCCUR_FIDNO_40,							/**> (13)Fault ID=0x40 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_41,							/**> (14)Fault ID=0x41 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_42,							/**> (15)Fault ID=0x42 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_43,							/**> (16)Fault ID=0x43 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_44,							/**> (17)Fault ID=0x44 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_50,							/**> (18)Fault ID=0x50 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_F0,							/**> (19)Fault ID=0xF0 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_F1,							/**> (20)Fault ID=0xF1 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_F2,							/**> (21)Fault ID=0xF2 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_FF,							/**> (22)Fault ID=0xFF Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_51,							/**> (23)Fault ID=0x51 Index			*//*	5G-商用DU	*/
	E_DU_SVCTL_OCCUR_FIDNO_MAX							/**> (24)Max Index Number				*/
};

#define E_DU_SVCTL_OCCUR_FIDNO_20			E_DU_SVCTL_OCCUR_FIDNO_11	/**> (09)Fault ID=0x20 Index			*//*	5G-評価DU	*/

/***** SV6INF	*****/
#define D_DU_SVCTL_L1_DISCONNECT_1			D_DU_ALMBIT_L1_LDOWN_FH1	/**> L1断 (PORT#1)						*/
#define D_DU_SVCTL_SFP_UNMOUNT_1			D_DU_ALMBIT_SFP_UNMOUNT_FH1	/**> SFP unmount (PORT#1)				*/

#define D_DU_SVCTL_PORT1_DISCONNECT			(D_DU_ALMBIT_L1_LDOWN_FH1 | D_DU_ALMBIT_SFP_UNMOUNT_FH1)
/***** SV16INF	*****/
#define D_DU_SVCTL_WAVE_STOP_ALL			0x00000001	/**> all ANT停波 for SyncE				*/
#define D_DU_SVCTL_WAVE_STOP_ALL_PTP		0x00000002	/**> all ANT停波 for PTP				*/
#define D_DU_SVCTL_WAVE_STOP_ALL_PTP_UNLOCK	0x00000004	/**> all ANT停波 for PTP UNLOCK			*/

#define	D_DU_SVCTL_WAVE_STOP				(	D_DU_SVCTL_WAVE_STOP_ALL |\
												D_DU_SVCTL_WAVE_STOP_ALL_PTP |\
												D_DU_SVCTL_WAVE_STOP_ALL_PTP_UNLOCK )

#define D_DU_SVCTL_SYNCE_STATE_DISABLE		0x00010000	/**> SyncE Status = Disable (Bit off用)	*/
#define D_DU_SVCTL_SYNCE_STATE_PARENT		0x00020000	/**> SyncE Status = Parent				*/
#define D_DU_SVCTL_SYNCE_STATE_OK			0x00040000	/**> SyncE Status = OK					*/
#define D_DU_SVCTL_SYNCE_STATE_NOK			0x00080000	/**> SyncE Status = NOK					*/
#define	D_DU_SVCTL_SYNCE_STATE_ALL			(	D_DU_SVCTL_SYNCE_STATE_DISABLE |\
												D_DU_SVCTL_SYNCE_STATE_PARENT |\
												D_DU_SVCTL_SYNCE_STATE_OK |\
												D_DU_SVCTL_SYNCE_STATE_NOK )

#define D_DU_SVCTL_SYNCE_STATE_LOCKED		0x01000000	/**> SyncE Locked Status = Locked		*/
#define D_DU_SVCTL_PTP_STATE_LOCKED			0x02000000	/**> PTP Locked Status = Locked			*/
#define D_DU_SVCTL_SPLAN_STS_ALL			( D_DU_SVCTL_SYNCE_STATE_LOCKED | D_DU_SVCTL_PTP_STATE_LOCKED )


#define D_DU_SVCTL_SYNC_STATE_FREERUN		0x10000000	/**> Sync Locked Status = Freerun		*/
#define D_DU_SVCTL_SYNC_STATE_LOCKED		0x20000000	/**> Sync Locked Status = Locked		*/
#define D_DU_SVCTL_SYNC_STATE_HOLDOVER		0x40000000	/**> Sync Locked Status = Holdover		*/
#define	D_DU_SVCTL_SYNC_STATE_ALL			(	D_DU_SVCTL_SYNC_STATE_FREERUN |\
												D_DU_SVCTL_SYNC_STATE_LOCKED |\
												D_DU_SVCTL_SYNC_STATE_HOLDOVER )


/*	SVCTL用(BIT ON/OFF操作)	*/
#define D_DU_SVCTL_ON_WAVE_STOP_ALL			D_DU_SVCTL_WAVE_STOP_ALL		/**> all ANT停波 for SyncE				*/
#define D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP		D_DU_SVCTL_WAVE_STOP_ALL_PTP	/**> all ANT停波 for PTP				*/
#define D_DU_SVCTL_ON_WAVE_STOP_ALL_PTP_UNLOCK		D_DU_SVCTL_WAVE_STOP_ALL_PTP_UNLOCK	/**> all ANT停波 for PTP Lock	*/
#define D_DU_SVCTL_ON_WAVE_STOP_NUM			3
#define D_DU_SVCTL_ON_WAVE_STOP_SFT			0

#define D_DU_SVCTL_ON_SYNCE_STATE_DISABLE	0x00000001	/**> SyncE Status = Disable (Bit off用)	*/
#define D_DU_SVCTL_ON_SYNCE_STATE_PARENT	0x00000002	/**> SyncE Status = Parent				*/
#define D_DU_SVCTL_ON_SYNCE_STATE_OK		0x00000004	/**> SyncE Status = OK					*/
#define D_DU_SVCTL_ON_SYNCE_STATE_NOK		0x00000008	/**> SyncE Status = NOK					*/
#define D_DU_SVCTL_ON_SYNCE_STATE_NUM		4
#define D_DU_SVCTL_ON_SYNCE_STATE_SFT		16

#define D_DU_SVCTL_ON_SYNCE_STATE_LOCKED	0x00000001	/**> SyncE Locked Status = Locked		*/
#define D_DU_SVCTL_ON_PTP_STATE_LOCKED		0x00000002	/**> PTP Locked Status = Locked			*/
#define D_DU_SVCTL_ON_SPLAN_NUM				2
#define D_DU_SVCTL_ON_SPLAN_SFT				24

#define D_DU_SVCTL_ON_SYNC_STATE_FREERUN	0x00000001	/**> Sync Locked Status = Freerun		*/
#define D_DU_SVCTL_ON_SYNC_STATE_LOCKED		0x00000002	/**> Sync Locked Status = Locked		*/
#define D_DU_SVCTL_ON_SYNC_STATE_HOLDOVER	0x00000004	/**> Sync Locked Status = Holdover		*/
#define D_DU_SVCTL_ON_SYNC_STATE_NUM		3
#define D_DU_SVCTL_ON_SYNC_STATE_SFT		28

#define D_DU_SVCTL_FL_DWNLD					0x00000001	/**> Downdoading file					*/
#define D_DU_SVCTL_ON_FL_DWNLD				0x00000001	/**> Downdoading file bit ON			*/
#define D_DU_SVCTL_FL_DWNLD_ALL				( D_DU_SVCTL_FL_DWNLD )
#define D_DU_SVCTL_FL_DWNLD_NUM				1
#define D_DU_SVCTL_FL_DWNLD_SFT				0

#define D_DU_SVCTL_ON_OFF_ALL				0x00000000

#define D_DU_SVCTL_ENDCODE					0xFFFFFFFF  /**> 終了コード							*/
#define D_DU_SVCTL_OFF						         0  /**> bit OFF Operation					*/
#define D_DU_SVCTL_ON						         1  /**> bit ON  Operation					*/
#define D_DU_SVCTL_CHG						         2  /**> changing value  Operation			*/
#define D_DU_SVCTL_CNT_MUX					         3  /**> cntrl max							*/
#define D_DU_SVCTL_SHIFT_INV					    32  /**> shift invalid						*/
#define D_DU_SVCTL_TYP_STA						     0  /**> state type							*/
#define D_DU_SVCTL_TYP_FLT						     1  /**> fault type							*/
#define D_DU_SVCTL_TYP_OTH						     2  /**> other type							*/
#define D_DU_SVINF_NUM								16	/**< SVバス情報数						*/
#define D_DU_SVINF_ALL								 0	/**< ALL SVバス情報指定					*/
/* @} */


/****************************************************************************/
/* 装置種別関連マクロ/define												*/
/****************************************************************************/
/*!
 * @name DU種別
 * @note DU種別定義
 * @{
 */
enum{
	E_RRH_REKIND_LLS = 0,
	E_RRH_REKIND_NUM = 1
};

/*!
 * @name 装置情報種別
 * @note 装置情報種別定義
 * @{
 */
enum{
	E_RRH_RRHKIND_5GDU = 0x10,							/* 0x10 : 5G-LLS (proto)				*/
	E_RRH_RRHKIND_5GDU_LLS_SUB6 = 0x13,					/* 0x13 : 5G-DU商用(sub6)				*/
	E_RRH_RRHKIND_5GDU_LLS_MMW,							/* 0x14 : 5G-DU商用(mmW)				*/
};
#define	D_RRH_RRHKIND_NUM					3			/* 装置種別数							*/
/* @} */

/*!
 * @name 対応周波数関連
 * @note 対応周波数関連定義
 * @{
 */
enum{
	E_RRH_RRHKIND_FREQ_45G = 0x10,						/* 0x10 : 4.8G					(sub6)	*/
	E_RRH_RRHKIND_FREQ_37G,								/* 0x11 : 3.7G					(sub6)	*/
	E_RRH_RRHKIND_FREQ_28G,								/* 0x12 : 28G					(mmW)	*/
};
#define	D_RRH_RRHKIND_FREQ_NUM				3			/* 対応周波数							*/
/* @} */

/*!
 * @name 装置情報/対応周波数取得マクロ
 * @note 装置情報/対応周波数取得マクロ定義
 * @{
 */
#define D_DU_RRHKIND_FREQ_MASK				0x00FF0000	/* 装置種別 : 対応周波数取得マスク値	*/
#define D_DU_RRHKIND_FREQ_SHFT				16			/* 装置種別 : 対応周波数取得ソフト値	*/
#define D_DU_RRHKIND_KIND_MASK				0x000000FF	/* 装置種別 : 装置種別取得マスク値		*/

#define M_DU_RRHKIND_FREQ_GET(val)		(UINT)( (val & D_DU_RRHKIND_FREQ_MASK) >> D_DU_RRHKIND_FREQ_SHFT )
#define M_DU_RRHKIND_KIND_GET(val)		(UINT)( val & D_DU_RRHKIND_KIND_MASK )
/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define     D_RRH_ALMCD_OS                  0x0001          /**< OS要因 */
#define     D_RRH_ALMCD_EXC                 0x0002          /**< マシンチェック例外 */
#define     D_RRH_ALMCD_DSI                 0x0003          /**< DSI例外 */
#define     D_RRH_ALMCD_ISI                 0x0004          /**< ISI例外 */
#define     D_RRH_ALMCD_ARAI                0x0005          /**< アライメント例外 */
#define     D_RRH_ALMCD_PRO                 0x0006          /**< プログラム例外 */
#define     D_RRH_ALMCD_TLB                 0x0007          /**< 命令TLBミス例外 */
#define     D_RRH_ALMCD_MIS                 0x0008          /**< Data TLB Miss例外 */
#define     D_RRH_ALMCD_INS                 0x0009          /**< Instruction TLB Miss例外 */
#define     D_RRH_ALMCD_FNC                 0x000A          /**< 関数戻り値異常 */
#define     D_RRH_ALMCD_TSK                 0x000B          /**< タスク起動異常 */
#define     D_RRH_ALMCD_BGET                0x000C          /**< バッファ獲得異常 */
#define     D_RRH_ALMCD_BFRE                0x000D          /**< バッファ解放異常 */
#define     D_RRH_ALMCD_TSTA                0x000E          /**< タイマ起動異常 */
#define     D_RRH_ALMCD_TEND                0x000F          /**< タイマ停止異常 */
#define     D_RRH_ALMCD_SGET                0x0010          /**< セマフォ獲得異常 */
#define     D_RRH_ALMCD_SFRE                0x0011          /**< セマフォ解放異常 */
#define     D_RRH_ALMCD_ANOT                0x0012          /**< その他の異常 */
#define     D_RRH_ALMCD_ARNG                0x0013          /**< 論理異常 */
#define     D_RRH_ALMCD_STNG                0x0014          /**< 状態異常 */
#define     D_RRH_ALMCD_TKST                0x0015          /**< タスク状態異常 */
#define     D_RRH_ALMCD_CSIF                0x0016          /**< カード状態異常 */
#define     D_RRH_ALMCD_TIF                 0x0017          /**< タスク間メッセージ異常 */
#define     D_RRH_ALMCD_UBUF                0x0018          /**< ユーザ管理バッファ枯渇 */
#define     D_RRH_ALMCD_DNG                 0x0019          /**< データ異常 */
#define     D_RRH_ALMCD_RNG                 0x001A          /**< ルート異常 */
#define     D_RRH_ALMCD_FNG                 0x001B          /**< 関数戻り値異常 */
#define     D_RRH_ALMCD_ANG                 0x001C          /**< その他の異常 */
#define     D_RRH_ALMCD_CIF                 0x001D          /**< カード間IF異常 */
#define     D_RRH_ALMCD_SUM                 0x001E          /**< 受信データチェックサム異常 */
#define     D_RRH_ALMCD_ANNG                0x001F          /**< その他の異常 */
#define     D_RRH_ALMCD_FWNG                0x0020          /**< FLASH-ROM書込失敗 */
#define     D_RRH_ALMCD_EENG                0x0021          /**< EEPROMアクセス失敗 */
#define     D_RRH_ALMCD_BUSNG               0x0022          /**< シリアルバス制御失敗 */
#define     D_RRH_ALMCD_ETHNG               0x0023          /**< ethernet制御失敗 */
#define     D_RRH_ALMCD_ANNG2               0x0024          /**< その他の異常 */
#define     D_RRH_ALMCD_FPGACNF             0x0024          /**< FPGA CONFIG NG(PPC用SLCと同様に、その他の異常と同じ値を指定) */
#define     D_RRH_ALMCD_DIA1                0x0025          /**< 一次診断NG1 */
#define     D_RRH_ALMCD_DIA2                0x0026          /**< 一次診断NG2 */
#define     D_RRH_ALMCD_DIA3                0x0027          /**< 一次診断NG3 */
#define     D_RRH_ALMCD_DIA4                0x0028          /**< 一次診断NG4 */
#define     D_RRH_ALMCD_DIA5                0x0029          /**< 一次診断NG5 */
#define     D_RRH_ALMCD_WDT                 0x002A          /**< WDT T.O */
#define     D_RRH_ALMCD_CSTP                0x002B          /**< Check STOP */
#define     D_RRH_ALMCD_REFLASHNG           0x002B          /**< 配下RE用FLASH-ROM書込失敗 */
#define     D_RRH_ALMCD_MISMATCH_PARM       0x002C          /**< パラメータ不一致 */
#define     D_RRH_ALMCD_DUM2                0x002D          /**< 予備 */
#define     D_RRH_ALMCD_DUM3                0x002E          /**< 予備 */
#define     D_RRH_ALMCD_DUM4                0x002F          /**< 予備 */
#define     D_RRH_ALMCD_DUM5                0x0030          /**< 予備 */
#define 	D_RRH_ALMCD_REMOTEALM			0x00AF
#define     D_RRH_ALMCD_PLLNG               0x00C0          /**< PLL UNLOCK */
#define     D_RRH_ALMCD_FPGANG1             0x00C1          /**< FPGA-PLL-UNLOCK1 */
#define     D_RRH_ALMCD_FPGANG2             0x00C2          /**< FPGA-PLL-UNLOCK2 */
#define     D_RRH_ALMCD_FPGANG3             0x00C3          /**< FPGA-PLL-UNLOCK3 */
#define     D_RRH_ALMCD_DUM6                0x00C4          /**< 予備 */
#define     D_RRH_ALMCD_DUM7                0x00C5          /**< 予備 */
#define     D_RRH_ALMCD_RFIC1               0x00C6          /**< RFIC ALM_1 */
#define     D_RRH_ALMCD_RFIC2               0x00C7          /**< RFIC ALM_2 */
#define     D_RRH_ALMCD_RFIC3               0x00C8          /**< RFIC ALM_3 */
#define     D_RRH_ALMCD_RFIC4               0x00C9          /**< RFIC ALM_4 */
#define     D_RRH_ALMCD_RFICIF1             0x00CA          /**< RFIC IF ALM_1 */
#define     D_RRH_ALMCD_RFICIF2             0x00CB          /**< RFIC IF ALM_2 */
#define     D_RRH_ALMCD_RFICIF3             0x00CC          /**< RFIC IF ALM_3 */
#define     D_RRH_ALMCD_RFICIF4             0x00CD          /**< RFIC IF ALM_4 */
#define     D_RRH_ALMCD_FPGAIF1             0x00CE          /**< PCIe IP ERR : EMIO[30]   */
#define     D_RRH_ALMCD_FPGAIF2             0x00CF          /**< FPGA IF ALM_2 */
#define     D_RRH_ALMCD_CPLDIF1             0x00D0          /**< CPLD IF ALM_1 */
#define     D_RRH_ALMCD_DUM8                0x00D1          /**< 予備 */
#define     D_RRH_ALMCD_TEMP                0x00D2          /**< TEMP ALM */
#define     D_RRH_ALMCD_DUM9                0x00D3          /**< 予備 */
#define     D_RRH_ALMCD_DUM10               0x00D4          /**< 予備 */
#define     D_RRH_ALMCD_DUM11               0x00D5          /**< 予備 */
#define     D_RRH_ALMCD_PSALM               0x00D6          /**< PS ALM */
#define     D_RRH_ALMCD_DUM12               0x00D7          /**< 予備 */
#define     D_RRH_ALMCD_DUM13               0x00D8          /**< 予備 */
#define     D_RRH_ALMCD_DUM14               0x00D9          /**< 予備 */
#define     D_RRH_ALMCD_DUM15               0x00DA          /**< 予備 */
#define     D_RRH_ALMCD_DUM16               0x00DB          /**< 予備 */
#define     D_RRH_ALMCD_DUM17               0x00DC          /**< 予備 */
#define     D_RRH_ALMCD_DUM18               0x00DD          /**< 予備 */
#define     D_RRH_ALMCD_DUM19               0x00DE          /**< 予備 */
#define     D_RRH_ALMCD_DUM20               0x00DF          /**< 予備 */
#define     D_RRH_ALMCD_DUM21               0x00E0          /**< 予備 */
#define     D_RRH_ALMCD_FWPLLBR0            0x00E1          /**< FW PLL ALM(BR#0) */
#define     D_RRH_ALMCD_FBPLLBR0            0x00E2          /**< FB PLL ALM(BR#0) */
#define     D_RRH_ALMCD_RXPLLBR0            0x00E3          /**< RX PLL ALM(BR#0) */
#define     D_RRH_ALMCD_RFIC1BR0            0x00E4          /**< RFIC ALM1(BR#0) */
#define     D_RRH_ALMCD_RFIC2BR0            0x00E5          /**< RFIC ALM2(BR#0) */
#define     D_RRH_ALMCD_FWJESDBR0           0x00E6          /**< FW JESD ALM(BR#0) */
#define     D_RRH_ALMCD_FBJESDBR0           0x00E7          /**< FB JESD ALM(BR#0) */
#define     D_RRH_ALMCD_RXJESDBR0           0x00E8          /**< RX JESD ALM(BR#0) */
#define     D_RRH_ALMCD_DUM22               0x00E9          /**< 予備 */
#define     D_RRH_ALMCD_TXGABR0             0x00EA          /**< TX GAIN ALM(BR#0) */
#define     D_RRH_ALMCD_TOVEBR0             0x00EB          /**< TOVER ALM(BR#0) */
#define     D_RRH_ALMCD_RXGABR0             0x00EC          /**< RX GAIN ALM(BR#0) */
#define     D_RRH_ALMCD_TDDSW1BR0           0x00ED          /**< TDD SW ALM1(BR#0) */
#define     D_RRH_ALMCD_TDDSW2BR0           0x00EE          /**< TDD SW ALM2(BR#0) */
#define     D_RRH_ALMCD_TDDSW3BR0           0x00EF          /**< TDD SW ALM3(BR#0) */
#define     D_RRH_ALMCD_DUM23               0x00F0          /**< 予備 */
#define     D_RRH_ALMCD_FWPLLBR1            0x00F1          /**< FW PLL ALM(BR#1) */
#define     D_RRH_ALMCD_FBPLLBR1            0x00F2          /**< FB PLL ALM(BR#1) */
#define     D_RRH_ALMCD_RXPLLBR1            0x00F3          /**< RX PLL ALM(BR#1) */
#define     D_RRH_ALMCD_RFIC1BR1            0x00F4          /**< RFIC ALM1(BR#1) */
#define     D_RRH_ALMCD_RFIC2BR1            0x00F5          /**< RFIC ALM2(BR#1) */
#define     D_RRH_ALMCD_FWJESDBR1           0x00F6          /**< FW JESD ALM(BR#1) */
#define     D_RRH_ALMCD_FBJESDBR1           0x00F7          /**< FB JESD ALM(BR#1) */
#define     D_RRH_ALMCD_RXJESDBR1           0x00F8          /**< RX JESD ALM(BR#1) */
#define     D_RRH_ALMCD_DUM24               0x00F9          /**< 予備 */
#define     D_RRH_ALMCD_TXGABR1             0x00FA          /**< TX GAIN ALM(BR#1) */
#define     D_RRH_ALMCD_TOVEBR1             0x00FB          /**< TOVER ALM(BR#1) */
#define     D_RRH_ALMCD_RXGABR1             0x00FC          /**< RX GAIN ALM(BR#1) */
#define     D_RRH_ALMCD_TDDSW1BR1           0x00FD          /**< TDD SW ALM1(BR#1) */
#define     D_RRH_ALMCD_TDDSW2BR1           0x00FE          /**< TDD SW ALM2(BR#1) */
#define     D_RRH_ALMCD_TDDSW3BR1           0x00FF          /**< TDD SW ALM3(BR#1) */
/* @} */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define     D_RRH_ALMCD_DUM25               0x01E0          /**< 予備 */
#define     D_RRH_ALMCD_FWPLLBR2            0x01E1          /**< FW PLL ALM(BR#2) */
#define     D_RRH_ALMCD_FBPLLBR2            0x01E2          /**< FB PLL ALM(BR#2) */
#define     D_RRH_ALMCD_RXPLLBR2            0x01E3          /**< RX PLL ALM(BR#2) */
#define     D_RRH_ALMCD_RFIC1BR2            0x01E4          /**< RFIC ALM1(BR#2) */
#define     D_RRH_ALMCD_RFIC2BR2            0x01E5          /**< RFIC ALM2(BR#2) */
#define     D_RRH_ALMCD_FWJESDBR2           0x01E6          /**< FW JESD ALM(BR#2) */
#define     D_RRH_ALMCD_FBJESDBR2           0x01E7          /**< FB JESD ALM(BR#2) */
#define     D_RRH_ALMCD_RXJESDBR2           0x01E8          /**< RX JESD ALM(BR#2) */
#define     D_RRH_ALMCD_DUM26               0x01E9          /**< 予備 */
#define     D_RRH_ALMCD_TXGABR2             0x01EA          /**< TX GAIN ALM(BR#2) */
#define     D_RRH_ALMCD_TOVEBR2             0x01EB          /**< TOVER ALM(BR#2) */
#define     D_RRH_ALMCD_RXGABR2             0x01EC          /**< RX GAIN ALM(BR#2) */
#define     D_RRH_ALMCD_TDDSW1BR2           0x01ED          /**< TDD SW ALM1(BR#2) */
#define     D_RRH_ALMCD_TDDSW2BR2           0x01EE          /**< TDD SW ALM2(BR#2) */
#define     D_RRH_ALMCD_TDDSW3BR2           0x01EF          /**< TDD SW ALM3(BR#2) */
#define     D_RRH_ALMCD_DUM27               0x01F0          /**< 予備 */
#define     D_RRH_ALMCD_FWPLLBR3            0x01F1          /**< FW PLL ALM(BR#3) */
#define     D_RRH_ALMCD_FBPLLBR3            0x01F2          /**< FB PLL ALM(BR#3) */
#define     D_RRH_ALMCD_RXPLLBR3            0x01F3          /**< RX PLL ALM(BR#3) */
#define     D_RRH_ALMCD_RFIC1BR3            0x01F4          /**< RFIC ALM1(BR#3) */
#define     D_RRH_ALMCD_RFIC2BR3            0x01F5          /**< RFIC ALM2(BR#3) */
#define     D_RRH_ALMCD_FWJESDBR3           0x01F6          /**< FW JESD ALM(BR#3) */
#define     D_RRH_ALMCD_FBJESDBR3           0x01F7          /**< FB JESD ALM(BR#3) */
#define     D_RRH_ALMCD_RXJESDBR3           0x01F8          /**< RX JESD ALM(BR#3) */
#define     D_RRH_ALMCD_DUM28               0x01F9          /**< 予備 */
#define     D_RRH_ALMCD_TXGABR3             0x01FA          /**< TX GAIN ALM(BR#3) */
#define     D_RRH_ALMCD_TOVEBR3             0x01FB          /**< TOVER ALM(BR#3) */
#define     D_RRH_ALMCD_RXGABR3             0x01FC          /**< RX GAIN ALM(BR#3) */
#define     D_RRH_ALMCD_TDDSW1BR3           0x01FD          /**< TDD SW ALM1(BR#3) */
#define     D_RRH_ALMCD_TDDSW2BR3           0x01FE          /**< TDD SW ALM2(BR#3) */
#define     D_RRH_ALMCD_TDDSW3BR3           0x01FF          /**< TDD SW ALM3(BR#3) */
/* @} */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/** @name Alarm bit  define */
/* @{ */
#define D_RRH_ALMBIT_PLLUNLOCK	0x00010000		/**< PLL UNLOCK ALM	*/
#define D_RRH_ALMBIT_FPGAUNLK1	0x00020000		/**< FPGA-PLL-UNLOCK_1 ALM	*/
#define D_RRH_ALMBIT_FPGAUNLK2	0x00040000		/**< FPGA-PLL-UNLOCK_2 ALM	*/
#define D_RRH_ALMBIT_FPGAUNLK3	0x00080000		/**< FPGA-PLL-UNLOCK_3 ALM	*/
#define D_RRH_ALMBIT_CPLDIF1	0x01000000		/**< CPLD IF ALM	*/
#define D_RRH_ALMBIT_TEMP		0x04000000		/**< TEMP_ALM	*/
#define D_RRH_ALMBIT_FPGAIF1	0x08000000		/**< FPGA IF ALM	*/
#define D_RRH_ALMBIT_FPGAIF2	0x10000000		/**< FPGA IF ALM_2	*/
#define D_RRH_ALMBIT_PSALM		0x40000000		/**< PS ALM	*/
#define	D_RRH_ALMBIT_RFIC1BR0	0x00000010		/* RFIC ALM1(BR#0) */
#define	D_RRH_ALMBIT_RFIC2BR0	0x00000020		/* RFIC ALM2(BR#0) */
#define	D_RRH_ALMBIT_FWJESDBR0	0x00000040		/* FW JESD ALM(BR#0) */
#define	D_RRH_ALMBIT_FBJESDBR0	0x00000080		/* FB JESD ALM(BR#0) */
#define	D_RRH_ALMBIT_RXJESDBR0	0x00000100		/* RX JESD ALM(BR#0) */
#define	D_RRH_ALMBIT_TXGABR0	0x00000400		/* TX GAIN ALM(BR#0)	*/
#define	D_RRH_ALMBIT_TOVEBR0	0x00000800		/* TOVER ALM(BR#0)	*/
#define	D_RRH_ALMBIT_RXGABR0	0x00001000		/* RX GAIN ALM(BR#0)	*/
#define	D_RRH_ALMBIT_TDDSW1BR0	0x00002000		/* TDD SW ALM1(BR#0)	*/
#define	D_RRH_ALMBIT_TDDSW2BR0	0x00004000		/* TDD SW ALM2(BR#0)	*/
#define	D_RRH_ALMBIT_RFIC1BR1	0x00100000		/* RFIC ALM1(BR#1) */
#define	D_RRH_ALMBIT_RFIC2BR1	0x00200000		/* RFIC ALM2(BR#1) */
#define	D_RRH_ALMBIT_FWJESDBR1	0x00400000		/* FW JESD ALM(BR#1) */
#define	D_RRH_ALMBIT_FBJESDBR1	0x00800000		/* FB JESD ALM(BR#1) */
#define	D_RRH_ALMBIT_RXJESDBR1	0x01000000		/* RX JESD ALM(BR#1) */
#define	D_RRH_ALMBIT_TXGABR1	0x04000000		/* TX GAIN ALM(BR#1)	*/
#define	D_RRH_ALMBIT_TOVEBR1	0x08000000		/* TOVER ALM(BR#1)	*/
#define	D_RRH_ALMBIT_RXGABR1	0x10000000		/* RX GAIN ALM(BR#1)	*/
#define	D_RRH_ALMBIT_TDDSW1BR1	0x20000000		/* TDD SW ALM1(BR#1)	*/
#define	D_RRH_ALMBIT_TDDSW2BR1	0x40000000		/* TDD SW ALM2(BR#1)	*/
#define	D_RRH_ALMBIT_RFIC1BR2	0x00000010		/* RFIC ALM1(BR#2) */
#define	D_RRH_ALMBIT_RFIC2BR2	0x00000020		/* RFIC ALM2(BR#2) */
#define	D_RRH_ALMBIT_FWJESDBR2	0x00000040		/* FW JESD ALM(BR#2) */
#define	D_RRH_ALMBIT_FBJESDBR2	0x00000080		/* FB JESD ALM(BR#2) */
#define	D_RRH_ALMBIT_RXJESDBR2	0x00000100		/* RX JESD ALM(BR#2) */
#define	D_RRH_ALMBIT_TXGABR2	0x00000400		/* TX GAIN ALM(BR#2)	*/
#define	D_RRH_ALMBIT_TOVEBR2	0x00000800		/* TOVER ALM(BR#2)	*/
#define	D_RRH_ALMBIT_RXGABR2	0x00001000		/* RX GAIN ALM(BR#2)	*/
#define	D_RRH_ALMBIT_TDDSW1BR2	0x00002000		/* TDD SW ALM1(BR#2)	*/
#define	D_RRH_ALMBIT_TDDSW2BR2	0x00004000		/* TDD SW ALM2(BR#2)	*/
#define	D_RRH_ALMBIT_RFIC1BR3	0x00100000		/* RFIC ALM1(BR#3) */
#define	D_RRH_ALMBIT_RFIC2BR3	0x00200000		/* RFIC ALM2(BR#3) */
#define	D_RRH_ALMBIT_FWJESDBR3	0x00400000		/* FW JESD ALM(BR#3) */
#define	D_RRH_ALMBIT_FBJESDBR3	0x00800000		/* FB JESD ALM(BR#3) */
#define	D_RRH_ALMBIT_RXJESDBR3	0x01000000		/* RX JESD ALM(BR#3) */
#define	D_RRH_ALMBIT_TXGABR3	0x04000000		/* TX GAIN ALM(BR#3)	*/
#define	D_RRH_ALMBIT_TOVEBR3	0x08000000		/* TOVER ALM(BR#3)	*/
#define	D_RRH_ALMBIT_RXGABR3	0x10000000		/* RX GAIN ALM(BR#3)	*/
#define	D_RRH_ALMBIT_TDDSW1BR3	0x20000000		/* TDD SW ALM1(BR#3)	*/
#define	D_RRH_ALMBIT_TDDSW2BR3	0x40000000		/* TDD SW ALM2(BR#3)	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */


/*!
 * @name CNTM_SV7INFのアラーム情報
 * @note
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
#define D_RRH_S3G_CARR_NUMALM			0x08000000		/*!< (LTE)キャリア数 */
#define D_RRH_S3G_BAND_WIDTH_ALM		0x04000000		/*!< (LTE)最大周波数帯域幅 */
#define D_RRH_S3G_FREQ_CMPLX_ALM		0x02000000		/*!< (LTE)対応周波数帯 */
#define D_RRH_S3G_SHARETYPE_ALM			0x01000000		/*!< (LTE)3GBTS共用方式 */
#define D_RRH_3G_DWN_RESRC_NUM_ALM		0x00200000		/*!< (3G)下りリソース数 */
#define D_RRH_3G_UP_RESRC_NUM_ALM		0x00100000		/*!< (3G)上りリソース数 */
#define D_RRH_3G_CARR_NUM_ALM			0x00080000		/*!< (3G)キャリア数 */
#define D_RRH_3G_FREQ_CMPLX_ALM			0x00020000		/*!< (3G)対応周波数帯 */
/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */

#define	D_RRH_PLLALM_BIT		(D_RRH_ALMBIT_PLLUNLOCK)
#define	D_RRH_CPRIALM_BIT		(D_RRH_ALMBIT_FPGAUNLK1)
#define	D_RRH_COMALM_BIT		(D_RRH_ALMBIT_FPGAIF1 | D_RRH_ALMBIT_FPGAIF2 | D_RRH_ALMBIT_CPLDIF1 | D_RRH_ALMBIT_TEMP | D_RRH_ALMBIT_PSALM )
#define D_RRH_REGMASK_SV6	( D_RRH_PLLALM_BIT | D_RRH_CPRIALM_BIT | D_RRH_COMALM_BIT )
#define D_RRH_REGMASK_SV7	( D_RRH_APD0ALM_BIT | D_RRH_APD1ALM_BIT )
#define D_RRH_REGMASK_SV8	( D_RRH_APD2ALM_BIT | D_RRH_APD3ALM_BIT )
/* @} */

/** @name register bit  define */
/* @{ */

/* ======================================================================== */
/* レジスタビット定義定数													*/
/* ======================================================================== */
/* 外部装置関連 */
#define D_RRH_BRETERM_ALM_BIT               0x8000          /**< BRE-TERMカードアラーム検出ビット           */
#define D_RRH_FAN_ALM_BIT                   0x0004          /**< FAN-ALM検出ビット                          */

/* TRA SVレジスタ関連 */
#define D_RRH_ACT_BIT                       0x2000          /**< カードACTビット                            */
#define D_RRH_NOINS_BIT                     0x4000          /**< カード未実装ビット                         */
#define D_RRH_INS_BIT                       0x8000          /**< カード実装ビット                           */
#define D_RRH_TST_BIT                       0x0200          /**< カード診断ビット                           */
#define D_RRH_BLK_BIT                       0x0400          /**< カード閉塞ビット                           */
#define D_RRH_RMTERR_BIT                    0x0080          /**< リモートERRビット                          */
#define D_RRH_BR0USE_C0_BIT                 0x0002          /**< BR0 USE C0 bit                             */
#define D_RRH_BR0USE_C1_BIT                 0x0004          /**< BR0 USE C0 bit                             */
#define D_RRH_BR1USE_C0_BIT                 0x0020          /**< BR1 USE C0 bit                             */
#define D_RRH_BR1USE_C1_BIT                 0x0040          /**< BR1 USE C1 bit                             */
#define D_RRH_BR2USE_C0_BIT                 0x0200          /**< BR2 USE C0 bit                             */
#define D_RRH_BR2USE_C1_BIT                 0x0400          /**< BR2 USE C1 bit                             */
#define D_RRH_BR3USE_C0_BIT                 0x2000          /**< BR3 USE C0 bit                             */
#define D_RRH_BR3USE_C1_BIT                 0x4000          /**< BR3 USE C1 bit                             */
#define D_RRH_USE_BIT                       0x00FF          /**< カードUSE検出ビット                        */
#define D_RRH_ALM_BIT                       0x6700          /**< カードALM検出ビット                        */
#define D_RRH_RSALM_BIT                     0xFF3A          /**< リソースALM 検出ビット                     */
#define D_RRH_HDALM_BIT                     0x0007          /**< ハードALM 検出ビット                       */
#define D_RRH_SOFTALM_BIT                   0x0020          /**< Soft ALM 検出ビット                        */
#define D_RRH_MAJALM_PLL_BIT                0x0001          /**< メジャーアラーム要因:PLL系アラーム         */
#define D_RRH_MAJALM_CPRI_BIT               0x0002          /**< メジャーアラーム要因:CPRI系アラーム        */
#define D_RRH_MAJALM_APDCOM_BIT             0x0004          /**< メジャーアラーム要因:TRX系アラーム         */
#define D_RRH_MAJALM_APD0_BIT               0x0008          /**< メジャーアラーム要因:TRX-Br#0系アラーム    */
#define D_RRH_MAJALM_APD1_BIT               0x0010          /**< メジャーアラーム要因:TRX-Br#1系アラーム    */
//TDD-SRE
#define D_RRH_MAJALM_APD2_BIT               0x0800          /**< メジャーアラーム要因:TRX-Br#2系アラーム    */
#define D_RRH_MAJALM_APD3_BIT               0x1000          /**< メジャーアラーム要因:TRX-Br#3系アラーム    */
//TDD-SRE
#define D_RRH_MAJALM_COM_BIT                0x0027          /**< メジャーアラーム要因:TRX-Br#1系アラーム    */
#define D_RRH_MAJALM_BR_BIT_NT2R			(D_RRH_MAJALM_APD0_BIT | D_RRH_MAJALM_APD1_BIT)
#define D_RRH_MAJALM_BR_BIT_NT4R			(D_RRH_MAJALM_APD0_BIT | D_RRH_MAJALM_APD1_BIT | D_RRH_MAJALM_APD2_BIT | D_RRH_MAJALM_APD3_BIT)
#define D_RRH_MAJALM_BR2_BR3_BIT			(D_RRH_MAJALM_APD2_BIT | D_RRH_MAJALM_APD3_BIT)

#define D_RRH_ERR_BIT                       0x0301          /**< カードERR検出ビット                        */
#define D_RRH_ERR_PAOFF_BIT                 0x0300          /**< カードPA#0_OFF/PA#1_OFF ERR検出ビット      */
#define D_RRH_RFUSE_BIT                     0x0044          /**< RF shared 3G carrier USE bit               */
#define D_RRH_ACT_BIT_ON                    0x2000          /**< USE bit set ON value in SV A0/A1           */
#define D_RRH_ACT_BIT_OFF                   0xC607          /**< USE bit set ON value in SV A0/A1           */
#define D_RRH_BLK_BIT_ON                    0x0400          /**< BLK bit set ON value in SV A0/A1           */
#define D_RRH_BLK_BIT_OFF                   0xE207          /**< BLK bit set ON value in SV A0/A1           */
#define D_RRH_USE_BIT_ON                    0x0004          /**< USE bit set ON value in SV A0/A1           */
#define D_RRH_USE_BIT_OFF                   0xE603          /**< USE bit set OFF value in SV A0/A1          */
#define D_RRH_ALM_BIT_ON                    0x0002          /**< ALM bit set ON value in SV A0/A1           */
#define D_RRH_ALM_BIT_OFF                   0xE605          /**< ALM bit set OFF value in SV A0/A1          */
#define D_RRH_ERR_BIT_ON                    0x0001          /**< ERR bit set ON value in SV A0/A1           */
#define D_RRH_ERR_BIT_OFF                   0xE606          /**< ERR bit set OFF value in SV A0/A1          */

//TDD-SRE
#define D_RRH_PAOFF_ALLBR_NT2R              0x0300          /**< PAOFF(BR0&BR1) bit in SV A6/A7             */
#define D_RRH_PAOFF_ALLBR_NT4R              0x0F00          /**< PAOFF(BR0&BR1&BR2&BR3) bit in SV A6/A7     */
//TDD-SRE
#define D_RRH_PAOFF_BR0                     0x0100          /**< PAOFF(BR0) bit in SV A6/A7                 */
#define D_RRH_PAOFF_BR1                     0x0200          /**< PAOFF(BR1) bit in SV A6/A7                 */
//TDD-SRE
#define D_RRH_PAOFF_BR2                     0x0400          /**< PAOFF(BR2) bit in SV A6/A7                 */
#define D_RRH_PAOFF_BR3                     0x0800          /**< PAOFF(BR3) bit in SV A6/A7                 */
//TDD-SRE

#define D_RRH_RMT_ERR_BIT                   0x8000          /**< Remote ERR bit in SV A8/A9                 */
#define D_RRH_TRX0ALM_BIT                   0x0008          /**< TRX0 ALM 検出ビット                        */
#define D_RRH_TRX1ALM_BIT                   0x0010          /**< TRX1 ALM 検出ビット                        */
#define D_RRH_BR0ALM_BIT                    0x0001          /**< BR0 ALM 検出ビット                         */
#define D_RRH_BR1ALM_BIT                    0x0010          /**< BR1 ALM 検出ビット                         */
//TD-SRE
#define D_RRH_BR2ALM_BIT                    0x0100          /**< BR0 ALM 検出ビット                         */
#define D_RRH_BR3ALM_BIT                    0x1000          /**< BR0 ALM 検出ビット                         */
//TD-SRE

#define D_RRH_CARR_BR0USE                   0x0001          /**< BR0 USE bit in Tx Carrier state            */
#define D_RRH_CARR_BR1USE                   0x0010          /**< BR1 USE bit in Tx Carrier state            */
#define D_RRH_CARR_BOTHUSE			(D_RRH_CARR_BR0USE | D_RRH_CARR_BR1USE)

#define D_RRH_HWALM_HW_COM_MASK				0x5D030000

/* ローカル/リモート */
#define D_RRH_LCL_BIT                       0x0080          /**< ローカル制御ビット                         */
#define D_RRH_RMT_BIT                       0x0040          /**< リモート制御ビット                         */

/* @} */

/** @name Parameter of RE card Control request */
/* @{ */
/******************************/
/* REカード制御要求パラメータ */
/******************************/
/* 制御 */
#define D_RRH_CARDRST						1 				/**< リセット                                   */
#define D_RRH_CARDBLK						2 				/**< 閉塞                                               */
#define D_RRH_NOBLK							3 				/**< 閉塞解除                                   */
#define D_RRH_ENFBLK						5 				/**< 強制閉塞                                   */
#define D_RRH_CARDALM						6				/**< ALM                                                */
#define D_RRH_2BR_SLP						19				/**< スリープ                                   */
#define D_RRH_BR0_SLP						20				/**< スリープ（0系                              */
#define D_RRH_BR1_SLP						21				/**< スリープ（1系）                    */
#define D_RRH_UNSLP 						22				/**< スリープ解除                               */
#define D_RRH_BR2_SLP						24				/**< スリープ（2系)                             */
#define D_RRH_BR3_SLP						25				/**< スリープ（3系)                             */
#define D_RRH_BR4_SLP						26				/**< スリープ（4系)                             */
#define D_RRH_BR5_SLP						27				/**< スリープ（5系)                             */
#define D_RRH_BR6_SLP						28				/**< スリープ（6系)                             */
#define D_RRH_BR7_SLP						29				/**< スリープ（7系)                             */
/* @} */

/* 制御 */
#define D_RRH_GETSTA						0				/**< 状態取得                                   */
#define D_RRH_RLYOFF						1				/**< リレーOFF                                  */
#define D_RRH_RLYON							2				/**< リレーON                                   */
/* @} */


/** @name LED state definition         */
#define D_RRH_LED1STATE_OFF                 0               /**< LED1: OFF                                  */
#define D_RRH_LED1STATE_BLNK                1               /**< LED1: Blink                                */
#define D_RRH_LED1STATE_ON                  2               /**< LED1: ON                                   */

#define D_RRH_LED2STATE_OFF                 0               /**< LED2: OFF                                  */
#define D_RRH_LED2STATE_BLNK                1               /**< LED2: Blink                                */
#define D_RRH_LED2STATE_ON                  2               /**< LED2: ON                                   */

#define D_RRH_LED3STATE_OFF                 0               /**< LED3: OFF                                  */
#define D_RRH_LED3STATE_BLNK                1               /**< LED3: Blink                                */
#define D_RRH_LED3STATE_ON                  2               /**< LED3: ON                                   */

#define D_RRH_CPRILEDSTATE_ACT_OFF          0               /**< CPRI LINK LED(ACT LED): OFF                */
#define D_RRH_CPRILEDSTATE_ACT_BLNK         1               /**< CPRI LINK LED(ACT LED): Blink              */
#define D_RRH_CPRILEDSTATE_ACT_ON           2               /**< CPRI LINK LED(ACT LED): ON                 */

#define D_RRH_CPRILEDSTATE_DWN_OFF          0               /**< CPRI LINK LED(DWN LED): OFF                */
#define D_RRH_CPRILEDSTATE_DWN_BLNK         1               /**< CPRI LINK LED(DWN LED): Blink              */
#define D_RRH_CPRILEDSTATE_DWN_ON           2               /**< CPRI LINK LED(DWN LED): ON                 */
/* @} */

/** @name External Device number index defination              */
#define D_RRH_OARA_INF_1					1				/**< OA-RA-INF1                                 */
#define D_RRH_OARA_INF_2					2				/**< OA-RA-INF2                                 */
#define D_RRH_MTT_INF						3				/**< MTT-INF					*/
#define D_RRH_TILT_INF						4				/**< TILT-INF                                   */


#define D_RRH_OARA_INF_S3G					1				/**< OA-RA-INF   S3G                    */
#define D_RRH_TILT_INF_S3G					2				/**< TILT-INF    S3G                    */


/** @name Internal Device number index defination              */
/* @{ */
#define D_RRH_DEVIDX_OA1					0				/**< OARA 1     */
#define D_RRH_DEVIDX_TILT					1				/**< TILT               */
#define D_RRH_IVL_VAL						65535			/**< 無効値(65535)                              */
/* @} */

/*!
 * @name CPRI state definition
 * @note CPRI state definition
 * @{
 */
#define D_RRH_CPRISTAT_A	0
#define D_RRH_CPRISTAT_B	1
#define D_RRH_CPRISTAT_C	2
#define D_RRH_CPRISTAT_D	3
#define D_RRH_CPRISTAT_E	4
#define D_RRH_CPRISTAT_F	5
#define D_RRH_CPRISTAT_HDLCACT	6
#define D_RRH_CPRISTAT_HDLCIDLE	7
#define D_RRH_CPRISTAT_HDLC	10
/* @} */

/*!
 * @name HDLC bit rate defintion
 * @note HDLC bit rate definition
 * @{
 */
#define D_RRH_LINE_BITRATE_24G		0
#define D_RRH_LINE_BITRATE_49G		1
#define D_RRH_LINE_BITRATE_98G		2


/* @} */

/** @name ERR bit mask pattern */
/* @{ */
/* マスクパターン */
/******************/
/* CPRIリンクERR設樗wジスタ	*/
#define D_RRH_ERR_CNT_L2ERR			0x0001		/**< L2 ERR						*/
#define D_RRH_ERR_CNT_L3ERR_3G		0x0002		/**< L3 ERR for 3G				*/
#define D_RRH_ERR_CNT_L3ERR_S3G		0x0004		/**< L3 ERR for S3G				*/
#define D_RRH_ERR_CNT_L2OFF			0xfffe		/**< L2 ERR off					*/
#define D_RRH_ERR_CNT_L3OFF_3G		0xfffd		/**< L3 ERR off for 3G			*/
#define D_RRH_ERR_CNT_L3OFF_S3G		0xfffb		/**< L3 ERR off for S3G			*/

/* ソフトERR制御(基本A7)		*/
#define D_RRH_ERR12_BR0_SIR_NG			0x0010		/**< TRA Br0シリアル制御NG		*/
#define D_RRH_ERR13_BR1_SIR_NG			0x0020		/**< TRA Br1シリアル制御NG		*/
#define D_RRH_ERR14_EXT_SIR_NG			0x0040		/**< EXTシリアル制御NG			*/

/* ソフトERR制御(基本A8)		*/
#define D_RRH_ERR16_BR0_SIR_NG			0x0001		/**< 2次診断TRABr0シリアル制御NG	*/
#define D_RRH_ERR17_BR1_SIR_NG			0x0002		/**< 2次診断TRABr1シリアル制御NG	*/
#define D_RRH_ERR18_BR0_ULIF_NG			0x0004		/**< TRABr0 上り高速IFNG			*/
#define D_RRH_ERR19_BR1_ULIF_NG			0x0008		/**< TRABr1 上り高速IFNG			*/
#define D_RRH_ERR20_BR0_DLIF_NG			0x0010		/**< TRABr0 下り高速IFNG			*/
#define D_RRH_ERR21_BR1_DLIF_NG			0x0020		/**< TRABr1 下り高速IFNG			*/
#define D_RRH_ERR22_MTT_NG				0x0040		/**< OF時のMTT回線断検出			*/
#define D_RRH_ERR23_MTT_PAL_NG			0x0040		/**< OF時のMTT回線パリティNG		*/

/* CeNB-F-083-028 FFCS)Xut add start */
#define	D_RRH_ERR_AMP_DISC_ERR			0x0010		/**< AMPｶﾏERR						*/
#define	D_RRH_ERR_AMP_DISC_ERR_OFF		0xFFEF		/**< AMPｶﾏERR OFF				*/
#define	D_RRH_ERR_SLP0_ERR_ON			0x0020		/**< SLP0 ERR ON					*/
#define	D_RRH_ERR_SLP0_ERR_OFF			0xFFDF		/**< SLP0 ERR OFF					*/
#define	D_RRH_ERR_SLP1_ERR_ON			0x0040		/**< SLP1 ERR ON					*/
#define	D_RRH_ERR_SLP1_ERR_OFF			0xFFBF		/**< SLP1 ERR OFF					*/
#define	D_RRH_ERR_SLP2_3_ERR_OFF			0xFE7F		/**< SLP2,3 ERR OFF					*/
/* CeNB-F-083-028 FFCS)Xut add end */

/* ソフトERR制御(基本A9)		*/
#define D_RRH_ERR24_SOFT_NG_SET				0x0001		/**< SOFT固有ERR検出時				*/
#define D_RRH_ERR24_SOFT_NG_REL				0x0000		/**< SOFT固有ERR解蒐鮑				*/
#define D_RRH_ERR_DL_INF_ERR				0x4000		/**< Download infor NG 				*/
#define D_RRH_ERR_DL_CMP_ERR				0x2000		/**< Download complete NG			*/
#define D_RRH_ERR_DL_INF_ERR_OFF			0xBFFF		/**< Download infor NG cancel		*/
#define D_RRH_ERR_DL_CMP_ERR_OFF			0xDFFF		/**< Download complete NG cancel	*/
#define D_RRH_ERR_DL_INF_S3G_ERR			0x1000		/**< Download infor NG for S3G 		*/
#define D_RRH_ERR_DL_CMP_S3G_ERR			0x0800		/**< Download complete NG for S3G	*/
#define D_RRH_ERR_DL_INF_S3G_ERR_OFF		0xEFFF		/**< Download infor NG cancel for S3G	*/
#define D_RRH_ERR_DL_CMP_S3G_ERR_OFF		0xF7FF		/**< Download complete NG cancel for S3G*/
#define D_RRH_ERR31_L3CARDCNT_ERR			0x8000		/**< SOFT固有ERR検出時					*/
#define D_RRH_ERR31_L3CARDCNT_ERR_OFF		0x7FFF		/**< SOFT固有ERR解蒐鮑					*/
#define D_RRH_ERR_PARITY_ERR 				0x0400		/**< parity error 						*/
#define D_RRH_ERR_PARITY_ERR_0FF 			0xFBFF		/**< parity error cancel 				*/
#define D_RRH_ERR_ORAR_SV_ERR 				0x0200		/**< OARA line off supervision error	*/
#define D_RRH_ERR_ORAR_SV_ERR_OFF    		0xFDFF		/**< OARA line off supervision error cancel	*/
#define D_RRH_ERR_MT_ADDR_ERR 				0x0100		/**< MT address setting error			*/
#define D_RRH_ERR_MT_ADDR_ERR_OFF    		0xFEFF		/**< MT address setting error cancel	*/
#define D_RRH_ERR_RF_61MCLK_ERR				0x0080		/* BRE-61MCLK ERR 						*/
#define D_RRH_ERR_RF_61MCLK_ERR_OFF			0xFF7F		/* BRE-61MCLK ERR cancel 				*/
/* @} */

/** @name Max system parameter length */
/* @{ */
/* Max system parameter length */
#define D_RRH_SYSPRAD_MAX			16		/* max system parameter D length			*/
#define D_RRH_SYSPRARCV_MAX		32		/* Received max system parameter length(SYSPAR-D +SYSPAR-E+ SYSPAR-F) */
#define D_RRH_EEPSINFMSK1			0x0001			/**< EEPROMﾆﾓﾃ貮驤ﾞ･ｹ･ｯ(0x0001)*/
/* @} */

/** @name Reset factor */
/* @{ */
#define D_RRH_RSTFACT_CRDCNT			1			/**< Card Control				*/
#define D_RRH_RSTFACT_RERST				2			/**< RE Reset request			*/
/* @} */

/** @name device related information */
/* @{ */

#define D_RRH_IND_POWERTYP_VALUE			0x0000		/**< Init value for power type */
#define D_RRH_IND_POWERTYP_MSK				0x0003		/**< Mask value for power type */
#define D_RRH_IND_MAKER_NAME				'F'			/**< Maker name */
/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/*!
 * @name 装置情報関連
 * @note 装置情報関連定義
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
enum{
	E_RRH_RRHKIND_15G_TRI = 0,							/* 1.5G対応 3セクタ							*/
	E_RRH_RRHKIND_17G_TRI,								/* 1.7G対応 3セクタ							*/
	E_RRH_RRHKIND_700M,									/* 700M対応									*/
	E_RRH_RRHKIND_SRE_15G,								/* 1.5G対応 SRE								*/
	E_RRH_RRHKIND_SLC_2G,								/* 2.0G対応 SLC-RRE							*/
	E_RRH_RRHKIND_SLC_800M,								/* 800M対応 SLC-RRE							*/
	E_RRH_RRHKIND_1CHP_15G,								/* 1.5G対応 1chip-RRE						*/
	E_RRH_RRHKIND_1CHP_17G,								/* 1.7G対応 1chip-RRE						*/
	E_RRH_RRHKIND_SRE_35G,								/* 3.5G対応 TDD-SRE							*/
	E_RRH_RRHKIND_RRE_35G,								/* 3.5G対応 TDD-RRE							*/
	E_RRH_RRHKIND_NUM									/* 装置種別数								*/
};
/* @} */

/*!
 * @name RE種別情報関連
 * @note RE種別情報関連定義
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
enum{
	E_RRH_REKIND_RRE = 0,								/* RRE										*/
	E_RRH_REKIND_SRE,									/* SRE										*/
	E_RRH_REKIND_SLC,									/* SLC										*/
	E_RRH_REKIND_1CHP,									/* 1CHP										*/
	E_RRH_REKIND_TDD_SRE,								/* TDD										*/
	E_RRH_REKIND_NUM_FHM									/* RE種別数									*/
};
/* @} */


/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/*!
 * @name RE種別情報関連２
 * @note RE種別情報関連定義
 * 設定値が0x10のため、既存のRE種別情報定義ではenum宣言で空データが必要なため別に定義する
 * @{
 */
#define	E_RRH_REKIND_FHM			0x10				/* FHM										*/
/* @} */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/*!
 * @name 配列要素
 * @note 配列要素定義
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
enum{
	E_DIM_NUM0 = 0,										/* 要素0									*/
	E_DIM_NUM1,											/* 要素1									*/
	E_DIM_NUM2,											/* 要素2									*/
	E_DIM_NUM3,											/* 要素3									*/
	E_DIM_NUM4,											/* 要素4									*/
	E_DIM_NUM5											/* 要素5									*/
};
/* @} */


/*!
  * @name アラームファクター要因種別
  * @note
  * @{
  */
#define		D_RRH_ALARM_FACTOR_TYPE_SW		1			/**< ソフト要因			*/
#define		D_RRH_ALARM_FACTOR_TYPE_HW		2			/**< ハード要因			*/
#define		D_RRH_ALARM_FACTOR_TYPE_INIT	3			/**< 初期化要因			*/
/* @} */

/*!
 * @name 自律リセットフラグ定義
 * @note 自律リセットフラグ定義  define
 * @{
 */
#define		D_RRH_ALM_SELF_RESET_ON				0x01					/*!< 自律リセットON						*/
#define		D_RRH_ALM_SELF_RESET_OFF			0x00					/*!< 自律リセットOFF					*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define		D_RRH_ALM_PROTECT_TIM				172800					/* 172800(sec) = 48(hour) */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @} */

/** @name BLK control kind(Remote/Local) */
/* @{ */
#define 		D_RRH_RMT_CNT				0			/**< ?????[?g????					*/
#define 		D_RRH_LCL_CNT				1			/**< ???[?J??????					*/
/* @} */

/** @name  Card ALM check need or not */
/* @{ */
#define		D_RRH_CHKALM_NCY				1			/**< Need check						*/
#define		D_RRH_CHKALM_UNNCY				0			/**< No need check					*/
/* @} */

/*!
  * @name Board Version
  * @note
  * @{
  */
#define		D_RRH_BVER_MSB_BIT				0xF0		/**< Board Version[7-4]				*/
#define		D_RRH_BVER_TEST3_HIGH_POW		0x00		/**< 試作3(高出力品(33dBm)			*/
#define		D_RRH_BVER_TEST3_LOW_POW		0x10		/**< 試作3(低出力品(0dBm)			*/
/* @} */

/*!
 * @name レジスタアクセスチェック診断関連
 * @note 
 * @{
 */
enum
{
	E_RRH_DIA_REGCHK_PL_FPGA = 0,						/*!< PL FPGA						*/
	E_RRH_DIA_REGCHK_RFIC,								/*!< RFIC							*/
	E_RRH_DIA_REGCHK_CPLD_LCC,							/*!< CPLD_LCC						*/
	E_RRH_DIA_REGCHK_CPLD_MMI,							/*!< CPLD_MMI						*/
	E_RRH_DIA_REGCHK_HDLC,								/*!< HDLC							*/
	E_RRH_DIA_REGCHK_NUM								/*!< レジスタアクセスチェック数		*/
};

/*!
 * @name カスケード親子情報
 * @note 
 * @{
 */
enum
{
	E_RRH_CASCADE_MASTER_ALONE = 0,						/*!< 親(単独状態)					*/
	E_RRH_CASCADE_MASTER,								/*!< 親(カスケード状態)				*/
	E_RRH_CASCADE_SLAVE,								/*!< 子								*/
	E_RRH_CASCADE_INIT
};
/* @} */

/*!
 * @name Vender Specific Check関連
 * @note 
 * @{
 */
enum
{
	E_RRH_DEVICE_REC = 0,								/*!< REC							*/
	E_RRH_DEVICE_MASTER,								/*!< 親(カスケード状態)				*/
	E_RRH_DEVICE_SLAVE									/*!< 子								*/
};
/* @} */

/*!
 * @name WDT LOG関連
 * @note 
 * @{
 */
#define		D_RRH_LOG_WDT_CLR_NUM			300			/**< クリアログ						*/
#define		D_RRH_LOG_WDT_RUNHIS_NUM		128			/**< 走行ログ						*/
#define		D_RRH_LOG_WDT_OVER_NUM			20			/**< 超過ログ						*/

enum
{
	E_RRH_LOG_WDT_RUNHIS_DPD = 0,						/*!< pf_dpd							*/
	E_RRH_LOG_WDT_RUNHIS_CPRI,							/*!< pf_cpr							*/
	E_RRH_LOG_WDT_RUNHIS_RSV,							/*!< pf_rsv							*/
	E_RRH_LOG_WDT_RUNHIS_L3RCT,							/*!< l3_rct							*/
	E_RRH_LOG_WDT_RUNHIS_L3CPR,							/*!< l3_cpr							*/
	E_RRH_LOG_WDT_RUNHIS_NUM							/*!< runhisログ数					*/
};

#define		D_RRH_LOG_WDT_PATH				"/var/log/wdtlog.tar.gz"		/* WDTログPATH			*/
#define		D_RRH_LOG_WDT_WARN_PATH			"/var/log/wdtwarnlog.log"		/* WDT(warning)ログPATH	*/
/* @} */

/*!
 * @name WDT LOG関連
 * @note 
 * @{
 */
#define		D_RRH_LOG_ECC_RCD_NUM			128			/**< ECCログ						*/
#define		D_RRH_LOG_ECC_REG_TBL_NUM		32			/**< ECCログ(レジスタ数)			*/
/* @} */

/*!
 * @name 48時間監視タイマ値
 * @note 
 * @{
 */
#define		D_RRH_ALM_SELFRESET_TIMECOUNT		48			/**< 48時間(1*48回)					*/
/* @} */

/*!
 * @name Ethernet リンク障害履歴ログ関連
 * @note 
 * @{
 */
#define		D_RRH_LOG_ETHERR_RCD_NUM		256			/**< Ethernet リンク障害履歴ログ Recode数	*/
/* @} */

/*!
 * @name PTP 状態変化履歴ログ関連
 * @note 
 * @{
 */
#define		D_RRH_LOG_PTP_RCD_NUM		256			/**< Ethernet リンク障害履歴ログ Recode数	*/
/* @} */




/*!
 * @name boot更新関連
 * @note 
 * @{
 */
#define		D_RRH_BOOT_RADISK_FILE			"/opt/boot/u-boot.bin"			/**< boot file		*/
#define		D_RRH_BOOT_RADISK_VER			"/opt/boot/ver"					/**< boot version	*/
/* @} */

/*!
 * @name ソフトリセットフラグ
 * @note 
 * 00:ソフトリセット無し(PowerOffとみなす)
 * 01:ソフトリセットあり(リセットコマンド)
 * 02:ソフトリセットあり(CUからのリセット)
 * 03:ALMによる自律リセット
 * 04:Mplane Ether LINK downによる自律リセット
 * 05:Mplane watch dogによる自律リセット
 * @{
 */
#define		D_RRH_SOFT_RESET_NORMAL			1
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define		D_RRH_SOFT_RESET_BOOTDWL		2
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
#define		D_RRH_SOFT_RESET_CU				2
#define		D_RRH_SOFT_RESET_ALM			3
#define		D_RRH_SOFT_RESET_MPLINKDOWN		4
#define		D_RRH_SOFT_RESET_MPWDT			5

/* @} */

/*!								
 * @brief QSPI Manufacture ID		
 * @{  								
 */								
#define QSPI_MICRON_ID                 (0x20)
#define QSPI_MACRONIX_ID               (0xC2)
/*	FLASH EOL対応 add end	*/

/* ======================================================================== */
/* CU接続情報		 														*/
/* ======================================================================== */
/**
 * @brief	VLAN pcp
**/
#define D_RRH_MAC_ADDR_SIZE			6										/* MAC addr Size	*/

#define D_RRH_IF_MAX				3										/* Port数			*/
#define D_RRH_IF_FHMAX				1										/* Port数			*/
#define D_RRH_IF_NAME_DBG			"eth0"									/* Debag Port IF name	*/
#define D_RRH_IF_NAME_0				"fheth0"								/* Port#0 IF name	*/
#define D_RRH_IF_NAME_1				"fheth1"								/* Port#1 IF name	*/
#define D_RRH_IF_NAME_SMAX			32										/* IF name最大文字数*/

#define D_RRH_VID_MIN				1										/* VID最小値		*/
#define D_RRH_VID_MAX				4094									/* VID最大値		*/
#define D_RRH_VID_DEFAULT			1										/* VID最大値		*/

#define D_RRH_PCP_MIN				0										/* PCP最小値		*/
#define D_RRH_PCP_MAX				7										/* PCP最大値		*/

#define D_RRH_PCP_MPLANE			2										/* PCP値(M-Plane)	*/
#define D_RRH_PCP_UPLANE			7										/* PCP値(U-Plane)	*/
#define D_RRH_PCP_CPLANE			7										/* PCP値(C-Plane)	*/
/* @} */

/**
 * @brief	Fronthaul Port No.
**/
#define D_RRH_FHPORT0				0										/* Fronthaul Port#0	*/
#define D_RRH_FHPORT1				1										/* Fronthaul Port#1	*/

/*!
 * @brief	M-Plane情報
 * @{
 */
#define D_RRH_MP_MSG_SIZE_MAX		60000									/* MP msg size max	*/

#define	D_RRH_MP_PCP				2										/* VLAN : PCP (MP)	*/
#define	D_RRH_MP_TCP				50000									/* TCP  : Port(MP)	*/
/* @} */

/*!
 * @brief	callhome関連
 * @{
 */
#define D_RRH_CALLHOME_TIMER_DFAULT		60							/* Callhome retry timer値	*/
/* @} */

/*!
 * @name デバッグ情報
 * @note 
 * @{
 */
#define D_RRH_MP_DBG_MAC_CU_0			0x46						/* Debug Mac Address (CU)	*/
#define D_RRH_MP_DBG_MAC_CU_1			0x4a						/* Debug Mac Address (CU)	*/
#define D_RRH_MP_DBG_MAC_CU_2			0x5f						/* Debug Mac Address (CU)	*/
#define D_RRH_MP_DBG_MAC_CU_3			0x70						/* Debug Mac Address (CU)	*/
#define D_RRH_MP_DBG_MAC_CU_4			0x30						/* Debug Mac Address (CU)	*/
#define D_RRH_MP_DBG_MAC_CU_5			0x30						/* Debug Mac Address (CU)	*/

#define D_RRH_MP_DBG_MAC_DU_DP_0		0x46						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_DP_1		0x4a						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_DP_2		0x5f						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_DP_3		0x70						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_DP_4		0x30						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_DP_5		0x30						/* Debug Mac Address (DU)	*/

#define D_RRH_MP_DBG_MAC_DU_P1_0		0x46						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P1_1		0x4a						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P1_2		0x5f						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P1_3		0x70						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P1_4		0x30						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P1_5		0x31						/* Debug Mac Address (DU)	*/

#define D_RRH_MP_DBG_MAC_DU_P2_0		0x46						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P2_1		0x4a						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P2_2		0x5f						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P2_3		0x70						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P2_4		0x30						/* Debug Mac Address (DU)	*/
#define D_RRH_MP_DBG_MAC_DU_P2_5		0x32						/* Debug Mac Address (DU)	*/

#define D_RRH_MP_DBG_VID_LOW			0x0001								/* Debug VID(Low)	*/
#define D_RRH_MP_DBG_VID_HIGH			0x0FFE								/* Debug VID(High)	*/
#define D_RRH_MP_DBG_VID_LAST_USED		0x0800								/* Debug VID(Last)	*/

#define D_RRH_MP_DBG_IP_CU				0x7f000001					/* Debug IP Address (CU)	*/
#define D_RRH_MP_DBG_IP_DU				0x7f000001					/* Debug IP Address (DU)	*/

/* @} */

/*!
 * @brief SV Ctrl operation type
 * @{  
 */
enum
{
	E_DU_SVCTL_OPE_ADM = 0,								/**> (00)Admin State				*/
	E_DU_SVCTL_OPE_ENG,									/**> (01)Energysaving State			*/
	E_DU_SVCTL_OPE_OPE,									/**> (02)Operational State			*/
	E_DU_SVCTL_OPE_AVL,									/**> (03)availability State			*/
	E_DU_SVCTL_OPE_USA,									/**> (04)usage State				*/
	E_DU_SVCTL_OPE_USE_A,								/**> (05)USE Ant#A					*/
	E_DU_SVCTL_OPE_USE_B,								/**> (06)USE Ant#B					*/
	E_DU_SVCTL_OPE_USE_C,								/**> (07)USE Ant#C					*/
	E_DU_SVCTL_OPE_USE_D,								/**> (08)USE Ant#D					*/
	E_DU_SVCTL_OPE_FLV,									/**> (09)Fault Level				*/
	E_DU_SVCTL_OPE_FID,									/**> (10)Fault ID					*/
	E_DU_SVCTL_OPE_SFT_ALMCD,							/**> (11)FID=0x01 Soft ALM code		*/
	E_DU_SVCTL_OPE_INI_ALMCD,							/**> (12)FID=0x02 Initialization failure code	*/
	E_DU_SVCTL_OPE_FID03_ALM,							/**> (13)FID=0x03 failure			*/
	E_DU_SVCTL_OPE_FID04_ALM,							/**> (14)FID=0x04 failure			*/
	E_DU_SVCTL_OPE_FID05_ALM,							/**> (15)FID=0x05 failure			*/
	E_DU_SVCTL_OPE_FID06_ALM,							/**> (16)FID=0x06 failure			*/
	E_DU_SVCTL_OPE_FID07_ALM,							/**> (17)FID=0x07 failure			*//**> (17)E_DU_SVCTL_OPE_FID07LL_ALM,E_DU_SVCTL_OPE_FID07T1_ALM	*/
	E_DU_SVCTL_OPE_FID08_ALM,							/**> (18)FID=0x08 failure			*/
	E_DU_SVCTL_OPE_FID10_ALM_A,							/**> (19)FID=0x10 failure Ant#A		*/
	E_DU_SVCTL_OPE_FID10_ALM_B,							/**> (20)FID=0x10 failure Ant#B		*/
	E_DU_SVCTL_OPE_FID10_ALM_C,							/**> (21)FID=0x10 failure Ant#C		*/
	E_DU_SVCTL_OPE_FID10_ALM_D,							/**> (22)FID=0x10 failure Ant#D		*/
	E_DU_SVCTL_OPE_FID11_ALM_A,							/**> (23)FID=0x11 failure Ant#A		*//**> (23)E_DU_SVCTL_OPE_FID20_ALM_A		*/
	E_DU_SVCTL_OPE_FID11_ALM_B,							/**> (24)FID=0x11 failure Ant#B		*//**> (24)E_DU_SVCTL_OPE_FID20_ALM_B		*/
	E_DU_SVCTL_OPE_FID11_ALM_C,							/**> (25)FID=0x11 failure Ant#C		*//**> (25)E_DU_SVCTL_OPE_FID20_ALM_C		*/
	E_DU_SVCTL_OPE_FID11_ALM_D,							/**> (26)FID=0x11 failure Ant#D		*//**> (26)E_DU_SVCTL_OPE_FID20_ALM_D		*/
	E_DU_SVCTL_OPE_FID12_ALM_A,							/**> (27)FID=0x12 failure Ant#A		*/
	E_DU_SVCTL_OPE_FID12_ALM_B,							/**> (28)FID=0x12 failure Ant#B		*/
	E_DU_SVCTL_OPE_FID12_ALM_C,							/**> (29)FID=0x12 failure Ant#C		*/
	E_DU_SVCTL_OPE_FID12_ALM_D,							/**> (30)FID=0x12 failure Ant#D		*/
	E_DU_SVCTL_OPE_FID10_ALM_VH,						/**> (31)FID=0x10 failure (VH偏波)	*/
	E_DU_SVCTL_OPE_FID30_ALM,							/**> (32)FID=0x30 failure			*/
	E_DU_SVCTL_OPE_FID31_ALM,							/**> (33)FID=0x31 failure			*/
	E_DU_SVCTL_OPE_FID40_ALM,							/**> (34)FID=0x40 failure			*/
	E_DU_SVCTL_OPE_FID41_ALM,							/**> (35)FID=0x41 failure			*/
	E_DU_SVCTL_OPE_FID42_ALM,							/**> (36)FID=0x42 failure			*/
	E_DU_SVCTL_OPE_FID43_ALM,							/**> (37)FID=0x43 failure			*/
	E_DU_SVCTL_OPE_FID44_ALM,							/**> (38)FID=0x44 failure			*/
	E_DU_SVCTL_OPE_FID50_ALM,							/**> (39)FID=0x50 failure			*/
	E_DU_SVCTL_OPE_FIDF0_ALM,							/**> (40)FID=0xF0 failure			*/
	E_DU_SVCTL_OPE_FIDF1_ALM,							/**> (41)FID=0xF1 failure			*/
	E_DU_SVCTL_OPE_FIDF2_ALM,							/**> (42)FID=0xF2 failure			*/
	E_DU_SVCTL_OPE_FIDFF_ALM,							/**> (43)FID=0xFF failure			*/
	E_DU_SVCTL_OPE_FLDL,								/**> (44)Running File Download		*/
	E_DU_SVCTL_OPE_STP_WAV,								/**> (45)all ANT 停波				*/
	E_DU_SVCTL_OPE_SYNCE,								/**> (46)SyncE Status				*/
	E_DU_SVCTL_OPE_SPLAN,								/**> (47)PTP/SyncE Locked Status	*/
	E_DU_SVCTL_OPE_SYNC,								/**> (48)Sync Status				*/
	E_DU_SVCTL_OPE_FID51_ALM,							/**> (49)FID=0x51 failure			*/
	E_DU_SVCTL_OPE_FID51_NOALM,							/**> (50)FID=0x51 No failure		*/
	E_DU_SVCTL_OPE_MAX									/**> (51)max value					*/
};


#define	E_DU_SVCTL_OPE_USE_E		E_DU_SVCTL_OPE_MAX			/**> (09)USE Ant#E					*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_USE_F		E_DU_SVCTL_OPE_MAX			/**> (10)USE Ant#F					*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_USE_G		E_DU_SVCTL_OPE_MAX			/**> (11)USE Ant#G					*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_USE_H		E_DU_SVCTL_OPE_MAX			/**> (12)USE Ant#H					*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID07LL_ALM	E_DU_SVCTL_OPE_FID07_ALM	/**> (21)FID=0x07 failure(LLB)		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID07T1_ALM	E_DU_SVCTL_OPE_FID07_ALM	/**> (22)FID=0x07 failure(TRX1)		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID07T2_ALM	E_DU_SVCTL_OPE_MAX			/**> (23)FID=0x07 failure(TRX2)		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID10_ALM_E	E_DU_SVCTL_OPE_MAX			/**> (29)FID=0x10 failure Ant#E		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID10_ALM_F	E_DU_SVCTL_OPE_MAX			/**> (30)FID=0x10 failure Ant#F		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID10_ALM_G	E_DU_SVCTL_OPE_MAX			/**> (31)FID=0x10 failure Ant#G		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID10_ALM_H	E_DU_SVCTL_OPE_MAX			/**> (32)FID=0x10 failure Ant#H		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID20_ALM_A	E_DU_SVCTL_OPE_FID11_ALM_A	/**> (33)FID=0x20 failure Ant#A		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID20_ALM_B	E_DU_SVCTL_OPE_FID11_ALM_B	/**> (34)FID=0x20 failure Ant#B		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID20_ALM_C	E_DU_SVCTL_OPE_FID11_ALM_C	/**> (35)FID=0x20 failure Ant#C		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID20_ALM_D	E_DU_SVCTL_OPE_FID11_ALM_D	/**> (36)FID=0x20 failure Ant#D		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID20_ALM_E	E_DU_SVCTL_OPE_MAX			/**> (37)FID=0x20 failure Ant#E		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID20_ALM_F	E_DU_SVCTL_OPE_MAX			/**> (38)FID=0x20 failure Ant#F		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID20_ALM_G	E_DU_SVCTL_OPE_MAX			/**> (39)FID=0x20 failure Ant#G		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_FID20_ALM_H	E_DU_SVCTL_OPE_MAX			/**> (40)FID=0x20 failure Ant#H		*//*	5G-評価DU	*/
#define	E_DU_SVCTL_OPE_INI_ALMCD_EX	E_DU_SVCTL_OPE_MAX			/**> (47)FID=0x02 Initialization failure code	*//*	5G-評価DU	*/

#define	D_DU_SVCTL_SV01_MAX		5						/**> SV info#01 kind number		*/
#define	D_DU_SVCTL_SV02_MAX		4						/**> SV info#02 kind number		*/
#define	D_DU_SVCTL_SV03_MAX		1						/**> SV info#03 kind number		*/
#define	D_DU_SVCTL_SV04_MAX		1						/**> SV info#04 kind number		*/
#define	D_DU_SVCTL_SV05_MAX		2						/**> SV info#05 kind number		*/
#define	D_DU_SVCTL_SV06_MAX		4						/**> SV info#06 kind number		*/
#define	D_DU_SVCTL_SV07_MAX		1						/**> SV info#07 kind number		*/
#define	D_DU_SVCTL_SV08_MAX		1						/**> SV info#08 kind number		*/
#define	D_DU_SVCTL_SV09_MAX		6						/**> SV info#09 kind number		*/
#define	D_DU_SVCTL_SV10_MAX		6						/**> SV info#10 kind number		*/
#define	D_DU_SVCTL_SV11_MAX		1						/**> SV info#11 kind number		*/
#define	D_DU_SVCTL_SV12_MAX		0						/**> SV info#12 kind number		*/
#define	D_DU_SVCTL_SV13_MAX		10						/**> SV info#13 kind number		*/
#define	D_DU_SVCTL_SV14_MAX		4						/**> SV info#14 kind number		*/
#define	D_DU_SVCTL_SV15_MAX		1						/**> SV info#15 kind number		*/
#define	D_DU_SVCTL_SV16_MAX		4						/**> SV info#16 kind number		*/
#define	D_DU_SVCTL_SVXX_MAX		10


#define D_DU_IFLOG_ON			0						/* T_RRH_HEADのuiSignalkindの値(default値) */
#define D_DU_IFLOG_OFF			0xdeadbeef				/* T_RRH_HEADのuiSignalkindの値(ログ保存なし) */


#define D_DU_PTP_UNLOCK_THRESHOLD_VAL		80			/* PTP_UNLOCKの閾値 */


#define D_DU_STARTUP_TIME		3000					/* 3000 *100sec => 300sec => 5min */
#define D_DU_T23ADJVAL_MAX		0x3FF					/* -512 */
#define D_DU_T23ADJVAL_MN		0x1FF					/* マイナス値判定 */

#define D_DU_DEFAULT_SSHPORTNO			830				/*!< ssh port番号 */
#define D_DU_DEFAULT_SSHPORTNO_CALLHOME	4334			/*!< callhome用 ssh port番号 */

/* ======================================================================== */
/* メッセージID		 														*/
/* ======================================================================== */
/**
 * @brief	message-id
 * @{
**/
/* Message ID(pf_pfm) @ 5GDU */
#define D_DU_MSGID_PFM_1S_TIMEOUT_NTF		0x100F0001	/**< 1secタイマ T.O.通知		*/
#define D_DU_MSGID_PFM_LOG_TIMEOUT_NTF		0x100F0002	/**< ログタイマ T.O.通知		*/


/* Message ID(pf_ptp) */
#define D_DU_MSGID_PTPSTSTO_NTC				0x10080001	/**< PTP-状態保護タイムアウト	*/
#define D_DU_MSGID_SYNCESTSTO_NTC			0x10080003	/**< SyncE-状態保護タイムアウト	*/
#define D_DU_MSGID_SYNCSTSTO_NTC			0x10080005	/**< Sync-状態保護タイムアウト	*/
#define D_DU_MSGID_SYNCESSMTO_NTC			0x10080007	/**< SyncE-SSMタイムアウト		*/

/* Message ID(pf_eio) *//*	5G-商用DU	*/
#define D_DU_MSGID_EIO_EXTOUTSV_TO			0x10170001	/**< pf_eio : 外部IO入力状態監視タイムアウト通知	*/
#define D_DU_MSGID_EIO_FANSVCTRL_TO			0x10170003	/**< pf_eio : FAN監視制御タイムアウト通知			*/


/* ======================================================================== */
/* SFN/RFN補正状態	 														*/
/* ======================================================================== */
/**
 * @brief	state of SFN/RFN calcuration
 * @{
**/
#define	D_DU_SFNRFN_CORRECTION_CALC_WAIT	2			/**< 1pps SFN/FRM計算 待ち		*/
#define	D_DU_SFNRFN_CORRECTION_CALC			1			/**< 1pps SFN/FRM計算 実行		*/
#define	D_DU_SFNRFN_CORRECTION_STOP			0			/**< 1pps SFN/FRM計算 停止		*/
/* @} */

/* ======================================================================== */
/* NETCONF user information													*/
/* ======================================================================== */
/**
 * @brief	NETCONF user information
 * @{
**/
#define	D_DU_NETCONF_NUM_OF_USER			5			/**< NETCONF user Info数 (Shared Mem)	*/
#define	D_DU_NETCONF_NUM_OF_USER_FLASH		150			/**< NETCONF user Info数 (FLASH Mem)	*/
#define	D_DU_NETCONF_CHAR_OF_USER			64			/**< NETCONF user 文字数		*/
#define	D_DU_NETCONF_CHAR_OF_PASS			256			/**< NETCONF pass 文字数		*/

#define	D_DU_NETCONF_USER_TABLE_SIZE		1696		/**< NETCONF User information Table size (shared memory)	*/

#define	D_DU_NETCONF_DEFAULT_USER			"ysjgmr"	/**< NETCONF Default User (63文字以下)	*/
/* @} */

/* ======================================================================== */
/* searchable-access-vlans information										*/
/* ======================================================================== */
/**
 * @brief	searchable-access-vlans information
 * @{
**/
#define	D_DU_SEARCHABLE_VLAN_MAX			512			/**< searchable-access-vlans領域サイズ	*/
/* @} */

/* ======================================================================== */
/* 外部IO情報テーブル定義(5G-商用DU)										*/
/* ======================================================================== */
/*!
 * @brief FAN location
 * @{  
 */
enum
{
	E_DU_FAN_1 = 0,										/**<  FAN#1						*/
	E_DU_FAN_2,											/**<  FAN#2						*/
	E_DU_FAN_3,											/**<  FAN#3						*/
	E_DU_FAN_MAXNO,										/**<  FAN数						*/
};
/* @} */


#define	D_DU_VAL_FAN_NORMAL					0			/**< FAN未実装&電源faultビットOFF	*/
#define	D_DU_BIT_FAN_UNMOUNT				1			/**< FAN未実装faultビットON			*/
#define	D_DU_BIT_FAN_PSALM					2			/**< FAN電源faultビットON			*/
#define	D_DU_BIT_FAN_FLTALL					3			/**< FAN未実装&電源faultビットON	*/

#define	D_DU_BIT_FAN_ALLOFF					0			/**> FAN#1~3全ビットOFF			*/
#define	D_DU_BIT_FAN1						1			/**> FAN#1ビットON				*/
#define	D_DU_BIT_FAN2						2			/**> FAN#1ビットON				*/
#define	D_DU_BIT_FAN3						4			/**> FAN#1ビットON				*/
#define	D_DU_BIT_FAN_ALLON					7			/**> FAN#1~3ビットON			*/

/*!
 * @brief FAN Speed ctrl status
 * @{  
 */
enum
{
	E_DU_FANSPEEDSTS_STOP = 0,							/**<  FAN停止状態				*/
	E_DU_FANSPEEDSTS_LOWDAC,							/**<  FAN低電圧状態				*/
/*	E_DU_FANSPEEDSTS_HIGHDAC,							*//**<  FAN高電圧状態				*//*	ハソv1.05削除	*/
/*	E_DU_FANSPEEDSTS_MAXDAC,							*//**<  FAN MAX状態				*//*	ハソv1.05削除	*/
	E_DU_FANSPEEDSTS_MAXNO,								/**<  FAN制御状態数				*/
};

#define	E_DU_FANSPEEDSTS_HIGHDAC			E_DU_FANSPEEDSTS_LOWDAC		/**> FAN高電圧状態			*//*	ハソv1.05変更(未使用)	*/
#define	E_DU_FANSPEEDSTS_MAXDAC				E_DU_FANSPEEDSTS_LOWDAC		/**> FAN MAX状態			*//*	ハソv1.05変更(未使用)	*/

/* @} */


/*!
 * @name hdlc rate bit
 * @note
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
#define D_RRH_HDLC_RATE_960			0x00030000			/* RE link HDLC RATE info is 960kbps	*/
#define D_RRH_HDLC_RATE_1920			0x00040000			/* RE link HDLC RATE info is 1920kbps	*/
#define D_RRH_HDLC_RATE_MASK			0x00070000			/* RE link HDLC RATE info is 1920kbps	*/

/*!
 * @name 装置構成情報のCPRIリンク装置構成情報
 * @note
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
 enum
{
	E_RRH_EQP_INF_EMP0 = 0,								/*!< 空 */
	E_RRH_EQP_INF_EMP1,									/*!< 空 */
	E_RRH_EQP_INF_NO,									/*!< 装置号番*/
	E_RRH_EQP_INF_RETYPE,								/*!< RE種別 */
	E_RRH_EQP_INF_3GBTS,								/*!< 3GBTS共用方式 */
	E_RRH_EQP_INF_EMP5,									/*!< 空 */
	E_RRH_EQP_INF_POWTYPE,								/*!< 電源種別 */
	E_RRH_EQP_INF_RCP_ANT,								/*!< 受信アンテナ数 */
	E_RRH_EQP_INF_TRA_ANT,								/*!< 送信アンテナ数 */
	E_RRH_EQP_INF_POWCLA_UNI,							/*!< パワークラス単位 */
	E_RRH_EQP_INF_POWCLA0,								/*!< パワークラス(0系) */
	E_RRH_EQP_INF_POWCLA1,								/*!< パワークラス(1系) */
	E_RRH_EQP_INF_POWCLA2,								/*!< パワークラス(2系) */
	E_RRH_EQP_INF_POWCLA3,								/*!< パワークラス(3系) */
	E_RRH_EQP_INF_EMP14,								/*!< 空 */
	E_RRH_EQP_INF_EMP15,								/*!< 空 */
	E_RRH_EQP_INF_FRQ,									/*!< 対応周波数帯 */
	E_RRH_EQP_INF_MAX_FRQ,								/*!< 最大周波数帯域幅 */
	E_RRH_EQP_INF_CAR_2G,								/*!< キャリア数(2GHz) */
	E_RRH_EQP_INF_CAR_15G,								/*!< キャリア数(1.5GHz) */
	E_RRH_EQP_INF_CAR_800,								/*!< キャリア数(800MHz) */
	E_RRH_EQP_INF_CAR_700,								/*!< キャリア数(700MHz) */
	E_RRH_EQP_INF_CAR_17,								/*!< キャリア数(1.7GHz) */
	E_RRH_EQP_INF_CAR_17_LOW,							/*!< キャリア数(1.7GHz Low Band) */
	E_RRH_EQP_INF_TRX_INF,								/*!< TRX-INFタイプ */
	E_RRH_EQP_INF_CAR_35,								/*!< キャリア数(3.5GHz) */
	E_RRH_EQP_INF_POWCLA4,								/*!< パワークラス(4系) */
	E_RRH_EQP_INF_POWCLA5,								/*!< パワークラス(5系) */
	E_RRH_EQP_INF_POWCLA6,								/*!< パワークラス(6系) */
	E_RRH_EQP_INF_POWCLA7,								/*!< パワークラス(7系) */
//	E_RRH_EQP_INF_EMP30,								/*!< 空 */
	E_RRH_EQP_INF_CAR_34,								/*!< キャリア数(3.4GHz) */
	E_RRH_EQP_INF_EMP31,								/*!< 空 */
	E_RRH_EQP_INF_EMP32,								/*!< 空 */
	E_RRH_EQP_INF_EMP33,								/*!< 空 */
	E_RRH_EQP_INF_EMP34,								/*!< 空 */
	E_RRH_EQP_INF_EMP35,								/*!< 空 */
	E_RRH_EQP_INF_EMP36,								/*!< 空 */
	E_RRH_EQP_INF_EMP37,								/*!< 空 */
	E_RRH_EQP_INF_EMP38,								/*!< 空 */
	E_RRH_EQP_INF_EMP39,								/*!< 空 */
	E_RRH_EQP_INF_EMP40,								/*!< 空 */
	E_RRH_EQP_INF_EMP41,								/*!< 空 */
	E_RRH_EQP_INF_EMP42,								/*!< 空 */
	E_RRH_EQP_INF_EMP43,								/*!< 空 */
	E_RRH_EQP_INF_EMP44,								/*!< 空 */
	E_RRH_EQP_INF_EMP45,								/*!< 空 */
	E_RRH_EQP_INF_EMP46,								/*!< 空 */
	E_RRH_EQP_INF_EMP47,								/*!< 空 */
	E_RRH_EQP_INF_EMP48,								/*!< 空 */
	E_RRH_EQP_INF_EMP49,								/*!< 空 */
	E_RRH_EQP_INF_EMP50,								/*!< 空 */
	E_RRH_EQP_INF_EMP51,								/*!< 空 */
	E_RRH_EQP_INF_EMP52,								/*!< 空 */
	E_RRH_EQP_INF_EMP53,								/*!< 空 */
	E_RRH_EQP_INF_EMP54,								/*!< 空 */
	E_RRH_EQP_INF_EMP55,								/*!< 空 */
	E_RRH_EQP_INF_EMP56,								/*!< 空 */
	E_RRH_EQP_INF_EMP57,								/*!< 空 */
	E_RRH_EQP_INF_EMP58,								/*!< 空 */
	E_RRH_EQP_INF_EMP59,								/*!< 空 */
	E_RRH_EQP_INF_EMP60,								/*!< 空 */
	E_RRH_EQP_INF_EMP61,								/*!< 空 */
	E_RRH_EQP_INF_EMP62,								/*!< 空 */
	E_RRH_EQP_INF_EMP63									/*!< 空 */
};
/* @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* 最後の有効エリアからE_RRH_EQP_INF_EMP63までの空エリア数を算出 */
#define D_RRH_EQP_INF_REM	(E_RRH_EQP_INF_EMP63 - E_RRH_EQP_INF_CAR_34)
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/*!
 * @name Laye3テーブルのRE状態
 * @note
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
 enum
{
	E_RRH_LAYER3_RE_IDLE =1,							/*!< 1:起動待ちアイドル状態 */
	E_RRH_LAYER3_RE_START,								/*!< 2:起動中状態 */
	E_RRH_LAYER3_RE_STOP,								/*!< 3:強制停止中 */
	E_RRH_LAYER3_RE_OPE,								/*!< 4:運用中状態 */
	E_RRH_LAYER3_RE_OPEIDLE,							/*!< 5:運用中アイドル状態 */
	E_RRH_LAYER3_REC_IDLE,								/*!< 6:REアイドル状態 */
	E_RRH_LAYER3_REC_OPE								/*!< 7:RE運用中状態 */
};

 /*!
  * @name layer_kind
  * @note
  * @date 2020/12/28 M&C) Merge 4G FHM src
  * @{
  */
 enum
 {
	 E_RRH_RAYER_KIND_1_2 = 0,							/*!< layer1 or 2 */
	 E_RRH_RAYER_KIND_3									/*!< layer3 */
 };

/* @} */

 /*!
  * @brief  強制停止要因
  * @date   2015/10/29 TDI)satou create
  * @date   2016/11/22 FJT)ohashi 16B スロット数制限機能追加
  * @date 2020/12/28 M&C) Merge 4G FHM src
  */
typedef enum
{
    E_RRH_RESTOP_FACTOR_MISMATCH_CARR,                  /*!< キャリア状態不一致 */
    E_RRH_RESTOP_FACTOR_DL_NG,                          /*!< ダウンロード失敗   */
    E_RRH_RESTOP_FACTOR_DELAY_NG,                       /*!< 遅延調整NG         */
    E_RRH_RESTOP_FACTOR_SLOT_NG,                        /*!< スロット数NG       */
    E_RRH_RESTOP_FACTOR_OTHER                           /*!< その他             */
} T_RRH_RESTOP_FACTOR;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */

#define D_RRH_TOFFSET_X_FHM			768
#define D_RRH_TOFFSET_Y_FHM			768

/* L1 inband reset wait 1sec */
#define D_RRH_L1INBAND_RESET_WAIT			1000
/* L1 inband reset retry counter */
#define D_RRH_L1INBAND_RETRY_MAX			3

/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/*!
 * @name TRX設定/TRX解放状態
 * @note
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
enum
{
	E_RRH_TRXSTA_IDLE,								/*!< 0:アイドル状態 */
	E_RRH_TRXSTA_SETING,							/*!< 1:TRX設定中状態 */
	E_RRH_TRXSTA_RELING,							/*!< 2:TRX解放中状態 */
};

/*********************************************************************************/
/**
 * @name SFP log
 * @{
 */
/*********************************************************************************/
/*!
 * @brief   取得契機
 * @date    2015/11/16 TDI)satou ハソ-QA-047 create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef enum {
    E_RRH_SFPLOG_TRIGGER_CARD_START = 1,            /*!< カード起動時               */
    E_RRH_SFPLOG_TRIGGER_CPRI_USE,                  /*!< CPRIリンク未使用→使用変化 */
    E_RRH_SFPLOG_TRIGGER_SFP_MOUNT,                 /*!< SFP未実装→実装変化        */
    E_RRH_SFPLOG_TRIGGER_CPRI_UNUSE                 /*!< CPRIリンク使用→未使用変化 */
} E_RRH_SFPLOG_TRIGGER;
/*!
 * @brief   サポート/未サポート
 * @date    2015/11/17 TDI)satou ハソ-QA-047 create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef enum {
    E_RRH_SFPLOG_SUPPORT_YES = 1,                   /*!< サポートするSFP            */
    E_RRH_SFPLOG_SUPPORT_NO,                        /*!< サポートしないSFP          */
} E_RRH_SFPLOG_SUPPORT;
/*!
 * @brief   SFP速度
 * @date    2015/11/17 TDI)satou ハソ-QA-047 create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef enum {
    E_RRH_SFPLOG_SPEED_98G_49G = 1,                 /*!< 9.8G/4.9G                  */
    E_RRH_SFPLOG_SPEED_24G,                         /*!< 2.4G                       */
    E_RRH_SFPLOG_SPEED_UNKNOWN                      /*!< 不明                       */
} E_RRH_SFPLOG_SPEED;
/** @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define D_RRH_SFPLOG_TOTAL_NUM 16
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */

/****************************************************************************************************************************/
/**
 * @name RE PORT状態報告
 * @date 2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
/****************************************************************************************************************************/
/** 共有メモリ(T_RRH_REPORTSTA_S3G) : RE PORT状態報告応答受信状態(未受信) */
#define D_RRH_REPORTSTA_RSP_NOT_RECEIVED 	(0)
/** 共有メモリ(T_RRH_REPORTSTA_S3G) : RE PORT状態報告応答受信状態(正常応答) */
#define D_RRH_REPORTSTA_RSP_NORMAL			(1)
/** 共有メモリ(T_RRH_REPORTSTA_S3G) : RE PORT状態報告応答受信状態(NG応答) */
#define D_RRH_REPORTSTA_RSP_NG				(2)
/** 共有メモリ(T_RRH_REPORTSTA_S3G) : RE PORT状態報告応答受信状態(タイムアウト) */
#define D_RRH_REPORTSTA_RSP_TO				(3)
/** 共有メモリ(T_RRH_REPORTSTA_S3G) : RE PORT状態報告応答受信状態(CPRIリンク断) */
#define D_RRH_REPORTSTA_RSP_CPRIDSC			(4)
/** @} */

#define	D_DU_FANDACVAL_STOP					0			/**<  FAN停止状態DAC値			*//*	テーブル初期化のみ実処理では未使用	*/
#define	D_DU_FANDACVAL_LOW					3456		/**<  FAN低電圧状態DAC値(TBD)	*//*	テーブル初期化のみ実処理では未使用	*/
/*#define	D_DU_FANDACVAL_HIGH					1024		*//**<  FAN高電圧状態DAC値(TBD)	*//*	ハソv1.05削除	*/
/*#define	D_DU_FANDACVAL_MAX					1024		*//**<  FAN MAX状態DAC値(TBD)		*//*	ハソv1.05削除	*/

#define D_DU_FANSPEED_MAX					127			/**<  FAN MAX回転数(3810[rpm])(TBD)				*/
#define D_DU_FANSPEED_CALRPM				30			/**<  FAN回転数rpm変換								*/
#define D_DU_THRESHOLD_FANSPEED				83			/**<  FAN回転数Fault閾値(1890[rpm]=D_EIO_FANSPEED_MAX/2)(TBD)	*/

/* ======================================================================== */
/* 外部IO制御ログテーブル定義(5G-商用DU)									*/
/* ======================================================================== */
#define	D_DU_EIOLOG_MAX						500			/**< 外部IO制御ログテーブル数	*/

/* ======================================================================== */
/* FHSTS																	*/
/* ======================================================================== */
#define	D_DU_FHSTATLOG_MAX					37	
#define	D_DU_FHSTATLOG_RCD_NUM				61	

/* ======================================================================== */
/* product-code																*/
/* ======================================================================== */
#define	D_DU_PRODUCT_CODE_37G_SUB6			"37GLLSDU"
#define	D_DU_PRODUCT_CODE_45G_SUB6			"45GLLSDU"
#define	D_DU_PRODUCT_CODE_280G_MMW			"28GLLSDU"

/* ======================================================================== */
/* MT INFO																	*/
/* ======================================================================== */
#define	D_DU_MTLOG_RCD_NUM					128
#define	D_DU_MTPORT_NO						60029

/* ======================================================================== */
/* TXPOW_DETECT																*/
/* ======================================================================== */
#define	D_DU_TXPOW_DETECT_HIS_RCD_NUM		64

enum
{
	E_DU_TXPOW_DETECT_V = 0,
	E_DU_TXPOW_DETECT_H,
	E_DU_TXPOW_DETECT_VH_NUM
};

#define	D_DU_TXPOW_DETECT_VH_MAX 16
#define	D_DU_TXPOW_DETECT_MAX 45


#define	D_DU_LUT_LOG_MAX 4


/*!
 * @name 主信号停波履歴
 * @note 
 * @{
 */
#define		D_DU_LOG_UPSIGSTOP_REG_NUM		7			/**< 3 + 4	*/
#define		D_DU_LOG_UPSIGSTOP_RCD_NUM		120			/**< 60s * 2 => 2時間	*/
/* @} */


/*!
 * @name FH L1履歴
 * @note 
 * @{
 */
#define		D_DU_LOG_FH_L1_HIS_REG_NUM		3
#define		D_DU_LOG_FH_L1_HIS_RCD_NUM		600			/**< 1s * 600 => 10min	*/
/* @} */

/*!
 * @name AISG情報
 * @note 
 * @{
 */
#define		D_DU_AIH_ALD_CTRL_DEV_MAX			0x100
#define		D_DU_AIH_ALD_CTRL_ADDR_ALL			0xFF
#define		D_DU_AIH_ALD_CTRL_FRAME_TYPE_I		0x0
#define		D_DU_AIH_ALD_CTRL_FRAME_TYPE_S		0x1
#define		D_DU_AIH_ALD_CTRL_FRAME_TYPE_U		0x2
#define		D_DU_AIH_ALD_CTRL_FRAME_TYPE_T		0x3
#define		D_DU_AIH_ALD_CTRL_FRAME_TYPE_E		0x4
#define		D_DU_AIH_ALD_CTRL_FRAME_TYPE_NUM	0x5

#define		D_DU_AIH_ALD_CTRL_DIR_TYPE_SND		0x0
#define		D_DU_AIH_ALD_CTRL_DIR_TYPE_RCV		0x1
#define		D_DU_AIH_ALD_CTRL_DIR_TYPE_TO		0x2
#define		D_DU_AIH_ALD_CTRL_DIR_TYPE_ERR		0x3

#define		D_DU_AIH_ALD_CTRL_UIDLEN			19

#define		D_DU_AIH_ALD_CTRL_DATA_MAX			1200

#define		D_DU_AIH_ALD_CTRL_LOG_SUPP_TO		1
#define		D_DU_AIH_ALD_CTRL_LOG_SUPP_ERR		2
#define		D_DU_AIH_ALD_CTRL_LOG_SUPP_DEVS		4

/* @} */

/*!
 * @name CAL履歴
 * @note 
 * @{
 */
#define		D_DU_LOG_CAL_HIS_FAST_NUM		3
#define		D_DU_LOG_CAL_HIS_SLOW_NUM		120
#define		D_DU_LOG_CAL_HIS_ERR_NUM		60

#define		D_DU_LOG_CAL_TYPE_SLOW			0
#define		D_DU_LOG_CAL_TYPE_FAST			1
#define		D_DU_LOG_CAL_TYPE_SLOW_ERR		2
#define		D_DU_LOG_CAL_TYPE_FAST_ERR		3
#define		D_DU_LOG_CAL_TYPE_MAX			4

/*!
 * @name DLF統計情報履歴
 * @note 
 * @{
 */
#define		D_DU_LOG_DLF_HIS_TIME					120 /* 60s*120=2時間分 					*/
#define		D_DU_LOG_DLF_HIS_TIME_MIN				60  /* 1s*60=1分						*/
#define		D_DU_LOG_DLF_HIS_GRP_NUM				15	/* DLF統計情報グループ数		 	*/
#define		D_DU_LOG_DLF_HIS_ADD_NUM				42	/* 偏波別DLF統計情報アドレス数 		*/
#define		D_DU_LOG_DLF_HIS_ADD_INC				4	/* アドレスオフセット 				*/

/* @} */


#endif

/*!
 * @skip  $ld:$
 * @file  f_cmn_com_def.h
 * @brief 共通関数定数定義ヘッダ
 * @date  2013/11/26 ALPHA) 近藤 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */
#ifndef F_CMN_COM_DEF_H
#define F_CMN_COM_DEF_H


/*!
 * @name アボート処理
 * @note アボート処理
 * @{
 */
#define  f_com_abort( processID, almCode ) f_cmn_com_abort( processID, almCode, __FILE__, __LINE__)

/* @} */

 /*!
 * @name 関数処理結果.
 * @note 各関数の処理結果
 * @{
 */
#define D_COM_FTP_PRELIM				1		 		/*!< FTP準備時						*/
#define D_COM_FTP_COMPLETE				2		 		/*!< FTP完了時						*/
#define D_COM_FTP_COMNTINUE				3		 		/*!< FTP継続時						*/
#define D_COM_FTP_TRANSIENT				4		 		/*!< FTP TRANSIENT時				*/
#define D_COM_FTP_ERROR					5		 		/*!< FTP異常時						*/
#define D_COM_FTP_NOT_FOUND				0xFFFFFFF0		/*!< FTPサーバ未発見				*/
/* @} */

 /*!
 * @name 共通関数 APIエラーオプション値
 * @note 共通関数でAPIエラー検出時、アボート関数へ渡すオプション値定義
 * @{
 */
#define D_COM_API_TSK_SPAWN		0x000F0001			/*!< f_com_taskSpawn		*/
#define D_COM_API_TSK_DLT		0x000F0002			/*!< f_com_taskDelete		*/
#define D_COM_API_TSK_DLY		0x000F0003			/*!< f_com_taskDelay		*/
#define D_COM_API_MSG_CRE		0x000F0004			/*!< f_com_msgQCreate		*/
#define D_COM_API_MSG_REC		0x000F0005			/*!< f_com_msgQReceive		*/
#define D_COM_API_MSG_SND		0x000F0006			/*!< f_com_msgQSend			*/
#define D_COM_API_MLC			0x000F0007			/*!< f_com_malloc			*/
#define D_COM_API_WD_CRE		0x000F0008			/*!< f_com_wdCreate			*/
#define D_COM_API_WD_DLT		0x000F0009			/*!< f_com_wdDelete			*/
#define D_COM_API_WD_STR		0x000F000A			/*!< f_com_wdStart			*/
#define D_COM_API_SEM_CRE		0x000F000B			/*!< f_com_semMCreate		*/
#define D_COM_API_SEM_TAK		0x000F000C			/*!< f_com_semTake			*/
#define D_COM_API_SEM_GIV		0x000F000D			/*!< f_com_semGive			*/
#define D_COM_API_CLK_GET		0x000F000E			/*!< f_com_clock_gettime	*/
#define D_COM_API_CLK_SET		0x000F000F			/*!< f_com_clock_settime	*/
#define D_COM_API_LGI_USR_DLT	0x000F0010			/*!< f_com_loginUserDelete	*/
#define D_COM_API_TRP_ARE		0x000F0011			/*!< f_com_TrapSend_ARE		*/
#define D_COM_API_TRP_AVC		0x000F0012			/*!< f_com_TrapSend_AVC		*/
#define D_COM_API_TRP_ME		0x000F0013			/*!< f_com_TrapSend_ME		*/
#define D_COM_API_TRP_STR		0x000F0014			/*!< f_com_TrapSend_STR		*/
#define D_COM_API_WD_CAN		0x000F0015			/*!< f_com_wdCancel			*/
/* @} */

/*!
 * @name API引数用
 * @note API引数用義
 * @{
 */
#define D_COM_MSG_PRI_NORMAL			0			/*!< msgQSend priority NORMAL	*/
#define D_COM_MSG_PRI_URGENT			1			/*!< msgQSend priority URGENT	*/
/* @} */

/*!
 * @name BCD変換関連
 * @note BCD変換関連定義
 * @{
 */
#define D_COM_BCDCONV_UPPER			0				/*!< BCDデータから大文字ASCII変換*/
#define D_COM_BCDCONV_LOWER			1				/*!< BCDデータから小文字ASCII変換	*/
/* @} */

/*!
 * @name LOG関連
 * @note LOG関連定義
 * @{
 */
#define D_COM_ALM_ANT_INFO_NUM		20				/*!< ログ関連定義				*/
/* @} */

/*!
 * @name MMI関連
 * @note MMI関連定義
 * @{
 */
#define D_COM_MMI_CMD_LOG_NUM			32				/*!< MMI command log保持最大値	*/
#define D_COM_PKT_LOG_NUM				512				/*!< Ether Packet log最大値		*/
#define D_COM_PKT_LOG_SIZE				1536			/*!< Ether Packet log1Packet 最大サイズ	*/
#define D_COM_CPRI_ETH_PKT_SND			0x01			/*!< CpriEther送信Packet		*/
#define D_COM_CPRI_ETH_PKT_RCV			0x02			/*!< CpriEther受信Packet		*/
/* @} */

/*!
 * @name 障害ログ関連
 * @note 障害ログ関連定義
 * @{
 */
#define D_COM_SEM_ID					"#COM SEM ID"					/*!< f_comw_semId				*/
#define D_COM_MSGQ_ID					"#COM MSGQ ID"					/*!< f_comw_msgQId				*/
#define D_COM_TIM_ID					"#COM TIM ID"					/*!< f_comw_timId				*/
#define D_COM_CRL_TAG					"#COM CRL TAG"					/*!< f_comw_crlTag				*/
#define D_COM_INV_MNG					"#COM INV MNG"					/*!< f_comw_inv_mng				*/
#define D_COM_LOAD_MNG					"#COM LOAD MNG"					/*!< f_comw_load_mng			*/
#define D_COM_SYS_TIM_INFO				"#COM SYS TIM INFO"				/*!< f_comw_systemTimeInfo		*/
#define D_COM_TASK_ID					"#COM TASK ID"					/*!< f_comw_comw_taskId			*/
#define D_COM_FLASH_WRITE_FLG			"#COM FLASH WRITE FLG"			/*!< f_comw_flashWriteFlag		*/
#define D_COM_STA_CHG_ABV_B_FLG			"#COM STA CHG ABV B FLG"		/*!< 		*/
#define D_COM_SYS_DHCP_ADDR				"#COM SYS DHCP ADDR"			/*!< f_comw_dhcpAddrTbl			*/
#define D_COM_STAT_F_FLAG				"#COM STAT F FLAG"				/*!< f_comw_StafFlg				*/
#define D_COM_STAT_E_FLAG				"#COM STAT E FLAG"				/*!< f_comw_StaEFlg				*/
#define D_COM_PREMAJOR_MINOR_STS		"#COM PREMAJOR MINOR STS"		/*!< f_comw_PreMajorMinorSts	*/

#define D_COM_FMG_TASK_STATE			"#FMG TASK STATE"				/*!< f_fmgw_taskstate			*/
#define D_COM_FMG_DL_FILE_STRAGE		"#FMG DL FILE STRAGE"			/*!< f_fmgw_dlFileStrage		*/

#define D_COM_FRW_TASK_STATE			"#FRW TASK STATE"				/*!< f_frww_taskState			*/

#define D_COM_FTP_TASK_STATE			"#FTP TASK STATE"				/*!< f_ftpw_taskstate			*/

#define D_COM_INI_TASK_STATE			"#INI TASK STATE"				/*!< f_iniw_taskstate			*/
#define D_COM_INI_DIAG_STATE			"#INI DIAG STATE"				/*!< f_iniw_diagState			*/
#define D_COM_INI_TASK_INI_MNG			"#INI TASK INI MNG"				/*!< f_iniw_taskIniMng			*/
#define D_COM_RCP_TASK_STATE			"#RCP TASK STATE"				/*!< f_rcpw_uiTaskState			*/
#define D_COM_RCP_INTERNAL_EVENT		"#RCP INTERNAL EVENT"			/*!< f_rcpw_uiInternalEvent		*/
#define D_COM_RCP_FIR_2_FLG				"#RCP FIR 2 FLG"				/*!< f_rcpw_tfir2Flg			*/
#define D_COM_RCP_LOC_CAR				"#RCP LOC CAR"					/*!< f_rcpw_tLocCar				*/
#define D_COM_RCP_RUN_HIS_TBL			"#RCP RUN HIS TBL"				/*!< f_rcpw_tRunHisTbl			*/

#define D_COM_RCP_RSSICOL      			"#RCP RSSICol"					/*!< f_rcpw_tRSSICol     		*/
#define D_COM_RCP_VSWRCOL      			"#RCP VSWRCol"					/*!< f_rcpw_tVSWRCol     		*/
#define D_COM_RCP_uiCollectVSWR			"#RCP CollectVSWR"				/*!< f_rcpw_uiCollectVSWR		*/
#define D_COM_RCP_ALMVSWR      			"#RCP AlmVSWR"					/*!< f_rcpw_uiAlmVSWR    		*/

#define D_COM_RSV_TASK_STATE			"#RSV TASK STATE"				/*!< f_rsvw_taststate			*/
#define D_COM_RSV_EVENT_NO				"#RSV IVENT NO"					/*!< f_rsvw_extEventNo			*/
#define D_COM_RSV_MAPPING_DI			"#RSV MAPPING DI"				/*!< f_rsvw_mapping_DI			*/
#define D_COM_RSV_RUN_HIS_TBL			"#RSV RUN HIS TBL"				/*!< f_rsvw_tRunHisTbl			*/
#define D_COM_RSV_ALARM_BIT_MAP			"#RSV ALARM BIT MAP"			/*!< f_rsvw_alarmBitMap			*/

#define D_COM_SVL_TASK_STATE			"#SVL TASK STATE"				/*!< f_svlw_taskState			*/
#define D_COM_SVL_TIMER_NUM				"#SVL TIMER NUM"				/*!< f_svlw_svltimernum			*/

#define D_COM_TMG_TASK_STATE			"#TMG TASK STATE"				/*!< f_tmgw_taskState			*/
#define D_COM_TMG_SYS_TIM_INFO			"#TMG SYS TIM INFO"				/*!< f_tmgw_sysTimInfo			*/
#define D_COM_TELNET_LOGIN_INFO			"#TELNET LOGIN INFO"			/*!< f_comw_telnetLog			*/
#define D_COM_COMMAND_HIS_LOG			"#COMMAND HISTORY LOG"			/*!< command histroy			*/

#define D_COM_LOG_TASK_STATE			"#LOG TASK STATE"				/*!< f_logw_taskState			*/

#define D_COM_FMG_RUN_HIS_TBL			"#FMG RUN HIS TBL"				/*!< f_fmgw_runHisTbl			*/
#define D_COM_FRW_RUN_HIS_TBL			"#FRW RUN HIS TBL"				/*!< f_frww_runHisTbl			*/
#define D_COM_LOG_RUN_HIS_TBL			"#LOG RUN HIS TBL"				/*!< f_logw_runHisTbl			*/

#define D_COM_AISGDEV					"#COM AISG Dev TBL"				/*!< f_comw_aisgDev				*/
#define D_COM_AL7_TASK_STATE			"#AL7 TASK STATE"				/*!< f_al7w_uiTaskState			*/
#define D_COM_AL7_RUN_HIS_TBL			"#AL7 RUN HIS TBL"				/*!< f_al7w_tRunHisTbl			*/
#define D_COM_AL7_AISG_TIM_FLAG			"#AL7 AISG TIM TBL"				/*!< f_al7w_uiAutoclsTimerFlg	*/
#define D_COM_AL2_MSG_HIS_TBL			"#AL2 MSG HIS TBL"				/*!< f_al2w_msgRunHisTbl		*/
#define D_COM_AL2_FRM_HIS_TBL			"#AL2 FRM HIS TBL"				/*!< f_al2w_frmRunHisTbl		*/
#define D_COM_AL2_INFMNG_TBL			"#AL2 INF MNG TBL"				/*!< f_al2w_infMngTbl			*/

#define D_COM_VUPU_RUN_HIS_TBL			"#VUPU RUN HIS LOG"				/*!< f_vupuw_runHisTbl			*/
#define D_COM_VUPU_ALM_LOG				"#VUPU ALM LOG"					/*!< f_vupuw_almLog				*/
/* @} */

/*!
 * @name Telnet Login Info Log関連
 * @note Telnet Login Info Log関連定義
 * @{
 */
#define D_COM_TELNET_LOG_MAX			32								/*!< Telnet Login Info ログ最大数		*/
/* @} */

/*!
 * @name 時刻変換関連
 * @note 時刻変換関連定義
 * @{
 */
#define D_COM_YEAR_1900					1900							/*!< 1900年								*/
#define D_COM_YEAR_1970					(1970-D_COM_YEAR_1900)			/*!< 1970年								*/
#define D_COM_YEAR_2030					(2030-D_COM_YEAR_1900)			/*!< 2030年								*/
#define D_COM_YEAR_2090					(2090-D_COM_YEAR_1900)			/*!< 2090年								*/
#define D_COM_YEAR_2100					(2100-D_COM_YEAR_1900)			/*!< 2100年								*/

#define D_COM_TIM_OFFSET_2030			0x70DBD880						/*!< 1970年～2030年の1secインクリ値		*/
#define D_COM_TIM_OFFSET_2090			0xE1B7B100						/*!< 1970年～2090年の1secインクリ値		*/
#define D_COM_TIM_OFFSET_2100			0xF4865700						/*!< 1970年～2100年の1secインクリ値		*/
/* @} */

/*!
 * @name I2C関連
 * @note I2C関連定義
 * @{
 */
/* デバイス情報 */
#define D_COM_I2C_DV_OPT0_A0H			BPF_HM_DEVC_I2C_DV_OPT0_A0H		/*!< デバイスID(A0h)					*/
#define D_COM_I2C_DV_OPT0_A2H			BPF_HM_DEVC_I2C_DV_OPT0_A2H		/*!< デバイスID(A2h)					*/
#define D_COM_I2C_DV_OPT1_A0H			BPF_HM_DEVC_I2C_DV_OPT1_A0H		/*!< デバイスID(xxh)					*/
#define D_COM_I2C_DV_OPT1_A2H			BPF_HM_DEVC_I2C_DV_OPT1_A2H		/*!< デバイスID(xxh)					*/
#define D_COM_I2C_DV_RF_EEP				BPF_HM_DEVC_I2C_DV_RF_EEP		/*!< RF-EEP予約							*/
#define D_COM_I2C_DV_RF_CPLD			BPF_HM_DEVC_I2C_DV_RF_CPLD		/*!< RF-cPLD							*/
#define D_COM_I2C_DV_OVSERV				BPF_HM_DEVC_I2C_DV_OVSERV		/*!< 電圧電流監視						*/

/* アドレス情報 */
#define D_COM_I2C_AD_TEMP				0xF0							/*!< 温度								*/
#define D_COM_I2C_AD_SET_VI				0x06							/*!< 電圧電流測定設定					*/
#define D_COM_I2C_AD_READ_INTENSITY		0x00							/*!< 電流リード							*/
#define D_COM_I2C_AD_READ_VOLT			0x02							/*!< 電圧リード							*/

/* ライトデータ情報 */
#define D_COM_I2C_DT_INTENSITY			0x0004							/*!< 電流								*/
#define D_COM_I2C_DT_VOLT				0x0024							/*!< 電圧								*/
/* @} */

/*!
 * @name 性能測定関連
 * @note 性能測定関連定義
 * @{
 */
#define D_COM_LABEL_SW_START				"SW Start"						/*!< Label "SW Start"					*/
#define D_COM_LABEL_VUPU_ERASE_START		"VUPU Erase Start"				/*!< Label "VUPU Erase Start"			*/
#define D_COM_LABEL_VUPU_ERASE_END			"VUPU Erase End"				/*!< Label "VUPU Erase End"				*/
#define D_COM_LABEL_VUPU_WRITE_START		"VUPU Write Start"				/*!< Label "VUPU Write Start"			*/
#define D_COM_LABEL_VUPU_WRITE_END			"VUPU Write End"				/*!< Label "VUPU Write End"				*/
#define D_COM_LABEL_DIA_START				"Dia Start"						/*!< Label "Dia Start"					*/
#define D_COM_LABEL_DIA_END					"Dia End"						/*!< Label "Dia End"					*/
#define D_COM_LABEL_ALL_TASKS_LAUNCHED		"All Tasks Launched"			/*!< Label "All Tasks Launched"			*/
#define D_COM_LABEL_STATE_E					"State E"						/*!< Label "State E"					*/
#define D_COM_LABEL_SW_INIT_COMPLETION		"SW Init Completion"			/*!< Label "SW Init Completion"			*/
#define D_COM_LABEL_DHCP_COMPLETION			"DHCP completion"				/*!< Label "DHCP completion"			*/
#define D_COM_LABEL_LM_CHKSUM_START1		"LM Check Sum Start 1"			/*!< Label "LM Check Sum Start 1"		*/
#define D_COM_LABEL_LM_CHKSUM_END1			"LM Check Sum End 1"			/*!< Label "LM Check Sum End 1"			*/
#define D_COM_LABEL_LM_CHKSUM_START2		"LM Check Sum Start 2"			/*!< Label "LM Check Sum Start 2"		*/
#define D_COM_LABEL_LM_CHKSUM_END2			"LM Check Sum End 2"			/*!< Label "LM Check Sum End 2"			*/
#define D_COM_LABEL_CPRI_CHKSUM_START1		"CPRI Check Sum Start 1"		/*!< Label "CPRI Check Sum Start 1"		*/
#define D_COM_LABEL_CPRI_CHKSUM_END1		"CPRI Check Sum End 1"			/*!< Label "CPRI Check Sum ENd 1"		*/
#define D_COM_LABEL_CPRI_CHKSUM_START2		"CPRI Check Sum Start 2"		/*!< Label "CPRI Check Sum Start 2"		*/
#define D_COM_LABEL_CPRI_CHKSUM_END2		"CPRI Check Sum End 2"			/*!< Label "CPRI Check Sum ENd 2"		*/
#define D_COM_LABEL_APD_CHKSUM_START1		"APD Check Sum Start 1"			/*!< Label "APD Check Sum Start 1"		*/
#define D_COM_LABEL_APD_CHKSUM_END1			"APD Check Sum End 1"			/*!< Label "APD Check Sum ENd 1"		*/
#define D_COM_LABEL_APD_CHKSUM_START2		"APD Check Sum Start 2"			/*!< Label "APD Check Sum Start 2"		*/
#define D_COM_LABEL_APD_CHKSUM_END2			"APD Check Sum End 2"			/*!< Label "APD Check Sum ENd 2"		*/
#define D_COM_LABEL_VUPU_CHKSUM_START1		"VUPU Check Sum Start 1"		/*!< Label "VUPU Check Sum Start 1"		*/
#define D_COM_LABEL_VUPU_CHKSUM_END1		"VUPU Check Sum End 1"			/*!< Label "VUPU Check Sum ENd 1"		*/
#define D_COM_LABEL_VUPU_CHKSUM_START2		"VUPU Check Sum Start 2"		/*!< Label "VUPU Check Sum Start 2"		*/
#define D_COM_LABEL_VUPU_CHKSUM_END2		"VUPU Check Sum End 2"			/*!< Label "VUPU Check Sum ENd 2"		*/
#define D_COM_LABEL_BOOT_CHKSUM_START1		"BOOT Check Sum Start 1"		/*!< Label "BOOT Check Sum Start 1"		*/
#define D_COM_LABEL_BOOT_CHKSUM_END1		"BOOT Check Sum End 1"			/*!< Label "BOOT Check Sum ENd 1"		*/
#define D_COM_LABEL_BOOT_CHKSUM_START2		"BOOT Check Sum Start 2"		/*!< Label "BOOT Check Sum Start 2"		*/
#define D_COM_LABEL_BOOT_CHKSUM_END2		"BOOT Check Sum End 2"			/*!< Label "BOOT Check Sum ENd 2"		*/
#define D_COM_LABEL_F0_CHKSUM_START			"Flash0 Check Sum Start"		/*!< Label "Flash0 Check Sum Start"		*/
#define D_COM_LABEL_F0_CHKSUM_END			"Flash0 Check Sum End"			/*!< Label "Flash0 Check Sum ENd"		*/
#define D_COM_LABEL_F1_CHKSUM_START			"Flash1 Check Sum Start"		/*!< Label "Flash1 Check Sum Start"		*/
#define D_COM_LABEL_F1_CHKSUM_END			"Flash1 Check Sum End"			/*!< Label "Flash1 Check Sum ENd"		*/


#define D_COM_LABEL_DL_START				"DL Start"						/*!< Label "DL Start"					*/
#define D_COM_LABEL_DL_LOADING				"DL Loading"					/*!< Label "DL Loading"					*/
#define D_COM_LABEL_FTP_START				"FTP Start"						/*!< Label "FTP Start"					*/
#define D_COM_LABEL_FTP_END					"FTP End"						/*!< Label "FTP End"					*/
#define D_COM_LABEL_DL_LOADED				"DL Loaded"						/*!< Label "DL Loaded"					*/
#define D_COM_LABEL_SW_DECOMPRESS_START		"SW Decompress Start"			/*!< Label "SW Decompress Start"		*/
#define D_COM_LABEL_SW_DECOMPRESS_END		"SW Decompress End"				/*!< Label "SW Decompress End"			*/
#define D_COM_LABEL_FLASH_FW_CLR_START		"Flash(FW) Clr Start"			/*!< Label "Flash(FW) Clr Start"		*/
#define D_COM_LABEL_FLASH_FW_CLR_END		"Flash(FW) Clr End"				/*!< Label "Flash(FW) Clr End"			*/
#define D_COM_LABEL_FLASH_FPGA_CLR_START	"Flash(FPGA) Clr Start"			/*!< Label "Flash(FPGA) Clr Start"		*/
#define D_COM_LABEL_FLASH_FPGA_CLR_END		"Flash(FPGA) Clr End"			/*!< Label "Flash(FPGA) Clr End"		*/
#define D_COM_LABEL_FLASH_APD_CLR_START		"Flash(APD) Clr Start"			/*!< Label "Flash(APD) Clr Start"		*/
#define D_COM_LABEL_FLASH_APD_CLR_END		"Flash(APD) Clr End"			/*!< Label "Flash(APD) Clr End"			*/
#define D_COM_LABEL_FLASH_VUPU_CLR_START	"Flash(VUPU) Clr Start"			/*!< Label "Flash(VUPU) Clr Start"		*/
#define D_COM_LABEL_FLASH_VUPU_CLR_END		"Flash(VUPU) Clr End"			/*!< Label "Flash(VUPU) Clr End"		*/
#define D_COM_LABEL_FLASH_FW_WRT_START		"Flash(FW) Wrt Start"			/*!< Label "Flash(FW) Wrt Start"		*/
#define D_COM_LABEL_FLASH_FW_WRT_END		"Flash(FW) Wrt End"				/*!< Label "Flash(FW) Wrt End"			*/
#define D_COM_LABEL_FLASH_FPGA_WRT_START	"Flash(FPGA) Wrt Start"			/*!< Label "Flash(FPGA) Wrt Start"		*/
#define D_COM_LABEL_FLASH_FPGA_WRT_END		"Flash(FPGA) Wrt End"			/*!< Label "Flash(FPGA) Wrt End"		*/
#define D_COM_LABEL_FLASH_APD_WRT_START		"Flash(APD) Wrt Start"			/*!< Label "Flash(APD) Wrt Start"		*/
#define D_COM_LABEL_FLASH_APD_WRT_END		"Flash(APD) Wrt End"			/*!< Label "Flash(APD) Wrt End"			*/
#define D_COM_LABEL_FLASH_VUPU_WRT_START	"Flash(VUPU) Wrt Start"			/*!< Label "Flash(VUPU) Wrt Start"		*/
#define D_COM_LABEL_FLASH_VUPU_WRT_END		"Flash(VUPU) Wrt End"			/*!< Label "Flash(VUPU) Wrt End"		*/
#define D_COM_LABEL_DL_READY				"DL Ready"						/*!< Label "DL Ready"					*/

#define D_COM_LABEL_EVENT_LOG_WRT_START		"Event Log Wrt Start"			/*!< Label "Event Log Wrt Start"		*/
#define D_COM_LABEL_EVENT_LOG_WRT_END		"Event Log Wrt End"				/*!< Label "Event Log Wrt End"			*/

#define D_COM_LABEL_RESET_START				"Reset Start"					/*!< Label "Reset Start"				*/
#define D_COM_LABEL_RESET_END				"Reset End"						/*!< Label "Reset End"					*/
/* @} */

/*!
 * @name hw alarm log関連
 * @note hw alarm log関連定義
 * @{
 */
#define D_COM_HW_ALM_LOG_NUM				10                      /*!< the max number of hw alarm log             */
#define D_COM_HW_ALM_LOG_ALM_NUM			28                      /*!< the max number of hw alarm log's alarm     */
/* @} */


/* FFCS add begin */
/*!
 * @name The flag that indicate delaying or not in function f_com_criSWalm
 * @note The flag that indicate delaying or not in function f_com_criSWalm define
 * @{
 */
#define D_COM_DELAY_FORBID              0x0             /*!<  forbit to delay task                                */
#define D_COM_DELAY_PERMIT              0x1             /*!<  permit to delay task                                */
/* @} */

/*!
 * @name The delay time before reset
 * @note The delay time before reset define
 * @{
 */
#define D_COM_DELAY_SEND_TICKS          1000/10         /*!<  delay time for sending alarm record message(1s)     */
#define D_COM_DELAY_FLASH_SEC           0x3C            /*!<  delay time for writing log to flash(1m)             */
#define D_COM_DELAY_DL_COUNT            30              /*!<  delay count for file download before reset          */
#define D_COM_DELAY_DL_TICKS            2*1000/10       /*!<  delay time in each counter for file download(2s)    */
/* @} */

/*!
 * @name Definition used in debug log function
 * @note Definition used in debug log function
 * @{
 */
#define D_COM_LOG_TASKNO_MAX            40              /*!<  Max task number                                     */
#define D_COM_LOG_BUFFER_MAX            100*1024        /*!<  Max size of buffer for BSP log (100k)               */
#define D_COM_LOG_SFALM_CAUSE           0x0001          /*!<  ALM要因, ソフトALMの場合                            */
/* @} */

/*!
 * @name The flag that indicate alarm setting or clearing
 * @note The flag that indicate alarm setting or clearing define
 * @{
 */
#define D_COM_ALM_CLR                   0x0             /*!<  clear an alarm                                      */
#define D_COM_ALM_SET                   0x1             /*!<  set an alarm                                        */
/* @} */

/*!
 * @name The status of an alarm in btsDevAlarmStatus
 * @note The status of an alarm in btsDevAlarmStatus define
 * @{
 */
#define D_COM_ALMSTS_CLR                0x00             /*!<  clear an alarm                                     */
#define D_COM_ALMSTS_SET_D1             0x01             /*!<  set an alarm for detector 1                        */
#define D_COM_ALMSTS_UNDEFINED          0xFF             /*!<  alarm-not defined                                  */
/* @} */

/*!
 * @name The Perceived Severity of an alarm
 * @note The Perceived Severity of an alarm define
 * @{
 */
#define D_COM_ALM_CRITICAL              0X01             /*!<  critical alarm                                     */
#define D_COM_ALM_MAJOR                 0X02             /*!<  major alarm                                        */
#define D_COM_ALM_MINOR                 0X03             /*!<  minor alarm                                        */
#define D_COM_ALM_WARNING               0X04             /*!<  warning                                            */
#define D_COM_ALM_CLEAR                 0X05             /*!<  alarm is cleared                                   */
/* @} */

/*!
 * @name The max index of common rom table
 * @note The max index of common rom table define
 * @{
 */
#define D_COM_ALM_CRSW_MAX                44                        /*!< the number of critical software alarms   */
#define D_COM_ALM_CRHW_MAX                40                        /*!< the number of critical hardware alarms   */
#define D_COM_ALM_MJ_CRI_MAX              6                          /*!< the number of major-critical alarm mapping  */
/* @} */

/*!
 * @name The Maximum Assrt log record
 * @note The Maximum Assrt log record define
 * @{
 */
#define D_COM_MAX_ASSERTLOGNUM            256                       /*!< Maximum assert log record                */
#define D_COM_MAX_ASSERTLOGBODY           64                        /*!< Maximum assert log body                  */
/* @} */

/*!
 * @name The Maximum Task IF log record
 * @note The Maximum Task IF log record define
 * @{
 */
#define D_COM_MAX_TASKIFNUM               256                       /*!< Maximum event log record                 */
#define D_COM_MAX_PERIODTSKIFLOGNUM       512                       /*!< Maximum Period event log record          */
/* @} */

/*!
 * @name The Maximum running history record
 * @note The Maximum running history record define
 * @{
 */
#define D_COM_MAX_RUNHISREC                32                      /*!< Maximum running history record            */
#define D_COM_MAX_RUNHISBODY              128                      /*!< Maximum running history body              */
/* @} */

/*
 * @name Sending or receipt flag
 * @note Sending or receipt flag define
 * @{
 */
#define D_COM_RUNHIS_FLG_RCV              0x00                     /*!< Sending or receipt flag(receipt)          */
#define D_COM_RUNHIS_FLG_SND              0x01                     /*!< Sending or receipt flag(Sending)          */
/* @} */

/*
 * @name The offset between btsDevAlarmStatus[x] and alarm
 * @note The offset between btsDevAlarmStatus[x] and alarm ID define
 * @{
 */
#define D_COM_ALMID_OFFSET                4500                     /*!< btsDevAlarmStatus[x] + 4500 = Alarm ID    */
/* @} */

/*!
 * @name The max number of external alarm
 * @name The max number of external alarm define
 * @{
 */
#define D_COM_ALMMAX_EXT                  256                      /*!< the max number of external alarm          */
/* @} */

/*
 *  source flag for function f_rsv_almProc
 */
#define D_COM_SRC_CARD_INIT           		1                        /*!< 1: Card init indication                   */
#define D_COM_SRC_IRQ0						2                        /*!< 0: IRQ0                                   */
#define D_COM_SRC_IRQ2						4                        /*!< 2: IRQ2                                   */
#define D_COM_SRC_IRQ4						8                        /*!< 4: IRQ4                                   */
/* @} */

/*!
 * @name The max number of bsp log
 * @name The max number of bsp log define
 * @{
 */
#define D_COM_BSPLOG_MAX                  100*1024                 /*!< the max number of bsp log                 */
/* @} */

/* @} */

/*!                                                   
* @name Alarm Record Event Event Type
* @note Alarm Record Event Event Typeの定義
* @{
*/
#define D_COM_COMMUNICATIONS_ALARM      0x02         /*!< communications-alarm                         */
#define D_COM_ENVIRONMENTAL_ALARM       0x03         /*!< environmental-alarm                          */
#define D_COM_EQUIPMENT_ALARM           0x04         /*!< equipment-alarm                              */
#define D_COM_PROCESSING_ERROR_ALARM    0x10         /*!< processing-error-alarm                       */
#define D_COM_QOS_ALARM                 0x11         /*!< quality-of-service-alarm                     */
#define D_COM_SECURITY_MECHANISM_ALARM  0x13         /*!< security-service-or-mechanismviolation-alarm */
/* @} */

/*!
* @name Alarm Record Event Probable Cause
* @note Alarm Record Event Probable Causeの定義
* @{
*/
#define D_COM_CAS_COMM_PROTOC_ERR       5            /*!< communications-protocol-error (5)            */
#define D_COM_CAS_COMM_SUBSYS_FAIL      6            /*!< communications-subsystem-failure (6)         */
#define D_COM_CAS_CONF_CUSTOM_ERR       7            /*!< configuration-or-customization-error (7)     */
#define D_COM_CAS_ENCLOSE_DOOR_OPEN     14           /*!< enclosure-door-open (14)                     */
#define D_COM_CAS_EQUIP_MALFUNCTION     15           /*!< equipment-malfunction (15)                   */
#define D_COM_CAS_FILE_ERROR            17           /*!< file-error (17)                              */
#define D_COM_CAS_COOLING               21           /*!< heating-ventilation-cooling (21)             */
#define D_COM_CAS_LOF                   28           /*!< loss-of-frame (28)                           */
#define D_COM_CAS_LOS                   29           /*!< loss-of-signal (29)                          */
#define D_COM_CAS_OUT_OF_MEM            32           /*!< out-of-memory (32)                           */
#define D_COM_CAS_OUTPUT_DEV_ERR        33           /*!< output-device-error (33)                     */
#define D_COM_CAS_PERFOM_DEGRADE        34           /*!< performance-degraded (34)                    */
#define D_COM_CAS_POWER_PROBLEM         35           /*!< power-problem (35)                           */
#define D_COM_CAS_PROCESSOR_PROBLEM     37           /*!< processor-problem (37)                       */
#define D_COM_CAS_RECV_FAIL             40           /*!< receive-failure (40)                         */
#define D_COM_CAS_RECVR_FAIL            41           /*!<receiver-failure (41)                         */
#define D_COM_CAS_REMOTE_TRANS_ERR      42           /*!< remote-node-transmission-error (42)          */
#define D_COM_CAS_RESOURCE_CAPACITY     43           /*!< resource-at-or-nearing-capacity (43)         */
#define D_COM_CAS_RESP_TIME_EXCESS      44           /*!< response-time-excessive (44)                 */
#define D_COM_CAS_RETRANS_RAT_EXCESS    45           /*!< retransmission-rate-excessive (45)           */
#define D_COM_CAS_SW_ERROR              46           /*!< software-error (46)                          */
#define D_COM_CAS_SW_PROG_ABNORMAL      47           /*!< software-program-abnormally (47)             */
#define D_COM_CAS_SW_PROG_ERROR         48           /*!< software-program-error (48)                  */
#define D_COM_CAS_STORAGE_CAPACITY      49           /*!< storage-capacity-problem (49)                */
#define D_COM_CAS_TEMPER_UNACCEPT       50           /*!< temperature-unacceptable (50)                */
#define D_COM_CAS_THRESHOLD_CROSSED     51           /*!< threshold-crossed (51)                       */
#define D_COM_CAS_TIMING_PROBLEM        52           /*!< timing-problem (52)                          */
#define D_COM_CAS_TRANSMIT_FAIL         54           /*!< transmit-Failure (54)                        */
#define D_COM_CAS_TRANSMITTER_FAIL      55           /*!< transmitter-Failure (55)                     */
#define D_COM_CAS_TRANSMITTER_FAIL      55           /*!< transmitter-Failure (55)                     */
#define D_COM_CAS_RESOURCE_UNAVAIL      56           /*!< underlying-resource-unavailable         */
/* @} */

/*!
* @name UIC related definition for BSP
* @note UIC number PRIORITY and causeBitNo define
* @{
*/
#define D_COM_BSP_UIC0                  0            /*!< UIC number 0                                 */
#define D_COM_BSP_UIC1                  1            /*!< UIC number 1                                 */
#define D_COM_BSP_UIC2                  2            /*!< UIC number 2                                 */

#define D_COM_BSP_UIC0_PRIORITY4        4            /*!< UIC0 PRIORITY 4                              */
#define D_COM_BSP_UIC1_PRIORITY3        3            /*!< UIC1 PRIORITY 3                              */
#define D_COM_BSP_UIC1_PRIORITY4        4            /*!< UIC1 PRIORITY 4                              */

#define D_COM_BSP_UIC0_CAUSEBIT9        9            /*!< UIC0 causeBitNo 9  : External IRQ 0          */
#define D_COM_BSP_UIC1_CAUSEBIT26       26           /*!< UIC1 causeBitNo 26 : External IRQ 2          */
#define D_COM_BSP_UIC1_CAUSEBIT30       30           /*!< UIC1 causeBitNo 30 : External IRQ 1          */
/* @} */

/*!
* @name Task IF log receive send flag
* @note Task IF log receive send flag define
* @{
*/
#define D_COM_TASKIF_FLG_RCV            0x00         /*!< Sending or receipt flag(receipt)             */
#define D_COM_TASKIF_FLG_SND            0x01         /*!< Sending or receipt flag(Sending)             */
/* @} */

/*!
* @name Startbit index
* @note Startbit index in IQ Setting Request(LTE)
* @{
*/
#define D_COM_STARTBIT_TXMAIN           0            /*!< TxMainIQ                                     */
#define D_COM_STARTBIT_TXDIV            1            /*!< TxDivIQ                                      */
#define D_COM_STARTBIT_RXMAIN           2            /*!< RxMainIQ                                     */
#define D_COM_STARTBIT_RXDIV            3            /*!< RxDivIQ                                      */
/* @} */

/* FFCS add end   */

/*!
 * @name レジスタログ
 * @note レジスタログ define
 * @{
 */
#define D_COM_MAX_REGLOGNUM               1637                     /*!< Maximum register log record               */
#define D_COM_REGLOG_R                    0x00                     /*!< reg log kind of read                      */
#define D_COM_REGLOG_W                    0x01                     /*!< reg log kind of write                     */
/* @} */

/*!
 * @name Alarm History log Delay定義
 * @note 
 * @{
 */
#define D_COM_ALARM_HIST_DELAY_TIME		10				/*!< 100msec (Delayは10ms単位)			*/
#define D_COM_ALARM_HIST_DELAY_CNT		0				/*!< WDT Error 回避カウント				*/
/* @} */

/*!
 * @name Register History log Delay定義
 * @note 
 * @{
 */
#define D_COM_REG_HIST_DELAY_TIME       10      /*!< 100msec (Delayは10ms単位)          */
#define D_COM_REG_HIST_DELAY_CNT        100     /*!< WDT Error 回避カウント             */
/* @} */


/** @name MAXIMUM constant definition
 *  @{
 */
#define D_COM_FLASH_WRT_MAX				2				/*!< Maximum of FLASH access info	*/
/** @} */

/*
* D_SYS_REG_CARDSTS_IDX_SW_ERROR1
* @{
*/
#define D_COM_INST_BCI_MIN				251
#define D_COM_INST_BCI_MAX				650
#define D_COM_INST_SW_ERROR_BASE		251
#define D_COM_INST_HNDRUN_EXC			D_COM_INST_SW_ERROR_BASE			/*!< Exception during handler running            */
#define D_COM_INST_DSI_EXC				(D_COM_INST_SW_ERROR_BASE + 1)		/*!< Data storage interrupt exception            */
#define D_COM_INST_ISI_EXC				(D_COM_INST_SW_ERROR_BASE + 2)		/*!< Instruction storage interrupt exception     */
#define D_COM_INST_ALI_EXC				(D_COM_INST_SW_ERROR_BASE + 3)		/*!< Alignment exception			             */
#define D_COM_INST_PRG_EXC				(D_COM_INST_SW_ERROR_BASE + 4)		/*!< Program exception				             */
#define D_COM_INST_CMDTLB_EXC			(D_COM_INST_SW_ERROR_BASE + 5)		/*!< Instruction TLB miss			             */
#define D_COM_INST_DLTLB_EXC			(D_COM_INST_SW_ERROR_BASE + 6)		/*!< Data load  TLB miss				         */
#define D_COM_INST_DSTLB_EXC			(D_COM_INST_SW_ERROR_BASE + 7)		/*!< Data store TLB miss			             */
#define D_COM_INST_SYSCALL_ERR			(D_COM_INST_SW_ERROR_BASE + 8)		/*!< System call failure			             */

#define D_COM_INST_THRDSPAWN_ERR		(D_COM_INST_SW_ERROR_BASE + 9)		/*!< Thread create failure			             */
#define D_COM_INST_THRDDELETE_ERR		(D_COM_INST_SW_ERROR_BASE + 10)		/*!< Thread delete failure			             */
#define D_COM_INST_THRDDELAY_ERR		(D_COM_INST_SW_ERROR_BASE + 11)		/*!< Thread delay failure			             */

#define D_COM_INST_BUFGET_ERR			(D_COM_INST_SW_ERROR_BASE + 12)		/*!< Buffer get failure					         */
#define D_COM_INST_BUFREL_ERR			(D_COM_INST_SW_ERROR_BASE + 13)		/*!< Buffer release failure						 */

#define D_COM_INST_TIMERCREATE_ERR		(D_COM_INST_SW_ERROR_BASE + 14)		/*!< Timer create failure						 */
#define D_COM_INST_TIMERSTR_ERR			(D_COM_INST_SW_ERROR_BASE + 15)		/*!< Timer start failure						 */
#define D_COM_INST_TIMERSTOP_ERR		(D_COM_INST_SW_ERROR_BASE + 16)		/*!< Timer stop failure							 */

#define D_COM_INST_SEMGET_ERR			(D_COM_INST_SW_ERROR_BASE + 17)		/*!< Semaphore get failure						 */
#define D_COM_INST_SEMREL_ERR			(D_COM_INST_SW_ERROR_BASE + 18)		/*!< Semaphore release failure					 */

#define D_COM_INST_BOOTCHKSUM_ERR		(D_COM_INST_SW_ERROR_BASE + 19)		/*!< Boot sum check failure						 */
#define D_COM_INST_FLASHCHK_ERR			(D_COM_INST_SW_ERROR_BASE + 20)		/*!< File check sum failure						 */
#define D_COM_INST_MAINMEM_ERR			(D_COM_INST_SW_ERROR_BASE + 21)		/*!< Memory check failure						 */
#define D_COM_INST_FPGACHK_ERR			(D_COM_INST_SW_ERROR_BASE + 22)		/*!< Register check failure						 */

#define D_COM_INST_SOFTCRITICAL_ERR		(D_COM_INST_SW_ERROR_BASE + 23)		/*!< Critical Internal Software Fault            */
/* @} */

/*
* D_SYS_REG_CARDSTS_IDX_SW_ERROR2
* @{
*/
#define D_COM_INST_SW_ERROR2_BASE		290
#define D_COM_INST_THRDSTA_ERR			(D_COM_INST_SW_ERROR2_BASE + 0)		/*!< Thread state abnormality				     */
#define D_COM_INST_THREADIF_ERR			(D_COM_INST_SW_ERROR2_BASE + 1)		/*!< Thread interface abnormality			     */


#define D_COM_INST_MSGQCREATE_ERR		(D_COM_INST_SW_ERROR2_BASE + 2)		/*!< Message queue create abnormality            */
#define D_COM_INST_MSGRCV_ERR			(D_COM_INST_SW_ERROR2_BASE + 3)		/*!< Message receive abnormality				 */
#define D_COM_INST_MSGSEND_ERR			(D_COM_INST_SW_ERROR2_BASE + 4)		/*!< Message send abnormality					 */

#define D_COM_INST_FIFOCRE_ERR			(D_COM_INST_SW_ERROR2_BASE + 5)		/*!< FIFO create error						     */
#define D_COM_INST_FIFORCV_ERR			(D_COM_INST_SW_ERROR2_BASE + 6)		/*!< FIFO receive error							 */
#define D_COM_INST_FIFOSEND_ERR			(D_COM_INST_SW_ERROR2_BASE + 7)		/*!< FIFO send error						     */

#define D_COM_INST_LINXOPEN_ERR			(D_COM_INST_SW_ERROR2_BASE + 8)		/*!< Linx open error						     */
#define D_COM_INST_LINXRCV_ERR			(D_COM_INST_SW_ERROR2_BASE + 9)		/*!< Linx receive error							 */
#define D_COM_INST_LINXSEND_ERR			(D_COM_INST_SW_ERROR2_BASE + 10)	/*!< Linx send error							 */
#define D_COM_INST_LINXOTHFUN_ERR		(D_COM_INST_SW_ERROR2_BASE + 11)	/*!< Linx other function error					 */
/* @} */

/*
* D_SYS_REG_CARDSTS_IDX_SW_ERROR3
* @{
*/
#define D_COM_INST_SW_ERROR3_BASE		320
#define D_COM_INST_EEPROMACCESS_ERR			(D_COM_INST_SW_ERROR3_BASE + 0)		/*!< Machine check exception					 */
#define D_COM_INST_FLASHACCESS_ERR			(D_COM_INST_SW_ERROR3_BASE + 1)		/*!< Machine check exception					 */
#define D_COM_INST_I2CACCESS_ERR			(D_COM_INST_SW_ERROR3_BASE + 2)		/*!< Machine check exception					 */
#define D_COM_INST_WATCHDOG					(D_COM_INST_BCI_MAX)		/*!< Machine check exception					 */

/* @} */


/*
* D_SYS_REG_CARDSTS_IDX_HW_FAULT1
* @{
*/
#define D_COM_INST_HW_FAULT1_BASE		350
#define D_COM_INST_CPLDSV_FAIL			(D_COM_INST_HW_FAULT1_BASE + 0)		/*!< CPLD SV bus failure	*/
#define D_COM_INST_LOWTEMP30M_ERR		(D_COM_INST_HW_FAULT1_BASE + 1)		/*!< Temperature(Low) abnormality over 30min	*/
/* @} */
/*
* D_SYS_REG_CARDSTS_IDX_HW_FAULT2
* @{
*/
#define D_COM_INST_HW_FAULT2_BASE		380
#define D_COM_INST_WATCHDOG_ERROR		650

/* @} */

/*
* D_SYS_REG_CARDSTS_IDX_HW_EVALUATION1
* @{
*/
#define D_COM_INST_HW_EVALUATION1_BASE	400
#define D_COM_INST_TXPLL_ALM			(D_COM_INST_HW_EVALUATION1_BASE + 0)	/*!< Tx PLL alarm			*/
#define D_COM_INST_CPRIPLL_ALM			(D_COM_INST_HW_EVALUATION1_BASE + 1)	/*!< CPRI PLL alarm			*/
#define D_COM_INST_SYSPLL_ALM			(D_COM_INST_HW_EVALUATION1_BASE + 2)	/*!< SYS PLL alarm			*/
#define D_COM_INST_INIT_FAIL			(D_COM_INST_HW_EVALUATION1_BASE + 3)	/*!< Initialization failure	*/
#define D_COM_INST_POW_ERR				(D_COM_INST_HW_EVALUATION1_BASE + 4)	/*!< Power supply alarm     */
#define D_COM_INST_CIRCUIT_ERR 			(D_COM_INST_HW_EVALUATION1_BASE + 5)	/*!< current supply alarm     */
#define D_COM_INST_RXJESD_FAIL			(D_COM_INST_HW_EVALUATION1_BASE + 6)	/*!< RX JESD failure		*/

/* @} */

/*
* D_SYS_REG_CARDSTS_IDX_EXCEPT_TEMP1
* @{
*/
#define D_COM_INST_EXPTHIGHTEMP_BASE	500
#define D_COM_INST_EXPTHIGHTEMP_ERR		(D_COM_INST_EXPTHIGHTEMP_BASE + 0)		/*!< Temperature(High) abnormality#0             */
/* @} */

/*
* D_SYS_REG_CARDSTS_IDX_EXCEPT_TEMP2
* @{
*/
#define D_COM_INST_EXPTLOWTEMP2_BASE	510
#define D_COM_INST_EXPTLOWTEMP_ERR		(D_COM_INST_EXPTLOWTEMP2_BASE + 0)			/*!< Temperature(Low) abnormality#0			  	 */
/* @} */


/*
* D_SYS_REG_CARDSTS_IDX_HW_EVALUATION2
* @{
*/
/* @} */


/*
* D_SYS_REG_CARDSTS_IDX_RF_PARTIAL
* @{
*/
#define D_COM_INST_TRD_MIN				651
#define D_COM_INST_TRD_MAX				850

#define D_COM_INST_RF_PARTIAL_BASE		700
#define D_COM_INST_RXPLL_ALM_1			(D_COM_INST_RF_PARTIAL_BASE + 0)	/*!< Rx PLL alarm 1				*/
#define D_COM_INST_RXPLL_ALM_2			(D_COM_INST_RF_PARTIAL_BASE + 1)	/*!< Rx PLL alarm 2				*/

#define D_COM_INST_TXPLL_ALM_1			(D_COM_INST_RF_PARTIAL_BASE + 10)	/*!< Tx FPGA internal PLL alarm 1*/
#define D_COM_INST_TXPLL_ALM_2			(D_COM_INST_RF_PARTIAL_BASE + 11)	/*!< Tx FPGA internal PLL alarm 2*/
#define D_COM_INST_TXSYS_ALM_1			(D_COM_INST_RF_PARTIAL_BASE + 12)	/*!< Tx SYS alarm  1		*/
#define D_COM_INST_TXSYS_ALM_2			(D_COM_INST_RF_PARTIAL_BASE + 13)	/*!< Tx SYS alarm  2		*/
#define D_COM_INST_TXFIXED_ALM_1		(D_COM_INST_RF_PARTIAL_BASE + 14)	/*!< Tx fixec alarm  1		*/
#define D_COM_INST_TXFIXED_ALM_2		(D_COM_INST_RF_PARTIAL_BASE + 15)	/*!< Tx fixec alarm  1		*/

#define D_COM_INST_POWER_SUPLY_ALM_1	(D_COM_INST_RF_PARTIAL_BASE + 22)	/*!< Power supply for PA alarm 1*/
#define D_COM_INST_POWER_SUPLY_ALM_2	(D_COM_INST_RF_PARTIAL_BASE + 23)	/*!< Power supply for PA alarm 2*/

#define D_COM_INST_TXBUS_FAIL_ALM_1		(D_COM_INST_RF_PARTIAL_BASE + 30)	/*!<Tx FPGA SV bus failure 1	*/
#define D_COM_INST_TXBUS_FAIL_ALM_2		(D_COM_INST_RF_PARTIAL_BASE + 31)	/*!<Tx FPGA SV bus failure 2	*/

#define D_COM_INST_FBADC_FAIL_ALM_1		(D_COM_INST_RF_PARTIAL_BASE + 40)	/*!<Feedback ADC failure 1		*/
#define D_COM_INST_FBADC_FAIL_ALM_2		(D_COM_INST_RF_PARTIAL_BASE + 41)	/*!<Feedback ADC failure 2		*/


#define D_COM_INST_RXGAIN_ALM_A			(D_COM_INST_RF_PARTIAL_BASE + 50)	/*!< RX Gain Alarm(#A)	*/
#define D_COM_INST_RXGAIN_ALM_B			(D_COM_INST_RF_PARTIAL_BASE + 51)	/*!< RX Gain Alarm(#B)	*/
#define D_COM_INST_RXGAIN_ALM_C			(D_COM_INST_RF_PARTIAL_BASE + 52)	/*!< RX Gain Alarm(#C)	*/
#define D_COM_INST_RXGAIN_ALM_D			(D_COM_INST_RF_PARTIAL_BASE + 53)	/*!< RX Gain Alarm(#D)	*/

#define D_COM_INST_TXGAIN_ALM_A			(D_COM_INST_RF_PARTIAL_BASE + 60)	/*!< Tx gain alarm(#A)	*/
#define D_COM_INST_TXGAIN_ALM_B			(D_COM_INST_RF_PARTIAL_BASE + 61)	/*!< Tx gain alarm(#B)	*/
#define D_COM_INST_TXGAIN_ALM_C			(D_COM_INST_RF_PARTIAL_BASE + 62)	/*!< Tx gain alarm(#B)	*/
#define D_COM_INST_TXGAIN_ALM_D			(D_COM_INST_RF_PARTIAL_BASE + 63)	/*!< Tx gain alarm(#D)	*/

#define D_COM_INST_POWOVER_A			(D_COM_INST_RF_PARTIAL_BASE + 70)	/*!< Over Power output(#A)	*/
#define D_COM_INST_POWOVER_B			(D_COM_INST_RF_PARTIAL_BASE + 71)	/*!< Over Power output(#B)	*/
#define D_COM_INST_POWOVER_C			(D_COM_INST_RF_PARTIAL_BASE + 72)	/*!< Over Power output(#C)	*/
#define D_COM_INST_POWOVER_D			(D_COM_INST_RF_PARTIAL_BASE + 73)	/*!< Over Power output(#D)  */

#define D_COM_INST_VSWR_ALM_A			(D_COM_INST_RF_PARTIAL_BASE + 80)	/*!< VSWR alarm(#A)	*/
#define D_COM_INST_VSWR_ALM_B			(D_COM_INST_RF_PARTIAL_BASE + 81)	/*!< VSWR alarm(#B)	*/
#define D_COM_INST_VSWR_ALM_C			(D_COM_INST_RF_PARTIAL_BASE + 82)	/*!< VSWR alarm(#C)	*/
#define D_COM_INST_VSWR_ALM_D			(D_COM_INST_RF_PARTIAL_BASE + 83)	/*!< VSWR alarm(#D)	*/

#define D_COM_INST_DPD_ALM_A			(D_COM_INST_RF_PARTIAL_BASE + 90)	/*!< DPD alarm(#A)	*/
#define D_COM_INST_DPD_ALM_B			(D_COM_INST_RF_PARTIAL_BASE + 91)	/*!< DPD alarm(#B)	*/
#define D_COM_INST_DPD_ALM_C			(D_COM_INST_RF_PARTIAL_BASE + 92)	/*!< DPD alarm(#C)	*/
#define D_COM_INST_DPD_ALM_D			(D_COM_INST_RF_PARTIAL_BASE + 93)	/*!< DPD alarm(#D)	*/
/* @} */


/*
*D_SYS_REG_CARDSTS_IDX_NORMAL_TEMP1
* @{
*/
#define D_COM_INST_NORMALTEMP_ERR_BASE	800
#define D_COM_INST_HIGNMTEMP_ERR1		(D_COM_INST_NORMALTEMP_ERR_BASE + 0)
#define D_COM_INST_HIGNMTEMP_ERR2		(D_COM_INST_NORMALTEMP_ERR_BASE + 1)
																/*!< Temperature too high degraded - warning */
/* @} */

/*
* D_SYS_REG_CARDSTS_IDX_NORMAL_TEMP2
* @{
*/
#define D_COM_INST_NORMALTEMP2_ERR_BASE 810
#define D_COM_INST_LOWNMTEMP_ERR1		(D_COM_INST_NORMALTEMP2_ERR_BASE + 0)
#define D_COM_INST_LOWNMTEMP_ERR2		(D_COM_INST_NORMALTEMP2_ERR_BASE + 1)
																/*!< Temperature too low degraded - warning */
/* @} */

/*
* D_SYS_REG_CARDSTS_IDX_VOLTAGE
* @{
*/
#define D_COM_INST_POWOVLD_BASE		910
#define D_COM_INST_POWOVLD_ERR		(D_COM_INST_POWOVLD_BASE + 0)		/*!< T.B.D			  */
/* @} */

/*
* D_SYS_REG_CARDSTS_IDX_POWER
* @{
*/
#define D_COM_INST_VOLRANGE_BASE	920
#define D_COM_INST_POWAISG_ERR		(D_COM_INST_VOLRANGE_BASE + 0)			/*!< Power supply(AISG) abnormality				 */
/* @} */

/*
* D_SYS_REG_CARDSTS_IDX_WARNNING
* @{
*/
#define D_COM_INST_PCUTERR 			930
#define D_COM_INST_POWER_PA_ALM_1 	931
#define D_COM_INST_POWER_PA_ALM 	931
/* @} */
/*
* Fault Priority
* @{
*/
#define D_COM_FAULTPRIO_NOT 		0
#define D_COM_FAULTPRIO_LOWEST 		1
#define D_COM_FAULTPRIO_LOW	 		2
#define D_COM_FAULTPRIO_HIGH	 	3
#define D_COM_FAULTPRIO_HIGHEST		4
/* @} */

/*
* The affection of fault
* @{
*/
#define D_COM_FAULTAFT_UL_A		0x0001
#define D_COM_FAULTAFT_UL_B		0x0002
#define D_COM_FAULTAFT_UL_1		0x0003
#define D_COM_FAULTAFT_UL_C		0x0004
#define D_COM_FAULTAFT_UL_D		0x0008
#define D_COM_FAULTAFT_UL_2		0x000C
#define D_COM_FAULTAFT_UL		0x000F

#define D_COM_FAULTAFT_DL_A		0x0100
#define D_COM_FAULTAFT_DL_B		0x0200
#define D_COM_FAULTAFT_DL_1		0x0300
#define D_COM_FAULTAFT_DL_C		0x0400
#define D_COM_FAULTAFT_DL_D		0x0800
#define D_COM_FAULTAFT_DL_2		0x0C00
#define D_COM_FAULTAFT_DL		0x0F00

#define D_COM_FAULTAFT_ALL		0xFFFF
/* @} */	

/*
* Fault attribuate
* @{
*/
#define D_COM_FAULT_DETECT				0x0001
#define D_COM_FAULT_RECOVER				0x0002
#define D_COM_FAULT_DEACTIVE			0x0100
#define D_COM_FAULT_ACTIVE				0x0200
/* @} */

/*
* The definition for behaviore & Restoration when fault occur
* @{
*/
#define D_COM_FAULT_RPT_RESET			0x0001		/*immediately reset*/
#define D_COM_FAULT_RPT_RESET_DELAY		0x0002		/*continue process,and reset in cdsv*/
#define D_COM_FAULT_RPT_RESET_DELAY2	0x0004		/*continue process,and reset in cdsv*/
#define D_COM_FAULT_RPT_PAOFF			0x0008		/*continue process,and paoff in cdsv*/
#define D_COM_FAULT_REST_REPLACE		0x0100		/*restoration is replecement*/
#define D_COM_FAULT_REST_RESET			0x0200		/*restoration is reset*/

/* @} */

#define D_COM_BCI_FAULT_ID_HW_FAULT 							0x101
#define D_COM_BCI_FAULT_ID_HW_FAULT_FOR_EVALUATION 				0x300
#define D_COM_BCI_FAULT_ID_SW_ERROR								0x301	
#define D_COM_BCI_FAULT_ID_EXCEPTIONAL_TEMPERATURE_TOO_HIGH 	0x103
#define D_COM_BCI_FAULT_ID_EXCEPTIONAL_TEMPERATURE_TOO_LOW		0x104

#define D_COM_TRD_FAULT_ID_HW_FAULT 							0x101
#define D_COM_TRD_FAULT_ID_HW_FAULT_FOR_EVALUATION				0x300

#define D_COM_TRD_FAULT_ID_HW_FAULT_PARTIAL 					0x501
#define D_COM_TRD_FAULT_ID_NORMAL_TEMPERATURE_TOO_HIGH		0x502
#define D_COM_TRD_FAULT_ID_NORMAL_TEMPERATURE_TOO_LOW		0x701

#define D_COM_ANT_FAULT_ID_POWER_OVERLOAD					0x505
#define D_COM_ANT_FAULT_ID_VOLTAGE_OUT_OF_RANGE				0x507
/* @} */

/*
* AISG statstics definition
* @{
*/
enum
{
	D_COM_STATTYP_SND_XID = 0,
	D_COM_STATTYP_RCV_XID,
	D_COM_STATTYP_FCS,
	D_COM_STATTYP_SHF,
	D_COM_STATTYP_LHF,
	D_COM_STATTYP_TO_XID,
	D_COM_STATTYP_ABF_XID,

	D_COM_STATTYP_SND_LNK,
	D_COM_STATTYP_RCV_LNK,
	D_COM_STATTYP_FCS_LNK,
	D_COM_STATTYP_SNRM,
	D_COM_STATTYP_DISC,
	D_COM_STATTYP_UA,
	D_COM_STATTYP_DM,
	D_COM_STATTYP_FRMR,
	D_COM_STATTYP_RRP1,
	D_COM_STATTYP_RRF1,
	D_COM_STATTYP_RNR,
	D_COM_STATTYP_I,
	D_COM_STATTYP_IP1,
	D_COM_STATTYP_IP0,
	D_COM_STATTYP_IF1,
	D_COM_STATTYP_IF0,
	D_COM_STATTYP_ERRNS,
	D_COM_STATTYP_ERRNR,
/*
	D_COM_STATTYP_SHF,
	D_COM_STATTYP_LHF,
*/
	D_COM_STATTYP_TO,
	D_COM_STATTYP_IP1HC,
	D_COM_STATTYP_IF1HC,
	D_COM_STATTYP_MAX
};
#define D_COM_STATTYP_XID_MAX 	(D_COM_STATTYP_ABF_XID + 1)
/* @} */
/*
* Max size of Software Compatibility list file
* @{
*/
#define D_COM_COMPDATA_SIZEMAX		0x100000
/* @} */

/*
* Length of the software version string
* @{
*/
#define D_COM_SWVER_LEN				32
/* @} */

 /*!
 * @name LMC related definition
 * @note LMC related definition
 * @{
 */
#define D_COM_LMCFILE_HEADADDR_0		0				/* T.B.D 		*/
#define D_COM_LMCFILE_HEADADDR_1		1				/* T.B.D 		*/
#define D_COM_FLSHADDR_SW0				0				/* T.B.D 		*/
#define D_COM_FLSHADDR_SW1				1				/* T.B.D 		*/
#define D_COM_FLSHADDR_CPRIFPGA_0		0				/* T.B.D 		*/
#define D_COM_FLSHADDR_CPRIFPGA_1		0				/* T.B.D 		*/
#define D_COM_FLSHADDR_TXFPGA_0			0				/* T.B.D 		*/
#define D_COM_FLSHADDR_TXFPGA_1			0				/* T.B.D 		*/
#define D_COM_FLSHADDR_COMP0			0				/* T.B.D 		*/
#define D_COM_FLSHADDR_COMP1			1				/* T.B.D 		*/
#define D_COM_LMCSIZE_MAX				37748736		/* T.B.D 		*/


#define D_COM_FILETYPE_VENDOR			0x01010101			/* Software 			*/
#define D_COM_FILETYPE_COMP				0x01000000			/* Compatibility 		*/
/* @} */


/*!
 * @name トレーニングデータレジスタテーブルINDEX.
 * @note トレーニングデータレジスタテーブルINDEXの定義
 * @{
 */
enum
{
/****************************************/
/* TX系									*/
/****************************************/
	E_COM_REG_TRA_RF_SENDPOW_0,
	E_COM_REG_TRA_RF_SENDPOW_1,
 	E_COM_REG_TRA_RF_GAIN_0,
	E_COM_REG_TRA_RF_GAIN_1,
	E_COM_REG_TRA_RF_GAIN_2,
	E_COM_REG_TRA_RF_GAIN_3,
	E_COM_REG_TRA_MCAR_GAIN_0,
	E_COM_REG_TRA_MCAR_GAIN_1,
	E_COM_REG_TRA_MCAR_GAIN_2,
	E_COM_REG_TRA_MCAR_GAIN_3,
	E_COM_REG_TRA_DEM_GAIN_Q_0,
	E_COM_REG_TRA_DEM_GAIN_Q_1,
	E_COM_REG_TRA_DEM_GAIN_Q_2,
	E_COM_REG_TRA_DEM_GAIN_Q_3,
	E_COM_REG_TRA_DEM_GAIN_I_0,
	E_COM_REG_TRA_DEM_GAIN_I_1,
	E_COM_REG_TRA_DEM_GAIN_I_2,
	E_COM_REG_TRA_DEM_GAIN_I_3,
	E_COM_REG_TRA_FB_DLYCLK_0,
	E_COM_REG_TRA_FB_DLYCLK_1,
	E_COM_REG_TRA_FB_DLYCLK_2,
	E_COM_REG_TRA_FB_DLYCLK_3,
	E_COM_REG_TRA_FB_DLYFIL_0,
	E_COM_REG_TRA_FB_DLYFIL_1,
	E_COM_REG_TRA_FB_DLYFIL_2,
	E_COM_REG_TRA_FB_DLYFIL_3,
	E_COM_REG_TRA_FB_DLYAC_0,
	E_COM_REG_TRA_FB_DLYAC_1,
	E_COM_REG_TRA_FB_DLYAC_2,
	E_COM_REG_TRA_FB_DLYAC_3,
	E_COM_REG_TRA_FB_DLYACFIL_0,
	E_COM_REG_TRA_FB_DLYACFIL_1,
	E_COM_REG_TRA_FB_DLYACFIL_2,
	E_COM_REG_TRA_FB_DLYACFIL_3,
	E_COM_REG_TRA_IDAC_DC_OFS_1_0,
	E_COM_REG_TRA_IDAC_DC_OFS_1_1,
	E_COM_REG_TRA_IDAC_DC_OFS_1_2,
	E_COM_REG_TRA_IDAC_DC_OFS_1_3,
	E_COM_REG_TRA_IDAC_DC_OFS_2_0,
	E_COM_REG_TRA_IDAC_DC_OFS_2_1,
	E_COM_REG_TRA_IDAC_DC_OFS_2_2,
	E_COM_REG_TRA_IDAC_DC_OFS_2_3,
	E_COM_REG_TRA_IDAC_DC_OFS_3_0,
	E_COM_REG_TRA_IDAC_DC_OFS_3_1,
	E_COM_REG_TRA_IDAC_DC_OFS_3_2,
	E_COM_REG_TRA_IDAC_DC_OFS_3_3,
	E_COM_REG_TRA_IDAC_DC_OFS_4_0,
	E_COM_REG_TRA_IDAC_DC_OFS_4_1,
	E_COM_REG_TRA_IDAC_DC_OFS_4_2,
	E_COM_REG_TRA_IDAC_DC_OFS_4_3,
	E_COM_REG_TRA_FW_VATT_0,
	E_COM_REG_TRA_FW_VATT_1,
	E_COM_REG_TRA_FW_VATT_2,
	E_COM_REG_TRA_FW_VATT_3,
	E_COM_REG_TRA_FBEQ_NUM_0,
	E_COM_REG_TRA_FBEQ_NUM_1,
	E_COM_REG_TRA_FBEQ_NUM_2,
	E_COM_REG_TRA_FBEQ_NUM_3,
	E_COM_REG_TRA_VDMGAIN_0,
	E_COM_REG_TRA_VDMGAIN_1,
	E_COM_REG_TRA_VDMGAIN_2,
	E_COM_REG_TRA_VDMGAIN_3,

/****************************************/
/* RX系									*/
/****************************************/
 	E_COM_REG_TRA_RXRX_VATT_0,
 	E_COM_REG_TRA_RXRX_VATT_1,
 	E_COM_REG_TRA_RXRX_VATT_2,
 	E_COM_REG_TRA_RXRX_VATT_3,
 	E_COM_REG_TRA_RXRX_VGA_0,
 	E_COM_REG_TRA_RXRX_VGA_1,
 	E_COM_REG_TRA_RXRX_VGA_2,
 	E_COM_REG_TRA_RXRX_VGA_3,
 	/* RxVer, CRC情報 */
	E_COM_REG_TRA_RXTPVER,
	E_COM_REG_TRA_RXCRC16,
 	/* RxACV情報(Proto17以降のみ使用する領域 */
	E_COM_REG_TRA_RXACV_0_00,
	E_COM_REG_TRA_RXACV_0_10,
	E_COM_REG_TRA_RXACV_0_11,
	E_COM_REG_TRA_RXACV_0_12,
	E_COM_REG_TRA_RXACV_0_13,
	E_COM_REG_TRA_RXACV_0_14,
	E_COM_REG_TRA_RXACV_0_15,
	E_COM_REG_TRA_RXACV_0_16,
	E_COM_REG_TRA_RXACV_1_00,
	E_COM_REG_TRA_RXACV_1_10,
	E_COM_REG_TRA_RXACV_1_11,
	E_COM_REG_TRA_RXACV_1_12,
	E_COM_REG_TRA_RXACV_1_13,
	E_COM_REG_TRA_RXACV_1_14,
	E_COM_REG_TRA_RXACV_1_15,
	E_COM_REG_TRA_RXACV_1_16,
	E_COM_REG_TRA_RXACV_2_00,
	E_COM_REG_TRA_RXACV_2_10,
	E_COM_REG_TRA_RXACV_2_11,
	E_COM_REG_TRA_RXACV_2_12,
	E_COM_REG_TRA_RXACV_2_13,
	E_COM_REG_TRA_RXACV_2_14,
	E_COM_REG_TRA_RXACV_2_15,
	E_COM_REG_TRA_RXACV_2_16,
	E_COM_REG_TRA_RXACV_3_00,
	E_COM_REG_TRA_RXACV_3_10,
	E_COM_REG_TRA_RXACV_3_11,
	E_COM_REG_TRA_RXACV_3_12,
	E_COM_REG_TRA_RXACV_3_13,
	E_COM_REG_TRA_RXACV_3_14,
	E_COM_REG_TRA_RXACV_3_15,
	E_COM_REG_TRA_RXACV_3_16,
 	E_COM_REG_TRA_MAX
};

/* 電力報告周期時	DPDAアラームハード収集情報レジスタ最大値 */
#define D_COM_REG_DATA_ALM_MAX			82				/* DPDAアラームレジスタ最大値	*/

/* ボードタイプ */
#define D_COM_BOARD_TYPE_PROT_1			0				/* ボードバージョン1.6以前	*/
#define D_COM_BOARD_TYPE_PROT_2			1				/* ボードバージョン1.7以降	*/
#define D_COM_BOARD_TYPE_NUM			2				/* ボードタイプ数			*/

#endif
/* @} */

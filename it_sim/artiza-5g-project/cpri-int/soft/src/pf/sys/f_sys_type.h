/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file   f_sys_type.h
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2015
 */
/******************************************************************************************************************************/

/*!
 * @addtogroup RRH_SYS
 * @{
 */
#ifndef F_SYS_TYP_H
#define F_SYS_TYP_H


typedef		int								(*FUNCPTR) ();
typedef		void							(*VOIDFUNCPTR) ();

/* 既存pfヘッダを流用できるようにするための再定義 */
typedef		T_RRH_HEAD		T_SYS_COMMON_THDIF;

typedef		T_RRH_HEAD		T_SYS_EQS_HEAD;


/******************************************************************************************************************************/
/** Thread Init completion notice
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Thread Init completion notice
 * @note	Thread Init completion notice is ..
 * 
 */
typedef struct{
	T_RRH_HEAD						head;								/**< Thread Common Header						*/
}T_SYS_INIT_COMP_NTC;
/** @} */

/******************************************************************************************************************************/
/** Thread Message Form
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Thread Common Header
 * @note	Thread Common Header is ..
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	UCHAR											msgData[8168];			/**< データ部									*/
}T_SYS_MSGFORM;

/******************************************************************************************************************************/
 /** T_SYS_TROUBLE_LOG_OUTPUT_REQ(0x10010003)
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_TROUBLE_LOG_OUTPUT_REQ
 * @note	T_SYS_TROUBLE_LOG_OUTPUT_REQ is ..
 * 
 */
typedef struct{
	T_RRH_HEAD						head;								/**< Thread Common Header						*/
	UINT							trouble_kind;						/**< 障害種別									*/
}T_SYS_TROUBLE_LOG_OUTPUT_REQ;

/*!
 * @name 障害種別
 * @note 障害種別定義
 * @{
 */
enum{
	E_SYS_TROUBLE_KIND_NORMAL = 0,			/* 通常障害			*/
	E_SYS_TROUBLE_KIND_HLTCHK,				/* HealthCheck断	*/
	E_SYS_TROUBLE_KIND_RST,					/* 障害(RESET有)	*/
	E_SYS_TROUBLE_KIND_MAX
};
/* @} */


/******************************************************************************************************************************/
 /** T_SYS_TROUBLE_LOG_OUTPUT_REQ(0x10010005)
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_RUN_LOG_OUTPUT_NTC
 * @note	T_SYS_RUN_LOG_OUTPUT_NTC is ..
 * 
 */
typedef struct{
	T_RRH_HEAD						head;								/**< Thread Common Header						*/
}T_SYS_RUN_LOG_OUTPUT_NTC;


/******************************************************************************************************************************/
 	/** T_SYS_TROUBLE_LOG_WRITE_REQ(0x10010009)
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_TROUBLE_LOG_WRITE_REQ
 * @note	T_SYS_TROUBLE_LOG_WRITE_REQ is ..
 * 
 */
typedef struct{
	T_RRH_HEAD						head;								/**< Thread Common Header						*/
	UINT							kind;								/**< kind										*/
	UINT							face;								/**< face										*/
	UINT							offset;								/**< offset										*/
}T_SYS_TROUBLE_LOG_WRITE_REQ;
/** @} */

/******************************************************************************************************************************/
 /** T_SYS_FRSP_FLSLOGWR_IND(0x100C0001)
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_FRSP_FLSLOGWR_IND
 * @note	T_SYS_FRSP_FLSLOGWR_IND is ..
 * 
 */
typedef struct{
	T_RRH_HEAD						head;								/**< Thread Common Header						*/
	UINT								eraseWriteKind;						/**< Erase Write Kind							*/
}T_SYS_FRSP_FLSLOGWR_IND;

/* Flash髢｢騾｣諠・黴?*/
#define	D_SYS_FLASH_ERASE									0				/**< Flash Erase							*/
#define	D_SYS_FLASH_WRITE									1				/**< Flash Write							*/
/** @} */


/******************************************************************************************************************************/
/**
 * @brief	All Task Initialization complete Notice 
 * @note	When all task initializations are complete, INI task will inform this message to all tasks.
 * 
 */
/******************************************************************************************************************************/
typedef struct
{
	T_RRH_HEAD	   head;				/**<  header	*/ 
}T_SYS_ALL_TASK_INIT_COMP_NTC;

/******************************************************************************************************************************/
/**
 * @brief	M-Plane SW起動完了通知
 * @note	When NETCONF initialization are complete, INI task will inform this message to all process
 * 
 */
/******************************************************************************************************************************/
typedef struct
{
	T_RRH_HEAD		head;				/**<  header	*/ 
}T_SYS_MPLANE_SW_INIT_COMP_NTC;

/******************************************************************************************************************************/
/** T_SYS_SYSTEM_SEND_POWER_ADJ_INFO_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_SYSTEM_SEND_POWER_ADJ_INFO_REQ
 * @note	T_SYS_SYSTEM_SEND_POWER_ADJ_INFO_REQ(header + data)
 * 
 */
typedef struct{
	USHORT											branch;
	USHORT											command;
}T_SYS_SYSTEM_SEND_POWER_ADJ_INFO_REQ;

/** @} */

/* 5GDU_chg */
/******************************************************************************************************************************/
/** T_SYS_CARRIER_ON_SETTING_REQ_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_CARRIER_ON_SETTING_REQ_INFO
 * @note	T_SYS_CARRIER_ON_SETTING_REQ_INFO is
 * 
 */
typedef struct{
	unsigned int 									uiAntNum;				/**< アンテナ番号(sub6:0~7, mmW:0~1)				*/
	unsigned int									uiCarrier;				/**< キャリア番号(sub6:0~1, mmW:0~7)				*/
	unsigned int									uiAccellId;				/**< ACELL ID(0x00~0x3F)							*/
	unsigned int									uiCcId;					/**< CC ID(0x0~0xF)									*/
	unsigned int									uiDuPortId;				/**< PORT ID(0~7)									*/
	unsigned int									uiCrCentFreq;			/**< 設定周波数(kHz単位)							*/
	unsigned int									uiTxRx;					/**< 送受信方向(0:RX(UL) 1:TX(DL))					*/
	unsigned int									uiTestFlg;				/**< テストコマンドフラグ(0:通常 1:テストコマンド)	*/
    unsigned int									uiBw;                   /**< bandwidth                                      */
    unsigned int									uiScs;                  /**< SCS                                            */
}T_SYS_CARRIER_ON_SETTING_REQ_INFO;
/** @} */

/******************************************************************************************************************************/
/** T_SYS_CARRIER_ON_SETTING_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_CARRIER_ON_SETTING_REQ
 * @note	T_SYS_CARRIER_ON_SETTING_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								head;					/**< Thread Common Header						*/
	T_SYS_CARRIER_ON_SETTING_REQ_INFO				carrierOnSettingReqInfo;/**< Carrier ON Setting Request Info			*/
}T_SYS_CARRIER_ON_SETTING_REQ;
/** @} */


/* 5GDU_chg */
/******************************************************************************************************************************/
/** T_SYS_CARRIER_OFF_SETTING_REQ_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_CARRIER_OFF_SETTING_REQ_INFO
 * @note	T_SYS_CARRIER_OFF_SETTING_REQ_INFO is
 * 
 */
typedef struct{
	unsigned int 									uiAntNum;				/**< アンテナ番号(sub6:0~7, mmW:0~1)				*/
	unsigned int									uiCarrier;				/**< キャリア番号(sub6:0~1, mmW:0~7)				*/
	unsigned int									uiCrCentFreq;			/**< 設定周波数(kHz単位)							*/
	unsigned int									uiTxRx;					/**< 送受信方向(0:RX(UL) 1:TX(DL))					*/
	unsigned int									uiTestFlg;				/**< テストコマンドフラグ(0:通常 1:テストコマンド)	*/
}T_SYS_CARRIER_OFF_SETTING_REQ_INFO;
/** @} */

/******************************************************************************************************************************/
/** T_SYS_CARRIER_OFF_SETTING_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_CARRIER_OFF_SETTING_REQ
 * @note	T_SYS_CARRIER_OFF_SETTING_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								head;						/**< Thread Common Header					*/
	T_SYS_CARRIER_OFF_SETTING_REQ_INFO				carrierOffSettingReqInfo;	/**< Carrier OFF Setting Request Info		*/
}T_SYS_CARRIER_OFF_SETTING_REQ;
/** @} */

/* 5GDU_chg */
/******************************************************************************************************************************/
/** T_SYS_RF_CAL_IN_USE_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_RF_CAL_IN_USE_INFO
 * @note	T_SYS_RF_CAL_IN_USE_INFO is
 * 
 */
typedef struct{
	UINT											txrx_flag;				/**< TxCAL or RxCAL要求フラグ(0:TxCAl 1:RxCal)	*/
}T_SYS_RF_CAL_IN_USE_INFO;
/** @} */

/* 5GDU_chg */
/******************************************************************************************************************************/
/** T_SYS_RF_CAL_IN_USE
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_RF_CAL_IN_USE
 * @note	T_SYS_RF_CAL_IN_USE(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								head;						/**< Thread Common Header					*/
	T_SYS_RF_CAL_IN_USE_INFO						calInfo;					/**< TxCal RxCal in use Request Info		*/
}T_SYS_RF_CAL_IN_USE;
/** @} */

/* 5GDU_chg */
/******************************************************************************************************************************/
/** T_SYS_RF_CAL_TRAINING_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_RF_CAL_TRAINING_INFO
 * @note	T_SYS_RF_CAL_TRAINING_INFO is
 * 
 */
typedef struct{
	UINT											txrx_flag;				/**< TxCAL or RxCAL要求フラグ(0:TxCAl 1:RxCal)	*/
}T_SYS_RF_CAL_TRAINING_INFO;
/** @} */

/* 5GDU_chg */
/******************************************************************************************************************************/
/** T_SYS_RF_CAL_TRAINING
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_RF_CAL_TRAINING
 * @note	T_SYS_RF_CAL_TRAINING(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								head;						/**< Thread Common Header					*/
	T_SYS_RF_CAL_TRAINING_INFO						calInfo;					/**< TxCal RxCal Training Request Info		*/
}T_SYS_RF_CAL_TRAINING;
/** @} */

/* 5GDU_chg */
/******************************************************************************************************************************/
/** T_SYS_RF_TDDEN_CHG_NTC
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_RF_TDDEN_CHG_NTC
 * @note	T_SYS_RF_TDDEN_CHG_NTC(header + data)
 * 
 */
typedef struct{
	UINT											uiAntNo;					/**< Antenna Number					*/
	UINT											uichgVal;					/**< chenge value					*/
	UINT											uiDpdCtl;					/**< DPD value						*/
}T_SYS_TDDEN_INFO;

typedef struct{
	T_SYS_COMMON_THDIF								head;						/**< Thread Common Header					*/
	T_SYS_TDDEN_INFO								tddEn;						/**< TDD Enable情報							*/
}T_SYS_RF_TDDEN_CHG_NTC;
/** @} */

/******************************************************************************************************************************/
/** T_SYS_NO_MOD_CARRIER_ONOFF_REQ_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_NO_MOD_CARRIER_ONOFF_REQ_INFO
 * @note	T_SYS_NO_MOD_CARRIER_ONOFF_REQ_INFO is ..
 * 
 */
typedef struct{
	UINT											sig_kind;				/**< 信号種別								*/
	UINT											branch;					/**< 系指定									*/
	UINT											carrier;				/**< Carrier								*/
	UINT											onoff;					/**< ON/OFF									*/
	UINT											power;					/**< 送信電力値								*/
	UINT											dummy;					/**< dummy									*/
}T_SYS_NO_MOD_CARRIER_ONOFF_REQ_INFO;

/** @} */


/******************************************************************************************************************************/
/** T_SYS_SYSTEM_PARAM_UPDATE_REQ_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_SYSTEM_PARAM_UPDATE_REQ_INFO
 * @note	T_SYS_SYSTEM_PARAM_UPDATE_REQ_INFO is
 * 
 */
typedef struct{
	USHORT							bandwidth[D_RRH_CAR_MAX];	/* bandwidth(2Carrier)						*/
	USHORT							sec_nChip[D_SYS_ANT_MAX];	/**< sector n-chip							*/
	USHORT 							tx_sys_num;					/* 送信系統数(2 or 4)						*/
	USHORT 							cpri_rate;					/* CPRI Rate(0:4.9, 1:9.8)					*/
}T_SYS_SYSTEM_PARAM_UPDATE_REQ_INFO;
/** @} */

/******************************************************************************************************************************/
/** T_SYS_SYSTEM_PARAM_UPDATE_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_SYSTEM_PARAM_UPDATE_REQ
 * @note	T_SYS_SYSTEM_PARAM_UPDATE_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								head;						/**< Thread Common Header					*/
	T_SYS_SYSTEM_PARAM_UPDATE_REQ_INFO				sysParaInfo;				/**< System Param Update Request Info		*/
}T_SYS_SYSTEM_PARAM_UPDATE_REQ;
/** @} */


/******************************************************************************************************************************/
/** T_SYS_PA_CONTROL_REQ_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_PA_CONTROL_REQ_INFO
 * @note	T_SYS_PA_CONTROL_REQ_INFO is
 * 
 */
typedef struct{
	UINT											onOffInfo;				/**< ON_OFF_INFO								*/
	UINT											antRfPort;				/**< RF_ANTENNA_PORT							*/
}T_SYS_PA_CONTROL_REQ_INFO;

/** @} */

/******************************************************************************************************************************/
/** T_SYS_NO_MOD_CAR_REQ_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_NO_MOD_CAR_REQ_INFO
 * @note	T_SYS_NO_MOD_CAR_REQ_INFO is
 * 
 */
typedef struct{
	USHORT											branch;				/**< branch										*/
	USHORT											carrier;			/**< carrier									*/
	USHORT											onoff;				/**< onoff										*/
	USHORT											power;				/**< 騾∽ｿ｡髮ｻ蜉帛､									*/
}T_SYS_NO_MOD_CAR_REQ_INFO;

/** @} */

/******************************************************************************************************************************/
/** T_SYS_NO_MOD_CAR_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_NO_MOD_CAR_REQ
 * @note	T_SYS_NO_MOD_CAR_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								head;					/**< Thread Common Header						*/
	T_SYS_NO_MOD_CAR_REQ_INFO						noModCarReqInfo;		/**< No Modultaion Car Request Info				*/
}T_SYS_NO_MOD_CAR_REQ;

/** @} */


/******************************************************************************************************************************/
/** T_SYS_CPRI_LINK_ARRAY
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_CPRI_LINK_ARRAY
 * @note	T_SYS_CPRI_LINK_ARRAY is ..
 * 
 */
typedef struct{
	UINT											cpriPort;				/**< CPRI_PORT									*/
	UINT											axcId;					/**< AXC_ID										*/
	UINT											IqPosition;				/**< IQ_POSITION								*/
}T_SYS_CPRI_LINK_ARRAY;

/** @} */


/******************************************************************************************************************************/
/** T_SYS_IQ_SETTING_REQ_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_IQ_SETTING_REQ_INFO
 * @note	T_SYS_IQ_SETTING_REQ_INFO is
 * 
 */
typedef struct{
	UINT											iqOnOff;				/**< IQ On Off									*/
	UINT											carrierId;				/**< CARRIER_ID									*/
	UINT											antRfPort;				/**< RF_ANTENNA_PORT							*/
	UINT											recvTrans;				/**< RECEIVE_TRANSMIT							*/
	UINT											bandWidth;				/**< bandwidth									*/
	UINT											ratType;				/**< RAT_TYPE									*/
	UINT											numOfSetupCpri;			/**< NUMBER_OF_SETUP_CPRI_LINKS					*/
	T_SYS_CPRI_LINK_ARRAY							cpriLinkArray[D_SYS_CPRI_PORT_NUM];	/**< CPRI_LINK_ARRAY				*/
}T_SYS_IQ_SETTING_REQ_INFO;

/** @} */

/******************************************************************************************************************************/
/** T_SYS_IQ_SETTING_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_IQ_SETTING_REQ
 * @note	T_SYS_IQ_SETTING_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								head;					/**< Thread Common Header						*/
	T_SYS_IQ_SETTING_REQ_INFO						iqSettingReqInfo;		/**< IQ Setting Request Info					*/
}T_SYS_IQ_SETTING_REQ;

/** @} */

/******************************************************************************************************************************/
/** T_SYS_SYSTEM_SEND_POWER_ADJ_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_SYSTEM_SEND_POWER_ADJ_REQ
 * @note	T_SYS_SYSTEM_SEND_POWER_ADJ_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header				*/
	T_SYS_SYSTEM_SEND_POWER_ADJ_INFO_REQ			sedPowAdjInfo;			/**< Send Power Adjust Request Info		*/
}T_SYS_SYSTEM_SEND_POWER_ADJ_REQ;

/** @} */

/******************************************************************************************************************************/
/** T_SYS_PA_CONTROL_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_PA_CONTROL_REQ
 * @note	T_SYS_PA_CONTROL_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	T_SYS_PA_CONTROL_REQ_INFO						paControlReqInfo;		/**< PA Control Request Info					*/
}T_SYS_PA_CONTROL_REQ;

/** @} */

/******************************************************************************************************************************/
/** T_SYS_APD_WAVE_ONOFF_REQ_INFO
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_APD_WAVE_ONOFF_REQ_INFO
 * @note	T_SYS_APD_WAVE_ONOFF_REQ_INFO is
 * 
 */
typedef struct{
	UINT											waveCtrl;				/**< waveControl								*/
	UINT											antRfPort;				/**< RF_ANTENNA_PORT							*/
}T_SYS_APD_WAVE_ONOFF_REQ_INFO;

/** @} */

/******************************************************************************************************************************/
/** T_SYS_APD_WAVE_ONOFF_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_APD_WAVE_ONOFF_REQ
 * @note	T_SYS_APD_WAVE_ONOFF_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	T_SYS_APD_WAVE_ONOFF_REQ_INFO					apdWaveReqInfo;			/**< APD Wave ON/OFF Request Info				*/
}T_SYS_APD_WAVE_ONOFF_REQ;
/** @} */

/******************************************************************************************************************************/
/** T_SYS_POW_PERIODIC_NOTI
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	DPDA Power Report Periodic Notification
 * @note
 */
typedef struct{
	T_SYS_COMMON_THDIF							thdif;						/**< Thread Common Header						*/
}T_SYS_POW_PERIODIC_NTC;

/** @} */

/****************************************************************************************************************************************/
/** T_SYS_HIGH_SPEED_NTC_INFO
 *  @addtogroup
 *  @{
 */
/****************************************************************************************************************************************/
/**
 * @brief	T_SYS_HIGH_SPEED_NTC_INFO
 * @note	T_SYS_HIGH_SPEED_NTC_INFO is
 * 
 */
typedef struct{
	UINT										txFpgaIrqReg;									/**< Tx FPGA Register info				*/
}T_SYS_HIGH_SPEED_NTC_INFO;

/** @} */

/****************************************************************************************************************************************/
/** }T_SYS_HIGH_SPEED_NTC
 *  @addtogroup RRH_SYS
 *  @{
 */
/****************************************************************************************************************************************/
/**
 * @brief	}T_SYS_HIGH_SPEED_NTC
 * @note	}T_SYS_HIGH_SPEED_NTC(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	T_SYS_HIGH_SPEED_NTC_INFO						highSpeedNtcInfo;		/**< HIGH_SPEED_NTC Info						*/
}T_SYS_HIGH_SPEED_NTC;
/** @} */

/****************************************************************************************************************************************/
/** T_SYS_CALIB_CTRL_REQ_INFO
 *  @addtogroup
 *  @{
 */
/****************************************************************************************************************************************/
/**
 * @brief	T_SYS_CALIB_CTRL_REQ_INFO
 * @note	T_SYS_CALIB_CTRL_REQ_INFO is
 * 
 */
typedef struct{
	UINT										calibItemNo;				/**< Calibraion item no							*/
}T_SYS_CALIB_CTRL_REQ_INFO;
/** @} */

/****************************************************************************************************************************************/
/** }T_SYS_CALIB_CTRL_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/****************************************************************************************************************************************/
/**
 * @brief	}T_SYS_CALIB_CTRL_REQ
 * @note	}T_SYS_CALIB_CTRL_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
	T_SYS_CALIB_CTRL_REQ_INFO						calibCtrlReqInfo;		/**< CALIB_CTRL_REQ Info						*/
}T_SYS_CALIB_CTRL_REQ;

/** @} */

/****************************************************************************************************************************************/
/** T_SYS_CALIB_CTRL_RSP_INFO
 *  @addtogroup
 *  @{
 */
/****************************************************************************************************************************************/
/**
 * @brief	T_SYS_CALIB_CTRL_RSP_INFO
 * @note	T_SYS_CALIB_CTRL_RSP_INFO is
 * 
 */
typedef struct{
	UINT										calibItemNo;				/**< Calibraion item no							*/
	UINT										result;						/**< result										*/
}T_SYS_CALIB_CTRL_RSP_INFO;
/** @} */

/****************************************************************************************************************************************/
/** }T_SYS_CALIB_CTRL_RSP
 *  @addtogroup RRH_SYS
 *  @{
 */
/****************************************************************************************************************************************/
/**
 * @brief	}T_SYS_CALIB_CTRL_RSP
 * @note	}T_SYS_CALIB_CTRL_RSP(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdIf;					/**< Thread Common Header						*/
	T_SYS_CALIB_CTRL_RSP_INFO						calibCtrlRspInfo;		/**< CALIB_CTRL_RSP Info						*/
}T_SYS_CALIB_CTRL_RSP;
/** @} */

/****************************************************************************************************************************************/
/** }T_SYS_FLASH_READ_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/****************************************************************************************************************************************/
/**
 * @brief	}T_SYS_FLASH_READ_REQ
 * @note	}T_SYS_FLASH_READ_REQ(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdIf;					/**< Thread Common Header						*/
	UINT											face;					/**< FLASH起動面								*/
	UINT											offset;					/**< FLASH-OFFSET-ADR							*/
	UINT											size;					/**< FLASH-READ-SIZE							*/
}T_SYS_FLASH_READ_REQ;

/****************************************************************************************************************************************/
/** }v
 *  @addtogroup RRH_SYS
 *  @{
 */
/****************************************************************************************************************************************/
/**
 * @brief	}T_SYS_FLASH_READ_RSP
 * @note	}T_SYS_FLASH_READ_RSP(header + data)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdIf;					/**< Thread Common Header						*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT											result;					/**< Result										*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT											size;					/**< FLASH-READ-SIZE							*/
	UCHAR											data[D_SYS_FLASH_READ_DATA];/**< READ-data								*/
}T_SYS_FLASH_READ_RSP;

/** @} */

/**
 * @brief	Structure of LMC Erase Request
 * @note	Structure of LMC Erase Request
 * 
 */
typedef struct{
	UINT				kind;			/**< flash kind  */
	UINT				face;			/**< flash face  */
	UINT				offset;			/**< flash offset  */
	UINT				size;			/**< Flash size  */
}T_SYS_CLRFROM;

typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;		/**< header	*/ 
	UINT				cause;			/**< clear cause	*/
	UINT				clearFileNum;	/**< clear file number 	*/
#if 0
	UINT				face;			/**< flash face  */
#endif
	T_SYS_CLRFROM	clearFlashRom[32];	/**< clearFlashRomRequest  */
}T_SYS_FLSERASE_REQ;

/**
 * @brief	Flash Erase Response
 * @note	Flash Erase Response
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;		/**< header		*/ 
	UINT               	cause;			/**< cause		*/       
	UINT               	result;			/**< Result		*/    
}T_SYS_FLSERASE_RSP;


/**
 * @brief	Structure of LMC Write Request
 * @note	Structure of LMC Write Request
 * 
 */
/**
 * @brief	writeFlashRomRequest
 * @note	writeFlashRomRequest is ..
 * 
 */
typedef struct{
	UINT				face;			/**< flash face   */
	UINT				offset;			/**< flash offset  */
	UCHAR				fileName[32];		/**< data address to flash */
	UINT				size;			/**< the data szie  */
	UCHAR				*data;			/**< data pointer */
}T_SYS_WRTFROM;

typedef struct
{
	T_SYS_COMMON_THDIF	thdIf;			/**< header	*/ 
	UINT				writeFileNum;	/**< write File number  */
	T_SYS_WRTFROM		writeFlashRom[32];	/**< writeFlashRomRequest  */
}T_SYS_LMCWRT_REQ;

/**
 * @brief	Flash Write Response
 * @note	Flash Write Response
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;			/**< header */ 
	UINT               result;				/**< Result		*/    
}T_SYS_LMCWRT_RSP;

/**
 * @brief	Tar Thread Response
 * @note	Tar Thread Response
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;			/**< header */ 
	UINT               result;				/**< Result		*/    
}T_SYS_UNZIP_RSP;

/**
 * @brief	Structure of FW Info Save Request
 * @note	Structure of FW Info Save Request
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	T_SYS_COMMON_THDIF	thdIf;			/**< header				*/
	USHORT				savepos;		/**< save position		*/
	T_RRH_FW_INFO		fwinfo;			/**< FW Info 			*/
}T_SYS_FWINFO_SAVE_REQ;

/**
 * @brief	FW Info Save Response
 * @note	FW Info Save Response
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct
{
	T_SYS_COMMON_THDIF	thdIf;			/**< header		*/
	UINT				result;			/**< Result		*/
}T_SYS_FWINFO_SAVE_RSP;


/******************************************************************************************************************************/
/**
 * @brief	External receive thread start
 * @note	External receive thread start is ..
 * 
 */
typedef struct{
	T_RRH_HEAD						head;								/**< Thread Common Header						*/
	INT								fdhandle;
}T_SYS_EXTRX_START;
/** @} */
/**
 * @brief	EXT receiving notice
 * @note	EXT receiving notice
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   	thdIf;					/**< header */ 
	INT						result;					/**< 受信結果				 */
	UINT					devno;					/**< 装置番号				 */
	UINT					datalen;				/**< データ長					 */
	CHAR					msgbuf[1024];				/**< メッセージ・バッファ		 */
}T_SYS_EXTRX_NTC;

/******************************************************************************************************************************/
/**
 * @brief	External receive thread start
 * @note	External receive thread start is ..
 * 
 */
typedef struct
{
	UINT					devno;					/**< 装置番号				 */
	UINT					datalen;				/**< データ長					 */
	CHAR					msgbuf[1024];				/**< メッセージ・バッファ		 */
}T_SYS_EXTSND_REQ;
/** @} */

/******************************************************************************************************************************/
/**
 * @brief	External sending response 
 * @note	External sending response is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   	thdIf;					/**< header */ 
	UINT					devno;					/**< 装置番号				 */
	UINT					result;					/**< データ長					 */
}T_SYS_EXTSND_RSP;
/** @} */

/**
 * @brief	data reserve common structure for FMNG receive data head
 * @note	Contents: data reserve common structure \n
 * 
 */

/* 16 bytes */
typedef struct
{
	UINT		 		dev_info;							/* device information */
	UINT		 		wrt_add;							/* write address */
	UINT		 		file_offset;						/* file information offset */
	UINT		 		dummy;								/* dummy */
}T_SYS_DLINFO_S3G;
/* @} */

/* 16 bytes */
typedef struct
{
	UINT				file_num;							/* file number */
	UINT				zipfile_offset;					/* compressed file offset */
	UINT				zipfile_size;						/* compressed file size */
	UINT				dl_valid_f;							/* DL valid flag */
	
}T_SYS_FILEHEAD_S3G;

/* 48 bytes */
typedef struct
{
	UINT				unzipfile_size;					/* decompressed file size */
	UINT				file_ver;							/* file version */
	UINT				zip_type;							/* compress type */
	UINT				dummy;								/* dummy */
	UCHAR				unzipfile_name[32];				/* decompressed file name */
}T_SYS_FILEINFO_S3G;

/* (256+4*4+16*15)=512 bytes */
typedef struct
{
	USHORT	 			file_sum;                			/**<   結合数                */
	USHORT				mpu_mj_ver;                			/**<   MPUファームMJバージョン    */
	USHORT				mpu_mn_ver;                			/**<   MPUファームMNバージョン    */
	USHORT				cprirre_mj_ver;            			/**<   CPRI-FPGA(RRE用)MJバージョン    */
	USHORT				cprirre_mn_ver;           	 		/**<   CPRI-FPGA(RRE用)MNバージョン    */
	USHORT				cpri_mj_ver;              	 		/**<   CPRI-FPGA(BRE/LRE用)MJバージョン   */
	USHORT				cpri_mn_ver;              	 		/**<   CPRI-FPGA(BRE/LRE用)MNバージョン   */
	USHORT				apd_mj_ver;               		 	/**<   APD-FPGA MJバージョン    */
	USHORT				apd_mn_ver;               			/**<   APD-FPGA MNバージョン    */
	USHORT				vupu_mj_ver;              	 		/**<   VUPU MJバージョン    */
	USHORT				vupu_mn_ver;              	 		/**<   VUPU MNバージョン    */
	UINT        		mpu_size;				  			/**<   MPUファームsize       */
	UINT        		cprirre_size;			  			/**<   CPRI-FPGA(RRE用)size       */
	UINT        		cpri_size;				  			/**<   CPRI-FPGA(BRE/LRE用)size       */
	UINT        		apd_size;				  			/**<   APD-FPGA size       */
	UINT	  			vupu_size;				  			/**<   vupu size       */
	USHORT				mpu_cstdwn;							/**<   MPUコストダウン有無		*/
	USHORT				hrd_cstdwn;							/**<   Hardコストダウン有無		*/
	USHORT				file_sum_lsi;						/**<   LSI版用ファイル数       */
	USHORT				cprirre_lsi_mj_ver;					/**<   CPRI-FPGA(RRE LSI用)MJバージョン    */
	USHORT				cprirre_lsi_mn_ver;					/**<   CPRI-FPGA(RRE LSI用)MNバージョン    */
	USHORT				vupu_lsi_mj_ver;					/**<   VUPU(RRE LSI用)MMJバージョン    */
	USHORT				vupu_lsi_mn_ver;					/**<   VUPU(RRE LSI用)MMNバージョン    */
	UINT        		cprirre_lsi_size;					/**<   CPRI-FPGA(RRE LSI用)size       */
	UINT				vupu_lsi_size;						/**<   VUPU(RRE LSI用)size       */
	USHORT				mpu_eco;							/**<   MPU  eCO-800M,1.5G 有無	*/
	USHORT				hrd_eco;							/**<   Hard eCO-800M,1.5G 有無	*/
	USHORT				dummy;								/**<   ダミー					*/
	USHORT				mpu_slc;							/**<   MPU  SLC-RRE 有無		*/
	USHORT				hrd_slc;							/**<   Hard SLC-RRE 有無		*/
	UINT        		cpri_tra_cpri_size;					/**<   CPRI-FPGA(TRA CPRI CD用)size	*/
	USHORT				mpu_tra_cpri;						/**<   MPU  TRA CPRI CD 有無		*/
	USHORT				hrd_tra_cpri;						/**<   Hard TRA CPRI CD 有無		*/
	UINT				mpu2_1chp_flg;
	UINT				mpu2_1chp_off;
	UINT				mpu2_1chp_size;
	UINT				cpri2_1chp_flg;						/**<   1chip FPGA 有無			*/
	UINT				cpri2_1chp_off;						/**<   1chip FPGA Offset		*/
	UINT				cpri2_1chp_size;					/**<   1chip FPGA Size			*/
	UCHAR		 		reserve[150];						/* reserve */
	UINT		 		dl_num;								/* download information number */
	UINT		 		filehead_offset;					/* file information head offset */
	UINT		 		filehead_size;						/* file information head size */
	UINT		 		zip_type;							/* zip type */
	T_SYS_DLINFO_S3G 	dl_info[D_SYS_NUM15];				/* download information(1~15) */
}__attribute__ ((packed)) T_SYS_MAKER_FIXHEAD_S3G;

/* (4+2*8+16*2+2+458+512)=1024 bytes */
typedef struct {
	UINT				pgm_size;						/**< プログラムサイズ			 */
	USHORT				chk_sum;						/**< チェックサム				 */
	USHORT				mk_jdg;							/**< メーカ識別子				 */
	UCHAR				mk_name[D_SYS_NUM16];				/**< メーカ名称				 */
	UCHAR				file_kind[D_SYS_NUM16];			/**< ファイル種別名称			 */
	USHORT				file_type;						/**< ファイルタイプ			 */
	USHORT				mjr_ver;						/**< メジャーバージョン		 */
	USHORT				min_ver;						/**< マイナーバージョン		 */
	USHORT				revision;						/**< リビジョン				 */
	USHORT				year;							/**< 年						 */
	UCHAR				month;							/**< month    */
	UCHAR				day;							/**< 月日						 */
	USHORT				mk_jdg2;                       			 /**< 第二メーカ識別子*/
	UCHAR				reserve[D_SYS_RCVDAT_MKCOM_RSV];		/**< 予約領域					 */
	T_SYS_MAKER_FIXHEAD_S3G mk_reserve;					/**< メーカ固有領域			 */
	UCHAR				data[0x1000000 - 0x400];		/**< 圧縮データ(16M- 1K)	 */
} T_SYS_RCVDATA_MKHEAD_S3G;

/* ========================================================================  */
/* MESSAGE :CPRI State変化通知/CPRIS State変化通知(T_SYS_CPRIST_IRQNTC)		 */
/* ========================================================================  */
typedef struct
{
	T_RRH_HEAD		head;					/**< タスク共通ヘッダ			 */
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	USHORT			link_num;				/**< CPRI 番号					 */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	UINT			data1;					/**< IRQ割込情報(0x80000120)	 */
	UINT			data2;					/**< 0x81000004(CPSTAT)			 */
	UINT			data3;					/**< 0x81000008(CPTRANS)		 */
} T_SYS_CPRIST_IRQNTC;						/* Primary,Secondary変化で型共通 */

/* ========================================================================  */
/* MESSAGE :MCPRI State変化通知(T_SYS_MCPRIST_IRQNTC)						 */
/* ========================================================================  */
typedef struct
{
	T_RRH_HEAD		head;					/**< タスク共通ヘッダ			 */
	UINT			data1;					/**< IRQ割込情報(0x80001120)	 */
	UINT			data2;					/**< 0x81001004(CPSTAT)			 */
	UINT			data3;					/**< 0x81001008(CPTRANS)		 */
} T_SYS_MCPRIST_IRQNTC;

/* ========================================================================  */
/* MESSAGE :TRAカード状態変化割込通知(T_SYS_TRA_IRQNTC)						 */
/* ========================================================================  */
typedef struct
{
	T_RRH_HEAD		head;						/**< タスク共通ヘッダ	   	 */
	UINT			sv_inf[D_RRH_SV_INF_NUM];	/**< TRA SV情報#SV1～SV#8  	 */
} T_SYS_TRA_IRQNTC;

/* ========================================================================  */
/* MESSAGE :TRAカード状態変化割込通知(T_SYS_TRA_IRQNTC)						 */
/* ========================================================================  */
typedef struct
{
	T_RRH_HEAD		head;						/**< タスク共通ヘッダ	   	 */
	UINT			sv_inf[D_RRH_SV_INF_NUM];	/**< TRA SV情報#SV1～SV#8  	 */
	UINT			sv_inf2[D_RRH_SV_INF_NUM];	/**< TRA SV情報#SV9～SV#16 	 */
} T_SYS_TRA_IRQNTC_RRE;

/* ========================================================================  */
/* MESSAGE :カード状態変化割込通知(T_SYS_EQS_ALM_INFO)						 */
/* ========================================================================  */
typedef struct
{
	T_SYS_EQS_HEAD	thdIf;						/**< タスク共通ヘッダ		 */
	UINT			sv_inf[D_DU_SVINF_NUM];		/**< SV情報SV#1～SV#16		 */
} T_SYS_EQS_ALM_INFO;

/* ========================================================================  */
/* MESSAGE :PORT変化通知(T_SYS_PORT_IRQNTC)									 */
/* ========================================================================  */
typedef struct{
	T_SYS_COMMON_THDIF	thdif;					/**< Thread Common Header	 */
	UINT				port_in;				/**< FLASH-READ-SIZE		 */
}T_SYS_PORT_IRQNTC;


/* ========================================================================  */
/* MESSAGE :高速モード要求(T_SYS_TX_HIGHSPEEDNTC)	 						 */
/* ========================================================================  */
typedef struct
{
	T_RRH_HEAD		head;					/**<  タスク共通ヘッダ		 */
	UINT				irqInfo;
} T_SYS_TX_HIGHSPEEDNTC;

/* ========================================================================  */
/* MESSAGE :高速モード要求(T_SYS_PASW_NTC)	 								 */
/* ========================================================================  */
/**
 * @brief	}T_SYS_PASW_NTC
 * @note	}T_SYS_PASW_NTC(header)
 * 
 */
typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header						*/
}T_SYS_PASW_NTC;

/* ========================================================================  */
/* MESSAGE :FWFB_JESD通知(T_SYS_FWFB_JESD)									 */
/* ========================================================================  */
typedef struct{
	T_SYS_COMMON_THDIF	thdif;					/**< Thread Common Header	 */
	UINT				infFWFB;				/**< FWFB_JESD割り込み通知	 */
}T_SYS_FWFB_JESD;


/** T_SYS_SYSTEM_CAR_POW_THRESH_SET_REQ
 *  @addtogroup RRH_SYS
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	T_SYS_SYSTEM_CAR_POW_THRESH_SET_REQ(キャリア電力閾値設定)
 * @note	T_SYS_SYSTEM_CAR_POW_THRESH_SET_REQ(header + data)
 * 
 */
typedef struct{
	USHORT											power;
}T_SYS_SYSTEM_CAR_POW_THRESH_SET_INFO_REQ;


typedef struct{
	T_SYS_COMMON_THDIF								thdif;					/**< Thread Common Header				*/
	T_SYS_SYSTEM_CAR_POW_THRESH_SET_INFO_REQ		powInfo;				/**< 送信出力調整要求情報 				*/
}T_SYS_SYSTEM_CAR_POW_THRESH_SET_REQ;
/** @} */

/* ========================================================================  */
/* MESSAGE :auto nego complete notification									 */
/* ========================================================================  */
typedef struct{
	T_SYS_COMMON_THDIF	thdif;					/**< Thread Common Header	*/
	UINT				bit_rate;				/**< Bit Rate	*/
	UINT				auto_nego_result;		/**< Auto Nego Result	*/
}T_SYS_AUTO_NEGO_COMPLETE_NTC;

/* ========================================================================  */
/* MESSAGE :auto nego start indication										 */
/* ========================================================================  */
typedef struct{
	T_SYS_COMMON_THDIF	thdif;					/**< Thread Common Header	*/
	UINT				bit_rate;				/**< Bit Rate	*/
}T_SYS_AUTO_NEGO_START_IND;

/* ========================================================================  */
/* MESSAGE :auto nego stop indication										 */
/* ========================================================================  */
typedef struct{
	T_SYS_COMMON_THDIF	thdif;					/**< Thread Common Header	*/
}T_SYS_AUTO_NEGO_STOP_IND;

/* ========================================================================  */
/* MESSAGE :auto nego timeout notification									 */
/* ========================================================================  */
typedef struct{
	T_SYS_COMMON_THDIF	thdif;					/**< Thread Common Header	*/
}T_SYS_AUTO_NEGO_TIMEOUT_NTC;

/* ========================================================================  */
/* MESSAGE :auto nego restart indication									 */
/* ========================================================================  */
typedef struct{
	T_SYS_COMMON_THDIF	thdif;					/**< Thread Common Header	*/
	UINT				bit_rate;				/**< Bit Rate	 */
}T_SYS_AUTO_NEGO_RESTART_IND;

/**********************************************************************
	The message IF between pf_al7  and pf_al2
***********************************************************************/

/**
 * @brief	XID sending request
 * @note	XID sending request is ..
 * @{
 */
typedef struct
{
	T_SYS_COMMON_THDIF	thdIf;				/**<  header	*/ 
	UINT               	uiHdlcAdr;			/**<  HDLC address		*/    
	UINT               	uiType;				/**<  XID type			*/   

	USHORT				usBaudRate;			/**< baudrate			*/    
	USHORT				usEndian;			/**< transfer endia		*/    
	
	UINT				uiVer3GPP;			/**<  3GPP version		*/   
	UINT				uiVerAisg;			/**<  AISG version		*/   
	UINT				uiMaxRxLength;		/**<  Max transmit size	*/   
	UINT				uiMaxTxLength;		/**<  Max receive size		*/   	  
	UINT				uiTxWndSize;		/**<  Transmit window size	*/   
	UINT				uiRxWndSize;		/**<  Receive window size	*/
}T_SYS_XID_SND_REQ;
/** @} */
/**
 * @brief	XID sending response
 * @note	XID sending response is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;				/**<  header	      */ 

	UINT               uiHdlcAdr;				/**<  HDLC address             */    
	UINT               uiResult;				/**<  Result                   */     
	UINT               uiType;					/**<  XID type                 */   
	UINT               uiVer3GPP;				/**<  3GPP version             */   
	UINT               uiVerAisg;				/**<  AISG version             */   
	UINT               uiMaxTxLength;			/**<  Max transmit size        */   
	UINT               uiMaxRxLength;			/**<  Max receive size         */   	  
	UINT               uiTxWndSize;				/**<  Transmit window size     */   
	UINT               uiRxWndSize;				/**<  Receive window size      */
	UCHAR              ucDevType;				/**<  rRHAisgDeviceType        */   
	UCHAR              dummy[3];				/**<  dummy                    */  	
}T_SYS_XID_SND_RES;

/**
 * @brief	AISG L1 receiving interrupt notification
 * @note	AISG L1 receiving interrupt notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	thdIf;					/**< header		*/ 
	VOIDFUNCPTR	   		f_func;					/**< function pointer		*/ 
	int					iParam;					/**< the input param		*/ 
}T_SYS_AISG_RCV_INT_NTC;
/** @} */

/**
 * @brief	AISG receiving interrupt notification
 * @note	AISG receiving interrupt notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	thdIf;					/**< header		*/ 
}T_SYS_AISG_RCVING_INT_NTC;
/** @} */

/**
 * @brief	AISG L1 receiving data notification
 * @note	AISG L1 receiving data notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;				/**< header					*/ 
	UINT			   uiResult;				/**< 0: Normal 1: L1 conflict		*/ 
	UINT			   uiLength;				/**< Data Length					*/ 
	UCHAR			   ucData[D_SYS_NUM128];	/**< Data (address + control + Info)*/ 
	USHORT			   usErr[D_SYS_NUM4];
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	UINT			   uiAllLength;				/**< ALL Length					*/ 
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
}T_SYS_AISG_RCV_DAT_NTC;
/** @} */

/**
 * @brief	Device scan indication
 * @note	Device scan indication is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;					/**< header 		*/ 

	USHORT               usBaudRate;				/**< baudrate			*/    
	USHORT               usEndian;					/**< transfer endia            */    

	UINT               uiUniqueLength;				/**<  Unique id length         */    
	UCHAR              ucUniqueID[D_SYS_NUM20];		/**<  aisgLinkEstablishUniqueId*/   
	UINT               uiMaskLength;				/**<  Mask length              */    
	UCHAR              ucMask[D_SYS_NUM20];			/**<  Mask                     */
}T_SYS_ADEV_SCAN_IND;
/** @} */

/**
 * @brief	Device scan complete notification
 * @note	Device scan complete notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF		thdIf;					/**< header			*/ 
	UINT               		uiResult;				/**< Result				*/     
}T_SYS_ADEV_SCANCMP_NTC;

/** @} */

/**
 * @brief	Device mount notification
 * @note	Device mount notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;					/**<  header				*/ 
	UINT				uiUniqueLength;				/**<  Unique id length         */    
	UCHAR              	ucUniqueID[D_SYS_NUM20];	/**<  rRHAisgDeviceScanUniqueID*/     
	UCHAR              	ucDevType;					/**<  rRHAisgDeviceType        */   
	UCHAR              	dummy[3];					/**<  dummy                    */   
}T_SYS_ADEV_MOUNT_NTC;
/** @} */

/**
 * @brief	DISC sending reqest
 * @note	DISC sending reqest is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;					/**<  header 					*/ 
	UINT               uiHdlcAdr;					/**<  HDLC address             */    
}T_SYS_DISC_SND_REQ;
/** @} */

/**
 * @brief	DISC sending response
 * @note	DISC sending response is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;					/**<  header		*/ 
	UINT               uiHdlcAdr;					/**<  HDLC address		*/    
	UINT               uiResult;					/**<  Result			*/     
}T_SYS_DISC_SND_RES;

/** @} */

/**
 * @brief	AISG link release indication
 * @note	AISG link release indication is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;				/**<  header		*/ 
	UINT               uiHdlcAdr;				/**<  HDLC address		*/    
}T_SYS_ALNK_REL_IND;
/** @} */

/**
 * @brief	AISG L1 sending complete notification
 * @note	AISG L1 sending complete notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;				/**<  header	*/ 
}T_SYS_AISG_SNDCMP_NTC;
/** @} */

/**
 * @brief	AISG L2 Timer Time out notification
 * @note	AISG L2 Timer Time out notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;				/**< header		*/ 
	UINT			   uiTimId;					/**< Time ID 			*/
}T_SYS_AISG_L2TO_NTC;
/** @} */

/**
 * @brief	AISG signal sending request
 * @note	AISG signal sending request ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;					/**<  header				*/ 
	UINT               uiHdlcAdr;					/**<  HDLC address				*/    
	UINT               uiLength;					/**<  Message length			*/     
	UCHAR              ucMsg[D_SYS_NUM74];			/**<  rRHAisgL7ResponseMessage	*/  
	UCHAR              dummy[D_SYS_NUM2];			/**<  dummy						*/  	
}T_SYS_ACMD_SND_REQ;

/** @} */

/**
 * @brief	AISG signal receiving notification
 * @note	AISG signal receiving notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;						/**<  header 	        	*/ 
	UINT               uiHdlcAdr;                  		/**<  HDLC address             */    
	UINT               uiLength;                  		/**<  Message length           */     
	UCHAR              ucMsg[D_SYS_NUM74];              /**<  rRHAisgL7ResponseMessage */ 
	UCHAR              dummy[D_SYS_NUM2];               /**<  dummy                    */  
}T_SYS_ACMD_RCV_NTC;

/** @} */

/**
 * @brief	AISG signal sending response
 * @note	AISG signal sending response is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;				/**<  header		*/ 
	UINT               uiHdlcAdr;				/**<  HDLC address		*/    
	UINT               uiResult;				/**<  Result			*/     
}T_SYS_ACMD_SND_RES;
/** @} */

/**
 * @brief	SNRM sending request
 * @note	SNRM sending request is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	thdIf;				/**<  header 		*/ 
	UINT				uiHdlcAdr;			/**<  HDLC address		*/    
	USHORT				usBaudRate;			/**<  HDLC address		*/    
	USHORT				usEndian; 			/**<  HDLC address		*/    
}T_SYS_SNRM_SND_REQ;
/** @} */

/**
 * @brief	SNRM sending response
 * @note	SNRM sending response is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;				/**<  header					*/ 
	UINT					uiHdlcAdr;			/**<  HDLC address             */    
	UINT					uiResult;			/**<  Result                   */     
	USHORT					usBandRate;
	USHORT					usEndian;
}T_SYS_SNRM_SND_RES;
/** @} */

/**
 * @brief	AISG link down notification
 * @note	AISG link down notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF	   thdIf;			/**<  header	*/ 
	UINT               uiHdlcAdr;			/**<  HDLC address	*/    
}T_SYS_ALNK_DWN_NTC;


/**
 * @brief	AISG auto close timeout notification
 * @note	AISG auto close timeout notification is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF		thdIf;						/**< task header */
	UINT					uiTimerId;
	UINT    				uiHdlcAdr;                  /**<  HDLC address             */    
}T_SYS_AISG_TIMRTO_NTC;

/**
 * @brief	MSI receive Indication
 * @note	MSI receive Indication is ....
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF		thdIf;						/**< task header */
}T_SYS_MSI_RCV_IND;

/**
 * @brief	MSI send Indication
 * @note	MSI send Indication is ..
 * 
 */
typedef struct
{
	T_SYS_COMMON_THDIF		thdIf;						/**< task header */
}T_SYS_MSI_SND_IND;

/**
 * @brief	T_SYS_NMA_LOG_WRITE_REQ
 * @note	T_SYS_NMA_LOG_WRITE_REQ is ..
 * 
 */
typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					eraseWriteKind;				/**< 消去書込み種別	*/
} T_SYS_NMA_LOG_WRITE_REQ;

/**
 * @brief	T_SYS_NMA_FILE_WRITE_REQ & RSP
 * @note	T_SYS_NMA_FILE_WRITE_REQ & RSP is ..
 * 
 */
typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					kind;						/**< ファイル種別	*/
	char					name[128];					/**< ファイル名		*/
} T_SYS_NMA_FILE_WRITE_REQ;

typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					kind;						/**< ファイル種別	*/
	UINT					result;						/**< 処理結果		*/
} T_SYS_NMA_FILE_WRITE_RSP;

/**
 * @brief	T_SYS_NMA_NETCONF_WRITE_REQ & RSP
 * @note	T_SYS_NMA_NETCONF_WRITE_REQ & RSP is ..
 * 
 */
typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					requestID;					/**< requestID		*/
} T_SYS_NMA_NETCONF_WRITE_REQ;

typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					result;						/**< 処理結果		*/
	UINT					requestID;					/**< requestID		*/
} T_SYS_NMA_NETCONF_WRITE_RSP;

/**
 * @brief	T_SYS_NMA_VLAN_WRITE_REQ & RSP
 * @note	T_SYS_NMA_VLAN_WRITE_REQ & RSP is ..
 * 
 */
typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					requestID;					/**< requestID		*/
	UCHAR					searchable_vlans[512];		/**< searchable-access-vlans	*/
} T_SYS_NMA_VLAN_WRITE_REQ;

typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					result;						/**< 処理結果		*/
	UINT					requestID;					/**< requestID		*/
} T_SYS_NMA_VLAN_WRITE_RSP;

/**
 * @brief	T_SYS_PFM_1S_TIMEOUT_NTF
 * @note	T_SYS_PFM_1S_TIMEOUT_NTF is ..
 * 
 */
typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					requestID;					/**< requestID		*/
} T_SYS_PFM_1S_TIMEOUT_NTF;
/** @} */


/**
 * @brief	T_SYS_MT_BEAMID_SET_REQ
 * @note	T_SYS_MT_BEAMID_SET_REQ is ..
 * 
 */
typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					uiBeamId;					/**< BeamId			*/
} T_SYS_MT_BEAMID_SET_REQ;
/** @} */

/**
 * @brief	T_SYS_MT_BEAMID_SET_RSP
 * @note	T_SYS_MT_BEAMID_SET_RSP is ..
 * 
 */
typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					uiBeamId;					/**< BeamId			*/
	UINT					uiOK_NG;					/**< OKNG			*/
	UINT					uiNG_Code;					/**< NG code		*/
} T_SYS_MT_BEAMID_SET_RSP;
/** @} */


/**
 * @brief	T_SYS_NMA_LUTLOG_WRITE_NFT
 * @note	T_SYS_NMA_LUTLOG_WRITE_NFT is ..
 * 
 */
typedef struct {
	T_SYS_COMMON_THDIF		thdIf;						/**< task header	*/
	UINT					uiIndex;					/**< writeIndex		*/
} T_SYS_NMA_LUTLOG_WRITE_NFT;
/** @} */

#endif	/* F_SYS_TYP_H */

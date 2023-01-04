/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_rsv_ftyp.h
 *  @brief	RE Supervision common function prototype definition
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
 *  @date   2018/02/14 FJT)H.Yoshida add 5G-DU対応
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2018
 */
/******************************************************************************************************************************/

/* 二重取込み防止 */
#ifndef		F_COM_TYP
#define		F_COM_TYP


/**
 * @brief	alarm mapping table
 * @note	alarm mapping table \n
 * 
 */
typedef struct
{
	USHORT 				alm_no;					/**< */
	UINT 				svctl_typ;				/**< */
	UINT				alm_factor_addr;		/**< */
	VOID*				alm_mask_addr;			/**< */
	UINT				svbit;					/**< */
	UCHAR				alm_text[16];			/**< */
}T_COM_HARDIDXTBL;

/**
 * @brief	alarm mapping table for 1chip
 * @note	alarm mapping table for 1chip\n
 * @date 2021/01/19 M&C) Merge 4G FHM src
 */
typedef struct
{
	USHORT 				alm_no;					/**< */
	UCHAR 				alm_typ;					/**< */
	UCHAR 				detect_typ;				/**< */
	UINT				alm_factor_addr;			/**< */
	UINT				alm_mask_addr;			/**< */
	UINT				svctl_addr;				/**< */
	UINT				svbit;					/**< */
	UCHAR				alm_text[16];				/**< */
}T_COM_HARDIDXTBL_1CHP;

/**
 * @brief	Flash log save info table
 * @note	Flash log save info table
 * 
 */
typedef struct {
	unsigned int		shmid;					/* shared memory index						*/
	int					logSize; 				/* log size									*/
}T_COM_LOG_SAVE_TBL;

/**
 * @brief	Flash log compress table
 * @note	Flash log compress table
 * 
 */
typedef struct {
	UCHAR				compsize[4];							/* shared memory index						*/
	UCHAR				logdata[D_RRH_FLASH_LOG_SIZE]; 			/* log size									*/
}T_COM_LOG_COMPRESS_TBL;

/**
 * @brief	AISG Device Entry
 * @note	AISG Device Entry is ..
 * 
 */
typedef struct{
	/*this uiLink is used for linkstate and device scannning*/
	USHORT				uiLink;						/**< logical link */
	USHORT				ucDevType;					/**< Device type */
	UINT				uiUniqueLength;				/**< Unique id length  */
	UCHAR				ucUniqueID[D_SYS_NUM20];	/**< Unique id  */

	UINT				uiHdlcAdr;					/**< HDLC address  */
	UINT				uiState;					/**< State  */

	USHORT 				usBaudRate;					/**< baud rate		*/
	USHORT				usEndian;					/**< FCS endian		*/
}T_COM_AISGDEV;
/* @} */

/**
 * @brief	Structure of AISG static
 * @note	Structure of AISG static table
 * 
 */
 typedef struct{
	UINT xidValue[D_COM_STATTYP_MAX];							/*the total bytes of transmission*/
	UINT linkValue[D_SYS_AISG_MAX_DEVNUM][D_COM_STATTYP_MAX];	/*the total bytes of transmission*/
 }T_COM_AISGSTATICS;
/* @} */

/**
 * @brief	Structure of DU Connetion Information Table (DBG)
 * @note	Structure of DU Connetion Information Table (DBG)
**/
typedef struct{
	UCHAR					dFlag;					/**< デバッグフラグ		*/
	UCHAR					dummy[3];				/**< dummy				*/
	UINT					src_addr[D_RRH_IF_MAX];	/**< src IP adr (dbg)	*/
	UINT					dst_addr;				/**< dst IP adr (dbg)	*/
} T_COM_DU_CNCT_DBG_TBL ;

/**
 * @brief	SV Bus contrl infomation table
 * @note	SV Bus contrl infomation table is ..
 * 
 */
typedef struct{
	UINT	kind;		/*	kind				*/
	UINT	val;		/*	value				*/
	UINT	svOpt1;		/*	Sv Option			*/
	UINT	opt1;		/*	option info1		*/
	UINT	opt2;		/*	option info2		*/
	UINT	astFlg;		/*	Assert log flag		*/
	UCHAR	optStr[36];	/*	log disp string		*/
}T_COM_DU_SVBUS_CTRL_INFO_TBL;


typedef struct{
	T_COM_DU_SVBUS_CTRL_INFO_TBL	*infop;		/*	info pointer				*/
	UINT							infoSize;	/*	info table size				*/
	UINT							type;		/*	info type					*/
	UINT							mask;		/*	mask value					*/
	UINT							addr;		/*	set address					*/
	UINT							shift;		/*	shift for changing vaule	*/
	UINT							def;		/*	default value				*/
}T_COM_DU_SVBUSCTRL_MAP_TBL;

typedef struct{
	UINT							addr;		/*	SV info register address		*/
	UINT							mskAddr;	/*	SV info mask register address	*/
	UINT							infoNum;	/*	SV info kind Munber				*/
	UINT							opt[D_DU_SVCTL_SVXX_MAX];	/*	SV info Operaition Number	*/
}T_COM_DU_SVOPT_DEF_TBL;

typedef struct{
	UINT							validFlg;
	INT								onOffNum;
}T_COM_DU_FLTNUMINF;

typedef struct{
	UINT							num;		/*	info number		*/
	UINT							osft[E_DU_SVCTL_FLVNO_MAX];
	T_COM_DU_FLTNUMINF				info[E_DU_SVCTL_FLVNO_MAX];
}T_COM_DU_FLTLVLNUM;

typedef struct{
	UINT							num;		/*	info number		*/
	UINT							osft[E_DU_SVCTL_OCCUR_FIDNO_MAX];
	T_COM_DU_FLTNUMINF				info[E_DU_SVCTL_OCCUR_FIDNO_MAX];
}T_COM_DU_FLTIDNUM;
/* @} */

/**
 * @brief	Structure of SFP monitor
 * @note	Structure of SFP monitor table
 * {@
 */
typedef struct{
	UINT							present;					/* present							*/
	UCHAR							vendor_id[8];				/* vendor-id						*/
	UCHAR							vendor_part[20];			/* vendor-part						*/
	UCHAR							vendor_rev[8];				/* vendor-rev						*/
	UCHAR							serial_no[20];				/* serial-no						*/
	UCHAR							SFF8472_compliance_code;	/* SFF8472-compliance-code			*/
	UCHAR							connector_type;				/* connector-type					*/
	UCHAR							nominal_bitrate;			/* nominal-bitrate					*/
	UCHAR							low_bitrate_margin;			/* low-bitrate-margin				*/
	UCHAR							high_bitrate_margin;		/* high-bitrate-margin				*/
	UCHAR							rx_power_type;				/* rx-power-type					*/
	DL								rx_power;					/* rx-power							*/
	DL								tx_power;					/* tx-power							*/
	DL								tx_bias_current;			/* tx-bias-current					*/
	DL								voltage;					/* voltage							*/
	DL								temperature;				/* temperature						*/
}T_COM_TRANSCEIVER_INFO;

typedef struct{
	UINT							error_cnt;
	T_COM_TRANSCEIVER_INFO			info[D_RRH_SFP_MAXNUM];		/* SFP monitor情報					*/
}T_COM_TRANSCEIVER_MONITOR_TBL;
/* @} */


typedef struct{
	UINT							checkbit;
	INT								sd;
	UCHAR							head[32];
}T_COM_DUMP_HEAD;
/* @} */

typedef struct{
	pthread_mutex_t					mutex;
	T_COM_DUMP_HEAD					dumpHead[3];
}T_COM_DUMP_MNG;
/* @} */



#endif	/* M_RS_FTYP */


/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also プロセス間IF仕様書(xxx-API).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/******************************************************************************************************************************/

#include "rrhApi_Com.h"
/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_FILE_H
#define RRHAPI_FILE_H

/*!
 * @addtogroup RRH_API_FILE
 * @{
 */
#define D_API_MSGID_FILE_BASE						0xA0080000
#define D_API_MSGID_FILE_FIRM_ERASE_REQ				0xA0080001
#define D_API_MSGID_FILE_FIRM_ERASE_RSP				0xA0080002
#define D_API_MSGID_FILE_FIRM_UPDATE_REQ			0xA0080003
#define D_API_MSGID_FILE_FIRM_UPDATE_RSP			0xA0080004
#define D_API_MSGID_FILE_FIRM_VERSION_REQ			0xA0080005
#define D_API_MSGID_FILE_FIRM_VERSION_RSP			0xA0080006
#define D_API_MSGID_FILE_FIRM_CANCEL_IND			0xA0080007
/* 2020/12/29 M&C) Merge 4G FHM src (start add) */
#define D_API_MSGID_FILE_FIRM_CANCEL_RSP            0xA0080008      /**< firmware update cancel response                      */
#define D_API_MSGID_FILE_FIRM_READ_REQ              0xA0080009      /**< firmware read request                                */
#define D_API_MSGID_FILE_FIRM_READ_RSP              0xA008000A      /**< firmware read response                               */
/* 2020/12/29 M&C) Merge 4G FHM src (end add) */
/** @} */

/**
 * @name    firmware type
 * @date    2020/12/28 M&C) Merge 4G FHM src
 * @{
 */
#define D_API_FILE_FW_TYPE_NONE                     0               /**< none                                                 */
#define D_API_FILE_FW_TYPE_FHM                      1               /**< FHM                                                  */
#define D_API_FILE_FW_TYPE_RE                       2               /**< RE                                                   */
/** @} */

/**
 * @brief	The request of load firm data into shared-memory
 * @note	The request of load firm data into shared-memory is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    UINT save_pos;
}T_API_LOAD_FW_DATA;

/**
 * @brief   The request of load firm data into shared-memory
 * @note    The request of load firm data into shared-memory is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;                                     /**< header                                               */
    T_API_LOAD_FW_DATA data;                                        /**< data                                                 */
}T_API_FILE_LOAD_FW_DATA_REQ;

/**
 * @brief	The response of load firm data into shared-memory
 * @note	The response of load firm data into shared-memory is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;                                     /**< header                                               */
    T_API_COM_RSP_DATA data;                                        /**< data                                                 */
}T_API_FILE_LOAD_FW_DATA_RSP;


/*!
 * @addtogroup RRH_API_FILE
 * @{
 */

 /******************************************************************************************************************************/
/** T_API_FILE_ERASE_FIRM_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of erasing firm
 * @note	The request of erasing firm is ..
 */
typedef struct{
	UINT				 side;
}T_API_FILE_ERASE_FIRM_REQ_DATA;
typedef struct{
	T_API_COMMON_HEADER  header;
	T_API_FILE_ERASE_FIRM_REQ_DATA data;
}T_API_FILE_ERASE_FIRM_REQ;
/** @} */

 /******************************************************************************************************************************/
/** T_API_FILE_ERASE_FIRM_RSP
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The response of erasing firm
 * @note	The response of erasing firm is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_COM_RSP_DATA data;
}T_API_FILE_ERASE_FIRM_RSP;
/** @} */

 /******************************************************************************************************************************/
/** T_API_FILE_UPDATE_FIRM_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of updating firm
 * @note	The request of updating firm is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'fw_type', 'save_pos', 'fw_info')
 */
typedef struct{
    UINT dlwType;
    UINT chksum;
    UINT time;
    USHORT fw_type;
    USHORT save_pos;
    T_RRH_FW_INFO fw_info;
}T_API_FILE_SAVE_FW_REQ_DATA;

/**
 * @brief   The request of updating firm
 * @note    The request of updating firm is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	T_API_COMMON_HEADER header;                                     /**< header                                               */
	T_API_FILE_SAVE_FW_REQ_DATA data;                               /**< data                                                 */
}T_API_FILE_SAVE_FW_REQ;

/**
 * @brief   The request of updating firm
 * @note    The request of updating firm is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	UINT	dlwType;
	UINT	chksum;
	UINT	time;
}T_API_FILE_UPDATE_FIRM_REQ_DATA;

typedef struct{
	T_API_COMMON_HEADER header;
	T_API_FILE_UPDATE_FIRM_REQ_DATA data;
}T_API_FILE_UPDATE_FIRM_REQ;
/** @} */

 /******************************************************************************************************************************/
/** T_API_FILE_UPDATE_FIRM_RSP
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of updating firm
 * @note	The request of updating firm is ..
 */
typedef struct{
    UINT dlwType;
    UINT dlwResult;
    CHAR errMsg[128];
}T_API_FILE_SAVE_FW_RSP_DATA;

/**
 * @brief   The request of updating firm
 * @note    The request of updating firm is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;                                     /**< header                                               */
    T_API_FILE_SAVE_FW_RSP_DATA data;                               /**< data                                                 */
}T_API_FILE_SAVE_FW_RSP;

/**
 * @brief   The request of updating firm
 * @note    The request of updating firm is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
	UINT	dlwType;
	UINT	dlwResult;
	CHAR	errMsg[128];
}T_API_FILE_UPDATE_FIRM_RSP_DATA;

typedef struct{
	T_API_COMMON_HEADER header;
	T_API_FILE_UPDATE_FIRM_RSP_DATA data;
}T_API_FILE_UPDATE_FIRM_RSP;
/** @} */

 /******************************************************************************************************************************/
/** T_API_FILE_ERASE_FIRM_RSP
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The response of erasing firm
 * @note	The response of erasing firm is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_FILE_FIRM_VERSION_REQ;
/** @} */
 /******************************************************************************************************************************/
/** T_API_FILE_UPDATE_FIRM_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of updating firm
 * @note	The request of updating firm is ..
 */
typedef struct{
	UCHAR	stringVer[16];
	USHORT	usever;
	USHORT	stupver;
	USHORT year;
	USHORT mon_day;
}T_API_FILE_FIRM_VERSION_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_FILE_FIRM_VERSION_DATA data;
}T_API_FILE_FIRM_VERSION_RSP;
/** @} */

/** @} */

 /******************************************************************************************************************************/
/** T_API_FILE_FIRM_CANCEL_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Cancel Update Firm notice
 * @noteCancel Update Firm notice is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_FILE_FIRM_CANCEL_IND;
/** @} */

/**
 * @brief	The response of load firm data
 * @note	The response of load firm data is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;                                     /**< header                                               */
    T_API_COM_RSP_DATA data;                                        /**< data                                                 */
}T_API_FILE_FIRM_CANCEL_RSP;

/******************************************************************************************************************************/
/** T_API_FILE_READ_EEPROM_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of reading EEPROM
 * @note	The request of reading EEPROM is ..
 */
typedef struct{
	UINT	eepAdr;
}T_API_FILE_READ_EEPROM_REQ_DATA;
typedef struct{
	T_API_COMMON_HEADER 			 header;
	T_API_FILE_READ_EEPROM_REQ_DATA 	 data;
}T_API_FILE_READ_EEPROM_REQ;
/** @} */

/******************************************************************************************************************************/
/** T_API_FILE_READ_EEPROM_RSP
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The response of reading EEPROM
 * @note	The response of reading EEPROM is ..
 */
typedef struct{
	USHORT	result;
	USHORT	eepVal;
}T_API_FILE_READ_EEPROM_RSP_DATA;
typedef struct{
	T_API_COMMON_HEADER 		     header;
	T_API_FILE_READ_EEPROM_RSP_DATA  data;
}T_API_FILE_READ_EEPROM_RSP;
/** @} */

/******************************************************************************************************************************/
/** T_API_FILE_WRITE_EEPROM_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of writing EEPROM
 * @note	The request of writing EEPROM is ..
 */
typedef struct{
	USHORT	eepAdr;
	USHORT	eepVal;
}T_API_FILE_WRITE_EEPROM_REQ_DATA;
typedef struct{
	T_API_COMMON_HEADER 			 header;
	T_API_FILE_WRITE_EEPROM_REQ_DATA 	 data;
}T_API_FILE_WRITE_EEPROM_REQ;
/** @} */

/******************************************************************************************************************************/
/** T_API_FILE_WRITE_EEPROM_RSP
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The response of writing EEPROM
 * @note	The response of writing EEPROM is ..
 */
typedef struct{
	T_API_COMMON_HEADER 		     header;
}T_API_FILE_WRITE_EEPROM_RSP;
/** @} */

/**
 * @brief	Cancel Load Firmware Data Request
 * @note	Cancel Load Firmware Data Request
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;                                     /**< header                                               */
}T_API_FILE_CANCEL_LOAD_FW_DATA_REQ;

/**
 * @brief	Cancel Load Firmware Data Response
 * @note	Cancel Load Firmware Data Response
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct{
    T_API_COMMON_HEADER header;                                     /**< header                                               */
    UINT result;                                                    /**< result of cancel                                     */
}T_API_FILE_CANCEL_LOAD_FW_DATA_RSP;


/**
 * @brief	The request of save firm
 * @note	The request of save firm is ..
 */
typedef struct{
    T_API_COMMON_HEADER header;                                     /**< header                                               */
    UINT result;                                                    /**< result                                               */
}API_FILE_SAVE_RE_FW_RSP;

extern E_RRHAPI_RCODE rrhApi_File_Mpr_SubGetFirmVersion(INT qid, INT wtime,VOID *data_p);
extern E_RRHAPI_RCODE rrhApi_File_Mnr_EraseFirm(	
										INT		qid, INT wtime,VOID	*data_p,
										UINT	side );
extern E_RRHAPI_RCODE rrhApi_File_Mnr_UpdateFirm(	INT	qid, INT wtime,VOID *data_p,
										UINT dlwType, UINT checksum, UINT time );
extern E_RRHAPI_RCODE rrhApi_File_Mnt_CancelUpdFirm(INT qid, INT wtime,VOID *data_p);
extern E_RRHAPI_RCODE rrhApi_File_Fsm_loadMemory(INT *handle);
extern E_RRHAPI_RCODE rrhApi_File_Fsm_SaveData(INT fd,VOID *data, UINT size);
extern E_RRHAPI_RCODE rrhApi_File_Fsm_unLoadMemory(INT fd, INT size, USHORT recvCheckSum);
extern VOID rrh_cm_CheckSumCalc(const VOID*, UINT, USHORT*);

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern E_RRHAPI_RCODE rrhApi_File_Mnr_SaveFirm(INT, UINT, UINT, USHORT, USHORT, T_RRH_FW_INFO*);
extern E_RRHAPI_RCODE rrhApi_File_Mnr_LoadReFileData(INT, USHORT);
extern E_RRHAPI_RCODE rrhApi_File_Fsm_GetReFileInfo(T_RRH_FW_INFO*);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#endif
/** @} */

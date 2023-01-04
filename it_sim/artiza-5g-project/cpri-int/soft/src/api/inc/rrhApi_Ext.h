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

/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_EXT_H
#define RRHAPI_EXT_H


/*!
 * @addtogroup RRH_API_EXT
 * @{
 */
#define D_API_MSGID_EXT_BASE						0xA0050000
#define D_API_MSGID_EXT_MNT_FANDBGMODE_CHG_NTC		0xA0050005
#define D_API_MSGID_EXT_MNT_FANDAC_CHG_NTC			0xA0050007
/** @} */

/*!
 * @addtogroup RRH_API_EXT
 * @{
 */
/******************************************************************************************************************************/
/** T_API_EXT_FAN_DGBMODE_CHG_NTC
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The notification of changing fan debuge mode
 * @note	The notification of changing fan debuge mode
 */
typedef struct{
	T_API_COMMON_HEADER	header;
}T_API_EXT_FAN_DGBMODE_CHG_NTC;
/** @} */

/******************************************************************************************************************************/
/** T_API_EXT_FAN_DAC_CHG_NTC
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The notification of changing parameter for fan control
 * @note	The notification of changing parameter for fan control
 */
typedef struct{
	UINT		dacNo;
	UINT		dacSetVal;
}T_API_EXT_FAN_DAC_INFO;

typedef struct{
	T_API_COMMON_HEADER		header;
	T_API_EXT_FAN_DAC_INFO	fanDacInfo;
}T_API_EXT_FAN_DAC_CHG_NTC;
/** @} */

extern E_RRHAPI_RCODE rrhApi_Ext_Mnt_FanDbgModeChgNtc(	
								INT			qid, 
								INT			wtime,
								VOID		*data_p );

extern E_RRHAPI_RCODE rrhApi_Ext_Mnt_FanDacChgNtc(	
								INT			qid, 
								INT			wtime,
								VOID		*data_p,
								UINT		dacNo,
								UINT		dacSetVal );

/** @} */

#endif

/** @} */


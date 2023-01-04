/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also プロセス間IF仕様書(xxx-API).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/******************************************************************************************************************************/

#include "rrhApi_Com.h"
/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_NEGO_H
#define RRHAPI_NEGO_H

/*!
 * @addtogroup RRH_API_NEGO
 * @{
 */
#define D_API_MSGID_NEGO_BITRATERESULT_DBG			0xA0000013
/** @} */

/****************************************************************************/
/* CPRI LINE BIT RATE														*/
/****************************************************************************/
enum
{
	D_API_NEGO_LINE_BITRATE_UNKNOWN = 0,
/* 2020/12/29 M&C) Merge 4G FHM src (start add) */
	D_API_NEGO_LINE_BITRATE_24G,                    /* 2.4G                     */
/* 2020/12/29 M&C) Merge 4G FHM src (end add) */
	D_API_NEGO_LINE_BITRATE_49G,					/* 4.9G						*/
	D_API_NEGO_LINE_BITRATE_98G						/* 9.8G						*/
};

/*!
 * @addtogroup RRH_API_NEGO
 * @{
 */
 /******************************************************************************************************************************/
/** T_API_NEGO_BITRATERESULT_DBG
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Auto Nego Result Set for Debug
 * @note	Auto Nego Result Set for Debug
 */
typedef struct{
	T_API_COMMON_HEADER header;
	UINT	line_bit_rate;
	UINT	result;
}T_API_NEGO_BITRATERESULT_DBG;
/** @} */

extern E_RRHAPI_RCODE rrhApi_Nego_Mnt_BitRateResult_Dbg(
								INT		qid,	INT		wtime,VOID	*data_p,
								UINT	line_bit_rate,	UINT	result);

/** @} */
#endif
/** @} */

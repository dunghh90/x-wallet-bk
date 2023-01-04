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
#ifndef RRHAPI_INV_H
#define RRHAPI_INV_H

#include "rrhApi_Com.h"

/*!
 * @addtogroup RRH_API_INV
 * @{
 */
extern E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetDevKind(	
								VOID	*data_p,INT size );

extern E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetTraInvInfo(	
								VOID	*data_p,INT size );
								
extern E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetPrioritySystem(VOID *data_p);

extern E_RRHAPI_RCODE rrhApi_Inv_Fsm_SetBandwidth(USHORT value);

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetDevInfo(	
								VOID	*data_p,INT size );
extern E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetSlfInvInfo(
								VOID	*data_p,INT size );
extern E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetSystemPara(
								VOID	*data_p,INT size );
extern E_RRHAPI_RCODE rrhApi_Inv_Fsm_SetSystemPara(
								VOID	*data_p,INT size, UINT offset, USHORT*efParam, INT len );
extern E_RRHAPI_RCODE rrhApi_Inv_Fsm_SetSystemPara_eeprom(UINT offset, USHORT*efParam, INT len);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/** @} */
#endif
/** @} */

/*!
 * @skip  $ld:$
 * @file  f_mpmw_srv.h
 * @brief M-plane Server middleware function
 * @date  2018/11/23 Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup MPMW_SRV
 * @{
 */

#ifndef F_MPMW_SRV_H
#define F_MPMW_SRV_H

#include <stdio.h>
#include <stdarg.h>

void MPMW_SRV_LOG_SET_LEVEL(int loglevel);
int MPMW_SRV_LOG_GET_LEVEL(void);
void MPMW_SRV_LOG(int app , int loglevel,const char *format,va_list args);
void MPMW_SRV_LOG_INIT( void );

#endif
/** @} */

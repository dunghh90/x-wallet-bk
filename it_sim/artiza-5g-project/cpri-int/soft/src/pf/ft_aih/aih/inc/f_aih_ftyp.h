/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   f_aih_ftyp.h
 *  @brief  function declaration file
 *  @date   2019/08/09 FJ)Takahashi Create
 *
 *  ALL Rights Reserved, Copyright FUJITSU Limited 2019
 */
 /********************************************************************************************************************/

 /*!
 * @addtogroup RRH_PF_AIH
 * @{
 */

#ifndef F_AIH_FTYP
#define F_AIH_FTYP

#include "f_aih_def.h"
#include "f_aih_typ.h"


extern	VOID  f_aih_main( VOID );
extern	VOID  f_aih_init( VOID );
extern	VOID  f_aih_anlz( VOID * ); 
extern	VOID  f_aih_nop( VOID * );
extern	VOID  f_aih_nop1( VOID * );
extern	VOID  f_aih_nop2( VOID * );
extern	VOID  f_aih_nop3( VOID * );
extern	VOID  f_aih_nop4( VOID * );
extern	VOID  f_aih_nop5( VOID * );
extern	VOID  f_aih_nop6( VOID * );
extern	VOID  f_aih_aldRcv( VOID * );
extern	VOID  f_aih_msiAldPwrCtrl( VOID * );
extern	VOID  f_aih_msiAldTransmit( VOID * );
extern	VOID  f_aih_aldRcvTO( VOID * );
extern	VOID  f_aih_aldBusyRcvTO( VOID * );
extern	VOID  f_aih_aldSendEnd( VOID * );
extern	VOID  f_aih_aldMsgLog( UINT ,UINT , VOID * );
extern	VOID  f_aih_abort(VOID* );
extern	VOID  f_aih_aldRcvStart( VOID * );
extern	UINT  f_aih_MSIanlz( VOID * );
extern	VOID  f_aih_msiAldPwrGet( VOID * );
extern	VOID  f_aih_aldPow_notification( UINT );
extern	VOID  f_aih_initCtl(UINT);

#endif

/** @} */

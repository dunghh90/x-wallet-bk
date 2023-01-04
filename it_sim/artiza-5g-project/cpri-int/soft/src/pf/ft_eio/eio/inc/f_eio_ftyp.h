/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eio_ext.h
 *  @brief  5GDU-LLS external IO supervision & cntro function prototype definition
 *  @date   2019/03/19 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/* ìÒèdéÊçûÇ›ñhé~ */
#ifndef		F_EIO_FTYP
#define		F_EIO_FTYP

/** @addtogroup RRH_PF_EIO
* @{ */

/*	f_eio_main.c	*/
extern	VOID f_eio_main( VOID );
extern	VOID f_eio_initProc( VOID );

/*	f_eio_anlz.c	*/
extern	VOID f_eio_anlz( VOID * );
extern	VOID f_eio_MSIanlz( VOID * );
extern	VOID f_eio_nop( VOID * );

/*	f_eio_msiExtInNtfyStartInd.c	*/
extern	VOID f_eio_msiExtInNtfyStartInd(  );

/*	f_eio_msiExtIoGetReq.c	*/
extern	VOID f_eio_msiExtIoGetReq( UINT );

/*	f_eio_msiExtOutEditReq.c	*/
extern	VOID f_eio_msiExtOutEditReq( UINT, VOID * );

/*	f_eio_msiFanGetReq.c	*/
extern	VOID f_eio_msiFanGetReq( UINT );

/*	f_eio_msgExtTimOutInd.c	*/
extern	VOID f_eio_msgExtTimOutInd( VOID * );

/*	f_eio_msgFanTimOutInd.c	*/
extern	VOID f_eio_msgFanTimOutInd( VOID * );
extern	VOID f_eio_fanMountSv( VOID );
extern	VOID f_eio_fanPsSv( VOID );
extern	VOID f_eio_fanCtrl( VOID );

/*	f_eio_apiMsgRecv.c	*/
extern	VOID f_eio_apiFanDbgInd( VOID * );
extern	VOID f_eio_apiFanDacInd( VOID * );

/*	f_eio_subFunc.c	*/
extern	VOID f_eio_fanUnitStsDetect( UINT, const T_EIOR_FANUNIT_SV_INFO * );
extern	VOID f_eio_fanReset(UINT fanIdx );
extern	VOID f_eio_extIoSvCtrlLog( const char* Message_p, ... );
extern  VOID f_eio_PowerMonitor(void);
/* @} */

#endif	/* F_EIO_FTYP */


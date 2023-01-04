/*!
* @skip $Id:$
* @file f_fmng_ftyp.h
* @brief fmng task's function declaration file
* @date 2015/08/11 TDIPS)sasaki update.
* @date 2015/08/19 TDIPS)sasaki Add f_fmng_re_flashWriteTO function prototype 
* @date 2015/08/19 TDIPS)sasaki 関数名変更:f_fmng_flashSaveReFwInfoRsp -> f_fmng_flashSaveReFwInfoCmp
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
*/
/** @addtogroup RRH_PF_FMNG
 *  @{
 */
#ifndef F_FMNG_FTYP_H
#define F_FMNG_FTYP_H

extern VOID f_fmng_main( VOID );

extern VOID f_fmng_init();

extern VOID f_fmng_anlz( VOID * );

extern VOID f_fmng_anlz_frwre( VOID * );

extern VOID f_fmng_nop(VOID *);

extern VOID f_fmng_apiLoadEraseReq(VOID *);

extern VOID f_fmng_apiLoadUpdReq(VOID *);

extern VOID f_fmng_apiGetFirmVersion(VOID *);

extern VOID f_fmng_apiCancelUpdNtc(VOID *);

extern VOID f_fmng_flashEraseRsp(VOID *);

extern VOID f_fmng_flashWrtReq(VOID *);

extern VOID f_fmng_flashWrtRsp(VOID *);

extern VOID f_fmng_flashAccessTimout(VOID *);

extern VOID f_fmng_flashEraseTimout(VOID *);

extern VOID f_fmng_flashWriteTimout(VOID *);

extern VOID f_fmng_com_sndLoadUpdRsp(UINT,CHAR*);

extern VOID f_fmng_com_sndLoadEraseRsp(UINT,CHAR*);

extern VOID f_fmng_cm_WriteEeprom( UINT, USHORT );

extern VOID f_fmng_cm_ChgBootFace( USHORT );

extern VOID f_fmng_cm_ChgBootFace_cprifpga( USHORT );

extern INT f_fmng_checkDLInfo(UINT,UINT,UINT*);

extern VOID f_fmng_rejApiLoadUpdReq(void *);

extern VOID f_fmng_rejApiLoadEraseReq(void *);

extern UINT f_fmng_getEraseFlgByAddr(UINT);

extern VOID f_fmng_com_startTimer(UINT, INT, UINT);

extern VOID f_fmng_com_stopTimer(UINT);

extern VOID f_fmng_cm_RunHisSet(UINT, UCHAR*);

extern VOID f_fmng_logPrint(FILE *outf);

extern VOID f_fmng_tarThread();

extern VOID f_fmng_sendTarResult(int);

extern VOID f_fmng_tarProcessRsp(void *);

extern VOID f_fmng_com_sndSubGetVerRsp();

extern VOID f_fmng_flashUpdReFwInfo( VOID* );

extern VOID f_fmng_flashDelReFwInfo( VOID* );

extern VOID f_fmng_flashSaveReFwInfoCmp( VOID* );

/********************************************************************************/
/* Function prototype declaretion for RE-FLASH Control							*/
/********************************************************************************/
extern VOID f_fmng_re_nop( VOID* );

extern VOID f_fmng_re_LoadFwDataReq( VOID* );

extern VOID f_fmng_re_SaveFwDataReq( VOID* );

extern VOID f_fmng_re_CancelFwAccessReq( VOID* );

extern VOID f_fmng_re_CancelSaveFwInfo( VOID* );

extern VOID f_fmng_re_rejLoadFwDataReq( VOID* );

extern VOID f_fmng_re_rejCancelFwAccessReq( VOID* );

extern VOID f_fmng_re_DiscardSavedWriteReq( VOID* );

extern VOID f_fmng_re_ExecSavedWriteReq( VOID* );

extern VOID f_fmng_re_ExecSavedEraseReq( VOID* );

extern VOID f_fmng_re_flashReadRsp( VOID* );

extern VOID f_fmng_re_flashReadCancelRsp( VOID* );

extern VOID f_fmng_re_flashWriteRsp( VOID* );

extern VOID f_fmng_re_flashEraseRsp( VOID* );

extern VOID f_fmng_re_flashSaveReFwInfoRsp( VOID* );

extern VOID f_fmng_re_SaveFwInfoTO( VOID* );

extern VOID f_fmng_re_flashAccessTO( VOID* );

extern VOID f_fmng_re_flashWriteTO( VOID* );

#endif
    
/** @} */

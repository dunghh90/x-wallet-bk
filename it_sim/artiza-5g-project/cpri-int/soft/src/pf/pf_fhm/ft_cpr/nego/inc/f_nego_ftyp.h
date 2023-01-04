/*!
* @skip $Id:$
* @file f_nego_ftyp.h
* @brief nego thread function declaration file
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-
*/
/** @addtogroup RRH_PF_NEGO
 *  @{
 */
#ifndef F_NEGO_FTYP_H
#define F_NEGO_FTYP_H

extern VOID f_nego_main();
extern VOID f_nego_Init();

extern VOID f_nego_anlz(UINT * );
extern VOID f_nego_nop(UINT * );

extern VOID f_nego_AutoNegoStartInd(UINT * );
extern VOID f_nego_AutoNegoStopInd(UINT * );
extern VOID f_nego_AutoNegoTimeOutNtc(UINT *);
extern VOID f_nego_AutoNegoReStartInd(UINT * );

#ifndef OPT_RRH_ZYNQ_REC
extern VOID f_nego_HardSetLyncUp_Start();
#else
extern VOID f_nego_HardSetLyncUp_Start(UINT);
#endif
extern VOID f_nego_HardSetHfnSync_Start();
extern VOID f_nego_HardSetLyncUp_ReStart();
extern VOID f_nego_HardSetHfnSync_ReStart();

extern VOID f_nego_cm_runHisSet(UINT , UCHAR* );
extern VOID f_nego_logPrint(FILE *);
extern VOID f_nego_dbg_bitrate_result(UINT *);

#endif
    
/** @} */

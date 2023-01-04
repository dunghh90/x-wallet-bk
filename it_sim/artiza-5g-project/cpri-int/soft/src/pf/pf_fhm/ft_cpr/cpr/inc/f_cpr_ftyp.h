/*!
* @skip $Id:$
* @file f_cpr_ftyp.h
* @brief ext task's function declaration file
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
*/
/** @addtogroup RRH_PF_CPR
 *  @{
 */
#ifndef F_CPR_FTYP_H
#define F_CPR_FTYP_H

extern VOID f_cpr_main();
extern VOID f_cpr_Init();

extern VOID f_cpr_anlz(UINT * );
extern VOID f_cpr_nop(UINT * );
extern VOID f_cpr_CpriLinkInitProc(UINT * );
extern VOID f_cpr_CpriLinkInitProcRE(UINT * );
extern VOID f_cpr_CpriLinkupTimeOutNtc(UINT * );
extern VOID f_cpr_CpriLinkEnableProc(UINT * );
extern VOID f_cpr_CpriLinkDisableProc(UINT *);
extern VOID f_cpr_CpriParamUpdProc(UINT * );
extern VOID f_cpr_CpriStateChgProc(UINT * );
extern VOID f_cpr_CpriStateChgProcRE(UINT * );
extern VOID f_cpr_CpriLinkStartupProcRE(USHORT linkno);
extern VOID f_cpr_CpriStateSubProc(UINT * );
extern VOID f_cpr_CpriStateUnsubProc(UINT * );
extern VOID f_cpr_CpriAutoNegoCompleteNtc(UINT * );
extern VOID f_cpr_CpriFiberOutput(UINT * );
extern VOID f_cpr_CpriStatistMain();
extern VOID f_cpr_com_CpriStateNtc(USHORT, UINT );
extern VOID f_cpr_cm_runHisSet(UINT , UCHAR* ); 
extern VOID f_cpr_logPrint(FILE *);
extern VOID f_cpr_dbg_link_setup(UINT *);
extern VOID f_cpr_CpriDelayOffset();
extern VOID f_cpr_CpriLinkStopProc(UINT *);
extern VOID f_cpr_DelayAdjustStep3Proc(UINT *);
extern VOID f_cpr_DelayAdjustStep7Proc(UINT *);
extern VOID f_cprLinkUpTimerCtrl(USHORT, USHORT);
extern VOID f_cpr_DelayAdjustStep10Proc(UINT *);
extern VOID f_cpr_DelayAdjust_CPMnDisable(USHORT);
extern VOID f_cpr_DelayAdjust_ClearStateProc(UINT *);
extern VOID f_cpr_DelayAdjust_ClearState(VOID);
extern VOID f_cpr_sfp_log(USHORT cpr_num, E_RRH_SFPLOG_TRIGGER trigger);
extern VOID f_cpr_cpmPwrNoProc(UINT * );

#endif
    
/** @} */

/*!
 * @skip  $ld:$
 * @file  f_frw_ram.h
 * @brief RAM table declaration.
 * @date 2013/11/13 FFCS)LINB Create\n
 * @date 2015/08/13 TDIPS)sasaki delete flashReadRsp(unused variable)
 * @date 2015/08/13 TDIPS)sasaki Add f_frww_fwInfo
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
    
/*!
 * @addtogroup RRH_PF_FRW
 * @{
 */

/*!
 * @brief	pf_frw thread state
 * @note	pf_frw thread state
 * 
 */
extern UINT             f_frww_thrState;
extern UINT             f_frww_respQid;
extern UCHAR            f_frww_compressData[D_FRW_COMPDATA];
extern T_RRH_FW_INFO    f_frww_fwInfo[D_RRH_CPRINO_RE_MAX];
extern T_FRW_RUNHIS_TBL f_frww_runhis_tbl;
extern VOID             *f_frww_assert_p;

/** @} */

/*!
* @skip $Id:$
* @file f_fmng_ram.h
* @brief FMNG RAM table extern declaration
* @date 2013/04/26 FFCS)Xut create.
* @date 2015/08/11 TDIPS)sasaki update RE-FLASH制御用変数の宣言追加
* @date 2015/08/20 TDIPS)sasaki REファイル用API種別保存変数を追加
* @date 2015/08/21 TDIPS)sasaki RE-FLASH WRITE/ERASE結果保存変数を追加
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
*/

/** @addtogroup RRH_PF_FMNG
 *  @{
 */

#ifndef F_FMNG_RAM_H
#define F_FMNG_RAM_H

extern UINT                             f_fmngw_thrdState;
extern UINT                             f_fmngw_preThrdState;
extern UINT                             f_fmngw_re_thrdState;
extern UINT                             f_fmngw_re_preThrdState;
extern UINT                             f_fmngw_extEventNo;
extern UINT                             f_fmngw_respQid;
extern UINT                             f_fmngw_file_ver[15];
extern USHORT                           f_fmngw_stupver_tbl;
extern USHORT                           f_fmngw_usever_tbl;
extern USHORT                           f_fmngw_useyear_tbl;
extern USHORT                           f_fmngw_usemonthday_tbl;
extern USHORT                           f_fmngw_usebootface;
extern USHORT                           f_fmngw_usebootface_cprifpga;
extern USHORT                           f_fmngw_apikind;
extern USHORT                           f_fmngw_re_apikind;
extern UINT                             f_fmngw_dlwType;
extern T_SYS_LMCWRT_REQ                 f_fmngw_flashWaitWrtReq;
extern T_SYS_LMCWRT_REQ                 f_fmngw_flashWaitWrtReq_slave;
extern T_SYS_LMCWRT_REQ                 f_fmngw_flashWaitWrtReq_re;
extern T_SYS_FLSERASE_REQ               f_fmngw_flashWaitEraReq_re;
extern UINT                             f_fmngw_verRespQid;
extern T_FMNG_RUNHIS_TBL                f_fmngw_runhis_tbl;
extern VOID*                            f_fmngw_assert_p;
extern UINT                             f_fmngw_flashRsp[D_SYS_NUM2];
extern UINT                             f_fmngw_hard_firm_decode_size;
extern UINT                             f_fmngw_hard_pl_decode_size;
extern UINT                             f_fmngw_hard_rfic_decode_size;
extern UINT                             f_fmngw_hard_cpri_decode_size;
extern UINT                             f_fmngw_read_cancel_re;
extern USHORT                           f_fmngw_fw_type;
extern USHORT                           f_fmngw_rsp_kind;
extern UINT                             f_fmngw_re_fileUpdResult;
extern T_API_FILE_SAVE_FW_REQ_DATA      f_fmngw_save_fw_reqdata;

#endif
/** @} */


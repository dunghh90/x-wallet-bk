/*!
 * @skip  $ld:$
 * @file  f_fmng_ram.c
 * @brief 
 * @date 2013/11/14 FFCS)linb create
 * @date 2015/06/16 ALP)murakami TDD-ZYNQ対応
 * @date 2015/08/10 TDIPS)sasaki f_fmngw_hard_rfic_decode_size 削除
 * @date 2015/08/11 TDIPS)sasaki RE-FLASH制御用変数追加
 * @date 2015/08/20 TDIPS)sasaki REファイル用API種別保存変数を追加
 * @date 2015/08/21 TDIPS)sasaki RE-FLASH WRITE/ERASE結果保存変数を追加
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

#include "f_fmng_inc.h"

/*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

/**
 * @brief fmng task state
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_thrdState;

/**
 * @brief fmng task previous state
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_preThrdState;

/**
 * @brief fmng task state for f_fmngr_re_matrix
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_re_thrdState;

/**
 * @brief fmng task previous state for f_fmngr_re_matrix
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_re_preThrdState;

/**
 * @brief fmng task external event number
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_extEventNo;

/**
 * @brief response queue id
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_respQid;

/**
 * @brief file version
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_file_ver[15];

/**
 * @brief startup version table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_stupver_tbl;

/**
 * @brief use version table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_usever_tbl;

/**
 * @brief use year table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_useyear_tbl;

/**
 * @brief use month and day table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_usemonthday_tbl;

/**
 * @brief use boot face
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_usebootface;

/**
 * @brief use boot face (CPRI FPGA)
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_usebootface_cprifpga;

/**
 * @brief received API kind
 * @note  [ 値の説明 ] erase=1, update=2, none=0
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_apikind;

/**
 * @brief received API kind for RE file
 * @note  [ 値の説明 ] erase=1, update=2, read=3, none=0
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_re_apikind;

/**
 * @brief download type
 * @note  [ 値の説明 ] startup=0, use=1
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_dlwType;

/**
 * @brief save the writing request in FLASH R/W
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_SYS_LMCWRT_REQ f_fmngw_flashWaitWrtReq;

/**
 * @brief save the writing request in FLASH R/W for slave
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_SYS_LMCWRT_REQ f_fmngw_flashWaitWrtReq_slave;

/**
 * @brief save the writing request in FLASH R/W for RE-FLASH
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_SYS_LMCWRT_REQ f_fmngw_flashWaitWrtReq_re;

/**
 * @brief save the erase request in FLASH R/W for RE-FLASH
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_SYS_FLSERASE_REQ f_fmngw_flashWaitEraReq_re;

/**
 * @brief version response queue id
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_verRespQid;

/**
 * @brief スレッド内アサートログ用
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
VOID *f_fmngw_assert_p;

/**
 * @brief FLASH response
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_flashRsp[D_SYS_NUM2];

/**
 * @brief firmware decode size
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_hard_firm_decode_size;

/**
 * @brief PL-FPGA decode size
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_hard_pl_decode_size;

/**
 * @brief CPRI-FPGA decode size
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_hard_cpri_decode_size;

/**
 * @brief cancel of reading RE data
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_read_cancel_re;

/**
 * @brief running history
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_FMNG_RUNHIS_TBL f_fmngw_runhis_tbl;

/**
 * @brief firmware file type
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_fw_type;

/**
 * @brief response kind
 * @note  [ 値の説明 ] 0=none, 1=cancel, 2=save
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_fmngw_rsp_kind;

/**
 * @brief result of RE file write/erase
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_fmngw_re_fileUpdResult;

/**
 * @brief processing FW save request
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_API_FILE_SAVE_FW_REQ_DATA f_fmngw_save_fw_reqdata;

/** @} */

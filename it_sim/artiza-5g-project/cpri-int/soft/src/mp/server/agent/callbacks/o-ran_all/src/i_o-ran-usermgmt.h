/*
 * @file    i_o-ran-usermgmt.h
 */

#ifndef _H_i_o_ran_user_mgmt
#define _H_i_o_ran_user_mgmt


/*!
 * @addtogroup  SAA_MOD_O_RAN_USERMGMT
 * @brief       SAA o-ran-usermgmt
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

#include "f_mpsw_msg_o-ran_usermgmt_def.h"
#include "f_mpsw_msg_o-ran_usermgmt_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define SAA_USERMGMT_MAX_USER_NUM           5            /* 最大登録可能ユーザ数 */
#define SAA_USERMGMT_O_RAN_GROUP_NMS         1            /* 0x01 o-ran-group nms  */
#define SAA_USERMGMT_O_RAN_GROUP_FM_PM       2            /* 0x02 o-ran-group fm-pm */
#define SAA_USERMGMT_O_RAN_GROUP_SWM         4            /* 0x04 o-ran-group swm */
#define SAA_USERMGMT_O_RAN_GROUP_NON         0            /* 0x00 o-ran-group nothing */

#define SAA_USERMGMT_USER_PASSWPORD_FLG         1        /* 0x01 password変更フラグ 変更あればflgとビットノア演算 commit正常ならビットオア演算 */
#define SAA_USERMGMT_USER_GROUP_FLG             2        /* 0x02 group変更フラグ */
#define SAA_USERMGMT_USER_ENABLED_FLG           4        /* 0x04 enabled変更フラグ  */

#define SAA_USERMGMT_USER_CHANGE_OK_FLG         7        /* 0x07 FLG=OK値 */
#define SAA_USERMGMT_USER_CHANGE_NO_FLG         0        /* 0x00 FLG=NG値 */

#define SAA_USERMGMT_ENABLED_FALSE              0           /* 0x0000 enabled FALSE */
#define SAA_USERMGMT_ENABLED_TRUE               1           /* 0x0001 enabled TRUE  */
#define SAA_USERMGMT_ENABLED_NON                0xFFFFFFFF  /* enabled 設定なし     */

#define i_mod_o_ran_usermgmt                               "o-ran-usermgmt"
#define i_para_o_ran_usermgmt_num_of_user                  "num_of_user"

#define SAA_USERMGMT_MAX_CHAR_128               128             /* 最大文字数 */
#define SAA_USERMGMT_MAX_CHAR_256               256             /* 最大文字数 */
#define SAA_USERMGMT_MAX_CHAR_512               512             /* 最大文字数 */
#define SAA_USERMGMT_MAX_CHAR_2048              2048            /* 最大文字数 */

#define D_O_RAN_USERMGMT_DB_SEND_MAX_LEN         4096            /* dbapi送信時の最大文字列長 */

#define SAA_ORG_USER_MGMT_STATUS_SUCCESS                                    "Successful"
#define SAA_ORG_USER_MGMT_STATUS_FLT                                        "Failed"
#define SAA_ORG_USER_MGMT_STATUS_MSG_SUCCESS                                "Current user's password is changed successfully"
#define SAA_ORG_USER_MGMT_STATUS_MSG_FLT_CURPASS_MISMATCH                   "Current password mismatch"
#define SAA_ORG_USER_MGMT_STATUS_MSG_FLT_NEWPASS_MISMATCH                   "New password mismatch"
#define SAA_ORG_USER_MGMT_STATUS_MSG_FLT_OTHER                              "Operation failed to change password"
#define SAA_ORG_USER_MGMT_STATUS_MSG_FLT_USER_NOTFOUND                      "User Not Found"

/********************************************************************************************************************/
/*  typedef                                                                                                        */
/********************************************************************************************************************/


/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/
extern T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ new_user_data[SAA_USERMGMT_MAX_USER_NUM];                     /* 更新用管理テーブル */
extern T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ current_user_data[SAA_USERMGMT_MAX_USER_NUM];                 /* 既存用管理テーブル */
extern UINT table_commit_flg[SAA_USERMGMT_MAX_USER_NUM];                                                /* 既存用管理テーブル更新フラグ(MSI_OK返答毎bit立て) */
extern UINT max_num_set_user_data;                                                                      /* 登録済ユーザ数 */

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/* init */
extern status_t i_o_ran_usermgmt_init(void);
extern status_t i_o_ran_usermgmt_init2(void);
extern status_t i_o_ran_usermgmt_get_init_para(void);

/* validate */
extern status_t i_o_ran_usermgmt_o_ran_user_profile_user_validate(const xmlChar *k_o_ran_user_profile_user_name, const xmlChar *newval_val, bool newval_bool, const UINT flg);
extern int i_o_ran_usermgmt_check_user_num(const xmlChar *k_o_ran_user_profile_user_name, UINT *user_num);
extern int i_o_ran_usermgmt_set_user_name_to_new_user_data(const xmlChar *k_o_ran_user_profile_user_name, UINT *user_num);

/* commit */
extern status_t i_o_ran_usermgmt_o_ran_user_profile_user_commit(const xmlChar *k_o_ran_user_profile_user_name, const UINT flg);
extern int i_o_ran_usermgmt_set_msi_data(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ *msg_data, UINT user_num, UINT flg, int newcur);

/* msi */
extern status_t i_o_ran_usermgmt_send_edit_msi(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ *msg_data, UINT flg, UINT user_num);

/* DB_API */
extern void i_o_ran_usermgmt_send_edit_config(UINT user_max);
extern void i_o_ran_usermgmt_send_edit_config_chg_password(UINT user_num);

/* rpc */
extern status_t i_o_ran_usermgmt_chg_password_invoke(const xmlChar *curval, const xmlChar *newval, const xmlChar *newconval, xmlChar *status_buff, xmlChar *status_message_buff);

#endif

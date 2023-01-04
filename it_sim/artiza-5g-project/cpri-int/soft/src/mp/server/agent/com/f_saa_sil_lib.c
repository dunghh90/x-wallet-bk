/*!
 *  @skip	$Id:$
 *  @file	f_saa_sil_lib.c
 *  @brief	SAA SIL-SA help functions.
 *  @author	KCN) Nakai Hiroyuki
 *  @date	2019/04/05 KCN)Nakai create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup	SAA
 * @brief		NETCONF sub-agent.
 * @note		Works with netconfd-pro
 * @{
 */


#include <xmlstring.h>
#include "obj.h"
#include "rpc.h"
#include "val.h"
#include "val_child.h"

#include "f_saa_sil_lib.h"

#define PASSWORD (const xmlChar *)"password"

/*!
 * @brief	RPCメッセージからpasswordの値を取得する。
 * @note	標準的なAPIアクセスではなくメッセージ形式決め打ちで抜き出している。

 * @param	msg RPCメッセージ
 * @return	msg中のpassword文字列へのポインタ
 * @date	2019/4/5 KCN)Nakai create
 * @warning	xran-softwaremanagement.yang, xran-file-namagement.yangでのみ検証済み
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
xmlChar *f_saa_com_sil_get_password(rpc_msg_t *msg){

    val_value_t *v_child_val = NULL;
    val_value_t *v_password_val = NULL;
    xmlChar *v_password = NULL;

    v_child_val = val_get_first_child(msg->rpc_input);

    while(v_child_val){
        if(LOGDEV1){
            log_dev1("\n%s:v_child_val->name=%s", __func__, v_child_val->name);
        }
        if(xml_strcmp(v_child_val->name, PASSWORD) == 0){
            v_password_val =val_get_first_child(v_child_val);
            if(v_password_val){
                v_password = VAL_STRING(v_password_val);
                if(LOGDEV1){
                    log_dev1("\n%s:v_password_val =%p, %s", __func__, v_password_val, v_password);
                }
            }
            else{
                if(LOGWARN){
                    log_warn("\n%s:v_password_val is NULL.", __func__);
                }
            }
            break;
        }
        v_child_val = val_get_next_child(v_child_val);
    } 

    if(v_password == NULL){
        if(LOGWARN){
            log_warn("\n%s:v_password is NULL", __func__);
        }
    }
    log_flush();

    return v_password;
}


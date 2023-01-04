/*!
 *  @skip       $Id:$
 *  @file       i_o-ran-file-management.c
 *  @brief      SAA o-ran-file-management
 *  @author     KCN) Nakai Hiroyuki
 *  @date       2019/03/11 KCN)Nakai create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup  SAA_MOD_O_RAN_FILE_MANAGEMENT
 * @brief       SAA o-ran-file-management
 * @note        charとxmlChar, boolとbooleanなど型表記が混在しています。
 * @note        共通関数や内部関数とのインタフェースには前者、 u_o-ran-file-managementとのインタフェースには後者の型を使ってます。
 * @{
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>

#include "agt.h"
#include "agt_util.h"
#include "rpc.h"

#include "f_mpsw_msg_MSI.h"
#include "f_mpsw_msg_enum.h"
#include "f_mpsw_msg_o-ran_file_management_def.h"
#include "f_mpsw_msg_o-ran_file_management_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_msi_lib.h"
#include "f_saa_ntf_lib.h"
#include "u_o-ran-file-management.h"
#include "i_o-ran-file-management.h"

/* for debug without MSI, uncomment the line below. */
//#define DEBUG_NO_MSI /*for debug */

#define D_FLM_MAX_FILENAME_LEN 256
#define D_FLM_MAX_PASSWORD_LEN 256
#define D_FLM_MAX_STATUS_LEN 32

char *m_file_list[D_FLM_MAX_FILE_NUM];

/* notification handlers */
void i_o_ran_file_management_upload_event_handler(void *data, uint32_t size);

/* internal functions */
static int upload(const char *local_logical_file_path, const char *remote_file_path, const char *password, char *message);
static int retrieve(const char *logical_path, const char *file_name_filter, char *file_name, char *message);
static int read_file_list(char *file_name, int *err);

/* =============================================================

   Function Definitions

   ============================================================= */

/*!
 * @brief   ソフトウェア管理の初期化を行う。
 * @note    notificationハンドラを登録する。
 * @param   なし
 * @return  なし
 * @date    2019/3/1 KCN)中井
 */
void i_o_ran_file_management_init2(void){
    if (LOGINFO) {
        log_info("\nEnter %s", __func__);
    }

    f_saa_com_reg_notification_handler(
        D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND,
        i_o_ran_file_management_upload_event_handler
        );

    memset(m_file_list, 0, sizeof(m_file_list));
}

/*!
 * @brief   ファイルアップロードのパラメータ検査を行う。
 * @note    
 * @param   local_logical_file_path ローカルパス
 * @param   remote_file_path リモートパス
 * @param   password パスワード
 * @return  0:成功, -1:失敗
 * @date    2019/3/12 KCN)中井
 */
int i_o_ran_file_management_file_upload_validate (
    const xmlChar *local_logical_file_path, const xmlChar *remote_file_path, const xmlChar *password){

    int rc = 0;
    const char *p = NULL;
    const char *start_with = "sftp://";

    /* strlen()を2回ずつ実行するのは無駄ですが、
       1回にして変数に入れても煩雑になるので。 */
    p = (const char *)local_logical_file_path;
    if(p == NULL || strlen(p) == 0
        || strlen(p) > (D_FLM_MAX_FILENAME_LEN - 1)){
        rc = -1;
    }
    else{
        p = (const char *)remote_file_path;
        if(p == NULL || strlen(p) == 0
            || strlen(p) > (D_FLM_MAX_FILENAME_LEN - 1)){
            rc = -1;
        }
        else if(strncmp(p, start_with, strlen(start_with)) != 0){
            rc = -1; 
        }
        else{
            p = (const char *)password;
            if(p == NULL || strlen(p) == 0
                || strlen(p) > (D_FLM_MAX_PASSWORD_LEN - 1)){
                rc = -1;
            }
        }
    }

    if(LOGDEV1){
        log_dev1("\n%s: local_logical_file_path=%s, remote_file_path=%s", __func__, local_logical_file_path, remote_file_path);
    }

    return rc;
}
    
/*!
 * @brief   ファイル一覧取得のパラメータ検査を行う。
 * @note    
 * @param   logical_path 検索対象の論理パス
 * @param   file_name_filter ファイル名のフィルタ
 * @param   password パスワード
 * @return  0:成功, -1:失敗
 * @date    2019/3/12 KCN)中井
 */
int i_o_ran_file_management_retrieve_file_list_validate(const xmlChar *logical_path, const xmlChar *file_name_filter){

    int rc = 0;
    const char *p = NULL;

    /* strlen()を2回ずつ実行するのは無駄ですが、
       1回にして変数に入れても煩雑になるので。 */
    p = (const char *)logical_path;
    if(p == NULL || strlen(p) == 0
        || strlen(p) > (D_FLM_MAX_FILENAME_LEN - 1)){
        rc = -1;
    }
    else{
        p = (const char *)file_name_filter;
        if(p == NULL || strlen(p) == 0
            || strlen(p) > (D_FLM_MAX_FILENAME_LEN - 1)){
            rc = -1;
        }
    }

    if(LOGDEV1){
        log_dev1("\n%s: logical_path=%s, file_name_filter=%s", __func__, logical_path, file_name_filter);
    }

    return rc;
}

/*!
 * @brief   ファイルアップロードを行う。
 * @note    処理の本体は内部関数 upload()で実行する。
 * @param   local_logical_file_path ローカルパス
 * @param   remote_file_path リモートパス
 * @param   password パスワード
 * @param   message  エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/11 KCN)中井
 */
int i_o_ran_file_management_file_upload_invoke (
    const xmlChar *local_logical_file_path, const xmlChar *remote_file_path, const xmlChar *password, xmlChar *message){

    int rc = -1;

    rc = upload((const char *)local_logical_file_path, (const char *)remote_file_path, (const char *)password, (char *)message);

    return rc;
}

/*!
 * @brief   ファイル名の取得を行う。
 * @note    処理の本体は内部関数 retreive()で実行する。
 * @param   logical_path 検索対象の論理パス
 * @param   file_name_filter ファイル名のフィルタ
 * @param   message  エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/11 KCN)中井
 */
int i_o_ran_file_management_retrieve_file_list_invoke(const xmlChar *logical_path, const xmlChar *file_name_filter, xmlChar ***file_list, xmlChar *message){

    int rc = -1;
    char file_name[D_FLM_MAX_FILENAME_LEN];
    int read_err = 0;

    if(LOGDEV1){
        log_dev1("\n%s logical_path=%s, file_name_filter=%s, file_list=%p: ", __func__, logical_path, file_name_filter, file_list);
    }

    *file_list = NULL;
    rc = retrieve((const char *)logical_path, (const char *)file_name_filter,
             file_name, (char *)message);
    if(rc == 0){
        rc = read_file_list(file_name, &read_err);
        if(rc == 0){
            *file_list = (xmlChar **)m_file_list;
        }
        else{
            sprintf((char *)message, "internal error. (%d)", read_err);
        }
    }

    if(LOGDEV1){
        log_dev1("\n%s rc=%d: ", __func__, rc);
    }

    return rc;
}

/* ====== Notification Handlers ====== */

/*!
 * @brief   アップロード結果のnotificationを送信する。
 * @note    
 * @param   data 結果データのアドレス
 * @param   size 結果データのサイズ
 * @return  なし
 * @date    2019/3/1 KCN)中井
 */
void i_o_ran_file_management_upload_event_handler(void *data, uint32_t size){
    T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND *info = NULL;
    char *local_logical_file_path = NULL;
    char *remote_file_path = NULL;
    char event_status[D_FLM_MAX_STATUS_LEN];
    char message[D_FLM_MAX_MESSAGE_LEN];

    (void)size;

    memset(message, 0, sizeof(message));

    info = (T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_NTF_IND *)data;
    local_logical_file_path = info->local_logical_file_path;
    remote_file_path = info->remote_file_path;
    snprintf(message, D_FLM_MAX_MESSAGE_LEN, "%d: %s",
        info->msgErr.errorCode, info->msgErr.errorDescription);

    switch(info->status){
        case E_FLM_STATUS_SUCCESS:
            strcpy(event_status,"SUCCESS");
            break;
        case E_FLM_STATUS_FAILURE:
        /* no break */
        default:
            strcpy(event_status,"FAILURE");
            break;
    }

    if(LOGINFO){
        log_info("\n%s: local_logical_file_path=%s, remote_file_path=%s, event_status=%s", __func__, local_logical_file_path, remote_file_path, event_status);
    }

    u_o_ran_file_management_file_upload_notification_send ((xmlChar *)local_logical_file_path, (xmlChar *)remote_file_path, (xmlChar *)event_status, (xmlChar *)message);
}

/* ====== Internal Functions ====== */

/*!
 * @brief   ファイルダウンロードを行う。
 * @note    
 * @param   local_logical_file_path ローカルパス
 * @param   remote_file_path リモートパス
 * @param   password パスワード
 * @param   message  エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/11 KCN)中井
 */
static int upload(const char *local_logical_file_path, const char *remote_file_path, const char *password, char *message){
    int rc = 0;
    int msi_rc = 0;
    uint32_t read_size = 0;

    /* validateを経てから呼ばれるので、ここでの各パラメータの検査は省略する。 */

    if(LOGINFO){
        log_info("\n%s: local_logical_file_path=%s, remote_file_path=%s", __func__, local_logical_file_path, remote_file_path);
    }

#ifndef DEBUG_NO_MSI
    /* CFM < REJ */
    T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ request_buffer;
    T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ response_buffer;

    memset(&request_buffer, 0, sizeof(request_buffer));
    memset(&response_buffer, 0, sizeof(response_buffer));

    strcpy(request_buffer.local_logical_file_path, local_logical_file_path);
    strcpy(request_buffer.remote_file_path, remote_file_path);
    strcpy(request_buffer.password, password);

    msi_rc = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ,
        sizeof(request_buffer),
        &request_buffer,
        D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_CFM,
        D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ,
        sizeof(T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REJ),
        &response_buffer, &read_size);

    if(msi_rc == D_MSI_CFM){
        if(LOGDEV1){
            log_dev1("\n%s: MSI responded: msi_rc=%d, read_size=%d.", __func__, msi_rc, read_size);
        }
        rc = 0;
    }
    else {
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: msi_rc=%d.", __func__, msi_rc);
        }
        rc = -1;

        if(msi_rc == D_MSI_REJ){
            snprintf(message, D_FLM_MAX_MESSAGE_LEN ,"%d: %s",
                response_buffer.msgErr.errorCode, response_buffer.msgErr.errorDescription);
        }
        else{
            sprintf(message,"internal error. (%d)", msi_rc);
        }
    }
#else
    /* デバッグ用にビルドされたときのみ */
    if(LOGDEV0){
        log_dev0("\n%s: debug OK, password=%s", __func__, password);
    }
    rc = 0;
    (void)message;
    (void)msi_rc;
#endif

    return rc;
}

/*!
 * @brief   ファイル名の取得を行う。
 * @note    処理の本体は内部関数 retreive()で実行する。
 * @param   logical_path 検索対象の論理パス
 * @param   file_name_filter ファイル名のフィルタ
 * @param   message  エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/11 KCN)中井
 */
static int retrieve(const char *logical_path, const char *file_name_filter, char *file_name, char *message){
    int rc = 0;
    int msi_rc = 0;
    uint32_t read_size = 0;

    /* validateを経てから呼ばれるので、ここでの各パラメータの検査は省略する。 */

    if(LOGINFO){
        log_info("\n%s: logical_path=%s, file_name_filter=%s", __func__, logical_path, file_name_filter);
    }

#ifndef DEBUG_NO_MSI
    /* CFM > REJ */
    T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ request_buffer;
    char response_buffer[max(sizeof(T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM), sizeof(T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ))];
    T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM *cfm = NULL;
    T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ *rej = NULL;

    memset(&request_buffer, 0, sizeof(request_buffer));
    memset(&response_buffer, 0, sizeof(response_buffer));
    cfm = (T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM *)&response_buffer;
    rej = (T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ *)&response_buffer;

    strcpy(request_buffer.logical_path, logical_path);
    strcpy(request_buffer.file_name_filter, file_name_filter);

    msi_rc = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ,
        sizeof(request_buffer),
        &request_buffer,
        D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM,
        D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ,
        max(sizeof(T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_CFM), sizeof(T_MPSW_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REJ)),
        &response_buffer, &read_size);

    if(msi_rc == D_MSI_CFM){
        if(LOGDEV1){
            log_dev1("\n%s: MSI responded: msi_rc=%d, read_size=%d.", __func__, msi_rc, read_size);
        }
        strcpy(file_name, cfm->file_name);
        rc = 0;
    }
    else {
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: msi_rc=%d.", __func__, msi_rc);
        }
        rc = -1;
        if(msi_rc == D_MSI_REJ){
            snprintf(message, D_FLM_MAX_MESSAGE_LEN, "%d: %s",
                rej->msgErr.errorCode, rej->msgErr.errorDescription);
        }
        else{
            sprintf(message,"internal error. (%d)", msi_rc);
        }
    }
#else
    /* デバッグ用にビルドされたときのみ */
    strcpy(file_name,"/tmp/sample_list");
    if(LOGDEV1){
        log_dev1("\n%s: debug OK, file_name=%s", __func__, file_name);
    }
    rc = 0;
    (void)message;
    (void)msi_rc;
#endif

    return rc;
}

/*!
 * @brief   ファイル名のリストをファイルから読み込む。
 * @param   file_name 読み込み対象のファイル名
 * @return  0:成功, -1:失敗
 * @date    2019/3/11 KCN)中井
 */
static int read_file_list(char *file_name, int *err){
    int rc = 0;
    int i = 0;
    char *p = NULL;
    FILE *fp = NULL;
    char buffer[D_FLM_MAX_FILENAME_LEN + 1];
    size_t length = 0;
    int counter = 0;

    memset(buffer, 0, sizeof(buffer));

    for(i=0; i<D_FLM_MAX_FILE_NUM; i++){
        if(m_file_list[i] != NULL){
            free(m_file_list[i]);
            m_file_list[i] = NULL;
        }
    }

    fp = fopen(file_name, "r");
    if(fp == NULL){
        *err = errno;
        if(LOGDEV0){
            log_dev0("\n%s: cannot open [%s].(%d)", __func__, file_name, errno);
        }
        return -1;
    }

    while(fgets(buffer, sizeof(buffer), fp) != NULL){
        if(LOGDEV1){
            log_dev1("\n%s: buffer=%s", __func__, buffer);
        }

        length = strlen(buffer);
        if(length > 0 && buffer[length - 1] == '\n'){
            buffer[length - 1] = 0;
            length = length - 1;
        }

        if(length == 0){
            if(LOGDEV1){
               log_dev1("\n%s: skip empty line, counter=%d.", __func__, counter);
            }
            continue;
        }

        p = malloc(length + 1);
        if(p == NULL){
            if(LOGDEV0){
                log_dev0("\n%s: cannot allocate memory. (%d)", __func__, errno);
            }
            rc = -1;
            break;
        }

        strncpy(p, buffer, length);
        p[length] = 0;
        if(LOGDEV1){
           log_dev1("\n%s: counter=%d, file_name=%s", __func__, counter, p);
        }
        
        m_file_list[counter] = p;
        counter++;

        if(LOGDEV1){
           log_dev1("\n%s: end of loop. counter=%d.", __func__, counter);
        }

        if(counter >= D_FLM_MAX_FILE_NUM){
            if(LOGDEV0){
                log_dev0("\n%s: file num over %d", __func__, D_FLM_MAX_FILE_NUM);
            }
            rc = -1;
            break;
        }
    }

    fclose(fp);
    if(LOGDEV1){
       log_dev1("\n%s: rc=%d, counter=%d.", __func__, rc, counter);
    }

    return rc; /* pgr0524 */
}

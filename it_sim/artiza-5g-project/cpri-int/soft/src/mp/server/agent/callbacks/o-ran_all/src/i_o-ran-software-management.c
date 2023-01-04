/*!
 *  @skip       $Id:$
 *  @file       i_o-ran-software-management.c
 *  @brief      SAA o-ran-software-management
 *  @author     KCN) Nakai Hiroyuki
 *  @date       2019/02/22 KCN)Nakai create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup  SAA_MOD_O_RAN_SOFTWARE_MANAGEMENT
 * @brief       SAA o-ran-software-management
 * @note        charとxmlChar, boolとboolean, unsigned intとUINTなどの型表記が混在しています。
 * @note        共通関数や内部関数とのインタフェースには前者、 u_o-ran-software-managementとのインタフェースには後者の型を使ってます。
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
#include "f_mpsw_msg_o-ran_software_management_def.h"
#include "f_mpsw_msg_o-ran_software_management_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_msi_lib.h"
#include "f_saa_ntf_lib.h"
#include "u_o-ran-software-management.h"
#include "i_o-ran-software-management.h"

/* for debug without MSI, uncomment the line below. */
//#define DEBUG_NO_MSI /*for debug */

#define D_SWM_SLOT_NUM 2
#define D_SWM_SLOT_FILE_MAX 64

#define MAX_FILENAME_LEN 256
#define MAX_STATUS_LEN 32
#define SKIP_TIME_MSEC 500 /* must be under 1000 */

#define D_SWM_URI_LEN 256
#define D_SWM_PASSWORD_LEN 256

/* インベントリ情報 */
static T_MPSW_SOFTWARE_INVENTORY m_inventory;
/* ファイル情報 */
static T_MPSW_FILE m_files[D_SWM_SLOT_NUM][D_SWM_SLOT_FILE_MAX];

/* インベントリ更新情報 */
static bool m_force_to_update = true;
static struct timespec m_last_updated;

/* notification handlers */
void i_o_ran_software_management_install_event_handler(void *data, uint32_t size);
void i_o_ran_software_management_download_event_handler(void *data, uint32_t size);
void i_o_ran_software_management_activation_event_handler(void *data, uint32_t size);

/* internal functions */
static int get_inventory(void);
void set_dummy_inventory(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM  *cfm);
static int download(const char *uri, const char *password, unsigned int *timeout, char *message);
static int install(const char *slot_name, const char *file_name, char *message);
static int activate(const char *slot_name, unsigned int *timeout, char *messages);
static void set_update_time(void);
static bool need_to_update(void);
static void dump_slot(T_MPSW_SOFTWARE_SLOT *slot);


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
void i_o_ran_software_management_init2(void){
    if (LOGINFO) {
        log_info("\nEnter %s", __func__);
    }

    f_saa_com_reg_notification_handler(
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND,
        i_o_ran_software_management_download_event_handler
        );

    f_saa_com_reg_notification_handler(
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND,
        i_o_ran_software_management_install_event_handler
        );

    f_saa_com_reg_notification_handler(
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND,
        i_o_ran_software_management_activation_event_handler
        );
}

/*!
 * @brief   ソフトウェアインベントリ情報を更新する。
 * @note    
 * @param   なし
 * @return  インベントリ情報のアドレス: 成功
 * @return  NULL: 失敗
 * @date    2019/3/1 KCN)中井
 */
T_MPSW_SOFTWARE_INVENTORY  *i_o_ran_software_management_inventory_update(void){
    int rc = 0;

    rc = get_inventory();
    if(rc == 0){
        return &m_inventory;
    }
    else{
        return NULL;
    }
}

/*!
 * @brief   スロットのインデックスを検索する。
 * @note    
 * @param   name          スロット名
 * @param   name_fixed    スロット名指定の有無
 * @param   name_present  スロット名入力の有無
 * @param   getnext       次レコード検索指示
 * @param   more_data     継続データの有無を格納するバッファ
 * @return  インデックス値: 成功
 * @return  -1: 失敗
 * @date    2019/3/1 KCN)中井
 */
int i_o_ran_software_management_slot_find(const xmlChar *name, boolean name_fixed, boolean name_present, boolean getnext, boolean *more_data){
    int slot_indx = -1;
    int i = 0;

    /* 一応入力してもらったが、使わなくてもよさそう */
    (void)name_fixed;
    (void)name_present;

    if(more_data != NULL){
        *more_data = FALSE;
    }

    if(name == NULL || xml_strlen(name) == 0){
        if(more_data != NULL){
            *more_data = TRUE;
        }
        if(LOGDEV1){
            log_dev1("\n%s: first, slot_indx=%d", __func__, slot_indx);
        }
        return 0;
    }

    for(i=0; i<D_SWM_SLOT_NUM; i++){
        if(strcmp((const char *)name, m_inventory.software_slot[i].name) == 0){
            slot_indx = i;
            break;
        }
    }

    if(slot_indx < 0){
        if(LOGWARN){
            log_warn("\n%s: slot name not found.", __func__);
        }
        return -1;
    }

    if(getnext){
        if(slot_indx >= (D_SWM_SLOT_NUM - 1)){
            slot_indx = -1;
            if(LOGDEV0){
                log_dev0("\n%s: no more slot.", __func__);
            }
        }
        else{
            slot_indx++;
            if(slot_indx < (D_SWM_SLOT_NUM - 1)){
                if(more_data != NULL){
                    *more_data = TRUE;
                }
            }
            if(LOGDEV1){
                log_dev1("\n%s: found next, slot_indx=%d", __func__, slot_indx);
            }
        }
    }

    if(LOGDEV1){
        log_dev1("\n%s: slot_name=%s, slot_indx=%d", __func__, name, slot_indx);
    }

    return slot_indx;
}

/*!
 * @brief   ファイルダウンロードを行う。
 * @note    処理の本体は内部関数 download()で実行する。
 * @param   uri      URI(sftp://<username>@<host>[:<port>])
 * @param   password パスワード
 * @param   timeout  タイムアウト値の格納アドレス(成功時のみ有効)
 * @param   message  エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/1 KCN)中井
 */
int i_o_ran_software_management_software_download_invoke (
    const xmlChar *uri, const xmlChar *password, UINT *timeout, xmlChar *message){

    int rc = -1;

    rc = download((const char *)uri, (const char *)password,
             (unsigned int *)timeout, (char *)message);

    return rc;
}

/*!
 * @brief   ファイルインストールを行う。
 * @note    処理の本体は内部関数 install()で実行する。
 * @note    ファイルは1つのみ指定できる。
 * @param   slot_name スロット名
 * @param   file_name ファイル名の
 * @param   message  エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/1 KCN)中井
 */
int i_o_ran_software_management_software_install_invoke (
    const xmlChar *slot_name, const xmlChar *file_name, xmlChar *message){

    int rc = -1;

    rc = install((const char *)slot_name, (const char *)file_name, (char *)message);

    return rc;
}

/*!
 * @brief   ファイルアクティベートを行う。
 * @note    処理の本体は内部関数 activate()で実行する。
 * @param   slot_name スロット名
 * @param   timeout   タイムアウト値の格納アドレス(成功時のみ有効)
 * @param   message   エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/1 KCN)中井
 */
int i_o_ran_software_management_software_activate_invoke (
    const xmlChar *slot_name,UINT *timeout, xmlChar *message){

    int rc = -1;

    rc = activate((const char *)slot_name,
             (unsigned int *)timeout, (char *)message);

    return rc;
}

/* ====== Notification Handlers ====== */

/*!
 * @brief   ダウンロード結果のnotificationを送信する。
 * @note    
 * @param   data 結果データのアドレス
 * @param   size 結果データのサイズ
 * @return  なし
 * @date    2019/3/1 KCN)中井
 */
void i_o_ran_software_management_download_event_handler(void *data, uint32_t size){
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND *info = NULL;
    char *file_name = NULL;
    char event_status[MAX_STATUS_LEN];
    char message[D_SWM_MAX_MESSAGE_LEN];

    (void)size;

    memset(message, 0, sizeof(message));

    info = (T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_NTF_IND *)data;
    file_name = info->file_name;
    snprintf(message, D_SWM_MAX_MESSAGE_LEN, "%d: %s",
        info->msgErr.errorCode, info->msgErr.errorDescription);

    switch(info->status){
        case E_SWM_DOWNLOAD_STATUS_COMPLETED:
            strcpy(event_status,"COMPLETED");
            break;
        case E_SWM_DOWNLOAD_STATUS_AUTHENTICATION_ERROR:
            strcpy(event_status,"AUTHENTICATION_ERROR");
            break;
        case E_SWM_DOWNLOAD_STATUS_PROTOCOL_ERROR:
            strcpy(event_status,"PROTOCOL_ERROR");
            break;
        case E_SWM_DOWNLOAD_STATUS_FILE_NOT_FOUND:
            strcpy(event_status,"FILE_NOT_FOUND");
            break;
        case E_SWM_DOWNLOAD_STATUS_TIMEOUT:
            strcpy(event_status,"TIMEOUT");
            break;
        case E_SWM_DOWNLOAD_STATUS_APPLICATION_ERROR:
        /* no break */
        default:
            strcpy(event_status,"APPLICATION_ERROR");
            break;
    }

    if(LOGINFO){
        log_info("\n%s: file_name=%s, event_status=%s", __func__, file_name, event_status);
    }

    u_o_ran_software_management_download_event_send((xmlChar *)file_name, (xmlChar *)event_status, (xmlChar *)message);
}


/*!
 * @brief   インストール結果のnotificationを送信する。
 * @note    
 * @param   data 結果データのアドレス
 * @param   size 結果データのサイズ
 * @return  なし
 * @date    2019/3/1 KCN)中井
 */
void i_o_ran_software_management_install_event_handler(void *data, uint32_t size){
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND *info = NULL;
    char *slot_name = NULL;
    char event_status[MAX_STATUS_LEN];
    char message[D_SWM_MAX_MESSAGE_LEN];

    (void)size;
    memset(message, 0, sizeof(message));

    info = (T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_NTF_IND *)data;
    slot_name = info->slot_name;
    snprintf(message, D_SWM_MAX_MESSAGE_LEN, "%d: %s",
        info->msgErr.errorCode, info->msgErr.errorDescription);

    switch(info->status){
        case E_SWM_INSTALL_STATUS_COMPLETED:
            strcpy(event_status,"COMPLETED");
            break;
        case E_SWM_INSTALL_STATUS_FILE_ERROR:
            strcpy(event_status,"FILE_ERROR");
            break;
        case E_SWM_INSTALL_STATUS_INTEGRITY_ERROR:
            strcpy(event_status,"INTEGRITY_ERROR");
            break;
        case E_SWM_INSTALL_STATUS_APPLICATION_ERROR:
        /* no break */
        default:
            strcpy(event_status,"APPLICATION_ERROR");
            break;
    }

    if(LOGINFO){
        log_info("\n%s: slot_name=%s, event_status=%s", __func__, slot_name, event_status);
    }

    u_o_ran_software_management_install_event_send((xmlChar *)slot_name, (xmlChar *)event_status, (xmlChar *)message);
}

/*!
 * @brief   アクティベート結果のnotificationを送信する。
 * @note    
 * @param   data 結果データのアドレス
 * @param   size 結果データのサイズ
 * @return  なし
 * @date    2019/3/1 KCN)中井
 */
void i_o_ran_software_management_activation_event_handler(void *data, uint32_t size){
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND *info = NULL;
    char *slot_name = NULL;
    char event_status[MAX_STATUS_LEN];
    uint8 code = 0;
    char message[D_SWM_MAX_MESSAGE_LEN];

    (void)size;
    memset(message, 0, sizeof(message));

    info = (T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND *)data;
    slot_name = info->slot_name;
    snprintf(message, D_SWM_MAX_MESSAGE_LEN, "%d: %s",
        info->msgErr.errorCode, info->msgErr.errorDescription);

    switch(info->status){
        case E_SWM_ACTIVATE_STATUS_COMPLETED:
            strcpy(event_status,"COMPLETED");
            break;
        case E_SWM_ACTIVATE_STATUS_APPLICATION_ERROR:
        /* no break */
        default:
            strcpy(event_status,"APPLICATION_ERROR");
            break;
    }

    if(LOGINFO){
        log_info("\n%s: slot_name=%s, event_status=%s", __func__, slot_name, event_status);
    }

    u_o_ran_software_management_activation_event_send((xmlChar *)slot_name, (xmlChar *)event_status, code, (xmlChar *)message);
}


/* ====== Utilities ====== */

/*!
 * @brief   ファイルのインデックスを検索する。
 * @note    
 * @param   slot_indx     スロットのインデックス値
 * @param   name          ファイル名
 * @param   name_fixed    ファイル名指定の有無
 * @param   name_present  ファイル名入力の有無
 * @param   getnext       次レコード検索指示
 * @param   more_data     継続データの有無を格納するバッファ
 * @return  インデックス値: 成功
 * @return  -1: 失敗
 * @date    2019/3/1 KCN)中井
 */
int i_o_ran_software_management_file_find(int slot_indx, const xmlChar *name, boolean name_fixed, boolean name_present, boolean getnext, boolean *more_data){
    int file_indx = -1;
    int i = 0;
    int file_num = 0;

    /* 一応入力してもらったが、使わなくてもよさそう */
    (void)name_fixed;
    (void)name_present;

    if(more_data != NULL){
        *more_data = FALSE;
    }

    if(name == NULL || xml_strlen(name) == 0){
        if(more_data != NULL){
            *more_data = TRUE;
        }
        if(LOGDEV1){
            log_dev1("\n%s: first, file_indx=%d", __func__, file_indx);
        }
        return 0;
    }

    file_num = m_inventory.software_slot[slot_indx].file_num;
    for(i=0; i< file_num && i<D_SWM_SLOT_FILE_MAX; i++){
        if(strcmp((const char *)name, m_files[slot_indx][i].name) == 0){
            file_indx = i;
            break;
        }
    }

    if(file_indx < 0){
        if(LOGWARN){
            log_warn("\n%s: file name not found.", __func__);
        }
        return -1;
    }

    if(getnext){
        /* find next */
        if(file_indx >= (file_num -1)){
            file_indx = -1;
            if(LOGDEV0){
                log_dev0("\n%s: no more file.", __func__);
            }
        }
        else{
            file_indx++;
            if(file_indx < (file_num -1)){
                if(more_data != NULL){
                    *more_data = TRUE;
                }
            }
            if(LOGDEV1){
                log_dev1("\n%s: found next, file_indx=%d", __func__, file_indx);
            }
        }
    }

    if(LOGDEV1){
        log_dev1("\n%s: file_name=%s, file_indx=%d", __func__, name, file_indx);
    }

    return file_indx;
}

/*!
 * @brief   スロット状態のENUMを文字列に変換する。
 * @note    
 * @param   indx     ENUM値
 * @return  変換した文字列のアドレス
 * @date    2019/3/1 KCN)中井
 */
xmlChar * i_o_ran_software_management_slot_status(int index){
    static char str[16];

    memset(str, 0, sizeof(str));

    switch (m_inventory.software_slot[index].status){
        case E_SWM_SLOT_STATUS_VALID:
            strcpy(str, "VALID");
            break;
        case E_SWM_SLOT_STATUS_INVALID:
            strcpy(str, "INVALID");
            break;
        case E_SWM_SLOT_STATUS_EMPTY:
        /* no break */
        default:
            strcpy(str, "EMPTY");
            break;
        }

    return (xmlChar *)str;
}

/*!
 * @brief   スロットアクセス種別のENUMを文字列に変換する。
 * @note    
 * @param   indx     ENUM値
 * @return  変換した文字列のアドレス
 * @date    2019/3/1 KCN)中井
 */
xmlChar * i_o_ran_software_management_slot_access(int index){
    static char str[16];

    memset(str, 0, sizeof(str));

    switch (m_inventory.software_slot[index].access){
        case E_SWM_SLOT_ACCESS_RO:
            strcpy(str, "READ_ONLY");
            break;
        case E_SWM_SLOT_ACCESS_RW:
            strcpy(str, "READ_WRITE");
            break;
        default:
            break;
    }

    return (xmlChar *)str;
}


/*!
 * @brief   ファイル名を得る。
 * @note    
 * @param   slot_indx     スロットのインデックス値
 * @param   file_indx     ファイルのインデックス値
 * @return  ファイル名
 * @date    2019/3/1 KCN)中井
 */
xmlChar * i_o_ran_software_management_slot_file_name(int slot_indx, int file_indx){
    xmlChar *xml_str = NULL;
    xml_str =  (xmlChar *)m_files[slot_indx][file_indx].name;
    if(LOGDEV1){
        log_dev1("\n%s: slot_indx=%d, file_indx=%d, file_name=%s", __func__, slot_indx, file_indx, xml_str);
    }
    return xml_str;
}

/*!
 * @brief   ファイルのバージョンを得る。
 * @note    
 * @param   slot_indx     スロットのインデックス値
 * @param   file_indx     ファイルのインデックス値
 * @return  バージョン
 * @date    2019/3/1 KCN)中井
 */
xmlChar * i_o_ran_software_management_slot_file_version(int slot_indx, int file_indx){
    return (xmlChar *)m_files[slot_indx][file_indx].version;
}

/*!
 * @brief   ファイルのローカルパスを得る。
 * @note    
 * @param   slot_indx     スロットのインデックス値
 * @param   file_indx     ファイルのインデックス値
 * @return  ローカルパス
 * @date    2019/3/1 KCN)中井
 */
xmlChar * i_o_ran_software_management_slot_file_local_path(int slot_indx, int file_indx){
    return (xmlChar *)m_files[slot_indx][file_indx].local_path;
}

/*!
 * @brief   ファイルの整合性を得る。
 * @note    
 * @param   slot_indx     スロットのインデックス値
 * @param   file_indx     ファイルのインデックス値
 * @return  整合性
 * @date    2019/3/1 KCN)中井
 */
xmlChar * i_o_ran_software_management_slot_file_integrity(int slot_indx, int file_indx){
    static char str[16];

    memset(str, 0, sizeof(str));

    switch (m_files[slot_indx][file_indx].integrity){
        case E_SWM_SLOT_INTEGRITY_OK:
            strcpy(str, "OK");
            break;
        case E_SWM_SLOT_INTEGRITY_NOK:
            strcpy(str, "NOK");
            break;
        default:
            break;
        }

    return (xmlChar *)str;
}

/*!
 * @brief   スロット名がインベントリ情報内に存在するかを検査する。
 * @note    
 * @param   name スロット名
 * @return  TRUE: 存在する, FALSE: 存在しない
 * @date    2019/3/1 KCN)中井
 */
boolean i_o_ran_software_management_slot_validate(const xmlChar *name){
    boolean rc = FALSE;
    int  cmd_rc = 0;
    int  slot_indx = 0;

    cmd_rc = get_inventory();
    if(cmd_rc < 0){
        return FALSE;
    }

    slot_indx = i_o_ran_software_management_slot_find(name, TRUE, TRUE, FALSE, NULL);
    if(slot_indx >= 0){
        rc = TRUE;
    }

    return rc;
}

/*!
 * @brief   ダウンロード要求パラメータが妥当かを検査する。
 * @note    
 * @param   uri      ダウンロード対象のURI
 * @param   password パスワード
 * @return  TRUE: 正常, FALSE: 異常
 * @date    2019/6/24 KCN)中井
 */
boolean i_o_ran_software_management_download_validate(const xmlChar *remote_file_path, const xmlChar *password){
    boolean rc = TRUE;

    if(xml_strlen(remote_file_path) > (D_SWM_URI_LEN -1) || xml_strlen(password) > (D_SWM_PASSWORD_LEN - 1)){
        rc = FALSE;
    }

    return rc;
}

/* ====== Internal Functions ====== */

/*!
 * @brief   ソフトウェアインベントリ情報を取得する。
 * @note    取得したデータはモジュール内のグローバル変数 m_inventory, m_filesに格納する。
 * @param   なし
 * @return  0:成功, -1:失敗
 * @date    2019/3/1 KCN)中井
 */
static int get_inventory(void){
    int rc = -1;
    uint32_t read_size = 0;
    T_MPSW_SOFTWARE_SLOT *slot_ptr = NULL;
    size_t delta = 0;

    char buffer[sizeof(m_inventory) + sizeof(m_files)];
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM *cfm = NULL;

    if(LOGINFO){
        log_info("\n%s: get_inventory called.", __func__);
    }

    if(!need_to_update()){
        return 0;
    }

    memset(&m_inventory, 0, sizeof(m_inventory));
    memset(&m_files, 0, sizeof(m_files));
    memset(buffer, 0, sizeof(buffer));
    cfm = (T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM *)buffer;


#ifndef DEBUG_NO_MSI
    if(LOGINFO){
        log_info("\n%s: start to get inventory from pf.", __func__);
    }

    rc = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REQ,
        0,
        NULL,
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM,
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_REJ,
        sizeof(buffer), buffer, &read_size);
#else
    if(LOGINFO){
        log_info("\n%s: set dummy data for test.", __func__);
    }
    set_dummy_inventory(cfm);
    set_update_time();
    rc = 0;
#endif

    if(LOGINFO){
        log_info("\n%s: MSI responded.: rc=%d, read_size=%d.", __func__, rc, read_size);
    }
    log_flush();

    if(rc == D_MSI_CFM){
        slot_ptr = &cfm->software_inventory.software_slot[0];

        int i,j;
        for(i=0; i<D_SWM_SLOT_NUM; i++){
            if(slot_ptr->status == E_SWM_SLOT_STATUS_VALID){
                if(LOGDEV1){
                    log_dev1("\n%s: slot[%d], valid", __func__, i);
                }
                dump_slot(slot_ptr);
                /* スロット情報のコピー */
                memcpy(&m_inventory.software_slot[i], slot_ptr, sizeof(T_MPSW_SOFTWARE_SLOT));
                /* ファイル情報のコピー */
                for(j=0; j<slot_ptr->file_num; j++){
                    memcpy(&m_files[i][j], &slot_ptr->file[j], sizeof(T_MPSW_FILE));
                }
            }
            else{
                /* INVALID/EMPYの場合は固定値を設定 */
                /* name, statusのみ受信したデータを使用する。*/
                if(LOGWARN){
                    log_warn("\n%s: slot[%d], status=%d", __func__, i, slot_ptr->status);
                }
                /* スロット情報の設定 */
                strcpy(m_inventory.software_slot[i].name, slot_ptr->name);
                m_inventory.software_slot[i].status = slot_ptr->status;
                m_inventory.software_slot[i].active = false;
                m_inventory.software_slot[i].running = false;
                m_inventory.software_slot[i].access = E_SWM_SLOT_ACCESS_RW;
                strcpy(m_inventory.software_slot[i].product_code, "0000");
                strcpy(m_inventory.software_slot[i].vendor_code, "0000");
                strcpy(m_inventory.software_slot[i].build_id, "0000");
                strcpy(m_inventory.software_slot[i].build_name, "0000");
                strcpy(m_inventory.software_slot[i].build_version, "0000");
                m_inventory.software_slot[i].file_num = 1;

                /* ファイル情報の設定 */
                for(j=0; j<slot_ptr->file_num; j++){
                    strcpy(m_files[i][j].name ,"0000");
                    strcpy(m_files[i][j].version ,"0000");
                    strcpy(m_files[i][j].local_path ,"");
                    m_files[i][j].integrity = E_SWM_SLOT_INTEGRITY_NOK;
                }
            }

            delta = sizeof(T_MPSW_SOFTWARE_SLOT) + sizeof(T_MPSW_FILE) * (unsigned int)(slot_ptr->file_num - 1);
            slot_ptr = (T_MPSW_SOFTWARE_SLOT *)((char *)slot_ptr + sizeof(char) * delta);
            if(LOGDEV1){
                log_dev1("\n%s: delta=%ld, slot_ptr:%p", __func__, delta, slot_ptr);
            }
        }
        log_flush();
        set_update_time();
    }
    else{
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: rc=%d.", __func__, rc);
        }
    }

    return rc;
}

/*!
 * @brief   テスト用のダミーのソフトウェアインベントリ情報を設定する。
 * @note    データはモジュール内のグローバル変数 m_inventory, m_filesに格納する。
 * @param   なし
 * @return  なし
 * @date    2019/3/1 KCN)中井
 */
void set_dummy_inventory(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM * cfm){
    strcpy(cfm->software_inventory.software_slot[0].name, "slot0");
    cfm->software_inventory.software_slot[0].status = E_SWM_SLOT_STATUS_VALID;
    cfm->software_inventory.software_slot[0].active = true;
    cfm->software_inventory.software_slot[0].running = true;
    cfm->software_inventory.software_slot[0].access = E_SWM_SLOT_ACCESS_RW;
    strcpy(cfm->software_inventory.software_slot[0].product_code, "0001");
    strcpy(cfm->software_inventory.software_slot[0].vendor_code, "FJ");
    strcpy(cfm->software_inventory.software_slot[0].build_id, "0001");
    strcpy(cfm->software_inventory.software_slot[0].build_name, "my_build_name");
    strcpy(cfm->software_inventory.software_slot[0].build_version, "0001");

    cfm->software_inventory.software_slot[0].file_num = 1;
    strcpy(cfm->software_inventory.software_slot[0].file[0].name,"filename0");
    strcpy(cfm->software_inventory.software_slot[0].file[0].version,"1234");
    strcpy(cfm->software_inventory.software_slot[0].file[0].local_path,"/somewhere");

    strcpy(cfm->software_inventory.software_slot[1].name, "slot1");
    cfm->software_inventory.software_slot[1].status = E_SWM_SLOT_STATUS_INVALID;
    cfm->software_inventory.software_slot[1].active = false;
    cfm->software_inventory.software_slot[1].running = false;
    cfm->software_inventory.software_slot[1].access = E_SWM_SLOT_ACCESS_RW;
    strcpy(cfm->software_inventory.software_slot[1].product_code, "0001");
    strcpy(cfm->software_inventory.software_slot[1].vendor_code, "FJ");
    strcpy(cfm->software_inventory.software_slot[1].build_id, "0001");
    strcpy(cfm->software_inventory.software_slot[1].build_name, "my_build_name");
    strcpy(cfm->software_inventory.software_slot[1].build_version, "0001");

    cfm->software_inventory.software_slot[1].file_num = 1;
    strcpy(cfm->software_inventory.software_slot[1].file[0].name,"filename1");
    strcpy(cfm->software_inventory.software_slot[1].file[0].version,"2345");
    strcpy(cfm->software_inventory.software_slot[1].file[0].local_path,"/somewhere-else");
}
        
/*!
 * @brief   ファイルダウンロードを行う。
 * @note    
 * @param   uri      ダウンロード対象のURI
 * @param   password パスワード
 * @param   timeout  タイムアウト値の格納アドレス(成功時のみ有効)
 * @param   message  エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/1 KCN)中井
 */
static int download(const char *uri, const char *password, unsigned int *timeout, char *message){
    int rc = 0;
    int msi_rc = 0;
    uint32_t read_size = 0;

    if(uri == NULL || password == NULL){
        if(LOGWARN){
            log_warn("\n%s: uri=%p, password=%p", __func__, uri, password);
        }
        return -1;
    }

    if(LOGINFO){
        log_info("\n%s: uri=%s", __func__, uri);
    }

#ifndef DEBUG_NO_MSI
    /* CFM < REJ */
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ request_buffer;
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ response_buffer;

    memset(&request_buffer, 0, sizeof(request_buffer));
    memset(&response_buffer, 0, sizeof(response_buffer));

    strcpy(request_buffer.uri, uri);
    strcpy(request_buffer.password, password);

    msi_rc = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REQ,
        sizeof(request_buffer),
        &request_buffer,
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM,
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_REJ,
        sizeof(response_buffer), &response_buffer, &read_size);

    if(msi_rc == D_MSI_CFM){
        *timeout = ((T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_DOWNLOAD_CFM *)&response_buffer)->notification_timeout;
        if(LOGDEV1){
            log_dev1("\n%s: MSI responded: msi_rc=%di, read_size=%d, timeout=%d.", __func__, msi_rc, read_size, *timeout);
        }
        rc = 0;
    }
    else {
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: msi_rc=%d.", __func__, msi_rc);
        }
        rc = -1;
        *timeout = 0;
        if(msi_rc == D_MSI_REJ){
            snprintf(message, D_SWM_MAX_MESSAGE_LEN, "%d: %s",
                response_buffer.msgErr.errorCode, response_buffer.msgErr.errorDescription);
        }
        else{
            strcpy(message,"internal error.");
        }
    }
#else
    /* デバッグ用にビルドされたときのみ */
    if(LOGINFO){
        log_info("\n%s: password=%s", __func__, password);
    }
    *timeout = 20;
    rc = 0;
    (void)msi_rc;
    (void)message;
    (void)read_size;
#endif

    return rc;
}
        
/*!
 * @brief   ファイルインストールを行う。
 * @note    ファイルは1つのみ指定できる。
 * @param   slot_name 対象スロット名
 * @param   file_num  ファイル数
 * @param   file_name ファイル名
 * @param   message  エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/1 KCN)中井
 */
static int install(const char *slot_name, const char *file_name, char *message){
    int rc = 0;
    int msi_rc = 0;
    uint32_t read_size = 0;

    if(LOGINFO){
        log_info("\n%s: slot_name=%s", __func__, slot_name);
    }

    /* この後のget_inventory()では必ずMSIでPFに問い合わせるようにする。 */
    m_force_to_update = true;

#ifndef DEBUG_NO_MSI

    /* CFM < REJ */
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ request_buffer;
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ response_buffer;

    memset(&request_buffer, 0, sizeof(request_buffer));
    memset(&response_buffer, 0, sizeof(response_buffer));

    strncpy(request_buffer.slot_name, slot_name, sizeof(request_buffer.slot_name));
    request_buffer.file_num = 1;
    strncpy(request_buffer.file_name[0], file_name, MAX_FILENAME_LEN);

    msi_rc = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REQ,
        sizeof(request_buffer),
        &request_buffer,
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_CFM,
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INSTALL_REJ,
        sizeof(response_buffer), &response_buffer, &read_size);

    if(msi_rc == D_MSI_CFM){
        rc = 0;
        if(LOGDEV1){
            log_dev1("\n%s: MSI responded: msi_rc=%d, read_size=%d.", __func__, msi_rc, read_size);
        }
    }
    else {
        rc = -1;
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: msi_rc=%d.", __func__, msi_rc);
        }
        if(msi_rc == D_MSI_REJ){
            snprintf(message, D_SWM_MAX_MESSAGE_LEN, "%d: %s",
                response_buffer.msgErr.errorCode, response_buffer.msgErr.errorDescription);
        }
        else{
            strcpy(message,"internal error.");
        }
    }
#else
    (void)slot_name;
    (void)file_name;
    (void)msi_rc;
    (void)message;
    (void)read_size;
    rc = 0;
#endif

    return rc;
}

/*!
 * @brief   ファイルのアクティベートを行う。
 * @note    
 * @param   slot_name 対象スロット名
 * @param   timeout  タイムアウト値の格納アドレス(成功時のみ有効)
 * @param   message  エラーメッセージの格納アドレス(失敗時のみ有効)
 * @return  0:成功, -1:失敗
 * @date    2019/3/1 KCN)中井
 */
static int activate(const char *slot_name, unsigned int *timeout, char *message){
    int rc = 0;
    int msi_rc = 0;
    uint32_t read_size = 0;

    if(LOGINFO){
        log_info("\n%s: slot_name=%s", __func__, slot_name);
    }

    /* この後のget_inventory()では必ずMSIでPFに問い合わせるようにする。 */
    m_force_to_update = true;

#ifndef DEBUG_NO_MSI
    /* CFM < REJ */
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ request_buffer;
    char response_buffer[max(sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM), sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ))];
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM *cfm = NULL;
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ *rej = NULL;

    memset(&request_buffer, 0, sizeof(request_buffer));
    memset(&response_buffer, 0, sizeof(response_buffer));
    cfm = (T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM *)response_buffer;
    rej = (T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ *)&response_buffer;

    strncpy(request_buffer.slot_name, slot_name, sizeof(request_buffer.slot_name));

    msi_rc = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ,
        sizeof(request_buffer),
        &request_buffer,
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM,
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ,
        max(sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM), sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REJ)),
        &response_buffer, &read_size);

    if(msi_rc == D_MSI_CFM){
        *timeout = cfm->notification_timeout;
        if(LOGDEV1){
            log_dev1("\n%s: MSI responded: msi_rc=%d, read_size=%d, timeout=%d.", __func__, msi_rc, read_size, *timeout);
        }
        rc = 0;
    }
    else{
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: msi_rc=%d.", __func__, msi_rc);
        }
        rc = -1;
        *timeout = 0;
        if(msi_rc == D_MSI_REJ){
            snprintf(message, D_SWM_MAX_MESSAGE_LEN, "%d: %s",
                rej->msgErr.errorCode, rej->msgErr.errorDescription);
        }
        else{
            strcpy(message,"internal error.");
        }
    }
#else
    (void)msi_rc;
    (void)slot_name;
    (void)message;
    (void)read_size;
    *timeout = 20;
    rc = 0;
#endif

    return rc;
}

/*!
 * @brief       インベントリ情報の更新時間を記録する。
 * @note        
 * @param       なし
 * @return      なし
 * @date        2019/3/1 KCN)Nakai create
 */
static void set_update_time(void){
    struct timespec current_time;

    if(clock_gettime(CLOCK_MONOTONIC_RAW, &current_time) == 0){
        m_last_updated.tv_sec = current_time.tv_sec;
        m_last_updated.tv_nsec = current_time.tv_nsec;
        m_force_to_update = false;
    }
    else{
        if(LOGDEV0){
            log_dev0("\n%s: cannot get current time. errno=%d.", __func__, errno);
        }
    }
}

/*!
 * @brief       インベントリ情報の更新要または不要を返す。
 * @note        
 * @param       なし
 * @return      true:要, false:不要
 * @date        2019/3/1 KCN)Nakai create
 */
static bool need_to_update(void){
    bool rc = false;
    time_t sec = 0;
    suseconds_t usec = 0;
    struct timespec current_time;

    if(clock_gettime(CLOCK_MONOTONIC_RAW, &current_time) == -1){
        if(LOGDEV0){
            log_dev0("\n%s: cannot get current time. errno=%d.", __func__, errno);
        }
        /* nothing to be able to do. */
        return false;
    }

    if(m_force_to_update == true){
        rc = true;
    }
    else{
        // #24560の対処として時刻取得をgettimeofday()からclock_gettime()に変更したが、
        // 元々usecで計算していた既存処理の変更を最小にするために最初にnanosec -> usecをやっている。
        sec  = current_time.tv_sec - m_last_updated.tv_sec;
        usec = (current_time.tv_nsec - m_last_updated.tv_nsec) / 1000;

        if(sec > 1){
            rc = true;
        }
        /* usecに直して比較 */
        else if(sec == 1){
            if((1000000 +  usec) > (1000 * SKIP_TIME_MSEC)){
                rc = true;
            }
        }
        else if(sec == 0){
            if(usec > (1000 *SKIP_TIME_MSEC)){
                rc = true;
            }
        }
        else {
            if(LOGDEV0){
                log_dev0("\n%s: time reversed.", __func__);
            }
            rc = true;
        }
    }

    return rc;
}

static void dump_slot(T_MPSW_SOFTWARE_SLOT *slot){
    int i = 0;

    if(LOGDEV1){
        log_dev1("\n%s: size of slot=%ld", __func__, sizeof(T_MPSW_SOFTWARE_SLOT ));
        log_dev1("\n%s: size of file=%ld", __func__, sizeof(T_MPSW_FILE));
        log_dev1("\n%s: slot: %p", __func__, slot);
        log_flush();

        log_dev1("\n%s: name=[%s]", __func__, slot->name);
        log_dev1("\n%s: status=%d", __func__, slot->status);
        log_dev1("\n%s: active=%d", __func__, slot->active);
        log_dev1("\n%s: running=%d", __func__, slot->running);
        log_dev1("\n%s: product_code=[%s]", __func__, slot->product_code);
        log_dev1("\n%s: vendor_code=[%s]", __func__, slot->vendor_code);
        log_dev1("\n%s: build_id=[%s]", __func__, slot->build_id);
        log_dev1("\n%s: build_name=[%s]", __func__, slot->build_name);
        log_dev1("\n%s: build_version=[%s]", __func__, slot->build_version);
        log_dev1("\n%s: file_num=%d", __func__, slot->file_num);
        log_flush();

        for(i=0; i<slot->file_num; i++){
            log_dev1("\n%s:     file[%d].name=[%s]", __func__,
                i, slot->file[i].name);
            log_dev1("\n%s:     file[%d].version=[%s]", __func__,
                i,slot->file[i].version);
            log_dev1("\n%s:     file[%d].local_path=[%s]", __func__,
                i,slot->file[i].local_path);
        }

        log_flush();
    }
}
    

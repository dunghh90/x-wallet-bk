/*
 * @file    i_o-ran-usermgmt.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_USERMGMT
 * @brief       SAA o-ran-usermgmt
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "u_o-ran-usermgmt.h"
#include "i_o-ran-usermgmt.h"
#include "sil_sa.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"

#include "agt_util.h"
#include "db_api.h"

/********************************************************************************************************************/
/*   valuable                                                                                                       */
/********************************************************************************************************************/
static char i_mod_name[]                            = i_mod_o_ran_usermgmt;
static char i_para_usermgmt_num_of_user[]           = i_para_o_ran_usermgmt_num_of_user;
UINT table_commit_flg[SAA_USERMGMT_MAX_USER_NUM];
UINT max_num_set_user_data;
T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ new_user_data[SAA_USERMGMT_MAX_USER_NUM];
T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ current_user_data[SAA_USERMGMT_MAX_USER_NUM];


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   管理テーブルの初期化を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/14 KCN)wakamizu
 */
status_t i_o_ran_usermgmt_init(void)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* 管理テーブル初期化 */
    max_num_set_user_data = 0;
    for(int i=0; i<SAA_USERMGMT_MAX_USER_NUM; i++){
        table_commit_flg[i] = SAA_USERMGMT_USER_CHANGE_OK_FLG;
        memset(&new_user_data[i], 0 , sizeof(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ));
        memset(&current_user_data[i], 0 , sizeof(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ));
        /* enabled初期値をFFFFとする */
        new_user_data[i].enabled = SAA_USERMGMT_ENABLED_NON;
        current_user_data[i].enabled = SAA_USERMGMT_ENABLED_NON;
    }
    return res;

} /* i_o_ran_usermgmt_init */

/*!
 * @brief   Datastoreの初期設定を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/12 KCN)wakamizu
 */
status_t i_o_ran_usermgmt_init2(void)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* 初期パラメータ取得 */
    res = i_o_ran_usermgmt_get_init_para();
    if (res != NO_ERR){
        if(LOGWARN) {
            log_warn("\n%s: Failed to get initial user info", __func__);
        }
        return res;
    }

    /* user数=0のときはAPIを送らない */
    if(max_num_set_user_data == 0){
        if(LOGINFO) {
            log_info("\n%s: no set user data.", __func__);
        }
        return res;
    }

    return res;

} /* i_o_ran_usermgmt_init2 */

/*!
 * @brief   初期パラメータをファイルから取得する
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/12 KCN)wakamizu
 */
status_t i_o_ran_usermgmt_get_init_para(void)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    UINT num_of_user = 0;
    xmlChar *para_temp;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }
    /* 有効ユーザ数を取得 */
    para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_name, i_para_usermgmt_num_of_user);
    if(para_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Operation Failed to get, module_name = %s, parameter_name = %s", 
                __func__, 
                i_mod_name, 
                i_para_usermgmt_num_of_user);
        }
        return ERR_NCX_OPERATION_FAILED;
    }
    else {
        num_of_user = (UINT)atoi((const char *)para_temp);
        max_num_set_user_data = num_of_user;
    }
    
    /* データがない場合は読み出しをしない */
    if(max_num_set_user_data == 0){
        if (LOGINFO) {
            log_info("\n%s: max_num_set_user_data = 0.", __func__);
        }
        return res;
    }
    else{
        if (LOGINFO) {
            log_info("\n%s: max_num_set_user_data = %d.", __func__, max_num_set_user_data);
        }
    }

    /* ユーザ情報を取得し管理テーブルを更新 */
    for( UINT i = 0; i < max_num_set_user_data; i++){
        /* name */
        snprintf(para_name, D_PARAM_NUM_MAX, "%s.%d.%s", y_o_ran_usermgmt_N_user, i, y_o_ran_usermgmt_N_name);
        para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_name, para_name);
        if(para_temp == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Operation Failed to get, module_name = %s, parameter_name = %s", 
                    __func__, 
                    i_mod_name, 
                    para_name);
            }
            res = ERR_NCX_OPERATION_FAILED;
            break;
        }
        else {
            strcpy(current_user_data[i].user, (const char *)para_temp);
        }

        /* password */
        snprintf(para_name, D_PARAM_NUM_MAX, "%s.%d.%s", y_o_ran_usermgmt_N_user, i, y_o_ran_usermgmt_N_password);
        para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_name, para_name);
        if(para_temp == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Operation Failed to get, module_name = %s, parameter_name = %s", 
                    __func__, 
                    i_mod_name, 
                    para_name);
            }
            res = ERR_NCX_OPERATION_FAILED;
            break;
        }
        else {
            strcpy(current_user_data[i].password, (const char *)para_temp);
        }

        /* enabled */
        snprintf(para_name, D_PARAM_NUM_MAX, "%s.%d.%s", y_o_ran_usermgmt_N_user, i, y_o_ran_usermgmt_N_enabled);
        para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_name, para_name);
        if(para_temp == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Operation Failed to get, module_name = %s, parameter_name = %s", 
                    __func__, 
                    i_mod_name,
                    para_name);
            }
            res = ERR_NCX_OPERATION_FAILED;
            break;
        }
        else {
            current_user_data[i].enabled = (UINT)atoi((const char *)para_temp);
        }
    }
    
    /* 値が正常に入っているかチェック */
    /* 入っていなければ初期化してエラー応答 */
    if(res != NO_ERR){
        for(UINT j=0; j<max_num_set_user_data; j++){
            memset(&current_user_data[j], 0 , sizeof(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ));
            current_user_data[j].enabled = SAA_USERMGMT_ENABLED_NON;
        }
    }
    /* 管理テーブル間の同期 */
    else{
        for(UINT i=0; i<max_num_set_user_data; i++){
            new_user_data[i] = current_user_data[i];
        }
    }

    return res;

} /* i_o_ran_usermgmt_get_init_para */


/*!
 * @brief   validate、管理テーブル更新
 * @note    
 * @param   [in] k_o_ran_user_profile_user_name
 * @param   [in] newval_val
 * @param   [in] flg
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/14 KCN)wakamizu
 */
status_t i_o_ran_usermgmt_o_ran_user_profile_user_validate(
    const xmlChar *k_o_ran_user_profile_user_name, 
    const xmlChar *newval_val,
    bool newval_bool,
    const uint flg)
{
    status_t res = NO_ERR;
    int rtn_saa;
    UINT user_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:name = %s flg = %d.", __func__, k_o_ran_user_profile_user_name,flg);
    }

    /* ユーザ名からユーザ番号を特定 */
    rtn_saa = i_o_ran_usermgmt_check_user_num(k_o_ran_user_profile_user_name, &user_num);

    /* new_user_dataにname未登録の場合は登録する */
    if(rtn_saa == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Add name to new user data table, name = %s flg = %d.", __func__, k_o_ran_user_profile_user_name,flg);
        }
        rtn_saa = i_o_ran_usermgmt_set_user_name_to_new_user_data(k_o_ran_user_profile_user_name, &user_num);
        if(rtn_saa == D_SYS_NG) {
            if(LOGWARN) {
                log_warn("\n%s: Failed to add new user data table, name = %s flg = %d.", __func__, k_o_ran_user_profile_user_name,flg);
            }
            return ERR_NCX_RESOURCE_DENIED;
        }
    }

    /* 管理テーブルの更新 */
    /* flg値の更新 */
    /* XOR演算 */
    /* flg値で更新対象を判定 */
    /* password */
    if(flg == SAA_USERMGMT_USER_PASSWPORD_FLG){
        strcpy(new_user_data[user_num].password, (const char *)newval_val);
    table_commit_flg[user_num] = table_commit_flg[user_num] ^ SAA_USERMGMT_USER_PASSWPORD_FLG;
    }
    /* enabled */
    else if(flg == SAA_USERMGMT_USER_ENABLED_FLG){
        new_user_data[user_num].enabled = newval_bool;
        table_commit_flg[user_num] = table_commit_flg[user_num] ^ SAA_USERMGMT_USER_ENABLED_FLG;
    }
    else{
        if(LOGWARN) {
            log_warn("\n%s: Undefined flg, name = %s flg = %d.", __func__, k_o_ran_user_profile_user_name,flg);
        }
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_usermgmt_o_ran_user_profile_user_validate */

/*!
 * @brief   new_user_dataからnameの検索を行う
 * @note    
 * @param   [in] k_o_ran_user_profile_user_name
 * @param   [out] user_num
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/02/14 KCN)wakamizu
 */
int i_o_ran_usermgmt_check_user_num(
    const xmlChar *k_o_ran_user_profile_user_name,
    UINT *user_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for (uint32 i = 0; i < max_num_set_user_data; i++){
        if (!xml_strcmp((xmlChar *)new_user_data[i].user, k_o_ran_user_profile_user_name)) {
            *user_num = i;
            res = D_SYS_OK;
            if (LOGINFO) {
                log_info("\n%s: User name registered, user_num = %d", __func__, i);
            }
            break;
        }
    }

    if (res == D_SYS_NG) {
        if (LOGDEV1) {
            log_dev1("\n%s: Missing user name, name = %s", __func__, k_o_ran_user_profile_user_name);
        }
    }

    return res;

} /* i_o_ran_usermgmt_check_user_num */

/*!
 * @brief   new_user_dataにnameの登録を行う
 * @brief   num_of_userの値を更新する
 * @brief   user数上限以上の場合NGを返す
 * @note    
 * @param   [in] k_o_ran_user_profile_user_name
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/02/14 KCN)wakamizu
 */
int i_o_ran_usermgmt_set_user_name_to_new_user_data(
    const xmlChar *k_o_ran_user_profile_user_name,
    UINT *user_num)
{
    int res = D_SYS_NG;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for (uint32 i = 0; i < SAA_USERMGMT_MAX_USER_NUM; i++){
        if (LOGDEV1) {
            log_dev1("\n%s: new_user_data[%u].user = %s", __func__, i, new_user_data[i].user);
        }
        if (new_user_data[i].user[0] == '\0') {
            /* new_user_dataにuser_nameを登録 */
            strcpy(new_user_data[i].user, (const char *)k_o_ran_user_profile_user_name);
            /* new_user_dataのnum_of_userを更新 */
            max_num_set_user_data = i+1;
            *user_num = i;
            res = D_SYS_OK;
            if (LOGDEV1) {
                log_dev1("\n%s: Add links name to new user data table, name = %s, num = %d", __func__, new_user_data[i].user, i);
            }
            break;
        }
    }
    if(res == D_SYS_NG){
        if(LOGWARN) {
            log_warn("\n%s: Resource denied, capacity lack of new user data table.", __func__);
        }
    }
    return res;

} /* i_o_ran_usermgmt_set_user_name_to_new_user_data */

/*!
 * @brief   MSI送信を行う
 * @brief   CFM受信した場合はフラグの更新を行う
 * @brief   フラグが全てOKとなっていた場合は管理テーブルの同期を行う
 * @note    
 * @param   [in] k_o_ran_user_profile_user_name
 * @param   [in] flg
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/02/14 KCN)wakamizu
 */
status_t i_o_ran_usermgmt_o_ran_user_profile_user_commit(
    const xmlChar *k_o_ran_user_profile_user_name,
    const UINT flg)
{
    status_t res = NO_ERR;
    int rtn_saa;
    UINT user_num = 0;
    T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ msg_data;
    UINT tmp_flg = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:flg = %d", __func__, flg);
    }

    /* msg_data初期化 */
    memset(&msg_data, 0, sizeof(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ));

    /* new_user_dataからname検索 */
    rtn_saa = i_o_ran_usermgmt_check_user_num(k_o_ran_user_profile_user_name, &user_num);
    if(rtn_saa == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to get name from new_user_data,  name=%s", __func__, k_o_ran_user_profile_user_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSI送信用のデータ設定 */
    /* flg値により異なるデータを入力 */
    rtn_saa = i_o_ran_usermgmt_set_msi_data(&msg_data, user_num, flg, D_SYS_OK);
    if(rtn_saa == D_SYS_NG) {
        if (LOGWARN) {
            log_warn("\n%s: Failed to set user data on MSI data, name=%s flg = %d. ", __func__, k_o_ran_user_profile_user_name,flg);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSI送信 */
    res = i_o_ran_usermgmt_send_edit_msi(&msg_data, user_num, flg);
    if(res != NO_ERR){
        if (LOGWARN) {
            log_warn("\n%s: Failed to send user data on MSI data, name=%s flg = %d.", __func__, k_o_ran_user_profile_user_name,flg);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* フラグ確認 */
    for(UINT i=0; i<max_num_set_user_data; i++){
        if(table_commit_flg[i] == SAA_USERMGMT_USER_CHANGE_OK_FLG){
            tmp_flg++;
        }
    }
    if(tmp_flg == max_num_set_user_data){
        /* 管理テーブル間の同期 */
        for(UINT j=0; j<max_num_set_user_data; j++){
            current_user_data[j] = new_user_data[j];
        }
    }


    return res;

} /* i_o_ran_usermgmt_o_ran_user_profile_user_commit */

/*!
 * @brief   MSI送信用のデータをnew_user_dataから入力する
 * @brief   MSI送信用のデータをcurrent_user_dataから入力する
 * @brief   OKNGによりデータソースを変更
 * @note    
 * @param   [in] *msg_data
 * @param   [in] user_num
 * @param   [in] flg
 * @param   [in] newcur
 * @param   [out] T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2018/12/12 KCN)wakamizu
 */
int i_o_ran_usermgmt_set_msi_data(
    T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ *msg_data,
    UINT user_num,
    UINT flg,
    int newcur)
{
    int res = D_SYS_OK;

    if (LOGINFO) {
        log_info("\nEnter %s: new or current = %d", __func__,newcur);
    }

    /* T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQのパラメータをセット */
    /* どのパラメータを詰めるかflgで判定 */

    /* user */
    if(newcur == 0){
        strcpy((char *)msg_data->user, (char *)new_user_data[user_num].user);
    }else{
        strcpy((char *)msg_data->user, (char *)current_user_data[user_num].user);
    }

    /* password */
    /* どのパラメータを詰めるかflgで判定 */
    /* 詰めない場合はNULLを指定 */
    if(flg == SAA_USERMGMT_USER_PASSWPORD_FLG || flg == SAA_USERMGMT_USER_CHANGE_OK_FLG){
        if(newcur == 0){
            strcpy((char *)msg_data->password, (char *)new_user_data[user_num].password);
        }else{
            strcpy((char *)msg_data->password, (char *)current_user_data[user_num].password);
        }
    }
    else{
        msg_data->password[0] = '\0';
    }

    /* enabled */
    /* どのパラメータを詰めるかflgで判定 */
    /* 詰めない場合はF詰めを指定 */
    if(flg == SAA_USERMGMT_USER_ENABLED_FLG || flg == SAA_USERMGMT_USER_CHANGE_OK_FLG){
        if(newcur == 0){
            if(new_user_data[user_num].enabled <= SAA_USERMGMT_ENABLED_TRUE){
                msg_data->enabled = new_user_data[user_num].enabled;
            }else{
                msg_data->enabled = SAA_USERMGMT_ENABLED_NON;
            }
        }else{
            if(current_user_data[user_num].enabled <= SAA_USERMGMT_ENABLED_TRUE){
                msg_data->enabled = current_user_data[user_num].enabled;
            }else{
                msg_data->enabled = SAA_USERMGMT_ENABLED_NON;
            }
        }
    }
    else
    {
        msg_data->enabled = SAA_USERMGMT_ENABLED_NON;
    }
    if (LOGINFO) {
        log_info("\n%s: setted MSI data, user = %s, enabled = %d.",
            __func__, msg_data->user, msg_data->enabled );
    }

    return res;

} /* i_o_ran_usermgmt_set_msi_data */

/*!
 * @brief   MSI送信を行う
 * @brief   CFM受信した場合はフラグの更新を行う
 * @note    
 * @param   [in] T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ *msg_data
 * @param   [in] UINT flg
 * @param   [in] UINT *user_num
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/02/14 KCN)wakamizu
 */
status_t i_o_ran_usermgmt_send_edit_msi(
    T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ *msg_data,
    UINT user_num,
    UINT flg)
{
    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REJ read_data;   /* NG応答 */
    int rtn_msi = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }


    /* read_data初期化 */
    memset(&read_data, 0 , sizeof(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REJ));

    /* MSI送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_USERMGMT_EDIT_REQ, sizeof(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REQ), (void*)msg_data);
    if(response_id == 0) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSIレスポンス待ち */
    rtn_msi = f_saa_com_get_response(response_id,
        sizeof(T_MPSW_MSI_O_RAN_USERMGMT_EDIT_REJ),
        &message_id,
        &read_size,
        (void *)&read_data,
        D_MSG_WAIT_TIME );


    /* MSIレスポンス判定 */
    switch (rtn_msi) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_USERMGMT_EDIT_CFM) {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x)", __func__, message_id);
            }
            return ERR_NCX_OPERATION_FAILED;
        }
        else {
            /* 正常応答受信 */
            /* flg値の更新 */
            /* OR演算 */
            table_commit_flg[user_num] = table_commit_flg[user_num] | flg;
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: too small buffer size.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
        break;
    case D_MSG_BUFFER_GET_ERR_TIMEOUT:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: time out.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
        break;
    case D_MSG_BUFFER_GET_ERR_OTHER:
    default:
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: other.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_usermgmt_send_edit_msi */

/*!
 * @brief   edit処理を呼んでDatastoreの更新を行う
 * @brief   new_user_data[]を更新に投げる
 * @note    
 * @param   [in] user_max
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/12  create KCN)wakamizu
 * @date    2019/03/22  change KCN)nanjo
 */
void i_o_ran_usermgmt_send_edit_config(UINT user_max)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* user未登録の場合は無処理 */
    if(user_max == 0)
    {
        if (LOGINFO) {
            log_info("\n user No acount.");
        }
        return;
    }

    /* send-data作成 */
    char value_buff[D_O_RAN_USERMGMT_DB_SEND_MAX_LEN] = "";
    char value_buff_tmp[D_O_RAN_USERMGMT_DB_SEND_MAX_LEN] = "";
    sprintf(value_buff_tmp,
        "/opt/agent/bin/DU_dbapi "
        "subsys_usermng "
        "'<config>"
            "<users xmlns=\"urn:o-ran:user-mgmt:1.0\">");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    for(UINT i=0; i<user_max; i++)
    {
        /* name */
        sprintf(value_buff_tmp,
                "<user>"
                "<name>%s</name>",
            new_user_data[i].user);
        strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

        /* passward */
        if(new_user_data[i].password[0] != '\0')
        {
            sprintf(value_buff_tmp,
                "<password>%s</password>",
                new_user_data[i].password);
            strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));
        }

        /* enabled */
        if(new_user_data[i].enabled == 1)
        {
            sprintf(value_buff_tmp, "<enabled>true</enabled>"
                                    "</user>");
            strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));
        }
        else
        {
            sprintf(value_buff_tmp, "<enabled>false</enabled>"
                                    "</user>");
            strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));
        }
    }

    sprintf(value_buff_tmp,
            "</users>"
        "</config>'");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    sprintf(value_buff_tmp, " --log-level=off 1>/dev/null 2>/dev/null &");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    if (LOGINFO) {
        log_info("\n value_buff : %s", value_buff);
    }

    system(value_buff);

    return;
} /* i_o_ran_usermgmt_send_edit_config */


/*!
 * @brief   管理テーブルの先頭ユーザのpasswordとcurrentPasswordを検証
 * @brief   newPasswordとnewPasswordConfirmを検証
 * @brief   statusおよびstatus_messageに値を詰める
 * @brief   管理テーブルの更新
 * @note    
 * @param   [in] curval
 * @param   [in] newval
 * @param   [in] newcomval
 * @param   [in] status_buff
 * @param   [in] status_message_buff
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/15 KCN)wakamizu
 */
status_t i_o_ran_usermgmt_chg_password_invoke(
    const xmlChar *curval,
    const xmlChar *newval,
    const xmlChar *newconval,
    xmlChar *status_buff,
    xmlChar *status_message_buff)
{
    status_t res = NO_ERR;
    int      rtn_saa = D_SYS_OK;
    UINT     user_num = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* rpc送信ユーザ名を取得 */
    const xmlChar *user = sil_sa_get_username();
    if ((user == NULL) || (*user == '\0'))
    {
        /* statusおよびstatus_messageに値を詰める */
        xml_strcpy(status_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_FLT);
        xml_strcpy(status_message_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_MSG_FLT_USER_NOTFOUND);
        return res;
    }
    else
    {
        log_debug("\n%s: username:%s", __func__, user);
    }

    /* new_user_dataからname検索 */
    rtn_saa = i_o_ran_usermgmt_check_user_num(user, &user_num);
    if(rtn_saa == D_SYS_NG)
    {
        if (LOGWARN) {
            log_warn("\n%s: Failed to get name from new_user_data, name:%s", __func__, user);
        }
        /* statusおよびstatus_messageに値を詰める */
        xml_strcpy(status_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_FLT);
        xml_strcpy(status_message_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_MSG_FLT_USER_NOTFOUND);
        return res;
    }

    /* 管理テーブルの先頭ユーザのpasswordとcurrentPasswordを検証 */
    if(xml_strcmp(curval, (const xmlChar *)current_user_data[user_num].password) != 0){
        if(LOGWARN) {
            log_warn("\n%s: Operation Failed. current password do not match.", __func__);
        }
        /* statusおよびstatus_messageに値を詰める */
        xml_strcpy(status_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_FLT);
        xml_strcpy(status_message_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_MSG_FLT_CURPASS_MISMATCH);
        return res;
    }

    /* newPasswordとnewPasswordConfirmを検証 */
    if(xml_strcmp(newval, newconval) != 0){
        if(LOGWARN) {
            log_warn("\n%s: Operation Failed. new password do not match.", __func__);
        }
        /* statusおよびstatus_messageに値を詰める */
        xml_strcpy(status_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_FLT);
        xml_strcpy(status_message_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_MSG_FLT_NEWPASS_MISMATCH);
        return res;
    }

    /* 管理テーブルの更新 */
    strcpy(new_user_data[user_num].password, (const char *)newval);

    /* MSI送信 */
    res = i_o_ran_usermgmt_o_ran_user_profile_user_commit((const xmlChar *)current_user_data[user_num].user, SAA_USERMGMT_USER_PASSWPORD_FLG);
    if(res != NO_ERR){
        if(LOGWARN) {
            log_warn("\n%s: Operation Failed. MSI fail.", __func__);
        }
        /* statusおよびstatus_messageに値を詰める */
        xml_strcpy(status_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_FLT);
        xml_strcpy(status_message_buff, (const xmlChar*)SAA_ORG_USER_MGMT_STATUS_MSG_FLT_OTHER);
        return res;
    }

    /* edit投げる */
    i_o_ran_usermgmt_send_edit_config_chg_password(user_num);

    return res;
}


/*!
 * @brief   chg_passwordで変更されたpasswordをDatastoreに更新する
 * @brief   new_user_data[]を更新に投げる
 * @note    
 * @param   [in] user_max
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/06  create KCN)nanjo
 */
void i_o_ran_usermgmt_send_edit_config_chg_password(UINT user_num)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* send-data作成 */
    char value_buff[D_O_RAN_USERMGMT_DB_SEND_MAX_LEN] = "";
    char value_buff_tmp[D_O_RAN_USERMGMT_DB_SEND_MAX_LEN] = "";
    sprintf(value_buff_tmp,
        "/opt/agent/bin/DU_dbapi "
        "subsys_usermng "
        "'<config>"
            "<users xmlns=\"urn:o-ran:user-mgmt:1.0\">"
                "<user>");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    /* name */
    sprintf(value_buff_tmp,
            "<name>%s</name>",
        new_user_data[user_num].user);
    strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    /* passward */
    if(new_user_data[user_num].password[0] != '\0')
    {
        sprintf(value_buff_tmp,
            "<password>%s</password>",
            new_user_data[user_num].password);
        strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));
    }


    sprintf(value_buff_tmp,
                "</user>"
            "</users>"
        "</config>'");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    sprintf(value_buff_tmp, " --log-level=off 1>/dev/null 2>/dev/null &");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_USERMGMT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    if (LOGINFO) {
        log_info("\n value_buff : %s", value_buff);
    }

    system(value_buff);

    return;
} /* i_o_ran_usermgmt_send_edit_config_chg_password */

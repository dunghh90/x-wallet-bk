/*
 * @file    i_o-ran-performance-management.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_PERFORMANCE_MANAGEMENT
 * @brief       SAA o-ran-performance-management
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "u_o-ran-performance-management.h"
#include "i_o-ran-performance-management.h"
#include "i_ietf-hardware.h"
#include "i_o-ran-processing-element.h"

#include "f_mpsw_msg_o-ran_performance_management_def.h"
#include "f_mpsw_msg_o-ran_performance_management_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
/********************************************************************************************************************/
/*   valuable                                                                                                       */
/********************************************************************************************************************/
UINT tr_ru_data[SAA_PFM_MNG_MAX_OBJ];                   /* RU/TRANSPORT判別用テーブル */
pthread_mutex_t mutex_for_tr_ru_data = PTHREAD_MUTEX_INITIALIZER;

static T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ rx_edit_data;   /* 管理テーブル */
static UINT chg_obj_flg[SAA_PFM_MNG_MAX_OBJ];           /* 同時送信object数 */
static UINT16 tmp_rx_mea_interval;                      /* 測定中更新テーブル 測定間隔 測定停止時に反映&クリア */
static UINT16 tmp_ntf_interval;                         /* 測定中更新テーブル 送信間隔 測定停止時に反映&クリア */
static int  measure_flg;                                /* all deactive = 0  even one active= 1 */
static T_MPSW_MSI_O_RAN_PFM_MNG_NTF_IND m_notification_data;

const char object_name[SAA_PFM_MNG_MAX_OBJ][SAA_PFM_MNG_MAX_LEN] = {
   "RX_ON_TIME",
   "RX_EARLY",
   "RX_LATE",
   "RX_CORRUPT",
   "RX_DUPL",
   "RX_TOTAL"
};
/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/*!
 * @brief   管理テーブルの初期化を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 * @date    2019/02/27 KCN)wakamizu
 */
status_t i_o_ran_pfm_mng_init(void)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* 管理テーブル初期化 */
    memset(&rx_edit_data, 0, sizeof(T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ));
    /* measure_obj設定(固定値) */
    rx_edit_data.rx_window_measure_t[0].measure_obj = E_MPSW_PFM_RXWIN_OBJ_RX_ON_TIME;
    rx_edit_data.rx_window_measure_t[1].measure_obj = E_MPSW_PFM_RXWIN_OBJ_RX_EARLY;
    rx_edit_data.rx_window_measure_t[2].measure_obj = E_MPSW_PFM_RXWIN_OBJ_RX_LATE;
    rx_edit_data.rx_window_measure_t[3].measure_obj = E_MPSW_PFM_RXWIN_OBJ_RX_CORRUPT;
    rx_edit_data.rx_window_measure_t[4].measure_obj = E_MPSW_PFM_RXWIN_OBJ_RX_DUPL;
    rx_edit_data.rx_window_measure_t[5].measure_obj = E_MPSW_PFM_RXWIN_OBJ_RX_TOTAL;

    /* lock */
    pthread_mutex_lock(&mutex_for_tr_ru_data);

    for(UINT i=0; i<SAA_PFM_MNG_MAX_OBJ; i++){
        chg_obj_flg[i]  = SAA_PFM_MNG_MEA_DEACTICVE;
        tr_ru_data[i]   = SAA_PFM_MNG_MEA_NOSET;
    }

    /* unlock */
    pthread_mutex_unlock(&mutex_for_tr_ru_data);

    tmp_rx_mea_interval = SAA_PFM_MNG_MEA_NOSET;
    tmp_ntf_interval    = SAA_PFM_MNG_MEA_NOSET;
    measure_flg         = SAA_PFM_MNG_MEA_DEACTICVE;

    return res;

} /* i_o_ran_pfm_mng_init */

/*!
 * @brief   active/deactive変化確認
 * @note    変化あればsend_obj_numをインクリメント
 *          
 * @param   [in] name
 * @param   [in] newval_val
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/27 KCN)wakamizu
 */
status_t i_pfm_mng_rx_window_active_validate(
    const xmlChar *name,
    bool newval_val)
{
    status_t res = NO_ERR;
    UINT   obj_num = 0;
    UINT16 act_flg = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:name = %s .", __func__, name);
    }

    /* object名からobject番号を特定 */
    res =i_pfm_mng_rx_window_active_check(name, &obj_num, &act_flg);
    if(res != NO_ERR){
        if(LOGWARN){
            log_warn("\n%s: active_check is fail.",__func__);
        }
        return res;
    }

    /* 差分があればchg_obj_flgをアクティブにする */
    if(act_flg != (int)newval_val){
        chg_obj_flg[obj_num] = SAA_PFM_MNG_MEA_ACTIVE;
    }

    return res;

} /* i_pfm_mng_rx_window_active_validate */

/*!
 * @brief   object名からactive/deactiveをチェック
 * @note    返り値でactive/deactiveを返す
 *          obj_numに配列番号を入れて返す
 * @param   [in] k_o_ran_user_profile_user_name
 * @param   [in] newval_val
 * @param   [in] flg
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/27 KCN)wakamizu
 */
status_t i_pfm_mng_rx_window_active_check(
    const xmlChar *name, 
    UINT *obj_num,
    UINT16 *act_flg)
{
    status_t res = NO_ERR;
    UINT object_cnt = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:name = %s.", __func__, name);
    }

    /* ユーザ名からユーザ番号を特定 */
    for(object_cnt = 0; object_cnt < SAA_PFM_MNG_MAX_OBJ; object_cnt++){
        if(!xml_strcmp((const xmlChar *)object_name[object_cnt], name)){
            *obj_num = object_cnt;
            *act_flg = (int)rx_edit_data.rx_window_measure_t[object_cnt].act_flag;
            break;
        }
    }
    if(object_cnt == SAA_PFM_MNG_MAX_OBJ){
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN){
            log_warn("\n%s: Object name is not found. name = %s.",__func__, name);
        }
    }

    return res;
} /* i_pfm_mng_rx_window_active_check */

/*!
 * @brief   管理テーブルの更新
 * @note    
 * @param   [in] name
 * @param   [in] flg
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/02/28 KCN)wakamizu
 */
status_t i_pfm_mng_rx_window_edit(
    const xmlChar *name,
    UINT16 newval_val, 
    bool   newval_bool,
    UINT flg)
{
    status_t res = NO_ERR;
    UINT   obj_num = 0;
    UINT16 act_flg = 0;

    if (LOGINFO) {
        log_info("\nEnter %s.", __func__);
    }

    /* flgを確認し、対応したパラメータを登録する     */
    /* rx_measurement_interval                       */
    /* 管理テーブルに更新する時にはtmpも更新する     */
    /* tmpに値がない場合は管理テーブルからコピーする */
    if(flg == SAA_PFM_MNG_RX_MEA_INTERVAL_FLG ){
        if(measure_flg == SAA_PFM_MNG_MEA_DEACTICVE){
            rx_edit_data.mea_interval = newval_val;
            tmp_rx_mea_interval = newval_val;
            if (LOGDEV1) {
                log_dev1("\n%s: mea_interval =%d.", __func__, rx_edit_data.mea_interval);
            }
        }
        else if (measure_flg == SAA_PFM_MNG_MEA_ACTIVE){
            tmp_rx_mea_interval = newval_val;
            if (LOGDEV1) {
                log_dev1("\n%s: tmp_rx_mea_interval =%d.", __func__, tmp_rx_mea_interval);
            }
        }
        else{
            if(LOGWARN){
                log_warn("\n%s: measure_flg error. measure_flg = %d.",__func__, measure_flg);
            }
        }
    }
    /* notification_interval                         */
    /* 管理テーブルに更新する時にはtmpも更新する     */
    /* tmpに値がない場合は管理テーブルからコピーする */
    if(flg == SAA_PFM_MNG_NTF_INTERVAL_FLG ){
        if(measure_flg == SAA_PFM_MNG_MEA_DEACTICVE){
            rx_edit_data.ntf_interval = newval_val;
            tmp_ntf_interval = newval_val;
            if (LOGDEV1) {
                log_dev1("\n%s: ntf_interval =%d.", __func__, rx_edit_data.ntf_interval);
            }
        }
        else if (measure_flg == SAA_PFM_MNG_MEA_ACTIVE){
            tmp_ntf_interval = newval_val;
            if (LOGDEV1) {
                log_dev1("\n%s: tmp_ntf_interval =%d.", __func__, tmp_ntf_interval);
            }
        }
        else{
            if(LOGWARN){
                log_warn("\n%s: measure_flg error. measure_flg = %d.",__func__, measure_flg);
            }
        }
    }

    /* object名からテーブル番号を取得 */
    /* act/deact RU/TRAN */
    if(flg == SAA_PFM_MNG_RX_ACT_FLG || flg == SAA_PFM_MNG_RU_TRS_FLG){
        res = i_pfm_mng_rx_window_active_check(name, &obj_num, &act_flg);
        if(res != NO_ERR){
            if(LOGWARN){
                log_warn("\n%s: active_check is fail.",__func__);
            }
            return res;
        }
    }

    /* active/deactive */
    if(flg == SAA_PFM_MNG_RX_ACT_FLG ){
        /* active trueが設定された際、measure-interval未設定の場合はNGを返す */
        if((rx_edit_data.mea_interval == 0) && (newval_bool == true)){
            if(LOGWARN){
                log_warn("\n%s: mea_interval is not set yet .", __func__);
            }
            chg_obj_flg[obj_num] = SAA_PFM_MNG_MEA_DEACTICVE;
            res = ERR_NCX_OPERATION_FAILED;
            return res;
        }

        rx_edit_data.rx_window_measure_t[obj_num].act_flag = (UINT)newval_bool;
        if (LOGDEV1) {
            log_dev1("\n%s: act_flag =%d.", __func__, rx_edit_data.rx_window_measure_t[obj_num].act_flag);
        }
    }

    /* RU/TRANCPORT */
    if(flg == SAA_PFM_MNG_RU_TRS_FLG ){

        /* lock */
        pthread_mutex_lock(&mutex_for_tr_ru_data);

        tr_ru_data[obj_num] = newval_val;
        if (LOGDEV1) {
            log_dev1("\n%s: tr_ru_data[%u] =%u.", __func__, obj_num, tr_ru_data[obj_num]);
        }

        /* unlock */
        pthread_mutex_unlock(&mutex_for_tr_ru_data);
    }

    return res;
} /* i_pfm_mng_rx_window_edit */

/*!
 * @brief   act_flagのcommit
 * @note    管理テーブルからMSIセット
 *          MSI送信関数呼ぶ
 *          measure_flgを判定
 *          
 * @param   [in] k_o_ran_user_profile_user_name
 * @param   [in] flg
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/02/14 KCN)wakamizu
 */
status_t i_pfm_mng_rx_window_act_flag_commit(void)
{
    status_t res = NO_ERR;
    UINT16 send_num = 0;
    T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ msg_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* msi初期化 */
    memset(&msg_data, 0 , sizeof(T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ));

    /* msiデータ設定 */
    /* rx_window_measure_tは配列前詰めで変更のあったものだけ送信する */
    for(UINT i = 0; i<SAA_PFM_MNG_MAX_OBJ; i++){
        if(chg_obj_flg[i] == SAA_PFM_MNG_MEA_ACTIVE){
            msg_data.rx_window_measure_t[send_num].measure_obj = rx_edit_data.rx_window_measure_t[i].measure_obj;
            msg_data.rx_window_measure_t[send_num].act_flag = rx_edit_data.rx_window_measure_t[i].act_flag;
            /* フラグ更新 */
            chg_obj_flg[i] = SAA_PFM_MNG_MEA_DEACTICVE;
            send_num++;
        }
    }
    if(send_num == 0){
        if(LOGDEV1){
            log_dev1("\n%s: active_flag is not changed. ",__func__);
        }
        return res;
    }
    /* 共通部分データ設定 */
    msg_data.mea_interval = rx_edit_data.mea_interval;
    msg_data.ntf_interval = rx_edit_data.ntf_interval;
    msg_data.measure_t_num = send_num;

    /* ログ出力 */
    if(LOGDEV1){
        log_dev1("\n%s: T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ",__func__);
        log_dev1("\n%s:     measure_t_num : %u",__func__, msg_data.measure_t_num);
        for(UINT i = 0; i<msg_data.measure_t_num; i++)
        {
            log_dev1("\n%s:     measure_obj[%u] : %u",__func__, i, msg_data.rx_window_measure_t[i].measure_obj);
            log_dev1("\n%s:     act_flag[%u]    : %u",__func__, i, msg_data.rx_window_measure_t[i].act_flag);
        }
        log_dev1("\n%s:     mea_interval   : %u",__func__, msg_data.mea_interval);
        log_dev1("\n%s:     ntf_interval   : %u",__func__, msg_data.ntf_interval);
    }

    /* MSI送信 */
    res = i_pfm_mng_rx_window_send_edit_msi(&msg_data);
    if(res != NO_ERR ){
        if(LOGWARN){
            log_warn("\n%s: send edit_msi failed.",__func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }
    /* flg値の更新 */
    /* msi正常送受信後にテーブル値からflgセット */
    measure_flg = SAA_PFM_MNG_MEA_DEACTICVE;
    for(UINT j=0; j<SAA_PFM_MNG_MAX_OBJ; j++){
        if(msg_data.rx_window_measure_t[j].act_flag == SAA_PFM_MNG_MEA_ACTIVE){
            measure_flg = SAA_PFM_MNG_MEA_ACTIVE;
        }
    }

    /* all deactiveの場合 */
    /* tmp_intervalを管理テーブルに反映する */
    if(measure_flg == SAA_PFM_MNG_MEA_DEACTICVE){
        rx_edit_data.mea_interval = (UINT16)tmp_rx_mea_interval;
        rx_edit_data.ntf_interval = (UINT16)tmp_ntf_interval;
    }

    return res;
} /* i_pfm_mng_rx_window_act_flag_commit */

/*!
 * @brief   EDIT_MSI送信を行う
 * @brief   CFM受信した場合はフラグの更新を行う
 * @note    
 * @param   [in] T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ *msg_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/02/14 KCN)wakamizu
 */
status_t i_pfm_mng_rx_window_send_edit_msi(
    T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ *msg_data)
{
    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MSG_MSI_O_RAN_PFM_MNG_EDIT_REJ read_data; /* NG応答 */
    int rtn_msi = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* msi初期化 */
    memset(&read_data, 0 , sizeof(T_MSG_MSI_O_RAN_PFM_MNG_EDIT_REJ));

    /* MSI送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_PFM_MNG_EDIT_REQ, sizeof(T_MPSW_MSI_O_RAN_PFM_MNG_EDIT_REQ), (void*)msg_data);
    if(response_id == 0) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* MSIレスポンス待ち */
    rtn_msi = f_saa_com_get_response(response_id,
        sizeof(T_MSG_MSI_O_RAN_PFM_MNG_EDIT_REJ),
        &message_id,
        &read_size,
        (void *)&read_data,
        D_MSG_WAIT_TIME );

    /* MSIレスポンス判定 */
    switch (rtn_msi) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_PFM_MNG_EDIT_CFM) {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x)", __func__, message_id);
            }
            return ERR_NCX_OPERATION_FAILED;
        }
        else {
            /* 正常応答受信 */
            if (LOGDEV1) {
                 log_dev1("\n%s: edit_msi send REQ recieve CFM OK.", __func__);
            }
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

} /* i_pfm_mng_rx_window_send_edit_msi */

/*!
 * @brief   GET_MSI送信を行う
 * @note    
 * @param   [in] T_MPSW_MSI_O_RAN_PFM_MNG_GET_CFM *get_data
 * @param   [in] UINT flg
 * @param   [in] UINT *user_num
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/02/14 KCN)wakamizu
 */
status_t i_pfm_mng_rx_window_send_get_msi(
    void *get_data)
{
    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    int rtn_msi = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_PFM_MNG_GET_REQ, 0, NULL);
    if(response_id == 0) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        res = ERR_NCX_OPERATION_FAILED;
        return res;
    }

    /* MSIレスポンス待ち */
    rtn_msi = f_saa_com_get_response(response_id,
        max(sizeof(T_MPSW_MSI_O_RAN_PFM_MNG_GET_CFM), sizeof(T_MPSW_MSI_O_RAN_PFM_MNG_GET_REJ)),
        &message_id,
        &read_size,
        get_data,
        D_MSG_WAIT_TIME );

    /* MSIレスポンス判定 */
    switch (rtn_msi) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_PFM_MNG_GET_CFM) {
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x)", __func__, message_id);
            }
        }
        else {
            /* 正常応答受信 */
            if (LOGDEV1) {
                 log_dev1("\n%s: get_msi send REQ recieve CFM OK.", __func__);
            }
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: too small buffer size.", __func__);
        }
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

} /* i_pfm_mng_rx_window_send_get_msi */

/*!
 * @brief   他モジュール関数アクセス
 * @note    他関数から内部テーブルの値を持ってくる
 * @param   [in] const char name
 * @param   [in] xmlChar *param
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/03/01 KCN)wakamizu
 */
status_t i_pfm_mng_get_other_module(
    UINT name,
    xmlChar *param)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* name判定 */
    switch(name){
    case SAA_PFM_MNG_HARD_MOD_NAME:
        i_ietf_hardware_name_give(param);
        if(LOGDEV1){
            log_dev1("\n%s: get  name = %d, param = %s .",__func__, SAA_PFM_MNG_HARD_MOD_NAME, param);
        }
        if(param[0] == '\0')
        {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: hardware-name is fail.", __func__);
            }
        }
        break;
    case SAA_PFM_MNG_PROELE_MOD_NAME0:
        i_o_ran_processing_element_processing_elements_ru_elements_name_give(SAA_PFM_MNG_PROELE_MOD_NAME0-1, param);
        if(LOGDEV1){
            log_dev1("\n%s: get  name = %d, param = %s .",__func__, SAA_PFM_MNG_PROELE_MOD_NAME0, param);
        }
        if(param[0] == '\0')
        {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGDEV1) {
                log_dev1("\n%s: processing-name0 is fail.", __func__);
            }
        }
        break;
    case SAA_PFM_MNG_PROELE_MOD_NAME1:
        i_o_ran_processing_element_processing_elements_ru_elements_name_give(SAA_PFM_MNG_PROELE_MOD_NAME1-1, param);
        if(LOGDEV1){
            log_dev1("\n%s: get  name = %d, param = %s .",__func__, SAA_PFM_MNG_PROELE_MOD_NAME1, param);
        }
        if(param[0] == '\0')
        {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGDEV1) {
                log_dev1("\n%s: processing-name1 is fail.", __func__);
            }
        }
        break;
    default:
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s wrong name. name=%d .", __func__, name);
        }
    }
    return res;

} /* i_pfm_mng_get_other_module */

/*!
 * @brief   他モジュール関数アクセス
 * @note    eth_nameの登録数を確認する
 * @param   [in]
 * @param   [in]
 * @return  [正常]num
 *          [異常]D_SYS_NG
 * @date    2019/03/01 KCN)wakamizu
 */
int i_pfm_mng_get_module_num(void)
{
    int num = D_SYS_OK;
    xmlChar param0[D_PARAM_NUM_MAX] = "";
    xmlChar param1[D_PARAM_NUM_MAX] = "";
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }
    /* port0確認 */
    i_o_ran_processing_element_processing_elements_ru_elements_name_give(SAA_PFM_MNG_PROELE_MOD_NAME0-1, param0);
    if(LOGDEV1){
        log_dev1("\n%s: get  name = %d, param = %s .",__func__, SAA_PFM_MNG_PROELE_MOD_NAME0, param0);
    }
    if(param0[0] != '\0')
    {
        num++;
        /* port1確認 */
        i_o_ran_processing_element_processing_elements_ru_elements_name_give(SAA_PFM_MNG_PROELE_MOD_NAME1-1, param1);
        if(LOGDEV1){
            log_dev1("\n%s: get  name = %d, param = %s .",__func__, SAA_PFM_MNG_PROELE_MOD_NAME1, param1);
        }
        if(param1[0] != '\0')
        {
            num++;
        }
    }
    if (LOGDEV1) {
        log_dev1("\nEnter %s: eth_num = %d .", __func__, num);
    }
    return num;

} /* i_pfm_mng_get_other_module */

/*!
 * @brief   notificationを送信する
 * @note    
 * @param   [in] data
 * @param   [in] size
 * @return  void
 * @date    2019/03/01 KCN)wakamizu
 */
void i_o_ran_performance_management_measurement_result_stats_handler(
    void *data,
    uint32_t size)
{
    if (LOGINFO) {
        log_info("\nEnter %s: size=%d", __func__, size);
    }

    memcpy(&m_notification_data, data, sizeof(m_notification_data));
    u_o_ran_performance_management_measurement_result_stats_send(NULL, NULL);

} /* i_o_ran_performance_management_measurement_result_stats_handler */

/*!
 * @brief   notificationデータを取得する。
 * @note    
 * @param   [in] data
 * @param   [in] size
 * @return  void
 * @date    2019/12/23 KCN)nakai
 */
T_MPSW_MSI_O_RAN_PFM_MNG_NTF_IND *i_o_ran_performance_management_get_notification_data(void){

    return &m_notification_data;
}

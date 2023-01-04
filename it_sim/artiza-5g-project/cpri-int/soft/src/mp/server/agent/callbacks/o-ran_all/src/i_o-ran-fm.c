/*
 * @file    i_o-ran-fm.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_FM
 * @brief       SAA o-ran-fm
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <pthread.h>

#include "agt_util.h"
#include "u_o-ran-fm.h"


#include "f_mpsw_msg_o-ran_fm_def.h"
#include "f_mpsw_msg_o-ran_fm_typ.h"
#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_ntf_lib.h"
#include "f_saa_utc_lib.h"

#include "i_o-ran-uplane-conf.h"



/********************************************************************************************************************/
#include "i_o-ran-fm.h"


/********************************************************************************************************************/
/*   valuable                                                                                                       */
/********************************************************************************************************************/
static char i_mod_fm[]                             = i_mod_o_ran_fm;
static char i_para_fm_fault_source_llsdu[]         = i_para_o_ran_fm_fault_source_llsdu;
static char i_para_fm_fault_source_port0[]         = i_para_o_ran_fm_fault_source_port0;
static char i_para_fm_fault_source_port1[]         = i_para_o_ran_fm_fault_source_port1;
static char i_para_fm_fault_source_fan1[]          = i_para_o_ran_fm_fault_source_fan1;
static char i_para_fm_fault_source_fan2[]          = i_para_o_ran_fm_fault_source_fan2;
static char i_para_fm_fault_source_fan3[]          = i_para_o_ran_fm_fault_source_fan3;
static char i_para_fm_fault_source_external[]      = i_para_o_ran_fm_fault_source_external;
static char i_para_fm_fault_severity_critical[]    = i_para_o_ran_fm_severity_critical;
static char i_para_fm_fault_severity_major[]       = i_para_o_ran_fm_severity_major;
static char i_para_fm_fault_severity_minor[]       = i_para_o_ran_fm_severity_minor;

static alarm_info_t    saa_fm_alarm_info_tbl[SAA_FM_MAX_ALARM_NUM];
static alarm_info_t    m_alarm_info_for_notification;

static xmlChar *saa_fm_llsdu;
static xmlChar *saa_fm_port0;
static xmlChar *saa_fm_port1;
static xmlChar *saa_fm_fan1;
static xmlChar *saa_fm_fan2;
static xmlChar *saa_fm_fan3;
static xmlChar *saa_fm_external;

static pthread_mutex_t mutex_for_fm_alarm_info_tbl = PTHREAD_MUTEX_INITIALIZER;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   管理テーブルの初期化を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/03 KCN)吉木
 */
status_t i_o_ran_fm_init(void)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* 初期パラメータ取得 */
    saa_fm_llsdu = (xmlChar *)f_saa_com_get_init_data(i_mod_fm, i_para_fm_fault_source_llsdu);
    saa_fm_port0 = (xmlChar *)f_saa_com_get_init_data(i_mod_fm, i_para_fm_fault_source_port0);
    saa_fm_port1 = (xmlChar *)f_saa_com_get_init_data(i_mod_fm, i_para_fm_fault_source_port1);
    saa_fm_fan1 = (xmlChar *)f_saa_com_get_init_data(i_mod_fm, i_para_fm_fault_source_fan1);
    saa_fm_fan2 = (xmlChar *)f_saa_com_get_init_data(i_mod_fm, i_para_fm_fault_source_fan2);
    saa_fm_fan3 = (xmlChar *)f_saa_com_get_init_data(i_mod_fm, i_para_fm_fault_source_fan3);
    saa_fm_external = (xmlChar *)f_saa_com_get_init_data(i_mod_fm, i_para_fm_fault_source_external);
    if(saa_fm_llsdu == NULL || saa_fm_port0 == NULL || saa_fm_port1 == NULL
        || saa_fm_fan1 == NULL || saa_fm_fan2 == NULL || saa_fm_fan3 == NULL || saa_fm_external ==NULL) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("\n%s: Init Data Missing.", __func__);
        }
        return res;
    }

    /* saa_fm_alarm_info_tbl初期化 */
    for(uint32 alarm_cnt = 0; alarm_cnt < SAA_FM_MAX_ALARM_NUM; alarm_cnt++) {
        saa_fm_alarm_info_tbl[alarm_cnt].fault_id = SAA_FM_DEF_VAL_FLT_ID;
        saa_fm_alarm_info_tbl[alarm_cnt].fault_source[0] = '\0';
        saa_fm_alarm_info_tbl[alarm_cnt].num_of_affected_obj = 0;
        saa_fm_alarm_info_tbl[alarm_cnt].fault_severity[0] = '\0';
        saa_fm_alarm_info_tbl[alarm_cnt].is_cleared = true;
        saa_fm_alarm_info_tbl[alarm_cnt].fault_text[0] = '\0';
        saa_fm_alarm_info_tbl[alarm_cnt].event_time[0] = '\0';

        for(uint32 aff_obj_cnt = 0; aff_obj_cnt < SAA_FM_MAX_NUM_AFFECTED_OBJ; aff_obj_cnt++) {
            saa_fm_alarm_info_tbl[alarm_cnt].affected_obj[aff_obj_cnt].name[0] = '\0';
        }
    }

    return res;

} /* i_o_ran_fm_init */

/*!
 * @brief   alarm notificationを送信する
 * @note    
 * @param   [in] data
 * @param   [in] size
 * @return  void
 * @date    2019/03/03 KCN)吉木
 */
void i_o_ran_fm_alarm_notif_handler(
    void *data,
    uint32_t size)
{
    time_t event_time;
    struct tm event_tm;
    char event_time_buff[D_PARAM_NUM_MAX];
    int rtn_saa;
    alarm_info_t alarm_info_tmp;
    uint32 alarm_cnt_tmp = 0;

    if (LOGINFO) {
        log_info("\nEnter %s: size=%d", __func__, size);
    }

    /* size確認 */
    if(sizeof(T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND) > size) {
        if(LOGWARN) {
            log_warn("\n%s: invalid data size, alarm notification MSI = %d", __func__, size);
        }
        return;
    }

    /* 時刻取得 */
    event_time = time(&event_time);
    if(event_time == -1) {
        if(LOGWARN) {
            log_warn("\n%s: time() is failed.", __func__);
        }
        return;
    }
    localtime_r(&event_time, &event_tm);
    
    /* yyyy-mm-ddThh:mm:ss.0000+00:00 */
    rtn_saa = f_saa_com_utc_to_offset_string(
        (uint16_t)(event_tm.tm_year + SAA_FM_TIME_YEAR_OFFSET),
        (uint8_t)(event_tm.tm_mon + SAA_FM_TIME_MON_OFFSET),
        (uint8_t)event_tm.tm_mday,
        (uint8_t)event_tm.tm_hour,
        (uint8_t)event_tm.tm_min,
        (uint8_t)event_tm.tm_sec,
        event_time_buff);
    if(rtn_saa != D_SYS_OK) {
        if(LOGWARN) {
            log_warn("\n%s: f_saa_com_utc_to_offset_string() is Failed. leaf is event-time.", __func__);
        }
        return;
    }

    /* alarm_info_tmp初期化 */
    alarm_info_tmp.fault_id = SAA_FM_DEF_VAL_FLT_ID;
    alarm_info_tmp.fault_source[0] = '\0';
    alarm_info_tmp.num_of_affected_obj = 0;
    alarm_info_tmp.fault_severity[0] = '\0';
    alarm_info_tmp.is_cleared = true;
    alarm_info_tmp.fault_text[0] = '\0';
    alarm_info_tmp.event_time[0] = '\0';

    for(uint32 aff_obj_cnt = 0; aff_obj_cnt < SAA_FM_MAX_NUM_AFFECTED_OBJ; aff_obj_cnt++) {
        alarm_info_tmp.affected_obj[aff_obj_cnt].name[0] = '\0';
    }

    /* データ変換 */
    rtn_saa = i_o_ran_fm_conv_msi_to_alarm_tbl(data, event_time_buff, &alarm_info_tmp);

    if(rtn_saa != D_SYS_OK) {
        if(LOGWARN) {
            log_warn("\n%s: Conver Failed alarm info from MSI data", __func__);
        }
        return;
    }

    /* mutex lock (mutex_for_fm_alarm_info_tbl) */
    pthread_mutex_lock(&mutex_for_fm_alarm_info_tbl);

    /* 管理テーブル更新 */
    for(uint32 alarm_cnt = 0; alarm_cnt < SAA_FM_MAX_ALARM_NUM; alarm_cnt++) {
        if(saa_fm_alarm_info_tbl[alarm_cnt].fault_id == SAA_FM_DEF_VAL_FLT_ID) {
            alarm_cnt_tmp = alarm_cnt;
            break;
        }
        else if(saa_fm_alarm_info_tbl[alarm_cnt].fault_id == alarm_info_tmp.fault_id) {
            if(!xml_strcmp(saa_fm_alarm_info_tbl[alarm_cnt].fault_source, alarm_info_tmp.fault_source)
                && !xml_strcmp(saa_fm_alarm_info_tbl[alarm_cnt].fault_severity, alarm_info_tmp.fault_severity)) {
                    alarm_cnt_tmp = alarm_cnt;
                    break;
            }
        }
    }
    if(alarm_cnt_tmp == SAA_FM_MAX_ALARM_NUM) {
        if(LOGWARN) {
            log_warn("\n%s: saa_fm_alarm_info_tbl is full", __func__);
        }
        /* mutex unlock (mutex_for_fm_alarm_info_tbl) */
        pthread_mutex_unlock(&mutex_for_fm_alarm_info_tbl);
        return;
    }
    else {
        saa_fm_alarm_info_tbl[alarm_cnt_tmp] = alarm_info_tmp;
    }

    /* mutex unlock (mutex_for_fm_alarm_info_tbl) */
    pthread_mutex_unlock(&mutex_for_fm_alarm_info_tbl);

    /* notification送信 */

    /* 送信データはここで設定する。 */
    m_alarm_info_for_notification = alarm_info_tmp;
    /* パラメータは無意味 */
    u_o_ran_fm_alarm_notif_send(0, NULL, NULL, NULL, false, NULL, NULL);

} /* i_o_ran_fm_alarm_notif_handler */

/*!
 * @brief   MSIデータをalarm_info_t形式に変換する
 * @note    
 * @param   [in] data
 * @param   [in] size
 * @param   [out] alarm_info
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/03/03 KCN)吉木
 */
int i_o_ran_fm_conv_msi_to_alarm_tbl(
    void *data,
    char *event_time_buff,
    alarm_info_t *alarm_info)
{
    int res = D_SYS_OK;

    if (LOGINFO) {
        log_info("\nEnter %s", __func__);
    }

    /* fault-id */
    alarm_info->fault_id = ((T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND *)data)->fault_id;
    if (LOGDEV1) {
        log_dev1("\n   fault_id = %hd ", alarm_info->fault_id);
    }

    /* fault-source */
    res = i_o_ran_fm_conv_fault_source_enum_to_string(
        ((T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND *)data)->fault_source,
        alarm_info->fault_source);
    if(res != D_SYS_OK) {
        return D_SYS_NG;
    }
    if (LOGDEV1) {
        log_dev1("\n   fault_source = %s ", alarm_info->fault_source);
    }

    /* affected-objects */
    alarm_info->num_of_affected_obj = ((T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND *)data)->name_num;
    if (LOGDEV1) {
        log_dev1("\n   num_of_affected_obj = %hd ", alarm_info->num_of_affected_obj);
    }
    /* affected-objectsはmandatory trueのためMSIが空の場合は装置種別をセット */
    if(alarm_info->num_of_affected_obj == 0) {
        alarm_info->num_of_affected_obj = 1;
        res = i_o_ran_fm_conv_fault_source_enum_to_string(
            E_FAULT_SOURCE_LLSDU,
            alarm_info->affected_obj[0].name);
        if(res != D_SYS_OK) {
            return D_SYS_NG;
        }
        if (LOGDEV1) {
            log_dev1("\n   num_of_affected_obj = %d, affected_obj[0] = %s ",
                alarm_info->num_of_affected_obj,
                alarm_info->affected_obj[0].name);
        }
    }
    else {
        for(unsigned char aff_cnt = 0; aff_cnt < alarm_info->num_of_affected_obj; aff_cnt++) {
            res = i_o_ran_fm_conv_fault_source_enum_to_string(
                ((T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND *)data)->affected_objects.name[aff_cnt],
                alarm_info->affected_obj[aff_cnt].name);
            if(res != D_SYS_OK) {
                return D_SYS_NG;
            }
            if (LOGDEV1) {
                log_dev1("\n   affected_obj[%hd] = %s ", aff_cnt, alarm_info->affected_obj[aff_cnt].name);
            }
        }
    }

    /* fault-serverity */
    res = i_o_ran_fm_conv_fault_severity_enum_to_string(
        ((T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND *)data)->fault_severity,
        alarm_info->fault_severity);
    if(res != D_SYS_OK) {
        return D_SYS_NG;
    }
    if (LOGDEV1) {
        log_dev1("\n   fault_severity = %s ", alarm_info->fault_severity);
    }

    /* is-cleared */
    alarm_info->is_cleared = ((T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND *)data)->is_cleared;
    if (LOGDEV1) {
        log_dev1("\n   is_cleared = %d ", alarm_info->is_cleared);
    }

    /* fault-text */
    xml_strcpy(alarm_info->fault_text, (xmlChar *)(((T_MPSW_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND *)data)->fault_text));
    if (LOGDEV1) {
        log_dev1("\n   fault_text = %s ", alarm_info->fault_text);
    }

    /* event-time */
    xml_strcpy(alarm_info->event_time, (xmlChar *)event_time_buff);
    if (LOGDEV1) {
        log_dev1("\n   event_time = %s ", alarm_info->event_time);
    }

    return res;

} /* i_o_ran_fm_conv_msi_to_alarm_tbl */

/*!
 * @brief   fault-sourceをenumからstringに変換する
 * @note    
 * @param   [in] fault_source_enum
 * @param   [out] fault_source_str
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/03/03 KCN)吉木
 */
int i_o_ran_fm_conv_fault_source_enum_to_string(
    E_FAULT_SOURCE fault_source_enum,
    xmlChar * fault_source_str)
{
    int res = D_SYS_OK;

    if (LOGINFO) {
        log_info("\nEnter %s: fault_source_enum=%d", __func__, fault_source_enum);
    }

    switch(fault_source_enum) {
    case E_FAULT_SOURCE_LLSDU:
        xml_strcpy(fault_source_str, saa_fm_llsdu);
        break;
    case E_FAULT_SOURCE_PORT_0:
        xml_strcpy(fault_source_str, saa_fm_port0);
        break;
    case E_FAULT_SOURCE_PORT_1:
        xml_strcpy(fault_source_str, saa_fm_port1);
        break;
    case E_FAULT_SOURCE_TX_ANT_A:
    case E_FAULT_SOURCE_TX_ANT_B:
    case E_FAULT_SOURCE_TX_ANT_C:
    case E_FAULT_SOURCE_TX_ANT_D:
    case E_FAULT_SOURCE_TX_ANT_E:
    case E_FAULT_SOURCE_TX_ANT_F:
    case E_FAULT_SOURCE_TX_ANT_G:
    case E_FAULT_SOURCE_TX_ANT_H:
    case E_FAULT_SOURCE_RX_ANT_A:
    case E_FAULT_SOURCE_RX_ANT_B:
    case E_FAULT_SOURCE_RX_ANT_C:
    case E_FAULT_SOURCE_RX_ANT_D:
    case E_FAULT_SOURCE_RX_ANT_E:
    case E_FAULT_SOURCE_RX_ANT_F:
    case E_FAULT_SOURCE_RX_ANT_G:
    case E_FAULT_SOURCE_RX_ANT_H:
        res = i_o_ran_uplane_conf_get_array_name_from_fm_info(fault_source_enum, fault_source_str);
        if(res != D_SYS_OK) {
            if(LOGWARN) {
                log_warn("\n%s: Convert Failed, fault_source from MSI data to string, fault_source_enum=%d", __func__, fault_source_enum);
            }
        }
        break;
    case E_FAULT_SOURCE_FAN_1:
        xml_strcpy(fault_source_str, saa_fm_fan1);
        break;
    case E_FAULT_SOURCE_FAN_2:
        xml_strcpy(fault_source_str, saa_fm_fan2);
        break;
    case E_FAULT_SOURCE_FAN_3:
        xml_strcpy(fault_source_str, saa_fm_fan3);
        break;
    default:
        xml_strcpy(fault_source_str, saa_fm_external);
        break;
    }

    return res;

} /* i_o_ran_fm_conv_fault_source_enum_to_string */

/*!
 * @brief   fault-severityをenumからstringに変換する
 * @note    
 * @param   [in] fault_severity_enum
 * @param   [out] fault_severity_str
 * @return  [正常]D_SYS_OK
 *          [異常]D_SYS_NG
 * @date    2019/03/03 KCN)吉木
 */
int i_o_ran_fm_conv_fault_severity_enum_to_string(
    E_FAULT_SEVERITY fault_severity_enum,
    xmlChar * fault_severity_str)
{
    int res = D_SYS_OK;

    if (LOGINFO) {
        log_info("\nEnter %s: fault_severity_enum=%d", __func__, fault_severity_enum);
    }

    switch(fault_severity_enum) {
    case E_FAULT_SEVERITY_CRITICAL:
        xml_strcpy(fault_severity_str, (xmlChar *)i_para_fm_fault_severity_critical);
        break;
    case E_FAULT_SEVERITY_MAJOR:
        xml_strcpy(fault_severity_str, (xmlChar *)i_para_fm_fault_severity_major);
        break;
    case E_FAULT_SEVERITY_MINOR:
        xml_strcpy(fault_severity_str, (xmlChar *)i_para_fm_fault_severity_minor);
        break;
    default:
        res = D_SYS_NG;
        if(LOGWARN) {
            log_warn("\n%s: Parameter Error fault_severity_enum=%d", __func__, fault_severity_enum);
        }
        return res;
    }

    return res;
} /* i_o_ran_fm_conv_fault_severity_enum_to_string */

/*!
 * @brief   active-alarmを管理テーブルから取得する
 * @note    
 * @param   [out] alarm_info_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_NO_INSTANCE
 * @date    2019/03/04 KCN)吉木
 */
status_t i_o_ran_fm_get_active_alarms(
    alarm_info_t *alarm_info_temp)
{
    status_t res = ERR_NCX_NO_INSTANCE;
    static uint32 mod_alarm_indx = 0;

    if (LOGDEV1) {
        log_dev1("\nEnter %s: mod_alarm_indx=%d", __func__, mod_alarm_indx);
    }

    /* mutex lock (mutex_for_fm_alarm_info_tbl) */
    pthread_mutex_lock(&mutex_for_fm_alarm_info_tbl);

    while(mod_alarm_indx < SAA_FM_MAX_ALARM_NUM){

        if (LOGDEV1) {
            log_dev1("\n%s: mod_alarm_indx=%d, fault_id=%hd, is_cleared=%d, fault_source=%s, fault_severity=%s",
                    __func__,
                mod_alarm_indx,
                saa_fm_alarm_info_tbl[mod_alarm_indx].fault_id,
                saa_fm_alarm_info_tbl[mod_alarm_indx].is_cleared,
                saa_fm_alarm_info_tbl[mod_alarm_indx].fault_source,
                saa_fm_alarm_info_tbl[mod_alarm_indx].fault_severity);
        }

        if((saa_fm_alarm_info_tbl[mod_alarm_indx].fault_id != SAA_FM_DEF_VAL_FLT_ID)
           && (saa_fm_alarm_info_tbl[mod_alarm_indx].is_cleared == false)) {
            *alarm_info_temp = saa_fm_alarm_info_tbl[mod_alarm_indx];
            res = NO_ERR;
            if(LOGDEV1){
                log_dev1("\n%s: alarm found: mod_alarm_indx=%d", __func__, mod_alarm_indx);
            }
            break;
        }
        mod_alarm_indx++;
    }

    if(res == NO_ERR){
        mod_alarm_indx++;
    }
    else{
        res = ERR_NCX_NO_INSTANCE;
        mod_alarm_indx = 0;
    }

    /* mutex unlock (mutex_for_fm_alarm_info_tbl) */
    pthread_mutex_unlock(&mutex_for_fm_alarm_info_tbl);

    if(LOGDEV1){
        log_dev1("\n%s: returns. res=%d, mod_alarm_indx=%d", __func__, res, mod_alarm_indx);
    }

    return res;

} /* i_o_ran_fm_get_active_alarms */

/*!
 * @brief   affected-objectを管理テーブルから取得する
 * @note    
 * @param   [out] affected_obj_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_NO_INSTANCE
 * @date    2019/03/04 KCN)吉木
 */
status_t i_o_ran_fm_get_affected_objects(
    affected_obj_info_t *affected_obj_temp)
{
    status_t res = ERR_NCX_NO_INSTANCE;
    static uint32 wk_alarm_indx = 0;
    static uint32 mod_affobj_indx = 0;
    unsigned char num_of_affected_obj  = 0;
    uint32 mod_alarm_indx = 0;

    if (LOGDEV1) {
        log_dev1("\nEnter %s: wk_alarm_indx==%d, mod_affobj_indx=%d",
            __func__, wk_alarm_indx, mod_affobj_indx);
    }

    /* mutex lock (mutex_for_fm_alarm_info_tbl) */
    pthread_mutex_lock(&mutex_for_fm_alarm_info_tbl);

     num_of_affected_obj  = saa_fm_alarm_info_tbl[wk_alarm_indx].num_of_affected_obj;
     if (LOGDEV1) {
         log_dev1("\n%s: wk_alarm_indx=%d, num_of_affected_obj=%d",
             __func__, wk_alarm_indx, num_of_affected_obj);
     }

     if(num_of_affected_obj != 0){
         if((mod_affobj_indx < num_of_affected_obj)
           && (mod_affobj_indx < SAA_FM_MAX_NUM_AFFECTED_OBJ)){
             *affected_obj_temp = saa_fm_alarm_info_tbl[wk_alarm_indx].affected_obj[mod_affobj_indx];
             if(LOGDEV1){
                 log_dev1("\n%s: affected_object found: wk_alarm_indx=%d, mod_affobj_indx=%d", __func__, wk_alarm_indx, mod_affobj_indx);
             }
             res = NO_ERR;
         }
     }

    if(res == NO_ERR){
        mod_affobj_indx ++;
    }
    else{
        res = ERR_NCX_NO_INSTANCE;
        mod_affobj_indx = 0;

        mod_alarm_indx = wk_alarm_indx;
        while(mod_alarm_indx < SAA_FM_MAX_ALARM_NUM) {
            mod_alarm_indx ++;
            if(saa_fm_alarm_info_tbl[mod_alarm_indx].num_of_affected_obj != 0) {
                if (LOGDEV1) {
                    log_dev1("\n%s: affected_obj still exists. wk_alarm_indx =%d, num_of_affected_obj=%d",
                        __func__, wk_alarm_indx, num_of_affected_obj);
                }
                break;
            }
        }
        if(mod_alarm_indx < SAA_FM_MAX_ALARM_NUM) {
            wk_alarm_indx ++;
        }
        else {
            if (LOGDEV1) {
                log_dev1("\n%s: affected_obj does not exists.", __func__);
            }
            wk_alarm_indx = 0;
        }
    }

    /* mutex unlock (mutex_for_fm_alarm_info_tbl) */
    pthread_mutex_unlock(&mutex_for_fm_alarm_info_tbl);

    if(LOGDEV1){
        log_dev1("\n%s: returns. res=%d, wk_alarm_indx=%d, mod_affobj_indx=%d", __func__, res, wk_alarm_indx, mod_affobj_indx);
    }

    return res;

} /* i_o_ran_fm_get_affected_objects */

/*!
 * @brief   alarm_infoをnotification用に取得する。
 * @note    
 * @param   なし
 * @return  alarm情報
 * @date    2019/12/23 KCN)中井
 */
alarm_info_t *i_o_ran_fm_get_notification_data(void){
    return &m_alarm_info_for_notification;
}

/*!
 * @brief   初期化
 * @note    
 * @param   なし
 * @return  alarm情報
 * @date    2019/12/23 KCN)中井
 */
status_t i_o_ran_fm_init2(void)
{

    if (LOGINFO) {
        log_info("\nEnter %s", __func__);
    }

    status_t res = NO_ERR;

    /* put your init2 code here */
    f_saa_com_reg_notification_handler(
        D_MSG_MSI_O_RAN_FM_ALARM_NOTIF_NTF_IND,
        i_o_ran_fm_alarm_notif_handler);

    (void)f_saa_com_add_request(D_MSG_MSI_O_RAN_FM_ALARM_START_IND, 0, NULL);
    if (LOGINFO) {
        log_info("\n    Send MSI_O_RAN_FM_ALARM_START_IND.");
    }

    return res;

} /* u_o_ran_fm_init2 */


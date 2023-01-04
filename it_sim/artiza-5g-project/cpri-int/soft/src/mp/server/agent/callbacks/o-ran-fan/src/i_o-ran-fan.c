/*
 * @file    i_o-ran-fan.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_FAN
 * @brief       SAA o-ran-fan
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "i_o-ran-fan.h"


/********************************************************************************************************************/
/*   valuable                                                                                                       */
/********************************************************************************************************************/
char i_mod_fan[]                                                      = i_mod_o_ran_fan;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   MSI_O_RAN_FAN_GET送信関数
 * @note    
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 * @return  [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/28 KCN)吉木
 */
status_t i_o_ran_fan_send_msi_get (
    T_MPSW_MSI_O_RAN_FAN_GET_CFM  *get_data)
{
    status_t res = NO_ERR;
    uint32_t read_size = 0;
    int ret = D_MSI_CFM;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    ret = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_FAN_GET_REQ,
        0,
        NULL,
        D_MSG_MSI_O_RAN_FAN_GET_CFM,
        0,
        sizeof(T_MPSW_MSI_O_RAN_FAN_GET_CFM),
        (void *)get_data,
        &read_size);

    if(ret != D_MSI_CFM || read_size != sizeof(T_MPSW_MSI_O_RAN_FAN_GET_CFM)){
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: ret=%d, size=%d.", __func__, ret, read_size);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_fan_send_msi_get */

/*!
 * @brief   fan nameからkey番号の取得を行う
 * @note    
 * @param   [in] key_str
 * @param   [out] curindex
 * @return  [正常]NO_ERR
 * @return  [異常]ERR_NCX_DATA_MISSING
 * @date    2019/03/28 KCN)吉木
 */
status_t i_o_ran_fan_fan_tray_fan_state_get_key_num(
    const xmlChar *key_str,
    uint32 *curindex)
{
    status_t res = ERR_NCX_DATA_MISSING;
    xmlChar *fan_name;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    for(uint32 i = 0; i < SAA_FAN_MAX_FAN_NUM; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s.%s.%d.%s",
            y_o_ran_fan_N_fan_tray,
            y_o_ran_fan_N_fan_state,
            i,
            y_o_ran_fan_N_name);
        fan_name = (xmlChar *)f_saa_com_get_init_data(i_mod_fan, para_name);
        if (fan_name == NULL) {
            if(LOGWARN) {
                log_warn("\n%s: Data Missing, para=%s", __func__, para_name);
            }
            return ERR_NCX_DATA_MISSING;
        }
        else if (!xml_strcmp(fan_name, key_str)) {
            *curindex = i;
            if (LOGDEV1) {
                log_dev1("\n%s: Get key index, num=%d", __func__, i);
            }
            res = NO_ERR;
            break;
        }
    }

    return res;

}/* i_o_ran_fan_fan_tray_fan_state_get_key_num */

/*!
 * @brief   fanの先頭のkey取得を行う
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 * @return  [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/28 KCN)吉木
 */
status_t i_o_ran_fan_fan_tray_fan_state_get_first_key(
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s.%s.%d.%s",
        y_o_ran_fan_N_fan_tray,
        y_o_ran_fan_N_fan_state,
        0,
        y_o_ran_fan_N_name);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_fan, para_name);
    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get first key.", __func__);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_fan_fan_tray_fan_state_get_first_key */

/*!
 * @brief   fanの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 * @return  [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/28 KCN)吉木
 */
status_t i_o_ran_fan_fan_tray_fan_state_get_target_key(
    uint32 curindex,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s.%s.%d.%s",
        y_o_ran_fan_N_fan_tray,
        y_o_ran_fan_N_fan_state,
        curindex,
        y_o_ran_fan_N_name);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_fan, para_name);
    if(*key_name_temp == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get target key, index = %d", __func__, curindex);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_fan_fan_tray_fan_state_get_target_key */


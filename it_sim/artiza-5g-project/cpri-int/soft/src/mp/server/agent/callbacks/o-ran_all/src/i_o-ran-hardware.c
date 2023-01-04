
/*
 * @file    i_o-ran-hardware.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_HARDWARE
 * @brief       SAA o-ran-hardware
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "u_o-ran-hardware.h"
#include "i_o-ran-hardware.h"
#include "i_ietf-hardware.h"

#include "f_mpsw_msg_o-ran_hardware_def.h"
#include "f_mpsw_msg_o-ran_hardware_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_msi_lib.h"
/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   MSI_O_RAN_HARDWARE_GET送信関数
 * @note    MSI_O_RAN_HARDWARE_GETを送信する
 * @param   [in] k_hardware_component_name
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_NOT_SUPPORTED
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/26 KCN)上本
 */
status_t i_o_ran_hardware_send_msi (void  *get_data)
{
    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    int ret = D_MSG_BUFFER_GET_OK;
    (void)get_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI_O_RAN_HARDWARE_GET送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_HARDWARE_GET_REQ, 0, NULL);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_HARDWARE_GETレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        max(sizeof(T_MPSW_MSI_O_RAN_HARDWARE_GET_CFM), sizeof(T_MPSW_MSI_O_RAN_HARDWARE_GET_REJ)),
        &message_id,
        &read_size,
        get_data,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_HARDWARE_GETレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_HARDWARE_GET_CFM) {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id (%0x).", __func__, message_id);
            }
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: buffer size too small.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_TIMEOUT:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: time out.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_OTHER:
    default:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: other.", __func__);
        }
    }

    return res;
} /* i_o_ran_hardware_send_msi */

/*!
 * @brief   label_content取得関数
 * @note    label_contentを取得する
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_o_ran_hardware_hardware_component_label_content_get (label_content_t *get_data)
{
    status_t res = NO_ERR;
    char mod_name[]  = i_mod_o_ran_hardware;
    char para_name0[] = i_para_o_ran_hardware_model_name;
    char para_name1[] = i_para_o_ran_hardware_serial_num;
    const xmlChar * init_model_name;
    const xmlChar * init_serial_num;
    (void)get_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    init_model_name = (const xmlChar *)f_saa_com_get_init_data(mod_name, para_name0);
    init_serial_num = (const xmlChar *)f_saa_com_get_init_data(mod_name, para_name1);

    /* model-name */
    if ((!xml_strcmp(i_o_ran_hardware_N_boolean_state_name, init_model_name))) {
        get_data->model_name = true;
        if (LOGDEV1) {
            log_dev1("\n%s: model_name:true", __func__);
        }
    } else {
        get_data->model_name = false;
        if (LOGDEV1) {
            log_dev1("\n%s: model_name:false", __func__);
        }
    }
    /* serial-number */
    if ((!xml_strcmp(i_o_ran_hardware_N_boolean_state_name, init_serial_num))) {
        get_data->serial_number = true;
        if (LOGDEV1) {
            log_dev1("\n%s: serial_number:true", __func__);
        }
    } else {
        get_data->serial_number = false;
        if (LOGDEV1) {
            log_dev1("\n%s: serial_number:false", __func__);
        }
    }

    return res;
} /* i_o_ran_hardware_hardware_component_label_content_get */

/*!
 * @brief   MSI_O_RAN_HARDWARE_EDIT送信関数
 * @note    MSI_O_RAN_HARDWARE_EDITを送信する
 * @param   [in] power_state
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/29 KCN)吉木
 */
status_t i_o_ran_hardware_send_msi_edit (boolean energy_saving_enabled)
{
    status_t res = NO_ERR;
    uint32_t read_size = 0;
    int ret = D_MSI_CFM;
    T_MPSW_MSI_O_RAN_HARDWARE_EDIT_REQ send_data;
    char read_data[D_PARAM_NUM_MAX] = "";

    if (LOGINFO) {
        log_info("\nEnter %s", __func__);
    }

    if(energy_saving_enabled)
    {
        send_data.energy_saving_enabled = E_HARD_POW_STAT_SLEEPING;
    }
    else
    {
        send_data.energy_saving_enabled = E_HARD_POW_STAT_AWAKE;
    }

    ret = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_HARDWARE_EDIT_REQ,
        sizeof(T_MPSW_MSI_O_RAN_HARDWARE_EDIT_REQ),
        (void *)&send_data,
        D_MSG_MSI_O_RAN_HARDWARE_EDIT_CFM,
        D_MSG_MSI_O_RAN_HARDWARE_EDIT_REJ,
        sizeof(read_data),
        (void *)read_data,
        &read_size);

    if(ret != D_MSI_CFM){
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: ret=%d, size=%d.", __func__, ret, read_size);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;
} /* i_o_ran_hardware_send_msi_edit */

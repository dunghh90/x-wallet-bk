
/*
 * @file    i_ietf-interfaces.c
 */

/*!
 * @addtogroup  SAA_MOD_IETF_INTERFACES
 * @brief       SAA ietf-interfaces
 * @note        
 * @{
 */

#include <string.h>

#include "xml_util.h"
#include "u_ietf-interfaces.h"
#include "i_ietf-interfaces.h"
#include "f_saa_msg_buffer.h"

uc_if_name_t uc_if_name;
xmlChar  m_ip_lower_layer_if[D_PARAM_NUM_MAX];
/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   MSI_IETF_INTERFACES_GET送信関数
 * @note    MSI_IETF_INTERFACES_GETを送信する
 * @param   [in] interface_name
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/21 KCN)南條
 */
status_t i_ietf_interface_send_msi (const xmlChar *interface_name, void  *get_data)
{
    status_t res = NO_ERR;
    T_MPSW_MSI_IETF_INTERFACES_GET_REQ  req_data;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    int ret = D_MSG_BUFFER_GET_OK;
    (void)get_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if (!xml_strcmp(interface_name, i_ietf_interface_N_name_fheth0)) {
        req_data.type   = D_IETF_INTERFACE_TYPE_ETHER;
        req_data.plane  = D_IETF_INTERFACE_ETH;
        req_data.port   = D_IETF_INTERFACE_FHETH_PORT0;
    } else if (!xml_strcmp(interface_name, i_ietf_interface_N_name_m_vlan0)) {
        req_data.type   = D_IETF_INTERFACE_TYPE_L2VLAN;
        req_data.plane  = D_IETF_INTERFACE_MPLANE;
        req_data.port   = D_IETF_INTERFACE_FHETH_PORT0;
    } else if (!xml_strcmp(interface_name, uc_if_name.uc_name)) {
        req_data.type   = D_IETF_INTERFACE_TYPE_L2VLAN;
        req_data.plane  = D_IETF_INTERFACE_UCPLANE;
        req_data.port   = D_IETF_INTERFACE_FHETH_PORT0;
    } else if (!xml_strcmp(interface_name, i_ietf_interface_N_name_m_ip)) {
        req_data.type   = D_IETF_INTERFACE_TYPE_IP;
        req_data.plane  = D_IETF_INTERFACE_MPLANE;
        req_data.port   = D_IETF_INTERFACE_FHETH_PORT0;
    }
    else {
        res = ERR_NCX_INVALID_VALUE;
        if(LOGWARN) {
            log_warn("\n%s: name is Not Support", __func__);
        }
        return res;
    }

    /* MSI_IETF_INTERFACES_GET送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_IETF_INTERFACES_GET_REQ, sizeof(T_MSG_MSI_IETF_INTERFACES_GET_REQ), &req_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed.: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_IETF_INTERFACES_GETレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        max(sizeof(T_MPSW_MSI_IETF_INTERFACES_GET_CFM), sizeof(T_MPSW_MSI_IETF_INTERFACES_GET_REJ)),
        &message_id,
        &read_size,
        get_data,
        D_MSG_WAIT_TIME );

    /* MSI_IETF_INTERFACES_GETレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_IETF_INTERFACES_GET_CFM) {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed.: wrong message id. (%0x)", __func__, message_id);
            }
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed.: buffer size too small. (%0x)", __func__, read_size);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_TIMEOUT:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed.: time out.", __func__);
        }
        break;
    case D_MSG_BUFFER_GET_ERR_OTHER:
    default:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed.: other error.", __func__);
        }
    }

    return res;
} /* i_ietf_interface_send_msi */

/*!
 * @brief   oper_state取得関数 
 * @note    oper_stateを取得する関数 
 * @param   [in] interface_name
 * @param   [out] v_oper_status
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_ietf_interface_get_oper_state (const xmlChar *interface_name, const xmlChar **oper_status)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if( !xml_strcmp(interface_name, uc_if_name.uc_name) ||
        !xml_strcmp(interface_name, i_ietf_interface_N_name_m_ip) ) {
        *oper_status = i_ietf_interface_N_oper_state_up;
    } else {
        res = ERR_NCX_DATA_MISSING;
        if(LOGWARN) {
            log_warn("\n%s: oper_status is Not Data. name : %s", __func__, interface_name);
        }
    }


    return res;
} /* i_ietf_interface_get_oper_state */

/*!
 * @brief   speed取得関数 
 * @note    speedを取得する関数 
 * @param   [in] interface_name
 * @param   [out] v_speed
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_ietf_interface_get_speed (const xmlChar *interface_name, uint64 *speed)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if( !xml_strcmp(interface_name, i_ietf_interface_N_name_fheth0) ){
        *speed = D_IETF_INTERFACE_SPEED;
    } else {
        res = ERR_NCX_DATA_MISSING;
        if(LOGWARN) {
            log_warn("\n%s: speed oper_status is Not Data. name : %s", __func__, interface_name);
        }
    }

    return res;
} /* i_ietf_interface_get_speed */

/*!
 * @brief   higher_layer_if取得関数 
 * @note    higher_layer_ifを取得する関数 
 * @param   [in] interface_name
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_ietf_interface_get_higher_layer_if (const xmlChar *interface_name, layer_if_t *get_data)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if( !xml_strcmp(interface_name, i_ietf_interface_N_name_fheth0) ) {
        get_data->layer_if[0] = i_ietf_interface_N_name_m_vlan0;
        /* uc_nameが空かどうかは呼び元で処理 */
        get_data->layer_if[1] = uc_if_name.uc_name;
    }else if( !xml_strcmp(interface_name, i_ietf_interface_N_name_m_vlan0) ) {
        get_data->layer_if[0] =i_ietf_interface_N_name_m_ip; 
    }
    else{
        res = ERR_NCX_DATA_MISSING;
        if(LOGWARN) {
            log_warn("\n%s:%d init_higher_layer_if is NULL", __func__, __LINE__);
        }
    } 

    return res;
} /* i_ietf_interface_get_higher_layer_if */

/*!
 * @brief   low_layer_if取得関数 
 * @note    low_layer_ifを取得する関数 
 * @param   [in] interface_name
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_ietf_interface_get_low_layer_if (const xmlChar *interface_name, layer_if_t *get_data)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if( !xml_strcmp(interface_name, i_ietf_interface_N_name_m_vlan0) ) {
        get_data->layer_if[0] = i_ietf_interface_N_name_fheth0;
    } else if( !xml_strcmp(interface_name, uc_if_name.uc_name) ) {
        get_data->layer_if[0] = i_ietf_interface_N_name_fheth0;

    } else if( !xml_strcmp(interface_name, i_ietf_interface_N_name_m_ip) ) {
        get_data->layer_if[0] = i_ietf_interface_N_name_m_vlan0;
    } else{
        res = ERR_NCX_DATA_MISSING;
        if(LOGWARN) {
            log_warn("\n%s:%d init_lower_layer_if is NULL", __func__, __LINE__);
        }
   }

    return res;
} /* i_ietf_interface_get_low_layer_if */

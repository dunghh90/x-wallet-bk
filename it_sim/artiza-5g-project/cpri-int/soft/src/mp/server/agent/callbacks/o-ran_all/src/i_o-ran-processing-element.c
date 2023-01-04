
/*
 * @file    i_o-ran-processing-element.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_PROCESSING_ELEMENT
 * @brief       SAA o-ran-processing-element
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <pthread.h>
#include "u_o-ran-processing-element.h"
#include "f_saa_msg_buffer.h"
#include "i_ietf-interfaces.h"
#include "i_o-ran-processing-element.h"

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/* struct of name & interface_name */
allocate_name_t allocate_name[2];
pthread_mutex_t mutex_for_allocate_name = PTHREAD_MUTEX_INITIALIZER;

process_send_msi_t  process_send_msi[2];
/*!
 * @brief   vlan_idのvalidateを行う
 * @note    
 * @param   [in] k_processing_elements_ru_elements_name
 * @param   [in] vlan_id_str
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_BAD_ELEMENT
 * @date    2018/11/21 KCN)南條
 */
status_t i_o_ran_processing_element_processing_elements_ru_elements_transport_flow_eth_flow_vlan_id_validate (
    const xmlChar *k_processing_elements_ru_elements_name,
    const xmlChar *vlan_id_str )
{
    status_t res = NO_ERR;
    char mod_name[]  = i_mod_o_ran_processing;
    char para_name[] = i_para_o_ran_processing_element_vlanid;

    uint16  vlan_id = 0;
    int64_t vlan_id_validate_min = 0;
    int64_t vlan_id_validate_max = 0;
    char   *vlan_id_validate;
    int interface_num = D_PROCESSING_ELEMENT_FHETH_PORT0;
    int ret = D_SYS_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_allocate_name);

    /* ru-elements*[name]判定 */
    if(!xml_strcmp(k_processing_elements_ru_elements_name, allocate_name[0].name)) {
        /* fheth0 */
        interface_num = D_PROCESSING_ELEMENT_FHETH_PORT0;
        if (LOGDEV1) {
            log_dev1("\n%s: interface_num = %d", __func__, interface_num);
        }
    } else if(!xml_strcmp(k_processing_elements_ru_elements_name, allocate_name[1].name)) {
        /* fheth1 */
        interface_num = D_PROCESSING_ELEMENT_FHETH_PORT1;
        if (LOGDEV1) {
            log_dev1("\n%s: interface_num = %d", __func__, interface_num);
        }
    } else {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: interface_name is Not Data", __func__);
            log_warn("\n    this name            =%s",k_processing_elements_ru_elements_name);
            log_warn("\n    allocate_name name[0]=%s",allocate_name[0].name);
            log_warn("\n    allocate_name name[1]=%s",allocate_name[1].name);
        }
        /* unlock */
        pthread_mutex_unlock(&mutex_for_allocate_name);
        return res;
    }

    /* unlock */
    pthread_mutex_unlock(&mutex_for_allocate_name);

    /* vlan-id変換(文字列->数値) */
    vlan_id = (uint16)atoi((const char*)vlan_id_str);

    /* validate 文字列取得 */
    vlan_id_validate = f_saa_com_get_validate_data(mod_name, para_name);
    if(vlan_id_validate == NULL)
    {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s:%d vlan_id is NULL", __func__, __LINE__);
        }
        return res;
    }

    /* validate min,max取得 */
    ret = f_saa_com_conversion_validate_data_max_min(vlan_id_validate, &vlan_id_validate_min, &vlan_id_validate_max);
    if(ret != D_SYS_OK) {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s:%d vlan_id is NULL", __func__, __LINE__);
        }
        return res;
    }

    if (LOGDEV1) {
        log_dev1("\n%s: vlan_id=%d", __func__, vlan_id);
        log_dev1("\n    vlan_id_validate_min=%d", (uint16)vlan_id_validate_min);
        log_dev1("\n    vlan_id_validate_max=%d", (uint16)vlan_id_validate_max);
    }
    /* validate */
    if( (uint16)vlan_id_validate_min > vlan_id || (uint16)vlan_id_validate_max < vlan_id )
    {
        /* validate NG */
        res = ERR_NCX_BAD_ELEMENT;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: vlan_id is not validated.", __func__);
            log_warn("\n%s: vlan_id=%d", __func__, vlan_id);
            log_warn("\n    vlan_id_validate_min=%d", (uint16)vlan_id_validate_min);
            log_warn("\n    vlan_id_validate_max=%d", (uint16)vlan_id_validate_max);
        }
        return res;
    }

     /* MSI送信フラグ更新 */
    process_send_msi[interface_num].msi_send_flg = true;

    /* MSI_O_RAN_PROCESSING_ELEMENT_EDIT Data部作成 */
    process_send_msi[interface_num].vlan_id = vlan_id;

    return res;
} /* i_o_ran_processing_element_processing_elements_ru_elements_transport_flow_eth_flow_vlan_id_validate */


/*!
 * @brief   ru_mac_address Edit値保持関数
 * @note    
 * @param   [in] k_processing_elements_ru_elements_name
 * @param   [in] ru_mac_address
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_o_ran_processing_element_processing_elements_ru_elements_transport_flow_eth_flow_ru_mac_address_validate (
    const xmlChar *k_processing_elements_ru_elements_name,
    const xmlChar *ru_mac_address )
{
    status_t res = NO_ERR;
    xmlChar  tmp_mac_address[D_PARAM_NUM_MAX];
    char     *tmp_address_p;
    int      size = 0;
    int      interface_num = D_PROCESSING_ELEMENT_FHETH_PORT0;

    if (LOGINFO) {
        log_info("\nEnter %s: ru_mac_address=%s", __func__, ru_mac_address);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_allocate_name);

    /* ru-elements*[name]判定 */
    if(!xml_strcmp(k_processing_elements_ru_elements_name, allocate_name[0].name)) {
        /* fheth0 */
        interface_num = D_PROCESSING_ELEMENT_FHETH_PORT0;
        if (LOGDEV1) {
            log_dev1("\n%s:%d interface_num = %d", __func__, __LINE__, interface_num);
        }
    } else if(!xml_strcmp(k_processing_elements_ru_elements_name, allocate_name[1].name)) {
        /* fheth1 */
        interface_num = D_PROCESSING_ELEMENT_FHETH_PORT1;
        if (LOGDEV1) {
            log_dev1("\n%s:%d interface_num = %d", __func__, __LINE__, interface_num);
        }
    } else {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s interface_name is Not Data", __func__);
            log_warn("\n    this name            =%s",k_processing_elements_ru_elements_name);
            log_warn("\n    allocate_name name[0]=%s",allocate_name[0].name);
            log_warn("\n    allocate_name name[1]=%s",allocate_name[1].name);
        }

        /* unlock */
        pthread_mutex_unlock(&mutex_for_allocate_name);
        return res;
    }
    /* unlock */
    pthread_mutex_unlock(&mutex_for_allocate_name);

    /* MSI送信フラグ更新 */
    process_send_msi[interface_num].msi_send_flg = true;

    /* MSI_O_RAN_PROCESSING_ELEMENT_EDIT Data部作成 */
    xml_strcpy(tmp_mac_address, ru_mac_address);
    tmp_address_p = strtok((char*)tmp_mac_address, ":");
    while(tmp_address_p) {

        process_send_msi[interface_num].ru_mac_address[size] = (UCHAR)strtol(tmp_address_p,NULL,16);
        tmp_address_p = strtok(NULL,  ":");
        size++;
    }

    return res;
} /* i_o_ran_processing_element_processing_elements_ru_elements_transport_flow_eth_flow_ru_mac_address_validate */


/*!
 * @brief   o_du_mac_address Edit値保持関数
 * @note    
 * @param   [in] k_processing_elements_ru_elements_name
 * @param   [in] o_du_mac_address
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_o_ran_processing_element_processing_elements_ru_elements_transport_flow_eth_flow_o_du_mac_address_validate (
    const xmlChar *k_processing_elements_ru_elements_name,
    const xmlChar *o_du_mac_address )
{
    status_t res = NO_ERR;
    xmlChar  tmp_mac_address[D_PARAM_NUM_MAX];
    char     *tmp_address_p;
    int      size = 0;
    int      interface_num = D_PROCESSING_ELEMENT_FHETH_PORT0;

    if (LOGINFO) {
        log_info("\nEnter %s: o_du_mac_address=%s", __func__, o_du_mac_address);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_allocate_name);

    /* ru-elements*[name]判定 */
    if(!xml_strcmp(k_processing_elements_ru_elements_name, allocate_name[0].name)) {
        /* fheth0 */
        interface_num = D_PROCESSING_ELEMENT_FHETH_PORT0;
        if (LOGDEV1) {
            log_dev1("\n%s:%d interface_num = %d", __func__, __LINE__, interface_num);
        }
    } else if(!xml_strcmp(k_processing_elements_ru_elements_name, allocate_name[1].name)) {
        /* fheth1 */
        interface_num = D_PROCESSING_ELEMENT_FHETH_PORT1;
        if (LOGDEV1) {
            log_dev1("\n%s:%d interface_num = %d", __func__, __LINE__, interface_num);
        }
    } else {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: interface_name is Not Data", __func__);
            log_warn("\nthis name            =%s",k_processing_elements_ru_elements_name);
            log_warn("\nallocate_name name[0]=%s",allocate_name[0].name);
            log_warn("\nallocate_name name[1]=%s",allocate_name[1].name);
        }
        /* unlock */
        pthread_mutex_unlock(&mutex_for_allocate_name);
        return res;
    }
    /* unlock */
    pthread_mutex_unlock(&mutex_for_allocate_name);

    /* MSI送信フラグ更新 */
    process_send_msi[interface_num].msi_send_flg = true;

    /* MSI_O_RAN_PROCESSING_ELEMENT_EDIT Data部作成 */
    xml_strcpy(tmp_mac_address, o_du_mac_address);
    tmp_address_p = strtok((char*)tmp_mac_address, ":");
    while(tmp_address_p) {
        process_send_msi[interface_num].o_du_mac_address[size] = (UCHAR)strtol(tmp_address_p,NULL,16);
        tmp_address_p = strtok(NULL,  ":");
        size++;
    }

    return res;
} /* i_o_ran_processing_element_processing_elements_ru_elements_transport_flow_eth_flow_o_du_mac_address_validate */


/*!
 * @brief   MSI_O_RAN_PROCESSING_ELEMENT_EDIT送信関数
 * @note    MSI_O_RAN_PROCESSING_ELEMENT_EDITを送信する
 * @param   [in] k_processing_elements_ru_elements_name
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_o_ran_processing_element_processing_elements_send_msi (
    const xmlChar *k_processing_elements_ru_elements_name)
{
    status_t res = NO_ERR;
    uint port = 0;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ     edit_data;
    T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ     read_data;
    int ret = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_allocate_name);

    /* ru-elements*[name]判定 */
    if(!xml_strcmp(k_processing_elements_ru_elements_name, allocate_name[0].name)) {
        /* fheth0 */
        port = D_PROCESSING_ELEMENT_FHETH_PORT0;
        if (LOGDEV1) {
            log_dev1("\n%s:%d port = %d", __func__, __LINE__, port);
        }
    } else if(!xml_strcmp(k_processing_elements_ru_elements_name, allocate_name[1].name)) {
        /* fheth1 */
        port = D_PROCESSING_ELEMENT_FHETH_PORT1;
        if (LOGDEV1) {
            log_dev1("\n%s:%d port = %d", __func__, __LINE__, port);
        }
    } else {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: interface_name is Not Data", __func__);
            log_warn("\n    this name            =%s",k_processing_elements_ru_elements_name);
            log_warn("\n    allocate_name name[0]=%s",allocate_name[0].name);
            log_warn("\n    allocate_name name[1]=%s",allocate_name[1].name);
        }
        /* unlock */
        pthread_mutex_unlock(&mutex_for_allocate_name);
        return res;
    }

    /* unlock */
    pthread_mutex_unlock(&mutex_for_allocate_name);

    if(process_send_msi[port].msi_send_flg == false) {
        if (LOGDEV1) {
            log_dev1("\n%s: Already sent MSI_O_RAN_PROCESSING_ELEMENT_EDIT", __func__);
        }
        return res;
    }
    /* MSI送信フラグ更新 */
    process_send_msi[port].msi_send_flg = false;

    /* editデータ設定 */
    edit_data.port      = port;
    edit_data.vlan_id   = process_send_msi[port].vlan_id;
    memcpy(edit_data.ru_mac_address, process_send_msi[port].ru_mac_address, sizeof(edit_data.ru_mac_address));
    memcpy(edit_data.o_du_mac_address, process_send_msi[port].o_du_mac_address, sizeof(edit_data.o_du_mac_address));

    /* MSI_O_RAN_PROCESSING_ELEMENT_EDIT送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ, sizeof(T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ), (void*)&edit_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_PROCESSING_ELEMENT_EDITレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        sizeof(T_MPSW_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REJ),
        &message_id,
        &read_size,
        &read_data,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_PROCESSING_ELEMENT_EDITレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM) {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x).", __func__, message_id);
            }
        }
        break;
    case D_MSG_BUFFER_GET_ERR_SIZE:
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
                log_warn("\n%s: MSI failed: too small buffer size.", __func__);
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
} /* i_o_ran_processing_element_processing_elements_send_msi */


/*!
 * @brief   component/nameを渡す
 * @note    
 * @param   [in] num
 *          [out] xmlChar *data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/03/01 KCN)若水
 */
void i_o_ran_processing_element_processing_elements_ru_elements_name_give (UINT num, xmlChar *data)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_allocate_name);

    /* dataに値を詰める */
    xml_strcpy(data, allocate_name[num].name);

    /* unlock */
    pthread_mutex_unlock(&mutex_for_allocate_name);

    return;
} /* i_o_ran_processing_element_processing_elements_ru_elements_name_give */

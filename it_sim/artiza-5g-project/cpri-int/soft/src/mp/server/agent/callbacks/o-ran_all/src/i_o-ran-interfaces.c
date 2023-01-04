
/*
 * @file    i_o-ran-interfaces.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_INTERFACES
 * @brief       SAA o-ran-interfaces
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "i_o-ran-interfaces.h"

static xmlChar *mod_mac_address = NULL;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   vlan_taggingのvalidateを行う
 * @note    
 * @param   [in] newval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_BAD_ELEMENT
 * @date    2018/11/24 KCN)wang
 */
status_t i_o_ran_interfaces_interfaces_interface_vlan_tagging_validate (boolean *newval)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(*newval != TRUE) {
        log_warn("\n%s: parameter isn't TRUE", __func__);
        res = ERR_NCX_BAD_ELEMENT;
    }

    return res;
} /* i_o_ran_interfaces_interfaces_interface_vlan_tagging_validate */


/*!
 * @brief   mac_addressのvalidateを行う
 * @note    
 * @param   [in] k_interfaces_interface_name
 * @param   [in] newval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_BAD_ELEMENT
 *          [異常]ERR_NCX_OPERATION_NOT_SUPPORTED
 * @date    2018/11/24 KCN)南條
 */
status_t i_o_ran_interfaces_interfaces_interface_mac_address_validate (
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *newval_val)
{
    status_t res = NO_ERR;
    char mod_name[]  = i_mod_o_ran_interfaces;
    char para_name[] = i_para_o_ran_mac_address;

    (void)k_interfaces_interface_name;

    if(mod_mac_address == NULL){
        mod_mac_address = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name);
        if (LOGINFO) {
            log_info("\n%s: MAC address [%s] from init_data", __func__, mod_mac_address);
        }
    }

    /* mac_address validate */
    if(xml_strcmp(newval_val, mod_mac_address))
    {
        log_warn("\n%s: mac_address doesn't match new=%s, init=%s",
            __func__, newval_val, mod_mac_address);
        res = ERR_NCX_BAD_ELEMENT;
    }

    return res;
} /* i_o_ran_interfaces_interfaces_interface_mac_address_validate */


/*!
 * @brief   EDIT_vlan_id取得関数
 * @note    EDIT_vlan_id取得関数
 * @param   [in] k_interfaces_interface_name
 * @param   [in] newval_val
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2018/11/21 KCN)南條
 */
status_t i_o_ran_interfaces_vlan_id_edit (
    const xmlChar *k_interfaces_interface_name,
    uint16 newval_val)
{
    status_t res = NO_ERR;
    T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ  edit_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* k_interfaces_interface_name は既にvalidateされているので
       本関数ではvalidate不要。 */

    if (!xml_strcmp(k_interfaces_interface_name, i_ietf_interface_N_name_m_vlan0)){
        return NO_ERR;
    }

    /* ポートは固定 */
    edit_data.port         = D_IETF_INTERFACE_FHETH_PORT0;

    /* MSI_O_RAN_INTERFACE_EDIT送信 */
    edit_data.plane        = D_O_RAN_INTERFACE_UCPLANE;
    edit_data.marking[0]   = D_O_RAN_INTERFACE_INDEFINITE;
    edit_data.marking[1]   = D_O_RAN_INTERFACE_INDEFINITE;
    edit_data.vlan_id      = newval_val;

    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
        log_dev1("\n    port:0x%02x",edit_data.port);
        log_dev1("\n    plane:0x%02x",edit_data.plane);
        log_dev1("\n    marking[0]:0x%04x",edit_data.marking[0]);
        log_dev1("\n    marking[1]:0x%04x",edit_data.marking[1]);
        log_dev1("\n    vlan_id:0x%04x",edit_data.vlan_id);
    }

    res = i_o_ran_interfaces_edit_send_msi(&edit_data);

    return res;
} /* i_o_ran_interfaces_vlan_id_edit */


/*!
 * @brief   EDIT_m_plane_marking取得関数
 * @note    EDIT_m_plane_marking取得関数
 * @param   [in] k_interfaces_interface_name
 * @param   [in] newval_val
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2018/11/21 KCN)南條
 */
status_t i_o_ran_interfaces_m_plane_marking_edit (
    const xmlChar *k_interfaces_interface_name,
    uint8 newval_val)
{
    status_t res = NO_ERR;

    (void)k_interfaces_interface_name;

    T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ  edit_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* k_interfaces_interface_name は既にvalidateされているので
       本関数ではvalidate不要。 */

    /* ポートは固定 */
    edit_data.port         = D_IETF_INTERFACE_FHETH_PORT0;

    /* MSI_O_RAN_INTERFACE_EDIT送信 */
    edit_data.plane        = D_O_RAN_INTERFACE_MPLANE;
    edit_data.marking[0]   = (USHORT)newval_val;
    edit_data.marking[1]   = D_O_RAN_INTERFACE_INDEFINITE;
    edit_data.vlan_id      = D_O_RAN_INTERFACE_INDEFINITE;

    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
        log_dev1("\n    port:0x%02x",edit_data.port);
        log_dev1("\n    plane:0x%02x",edit_data.plane);
        log_dev1("\n    marking[0]:0x%04x",edit_data.marking[0]);
        log_dev1("\n    marking[1]:0x%04x",edit_data.marking[1]);
        log_dev1("\n    vlan_id:0x%04x",edit_data.vlan_id);
    }

    res = i_o_ran_interfaces_edit_send_msi(&edit_data);

    return res;
} /* i_o_ran_interfaces_m_plane_marking_edit */


/*!
 * @brief   EDIT_uc_plane_marking取得関数
 * @note    EDIT_uc_plane_marking取得関数
 * @param   [in] k_interfaces_interface_name
 * @param   [in] newval_val
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2018/11/21 KCN)南條
 */
status_t i_o_ran_interfaces_uc_plane_marking_edit (
    const xmlChar *k_interfaces_interface_name,
    uint8 newval_val,
    uint8 plane_kind)
{
    status_t res = NO_ERR;

    (void)k_interfaces_interface_name;

    T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ  edit_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* k_interfaces_interface_name は既にvalidateされているので
       本関数ではvalidate不要。 */

    /* ポートは固定 */
    edit_data.port         = D_IETF_INTERFACE_FHETH_PORT0;

    if(plane_kind == D_O_RAN_INTERFACE_UPLANE) {
        /* U-Plane */
        /* MSI_O_RAN_INTERFACE_EDIT パラメータ設定 */
        edit_data.marking[0]   = (USHORT)newval_val;;
        edit_data.marking[1]   = D_O_RAN_INTERFACE_INDEFINITE;
    } else {
        /* C-Plane */
        /* MSI_O_RAN_INTERFACE_EDIT パラメータ設定 */
        edit_data.marking[0]   = D_O_RAN_INTERFACE_INDEFINITE;
        edit_data.marking[1]   = (USHORT)newval_val;;
    }

    /* MSI_O_RAN_INTERFACE_EDIT送信 */
    edit_data.plane        = D_O_RAN_INTERFACE_UCPLANE;
    edit_data.vlan_id      = D_O_RAN_INTERFACE_INDEFINITE;

    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
        log_dev1("\n    port:0x%02x",edit_data.port);
        log_dev1("\n    plane:0x%02x",edit_data.plane);
        log_dev1("\n    marking[0]:0x%04x",edit_data.marking[0]);
        log_dev1("\n    marking[1]:0x%04x",edit_data.marking[1]);
        log_dev1("\n    vlan_id:0x%04x",edit_data.vlan_id);
    }

    res = i_o_ran_interfaces_edit_send_msi(&edit_data);

    return res;
} /* i_o_ran_interfaces_uc_plane_marking_edit */


/*!
 * @brief   MSI_O_RAN_INTERFACE_EDIT送信関数
 * @note    MSI_O_RAN_INTERFACE_EDITを送信する
 * @param   [in] ru_aliasmac_address
 * @param   [in] vlan_id
 * @param   [in] o_du_mac_address
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/21 KCN)南條
 */
status_t i_o_ran_interfaces_edit_send_msi (T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ  *edit_data)
{
    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MSG_MSI_O_RAN_INTERFACE_EDIT_REJ   read_data;
    int ret = D_MSG_BUFFER_GET_OK;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI_O_RAN_INTERFACE_EDIT送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_INTERFACE_EDIT_REQ, sizeof(T_MPSW_MSI_O_RAN_INTERFACE_EDIT_REQ), (void*)edit_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_INTERFACE_EDITレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        sizeof(T_MSG_MSI_O_RAN_INTERFACE_EDIT_REJ),
        &message_id,
        &read_size,
        &read_data,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_INTERFACE_EDITレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_INTERFACE_EDIT_CFM) {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: message id. (%0x)", __func__, message_id);
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
} /* i_o_ran_interfaces_edit_send_msi */

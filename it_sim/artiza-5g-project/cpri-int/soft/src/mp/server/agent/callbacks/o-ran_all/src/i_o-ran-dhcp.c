
/*
 * @file    i_o-ran-dhcp.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_DHCP
 * @brief       SAA o-ran-dhcp
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "xml_util.h"
#include "i_o-ran-dhcp.h"


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   MSI_O_RAN_DHCP_GET送信関数
 * @note    MSI_O_RAN_DHCP_GETを送信する
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/14 KCN)南條
 */
status_t i_o_ran_dhcp_send_msi (void *get_data)
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

    /* MSI_O_RAN_DHCP_GET送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_DHCP_GET_REQ, 0, NULL);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_DHCP_GETレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        max(sizeof(T_MPSW_MSI_O_RAN_DHCP_GET_CFM), sizeof(T_MPSW_MSI_O_RAN_DHCP_GET_REJ)),
        &message_id,
        &read_size,
        get_data,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_DHCP_GETレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_DHCP_GET_CFM) {
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
} /* i_o_ran_dhcp_send_msi */

/*!
 * @brief   initデータ取得関数 
 * @note    initデータを取得する関数 
 * @param   [in] init_param_name
 * @param   [out] v_init_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/02/13 KCN)南條
 */
status_t i_o_ran_dhcp_get_init_data (char *init_param_name, xmlChar **v_init_data)
{
    status_t res = NO_ERR;
    char mod_name[]  = i_mod_o_ran_dhcp;
    char *init_data;
    (void)v_init_data;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    init_data = f_saa_com_get_init_data(mod_name, init_param_name);
    /* f_saa_com_get_init_data NGチェック */
    if(init_data == NULL) {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: init_data is NULL", __func__);
        }
        return res;
    }
    *v_init_data = (xmlChar *)init_data;

    return res;
} /* i_o_ran_dhcp_get_init_data */
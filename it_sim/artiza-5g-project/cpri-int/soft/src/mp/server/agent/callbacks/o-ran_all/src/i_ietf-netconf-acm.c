/*
 * @file    i_ietf-netconf-acm.c
 */

/*!
 * @addtogroup  SAA_MOD_IETF_NETCONF_ACM
 * @brief       SAA ietf-netconf-acm
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "u_ietf-netconf-acm.h"
#include "i_ietf-netconf-acm.h"

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   listのValidateを行う
 * @note    
 * @param   [in] para_name
 * @param   [in] target
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2020/1/7 FJT)長谷川
 */
status_t i_ietf_netconf_acm_validate_list(
    char *para_name,
    const xmlChar *target)
{
    status_t res = ERR_NCX_INVALID_VALUE;
    int rtn_saa;
    char *validate_param_addr;
    char modname[] = i_mod_ietf_netconf_acm;
    T_SAA_COM_PARAM_LIST_TBL validate_param_list;

    if (LOGINFO) {
        log_info("\nEnter %s: para_name=%s, target=%s", __func__, para_name, target);
    }

    /* validate_param_list初期化 */
    memset((void *)&validate_param_list, 0, sizeof(T_SAA_COM_PARAM_LIST_TBL));

    /* validate用パラメータを取得 */
    validate_param_addr = f_saa_com_get_validate_data(modname, para_name);
    if (validate_param_addr == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to get validate data, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate用パラメータの変換 */
    rtn_saa = f_saa_com_conversion_validate_data_list(validate_param_addr, &validate_param_list);
    if (rtn_saa == D_SYS_NG) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to convert validate data, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    /* validate */
    for (int i = 0; validate_param_list.list[i][0] != '\0'; i++) {
        if (!xml_strcmp((xmlChar *)validate_param_list.list[i], target)){
            if (LOGDEV1) {
                log_dev1("\n%s: Validation Completed, para_name=%s, target=%s", __func__, para_name, target);
            }
            res = NO_ERR;
            break;
        }
    }
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed, para_name=%s", __func__, para_name);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;
} /* i_ietf_netconf_acm_validate_list */

/*!
 * @brief   group/nameのValidateを行う
 * @note    
 * @param   [in] newval
 * @param   [in] curval
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2020/1/7 FJT)長谷川
 */
status_t i_ietf_netconf_acm_nacm_groups_group_validate(
    const xmlChar *k_nacm_groups_group_name)
{
    status_t res;
    char i_para_nacm_group_name[] = i_para_ietf_netconf_acm_nacm_group_name;
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate */
    res = i_ietf_netconf_acm_validate_list(i_para_nacm_group_name, k_nacm_groups_group_name);
    if (res != NO_ERR) {
        if(LOGWARN) {
            log_warn("\n%s: Validation Failed.", __func__);
        }
    }
    return res;
} /* i_ietf_netconf_acm_nacm_groups_group_validate */


/*
 * @file    i_o-ran-delay-management.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_DELAY_MANAGEMENT
 * @brief       SAA o-ran-delay-management
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "i_o-ran-delay-management.h"


/********************************************************************************************************************/
/*  static valuable                                                                                                 */
/********************************************************************************************************************/
static char i_mod_name[]                               = i_mod_o_ran_delay_management;


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   ru-delay-profileの指定されたパラメータ取得を行う
 * @note    
 * @param   [in] k_delay_management_bandwidth_scs_delay_state_bandwidth
 * @param   [in] k_delay_management_bandwidth_scs_delay_state_subcarrier_spacing
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/02/XX KCN)吉木
 */
status_t i_o_ran_delay_management_delay_management_bandwidth_scs_delay_state_ru_delay_profile_get_para(
    uint32 k_delay_management_bandwidth_scs_delay_state_bandwidth,
    uint32 k_delay_management_bandwidth_scs_delay_state_subcarrier_spacing,
    const xmlChar *target_para_name,
    xmlChar **para_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    
    if (LOGINFO) {
        log_info("\nEnter %s: bw = %d, scs = %d, para = %s", 
            __func__, 
            k_delay_management_bandwidth_scs_delay_state_bandwidth, 
            k_delay_management_bandwidth_scs_delay_state_subcarrier_spacing, 
            target_para_name);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s.%d.%d.%s.%s", 
        y_o_ran_delay_management_N_bandwidth_scs_delay_state, 
        k_delay_management_bandwidth_scs_delay_state_bandwidth, 
        k_delay_management_bandwidth_scs_delay_state_subcarrier_spacing, 
        y_o_ran_delay_management_N_ru_delay_profile, 
        target_para_name);

    *para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_name, para_name);
    if(*para_temp == NULL) {
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param, %s, para = %s", 
                __func__, 
                i_mod_name, 
                para_name);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_delay_management_delay_management_bandwidth_scs_delay_state_ru_delay_profile_get_para */

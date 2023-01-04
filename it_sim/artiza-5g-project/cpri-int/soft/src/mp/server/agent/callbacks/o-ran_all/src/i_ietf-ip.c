/*
 * @file    i_ietf_ip.c
 */

/*!
 * @addtogroup  SAA_MOD_IETF_IP
 * @brief       SAA ietf-ip
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "i_ietf-ip.h"


/********************************************************************************************************************/
/*  static valuable                                                                                                 */
/********************************************************************************************************************/
static char i_mod_upconf[]                                                      = i_mod_ietf_ip;
static char i_para_ip_ipv4_address_origin[]                                     = i_para_ietf_ip_ipv4_address_origin;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   ipv4 address origin取得を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/29 KCN)wang
 */
status_t i_ietf_ip_interfaces_interface_ipv4_address_origin_get(const xmlChar *k_interfaces_interface_name, const xmlChar *k_interfaces_interface_ipv4_address_ip, xmlChar **v_origin_temp)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    if(k_interfaces_interface_name != NULL) {
	    if(k_interfaces_interface_ipv4_address_ip != NULL) {
	        *v_origin_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_upconf, i_para_ip_ipv4_address_origin);
	        if(*v_origin_temp == NULL) {
	            res = ERR_NCX_OPERATION_FAILED;
	            if(LOGWARN) {
	                log_warn("\n%s: Operation Failed to get type, ipv4 address origin", __func__);
	            }
	        }
        }
        else {
	        if(LOGWARN) {
	            log_warn("\n%s: Data Missing, upconf key = %s", __func__, k_interfaces_interface_ipv4_address_ip);
	        }
	        res = ERR_NCX_DATA_MISSING;
		}
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: Data Missing, upconf key = %s", __func__, k_interfaces_interface_name);
        }
        res = ERR_NCX_DATA_MISSING;
    }
    
    return res;
} /* i_ietf_ip_interfaces_interface_ipv4_address_origin_get */



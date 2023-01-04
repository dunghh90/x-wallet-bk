
/*
 * @file    i_o-ran-beamforming.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_BEAMFORMING
 * @brief       SAA o-ran-beamforming
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "u_o-ran-beamforming.h"
#include "i_o-ran-beamforming.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"

/********************************************************************************************************************/
/*  static valuable                                                                                                 */
/********************************************************************************************************************/
uint32 saa_beamforming_max_ant_num;
uint32 saa_beamforming_max_band_num;


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   static-properties取得関数
 * @note    static-propertiesを取得する
 * @param   [in]  *k_hardware_component_nameポインタ
 * @        [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_static_properties_get(  const xmlChar   *target_para_name ,
                                                                                        xmlChar         **para_temp )
{
    status_t        res = NO_ERR;
    char            para_name[D_PARAM_NUM_MAX];
    char            mod_name[]  = i_mod_beamforming_config;
    char            para_nobm[] = i_para_o_ran_beamforming_num_of_beam;
    char            para_rtbf[] = i_para_o_ran_beamforming_rt_bf_weights_update_support;

    if( LOGINFO )
    {
        log_info("¥nEnter %s:", __func__);
    }

    if( !xml_strcmp( y_o_ran_beamforming_N_rt_bf_weights_update_support , target_para_name ))
    {
        snprintf( para_name , D_PARAM_NUM_MAX , "%s" , para_rtbf );
    }
    else if( !xml_strcmp( y_o_ran_beamforming_N_number_of_beams , target_para_name ))
    {
        snprintf( para_name , D_PARAM_NUM_MAX , "%s" , para_nobm );
    }
    else
    {
        if( LOGWARN )
        {
            log_warn("¥n%s: Data Missing, beamforming static_properties para = %s", __func__, target_para_name);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name);
    if(*para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if( LOGWARN )
        {
            log_warn("¥n%s: Operation Failed to get param, beamforming static_properties para = %s", __func__, para_name);
        }
    }
    return res;
}   /* i_o_ran_beamforming_beamforming_config_per_band_config_static_properties_get */

/*!
 * @brief   static-properties取得関数
 * @note    static-propertiesを取得する
 * @param   [in]  *k_hardware_component_nameポインタ
 * @        [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_beamforming_property_get_para(  uint16          beam_id_index ,
                                                                                                                                        const xmlChar   *target_para_name,
                                                                                                                                        xmlChar         **para_temp )
{
    status_t    res = NO_ERR;
    char        para_name[D_PARAM_NUM_MAX];
    char        para_beamfonforming [] = i_para_o_ran_beamforming_band_config;
    char        mod_name[]  = i_mod_beamforming_config;
    char        para_bmtp[] = i_para_o_ran_beamforming_beam_type;
    char        para_gpid[] = i_para_o_ran_beamforming_beam_groups_id;

    if( LOGINFO )
    {
        log_info("¥nEnter %s:", __func__);
    }

    if( !xml_strcmp( y_o_ran_beamforming_N_beam_type , target_para_name ))
    {
        snprintf( para_name , D_PARAM_NUM_MAX , "%s%d%s" , para_beamfonforming , beam_id_index , para_bmtp );
    }
    else if( !xml_strcmp( y_o_ran_beamforming_N_beam_group_id , target_para_name ))
    {
        snprintf( para_name , D_PARAM_NUM_MAX , "%s%d%s" , para_beamfonforming , beam_id_index , para_gpid );
    }
    else
    {
        if( LOGWARN )
        {
            log_warn("¥n%s: Data Missing, beamforming beam-id para = %s", __func__, target_para_name);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name);
    if(*para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
        if( LOGWARN )
        {
            log_warn("¥n%s: Operation Failed to get param, beamforming beam-id para = %s", __func__, para_name);
        }
    }
    return res;
}   /* i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_beamforming_property_get_para */

/*!
 * @brief   static-properties取得関数
 * @note    static-propertiesを取得する
 * @param   [in]  *k_hardware_component_nameポインタ
 * @        [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_beamforming_property_get_para2( uint16          beam_id_index ,
                                                                                                                                        unsigned int    list_counter ,
                                                                                                                                        const xmlChar   *target_para_name,
                                                                                                                                        xmlChar         **para_temp )
{
    status_t    res = NO_ERR;
    char        para_name[D_PARAM_NUM_MAX];
    char        para_beamfonforming [] = i_para_o_ran_beamforming_band_config;
    char        mod_name[]  = i_mod_beamforming_config;
    char        para_beam[] = i_para_o_ran_beamforming_neighbor_beam;

    if( LOGINFO )
    {
        log_info("¥n%s:", __func__);
    }

    if( !xml_strcmp( y_o_ran_beamforming_N_neighbour_beams , target_para_name ))
    {
        snprintf( para_name , D_PARAM_NUM_MAX , "%s%d%s%d" , para_beamfonforming , beam_id_index , para_beam , list_counter );
    }
    else
    {
        if(LOGWARN)
        {
            log_warn("¥n%s: Data Missing, beamforming beam-id para = %s", __func__, target_para_name);
        }
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(mod_name, para_name);
    if(*para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
    }
    return res;
}   /* i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_beamforming_property_get_para2 */

/*!
 * @brief   beam-idのkeyからkey番号の取得を行う
 * @note    
 * @param   [in] k_beamforming_config_per_band_config_band_number
 * @param   [out] curindex
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_get_key_num(
    uint32 *curindex ,
    uint32  max_num_of_use ,
    const xmlChar *temp_key_name)
{
    status_t res = ERR_NCX_DATA_MISSING;
    xmlChar *array_name;
    char    para_name[D_PARAM_NUM_MAX];
    char    i_para_beamform_band_configs[]  = i_para_o_ran_beamforming_band_config;
    char    i_para_beam_id[]                = i_para_o_ran_beamforming_beam_id;
    char    i_mod_beamforming[]             = i_mod_beamforming_config;

    if (LOGINFO) {
        log_info("¥nEnter %s:", __func__);
        log_info("¥n    Parameter curindex=%d,  max_num=%d", *curindex , max_num_of_use );
        log_info("¥n    Parameter key_name=%s", temp_key_name);
    }

    for(uint32 i = 0; i < max_num_of_use; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s", i_para_beamform_band_configs, i, i_para_beam_id);
        array_name = (xmlChar *)f_saa_com_get_init_data(i_mod_beamforming, para_name);
        if (array_name == NULL) {
            return res;
        }
        else if (!xml_strcmp(array_name, temp_key_name)) {
            *curindex = i;
            res = NO_ERR;
            if(LOGDEV1)
            {
                log_dev1("¥n%s: key cmp OK! curindex=%d", __func__, i);
            }
            break;
        }
        if (LOGDEV1) {
            log_dev1("¥n%s: Parameter array_name=%s", __func__, array_name);
        }
    }

    if( *curindex == max_num_of_use )
    {
        if(LOGDEV1)
        {
            log_dev1("¥n%s: Key Check Error!", __func__);
        }
    }
    return res;

}/* i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_get_key_num */

/*!
 * @brief   beam-idの先頭のkey取得を行う
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_get_first_key(
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    char    i_para_beamform_band_configs[]  = i_para_o_ran_beamforming_band_config;
    char    i_para_beam_id[]                = i_para_o_ran_beamforming_beam_id;
    char    i_mod_beamforming[]             = i_mod_beamforming_config;

    if (LOGINFO) {
        log_info("¥nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s", i_para_beamform_band_configs, 0, i_para_beam_id);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_beamforming, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_get_first_key */

/*!
 * @brief   beam-idの指定されたkey取得を行う
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_get_target_key(
    uint32 curindex,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    char    i_para_beamform_band_configs[]  = i_para_o_ran_beamforming_band_config;
    char    i_para_beam_id[]                = i_para_o_ran_beamforming_beam_id;
    char    i_mod_beamforming[]             = i_mod_beamforming_config;

    if (LOGINFO) {
        log_info("¥nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s", i_para_beamform_band_configs, curindex, i_para_beam_id);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_beamforming, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_beamforming_properties_get_target_key */

/*!
 * @brief   band_numberのkeyからkey番号の取得を行う
 * @note    
 * @param   [in] k_beamforming_config_per_band_config_band_number
 * @param   [out] curindex
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_get_key_num(
    uint32 *curindex ,
    uint32  max_num_of_use ,
    const xmlChar * temp_key_name)
{
    status_t res = ERR_NCX_DATA_MISSING;
    xmlChar *array_name;
    char    para_name[D_PARAM_NUM_MAX];
    char    i_para_beamform_per_band_configs[]  = i_para_o_ran_beamforming_per_band_cong;
    char    i_para_beam_number[]                = i_para_o_ran_beamforming_band_number;
    char    i_mod_beamforming[]                 = i_mod_beamforming_config;

    if (LOGINFO) {
        log_info("¥nEnter %s:", __func__);
        log_info("¥n    Parameter curindex=%d,  max_num=%d", *curindex , max_num_of_use );
        log_info("¥n    Parameter key_name=%s", temp_key_name);
    }

    for(uint32 i = 0; i < max_num_of_use; i++) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s", i_para_beamform_per_band_configs, i, i_para_beam_number);
        array_name = (xmlChar *)f_saa_com_get_init_data(i_mod_beamforming, para_name);
        if (array_name == NULL) {
            return res;
        }
        else if (!xml_strcmp(array_name, temp_key_name)) {
            *curindex = i;
            res = NO_ERR;
            if(LOGDEV1)
            {
                log_dev1("¥n%s: key cmp OK! curindex = %d", __func__, i);
            }
            break;
        }
        if (LOGDEV1) {
            log_dev1("¥n%s: Parameter array_name = %s", __func__, array_name);
        }
    }

    if( *curindex == max_num_of_use )
    {
        if(LOGDEV1)
        {
            log_dev1("¥n%s: Key Check Error!", __func__);
        }
    }
    return res;

}/* i_o_ran_beamforming_beamforming_config_per_band_config_get_key_num */

/*!
 * @brief   band_numberの先頭キーを取得する
 * @note    
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_get_first_key(
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    char    i_para_beamform_per_band_configs[]  = i_para_o_ran_beamforming_per_band_cong;
    char    i_para_beam_number[]                = i_para_o_ran_beamforming_band_number;
    char    i_mod_beamforming[]                 = i_mod_beamforming_config;

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s", i_para_beamform_per_band_configs, 0, i_para_beam_number);

    if (LOGINFO) {
        log_info("¥nEnter %s:", __func__);
    }

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_beamforming, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_beamforming_beamforming_config_per_band_config_get_first_key */

/*!
 * @brief   per band configのターゲットキーを取得する
 * @note    
 * @param   [in] curindex
 * @param   [out] key_name_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_get_target_key(
    uint32 curindex,
    xmlChar **key_name_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    char    i_para_beamform_per_band_configs[]  = i_para_o_ran_beamforming_per_band_cong;
    char    i_para_beam_number[]                = i_para_o_ran_beamforming_band_number;
    char    i_mod_beamforming[]                 = i_mod_beamforming_config;

    if (LOGINFO) {
        log_info("¥nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s", i_para_beamform_per_band_configs, curindex, i_para_beam_number);

    *key_name_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_beamforming, para_name);
    if(*key_name_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res;

}/* i_o_ran_beamforming_beamforming_config_per_band_config_get_target_key */

/*!
 * @brief   per band congigのパラメータ設定を行う
 * @note    
 * @param   [in] curindex
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/11/28 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_get_para(
    uint32 curindex,
    uint32 ant_num,
    const xmlChar *target_para_name,
    xmlChar **para_temp)
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    char    i_para_beamform_per_band_configs[]  = i_para_o_ran_beamforming_per_band_cong;
    char    i_para_txarray[]                    = i_para_o_ran_beamforming_txarray;
    char    i_para_rxarray[]                    = i_para_o_ran_beamforming_rxarray;
    char    i_mod_beamforming[]                 = i_mod_beamforming_config;

    if (LOGINFO) {
        log_info("¥nEnter %s:", __func__);
    }

    if(!xml_strcmp(y_o_ran_beamforming_N_tx_array, target_para_name)){
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s%s%d", i_para_beamform_per_band_configs, curindex, i_para_txarray, ".", ant_num);
    }
    else if(!xml_strcmp(y_o_ran_beamforming_N_rx_array, target_para_name)) {
        snprintf(para_name, D_PARAM_NUM_MAX, "%s%d%s%s%d", i_para_beamform_per_band_configs, curindex, i_para_rxarray, ".", ant_num);
    }
    else {
        res = ERR_NCX_DATA_MISSING;
        return res;
    }

    *para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_beamforming, para_name);
    if(*para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_beamforming_beamforming_config_per_band_config_get_para */


/*!
 * @brief   MAXプロパティ数の取得
 * @note    
 * @param   [in] curindex
 * @param   [in] target_para_name
 * @param   [out] para_temp
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2018/12/4 KCN)上本
 */
status_t i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_get_para( xmlChar **para_temp )
{
    status_t res = NO_ERR;
    char para_name[D_PARAM_NUM_MAX];
    char    i_mod_beamforming[]                 = i_mod_beamforming_config;
    char    i_para_beamform_per_band_configs[]  = i_para_o_ran_beamforming_per_band_cong;
    char    i_para_beamform_max_properties[]        = i_para_o_ran_beamforming_max_properties;

    if (LOGINFO) {
        log_info("¥nEnter %s:", __func__);
    }

    snprintf(para_name, D_PARAM_NUM_MAX, "%s0%s", i_para_beamform_per_band_configs , i_para_beamform_max_properties );

    *para_temp = (xmlChar *)f_saa_com_get_init_data(i_mod_beamforming, para_name);
    if(*para_temp == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_beamforming_beamforming_config_per_band_config_beam_information_get_para */

/*!
 * @brief   各種パラメータの初期化を行う
 * @note    
 * @param   [in] void
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2018/12/08 KCN)上本
 */
status_t i_o_ran_beamformaing_init( void )
{
    status_t res = NO_ERR;
    char    *para_temp;
    char    mod_name[]              = i_mod_beamforming_config;
    char    i_para_beam_number[]    = i_para_o_ran_beamforming_band_number_init;
    char    i_para_antena_num[]     = i_para_o_ran_beamforming_max_ant_num;

    if (LOGINFO) {
        log_info("¥nEnter %s:", __func__);
    }

    /* max_ant_numの初期化 */
    para_temp = f_saa_com_get_init_data( mod_name , i_para_antena_num );
    if(para_temp == NULL)
    {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN)
        {
            log_warn("¥n%s: Operation Failed to get param, antena num para = %s", __func__, i_para_antena_num);
        }
        return res;
    }
    else
    {
        sscanf (para_temp, "%d", &saa_beamforming_max_ant_num);
    }
    if (LOGDEV1) {
        log_dev1("¥n%s: saa_beamforming_max_ant_num = %d", __func__, saa_beamforming_max_ant_num);
    }

    /* max_band_numの初期化 */
    para_temp = f_saa_com_get_init_data(mod_name , i_para_beam_number);
    if(para_temp == NULL)
    {
        res = ERR_NCX_OPERATION_FAILED;
        if(LOGWARN) {
            log_warn("¥n%s: Operation Failed to get param, beam num para = %s", __func__, i_para_beam_number);
        }
        return res;
    }
    else
    {
        sscanf (para_temp, "%d", &saa_beamforming_max_band_num);
    }
    if (LOGDEV1) {
        log_dev1("¥n%s: saa_beamforming_max_band_num = %d", __func__, saa_beamforming_max_band_num);
    }

    return res;

} /* i_o_ran_beamformaing_init */

/*!
 * @brief   saa_init_dataの取得
 * @note    
 * @param   [in]  *parameter_name
 * @param   [out] **init_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/12 KCN)南條
 */
status_t i_o_ran_beamforming_saa_init_data_get(char *parameter_name, xmlChar **init_data)
{
    status_t    res = NO_ERR;
    char        i_mod_beamforming[] = i_mod_beamforming_config;

    if (LOGINFO) {
        log_info("¥nEnter %s: parameter_name=%s", __func__, parameter_name);
    }

    *init_data = (xmlChar *)f_saa_com_get_init_data(i_mod_beamforming, parameter_name);
    if(*init_data == NULL) {
        res = ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_beamforming_saa_init_data_get */

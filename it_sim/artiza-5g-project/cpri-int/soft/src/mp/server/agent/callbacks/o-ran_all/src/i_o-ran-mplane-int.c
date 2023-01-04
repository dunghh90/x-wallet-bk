
/*
 * @file    i_o-ran-mplane-int.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_MPLANE_INT
 * @brief       SAA o-ran-mplane-int
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <stdlib.h>
#include <pthread.h>
#include "xml_util.h"
#include "i_o-ran-mplane-int.h"

static char   i_o_ran_mplane_int_num_of_vlan_str[] = i_para_o_ran_mplane_int_num_of_vlan;

struct mplane_int_config_t {
    uint16 searchable_access_vlan_cnt;
    bool   searchable_access_vlans[D_O_RAN_MPLANE_INT_MAX_VLANS];
    uint16 lowest_vlan_id;
    uint16 highest_vlan_id;
};

struct mplane_int_msi_t {
    bool   searchable_access_vlan_flg;
    uint16 searchable_access_vlan_cnt;
    bool   searchable_access_vlans[D_O_RAN_MPLANE_INT_MAX_VLANS];
    bool   lowest_vlan_id_send_flg;
    uint16 lowest_vlan_id;
    bool   highest_vlan_id_send_flg;
    uint16 highest_vlan_id;
    bool call_home_ssh_port_flg;
    uint call_home_ssh_port;
    bool server_ssh_port_flg;
    uint server_ssh_port;
};

/* 現在のconfig値を保持するテーブル */
static struct mplane_int_config_t mplane_int_config_table;
/* MSI送信用テーブル */
static struct mplane_int_msi_t mplane_int_msi_table;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   init関数
 * @note    init処理を実施する
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_init2(void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;
    char param_name[D_PARAM_NUM_MAX] = "";
    char *init_data = 0;
    uint16 searchable_access_vlan = 0;

    /* searchable_access_vlans初期化 */
    memset(&mplane_int_config_table, 0, sizeof(mplane_int_config_table));

    /** saa_nvm_data.confからデータを取得する **/
    /* num_of_seachable_vlan_id */
    res = i_o_ran_mplane_int_get_init_data(i_o_ran_mplane_int_num_of_vlan_str, &init_data);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: %s is NULL", __func__, i_o_ran_mplane_int_num_of_vlan_str);
        }
        return res;
    }
    mplane_int_config_table.searchable_access_vlan_cnt = (uint16)atoi((const char *)init_data);

    /* searchable_access_vlans */
    for(uint16 i = 0; i < mplane_int_config_table.searchable_access_vlan_cnt; i++)
    {
        snprintf(param_name, D_PARAM_NUM_MAX, "%s.%s.%s.%d", y_o_ran_mplane_int_N_mplane_info,
                                                             y_o_ran_mplane_int_N_searchable_mplane_access_vlans_info,
                                                             y_o_ran_mplane_int_N_searchable_access_vlans, i);

        res = i_o_ran_mplane_int_get_init_data(param_name, &init_data);
        if(res != NO_ERR)
        {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: %s is NULL", __func__, param_name);
            }
            return res;
        }
        searchable_access_vlan = (uint16)atoi((const char *)init_data);
        if((searchable_access_vlan >= D_O_RAN_MPLANE_INT_MIN_VLANS) && (searchable_access_vlan <= D_O_RAN_MPLANE_INT_MAX_VLANS))
        {
            mplane_int_config_table.searchable_access_vlans[searchable_access_vlan - 1] = true;
        }
        else
        {
            res = ERR_NCX_BAD_ELEMENT;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: searchable_access_vlan is validateNG. :%u", __func__, searchable_access_vlan);
            }
            return res;
        }
    }

    /* lowest-vlan-id */
    snprintf(param_name, D_PARAM_NUM_MAX, "%s.%s.%s.%s", y_o_ran_mplane_int_N_mplane_info,
                                                         y_o_ran_mplane_int_N_searchable_mplane_access_vlans_info,
                                                         y_o_ran_mplane_int_N_vlan_range,
                                                         y_o_ran_mplane_int_N_lowest_vlan_id);
    res = i_o_ran_mplane_int_get_init_data(param_name, &init_data);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: %s is NULL", __func__, param_name);
        }
        return res;
    }
    mplane_int_config_table.lowest_vlan_id = (uint16)atoi((const char *)init_data);
    res = i_o_ran_mplane_int_vlan_range_validate(mplane_int_config_table.lowest_vlan_id);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: lowest_vlan_id is validateNG. :%u", __func__, mplane_int_config_table.lowest_vlan_id);
        }
        return res;
    }

    /* highest-vlan-id */
    snprintf(param_name, D_PARAM_NUM_MAX, "%s.%s.%s.%s", y_o_ran_mplane_int_N_mplane_info,
                                                         y_o_ran_mplane_int_N_searchable_mplane_access_vlans_info,
                                                         y_o_ran_mplane_int_N_vlan_range,
                                                         y_o_ran_mplane_int_N_highest_vlan_id);
    res = i_o_ran_mplane_int_get_init_data(param_name, &init_data);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: %s is NULL", __func__, param_name);
        }
        return res;
    }
    mplane_int_config_table.highest_vlan_id = (uint16)atoi((const char *)init_data);
    res = i_o_ran_mplane_int_vlan_range_validate(mplane_int_config_table.highest_vlan_id);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: highest_vlan_id is validateNG. :%u", __func__, mplane_int_config_table.highest_vlan_id);
        }
        return res;
    }

    /* lowest <= highestの整合性チェック */
    if(mplane_int_config_table.lowest_vlan_id > mplane_int_config_table.highest_vlan_id)
    {
        /* validate NG */
        res = ERR_NCX_BAD_ELEMENT;
        /* error log */
        if(LOGWARN) {
            log_warn("\nvlan range is not validated.");
            log_warn("\nlowest_vlan_id.  :%u",mplane_int_config_table.lowest_vlan_id);
            log_warn("\nhighest_vlan_id. :%u",mplane_int_config_table.highest_vlan_id);
        }
        return res;
    }

    if (LOGINFO) {
        log_info("\n searchable_access_vlan_cnt : %u", mplane_int_config_table.searchable_access_vlan_cnt);
        log_info("\n lowest_vlan_id  : %u", mplane_int_config_table.lowest_vlan_id);
        log_info("\n highest_vlan_id : %u", mplane_int_config_table.highest_vlan_id);
    }

    /* MSI送信用テーブルの初期化 */
    i_o_ran_mplane_int_msi_table_init();

    return res;
} /* i_o_ran_mplane_int_init2 */

/*!
 * @brief   searchable-access-vlanのvalidate関数 
 * @note    searchable-access-vlanのvalidateを実施する 
 * @param   v_searchable_access_vlan
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_BAD_ELEMENT
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_vlan_range_validate(uint16 vlan_id)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;

    if((vlan_id > D_O_RAN_MPLANE_INT_MAX_VLANS) || (vlan_id < D_O_RAN_MPLANE_INT_MIN_VLANS))
    {
        /* validate NG */
        res = ERR_NCX_BAD_ELEMENT;
        /* error log */
        if(LOGWARN) {
            log_warn("\nvlan_id is not validated.");
        }
    }

    return res;
} /* i_o_ran_mplane_int_vlan_range_validate */

/*!
 * @brief   MSI送信用テーブルの初期化
 * @note    MSI送信用テーブルの初期化を実施する
 * @param   -
 * @return  -
 * @date    2019/03/01 KCN)南條
 */
void i_o_ran_mplane_int_msi_table_init(void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    mplane_int_msi_table.searchable_access_vlan_flg = false;
    mplane_int_msi_table.searchable_access_vlan_cnt = mplane_int_config_table.searchable_access_vlan_cnt;
    memcpy(mplane_int_msi_table.searchable_access_vlans, mplane_int_config_table.searchable_access_vlans, sizeof(mplane_int_msi_table.searchable_access_vlans));
    mplane_int_msi_table.lowest_vlan_id_send_flg = false;
    mplane_int_msi_table.lowest_vlan_id = D_O_RAN_MPLANE_INT_VLAN_INFO_INDEFINITE;
    mplane_int_msi_table.highest_vlan_id_send_flg = false;
    mplane_int_msi_table.highest_vlan_id = D_O_RAN_MPLANE_INT_VLAN_INFO_INDEFINITE;
    mplane_int_msi_table.call_home_ssh_port_flg = false;
    mplane_int_msi_table.call_home_ssh_port = D_O_RAN_MPLANE_INT_SSH_PORT_INDEFINITE;
    mplane_int_msi_table.server_ssh_port_flg = false;
    mplane_int_msi_table.server_ssh_port = D_O_RAN_MPLANE_INT_SSH_PORT_INDEFINITE;

    return;
} /* i_o_ran_mplane_int_msi_table_init */

/*!
 * @brief   searchable-access-vlanのvalidate関数 
 * @note    searchable-access-vlanのvalidateを実施する 
 * @param   v_searchable_access_vlan
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_BAD_ELEMENT
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_searchable_access_vlan_validate(uint16 v_searchable_access_vlan)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;

    if((v_searchable_access_vlan >= D_O_RAN_MPLANE_INT_MIN_VLANS) && (v_searchable_access_vlan <= D_O_RAN_MPLANE_INT_MAX_VLANS))
    {
        /* MSI送信テーブル更新 */
        if(mplane_int_msi_table.searchable_access_vlans[v_searchable_access_vlan-1] == false)
        {
            mplane_int_msi_table.searchable_access_vlan_flg = true;
            mplane_int_msi_table.searchable_access_vlan_cnt++;
            mplane_int_msi_table.searchable_access_vlans[v_searchable_access_vlan-1] = true;
        }
    }
    else
    {
        /* validate NG */
        res = ERR_NCX_BAD_ELEMENT;
        /* error log */
        if(LOGWARN) {
            log_warn("\n v_searchable_access_vlan is not validated. v_searchable_access_vlan : %u.", v_searchable_access_vlan);
        }
    }

    return res;
} /* i_o_ran_mplane_int_searchable_access_vlan_validate */

/*!
 * @brief   lowest-vlan-idのvalidate関数 
 * @note    lowest-vlan-idのvalidateを実施する 
 * @param   v_lowest_vlan_id
 * @return  -
 * @date    2019/03/01 KCN)南條
 */
void i_o_ran_mplane_int_lowest_vlan_id_validate(uint16 v_lowest_vlan_id)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI送信テーブル更新 */
    mplane_int_msi_table.lowest_vlan_id_send_flg = true;
    mplane_int_msi_table.lowest_vlan_id = v_lowest_vlan_id;

    return;
} /* i_o_ran_mplane_int_lowest_vlan_id_validate */

/*!
 * @brief   highest-vlan-idのvalidate関数 
 * @note    highest-vlan-idのvalidateを実施する 
 * @param   v_highest_vlan_id
 * @return  -
 * @date    2019/03/01 KCN)南條
 */
void i_o_ran_mplane_int_highest_vlan_id_validate(uint16 v_highest_vlan_id)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* MSI送信テーブル更新 */
    mplane_int_msi_table.highest_vlan_id_send_flg = true;
    mplane_int_msi_table.highest_vlan_id = v_highest_vlan_id;

    return;
} /* i_o_ran_mplane_int_highest_vlan_id_validate */

/*!
 * @brief   call-home-ssh-portのvalidate関数 
 * @note    call-home-ssh-portのvalidateを実施する 
 * @param   v_call_home_ssh_port
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/06/26 FJT)伊藤
 */
status_t i_o_ran_mplane_int_call_home_ssh_port_validate(uint16 v_call_home_ssh_port)
{
    status_t res = NO_ERR;
    char mod_name[]  = i_mod_o_ran_mplane_int;
    char para_name[] = i_para_o_ran_mplane_int_call_home_ssh_port;
    char        *validate_str;
    uint16  call_home_ssh_port_validate = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate */

    /* validate 文字列取得 */
    validate_str = f_saa_com_get_validate_data(mod_name, para_name);
    if(validate_str == NULL)
    {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s:%d validate_str is NULL", __func__, __LINE__);
        }
        return res;
    }

    /* vlan-id変換(文字列->数値) */
    call_home_ssh_port_validate = (uint16)atoi((const char*)validate_str);

    if(v_call_home_ssh_port != call_home_ssh_port_validate)
    {
        /* validate NG */
        res = ERR_NCX_INVALID_VALUE;
        /* error log */
        log_warn("\n%s: v_call_home_ssh_port is not validated.", __func__);
        log_warn("\n%s: v_call_home_ssh_port=%d", __func__, v_call_home_ssh_port);
        return res;
    }

    /* MSI送信テーブル更新 */
    mplane_int_msi_table.call_home_ssh_port_flg = true;
    mplane_int_msi_table.call_home_ssh_port = (uint16)v_call_home_ssh_port;

    return res;
} /* i_o_ran_mplane_int_call_home_ssh_port_validate */

/*!
 * @brief   server-ssh-portのvalidate関数 
 * @note    server-ssh-portのvalidateを実施する 
 * @param   v_server_ssh_port
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/06/26 FJT)伊藤
 */
status_t i_o_ran_mplane_int_server_ssh_port_validate(uint16 v_server_ssh_port)
{
    status_t res = NO_ERR;
    char mod_name[]  = i_mod_o_ran_mplane_int;
    char para_name[] = i_para_o_ran_mplane_int_server_ssh_port;
    char        *validate_str;
    uint16  call_home_ssh_port_validate = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* validate */

    /* validate 文字列取得 */
    validate_str = f_saa_com_get_validate_data(mod_name, para_name);
    if(validate_str == NULL)
    {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s:%d validate_str is NULL", __func__, __LINE__);
        }
        return res;
    }

    /* vlan-id変換(文字列->数値) */
    call_home_ssh_port_validate = (uint16)atoi((const char*)validate_str);

    if(v_server_ssh_port != call_home_ssh_port_validate)
    {
        /* validate NG */
        res = ERR_NCX_INVALID_VALUE;
        /* error log */
        log_warn("\n%s: v_server_ssh_port is not validated.", __func__);
        log_warn("\n%s: v_server_ssh_port=%d", __func__, v_server_ssh_port);
        return res;
   }

    /* MSI送信テーブル更新 */
    mplane_int_msi_table.server_ssh_port_flg = true;
    mplane_int_msi_table.server_ssh_port = v_server_ssh_port;

    return res;
} /* i_o_ran_mplane_int_server_ssh_port_validate */

/*!
 * @brief   searchable-access-vlanのcommit関数 
 * @note    searchable-access-vlanのcommitを実施する 
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_searchable_access_vlan_commit(void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;

    mplane_int_msi_table.searchable_access_vlan_flg = false;
    /* MSI送信 */
    res = i_o_ran_mplane_int_send_msi();
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI is fail. res=%d", __func__, res);
        }
    }

    return res;
} /* i_o_ran_mplane_int_searchable_access_vlan_commit */

/*!
 * @brief   lowest-vlan-idのcommit関数 
 * @note    lowest-vlan-idのcommitを実施する 
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_lowest_vlan_id_commit(void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;

    mplane_int_msi_table.lowest_vlan_id_send_flg = false;
    /* MSI送信 */
    res = i_o_ran_mplane_int_send_msi();
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI is fail. res=%d", __func__, res);
        }
    }

    return res;
} /* i_o_ran_mplane_int_lowest_vlan_id_commit */

/*!
 * @brief   highest-vlan-idのcommit関数 
 * @note    highest-vlan-idのcommitを実施する 
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_highest_vlan_id_commit(void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;

    mplane_int_msi_table.highest_vlan_id_send_flg = false;
    /* MSI送信 */
    res = i_o_ran_mplane_int_send_msi();
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI is fail. res=%d", __func__, res);
        }
    }

    return res;
} /* i_o_ran_mplane_int_highest_vlan_id_commit */

/*!
 * @brief   call-home-ssh-portのcommit関数 
 * @note    call-home-ssh-portのcommitを実施する 
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_call_home_ssh_port_commit(void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;

    mplane_int_msi_table.call_home_ssh_port_flg = false;
    /* MSI送信 */
    res = i_o_ran_mplane_int_send_msi();
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI is fail. res=%d", __func__, res);
        }
    }

    return res;
} /* i_o_ran_mplane_int_call_home_ssh_port_commit */

/*!
 * @brief   call-home-ssh-portのcommit関数 
 * @note    call-home-ssh-portのcommitを実施する 
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_server_ssh_port_commit(void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;

    mplane_int_msi_table.server_ssh_port_flg = false;
    /* MSI送信 */
    res = i_o_ran_mplane_int_send_msi();
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI is fail. res=%d", __func__, res);
        }
    }

    return res;
} /* i_o_ran_mplane_int_server_ssh_port_commit */

/*!
 * @brief   MSI_O_RAN_MPLANE_INT_EDIT送信関数
 * @note    MSI_O_RAN_MPLANE_INT_EDITを送信する
 * @param   [in] -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_send_msi(void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REQ     edit_data;
    T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REJ     read_data;
    uint16 vlan_cnt = 0;
    int ret = D_MSG_BUFFER_GET_OK;

    /* MSI送信フラグ判定 */
    if( mplane_int_msi_table.searchable_access_vlan_flg ||
        mplane_int_msi_table.lowest_vlan_id_send_flg    ||
        mplane_int_msi_table.highest_vlan_id_send_flg   ||
        mplane_int_msi_table.call_home_ssh_port_flg     ||
        mplane_int_msi_table.server_ssh_port_flg )
    {
        if (LOGINFO) {
            log_info("\n%s:MSI has not been transmitted yet.", __func__);
            log_info("\nWhen all the preparations are completed, it transmits. ");
        }
        return res;
    }

    /* editデータ設定 */
    memset(&edit_data, 0, sizeof(edit_data));
    edit_data.mplane_ssh_port.call_home_ssh_port = mplane_int_msi_table.call_home_ssh_port;
    edit_data.mplane_ssh_port.server_ssh_port    = mplane_int_msi_table.server_ssh_port;
    edit_data.access_vlan_info.lowest_vlan_id    = mplane_int_msi_table.lowest_vlan_id;
    edit_data.access_vlan_info.highest_vlan_id   = mplane_int_msi_table.highest_vlan_id;
    edit_data.access_vlan_info.num_of_vlans      = mplane_int_msi_table.searchable_access_vlan_cnt;
    if (LOGINFO) {
        log_info("\nMSI_O_RAN_MPLANE_INT_EDIT edit data:");
        log_info("\n call_home_ssh_port         : %u", edit_data.mplane_ssh_port.call_home_ssh_port);
        log_info("\n server_ssh_port            : %u", edit_data.mplane_ssh_port.server_ssh_port);
        log_info("\n lowest_vlan_id             : %u", edit_data.access_vlan_info.lowest_vlan_id);
        log_info("\n highest_vlan_id            : %u", edit_data.access_vlan_info.highest_vlan_id);
        log_info("\n searchable_access_vlan_cnt : %u", edit_data.access_vlan_info.num_of_vlans);
    }

    for(USHORT i=0; i < D_O_RAN_MPLANE_INT_MAX_VLANS; i++)
    {
        if(vlan_cnt == edit_data.access_vlan_info.num_of_vlans)
        {
            break;
        }
        if(mplane_int_msi_table.searchable_access_vlans[i] == true)
        {
            if (LOGINFO) {
                log_info("\n searchable_access_vlans    : %u", i+1);
            }
            edit_data.access_vlan_info.searchable_vlan_id[vlan_cnt] = (USHORT)(i+1);
            vlan_cnt++;
        }
    }

    /* MSI_O_RAN_MPLANE_INT_EDIT送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_REQ, sizeof(T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REQ), (void*)&edit_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        /* MSI送信用テーブルの初期化 */
        i_o_ran_mplane_int_msi_table_init();
        return res;
    }

    /* MSI_O_RAN_MPLANE_INT_EDITレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        sizeof(T_MPSW_MSI_O_RAN_MPLANE_INT_EDIT_REJ),
        &message_id,
        &read_size,
        &read_data,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_MPLANE_INT_EDITレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_MPLANE_INT_EDIT_CFM)
        {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed: wrong message id. (%0x).", __func__, message_id);
            }
        }
        else
        {
            /* configテーブルを更新 */
            mplane_int_config_table.searchable_access_vlan_cnt = mplane_int_msi_table.searchable_access_vlan_cnt;
            memcpy(mplane_int_config_table.searchable_access_vlans, mplane_int_msi_table.searchable_access_vlans, sizeof(mplane_int_config_table.searchable_access_vlans));
            if(mplane_int_msi_table.lowest_vlan_id != D_O_RAN_MPLANE_INT_VLAN_INFO_INDEFINITE)
            {
                mplane_int_config_table.lowest_vlan_id = mplane_int_msi_table.lowest_vlan_id;
            }
            if(mplane_int_msi_table.highest_vlan_id != D_O_RAN_MPLANE_INT_VLAN_INFO_INDEFINITE)
            {
                mplane_int_config_table.highest_vlan_id = mplane_int_msi_table.highest_vlan_id;
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
    /* MSI送信用テーブルの初期化 */
    i_o_ran_mplane_int_msi_table_init();

    return res;
} /* i_o_ran_mplane_int_send_msi */

/*!
 * @brief   initデータ取得関数
 * @note    initデータを取得する
 * @param   [in] init_param_name
 * @param   [out] init_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_get_init_data (char *init_param_name, char **init_data)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;
    char mod_name[]  = i_mod_o_ran_mplane_int;
    (void)init_data;

    *init_data = f_saa_com_get_init_data(mod_name, init_param_name);
    /* f_saa_com_get_init_data NGチェック */
    if(*init_data == NULL) {
        /* error */
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: init_data is NULL", __func__);
        }
    }

    return res;
} /* i_o_ran_mplane_int_get_init_data */

/*!
 * @brief   DB更新関数
 * @note    DBを更新する
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_DATA_MISSING
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_mplane_int_send_edit_config (void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;
    char param_name[D_PARAM_NUM_MAX] = "";
    char *init_data = 0;
    uint16 interface_name = 0;
    uint16 sub_interface = 0;
    uint16 call_home_ssh_port = 0;
    uint16 server_ssh_port = 0;

    /* initデータ取得 */
    /* interface-name */
    snprintf(param_name, D_PARAM_NUM_MAX, "%s.%s.%s.%s", y_o_ran_mplane_int_N_mplane_info,
                                                         y_o_ran_mplane_int_N_m_plane_interfaces,
                                                         y_o_ran_mplane_int_N_m_plane_sub_interfaces,
                                                         y_o_ran_mplane_int_N_interface_name);
    res = i_o_ran_mplane_int_get_init_data(param_name, &init_data);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: %s is NULL", __func__, param_name);
        }
        return res;
    }
    interface_name = (uint16)atoi((const char *)init_data);

    /* sub-interface */
    snprintf(param_name, D_PARAM_NUM_MAX, "%s.%s.%s.%s", y_o_ran_mplane_int_N_mplane_info,
                                                         y_o_ran_mplane_int_N_m_plane_interfaces,
                                                         y_o_ran_mplane_int_N_m_plane_sub_interfaces,
                                                         y_o_ran_mplane_int_N_sub_interface);
    res = i_o_ran_mplane_int_get_init_data(param_name, &init_data);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: %s is NULL", __func__, param_name);
        }
        return res;
    }
    sub_interface = (uint16)atoi((const char *)init_data);

    /* port */
    /* call-home-ssh-port */
    snprintf(param_name, D_PARAM_NUM_MAX, "%s.%s.%s.%s", y_o_ran_mplane_int_N_mplane_info,
                                                         y_o_ran_mplane_int_N_m_plane_interfaces,
                                                         y_o_ran_mplane_int_N_m_plane_ssh_ports,
                                                         y_o_ran_mplane_int_N_call_home_ssh_port);
    res = i_o_ran_mplane_int_get_init_data(param_name, &init_data);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: %s is NULL", __func__, param_name);
        }
        return res;
    }
    call_home_ssh_port = (uint16)atoi((const char *)init_data);

    /* server-ssh-port */
    snprintf(param_name, D_PARAM_NUM_MAX, "%s.%s.%s.%s", y_o_ran_mplane_int_N_mplane_info,
                                                         y_o_ran_mplane_int_N_m_plane_interfaces,
                                                         y_o_ran_mplane_int_N_m_plane_ssh_ports,
                                                         y_o_ran_mplane_int_N_server_ssh_port);
    res = i_o_ran_mplane_int_get_init_data(param_name, &init_data);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: %s is NULL", __func__, param_name);
        }
        return res;
    }
    server_ssh_port = (uint16)atoi((const char *)init_data);


    /* send-data作成 */
    char value_buff[D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN] = "";
    char value_buff_tmp[D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN] = "";
    sprintf(value_buff_tmp,
        "/opt/agent/bin/DU_dbapi "
        "subsys_mplaneint "
        "'<config>");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    /* ietf-interfaces */
    if(sub_interface >= D_O_RAN_MPLANE_INT_MIN_VLANS)
    {
        sprintf(value_buff_tmp,
                "<interfaces xmlns=\"urn:ietf:params:xml:ns:yang:ietf-interfaces\">"
                    "<interface>"
                        "<name>m-vlan%u</name>"
                        "<vlan-id xmlns=\"urn:o_ran:interfaces:1.1\">%u</vlan-id>"
                    "</interface>"
                "</interfaces>",
            interface_name, sub_interface);
        strncat(value_buff, value_buff_tmp, (D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));
    }
    sprintf(value_buff_tmp, "<mplane-info xmlns=\"urn:o_ran:mplane-interfaces:1.0\">");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    /* searchable-mplane-access-vlans-info */
    sprintf(value_buff_tmp, "<searchable-mplane-access-vlans-info>");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));
    /* vlan-range */
    sprintf(value_buff_tmp,
            "<vlan-range>"
                "<lowest-vlan-id>%u</lowest-vlan-id>"
                "<highest-vlan-id>%u</highest-vlan-id>"
            "</vlan-range>"
        "</searchable-mplane-access-vlans-info>",
        mplane_int_config_table.lowest_vlan_id,
        mplane_int_config_table.highest_vlan_id);
    strncat(value_buff, value_buff_tmp, (D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    /* m-plane-interfaces */
    sprintf(value_buff_tmp, "<m-plane-interfaces>");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    if(sub_interface >= D_O_RAN_MPLANE_INT_MIN_VLANS)
    {
        sprintf(value_buff_tmp,
                            "<m-plane-sub-interfaces>"
                                "<interface-name>m-vlan%u</interface-name>"
                                "<sub-interface>%u</sub-interface>"
                            "</m-plane-sub-interfaces>",
            interface_name,
            sub_interface);
        strncat(value_buff, value_buff_tmp, (D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));
    }

    /* m-plane-interfaces */
    sprintf(value_buff_tmp,
                        "<m-plane-ssh-ports>"
                            "<call-home-ssh-port>%u</call-home-ssh-port>"
                            "<server-ssh-port>%u</server-ssh-port>"
                        "</m-plane-ssh-ports>"
                    "</m-plane-interfaces>"
                "</mplane-info>"
            "</config>'",
        call_home_ssh_port,
        server_ssh_port);
    strncat(value_buff, value_buff_tmp, (D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    sprintf(value_buff_tmp, " --log-level=off 1>/dev/null 2>/dev/null &");
    strncat(value_buff, value_buff_tmp, (D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN - strlen(value_buff) - 1 ));

    if (LOGINFO) {
        log_info("\n value_buff : %s", value_buff);
    }
    system(value_buff);

    return res;
} /* i_o_ran_mplane_int_send_edit_config */


/*!
 * @brief   MSI_O_RAN_MPLAN_INT_GET送信関数
 * @note    MSI_O_RAN_MPLAN_INT_GETを送信する
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_INVALID_VALUE
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/06/21 FJT)伊藤
 */
status_t i_o_ran_mplain_int_get_send_msi(void *get_data)
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

    /* MSI_O_RAN_MPLAN_INT_GET送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_MPLANE_INT_GET_REQ, 0, NULL);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_MPLAN_INT_GETレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        max(sizeof(T_MPSW_MSI_O_RAN_MPLANE_INT_GET_CFM), sizeof(T_MPSW_MSI_O_RAN_MPLANE_INT_GET_REJ)),
        &message_id,
        &read_size,
        get_data,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_MPLAN_INT_GETレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_MPLANE_INT_GET_CFM) {
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
} /* i_o_ran_mplain_int_get_send_msi */


/*
 * @file    i_o-ran-lbm.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_LBM
 * @brief       SAA o-ran-lbm
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "xml_util.h"
#include "i_o-ran-lbm.h"


#define D_O_RAN_LBM_MAX_NUM              8       /* LBM最大設定数(level:0～7)    */
#define D_O_RAN_LBM_MAX_STR              256     /* 文字列の最大長               */
#define D_O_RAN_LBM_MAX_NAME_STR         44      /* nema文字列の最大長           */
#define D_O_RAN_LBM_LEVEL_INDEFINITE     0xffff  /* md-levelの不定値             */

struct lbm_config_t {
    char id[D_O_RAN_LBM_MAX_STR];
    char name[D_O_RAN_LBM_MAX_NAME_STR];
    USHORT md_level;
};

/* 現在のconfig値を保持するテーブル */
static struct lbm_config_t lbm_config_table[D_O_RAN_LBM_MAX_NUM];

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   lbm_config_tableの初期化
 * @note    lbm_config_tableの初期化
 * @param   v_searchable_access_vlan
 * @return  -
 * @date    2019/03/01 KCN)南條
 */
void i_o_ran_lbm_config_table_init(void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* 初期化 */
    for(int i = 0; i < D_O_RAN_LBM_MAX_NUM; i++)
    {
        lbm_config_table[i].id[0]   = '\0';
        lbm_config_table[i].name[0] = '\0';
        lbm_config_table[i].md_level = D_O_RAN_LBM_LEVEL_INDEFINITE;
    }

    return;
} /* i_o_ran_lbm_config_table_init */

/*!
 * @brief   nameのedit関数 
 * @note    nameのeditを実施する 
 * @param   k_maintenance_domain_id
 * @param   v_name
 * @return  -
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_lbm_maintenance_domain_name_edit(const xmlChar *k_maintenance_domain_id, const xmlChar *v_name, agt_cbtyp_t cbtyp)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;
    
    if(xml_strlen(k_maintenance_domain_id) >= D_O_RAN_LBM_MAX_STR)
    {
        /* validate NG */
        res = ERR_NCX_BAD_ELEMENT;
        /* error log */
        if(LOGWARN) {
            log_warn("\nID length error.");
            log_warn("\nID   : %s",k_maintenance_domain_id);
            log_warn("\nname : %s",v_name);
        }
        return res;
    }
    
    if(xml_strlen(v_name) >= D_O_RAN_LBM_MAX_NAME_STR)
    {
        /* validate NG */
        res = ERR_NCX_BAD_ELEMENT;
        /* error log */
        if(LOGWARN) {
            log_warn("\nname length error.");
            log_warn("\nID   : %s",k_maintenance_domain_id);
            log_warn("\nname : %s",v_name);
        }
        return res;
    }
    
    int count = 0;

    for(count = 0; count < D_O_RAN_LBM_MAX_NUM; count++)
    {
        /* 登録済IDの場合 */
        if(!strcmp(lbm_config_table[count].id, (const char *)k_maintenance_domain_id))
        {
            if(cbtyp == AGT_CB_COMMIT)
            {
                snprintf(lbm_config_table[count].name, D_O_RAN_LBM_MAX_NAME_STR-1, "%s", (const char *)v_name);
                if (LOGINFO) {
                    log_info("\nid       :%s:", lbm_config_table[count].id);
                    log_info("\nname     :%s:", lbm_config_table[count].name);
                    log_info("\nmd_level :%u:", lbm_config_table[count].md_level);
                }
        	}
            break;
        }
        /* 未登録IDの場合 */
        if(lbm_config_table[count].id[0] == '\0')
        {
            if(cbtyp == AGT_CB_COMMIT)
            {
                snprintf(lbm_config_table[count].id, D_O_RAN_LBM_MAX_STR-1, "%s", (const char *)k_maintenance_domain_id);
                snprintf(lbm_config_table[count].name, D_O_RAN_LBM_MAX_NAME_STR-1, "%s", (const char *)v_name);
                if (LOGINFO) {
                    log_info("\nid       :%s:", lbm_config_table[count].id);
                    log_info("\nname     :%s:", lbm_config_table[count].name);
                    log_info("\nmd_level :%u:", lbm_config_table[count].md_level);
                }
            }
            break;
        }
        /* IDが上限に達していた場合 */
        if(count == (D_O_RAN_LBM_MAX_NUM-1))
        {
            /* validate NG */
            res = ERR_NCX_BAD_ELEMENT;
            /* error log */
            if(LOGWARN) {
                log_warn("\nID is upper limit.");
                log_warn("\nID   : %s",k_maintenance_domain_id);
                log_warn("\nname : %s",v_name);
            }
            return res;
        }
    }

    /* md-levelが設定済の場合MSI送信 */
    if(cbtyp == AGT_CB_COMMIT)
    {
        if(lbm_config_table[count].md_level != D_O_RAN_LBM_LEVEL_INDEFINITE)
        {
            /* MSI送信 */
            res = i_o_ran_lbm_send_msi(count);
            if(res != NO_ERR)
            {
                /* error log */
                if(LOGWARN) {
                    log_warn("\n%s: MSI is fail. res=%d", __func__, res);
                }
            }
        }
    }

    return res;
} /* i_o_ran_lbm_maintenance_domain_name_edit */

/*!
 * @brief   md_levelのedit関数 
 * @note    md_levelのeditを実施する 
 * @param   k_maintenance_domain_id
 * @param   v_md_level
 * @return  -
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_lbm_maintenance_domain_md_level_edit(const xmlChar *k_maintenance_domain_id, int32 v_md_level, agt_cbtyp_t cbtyp)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;
    int count = 0;

    for(count = 0; count < D_O_RAN_LBM_MAX_NUM; count++)
    {
        /* 登録済IDの場合 */
        if(!strcmp(lbm_config_table[count].id, (const char *)k_maintenance_domain_id))
        {
            if(cbtyp == AGT_CB_COMMIT)
            {
                lbm_config_table[count].md_level = (USHORT)v_md_level;
                if (LOGINFO) {
                    log_info("\nid       :%s:", lbm_config_table[count].id);
                    log_info("\nname     :%s:", lbm_config_table[count].name);
                    log_info("\nmd_level :%u:", lbm_config_table[count].md_level);
                }
                break;
            }
        }
        /* 未登録IDの場合 */
        if(lbm_config_table[count].id[0] == '\0')
        {
            if(cbtyp == AGT_CB_COMMIT)
            {
                snprintf(lbm_config_table[count].id, D_O_RAN_LBM_MAX_STR, "%s", (const char *)k_maintenance_domain_id);
                lbm_config_table[count].md_level = (USHORT)v_md_level;
                if (LOGINFO) {
                    log_info("\nid       :%s:", lbm_config_table[count].id);
                    log_info("\nname     :%s:", lbm_config_table[count].name);
                    log_info("\nmd_level :%u:", lbm_config_table[count].md_level);
                }
            }
            break;
        }
        /* IDが上限に達していた場合 */
        if(count == (D_O_RAN_LBM_MAX_NUM-1))
        {
            /* validate NG */
            res = ERR_NCX_BAD_ELEMENT;
            /* error log */
            if(LOGWARN) {
                log_warn("\nID is upper limit.");
                log_warn("\nID       : %s",k_maintenance_domain_id);
                log_warn("\nmd-level : %u",v_md_level);
            }
            return res;
        }
    }

    /* MSI送信 */
    if(cbtyp == AGT_CB_COMMIT)
    {
        res = i_o_ran_lbm_send_msi(count);
        if(res != NO_ERR)
        {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI is fail. res=%d", __func__, res);
            }
        }
    }

    return res;
} /* i_o_ran_lbm_maintenance_domain_md_level_edit */

/*!
 * @brief   MSI_O_RAN_LBM_EDIT送信関数
 * @note    MSI_O_RAN_LBM_EDITを送信する
 * @param   [in] count
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/01 KCN)南條
 */
status_t i_o_ran_lbm_send_msi(int count)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;
    uint32_t response_id = 0;
    uint32_t message_id  = 0;
    uint32_t read_size   = 0;
    T_MPSW_MSI_O_RAN_LBM_EDIT_REQ     edit_data;
    T_MPSW_MSI_O_RAN_LBM_EDIT_REJ     read_data;
    int ret = D_MSG_BUFFER_GET_OK;

    /* editデータ設定 */
    memset(&edit_data, 0, sizeof(edit_data));
    memcpy(edit_data.id, lbm_config_table[count].id, D_O_RAN_LBM_MAX_STR);
    memcpy(edit_data.name, lbm_config_table[count].name, D_O_RAN_LBM_MAX_NAME_STR);
    edit_data.md_level = lbm_config_table[count].md_level;
    if (LOGINFO) {
        log_info("\nMSI_O_RAN_LBM_EDIT edit data:");
        log_info("\n id       : %s", edit_data.id);
        log_info("\n name     : %s", edit_data.name);
        log_info("\n md_level : %u", edit_data.md_level);
    }

    /* MSI_O_RAN_LBM_EDIT送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_LBM_EDIT_REQ, sizeof(T_MPSW_MSI_O_RAN_LBM_EDIT_REQ), (void*)&edit_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed: cannot add a request.", __func__);
        }
        return res;
    }

    /* MSI_O_RAN_LBM_EDITレスポンス待ち */
    ret = f_saa_com_get_response(response_id,
        sizeof(T_MPSW_MSI_O_RAN_LBM_EDIT_REJ),
        &message_id,
        &read_size,
        &read_data,
        D_MSG_WAIT_TIME );

    /* MSI_O_RAN_LBM_EDITレスポンス判定 */
    switch (ret) {
    case D_MSG_BUFFER_GET_OK:
        if(message_id != D_MSG_MSI_O_RAN_LBM_EDIT_CFM)
        {
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
} /* i_o_ran_lbm_send_msi */

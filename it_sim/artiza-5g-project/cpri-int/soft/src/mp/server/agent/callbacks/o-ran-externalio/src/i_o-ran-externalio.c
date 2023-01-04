/*
 * @file    i_o-ran-externalio.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_EXTERNALIO
 * @brief       SAA o-ran-externalio
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <stdint.h>

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_ntf_lib.h"
#include "f_saa_msi_lib.h"

#include "u_o-ran-externalio.h"
#include "i_o-ran-externalio.h"


/********************************************************************************************************************/
/*   valuable                                                                                                       */
/********************************************************************************************************************/
static T_MPSW_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND  m_notification_data;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   MSI_O_RAN_EXTERNALIO_GET送信関数
 * @note    
 * @param   [out] get_data
 * @return  [正常]NO_ERR
 * @return  [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/28 KCN)吉木
 */
status_t i_o_ran_externalio_send_msi_get (
    T_MPSW_MSI_O_RAN_EXTERNALIO_GET_CFM  *get_data)
{
    status_t res = NO_ERR;
    uint32_t read_size = 0;
    int ret = D_MSI_CFM;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    ret = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_EXTERNALIO_GET_REQ,
        0,
        NULL,
        D_MSG_MSI_O_RAN_EXTERNALIO_GET_CFM,
        0,
        sizeof(T_MPSW_MSI_O_RAN_EXTERNALIO_GET_CFM),
        (void *)get_data,
        &read_size);

    if(ret != D_MSI_CFM || read_size != sizeof(T_MPSW_MSI_O_RAN_EXTERNALIO_GET_CFM)){
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: ret=%d, size=%d.", __func__, ret, read_size);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;

} /* i_o_ran_externalio_send_msi_get */

/*!
 * @brief   MSI_O_RAN_EXTERNALIO_EDIT送信関数
 * @note    
 * @param   [in] newval_val
 * @return  [正常]NO_ERR
 * @return  [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/03/28 KCN)吉木
 */
status_t i_o_ran_externalio_send_msi_edit (
    boolean  newval_val)
{
    status_t res = NO_ERR;
    uint32_t read_size = 0;
    int ret = D_MSI_CFM;
    T_MPSW_MSI_O_RAN_EXTERNALIO_EDIT_REQ send_data;
    char read_data[D_PARAM_NUM_MAX] = "";

    if (LOGINFO) {
        log_info("\nEnter %s: line-out = %d", __func__, newval_val);
    }

    send_data.port_num = 0; /* ポート番号に0(固定値)を設定 */

    if(newval_val == true){
        send_data.line_out = SAA_EXIO_OPEN;
    }
    else {
        send_data.line_out = SAA_EXIO_CLOSED;
    }

    ret = f_saa_com_do_msi_request(
        D_MSG_MSI_O_RAN_EXTERNALIO_EDIT_REQ,
        sizeof(T_MPSW_MSI_O_RAN_EXTERNALIO_EDIT_REQ),
        (void *)&send_data,
        D_MSG_MSI_O_RAN_EXTERNALIO_EDIT_CFM,
        0,
        0,
        (void *)read_data,
        &read_size);

    if(ret != D_MSI_CFM){
        if(LOGWARN){
            log_warn("\n%s: MSI error occurred: ret=%d, size=%d.", __func__, ret, read_size);
        }
        return ERR_NCX_OPERATION_FAILED;
    }

    return res;
} /* i_o_ran_externalio_send_msi_edit */

/*!
 * @brief   open/closedをbooleanに変換する
 * @note    
 * @param   [in] line_state
 * @param   [out] line_state_bool
 * @return  [正常]NO_ERR
 * @return  [異常]ERR_NCX_INVALID_VALUE
 * @date    2019/03/28 KCN)吉木
 */
status_t i_o_ran_externalio_conv_line_state_to_bool(
    uint32 line_state,
    boolean *line_state_bool)
{

    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s: line_state = %d", __func__, line_state);
    }

    if(line_state == SAA_EXIO_OPEN) {
        *line_state_bool = true;
    }
    else if(line_state == SAA_EXIO_CLOSED) {
        *line_state_bool = false;
    }
    else {
        if(LOGWARN) {
            log_warn("\n%s: input value is invalid, line_state = %d", __func__, line_state);
        }
        return ERR_NCX_INVALID_VALUE;
    }

    return res;

} /* i_o_ran_externalio_conv_line_state_to_bool */

/*!
 * @brief   external-input-changeを送信する
 * @note    
 * @param   [in] data
 * @param   [in] size
 * @return  void
 * @date    2019/03/28 KCN)吉木
 */
void i_o_ran_externalio_external_input_change_handler(
    void *data,
    uint32_t size)
{

    if (LOGINFO) {
        log_info("\nEnter %s: size=%d", __func__, size);
    }

    /* size確認 */
    if(sizeof(T_MPSW_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND) != size) {
        if(LOGWARN) {
            log_warn("\n%s: invalid data size, external-input-change MSI = %d", __func__, size);
        }
        return;
    }

    memcpy(&m_notification_data, data, sizeof(m_notification_data));
    u_o_ran_externalio_external_input_change_send(NULL);

} /* i_o_ran_externalio_external_input_change_handler */

/*!
 * @brief   notificationデータを取得する。
 * @note    
 * @param   なし
 * @return  データ
 * @date    2019/12/23 KCN)中井
 */
T_MPSW_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND *
    i_o_ran_externalio_get_notification_data(void){
    return &m_notification_data;
}

/*!
 * @brief   初期化
 * @note    
 * @param   なし
 * @return  なし
 * @date    2019/12/23 KCN)中井
 */
void i_o_ran_externalio_init2 (void)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    f_saa_com_reg_notification_handler(
        D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND,
        i_o_ran_externalio_external_input_change_handler);

    (void)f_saa_com_add_request(D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND, 0, NULL);

}

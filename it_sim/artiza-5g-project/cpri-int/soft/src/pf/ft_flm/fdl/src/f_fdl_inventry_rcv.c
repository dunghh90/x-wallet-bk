/*!
 * @skip  $ld:$
 * @file  f_fdl_inventry_rcv.c
 * @brief FDLタスク インベントリ情報読み出し処理
 * @date  2019/02/21 KCN)上本 Create
 * @date 2019/06/05 KCN)中井 modify
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_fdl_inc.h"


/*!
 * @brief 関数機能概要:インベントリ情報の読出しを行う
 * @note  関数処理内容.
 *        -# ソフトウェアインストールに必要なインベントリ情報を読み出す
 *        -# 読出し結果をＭＳＩに返却する
 * @param  *rcvMsg_p    [in]    受信メッセージ
 * @return INT
 * @retval D_SYS_OK        0:正常終了
 * @date 2019/02/21 KCN)上本 Create
 * @date 2019/06/05 KCN)中井 modify
 */
INT    f_fdl_inventry_rcv( VOID* rcvMsg_p )
{
    INT    rtn = D_SYS_OK ;

    UCHAR  start_flg    = 0;
    INT    i = 0;
    T_RRH_TRAINVTBL *tbl = NULL;
    T_MPSW_SOFTWARE_INVENTORY *inventory = NULL;
    E_SWM_SLOT_STATUS status = 0;
    UCHAR *build_name = NULL;
    UCHAR *build_version = NULL;

    UINT   messageId = 0;
    UINT   requestId = 0;
    INT    payloadSizePtr = 0;
    VOID * payloadPtr = NULL;
    T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM    response ;

    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_inventry_rcv start.") ;

    memset(&response, 0, sizeof(response));
    tbl = f_cmw_tra_inv_tbl;
    inventory = &response.software_inventory;

    /* MSI受信データのパラメータを取得する。*/
    f_com_MSIgetPayload(rcvMsg_p, &messageId, &requestId, &payloadSizePtr, &payloadPtr );
    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "Message ID : %08x Request ID : %08x", messageId, requestId);


    /* === インベントリ情報を取得する === */

    /* アクティブ状態のEEPROMからの読出し */
    (VOID)BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_START_FLG, &start_flg) ;
    if(start_flg == D_RRH_BOOT_ACT0) {
        inventory->software_slot[0].active = TRUE ;
        inventory->software_slot[1].active = FALSE ;
    }
    else {
        inventory->software_slot[0].active = FALSE ;
        inventory->software_slot[1].active = TRUE ;
    }

    /* 運用面の共有メモリからの読出し */
    if(tbl->cmx_bootup_face == D_RRH_BOOT_ACT0) {
        inventory->software_slot[0].running = TRUE ;
        inventory->software_slot[1].running = FALSE ;
    }
    else {
        inventory->software_slot[0].running = FALSE ;
        inventory->software_slot[1].running = TRUE ;
    }

    /* その他のスロット毎の情報を取得する。 */
    for(i=0; i < E_FDL_SLOT_NUM; i++){

        /* スロット名には内部定義した値を使用 */
        strncpy(inventory->software_slot[i].name,
            f_fdl_Slot_nametbl[i],
            sizeof(inventory->software_slot[i].name) -1);

        /* アクセス種別はE_SWM_SLOT_ACCESS_RW固定 */
        inventory->software_slot[i].access = E_SWM_SLOT_ACCESS_RW ;

        switch( tbl->cmx_slot_status[i] ){
        case D_DU_EEP_STATUS_VALID:
            status = E_SWM_SLOT_STATUS_VALID;
            break;
        case D_DU_EEP_STATUS_INVALID:
            status = E_SWM_SLOT_STATUS_INVALID;
            break;
        default:
            status = E_SWM_SLOT_STATUS_EMPTY ;
            break ;
        }
        inventory->software_slot[i].status = status;

        /* statusがVALIDでない場合はM-Planeで固定値を設定するので
           初期値でよい。*/
        if(status != E_SWM_SLOT_STATUS_VALID) {
            /* これだけは設定する。ファイル数は1固定 */
            inventory->software_slot[i].file_num = 1;
            continue;
        }

        /* product_codeはシステム起動時にグローバル変数 f_com_product_code
           に設定されている。 */
        strncpy(inventory->software_slot[i].product_code,
            f_com_product_code, 
            sizeof(inventory->software_slot[i].product_code) -1);

        strncpy(inventory->software_slot[i].vendor_code,
            E_FDL_VENDOR_CODE, 
            sizeof(inventory->software_slot[i].vendor_code) -1);
        sprintf(inventory->software_slot[i].build_id,
            "%02x%02x",
            tbl->cmx_device_typ, tbl->cmx_device_BW);

        if(i == 0){
            build_name = tbl->cmx_build0_name;
            build_version = (UCHAR *)tbl->cmx_f0_sver;
        }
        else{
            build_name = tbl->cmx_build1_name;
            build_version = (UCHAR *)tbl->cmx_f1_sver;
        }

        strncpy(inventory->software_slot[i].build_name,
            (const char *)build_name,
            sizeof(inventory->software_slot[i].build_name) -1);
        sprintf(inventory->software_slot[i].build_version,
            "%02x%02x",build_version[1], build_version[0]);

        /* --- 以下はスロット内のファイルに関する情報 --- */

        /* ファイル数は1固定 */
        inventory->software_slot[i].file_num = 1;
        /* ファイル名はbuild_nameと同じ */
        strncpy(inventory->software_slot[i].file[0].name,
            inventory->software_slot[i].build_name,
            sizeof(inventory->software_slot[i].file[0].name) -1);
        /* バージョンはbuild_versionと同じ */
        strncpy(inventory->software_slot[i].file[0].version,
            inventory->software_slot[i].build_version,
            sizeof(inventory->software_slot[i].file[0].version) -1);
        /* local_pathは固定  */
        strcpy(inventory->software_slot[i].file[0].local_path,
            E_FDL_LOCAL_PATH);
        /* integrityはOK固定 */
        inventory->software_slot[i].file[0].integrity = E_SWM_SLOT_INTEGRITY_OK ;

    }

    /* === レスポンスを送信する === */

    /* MSI送信処理 */
    (VOID)f_com_MSIsend(
        D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM,
        requestId,
        sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_INVENTROY_GET_CFM),
        &response,
        D_RRH_PROCQUE_PF,
        D_SYS_THDQID_PF_FDL );

    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_inventry_rcv end.") ;
    return rtn ;
}
/* @} */



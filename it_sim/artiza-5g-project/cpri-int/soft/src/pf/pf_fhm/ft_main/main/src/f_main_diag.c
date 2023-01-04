/*!
 * @skip        $ld:$
 * @file        f_main_diag.c
 * @brief       一次診断処理
 * @date        2015/08/04 TDI) satou 新規作成
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/*!
 * @brief       一次診断(DDR3 Memory)
 * @note        0x5555_5555/0xAAAA_AAAAを書き込み後、読出し値が書込み値と一致していることを確認する.
 * @note        ハードソフト仕様書で「空き領域を対象とする(範囲等はソフト一任)」
 *              とあるためグローバル変数で領域を用意.
 * @return      一次診断結果
 * @retval      D_SYS_OK    0:正常
 * @retval      D_SYS_NG    1:異常
 * @date        2015/08/04 TDI) satou 新規作成
 */
static UINT f_main_diag_memAccessCheck(VOID) {
    f_mainw_diagmem = 0x55555555;
    if (0x55555555 != f_mainw_diagmem) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_memAccessCheck] f_mainw_diagmem=%08X", f_mainw_diagmem);
        return D_SYS_NG;
    }

    f_mainw_diagmem = 0xAAAAAAAA;
    if (0xAAAAAAAA != f_mainw_diagmem) {
        BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_memAccessCheck] f_mainw_diagmem=%08X", f_mainw_diagmem);
        return D_SYS_NG;
    }

    return D_SYS_OK;
}

/*!
 * @brief       一次診断(HDLC-Buff)
 * @note        0x5555_5555/0xAAAA_AAAAを書き込み後、読出し値が書込み値と一致していることを確認する.
 * @note        ハードソフト仕様書で「CPRIレイヤ2 DDR受信モードを適用する場合は、
 *              2項 HDLC Rx-Buffのメモリチェックは不要」とあり、受信はDDRで行うため、
 *              HDLC Rx-Buffのメモリチェックは行わない.
 * @return      一次診断結果
 * @retval      D_SYS_OK    0:正常
 * @retval      D_SYS_NG    1:異常
 * @date        2015/08/04 TDI) satou 新規作成
 * @date        2015/09/15 TDI) ikeda レジスタ相違指摘反映
 */
static UINT f_main_diag_hdlcAccessCheck(VOID) {
    UINT data_r;                                            /* read data             */
    UINT data_w;                                            /* write data            */
    UINT rtn_bpf;                                           /* return value from BPF */
    const UINT reg_addr = D_RRH_REG_CM_TX;              /* register address      */
    const UINT test_data[] = { 0x55555555, 0xAAAAAAAA };    /* test data             */
    INT i;                                                  /* for loop              */

    for (i = 0; i < (sizeof(test_data)/sizeof(test_data[0])); i++) {
        data_w = test_data[i];

        rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_addr, &data_w);
        if (BPF_HM_DEVC_COMPLETE != rtn_bpf) {
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_hdlcAccessCheck] BPF_HM_DEVC_REG_WRITE NG(%d, 0x%08X, 0x%08X)", rtn_bpf, reg_addr, data_w);
            return D_SYS_NG;
        }

        rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &data_r);
        if (BPF_HM_DEVC_COMPLETE != rtn_bpf) {
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_hdlcAccessCheck] BPF_HM_DEVC_REG_READ NG(%d, 0x%08X)", rtn_bpf, reg_addr);
            return D_SYS_NG;
        }

        if (data_w != data_r) {
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_hdlcAccessCheck] data_w=%08X, data_r=%08X", data_w, data_r);
            return D_SYS_NG;
        }
    }

    return D_SYS_OK;
}

/*!
 * @brief       一次診断(FPGA)
 * @note        0x5555_5555/0xAAAA_AAAAを書き込み後、読出し値が書込み値と一致していることを確認する.
 * @return      一次診断結果
 * @retval      D_SYS_OK    0:正常
 * @retval      D_SYS_NG    1:異常
 * @date        2015/08/04 TDI) satou 新規作成
 */
static UINT f_main_diag_fpgaAccessCheck(VOID) {
    UINT reg_addr;                                          /* register address      */
    UINT data_r;                                            /* read data             */
    UINT data_w;                                            /* write data            */
    UINT rtn_bpf;                                           /* return value from BPF */
    const UINT test_addr[] =                                /* register address      */
        { D_RRH_REG_CNT_REGCHK, D_RRH_REG_CNTS_REGCHK };
    const UINT test_data[] =                                /* test data             */
        { 0x55555555, 0xAAAAAAAA };
    INT loopReg, loopData;                                  /* for loop              */

    for (loopReg = 0; loopReg < (sizeof(test_addr)/sizeof(test_addr[0])); loopReg++) {
        reg_addr = test_addr[loopReg];

        for (loopData = 0; loopData < (sizeof(test_data)/sizeof(test_data[0])); loopData++) {
            data_w = test_data[loopData];

            rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, reg_addr, &data_w);
            if (BPF_HM_DEVC_COMPLETE != rtn_bpf) {
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_fpgaAccessCheck] BPF_HM_DEVC_REG_WRITE NG(%d, 0x%08X, 0x%08X)", rtn_bpf, reg_addr, data_w);
                return D_SYS_NG;
            }

            rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, reg_addr, &data_r);
            if (BPF_HM_DEVC_COMPLETE != rtn_bpf) {
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_fpgaAccessCheck] BPF_HM_DEVC_REG_READ NG(%d, 0x%08X)", rtn_bpf, reg_addr);
                return D_SYS_NG;
            }

            if (data_w != data_r) {
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_fpgaAccessCheck] data_w=%08X, data_r=%08X", data_w, data_r);
                return D_SYS_NG;
            }
        }
    }

    return D_SYS_OK;
}

/*!
 * @brief       一次診断(MMI-CPLD)
 * @note        0x5555/0xAAAAを書き込み後、読出し値が書込み値と一致していることを確認する.
 * @return      一次診断結果
 * @retval      D_SYS_OK    0:正常
 * @retval      D_SYS_NG    1:異常
 * @date        2015/08/04 TDI) satou 新規作成
 */
static UINT f_main_diag_cpldAccessCheck(VOID) {
    UINT data_r;                                            /* read data             */
    UINT data_w;                                            /* write data            */
    UINT rtn_bpf;                                           /* return value from BPF */
    const UINT reg_addr = D_RRH_REG_CPLD_MMI_CHECK;         /* register address      */
    const UINT test_data[] = { 0x5555, 0xAAAA };            /* test data             */
    INT i;                                                  /* for loop              */

    for (i = 0; i < (sizeof(test_data)/sizeof(test_data[0])); i++) {
        data_w = test_data[i];

        rtn_bpf = BPF_HM_DEVC_MMI_CPLD_WRITE(reg_addr, &data_w);
        if (BPF_HM_DEVC_COMPLETE != rtn_bpf) {
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_cpldAccessCheck] BPF_HM_DEVC_MMI_CPLD_WRITE NG(%d, 0x%08X, 0x%08X)", rtn_bpf, reg_addr, data_w);
            return D_SYS_NG;
        }

        rtn_bpf = BPF_HM_DEVC_MMI_CPLD_READ(reg_addr, &data_r);
        if (BPF_HM_DEVC_COMPLETE != rtn_bpf) {
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_cpldAccessCheck] BPF_HM_DEVC_MMI_CPLD_READ NG(%d, 0x%08X)", rtn_bpf, reg_addr);
            return D_SYS_NG;
        }

        if (data_w != data_r) {
            BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_diag_cpldAccessCheck] data_w=%08X, data_r=%08X", data_w, data_r);
            return D_SYS_NG;
        }
    }

    return D_SYS_OK;
}

/*!
 * @brief       一次診断(メモリ/レジスタチェック)
 * @return      一次診断結果
 * @retval      D_SYS_OK    0:正常
 * @retval      D_SYS_NG    1:異常
 * @date        2015/08/04 TDI) satou 新規作成
 */
UINT f_main_diag(VOID) {
    UINT errcode = D_SYS_OK;

    /* DDR3 Memory */
    if (D_SYS_OK != f_main_diag_memAccessCheck()) {
        errcode = D_SYS_NG;
    }

    /* HDLC-Buff */
    if (D_SYS_OK != f_main_diag_hdlcAccessCheck()) {
        errcode = D_SYS_NG;
    }

    /* FPGA */
    if (D_SYS_OK != f_main_diag_fpgaAccessCheck()) {
        errcode = D_SYS_NG;
    }

    /* MMI-CPLD */
    if (D_SYS_OK != f_main_diag_cpldAccessCheck()) {
        errcode = D_SYS_NG;
    }

    return errcode;
}

/*! @} */

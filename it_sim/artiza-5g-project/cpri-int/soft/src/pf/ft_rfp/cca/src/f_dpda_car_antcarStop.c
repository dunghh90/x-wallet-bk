#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:Txの停波処理
 * @note  関数処理内容.
 *          -# 停波処理を行う
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~3:ANT.D)
 * @return int
 * @date 2019/07/19 FJT)長谷川 Create
 */
int  f_dpda_car_TxantcarStop(unsigned int uiAntNum)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_TxantcarStop called, uiAntNum = %d", uiAntNum);

    unsigned int ret = BPF_HM_DEVC_COMPLETE;

    /* DCALENB */
    ret = BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DPDA_CAL_TX_DCALENB_OFFSET, (0x000000001 << uiAntNum));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
        return D_DPDA_RTN_NG;
    }

    return D_DPDA_RTN_COMPLETE;
}

/*!
 * @brief 関数機能概要:Rxの停波処理
 * @note  関数処理内容.
 *          -# 停波処理を行う
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~3:ANT.D)
 * @return int
 * @date 2019/07/19 FJT)長谷川 Create
 */
int  f_dpda_car_RxantcarStop(unsigned int uiAntNum)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_RxantcarStop called, uiAntNum = %d", uiAntNum);

    unsigned int ret = BPF_HM_DEVC_COMPLETE;
    unsigned int i;
    unsigned int regAddrMap[D_DPDA_RX_ANTCAR_STOP_MAX];
    unsigned int write_data[D_DPDA_RX_ANTCAR_STOP_MAX];

    /* DCALENB */
    ret = BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DPDA_CAL_RX_DCALENB_OFFSET, (0x000000001 << uiAntNum));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
        return D_DPDA_RTN_NG;
    }

    /* 停波設定のレジスタアドレスマップ作成 */
    regAddrMap[0] = 0xA2401184;                     /* RFCALREGSEL */
    regAddrMap[1] = 0xA2402000;                     /* RFCALCOEF[0] */
    regAddrMap[2] = 0xA2402004;                     /* RFCALCOEF[1] */
    regAddrMap[3] = 0xA2402008;                     /* RFCALCOEF[2] */
    regAddrMap[4] = 0xA240200C;                    /* RFCALCOEF[3]   */
    regAddrMap[5] = 0xA2401180;                    /* RFCALENB    */

    /*  停波設定先のWrite Dataマップ作成 */
    write_data[0] = uiAntNum;      /* Ant num   */
    write_data[1] = 0x00000000;    /* 0         */
    write_data[2] = 0x00000000;    /* 0         */
    write_data[3] = 0x00000000;    /* 0         */
    write_data[4] = 0x00000000;   /* 0         */
    write_data[5] = 0x1;          /* 1:補正あり */

    for(i = 0; i < D_DPDA_RX_ANTCAR_STOP_MAX; i++) {
        ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, regAddrMap[i], &write_data[i]);
        if(ret != BPF_HM_DEVC_COMPLETE) {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "antcarStop write ant = %d , regAddrMap[%d]=%d , error=0x%08x",uiAntNum, i, regAddrMap[i], ret);
            return D_DPDA_RTN_NG;
        }
    }

    return D_DPDA_RTN_COMPLETE;
}
/* @} */


/*!
 * @brief 関数機能概要:Txの停波解除処理
 * @note  関数処理内容.
 *          -# 停波解除処理を行う
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~3:ANT.D)
 * @return int
 * @date 2019/09/12 FJT)長谷川 Create
 */
int  f_dpda_car_TxantcarStopOff(unsigned int uiAntNum)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_TxantcarStopOff called, uiAntNum = %d", uiAntNum);

    unsigned int ret = BPF_HM_DEVC_COMPLETE;

    /* DCALENB */
    ret = BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DPDA_CAL_TX_DCALENB_OFFSET, (0x000000001 << uiAntNum));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
        return D_DPDA_RTN_NG;
    }

    /* DCALENB */
    ret = BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DPDA_CAL_TX_DCALENB_OFFSET, (0x000010000 << uiAntNum));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
        return D_DPDA_RTN_NG;
    }

    return D_DPDA_RTN_COMPLETE;
}

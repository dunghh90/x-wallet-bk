
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"

/*!
 * @brief 関数機能概要:CAL位相補正(TX)
 * @note  関数処理内容.
 *          -# CAL位相補正を行う
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(ANT.A~ANT.H)
 * @return int
 * @date 2018/09/10 KCN)南條 Create
 */
int f_dpda_car_setTxCalPhaseCorrection(unsigned int uiCarrier,unsigned int uiAntNum)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_setTxCalPhaseCorrection called, uiAntNum = %d", uiAntNum);

    unsigned int   ret = BPF_HM_DEVC_COMPLETE;

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


/*!
 * @brief 関数機能概要:CAL位相補正(RX)
 * @note  関数処理内容.
 *          -# CAL位相補正を行う
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(ANT.A~ANT.H)
 * @return int
 * @date 2018/09/10 KCN)南條 Create
 */
int f_dpda_car_setRxCalPhaseCorrection(unsigned int uiCarrier,unsigned int uiAntNum)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_setRxCalPhaseCorrection called, uiAntNum = %d", uiAntNum);

    unsigned int   ret = BPF_HM_DEVC_COMPLETE;

    /* DCALENB */
    ret = BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DPDA_CAL_RX_DCALENB_OFFSET, (0x000000001 << uiAntNum));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
        return D_DPDA_RTN_NG;
    }

    /* DCALENB */
    ret = BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DPDA_CAL_RX_DCALENB_OFFSET, (0x000010000 << uiAntNum));
        if(ret != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "DCALENB write error=0x%08x", ret );
        return D_DPDA_RTN_NG;
    }

    return D_DPDA_RTN_COMPLETE;
}

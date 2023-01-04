
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"

/*!
 * @brief 関数機能概要:TXキャリア イネーブル/ディセーブル設定
 * @note  関数処理内容.
 *          -# TXキャリアのイネーブル/ディセーブル設定を行う
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @param  uiEnDis      [in]unsigned int    イネーブル/ディセーブル(1:イネーブル、0:ディセーブル)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_car_TxcarEnDis(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiUnitType, unsigned int uiEnDis)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_TxcarEnDis called, uiAntNum = %d, uiCarrier = %d, uiUnitType = %d, uiEnDis = %d", uiAntNum, uiCarrier, uiUnitType, uiEnDis);

    unsigned int uiBaseAddr;
    unsigned int uiSetAddr;
    unsigned int uiSetData;
    unsigned int uiSetBit;
    int iRtnBpf;

    uiBaseAddr = 0xB1100000;

    uiSetAddr = uiBaseAddr + (0x00000400 * uiAntNum);

    //TXCARENレジスタの現在の設定値を取得
    iRtnBpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, uiSetAddr, &uiSetData);
    if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA read error=0x%08x", iRtnBpf);
        return D_DPDA_RTN_NG;
    }

    //自Crのbit位置に1b:Enable, 0b:Disableを設定
    uiSetBit = 0x00000001 << uiCarrier;
    uiSetData = (uiSetData & (~uiSetBit)) | (uiSetBit * uiEnDis);

    iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr, &uiSetData);
    if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x", iRtnBpf);
        return D_DPDA_RTN_NG;
    }

    //グローバル変数に設定状況を保持
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].CarOnOff = (unsigned short)uiEnDis;
    
    /* BeamID設定時はUse変化しないようにskipする */
    if( f_comw_rf_setup_flg == D_RRH_OFF){
      BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "Skip SV bus USE setting uiAntNum = %d, uiCarrier = %d, uiUnitType = %d, uiEnDis = %d", uiAntNum, uiCarrier, uiUnitType, uiEnDis);
      return D_DPDA_RTN_COMPLETE;
	}
    
    //SVレジスタのUSE値を更新
    if(uiEnDis == 1)
    {
        f_com_SVCtl((E_DU_SVCTL_OPE_USE_A + uiAntNum), D_DU_SVCTL_ON, (D_DU_SVCTL_ON_USE_C0 << uiCarrier));
    }
    else
    {
        if(f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].CarOnOff == 0)
        {
            f_com_SVCtl((E_DU_SVCTL_OPE_USE_A + uiAntNum), D_DU_SVCTL_OFF, (D_DU_SVCTL_ON_USE_C0 << uiCarrier));
        }
    }

    return D_DPDA_RTN_COMPLETE;
}


/*!
 * @brief 関数機能概要:RXキャリア イネーブル/ディセーブル設定
 * @note  関数処理内容.
 *          -# RXキャリアのイネーブル/ディセーブル設定を行う
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @param  uiEnDis      [in]unsigned int    イネーブル/ディセーブル(1:イネーブル、0:ディセーブル)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_car_RxcarEnDis(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiUnitType, unsigned int uiEnDis)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_RxcarEnDis called, uiAntNum = %d, uiCarrier = %d, uiUnitType = %d, uiEnDis = %d", uiAntNum, uiCarrier, uiUnitType, uiEnDis);

    unsigned int uiSetAddr;
    unsigned int uiSetData;
    unsigned int uiSetBit;
    int iRtnBpf;

   uiSetAddr = 0xB1300000 + (0x00000400 * uiAntNum);
    //TXCARENレジスタの現在の設定値を取得
    iRtnBpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, uiSetAddr, &uiSetData);
    if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA read error=0x%08x", iRtnBpf);
        return D_DPDA_RTN_NG;
    }

    //自Crのbit位置に1b:Enable, 0b:Disableを設定
    uiSetBit = 0x00000001 << uiCarrier;
    uiSetData = (uiSetData & (~uiSetBit)) | (uiSetBit * uiEnDis);

    iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr, &uiSetData);
    if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x", iRtnBpf);
        return D_DPDA_RTN_NG;
    }

    //グローバル変数に設定状況を保持
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].CarOnOff = (unsigned short)uiEnDis;

    /* BeamID設定時はUse変化しないようにskipする */
    if( f_comw_rf_setup_flg == D_RRH_OFF ){
      BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "Skip SV bus USE setting uiAntNum = %d, uiCarrier = %d, uiUnitType = %d, uiEnDis = %d", uiAntNum, uiCarrier, uiUnitType, uiEnDis);
      return D_DPDA_RTN_COMPLETE;
	}

    //SVレジスタのUSE値を更新
    if(uiEnDis == 1)
    {
        f_com_SVCtl((E_DU_SVCTL_OPE_USE_A + uiAntNum), D_DU_SVCTL_ON, (D_DU_SVCTL_ON_USE_C0 << uiCarrier));
    }
    else{
        if(f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].CarOnOff == 0)
        {
            f_com_SVCtl((E_DU_SVCTL_OPE_USE_A + uiAntNum), D_DU_SVCTL_OFF, (D_DU_SVCTL_ON_USE_C0 << uiCarrier));
        }
    }

    return D_DPDA_RTN_COMPLETE;
}

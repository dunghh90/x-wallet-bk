
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"


/*!
 * @brief 関数機能概要:パラメータの設定(Tx)
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiAccellId   [in]unsigned int    ACELL_ID
 * @param  uiCcId       [in]unsigned int    CC_ID
 * @param  uiDuPortId   [in]unsigned int    PORT_ID
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @param  uiBw         [in]unsigned int    bandwidth
 *                                          0x0(10M) 0x1(20M) 0x2(30M) 0x3(40M) 0x4(50M) 0x5(60M) 
 *                                          0x6(70M) 0x7(80M) 0x8(90M) 0x9(100M) 0xA(200M) 0xB(400M)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_car_setTxParameter(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiAccellId, unsigned int uiCcId, unsigned int uiDuPortId, unsigned int uiUnitType, unsigned int uiBw)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_setTxParameter called, uiAntNum = %d, uiCarrier = %d, uiAccellId = %08x, uiCcId = %08x, uiDuPortId = %08x, uiUnitType = %d uiBw = 0x%x", uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId, uiUnitType, uiBw);
    
    unsigned int uiSetData;

	if(uiUnitType != E_DPDA_REKIND_5GDU_SUB6){
		switch(uiBw){
			case 0x9:
				uiSetData = 5;
				break;
			case 0xA:
				uiSetData = 6;
				break;
			default:
				return D_DPDA_RTN_NG;
				break;
		}
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2010018, &uiSetData);
	}
	return D_DPDA_RTN_COMPLETE;
}

/*!
 * @brief 関数機能概要:パラメータの設定(Rx)
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiAccellId   [in]unsigned int    ACELL_ID
 * @param  uiCcId       [in]unsigned int    CC_ID
 * @param  uiDuPortId   [in]unsigned int    PORT_ID
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @param  uiBw         [in]unsigned int    bandwidth
 *                                          0x0(10M) 0x1(20M) 0x2(30M) 0x3(40M) 0x4(50M) 0x5(60M) 
 *                                          0x6(70M) 0x7(80M) 0x8(90M) 0x9(100M) 0xA(200M) 0xB(400M)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_car_setRxParameter(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiAccellId, unsigned int uiCcId, unsigned int uiDuPortId, unsigned int uiUnitType, unsigned int uiBw)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_setRxParameter called, uiAntNum = %d, uiCarrier = %d, uiAccellId = %08x, uiCcId = %08x, uiDuPortId = %08x, uiUnitType = %d uiBw = 0x%x", uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId, uiUnitType, uiBw);

    unsigned int uiSetData;
    unsigned int uiSetData2;
    int iRtnBpf;

    if(uiUnitType == E_DPDA_REKIND_5GDU_MMW)
    {
		//mmW設定
		if(uiBw == 0xA){
			uiSetData = 6;
			uiSetData2 = 0x01E07A0D;
		}else{
			uiSetData = 5;
			uiSetData2 = 0x01D07A0D;
			
		}
		iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2200018, &uiSetData);
		iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2204110, &uiSetData2);
		iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2401018, &uiSetData);
		if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x, uiSetData = %08x,uiSetData2 = %08x", iRtnBpf, uiSetData,uiSetData2);
			return D_DPDA_RTN_NG;
		}
	}
	return D_DPDA_RTN_COMPLETE;
}


#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"
#include <math.h>


/*!
 * @brief 関数機能概要:LPFGain調整値の設定(Tx)
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @param  uiBw         [in]unsigned int    bandwidth
 *                                          0x0(10M) 0x1(20M) 0x2(30M) 0x3(40M) 0x4(50M) 0x5(60M) 
 *                                          0x6(70M) 0x7(80M) 0x8(90M) 0x9(100M) 0xA(200M) 0xB(400M)
 * @return int
 * @date 2018/12/07 KCN)高木 Create
 */
int f_dpda_car_setTxLPFGain(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiUnitType, long long int Gain)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s called, uiAntNum = %d, uiCarrier = %d, uiUnitType = %d Gain = %f", __func__,uiAntNum, uiCarrier, uiUnitType, (DL)Gain);
    
    unsigned int uiSetAddr;
    unsigned int uiSetData;

    int iRtnBpf;
    DL	dlGain = (DL)Gain;
    DL	dlMaxPower = 0;

    //LPF-GAIN調整値の設定
	uiSetAddr = D_DU_REG_CA_LPFGAIN0 + (0x400 * uiAntNum) + (0x00000004 * uiCarrier);
	if(Gain == 0xFFFFFFFF){
		if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
		{
			//sub6設定
			uiSetData = 0x00002000;
		}
		else
		{
			//mmW設定
			uiSetData = 0x00001009;
		}
	} else{
		if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6){
            /* module-capability:max-power-per-pa-antennaの値をdlMaxPowerに設定する */
			if(f_cmw_tra_inv_tbl->cmx_device_BW == D_DU_EEP_DEV_FREQ_37G){
				dlMaxPower = 28.5000;
			}else{
				dlMaxPower = 29.9000;
			}
		}else{
				dlMaxPower = 15.2000;
		}
		/* dlGainはfraction-digits 4のため1/10000する*/
		dlGain = dlGain / 10000.0000;

        if(uiUnitType == E_DPDA_REKIND_5GDU_MMW) {
            /* mmwの場合：        4096×10^((gain - (max-power-per-pa-antenna +3 -6 -3))/20)で算出したオフセット値をLPF Gainに設定する */
            uiSetData = (UINT)(4096.0 * pow(10.0, (( dlGain - (dlMaxPower+3.0-6.0-3.0)) / 20.0)));
        }
        else {
            if(f_comw_bf_mode->flag == D_RRH_ON){
                /* Sub6 BF無しの場合: 8192×10^((gain - (max-power-per-pa-antenna + 6-6 ))/20)で算出したオフセット値をLPF Gainに設定する */
                uiSetData = (UINT)(8192.0 * pow(10.0, (( dlGain - (dlMaxPower+6.0-6.0)) / 20.0)));
            }
            else {
                /* Sub BF有りの場合: 8192×10^((gain - (max-power-per-pa-antenna + 6-3 ))/20)で算出したオフセット値をLPF Gainに設定する */
                uiSetData = (UINT)(8192.0 * pow(10.0, (( dlGain - (dlMaxPower+6.0-3.0)) / 20.0)));
            }
        }
    
	    if(uiSetData > 8192){
		    /* 8192を超える場合は8192を設定する。Sub6、mmW共通 */
		    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Over gain setting!!!Set:0x%08x pow:%f max:%f ", uiSetData ,dlGain, dlMaxPower);
		    uiSetData = 8192;	/* fail safe */
	    }
    }

	iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr, &uiSetData);
	if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x, addr = %08x Set:0x%08x", iRtnBpf, uiSetAddr,uiSetData);
		return D_DPDA_RTN_NG;
	}
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s finish Set:0x%08x max:%f val:%f",__func__,uiSetData,dlMaxPower,dlGain);
	return D_DPDA_RTN_COMPLETE;
}


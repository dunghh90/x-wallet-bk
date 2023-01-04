
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"

/*!
 * @brief 関数機能概要:送信帯域、周波数などの設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiNco        [in]unsigned int    設定周波数(kHz単位)
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_car_setTxBwandNco(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiNco, unsigned int uiBw, unsigned int uiUnitType)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_setTxBwandNco called, uiAntNum = %d, uiCarrier = %d, uiNco = %d, uiBw = %d, uiUnitType = %d", uiAntNum, uiCarrier, uiNco, uiBw, uiUnitType);

    unsigned int uiBaseAddr;
    unsigned int uiSetAddr;
    unsigned int uiSetData;
    unsigned char ucCentFreq;
    unsigned char ucCentFreqFlg;
    unsigned int  uiCentFreq;

    int iRtnBpf;
    int iNcoSetData; //NCO設定値はsigned表記

     uiBaseAddr = 0xB1100010;
     uiSetAddr = uiBaseAddr + (0x00000400 * uiAntNum) + (0x00000004 * uiCarrier);
     uiSetData = uiBw & 0x0000000F; //設定値(信号帯域設定値)
     iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr, &uiSetData);
     if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
     {
         BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x, addr = %08x", iRtnBpf, uiSetAddr);
         return D_DPDA_RTN_NG;
     }


     //センター周波数の取得
     iRtnBpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_FLG, &ucCentFreqFlg);
     if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
     {
         BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "EEPOM READ error=0x%08x, addr = %08x", iRtnBpf, 0x041);
         return D_DPDA_RTN_NG;
     }

	if(ucCentFreqFlg == D_RRH_ON){
		uiCentFreq = 0;
         iRtnBpf  = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_OFFSET_1, &ucCentFreq);
         uiCentFreq |= (UINT)ucCentFreq << 24 ;
         iRtnBpf |= BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_OFFSET_2 ,&ucCentFreq);
         uiCentFreq |= (UINT)ucCentFreq << 16;
         iRtnBpf |= BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_OFFSET_3, &ucCentFreq);
         uiCentFreq |= (UINT)ucCentFreq << 8;
         iRtnBpf |= BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_OFFSET_4, &ucCentFreq);
         uiCentFreq |= (UINT)ucCentFreq;
	}else{
		iRtnBpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND1, &ucCentFreqFlg);
		switch(ucCentFreqFlg){
			case D_DU_EEP_DEV_FREQ_45G:
				uiCentFreq = 4500150 ;
				break;
			case D_DU_EEP_DEV_FREQ_37G:
				uiCentFreq = 3600150;
				break;
			case D_DU_EEP_DEV_FREQ_28G:
				uiCentFreq = 27600000;
				break;
			default:
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "NCO Range ucCentFreqFlg = %08x", ucCentFreqFlg);
				return D_DPDA_RTN_NG;
				break;
		}
	}


	//送信帯域設定 TXNCO#
	if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6){
	     iNcoSetData = (int)(((int)uiNco - (int)uiCentFreq) / 15);
	}else{
	     iNcoSetData = (int)(((int)uiNco - (int)uiCentFreq) / 60);
	}
	
	if(iNcoSetData > 16383)
	{
	    iNcoSetData = 16383;
	}
	else if(iNcoSetData < -16384)
	{
	    iNcoSetData = -16384;
	}
	uiSetData = (unsigned int)(iNcoSetData & 0x00007FFF);
	
	//NCO設定値の設定
	uiSetAddr = 0xB1100220 + (0x00000400 * uiAntNum) + (0x00000004 * uiCarrier);
	iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr, &uiSetData);
	if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
	{
	    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x, addr = %08x", iRtnBpf, uiSetAddr);
	    return D_DPDA_RTN_NG;
	}

    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s CentFreq %d inNco:%d adjNCO:%d setNCO:0x%08x", __func__ , uiCentFreq,uiNco,iNcoSetData , uiSetData);


    return D_DPDA_RTN_COMPLETE;
}

/*!
 * @brief 関数機能概要:受信帯域、周波数などの設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiNco        [in]unsigned int    設定周波数(kHz単位)
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_car_setRxBwandNco(unsigned int uiAntNum, unsigned int uiCarrier, unsigned int uiNco, unsigned int uiBw, unsigned int uiUnitType)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_setRxBwandNco called, uiAntNum = %d, uiCarrier = %d, uiNco = %d, uiBw = %d, uiUnitType = %d", uiAntNum, uiCarrier, uiNco, uiBw, uiUnitType);

    unsigned int uiSetAddr;
    unsigned int uiSetData;
    unsigned char ucCentFreq;
    unsigned char ucCentFreqFlg;
    int iRtnBpf;
    int iNcoSetData; //NCO設定値はsigned表記
    unsigned int  uiCentFreq;

    //受信帯域設定 RXBW#
    uiSetAddr = 0xB1300010 + (0x00000400 * uiAntNum) + (0x00000004 * uiCarrier);
    uiSetData = (uiBw << 24) & 0x0F000000; //設定値(信号帯域設定値)
    iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr, &uiSetData);
    if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x, addr = %08x", iRtnBpf, uiSetAddr);
        return D_DPDA_RTN_NG;
    }

    //センター周波数の取得
     iRtnBpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_FLG, &ucCentFreqFlg);
     if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
     {
         BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "EEPOM READ error=0x%08x, addr = %08x", iRtnBpf, 0x041);
         return D_DPDA_RTN_NG;
     }

	if(ucCentFreqFlg == D_RRH_ON){
		uiCentFreq = 0;
         iRtnBpf  = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_OFFSET_1, &ucCentFreq);
         uiCentFreq |= (UINT)ucCentFreq << 24 ;
         iRtnBpf |= BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_OFFSET_2 ,&ucCentFreq);
         uiCentFreq |= (UINT)ucCentFreq << 16;
         iRtnBpf |= BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_OFFSET_3, &ucCentFreq);
         uiCentFreq |= (UINT)ucCentFreq << 8;
         iRtnBpf |= BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_OFFSET_4, &ucCentFreq);
         uiCentFreq |= (UINT)ucCentFreq;
	}else{
		iRtnBpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND1, &ucCentFreqFlg);
		switch(ucCentFreqFlg){
			case D_DU_EEP_DEV_FREQ_45G:
				uiCentFreq = 4500150 ;
				break;
			case D_DU_EEP_DEV_FREQ_37G:
				uiCentFreq = 3600150;
				break;
			case D_DU_EEP_DEV_FREQ_28G:
				uiCentFreq = 27600000;
				break;
			default:
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "NCO Range ucCentFreqFlg = %08x", ucCentFreqFlg);
				return D_DPDA_RTN_NG;
				break;
		}
	}

	//受信帯域設定 RXNCO#
	if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6){
	     iNcoSetData = (int)(((int)uiCentFreq - (int)uiNco ) / 15);
	}else{
	     iNcoSetData = (int)(((int)uiCentFreq - (int)uiNco ) / 60);
	}

	if(iNcoSetData > 8191)
	{
	    iNcoSetData = 8191;
	}
	else if(iNcoSetData < -8192)
	{
	    iNcoSetData = -8192;
	}


    // RXNCO#
	uiSetData = (unsigned int)(iNcoSetData & 0x00003FFF);
    uiSetAddr = 0xB1300200 + (0x00000400 * uiAntNum) + (0x00000004 * uiCarrier);
    iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr, &uiSetData);
      if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
     {
         BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "error=0x%08x, addr = %08x", iRtnBpf, uiSetAddr);
         return D_DPDA_RTN_NG;
     }

    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s CentFreq %d inNco:%d adjNCO:%d setNCO:0x%08x", __func__ , uiCentFreq,uiNco,iNcoSetData , uiSetData);

    return D_DPDA_RTN_COMPLETE;
}

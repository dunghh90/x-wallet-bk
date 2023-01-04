
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"


/*!
 * @brief 関数機能概要:インパルス係数設定
 * @note  関数処理内容.
 *          -# インパルス係数設定を行う
 * @param  uiAntNum         [in]unsigned int    アンテナ番号(ANT.A~ANT.H)
 * @param  uiNumberOfCar    [in]unsigned int    キャリアON数(0~2)
 * @param  uiNco            [in]unsigned int    NCOテーブルサイズ
 * @return int
 * @date 2018/09/10 KCN)南條 Create
 */
int f_dpda_com_setImpulseCoefficient( unsigned int uiAntNum, unsigned int uiNumberOfCar, unsigned int uiNco )
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_com_setImpulseCoefficient called, uiAntNum = %d, uiNumberOfCar = %d, uiNco = %d", uiAntNum, uiNumberOfCar, uiNco);

    unsigned int ret = BPF_HM_DEVC_COMPLETE;
    int NcoF = 0;
    short RealNco[255];
    short ImagNco[255];
    double cos_n;
    double sin_n;
    unsigned int implus_write_data  = 0x00000000;
    unsigned int fpga_addr          = 0x00000000;
    unsigned char ucCentFreq;
    unsigned char ucCentFreqFlg;
    unsigned char ucBand;
    int iRtnBpf;
    unsigned int uiCentFreq = 0;

    double d_uiNco;
    unsigned int icfr_data;
    unsigned int icfra_data;
    unsigned int ecfr_data;

    /* キャリアON数が0の場合は設定無 */
    if(uiNumberOfCar == 0)
    {
        return D_DPDA_RTN_COMPLETE;
    }


     //センター周波数の取得
     iRtnBpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_CFREQ_FLG, &ucCentFreqFlg);
     iRtnBpf |= BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_DEVICE_KIND1, &ucBand);
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
		switch(ucBand){
			case D_DU_EEP_DEV_FREQ_45G:
				uiCentFreq = 4500150 ;
				break;
			case D_DU_EEP_DEV_FREQ_37G:
				uiCentFreq = 3600150;
				break;
			default:
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "NCO Range ucBand = %08x", ucBand);
				return D_DPDA_RTN_NG;
				break;
		}
	}

    d_uiNco = uiNco;

    //NCO設定値の算出
     NcoF = round((d_uiNco - uiCentFreq) / 15);
     if(NcoF > 16383)
     {
         NcoF = 16383;
     }
     else if(NcoF < -16384)
     {
         NcoF = -16384;
     }


    for(int n = -127; n < 128; n++)
    {
        /* cos[n] */
        cos_n = cos(n * 2 * M_PI * (double)NcoF / 32768);
        /* sin[n] */
        sin_n = sin(n * 2 * M_PI * (double)NcoF / 32768);

        /* 2Crインパルス係数 */
        if((uiNumberOfCar-2) == D_DPDA_IMPULSE_TABLE_TYPE_CAR2)
        {
            /* キャリアインパルス応答 */
            /* NCO=xkHz次の係数 Real  */
            RealNco[n+127] = (short)f_dpdar_impulseCoefficient_rre[D_DPDA_IMPULSE_TABLE_TYPE_CAR2][n+127].real;
            /* NCO=xkHz次の係数 Imag  */
            ImagNco[n+127] = (short)f_dpdar_impulseCoefficient_rre[D_DPDA_IMPULSE_TABLE_TYPE_CAR2][n+127].imag;
        }
        /* 1Crインパルス係数 */
        else
        {
            /* キャリアインパルス応答 */
            /* NCO=xkHz次の係数 Real  */
            RealNco[n+127] = (short)round(cos_n * f_dpdar_impulseCoefficient_rre[D_DPDA_IMPULSE_TABLE_TYPE_CAR1_NCO_IS_0][n+127].real) - (sin_n * f_dpdar_impulseCoefficient_rre[D_DPDA_IMPULSE_TABLE_TYPE_CAR1_NCO_IS_0][n+127].imag);
            /* NCO=xkHz次の係数 Imag  */
            ImagNco[n+127] = (short)round(sin_n * f_dpdar_impulseCoefficient_rre[D_DPDA_IMPULSE_TABLE_TYPE_CAR1_NCO_IS_0][n+127].real) - (cos_n * f_dpdar_impulseCoefficient_rre[D_DPDA_IMPULSE_TABLE_TYPE_CAR1_NCO_IS_0][n+127].imag);
        }


        /* インパルス応答更新メモリ変更 */
        /* [b31:b16]=Real[n] */
        /* [b15:b0] =Imag[n] */
        implus_write_data = ((RealNco[n+127] & 0xFFFF) << 16);
        implus_write_data = implus_write_data | (ImagNco[n+127] & 0xFFFF);
        /* ANT:A~D */
        fpga_addr = D_DPDA_IMPULSE_SET_ANT_ATOD_OFFSET + (4 * (n + 127));
        ret = BPF_HM_DEVC_REG_WRITE(0, fpga_addr, &implus_write_data);
        if(ret != BPF_HM_DEVC_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "implus data write ant = %d , n=%d , error=0x%08x",uiAntNum, n, ret);
            return D_DPDA_RTN_NG;
        }
    }


    /* メモリ更新設定 */
    /* ANT:A~D */
        /* CFR */
	fpga_addr = D_DPDA_IMPULSE_SET_CFR_OFFSET;
	implus_write_data = D_DPDA_IMPULSE_SET_BASE_DATA | (0x100 << uiAntNum);
	ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &implus_write_data);
	if(ret != BPF_HM_DEVC_COMPLETE)
	{
	    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CFR Data write ant = %d , error=0x%08x",uiAntNum, ret );
	    return D_DPDA_RTN_NG;
	}
	
	switch(ucBand){
		case D_DU_EEP_DEV_FREQ_45G:
			icfr_data = D_DPDA_IMPULSE_SET_ICFR_DATA_45G;
			icfra_data = D_DPDA_IMPULSE_SET_ICFR_DATA_45G;
			ecfr_data = D_DPDA_IMPULSE_SET_ECFR_DATA_45G;
			break;
		case D_DU_EEP_DEV_FREQ_37G:
			icfr_data = D_DPDA_IMPULSE_SET_ICFR_DATA;
			icfra_data = D_DPDA_IMPULSE_SET_ICFR_DATA;
			ecfr_data = D_DPDA_IMPULSE_SET_ECFR_DATA;
			break;
		default:
			return D_DPDA_RTN_NG;
			break;
	}
	
	
	/* ICFRB */
	fpga_addr = D_DPDA_IMPULSE_SET_ICFRB_OFFSET + (uiAntNum * 0x400);
	ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &icfr_data);
	/* ICFRA */
	fpga_addr = D_DPDA_IMPULSE_SET_ICFRA_OFFSET + (uiAntNum * 0x400);
	ret |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &icfra_data);
	/* ECFR */
	fpga_addr = D_DPDA_IMPULSE_SET_ECFR_OFFSET + (uiAntNum * 0x400);
	ret |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &ecfr_data);

	if(ret != BPF_HM_DEVC_COMPLETE)
	{
	    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CFR Data write ant = %d , error=0x%08x",uiAntNum, ret );
	    return D_DPDA_RTN_NG;
	}

    /* 100usec wait */
    usleep(100);
    return D_DPDA_RTN_COMPLETE;
}

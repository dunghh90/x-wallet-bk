
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"


/*!
 * @brief 関数機能概要:位相補償Table設定(TX)
 * @note  関数処理内容.
 *          -# 関数機能概要:位相補償Table設定(TX)を行う
 * @param  uiCarrier        [in]unsigned int    キャリア番号(sub6.0～1、mmw.0～7)
 * @param  uiCrCentFreq     [in]unsigned int    frequency
 * @param  uiScsn           [in]unsigned int    SCS(sub6.30kHz、mmw.120kHz,240kHz)
 * @param  uiUnitType       [in]unsigned int    装置種別(sub6 or mmw)
 * @param  uiScs            [in]unsigned int    SCS(0~2:未使用 3:120kHz 4:240kHz)
 * @return int
 * @date 2018/09/10 KCN)南條 Create
 */
int f_dpda_car_setTxPhaseCompensationTable(unsigned int uiCarrier, unsigned int uiCrCentFreq, unsigned int uiScsn, unsigned int uiUnitType)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_setTxPhaseCompensationTable called, uiCarrier = %d, uiCrCentFreq = %d, uiScsn = %d, uiUnitType = %d", uiCarrier, uiCrCentFreq, uiScsn, uiUnitType);

    unsigned int ret = BPF_HM_DEVC_COMPLETE;
    unsigned int i;
    unsigned int n_max = 0;
    unsigned int uiSampleNum;
    unsigned int uiNcoSetData;
    int iIData;
    int iQData;
    unsigned int uiWriteIQData;
    unsigned int uiWriteEnable = 0x1;
    unsigned int fpga_addr = 0x00000000;
    unsigned int slot_no = 0;

    //n_maxの設定
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //sub6設定
        //現状scs=30kHzのみなので、scsの値はチェックしない
        n_max = D_SYS_CAR_N_MAX_SUB6_30K;
    }
    else
    {
        //mmw設定
        switch(uiScsn)
        {
        case 0x3: 
            n_max = D_SYS_CAR_N_MAX_MMW_120K;
            break;
        case 0x4: 
            n_max = D_SYS_CAR_N_MAX_MMW_240K;
            break;
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Parameter error uiScsn = %d.", uiScsn );
            return D_DPDA_RTN_NG;
        }
    }

    for(i=0; i<n_max+1;i++)
    {
        /* sub6 */
        if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
        {
            //サンプル数の取得
            /* n = 0~13 */
            if(i < 14)
            {
                uiSampleNum = D_SYS_CAR_SAMPLE_BASE0_SUB6_30K + i * D_SYS_CAR_SAMPLE_INC_SUB6_30K;
            }
            /* n = 14~27 */
            else
            {
                uiSampleNum = D_SYS_CAR_SAMPLE_BASE1_SUB6_30K + (i - 14) * D_SYS_CAR_SAMPLE_INC_SUB6_30K;
            }

            //NCOアドレス算出
            uiNcoSetData = (unsigned int)((((unsigned long long)uiCrCentFreq * uiSampleNum * 6400) / 1000) % D_SYS_CAR_NCO_TABLE_SIZE_SUB6);

            //IQデータの算出
            iIData = (int)round(cos(2 * M_PI * (double)uiNcoSetData / D_SYS_CAR_NCO_TABLE_SIZE_SUB6) * 16384);
            iQData = 0 - (int)round(sin(2 * M_PI * (double)uiNcoSetData / D_SYS_CAR_NCO_TABLE_SIZE_SUB6) * 16384);

            //FPGAの補正テーブルにIQデータ値を格納
            //キャリア番号
            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_SYS_CAR_TX_DPHASFTSEL_OFFSET, &uiCarrier);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA Carrier write error=0x%08x", ret );
                return D_DPDA_RTN_NG;
            }
            // I[b31:b16]、Q[b15:b0]
            uiWriteIQData = (unsigned int)((iIData & 0x0000FFFF) << 16);
            uiWriteIQData = uiWriteIQData | (unsigned int)(iQData & 0x0000FFFF);
            /* slot:0 */
            if(i < 14)
            {
                fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS0_OFFSET_SUB6 + i * 0x4;
                ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &uiWriteIQData);
                if(ret != BPF_HM_DEVC_COMPLETE)
                {
                    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA IQData write n=%d , error=0x%08x", i, ret );
                    return D_DPDA_RTN_NG;
                }
            }
            /* slot:1 */
            else
            {
                fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS1_OFFSET_SUB6 + (i - 14) * 0x4;
                ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &uiWriteIQData);
                if(ret != BPF_HM_DEVC_COMPLETE)
                {
                    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA IQData write n=%d , error=0x%08x", i, ret );
                    return D_DPDA_RTN_NG;
                }
            }
            //Enable
            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_SYS_CAR_TX_DPHASFTENB_OFFSET, &uiWriteEnable);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA Enable write error=0x%08x", ret );
                return D_DPDA_RTN_NG;
            }
        }
        /* mmw */
        else
        {
            //サンプル数の取得
            /* SCS = 120kHz */
            if(uiScsn == 0x3) 
            {
                uiSampleNum = D_SYS_CAR_SAMPLE_BASE_MMW_120K + i * D_SYS_CAR_SAMPLE_INC_MMW_120K;
            }
            /* SCS = 240kHz */
            else
            {
                uiSampleNum = D_SYS_CAR_SAMPLE_BASE_MMW_240K + i * D_SYS_CAR_SAMPLE_INC_MMW_240K;
            }

            //NCOアドレス算出
            uiNcoSetData = (unsigned int)((((unsigned long long)uiCrCentFreq * uiSampleNum * 800) / 1000) % D_SYS_CAR_NCO_TABLE_SIZE_MMW);

            //IQデータの算出
            iIData = (int)round(cos(2 * M_PI * (double)uiNcoSetData / D_SYS_CAR_NCO_TABLE_SIZE_MMW) * 16384);
            iQData = 0 - (int)round(sin(2 * M_PI * (double)uiNcoSetData / D_SYS_CAR_NCO_TABLE_SIZE_MMW) * 16384);

            //FPGAの補正テーブルにIQデータ値を格納
            //キャリア番号
            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_SYS_CAR_TX_DPHASFTSEL_OFFSET, &uiCarrier);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA Carrier write error=0x%08x", ret );
                return D_DPDA_RTN_NG;
            }
            //slot_no計算
            slot_no = i / 14;
            // I[b31:b16]、Q[b15:b0]
            uiWriteIQData = (unsigned int)((iIData & 0x0000FFFF) << 16);
            uiWriteIQData = uiWriteIQData | (unsigned int)(iQData & 0x0000FFFF);
            /* SCS = 120kHz */
            if(uiScsn == 0x3)  
            {
                switch(slot_no)
                {
                case 0:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS0_OFFSET_MMW_120K + i * 0x4;
                    break;
                case 1:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS1_OFFSET_MMW_120K + (i - slot_no * 14) * 0x4;
                    break;
                case 2:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS2_OFFSET_MMW_120K + (i - slot_no * 14) * 0x4;
                    break;
                case 3:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS3_OFFSET_MMW_120K + (i - slot_no * 14) * 0x4;
                    break;
                }
            }
            /* SCS = 240kHz */
            else
            {
                switch(slot_no)
                {
                case 0:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS0_OFFSET_MMW_240K + i * 0x4;
                    break;
                case 1:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS1_OFFSET_MMW_240K + (i - slot_no * 14) * 0x4;
                    break;
                case 2:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS2_OFFSET_MMW_240K + (i - slot_no * 14) * 0x4;
                    break;
                case 3:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS3_OFFSET_MMW_240K + (i - slot_no * 14) * 0x4;
                    break;
                case 4:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS4_OFFSET_MMW_240K + (i - slot_no * 14) * 0x4;
                    break;
                case 5:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS5_OFFSET_MMW_240K + (i - slot_no * 14) * 0x4;
                    break;
                case 6:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS6_OFFSET_MMW_240K + (i - slot_no * 14) * 0x4;
                    break;
                case 7:
                    fpga_addr = D_SYS_CAR_TX_DPCCOEF30KS7_OFFSET_MMW_240K + (i - slot_no * 14) * 0x4;
                    break;
                }
            }
            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &uiWriteIQData);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA IQData write n=%d , error=0x%08x", i, ret );
                return D_DPDA_RTN_NG;
            }
            //Enable
            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_SYS_CAR_TX_DPHASFTENB_OFFSET, &uiWriteEnable);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA Enable write error=0x%08x", ret );
                return D_DPDA_RTN_NG;
            }
        }
    }
    return D_DPDA_RTN_COMPLETE;
}


/*!
 * @brief 関数機能概要:位相補償Table設定(RX)
 * @note  関数処理内容.
 *          -# 関数機能概要:位相補償Table設定(RX)を行う
 * @param  uiCarrier        [in]unsigned int    キャリア番号(sub6.0～1、mmw.0～7)
 * @param  uiCrCentFreq     [in]unsigned int    frequency
 * @param  uiScsn           [in]unsigned int    SCS(sub6.30kHz、mmw.120kHz,240kHz)
 * @param  uiUnitType       [in]unsigned int    装置種別(sub6 or mmw)
 * @return int
 * @date 2018/09/10 KCN)南條 Create
 */
int f_dpda_car_setRxPhaseCompensationTable(unsigned int uiCarrier, unsigned int uiCrCentFreq, unsigned int uiScsn, unsigned int uiUnitType)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_setRxPhaseCompensationTable called, uiCarrier = %d, uiCrCentFreq = %d, uiScsn = %d, uiUnitType = %d", uiCarrier, uiCrCentFreq, uiScsn, uiUnitType);

    unsigned int ret = BPF_HM_DEVC_COMPLETE;
    unsigned int i;
    unsigned int n_max = 0;
    unsigned int uiSampleNum;
    unsigned int uiNcoSetData;
    int iIData;
    int iQData;
    unsigned int uiWriteIQData;
    unsigned int uiWriteEnable = 0x1;
    unsigned int fpga_addr = 0x00000000;
    unsigned int slot_no = 0;

    //n_maxの設定
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //sub6設定
        //現状scs=30kHzのみなので、scsの値はチェックしない
        n_max = D_SYS_CAR_N_MAX_SUB6_30K;
    }
    else
    {
         n_max = D_SYS_CAR_N_MAX_MMW_120K;
    }

    for(i=0; i<n_max+1;i++)
    {
        /* sub6 */
        if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
        {
            //サンプル数の取得
            /* n = 0~13 */
            if(i < 14)
            {
                uiSampleNum = D_SYS_CAR_SAMPLE_BASE0_SUB6_30K + i * D_SYS_CAR_SAMPLE_INC_SUB6_30K;
            }
            /* n = 14~27 */
            else
            {
                uiSampleNum = D_SYS_CAR_SAMPLE_BASE1_SUB6_30K + (i - 14) * D_SYS_CAR_SAMPLE_INC_SUB6_30K;
            }

            //NCOアドレス算出
            uiNcoSetData = (unsigned int)((((unsigned long long)uiCrCentFreq * uiSampleNum * 6400) / 1000) % D_SYS_CAR_NCO_TABLE_SIZE_SUB6);

            //IQデータの算出
            iIData = (int)round(cos(2 * M_PI * (double)uiNcoSetData / D_SYS_CAR_NCO_TABLE_SIZE_SUB6) * 16384);
            iQData = 0 - (int)round(sin(2 * M_PI * (double)uiNcoSetData / D_SYS_CAR_NCO_TABLE_SIZE_SUB6) * 16384);

            //FPGAの補正テーブルにIQデータ値を格納
            //キャリア番号
            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_SYS_CAR_RX_DPHASFTSEL_OFFSET, &uiCarrier);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA Carrier write error=0x%08x", ret );
                return D_DPDA_RTN_NG;
            }
            // I[b31:b16]、Q[b15:b0]
            uiWriteIQData = (unsigned int)((iIData & 0x0000FFFF) << 16);
            uiWriteIQData = uiWriteIQData | (unsigned int)(iQData & 0x0000FFFF);
            /* slot:0 */
            if(i < 14)
            {
                fpga_addr = D_SYS_CAR_RX_DPCCOEF30KS0_OFFSET_SUB6 + i * 0x4;
                ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &uiWriteIQData);
                if(ret != BPF_HM_DEVC_COMPLETE)
                {
                    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA IQData write n=%d , error=0x%08x", i, ret );
                    return D_DPDA_RTN_NG;
                }
            }
            /* slot:1 */
            else
            {
                fpga_addr = D_SYS_CAR_RX_DPCCOEF30KS1_OFFSET_SUB6 + (i - 14) * 0x4;
                ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &uiWriteIQData);
                if(ret != BPF_HM_DEVC_COMPLETE)
                {
                    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA IQData write n=%d , error=0x%08x", i, ret );
                    return D_DPDA_RTN_NG;
                }
            }
            //Enable
            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_SYS_CAR_RX_DPHASFTENB_OFFSET, &uiWriteEnable);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA Enable write error=0x%08x", ret );
                return D_DPDA_RTN_NG;
            }
        }
        /* mmw */
        else
        {
            //サンプル数の取得
            /* SCS = 120kHz */
             uiSampleNum = D_SYS_CAR_SAMPLE_BASE_MMW_120K + i * D_SYS_CAR_SAMPLE_INC_MMW_120K;

            //NCOアドレス算出
            uiNcoSetData = (unsigned int)((((unsigned long long)uiCrCentFreq * uiSampleNum * 800) / 1000) % D_SYS_CAR_NCO_TABLE_SIZE_MMW);

            //IQデータの算出
            iIData = (int)round(cos(2 * M_PI * (double)uiNcoSetData / D_SYS_CAR_NCO_TABLE_SIZE_MMW) * 16384);
            iQData = 0 - (int)round(sin(2 * M_PI * (double)uiNcoSetData / D_SYS_CAR_NCO_TABLE_SIZE_MMW) * 16384);

            //FPGAの補正テーブルにIQデータ値を格納
            //キャリア番号
            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_SYS_CAR_RX_DPHASFTSEL_OFFSET_MMW, &uiCarrier);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA Carrier write error=0x%08x", ret );
                return D_DPDA_RTN_NG;
            }
            //slot_no計算
            slot_no = i / 14;
            // I[b31:b16]、Q[b15:b0]
            uiWriteIQData = (unsigned int)((iIData & 0x0000FFFF) << 16);
            uiWriteIQData = uiWriteIQData | (unsigned int)(iQData & 0x0000FFFF);
            /* SCS = 120kHz */
            switch(slot_no)
            {
            case 0:
                fpga_addr = D_SYS_CAR_RX_DPCCOEF30KS0_OFFSET_MMW_120K + i * 0x4;
                break;
            case 1:
                fpga_addr = D_SYS_CAR_RX_DPCCOEF30KS1_OFFSET_MMW_120K + (i - slot_no * 14) * 0x4;
                break;
            case 2:
                fpga_addr = D_SYS_CAR_RX_DPCCOEF30KS2_OFFSET_MMW_120K + (i - slot_no * 14) * 0x4;
                break;
            case 3:
                fpga_addr = D_SYS_CAR_RX_DPCCOEF30KS3_OFFSET_MMW_120K + (i - slot_no * 14) * 0x4;
                break;
            }

            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &uiWriteIQData);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA IQData write n=%d , error=0x%08x", i, ret );
                return D_DPDA_RTN_NG;
            }
            //Enable
            ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_SYS_CAR_RX_DPHASFTENB_OFFSET_MMW, &uiWriteEnable);
            if(ret != BPF_HM_DEVC_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA Enable write error=0x%08x", ret );
                return D_DPDA_RTN_NG;
            }
        }
    }
    return D_DPDA_RTN_COMPLETE;
}

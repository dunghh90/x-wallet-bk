
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"


/*!
 * @brief 関数機能概要:TDD設定
 * @note  関数処理内容.
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_com_setTddConfig(unsigned int uiUnitType)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_com_setTddConfig called, uiUnitType = %d", uiUnitType);

    unsigned int uiBaseAddr = 0xB1000700;
    unsigned int uiSetData;
    int          iRtnBpf;

	 //sub6のみ設定
	 if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
	 {
	     uiSetData = 0;
	     iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000014, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000010, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000018, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000020, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000028, &uiSetData);

	     uiSetData = 0x00000005;
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000030, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000038, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000034, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x0000003C, &uiSetData);

	     uiSetData = 0x00000080;
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000044, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000040, &uiSetData);

	     uiSetData = 0x00000000;
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000048, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000050, &uiSetData);
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000058, &uiSetData);

	     uiSetData = 0x00001200;
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000064, &uiSetData);
	
	     uiSetData = 0x00007300;         
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000060, &uiSetData);
	
	     uiSetData = 0x01202221;         
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x0000006C, &uiSetData);

	     uiSetData = 0x03303337;         
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000068, &uiSetData);
	
	 }
	 //mmWのみ設定
	 else
	 {
	     uiSetData = 0x00000000;
	     iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000010, &uiSetData);
	     uiSetData = 0x00000004;
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000044, &uiSetData);
	     uiSetData = 0x00000024; 
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000040, &uiSetData);
	     uiSetData = 0x00000004; 
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x0000004C, &uiSetData);
	     uiSetData = 0x00000024; 
	     iRtnBpf |= BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiBaseAddr + 0x00000048, &uiSetData);
	 }
	
	if(iRtnBpf != BPF_HM_DEVC_COMPLETE){
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x", iRtnBpf);
	}
	return D_DPDA_RTN_COMPLETE;
}



/*!
 * @brief 関数機能概要:TDD Enable/Disable設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @param  uiEnDis      [in]unsigned int    イネーブル/ディセーブル(1:イネーブル、0:ディセーブル)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_com_TddAntEnDis(unsigned int uiCarrier, unsigned int uiAntNum, unsigned int uiUnitType, unsigned int uiEnDis)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_com_TddAntEnDis called, uiAntNum = %d, uiUnitType = %d, uiEnDis = %d", uiAntNum, uiUnitType, uiEnDis);

    unsigned int uiSetData; 
    int          iRtnBpf;
    

    //TDDEN設定 Enable時に実施
    if(uiEnDis == 1)
    {
        uiSetData = uiEnDis;
        iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000000, &uiSetData);
        if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Setting TDD Enable NG (ad-0xB1000000, dt-0x%08x, errno=0x%08x", 
                                uiSetData, iRtnBpf);
            return D_DPDA_RTN_NG;
        }
		uiSetData = 0x00000003;
		iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1000050+uiAntNum*4, &uiSetData);
		if(iRtnBpf != BPF_HM_DEVC_COMPLETE) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Setting TDD Enable NG (ad-0x%08x, dt-0x%08x, errno=0x%08x", 
								0xB1000050+uiAntNum*4, uiSetData, iRtnBpf);
			return D_DPDA_RTN_NG;
		}
		if( f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6 ){
			uiSetData = 0x00000000;
			iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1100140+uiAntNum*(0x400), &uiSetData);
			if(iRtnBpf != BPF_HM_DEVC_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Setting TDD Enable NG (ad-0x%08x, dt-0x%08x, errno=0x%08x", 
									0xB1100140+uiAntNum*(0x400), uiSetData, iRtnBpf);
				return D_DPDA_RTN_NG;
			}
		}
		f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[D_SYS_ANT_DL][uiCarrier].CarSleep = E_UP_CAR_ACTIVE_ACTIVE;
    }

    return D_DPDA_RTN_COMPLETE; 
}

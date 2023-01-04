// ytakagi add
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"


/*!
 * @brief �֐��@�\�T�v:TX�L�����A SLEEP�ݒ�
 * @note  �֐��������e:TX�L�����A SLEEP�ݒ�
 * @param  uiAntNum     [in]unsigned int    �A���e�i�ԍ�(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    �L�����A�ԍ�(0~7)
 * @return int
 * @date 2020/01/30 FJT)���J�� Create
 */
int f_dpda_act_txCarrierSleepSettingRequest(unsigned int uiAntNum, unsigned int uiCarrier)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_txCarrierSleepSettingRequest called, uiAntNum = %d, uiCarrier = %d", uiAntNum, uiCarrier);

    int ret = D_DPDA_RTN_COMPLETE;
    /* tx-array-carrier���X���[�v���� ON */

    BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, 0xB1000050+uiAntNum*4, 0x2);

    /* SV���W�X�^��SLEEP�l���X�V(ON)*/
    f_com_SVCtl((E_DU_SVCTL_OPE_USE_A + uiAntNum), D_DU_SVCTL_ON, (D_DU_SVCTL_ON_SLP_C0 << uiCarrier));

    return ret;
}

/*!
 * @brief �֐��@�\�T�v:TX�L�����A SLEEP OFF�ݒ�
 * @note  �֐��������e:TX�L�����A SLEEP OFF�ݒ�
 * @param  uiAntNum     [in]unsigned int    �A���e�i�ԍ�(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    �L�����A�ԍ�(0~7)
 * @return int
 * @date 2020/01/30 FJT)���J�� Create
 */
int f_dpda_act_txCarrierSleepOffSettingRequest(unsigned int uiAntNum, unsigned int uiCarrier)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_txCarrierSleepOffSettingRequest called, uiAntNum = %d, uiCarrier = %d", uiAntNum, uiCarrier);

    int ret = D_DPDA_RTN_COMPLETE;
    /* tx-array-carrier���X���[�v���� OFF */

    BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, 0xB1000050+uiAntNum*4, 0x2);

	f_dpda_com_clearQEC();

    /* SV���W�X�^��SLEEP�l���X�V(OFF)*/
    f_com_SVCtl((E_DU_SVCTL_OPE_USE_A + uiAntNum), D_DU_SVCTL_OFF, (D_DU_SVCTL_ON_SLP_C0 << uiCarrier));

    return ret;
}


/*!
 * @brief �֐��@�\�T�v:RX�L�����A SLEEP�ݒ�
 * @note  �֐��������e:RX�L�����A SLEEP�ݒ�
 * @param  uiAntNum     [in]unsigned int    �A���e�i�ԍ�(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    �L�����A�ԍ�(0~7)
 * @return int
 * @date 2020/01/30 FJT)���J�� Create
 */
int f_dpda_act_rxCarrierSleepSettingRequest(unsigned int uiAntNum, unsigned int uiCarrier)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxCarrierSleepSettingRequest called, uiAntNum = %d, uiCarrier = %d", uiAntNum, uiCarrier);
    int ret = D_DPDA_RTN_COMPLETE;
    /* rx-array-carrier���X���[�v���� ON */

    /* �����Ȃ� */

    return ret;
}

/*!
 * @brief �֐��@�\�T�v:RX�L�����A SLEEP OFF�ݒ�
 * @note  �֐��������e:RX�L�����A SLEEP OFF�ݒ�
 * @param  uiAntNum     [in]unsigned int    �A���e�i�ԍ�(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    �L�����A�ԍ�(0~7)
 * @return int
 * @date 2020/01/30 FJT)���J�� Create
 */
int f_dpda_act_rxCarrierSleepOffSettingRequest(unsigned int uiAntNum, unsigned int uiCarrier)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxCarrierSleepOffSettingRequest called, uiAntNum = %d, uiCarrier = %d", uiAntNum, uiCarrier);
    int ret = D_DPDA_RTN_COMPLETE;
    /* rx-array-carrier���X���[�v���� OFF */

    /* �����Ȃ� */

	f_dpda_com_clearQEC();

    return ret;
}
// ytakagi add end

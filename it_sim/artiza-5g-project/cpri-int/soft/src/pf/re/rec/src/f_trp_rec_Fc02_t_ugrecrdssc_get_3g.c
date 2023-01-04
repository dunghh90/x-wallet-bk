// [16B] add start
/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_ugrecrdssc_get_3g.c
 * @brief  RE�J�[�h���(�Z�N�^��)���擾�X���b�g�ԍ��擾
 * @date   2016/10/06 KCN)Takagi create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE�J�[�h���(�Z�N�^��)���擾�X���b�g�ԍ��擾
 * @note   RE�J�[�h���(�Z�N�^��)���擾�X���b�g�ԍ����擾����
 *         - �X���b�g�ԍ����^�[��
 * @param  -
 * @return �X���b�g�ԍ�
 * @date   2016/10/06 KCN)Takagi 16B�@�\�ǉ�
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_ugrecrdssc_get_3g(                   /* �X���b�g�ԍ�:R */
) {
    USHORT          cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
//    T_REC_SLTLST    slt_lst;                          /* �X���b�g�ԍ����X�g */
    USHORT          slt_lst_idx;                /* �X���b�g�ԍ��C���f�b�N�X */
    USHORT          crdssc_get;
    USHORT          slt_num;                    // �X���b�g��
    
    /* �֐��g���[�X���O */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

//    /**********************************************************************/
//    /* RE���(�Z�N�^��)�Ǘ��e�[�u��(RE�X���b�g���)�X���b�g�ԍ����X�g�擾 */
//    /**********************************************************************/
//    f_trp_rec_Fc02_t_resltlsc_get_3g(                                /* �Ȃ�:R */
//        &slt_lst                                    /* �X���b�g�ԍ����X�g:O */
//    );

    slt_num = f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_no_idx].resltinf.slt_num;

    crdssc_get = f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].crdssc_num;

//    if (slt_lst.num <= crdssc_get) {
    if (slt_num <= crdssc_get) {
        /* ���擾�Ȃ������ꍇ */
        /**************/
        /* NG���^�[�� */
        /**************/

        return CMD_NUM0;
    }

    slt_lst_idx = crdssc_get;

//    return (slt_lst.slt_no[slt_lst_idx]);
    return (f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_no_idx].resltinf.slt_no[slt_lst_idx]);
}
// [16B] add end

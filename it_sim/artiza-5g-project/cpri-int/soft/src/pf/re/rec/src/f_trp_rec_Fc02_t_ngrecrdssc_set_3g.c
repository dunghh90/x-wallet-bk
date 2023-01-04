// [16B] add start
/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_ngrecrdssc_set_3g.c
 * @brief  RE�J�[�h���(�Z�N�^��)�擾NG�X���b�g�ԍ��ݒ�
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
 * @brief  RE�J�[�h���(�Z�N�^��)�擾NG�X���b�g�ԍ��ݒ�
 * @note   RE�J�[�h���(�Z�N�^��)�擾NG�X���b�g�ԍ���ݒ肷��
 *         - �X���b�g�ԍ��ݒ�
 * @param  slt_no [in] �X���b�g�ԍ�
 * @return None
 * @date   2016/10/06 KCN)Takagi 16B�@�\�ǉ�
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_ngrecrdssc_set_3g(                             /* �Ȃ�:R */
    USHORT          slt_no                                /* �X���b�g�ԍ�:I */
) {
    USHORT          cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
    UINT            uit_code;

    /* �֐��g���[�X���O */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************************************************/
    /* RE���(�Z�N�^��)�Ǘ��e�[�u��(RE�J�[�h���)������ */
    /****************************************************/
    uit_code = f_trp_rec_Fc02_t_recrdssc_i_ini_3g(                   /* �ω�:R */
        D_REC_INK_RNG,                                    /* �X���b�g�ԍ�:I */
        slt_no                                      /* L3�o�b�t�@�|�C���^:I */
    );
//    if (uit_code == D_REC_RFBSTS_CHG) {
//        /***********************************/
//        /* RE Function Block Status change */
//        /***********************************/
//        f_trp_rec_Fc02_t_refbstsntc_snd(                         /* �Ȃ�:R */
//            CMD_OFF                                        /* Own Notice:I */
//        );

    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].crdssc_num++;

    return;
}
// [16B] add end

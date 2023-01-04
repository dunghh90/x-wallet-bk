/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_l3_LogSave.c
 * @brief  L3���b�Z�[�W(�v��/����)����M���O�̑ޔ�
 * @date   2010/07/22 FJT)Taniguchi create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2011
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  L3���b�Z�[�W(�v��/����)����M���O�̑ޔ�
 * @note   L3���b�Z�[�W(�v��/����)����M���O�̑ޔ�
 * @param  logkind       [in] Log kind(D_REC_LOGKIND_CARRIER/D_REC_LOGKIND_CARD)
 * @param  linkno_offset [in] Link no offset(0�`5)
 * @param  savekind      [in] save kind(D_REC_SAVEKIND_NOMAL/D_REC_SAVEKIND_TIMEOUT/D_REC_SAVEKIND_TIMESTOPNG)
 * @return None
 * @date   2010/07/22 FJT)Taniguchi create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_l3_LogSave( USHORT logkind, USHORT linkno_offset, USHORT savekind )
{
	T_REC_REQRSP_TBL*			tbl_p;
	T_REC_REQRSP_SAVE_TBL*		savetbl_p;
	USHORT						SaveCount;

	tbl_p = &f_trp_rec_Wcom_ReqRspLog_tbl[logkind][linkno_offset];
	savetbl_p = &f_trp_rec_Wcom_ReqRspLogSave_tbl[logkind][linkno_offset];

	/*	�ޔ����R�[�h�J�E���^�擾	*/
	SaveCount = savetbl_p->SaveCount;

	/*	����M���O�̑ޔ�����	*/
	savetbl_p->SaveKind[SaveCount] = savekind;
	f_trp_com_Fmemcpy(
		&(savetbl_p->SaveData[SaveCount]),
		tbl_p,
		sizeof(T_REC_REQRSP_TBL) );

	/*	�ޔ����R�[�h�J�E���^�C���N�������g	*/
	savetbl_p->SaveCount ++;
	if(savetbl_p->SaveCount >= D_REC_SAVECOUNT_MAX)
	{
		savetbl_p->SaveCount = 8;
	}

	return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

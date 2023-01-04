/*!
 * @skip 		$ld:$
 * @file 		f_dhcp_anlz.c
 * @brief		pf_dhcp�X���b�h ��M���b�Z�[�W��͏���
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dhcp_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_anlz
 * @note		��M���b�Z�[�W���C�x���g�ԍ��ƃX���b�h��Ԃ��珈���֐������s����
 *				-# ��M���b�Z�[�W���C�x���g�ԍ��ُ햔�̓X���b�h��Ԉُ�̏ꍇ�A�A�{�[�g���������{����(f_dhcp_abort())
 *				-# ��M���b�Z�[�W�����ُ�̏ꍇ�AAssert���O���������{����(f_dhcp_assert))
 *				-# �C�x���g�ԍ��ƃX���b�h��Ԃ�Index�Ƃ����W�����v�e�[�u�����Y��������Call����
 * @param		*msgP			[in]VOID	��M���b�Z�[�W
 * @retval		D_SYS_OK		0:����I��
 * @retval		D_SYS_NG		1:�ُ�I��
 * @return		INT				���s����
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT	f_dhcp_anlz(VOID*	msgP)
{
	INT								rtn = D_SYS_NG;						/* ���A�l				*/
	UINT							evtNo;								/* �C�x���g�ԍ�			*/

	/* ��Mmsg�`�F�b�N	*/
	switch( ((T_SYS_COMMON_THDIF *)msgP)->uiEventNo ) {
		case	D_API_MSGID_MPN_MPLANE_READY_REQ:						/* M-Plane�ڑ������v��	*/
			evtNo = D_DHCP_EVT_MPLANE_CNCT_READY;
			break;

		case	D_SYS_MSGID_MSI_RCV_IND:								/* MSI��M�ʒm			*/
			evtNo = D_DHCP_MSGID_MSI_RCV_IND;
			break;

		case	D_SYS_MSGID_NMA_VLAN_WRITE_RSP:							/* serchable-vlans�����݉���	*/
			evtNo = D_DHCP_MSGID_NMA_VLAN_WRITE_RSP;
			break;

		default:														/* �N���v���ُ�			*/
			/* Assert���O�o��	*/
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 				/* Assert���x��			*/
								"Input Event Error, evt=%08x", 			/* message				*/
								((T_SYS_COMMON_THDIF *)msgP)->uiEventNo );	/* Information		*/
			/* Abort���� : �^�X�N�ԃ��b�Z�[�W�ُ�	*/
			f_com_abort( D_RRH_PROCID_PF, D_DU_ALMCD_TIF );
			return	rtn;												/* �ُ�̂��ߏ����I��	*/
	}

	/* Thread��ԃ`�F�b�N	*/
	if( f_dhcpw_thdInfo.thdState >= D_DHCP_EVENTNUM ) {

		/* Assert���O�o��	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 					/* Assert���x��			*/
							"Thread Status Error, sta=%d",				/* message				*/
							f_dhcpw_thdInfo.thdState );					/* Information			*/

		/* Abort���� : Thread��Ԉُ�	*/
		f_com_abort( D_RRH_PROCID_PF, D_DU_ALMCD_TKST );
		return	rtn;													/* �ُ�̂��ߏ����I��	*/
	}

	/* �}�g���N�X�֐��R�[��	*/
	rtn = (*(f_dhcpr_callSetRell_mtxtbl[f_dhcpw_thdInfo.thdState][evtNo].eventProcess_addr))(msgP);

	/* �}�g���N�X�֐��������ʃ`�F�b�N	*/
	if( rtn != D_SYS_OK ) {

		/* Assert���O�o��	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 					/* Assert���x��			*/
							"Mtrx Func Result Error, rtn=%d, sta=%d, evt=%d", 	/* message		*/
							rtn, f_dhcpw_thdInfo.thdState, evtNo );		/* Information			*/
	}
	return	rtn;
}

/* @} */

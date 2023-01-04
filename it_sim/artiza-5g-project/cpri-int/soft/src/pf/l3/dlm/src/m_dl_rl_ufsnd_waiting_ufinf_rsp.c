/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufsnd_waiting_ufinf_rsp.c
 * @brief  the processing for "�^�p���t�@�C�����񍐉���" receiving.
 * @date   2015/08/20 FPT)VietCV Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */

/*!
 *  @brief  the processing for "�^�p���t�@�C�����񍐉���" receiving
 *  ��REC��ԑJ��: �^�p���t�@�C���f�[�^���M(FW)�҂� state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/25 FPT)DuongCD Create
 */

VOID m_dl_rl_ufsnd_waiting_ufinf_rsp(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rl_ufsnd_waiting_ufinf_rsp] ENTER" );
	
	/* ��RE��ԑJ��(REC�哱)�֓��͂��� */
	m_dl_rec_main(bufp, cpri_no);
	
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rl_ufsnd_waiting_ufinf_rsp] RETURN" );
	return;
}
/** @} */

/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fsnd_waiting_ufwrep_req.c
 * @brief  Event: �^�p���t�@�[���E�F�A�t�@�C���񍐗v��, Status: �t�@�C���f�[�^���M��(FW)
 * @date   2015/08/22 FPT)Khiemnv1 Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */

/*!
 *  @brief  Event: �^�p���t�@�[���E�F�A�t�@�C���񍐗v��, Status: �t�@�C���f�[�^���M��(FW)
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/09/24 FPT)Yen update
 */


VOID    m_dl_fhm_fsnd_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no)
{
	USHORT		lw_REFW_index = 0;
	INT			llErrcd;
	UINT		lwTimerId;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_fsnd_waiting_ufwrep_req] ENTER" );

	/* �t�@�C���f�[�^���M�Ԋu�^�C�}��~ */
	lwTimerId = CMD_TIMID_FILEDATSND + cpri_no -1;
	cm_TStop( lwTimerId, &llErrcd );

	/* RE�t�@�C���X�V����(NG)���M */
	m_dl_UpdateExpandREDowloadingFile(bufp, cpri_no);

	lw_REFW_index = convert_CPRI_no_REFW_index(cpri_no);
	set_RE_status(lw_REFW_index, DLD_RESTA_IDLE);

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_fsnd_waiting_ufwrep_req] RETURN" );
}
/* @} */
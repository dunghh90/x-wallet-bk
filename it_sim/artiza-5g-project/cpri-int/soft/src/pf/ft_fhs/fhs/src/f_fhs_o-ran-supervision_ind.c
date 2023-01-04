/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_fhs_o-ran-supervision_ind.c
 *  @brief MSI_O_RAN_SUPERVISION_NTF_START_IND function
 *  @date   2019/07/09 FJT)Taniguchi
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_FHS
* @{ */

#include "f_fhs_inc.h"


/* @{ */
/**
* @brief      MSI_O_RAN_SUPERVISION_NTF_START_IND function
* @note       MSI_O_RAN_SUPERVISION_NTF_START_IND flag set.\n
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     result of procedure
* @date       2019/07/09 FJT)Taniguchi
* @warning    N/A
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*/
INT f_fhs_o_ran_supervision_ind( VOID *req, UINT w_requestId )
{
	f_comw_mp_supervison_flg = D_RRH_ON;
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Starting M-plane Supervision(0x%x)...",w_requestId );
	BPF_COM_LOG_DMESG("Starting M-plane Supervision(0x%x)...\n",w_requestId );
	return	D_RRH_OK;
}
/* @} */



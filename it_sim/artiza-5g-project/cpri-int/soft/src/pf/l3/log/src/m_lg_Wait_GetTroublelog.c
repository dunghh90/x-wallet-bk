/*!
 * @skip   $Id$
 * @file   m_lg_Wait_GetTroublelog.c
 * @brief  the processing after receiving RE trouble log get request(the state is log getting).
 * @date   2008/07/29 FFCS)Wuh Create for eNB-009-004.
 * @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) 
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  the function processing after receiving RE trouble log get request(the state is log getting).
 *  @note   This function is processed as follows.
 *          1) send RE trouble log get response(NG)
 *          2) don't change the state
 *  @param  *a_rcv_adr [in]  the received message
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return VOID
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *  @date   2008/12/18 FFCS)chenwei modify for M-S3G-eNBPF-01029
 *  @date   2013/11/04 ALPHA)yokoyama modiry for ZYNQ
 */

VOID m_lg_Wait_GetTroublelog( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
	cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "m_lg_Wait_GetTroublelog enter");
	
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
	m_lg_makeTroubleLog(a_rcv_adr,a_rec_typ);
	
	cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "m_lg_Wait_GetTroublelog return");
}

/* @} */


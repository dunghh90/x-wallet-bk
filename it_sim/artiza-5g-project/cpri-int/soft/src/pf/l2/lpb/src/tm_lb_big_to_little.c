/*!
 * @skip    $Id$
 * @file    tm_lb_big_to_little.c
 * @brief   Big endian to little endian
 *  @date   2013/11/18 FFCS)hongj Create for zynq
 * 
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"			/* RRH common head file		       	*/
#include "l2_com_inc.h"



/********************************************************************************************************************/
/*!
 *  @brief  Big endian to little endian
 *  @note   Big endian to little endian
 *  @param  *a_inf_p [in]  Pointer of buffer
 *  @retval -
 *  @date   2013/11/18 FFCS)hongj Create for zynq
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
 */
 /********************************************************************************************************************/
void tm_lb_big_to_little(UINT * a_inf_p)
{
	L2_COM_CMT_CPRIF_MAPPING * mapping;
	USHORT signal_kind;
	CHAR *body;
	UINT l3_len;

	mapping = (L2_COM_CMT_CPRIF_MAPPING *)a_inf_p;
	signal_kind = l2_com_swap_USHORT(mapping->m2.signal_kind);
	body = mapping->m2.body;

	l3_len = mapping->head.uiLength - sizeof(mapping->head) - sizeof(mapping->m);

	l2_com_swap_convert(mapping->m.cprircv_inf.link_num, signal_kind, body, 'R', l3_len);

	return;
}


/* @} */


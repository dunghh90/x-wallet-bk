/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_Ffree_buf.c
 *  @brief  free buffer
 *  @date   20070316  FFCS)linjunhua create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2007
 */
 
#include "f_trp_com.h"				/* TRIFプロセス共通ヘッダ			*/

/*! @addtogroup TRIF_TCM
 * @{ */

/*!
 *  @brief to release buffer
 *  @note 
 *  @param  buf_addr [IN] the buffer address
 *  @return none
 *  @retval none
 *  @date 20070316 FFCS)linjunhua create
 */
  

VOID f_trp_com_Ffree_buf(VOID* buf_addr)
{
	/* Use cm_L2BfGet don't need free */
	cm_BFree( buf_addr );
	return ;
}
/* @} */  /* group TRIF_TCM */


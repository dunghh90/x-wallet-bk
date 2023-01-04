/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_Fsize_adj.c
 *  @brief  size adjusting
 *	@date	2009/01/06	FJT)Nagasima create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
 

#include "f_trp_com.h"		/*TRIF common header file		*/
/*! @addtogroup TRIF_TCM
 * @{ */

/*!
 *  @brief to alloc buffer
 *  @note step:
 *		1.calculate the adjusting length
 *  @param  len  [IN] adjusting length
 *  @param  size [IN] adjusting size
 *  @return adjusted length
 *  @date 20090106 FJT)Nagasima create
 */
  
UINT f_trp_com_Fsize_adj(UINT len, UINT size)
{
	UINT ret_len;

	ret_len = len;
	if ((len%size) != CMD_NUM0) {
		ret_len += size - (len%size);
	}

	return ret_len;
}
/* @} */  /* group TRIF_TCM */


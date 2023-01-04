/*!
 * @skip  $ld:$
 * @file  BPF_COM_LOG_SPI.c 
 * @brief SPI log function.
 * @date  2013/12/16 ALPHA)Yokoyama Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include <sys/syscall.h>
#include <sys/types.h>
#include "bpf_i_com_log.h"

/*!
 *  @brief  This fuction will be called to write SPI log.
 *  @note   This function is processed as follows.\n
 *          - Write SPI log
 *  @param  devkind        [in]   device kind
 *  @param  rwkind         [in]   R/W kind
 *  @param  addr           [in]   Reg Address
 *  @param  data           [in]   R/W data
 *  @return VOID
 *  @retval none
 *  @date  2013/12/16 ALPHA)Yokoyama Create
 */
VOID  BPF_COM_LOG_SPI(	UINT				devkind,
						UINT				rwkind,
						UINT				addr,
						UINT				data )
{
	switch(rwkind)
	{
		/* devkind nnssxxxx */
		case D_RRH_WRITE:
			BPF_COM_PLOG_SPI(E_SPI_LV_WRITE,addr,data,(UINT)((devkind >> 24) & 0x0F) ,(UINT)(devkind & 0x000FFFFF));
			break;
		case D_RRH_READ:
			BPF_COM_PLOG_SPI(E_SPI_LV_READ,addr,data,(UINT)((devkind >> 24) & 0x0F) ,(UINT)(devkind & 0x000FFFFF));
			break;
		default:
			BPF_COM_PLOG_SPI(rwkind,addr,data,(UINT)((devkind >> 24) & 0x0F) ,(UINT)(devkind & 0x000FFFFF));
			break;
	}
	return;
}

/* @} */


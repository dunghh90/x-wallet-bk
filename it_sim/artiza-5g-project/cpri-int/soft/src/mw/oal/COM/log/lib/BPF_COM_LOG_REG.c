/*!
 * @skip  $ld:$
 * @file  BPF_COM_LOG_REG.c (base:f_com_RegLog.c)
 * @brief Register log function.
 * @date  2010/11/18 COMAS)Hanaizumi Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2010-2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include <sys/syscall.h>
#include <sys/types.h>
#include "bpf_i_com_log.h"
#include "bpf_i_rm_svrm.h"

/*!
 *  @brief  This fuction will be called to write register log.
 *  @note   This function is processed as follows.\n
 *          - Write register log
 *  @param  loglevel       [in]   log level
 *  @param  access_type    [in]   access type
 *  @param  addr           [in]   Reg Address
 *  @param  data1          [in]   R/W data
 *  @param  data2          [in]   R/W data
 *  @return VOID
 *  @retval none
 *  @date  2010/11/18 COMAS)Hanaizumi Create
 *  @date  2011/01/25 COMAS)花泉 M-RRU-ZSYS-00592対応
 *  @date  2011/01/26 COMAS)Nakajima M-RRU-ZSYS-00577対応
 *  @date  2011/12/12 COMAS)Hanaizumi Samsung 2.1G開発
 *  @date  2011/12/13 COMAS)Nakajima modify for Samsung 2.1G 開発
 *  @date  2012/01/10 ALPHA)高橋 M-RRU-ZSYS-00766対処
 *  @date  2012/01/23 ALP)Fujii       M-RRU-ZSYS-00792
 *  @date  2012/03/09 COMAS)Hanaizumi modify for NSN2.1G
 *  @date  2012/10/16 COMAS)Nakajima modify for MTCD
 *  @date  2013/04/01 ALPHA)Matsunobu modify for B25 (base:f_com_RegLog.c)
 *  @date  2015/04/09 FJT)Taniguchi for 3.5G TDD 
 */
VOID  BPF_COM_LOG_REG(	UINT				logLevel,
						UINT				access_type,
						UINT				addr,
						UINT				data1,
						UINT				data2 )
{
	switch(logLevel){
		case D_RRH_LOG_REG_LV_READ:
			BPF_COM_PLOG_REG(E_REG_LV_READ,addr,data1);
			break;
		case D_RRH_LOG_REG_LV_WRITE:
			BPF_COM_PLOG_REG(E_REG_LV_WRITE,addr,data1);
			break;
		case D_RRH_LOG_REG_LV_READ_WRITE:
			BPF_COM_PLOG_REG(E_REG_LV_READ ,addr,data1);
			BPF_COM_PLOG_REG(E_REG_LV_WRITE,addr,data2);
			break;
		case D_RRH_LOG_REG_LV_READ_BUF:
			BPF_COM_PLOG_REG(E_REG_LV_BUF_READ,addr,data1);
			break;
		case D_RRH_LOG_REG_LV_WRITE_BUF:
			BPF_COM_PLOG_REG(E_REG_LV_BUF_WRITE,addr,data1);
			break;
		case D_RRH_LOG_REG_LV_WRITE_POL:
			BPF_COM_PLOG_REG(E_REG_LV_POLL_WRITE,addr,data1);
			break;
		case D_RRH_LOG_REG_LV_READ_BUSY:
			BPF_COM_PLOG_REG(E_REG_LV_POLL_READ,addr,data1);
			break;
		case D_RRH_LOG_REG_LV_HDL_READ:
		case D_RRH_LOG_REG_LV_HDL_READ_BUF:
			BPF_COM_PLOG_REG(E_REG_LV_READ_HDL,addr,data1);
			break;
		case D_RRH_LOG_REG_LV_HDL_WRITE:
		case D_RRH_LOG_REG_LV_HDL_WRITE_BUF:
			BPF_COM_PLOG_REG(E_REG_LV_WRITE_HDL,addr,data1);
			break;
		case D_RRH_LOG_REG_LV_HDL_READ_WRITE:
			BPF_COM_PLOG_REG(E_REG_LV_READ_HDL,addr,data1);
			BPF_COM_PLOG_REG(E_REG_LV_WRITE_HDL,addr,data2);
			break;
		default:
			break;
	}
	return;
}

/* @} */


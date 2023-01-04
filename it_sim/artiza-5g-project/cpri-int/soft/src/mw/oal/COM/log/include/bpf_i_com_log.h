/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_com_log.h
 *  @brief  Definitions for BPF COM Internal
 *  @date   $Date:$ $Author:$
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_COM_LOG_H_
#define _BPF_I_COM_LOG_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM_LOG.h"
#include "BPF_COM_PLOG.h"
#include "BPF_RU_IPCM.h"
#include "BPF_HM_DEVC.h"
#include <sys/mman.h>

/********************************************************************************************************************/

#define D_BPF_COM_LOG_RST_REASON_NUM		11

#define D_BPF_LOG_SHLOG_MAX_CHAR_A_LINE 480    					/* It contains NULL. 						 */

#define D_BPF_LOG_COMP_OFF	0
#define D_BPF_LOG_COMP_ON	1

/* shell script log functions for logging function */
#define D_BPF_LOG_SHLOG_CFGPFNAME	 "/opt/cmd/log/cmd_csllog.cfg"	/* log config file name (full path)			*/
#define D_BPF_LOG_SHLOG_OUTDIR		 "/var/log"		 				/* output file directory of shell script log*/
#define D_BPF_LOG_SHLOG_OUTFNAME	 "shoutputlog." 				/* output file name  of shell script log 	*/
#define D_BPF_LOG_SHLOG_MAX_LENGTH_OUTFNAME 16						/* It contains NULL. 						*/

/* ログファイル出力用定義	*/
#define D_BPF_LOG_CMDLOG_OUTDIR_NORMAL			"/tmp/"
#define D_BPF_LOG_CMDLOG_OUTDIR_ALM				"/var/log/troublelog/"
#define D_BPF_LOG_CMDLOG_OUTDIR_ALM_RST			"/var/log/troublelog_rst/"
#define D_BPF_LOG_CMDLOG_OUTDIR_SEND			"/var/log/sendlog/"
#define D_BPF_LOG_CMDLOG_OUTDIR_ALM_HEALTCHK	"/var/log/healthchklog/"
#define D_BPF_LOG_CMDLOG_OUTDIR_SEND_3G			"/var/log/sendlog_3g/"

#endif /* _BPF_I_COM_LOG_H_ */


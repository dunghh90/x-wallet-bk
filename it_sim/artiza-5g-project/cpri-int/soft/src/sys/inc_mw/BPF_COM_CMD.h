/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_COM_CMD.h
 *  @brief  Definitions for BPF Internal Common CMD
 *  @date   $Date:$ $Author:$
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2018-
 */
/********************************************************************************************************************/

#ifndef _BPF_COM_CMD_H_
#define _BPF_COM_CMD_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"
#include "BPF_COM_PLOG.h"
#include "BPF_RU_IPCM.h"
#include "f_rrh_inc.h"

VOID BPF_COM_CMD_SET_CUR_CLK_CLASS(UCHAR current_clock_class);
VOID BPF_COM_CMD_SET_RESET_REASON(UINT reset_reason);
VOID BPF_COM_CMD_SET_CUR_PORTNO(UINT port_number);
VOID BPF_COM_CMD_SET_CUR_START_FACE(UINT startup_face);
VOID BPF_COM_CMD_SET_VLANTAG(UINT port ,UINT vid_dl , UINT vid_ul , UINT pcp_ul );
VOID BPF_COM_CMD_SET_REMOTEMAC(UINT port ,UINT remote_mac_addr_h , UINT remote_mac_addr_l );
VOID BPF_COM_CMD_SET_TXENDP(UINT carno ,UINT antno , UINT onoff, UINT Acell_id, UINT CC_id,UINT Port_id, UINT Acell_id_ex, UINT CC_id_ex,UINT Port_id_ex);
VOID BPF_COM_CMD_SET_RXENDP(UINT carno ,UINT antno , UINT onoff, UINT Acell_id, UINT CC_id,UINT Port_id, UINT Acell_id_ex, UINT CC_id_ex,UINT Port_id_ex);
VOID BPF_COM_CMD_SET_TXCC(UINT carno ,UINT antno ,UINT nr_freq, UINT nr_bandwidth, UINT nr_scs );
VOID BPF_COM_CMD_SET_RXCC(UINT carno ,UINT antno ,UINT nr_freq, UINT nr_bandwidth, UINT nr_scs,UINT ul_fft_smpl_ofs,UINT ta_offset );
VOID BPF_COM_CMD_CLR_ENDP(UINT val);
VOID BPF_COM_CMD_SETUP( VOID );
VOID BPF_COM_CMD_SET_TXGAIN(UINT txgain);
VOID BPF_COM_CMD_SET_PTPINVALID(VOID);

/********************************************* EOF ******************************************************************/
#endif /* _BPF_COM_CMD_H__ */

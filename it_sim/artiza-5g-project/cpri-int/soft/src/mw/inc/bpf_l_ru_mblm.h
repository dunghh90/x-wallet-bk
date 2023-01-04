/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_l_ru_mblm.h
 *  @brief  Definitions for BPF mblm library
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_L_RU_MBLM_H_
#define _BPF_L_RU_MBLM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_RU_MBLM.h"

#define BPF_RU_MBLM_OK        0    /* OK */
#define BPF_RU_MBLM_NG        1    /* NG */
#define BPF_RU_MBLM_ASSIGN_NG 2    /* ASSIGN NG */

/* Function Prototype */
int bpf_ru_mblm_bufferassign(unsigned int process_desc);


/********************************************* EOF ******************************************************************/
#endif /* _BPF_L_RU_MBLM_H_ */


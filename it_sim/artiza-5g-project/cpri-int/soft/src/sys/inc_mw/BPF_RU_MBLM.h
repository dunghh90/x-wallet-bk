/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_MBLM.h
 *  @brief  Definitions for BPF MBLM API
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_RM_MBLM_H_
#define _BPF_RM_MBLM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"


/********************************************************************************************************************/
/* External Definition                                                                                              */
/********************************************************************************************************************/

/* Return code */
#define BPF_RU_MBLM_COMPLETE      0        /* Normal                 */
#define BPF_RU_MBLM_ERROR_PARAM   1        /* Invalid parameter      */
#define BPF_RU_MBLM_NO_RESOURCE   2        /* No resource            */
#define BPF_RU_MBLM_BUFALLOC_NG   3        /* Buffer alloc failure   */
#define BPF_RU_MBLM_ALRFREE       4        /* Buffer already freed   */
#define BPF_RU_MBLM_ADDR_INVALID  5        /* Invalid buffer address */
#define BPF_RU_MBLM_BUFFREE_NG    6        /* Buffer free failure    */
#define BPF_RU_MBLM_BUFFREE_BROKEN 7       /* Buffer broken          */

/* Buffer Type */

#define BPF_RU_MBLM_BUFTYPE_256   (0)       /* Buffer type 256byte  */
#define BPF_RU_MBLM_BUFTYPE_4096  (1)       /* Buffer type 4096byte */
#define BPF_RU_MBLM_BUFTYPE_8192  (2)       /* Buffer type 8192byte */

/* Function Prototype */
int BPF_RU_MBLM_BUFFERALLOC(unsigned int buffer_type, unsigned int* buffer_remain, 
                           unsigned int* alloc_buffer_type, void** buffer_address);
int BPF_RU_MBLM_BUFFERFREE(void* buffer_address);



/********************************************* EOF ******************************************************************/
#endif /* _BPF_RM_MBLM_H_ */

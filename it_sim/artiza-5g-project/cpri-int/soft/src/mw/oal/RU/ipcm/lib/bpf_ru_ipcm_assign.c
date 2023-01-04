/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_ipcm_init.c
 *	@brief	Inter Process Communication Management Initialize.
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_IPCM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_ru_ipcm.h"

/** @addtogroup BPF_RU_IPCM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management Initialize.
 *  @note   BPF internal function. 
 *  @param  p_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_ESEMINIT  semaphore initialize error
 *  @retval BPF_RU_IPCM_ESHMINIT  shared memory initialize error
 *  @retval BPF_RU_IPCM_EMSGQINIT message queue initialize error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_assign(unsigned int p_desc)
{
    int ret;

    /********************************************
    * semaphore initialize processing.          *
    ********************************************/
    ret = bpf_ru_ipcm_sem_init(p_desc);
    if( ret != BPF_RU_IPCM_OK ){
		return BPF_RU_IPCM_ESEMINIT;
	}

	/********************************************
    * shared memory initialize processing.      *
    ********************************************/
    ret = bpf_ru_ipcm_shm_init(p_desc);
    if( ret != BPF_RU_IPCM_OK ){
		return BPF_RU_IPCM_ESHMINIT;
	}

    /********************************************
    * message queue initialize processing.      *
    ********************************************/
    ret = bpf_ru_ipcm_msgq_init(p_desc);
    if( ret != BPF_RU_IPCM_OK ){
		return BPF_RU_IPCM_EMSGQINIT;
	}

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Process Communication Management UnInitialize.
 *  @note   BPF internal function. 
 *  @param  p_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_RU_IPCM_OK        normal end
 *  @retval BPF_RU_IPCM_ESEMINIT  semaphore initialize error
 *  @retval BPF_RU_IPCM_ESHMINIT  shared memory initialize error
 *  @retval BPF_RU_IPCM_EMSGQINIT message queue initialize error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_ipcm_unassign(unsigned int p_desc)
{
    int ret;

    /********************************************
    * semaphore uninitialize processing         *
    ********************************************/
    ret = bpf_ru_ipcm_sem_uninit(p_desc);
    if( ret != BPF_RU_IPCM_OK ){
		return BPF_RU_IPCM_ESEMINIT;
	}

	/********************************************
    * shared memory uninitialize processing.    *
    ********************************************/
    ret = bpf_ru_ipcm_shm_uninit(p_desc);
    if( ret != BPF_RU_IPCM_OK ){
		return BPF_RU_IPCM_ESHMINIT;
	}

    /********************************************
    * message queue uninitialize processing.    *
    ********************************************/
    ret = bpf_ru_ipcm_msgq_uninit(p_desc);
    if( ret != BPF_RU_IPCM_OK ){
		return BPF_RU_IPCM_EMSGQINIT;
	}

    return BPF_RU_IPCM_OK;
}
/* @} */

/* @} */

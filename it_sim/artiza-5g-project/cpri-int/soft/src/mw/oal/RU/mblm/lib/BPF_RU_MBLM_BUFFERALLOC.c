/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_MBLM_BUFFERALLOC.c
 *	@brief	Buffer Management Buffer Allocate.
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_MBLM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_ru_mblm.h"

/** @addtogroup BPF_RU_MBLM
 * @{ */

/* Function prototype */
int bpf_ru_mblm_get_freembm(unsigned int buffer_type, 
                                unsigned int *alloc_buffer_type,
                                void **buffer_addr, 
                                unsigned int *remain_num);
int bpf_ru_mblm_free_mbl_get(t_bpf_ru_mblm_free_blk_mng* free_mng, 
                                 unsigned int *get_mbm_no,
                                 unsigned int *remain_num);

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Buffer Allocate.
 *  @note   API function. 
 *  @param  buffer_type       [in]  buffer type
 *  @param  buffer_remain     [out] remain number of buffer
 *  @param  alloc_buffer_type [out] allocated buffer type
 *  @param  buffer_address    [out] buffer address
 *  @return result code
 *  @retval BPF_RU_MBLM_COMPLETE    normal end
 *  @retval BPF_RU_MBLM_ERROR_PARAM parameter error
 *  @retval BPF_RU_MBLM_NO_RESOURCE no resource error
 *  @retval BPF_RU_MBLM_BUFALLOC_NG buffer allocation error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int BPF_RU_MBLM_BUFFERALLOC(unsigned int buffer_type,
                         unsigned int* buffer_remain,
                         unsigned int* alloc_buffer_type,
                         void** buffer_address)
{
    int  sysret;     /* System call return value    */
    int  funcret;    /* Function return value       */
    int  ret;        /* This function return value  */

    /********************************************
    * buffer assign check processing.           *
    ********************************************/
    if( bpf_ru_mblm_assign_flag != BPF_RU_MBLM_FLG_ON ){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                       "MBLM no assign. type=%d",
                       buffer_type);

        /* Non assign */
        return BPF_RU_MBLM_BUFALLOC_NG;
    }

    /********************************************
    * check memory broken detect                *
    ********************************************/
    if(mblm_memory_broken_detect == BPF_RU_MBLM_FLG_ON){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                           "MBLM stop so memory brokent. type=%d",
                           buffer_type);

        /* Buffer allocate ng */
        return BPF_RU_MBLM_BUFALLOC_NG;
    }

    /********************************************
    * parameter check processing.               *
    ********************************************/
/* RRH-001 MD alpha)matsuhashi add start */
    if( (buffer_type < BPF_RU_MBLM_BUFTYPE_256) ||	/* pgr0036 */
        (buffer_type > BPF_RU_MBLM_BUFTYPE_8192) ) {
/* RRH-001 MD alpha)matsuhashi add end */

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                       "Illegal buffer type. type=%d",
                       buffer_type);

        /* Illegal buffer type */
        return BPF_RU_MBLM_ERROR_PARAM;
    }
    /* Buffer type validate check */
    if(bpf_ru_mblm_buffer_mng[buffer_type].buffer_size == 0){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                       "Buffer type is not valid. type=%d",
                       buffer_type);
        /* buffer type is not valid */
        return BPF_RU_MBLM_BUFALLOC_NG;
    }

    /********************************************
    * mutex lock processing.                    *
    ********************************************/
    sysret = pthread_mutex_lock(&bpf_ru_mblm_buffer_mng[buffer_type].mutex); 
    if(sysret){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                       "Mutex lock ng. type=%d errcd=%d",
                       buffer_type,
                       errno);

        /* mutex lock failure */
        return BPF_RU_MBLM_BUFALLOC_NG;
    }

    /********************************************
    * buffer get processing.                    *
    ********************************************/
    funcret = bpf_ru_mblm_get_freembm(buffer_type, 
                               alloc_buffer_type,
                               buffer_address, 
                               buffer_remain);

    /********************************************
    * mutex unlock processing.                  *
    ********************************************/
    sysret = pthread_mutex_unlock(&bpf_ru_mblm_buffer_mng[buffer_type].mutex); 
    if(sysret){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                       "Mutex unlock ng. type=%d errcd=%d",
                       buffer_type,
                       errno);

        /* mutex unlock failure */
        /* Only log */
    }

    /* Return code set */
    if(funcret == BPF_RU_MBLM_COMPLETE){
        ret = BPF_RU_MBLM_COMPLETE;
    }else if(funcret == BPF_RU_MBLM_NO_RESOURCE){
        ret = BPF_RU_MBLM_NO_RESOURCE;
    }else{
        ret = BPF_RU_MBLM_BUFALLOC_NG;
    }

    return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Free Buffer Get.
 *  @note   MBLM internal function. 
 *  @param  buffer_type       [in]  buffer type
 *  @param  alloc_buffer_type [out] allocated buffer type
 *  @param  buffer_addr       [out] buffer address
 *  @param  remain_num        [out] remain number of buffer
 *  @return result code
 *  @retval BPF_RU_MBLM_COMPLETE    normal end
 *  @retval BPF_RU_MBLM_NO_RESOURCE no resource error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_mblm_get_freembm(unsigned int buffer_type, 
                                unsigned int *alloc_buffer_type,
                                void **buffer_addr, 
                                unsigned int *remain_num)
{

    int          func_ret;     /* Function return value    */
    char         *mbmtop;      /* memory block top address */
    unsigned int get_mbm_no;   /* got memory block no      */
    t_bpf_ru_mblm_mbm_top_info  *top_info;  /* Memory block head information */

    /* Allocation request count Increment */
    bpf_ru_mblm_buffer_mng[buffer_type].total_allocreq_num++;

    /********************************************
    * free memory block get processing.         *
    ********************************************/
    func_ret = bpf_ru_mblm_free_mbl_get(&bpf_ru_mblm_buffer_mng[buffer_type].free_mng, 
                                     &get_mbm_no, remain_num);

    if(func_ret != BPF_RU_MBLM_OK){

        /* Allocation failure count Increment */
        bpf_ru_mblm_buffer_mng[buffer_type].total_allocng_num++;
        /* Update minmum free number */
        bpf_ru_mblm_buffer_mng[buffer_type].minimam_nouse_num = 0; 

        /* No free buffer */
        return BPF_RU_MBLM_NO_RESOURCE;
    }

    /* Updata minimum free number */
    if( *remain_num < bpf_ru_mblm_buffer_mng[buffer_type].minimam_nouse_num){
        bpf_ru_mblm_buffer_mng[buffer_type].minimam_nouse_num = *remain_num; 
    }

    /********************************************
    * buffer initialize processing.             *
    ********************************************/

    mbmtop = MBLM_GET_MBM_ADDR(buffer_type, get_mbm_no);

    top_info = (t_bpf_ru_mblm_mbm_top_info*)mbmtop;
	(void)MBML_TAIL_INFO_ADDR(mbmtop, bpf_ru_mblm_buffer_mng[buffer_type].buffer_size);

    /* Use flag set to USING */
    top_info->mbm_info.use_flg = BPF_RU_MBLM_BUFFER_USE;

    /* Allocation count Increment */
    top_info->mbm_info.alloc_cnt++;

    /* thread id set */
    top_info->mbm_info.thread_id = (unsigned long int)pthread_self();

    *buffer_addr = MBML_BUFFER_TOP_ADDR(mbmtop);
    *alloc_buffer_type = buffer_type;

    return BPF_RU_MBLM_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Free Memory Block Get.
 *  @note   MBLM internal function. 
 *  @param  free_mng   [in]  free buffer management table address
 *  @param  get_mbm_no [out] allocated memory block number
 *  @param  remain_num [out] remain number of memory block
 *  @return result code
 *  @retval BPF_RU_MBLM_OK          normal end
 *  @retval BPF_RU_MBLM_NG          abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_mblm_free_mbl_get(t_bpf_ru_mblm_free_blk_mng* free_mng, 
                                 unsigned int *get_mbm_no,
                                 unsigned int *remain_num)
{

    /********************************************
    * free memory block detach processing.      *
    ********************************************/

    /* Check free number */
    if(free_mng->head == BPF_RU_MBLM_LISTEND){
        /* No free memory block */
        *remain_num = free_mng->free_num;
        return BPF_RU_MBLM_NG;
    }

    /* take out */
    *get_mbm_no = free_mng->head;
    free_mng->free_num--;

    if( free_mng->mbm_list[free_mng->head].next != BPF_RU_MBLM_LISTEND ){
        free_mng->head = free_mng->mbm_list[free_mng->head].next;
    }else{
        free_mng->head = BPF_RU_MBLM_LISTEND;
        free_mng->tail = BPF_RU_MBLM_LISTEND;
    }
    free_mng->mbm_list[*get_mbm_no].next = BPF_RU_MBLM_LISTUSE;

    *remain_num = free_mng->free_num;

    return BPF_RU_MBLM_OK;
}
/* @} */

/* @} */

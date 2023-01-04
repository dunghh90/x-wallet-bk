/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_MBLM_BUFFERFREE.c
 *	@brief	Buffer Management Buffer Free.
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
int bpf_ru_mblm_release_mbl(unsigned int, unsigned int, void*);
int bpf_ru_mblm_free_mbl_rel(t_bpf_ru_mblm_free_blk_mng*, unsigned int);

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Buffer Free.
 *  @note   API function. 
 *  @param  buffer_address    [in]  buffer address
 *  @return result code
 *  @retval BPF_RU_MBLM_COMPLETE     normal end
 *  @retval BPF_RU_MBLM_ALRFREE      no allocated free error
 *  @retval BPF_RU_MBLM_ADDR_INVALID buffer address error
 *  @retval BPF_RU_MBLM_BUFFREE_NG   buffer free error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int BPF_RU_MBLM_BUFFERFREE(void* buffer_address)
{

    int  sysret;     /* System call return value   */
    int  funcret;    /* Function return value      */
    int  ret;        /* return value               */
    char *mbmtop;    /* Memory block top address   */
    unsigned int buftype; /* Buffer type           */
    unsigned int mbmno;   /* Memory block number   */
    t_bpf_ru_mblm_mbm_top_info  *top_info;  /* Memory block head information */
    t_bpf_ru_mblm_mbm_tail_info *tail_info; /* Memory block tail information */
    char errlog[BPF_COM_LOG_DATALEN_MAX];/* For error output  */

    /********************************************
    * buffer assign check processing.           *
    ********************************************/
    if( bpf_ru_mblm_assign_flag != BPF_RU_MBLM_FLG_ON ){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                           "No assign. bufaddr=%p",
                           buffer_address);

        /* no assign */
        return BPF_RU_MBLM_BUFFREE_NG;
    }


    /********************************************
    * check memory broken detect                *
    ********************************************/
    if(mblm_memory_broken_detect == BPF_RU_MBLM_FLG_ON){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                           "MBLM stop so memory brokent. bufaddr=%p",
                           buffer_address);

        /* Buffer free ng */
        return BPF_RU_MBLM_BUFFREE_NG;
    }


    /********************************************
    * parameter check processing.               *
    ********************************************/

    /* Get memory block top address */
    mbmtop = MBML_BUFFER_TO_MBMTOP_ADDR(buffer_address);

    /* Address range check */
    if( (mbmtop < bpf_ru_mblm_prc_info.shmaddr) ||
        (mbmtop > (bpf_ru_mblm_prc_info.shmaddr + bpf_ru_mblm_prc_info.shmsize)) ){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                           "Invalid address. bufaddr=%p shm=%p size=%d",
                           buffer_address,
                           bpf_ru_mblm_prc_info.shmaddr,
                           bpf_ru_mblm_prc_info.shmsize);

        /* Outside range */
        return BPF_RU_MBLM_ADDR_INVALID;
    }

    /* Get memory block head information */
    top_info = (t_bpf_ru_mblm_mbm_top_info*)mbmtop;

    /* Check memory block head identifier */
    if(top_info->buftop_id != BPF_RU_MBLM_MBM_TOP_ID){

        /* DBGLOG output */
        snprintf(errlog, sizeof(errlog), 
                          "Top-id is broken. bufaddr=%p id=%08X shm=%p",
                          buffer_address, top_info->buftop_id, bpf_ru_mblm_prc_info.shmaddr);
        /* LOG(ERROR) */
        bpf_com_dbglog(BPF_COM_LOG_ERR_L1,
                       errlog, mbmtop, BPF_COM_LOG_DATALEN_MAX);

        /* Set memory Brokent detection flag ON */
        mblm_memory_broken_detect = BPF_RU_MBLM_FLG_ON;
		
        /* Broken memory */
        return BPF_RU_MBLM_BUFFREE_BROKEN;
    }

    /* Get buffer type */
    buftype = top_info->mbm_info.buffer_type;
    /* Get memory block number */
    mbmno = top_info->mbm_info.mbm_no;

    /* Check buffer type valid */
    if(bpf_ru_mblm_buffer_mng[buftype].buffer_size == 0){

        /* DBGLOG output */
        snprintf(errlog, sizeof(errlog), 
                          "Buffer type is not valid. bufaddr=%p, type=%d shm=%p",
                          buffer_address, buftype, bpf_ru_mblm_prc_info.shmaddr);
        /* LOG(ERROR) */
        bpf_com_dbglog(BPF_COM_LOG_ERR_L2,
                       errlog, mbmtop, BPF_COM_LOG_DATALEN_MAX);

        /* Invalid buffer type */
        return BPF_RU_MBLM_BUFFREE_NG;
    }

    /* Get memory block tail information */
    tail_info = MBML_TAIL_INFO_ADDR(mbmtop, bpf_ru_mblm_buffer_mng[buftype].buffer_size);

    /* Check memory block tail identifire */
    if(tail_info->buftail_id != BPF_RU_MBLM_MBM_TAIL_ID){

        /* DBGLOG output */
        snprintf(errlog, sizeof(errlog),
                          "Tail-id is broken. bufaddr=%p, id=%08X shm=%p",
                          buffer_address, tail_info->buftail_id, bpf_ru_mblm_prc_info.shmaddr);
        /* LOG(ERROR) */
        bpf_com_dbglog(BPF_COM_LOG_ERR_L1,
                       errlog, mbmtop, BPF_COM_LOG_DATALEN_MAX);

        /* Set memory Brokent detection flag ON */
        mblm_memory_broken_detect = BPF_RU_MBLM_FLG_ON;
        /* memory bloken */
        return BPF_RU_MBLM_BUFFREE_BROKEN;
    }


    /* Check use flag */
    if(top_info->mbm_info.use_flg !=  BPF_RU_MBLM_BUFFER_USE){

        /* DBGLOG output */
        snprintf(errlog, sizeof(errlog), 
                          "Already released. bufaddr=%p, uflg=%d type=%d mbmno=%d ftbl=%x shm=%p",
                          buffer_address, top_info->mbm_info.use_flg,
                          buftype, mbmno, bpf_ru_mblm_buffer_mng[buftype].free_mng.mbm_list[mbmno].next,
                          bpf_ru_mblm_prc_info.shmaddr);
        /* LOG(ERROR) */
        bpf_com_dbglog(BPF_COM_LOG_ERR_L2,
                       errlog, mbmtop, BPF_COM_LOG_DATALEN_MAX);

        /* Unused */
        return BPF_RU_MBLM_ALRFREE;
    }


    /********************************************
    * mutex lock processing.                    *
    ********************************************/
    sysret = pthread_mutex_lock(&bpf_ru_mblm_buffer_mng[buftype].mutex); 
    if(sysret){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                       "Mutex lock ng. bufaddr=%p, errcd=%d type=%d mbmno=%d tid=%d",
                        buffer_address, errno, buftype, mbmno, 
                        top_info->mbm_info.thread_id);

        /* mutex lock failure */
        return BPF_RU_MBLM_BUFFREE_NG;
    }

    /********************************************
    * buffer free processing.                   *
    ********************************************/
    funcret = bpf_ru_mblm_release_mbl(buftype, 
                               mbmno,
                               buffer_address);

    /********************************************
    * mutex unlock processing.                  *
    ********************************************/
    sysret = pthread_mutex_unlock(&bpf_ru_mblm_buffer_mng[buftype].mutex); 
    if(sysret){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
                       "Mutex unlock ng. bufaddr=%p, errcd=%d type=%d mbmno=%d tid=%d",
                        buffer_address, errno, buftype, mbmno, 
                        top_info->mbm_info.thread_id);

        /* mutex unlock failure */
        /* Only log collection  */
    }

    /* Set return value */
    if(funcret == BPF_RU_MBLM_COMPLETE){
        ret = BPF_RU_MBLM_COMPLETE;
    }else{
        ret = BPF_RU_MBLM_BUFFREE_NG;
    }

    return ret;

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Free Buffer Release.
 *  @note   MBLM internal function. 
 *  @param  buffer_type       [in]  buffer type
 *  @param  mbmno             [in]  memory block number
 *  @param  buffer_addr       [in]  buffer address
 *  @return result code
 *  @retval BPF_RU_MBLM_COMPLETE    normal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_mblm_release_mbl(unsigned int buffer_type, 
                                 unsigned int mbmno,
                                 void* buffer_addr)
{
    char *mbmtop;                        /* Memory block top address */
    int  func_ret;                       /* Function return value    */
    t_bpf_ru_mblm_mbm_top_info  *top_info;  /* Memory block head information  */

    /********************************************
    * memory block release processing.          *
    ********************************************/
    /* Get memory block top address */
    mbmtop = MBML_BUFFER_TO_MBMTOP_ADDR(buffer_addr);

    /* Get memory block head information */
    top_info = (t_bpf_ru_mblm_mbm_top_info*)mbmtop;

    /* Set use flag to Unuse */
    top_info->mbm_info.use_flg = BPF_RU_MBLM_BUFFER_NOUSE;

    /* Release free memory block number  */
    func_ret = bpf_ru_mblm_free_mbl_rel(&bpf_ru_mblm_buffer_mng[buffer_type].free_mng,
                          mbmno);

    return func_ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Free Memory Block Release.
 *  @note   MBLM internal function. 
 *  @param  free_mng   [in]  free buffer management table address
 *  @param  rel_mbm_no [in]  release memory block number
 *  @return result code
 *  @retval BPF_RU_MBLM_OK          normal end
 *  @retval BPF_RU_MBLM_NG          abnormal end
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_mblm_free_mbl_rel(t_bpf_ru_mblm_free_blk_mng* free_mng, 
                                 unsigned int rel_mbm_no)
{

    /* Check memory block ues or not */
    if(free_mng->mbm_list[rel_mbm_no].next != BPF_RU_MBLM_LISTUSE){

        /* Unused */
        return BPF_RU_MBLM_NG;
    }

    /********************************************
    * free memory block attach processing.      *
    ********************************************/
    /* Last number check */
    if(free_mng->head == BPF_RU_MBLM_LISTEND){
        /* last numbet 0 */
        free_mng->head = rel_mbm_no;
        free_mng->tail = rel_mbm_no;
        free_mng->mbm_list[rel_mbm_no].next = BPF_RU_MBLM_LISTEND;
        free_mng->free_num = 1;

    }else{
        free_mng->mbm_list[free_mng->tail].next = rel_mbm_no;
        free_mng->tail = rel_mbm_no;
        free_mng->mbm_list[rel_mbm_no].next = BPF_RU_MBLM_LISTEND;
        free_mng->free_num++;
    }

    return BPF_RU_MBLM_OK;
}
/* @} */

/* @} */

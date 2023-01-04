/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RM_SVRM_SHM_CREATE.c
 *  @brief  
 *  @date   2013/10/10 ALPHA)yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RM_SVRM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_rm_svrm.h"
#include "bpf_i_ru_ipcm.h"

/** @addtogroup BPF_RM_SVRM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  共有メモリ作成
 *  @note   ユーザが使用する共有メモリの作成を行う
 *  @param  process_desc [in]   プロセス識別子
 *  @return result code
 *  @retval BPF_RM_SVRM_COMPLETE
 *  @retval BPF_RM_SVRM_ERR_DISC
 *  @retval BPF_RM_SVRM_ERR_EXEC
 *  @retval BPF_RM_SVRM_ERR_SYS
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int BPF_RM_SVRM_SHM_CREATE(unsigned int p_desc, unsigned int data_cnt, void *shmTable)
{
	bpf_ru_ipcm_shm_set_table( p_desc, data_cnt, (T_RRH_SYS_CREATE_SHMMEM *)shmTable );

    return BPF_RM_SVRM_COMPLETE;
}

/* @} */
/* @} */

/* RRH-001 MD alpha)yokoyama add end */

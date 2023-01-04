/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_hrtm_assign.c
 *	@brief	Timer Management Timer Assgin.
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_HRTM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_l_ru_hrtm.h"
#include "bpf_i_ru_hrtm.h"


/** @addtogroup BPF_RU_HRTM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Timer Management Assign.
 *  @note   BPF internal function. 
 *  @param  process_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_RU_HRTM_COMPLETE   normal end
 *  @retval BPF_RU_HRTM_ERR_ASSIGN assign error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int bpf_ru_hrtm_assign(
        unsigned int process_desc)
{
	int ret; 
	key_t ipc_key_mng;
	key_t ipc_key_info;
	int shmid_mng;
	int shmid_info;

	struct rlimit hrtm_limit;
	
	if(di_hrtm_assign_flag == HRTM_FLG_ON){
		return BPF_RU_HRTM_COMPLETE;
	}

	
/* RRH-001 MD alpha)matsuhashi add start */
	di_tim_max = HRTM_NUM;		/* タイマ最大使用数を設定する。*/
/* RRH-001 MD alpha)matsuhashi add end */
	
	
	/********************************************
	* IPC key create processing.                *
	********************************************/
	/* IPCKEY 生成マクロ  */
	ipc_key_mng  = BPF_COM_CREATE_INTERNAL_IPCKEY(BPF_COM_IPCKEY_MASK_HRTM,process_desc,0);
	ipc_key_info = BPF_COM_CREATE_INTERNAL_IPCKEY(BPF_COM_IPCKEY_MASK_HRTM,process_desc,1);
	
	/********************************************
	* shared memory get processing.             *
	********************************************/
	shmid_mng = shmget(	ipc_key_mng,
						sizeof(t_bpf_ru_hrtm_tim_tbl),
						BPF_COM_SEMFLG);
	if( shmid_mng == HRTM_SYSCALL_NG ){	/* pgr0203 */
		/* LOG(ERROR) */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmget1 ipckey=%X size=%d errno=%d",
							ipc_key_mng,sizeof(t_bpf_ru_hrtm_tim_tbl),errno);
		return BPF_RU_HRTM_ERR_ASSIGN;
	}
	
	shmid_info = shmget(ipc_key_info,
						sizeof(t_bpf_ru_hrtm_tim_info) * (di_tim_max + 1),
						BPF_COM_SEMFLG);
										/* index 0 は未使用なので1面多く取る */
	if( shmid_info == HRTM_SYSCALL_NG ){ /* pgr0203 */
		/* LOG(ERROR) */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmget2 ipckey=%X size=%d errno=%d",
							ipc_key_info,sizeof(t_bpf_ru_hrtm_tim_info) * (di_tim_max + 1),errno);
		return BPF_RU_HRTM_ERR_ASSIGN;
	}
	
	/********************************************
	* shared memory attach processing.          *
	********************************************/
	di_tim_tbl_p = (t_bpf_ru_hrtm_tim_tbl *)shmat(shmid_mng,0,0);
	if(di_tim_tbl_p == (void *)HRTM_SYSCALL_NG ){ /* pgr0203 */
		/* LOG(ERROR) */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmat1 ipckey=%X size=%d errno=%d shmid=%d",
							ipc_key_mng,sizeof(t_bpf_ru_hrtm_tim_tbl),errno,shmid_mng);
		return BPF_RU_HRTM_ERR_ASSIGN;
	}
	di_tim_info_p = (t_bpf_ru_hrtm_tim_info *)shmat(shmid_info,0,0);
	if(di_tim_info_p == (void *)HRTM_SYSCALL_NG ){ /* pgr0203 */
		/* LOG(ERROR) */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmat2 ipckey=%X size=%d errno=%d shmid=%d",
							ipc_key_info,sizeof(t_bpf_ru_hrtm_tim_info) * (di_tim_max + 1),errno,shmid_info);
		return BPF_RU_HRTM_ERR_ASSIGN;
	}
	
	/********************************************
	* timer table initialize processing.        *
	********************************************/
	
	bpf_ru_hrtm_init(process_desc,ipc_key_mng,ipc_key_info);
	
	/**********************************************
	* RLIMIT_SIGPENDING (Since Linux 2.6.8)       *
	* Shud Not ERROR if syscall may failed.       *
	***********************************************/
	memset(&hrtm_limit, 0, sizeof(hrtm_limit));
	ret = getrlimit(RLIMIT_SIGPENDING, &hrtm_limit);
	if( ret == HRTM_SYSCALL_NG ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"RLIMIT_SIGPENDING GET NG %d\n",errno);
	} else {
		hrtm_limit.rlim_max = RLIM_INFINITY;	/* pgr0821 */
		hrtm_limit.rlim_cur = RLIM_INFINITY;	/* pgr0821 */
		ret = setrlimit(RLIMIT_SIGPENDING, &hrtm_limit);
		if( ret == HRTM_SYSCALL_NG ){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"RLIMIT_SIGPENDING SET NG %d\n",errno);
		}
	}
		
	di_hrtm_assign_flag = HRTM_FLG_ON;
	
	return BPF_RU_HRTM_COMPLETE;
}
/* @} */

/* @} */

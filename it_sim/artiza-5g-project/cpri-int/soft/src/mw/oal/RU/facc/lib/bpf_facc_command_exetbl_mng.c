/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_facc_command_execute_table_mng.c
 *	@brief	command execute table manage
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_FACC /* Pre-define */



/** @addtogroup BPF_RU_FACC
 * @{ */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include <sys/types.h> 
#include <unistd.h> 
#include "bpf_l_com.h"
#include "bpf_i_ru_facc.h"


/* Initialization flag. if flag is off, you must init _*/
static int bpf_ru_facc_extbl_init_flg = BPF_RU_FACC_FLAG_OFF;

pid_t  bpf_ru_facc_extbl[BPF_RU_FACC_EXTBL_MAX];

/* Command exec free mng table head */
bpf_ru_facc_extbl_free_mngtable_head_t  bpf_ru_facc_extbl_free_mngtable_head;
/* Command exec free mng table */
int bpf_ru_facc_extbl_free_mngtbl[BPF_RU_FACC_EXTBL_MAX];

/* Mutex */
pthread_mutex_t bpf_ru_facc_extbl_mutex;

/********************************************************************************************************************/
/**
 *  @brief  Init Command execute table and free mng table
 *  @note   Common Function For FACC
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
void bpf_ru_facc_cmn_init_exetbl( void )
{
	int cnt;

	bpf_ru_facc_extbl_free_mngtable_head.free_num  = BPF_RU_FACC_EXTBL_MAX;
	bpf_ru_facc_extbl_free_mngtable_head.free_head = BPF_COM_NUM0;
	bpf_ru_facc_extbl_free_mngtable_head.free_tail = BPF_RU_FACC_EXTBL_MAX-1;
	for(cnt = BPF_COM_NUM0; cnt < BPF_RU_FACC_EXTBL_MAX; cnt++){
		if(cnt != (BPF_RU_FACC_EXTBL_MAX-1)){
            /* excluding last */
			bpf_ru_facc_extbl_free_mngtbl[cnt]=cnt+1;
		}else{
            /* Last(Set Last Identifier) */
            bpf_ru_facc_extbl_free_mngtbl[cnt]=BPF_RU_FACC_LISTEND;
        }
	}
	memset(bpf_ru_facc_extbl, BPF_COM_NUM0, sizeof(bpf_ru_facc_extbl));

	pthread_mutex_init(&bpf_ru_facc_extbl_mutex, NULL);

    bpf_ru_facc_extbl_init_flg = BPF_RU_FACC_FLAG_ON;
    return;
}

/********************************************************************************************************************/
/**
 *  @brief  Set command execute pid
 *  @note   Common Function for facc
 *  @param  pid       [in] command execute process id
 *  @param  exetbl_id [out] command execute table identification
 *  @return result code
 *  @retval BPF_RU_FACC_OK                : 正常時
 *  @retval BPF_RU_FACC_NG                : 異常時
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_facc_cmn_set_exetbl( pid_t pid, unsigned int *exetbl_id )
{
    unsigned int cnt;
    int ret;

	if( bpf_ru_facc_extbl_init_flg == BPF_RU_FACC_FLAG_OFF ){
		bpf_ru_facc_cmn_init_exetbl();
	}

	/* Mutex lock */
	ret = pthread_mutex_lock(&bpf_ru_facc_extbl_mutex);
	if( ret != 0 ){
		return BPF_RU_FACC_NG;
	}

	if(bpf_ru_facc_extbl_free_mngtable_head.free_num <= BPF_COM_NUM0 ){
        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,
                          "Maxmum setted. Cannot set pid=%d.", pid);

		/* Mutex Unlock */
		pthread_mutex_unlock(&bpf_ru_facc_extbl_mutex);

		return BPF_RU_FACC_NG;
	}

    /* take out */
    cnt = bpf_ru_facc_extbl_free_mngtable_head.free_head;
    bpf_ru_facc_extbl_free_mngtable_head.free_num--;

    if( bpf_ru_facc_extbl_free_mngtbl[cnt] != BPF_RU_FACC_LISTEND ){
         bpf_ru_facc_extbl_free_mngtable_head.free_head 
         			= bpf_ru_facc_extbl_free_mngtbl[cnt];
    }else{
        bpf_ru_facc_extbl_free_mngtable_head.free_head = BPF_RU_FACC_LISTEND;
        bpf_ru_facc_extbl_free_mngtable_head.free_tail = BPF_RU_FACC_LISTEND;
    }
	bpf_ru_facc_extbl_free_mngtbl[cnt] = BPF_RU_FACC_LISTUSE;

	/* Set PID */
	bpf_ru_facc_extbl[cnt] = pid;

	/* Mutex Unlock */
	pthread_mutex_unlock(&bpf_ru_facc_extbl_mutex);

	*exetbl_id = cnt;
    return BPF_RU_FACC_OK;
}


/********************************************************************************************************************/
/**
 *  @brief  Delete command execute pid
 *  @note   Common Function For FACC
 *  @param  exetbl_id [in] command execute table identification
 *  @return pid
 *  @retval 0      Invalid exetbl_id
 *  @retval not 0  PID
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
pid_t bpf_ru_facc_cmn_delete_exetbl( unsigned int exetbl_id )
{
	int ret;
	pid_t pid;

	if( bpf_ru_facc_extbl_init_flg == BPF_RU_FACC_FLAG_OFF ){
		return 0;
	}

	if( bpf_ru_facc_extbl_free_mngtbl[exetbl_id] != BPF_RU_FACC_LISTUSE){
		/* Not Use */
		return 0;
	}

	/* Mutex lock */
	ret = pthread_mutex_lock(&bpf_ru_facc_extbl_mutex);
	if( ret != 0 ){
		return 0;
	}

	pid = bpf_ru_facc_extbl[exetbl_id];
	/* Delete PID */
	bpf_ru_facc_extbl[exetbl_id] = BPF_COM_NUM0;

    /* Last number check */
    if(bpf_ru_facc_extbl_free_mngtable_head.free_head == BPF_RU_FACC_LISTEND){
        /* last numbet 0 */
        bpf_ru_facc_extbl_free_mngtable_head.free_head = exetbl_id;
        bpf_ru_facc_extbl_free_mngtable_head.free_tail = exetbl_id;
        bpf_ru_facc_extbl_free_mngtbl[exetbl_id] = BPF_RU_FACC_LISTEND;
        bpf_ru_facc_extbl_free_mngtable_head.free_num = 1;

    }else{
        bpf_ru_facc_extbl_free_mngtbl[bpf_ru_facc_extbl_free_mngtable_head.free_tail] = exetbl_id;
        bpf_ru_facc_extbl_free_mngtable_head.free_tail = exetbl_id;
        bpf_ru_facc_extbl_free_mngtbl[exetbl_id]  = BPF_RU_FACC_LISTEND;
        bpf_ru_facc_extbl_free_mngtable_head.free_num++;
    }

	/* Mutex Unlock */
	pthread_mutex_unlock(&bpf_ru_facc_extbl_mutex);

    return pid;
}

/* @} */

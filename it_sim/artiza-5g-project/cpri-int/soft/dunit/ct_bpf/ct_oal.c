#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "f_rrh_inc.h"
#include "bpf_l_com.h"
#include "BPF_RM_SVRM.h"	
#include "BPF_RU_IPCM.h"	
#include "BPF_COM_LOG.h"	
#include "BPF_RU_ITCM.h"	
#include "BPF_COM_PLOG.h"
#include "BPF_RU_ITCM.h"
#include "BPF_RU_MBLM.h"
#include "BPF_RU_HRTM.h"

#include "fldccom.h"
#include "fldc4.h"

extern void f_rrulib_dmsg(
	const char* name ,
	const char* funcname,
	const char* format,
	  ...);

extern void f_rrulib_dconvert_log(
	char*	device,
	char*	mode,
	const char* format,
	  ...);

extern int f_rrulib_memchecker( unsigned char* chkbufp );

extern void* rrulib_shm_addrget(unsigned int kind , char** name);
extern unsigned int rrulib_log_convert;

typedef struct{
	unsigned int vflg;
	int			 result;
}t_call_result;

typedef struct{
	char*			name;
	t_call_result*	ptn;
}t_call_result_ptn;

t_call_result result_BPF_RU_IPCM_PROCMSGQ_SEND;
t_call_result result_BPF_RU_IPCM_PROCMSGQ_RECEIVE;
t_call_result result_BPF_RU_IPCM_PROCMSGQ_NUMMSGS;
t_call_result result_BPF_RU_IPCM_PROCMSGQ_RECEIVE3;
t_call_result result_BPF_RU_IPCM_PROCSEM_TAKE;
t_call_result result_BPF_RU_IPCM_PROCSEM_GIVE;
t_call_result result_BPF_RU_IPCM_PROCSHM_ADDRGET;
t_call_result result_BPF_RM_SVRM_INIT;
t_call_result result_BPF_RM_SVRM_ASSIGN;
t_call_result result_BPF_RM_SVRM_UNASSIGN;
t_call_result result_BPF_RM_SVRM_INITEND_IND;
t_call_result result_BPF_RM_SVRM_INITEND_IND_RCV;
t_call_result result_BPF_RM_SVRM_CPUAFFINITY;
t_call_result result_BPF_RM_SVRM_CPUAFFINITY_ADD;
t_call_result result_BPF_RM_SVRM_SHM_CREATE;
t_call_result result_BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY;
t_call_result result_BPF_RM_SVRM_THREAD_INFO_SET;
t_call_result result_BPF_RU_ITCM_THREAD_CREATE      ;
t_call_result result_BPF_RU_FACC_FOPEN              ;
t_call_result result_BPF_RU_FACC_FREAD              ;
t_call_result result_BPF_RU_FACC_FWRITE             ;
t_call_result result_BPF_RU_FACC_FSEEK              ;
t_call_result result_BPF_RU_FACC_FCLOSE             ;
t_call_result result_BPF_RU_FACC_FSIZE              ;
t_call_result result_BPF_RU_ITCM_THREAD_DELAY       ;
t_call_result result_BPF_RU_ITCM_THREAD_DELETE      ;
t_call_result result_BPF_RU_ITCM_THREAD_IDSELF      ;
t_call_result result_f_com_FA_encode                ;
t_call_result result_f_com_FA_decode                ;
t_call_result result_BPF_RU_ITCM_THREAD_MSGQ_SEND   ;
t_call_result result_BPF_RU_ITCM_THREAD_MSGQ_RECEIVE;
t_call_result result_BPF_RU_ITCM_THREAD_MSGQ_NUMMSGS;
t_call_result result_BPF_RU_ITCM_THREAD_SEM_TAKE    ;
t_call_result result_BPF_RU_ITCM_THREAD_SEM_GIVE    ;
t_call_result result_BPF_RU_MBLM_BUFFERALLOC        ;
t_call_result result_BPF_RU_MBLM_BUFFERFREE         ;
t_call_result result_BPF_RU_HRTM_REGISTER           ;
t_call_result result_BPF_RU_HRTM_CANCEL             ;
t_call_result result_BPF_RU_FACC_FREMOVE;
t_call_result result_BPF_RU_IPCM_MSGQ_SEND;
t_call_result result_BPF_RU_IPCM_MSGQ_RECV;
t_call_result result_BPF_RU_IPCM_MSGQ_RECV_exitcount;

static t_call_result_ptn result[] = 
{
	{ "BPF_RU_IPCM_MSGQ_SEND"             , &result_BPF_RU_IPCM_MSGQ_SEND    },
	{ "BPF_RU_IPCM_MSGQ_RECV"             , &result_BPF_RU_IPCM_MSGQ_RECV    },
	{ "BPF_RU_IPCM_MSGQ_RECV_exitcount"   , &result_BPF_RU_IPCM_MSGQ_RECV_exitcount    },
	{ "BPF_RU_IPCM_PROCMSGQ_SEND"         , &result_BPF_RU_IPCM_PROCMSGQ_SEND       },
	{ "BPF_RU_IPCM_PROCMSGQ_RECEIVE"      , &result_BPF_RU_IPCM_PROCMSGQ_RECEIVE    },
	{ "BPF_RU_IPCM_PROCMSGQ_NUMMSGS"      , &result_BPF_RU_IPCM_PROCMSGQ_NUMMSGS    },
	{ "BPF_RU_IPCM_PROCMSGQ_RECEIVE3"     , &result_BPF_RU_IPCM_PROCMSGQ_RECEIVE3   },
	{ "BPF_RU_IPCM_PROCSEM_TAKE"          , &result_BPF_RU_IPCM_PROCSEM_TAKE        },
	{ "BPF_RU_IPCM_PROCSEM_GIVE"          , &result_BPF_RU_IPCM_PROCSEM_GIVE        },
	{ "BPF_RU_IPCM_PROCSHM_ADDRGET"       , &result_BPF_RU_IPCM_PROCSHM_ADDRGET     },
	{ "BPF_RM_SVRM_INIT"                  , &result_BPF_RM_SVRM_INIT                },
	{ "BPF_RM_SVRM_ASSIGN"                , &result_BPF_RM_SVRM_ASSIGN              },
	{ "BPF_RM_SVRM_UNASSIGN"              , &result_BPF_RM_SVRM_UNASSIGN            },
	{ "BPF_RM_SVRM_INITEND_IND"           , &result_BPF_RM_SVRM_INITEND_IND         },
	{ "BPF_RM_SVRM_INITEND_IND_RCV"       , &result_BPF_RM_SVRM_INITEND_IND_RCV     },
	{ "BPF_RM_SVRM_CPUAFFINITY"           , &result_BPF_RM_SVRM_CPUAFFINITY         },
	{ "BPF_RM_SVRM_CPUAFFINITY_ADD"       , &result_BPF_RM_SVRM_CPUAFFINITY_ADD     },
	{ "BPF_RM_SVRM_SHM_CREATE"            , &result_BPF_RM_SVRM_SHM_CREATE          },
	{ "BPF_RM_SVRM_THREAD_INFO_SET"       , &result_BPF_RM_SVRM_THREAD_INFO_SET     },
	{ "BPF_RU_ITCM_THREAD_CREATE"         , &result_BPF_RU_ITCM_THREAD_CREATE       },
	{ "BPF_RU_FACC_FOPEN"                 , &result_BPF_RU_FACC_FOPEN               },
	{ "BPF_RU_FACC_FREAD"                 , &result_BPF_RU_FACC_FREAD               },
	{ "BPF_RU_FACC_FWRITE"                , &result_BPF_RU_FACC_FWRITE              },
	{ "BPF_RU_FACC_FSEEK"                 , &result_BPF_RU_FACC_FSEEK               },
	{ "BPF_RU_FACC_FCLOSE"                , &result_BPF_RU_FACC_FCLOSE              },
	{ "BPF_RU_FACC_FSIZE"                 , &result_BPF_RU_FACC_FSIZE               },
	{ "BPF_RU_ITCM_THREAD_DELAY"          , &result_BPF_RU_ITCM_THREAD_DELAY        },
	{ "BPF_RU_ITCM_THREAD_DELETE"         , &result_BPF_RU_ITCM_THREAD_DELETE       },
	{ "BPF_RU_ITCM_THREAD_IDSELF"         , &result_BPF_RU_ITCM_THREAD_IDSELF       },
	{ "f_com_FA_encode"                   , &result_f_com_FA_encode                 },
	{ "f_com_FA_decode"                   , &result_f_com_FA_decode                 },
	{ "BPF_RU_ITCM_THREAD_MSGQ_SEND"      , &result_BPF_RU_ITCM_THREAD_MSGQ_SEND    },
	{ "BPF_RU_ITCM_THREAD_MSGQ_RECEIVE"   , &result_BPF_RU_ITCM_THREAD_MSGQ_RECEIVE },
	{ "BPF_RU_ITCM_THREAD_MSGQ_NUMMSGS"   , &result_BPF_RU_ITCM_THREAD_MSGQ_NUMMSGS },
	{ "BPF_RU_ITCM_THREAD_SEM_TAKE"       , &result_BPF_RU_ITCM_THREAD_SEM_TAKE     },
	{ "BPF_RU_ITCM_THREAD_SEM_GIVE"       , &result_BPF_RU_ITCM_THREAD_SEM_GIVE     },
	{ "BPF_RU_MBLM_BUFFERALLOC"           , &result_BPF_RU_MBLM_BUFFERALLOC         },
	{ "BPF_RU_MBLM_BUFFERFREE"            , &result_BPF_RU_MBLM_BUFFERFREE          },
	{ "BPF_RU_HRTM_REGISTER"              , &result_BPF_RU_HRTM_REGISTER            },
	{ "BPF_RU_HRTM_CANCEL"                , &result_BPF_RU_HRTM_CANCEL              },
	{ "BPF_RU_FACC_FREMOVE"               , &result_BPF_RU_FACC_FREMOVE},
	{ NULL      , NULL            }
};

void oal_callresult_init(char* name )
{
	unsigned int count;
	for( count=0 ; count < (sizeof(result)/sizeof(result[0])) ; count++)
	{
		if(result[count].name == NULL)
		{
			break;
		}
		if( strcmp(result[count].name , name ) )
		{
			continue;
		}
		result[count].ptn->vflg = 0;
		break;
	}
	return;
}

void oal_callresult_set(char* name , int val)
{
	unsigned int count;
	for( count=0 ; count < (sizeof(result)/sizeof(result[0])) ; count++)
	{
		if(result[count].name == NULL)
		{
			break;
		}
		if( strcmp(result[count].name , name ) )
		{
			continue;
		}
		result[count].ptn->vflg = 1;
		result[count].ptn->result = val;
		break;
	}
	return;
}

int oal_callresult_get(char* name )
{
	unsigned int count;
	int ret = 0;
	for( count=0 ; count < (sizeof(result)/sizeof(result[0])) ; count++)
	{
		if(result[count].name == NULL)
		{
			break;
		}
		if( strcmp(result[count].name , name ) )
		{
			continue;
		}
		if( result[count].ptn->vflg != 0)
		{
			ret = result[count].ptn->result;
		}
		else
		{
			ret = 0;
		}
		break;
	}
	return(ret);
}

#define DPRINT(...) f_rrulib_dmsg("OAL", __func__ ,__VA_ARGS__ )
#define DPRINTCONVERT(dev,mode,...) f_rrulib_dconvert_log(dev,mode, __VA_ARGS__ )
#define MEMCHECK(addr) f_rrulib_memchecker(addr)

#define RETURN()  return (oal_callresult_get((char*)__func__))

#define MSG_HEADER_DUMP(p) \
do { \
	DPRINT(" MsgPointer   :0x%lx  \n",(unsigned long)p); \
	DPRINT("  EventNo     :0x%08x \n",((T_RRH_HEAD*)p)->uiEventNo); \
	DPRINT("  Signalkind  :0x%08x \n",((T_RRH_HEAD*)p)->uiSignalkind); \
	DPRINT("  DstPQueueID :0x%08x \n",((T_RRH_HEAD*)p)->uiDstPQueueID); \
	DPRINT("  DstTQueueID :0x%08x \n",((T_RRH_HEAD*)p)->uiDstTQueueID); \
	DPRINT("  SrcPQueueID :0x%08x \n",((T_RRH_HEAD*)p)->uiSrcPQueueID); \
	DPRINT("  SrcTQueueID :0x%08x \n",((T_RRH_HEAD*)p)->uiSrcTQueueID); \
	DPRINT("  Length      :0x%08x(%dbytes) \n",((T_RRH_HEAD*)p)->uiLength,((T_RRH_HEAD*)p)->uiLength); \
	DPRINT("  KeyID       :0x%08x \n",((T_RRH_HEAD*)p)->uiKeyID); \
}while(0)

int BPF_RU_IPCM_MSGQ_SEND(	unsigned int	q_num,
							void			*msgp ){
	if(MEMCHECK(msgp) != 0){
		DPRINT("ERROR strange pointer msgp:0x%lx \n",(unsigned long)msgp);
		abort();
	}
	DPRINT("SEND Qid:0x%08x \n",q_num);
	MSG_HEADER_DUMP(msgp);

	int size = ((T_RRH_HEAD*)msgp)->uiLength;
	unsigned int bufkind;
	char* name;
	void* addr;

	if(size <= BPF_MESSAGE_SIZE_S){
		bufkind = BPF_MESSAGE_SIZE_S + E_RRH_SHMID_APL_MAX;
	}else if(size <= BPF_MESSAGE_SIZE_M){
		bufkind = BPF_MESSAGE_SIZE_M + E_RRH_SHMID_APL_MAX;
	}else{
		bufkind = BPF_MESSAGE_SIZE_L + E_RRH_SHMID_APL_MAX;
	}
	if((q_num & 0xFFFF0000 ) != 0){
		bufkind += 0x10000000;
	}
	addr = rrulib_shm_addrget(bufkind ,&name);
	memcpy(addr,(void*)&q_num,sizeof(q_num));
	addr += sizeof(q_num);
	memcpy(addr,msgp,size);
	DPRINT("SEND addr:0x%lx(%s) <- %d - msgp:%lx\n",addr,name,size,msgp);
	RETURN();

}

static t_call_result retrycount;
int BPF_RU_IPCM_MSGQ_RECV(	unsigned int	q_num,
							unsigned int	wait_time,
							void			**msgp,
							unsigned int	*msgsize )
{
	if((MEMCHECK((void*)msgp) != 0) || (MEMCHECK((void*)msgsize) != 0)){
		DPRINT("ERROR strange pointer msgp:0x%lx \n",(unsigned long)msgp);
		abort();
	}

	char* name;
	void* addr;
	unsigned int	tmp_q_num;

	DPRINT("RECV Qid:0x%08x Wait:%u \n",q_num,wait_time);
	unsigned int bufkind = 0;
	if((q_num & 0xFFFF0000 ) != 0){
		bufkind = 0x10000000;
	}

	addr = rrulib_shm_addrget(bufkind + BPF_MESSAGE_SIZE_S + E_RRH_SHMID_APL_MAX ,&name);
	memcpy((void*)&tmp_q_num,addr,sizeof(tmp_q_num));
	if(tmp_q_num != q_num){
		addr = rrulib_shm_addrget(bufkind + BPF_MESSAGE_SIZE_M + E_RRH_SHMID_APL_MAX ,&name);
		memcpy(&tmp_q_num,addr,sizeof(tmp_q_num));
		if(tmp_q_num != q_num){
			addr = rrulib_shm_addrget(bufkind + BPF_MESSAGE_SIZE_L + E_RRH_SHMID_APL_MAX ,&name);
			memcpy(&tmp_q_num,addr,sizeof(tmp_q_num));
			if(tmp_q_num != q_num){
				if(memcmp((void*)&retrycount, (void*)&result_BPF_RU_IPCM_MSGQ_RECV_exitcount,sizeof(retrycount)) == 0){
					DPRINT("NOT RECV Exit\n");
					exit(1);
				}
				DPRINT("NOT RECV(%d)\n",retrycount.result);
				retrycount.vflg = 1;
				retrycount.result += 1;
				RETURN();
			}
		}
	}
	addr+=sizeof(q_num);
	*msgp = addr;
	*msgsize = ((T_RRH_HEAD*)*msgp)->uiLength;
	DPRINT("RECV addr:0x%lx(%s) size:0x%x(%ubytes)\n",addr,name,*msgsize);
	MSG_HEADER_DUMP(*msgp);
	RETURN();
}

int BPF_RU_IPCM_PROCMSGQ_SEND(unsigned int a, const void * b, unsigned int c, int d,int * e){
	DPRINT("\n");
	RETURN();
}

int BPF_RU_IPCM_PROCMSGQ_RECEIVE(unsigned int a, int b, unsigned int c, void **d,unsigned int *e, int *f){DPRINT("\n");RETURN();}
int BPF_RU_IPCM_PROCMSGQ_NUMMSGS(unsigned int a, unsigned int *b, int *c){DPRINT("\n");RETURN();}
int BPF_RU_IPCM_PROCMSGQ_RECEIVE3(unsigned int a, int b, unsigned int c, void * d,unsigned int *e, int *f){DPRINT("\n");RETURN();}
int BPF_RU_IPCM_PROCSEM_TAKE(unsigned int sem_num, int sem_rwlock, int sem_flag, 
                          struct bpf_timespec *timeout, int *errcd)
{	
	DPRINT("sem_num:0x%08x(%d) sem_rwlock:0x%08x sem_flag:0x%08x \n",sem_num,sem_num,sem_rwlock,sem_flag);
	RETURN();
}

int BPF_RU_IPCM_PROCSEM_GIVE(unsigned int sem_num, int sem_rwlock, int *errcd)
{	
	DPRINT("sem_num:0x%08x(%d) sem_rwlock:0x%08x \n",sem_num,sem_num,sem_rwlock);
	RETURN();
}

int BPF_RU_IPCM_PROCSHM_ADDRGET(unsigned int a, void **b, int *c )
{
	char* name;
	*b = rrulib_shm_addrget(a,&name);
	*c = 0;
	DPRINT("%s(0x%08x) addr:0x%lx err:0x%x\n",name,a,(unsigned long)*b,*c);
	RETURN();
}

int BPF_RU_IPCM_PROCMSG_ADDRGET(unsigned int kind, unsigned int size, void **addr, int *errcd){
	char* name;
	unsigned int bufkind;
	
	if(kind != E_BPF_RU_IPCM_BUFF_KIND_TASK){
		*errcd = 0x12345678;
		return -1;
	}
	
	*addr = rrulib_shm_addrget(0x20000000+BPF_MESSAGE_SIZE_L + E_RRH_SHMID_APL_MAX ,&name);
	*errcd = 0;
	DPRINT("%s(0x%08x) addr:0x%lx err:0x%x\n",name,kind,*addr,*errcd);
	RETURN();
}

int BPF_RU_IPCM_PROCMSG_ADDRFREE( void * a){
	DPRINT("addr:0x%lx\n",a);
	RETURN();
}




int BPF_RM_SVRM_INIT(unsigned int process_desc){DPRINT("0x%08x(%d)\n",process_desc,process_desc);RETURN();}
int BPF_RM_SVRM_ASSIGN(unsigned int process_desc){DPRINT("0x%08x(%d)\n",process_desc,process_desc);RETURN();}
int BPF_RM_SVRM_UNASSIGN(unsigned int process_desc){DPRINT("0x%08x(%d)\n",process_desc,process_desc);RETURN();}
int BPF_RM_SVRM_INITEND_IND(bpf_rm_svrm_result result){ DPRINT("\n");RETURN();}
int BPF_RM_SVRM_INITEND_IND_RCV (unsigned int *prockind , unsigned int *procnum , bpf_rm_svrm_result *result, sigset_t *sigSetVal , unsigned int* sigUsr1Flg , unsigned int* sigUsr2Flg ){	DPRINT("\n");RETURN();	}
int BPF_RM_SVRM_CPUAFFINITY (unsigned int process_desc, int cpu, int* errcd)
{
	DPRINT("process_desc:0x%08x cpu:0x%08x\n",process_desc,cpu);
	RETURN();
}
int BPF_RM_SVRM_CPUAFFINITY_ADD (unsigned int process_desc, int cpu, int* errcd)
{
	DPRINT("process_desc:0x%08x cpu:0x%08x\n",process_desc,cpu);
	RETURN();
}

void BPF_RM_SVRM_SIGNAL_SEND(int pid, int signo, int option){DPRINT("pid 0x%x sig:0x%x opt 0x%x\n",pid,signo,option);return;}


int BPF_RM_SVRM_SHM_CREATE(unsigned int p_desc, unsigned int data_cnt, void *shmTable){DPRINT("\n");RETURN();}
int BPF_RM_SVRM_SIGNAL_HANDLE_ENTRY( int* funcP, sigset_t* sigSetVal , int sig_sndonoff ){DPRINT("\n");RETURN();}
char* BPF_RM_SVRM_SIGNAL_COREDUMP(FILE* fp, char* topbuf_p, unsigned int size ){DPRINT("\n"); return NULL ;}
void BPF_RM_SVRM_SIGNAL_TBLSAVE( int fd , int tblnum , char* tbl_top_p){DPRINT("\n");return;}
void BPF_RM_SVRM_SIGNAL_OUTPUT_CONTEXT( void* ctx ){DPRINT("\n");return;}
int BPF_RM_SVRM_THREAD_INFO_SET( unsigned int processID, unsigned int threadID, unsigned int threadQueueID , unsigned long int pThreadID ){DPRINT("\n");RETURN();}


VOID BPF_COM_LOG_EVENT(UINT logId, VOID* data ){DPRINT("\n");return;}
VOID BPF_COM_LOG_ERROR(UINT ErrorCode, VOID* errorTitle, UCHAR* traceInfo){DPRINT("\n");return;}
UINT BPF_COM_LOG_GET_EVENT( VOID* data ){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_GET_ERROR(VOID* data){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_CLEAR_ERROR( VOID ){DPRINT("\n");RETURN();}
VOID BPF_COM_LOG_HW_REPAIR(VOID* textData){DPRINT("\n");return;}
VOID BPF_COM_LOG_HW_REPAIR_SPEC_DATE(VOID* dateData, VOID* textData){DPRINT("\n");return;}
VOID BPF_COM_LOG_HW_REPAIR_INTERNAL(T_RRH_DATETIME* date, VOID* textData){DPRINT("\n");return;}
UINT BPF_COM_LOG_HW_SYSTEM(UINT logId, VOID* textData){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_HW_SYSTEM_WITH_DATE(UINT logId, T_RRH_DATETIME *tDateTime, VOID *textData ){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_GET_TRACE_AND_ERROR( VOID* data){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_CLEAR_TRACE_AND_ERROR( VOID ){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_GET_HW_SYSTEM( VOID* data){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_GET_HW_SYSTEM_LOGID( UINT logId, UINT *numOfEntry, CHAR *hwLogData ){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_CLEAR_HW_SYSTEM( VOID ){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_CLEAR_HW_REPAIR( VOID ){DPRINT("\n");RETURN();}
VOID BPF_COM_LOG_PROCIF(UINT logLevel, UINT sr_kind, VOID* procIfMsg){DPRINT("\n");return;}
VOID BPF_COM_LOG_PMD(){DPRINT("\n");return;}
UINT BPF_COM_LOG_GET_PMD(VOID* data){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_CLEAR_PMD( VOID ){DPRINT("\n");RETURN();}
VOID BPF_COM_LOG_REG( UINT logLevel, UINT access_type, UINT addr, UINT data1, UINT data2 ){return;}
VOID BPF_COM_LOG_TRACE_AND_ERROR(USHORT logKind, VOID* data, USHORT traceNum){DPRINT("\n");return;}
VOID BPF_COM_LOG_THDIF(UINT logLevel, UINT sr_kind, UINT state, VOID* thdifMsg){DPRINT("\n");return;}
VOID BPF_COM_LOG_RST_REASON(UINT version, UINT rstReason, UINT addInfo ){DPRINT("\n");return;}
VOID BPF_COM_LOG_SAVE_FLASH( VOID ){DPRINT("\n");return;}
VOID BPF_COM_LOG_PROC_DEAD( VOID* data ){DPRINT("\n");return;}
UINT BPF_COM_LOG_GET_BPF_LOG( VOID ){DPRINT("\n");RETURN();}
VOID BPF_COM_LOG_RICR(UCHAR port, VOID* data ){DPRINT("\n");return;}
UINT BPF_COM_LOG_GET_RST_REASON( UINT *rstReason ){DPRINT("\n");RETURN();}
UINT BPF_COM_LOG_CLEAR_EVENT(VOID){DPRINT("\n");RETURN();}

void bpf_com_plog_trace(
	char*			 lname,
	e_bpf_com_plog_level level ,
	const char*		 file,
	int				 line,
	const char*		 format, ...
)
{
	const char*	out_p;
	char str[256];
	va_list args;
	int len;
	va_start(args, format);	/* pgr0039 */
	len = vsnprintf( str , sizeof(str), format, args);
	va_end(args);

	if((out_p = strrchr(file, '/')) == NULL)
	{
		out_p = file;
	}
	else
	{
		out_p++;
	}
	DPRINT("%s;lvl:%08x;%s(%d);len(%d):%s\n",lname,level,out_p,line,len,str);
	return;
}

void bpf_com_plog_trace_mp(
	int				 level ,
	const char*		 format, ...)
{
	char str[512];

	va_list args;
	va_start(args, format);	/* pgr0039 */
	vsnprintf( str , sizeof(str), format, args);
	va_end(args);

	DPRINT("level:%d:%s\n",level,str);
	return;
}


void bpf_com_plog_com(
	char*			     lname,
	e_bpf_com_plog_level level ,
	int					 len,
	void*				 data_p,
	unsigned int		 ipaddr_ofs
)
{
	DPRINT("\n");
	return;	
}

static void* (*bpf_thread_func_p)(void*) = NULL;

int BPF_RU_ITCM_THREAD_CREATE(	unsigned long int *thread, int policy, int priority, unsigned int stacksize,
								void *start_routine, void *arg, int *errcd)
{
	DPRINT("policy:%d,priority:%d,stacksize:%08x(%d) \n",policy,priority,stacksize,stacksize);
	if(start_routine != NULL){
		bpf_thread_func_p = start_routine;	
		bpf_thread_func_p(arg);
		*thread = (unsigned long int)(bpf_thread_func_p);
	}
	else{
		 *thread = 0 ;
	}
	RETURN();
}

int BPF_RU_FACC_FOPEN(
			const bpf_ru_facc_dpathname *dir_path_name,		/* ディレクトリパス名				*/
			const bpf_ru_facc_fname *file_name,				/* ファイル名						*/
			bpf_ru_facc_openmode open_mode,					/* オープンモード					*/
			unsigned int create_mode,						/* ファイル作成モード				*/
			int *file_descriptor,							/* ファイルディスクリプタ			*/
			int *errcd)									/* 詳細NGコード						*/
{
	DPRINT("\n");
	RETURN();
}

int BPF_RU_FACC_FREAD(
			int file_descriptor,							/* ファイルディスクリプタ			*/
			unsigned int read_size,							/* 読み込みサイズ					*/
			char *read_data,								/* 読み込みデータ格納領域			*/
			unsigned int *act_read_size,					/* 実読み出しサイズ					*/
			int *errcd)									/* 詳細NGコード						*/
{
	DPRINT("\n");
	RETURN();
}

/* ファイル書き込み */
int BPF_RU_FACC_FWRITE(
			int file_descriptor,							/* ファイルディスクリプタ			*/
			unsigned int write_size,						/* 書き込みサイズ					*/
			const char *write_data,							/* 書き込みデータ格納領域			*/
			unsigned int *act_write_size,					/* 実書き込みデータサイズ			*/
			int *errcd)									/* 詳細NGコード						*/
{
	DPRINT("\n");
	RETURN();
}

/* ファイルシーク */
int BPF_RU_FACC_FSEEK(
			int file_descriptor, 							/* ファイルディスクリプタ			*/
			bpf_ru_facc_off_t offset,						/* オフセット						*/
			bpf_ru_facc_seekmode seek_mode,					/* シークモード						*/
			int *errcd)									/* 詳細NGコード						*/
{
	DPRINT("\n");
	RETURN();
}

/* ファイルクローズ */
int BPF_RU_FACC_FCLOSE(
			int file_descriptor, 							/* ファイルディスクリプタ			*/
			int *errcd)									/* 詳細NGコード						*/
{
	DPRINT("\n");
	RETURN();
}

/* ファイルサイズ取得 */
int BPF_RU_FACC_FSIZE(
			const bpf_ru_facc_dpathname *dir_path_name,		/* ディレクトリパス名				*/
			const bpf_ru_facc_fname *file_name,				/* ファイル名						*/
			long *file_sz,									/* ファイルサイズ					*/
			int *errcd)									/* 詳細NGコード						*/
{
	DPRINT("\n");
	RETURN();
}

int BPF_RU_ITCM_THREAD_DELAY(struct bpf_timespec * a, int b, struct bpf_timespec *c)
{
	DPRINT("%dsec.%3dmsec.%3dusec (_ _)Zzz \n",a->tv_sec, a->tv_nsec/1000000 , ((a->tv_nsec)%1000000));
	nanosleep(a, c);
	RETURN();
}
void BPF_RU_ITCM_THREAD_EXIT(void)
{
	DPRINT("\n");
	return ;
}
int BPF_RU_ITCM_THREAD_DELETE(unsigned long int a)
{
	DPRINT("\n");
	RETURN();
}
int BPF_RU_ITCM_THREAD_IDSELF(unsigned long int *a)
{
	DPRINT("\n");
	RETURN();
}

int f_com_FA_encode(
 unsigned char *InBuf,		 	/* 被圧縮データ用バッファアドレス	 	*/
 unsigned char *OutBuf, 	 	/* 圧縮データ用バッファアドレス 		*/
 unsigned int	inlen,	 		/* 被圧縮データのデータバイトサイズ		*/
 unsigned int	outbsz, 		/* 圧縮データ用バッファバイトサイズ		*/
 FLDC4_ELEM    *blsadr, 		/* fldc4用圧縮辞書アドレス 				*/
 unsigned int  *outlen			/* 圧縮後のデータバイトサイズ 			*/
){DPRINT("\n");RETURN();}

int f_com_FA_decode(
 unsigned char *InBuf,		 	/* 圧縮データ用バッファアドレス 		*/
 unsigned char *OutBuf, 	 	/* 復元データ用バッファアドレス 		*/
 unsigned int	inlen,	 		/* 圧縮データのデータバイトサイズ	 	*/
 unsigned int	outbsz, 		/* 復元データ用バッファバイトサイズ		*/
 unsigned char *TmpBuf,			/* 復元テンポラリバッファアドレス		*/
 FLDC4_ELEM    *blsadr,			/* fldc4用圧縮辞書アドレス 				*/
 unsigned int  *outlen	 		/* 復元後のデータバイトサイズ	 		*/
){DPRINT("\n");RETURN();}


/* MSGQ */
int BPF_RU_ITCM_THREAD_MSGQ_SEND(unsigned int thr_q_num, unsigned int size,
									const void *msgp, int thr_q_flag,
									struct bpf_timespec *timeout)
{

 unsigned int count;
 unsigned int count2;
 unsigned int*  data = (unsigned int*)msgp;
 int len;

 DPRINT("q(%x) sz(%x(%d)) flg(%x) mId(%x) rId(%x) sId(%x) dId(%x) opt(%x) dlen(%x) \n",
 			thr_q_num,size,size,thr_q_flag,
 			data[0],
 			data[1],
 			data[2],
 			data[3],
 			data[4],
 			data[5]
 		);

 len = data[5];
 
 if(len > 0){
   unsigned int*  mdata;
   unsigned char* str_p;
   char datastr[256];
   int datalen;
   char str[64];
   int strlen;
   unsigned char val;
   int max;
   
   mdata = &data[6];
   str_p = (unsigned char*)mdata;
   datalen = 0;
	memset(datastr,0,sizeof(datastr));

	for( count = 0 ; count < (len/4) ; count++ ){
		datalen += snprintf(&datastr[datalen],sizeof(datastr)-datalen,"%08x ",mdata[count]);
		if((((count+1)%4) == 0) || ((count+1) >= (len/4)))
		{
			memset(str,0,sizeof(str));
			strlen=0;
			max = 16;
			if((((len/4) - count)*4) < 16)
			{
				max = (((len/4) - count)*4);
			}
			for(count2 = 0 ; count2 < max ; count2++,str_p++)
			{
				val = *str_p;
				if((val >= 0x21) && (val<=  0x7e))
				{
					strlen += snprintf(&str[strlen],sizeof(str)-strlen,"%c",val);
				}
				else
				{
					strlen += snprintf(&str[strlen],sizeof(str)-strlen,".");
				}
			}
			DPRINT("%s %s\n",datastr,str);
			str_p = (unsigned char*)&mdata[count+1];
			datalen = 0;
			memset(datastr,0,sizeof(datastr));
		}
	}
 }
  RETURN(); 

}
int BPF_RU_ITCM_THREAD_MSGQ_RECEIVE(unsigned int a , unsigned int b , void **c , unsigned int *d, int e, struct bpf_timespec *f)
{DPRINT("\n");RETURN();}
int BPF_RU_ITCM_THREAD_MSGQ_NUMMSGS(unsigned int a, unsigned int* b)
{DPRINT("\n");RETURN();}

/* MUTEX */
int BPF_RU_ITCM_THREAD_SEM_TAKE(unsigned int a, int b, struct bpf_timespec * c)
{DPRINT("\n");RETURN();}
int BPF_RU_ITCM_THREAD_SEM_GIVE(unsigned int a)
{DPRINT("\n");RETURN();}

/* Function Prototype */
int BPF_RU_MBLM_BUFFERALLOC(unsigned int buffer_type, unsigned int* buffer_remain, 
                           unsigned int* alloc_buffer_type, void** buffer_address){DPRINT("\n");RETURN();}
int BPF_RU_MBLM_BUFFERFREE(void* buffer_address){DPRINT("\n");RETURN();}


/* Function prototype */
int BPF_RU_HRTM_REGISTER (
			bpf_ru_hrtm_notify_type_t a,	/* notify type */
			unsigned int b,				/* timer ID		*/
			unsigned int c ,				/* time val msec */
			unsigned int d,				/* event no		*/
			unsigned int e,				/* queue id		*/
			int*f )						/* errno */
{DPRINT("\n");RETURN();}

int BPF_RU_HRTM_CANCEL(unsigned int a,int*b)
{DPRINT("\n");RETURN();}

unsigned int bpf_com_plog_get_count(int mode)
{DPRINT("\n");RETURN();}

void bpf_com_plog_device(
	char*			lname,
	e_bpf_com_plog_level level ,
	unsigned int     addr ,
	unsigned int     data ,
	unsigned int     opt1 ,
	unsigned int     opt2	
){
	DPRINT("\n");
	return;
}

void bpf_com_plog_set_level( char* name , int level ){DPRINT("\n");return ;}
void bpf_com_plog_set_level_local( int mode , int level ){DPRINT("\n");return ;}

int  bpf_com_plog_get_level( char* name ){DPRINT("\n");RETURN();}
int bpf_com_plog_get_level_local( int mode ){DPRINT("\n");RETURN();}

VOID BPF_COM_LOG_CPRI( VOID*	data )
{
	DPRINT("len(%d):%s\n",strlen(data),data);
	return;
}

VOID bpf_com_log_fprint(const char* outname,			/* Out先					*/
						int   logLevel,					/* Level(dmsegと同じ)		*/
						const char*	Message_p,			/* Message					*/
						... )
{
	FILE* fp = NULL;
	va_list args;
	char msg[256];

	va_start(args, Message_p);	/* pgr0039 */
	vsnprintf( msg , sizeof(msg) , Message_p, args );
	va_end(args);
	DPRINT("<%d>%s\n",logLevel,msg);
	return;
}


int BPF_RU_FACC_FREMOVE(
			const bpf_ru_facc_dpathname *dir_path_name,		/* ディレクトリパス名				*/
			const bpf_ru_facc_fname *file_name,				/* ファイル名						*/
			int *errcd){DPRINT("\n");return 0;}	


UINT  BPF_COM_LOG_SW_SYSTEM( UINT	logId,VOID*	textData ){DPRINT("\n");RETURN();}
UINT  BPF_COM_LOG_CLEAR_SW_SYSTEM( VOID ){DPRINT("\n");RETURN();}
UINT  BPF_COM_LOG_GET_SW_SYSTEM( VOID*	data){DPRINT("\n");RETURN();}

VOID BPF_COM_CMD_SET_CUR_CLK_CLASS(UCHAR current_clock_class){ return; }
VOID BPF_COM_CMD_SET_RESET_REASON(UINT reset_reason){ return; }
VOID BPF_COM_CMD_SET_CUR_PORTNO(UINT port_number){ return; }
VOID BPF_COM_CMD_SET_CUR_START_FACE(UINT startup_face){ return; }
VOID BPF_COM_CMD_SET_VLANTAG(UINT port ,UINT vid_dl , UINT vid_ul , UINT pcp_ul ){ return; }
VOID BPF_COM_CMD_SET_REMOTEMAC(UINT port ,UINT remote_mac_addr_h , UINT remote_mac_addr_l ){ return; }
VOID BPF_COM_CMD_SET_TXENDP(UINT carno ,UINT antno , UINT onoff, UINT Acell_id, UINT CC_id,UINT Port_id, UINT Acell_id_ex, UINT CC_id_ex,UINT Port_id_ex){ return; }
VOID BPF_COM_CMD_SET_RXENDP(UINT carno ,UINT antno , UINT onoff, UINT Acell_id, UINT CC_id,UINT Port_id, UINT Acell_id_ex, UINT CC_id_ex,UINT Port_id_ex){ return; }
VOID BPF_COM_CMD_SET_TXCC(UINT carno ,UINT antno ,UINT nr_freq, UINT nr_bandwidth, UINT nr_scs ){ return; }
VOID BPF_COM_CMD_SET_RXCC(UINT carno ,UINT antno ,UINT nr_freq, UINT nr_bandwidth, UINT nr_scs,UINT ul_fft_smpl_ofs,UINT ta_offset ){ return; }
VOID BPF_COM_CMD_CLR_ENDP(UINT val){ return; }
VOID BPF_COM_CMD_SETUP( VOID ){ return; }
VOID BPF_COM_CMD_SET_TXGAIN(UINT txgain){ return; }


unsigned int bpf_com_plog_tracekind = 0xFFFFFFFF;				/* トレース種別 */

VOID bpf_com_log_assert_real(	UINT	logLevel,				/* Level					*/
								const char* name ,				/* File Name				*/
								int line ,						/* 行数						*/
								const char*	Message_p,			/* Message					*/
								... ){

	va_list args;
	char msg[512];

	va_start(args, Message_p);	/* pgr0039 */
	vsnprintf( msg , sizeof(msg) , Message_p, args );
	va_end(args);
	DPRINT("<%d>;%s(%d);%s\n",logLevel,name,line,msg);
	return;
}



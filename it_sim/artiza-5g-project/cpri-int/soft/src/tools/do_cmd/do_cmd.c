#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>

void cmd_fprint(const char* outname,					/* Outæ					*/
						int   logLevel,					/* Level(dmseg‚Æ“¯‚¶)		*/
						const char*	Message_p,			/* Message					*/
						... )							/* ‰Â•Ïî•ñ					*/
{
	FILE* fp = NULL;
	va_list args;
	char msg[256];
	if(( fp = fopen(outname,"w") ) != NULL ){
		va_start(args, Message_p);	/* pgr0039 */
		vsnprintf( msg , sizeof(msg) , Message_p, args );
		va_end(args);
		fprintf(fp,"<%d>%s",logLevel,msg);
		fflush(fp);
		fclose(fp);
	}
	return;
}

#define CMD_DMESG( Message_p, ... ) cmd_fprint( "/dev/kmsg" , 0 , Message_p, ##__VA_ARGS__ )
void cmd_fprint(const char* outname , int logLevel , const char* Message_p , ... );

int main( int argc, char* argv[] )
{
	int cmdpid = 0;
	char* cmd_argv[2];
	int policy = 0;
	int cmdpolicy = 0;
	int cmdpriority = 0;
	struct sched_param param;
	int rtn = 0;
	char top_char;

	if( argc < 2 ){
		printf("param error,%s <cmdname or pid> <policy> <priority> \n",argv[0]);
		return -1;
	}
	if(argv[1] == NULL){
		printf("cmd_argv is null \n");
		return -1;
	}

	cmd_argv[0] = argv[1];
	cmd_argv[1] = NULL;

	/* policy */
	policy = 0;
	if(argc > 2){
		policy = atoi(argv[2]);
	}
	switch(policy){
		case 1:
			cmdpolicy = SCHED_FIFO;
			break;
		case 2:
			cmdpolicy = SCHED_RR;
			break;
		case 0:
		default:
			cmdpolicy = SCHED_OTHER;
			break;
	}
	/* priority */
	cmdpriority = 0;
	if(argc > 3){
		cmdpriority = atoi(argv[3]);
	}
	memset(&param, 0, sizeof(param));
	param.sched_priority = cmdpriority;
	errno = 0;

	memcpy(&top_char,cmd_argv[0],sizeof(top_char));
	if(( top_char >='0' )&& (top_char <= '9')){
		cmdpid = (int)strtoul(cmd_argv[0], NULL,10);
		rtn = sched_setscheduler(cmdpid, cmdpolicy, &param);
		CMD_DMESG("%s set(%d)(pid:%d,poli:%d,prio:%d)\n",cmd_argv[0],rtn,cmdpid,cmdpolicy,cmdpriority);
	}else{
		cmdpid = fork();
		if(cmdpid == 0){
			if (( rtn = execve(cmd_argv[0],cmd_argv,NULL)) != 0 ){
				CMD_DMESG("%s execve error(rc:%d eno:%d) \n",cmd_argv[0],rtn,errno);
				return -2;
			}
		}
		else if(cmdpid == -1){
			CMD_DMESG("fork error(%d) \n",errno);
			return -3;
		}
		else{
			rtn = sched_setscheduler(cmdpid, cmdpolicy, &param);
			CMD_DMESG("%s start(%d)(pid:%d,poli:%d,prio:%d)\n",cmd_argv[0],rtn,cmdpid,cmdpolicy,cmdpriority);
		}
	}
	return 0;
}


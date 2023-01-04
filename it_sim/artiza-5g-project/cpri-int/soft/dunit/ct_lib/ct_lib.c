#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>

typedef struct{
	unsigned int vflg;
	long		 result;
}t_call_result;

typedef struct{
	char*			name;
	t_call_result*	ptn;
}t_call_result_ptn;

t_call_result result_open;
t_call_result result_close;
t_call_result result_socket;
t_call_result result_system;
t_call_result result_ioctl;
t_call_result result_fork;
t_call_result result_fopen;
t_call_result result_fclose;
t_call_result result_bind;
t_call_result result_accept;
t_call_result result_accept_cnt;
t_call_result result_listen;
t_call_result result_send;
t_call_result result_recv;
t_call_result result_recv_cnt;
t_call_result result_recv_len;

static t_call_result_ptn result[] = 
{
	{ "open"    , &result_open    },
	{ "close"   , &result_close   },
	{ "system"  , &result_system  },
	{ "socket"  , &result_socket  },
	{ "ioctl"   , &result_ioctl   },
	{ "fork"    , &result_fork    },
	{ "fopen"   , &result_fopen   },
	{ "fclose"  , &result_fclose  },
	{ "bind"    , &result_bind    },
	{ "accept"  , &result_accept  },
	{ "acceptCnt"  , &result_accept_cnt  },
	{ "listen"  , &result_listen  },
	{ "send"    , &result_send    },
	{ "recv"    , &result_recv    },
	{ "recvCnt" , &result_recv_cnt },
	{ "recvLen" , &result_recv_len },
	{ NULL      , NULL            }
};

void rrulib_memset(char* s1 , char val , int size)
{
	unsigned int count;
	
	for(count=0;count < size ; count++)
	{
		s1[count]=val;
	}
	return;
}


void rrulib_memcpy(char* s1 , char* s2 , int size)
{
	unsigned int count;
	
	for(count=0;count < size ; count++)
	{
		s1[count]=s2[count];
	}
	return;
}

int rrulib_strcmp(char* s1 , char* s2)
{
	unsigned int  cnt = 0;
	int           ret = 0;
	unsigned int  len = 0;

	while( 1 )
	{
		if (s1[cnt] == '\0')
		{
			break;
		}
		cnt++;
	}
	len = cnt + 1;
	for (cnt = 0 ; cnt < len ; cnt++)
	{
		if (s1[cnt] > s2[cnt])
		{
			ret = 1;
			break;
		}
		else if (s1[cnt] < s2[cnt])
		{
			ret = -1;
			break;
		}
		else
		{
			continue;
		}
	}
	return(ret);
}


void rrulib_callresult_init(char* name )
{
	unsigned int count;
	for( count=0 ; count < (sizeof(result)/sizeof(result[0])) ; count++)
	{
		if(result[count].name == NULL)
		{
			break;
		}
		if( rrulib_strcmp(result[count].name , name ) )
		{
			continue;
		}
		result[count].ptn->vflg = 0;
		break;
	}
	return;
}

void rrulib_callresult_set(char* name , long val)
{
	unsigned int count;
	for( count=0 ; count < (sizeof(result)/sizeof(result[0])) ; count++)
	{
		if(result[count].name == NULL)
		{
			break;
		}
		if( rrulib_strcmp(result[count].name , name ) )
		{
			continue;
		}
		result[count].ptn->vflg = 1;
		result[count].ptn->result = val;
		break;
	}
	return;
}

long rrulib_callresult_get(char* name )
{
	unsigned int count;
	long ret = 0;
	for( count=0 ; count < (sizeof(result)/sizeof(result[0])) ; count++)
	{
		if(result[count].name == NULL)
		{
			break;
		}
		if( rrulib_strcmp(result[count].name , name ) )
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


static void* syscall_fp = NULL;


extern void f_rrulib_dmsg(
	const char* name ,
	const char* funcname,
	const char* format,
	  ...);

#define DPRINT(...) f_rrulib_dmsg("SYS", __func__ ,__VA_ARGS__ )

static void* rrulib_symsearch(const char* libname , const char* callname ,void** fd)
{
	if(*fd == NULL)
	{
		if((*fd = dlopen(libname, RTLD_LAZY)) == NULL)
		{
			fprintf (stderr, "%s\n", dlerror());
			return NULL;
		}
	}
	return dlsym(*fd, callname);
}

/*********************************************************************************/
/* global																		 */
/*********************************************************************************/
int socket(int domain, int type, int protocol)
{
	int ret = 0;
	int (*func)(int, int , int );

	DPRINT("%x,%x,%x \n",domain,type,protocol);
	if( result_socket.vflg != 0)
	{
		ret = result_socket.result;
		DPRINT("debug Return %x\n",ret);
	}
	else
	{
		func = rrulib_symsearch("libc.so.6","socket",&syscall_fp);
		ret = func(domain,type,protocol);
		DPRINT("Return %x\n",ret);
	}
	return ret;
}

int close(int fd)
{
	int ret = 0;
	int (*func)(int);

	DPRINT("%x \n",fd);
	if( result_close.vflg != 0)
	{
		ret = result_close.result;
		DPRINT("debug Return %x\n",ret);
	}
	else
	{
		func = rrulib_symsearch("libc.so.6","close",&syscall_fp);
		ret = func(fd);
		DPRINT("Return %x\n",ret);
	}
	return ret;
}

int ioctl(int d, unsigned long request, ...)
{
	int ret = 0;

	DPRINT("%x \n",d);
	if( result_ioctl.vflg != 0)
	{
		ret = result_ioctl.result;
		DPRINT("debug Return %x\n",ret);
	}
	else
	{
		ret = 0;
		DPRINT("Return %x\n",ret);
	}
	return ret;
}

int system(char* cmd)
{
	int ret = 0;

	DPRINT("%s \n",cmd);
	if( result_system.vflg != 0)
	{
		ret = result_system.result;
		DPRINT("debug Return %x\n",ret);
	}
	else
	{
		ret = 0;
		DPRINT("Return %x\n",ret);
	}
	return ret;
}

int open(const char *pathname, int flags, mode_t mode)
{
	int ret = 0;
	int (*func)(const char *, int , mode_t );

	DPRINT("%s %x %x \n",pathname,flags,mode);
	if( result_open.vflg != 0)
	{
		ret = result_open.result;
		DPRINT("debug Return %x\n",ret);
	}
	else
	{
		func = rrulib_symsearch("libc.so.6","open",&syscall_fp);
		ret = func(pathname,flags,mode);
		DPRINT("Return %x\n",ret);
	}
	return ret;
}
pid_t fork(void)
{
	int ret = 0;
	if( result_fork.vflg != 0)
	{
		ret = result_fork.result;
		DPRINT("debug Return %x\n",ret);
	}
	else
	{
		ret = 0;
		DPRINT("Return %x\n",ret);
	}
	return 0;
}

FILE *fopen(const char *filename, const char *mode)
{
	FILE* ret = NULL;
	FILE* (*func)(const char *, const char* );
	char  name[384];

	DPRINT("path:\"%s\" mode:\"%s\" \n",filename,mode);
	if( result_fopen.vflg != 0)
	{
		ret = (FILE*)(result_fopen.result);
		DPRINT("debug Return %lx\n",(unsigned long)ret);
	}
	else
	{
		if(filename[0] == '/')
		{
			rrulib_memset(name,0,sizeof(name));
			snprintf(name,sizeof(name),"./rootfs%s",filename);
			func = rrulib_symsearch("libc.so.6","fopen",&syscall_fp);
			ret = func(name,mode);
			DPRINT("change to \"%s\" Return %lx\n",name,(unsigned long)ret);
		}
		else
		{
			func = rrulib_symsearch("libc.so.6","fopen",&syscall_fp);
			ret = func(filename,mode);
		}
	}
	return ret;
}

int fclose( FILE *fp )
{
	int ret;
	int (*func)(FILE* );

	DPRINT("%lx \n",(unsigned long)fp);
	if( result_fclose.vflg != 0)
	{
		ret = result_fclose.result;
		DPRINT("debug Return %x\n",ret);
	}
	else
	{

		func = rrulib_symsearch("libc.so.6","fclose",&syscall_fp);
		ret=func(fp);
	}
	return ret;
}

int bind(int fd, void *addr, int addrlen)
{
	int ret = 0;
	DPRINT("%x \n",fd);
	if( result_bind.vflg != 0)
	{
		ret = result_bind.result;
		DPRINT("debug Return %x\n",ret);
	}
	else
	{
		ret = 0;
		DPRINT("Return %x\n",ret);
	}
	return ret;
}

int accept(int fd, void *addr, void *addrlen)
{
	int ret = 0;
	DPRINT("%x \n",fd);
	if((result_accept.vflg != 0) && (result_accept_cnt.vflg != 0) && (result_accept_cnt.result > 0))
	{
		ret = result_accept.result;
		DPRINT("debug Return %x\n",ret);
		result_accept_cnt.result--;
	}
	else
	{
		ret = -1;
		DPRINT("Return %x\n",ret);
	}
	return ret;
}

int listen(int fd, int backlog)
{
	int ret = 0;
	DPRINT("%x \n",fd);
	if( result_listen.vflg != 0)
	{
		ret = result_listen.result;
		DPRINT("debug Return %x\n",ret);
	}
	else
	{
		ret = 0;
		DPRINT("Return %x\n",ret);
	}
	return ret;
}

static char	tmp_buf_str[1024*1024];

long send(int fd, const void *buf, long len, int flags)
{
	long	ret;
	char*	tmp_buf;
	char*	tmp_buf2;

	unsigned int count;
	unsigned int count2;
	unsigned int count3;

	DPRINT("%x \n",fd);
	if( result_send.vflg != 0)
	{
		ret = result_send.result;
		DPRINT("debug Return %lx\n",ret);
	}
	else
	{
		ret = len;
		if ( buf != NULL){
			tmp_buf = (char*)buf;
			tmp_buf2 = (char*)buf;
			rrulib_memset(tmp_buf_str,0,sizeof(tmp_buf_str));
			for(count=0 , count2=0 ; ((count < (unsigned int)len)) && (( count2 < sizeof(tmp_buf_str) )) ; count++ ,tmp_buf++){
				count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ,"%02X",(unsigned char)*tmp_buf);
				if(((count + 1) % 4) == 0){
					count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ," ");
				}
				if(((count + 1) % 32) == 0){
					for(count3=0;count3<32;count3++){
						if( (*tmp_buf2 >= 0x21) && (*tmp_buf2 <= 0x7e) ){
							count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ,"%c",(unsigned char)*tmp_buf2);
						}else{
							count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ,".");
						}
						tmp_buf2++;
					}
					count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ,"\n");
				}
			}
		}

		DPRINT("Message %dbytes\n",ret);
		DPRINT("\n%s \n",tmp_buf_str);
	}
	return ret;
}

long recv(int fd, void *buf, long len, int flags)
{
	long	ret;
	char*	tmp_buf;
	char*	tmp_buf2;
	
	unsigned int count;
	unsigned int count2;
	unsigned int count3;

	long		slen;

	DPRINT("%x \n",fd);
	if(( result_recv.vflg != 0) && (result_recv.result != 0 ) && (result_recv_cnt.vflg != 0) && (result_recv_cnt.result > 0))
	{
		if((result_recv_len.vflg != 0 ) && (len > result_recv_len.result)){
			rrulib_memcpy(buf, (char*)(result_recv.result),result_recv_len.result);
			slen = result_recv_len.result;
		}else{
			rrulib_memcpy(buf, (char*)(result_recv.result),len);
			slen = len;
		}
		tmp_buf = (char*)buf;
		tmp_buf2 = (char*)buf;

		rrulib_memset(tmp_buf_str,0,sizeof(tmp_buf_str));
		for(count=0 , count2=0 ; ((count < (unsigned int)slen)) && (( count2 < sizeof(tmp_buf_str) )) ; count++ ,tmp_buf++){
			count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ,"%02X",(unsigned char)*tmp_buf);
			if(((count + 1) % 4) == 0){
				count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ," ");
			}
			if(((count + 1) % 32) == 0){
				for(count3=0;count3<32;count3++){
					if( (*tmp_buf2 >= 0x21) && (*tmp_buf2 <= 0x7e) ){
						count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ,"%c",(unsigned char)*tmp_buf2);
					}else{
						count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ,".");
					}
					tmp_buf2++;
				}
				count2 += snprintf( &tmp_buf_str[count2],sizeof(tmp_buf_str) - count2 ,"\n");
			}
		}
		ret = slen;
		DPRINT("Message %d bytes\n",slen);
		DPRINT("\n%s \n",(unsigned char*)tmp_buf_str);
		result_recv_cnt.result--;
	}
	else
	{
		ret = -1;
		DPRINT("Return %lx\n",ret);
	}
	return ret;
}

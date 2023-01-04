#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/types.h>
#include <errno.h>

#define D_DEV_REG_NAME			"/dev/mem"
#define D_DEV_REG_AL32		4
#define D_DEV_REG_AL16		2
#define D_DEV_REG_AL8		1
#define D_DEV_SRAM_SIZE		(8*1024*1024)	/* 8M */



struct cmd_info{
	unsigned int addr;
	unsigned int val;
};

struct fjio_cmd_info {
	int				inum;
	struct cmd_info info[1];		/* 可変長 */
};


typedef enum{
	E_DEV_CTL_QSPI_LI = 0,
	E_DEV_CTL_QSPI_IO,
	E_DEV_CTL_QSPI_MAX
}e_dev_qspi;

typedef enum{
	E_DEV_CTL_NONE = 0,
	E_DEV_CTL_READ,
	E_DEV_CTL_WRITE,
	E_DEV_CTL_WRITE_K,
	E_DEV_CTL_READ_EEP,
	E_DEV_CTL_READ_EEPALL,
	E_DEV_CTL_WRITE_EEP,
	E_DEV_CTL_READ_QSPI,
	E_DEV_CTL_ERASE_QSPI,
	E_DEV_CTL_WRITE_QSPI,
	E_DEV_CTL_IRQ,
	E_DEV_CTL_READ_H,
	E_DEV_CTL_WRITE_H,
	E_DEV_CTL_WRITE_UART1,
	E_DEV_CTL_READ_UART1,
	E_DEV_CTL_READ_WRITE,
	E_DEV_CTL_READ_MASK_WRITE,
	E_DEV_CTL_WRITE_SPI,
	E_DEV_CTL_READ_SPI
}e_dev_ctl;

typedef struct 
{
	e_dev_ctl		mode;
	int				fd;
	unsigned int	addr;
	unsigned int	size;
	unsigned int	val;
	unsigned int	lsbfst;
	void*			page_ptr;
	unsigned		page_offset;
	unsigned		page_size;
	void*			workbuf_p;
	int				worksize;
}t_dev_ctrl;

static int flg_dbg = 0;

void func_dmsg(const char* filename , const int line,  const char* format, ...)
{
	if(flg_dbg == 0)
	{
		return;
	}
	char str[256];
	int len;
	va_list args;

	len = snprintf(str,sizeof(str),"DEBUG;%s(%d);", filename,line );
	va_start(args, format);	/* pgr0039 */
	len +=  vsnprintf( &str[len] , sizeof(str) - len, format, args);
	va_end(args);

	fprintf(stdout, str);
	fflush(stdout);
	return;
}

#define dmsg(...) func_dmsg( __FILE__ , __LINE__ , __VA_ARGS__ )


void usage( char* argv )
{
	printf("SRAM ctrl tool usage:\n");
	printf("\n");
	printf(" - SRAM write \n");
	printf("   %s w <sram_no> <file_name>       \n",argv);
	printf("        <sram_no>  0:SRAM0 , 1:SRAM1  \n");
	printf("        <file_name> :Input IQ data    \n");
	printf(" - SRAM read \n");
	printf("   %s r <sram_no> <file_name>       \n",argv);
	printf("        <sram_no>  0:SRAM0 , 1:SRAM1  \n");
	printf("        <file_name> :Output IQ data   \n");
	printf("\n");
	return;
}

unsigned int func_argv_conv( char* in_p , char* out_p , int maxlen )
{
	int len;
	
	memset(out_p,0,maxlen);
	
	len = strlen(in_p);
	if( len < 2)
	{
		snprintf(out_p,maxlen,"0x%s",in_p);
	}
	else
	{
		if(!strncmp(in_p,"0x",2))
		{
			snprintf(out_p,maxlen,"%s",in_p);
		}
		else
		{
			snprintf(out_p,maxlen,"0x%s",in_p);
		}
	}	
	return ( strtoul(out_p,NULL,0) );
}

void* func_reg_mmap(int fd, unsigned int addr , unsigned int size ,unsigned* page_offset ,unsigned* page_size_o )
{
	void *ptr;
	unsigned page_addr;
	unsigned page_addr_l;
	unsigned page_size=sysconf(_SC_PAGESIZE);

	page_addr   = (addr & ~(page_size-1));
	page_addr_l = ((addr + size) & ~(page_size-1));
	/* ページ越え */
	if(page_addr != page_addr_l)
	{
		page_size += page_addr_l - page_addr;
	}
	if((ptr = mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,page_addr)) == (void*)-1 )	/* pgr0203 */
	{
		return (ptr);
	}
	*page_offset= addr - page_addr;
	*page_size_o = page_size;
	return (ptr);
}


e_dev_ctl func_dev_mode(int argc, char *argv[])
{
	int len = 0;
	e_dev_ctl mode;

	if(( argc < 2 ) || (argc >= 16 ))
	{
		return(E_DEV_CTL_NONE);
	}
	len = strlen(argv[1]);
	if(len < 1)
	{
		return(E_DEV_CTL_NONE);
	}
	/* mode */
	mode = E_DEV_CTL_NONE;
	switch(argv[1][0])
	{
		case 'w':
			mode = E_DEV_CTL_WRITE;
			break;
		case 'r':
			mode = E_DEV_CTL_READ;
			break;
		default:
			return(E_DEV_CTL_NONE);
			break;
	}
	return(mode);
}

void* func_dev_create( int argc, char *argv[] , t_dev_ctrl* ctrl_p )
{
	e_dev_ctl		mode;
	unsigned int	addr = 0;
	unsigned int	size  = 0;
	unsigned int	val = 0;
	char			argv_conv[16];
	void*			ptr = (void*)-1;
	int				fd = -1;
	unsigned		page_offset = -1;
	unsigned		page_size = -1;
	void*			workbuf_p = NULL;
	unsigned int	sram_no;
	
	/* mode */
	mode = func_dev_mode(argc,argv);
	switch(mode)
	{
		case E_DEV_CTL_NONE:
			ctrl_p->mode = E_DEV_CTL_NONE;
			return(NULL);
			break;
		case E_DEV_CTL_WRITE:
		case E_DEV_CTL_READ:
			if( argc < 4)
			{
				return((void*)-1);
			}
			sram_no = func_argv_conv(argv[2],argv_conv,sizeof(argv_conv));
			switch(sram_no){
				case 0:
					addr = 0xA3000000;
					break;
				case 1:
					addr = 0xA4000000;
					break;
				default:
					return((void*)-1);
					break;
			}
			size = D_DEV_SRAM_SIZE;
			val  = sram_no;
			break;
		default:
			return((void*)-1);
			break;
	}
	switch(mode)
	{
		case E_DEV_CTL_WRITE:
		case E_DEV_CTL_READ:
			if( ( fd = open(D_DEV_REG_NAME,O_RDWR) ) == -1)
			{
				if(workbuf_p != NULL)
				{
					free(workbuf_p);
				}
				return((void*)-1);
			}
			if(( ptr = func_reg_mmap(fd,addr,size,&page_offset,&page_size )) == (void*)-1 ) /* pgr0203 */
			{
				close(fd);
				if(workbuf_p != NULL)
				{
					free(workbuf_p);
				}
				return((void*)-1);
			}
			break;
		default:
			if(workbuf_p != NULL){
				free(workbuf_p);
			}
			return((void*)-1);
			break;
	}
	/* ctrl setting */
	ctrl_p->mode		 = mode;
	ctrl_p->fd			 = fd;
	ctrl_p->addr		 = addr;
	ctrl_p->size		 = size;
	ctrl_p->val			 = val;
	ctrl_p->page_ptr	 = ptr;
	ctrl_p->page_offset	 = page_offset;
	ctrl_p->page_size	 = page_size;
	ctrl_p->workbuf_p	 = workbuf_p;
	ctrl_p->worksize	 = size;
	return(ptr); /* pgr0524 */
}

/* clean */
void func_dev_clean( t_dev_ctrl* ctrl_p )
{
	if(ctrl_p->workbuf_p != NULL)
	{
		free(ctrl_p->workbuf_p);
	}
	if( (ctrl_p->page_ptr != (void*)-1) && (ctrl_p->page_ptr != NULL) ) /* pgr0203 */
	{
		munmap(ctrl_p->page_ptr,ctrl_p->page_size);
	}
	if(ctrl_p->fd != -1)
	{
		close(ctrl_p->fd);
	}
	return;
}

int main(int argc, char *argv[])
{
	t_dev_ctrl ctrl;
	void *ptr = (void*)-1;
	unsigned int count = 0;
	unsigned int wcount = 0;
	unsigned int rtn=0;
	FILE*			fp;
	volatile unsigned int *sram_addr_p;
	int	size;
	int	len;
	void*	buf_p;
	volatile unsigned int *	wbuf_p;

	if(argc < 2 )
	{
		printf("input param error\n");
		usage(argv[0]);
		exit(-1);
	}

	for(count=0; count < argc ;count++){
		switch(argv[count][0]){
			case 'v':
				flg_dbg = 1;
				break;
			default:
				break;
		}
		if(flg_dbg == 1){
			break;
		}
	}
	
	memset(&ctrl,0,sizeof(ctrl));
	if(( ptr = func_dev_create( argc, argv , &ctrl ) ) == (void*)-1 ) /* pgr0203 */
	{
		printf("input param error 2\n");
		exit(-1);
	}
	/* デバイズ毎の処理実行 */
	switch(ctrl.mode)
	{
		case E_DEV_CTL_WRITE:

			printf("IQ data Input file(%s) read...\n",argv[3]);
			if((fp = fopen(argv[3],"rb")) == NULL){
				printf("File Open error(%s)\n",argv[3]);
				rtn = 1;
				break;
			}
			fseek(fp,0, SEEK_END);
			size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			if((buf_p = malloc(size)) == NULL){
				printf("alloc memory error(%dbytes)\n",size);
				fclose(fp);
				rtn = 2;
				break;
			}
			if((len = fread(buf_p, 1, size , fp)) != size){
				printf("file read error(%d,%d) \n",len,size);
				free(buf_p);
				fclose(fp);
				rtn = 3;
				break;
			}
			fclose(fp);
			printf("%dbytes OK\n",len);
			sram_addr_p = (volatile unsigned int*)(ptr + ctrl.page_offset);
			wbuf_p = (volatile unsigned int*)buf_p;
			if(size > ctrl.size){
				wcount = ctrl.size/4;
			}else{
				wcount = size/4;
			}
			printf("SRAM%d write(%d counts) start...\n",ctrl.val,wcount);
			for(count = 0 ; count < wcount ; count++ , sram_addr_p++,wbuf_p++){
				*sram_addr_p = htonl(*wbuf_p);
			}
			msync( ptr , ctrl.page_size , MS_SYNC );
			printf("OK\n");
			free(buf_p);
			break;
		case E_DEV_CTL_READ:

			printf("IQ data Output file(%s) open...\n",argv[3]);
			if((fp = fopen(argv[3],"w")) == NULL){
				printf("File Open error(%s)\n",argv[3]);
				rtn = 1;
				break;
			}
			if((buf_p = malloc(ctrl.size)) == NULL){
				printf("alloc memory error(%dbytes)\n",ctrl.size);
				fclose(fp);
				rtn = 2;
				break;
			}
			memset(buf_p,0,ctrl.size);
			printf("OK\n");

			sram_addr_p = (volatile unsigned int*)(ptr + ctrl.page_offset);
			wbuf_p = (volatile unsigned int*)buf_p;
			wcount = ctrl.size/4;

			printf("SRAM%d read(%d counts) start...\n",ctrl.val,wcount);
			for(count = 0 ; count < wcount ; count++ , sram_addr_p++,wbuf_p++){
				*wbuf_p = htonl(*sram_addr_p);
			}
			if((len = fwrite(buf_p, 1, ctrl.size , fp)) < 1){
				printf("file write error(%d,%d)\n",len,ctrl.size);
				free(buf_p);
				fclose(fp);
				rtn = 3;
				break;
			}
			free(buf_p);
			fflush(fp);
			fclose(fp);
			printf("OK(%dbytes)\n", ctrl.size);
			break;
		case E_DEV_CTL_NONE:
			usage(argv[0]);
			return 0;
			break;
		default:
			usage(argv[0]);
			break;
	}
	func_dev_clean( &ctrl );
	return ((int)rtn);
}

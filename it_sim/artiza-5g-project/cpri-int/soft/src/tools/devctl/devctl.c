#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <mtd/mtd-user.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <errno.h>

#define D_DEV_EEPROM_MAX		0x800
#define D_DEV_QSPI_MAX			0x8000000		/* 128M */

#define D_DEV_REG_NAME			"/dev/mem"
#define D_DEV_EEP_NAME			"/dev/i2c-0"
#define D_DEV_QSPI_NAME			"/dev/mtd0"
#define D_DEV_UIO_NAME			"/dev/uio"
#define D_DEV_UART1_NAME		"/dev/ttyPS1"

#define D_DEV_SPI_PTP_NAME		"/dev/spidev2.0"
#define D_DEV_SPI_RFICPLL_NAME	"/dev/spidev2.1"
#define D_DEV_SPI_RFIC_NAME		"/dev/spidev3.1"
#define D_DEV_SPI_PLL_NAME		"/dev/spidev3.0"

#define D_DEV_KREG_NAME			"/dev/fjio"

#define D_DEV_EEP_PROTECT_UNLOCK	"/opt/debug/cmd_devctl rm FF0A0044 00001000 1>/dev/null"
#define D_DEV_EEP_PROTECT_LOCK		"/opt/debug/cmd_devctl rw FF0A0044 00001000 1>/dev/null"

#define D_DEV_REG_AL32		4
#define D_DEV_REG_AL16		2
#define D_DEV_REG_AL8		1


#define FJIO_SET	0
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
	printf("device ctrl tool usage:\n");
	printf("\n");
	printf(" - device read \n");
	printf("   %s r  <addr> <size>            : register read      \n",argv);
	printf("   %s rh <addr> <size>            : register half read \n",argv);
	printf("   %s rm <addr> <size>            : register read and mask write \n",argv);
	printf("   %s rw <addr> <size>            : register read and write \n",argv);
	printf("   %s er <addr> <size>            : eeprom read        \n",argv);
	printf("   %s ea                          : eeprom read all    \n",argv);
	printf("   %s fr <ofs> <size>             : flash read         \n",argv);
	printf("   %s ur <size>                   : read uart1         \n",argv);
	printf("   %s sr <addr> <size> <spidevno> : spi read,<spidevno> is 0:ptp,1:rfic,2:pll      \n",argv);
	printf("\n");
	printf(" - device write \n");
	printf("   %s w  <addr> <val>             : register write        \n",argv);
	printf("   %s wh <addr> <size>            : register half write   \n",argv);
	printf("   %s wk  <addr> <val> <dname>    : register write by kernelmode    \n",argv);
	printf("   %s ew <addr> <val>             : eeprom write          \n",argv);
	printf("   %s fe <ofs> <size>             : flash erase           \n",argv);
	printf("   %s fw <ofs> <file path>        : flash write           \n",argv);
	printf("   %s uw <val>                    : write uart1           \n",argv);
	printf("   %s sw <addr> <val> <spidevno>  : spi write,<spidevno> is 0:ptp,1:rfic,2:pll      \n",argv);
	printf("\n");
	printf(" - device irq read \n");
	printf("   %s i <UIO No>                 : UIO no               \n",argv);
	printf(" **** <addr>, <ofs> , <val> , <size> is hex value. ");	
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

unsigned int func_io_write(int fd, unsigned int addr , unsigned int val )
{
	struct fjio_cmd_info fj_cmd;

	fj_cmd.inum = 1;
	fj_cmd.info[0].addr = addr;
	fj_cmd.info[0].val = val;	
	if( ioctl(fd, FJIO_SET, &fj_cmd ) != 0 ){
		return 1;
	}
	return 0;
}


unsigned int func_eep_write(int fd, unsigned int addr , unsigned int val )
{
	char buf[2];
	unsigned baseaddr;
	struct timespec stime;

	baseaddr = ((addr >> 8) & 0x07) | 0x00000050 ;
	if( ioctl(fd, I2C_SLAVE, baseaddr ) != 0 )
	{
		return 1;
	}
	memset(buf,0,sizeof(buf));
	stime.tv_sec = 0;
	stime.tv_nsec = 10000000;	/* 10msec */
	do {
		buf[0] = (char)(addr & 0xFF);
		buf[1] = (char)(val & 0xFF);
		if(write(fd,buf,sizeof(buf)) <= 0 )
		{
			return 2;
		}
		nanosleep(&stime,NULL);
	}while( 0 );
	return 0;
}


unsigned char func_eep_read(int  fd, unsigned int addr )
{
	char buf[2];
	unsigned baseaddr;

	memset(buf,0,sizeof(buf));
	baseaddr = ((addr >> 8) & 0x07) | 0x00000050 ;
	if( ioctl(fd, I2C_SLAVE, baseaddr ) != 0 )
	{
		return -1;
	}
	buf[0] = (char)(addr & 0xFF);
	if( write(fd,buf,1) <= 0 )
	{
		buf[0] = 0;
		return -1;
	}
	else
	{
		buf[0] = 0;
		if( read(fd,buf,1) <= 0 )
		{
			return -1;
		}
	}
	return(buf[0]);
}

int func_qspi_erase(int  fd, unsigned int addr , unsigned int size )
{
	int ret;
	mtd_info_t		mtd_info;
	erase_info_t	ei;

	memset(&mtd_info,0,sizeof(mtd_info));
	memset(&ei,0,sizeof(ei));
	ret = 0;

	if(lseek(fd,0,SEEK_SET) == -1)
	{
		return(-1);
	}
	if(ioctl(fd, MEMGETINFO, &mtd_info ) == -1)
	{
		return(-2);
	}
	ei.length = mtd_info.erasesize;
	for( ei.start = addr; ei.start < (addr + size) ; ei.start += mtd_info.erasesize )
	{
		if(ioctl(fd, MEMERASE, &ei ) == -1 )
		{
			ret= -3;
			break;
		}
	}
	return(ret);
}

int func_qspi_read(int  fd, unsigned int addr , unsigned int size , char* data )
{
	if(lseek(fd,addr,SEEK_SET) == -1)
	{
		return(-1);
	}
	return(read(fd,data,size));
}

int func_qspi_write(int  fd, unsigned int addr , unsigned int size , char* data )
{
	if(lseek(fd,addr,SEEK_SET) == -1)
	{
		return(-1);
	}
	return(write(fd,data,size));
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


unsigned char fund_reverse_bit8(unsigned char m , unsigned int lsbfst)
{
#if 0
	/* mode でlsb,msb設定できるため不要 */
	if(lsbfst==1){
		m = ((m & 0x55) << 1) | ((m & 0xAA) >> 1);
		m = ((m & 0x33) << 2) | ((m & 0xCC) >> 2);
		m = ((m << 4) | (m >> 4));
	}
#endif
	return m;
}


int func_spi_write(int  fd, unsigned int addr ,unsigned int data , char ac_size , unsigned int lsbfst)
{
	int					ret;
	unsigned char		tx_buf[16];
	unsigned char		rx_buf[16];
	int					len;
	
	memset( tx_buf, 0x00, sizeof(tx_buf) );
	memset( rx_buf, 0x00, sizeof(rx_buf) );

	if(ac_size == 1){
		tx_buf[0]  = fund_reverse_bit8((unsigned char)(addr & 0xFF),lsbfst);
		tx_buf[1]  = fund_reverse_bit8((unsigned char)(data & 0xFF),lsbfst);
		len = 2;
	}else{
		tx_buf[0]  = fund_reverse_bit8((unsigned char)((addr>>8) & 0x7F),lsbfst);	/* write bit is 0 */
		tx_buf[1]  = fund_reverse_bit8((unsigned char)(addr & 0xFF),lsbfst);
		tx_buf[2]  = fund_reverse_bit8((unsigned char)(data & 0xFF),lsbfst);
		len = 3;
	}

	dmsg("spi write addr:%08x send %02x %02x %02x %02x \n",addr,tx_buf[0],tx_buf[1],tx_buf[2],tx_buf[3]);
	struct spi_ioc_transfer spi_data = 
	{
		.tx_buf = (unsigned long)tx_buf,
		.rx_buf = (unsigned long)rx_buf,
		.len = len,
		.delay_usecs = 0,
		.speed_hz = 12500000,
		.bits_per_word = 8,
	};

#if 1
	unsigned char		mode;
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1){
		printf("SPI_IOC_WR_MODE error(%d)\n",errno);
		return(ret);
	}
	mode &= ~(SPI_CPHA | SPI_CPOL);
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1){
		printf("SPI_IOC_WR_MODE error(%d)\n",errno);
		return(ret);
	}
#endif
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &(spi_data.bits_per_word));
	if (ret == -1){
		printf("SPI_IOC_WR_BITS_PER_WORD error(%d)\n",errno);
		return(ret);
	}
	
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &(spi_data.speed_hz ));
	if (ret == -1){
		printf("SPI_IOC_WR_MAX_SPEED_HZ error(%d)\n",errno);
		return(ret);
	}
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &spi_data );
	if (ret < 1){
		printf("SPI_IOC_MESSAGE error(%d)\n",errno);
		return(ret);
	}
	return 0;
}

int func_spi_read(int  fd, unsigned int addr ,char ac_size ,unsigned int lsbfst)
{
	int					ret;
	unsigned char		tx_buf[16];
	unsigned char		rx_buf[16];
	int					val;
	int					len;
	
	memset( tx_buf, 0x00, sizeof(tx_buf) );
	memset( rx_buf, 0x00, sizeof(rx_buf) );

	if(ac_size == 1){
		tx_buf[0]  = fund_reverse_bit8((unsigned char)(addr & 0xFF),lsbfst);
		len = 2;
	}else{
		tx_buf[0]  = fund_reverse_bit8((unsigned char)((addr>>8) & 0x7F),lsbfst);
		tx_buf[1]  = fund_reverse_bit8((unsigned char)(addr & 0xFF),lsbfst);
		len = 3;
	}
	tx_buf[0] |= 0x80;		/* read bit is 0 */

	dmsg("spi read addr:%08x send %02x %02x \n",addr,tx_buf[0],tx_buf[1]);

	struct spi_ioc_transfer spi_data = 
	{
		.tx_buf = (unsigned long)tx_buf,
		.rx_buf = (unsigned long)rx_buf,
		.len = len,
		.delay_usecs = 0,
		.speed_hz = 12500000,
		.bits_per_word = 8,
	};

#if 1
	unsigned char		mode;
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1){
		printf("SPI_IOC_WR_MODE error(%d)\n",errno);
		return(ret);
	}
	mode &= ~(SPI_CPHA | SPI_CPOL);
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1){
		printf("SPI_IOC_WR_MODE error(%d)\n",errno);
		return(ret);
	}
#endif
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &(spi_data.bits_per_word));
	if (ret == -1){
		printf("SPI_IOC_WR_BITS_PER_WORD error(%d)\n",errno);
		return(ret);
	}
	
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &(spi_data.speed_hz ));
	if (ret == -1){
		printf("SPI_IOC_WR_MAX_SPEED_HZ error(%d)\n",errno);
		return(ret);
	}
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &spi_data );
	if (ret < 1){
		printf("SPI_IOC_MESSAGE error(%d)\n",errno);
		return(ret);
	}
	if(len == 2){
		val = (int)fund_reverse_bit8(rx_buf[1],lsbfst);
	}
	else{
		val = (int)fund_reverse_bit8(rx_buf[2],lsbfst);
	}
	dmsg("spi recv %02x %02x %02x %02x \n",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3]);
	return val;
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
		case 'i':
			mode = E_DEV_CTL_IRQ;
			break;
		case 'w':
			mode = E_DEV_CTL_WRITE;
			if(len >= 2)
			{
				switch(argv[1][1])
				{
					case 'h':
						mode = E_DEV_CTL_WRITE_H;
						break;
					case 'k':
						mode = E_DEV_CTL_WRITE_K;
						break;
					default:
						return(E_DEV_CTL_NONE);
					break;
				}
			}
			break;
		case 'r':
			mode = E_DEV_CTL_READ;
			if(len >= 2)
			{
				switch(argv[1][1])
				{
					case 'h':
						mode = E_DEV_CTL_READ_H;
						break;
					case 'w':
						mode = E_DEV_CTL_READ_WRITE;
						break;
					case 'm':
						mode = E_DEV_CTL_READ_MASK_WRITE;
						break;
					default:
						return(E_DEV_CTL_NONE);
					break;
				}
			}
			break;
		case 'e':
			if(len < 2)
			{
				return(E_DEV_CTL_NONE);
			}
			switch(argv[1][1])
			{
				case 'r':
					mode = E_DEV_CTL_READ_EEP;
					break;
				case 'w':
					mode = E_DEV_CTL_WRITE_EEP;
					break;
				case 'a':
					mode = E_DEV_CTL_READ_EEPALL;
					break;
				default:
					return(E_DEV_CTL_NONE);
				break;
			}
			break;
		case 'f':
			if(len < 2)
			{
				return(E_DEV_CTL_NONE);
			}
			switch(argv[1][1])
			{
				case 'r':
					mode = E_DEV_CTL_READ_QSPI;
					break;
				case 'w':
					mode = E_DEV_CTL_WRITE_QSPI;
					break;
				case 'e':
					mode = E_DEV_CTL_ERASE_QSPI;
					break;
				default:
					return(E_DEV_CTL_NONE);
				break;
			}
			break;
		case 'u':
			if(len < 2)
			{
				return(E_DEV_CTL_NONE);
			}
			switch(argv[1][1])
			{
				case 'r':
					mode = E_DEV_CTL_READ_UART1;
					break;
				case 'w':
					mode = E_DEV_CTL_WRITE_UART1;
					break;
				default:
					return(E_DEV_CTL_NONE);
				break;
			}
			break;
		case 's':
			if(len < 2)
			{
				return(E_DEV_CTL_NONE);
			}
			switch(argv[1][1])
			{
				case 'r':
					mode = E_DEV_CTL_READ_SPI;
					break;
				case 'w':
					mode = E_DEV_CTL_WRITE_SPI;
					break;
				default:
					return(E_DEV_CTL_NONE);
				break;
			}
			break;
		default:
			return(E_DEV_CTL_NONE);
			break;
	}
	return(mode);
}

void* func_dev_create( int argc, char *argv[] , t_dev_ctrl* ctrl_p )
{
	e_dev_ctl		 mode;
	unsigned int	 addr = 0;
	unsigned int	 size  = 0;
	unsigned int	 val = 0;
	unsigned int	 spino = 0;
	unsigned int	 lsbfst = 0;	
	char			 argv_conv[16];
	void*			 ptr = (void*)-1;
	int				 fd = -1;
	unsigned		 page_offset = -1;
	unsigned		 page_size = -1;
	FILE*			 fp;
	void*			 workbuf_p = NULL;
	unsigned int	 align = D_DEV_REG_AL8;
	struct termios	 oldtio;
	struct termios	 tty;
	unsigned int     limitmode;
	void*			 workbuf_p2 = NULL;
	
	/* mode */
	mode = func_dev_mode(argc,argv);
	switch(mode)
	{
		case E_DEV_CTL_NONE:
			ctrl_p->mode = E_DEV_CTL_NONE;
			return(NULL);
			break;
		case E_DEV_CTL_IRQ:
			if( argc < 2 )
			{
				return((void*)-1);
			}
			addr = func_argv_conv(argv[2],argv_conv,sizeof(argv_conv));
			break;
		case E_DEV_CTL_READ:
		case E_DEV_CTL_READ_H:
		case E_DEV_CTL_READ_EEP:
			if( argc < 3)
			{
				return((void*)-1);
			}
			addr = func_argv_conv(argv[2],argv_conv,sizeof(argv_conv));
			if( argc > 3)
			{
				val = func_argv_conv(argv[3],argv_conv,sizeof(argv_conv));
			}
			if(val < 1)
			{
				val = 1;
			}
			switch(mode)
			{
				case E_DEV_CTL_READ_EEP:
					align = D_DEV_REG_AL8;
					break;
				case E_DEV_CTL_READ_H:
					align = D_DEV_REG_AL16;
					break;
				default:
					align = D_DEV_REG_AL32;
					break;
			}
			break;
		case E_DEV_CTL_WRITE:
		case E_DEV_CTL_READ_WRITE:
		case E_DEV_CTL_READ_MASK_WRITE:
		case E_DEV_CTL_WRITE_H:
		case E_DEV_CTL_WRITE_K:
		case E_DEV_CTL_WRITE_EEP:
			if( argc < 4)
			{
				return((void*)-1);
			}
			addr = func_argv_conv(argv[2],argv_conv,sizeof(argv_conv));
			val = func_argv_conv(argv[3],argv_conv,sizeof(argv_conv));
			switch(mode)
			{
				case E_DEV_CTL_WRITE_EEP:
					size = 1;
					align = D_DEV_REG_AL8;
					break;
				case E_DEV_CTL_WRITE_H:
					size = 2;
					align = D_DEV_REG_AL16;
					break;
				case E_DEV_CTL_WRITE_K:
					size = 4;
					align = D_DEV_REG_AL32;
					memset(argv_conv,0,sizeof(argv_conv));
					if(argc < 5){
						return((void*)-1);
					}
					else{
						strncpy(argv_conv,argv[4],sizeof(argv_conv));
					}
					break;
				default:
					size = 4;
					align = D_DEV_REG_AL32;
					break;
			}
			break;
		case E_DEV_CTL_READ_EEPALL:
			addr = 0;
			size = val = D_DEV_EEPROM_MAX;
			break;
		case E_DEV_CTL_WRITE_QSPI:
			if( argc < 4)
			{
				return((void*)-1);
			}
			addr = func_argv_conv(argv[2],argv_conv,sizeof(argv_conv));
			if(addr >= D_DEV_QSPI_MAX)
			{
				return((void*)-1);
			}	
			fp = fopen(argv[3],"rb");
			if(fp == NULL)
			{
				return((void*)-1);
			}
			fseek(fp, 0, SEEK_END);
			size = ftell(fp);
			if((int)size == -1 )
			{
				fclose(fp);
				return((void*)-1);
			}
			limitmode = 0;
			if( argc > 4){
				val = func_argv_conv(argv[4],argv_conv,sizeof(argv_conv));
				if((size + 4 + ( 4 - (size % 4))) > val ){
					size = val - (4 + ( 4 - (size % 4))) ; /* limit */
					val = size;
				}
				limitmode = 1;
			}
			val = size;
			if((addr + size) > 0x01FFFFFF)
			{
				fclose(fp);
				return((void*)-1);
			}
			fseek(fp, 0, SEEK_SET);
			workbuf_p = malloc(size + 4 + ( 4 - (size % 4)));
			if(workbuf_p == NULL)
			{
				fclose(fp);
				return((void*)-1);
			}
			memset(workbuf_p,0,(size + 4 + ( 4 - (size % 4))));
			workbuf_p2 = workbuf_p;
			if(limitmode > 0){
				memcpy(workbuf_p,&size,4);
				workbuf_p2 = workbuf_p + 4;
			}
			if( fread(workbuf_p2, 1, size , fp) != size )
			{
				free(workbuf_p);
				fclose(fp);
				return((void*)-1);
			}
			if(limitmode > 0){
				size = size + 4 + ( 4 - (size % 4));
				val = size;
			}
			fflush(fp);
			fclose(fp);
			break;
		case E_DEV_CTL_ERASE_QSPI:
		case E_DEV_CTL_READ_QSPI:
			if( argc < 4)
			{
				return((void*)-1);
			}
			addr = func_argv_conv(argv[2],argv_conv,sizeof(argv_conv));
			if(addr >= D_DEV_QSPI_MAX)
			{
				return((void*)-1);
			}
			size = val = func_argv_conv(argv[3],argv_conv,sizeof(argv_conv));
			if(size == 0)
			{
				return((void*)-1);
			}
			if(mode == E_DEV_CTL_ERASE_QSPI)
			{
				if(( size % 0x10000 ) > 0)
				{
					size += 0x10000 - ( size % 0x10000);
				}
			}
			if((addr + size) > 0x01FFFFFF)
			{
				return((void*)-1);
			}
			workbuf_p = malloc(size);
			break;
		case E_DEV_CTL_READ_UART1:
			if( argc < 3)
			{
				return((void*)-1);
			}
			size = func_argv_conv(argv[2],argv_conv,sizeof(argv_conv));
			if(( workbuf_p = malloc(size)) == NULL){
				return((void*)-1);
			}
			memset(workbuf_p,0,size);
			break;
		case E_DEV_CTL_WRITE_UART1:
			if( argc < 3)
			{
				return((void*)-1);
			}
			size = strlen(argv[2]);
			if(( workbuf_p = malloc(size)) == NULL){
				return((void*)-1);
			}
			memset(workbuf_p,0,size);
			strncpy(workbuf_p,argv[2],size);
			break;
		case E_DEV_CTL_READ_SPI:
		case E_DEV_CTL_WRITE_SPI:
			if( argc < 5)
			{
				return((void*)-1);
			}
			addr = func_argv_conv(argv[2],argv_conv,sizeof(argv_conv));
			val = func_argv_conv(argv[3],argv_conv,sizeof(argv_conv));
			break;
		default:
			return((void*)-1);
			break;
	}

	if(
	   (mode == E_DEV_CTL_READ)    || 
	   (mode == E_DEV_CTL_WRITE)   || 
	   (mode == E_DEV_CTL_READ_WRITE)   || 
	   (mode == E_DEV_CTL_READ_MASK_WRITE)   || 
	   (mode == E_DEV_CTL_READ_H)  || 
	   (mode == E_DEV_CTL_WRITE_K)  || 
	   (mode == E_DEV_CTL_WRITE_H)
	  )
	{
		if(( addr % align ) > 0)
		{
			addr -= ( addr % align );
		}
		if((mode == E_DEV_CTL_READ) || (mode == E_DEV_CTL_READ_H))
		{
			if( val < align )
			{
				val = align;
			}
			if(( val % align ) > 0)
			{
				val += align - (val % align);
			}
			size = val;
		}
	}	
	char name[256];
	switch(mode)
	{
		case E_DEV_CTL_IRQ:
			memset(name,0,sizeof(name));
			snprintf(name,sizeof(name),"%s%d",D_DEV_UIO_NAME,addr);
			if( ( fd = open(name,O_RDWR) ) == -1)
			{
				if(workbuf_p != NULL)
				{
					free(workbuf_p);
				}
				return((void*)-1);
			}
			ptr = NULL;
			break;
		case E_DEV_CTL_WRITE_K:
			snprintf(name,sizeof(name),"%s%s",D_DEV_KREG_NAME,argv_conv);
			if( ( fd = open(name,O_RDWR) ) == -1)
			{
				if(workbuf_p != NULL){
					free(workbuf_p);
				}
				return((void*)-1);
			}
			ptr = NULL;
			break;
		case E_DEV_CTL_WRITE:
		case E_DEV_CTL_READ:
		case E_DEV_CTL_WRITE_H:
		case E_DEV_CTL_READ_H:
		case E_DEV_CTL_READ_WRITE:
		case E_DEV_CTL_READ_MASK_WRITE:
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
		case E_DEV_CTL_WRITE_EEP:
		case E_DEV_CTL_READ_EEP:
		case E_DEV_CTL_READ_EEPALL:
			if (( fd = open(D_DEV_EEP_NAME,O_RDWR) ) == -1 )
			{
				if(workbuf_p != NULL)
				{
					free(workbuf_p);
				}
				return((void*)-1);
			}
			ptr = NULL;
			break;
		case E_DEV_CTL_WRITE_QSPI:
		case E_DEV_CTL_READ_QSPI:
		case E_DEV_CTL_ERASE_QSPI:
			if (( fd = open(D_DEV_QSPI_NAME,O_RDWR) ) == -1 )
			{
				if(workbuf_p != NULL)
				{
					free(workbuf_p);
				}
				return((void*)-1);
			}
			ptr = NULL;
			break;
		case E_DEV_CTL_READ_UART1:
		case E_DEV_CTL_WRITE_UART1:
			if (( fd = open(D_DEV_UART1_NAME,O_RDWR) ) == -1 )
			{
				if(workbuf_p != NULL)
				{
					free(workbuf_p);
				}
				return((void*)-1);
			}
			memset( &oldtio, 0, sizeof(oldtio) );
			memset( &tty, 0, sizeof(tty) );
			ioctl( fd, TCGETS, oldtio);
			tty = oldtio;
			tty.c_cflag		= CS8 | CLOCAL | CREAD;
			tty.c_iflag		= IXON | IXOFF;
			tty.c_cc[VMIN]	= 1;
			tty.c_cc[VTIME]	= 0;
			cfsetospeed( &tty, B9600 );
			cfsetispeed( &tty, B9600 );
			tcflush( fd, TCIFLUSH );
			tcsetattr(fd, TCSANOW, &tty );
			ptr = NULL;
			break;
		case E_DEV_CTL_READ_SPI:
		case E_DEV_CTL_WRITE_SPI:
			memset(name,0,sizeof(name));
			spino = func_argv_conv(argv[4],argv_conv,sizeof(argv_conv));
			switch(spino){
				case 0:
					snprintf(name,sizeof(name),"%s",D_DEV_SPI_PTP_NAME);
					size = 1;
					lsbfst = 1;
					break;
				case 1:
					snprintf(name,sizeof(name),"%s",D_DEV_SPI_RFIC_NAME);
					size = 2;
					break;
				case 2:
					snprintf(name,sizeof(name),"%s",D_DEV_SPI_PLL_NAME);
					size = 2;
					break;
				case 3:
					snprintf(name,sizeof(name),"%s",D_DEV_SPI_RFICPLL_NAME);
					size = 2;
					break;
				default:
					break;
			}
			if( ( fd = open(name,O_RDWR) ) == -1)
			{
				if(workbuf_p != NULL)
				{
					free(workbuf_p);
				}
				return((void*)-1);
			}
			ptr = NULL;
			break;
		default:
			if(workbuf_p != NULL)
			{
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
	ctrl_p->lsbfst		 = lsbfst;
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
	void *read_p = NULL;
	unsigned baseaddr = 0;
	unsigned int readlen = 0;
	unsigned int count = 0;
	unsigned int count2 = 0;
	unsigned val;
	unsigned int rtn=0;
	int ret = 0;
	
	if(argc < 2 )
	{
		printf("input param error\n");
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
		case E_DEV_CTL_WRITE_SPI:
			if((rtn = func_spi_write(ctrl.fd,ctrl.addr,ctrl.val,ctrl.size ,ctrl.lsbfst) ) != 0){
				printf("SPI write(%d) ERROR(0x%08x <- 0x%08x)\n",ctrl.size,ctrl.addr,ctrl.val);
				break;
			}
			printf("write OK(lsb:%d addr:%x val:%x)\n",ctrl.lsbfst,ctrl.addr,ctrl.val);
			break;
		case E_DEV_CTL_READ_SPI:
			if(ctrl.val > 1)
			{
				printf("ADR   +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F \n");
				printf("----  ------------------------------------------------\n");
				baseaddr = ctrl.addr & 0x00000FF0;
				printf("%04X   ", baseaddr);
				for(count=0 ; count < (ctrl.addr - baseaddr)  ; count++)
				{
					printf(".. ");
				}
				baseaddr = ctrl.addr;
				readlen = ctrl.val;
				readlen += count;
			}
			else
			{
				printf("ADR   +0 \n");
				printf("----  ---\n");
				baseaddr = ctrl.addr;
				printf("%04X   ", baseaddr);
				baseaddr = ctrl.addr;
				readlen = ctrl.val;
				count = 0;
			}
			for( count2=count ; ( count2 < readlen ) && ( baseaddr < 0x7F ) ; count2++ ,baseaddr++)
			{
				rtn = func_spi_read(ctrl.fd,baseaddr,ctrl.size ,ctrl.lsbfst);
				printf("%02X ",(unsigned char)rtn);
				if((((count2+1) % 16) == 0) && ((count2 + 1) < readlen ))
				{
					printf("\n%04X   ", baseaddr + 1);
				}
			}
			printf("\n");
			break;
		case E_DEV_CTL_IRQ:
			val = 1;
			if( write(ctrl.fd,&val,sizeof(val)) > 0)
			{
				ctrl.val = 0;
				printf("Wait for UIO%d ...",ctrl.addr);
				if( read(ctrl.fd,&ctrl.val,sizeof(ctrl.val)) > 0 )
				{
					printf("Raise count:%d  \n", ctrl.val);
				}
				else
				{
					printf("Error(0x%08x) \n", ctrl.val);
				}
			}
			else
			{
				printf("Write Error UIO%d ...",ctrl.addr);
			}
			break;
		case E_DEV_CTL_WRITE_K:
			if((rtn = func_io_write(ctrl.fd,ctrl.addr,ctrl.val) ) != 0){
				printf("write ERROR(0x%08x <- 0x%08x)\n",ctrl.addr,ctrl.val);
				break;
			}
			printf("write OK\n");
			break;
		case E_DEV_CTL_READ:
			if(ctrl.val > 4)
			{
				printf("ADDRESS    +0       +4       +8       +C        \n");
				printf("--------   -------------------------------------\n");
				baseaddr = ctrl.addr & 0xFFFFFFF0;
				printf("%08X   ", baseaddr);
				for(count=0 ; count < ((ctrl.addr - baseaddr)/4)  ; count++)
				{
					printf("........ ");
				}
				baseaddr = ctrl.addr;
				readlen = (ctrl.val)/4;
				readlen += count;
			}
			else
			{
				printf("ADDRESS    +0       \n");
				printf("--------   ---------\n");
				baseaddr = ctrl.addr;
				printf("%08X   ", baseaddr);
				readlen = (ctrl.val)/4;
				count = 0;
			}
			read_p = (ptr + ctrl.page_offset);
			for(count2 = count ; count2 < readlen  ; count2++ ,baseaddr+=4)
			{
				rtn = (unsigned int)(*(unsigned *)read_p);
				printf("%08X ", rtn);
				if((((count2+1) % 4) == 0) && ((count2 + 1) < readlen ))
				{
					printf("\n%08X   ", baseaddr + 4);
				}
				read_p+=4;
			}
			printf("\n");
			break;
		case E_DEV_CTL_READ_H:
			if(ctrl.val > 2)
			{
				printf("ADDRESS    +0   +2   +4   +6   +8   +A   +C   +E    \n");
				printf("--------   -----------------------------------------\n");
/*				printf("ADDRESS    0000 0000 0000 0000 0000 0000 0000 0000  \n");	*/

				baseaddr = ctrl.addr & 0xFFFFFFF0;
				printf("%08X   ", baseaddr);
				for(count=0 ; count < ((ctrl.addr - baseaddr)/2)  ; count++)
				{
					printf(".... ");
				}
				baseaddr = ctrl.addr;
				readlen = (ctrl.val)/2;
				readlen += count;
			}
			else
			{
				printf("ADDRESS    +0       \n");
				printf("--------   ---------\n");
				baseaddr = ctrl.addr;
				printf("%08X   ", baseaddr);
				readlen = (ctrl.val)/2;
				count = 0;
			}
			read_p = (ptr + ctrl.page_offset);
			for(count2 = count ; count2 < readlen  ; count2++ ,baseaddr+=2 )
			{
				rtn = (unsigned short)(*(unsigned short*)read_p);
				printf("%04X ", rtn);
				if((((count2+1) % 8) == 0) && ((count2 + 1) < readlen ))
				{
					printf("\n%08X   ", baseaddr + 2);
				}
				read_p+=2;
			}
			printf("\n");
			break;
		case E_DEV_CTL_WRITE:
			*((unsigned *)(ptr + ctrl.page_offset)) = ctrl.val;
			msync( ptr , ctrl.page_size , MS_SYNC );
			printf("write OK\n");
			break;
		case E_DEV_CTL_READ_WRITE:
			val = *((unsigned *)(ptr + ctrl.page_offset));
			*((unsigned *)(ptr + ctrl.page_offset)) = ( ctrl.val | val );
			msync( ptr , ctrl.page_size , MS_SYNC );
			printf("write OK(%x)\n",val);
			break;
		case E_DEV_CTL_READ_MASK_WRITE:
			val = *((unsigned *)(ptr + ctrl.page_offset));
			*((unsigned *)(ptr + ctrl.page_offset)) = ( val & (~ctrl.val));
			msync( ptr , ctrl.page_size , MS_SYNC );
			printf("write OK(%x)\n",val);
			break;
		case E_DEV_CTL_WRITE_H:
			*((unsigned short*)(ptr + ctrl.page_offset)) = (unsigned short)(ctrl.val);
			msync( ptr , ctrl.page_size , MS_SYNC );
			printf("Half write OK\n");
			break;
		case E_DEV_CTL_WRITE_EEP:
			printf("PROTECT_UNLOCK(FF0A0044 & ~0x1000) %x\n",system(D_DEV_EEP_PROTECT_UNLOCK));
			if(( rtn = func_eep_write(ctrl.fd, ctrl.addr , ctrl.val)) == 0){
				printf("write OK\n");
			}
			printf("PROTECT_LOCK(FF0A0044 | 0x1000) %x\n",system(D_DEV_EEP_PROTECT_LOCK));
			break;
		case E_DEV_CTL_READ_QSPI:
			if ( func_qspi_read(ctrl.fd , ctrl.addr , ctrl.worksize ,ctrl.workbuf_p) <= 0 )
			{
				printf("Read ERROR \n");
				break;
			}
			if(ctrl.worksize > 4)
			{
				printf("ADDRESS    +0       +4       +8       +C        \n");
				printf("--------   -------------------------------------\n");
				baseaddr = ctrl.addr & 0xFFFFFFF0;
				printf("%08X   ", baseaddr);
				for(count=0 ; count < ((ctrl.addr - baseaddr)/4)  ; count++)
				{
					printf("........ ");
				}
				baseaddr = ctrl.addr;
				readlen = (ctrl.worksize)/4;
				readlen += count;
			}
			else
			{
				printf("ADDRESS    +0       \n");
				printf("--------   ---------\n");
				baseaddr = ctrl.addr;
				printf("%08X   ", baseaddr);
				readlen = (ctrl.worksize)/4;
				count = 0;
			}
			read_p = (ctrl.workbuf_p);
			for(count2 = count ; count2 < readlen  ; count2++ ,baseaddr+=4)
			{
				printf("%08X ", htonl((unsigned int)(*(unsigned *)read_p)));
				if((((count2+1) % 4) == 0) && ((count2 + 1) < readlen ))
				{
					printf("\n%08X   ", baseaddr + 4);
				}
				read_p+=4;
			}
			printf("\n");
			break;
		case E_DEV_CTL_ERASE_QSPI:
			if (( ret = func_qspi_erase(ctrl.fd , ctrl.addr , ctrl.worksize )) < 0 )
			{
				printf("ERASE ERROR(%d,%08X,%08X) \n",ret,ctrl.addr,ctrl.worksize);
				break;
			}
			break;
		case E_DEV_CTL_WRITE_QSPI:
			if ( func_qspi_write(ctrl.fd , ctrl.addr , ctrl.worksize ,ctrl.workbuf_p) <= 0 )
			{
				printf("Write ERROR \n");
				break;
			}
			sync();
			break;			
		case E_DEV_CTL_READ_EEP:
		case E_DEV_CTL_READ_EEPALL:
			if(ctrl.val > 1)
			{
				printf("ADR   +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F \n");
				printf("----  ------------------------------------------------\n");
				baseaddr = ctrl.addr & 0x00000FF0;
				printf("%04X   ", baseaddr);
				for(count=0 ; count < (ctrl.addr - baseaddr)  ; count++)
				{
					printf(".. ");
				}
				baseaddr = ctrl.addr;
				readlen = ctrl.val;
				readlen += count;
			}
			else
			{
				printf("ADR   +0 \n");
				printf("----  ---\n");
				baseaddr = ctrl.addr;
				printf("%04X   ", baseaddr);
				baseaddr = ctrl.addr;
				readlen = ctrl.val;
				count = 0;
			}
			for( count2=count ; ( count2 < readlen ) && ( baseaddr < D_DEV_EEPROM_MAX ) ; count2++ ,baseaddr++)
			{
				rtn = func_eep_read(ctrl.fd,baseaddr);
				printf("%02X ",(unsigned char)rtn);
				if((((count2+1) % 16) == 0) && ((count2 + 1) < readlen ))
				{
					printf("\n%04X   ", baseaddr + 1);
				}
			}
			printf("\n");
			break;
		case E_DEV_CTL_READ_UART1:
			if(( ret = read( ctrl.fd , ctrl.workbuf_p, ctrl.worksize )) < 0 )
			{
				printf("READ ERROR \n");
				break;
			}
			printf("len:%dbytes\n", ret);
			for(count=0;count < ret;count++)
			{
				printf("%c", (unsigned char)*((unsigned char*)(ctrl.workbuf_p) + count));
			}
			printf("\n");
			break;
		case E_DEV_CTL_WRITE_UART1:
			if(( ret = write( ctrl.fd , ctrl.workbuf_p, ctrl.worksize )) < 0 )
			{
				printf("WRITE ERROR \n");
				break;
			}
			printf("write OK\n");
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

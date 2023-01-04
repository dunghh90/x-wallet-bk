/*
 * simI2C
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "reg_def.h"


/**
 * 
 */
void getI2Cfnam(unsigned int dv, char *i2cfpath ) {
	char	*envnam = NULL ;
	envnam = getenv("DEB_I2C");
	if( envnam == NULL ) {
		envnam = "./i2c/i2cfile";
	}
	sprintf( i2cfpath, "%s%d", envnam, dv ) ;
}

int BPF_HM_DEVC_SFP_READ(unsigned int rev_dev,unsigned int rev_ad,unsigned short *rev_dt) 
{
	int fd ;
	char i2cfpath[PATH_MAX]	;

	getI2Cfnam( rev_dev, i2cfpath ) ;
	if(( fd = open( i2cfpath, O_RDONLY )) < 0 ) { 
		perror("open file" ) ; 
		return -1;
	}

	/* seek */
	if( lseek( fd, rev_ad, SEEK_SET) <0 ) {
		perror("lseek2" ) ;
		return(-1);
	}	

	if( read( fd, (char *)rev_dt, 1 ) < 0 ) {
		perror("read") ;
		return -1 ;
	}

	if( close(fd) < 0 ) {
		perror("close2") ;
		return -1 ;
	}
	return 0;
}

int BPF_HM_DEVC_SFP_READ_BUFFER( unsigned int kind, unsigned short offset,  unsigned int count, unsigned char *data_p )
{
	int count2;
	unsigned short data = 0;

	for(count2=0 ; count2 < count ; count2++ ,data_p++)
	{
		data = 0;
		BPF_HM_DEVC_SFP_READ(kind,offset+count2,&data);
		*data_p = (unsigned char)data;
	}
	return 0;
}

int BPF_RU_IPCM_PROCMSG_ADDRFREE(void *buf)
{
	return 0;
}

int BPF_HM_DEVC_EEPROM_PL_WRITE(unsigned short offset, unsigned char *data_p)
{
    int fd ;
    unsigned int start;
    char regfpath[PATH_MAX] ;
    char    *envnam = NULL ;
    envnam = getenv("DEB_E2P");
    if( envnam == NULL ) {
        envnam = "e2p/e2pfile";
    }
    if(offset >= 2048)
    {
        perror("offset error" ) ;
        return -1;
	}

    if(( fd = open( envnam, O_WRONLY )) < 0 ) {
        perror("open file" ) ;
        return -1;
    }

    /* seek */
    if( lseek(fd, (offset+2048), SEEK_SET) <0 ) {
        perror("lseek1" ) ;
        return(-1);
    }  

    if( write(fd, (char *)data_p, 2 ) < 0 ) {
        perror("write1") ;
        return -1 ;
    }

    if( close(fd) < 0 ) {
        perror("close1") ;
        return -1 ;
    }
    return (0);

}

int BPF_HM_DEVC_EEPROM_PL_READ(unsigned short offset, unsigned char *data_p)
{
    int fd ;
    unsigned int start;
    char regfpath[PATH_MAX] ;
    char    *envnam = NULL ;
    envnam = getenv("DEB_E2P");
    if( envnam == NULL ) {
        envnam = "e2p/e2pfile";
	}

    if(offset >= 2048)
    {
        perror("offset error" ) ;
        return -1;
	}
    if(( fd = open( envnam, O_RDONLY )) < 0 ) {
        perror("open file" ) ;
        return -1;
    }

    /* seek */
    if( lseek( fd, (offset+2048), SEEK_SET) <0 ) {
        perror("lseek2" ) ;
        return(-1);
    }

    if( read( fd, (char *)data_p, 4 ) < 0 ) {
        perror("read") ;
        return -1 ;
    }

    if( close(fd) < 0 ) {
        perror("close2") ;
        return -1 ;
    }
    return 0;
}



#if 0
main()
{
	unsigned short a = 0xa5a5;
	unsigned short b ;
	printf("write : %d\n",        sim_i2c_write(0, 10, &a )) ;
	printf("read  : result %d\n", sim_i2c_read (0, 10, &b )) ;
	printf("        data : %x\n", b ) ;

}

#endif

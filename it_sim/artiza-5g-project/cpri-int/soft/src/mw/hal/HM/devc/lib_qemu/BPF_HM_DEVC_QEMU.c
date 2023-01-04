/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_EEPROM_READ.c
 *  @brief
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#define _GNU_SOURCE
#include <sched.h>
#include <pthread.h>
#include "bpf_i_hm_devc.h"
#include "BPF_COM_LOG.h"
#include <sys/syscall.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>

static int qemu_sched_getparam( int* policy )
{
	struct sched_param	prams;
	(void)pthread_getschedparam(pthread_self(), policy,&prams);
	return prams.sched_priority;
}

static void qemu_sched_setparam( int priority , int policy )
{
	struct sched_param	prams;
	prams.sched_priority = priority;
	(void)pthread_setschedparam(pthread_self(),policy,(const struct sched_param*)&prams);
	return;
}


static char qemu_threadname[32];

static char* qemu_get_threadname(void)
{
	char filename[32];
	int fd;
	int len;
	int selftid = (int)syscall(SYS_gettid);

	memset(filename,0,sizeof(filename));
	snprintf((char*)filename, sizeof(filename),"/proc/%d/comm",selftid);
	if ((fd = open(filename, O_RDONLY)) > 0){
		len = read(fd,qemu_threadname,sizeof(qemu_threadname));
		if(len <= 0){
			strcpy(qemu_threadname,"Unknown");
		}
		close(fd);
	}
	else{
		strcpy(qemu_threadname,"Unknown");
	}
	return qemu_threadname;
}

static int		semIndex = 0;
static sem_t*	semInfo[128] = { 0 };
static char		semInfoName[128][256];


sem_t* AaSemSearch(const char*name ){
	UINT	count;
	UINT	max;
	sem_t*	mysem = NULL;
	int    iRetCode;
	int    iErrCode;
	const char*				wname;
	struct bpf_timespec		dtime;	/* sem waittime */
	dtime.tv_sec = 100;
	dtime.tv_nsec = 0;


	iRetCode = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_REG_ACCESS_LOG,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT,&dtime,&iErrCode);
	if(iRetCode != 0){
		printf("Sem take Error %s %s\n",name,qemu_get_threadname());
		return NULL;
	}
	max = semIndex;
	BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_REG_ACCESS_LOG, BPF_RU_IPCM_LOCK_RW, &iErrCode);

	wname = strrchr(name, '/');
	if(wname == NULL)
	{
		wname = name;
	}
	else
	{
		wname++;
	}

	if(!strncmp(wname,"dev_irq",strlen("dev_irq"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_sfp",strlen("dev_sfp"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_rfpll",strlen("dev_rfpll"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_txdac",strlen("dev_txdac"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_rxdac",strlen("dev_rxdac"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_pgc",strlen("dev_pgc"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_pgclut",strlen("dev_pgclut"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_qmod",strlen("dev_qmod"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_qmem",strlen("dev_qmem"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_pavgdac",strlen("dev_pavgdac"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_rxvattadc",strlen("dev_rxvattadc"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_txvattdac",strlen("dev_txvattdac"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_jcpll",strlen("dev_jcpll"))){
		return (sem_t*)-1;
	}

	if(!strncmp(wname,"dev_temp",strlen("dev_temp"))){
		return (sem_t*)-1;
	}

	for(count = 0; count < max ; count++){
		if(!strncmp(wname,(const char*)semInfoName[count],strlen(wname))){
			mysem = semInfo[count];
			break;
		}
	}
	return mysem;
}


sem_t* AaSemCreateC(const char*name )
{
	char sem_filename[256];
	const char*				wname;
	sem_t*	mysem = NULL;
	int    iRetCode;
	int    iErrCode;
	struct bpf_timespec		dtime;	/* sem waittime */
	dtime.tv_sec = 100;
	dtime.tv_nsec = 0;

	iRetCode = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_REG_ACCESS_LOG,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT,&dtime,&iErrCode);
	if(iRetCode != 0){
		printf("Sem take Error %s %s\n",name,qemu_get_threadname());
		return NULL;
	}
	if( semIndex >= 127){
		printf("Sem Index Over Error %s %s\n",name,qemu_get_threadname());
		BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_REG_ACCESS_LOG, BPF_RU_IPCM_LOCK_RW, &iErrCode);
		return NULL;
	}
	wname = strrchr(name, '/');
	if(wname == NULL)
	{
		wname = name;
	}
	else
	{
		wname++;
	}

	memset(sem_filename,0,sizeof(sem_filename));
	snprintf(sem_filename,sizeof(sem_filename),"/%s",wname);

	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"Sem Create(%s)",sem_filename);

	int fret;
	int fd = shm_open(sem_filename, O_RDWR, 0600);
	if( fd < 0 ){
		fd = shm_open(sem_filename, O_CREAT|O_EXCL|O_RDWR, 0600);
		if(fd < 0) { printf("shm open errn %s %s\n",sem_filename,qemu_get_threadname()); abort(); }
		if((fret =  ftruncate(fd, sizeof(sem_t))) == -1) { printf("ftruncate error %s %s\n",sem_filename,qemu_get_threadname()); abort(); }
		mysem = mmap(NULL, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		sem_init(mysem, 1, 1);
//		BPF_COM_PLOG_RSC_SEM(E_SEM_LV_INIT,(unsigned long)mysem,1);
	}else{
		mysem = mmap(NULL, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	}

	semInfo[semIndex] = mysem;
	memset(semInfoName[semIndex],0,sizeof(semInfoName[semIndex]));
	strncpy(semInfoName[semIndex],wname,strlen(wname));
	semIndex++;

	BPF_RU_IPCM_PROCSEM_GIVE( E_RRH_SEMID_APL_REG_ACCESS_LOG, BPF_RU_IPCM_LOCK_RW, &iErrCode);
	return mysem;
}

void AaSemPost(const char*name )
{
	sem_t*	semPtr = AaSemSearch(name);
	if(semPtr == (sem_t*)-1){
		return;
	}
	if(semPtr == NULL){
		printf("Sem post Error\n");
		return;
	}
	errno = 0;
	if( sem_post((sem_t*)semPtr) != 0){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"AaSem post error(%d)",errno);
	}
//	BPF_COM_PLOG_RSC_SEM(E_SEM_LV_POST,(unsigned long)semPtr,0);
	return;
}

void AaSemWait(const char*name )
{
	sem_t*	semPtr = AaSemSearch(name);
	if(semPtr == (sem_t*)-1){
		return;
	}
	if(semPtr == NULL){
		if((semPtr =  AaSemCreateC(name)) == NULL){
			printf("Create Sem take Error 0x%s %s\n",name,qemu_get_threadname());
			return;
		}
	}

//	BPF_COM_PLOG_RSC_SEM(E_SEM_LV_WAIT,(unsigned long)semPtr,0);
	errno = 0;
	if( sem_wait((sem_t*)semPtr) != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,"AaSem wait error(%d)",errno);
	}
	return;
}


/** @addtogroup BPF_HM_DEVC
 * @{ */

FILE* qemu_create_cfgfile(const char* name ,int size){
	FILE*	fp;
	int		cnt;
	char	buf[256];
	int		len;

	fp = fopen(name,"w+");
	if(fp == NULL){
		return NULL;
	}
	memset(buf,0,sizeof(buf));
	for(cnt=0;cnt <  256 ;cnt++){
		switch(size){
			case 2:
				len = snprintf(buf,sizeof(buf),"%08X   0000 0000 0000 0000 0000 0000 0000 0000\n",cnt*0x10);
				break;
			case 8:
				len = snprintf(buf,sizeof(buf),"%08X   0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000\n",cnt*0x10);
				break;
			case 4:
				len = snprintf(buf,sizeof(buf),"%08X   00000000 00000000 00000000 00000000\n",cnt*0x10);
				break;
			case 1:
			default:
				len = snprintf(buf,sizeof(buf),"%08X   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n",cnt*0x10);
				break;
		}
		fwrite(buf,1,len,fp);
	}
	fseek(fp, 0, SEEK_SET);
	return fp;
}


int qemu_read_data(const char* name , unsigned int addr , int size)
{
	FILE* fp;
	int ofs;
	int  value[16];
	int ret;
	char	linebuff[256];
	int	line;
	int	linecount;
	unsigned int	pos;
	int	fp_pos;

	/* format : 00000000   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  */
	/* format : 00000000   0000 0000 0000 0000 0000 0000 0000 0000  */
	/* format : 00000000   00000000 00000000 00000000 00000000  */

	int policy;
	int priority = qemu_sched_getparam(&policy);
	qemu_sched_setparam(0,SCHED_OTHER);
	/* 2021/02/23 M&C) Update for intermediate device - add start */
	char *qemuPath;
	qemuPath = getenv("PATH_QEMU");
	if (qemuPath == NULL)
    {
		//	AaSemWait(name);

		if( ( fp = fopen(name,"r")) == NULL ){
			printf("%s open Error , cfg file create start ...\n",name);
			if((fp = qemu_create_cfgfile(name ,size)) == NULL){
				qemu_sched_setparam(priority,policy);
				return 0;
			}
		}
    }
    else
    {
		char temp[256];
     	if(strstr(name, "/qemu/")!= NULL)
	 	{
			memset(temp,0,sizeof(temp));
			strcat(temp,qemuPath);
			strcat(temp,name);
		}
		//	AaSemWait(name);

		if( ( fp = fopen(temp,"r")) == NULL ){
			if((fp = qemu_create_cfgfile(temp ,size)) == NULL){
				qemu_sched_setparam(priority,policy);
				return 0;
			}
		}
    }
	/* 2021/02/23 M&C) Update for intermediate device - add end */
	fseek(fp, 0, SEEK_SET);
	line = addr / 16;
	switch(size){
		case 2:
			pos = ( addr % 16 ) / 2 ;
		break;
		case 4:
			pos = ( addr % 16 ) / 4 ;
		break;
		case 1:
		case 8:
		default:
			pos = ( addr % 16 );
		break;
	}
	linecount = 0;
	memset(value,0,sizeof(value));
	fp_pos = 0;
	while ( fgets(linebuff,sizeof(linebuff), fp) != NULL ) {
		if(line != linecount){
			fp_pos = ftell(fp);
			fp_pos *= line;
			fseek(fp, fp_pos, SEEK_SET);
			linecount = line;
			continue;
		}
		switch(size){
			case 2:
				if((ret = sscanf( linebuff,
						"%x   %x %x %x %x %x %x %x %x",
						&ofs,&value[0],&value[1],&value[2],&value[3],&value[4],&value[5],&value[6],&value[7]))
						 != EOF)
				{
					value[pos] &= 0xFFFF;
				}
				break;
			case 4:
				if((ret = sscanf( linebuff,
						"%x   %x %x %x %x",
						&ofs,&value[0],&value[1],&value[2],&value[3]))
						 != EOF)
				{
					;
				}
				break;
			case 8:
				if((ret = sscanf( linebuff,
						"%x   %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
						&ofs,&value[0],&value[1],&value[2],&value[3],&value[4],&value[5],&value[6],&value[7],
						&value[8],&value[9],&value[10],&value[11],&value[12],&value[13],&value[14],&value[15]))
						 != EOF)
				{
					value[pos] &= 0xFFFF;
				}
				break;
			case 1:
			default:
				if((ret = sscanf( linebuff,
						"%x   %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
						&ofs,&value[0],&value[1],&value[2],&value[3],&value[4],&value[5],&value[6],&value[7],
						&value[8],&value[9],&value[10],&value[11],&value[12],&value[13],&value[14],&value[15]))
						 != EOF)
				{
					value[pos] &= 0xFF;
				}
				break;
		}
		break;
	}
	fclose(fp);

//	AaSemPost(name);

	qemu_sched_setparam(priority,policy);
	return value[pos];
}

void qemu_write_data(const char* name , unsigned int addr , int data , int size)
{
	FILE* fp;
	int ofs;
	int  value[16];
	int ret;
	char	linebuff[256];
	int	line;
	int	linecount;
	unsigned int	pos;
	int	fp_pos;

	/* format : 00000000   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  */
	/* format : 00000000   0000 0000 0000 0000 0000 0000 0000 0000  */
	/* format : 00000000   00000000 00000000 00000000 00000000  */

	int policy;
	int priority = qemu_sched_getparam(&policy);
	qemu_sched_setparam(0,SCHED_OTHER);
	/* 2021/02/23 M&C) Update for intermediate device - add start */
	char *qemuPath;
	qemuPath = getenv("PATH_QEMU");
	if (qemuPath == NULL)
    {
		//	AaSemWait(name);

		if( ( fp = fopen(name,"r+")) == NULL ){
			printf("%s open Error , cfg file create start ...\n",name);
			if((fp = qemu_create_cfgfile(name ,size)) == NULL){
				qemu_sched_setparam(priority,policy);
				return;
			}
		}
    }
    else
    {
		char temp[256];
     	if(strstr(name, "/qemu/")!= NULL)
	 	{
			memset(temp,0,sizeof(temp));
			strcat(temp,qemuPath);
			strcat(temp,name);
	 	}
		//	AaSemWait(name);

	if( ( fp = fopen(temp,"r+")) == NULL ){
		printf("%s open Error , cfg file create start ....\n",name);
		if((fp = qemu_create_cfgfile(temp ,size)) == NULL){
			printf("create Error\n");
			qemu_sched_setparam(priority,policy);
			return;
		}
		}
    }
	/* 2021/02/23 M&C) Update for intermediate device - add end */
	fseek(fp, 0, SEEK_SET);
	line = addr / 16;
	switch(size){
		case 2:
			pos = ( addr % 16 ) / 2 ;
		break;
		case 4:
			pos = ( addr % 16 ) / 4 ;
		break;
		case 1:
		case 8:
		default:
			pos = ( addr % 16 );
		break;
	}
	linecount = 0;
	memset(value,0,sizeof(value));
	fp_pos = 0;
	while ( fgets(linebuff,sizeof(linebuff), fp) != NULL ) {
		if(line != linecount){
			fp_pos = ftell(fp);
			fp_pos *= line;
			fseek(fp, fp_pos, SEEK_SET);
			linecount = line;
			continue;
		}
		switch(size){
			case 2:
				if((ret = sscanf( linebuff,
						"%x   %x %x %x %x %x %x %x %x",
						&ofs,&value[0],&value[1],&value[2],&value[3],&value[4],&value[5],&value[6],&value[7]))
						 != EOF)
				{
					value[pos] = data & 0xFFFF;
					snprintf(linebuff,sizeof(linebuff),"%08X   %04X %04X %04X %04X %04X %04X %04X %04X",
						ofs,value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]);
				}
				break;
			case 4:
				if((ret = sscanf( linebuff,
						"%x   %x %x %x %x",
						&ofs,&value[0],&value[1],&value[2],&value[3]))
						 != EOF)
				{
					value[pos] = data;
					snprintf(linebuff,sizeof(linebuff),"%08X   %08X %08X %08X %08X",
						ofs,value[0],value[1],value[2],value[3]);
				}
				break;
			case 8:
				if((ret = sscanf( linebuff,
						"%x   %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
						&ofs,&value[0],&value[1],&value[2],&value[3],&value[4],&value[5],&value[6],&value[7],
						&value[8],&value[9],&value[10],&value[11],&value[12],&value[13],&value[14],&value[15]))
						 != EOF)
				{
					value[pos] = data & 0xFFFF;
					snprintf(linebuff,sizeof(linebuff),"%08X   %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X",
						ofs,value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],
						value[8],value[9],value[10],value[11],value[12],value[13],value[14],value[15]);
				}
				break;
			case 1:
			default:
				if((ret = sscanf( linebuff,
						"%x   %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
						&ofs,&value[0],&value[1],&value[2],&value[3],&value[4],&value[5],&value[6],&value[7],
						&value[8],&value[9],&value[10],&value[11],&value[12],&value[13],&value[14],&value[15]))
						 != EOF)
				{
					value[pos] = data & 0xFF;
					snprintf(linebuff,sizeof(linebuff),"%08X   %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
						ofs,value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],
						value[8],value[9],value[10],value[11],value[12],value[13],value[14],value[15]);
				}
				break;
		}
		fseek(fp, fp_pos, SEEK_SET);
		fputs(linebuff, fp);
		break;
	}
	fclose(fp);
//	AaSemPost(name);
	qemu_sched_setparam(priority,policy);
	return;
}


int BPF_HM_DEVC_EEPROM_READ(unsigned short offset, unsigned char *data_p){
	*data_p = qemu_read_data("/qemu/reg/dev_eeprom.cfg" ,offset , 1);
	//BPF_COM_PLOG_I2C(E_I2C_LV_READ,offset,(unsigned int)(*data_p),0,0);	/* packet log */ //M&C add comment
	return 0;
}

int BPF_HM_DEVC_EEPROM_WRITE(unsigned short offset, unsigned char *data_p){
	qemu_write_data("/qemu/reg/dev_eeprom.cfg" ,offset , *data_p, 1);
	//BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,offset,(unsigned int)*data_p,0,0);//M&C add comment
	return 0;
}

int BPF_HM_DEVC_MMI_CPLD_READ(unsigned int offset, unsigned int *data_p){

	*data_p = qemu_read_data("/qemu/reg/dev_cpld.cfg" ,offset , 1);
	//BPF_COM_PLOG_I2C(E_I2C_LV_READ,offset,(unsigned int)(*data_p),0,1);	/* packet log *///M&C add comment
	return 0;

}
int BPF_HM_DEVC_MMI_CPLD_WRITE(unsigned int offset, unsigned int *data_p)
{
	qemu_write_data("/qemu/reg/dev_cpld.cfg" ,offset , *data_p, 1);
	//BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,offset,(unsigned int)*data_p,0,1);//M&C add comment
	return 0;
}

int BPF_HM_DEVC_TEMP_READ(signed int *temp_p){
	*temp_p = qemu_read_data("/qemu/reg/dev_temp.cfg" ,0 , 2);
	//BPF_COM_PLOG_I2C(E_I2C_LV_READ,0,(unsigned int)(*temp_p),0,1);	/* packet log *///M&C add comment
	return 0;
}

void BPF_HM_DEVC_TEMP_INIT( void ){
	BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,0,0x60,0,1);	/* packet log */
	return;
}

int BPF_HM_DEVC_TEMP2_READ(signed int *temp_p){
	*temp_p = qemu_read_data("/qemu/reg/dev_temp2.cfg" ,0 , 2);
	BPF_COM_PLOG_I2C(E_I2C_LV_READ,0,(unsigned int)(*temp_p),0,1);	/* packet log */
	return 0;
}


int BPF_HM_DEVC_SFP_READ( unsigned int kind, unsigned short offset, unsigned char *data_p){
	char* name = NULL;
	switch(kind){
		case BPF_HM_DEVC_SFP_A0:
			name = "/qemu/reg/dev_sfp0_a0.cfg";
			break;
		case BPF_HM_DEVC_SFP_A2:
			name = "/qemu/reg/dev_sfp0_a2.cfg";
			break;
		case BPF_HM_DEVC_SFP_SA0:
			name = "/qemu/reg/dev_sfp1_a0.cfg";
			break;
		case BPF_HM_DEVC_SFP_SA2:
			name = "/qemu/reg/dev_sfp1_a2.cfg";
			break;
		default:
			printf("SFP read(%d) ERROR!!!!!!!!!\n",kind);
			return -1;
			break;
	}
	*data_p = qemu_read_data(name,offset , 1);
	BPF_COM_PLOG_I2C(E_I2C_LV_READ,offset,(unsigned int)*data_p,1,kind);
	return 0;
}
int BPF_HM_DEVC_SFP_READ_BUFFER( unsigned int kind, unsigned short offset,  unsigned int count, unsigned char *data_p ){
	int cnt;
	for(cnt=0;cnt < count ;cnt++){
		BPF_HM_DEVC_SFP_READ(kind,offset+cnt,data_p+cnt);
	}
	return 0;
}

int BPF_HM_DEVC_SFP_WRITE( unsigned int kind, unsigned short offset, unsigned char *data_p){
	char* name = NULL;
	switch(kind){
		case BPF_HM_DEVC_SFP_A0:
			name = "/qemu/reg/dev_sfp0_a0.cfg";
			break;
		case BPF_HM_DEVC_SFP_A2:
			name = "/qemu/reg/dev_sfp0_a2.cfg";
			break;
		case BPF_HM_DEVC_SFP_SA0:
			name = "/qemu/reg/dev_sfp1_a0.cfg";
			break;
		case BPF_HM_DEVC_SFP_SA2:
			name = "/qemu/reg/dev_sfp1_a2.cfg";
			break;
		default:
			printf("SFP read(%d) ERROR!!!!!!!!!\n",kind);
			return -1;
			break;
	}
	qemu_write_data(name ,offset , *data_p, 1);
	BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,offset,(unsigned int)*data_p,1,kind);
	return 0;
}

int BPF_HM_DEVC_REG_READ(unsigned int loglevel, unsigned int offset, unsigned int *data_p){
	char name[256]; //C9 add code
	int	len;

	len = snprintf(name,sizeof(name),"/qemu/reg/dev_%08X.cfg",offset&0xfffff000);
	memset(&name[len],0,sizeof(name)-len);
	*data_p = qemu_read_data(name,offset&0xfff , 4);
	if(offset == 0xa0000150) {int C9 =1;*data_p =&C9;} //C9 add code
	BPF_COM_LOG_REG( loglevel, 0, offset, *data_p, 0 );
	return 0;
}



int BPF_HM_DEVC_REG_WRITE(unsigned int loglevel, unsigned int offset, unsigned int *data_p){
/* 2021/02/18 M&C)Tri.hn Update for intermediate device - add start */
	// char name[32];
	char name[256];
/* 2021/02/18 M&C)Tri.hn Update for intermediate device - add end */
	int	len;

	/*	JSFW内部SYSREFマスク	*/
	if(offset == 0xB1204020)
	{
		/*	JSFW内部SYSREFマスク解除設定(B[17:16]=[0,0])	*/
		if((*data_p & 0x00030000) == 0)
		{
			/*	0xB1204034 = 3	*/
			qemu_write_data( (const char *)"/qemu/reg/dev_B1204000.cfg", 0x34, 3, 4);
		}
	}
	len = snprintf(name,sizeof(name),"/qemu/reg/dev_%08X.cfg",offset&0xfffff000);
	memset(&name[len],0,sizeof(name)-len);
	qemu_write_data(name ,offset&0xfff , *data_p, 4);
	BPF_COM_LOG_REG( loglevel, 0, offset, *data_p, 0 );
	return 0;
}

int BPF_HM_DEVC_REG_READ_BUFFER(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p){
	int cnt;
	int maxcnt= size / 4;
	unsigned int* tmp_p;

	for(cnt=0 , tmp_p = data_p ;cnt < maxcnt;cnt++,tmp_p++){
		BPF_HM_DEVC_REG_READ(loglevel,offset+(cnt*4),tmp_p);
	}
	return 0;
}

int BPF_HM_DEVC_REG_READ_BUFFER_END(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p){
	int cnt;
	int maxcnt= size / 4;
	unsigned int* tmp_p;
	unsigned int tmp;
	for(cnt=0 , tmp_p = data_p ;cnt < maxcnt;cnt++,tmp_p++){
		BPF_HM_DEVC_REG_READ(loglevel,offset+(cnt*4),tmp_p);
		tmp = ntohl(*tmp_p);
		*tmp_p = tmp;
	}
	return 0;
}

int BPF_HM_DEVC_REG_READ_MULTI(unsigned int loglevel, unsigned int reg_cnt, unsigned int *reg_addr, unsigned int *data_p){
	int cnt;
	unsigned int* tmp_p;
	unsigned int addr;

	for(cnt=0 , tmp_p = data_p ;cnt < reg_cnt ;cnt++,tmp_p++){
		addr = *(reg_addr+cnt);
		BPF_HM_DEVC_REG_READ(loglevel,addr,tmp_p);
	}
	return 0;
}

int BPF_HM_DEVC_REG_READ_BIT(unsigned int loglevel, unsigned int offset, unsigned int check_bit, unsigned int *data_p){
	unsigned int data;

	BPF_HM_DEVC_REG_READ(loglevel,offset,&data);
	*data_p = check_bit & data;
	return 0;
}

int BPF_HM_DEVC_REG_READ_NOMUTEX(unsigned int loglevel, unsigned int offset, unsigned int *data_p){
	BPF_HM_DEVC_REG_READ(loglevel,offset,data_p);
	return 0;
}

int BPF_HM_DEVC_REG_WRITE_BUFFER(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p){
	int cnt;
	int maxcnt= size / 4;
	unsigned int* tmp_p;

	for(cnt=0 , tmp_p = data_p ;cnt < maxcnt;cnt++,tmp_p++){
		BPF_HM_DEVC_REG_WRITE(loglevel,offset+(cnt*4),tmp_p);
	}
	return 0;
}

int BPF_HM_DEVC_REG_WRITE_BUFFER_END(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p){
	int cnt;
	int maxcnt= size / 4;
	unsigned int* tmp_p;
	unsigned int tmp;
	for(cnt=0 , tmp_p = data_p ;cnt < maxcnt;cnt++,tmp_p++){
		tmp = htonl(*tmp_p);
		BPF_HM_DEVC_REG_WRITE(loglevel,offset+(cnt*4),&tmp);
	}
	return 0;
}

int BPF_HM_DEVC_REG_WRITE_MULTI(unsigned int loglevel, unsigned int reg_cnt, unsigned int *reg_addr, unsigned int *data_p){
	int cnt;
	unsigned int* tmp_p;
	unsigned int addr;

	for(cnt=0 , tmp_p = data_p ;cnt < reg_cnt ;cnt++,tmp_p++){
		addr = *(reg_addr+cnt);
		BPF_HM_DEVC_REG_WRITE(loglevel,addr,tmp_p);
	}
	return 0;
}

int BPF_HM_DEVC_REG_WRITE_BITON(unsigned int loglevel, unsigned int offset, unsigned int bitifno ){
	unsigned int data;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,offset,&data);
	data |= bitifno;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,offset,&data);
	return 0;
}

int BPF_HM_DEVC_REG_WRITE_BITOFF(unsigned int loglevel, unsigned int offset, unsigned int bitifno ){
	unsigned int data;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,offset,&data);
	data &= ~bitifno;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,offset,&data);
	return 0;
}

static UINT	qemu_rfic[2][BPF_HM_DEVC_RFIC_UNIT_RFIC_MAX];

int BPF_HM_DEVC_RFIC_READ( UINT txRx, UINT unit, UINT address, UINT *data_p){
	char* name = NULL;
	switch(unit){
		case BPF_HM_DEVC_RFIC_UNIT_RFIC1:
			name = "/qemu/reg/dev_rfic_1.cfg";
			break;
		case BPF_HM_DEVC_RFIC_UNIT_RFIC2:
			name = "/qemu/reg/dev_rfic_2.cfg";
			break;
		default:
			printf("BUG!!!!????  RFIC read access add:0x%x unit ERROR(0x%x(%d)) by QEMU!!!!!!!!!\n",address,unit,unit);
			return 0;
			break;
	}

	if(address == 0x3EE0){
		*data_p = qemu_rfic[0][unit];
	}
	else if(address == 0x3EE1){
		*data_p = 0xA5;
	}else{
		*data_p = qemu_read_data(name ,address , 8);
	}
	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI1_SS1 | ((1 << unit) & 0xF )), D_RRH_READ, address, *data_p );
	return 0;
}
int BPF_HM_DEVC_RFIC_WRITE( UINT txRx, UINT unit, UINT address, UINT data ){
	char* name = NULL;
	switch(unit){
		case BPF_HM_DEVC_RFIC_UNIT_RFIC1:
			name = "/qemu/reg/dev_rfic_1.cfg";
			break;
		case BPF_HM_DEVC_RFIC_UNIT_RFIC2:
			name = "/qemu/reg/dev_rfic_2.cfg";
			break;
		default:
			BPF_HM_DEVC_RFIC_WRITE( 0, BPF_HM_DEVC_RFIC_UNIT_RFIC1, address, data );
			BPF_HM_DEVC_RFIC_WRITE( 0, BPF_HM_DEVC_RFIC_UNIT_RFIC2, address, data );
			return 0;
			break;
	}
	if(address == 0x3EE0){
		qemu_rfic[0][unit] = data;
	}
	else if(address == 0x3EE1){
		qemu_rfic[1][unit] = data;
	}else{
		qemu_write_data(name ,address , data, 8);
	}
	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI1_SS1 | ((1 << unit) & 0xF )), D_RRH_WRITE, address, data );
	return 0;
}

int BPF_HM_DEVC_EXT_PLL_READ (UINT address, UINT *data_p){
	char* name = "/qemu/reg/dev_jcpll.cfg";

	if(address == 0xbe){
		*data_p = 0x03;
	}else{
		*data_p = qemu_read_data(name ,address , 2);
	}
	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI1_SS0 | BPF_HM_DEVC_CPLD_SET_PLL), D_RRH_READ, address, *data_p );
	return 0;
}

int BPF_HM_DEVC_EXT_PLL_WRITE (UINT address, UINT data){
	char* name = "/qemu/reg/dev_jcpll.cfg";

	qemu_write_data(name ,address , data, 2);
	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI1_SS0 | BPF_HM_DEVC_CPLD_SET_PLL), D_RRH_WRITE, address, data );
	return 0;
}

UINT BPF_HM_DEVC_PTP_PAGE=0;
UINT BPF_HM_DEVC_PTP_REG[0x100][0x1000];

int BPF_HM_DEVC_PTP_READ (UINT address, UINT *data_p){

	qemu_sched_setparam(0,SCHED_OTHER);
	switch(address){
		case 0x7f:
			*data_p = BPF_HM_DEVC_PTP_PAGE;
			break;
		case 0x04:
			*data_p = 0;
			break;
		default:
			*data_p = BPF_HM_DEVC_PTP_REG[BPF_HM_DEVC_PTP_PAGE][address];
			break;
	}
//	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI0_SS0 | BPF_HM_DEVC_CPLD_SET_PLL), D_RRH_READ, address, *data_p );
	return 0;
}

int BPF_HM_DEVC_PTP_WRITE (UINT address, UINT data){

	qemu_sched_setparam(0,SCHED_OTHER);
	if(address == 0x7f){
		BPF_HM_DEVC_PTP_PAGE = data;
	}
	else{
		BPF_HM_DEVC_PTP_REG[BPF_HM_DEVC_PTP_PAGE][address] = data;
	}
//	BPF_COM_LOG_SPI( (BPF_HM_DEVC_SPI0_SS0 | BPF_HM_DEVC_CPLD_SET_PLL), D_RRH_WRITE, address, data );
	return 0;
}

int qemu_pre_svregval[16];

int BPF_HM_DEVC_RESERVE(unsigned int dev){
/* 2021/02/18 M&C)Tri.hn Update for intermediate device - add start */
	//char name[32];
	char name[256];
/* 2021/02/18 M&C)Tri.hn Update for intermediate device - add end */
	int	len;
	int	val;
	int svregval;
	int svdif;
	int count;

	len = snprintf(name,sizeof(name),"/qemu/reg/dev_irq%02d.cfg",dev);
	memset(&name[len],0,sizeof(name)-len);
	while(1){
		sleep(1);
		if(dev == BPF_IRQ_SV){
			svdif=0;
			for(count=0 ; count < 16 ;count++){
				svregval = qemu_read_data("/qemu/reg/dev_A0000000.cfg" ,0x150 + (count*4) , 4);
				if(qemu_pre_svregval[count] != svregval ){
					svdif++;
				}
				qemu_pre_svregval[count] = svregval;
			}
			if(svdif == 0){
				continue;
			}
		}
		val = qemu_read_data(name ,0 , 4);
		if(val != 0 ){
			val--;
			qemu_write_data(name,0, val, 4);
			break;
		}
	}
	return 0;
}


int BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ(unsigned int flash_address, unsigned int len,unsigned char *buffer){
	BPF_COM_PLOG_QSPI(E_QSPI_LV_READ_S,flash_address,len,1,0);
	BPF_COM_PLOG_QSPI(E_QSPI_LV_READ,flash_address,len,1,0);
	return 0;
}

int BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(unsigned int flash_address, unsigned int len,unsigned char *buffer){
	BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE_S,flash_address,len,1,0);
	BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE,flash_address,len,1,0);
	return 0;
}

int BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE(unsigned int add, unsigned int len){
	BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE_S,add,len,1,0);
	BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE,add,len,1,0);
	return 0;
}

int BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT(void){
	return 0;
}

void BPF_HM_DEVC_SPI_INIT()
{
	return;
}


int BPF_HM_DEVC_RFPLL_READ(unsigned int address, unsigned int *data_p)
{
	char* name = "/qemu/reg/dev_rfpll.cfg";
	*data_p = qemu_read_data(name ,address , 2);

	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI0_SS1, D_RRH_READ, address, *data_p );
	return 0;
}

int BPF_HM_DEVC_RFPLL_WRITE(unsigned int address, unsigned int data)
{
	char* name = "/qemu/reg/dev_rfpll.cfg";
	qemu_write_data(name ,address , data, 2);

	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI0_SS1, D_RRH_WRITE, address, data);
	return 0;
}


int BPF_HM_DEVC_TXDAC_READ(unsigned int address, unsigned int *data_p)
{
	char* name = "/qemu/reg/dev_txdac.cfg";
	*data_p = qemu_read_data(name ,address , 1);

	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI2_SS1, D_RRH_READ, address, *data_p );
	return 0;
}

int BPF_HM_DEVC_TXDAC_WRITE(unsigned int address, unsigned int data){
	if((address == 0x3A) || (address == 0x113))
	{
		return 0;
	}
	char* name = "/qemu/reg/dev_txdac.cfg";
	qemu_write_data(name ,address , data, 1);

	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI2_SS1, D_RRH_WRITE, address,data );
	return 0;
}


int BPF_HM_DEVC_RXADC_READ(unsigned int address, unsigned int *data_p){
	char* name = "/qemu/reg/dev_rxdac.cfg";
	*data_p = qemu_read_data(name ,address , 1);
	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI2_SS2, D_RRH_READ, address, *data_p );
	return 0;
}

int BPF_HM_DEVC_RXADC_WRITE(unsigned int address, unsigned int data){
	char* name = "/qemu/reg/dev_rxdac.cfg";
	qemu_write_data(name ,address , data, 1);
	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI2_SS2, D_RRH_WRITE, address,data );
	return 0;
}


int BPF_HM_DEVC_RFEEP_READ(unsigned int address, unsigned int *data_p){
	char* name = "/qemu/reg/dev_rfeep.cfg";
	*data_p = qemu_read_data(name ,address , 1);
	//BPF_COM_PLOG_I2C( E_I2C_LV_READ ,address,(unsigned int)*data_p,2,2);//M&C comment
	return 0;
}

int BPF_HM_DEVC_RFEEP_WRITE(unsigned int address, unsigned int data){
	char* name = "/qemu/reg/dev_rfeep.cfg";
	qemu_write_data(name ,address , data, 1);
	BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,address,(unsigned int)data,2,2);
	return 0;
}


int BPF_HM_DEVC_PGC_READ(unsigned char grp, unsigned char sa_add, unsigned char srn, unsigned int *data_p){
	char name[256]; //C9
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_pgc_g%d_sr%d.cfg",grp,srn);
	*data_p = qemu_read_data(name ,sa_add , 1);
	BPF_COM_PLOG_I2C(E_I2C_LV_READ,(unsigned int)(((grp << 24)&0xFF000000) | ((srn << 16) & 0x00FF0000) | sa_add ),*data_p,2,0);	/* packet log */
	return 0;
}

int BPF_HM_DEVC_PGC_WRITE(unsigned char grp, unsigned char sa_add, unsigned char srn, unsigned int data){
	char name[256]; //C9
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_pgc_g%d_sr%d.cfg",grp,srn);
	qemu_write_data(name ,sa_add , data, 1);
	BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,(unsigned int)(((grp << 24)&0xFF000000) | ((srn << 16) & 0x00FF0000) | sa_add ),data,2,0);	/* packet log */

	return 0;
}


int BPF_HM_DEVC_PGCLUT_READ(unsigned char grp, unsigned char sa_add, unsigned char chrow, unsigned char address, unsigned int *data_p)
{
	unsigned char parity = 0;
	char name[256]; //C9
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_pgclut_g%d_ad%d.cfg",grp,sa_add);
	*data_p = qemu_read_data(name ,address , 1);
	BPF_COM_PLOG_I2C(E_I2C_LV_READ,
					(unsigned int)(((grp << 24)&0xFF000000) | ((sa_add << 8) & 0x0000FF00) | address),
					*data_p,2,
					(unsigned int)(((chrow << 16) & 0x00FF0000) | ((parity << 8) & 0x0000FF00) | 1)
					);	/* packet log */
	return 0;
}

int BPF_HM_DEVC_PGCLUT_WRITE(unsigned char grp, unsigned char sa_add, unsigned char chrow, unsigned char address, unsigned int data)
{
	unsigned char parity = 0;
	char name[256]; //C9
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_pgclut_g%d_ad%d.cfg",grp,sa_add);
	qemu_write_data(name ,address ,data, 1);
	BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,
					(unsigned int)(((grp << 24)&0xFF000000) | ((sa_add << 8) & 0x0000FF00) | address),
					data,2,
					(unsigned int)(((chrow << 16) & 0x00FF0000) | ((parity << 8) & 0x0000FF00) | 1)
					);	/* packet log */
	return 0;
}


int BPF_HM_DEVC_QMOD_READ(unsigned char ant, unsigned int sel, unsigned int address, unsigned int *data_p)
{
	char name[256];
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_qmod_a%d_s%d.cfg",ant,sel);
	*data_p = qemu_read_data(name ,address , 1);

	unsigned int devkind;
	switch(sel){
		case BPF_HM_DEVC_TRXSPI_SEL0:
			devkind = BPF_HM_DEVC_SPI3_SS0;
			break;
		case BPF_HM_DEVC_TRXSPI_SEL1:
			devkind = BPF_HM_DEVC_SPI3_SS1;
			break;
		case BPF_HM_DEVC_TRXSPI_SEL3:
			devkind = BPF_HM_DEVC_SPI3_SS3;
			break;
		default:
			return 1;
			break;
	}
	BPF_COM_LOG_SPI( devkind, D_RRH_READ,
					(unsigned int)(((ant << 16 ) & 0xFFFF0000) | (address & BPF_HM_DEVC_TRXSPI_QMOD_ADDRESS_EN_BIT)),
					 *data_p );

	return 0;
}

int BPF_HM_DEVC_QMOD_WRITE(unsigned char ant, unsigned int sel, unsigned int address, unsigned int data)
{
	char name[256]; //C9
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_qmod_a%d_s%d.cfg",ant,sel);
	qemu_write_data(name ,address ,data, 1);

	unsigned int devkind;
	switch(sel){
		case BPF_HM_DEVC_TRXSPI_SEL0:
			devkind = BPF_HM_DEVC_SPI3_SS0;
			break;
		case BPF_HM_DEVC_TRXSPI_SEL1:
			devkind = BPF_HM_DEVC_SPI3_SS1;
			break;
		case BPF_HM_DEVC_TRXSPI_SEL3:
			devkind = BPF_HM_DEVC_SPI3_SS3;
			break;
		default:
			return 1;
			break;
	}

	BPF_COM_LOG_SPI( devkind, D_RRH_WRITE,
					(unsigned int)(((ant << 16 ) & 0xFFFF0000) | (address & BPF_HM_DEVC_TRXSPI_QMOD_ADDRESS_EN_BIT)),
					 data );

	return 0;
}


int BPF_HM_DEVC_I2CSOFT_RESET()
{
	return 0;
}


int BPF_HM_DEVC_QDEM_READ(unsigned char ant, unsigned int sel, unsigned int address, unsigned int *data_p)
{
	char name[256]; //C9
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_qmem_a%d_s%d.cfg",ant,sel);
	*data_p = qemu_read_data(name ,address , 1);
	unsigned int devkind;
	switch(sel){
		case BPF_HM_DEVC_TRXSPI_SEL2:
			devkind = BPF_HM_DEVC_SPI3_SS2;
			break;
		case BPF_HM_DEVC_TRXSPI_SEL3:
			devkind = BPF_HM_DEVC_SPI3_SS3;
			break;
		default:
			return 1;
			break;
	}
	BPF_COM_LOG_SPI( devkind, D_RRH_READ,
					(unsigned int)(((ant << 16 ) & 0xFFFF0000) | (address & BPF_HM_DEVC_TRXSPI_QMOD_ADDRESS_EN_BIT)),
					 *data_p );
	return 0;
}

int BPF_HM_DEVC_QDEM_WRITE(unsigned char ant, unsigned int sel, unsigned int address, unsigned int data)
{
	char name[256]; //C9
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_qmem_a%d_s%d.cfg",ant,sel);
	qemu_write_data(name ,address ,data, 1);

	unsigned int devkind;
	switch(sel){
		case BPF_HM_DEVC_TRXSPI_SEL2:
			devkind = BPF_HM_DEVC_SPI3_SS2;
			break;
		case BPF_HM_DEVC_TRXSPI_SEL3:
			devkind = BPF_HM_DEVC_SPI3_SS3;
			break;
		default:
			return 1;
			break;
	}

	BPF_COM_LOG_SPI( devkind, D_RRH_WRITE,
					(unsigned int)(((ant << 16 ) & 0xFFFF0000) | (address & BPF_HM_DEVC_TRXSPI_QMOD_ADDRESS_EN_BIT)),
					 data );

	return 0;
}


int BPF_HM_DEVC_PAVGDAC_READ(unsigned char ant, unsigned int *data_p)
{
	unsigned int kind = qemu_read_data("/qemu/reg/dev_eeprom.cfg" ,0x40 , 1);
	char name[256];
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_pavgdac_a%d.cfg",ant);
	*data_p = qemu_read_data(name ,0 , 1);
	unsigned int devkind;
	switch(kind){
		case D_RRH_EEP_DEV_KIND_5GDU_SUB6:
			devkind = BPF_HM_DEVC_SPI3_SS6;
			break;
		case D_RRH_EEP_DEV_KIND_5GDU_MMW:
			devkind = BPF_HM_DEVC_SPI3_SS4;
			break;
		default:
			return 1;
			break;
	}
	BPF_COM_LOG_SPI( devkind, D_RRH_READ,ant,*data_p );
	return 0;
}

int BPF_HM_DEVC_PAVGDAC_WRITE(unsigned char ant, unsigned int data)
{
	unsigned int kind = qemu_read_data("/qemu/reg/dev_eeprom.cfg" ,0x40 , 1);
	char name[256];
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_pavgdac_a%d.cfg",ant);
	qemu_write_data(name ,0 ,data, 1);

	unsigned int devkind;
	switch(kind){
		case D_RRH_EEP_DEV_KIND_5GDU_SUB6:
			devkind = BPF_HM_DEVC_SPI3_SS6;
			break;
		case D_RRH_EEP_DEV_KIND_5GDU_MMW:
			devkind = BPF_HM_DEVC_SPI3_SS4;
			break;
		default:
			return 1;
			break;
	}
	BPF_COM_LOG_SPI( devkind, D_RRH_WRITE,ant,data );
	return 0;
}


int BPF_HM_DEVC_TXVATTDAC_READ(unsigned char ant, unsigned int *data_p)
{
	unsigned int kind = qemu_read_data("/qemu/reg/dev_eeprom.cfg" ,0x40 , 1);
	char name[256];
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_txvattdac_a%d.cfg",ant);
	*data_p = qemu_read_data(name ,0 , 1);

	unsigned int devkind;
	switch(kind){
		case D_RRH_EEP_DEV_KIND_5GDU_SUB6:
			devkind = BPF_HM_DEVC_SPI3_SS5;
			break;
		case D_RRH_EEP_DEV_KIND_5GDU_MMW:
			devkind = BPF_HM_DEVC_SPI3_SS7;
			break;
		default:
			return 1;
			break;
	}
	BPF_COM_LOG_SPI( devkind, D_RRH_READ,ant,*data_p );
	return 0;
}

int BPF_HM_DEVC_TXVATTDAC_WRITE(unsigned char ant, unsigned int data)
{
	unsigned int kind = qemu_read_data("/qemu/reg/dev_eeprom.cfg" ,0x40 , 1);
	char name[256];
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_txvattdac_a%d.cfg",ant);
	qemu_write_data(name ,0 ,data, 1);

	unsigned int devkind;
	switch(kind){
		case D_RRH_EEP_DEV_KIND_5GDU_SUB6:
			devkind = BPF_HM_DEVC_SPI3_SS5;
			break;
		case D_RRH_EEP_DEV_KIND_5GDU_MMW:
			devkind = BPF_HM_DEVC_SPI3_SS7;
			break;
		default:
			return 1;
			break;
	}
	BPF_COM_LOG_SPI( devkind, D_RRH_WRITE,ant,data );
	return 0;
}


int BPF_HM_DEVC_RXVATTDAC_READ(unsigned char ant, unsigned int *data_p)
{
	char name[256]; //C9
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_rxvattadc_a%d.cfg",ant);
	*data_p = qemu_read_data(name ,0 , 1);
	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI3_SS8, D_RRH_READ,ant,*data_p );
	return 0;
}

int BPF_HM_DEVC_RXVATTDAC_WRITE(unsigned char ant, unsigned int data)
{
	char name[256]; //C9
	memset(name,0,sizeof(name));
	snprintf(name,sizeof(name),"/qemu/reg/dev_rxvattadc_a%d.cfg",ant);
	qemu_write_data(name ,0 ,data, 1);
	BPF_COM_LOG_SPI( BPF_HM_DEVC_SPI3_SS8, D_RRH_WRITE,ant,data );
	return 0;
}


int BPF_HM_DEVC_FANDAC_WRITE(unsigned int data)
{
	qemu_write_data("/qemu/reg/dev_fabdac.cfg" ,0 ,data, 4);
	return 0;
}

int BPF_HM_DEVC_PGC_GAIN_PHASE_WRITE(
	 unsigned char chain_grp,
	 unsigned char txrx,
	 unsigned int gain1 ,
	 unsigned int phase1,
	 unsigned int gain2 ,
	 unsigned int phase2,
	 unsigned int gain3 ,
	 unsigned int phase3,
	 unsigned int gain4 ,
	 unsigned int phase4
)
{
	qemu_write_data("/qemu/reg/dev_gain_pahse.cfg" ,0 ,gain1, 4);
	qemu_write_data("/qemu/reg/dev_gain_pahse.cfg" ,4 ,phase1, 4);

	qemu_write_data("/qemu/reg/dev_gain_pahse.cfg" ,0x10 ,gain2, 4);
	qemu_write_data("/qemu/reg/dev_gain_pahse.cfg" ,0x14 ,phase2, 4);

	qemu_write_data("/qemu/reg/dev_gain_pahse.cfg" ,0x20 ,gain3, 4);
	qemu_write_data("/qemu/reg/dev_gain_pahse.cfg" ,0x24 ,phase3, 4);

	qemu_write_data("/qemu/reg/dev_gain_pahse.cfg" ,0x30 ,gain4, 4);
	qemu_write_data("/qemu/reg/dev_gain_pahse.cfg" ,0x34 ,phase4, 4);


	return 0;
}


int BPF_HM_DEVC_PGC_GAIN_PHASE_READ(
	 unsigned char chain_grp,
	 unsigned char txrx,
	 unsigned int* gain1 ,
	 unsigned int* phase1,
	 unsigned int* reg1,
	 unsigned int* gain2 ,
	 unsigned int* phase2,
	 unsigned int* reg2,
	 unsigned int* gain3 ,
	 unsigned int* phase3,
	 unsigned int* reg3,
	 unsigned int* gain4 ,
	 unsigned int* phase4,
	 unsigned int* reg4
)
{
	*gain1 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0 ,4);
	*phase1 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,4 ,4);
	*reg1 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,8 ,4);

	*gain2 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0x10 ,4);
	*phase2 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0x14 ,4);
	*reg2 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0x18 ,4);

	*gain3 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0x20 ,4);
	*phase3 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0x24 ,4);
	*reg3 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0x28 ,4);

	*gain4 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0x30 ,4);
	*phase4 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0x34 ,4);
	*reg4 = qemu_read_data("/qemu/reg/dev_gain_pahse.cfg" ,0x38 ,4);


	return 0;
}


int BPF_HM_DEVC_PGC_WRITE2(
	 unsigned char chain_grp,
	 unsigned int data1,
	 unsigned int data2,
	 unsigned int data3,
	 unsigned int data4
)
{
	qemu_write_data("/qemu/reg/dev_pgc2.cfg" ,0 ,data1, 4);
	qemu_write_data("/qemu/reg/dev_pgc2.cfg" ,0x10 ,data2, 4);
	qemu_write_data("/qemu/reg/dev_pgc2.cfg" ,0x20 ,data3, 4);
	qemu_write_data("/qemu/reg/dev_pgc2.cfg" ,0x30 ,data4, 4);

	return 0;
}


int BPF_HM_DEVC_PGC_READ2(
	 unsigned char chain_grp,
	 unsigned char bank,
	 unsigned int* gain1 ,
	 unsigned int* phase1,
	 unsigned int* reg1,
	 unsigned int* gain2 ,
	 unsigned int* phase2,
	 unsigned int* reg2,
	 unsigned int* gain3 ,
	 unsigned int* phase3,
	 unsigned int* reg3,
	 unsigned int* gain4 ,
	 unsigned int* phase4,
	 unsigned int* reg4
)
{
	*gain1   = qemu_read_data("/qemu/reg/dev_pgc2.cfg",0 , 4);
	*phase1 = qemu_read_data("/qemu/reg/dev_pgc2.cfg" ,4 , 4);
	*reg1   = qemu_read_data("/qemu/reg/dev_pgc2.cfg" ,8 , 4);

	*gain2   = qemu_read_data("/qemu/reg/dev_pgc2.cfg",0x10 , 4);
	*phase2 = qemu_read_data("/qemu/reg/dev_pgc2.cfg" ,0x14 , 4);
	*reg2   = qemu_read_data("/qemu/reg/dev_pgc2.cfg" ,0x18 , 4);

	*gain3   = qemu_read_data("/qemu/reg/dev_pgc2.cfg",0x20 , 4);
	*phase3 = qemu_read_data("/qemu/reg/dev_pgc2.cfg" ,0x24 , 4);
	*reg3   = qemu_read_data("/qemu/reg/dev_pgc2.cfg" ,0x28 , 4);

	*gain4   = qemu_read_data("/qemu/reg/dev_pgc2.cfg",0x30 , 4);
	*phase4 = qemu_read_data("/qemu/reg/dev_pgc2.cfg" ,0x34 , 4);
	*reg4   = qemu_read_data("/qemu/reg/dev_pgc2.cfg" ,0x38 , 4);

	return 0;
}


int BPF_HM_DEVC_PGCLUT_WRITE2(unsigned int data_len, unsigned char* data_p)
{
	return 0;
}



extern int bpf_hm_devc_qspi_flash_read(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer);
int BPF_HM_DEVC_QSPI_FLASH_READ(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer)
{
	/*	mmW PGCトレーニング領域の場合	*/
	if(offset == 0x06A00000)
	{
		memset(buffer, 0, size);
		return BPF_HM_DEVC_COMPLETE;
	}

	int rtn = bpf_hm_devc_qspi_flash_read( 0, offset, size, buffer );	/* always 0 for QEMU */
	if(rtn == BPF_HM_DEVC_COMPLETE)
	{
		BPF_COM_PLOG_QSPI(E_QSPI_LV_READ,offset,size,0,face);
	}
	return rtn;
}

extern int bpf_hm_devc_qspi_flash_write(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer);
int BPF_HM_DEVC_QSPI_FLASH_WRITE(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer)
{
	BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE_S,offset,size,0,face);
	int rtn = bpf_hm_devc_qspi_flash_write( 0, offset, size, buffer );	/* always 0 for QEMU */
	if(rtn == BPF_HM_DEVC_COMPLETE)
	{
		/* packet log send */
		BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE,offset,size,0,face);
	}
	return rtn;
}

extern int bpf_hm_devc_qspi_flash_erase(unsigned int face,unsigned int offset, unsigned int size);
int BPF_HM_DEVC_QSPI_FLASH_ERASE(unsigned int face,unsigned int offset, unsigned int size)
{
	BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE_S,offset,size,0,face);
	int rtn = bpf_hm_devc_qspi_flash_erase( 0, offset, size );	/* always 0 for QEMU */
	if(rtn == BPF_HM_DEVC_COMPLETE)
	{
		BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE,offset,size,0,face);
	}
	return rtn;
}

int BPF_HM_DEVC_GETTIME(T_RRH_SYSTEM_TIME* sysTime )
{
	struct timeval tv;
	int rtn;
	struct tm ltm;
	struct tm *dmy_tm;
	struct timespec	boottime;

	if( (rtn = gettimeofday(&tv,NULL)) == BPF_HM_DEVC_SYS_NG ){
		return BPF_HM_DEVC_NG;
	}
	if( (dmy_tm = localtime_r(&tv.tv_sec, &ltm)) == NULL ){
		return BPF_HM_DEVC_NG;
	}
	(void)clock_gettime( CLOCK_MONOTONIC_COARSE   , &boottime);	/* 時刻取得(bootup時刻) */

	sysTime->year = (ltm.tm_year + BPF_HMDEVC_YEARE_OFFSET);
	sysTime->month = ltm.tm_mon + BPF_HMDEVC_MONTH_OFFSET;
	sysTime->day   = ltm.tm_mday;
	sysTime->hour  = ltm.tm_hour;
	sysTime->min   = ltm.tm_min;
	sysTime->sec   = ltm.tm_sec;
	sysTime->msec  = (tv.tv_usec / 10000);
	sysTime->ucount= (UINT)(boottime.tv_sec);  /* bootup時刻 */
	return BPF_HM_DEVC_COMPLETE;
}

int RFIC_TAL_START(void)
{
	printf("RFIC_TAL_START skip for QEMU.\n");
	return BPF_HM_DEVC_COMPLETE;
}


int BPF_RM_SVRM_CPUAFFINITY (unsigned int process_desc, int cpu, int* errcd){
	cpu_set_t cpu_mask;                /* CPU affinity mask */

	(void)process_desc;
	(void)cpu;

	__CPU_ZERO_S(sizeof(cpu_mask),&cpu_mask);
	__CPU_SET_S(0,sizeof(cpu_mask),&cpu_mask);
	(void)sched_setaffinity(0, sizeof(cpu_mask), &cpu_mask);
	__CPU_SET_S(1,sizeof(cpu_mask),&cpu_mask);
	(void)sched_setaffinity(0, sizeof(cpu_mask), &cpu_mask);
 	*errcd = 0;
	return BPF_HM_DEVC_COMPLETE;
}


int BPF_HM_DEVC_GET_ENV( void )
{
	return BPF_HM_DEVC_NG;	/* QEMU環境は常にNG */
}

int RFIC_TAL_GETTXQECSTATUS(uint8_t unit , int channelSel ,int status_len , void* status )
{
	memset(status,0x11,status_len);
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

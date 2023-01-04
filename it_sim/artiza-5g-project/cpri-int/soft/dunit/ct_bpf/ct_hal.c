#include <stdio.h>
#include <stdarg.h>
#include "f_rrh_inc.h"
#include "bpf_l_com.h"
#include "BPF_HM_DEVC.h"

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


extern unsigned int rrulib_reg_getval(unsigned int addr , char** name);
extern void rrulib_reg_setval(unsigned int addr , unsigned int val , char** name);
extern unsigned int rrulib_spi_getval(unsigned int kind , unsigned int subkind,unsigned int addr , char** name);
extern void rrulib_spi_setval(unsigned int kind , unsigned int subkind,unsigned int addr , unsigned int data ,char** name);
extern void* rrulib_shm_addrget(unsigned int kind , char** name);
extern void rrulib_qspi_get(unsigned int face , unsigned int addr , unsigned int size, unsigned char* buf , char** name);
extern void rrulib_qspi_set(unsigned int face , unsigned int addr , unsigned int size, unsigned char* buf , char** name);
extern void rrulib_i2c_init(unsigned int val);
extern unsigned int rrulib_i2c_getval(unsigned int kind , unsigned int subkind , unsigned int addr, char**name);
extern void rrulib_i2c_setval(unsigned int kind , unsigned int subkind , unsigned int addr , unsigned int val , char** name);
extern unsigned int rrulib_log_convert;

typedef struct{
	unsigned int vflg;
	int			 result;
}t_call_result;

typedef struct{
	char*			name;
	t_call_result*	ptn;
}t_call_result_ptn;

t_call_result result_BPF_HM_DEVC_RESERVE;
t_call_result result_BPF_HM_DEVC_QSPI_FLASH_READ;
t_call_result result_BPF_HM_DEVC_QSPI_FLASH_WRITE;
t_call_result result_BPF_HM_DEVC_QSPI_FLASH_ERASE;
t_call_result result_BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER;
t_call_result result_BPF_HM_DEVC_QSPI_FLASH_READ_FLASH_KIND;
t_call_result result_BPF_HM_DEVC_EEPROM_READ        ;
t_call_result result_BPF_HM_DEVC_EEPROM_WRITE       ;
t_call_result result_BPF_HM_DEVC_SFP_READ;
t_call_result result_BPF_HM_DEVC_SFP_READ_BUFFER;
t_call_result result_BPF_HM_DEVC_SFP_WRITE;
t_call_result result_BPF_HM_DEVC_MMI_CPLD_READ;
t_call_result result_BPF_HM_DEVC_MMI_CPLD_WRITE;
t_call_result result_BPF_HM_DEVC_TEMP_READ;
t_call_result result_BPF_HM_DEVC_TEMP2_READ;
t_call_result result_BPF_HM_DEVC_REG_READ            ;
t_call_result result_BPF_HM_DEVC_REG_READ_BUFFER     ;
t_call_result result_BPF_HM_DEVC_REG_READ_BUFFER_END ;
t_call_result result_BPF_HM_DEVC_REG_READ_MULTI      ;
t_call_result result_BPF_HM_DEVC_REG_READ_BIT        ;
t_call_result result_BPF_HM_DEVC_REG_READ_NOMUTEX    ;
t_call_result result_BPF_HM_DEVC_REG_WRITE          ;
t_call_result result_BPF_HM_DEVC_REG_WRITE_BUFFER   ;
t_call_result result_BPF_HM_DEVC_REG_WRITE_BUFFER_END   ;
t_call_result result_BPF_HM_DEVC_REG_WRITE_MULTI    ;
t_call_result result_BPF_HM_DEVC_REG_WRITE_BITON    ;
t_call_result result_BPF_HM_DEVC_REG_WRITE_BITOFF   ;
t_call_result result_BPF_HM_DEVC_GPIO_READ          ;
t_call_result result_BPF_HM_DEVC_GPIO_WRITE         ;
t_call_result result_BPF_HM_DEVC_SETTIM             ;
t_call_result result_BPF_HM_DEVC_SETTIM_BCD         ;
t_call_result result_BPF_HM_DEVC_GETTIME            ;
t_call_result result_BPF_HM_DEVC_GETTIM_BCD         ;
t_call_result result_BPF_HM_DEVC_GETTIM_OS          ;
t_call_result result_BPF_HM_DEVC_RFIC_READ          ;
t_call_result result_BPF_HM_DEVC_RFIC_WRITE         ;
t_call_result result_BPF_HM_DEVC_RFIC_RESET         ;
t_call_result result_BPF_HM_DEVC_EXT_PLL_READ       ;
t_call_result result_BPF_HM_DEVC_EXT_PLL_WRITE       ;
t_call_result result_BPF_HM_DEVC_PTP_READ       ;
t_call_result result_BPF_HM_DEVC_PTP_WRITE       ;
t_call_result result_BPF_HM_DEVC_ASSIGN       ;
t_call_result result_BPF_HM_DEVC_FORCE_ACCESS       ;
t_call_result result_BPF_HM_DEVC_FPGA_ACCESS       ;
t_call_result result_BPF_HM_DEVC_PL_CONFIG_STATE_SET       ;
t_call_result result_BPF_HM_DEVC_PL_FLASH_ID_SET       ;
t_call_result result_BPF_HM_DEVC_WDT_SET       ;
t_call_result result_BPF_HM_DEVC_WDT_CLEAR       ;
t_call_result result_BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ       ;
t_call_result result_BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE       ;
t_call_result result_BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE       ;
t_call_result result_BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT       ;
t_call_result result_BPF_HM_DEVC_GETTIME_STR;
t_call_result result_BPF_HM_DEVC_RFPLL_READ;
t_call_result result_BPF_HM_DEVC_RFPLL_WRITE;
t_call_result result_BPF_HM_DEVC_JCPLL_READ;
t_call_result result_BPF_HM_DEVC_JCPLL_WRITE;
t_call_result result_BPF_HM_DEVC_TXDAC_READ;
t_call_result result_BPF_HM_DEVC_TXDAC_WRITE;
t_call_result result_BPF_HM_DEVC_RXADC_READ;
t_call_result result_BPF_HM_DEVC_RXADC_WRITE;
t_call_result result_BPF_HM_DEVC_RFEEP_READ;
t_call_result result_BPF_HM_DEVC_RFEEP_WRITE;
t_call_result result_BPF_HM_DEVC_PGC_READ;
t_call_result result_BPF_HM_DEVC_PGC_WRITE;
t_call_result result_BPF_HM_DEVC_PGCLUT_READ;
t_call_result result_BPF_HM_DEVC_PGCLUT_WRITE;
t_call_result result_BPF_HM_DEVC_QMOD_READ;
t_call_result result_BPF_HM_DEVC_QMOD_WRITE;
t_call_result result_BPF_HM_DEVC_I2CSOFT_RESET;
t_call_result result_BPF_HM_DEVC_QDEM_READ;
t_call_result result_BPF_HM_DEVC_QDEM_WRITE;
t_call_result result_BPF_HM_DEVC_PAVGDAC_READ;
t_call_result result_BPF_HM_DEVC_PAVGDAC_WRITE;
t_call_result result_BPF_HM_DEVC_TXVATTDAC_READ;
t_call_result result_BPF_HM_DEVC_TXVATTDAC_WRITE;
t_call_result result_BPF_HM_DEVC_RXVATTDAC_READ;
t_call_result result_BPF_HM_DEVC_RXVATTDAC_WRITE;
t_call_result result_BPF_HM_DEVC_TEMP_INIT;
t_call_result result_BPF_HM_DEVC_FANDAC_WRITE;
t_call_result result_BPF_HM_DEVC_PGC_GAIN_PHASE_WRITE;
t_call_result result_BPF_HM_DEVC_PGC_GAIN_PHASE_READ;
t_call_result result_BPF_HM_DEVC_PGC_WRITE2;
t_call_result result_BPF_HM_DEVC_PGC_READ2;
t_call_result result_BPF_HM_DEVC_PGCLUT_WRITE2;


static t_call_result_ptn result[] = 
{
	{ "BPF_HM_DEVC_RESERVE", &result_BPF_HM_DEVC_RESERVE },
	{ "BPF_HM_DEVC_QSPI_FLASH_READ", &result_BPF_HM_DEVC_QSPI_FLASH_READ },
	{ "BPF_HM_DEVC_QSPI_FLASH_WRITE", &result_BPF_HM_DEVC_QSPI_FLASH_WRITE },
	{ "BPF_HM_DEVC_QSPI_FLASH_ERASE", &result_BPF_HM_DEVC_QSPI_FLASH_ERASE },
	{ "BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER", &result_BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER },
	{ "BPF_HM_DEVC_QSPI_FLASH_READ_FLASH_KIND", &result_BPF_HM_DEVC_QSPI_FLASH_READ_FLASH_KIND },
	{ "BPF_HM_DEVC_EEPROM_READ", &result_BPF_HM_DEVC_EEPROM_READ },
	{ "BPF_HM_DEVC_EEPROM_WRITE", &result_BPF_HM_DEVC_EEPROM_WRITE },
	{ "BPF_HM_DEVC_SFP_READ", &result_BPF_HM_DEVC_SFP_READ },
	{ "BPF_HM_DEVC_SFP_READ_BUFFER", &result_BPF_HM_DEVC_SFP_READ_BUFFER },
	{ "BPF_HM_DEVC_SFP_WRITE", &result_BPF_HM_DEVC_SFP_WRITE },
	{ "BPF_HM_DEVC_MMI_CPLD_READ", &result_BPF_HM_DEVC_MMI_CPLD_READ },
	{ "BPF_HM_DEVC_MMI_CPLD_WRITE", &result_BPF_HM_DEVC_MMI_CPLD_WRITE },
	{ "BPF_HM_DEVC_TEMP_READ", &result_BPF_HM_DEVC_TEMP_READ },
	{ "BPF_HM_DEVC_TEMP2_READ", &result_BPF_HM_DEVC_TEMP2_READ },
	{ "BPF_HM_DEVC_REG_READ", &result_BPF_HM_DEVC_REG_READ },
	{ "BPF_HM_DEVC_REG_READ_BUFFER", &result_BPF_HM_DEVC_REG_READ_BUFFER },
	{ "BPF_HM_DEVC_REG_READ_BUFFER_END", &result_BPF_HM_DEVC_REG_READ_BUFFER_END },
	{ "BPF_HM_DEVC_REG_READ_MULTI", &result_BPF_HM_DEVC_REG_READ_MULTI },
	{ "BPF_HM_DEVC_REG_READ_BIT", &result_BPF_HM_DEVC_REG_READ_BIT },
	{ "BPF_HM_DEVC_REG_READ_NOMUTEX", &result_BPF_HM_DEVC_REG_READ_NOMUTEX },
	{ "BPF_HM_DEVC_REG_WRITE", &result_BPF_HM_DEVC_REG_WRITE },
	{ "BPF_HM_DEVC_REG_WRITE_BUFFER", &result_BPF_HM_DEVC_REG_WRITE_BUFFER },
	{ "BPF_HM_DEVC_REG_WRITE_BUFFER_END", &result_BPF_HM_DEVC_REG_WRITE_BUFFER_END },
	{ "BPF_HM_DEVC_REG_WRITE_MULTI", &result_BPF_HM_DEVC_REG_WRITE_MULTI },
	{ "BPF_HM_DEVC_REG_WRITE_BITON", &result_BPF_HM_DEVC_REG_WRITE_BITON },
	{ "BPF_HM_DEVC_REG_WRITE_BITOFF", &result_BPF_HM_DEVC_REG_WRITE_BITOFF },
	{ "BPF_HM_DEVC_GPIO_READ", &result_BPF_HM_DEVC_GPIO_READ },
	{ "BPF_HM_DEVC_GPIO_WRITE", &result_BPF_HM_DEVC_GPIO_WRITE },
	{ "BPF_HM_DEVC_SETTIM", &result_BPF_HM_DEVC_SETTIM },
	{ "BPF_HM_DEVC_SETTIM_BCD", &result_BPF_HM_DEVC_SETTIM_BCD },
	{ "BPF_HM_DEVC_GETTIME", &result_BPF_HM_DEVC_GETTIME },
	{ "BPF_HM_DEVC_GETTIM_BCD", &result_BPF_HM_DEVC_GETTIM_BCD },
	{ "BPF_HM_DEVC_GETTIM_OS", &result_BPF_HM_DEVC_GETTIM_OS },
	{ "BPF_HM_DEVC_RFIC_READ", &result_BPF_HM_DEVC_RFIC_READ },
	{ "BPF_HM_DEVC_RFIC_WRITE", &result_BPF_HM_DEVC_RFIC_WRITE },
	{ "BPF_HM_DEVC_RFIC_RESET", &result_BPF_HM_DEVC_RFIC_RESET },
	{ "BPF_HM_DEVC_EXT_PLL_READ", &result_BPF_HM_DEVC_EXT_PLL_READ },
	{ "BPF_HM_DEVC_EXT_PLL_WRITE", &result_BPF_HM_DEVC_EXT_PLL_WRITE },
	{ "BPF_HM_DEVC_PTP_READ", &result_BPF_HM_DEVC_PTP_READ },
	{ "BPF_HM_DEVC_PTP_WRITE", &result_BPF_HM_DEVC_PTP_WRITE },
	{ "BPF_HM_DEVC_ASSIGN", &result_BPF_HM_DEVC_ASSIGN },
	{ "BPF_HM_DEVC_FORCE_ACCESS", &result_BPF_HM_DEVC_FORCE_ACCESS },
	{ "BPF_HM_DEVC_FPGA_ACCESS", &result_BPF_HM_DEVC_FPGA_ACCESS },
	{ "BPF_HM_DEVC_PL_CONFIG_STATE_SET", &result_BPF_HM_DEVC_PL_CONFIG_STATE_SET },
	{ "BPF_HM_DEVC_PL_FLASH_ID_SET", &result_BPF_HM_DEVC_PL_FLASH_ID_SET },
	{ "BPF_HM_DEVC_WDT_SET", &result_BPF_HM_DEVC_WDT_SET },
	{ "BPF_HM_DEVC_WDT_CLEAR", &result_BPF_HM_DEVC_WDT_CLEAR },
	{ "BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ", &result_BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ },
	{ "BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE", &result_BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE },
	{ "BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE", &result_BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE },
	{ "BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT", &result_BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT },
	{ "BPF_HM_DEVC_GETTIME_STR", &result_BPF_HM_DEVC_GETTIME_STR },
	{ "BPF_HM_DEVC_RFPLL_READ", &result_BPF_HM_DEVC_RFPLL_READ },
	{ "BPF_HM_DEVC_RFPLL_WRITE", &result_BPF_HM_DEVC_RFPLL_WRITE },
	{ "BPF_HM_DEVC_JCPLL_READ", &result_BPF_HM_DEVC_JCPLL_READ },
	{ "BPF_HM_DEVC_JCPLL_WRITE", &result_BPF_HM_DEVC_JCPLL_WRITE },
	{ "BPF_HM_DEVC_TXDAC_READ", &result_BPF_HM_DEVC_TXDAC_READ },
	{ "BPF_HM_DEVC_TXDAC_WRITE", &result_BPF_HM_DEVC_TXDAC_WRITE },
	{ "BPF_HM_DEVC_RXADC_READ", &result_BPF_HM_DEVC_RXADC_READ },
	{ "BPF_HM_DEVC_RXADC_WRITE", &result_BPF_HM_DEVC_RXADC_WRITE },
	{ "BPF_HM_DEVC_RFEEP_READ", &result_BPF_HM_DEVC_RFEEP_READ },
	{ "BPF_HM_DEVC_RFEEP_WRITE", &result_BPF_HM_DEVC_RFEEP_WRITE },
	{ "BPF_HM_DEVC_PGC_READ", &result_BPF_HM_DEVC_PGC_READ },
	{ "BPF_HM_DEVC_PGC_WRITE", &result_BPF_HM_DEVC_PGC_WRITE },
	{ "BPF_HM_DEVC_PGCLUT_READ", &result_BPF_HM_DEVC_PGCLUT_READ },
	{ "BPF_HM_DEVC_PGCLUT_WRITE", &result_BPF_HM_DEVC_PGCLUT_WRITE },
	{ "BPF_HM_DEVC_QMOD_READ", &result_BPF_HM_DEVC_QMOD_READ },
	{ "BPF_HM_DEVC_QMOD_WRITE", &result_BPF_HM_DEVC_QMOD_WRITE },
	{ "BPF_HM_DEVC_I2CSOFT_RESET", &result_BPF_HM_DEVC_I2CSOFT_RESET },
	{ "BPF_HM_DEVC_QDEM_READ", &result_BPF_HM_DEVC_QDEM_READ },
	{ "BPF_HM_DEVC_QDEM_WRITE", &result_BPF_HM_DEVC_QDEM_WRITE },
	{ "BPF_HM_DEVC_PAVGDAC_READ", &result_BPF_HM_DEVC_PAVGDAC_READ },
	{ "BPF_HM_DEVC_PAVGDAC_WRITE", &result_BPF_HM_DEVC_PAVGDAC_WRITE },
	{ "BPF_HM_DEVC_TXVATTDAC_READ", &result_BPF_HM_DEVC_TXVATTDAC_READ },
	{ "BPF_HM_DEVC_TXVATTDAC_WRITE", &result_BPF_HM_DEVC_TXVATTDAC_WRITE },
	{ "BPF_HM_DEVC_RXVATTDAC_READ", &result_BPF_HM_DEVC_RXVATTDAC_READ },
	{ "BPF_HM_DEVC_RXVATTDAC_WRITE", &result_BPF_HM_DEVC_RXVATTDAC_WRITE },
	{ "BPF_HM_DEVC_TEMP_INIT", &result_BPF_HM_DEVC_TEMP_INIT },
	{ "BPF_HM_DEVC_FANDAC_WRITE", &result_BPF_HM_DEVC_FANDAC_WRITE },
	{ "BPF_HM_DEVC_PGC_GAIN_PHASE_WRITE", &result_BPF_HM_DEVC_PGC_GAIN_PHASE_WRITE },
	{ "BPF_HM_DEVC_PGC_GAIN_PHASE_READ", &result_BPF_HM_DEVC_PGC_GAIN_PHASE_READ },
	{ "BPF_HM_DEVC_PGC_WRITE2", &result_BPF_HM_DEVC_PGC_WRITE },
	{ "BPF_HM_DEVC_PGC_READ2", &result_BPF_HM_DEVC_PGC_READ2 },
	{ "BPF_HM_DEVC_PGCLUT_WRITE2", &result_BPF_HM_DEVC_PGCLUT_WRITE2 },
	{ NULL      , NULL            }
};

void hal_callresult_init(char* name )
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

void hal_callresult_set(char* name , int val)
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

int hal_callresult_get(char* name )
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

#define DPRINT(...) f_rrulib_dmsg("HAL", __func__ ,__VA_ARGS__ )
#define DPRINTCONVERT(dev,mode,...) f_rrulib_dconvert_log(dev,mode, __VA_ARGS__ )

#define RETURN()  return (hal_callresult_get((char*)__func__))

int BPF_HM_DEVC_RESERVE(unsigned int dev){DPRINT("dev:0x%x\n",dev);RETURN();}
int BPF_HM_DEVC_QSPI_FLASH_READ(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer)
{
	char* name;
	rrulib_qspi_get(face,offset,size,buffer,&name);
	DPRINT("%08x -> %08x [%s]\n",offset,size,name);
	RETURN();
}
int BPF_HM_DEVC_QSPI_FLASH_WRITE(unsigned int face,unsigned int offset,unsigned int size,unsigned char *buffer)
{
	char* name;
	rrulib_qspi_set(face,offset,size,buffer,&name);
	DPRINT("%08x <- %08x [%s]\n",offset,size,name);
	RETURN();
}

int BPF_HM_DEVC_QSPI_FLASH_ERASE(unsigned int face,unsigned int offset, unsigned int size)
{
	void* buf;
	char* name;

	buf = malloc(size);
	memset(buf,0xff,size);
	rrulib_qspi_set(face,offset,size,buf,&name);
	free(buf);
	DPRINT("%08x <- %08x [%s]\n",offset,size,name);
	RETURN();
}

int BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ(unsigned int flash_address, unsigned int len,unsigned char *buffer){RETURN();}
int BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(unsigned int flash_address, unsigned int len,unsigned char *buffer){RETURN();}
int BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE(unsigned int add, unsigned int len){RETURN();}
int BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT(void){RETURN();}

int BPF_HM_DEVC_EEPROM_READ(unsigned short offset, unsigned char *data_p)
{
	char* name;
	*data_p = (unsigned char)(rrulib_i2c_getval(0,0,offset,&name));
	DPRINT("%08x -> %08x [%s]\n",offset,*data_p,name);
	RETURN();
}
int BPF_HM_DEVC_EEPROM_WRITE(unsigned short offset, unsigned char *data_p)
{
	char* name;
	rrulib_i2c_setval(0,0,offset,(unsigned int)(*data_p),&name);
	DPRINT("%08x <- %08x [%s]\n",offset,*data_p,name);
	RETURN();
}

int BPF_HM_DEVC_REG_READ(unsigned int loglevel, unsigned int offset, unsigned int *data_p){
	char* name;
	*data_p = rrulib_reg_getval(offset,&name);
	DPRINTCONVERT("REG","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,*data_p);
	DPRINT("[log:%x]%08x -> %08x [%s]\n",loglevel,offset,*data_p,name);
	RETURN();
}

int BPF_HM_DEVC_REG_WRITE(unsigned int loglevel, unsigned int offset, unsigned int *data_p)
{	
	char* name;
	rrulib_reg_setval(offset,*data_p,&name);
	DPRINTCONVERT("REG","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,*data_p);
	DPRINT("[log:%x]%08x <- %08x [%s]\n",loglevel,offset,*data_p,name);
	RETURN();
}


int BPF_HM_DEVC_REG_WRITE_BITON(unsigned int loglevel, unsigned int offset, unsigned int bitifno )
{
	unsigned int data;
	char* name;

	data =rrulib_reg_getval(offset,&name);
	DPRINTCONVERT("REG","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,data);
	DPRINT("[log:%x]%08x -> %08x [%s]\n",loglevel,offset,data,name);
	data |= bitifno;
	rrulib_reg_setval(offset,data,&name);
	DPRINTCONVERT("REG","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,data);
	DPRINT("[log:%x]%08x <- %08x [%s]\n",loglevel,offset,data,name);
	RETURN();
}
int BPF_HM_DEVC_REG_WRITE_BITOFF(unsigned int loglevel, unsigned int offset, unsigned int bitifno )
{
	unsigned int data;
	char* name;

	data =rrulib_reg_getval(offset,&name);
	DPRINTCONVERT("REG","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,data);
	DPRINT("[log:%x]%08x -> %08x [%s]\n",loglevel,offset,data,name);
	data &= ~bitifno;
	rrulib_reg_setval(offset,data,&name);
	DPRINTCONVERT("REG","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,data);
	DPRINT("[log:%x]%08x <- %08x [%s]\n",loglevel,offset,data,name);
	RETURN();
}

int BPF_HM_DEVC_REG_READ_BUFFER(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p){
	unsigned int tmp_offset=offset;
	unsigned int *tmp_data_p=data_p;
	unsigned int count;
	
	DPRINT("\n");
	for(count=0; count < (size/4) ; count++){
		BPF_HM_DEVC_REG_READ(loglevel,tmp_offset,tmp_data_p);
		tmp_data_p++;
		tmp_offset+=4 ;
	}
	RETURN();
}
int BPF_HM_DEVC_REG_WRITE_BUFFER(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_GPIO_READ(unsigned int loglevel, unsigned int offset, unsigned int *data_p)
{
	char* name;
	*data_p = rrulib_reg_getval(offset,&name);
	DPRINTCONVERT("GPIO","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,*data_p);
	DPRINT("[log:%x]%08x -> %08x [%s]\n",loglevel,offset,*data_p,name);
	RETURN();
}
int BPF_HM_DEVC_GPIO_WRITE(unsigned int loglevel, unsigned int offset, unsigned int *data_p)
{
	char* name;
	rrulib_reg_setval(offset,*data_p,&name);
	DPRINTCONVERT("GPIO","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,*data_p);
	DPRINT("[log:%x]%08x <- %08x [%s]\n",loglevel,offset,*data_p,name);
	RETURN();
}

int BPF_HM_DEVC_SETTIME(T_RRH_SYSTEM_TIME sysTime ){DPRINT("\n");RETURN();}

int BPF_HM_DEVC_GETTIME(T_RRH_SYSTEM_TIME* sysTime ){

	if( sysTime == NULL)
	{
		DPRINT("NULL detect \n");
		abort();
	}
	time_t cur_timer;
	struct tm *t_cur;
	time(&cur_timer);
	t_cur = localtime(&cur_timer);
	sysTime->year  = t_cur->tm_year + 1900;
	sysTime->month = t_cur->tm_mon + 1;
	sysTime->day   = t_cur->tm_mday;
	sysTime->hour  = t_cur->tm_hour;
	sysTime->min   = t_cur->tm_min;
	sysTime->sec   = t_cur->tm_sec;
	sysTime->msec  = 0;

	DPRINT("%04d/%02d/%02d %02d:%02d:%02d.%02d0 \n",
		sysTime->year,
		sysTime->month,
		sysTime->day  ,
		sysTime->hour  ,
		sysTime->min   ,
		sysTime->sec   ,
		sysTime->msec  
		);
	RETURN();
}

int BPF_HM_DEVC_ASSIGN(unsigned int assign_onoff){DPRINT("\n");RETURN();}

int BPF_HM_DEVC_PLD_READ(unsigned int offset, unsigned int *data_p){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_PLD_WRITE(unsigned int offset, unsigned int data){DPRINT("\n");RETURN();}
void BPF_HM_DEVC_SFP_ENDS(unsigned int num ,unsigned char* portno_p , unsigned int mode ){DPRINT("\n");return;}
int BPF_HM_DEVC_SFP_READ( unsigned int kind, unsigned short offset, unsigned char *data_p){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_SFP_WRITE( unsigned int kind, unsigned short offset, unsigned char *data_p){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_SFP_READ_BUFFER( unsigned int kind, unsigned short offset,  unsigned int count, unsigned char *data_p ){DPRINT("\n");RETURN();}


int BPF_HM_DEVC_PLL_READ(unsigned int offset, unsigned int  *data_p){
	char* name;
	*data_p = rrulib_reg_getval(offset,&name);
	DPRINTCONVERT("PLL","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,*data_p);
	DPRINT("%08x -> %08x [%s]\n",offset,*data_p,name);
	RETURN();
}
int BPF_HM_DEVC_PLL_WRITE(unsigned int offset, unsigned int data){
	char* name;
	rrulib_reg_setval(offset,data,&name);
	DPRINTCONVERT("PLL","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,data);
	DPRINT("%08x <- %08x [%s]\n",offset,data,name);
	RETURN();
}

int BPF_HM_DEVC_TEMP_READ(signed int *temp_p){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_TEMP2_READ(signed int *temp_p){DPRINT("\n");RETURN();}

int BPF_HM_DEVC_WDT_SET(unsigned int uiSec_AXI, unsigned int uiSec_SWDT){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER(unsigned int face,unsigned int offset,unsigned char *buffer){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_REG_READ_BIT(unsigned int loglevel, unsigned int offset, unsigned int check_bit, unsigned int *data_p){
	char* name;
	*data_p = rrulib_reg_getval(offset,&name);
	*data_p &= check_bit;
	DPRINTCONVERT("REG","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",offset,name,*data_p);
	DPRINT("[log:%x]%08x -> %08x [%s]\n",loglevel,offset,*data_p,name);
	RETURN();
}
void BPF_HM_DEVC_TEMP_INIT( void ){DPRINT("\n");return;}
int BPF_HM_DEVC_REG_READ_DBG(unsigned int addr, unsigned int *data_p){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_REG_WRITE_DBG(unsigned int addr, unsigned int data){DPRINT("\n");RETURN();}

int BPF_HM_DEVC_RFIC_READ( UINT txRx, UINT unit, UINT address, UINT *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(1 , 1 ,address, &name);
	DPRINTCONVERT("RFIC","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,*data_p);
	DPRINT("%08x.%08x.%08x -> %08x [%s]\n",txRx,unit,address,*data_p,name);
	RETURN();

}
int BPF_HM_DEVC_RFIC_WRITE( UINT txRx, UINT unit, UINT address, UINT data )
{
	char* name;
	rrulib_spi_setval(1 , 1 ,address,data, &name);
	DPRINTCONVERT("RFIC","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,data);
	DPRINT("%08x.%08x.%08x <- %08x [%s]\n",txRx,unit,address,data,name);
	RETURN();

}
int BPF_HM_DEVC_RFIC_RESET( UINT txRx, UINT unit ){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_EXT_PLL_READ (UINT address, UINT *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(1 , 0 ,address, &name);
	DPRINTCONVERT("JCPLL","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,*data_p);
	DPRINT("%08x -> %08x [%s]\n",address,*data_p,name);
	RETURN();

}
int BPF_HM_DEVC_EXT_PLL_WRITE (UINT address, UINT data)
{
	char* name;
	rrulib_spi_setval(1 , 0 ,address,data, &name);
	DPRINTCONVERT("JCPLL","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,data);
	DPRINT("%08x <- %08x [%s]\n",address,data,name);
	RETURN();

}
int BPF_HM_DEVC_PTP_READ(UINT address, UINT *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(0 , 0 ,address, &name);
	DPRINTCONVERT("PTP","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,*data_p);
	DPRINT("%08x -> %08x [%s]\n",address,*data_p,name);
	RETURN();

}

int BPF_HM_DEVC_PTP_WRITE(UINT address, UINT data)
{
	char* name;
	rrulib_spi_setval(0 , 0 ,address,data, &name);
	DPRINTCONVERT("PTP","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,data);
	DPRINT("%08x <- %08x [%s]\n",address,data,name);
	RETURN();

}

void BPF_HM_DEVC_SPI_INIT(){ return; }
int BPF_HM_DEVC_GETTIME_STR(char *timestring){DPRINT("\n");RETURN();}

int BPF_HM_DEVC_RFPLL_READ(unsigned int address, unsigned int *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(0 , 1 ,address, &name);
	DPRINTCONVERT("RFPLL","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,*data_p);
	DPRINT("%08x -> %08x [%s]\n",address,*data_p,name);
	RETURN();
}


int BPF_HM_DEVC_RFPLL_WRITE(unsigned int address, unsigned int data)
{
	char* name;
	rrulib_spi_setval(0 , 1 ,address,data, &name);
	DPRINTCONVERT("RFPLL","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,data);
	DPRINT("%08x <- %08x [%s]\n",address,data,name);
	RETURN();

}

int BPF_HM_DEVC_JCPLL_READ(unsigned int address, unsigned int *data_p){DPRINT("\n");RETURN();}
int BPF_HM_DEVC_JCPLL_WRITE(unsigned int address, unsigned int data){DPRINT("\n");RETURN();}

int BPF_HM_DEVC_TXDAC_READ(unsigned int address, unsigned int *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(2 , 0 ,address, &name);
	DPRINTCONVERT("TXDAC","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,*data_p);
	DPRINT("%08x -> %08x [%s]\n",address,*data_p,name);
	RETURN();
}
int BPF_HM_DEVC_TXDAC_WRITE(unsigned int address, unsigned int data)
{
	char* name;
	rrulib_spi_setval(2 , 0 ,address,data, &name);
	DPRINTCONVERT("TXDAC","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,data);
	DPRINT("%08x <- %08x [%s]\n",address,data,name);
	RETURN();
}



int BPF_HM_DEVC_RXADC_READ(unsigned int address, unsigned int *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(2 , 1 ,address, &name);
	DPRINTCONVERT("RXADC","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,*data_p);
	DPRINT("%08x -> %08x [%s]\n",address,*data_p,name);
	RETURN();
}

int BPF_HM_DEVC_RXADC_WRITE(unsigned int address, unsigned int data)
{
	char* name;
	rrulib_spi_setval(2 , 1 ,address,data, &name);
	DPRINTCONVERT("RXDAC","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,data);
	DPRINT("%08x <- %08x [%s]\n",address,data,name);
	RETURN();
}


int BPF_HM_DEVC_RFEEP_READ(unsigned int address, unsigned int *data_p)
{
	char* name;
	*data_p = (unsigned char)(rrulib_i2c_getval(2,2,address,&name));
	DPRINTCONVERT("RFEEP","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,*data_p);
	DPRINT("%08x -> %08x [%s]\n",address,*data_p,name);
	RETURN();	
}

int BPF_HM_DEVC_RFEEP_WRITE(unsigned int address, unsigned int data)
{
	char* name;
	rrulib_i2c_setval(2,2,address,data,&name);
	DPRINTCONVERT("RFEEP","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",address,name,data);
	DPRINT("%08x <- %08x [%s]\n",address,data,name);
	RETURN();
}

int BPF_HM_DEVC_PGC_READ(unsigned char grp, unsigned char sa_add, unsigned char srn, unsigned int *data_p)
{
	char* name;
	if((grp & 0x0F) != 0){
		*data_p = (unsigned char)(rrulib_i2c_getval(2,0 ,((((unsigned int)grp << 12) & 0xFF000) | ((sa_add << 4)& 0xFF0) | srn) ,&name));
	}else{
		*data_p = (unsigned char)(rrulib_i2c_getval(2,1 ,((((unsigned int)grp << 12) & 0xFF000) | ((sa_add << 4)& 0xFF0) | srn) ,&name));
	}
	DPRINTCONVERT("PGC","READ","grp-0x%08x sa_add-0x%02x,srn-0x%02x\t[%s]\tdt-0x%08x\n",grp,sa_add,srn,*data_p);
	DPRINT("%02x,%02%x,%02%x -> %08x [%s]\n",grp,sa_add,srn,*data_p,name);
	RETURN();
}

int BPF_HM_DEVC_PGC_WRITE(unsigned char grp, unsigned char sa_add, unsigned char srn, unsigned int data)
{
	char* name;
	if((grp & 0x0F) != 0){
		rrulib_i2c_setval(2,0 ,((((unsigned int)grp << 12) & 0xFF000) | ((sa_add << 4)& 0xFF0) | srn),data ,&name);
	}else{
		rrulib_i2c_setval(2,1 ,((((unsigned int)grp << 12) & 0xFF000) | ((sa_add << 4)& 0xFF0) | srn),data ,&name);
	}
	DPRINTCONVERT("PGC","WRITE","grp-0x%08x sa_add-0x%02x,srn-0x%02x\t[%s]\tdt-0x%08x\n",grp,sa_add,srn,data);
	DPRINT("%02x,%02%x,%02%x ,- %08x [%s]\n",grp,sa_add,srn,data,name);
	RETURN();
}

int BPF_HM_DEVC_PGCLUT_READ(unsigned char grp, unsigned char sa_add, unsigned char chrow, unsigned char address, unsigned int *data_p)
{
	DPRINT("grp:%x,sa_add:%x,chrow:%x,addr:%x -> %08x [PGCLUT]\n",grp,sa_add,chrow,address,*data_p);
	RETURN();
}

int BPF_HM_DEVC_PGCLUT_WRITE(unsigned char grp, unsigned char sa_add, unsigned char chrow, unsigned char address, unsigned int data)
{
	DPRINT("grp:%x,sa_add:%x,chrow:%x addr:%x <- %08x [PGCLUT]\n",grp,sa_add,chrow,address,data);
	RETURN();
}

int BPF_HM_DEVC_QMOD_READ(unsigned char ant, unsigned int sel, unsigned int address, unsigned int *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(3 , sel ,address, &name);
	DPRINTCONVERT("QMOD","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",((ant << 16)&0xF0000) | address,name,*data_p);
	DPRINT("%08x <- %08x [%s.%x]\n",address,*data_p,name,sel);
	RETURN();	
}

int BPF_HM_DEVC_QMOD_WRITE(unsigned char ant, unsigned int sel, unsigned int address, unsigned int data)
{
	char* name;
	rrulib_spi_setval(3 , sel ,address,data, &name);
	DPRINTCONVERT("QMOD","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",((ant << 16)&0xF0000) | address,name,data);
	DPRINT("%08x <- %08x [%s.%x]\n",address,data,name,sel);
	RETURN();	
}

int BPF_HM_DEVC_I2CSOFT_RESET(){DPRINT("\n");RETURN();}


int BPF_HM_DEVC_QDEM_READ(unsigned char ant, unsigned int sel, unsigned int address, unsigned int *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(3 , 2 ,address, &name);
	DPRINTCONVERT("QDEM","READ","ad-0x%08x\t[%s]\tdt-0x%08x\n",((ant << 16)&0xF0000) | address,name,*data_p);
	DPRINT("%08x -> %08x [%s.%x]\n",address,*data_p,name,sel);
	RETURN();	
}

int BPF_HM_DEVC_QDEM_WRITE(unsigned char ant, unsigned int sel, unsigned int address, unsigned int data)
{
	char* name;
	rrulib_spi_setval(3 , 2 ,address,data, &name);
	DPRINTCONVERT("QDEM","WRITE","ad-0x%08x\t[%s]\tdt-0x%08x\n",((ant << 16)&0xF0000) | address,name,data);
	DPRINT("%08x <- %08x [%s.%x]\n",address,data,name,sel);
	RETURN();	
}

int BPF_HM_DEVC_PAVGDAC_READ(unsigned char ant, unsigned int *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(3 , 4 ,ant , &name);
	DPRINT("%08x -> %08x [%s]\n",ant,*data_p,name);
	RETURN();
}

int BPF_HM_DEVC_PAVGDAC_WRITE(unsigned char ant, unsigned int data)
{
	char* name;
	rrulib_spi_setval(3 , 4 ,ant,data, &name);
	DPRINT("%08x <- %08x [%s]\n",ant,data,name);
	RETURN();
}

int BPF_HM_DEVC_TXVATTDAC_READ(unsigned char ant, unsigned int *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(3 , 7 ,ant, &name);
	DPRINT("%08x -> %08x [%s]\n",ant,*data_p,name);
	RETURN();
}
int BPF_HM_DEVC_TXVATTDAC_WRITE(unsigned char ant, unsigned int data)
{
	char* name;
	rrulib_spi_setval(3 , 7 ,ant,data, &name);
	DPRINT("%08x <- %08x [%s]\n",ant,data,name);
	RETURN();
}

int BPF_HM_DEVC_RXVATTDAC_READ(unsigned char ant, unsigned int *data_p)
{
	char* name;
	*data_p = rrulib_spi_getval(3 , 8 ,ant, &name);
	DPRINT("%08x -> %08x [%s]\n",ant,*data_p,name);
	RETURN();
}
int BPF_HM_DEVC_RXVATTDAC_WRITE(unsigned char ant, unsigned int data)
{
	char* name;

	rrulib_spi_setval(3 , 8 ,ant,data, &name);
	DPRINT("%08x <- %08x [%s]\n",ant,data,name);
	RETURN();
}


int BPF_HM_DEVC_FANDAC_WRITE(unsigned int data)
{
	RETURN();
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
	RETURN();
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
	RETURN();
}


int BPF_HM_DEVC_PGC_WRITE2(
	 unsigned char chain_grp,
	 unsigned int data1,
	 unsigned int data2,
	 unsigned int data3,
	 unsigned int data4
)
{
	RETURN();
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
	RETURN();
}


int BPF_HM_DEVC_PGCLUT_WRITE2(unsigned int data_len, unsigned char* data_p)
{
	RETURN();
}


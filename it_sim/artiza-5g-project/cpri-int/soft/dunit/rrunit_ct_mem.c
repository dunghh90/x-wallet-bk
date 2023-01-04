#include "rrunitlib.h"
#include "BPF_HM_DEVC.h"
#include "bpf_l_com.h"
#include "bpf_i_ru_ipcm_shm.h"

#define DPRINT(...) f_rrulib_dmsg("CORE", __func__ ,__VA_ARGS__ )

/* 本ファイルはレジスタを擬似するソースファイルです */

/**********************************************************************************************/
/* FPGA REG                                                                                   */
/**********************************************************************************************/
static unsigned char rrulib_reg_0[0x5000000];
static unsigned char rrulib_reg_1[0x2000000];
static unsigned char rrulib_reg_2[0x2000000];
static unsigned char rrulib_reg_3[0x0EFFFFF];

static t_rrulib_mem  rrulib_reg_mem[] = 
{
	{ 0xA0000000 , sizeof(rrulib_reg_0)       , rrulib_reg_0           , "hpm0"      ,4},
	{ 0xB0000000 , sizeof(rrulib_reg_1)       , rrulib_reg_1           , "trx1"      ,4},
	{ 0xB2000000 , sizeof(rrulib_reg_2)       , rrulib_reg_2           , "trx2"      ,4},
	{ 0xFF000000 , sizeof(rrulib_reg_3)       , rrulib_reg_3           , "cpu"       ,4},
	{ 0xffffffff , 0                          , NULL                   , "UNKNOWN"   ,0}
};

void rrulib_reg_init(unsigned int val)
{
	unsigned int count;
	unsigned int count2;
	unsigned int* val_p;

	for(count=0;count < (sizeof(rrulib_reg_mem)/sizeof(rrulib_reg_mem[0]));count++)
	{
		if(rrulib_reg_mem[count].addr == (void*)-1)
		{
			break;
		}
		
		val_p = (unsigned int*)(rrulib_reg_mem[count].addr);
		for(count2=0;count2 < (rrulib_reg_mem[count].size/rrulib_reg_mem[count].access) ;count2++,val_p++)
		{
			*(val_p) = val;
		}
	}
	return;
}

unsigned int rrulib_reg_getval(unsigned int addr , char** name)
{
	unsigned int count;
	unsigned int val = 0;

	for(count=0;count < (sizeof(rrulib_reg_mem)/sizeof(rrulib_reg_mem[0]));count++)
	{
		if(rrulib_reg_mem[count].addr == (void*)-1)
		{
			break;
		}
		if(
		   (addr >= rrulib_reg_mem[count].top ) && 
		   (addr < rrulib_reg_mem[count].top + rrulib_reg_mem[count].size)
		)
		{
			val = *(unsigned int*)(rrulib_reg_mem[count].addr + (addr - rrulib_reg_mem[count].top));
			*name = rrulib_reg_mem[count].name;
			break;
		}
	}
	if(count >= (sizeof(rrulib_reg_mem)/sizeof(rrulib_reg_mem[0])))
	{
		DPRINT("ERROR addr %x\n",addr);
		*name = rrulib_reg_mem[count].name;
	}
	return(val);
}

void rrulib_reg_setval(unsigned int addr , unsigned int val , char** name)
{
	unsigned int count;
	for(count=0;count < (sizeof(rrulib_reg_mem)/sizeof(rrulib_reg_mem[0]));count++)
	{
		if(rrulib_reg_mem[count].addr == (void*)-1)
		{
			break;
		}

		if(
		   (addr >= rrulib_reg_mem[count].top ) && 
		   (addr < rrulib_reg_mem[count].top + rrulib_reg_mem[count].size)
		)
		{
			*(unsigned int*)(rrulib_reg_mem[count].addr + (addr - rrulib_reg_mem[count].top)) = val;
			*name = rrulib_reg_mem[count].name;
			break;
		}
	}
	if(count >= (sizeof(rrulib_reg_mem)/sizeof(rrulib_reg_mem[0])))
	{
		DPRINT("ERROR addr %x\n",addr);
		*name = rrulib_reg_mem[count].name;
	}
	return;
}

/**********************************************************************************************/
/* I2C                                                                                        */
/**********************************************************************************************/
static unsigned char rrulib_i2c_eep[0x400];
static unsigned char rrulib_i2c_temp[0x2];

static t_rrulib_mem  rrulib_i2c0_mem[] = 
{
	{ 0          , sizeof(rrulib_i2c_eep)        , rrulib_i2c_eep             , "eep"     ,1},
	{ 0          , sizeof(rrulib_i2c_temp)       , rrulib_i2c_temp            , "temp"    ,2},
	{ 0          , 0                             , NULL                       , "UNKNOWN" ,0}
};

static unsigned char rrulib_i2c_sfp0_a0[0x100];
static unsigned char rrulib_i2c_sfp0_a2[0x100];
static unsigned char rrulib_i2c_sfp1_a0[0x100];
static unsigned char rrulib_i2c_sfp1_a2[0x100];
static t_rrulib_mem  rrulib_i2c1_mem[] = 
{
	{ BPF_HM_DEVC_SFP_A0 , sizeof(rrulib_i2c_sfp0_a0)   ,rrulib_i2c_sfp0_a0   , "sfp0_a0"  , 1},
	{ BPF_HM_DEVC_SFP_A2 , sizeof(rrulib_i2c_sfp0_a2)   ,rrulib_i2c_sfp0_a2   , "sfp0_a2"  , 1},
	{ BPF_HM_DEVC_SFP_SA0 , sizeof(rrulib_i2c_sfp1_a0)   ,rrulib_i2c_sfp1_a0   , "sfp1_a0"  , 1},
	{ BPF_HM_DEVC_SFP_SA2 , sizeof(rrulib_i2c_sfp1_a2)   ,rrulib_i2c_sfp1_a2   , "sfp1_a2"  , 1},
	{ 0                           , 0                            , NULL                , "UNKNOWN"  , 0}
};


static unsigned char rrulib_i2c_pgc_v[0x100000];
static unsigned char rrulib_i2c_pgc_h[0x100000];
static unsigned char rrulib_i2c_rfeep[0x1000];



static t_rrulib_mem  rrulib_i2c2_mem[] = 
{
	{ 0 , sizeof(rrulib_i2c_pgc_v)   ,rrulib_i2c_pgc_v   , "pgc_v"  , 1},
	{ 1 , sizeof(rrulib_i2c_pgc_h)   ,rrulib_i2c_pgc_h   , "pgc_h"  , 1},
	{ 2 , sizeof(rrulib_i2c_rfeep)   ,rrulib_i2c_rfeep   , "rfeep"  , 1},
	{ 0                           , 0                            , NULL                , "UNKNOWN"  , 0}
};



void rrulib_i2c_init(unsigned int val)
{
	unsigned int count;
	unsigned int count2;
	unsigned int count3;
	unsigned char* val_p;
	unsigned short* sval_p;
	unsigned int maxcount;
	t_rrulib_mem*  memtbl_p;

	for(count3=0;count3 < 3 ;count3++)
	{
		if(count3 == 0){
			maxcount = sizeof(rrulib_i2c0_mem)/sizeof(rrulib_i2c0_mem[0]);
			memtbl_p = rrulib_i2c0_mem;
		}
		else if(count3 == 1){
			maxcount = sizeof(rrulib_i2c1_mem)/sizeof(rrulib_i2c1_mem[0]);
			memtbl_p = rrulib_i2c1_mem;
		}
		else{
			maxcount = sizeof(rrulib_i2c2_mem)/sizeof(rrulib_i2c2_mem[0]);
			memtbl_p = rrulib_i2c2_mem;
		}
		for(count=0;count < maxcount ;count++)
		{
			if(memtbl_p[count].addr == NULL)
			{
				break;
			}
			switch(memtbl_p[count].access)
			{
				case 1:
					val_p = (unsigned char*)memtbl_p[count].addr;
					for(count2=0;count2 < (memtbl_p[count].size/memtbl_p[count].access) ;count2++,val_p++)
					{
						*(val_p) = (unsigned char)val;
					}
					break;
				case 2:
					sval_p = (unsigned short*)memtbl_p[count].addr;
					for(count2=0;count2 < (memtbl_p[count].size/memtbl_p[count].access) ;count2++,sval_p++)
					{
						*(sval_p) = (unsigned short)val;
					}
					break;
				default:
					break;
			}
		}
	}
	return;
}

unsigned int rrulib_i2c_getval(unsigned int kind , unsigned int subkind , unsigned int addr, char**name)
{
	unsigned int count;
	unsigned int val = 0;
	unsigned int maxcount;
	t_rrulib_mem*  memtbl_p;
	
	switch(kind){
		case 0:
			maxcount = sizeof(rrulib_i2c0_mem)/sizeof(rrulib_i2c0_mem[0]);
			memtbl_p = rrulib_i2c0_mem;
			break;
		case 1:
			maxcount = sizeof(rrulib_i2c1_mem)/sizeof(rrulib_i2c1_mem[0]);
			memtbl_p = rrulib_i2c1_mem;
			break;
		case 2:
			maxcount = sizeof(rrulib_i2c2_mem)/sizeof(rrulib_i2c2_mem[0]);
			memtbl_p = rrulib_i2c2_mem;
			break;
		default:
			DPRINT("ERROR Kind %x\n",kind);
			return(0);
			break;
	}
	
	for(count=0;count < maxcount ;count++)
	{
		if(memtbl_p[count].addr == NULL)
		{
			break;
		}		
		if( subkind ==  memtbl_p[count].top )
		{
			switch(memtbl_p[count].access)
			{
				case 1:
					val = (unsigned int)*(unsigned char*)(memtbl_p[count].addr + addr );
					break;
				case 2:
					val = (unsigned int)*(unsigned short*)(memtbl_p[count].addr + addr );
					break;
				default:
					break;
			}
			*name= memtbl_p[count].name;
			break;
		}
	}
	if(count >= maxcount)
	{
		DPRINT("ERROR Kind %x\n",kind);
		*name = memtbl_p[count].name;
	}
	return(val);
}

void rrulib_i2c_setval(unsigned int kind , unsigned int subkind , unsigned int addr , unsigned int val , char** name)
{
	unsigned int count;
	unsigned int maxcount;
	t_rrulib_mem*  memtbl_p;
	
	switch(kind){
		case 0:
			maxcount = sizeof(rrulib_i2c0_mem)/sizeof(rrulib_i2c0_mem[0]);
			memtbl_p = rrulib_i2c0_mem;
			break;
		case 1:
			maxcount = sizeof(rrulib_i2c1_mem)/sizeof(rrulib_i2c1_mem[0]);
			memtbl_p = rrulib_i2c1_mem;
			break;
		case 2:
			maxcount = sizeof(rrulib_i2c2_mem)/sizeof(rrulib_i2c2_mem[0]);
			memtbl_p = rrulib_i2c2_mem;
			break;
		default:
			DPRINT("ERROR Kind %x\n",kind);
			return;
			break;
	}

	for(count=0;count < maxcount ;count++)
	{
		if(memtbl_p[count].addr == NULL)
		{
			break;
		}
		if(subkind  ==  memtbl_p[count].top )
		{
			switch(memtbl_p[count].access)
			{
				case 1:
					*(unsigned char*)(memtbl_p[count].addr + addr ) = (unsigned char)val;
					break;
				case 2:
					*(unsigned short*)(memtbl_p[count].addr + addr ) = (unsigned short)val;
					break;
				default:
					*(unsigned int*)(memtbl_p[count].addr + addr ) = val;
					break;
			}
			*(unsigned int*)(memtbl_p[count].addr + addr ) = val;
			*name= memtbl_p[count].name;
			break;
		}
	}
	if(count >= maxcount)
	{
		DPRINT("ERROR Kind %x\n",kind);
		*name = memtbl_p[count].name;
	}
	return;
}

/**********************************************************************************************/
/* QSPI                                                                                       */
/**********************************************************************************************/
static unsigned char rrulib_qspi[2][128*1024*1024];
static unsigned char rrulib_qspi2[1][128*1024*1024];

static t_rrulib_mem  rrulib_qspi_mem[] = 
{
	{ 0          , sizeof(rrulib_qspi[0])        , rrulib_qspi[0]             , "qspi_0"  ,4},
	{ 1          , sizeof(rrulib_qspi[1])        , rrulib_qspi[1]             , "qspi_1"  ,4},
	{ 2          , sizeof(rrulib_qspi2[1])       , rrulib_qspi2[1]            , "qspi_pl"  ,4},
	{ 0          , 0                             , NULL                       , "UNKNOWN" ,0}
};

void rrulib_qspi_get(unsigned int face , unsigned int addr , unsigned int size, unsigned char* buf , char** name)
{
	unsigned int count;
	for(count=0;count < (sizeof(rrulib_qspi_mem)/sizeof(rrulib_qspi_mem[0]));count++)
	{
		if(rrulib_qspi_mem[count].addr  == NULL )
		{
			continue;
		}		
		if(rrulib_qspi_mem[count].top  != face )
		{
			continue;
		}
		if( (rrulib_qspi_mem[count].addr + addr + size) > (rrulib_qspi_mem[count].addr + rrulib_qspi_mem[count].size))
		{
			DPRINT("ERROR addr %x\n",addr);
			*name = rrulib_qspi_mem[count].name;
			return;
		}
		memcpy(buf,rrulib_qspi_mem[count].addr + addr , size);
		*name = rrulib_qspi_mem[count].name;
		break;
	}
	if(count >= (sizeof(rrulib_qspi_mem)/sizeof(rrulib_qspi_mem[0])))
	{
		DPRINT("ERROR addr %x\n",addr);
		*name = rrulib_qspi_mem[count].name;
	}
	return;
}

void rrulib_qspi_set(unsigned int face , unsigned int addr , unsigned int size, unsigned char* buf , char** name)
{
	unsigned int count;
	for(count=0;count < (sizeof(rrulib_qspi_mem)/sizeof(rrulib_qspi_mem[0]));count++)
	{
		if(rrulib_qspi_mem[count].addr  == NULL )
		{
			break;
		}
		if(rrulib_qspi_mem[count].top  != face )
		{
			continue;
		}
		if( (rrulib_qspi_mem[count].addr + addr + size) > (rrulib_qspi_mem[count].addr + rrulib_qspi_mem[count].size))
		{
			DPRINT("ERROR addr %x\n",addr);
			*name = rrulib_qspi_mem[count].name;
			return;
		}
		memcpy(rrulib_qspi_mem[count].addr + addr , buf , size);
		*name = rrulib_qspi_mem[count].name;
		break;
	}
	if(count >= (sizeof(rrulib_qspi_mem)/sizeof(rrulib_qspi_mem[0])))
	{
		DPRINT("ERROR addr %x\n",addr);
		*name = rrulib_qspi_mem[count].name;
	}
	return;
}

void rrulib_qspi_init(unsigned int val)
{
	unsigned int count;
	for(count=0;count < (sizeof(rrulib_qspi_mem)/sizeof(rrulib_qspi_mem[0]));count++)
	{
		if(rrulib_qspi_mem[count].addr  == NULL )
		{
			break;
		}
		memset(rrulib_qspi_mem[count].addr , (unsigned char)val , rrulib_qspi_mem[count].size);
	}
	return;
}

int rrulib_qspi_fileinit(unsigned int face , unsigned int ofs ,unsigned int size ,char* filename)
{
	FILE*	fp;
	long	fsize;
	void*	buf;
	char*	name;
	
	if(( fp = fopen(filename,"rb") ) == NULL)
	{
		return(-1);
	}
	fseek(fp, 0L, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	if(size > fsize )
	{
		size = fsize;
	}
	if(( buf = malloc(size) ) != NULL )
	{
		fread(buf,1,size,fp);
		rrulib_qspi_set(face,ofs,size,buf,&name);
		free(buf);
	}
	fclose(fp);
	return(fsize);
}


/**********************************************************************************************/
/* SPI                                                                                        */
/**********************************************************************************************/
static unsigned char rrulib_spi0_dev0[0x1000];
static unsigned char rrulib_spi0_dev1[0x1000];
static unsigned char rrulib_spi0_dev2[0x1000];
static unsigned char rrulib_spi1_dev0[0x1000];
static unsigned char rrulib_spi1_dev1[0x1000];
static unsigned char rrulib_spi2_dev0[0x1000];
static unsigned char rrulib_spi2_dev1[0x1000];
static unsigned char rrulib_spi3_dev0[0x100000];
static unsigned char rrulib_spi3_dev1[0x100000];
static unsigned char rrulib_spi3_dev2[0x100000];
static unsigned char rrulib_spi3_dev3[0x1000];
static unsigned char rrulib_spi3_dev4[0x1000];
static unsigned char rrulib_spi3_dev5[0x1000];
static unsigned char rrulib_spi3_dev6[0x1000];
static unsigned char rrulib_spi3_dev7[0x1000];
static unsigned char rrulib_spi3_dev8[0x1000];


static t_rrulib_mem  rrulib_spi0_mem[] = 
{
	{ 0                                       ,  sizeof(rrulib_spi0_dev0[0])   , rrulib_spi0_dev0         , "ptppll"  ,1},
	{ 1                                       ,  sizeof(rrulib_spi0_dev1[0])   , rrulib_spi0_dev1         , "rficpll"  ,1},
	{ 2                                       ,  sizeof(rrulib_spi0_dev1[0])   , rrulib_spi0_dev1         , "rficpll_r"  ,1},
	{ 0                                       , 0                              , NULL                     , "UNKNOWN" ,0}
};


static t_rrulib_mem  rrulib_spi1_mem[] = 
{
	{ 0                                       ,  sizeof(rrulib_spi1_dev0[0])   , rrulib_spi1_dev0         , "jc-pll"  ,1},
	{ 1                                       ,  sizeof(rrulib_spi1_dev1[0])   , rrulib_spi1_dev1         , "rfic"  ,1},
	{ 0                                       , 0                              , NULL                       , "UNKNOWN" ,0}
};

static t_rrulib_mem  rrulib_spi2_mem[] = 
{
	{ 0                                       ,  sizeof(rrulib_spi2_dev0[0])   , rrulib_spi2_dev0         , "tx-dac"  ,1},
	{ 1                                       ,  sizeof(rrulib_spi2_dev1[0])   , rrulib_spi2_dev1         , "rx-adc"  ,1},
	{ 0                                       , 0                              , NULL                       , "UNKNOWN" ,0}
};

static t_rrulib_mem  rrulib_spi3_mem[] = 
{
	{ 0                                       ,  sizeof(rrulib_spi3_dev0[0])   , rrulib_spi3_dev0         , "qmod_txmix"  ,1},
	{ 1                                       ,  sizeof(rrulib_spi3_dev1[0])   , rrulib_spi3_dev1         , "qmod_txvga"  ,1},
	{ 2                                       ,  sizeof(rrulib_spi3_dev2[0])   , rrulib_spi3_dev2         , "qdem"        ,1},
	{ 3                                       ,  sizeof(rrulib_spi3_dev3[0])   , rrulib_spi3_dev3         , "qmod_dac"    ,1},
	{ 4                                       ,  sizeof(rrulib_spi3_dev4[0])   , rrulib_spi3_dev4         , "pavg"  ,1},
	{ 5                                       ,  sizeof(rrulib_spi3_dev5[0])   , rrulib_spi3_dev5         , "txvatt"  ,1},
	{ 6                                       ,  sizeof(rrulib_spi3_dev6[0])   , rrulib_spi3_dev6         , "pavg2"  ,1},
	{ 7                                       ,  sizeof(rrulib_spi3_dev7[0])   , rrulib_spi3_dev7         , "txvatt_dac"  ,1},
	{ 8                                       ,  sizeof(rrulib_spi3_dev8[0])   , rrulib_spi3_dev8         , "rxvatt_adc"  ,1},
	{ 0                                       , 0                              , NULL                       , "UNKNOWN" ,0}
};


void rrulib_spi_init(unsigned int val)
{
	unsigned int count;
	unsigned int count2;
	unsigned int count3;
	unsigned char* val_p;
	unsigned short* sval_p;
	unsigned int maxcount;
	t_rrulib_mem*  memtbl_p;

	for(count3=0;count3 < 4 ;count3++)
	{
		switch(count3){
			case 0:
				maxcount = sizeof(rrulib_spi0_mem)/sizeof(rrulib_spi0_mem[0]);
				memtbl_p = rrulib_spi0_mem;
				break;
			case 1:
				maxcount = sizeof(rrulib_spi1_mem)/sizeof(rrulib_spi1_mem[0]);
				memtbl_p = rrulib_spi1_mem;
				break;
			case 2:
				maxcount = sizeof(rrulib_spi2_mem)/sizeof(rrulib_spi2_mem[0]);
				memtbl_p = rrulib_spi2_mem;
				break;
			case 3:
				maxcount = sizeof(rrulib_spi3_mem)/sizeof(rrulib_spi3_mem[0]);
				memtbl_p = rrulib_spi3_mem;
				break;
			default:
				continue;
				break;
		}
		for(count=0;count < maxcount ;count++)
		{
			if(memtbl_p[count].addr == NULL)
			{
				break;
			}
			switch(memtbl_p[count].access)
			{
				case 1:
					val_p = (unsigned char*)memtbl_p[count].addr;
					for(count2=0;count2 < (memtbl_p[count].size/memtbl_p[count].access) ;count2++,val_p++)
					{
						*(val_p) = (unsigned char)val;
					}
					break;
				case 2:
					sval_p = (unsigned short*)memtbl_p[count].addr;
					for(count2=0;count2 < (memtbl_p[count].size/memtbl_p[count].access) ;count2++,sval_p++)
					{
						*(sval_p) = (unsigned short)val;
					}
					break;
				default:
					break;
			}
		}
	}
	
	return;
}


unsigned int rrulib_spi_getval(unsigned int kind , unsigned int subkind ,unsigned int addr, char**name)
{

	unsigned int count;
	unsigned int val = 0;
	unsigned int maxcount;
	t_rrulib_mem*  memtbl_p;
	
	switch(kind){
		case 0:
			maxcount = sizeof(rrulib_spi0_mem)/sizeof(rrulib_spi0_mem[0]);
			memtbl_p = rrulib_spi0_mem;
			break;
		case 1:
			maxcount = sizeof(rrulib_spi1_mem)/sizeof(rrulib_spi1_mem[0]);
			memtbl_p = rrulib_spi1_mem;
			break;
		case 2:
			maxcount = sizeof(rrulib_spi2_mem)/sizeof(rrulib_spi2_mem[0]);
			memtbl_p = rrulib_spi2_mem;
			break;
		case 3:
			maxcount = sizeof(rrulib_spi3_mem)/sizeof(rrulib_spi3_mem[0]);
			memtbl_p = rrulib_spi2_mem;
			break;
		default:
			DPRINT("ERROR Kind %x\n",kind);
			return(0);
			break;
	}
	
	for(count=0;count < maxcount ;count++)
	{
		if(memtbl_p[count].addr == NULL)
		{
			break;
		}		
		if( subkind ==  memtbl_p[count].top )
		{
			switch(memtbl_p[count].access)
			{
				case 1:
					val = (unsigned int)*(unsigned char*)(memtbl_p[count].addr + addr );
					break;
				case 2:
					val = (unsigned int)*(unsigned short*)(memtbl_p[count].addr + addr );
					break;
				default:
					break;
			}
			*name= memtbl_p[count].name;
			break;
		}
	}
	if(count >= maxcount)
	{
		DPRINT("ERROR Kind %x\n",kind);
		*name = memtbl_p[count].name;
	}
	return(val);
}

void rrulib_spi_setval(unsigned int kind , unsigned int subkind ,unsigned int addr , unsigned int val , char** name)
{
	unsigned int count;
	unsigned int maxcount;
	t_rrulib_mem*  memtbl_p;
	
	switch(kind){
		case 0:
			maxcount = sizeof(rrulib_spi0_mem)/sizeof(rrulib_spi0_mem[0]);
			memtbl_p = rrulib_spi0_mem;
			break;
		case 1:
			maxcount = sizeof(rrulib_spi1_mem)/sizeof(rrulib_spi1_mem[0]);
			memtbl_p = rrulib_spi1_mem;
			break;
		case 2:
			maxcount = sizeof(rrulib_spi2_mem)/sizeof(rrulib_spi2_mem[0]);
			memtbl_p = rrulib_spi2_mem;
			break;
		case 3:
			maxcount = sizeof(rrulib_spi3_mem)/sizeof(rrulib_spi3_mem[0]);
			memtbl_p = rrulib_spi3_mem;
			break;
		default:
			DPRINT("ERROR Kind %x\n",kind);
			return;
			break;
	}

	for(count=0;count < maxcount ;count++)
	{
		if(memtbl_p[count].addr == NULL)
		{
			break;
		}
		if(subkind  ==  memtbl_p[count].top )
		{
			switch(memtbl_p[count].access)
			{
				case 1:
					*(unsigned char*)(memtbl_p[count].addr + addr ) = (unsigned char)val;
					break;
				case 2:
					*(unsigned short*)(memtbl_p[count].addr + addr ) = (unsigned short)val;
					break;
				default:
					*(unsigned int*)(memtbl_p[count].addr + addr ) = val;
					break;
			}
			*(unsigned int*)(memtbl_p[count].addr + addr ) = val;
			*name= memtbl_p[count].name;
			break;
		}
	}
	if(count >= maxcount)
	{
		DPRINT("ERROR Kind %x\n",kind);
		*name = memtbl_p[count].name;
	}
	return;
}


/**********************************************************************************************/
/* Shared Mem                                                                                 */
/**********************************************************************************************/
static unsigned char rrulib_shm_01[sizeof(struct ipcm_shm_create_data_t)];
static unsigned char rrulib_shm_02[sizeof(BPF_MNG_COM_MEMORY) * E_BPF_RU_IPCM_SHMKIND_MAX];
static unsigned char rrulib_shm_03[sizeof(T_RRH_LOG_LEVEL_MNG)];
static unsigned char rrulib_shm_04[0x00001000];
static unsigned char rrulib_shm_05[sizeof(T_RRH_PID_MNG)];
static unsigned char rrulib_shm_06[sizeof(T_RRH_TID_MNG)];
static unsigned char rrulib_shm_07[sizeof(T_RRH_LOG_RST_REASON)];
static unsigned char rrulib_shm_08[sizeof(T_RRH_LOG_ASSERT)];
static unsigned char rrulib_shm_09[sizeof(T_RRH_ALARM_HISTORYMNG)];
static unsigned char rrulib_shm_10[sizeof(T_RRH_LOG_BUFF_HIS)];
static unsigned char rrulib_shm_11[sizeof(T_RRH_LOG_TIMER_HIS)];
static unsigned char rrulib_shm_12[sizeof(T_RRH_LOG_SEM_HIS)];
static unsigned char rrulib_shm_13[0x00146000];
static unsigned char rrulib_shm_14[0x00002000];
static unsigned char rrulib_shm_15[sizeof(T_RRH_TRAINVTBL)];
static unsigned char rrulib_shm_16[0x00000004];
static unsigned char rrulib_shm_17[sizeof(T_RRH_ALM_LOG_FOR_HIST)];
static unsigned char rrulib_shm_18[sizeof(T_RRH_LOG_TEMP)];
static unsigned char rrulib_shm_19[sizeof(T_RRH_SFPINFO_HIS)];
static unsigned char rrulib_shm_20[sizeof(T_RRH_SFPINFO_HIS)];
static unsigned char rrulib_shm_21[0x00001000];
static unsigned char rrulib_shm_22[0x00001000];
static unsigned char rrulib_shm_23[0x00001000];
static unsigned char rrulib_shm_24[0x00001000];
static unsigned char rrulib_shm_25[0x00009000];
static unsigned char rrulib_shm_26[sizeof(T_RRH_PTPCONF_MNG)];
static unsigned char rrulib_shm_27[sizeof(T_RRH_LOG_PTP_STATUS)];
static unsigned char rrulib_shm_28[sizeof(T_RRH_PTP_APR_PARM)];
static unsigned char rrulib_shm_29[sizeof(T_RRH_LOG_UPSIGSTOP_HIS)];
static unsigned char rrulib_shm_30[sizeof(T_RRH_LOG_FH_L1_HIS)];
static unsigned char rrulib_shm_31[sizeof(T_RRH_SFNFRM_CALB_TBL)];
static unsigned char rrulib_shm_32[sizeof(T_RRH_TXPOW_DETECT_TBL)];
static unsigned char rrulib_shm_33[sizeof(T_RRH_TXPOW_DETECT_LOG)];
static unsigned char rrulib_shm_34[sizeof(T_RRH_LUT_LOG)];
static unsigned char rrulib_shm_35[sizeof(T_RRH_LOG_FHSTAT_HIS)];
static unsigned char rrulib_shm_36[sizeof(T_RRH_LOG_ETH_ERR_HIS)];
static unsigned char rrulib_shm_37[sizeof(T_RRH_LOG_ETH_ERR_HIS)];
static unsigned char rrulib_shm_38[0x00001000];
static unsigned char rrulib_shm_39[sizeof(T_RRH_SVSTATECHG_HIS)];
static unsigned char rrulib_shm_40[sizeof(T_RRH_TRAINING_DATA)];
static unsigned char rrulib_shm_41[sizeof(T_RRH_DU_INFORMATION_TBL)];
static unsigned char rrulib_shm_42[0x00002000];
static unsigned char rrulib_shm_43[sizeof(T_RRH_DU_PROCESS_INFO)];
static unsigned char rrulib_shm_44[sizeof(T_RRH_RF_TRAINING_DATA)];
static unsigned char rrulib_shm_45[sizeof(T_RRH_DU_CMD_INFO)];
static unsigned char rrulib_shm_46[sizeof(T_RRH_DU_FAULT_MNG)];
static unsigned char rrulib_shm_47[ BPF_MESSAGE_SIZE_S ];
static unsigned char rrulib_shm_48[ BPF_MESSAGE_SIZE_M ];
static unsigned char rrulib_shm_49[ BPF_MESSAGE_SIZE_L ];
static unsigned char rrulib_shm_50[ BPF_MESSAGE_SIZE_S ];
static unsigned char rrulib_shm_51[ BPF_MESSAGE_SIZE_M ];
static unsigned char rrulib_shm_52[ BPF_MESSAGE_SIZE_L ];
static unsigned char rrulib_shm_53[ BPF_MESSAGE_SIZE_L ];

static unsigned char rrulib_shm_54[	sizeof(T_RRH_NETCONF_INFO_TBL)			];
static unsigned char rrulib_shm_55[	sizeof(T_RRH_CAL_COEF_TBL)				];
static unsigned char rrulib_shm_56[	sizeof(T_RRH_CAL_COEF_TBL)				];
static unsigned char rrulib_shm_57[	sizeof(T_RRH_PFM_PERFORMANCE_TBL)		];
static unsigned char rrulib_shm_58[	sizeof(T_RRH_PFM_PERFORMANCE_MNG_TBL)	];
static unsigned char rrulib_shm_59[	sizeof(T_RRH_LBM_MNGMENT_TBL)			];
static unsigned char rrulib_shm_60[	sizeof(T_RRH_DU_EIO_SVCTRL_MNG)			];
static unsigned char rrulib_shm_61[	sizeof(T_RRH_DU_EIO_SVCTRL_LOG)			];
static unsigned char rrulib_shm_62[	sizeof(T_RRH_DU_CMD_INFO)				];
static unsigned char rrulib_shm_63[	sizeof(T_RRH_MT_INFO)					];
static unsigned char rrulib_shm_64[	sizeof(T_RRH_BF_MODE_TBL)				];

static t_rrulib_mem  rrulib_shm_mem[E_RRH_SHMID_APL_MAX + 7 +1] = 
{

	{	E_RRH_SHMID_BPF_MNG,						sizeof(rrulib_shm_01)             , rrulib_shm_01             , "E_RRH_SHMID_BPF_MNG "  ,0},
	{	E_RRH_SHMID_MNG_COM_MEMORY,					sizeof(rrulib_shm_02)             , rrulib_shm_02             , "E_RRH_SHMID_MNG_COM_MEMORY"  ,0},	
	{	E_RRH_SHMID_LOGLEVEL_MNG,					sizeof(rrulib_shm_03)             , rrulib_shm_03             , "E_RRH_SHMID_LOGLEVEL_MNG"  ,0},	
	{	E_RRH_SHMID_TIMER_MNG,						sizeof(rrulib_shm_04)             , rrulib_shm_04             , "E_RRH_SHMID_TIMER_MNG"  ,0},	
	{	E_RRH_SHMID_APL_PROC_ID_MNG_INFO,			sizeof(rrulib_shm_05)             , rrulib_shm_05             , "E_RRH_SHMID_APL_PROC_ID_MNG_INFO"  ,0},	
	{	E_RRH_SHMID_APL_THRD_ID_MNG_INFO,			sizeof(rrulib_shm_06)             , rrulib_shm_06             , "E_RRH_SHMID_APL_THRD_ID_MNG_INFO"  ,0},	
	{	E_RRH_SHMID_APL_RST_REASON_LOG,				sizeof(rrulib_shm_07)             , rrulib_shm_07             , "E_RRH_SHMID_APL_RST_REASON_LOG"  ,0},
	{	E_RRH_SHMID_APL_ASSERT_LOG,					sizeof(rrulib_shm_08)             , rrulib_shm_08             , "E_RRH_SHMID_APL_ASSERT_LOG"  ,0},	
	{	E_RRH_SHMID_APL_ALM_HISTORY_LOG,			sizeof(rrulib_shm_09)             , rrulib_shm_09             , "E_RRH_SHMID_APL_ALM_HISTORY_LOG"  ,0},
	{	E_RRH_SHMID_APL_BUFFER_HISTORY,				sizeof(rrulib_shm_10)             , rrulib_shm_10             , "E_RRH_SHMID_APL_BUFFER_HISTORY"  ,0},
	{	E_RRH_SHMID_APL_TIMER_HISTORY,				sizeof(rrulib_shm_11)             , rrulib_shm_11             , "E_RRH_SHMID_APL_TIMER_HISTORY"  ,0},
	{	E_RRH_SHMID_APL_SEMAPHORE_HISTORY,			sizeof(rrulib_shm_12)             , rrulib_shm_12             , "E_RRH_SHMID_APL_SEMAPHORE_HISTORY"  ,0},
	{	E_RRH_SHMID_APL_DPDA_ALM_LOG,				sizeof(rrulib_shm_13)             , rrulib_shm_13             , "E_RRH_SHMID_APL_DPDA_ALM_LOG"  ,0},
	{	E_RRH_SHMID_APL_DPDA_MNG_TBL,				sizeof(rrulib_shm_14)             , rrulib_shm_14             , "E_RRH_SHMID_APL_DPDA_MNG_TBL"  ,0},
	{	E_RRH_SHMID_APL_INVENTORY,					sizeof(rrulib_shm_15)             , rrulib_shm_15             , "E_RRH_SHMID_APL_INVENTORY"  ,0},
	{	E_RRH_SHMID_APL_RRH_KIND,					sizeof(rrulib_shm_16)             , rrulib_shm_16             , "E_RRH_SHMID_APL_RRH_KIND"  ,0},
	{	E_RRH_SHMID_APL_ALM_HIS_PREPARE,			sizeof(rrulib_shm_17)             , rrulib_shm_17             , "E_RRH_SHMID_APL_ALM_HIS_PREPARE"  ,0},
	{	E_RRH_SHMID_APL_TEMP_LOG_TBL,				sizeof(rrulib_shm_18)             , rrulib_shm_18             , "E_RRH_SHMID_APL_TEMP_LOG_TBL"  ,0},
	{	E_RRH_SHMID_APL_SFP0_MNG_INFO_HIS,			sizeof(rrulib_shm_19)             , rrulib_shm_19             , "E_RRH_SHMID_APL_SFP0_MNG_INFO_HIS"  ,0},
	{	E_RRH_SHMID_APL_SFP1_MNG_INFO_HIS,			sizeof(rrulib_shm_20)             , rrulib_shm_20             , "E_RRH_SHMID_APL_SFP1_MNG_INFO_HIS"  ,0},
	{	E_RRH_SHMID_APL_LED_HIS,					sizeof(rrulib_shm_21)             , rrulib_shm_21             , "E_RRH_SHMID_APL_LED_HIS"  ,0},
	{	E_RRH_SHMID_APL_ALM_NTC_HIS,				sizeof(rrulib_shm_22)             , rrulib_shm_22             , "E_RRH_SHMID_APL_ALM_NTC_HIS"  ,0},
	{	E_RRH_SHMID_APL_EQ_HIS,						sizeof(rrulib_shm_23)             , rrulib_shm_23             , "E_RRH_SHMID_APL_EQ_HIS"  ,0},
	{	E_RRH_SHMID_APL_ACTIVE_ALM_LIST,			sizeof(rrulib_shm_24)             , rrulib_shm_24             , "E_RRH_SHMID_APL_ACTIVE_ALM_LIST"  ,0},
	{	E_RRH_SHMID_APL_CAR_STAT_HIS,				sizeof(rrulib_shm_25)             , rrulib_shm_25             , "E_RRH_SHMID_APL_CAR_STAT_HIS"  ,0},
	{	E_RRH_SHMID_APL_PTP_CONF,					sizeof(rrulib_shm_26)             , rrulib_shm_26             , "E_RRH_SHMID_APL_PTP_CONF"  ,0},
	{	E_RRH_SHMID_APL_PTP_STAT_HIS,				sizeof(rrulib_shm_27)             , rrulib_shm_27             , "E_RRH_SHMID_APL_PTP_STAT_HIS"  ,0},
	{	E_RRH_SHMID_APL_PTP_TIME_ADJ,				sizeof(rrulib_shm_28)             , rrulib_shm_28             , "E_RRH_SHMID_APL_PTP_TIME_ADJ"  ,0},
	{	E_RRH_SHMID_APL_UPSIGSTOP_HIS,				sizeof(rrulib_shm_29)             , rrulib_shm_29             , "E_RRH_SHMID_APL_UPSIGSTOP_HIS"  ,0},
	{	E_RRH_SHMID_APL_FH_L1_HIS,					sizeof(rrulib_shm_30)             , rrulib_shm_30             , "E_RRH_SHMID_APL_FH_L1_HIS"  ,0},
	{	E_RRH_SHMID_APL_SFNFRM_CALB_TBL,			sizeof(rrulib_shm_31)             , rrulib_shm_31             , "E_RRH_SHMID_APL_SFNFRM_CALB_TBL"  ,0},
	{	E_RRH_SHMID_APL_TXPOW_DETECT_TBL,			sizeof(rrulib_shm_32)             , rrulib_shm_32             , "E_RRH_SHMID_APL_TXPOW_DETECT_TBL"  ,0},
	{	E_RRH_SHMID_APL_TXPOW_DETECT_LOG,			sizeof(rrulib_shm_33)             , rrulib_shm_33             , "E_RRH_SHMID_APL_TXPOW_DETECT_LOG"  ,0},
	{	E_RRH_SHMID_APL_LUT_LOG,					sizeof(rrulib_shm_34)             , rrulib_shm_34             , "E_RRH_SHMID_APL_LUT_LOG"  ,0},
	{	E_RRH_SHMID_APL_FH_STAT,					sizeof(rrulib_shm_35)             , rrulib_shm_35             , "E_RRH_SHMID_APL_FH_STAT"  ,0},
	{	E_RRH_SHMID_APL_MAC_STAT_ETH0,				sizeof(rrulib_shm_36)             , rrulib_shm_36             , "E_RRH_SHMID_APL_MAC_STAT_ETH0"  ,0},
	{	E_RRH_SHMID_APL_MAC_STAT_ETH1,				sizeof(rrulib_shm_37)             , rrulib_shm_37             , "E_RRH_SHMID_APL_MAC_STAT_ETH1"  ,0},
	{	E_RRH_SHMID_APL_PKT_STAT_MPLANE,			sizeof(rrulib_shm_38)             , rrulib_shm_38             , "E_RRH_SHMID_APL_PKT_STAT_MPLANE"  ,0},
	{	E_RRH_SHMID_APL_SV_STATE_HISTORY,			sizeof(rrulib_shm_39)             , rrulib_shm_39             , "E_RRH_SHMID_APL_SV_STATE_HISTORY"  ,0},
	{	E_RRH_SHMID_APL_TRAINING_DATA,				sizeof(rrulib_shm_40)             , rrulib_shm_40             , "E_RRH_SHMID_APL_TRAINING_DATA"  ,0},
	{	E_RRH_SHMID_APL_DU_INFORMATION_TBL,			sizeof(rrulib_shm_41)             , rrulib_shm_41             , "E_RRH_SHMID_APL_DU_INFORMATION_TBL"  ,0},
	{	E_RRH_SHMID_APL_TALISE_CONF_INFO,			sizeof(rrulib_shm_42)             , rrulib_shm_42             , "E_RRH_SHMID_APL_TALISE_CONF_INFO"  ,0},
	{	E_RRH_SHMID_APL_DU_PROCESS_INFO,			sizeof(rrulib_shm_43)             , rrulib_shm_43             , "E_RRH_SHMID_APL_DU_PROCESS_INFO"  ,0},
	{	E_RRH_SHMID_APL_RF_TRAINING_DATA,			sizeof(rrulib_shm_44)             , rrulib_shm_44             , "E_RRH_SHMID_APL_RF_TRAINING_DATA"  ,0},
	{	E_RRH_SHMID_APL_DU_CMD_INFO,				sizeof(rrulib_shm_45)             , rrulib_shm_45             , "E_RRH_SHMID_APL_DU_CMD_INFO"  ,0},
	{	E_RRH_SHMID_APL_DU_FLT_MNG,					sizeof(rrulib_shm_46)             , rrulib_shm_46             , "E_RRH_SHMID_APL_DU_FLT_MNG"  ,0},

	{	E_RRH_SHMID_APL_T_RRH_NETCONF_INFO_TBL,		sizeof(rrulib_shm_54)             , rrulib_shm_54             , "E_RRH_SHMID_APL_T_RRH_NETCONF_INFO_TBL"  ,0},					
	{	E_RRH_SHMID_APL_TXCAL_COEF,					sizeof(rrulib_shm_55)             , rrulib_shm_55             , "E_RRH_SHMID_APL_TXCAL_COEF"  ,0},					
	{	E_RRH_SHMID_APL_RXCAL_COEF,					sizeof(rrulib_shm_56)             , rrulib_shm_56             , "E_RRH_SHMID_APL_RXCAL_COEF"  ,0},					
	{	E_RRH_SHMID_APL_PFM_PERFORMANCE_TBL,		sizeof(rrulib_shm_57)             , rrulib_shm_57             , "E_RRH_SHMID_APL_PFM_PERFORMANCE_TBL"  ,0},					
	{	E_RRH_SHMID_APL_PFM_PERFORMANCE_MNG_TBL,	sizeof(rrulib_shm_58)             , rrulib_shm_58             , "E_RRH_SHMID_APL_PFM_PERFORMANCE_MNG_TBL"  ,0},					
	{	E_RRH_SHMID_APL_LBM_MNGMENT_TBL,			sizeof(rrulib_shm_59)             , rrulib_shm_59             , "E_RRH_SHMID_APL_LBM_MNGMENT_TBL"  ,0},						
	{	E_RRH_SHMID_EIO_SVCTRL_MNG_TBL,				sizeof(rrulib_shm_60)             , rrulib_shm_60             , "E_RRH_SHMID_EIO_SVCTRL_MNG_TBL"  ,0},						
	{	E_RRH_SHMID_EIO_SVCTRL_LOG_TBL,				sizeof(rrulib_shm_61)             , rrulib_shm_61             , "E_RRH_SHMID_EIO_SVCTRL_LOG_TBL"  ,0},						
	{	E_RRH_SHMID_APL_TP_CMD_INFO,				sizeof(rrulib_shm_62)             , rrulib_shm_62             , "E_RRH_SHMID_APL_TP_CMD_INFO"  ,0},						
	{	E_RRH_SHMID_APL_MT_INFO,					sizeof(rrulib_shm_63)             , rrulib_shm_63             , "E_RRH_SHMID_APL_MT_INFO"  ,0},						
	{	E_RRH_SHMID_APL_BF_MODE_TBL,				sizeof(rrulib_shm_64)             , rrulib_shm_64             , "E_RRH_SHMID_APL_BF_MODE_TBL"  ,0},						

	{	BPF_MESSAGE_SIZE_S + E_RRH_SHMID_APL_MAX,	sizeof(rrulib_shm_47)             , rrulib_shm_47             , "BPF_MESSAGE_SIZE_S_T"  ,0},
	{	BPF_MESSAGE_SIZE_M + E_RRH_SHMID_APL_MAX,	sizeof(rrulib_shm_48)             , rrulib_shm_48             , "BPF_MESSAGE_SIZE_M_T"  ,0},
	{	BPF_MESSAGE_SIZE_L + E_RRH_SHMID_APL_MAX,	sizeof(rrulib_shm_49)             , rrulib_shm_49             , "BPF_MESSAGE_SIZE_L_T"  ,0},
	{	0x10000000+BPF_MESSAGE_SIZE_S + E_RRH_SHMID_APL_MAX,	sizeof(rrulib_shm_50)             , rrulib_shm_50             , "BPF_MESSAGE_SIZE_S_P"  ,0},
	{	0x10000000+BPF_MESSAGE_SIZE_M + E_RRH_SHMID_APL_MAX,	sizeof(rrulib_shm_51)             , rrulib_shm_51             , "BPF_MESSAGE_SIZE_M_P"  ,0},
	{	0x10000000+BPF_MESSAGE_SIZE_L + E_RRH_SHMID_APL_MAX,	sizeof(rrulib_shm_52)             , rrulib_shm_52             , "BPF_MESSAGE_SIZE_L_P"  ,0},
	{	0x20000000+BPF_MESSAGE_SIZE_L + E_RRH_SHMID_APL_MAX,	sizeof(rrulib_shm_53)             , rrulib_shm_53             , "BPF_MESSAGE_SIZE_L_C"  ,0},

	{ 0                                         , 0                                    , NULL                         , "UNKNOWN" ,0}
};

void* rrulib_shm_addrget(unsigned int kind , char** name)
{
	unsigned int count;
	void*		 addr = NULL;

	for(count=0;count < (sizeof(rrulib_shm_mem)/sizeof(rrulib_shm_mem[0]));count++)
	{
		if(rrulib_shm_mem[count].addr == NULL)
		{
			break;
		}
		if(kind == rrulib_shm_mem[count].top )
		{
			addr = rrulib_shm_mem[count].addr;
			*name = rrulib_shm_mem[count].name;
			break;
		}
	}
	if(count >= (sizeof(rrulib_shm_mem)/sizeof(rrulib_shm_mem[0])))
	{
		DPRINT("ERROR Kind %d\n",kind);
		*name = rrulib_shm_mem[count].name;
	}
	return(addr);
}

void* rrulib_shm_getaddr(char* name)
{
	unsigned int count;
	for(count=0;count < (sizeof(rrulib_shm_mem)/sizeof(rrulib_shm_mem[0]));count++)
	{
		if(!strcmp(name,rrulib_shm_mem[count].name))
		{
			return(rrulib_shm_mem[count].addr);
		}
	}
	if(count >= (sizeof(rrulib_shm_mem)/sizeof(rrulib_shm_mem[0])))
	{
		DPRINT("ERROR \n");
	}
	return(NULL);
}

void* rrulib_shm_set(char* name , unsigned int ofs , unsigned int size , unsigned int val)
{
	void* addr = NULL;
	if(( addr = rrulib_shm_getaddr(name)) == NULL)
	{
		DPRINT("Set error (%s)\n",name);
		return(addr);
	}
	addr += ofs;
	switch(size)
	{
		case 1:
			*(unsigned char*)addr = (unsigned char)val;
			break;
		case 2:
			*(unsigned short*)addr = (unsigned short)val;
			break;
		case 4:
			*(unsigned int*)addr = val;
			break;
		default:
			memset(addr,(unsigned char)val,size);
			break;
	}
	return(addr);
}

unsigned int rrulib_shm_get(char* name , unsigned int ofs , unsigned int size )
{
	void* addr = NULL;
	unsigned int val;
	if(( addr = rrulib_shm_getaddr(name)) == NULL)
	{
		DPRINT("Gett error (%s)\n",name);
		return(0);
	}
	addr += ofs;
	switch(size)
	{
		case 1:
			val = *(unsigned char*)addr;
			val &= 0xFF;
			break;
		case 2:
			val = *(unsigned short*)addr;
			val &= 0xFFFF;
			break;
		case 4:
			val = *(unsigned int*)addr;
			break;
		default:
			break;
	}
	return(val);
}


/**********************************************************************************************/
/* Symbol Mem                                                                                 */
/**********************************************************************************************/
static unsigned int rrulib_sym_count = 0;
static t_rrulib_sym_mng rrulib_sym_mng[32];

void* rrulib_sym_getaddr(char* name)
{
	unsigned int count;
	unsigned int count2;

	if(rrulib_sym_count == 0)
	{
/*		DPRINT("Not entry(%s)\n",name);	*/
		return(NULL);
	}
	for(count=0 ; 
		count < (sizeof(rrulib_sym_mng) / sizeof(rrulib_sym_mng[0])) ;
		count++)
	{
		if(rrulib_sym_mng[count].rrulib_sym_posbit == 0)
		{
			continue;
		}
		for(count2=0 ; count2 < 32 ;count2++)
		{
			if(	(( rrulib_sym_mng[count].rrulib_sym_posbit ) &
				 (unsigned int)(0x01 << count2)) == 0)
			{
				continue;
			}
			if(!strcmp(name,rrulib_sym_mng[count].symbol[count2].name))
			{
				return(rrulib_sym_mng[count].symbol[count2].addr);
			}
		}
	}
	DPRINT("Can't getaddr(%s)\n",name);
	return(NULL);
}

void* rrulib_sym_alloc(char* name , unsigned int size )
{
	void* addr = NULL;
	unsigned int count;
	unsigned int count2;
	
	if(( addr = rrulib_sym_getaddr(name)) != NULL)
	{
		DPRINT("Already alloc name(%s)\n",name);
		return(addr);
	}
	for(count=0 ; 
		count < (sizeof(rrulib_sym_mng) / sizeof(rrulib_sym_mng[0])) ;
		count++)
	{
		for(count2=0 ; count2 < 32 ;count2++)
		{
			if(	(( rrulib_sym_mng[count].rrulib_sym_posbit ) &
				 (unsigned int)(0x01 << count2)) != 0)
			{
				continue;
			}
			strncpy(rrulib_sym_mng[count].symbol[count2].name,
					name,
					sizeof(rrulib_sym_mng[count].symbol[count2].name));
			rrulib_sym_mng[count].symbol[count2].size = size;
			rrulib_sym_mng[count].symbol[count2].addr = malloc(size);
			addr = rrulib_sym_mng[count].symbol[count2].addr;
			rrulib_sym_mng[count].rrulib_sym_posbit |= ((unsigned int)(0x01 << count2));
			rrulib_sym_count++;
			DPRINT("alloc name(%s) %p \n",name,addr);
			return(addr);
		}
	}
	DPRINT("Can't alloc name(%s) %p \n",name,addr);
	return(addr);
}

void* rrulib_sym_set(char* name , unsigned int ofs , unsigned int size , unsigned int val)
{
	void* addr = NULL;
	if(( addr = rrulib_sym_getaddr(name)) == NULL)
	{
		DPRINT("Set error (%s)\n",name);
		return(addr);
	}
	addr += ofs;
	switch(size)
	{
		case 1:
			*(unsigned char*)addr = (unsigned char)val;
			break;
		case 2:
			*(unsigned short*)addr = (unsigned short)val;
			break;
		case 4:
			*(unsigned int*)addr = val;
			break;
		default:
			memset(addr,(unsigned char)val,size);
			break;
	}
	return(addr);
}

void* rrulib_sym_set_s(char* name , unsigned int ofs , unsigned int size , char* val)
{
	void* addr = NULL;
	if(( addr = rrulib_sym_getaddr(name)) == NULL)
	{
		DPRINT("Set error (%s)\n",name);
		return(addr);
	}
	addr += ofs;
	memcpy(addr,val,size);
	return(addr);
}

unsigned int rrulib_sym_get(char* name , unsigned int ofs , unsigned int size )
{
	void* addr = NULL;
	unsigned int val;
	if(( addr = rrulib_sym_getaddr(name)) == NULL)
	{
		DPRINT("Gett error (%s)\n",name);
		return(0);
	}
	addr += ofs;
	switch(size)
	{
		case 1:
			val = *(unsigned char*)addr;
			val &= 0xFF;
			break;
		case 2:
			val = *(unsigned short*)addr;
			val &= 0xFFFF;
			break;
		case 4:
			val = *(unsigned int*)addr;
			break;
		default:
			break;
	}
	return(val);
}

void rrulib_sym_free(char* name)
{
	void* addr = NULL;
	unsigned int count;
	unsigned int count2;

	if(rrulib_sym_count == 0)
	{
		return;
	}
	if(( addr = rrulib_sym_getaddr(name)) == NULL)
	{
		return;
	}
	for(count=0 ; 
		count < (sizeof(rrulib_sym_mng) / sizeof(rrulib_sym_mng[0])) ;
		count++)
	{
		for( count2=0 ; count2 < 32 ; count2++ )
		{
			if(	(( rrulib_sym_mng[count].rrulib_sym_posbit ) &
				 (unsigned int)(0x01 << count2)) == 0)
			{
				continue;
			}
			if(addr != rrulib_sym_mng[count].symbol[count2].addr)
			{
				continue;
			}
			memset(rrulib_sym_mng[count].symbol[count2].name,0,sizeof(rrulib_sym_mng[count].symbol[count2].name));
			rrulib_sym_mng[count].symbol[count2].size = 0;
			rrulib_sym_mng[count].symbol[count2].addr = (void*)NULL;
			free(addr);
			rrulib_sym_mng[count].rrulib_sym_posbit &= ~((unsigned int)(0x01 << count2));
			rrulib_sym_count--;
			return;
		}
	}
	return;
}

void rrulib_sym_allclean( void )
{
	void* addr;
	unsigned int count;
	unsigned int count2;

	for(count=0 ; 
		count < (sizeof(rrulib_sym_mng) / sizeof(rrulib_sym_mng[0])) ;
		count++)
	{
		for( count2=0 ; count2 < 32 ; count2++ )
		{
			if(	(( rrulib_sym_mng[count].rrulib_sym_posbit ) &
				 (unsigned int)(0x01 << count2)) == 0)
			{
				continue;
			}
			memset(rrulib_sym_mng[count].symbol[count2].name,0,sizeof(rrulib_sym_mng[count].symbol[count2].name));
			addr = rrulib_sym_mng[count].symbol[count2].addr;
			rrulib_sym_mng[count].symbol[count2].addr = (void*)NULL;
			rrulib_sym_mng[count].symbol[count2].size = 0;
			free(addr);
			rrulib_sym_mng[count].rrulib_sym_posbit &= ~((unsigned int)(0x01 << count2));
			rrulib_sym_count--;
		}
	}	
	return;
}





#include "rrupfdump.h"
#include "bpf_l_hm_devc.h"
#include "BPF_HM_DEVC.h"
#include "BPF_RU_IPCM.h"

static pcap_thrd_name rrupf_thd_name_ast [] = 
{
	{"UNKNOWN "},		/* need */
	{"debug   "},
	{"debug2  "},
	{"INFO    "},
	{"WARNING "},
	{"ERROR   "},
	{"CRITICAL"},
	{"CLEAN   "},
	{"END"}				/* need */
};

static pcap_thrd_name rrupf_thd_name_reg [] = 
{
	{"UNKNOWN "},
	{"HDL_R   "},
	{"HDL_W   "},
	{"POLL_R  "},
	{"POLL_W  "},
	{"BUF_R   "},
	{"BUF_W   "},
	{"READ    "},
	{"WRITE   "},
	{"CLEAN   "},
	{"END"}
};

static pcap_thrd_name rrupf_thd_name_spi [] = 
{
	{"UNKNOWN "},
	{"READ_L  "},
	{"WRITE_L "},
	{"READ    "},
	{"WRITE   "},
	{"CLEAN   "},
	{"END"}
};

static pcap_thrd_name rrupf_thd_name_i2c [] = 
{
	{"UNKNOWN "},
	{"READ    "},
	{"WRITE   "},
	{"CLEAN   "},
	{"END"}
};

static pcap_thrd_name rrupf_thd_name_qspi [] = 
{
	{"UNKNOWN "},
	{"READ_S  "},	/* start */
	{"READ_E  "},	/* end */
	{"WRITE_S "},
	{"WRITE_E "},
	{"ERASE_S "},
	{"ERASE_E "},
	{"CLEAN   "},
	{"END"}
};

static pcap_thrd_name rrupf_thd_name_trc [] = 
{
	{"UNKNOWN "},
	{"debug   "},
	{"InfoLow "},
	{"InfoHigh"},		/* yellow */
	{"ABN     "},		/* red */
	{"BUG     "},		/* red */
	{"CLEAN   "},
	{"END"}
};

static pcap_thrd_name rrupf_thd_name_sem [] = 
{
	{"UNKNOWN "},
	{"DESTROY "},
	{"INIT    "},
	{"WAIT    "},
	{"POST    "},
	{"CLEAN   "},
	{"END"}
};

static pcap_thrd_name rrupf_thd_name_tim [] = 
{
	{"UNKNOWN "},
	{"SET_LOW "},
	{"CAN_LOW "},
	{"TOUT_LOW"},
	{"SET     "},
	{"CAN     "},
	{"TOUT    "},
	{"CLEAN   "},
	{"END"}
};


static pcap_thrd_name rrupf_thd_name_buf [] = 
{
	{"UNKNOWN "},
	{"GET_LOW "},
	{"FREE_LOW"},
	{"GET     "},
	{"FREE    "},
	{"CLEAN   "},
	{"END"}
};

pcap_thrd_name rrupf_thd_name_aisg [] = 
{
	{"UNKNOWN "},
	{"debug   "},
	{"READ    "},
	{"WRITE   "},
	{"SND     "},
	{"RCV     "},
	{"TIMER   "},
	{"ERROR   "},
	{"BUG     "},
	{"CLEAN   "},
	{"END"}
};

pcap_thrd_name rrupf_thd_name_aisghc [] = 
{
	{"UNKNOWN "},
	{"debug   "},
	{"AISG SND"},
	{"AISG RCV"},
	{"CPRI SND"},
	{"CPRI RCV"},
	{"LINK DWN"},
	{"NO ASIGN"},
	{"END"}
};


pcap_thrd_name rrupf_thd_name_ptpinfo [] = 
{
	{"UNKNOWN "},
	{"debug   "},
	{"DATA    "},
	{"TRACE   "},
	{"ERROR   "},
	{"BUG     "},
	{"CLEAN   "},
	{"END"}
};


static pcap_thrd_name rrupf_thd_name_oth [] = 
{
	{"UNKNOWN "},
	{"END"}
};


static int rrupf_proctid_pre = -1;
static char rrupf_procname_pre[32];

static char* func_get_prcname(int selftid)
{
	char filename[32];
	int fd;
	int len;
	
	if(rrupf_proctid_pre != selftid){
		memset(filename,0,sizeof(filename));
		snprintf((char*)filename, sizeof(filename),"/proc/%d/comm", selftid);
		if ((fd = open(filename, O_RDONLY)) > 0){
			len = read(fd,rrupf_procname_pre,sizeof(rrupf_procname_pre));
			close(fd);
			rrupf_procname_pre[len - 1] = '\0';
			rrupf_proctid_pre = selftid;
		}
		else{
			rrupf_proctid_pre = -1;
			return(rrupf_thd_name_oth[0].name);
		}
	}
	return rrupf_procname_pre;
}


static void func_ptif_header(FILE* fp , char* data_p ,int datalen)
{
	if(datalen < ( sizeof(struct timespec)+ sizeof(unsigned int)))
	{
		return;
	}
	fprintf(fp,"(%lu.%06ld);",
	(((pcap_rrupf_userheader* )data_p)->data.tcount.tv_sec),
	((((pcap_rrupf_userheader* )data_p)->data.tcount.tv_nsec)/1000)	/* usec */
	);
	return;
}

static char* other_proc_name[] = 
{
	""
};

static int func_ptif_ast(FILE* fp , char* data_p ,int datalen, unsigned int kind )
{
	if(datalen < sizeof(unsigned int))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	fprintf(fp,"%s",
//	((pcap_rrupf_userheader* )data_p)->data.info.ast.code,
	((pcap_rrupf_userheader* )data_p)->data.info.ast.text);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}

typedef struct 
{
	char*	 	 name;
	unsigned int addr;
	unsigned int size;
}t_reg_name_detail;

typedef struct 
{
	unsigned int		base;
	unsigned int		d_count;
	t_reg_name_detail* 	detail;
}t_reg_name;


#if defined (OPT_RRH_ARCH_ZYNQ) 
#define RRUPFDUMP_REG_OFS_FPGA1		0xA0000000
#define RRUPFDUMP_REG_OFS_FPGA2		0xA1000000
#define RRUPFDUMP_REG_OFS_FPGA3		0xA2000000
#define RRUPFDUMP_REG_OFS_FPGA4		0xB0000000
#define RRUPFDUMP_REG_OFS_FPGA5		0xB1000000
#define RRUPFDUMP_REG_OFS_FPGA6		0xB2000000
#define RRUPFDUMP_REG_OFS_FPGA7		0xB3000000
#define RRUPFDUMP_REG_OFS_FPGA8		0xA3000000
#define RRUPFDUMP_REG_OFS_FPGA9		0xA4000000
#define RRUPFDUMP_REG_OFS_FPGA10	0xBC000000
#define RRUPFDUMP_REG_OFS_FPGA11	0xE0000000
#define RRUPFDUMP_REG_OFS_FPGA12	0xFF000000

static t_reg_name_detail reg_name_fpga[] = 
{
	{ "CNT"                 ,  0xA0000000 , 0x01000000  },
	{ "unknown"           ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga2[] = 
{
	{ "ETH"              ,  0xA1000000 , 0x01000000     },
	{ "unknown"           ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga3[] = 
{
	{ "PHYL"             ,  0xA2000000 , 0x01000000     },
	{ "unknown"           ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga4[] = 
{
	{ "TRX1_0"             ,  0xB0000000 , 0x01000000     },
	{ "unknown"            ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga5[] = 
{
	{ "TRX1_1"             ,  0xB1000000 , 0x01000000     },
	{ "unknown"            ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga6[] = 
{
	{ "TRX2_0"           ,  0xB2000000 , 0x01000000     },
	{ "unknown"          ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga7[] = 
{
	{ "TRX2_1"           ,  0xB3000000 , 0x01000000     },
	{ "unknown"          ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga8[] = 
{
	{ "SRAM1"             ,  0xA3000000 , 0x01000000     },
	{ "unknown"          ,  0          ,          0 }
};
static t_reg_name_detail reg_name_fpga9[] = 
{
	{ "SRAM2"            ,  0xA4000000 , 0x01000000     },
	{ "unknown"          ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga10[] = 
{
	{ "HPM_PCIe"         ,  0xBC000000 , 0x01000000     },
	{ "unknown"          ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga11[] = 
{
	{ "PCIe"             ,  0xE0000000 , 0x00200000     },
	{ "unknown"          ,  0          ,          0 }
};


static t_reg_name_detail reg_name_fpga12[] = 
{
	{ "CPU"             ,  0xFF000000 , 0x00C00000  },
	{ "unknown"          ,  0          ,          0 }
};



static t_reg_name reg_name_info[ ] = 
{
	{ RRUPFDUMP_REG_OFS_FPGA1 , sizeof(reg_name_fpga)/sizeof(reg_name_fpga[0])          ,reg_name_fpga       },
	{ RRUPFDUMP_REG_OFS_FPGA2 , sizeof(reg_name_fpga2)/sizeof(reg_name_fpga2[0])        ,reg_name_fpga2      },
	{ RRUPFDUMP_REG_OFS_FPGA3 , sizeof(reg_name_fpga3)/sizeof(reg_name_fpga3[0])        ,reg_name_fpga3      },
	{ RRUPFDUMP_REG_OFS_FPGA4 , sizeof(reg_name_fpga4)/sizeof(reg_name_fpga4[0])        ,reg_name_fpga4      },
	{ RRUPFDUMP_REG_OFS_FPGA5 , sizeof(reg_name_fpga5)/sizeof(reg_name_fpga5[0])        ,reg_name_fpga5      },
	{ RRUPFDUMP_REG_OFS_FPGA6 , sizeof(reg_name_fpga6)/sizeof(reg_name_fpga6[0])        ,reg_name_fpga6      },
	{ RRUPFDUMP_REG_OFS_FPGA7 , sizeof(reg_name_fpga7)/sizeof(reg_name_fpga7[0])        ,reg_name_fpga7      },
	{ RRUPFDUMP_REG_OFS_FPGA8 , sizeof(reg_name_fpga8)/sizeof(reg_name_fpga8[0])        ,reg_name_fpga8      },
	{ RRUPFDUMP_REG_OFS_FPGA9 , sizeof(reg_name_fpga9)/sizeof(reg_name_fpga9[0])        ,reg_name_fpga9      },
	{ RRUPFDUMP_REG_OFS_FPGA10, sizeof(reg_name_fpga10)/sizeof(reg_name_fpga10[0])      ,reg_name_fpga10      },
	{ RRUPFDUMP_REG_OFS_FPGA11, sizeof(reg_name_fpga11)/sizeof(reg_name_fpga11[0])      ,reg_name_fpga11      },
	{ RRUPFDUMP_REG_OFS_FPGA12, sizeof(reg_name_fpga12)/sizeof(reg_name_fpga12[0])      ,reg_name_fpga12      }
};

#else
#define RRUPFDUMP_REG_OFS_FPGA1		0x00000000
#define RRUPFDUMP_REG_OFS_FPGA2		0x00000000
#define RRUPFDUMP_REG_OFS_FPGA3		0x00000000

static t_reg_name_detail reg_name_fpga[] = 
{
	{ "fpga"              ,  RRUPFDUMP_REG_OFS_FPGA1 , 0x0FFFFFFF },
	{ "unknown"           ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga2[] = 
{
	{ "fpga2"             ,  RRUPFDUMP_REG_OFS_FPGA2 , 0x0FFFFFFF },
	{ "unknown"           ,  0          ,          0 }
};

static t_reg_name_detail reg_name_fpga3[] = 
{
	{ "fpga3"             ,  RRUPFDUMP_REG_OFS_FPGA3 , 0x0FFFFFFF },
	{ "unknown"           ,  0          ,          0 }
};

static t_reg_name reg_name_info[ ] = 
{
	{ RRUPFDUMP_REG_OFS_FPGA1 , sizeof(reg_name_fpga)/sizeof(reg_name_fpga[0])          ,reg_name_fpga       },
	{ RRUPFDUMP_REG_OFS_FPGA2 , sizeof(reg_name_fpga2)/sizeof(reg_name_fpga2[0])        ,reg_name_fpga2      },
	{ RRUPFDUMP_REG_OFS_FPGA3 , sizeof(reg_name_fpga3)/sizeof(reg_name_fpga3[0])        ,reg_name_fpga3      }
};


#endif


int func_ptif_reg(FILE* fp , char* data_p ,int datalen, unsigned int kind )
{
	char*			name = "unknown";
	unsigned int	addr;
	unsigned int	top_addr;
	unsigned int	end_addr;
	unsigned int	base;
	unsigned int	count;
	unsigned int	count2;
	t_reg_name_detail* 	detail;

	if(datalen < sizeof(t_log_reg))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	addr = ((pcap_rrupf_userheader* )data_p)->data.info.reg.addr;
	base = (unsigned int)(addr & 0xFF000000);
	for(count=0 ; count < (sizeof(reg_name_info)/sizeof(reg_name_info[0])) ; count++)
	{
		if(base != reg_name_info[count].base)
		{
			continue;
		}
		detail = reg_name_info[count].detail;
		for(count2=0;count2 < reg_name_info[count].d_count ; count2++,detail++)
		{
			top_addr = detail->addr;
			end_addr = top_addr + detail->size;
			if((addr >= top_addr ) && (addr <= end_addr))
			{
				name = detail->name;
				break;
			}
		}
		break;
	}
	fprintf(fp,"ad-0x%08x;[%s];dt-0x%08x",
		((pcap_rrupf_userheader* )data_p)->data.info.reg.addr,
		name,
		((pcap_rrupf_userheader* )data_p)->data.info.reg.data
	);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}

static char* spi00_proc_name[1] = 
{
	"PTP"
};

static char* spi01_proc_name[1] = 
{
	"JC-PLL"
};

static char* spi02_proc_name[1] = 
{
	"RF-PLL"
};


static char* spi10_proc_name[1] = 
{
	"RFIC#1"
};

static char* spi11_proc_name[1] = 
{
	"RFIC#2"
};


static char* spi20_proc_name[0x10] = 
{
	"FAN-DAC",
	"TX-DAC",
	"RX-ADC",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};

static char* spi30_proc_name[0x10] = 
{
	"QMOD(M/V)-V",
	"QMOD(VGA)-V",
	"QDEM-V",
	"QMOD(M/V)-H",
	"QMOD(VGA)-H",
	"QDEM-H",
	"QMOD/DEM_DAC",
	"PAVG_DAC",
	"TxVA_DAC",
	"PAVG_DAC",
	"TxVA_DAC-V",
	"RxVA_ADC-V",
	"TxVA_DAC-H",
	"RxVA_ADC-H",
	"",
	""
};

int func_ptif_spi(FILE* fp , char* data_p ,int datalen, unsigned int kind )
{
	char*			name = other_proc_name[0];
	unsigned int	no;
	unsigned int	ss;
//	unsigned int	ssd;
//	unsigned int	ssdno;

	if(datalen < sizeof(t_log_spi))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	no = ((pcap_rrupf_userheader* )data_p)->data.info.spi.no;
	ss = ((pcap_rrupf_userheader* )data_p)->data.info.spi.ssno;
//	ssd = (ss & 0xFFFF);
	ss = (ss >> 16) & 0xF;
	switch(no){
		case 0:
			switch(ss){
				case 0:
					name = spi00_proc_name[0];
					break;
				case 1:
					name = spi01_proc_name[0];
					break;
				case 2:
					name = spi02_proc_name[0];
					break;
				default:
					break;
			}
			break;
		case 1:
			switch(ss){
				case 0:
					name = spi10_proc_name[0];
					break;
				case 1:
					name = spi11_proc_name[0];
					break;
				default:
					break;
			}
			break;
		case 2:
			name = spi20_proc_name[ss];
			break;
		case 3:
			name = spi30_proc_name[ss];
			break;
		default:
			break;
	}
	fprintf(fp,"%s(%d.%d);ad-0x%08x;dt-0x%08x",
		name,
		no,
		ss,
		((pcap_rrupf_userheader* )data_p)->data.info.spi.addr,
		((pcap_rrupf_userheader* )data_p)->data.info.spi.data
	);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}


static char* i2c0_proc_name[2] = 
{
	"EEPROM",
	"TEMP "
};


static char* i2c1_proc_name[2] = 
{
	"SFP-0(A0)",
	"SFP-0(A2)"
};

static char* i2c2_proc_name[1] = 
{
	"RF-EEP"
};


int func_ptif_i2c(FILE* fp , char* data_p ,int datalen, unsigned int kind )
{
	char*			name = other_proc_name[0];
	unsigned int	no;
	unsigned int	ss;

	if(datalen < sizeof(t_log_i2c))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	no = ((pcap_rrupf_userheader* )data_p)->data.info.i2c.no;
	ss = ((pcap_rrupf_userheader* )data_p)->data.info.i2c.ssno;
	switch(no)
	{
		case 0:
			if(ss < 2)
			{
				name = i2c0_proc_name[ss];
			}
			break;
		case 1:
			if(ss < 4)
			{
				name = i2c1_proc_name[ss];
			}
			break;
		case 2:
			if((ss & 0xF) < 3)
			{
				name = i2c2_proc_name[ss & 0xF];
			}
			break;
		default:
			break;
	}

	fprintf(fp,"%s;ad-0x%08x;dt-0x%08x;no-0x%08x;ss-0x%08x",
		name,
		((pcap_rrupf_userheader* )data_p)->data.info.i2c.addr,
		((pcap_rrupf_userheader* )data_p)->data.info.i2c.data,
		((pcap_rrupf_userheader* )data_p)->data.info.i2c.no,
		((pcap_rrupf_userheader* )data_p)->data.info.i2c.ssno
	);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}

static char* qspi_proc_name[2] = 
{
	"MASTER",
	"SLAVE"
};

int func_ptif_qspi(FILE* fp , char* data_p ,int datalen, unsigned int kind)
{
	char*			name = other_proc_name[0];
	unsigned int	no;

	if(datalen < sizeof(t_log_qspi))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	no = ((pcap_rrupf_userheader* )data_p)->data.info.qspi.no;
	if(no < 2)
	{
		name = qspi_proc_name[no];
	}
	fprintf(fp,"%s;ad-0x%08x;sz-0x%08x;no-%d;face-%d",
		name,
		((pcap_rrupf_userheader* )data_p)->data.info.qspi.addr,
		((pcap_rrupf_userheader* )data_p)->data.info.qspi.size,
		((pcap_rrupf_userheader* )data_p)->data.info.qspi.no,
		((pcap_rrupf_userheader* )data_p)->data.info.qspi.face
	);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}

int func_ptif_trc(FILE* fp , char* data_p ,int datalen, unsigned int kind )
{

	if(datalen < sizeof(unsigned int))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	fprintf(fp,"%s(%d);%s",
		((pcap_rrupf_userheader* )data_p)->data.info.trace.file,
		((pcap_rrupf_userheader* )data_p)->data.info.trace.line,
		((pcap_rrupf_userheader* )data_p)->data.info.trace.text
	);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}

int func_ptif_sem(FILE* fp , char* data_p ,int datalen, unsigned int kind)
{
	if(datalen < sizeof(t_log_sem))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	fprintf(fp,"id-0x%lx val-0x%08x",((pcap_rrupf_userheader* )data_p)->data.info.sem.id,
									((pcap_rrupf_userheader* )data_p)->data.info.sem.val1);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}

#define D_RRUPKTDUMP_TIMERNUM_MAX	1
static char* func_pif_timer_name[D_RRUPKTDUMP_TIMERNUM_MAX] = 
{
	"UNKNOWN",
};


int func_ptif_tim(FILE* fp , char* data_p ,int datalen, unsigned int kind)
{
	char*			name = func_pif_timer_name[0];
	unsigned int	id;

	if(datalen < sizeof(t_log_tim))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	id = ((pcap_rrupf_userheader* )data_p)->data.info.tim.id;
	if(id < D_RRUPKTDUMP_TIMERNUM_MAX)
	{
		name = func_pif_timer_name[id];
	}
	fprintf(fp,"%s(%u);bid-0x%08x;evt-0x%08x",
		name,id,
		((pcap_rrupf_userheader* )data_p)->data.info.tim.id_bpf,
		((pcap_rrupf_userheader* )data_p)->data.info.tim.id_os
/*		((pcap_rrupf_userheader* )data_p)->data.info.tim.value	*/
	);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}

int func_ptif_tim2(FILE* fp , char* data_p ,int datalen, unsigned int kind)
{
	if(datalen < sizeof(t_log_tim2))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	fprintf(fp,"id-0x%lx;latency-0x%08x;period-0x%08x",
		((pcap_rrupf_userheader* )data_p)->data.info.tim2.id,
		((pcap_rrupf_userheader* )data_p)->data.info.tim2.latency,
		((pcap_rrupf_userheader* )data_p)->data.info.tim2.period
	);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}

int func_ptif_buf(FILE* fp , char* data_p ,int datalen, unsigned int kind)
{
	char*			name = other_proc_name[0];
	unsigned int	bkind;

	if(datalen < sizeof(t_log_buf))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	bkind = ((pcap_rrupf_userheader* )data_p)->data.info.buf.shmid;
	switch(bkind)
	{
		case E_BPF_RU_IPCM_SHMKIND_TASK_S:
			name = "TASK_S";
			break;
		case E_BPF_RU_IPCM_SHMKIND_TASK_M:
			name = "TASK_M";
			break;
		case E_BPF_RU_IPCM_SHMKIND_TASK_L:
			name = "TASK_L";
			break;
		default:
			break;
	}
	fprintf(fp,"%s(0x%08x);id-0x%08x;use-%d;pid-%d",
		name,
		bkind,
		((pcap_rrupf_userheader* )data_p)->data.info.buf.id,
		((pcap_rrupf_userheader* )data_p)->data.info.buf.addr,
		((pcap_rrupf_userheader* )data_p)->data.info.buf.size
	);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}


const char* func_aisg_ctlwrd [] =
{
		/* 00                 01                 02                 03                 04                 05                 06                 07                */
		/* ------------------------------------------------------------------------------------------------------------------------------------------------------ */
/* 0 */	   "IFRM(P/F0,S0/R0)","RR  (P/F0,R0)   ","IFRM(P/F0,S1/R0)","No entry        ","IFRM(P/F0,S2/R0)","RNR (P/F0,R0)   ","IFRM(P/F0,S3/R0)","No entry        ",
		   "IFRM(P/F0,S4/R0)","No entry        ","IFRM(P/F0,S5/R0)","No entry        ","IFRM(P/F0,S6/R0)","No entry        ","IFRM(P/F0,S7/R0)","DM  (F0)        ",
/* 10 */   "IFRM(P/F1,S0/R0)","RR  (P/F1,R0)   ","IFRM(P/F1,S1/R0)","No entry        ","IFRM(P/F1,S2/R0)","RNR (P/F1,R0)   ","IFRM(P/F1,S3/R0)","No entry        ",
		   "IFRM(P/F1,S4/R0)","No entry        ","IFRM(P/F1,S5/R0)","No entry        ","IFRM(P/F1,S6/R0)","No entry        ","IFRM(P/F1,S7/R0)","DM  (F1)        ",
/* 20 */   "IFRM(P/F0,S0/R1)","RR  (P/F0,R1)   ","IFRM(P/F0,S1/R1)","No entry        ","IFRM(P/F0,S2/R1)","RNR (P/F0,R1)   ","IFRM(P/F0,S3/R1)","No entry        ",
		   "IFRM(P/F0,S4/R1)","No entry        ","IFRM(P/F0,S5/R1)","No entry        ","IFRM(P/F0,S6/R1)","No entry        ","IFRM(P/F0,S7/R1)","No entry        ",
/* 30 */   "IFRM(P/F1,S0/R1)","RR  (P/F1,R1)   ","IFRM(P/F1,S1/R1)","No entry        ","IFRM(P/F1,S2/R1)","RNR (P/F1,R1)   ","IFRM(P/F1,S3/R1)","No entry        ",
		   "IFRM(P/F1,S4/R1)","No entry        ","IFRM(P/F1,S5/R1)","No entry        ","IFRM(P/F1,S6/R1)","No entry        ","IFRM(P/F1,S7/R1)","No entry        ",
/* 40 */   "IFRM(P/F0,S0/R2)","RR  (P/F0,R2)   ","IFRM(P/F0,S1/R2)","DISC(P0)        ","IFRM(P/F0,S2/R2)","RNR (P/F0,R2)   ","IFRM(P/F0,S3/R2)","No entry        ",
		   "IFRM(P/F0,S4/R2)","No entry        ","IFRM(P/F0,S5/R2)","No entry        ","IFRM(P/F0,S6/R2)","No entry        ","IFRM(P/F0,S7/R2)","No entry        ",
/* 50 */   "IFRM(P/F1,S0/R2)","RR  (P/F1,R2)   ","IFRM(P/F1,S1/R2)","DISC(P1)        ","IFRM(P/F1,S2/R2)","RNR (P/F1,R2)   ","IFRM(P/F1,S3/R2)","No entry        ",
		   "IFRM(P/F1,S4/R2)","No entry        ","IFRM(P/F1,S5/R2)","No entry        ","IFRM(P/F1,S6/R2)","No entry        ","IFRM(P/F1,S7/R2)","No entry        ",
/* 60 */   "IFRM(P/F0,S0/R3)","RR  (P/F0,R3)   ","IFRM(P/F0,S1/R3)","UA  (F0)        ","IFRM(P/F0,S2/R3)","RNR (P/F0,R3)   ","IFRM(P/F0,S3/R3)","No entry        ",
		   "IFRM(P/F0,S4/R3)","No entry        ","IFRM(P/F0,S5/R3)","No entry        ","IFRM(P/F0,S6/R3)","No entry        ","IFRM(P/F0,S7/R3)","No entry        ",
/* 70 */   "IFRM(P/F1,S0/R3)","RR  (P/F1,R3)   ","IFRM(P/F1,S1/R3)","UA  (F1)        ","IFRM(P/F1,S2/R3)","RNR (P/F1,R3)   ","IFRM(P/F1,S3/R3)","No entry        ",
		   "IFRM(P/F1,S4/R3)","No entry        ","IFRM(P/F1,S5/R3)","No entry        ","IFRM(P/F1,S6/R3)","No entry        ","IFRM(P/F1,S7/R3)","No entry        ",
/* 80 */   "IFRM(P/F0,S0/R4)","RR  (P/F0,R4)   ","IFRM(P/F0,S1/R4)","SNRM(P0)        ","IFRM(P/F0,S2/R4)","RNR (P/F0,R4)   ","IFRM(P/F0,S3/R4)","FRMR(F0)        ",
		   "IFRM(P/F0,S4/R4)","No entry        ","IFRM(P/F0,S5/R4)","No entry        ","IFRM(P/F0,S6/R4)","No entry        ","IFRM(P/F0,S7/R4)","No entry        ",
/* 90 */   "IFRM(P/F1,S0/R4)","RR  (P/F1,R4)   ","IFRM(P/F1,S1/R4)","SNRM(P1)        ","IFRM(P/F1,S2/R4)","RNR (P/F1,R4)   ","IFRM(P/F1,S3/R4)","FRMR(F1)        ",
		   "IFRM(P/F1,S4/R4)","No entry        ","IFRM(P/F1,S5/R4)","No entry        ","IFRM(P/F1,S6/R4)","No entry        ","IFRM(P/F1,S7/R4)","No entry        ",
/* A0 */   "IFRM(P/F0,S0/R5)","RR  (P/F0,R5)   ","IFRM(P/F0,S1/R5)","No entry        ","IFRM(P/F0,S2/R5)","RNR (P/F0,R5)   ","IFRM(P/F0,S3/R5)","No entry        ",
		   "IFRM(P/F0,S4/R5)","No entry        ","IFRM(P/F0,S5/R5)","No entry        ","IFRM(P/F0,S6/R5)","No entry        ","IFRM(P/F0,S7/R5)","XID(P0)         ",
/* B0 */   "IFRM(P/F1,S0/R5)","RR  (P/F1,R5)   ","IFRM(P/F1,S1/R5)","No entry        ","IFRM(P/F1,S2/R5)","RNR (P/F1,R5)   ","IFRM(P/F1,S3/R5)","No entry        ",
		   "IFRM(P/F1,S4/R5)","No entry        ","IFRM(P/F1,S5/R5)","No entry        ","IFRM(P/F1,S6/R5)","No entry        ","IFRM(P/F1,S7/R5)","XID(P1)         ",
/* C0 */   "IFRM(P/F0,S0/R6)","RR  (P/F0,R6)   ","IFRM(P/F0,S1/R6)","No entry        ","IFRM(P/F0,S2/R6)","RNR (P/F0,R6)   ","IFRM(P/F0,S3/R6)","No entry        ",
		   "IFRM(P/F0,S4/R6)","No entry        ","IFRM(P/F0,S5/R6)","No entry        ","IFRM(P/F0,S6/R6)","No entry        ","IFRM(P/F0,S7/R6)","No entry        ",
/* D0 */   "IFRM(P/F1,S0/R6)","RR  (P/F1,R6)   ","IFRM(P/F1,S1/R6)","No entry        ","IFRM(P/F1,S2/R6)","RNR (P/F1,R6)   ","IFRM(P/F1,S3/R6)","No entry        ",
		   "IFRM(P/F1,S4/R6)","No entry        ","IFRM(P/F1,S5/R6)","No entry        ","IFRM(P/F1,S6/R6)","No entry        ","IFRM(P/F1,S7/R6)","No entry        ",
/* E0 */   "IFRM(P/F0,S0/R7)","RR  (P/F0,R7)   ","IFRM(P/F0,S1/R7)","No entry        ","IFRM(P/F0,S2/R7)","RNR (P/F0,R7)   ","IFRM(P/F0,S3/R7)","No entry        ",
		   "IFRM(P/F0,S4/R7)","No entry        ","IFRM(P/F0,S5/R7)","No entry        ","IFRM(P/F0,S6/R7)","No entry        ","IFRM(P/F0,S7/R7)","No entry        ",
/* F0 */   "IFRM(P/F1,S0/R7)","RR  (P/F1,R7)   ","IFRM(P/F1,S1/R7)","No entry        ","IFRM(P/F1,S2/R7)","RNR (P/F1,R7)   ","IFRM(P/F1,S3/R7)","No entry        ",
		   "IFRM(P/F1,S4/R7)","No entry        ","IFRM(P/F1,S5/R7)","No entry        ","IFRM(P/F1,S6/R7)","No entry        ","IFRM(P/F1,S7/R7)","No entry        "
};

static void func_ptif_comdata(FILE* fp , char* data_p ,int datalen, unsigned int kind )
{
	unsigned int count;
	unsigned char* wdata_p = (unsigned char* )data_p;

	if(datalen > 0)
	{
		fprintf(fp,"           <0x0000>;");
		for(count=0;  (count < datalen ) && ( count < D_BPF_PLOG_COM_DATA ) ;count++ , wdata_p++)
		{
			fprintf(fp,"%02x ", *wdata_p );
			if(((count+1) % 32) == 0)
			{
				fprintf(fp,"\n           <0x%04x>;",count+1);
			}
		}
	}
	return;
}

int func_ptif_aisg_com(FILE* fp , char* data_p ,int datalen, unsigned int kind)
{
	if((((pcap_rrupf_userheader* )data_p)->data.len) < 2)
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	fprintf(fp,"ad-0x%02x;ctl-0x%02x(%s)",
		((pcap_rrupf_userheader* )data_p)->data.info.com.data[0],
		((pcap_rrupf_userheader* )data_p)->data.info.com.data[1],
		func_aisg_ctlwrd[ (unsigned char)(((pcap_rrupf_userheader* )data_p)->data.info.com.data[1]) ]
	);	
	if(((pcap_rrupf_userheader* )data_p)->data.len > 2)
	{
		fprintf(fp,"\n");
		func_ptif_comdata(fp,((pcap_rrupf_userheader* )data_p)->data.info.com.data,((pcap_rrupf_userheader* )data_p)->data.len,kind);
	}
	return D_RRUPKTDUMP_TYPE_INTERNL;
}

static const char* ptp_state[17] = 
{
	"FREQ_LOCK_ACQUIRING",
	"FREQ_LOCK_ACQUIRED",
	"PHASE_LOCK_ACQUIRED",
	"HOLDOVER",
	"REF_FAILED",
	"NO_ACTIVE_SERVER",
	"UNKNOWN",
	"MANUAL_FREERUN",
	"MANUAL_HOLDOVER",
	"MANUAL_SERVO_HOLDOVER",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"INITIAL"	/* fj original */
};

typedef struct {
	unsigned short   state;
	char phaseAlignAccuracy_sign;
	char mostRecentCalculatedOffset_sign;
	unsigned int phaseAlignAccuracy_hi;
	unsigned int phaseAlignAccuracy_lo;
	unsigned int mostRecentCalculatedOffset_hi;
	unsigned int mostRecentCalculatedOffset_lo;
	unsigned int mostRecentCalculatedOffset_hi2[10];
	unsigned int mostRecentCalculatedOffset_lo2[10];
	unsigned int adjustmentNetworkQuality2[10];
	int          freqOffset;
	char startElement[10];
	char mostRecentCalculatedOffset_sign2[10];
	char adjustmentUsable[10];
	char adjustmentApplied[10];
}ptp_ploginfo_t;	/* 184 */


int func_ptif_ptpinfo(FILE* fp , char* data_p ,int datalen, unsigned int kind)
{
	if((((pcap_rrupf_userheader* )data_p)->data.len) < 2)
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	ptp_ploginfo_t* ptp_data_p;
	unsigned int count;
	
	ptp_data_p = (ptp_ploginfo_t*)&(((pcap_rrupf_userheader* )data_p)->data.info.com.data[0]);
	fprintf(fp,"\nCurrent State(%d) %s \n",ptp_data_p->state,ptp_state[ptp_data_p->state & 0xF]);	
	fprintf(fp,"The phase alignment accuracy         %c0x%X%08X \n",ptp_data_p->phaseAlignAccuracy_sign,ptp_data_p->phaseAlignAccuracy_hi,ptp_data_p->phaseAlignAccuracy_lo);	
	fprintf(fp,"The current phase alignment accuracy %c0x%X%08X \n",ptp_data_p->mostRecentCalculatedOffset_sign,ptp_data_p->mostRecentCalculatedOffset_hi,ptp_data_p->mostRecentCalculatedOffset_lo);
	if((ptp_data_p->mostRecentCalculatedOffset_hi != 0) || (ptp_data_p->mostRecentCalculatedOffset_lo != 0)){
		for(count=0;count < 10;count++){
	fprintf(fp,"[%d] adjustment %c0x%X%08X ns usable %d applied %d quality %d\n",
			ptp_data_p->startElement[count],
			ptp_data_p->mostRecentCalculatedOffset_sign2[count],
			ptp_data_p->mostRecentCalculatedOffset_hi2[count],
			ptp_data_p->mostRecentCalculatedOffset_lo2[count],
			ptp_data_p->adjustmentUsable[count],
			ptp_data_p->adjustmentApplied[count],
			ptp_data_p->adjustmentNetworkQuality2[count]);
		}
	}
	fprintf(fp,"The frequency offset is %d ppt\n",ptp_data_p->freqOffset);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}



pcap_ptif_name rrupf_ptif_name_ast [] = 
{
	{0,0,"",func_ptif_ast},
	{0,0,"Unknown",NULL}
};

pcap_ptif_name rrupf_ptif_name_reg [] = 
{
	{0,0,"",func_ptif_reg},	
	{0,0,"Unknown",NULL}	
};

pcap_ptif_name rrupf_ptif_name_spi [] = 
{
	{0,0,"",func_ptif_spi},		
	{0,0,"Unknown",NULL}	
};

pcap_ptif_name rrupf_ptif_name_i2c [] = 
{
	{0,0,"",func_ptif_i2c},		
	{0,0,"Unknown",NULL}	
};

pcap_ptif_name rrupf_ptif_name_qspi [] = 
{
	{0,0,"",func_ptif_qspi },
	{0,0,"Unknown",NULL}	
};

pcap_ptif_name rrupf_ptif_name_trc [] = 
{
	{0,0,"",func_ptif_trc },
	{0,0,"Unknown",NULL}
};

pcap_ptif_name rrupf_ptif_name_sem [] = 
{
	{0,0,"",func_ptif_sem },
	{0,0,"Unknown",NULL}
};

pcap_ptif_name rrupf_ptif_name_tim [] = 
{
	{0,0,"",func_ptif_tim },
	{0,0,"Unknown",NULL}
};

pcap_ptif_name rrupf_ptif_name_tim2 [] = 
{
	{0,0,"",func_ptif_tim2 },
	{0,0,"Unknown",NULL}
};


pcap_ptif_name rrupf_ptif_name_buf [] = 
{
	{0,0,"",func_ptif_buf },
	{0,0,"Unknown",NULL}
};

pcap_ptif_name rrupf_ptif_name_aisg [] = 
{
	{0,0,"",func_ptif_trc },
	{0,0,"",func_ptif_aisg_com },
	{0,0,"",func_ptif_reg },
	{0,0,"Unknown",NULL}
};

pcap_ptif_name rrupf_ptif_name_ptpinfo [] = 
{
	{0,0,"",func_ptif_trc },
	{0,0,"",func_ptif_ptpinfo },
	{0,0,"Unknown",NULL}
};

pcap_ptif_name rrupf_ptif_name_oth [] = 
{
	{0,0,"Unknown",NULL}
};



pcap_proc_name proc_name [] = 
{
	{""       ,rrupf_thd_name_oth    , rrupf_ptif_name_oth  },
	{""       ,rrupf_thd_name_ast    , rrupf_ptif_name_ast  },	/* 1 */
	{"REG"    ,rrupf_thd_name_reg    , rrupf_ptif_name_reg  },	/* 2 */
	{"SPI"    ,rrupf_thd_name_spi    , rrupf_ptif_name_spi  },	/* 3 */
	{"I2C"    ,rrupf_thd_name_i2c    , rrupf_ptif_name_i2c  },	/* 4 */
	{"QSPI"   ,rrupf_thd_name_qspi   , rrupf_ptif_name_qspi },	/* 5 */
	{"TMMP"   ,rrupf_thd_name_tim    , rrupf_ptif_name_tim2 },	/* 6 */
	{""       ,rrupf_thd_name_oth    , rrupf_ptif_name_oth  },	/* 7 */
	{""       ,rrupf_thd_name_trc    , rrupf_ptif_name_trc  },	/* 8 Trace */
	{"PTPI"   ,rrupf_thd_name_ptpinfo , rrupf_ptif_name_ptpinfo },	/* 9 */
	{""       ,rrupf_thd_name_aisg   , rrupf_ptif_name_aisg },	/* A */
	{"SEM"    ,rrupf_thd_name_sem    , rrupf_ptif_name_sem  },	/* B */
	{"TIM"    ,rrupf_thd_name_tim    , rrupf_ptif_name_tim  },	/* C */
	{"BUF"    ,rrupf_thd_name_buf    , rrupf_ptif_name_buf  },	/* D */
	{""       ,rrupf_thd_name_aisghc , rrupf_ptif_name_aisg }	/* E */
};


static pcap_thrd_name rrupf_thd_name_mp [] =
{
	{"Who? "},
	{"SAA  "},
	{"SPV  "},
	{"PFM  "},
	{"NOTI "},
	{"NSA  "},	/* netconfd-pro */
	{"Who?1"},
	{"Who?2"},
	{"Who?3"}
};

static const char* mplog_level [] = 
{
	"NONE  ",	
	"OFF   ",
	"WRITE ",
	"DEV0  ",
	"ERROR ",
	"WARN  ",
	"INFO  ",
	"DEV1  ",
	"DEBUG ",
	"DEBUG2",
	"DEBUG3",
	"DEBUG4",
	"UNKNW1",
	"UNKNW2",
	"UNKNW3",
	"END"
};



static int func_ptif_mp(FILE* fp , char* data_p ,int datalen, unsigned int kind )
{
	if(datalen < sizeof(unsigned int))
	{
		return D_RRUPKTDUMP_TYPE_UNKNOWN;
	}
	int slen = strlen(((pcap_rrupf_userheader_mp* )data_p)->data.info.com.text);
	fprintf(fp,"%s:%*s",
	mplog_level[((pcap_rrupf_userheader_mp* )data_p)->data.info.com.loglevel & 0xF ] ,
	slen,
	((pcap_rrupf_userheader_mp* )data_p)->data.info.com.text);
	return D_RRUPKTDUMP_TYPE_INTERNL;
}


pcap_ptif_name rrupf_ptif_name_mp [] = 
{
	{0,0,"",func_ptif_mp},
	{0,0,"Unknown",NULL}
};


pcap_proc_name proc_name_mp [] = 
{
	{"MPSW"       ,rrupf_thd_name_mp    , rrupf_ptif_name_mp  },	/* 1 */

};

pcap_proc_name proc_name_spprc [] = 
{
	{"INIT" ,rrupf_thd_name_trc          , rrupf_ptif_name_trc  },
	{"PF"   ,rrupf_thd_name_trc          , rrupf_ptif_name_trc  },
	{"PTPL" ,rrupf_thd_name_trc          , rrupf_ptif_name_trc  },
	{"PTP"  ,rrupf_thd_name_trc          , rrupf_ptif_name_trc  },
	{"PTPH" ,rrupf_thd_name_trc          , rrupf_ptif_name_trc  },
	{"MNT"  ,rrupf_thd_name_trc          , rrupf_ptif_name_trc  },
	{"CAL"  ,rrupf_thd_name_trc          , rrupf_ptif_name_trc  },
	{"RFIC" ,rrupf_thd_name_trc          , rrupf_ptif_name_trc  },
	{"PL"   ,rrupf_thd_name_trc          , rrupf_ptif_name_trc  }
};

unsigned int func_com_msg_index(unsigned int dstip , unsigned int* kind)
{
	unsigned int index;
	*kind = 0;

	index = (unsigned int)((dstip & 0x00000F00) >> 8);
	*kind = (unsigned int)((dstip & 0x0000F000) >> 12);
	if(index >= ((sizeof(proc_name)/sizeof(proc_name[0]))) )
	{
		index = 0;
	}
	return index;
}

unsigned int func_com_msg_opeindex(unsigned int dstip)
{
	unsigned int index;
	unsigned int opeindex = 0;
	unsigned int kind;
	index = func_com_msg_index(dstip , &kind );
	opeindex = dstip & 0x000000FF;
	switch ( index )
	{
		case E_AST_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_ast)/sizeof(rrupf_thd_name_ast[0])))
			{
				opeindex = 0;
			}
			break;
		case E_REG_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_reg)/sizeof(rrupf_thd_name_reg[0])))
			{
				opeindex = 0;
			}
			break;
		case E_SPI_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_spi)/sizeof(rrupf_thd_name_spi[0])))
			{
				opeindex = 0;
			}
			break;
		case E_I2C_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_i2c)/sizeof(rrupf_thd_name_i2c[0])))
			{
				opeindex = 0;
			}
			break;
		case E_QSPI_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_qspi)/sizeof(rrupf_thd_name_qspi[0])))
			{
				opeindex = 0;
			}
			break;
		case E_TRC_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_trc)/sizeof(rrupf_thd_name_trc[0])))
			{
				opeindex = 0;
			}
			break;
		case E_SEM_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_sem)/sizeof(rrupf_thd_name_sem[0])))
			{
				opeindex = 0;
			}
			break;
		case E_TIM_NO:
		case E_TIM2_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_tim)/sizeof(rrupf_thd_name_tim[0])))
			{
				opeindex = 0;
			}
			break;
		case E_BUF_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_buf)/sizeof(rrupf_thd_name_buf[0])))
			{
				opeindex = 0;
			}
			break;
		case E_PTPINFO_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_ptpinfo)/sizeof(rrupf_thd_name_ptpinfo[0])))
			{
				opeindex = 0;
			}
			break;
		case E_AISG2_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_aisg)/sizeof(rrupf_thd_name_aisg[0])))
			{
				opeindex = 0;
			}
			break;
		case E_AISGHC_NO:
			if(opeindex >= (sizeof(rrupf_thd_name_aisghc)/sizeof(rrupf_thd_name_aisghc[0])))
			{
				opeindex = 0;
			}
			break;
		default:
			opeindex = 0;
			break;
	}
	return opeindex;
}

/* ユーザデータ表示関数 */
int func_userdata_dsp(
	FILE* fp,
	pcap_rrupf_userheader* user_p,
	int len,
	unsigned int index,
	unsigned int subindex,
	int			 srcthdno,
	unsigned int kind
)
{
	unsigned int count;
	int rtn = D_RRUPKTDUMP_TYPE_UNKNOWN;
	
	for(count=0 ; ; count++)
	{
		if(proc_name[index].ptif[count].func == NULL)
		{
			break;
		}
		if( index == E_PTPINFO_NO )
		{
			if(subindex == ( E_PTPINFO_LV_DATA & 0x0F )){
				count = 1;
			}
			else
			{
				count = 0;
			}
		}
		func_ptif_header(fp,(char*)user_p,len);
		if(gmode != 0)
		{
			fprintf(fp,"%s;%-8s;%s(%u);len(%u);",proc_name[index].name,proc_name[index].thrd[subindex].name,(unsigned char*)(func_get_prcname(srcthdno)),srcthdno,len);
		}
		else
		{
			fprintf(fp,"%s;%-8s;%s(%u);",proc_name[index].name,proc_name[index].thrd[subindex].name,(unsigned char*)(func_get_prcname(srcthdno)),srcthdno);
		}
		rtn = proc_name[index].ptif[count].func(fp,(char*)user_p,len,kind);
		break;
	}
	return(rtn);
}


int func_userdata_dsp_mp(
	FILE* fp,
	pcap_rrupf_userheader* user_p,
	int len,
	unsigned int index,
	unsigned int subindex,
	int			 srcthdno,
	unsigned int kind
)
{
	unsigned int count;
	int rtn = D_RRUPKTDUMP_TYPE_UNKNOWN;
	
	for(count=0 ; ; count++)
	{
		if(proc_name_mp[index].ptif[count].func == NULL)
		{
			break;
		}
		func_ptif_header(fp,(char*)user_p,len);
		fprintf(fp,"%-5s;%s(%u);",proc_name_mp[index].thrd[subindex].name,(unsigned char*)(func_get_prcname(srcthdno)),srcthdno);
		rtn = proc_name_mp[index].ptif[count].func(fp,(char*)user_p,len,kind);
		break;
	}
	return(rtn);
}


/* IPヘッダ表示関数 */
unsigned int func_iphead_dsp(
	FILE* fp ,
	pcap_data_timeoffs_t* time_p,
	pcap_rrupf_ipheader* data,
	unsigned int* subindex,
	unsigned int* kind
)
{
	unsigned int index;
	unsigned int dstip;
	struct tm lctimep;
	time_t local_sec;


	dstip = ntohl(data->h_ip.ip_dst.s_addr);
	if(time_p != NULL)
	{
		memset(&lctimep,0,sizeof(lctimep));
		local_sec = (time_t)time_p->sec;
		localtime_r((const time_t *)&local_sec,&lctimep);
		fprintf(fp,"%04d/%02d/%02d %02d:%02d:%02d.%06ld;",
			lctimep.tm_year + 1900,
			lctimep.tm_mon+1,
			lctimep.tm_mday,
			lctimep.tm_hour,
			lctimep.tm_min,
			lctimep.tm_sec,
			time_p->usec
		);
	}
	index = func_com_msg_index(dstip,kind);
	*subindex = func_com_msg_opeindex(dstip);
	func_gmsg("(src:%08x->dst:%08x)",ntohl((unsigned int)(data->h_ip.ip_src.s_addr)),dstip);
	return(index);
}

static void func_help_fil_list_sub(unsigned int pindex,unsigned int index , pcap_thrd_name* tbl_p , pcap_proc_name* name_p)
{
	unsigned int count;
	printf("%-7s : 127.%3d.%3d.1..254 / ",name_p->name,pindex,index);
	for(count=0; ;count++,tbl_p++)
	{
		if(!strcmp(tbl_p->name , "UNKNOWN "))
		{
			continue;
		}
		if(!strcmp(tbl_p->name , "NOUSE   "))
		{
			continue;
		}
		if(!strcmp(tbl_p->name , "CLEAN   "))
		{
			continue;
		}
		if(!strcmp(tbl_p->name , "END"))
		{
			break;
		}
		printf("%s:%2d ",tbl_p->name,count);
	}
	printf("\n");
	return;
}

void func_help_fil_list( void )
{
	unsigned int count;
	unsigned int count2;

	printf("\n");
	printf("log IP and LEVEL is below . ** If you want to change log LEVEL , Please echo LEVEL > /proc/rru/xxx . ** \n");
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	for(count=1;count < (sizeof(proc_name)/sizeof(proc_name[0]));count++)
	{
		if( count == E_THD_NO)
		{
			continue;
		}
		func_help_fil_list_sub(1,count, proc_name[count].thrd, &proc_name[count]);
		if(count == E_TRC_NO)
		{
			for(count2=0;count2 < (sizeof(proc_name_spprc)/sizeof(proc_name_spprc[0]));count2++)
			{
				func_help_fil_list_sub(1,
									(((E_TRC_NO_INIT * count2) + E_TRC_NO_INIT) | E_TRC_NO),
									 proc_name_spprc[count2].thrd,
									 &proc_name_spprc[count2]);
			}
		}
	}
	printf("\n");
	return;
}


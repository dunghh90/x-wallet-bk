#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "f_rrh_inc.h"
#include "RFIC_TALISE.h"

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

t_call_result result_RFIC_TALISE_OPENHW;
t_call_result result_RFIC_TALISE_RESETDEVICE;
t_call_result result_RFIC_TALISE_INITIALIZE;
t_call_result result_RFIC_TALISE_GETPLLSLOCKSTATUS;
t_call_result result_RFIC_TALISE_ENABLEMULTICHIPSYNC;
t_call_result result_RFIC_TALISE_INITARM;
t_call_result result_RFIC_TALISE_LOADSTREAM;
t_call_result result_RFIC_TALISE_LOADARM;
t_call_result result_RFIC_TALISE_VERIFYARMCHECKSUM;
t_call_result result_RFIC_TALISE_SETRFPLLFREQUENCY;
t_call_result result_RFIC_TALISE_RUNINITCALS;
t_call_result result_RFIC_TALISE_WAITINITCALS;
t_call_result result_RFIC_TALISE_ENABLEFRAMERLINK;
t_call_result result_RFIC_TALISE_ENABLESYSREFTOFRAMER;
t_call_result result_RFIC_TALISE_ENABLEDEFRAMERLINK;
t_call_result result_RFIC_TALISE_ENABLESYSREFTODEFRAMER;
t_call_result result_RFIC_TALISE_READDEFRAMERSTATUS;
t_call_result result_RFIC_TALISE_READFRAMERSTATUS;
t_call_result result_RFIC_TALISE_ENABLETRACKINGCALS;
t_call_result result_RFIC_TALISE_RADIOON;
t_call_result result_RFIC_TALISE_RADIOOFF;
t_call_result result_RFIC_TALISE_SHUTDOWN;
t_call_result result_RFIC_TALISE_CLOSEHW;
t_call_result result_RFIC_TALISE_SETUPAUXDACS;
t_call_result result_RFIC_TALISE_WRITEAUXDAC;
t_call_result result_RFIC_TALISE_GETTEMPERATURE;
t_call_result result_RFIC_TALISE_SETTXATTENUATION;
t_call_result result_RFIC_TALISE_GETTXATTENUATION;
t_call_result result_RFIC_TALISE_SETTXTOORXMAPPING;
t_call_result result_RFIC_TALISE_SETARMGPIOPINS;
t_call_result result_RFIC_TALISE_SETRADIOCTLPINMODE;
t_call_result result_RFIC_TALISE_SETRXMANUALGAIN;
t_call_result result_RFIC_TALISE_SETOBSRXMANUALGAIN;
t_call_result result_RFIC_TALISE_SETRXTXENABLE;
t_call_result result_RFIC_TALISE_GETRXTXENABLE;
t_call_result result_RFIC_TALISE_GETRADIOSTATE;
t_call_result result_RFIC_TALISE_GETINITCALSTATUS;
t_call_result result_RFIC_TALISE_CHECKINITCALCOMPLETE;
t_call_result result_RFIC_TALISE_ENABLEFRAMERTESTDATA;
t_call_result result_RFIC_TALISE_SETUPADCSAMPLEXBAR;
t_call_result result_RFIC_TALISE_GETERRCODE;
t_call_result result_RFIC_TALISE_RESCHEDULETRACKINGCAL;
t_call_result result_RFIC_TALISE_ENABLEDEFRAMERPRBSCHECKER;
t_call_result result_RFIC_TALISE_READDEFRAMERPRBSCOUNTERS;
t_call_result result_RFIC_TALISE_SETRFPLLLOOPFILTER;
t_call_result result_RFIC_TALISE_GETRFPLLLOOPFILTER;
t_call_result result_RFIC_TAL_FPGA_INITIALIZE;
t_call_result result_RFIC_TAL_RFIC_INITIALIZE;
t_call_result result_RFIC_TAL_PLLIC_MCS;
t_call_result result_RFIC_TAL_ARM_START;
t_call_result result_RFIC_TAL_RFIC_CAL;
t_call_result result_RFIC_TAL_JESD_SET;
t_call_result result_RFIC_TAL_PLLIC_JESD;
t_call_result result_RFIC_TAL_SENDRCV_SET;

static t_call_result_ptn result[] = 
{
	{ "RFIC_TALISE_OPENHW" , &result_RFIC_TALISE_OPENHW },
	{ "RFIC_TALISE_RESETDEVICE" , &result_RFIC_TALISE_RESETDEVICE },
	{ "RFIC_TALISE_INITIALIZE" , &result_RFIC_TALISE_INITIALIZE },
	{ "RFIC_TALISE_GETPLLSLOCKSTATUS" , &result_RFIC_TALISE_GETPLLSLOCKSTATUS },
	{ "RFIC_TALISE_ENABLEMULTICHIPSYNC" , &result_RFIC_TALISE_ENABLEMULTICHIPSYNC },
	{ "RFIC_TALISE_INITARM" , &result_RFIC_TALISE_INITARM },
	{ "RFIC_TALISE_LOADSTREAM" , &result_RFIC_TALISE_LOADSTREAM },
	{ "RFIC_TALISE_LOADARM" , &result_RFIC_TALISE_LOADARM },
	{ "RFIC_TALISE_VERIFYARMCHECKSUM" , &result_RFIC_TALISE_VERIFYARMCHECKSUM },
	{ "RFIC_TALISE_SETRFPLLFREQUENCY" , &result_RFIC_TALISE_SETRFPLLFREQUENCY },
	{ "RFIC_TALISE_RUNINITCALS" , &result_RFIC_TALISE_RUNINITCALS },
	{ "RFIC_TALISE_WAITINITCALS" , &result_RFIC_TALISE_WAITINITCALS },
	{ "RFIC_TALISE_ENABLEFRAMERLINK" , &result_RFIC_TALISE_ENABLEFRAMERLINK },
	{ "RFIC_TALISE_ENABLESYSREFTOFRAMER" , &result_RFIC_TALISE_ENABLESYSREFTOFRAMER },
	{ "RFIC_TALISE_ENABLEDEFRAMERLINK" , &result_RFIC_TALISE_ENABLEDEFRAMERLINK },
	{ "RFIC_TALISE_ENABLESYSREFTODEFRAMER" , &result_RFIC_TALISE_ENABLESYSREFTODEFRAMER },
	{ "RFIC_TALISE_READDEFRAMERSTATUS" , &result_RFIC_TALISE_READDEFRAMERSTATUS },
	{ "RFIC_TALISE_READFRAMERSTATUS" , &result_RFIC_TALISE_READFRAMERSTATUS },
	{ "RFIC_TALISE_ENABLETRACKINGCALS" , &result_RFIC_TALISE_ENABLETRACKINGCALS },
	{ "RFIC_TALISE_RADIOON" , &result_RFIC_TALISE_RADIOON },
	{ "RFIC_TALISE_RADIOOFF" , &result_RFIC_TALISE_RADIOOFF },
	{ "RFIC_TALISE_SHUTDOWN" , &result_RFIC_TALISE_SHUTDOWN },
	{ "RFIC_TALISE_CLOSEHW" , &result_RFIC_TALISE_CLOSEHW },
	{ "RFIC_TALISE_SETUPAUXDACS" , &result_RFIC_TALISE_SETUPAUXDACS },
	{ "RFIC_TALISE_WRITEAUXDAC" , &result_RFIC_TALISE_WRITEAUXDAC },
	{ "RFIC_TALISE_GETTEMPERATURE" , &result_RFIC_TALISE_GETTEMPERATURE },
	{ "RFIC_TALISE_SETTXATTENUATION" , &result_RFIC_TALISE_SETTXATTENUATION },
	{ "RFIC_TALISE_GETTXATTENUATION" , &result_RFIC_TALISE_GETTXATTENUATION },
	{ "RFIC_TALISE_SETTXTOORXMAPPING" , &result_RFIC_TALISE_SETTXTOORXMAPPING },
	{ "RFIC_TALISE_SETARMGPIOPINS" , &result_RFIC_TALISE_SETARMGPIOPINS },
	{ "RFIC_TALISE_SETRADIOCTLPINMODE" , &result_RFIC_TALISE_SETRADIOCTLPINMODE },
	{ "RFIC_TALISE_SETRXMANUALGAIN" , &result_RFIC_TALISE_SETRXMANUALGAIN },
	{ "RFIC_TALISE_SETOBSRXMANUALGAIN" , &result_RFIC_TALISE_SETOBSRXMANUALGAIN },
	{ "RFIC_TALISE_SETRXTXENABLE" , &result_RFIC_TALISE_SETRXTXENABLE },
	{ "RFIC_TALISE_GETRXTXENABLE" , &result_RFIC_TALISE_GETRXTXENABLE },
	{ "RFIC_TALISE_GETRADIOSTATE" , &result_RFIC_TALISE_GETRADIOSTATE },
	{ "RFIC_TALISE_GETINITCALSTATUS" , &result_RFIC_TALISE_GETINITCALSTATUS },
	{ "RFIC_TALISE_CHECKINITCALCOMPLETE" , &result_RFIC_TALISE_CHECKINITCALCOMPLETE },
	{ "RFIC_TALISE_ENABLEFRAMERTESTDATA" , &result_RFIC_TALISE_ENABLEFRAMERTESTDATA },
	{ "RFIC_TALISE_SETUPADCSAMPLEXBAR" , &result_RFIC_TALISE_SETUPADCSAMPLEXBAR },
	{ "RFIC_TALISE_GETERRCODE" , &result_RFIC_TALISE_GETERRCODE },
	{ "RFIC_TALISE_RESCHEDULETRACKINGCAL" , &result_RFIC_TALISE_RESCHEDULETRACKINGCAL },
	{ "RFIC_TALISE_ENABLEDEFRAMERPRBSCHECKER" , &result_RFIC_TALISE_ENABLEDEFRAMERPRBSCHECKER },
	{ "RFIC_TALISE_READDEFRAMERPRBSCOUNTERS" , &result_RFIC_TALISE_READDEFRAMERPRBSCOUNTERS },
	{ "RFIC_TALISE_SETRFPLLLOOPFILTER" , &result_RFIC_TALISE_SETRFPLLLOOPFILTER },
	{ "RFIC_TALISE_GETRFPLLLOOPFILTER" , &result_RFIC_TALISE_GETRFPLLLOOPFILTER },
	{ "RFIC_TAL_FPGA_INITIALIZE" , &result_RFIC_TAL_FPGA_INITIALIZE },
	{ "RFIC_TAL_RFIC_INITIALIZE" , &result_RFIC_TAL_RFIC_INITIALIZE },
	{ "RFIC_TAL_PLLIC_MCS" , &result_RFIC_TAL_PLLIC_MCS },
	{ "RFIC_TAL_ARM_START" , &result_RFIC_TAL_ARM_START },
	{ "RFIC_TAL_RFIC_CAL" , &result_RFIC_TAL_RFIC_CAL },
	{ "RFIC_TAL_JESD_SET" , &result_RFIC_TAL_JESD_SET },
	{ "RFIC_TAL_PLLIC_JESD" , &result_RFIC_TAL_PLLIC_JESD },
	{ "RFIC_TAL_SENDRCV_SET" , &result_RFIC_TAL_SENDRCV_SET },
	{ NULL      , NULL            }
};

void rfic_callresult_init(char* name )
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

void rfic_callresult_set(char* name , int val)
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

int rfic_callresult_get(char* name )
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

#define DPRINT(...) f_rrulib_dmsg("RFIC", __func__ ,__VA_ARGS__ )
#define DPRINTCONVERT(dev,mode,...) f_rrulib_dconvert_log(dev,mode, __VA_ARGS__ )
#define RETURN()  return (rfic_callresult_get((char*)__func__))

int RFIC_TALISE_OPENHW(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_RESETDEVICE(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_INITIALIZE(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_GETPLLSLOCKSTATUS(uint8_t unit, uint8_t *status){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_ENABLEMULTICHIPSYNC(uint8_t unit, uint8_t enable){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_INITARM(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_LOADSTREAM(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_LOADARM(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_VERIFYARMCHECKSUM(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETRFPLLFREQUENCY(uint8_t unit, unsigned long dev_id,unsigned long freq_khz){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_RUNINITCALS(uint8_t unit, unsigned long cal_mask){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_WAITINITCALS(uint8_t unit, unsigned long timeoutMs, uint8_t *status){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_ENABLEFRAMERLINK(uint8_t unit, unsigned long framerSel ,unsigned long enable){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_ENABLESYSREFTOFRAMER(uint8_t unit, unsigned long framerSel, unsigned long enable){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_ENABLEDEFRAMERLINK(uint8_t unit, unsigned long deframerSel, unsigned long enable){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_ENABLESYSREFTODEFRAMER(uint8_t unit, unsigned long deframerSel, unsigned long enable){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_READDEFRAMERSTATUS(uint8_t unit, unsigned long deframerSel){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_READFRAMERSTATUS(uint8_t unit, unsigned long framerSel){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_ENABLETRACKINGCALS(uint8_t unit, unsigned long cal_mask){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_RADIOON(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_RADIOOFF(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SHUTDOWN(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_CLOSEHW(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETUPAUXDACS(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_WRITEAUXDAC(uint8_t unit, unsigned long auxDacIndex, unsigned long auxDacCode){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_GETTEMPERATURE(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETTXATTENUATION(uint8_t unit, unsigned long txChannel, unsigned long txAttenuation_mdB){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_GETTXATTENUATION(uint8_t unit, unsigned long txChannel){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETTXTOORXMAPPING(uint8_t unit, unsigned long txCalEnable, unsigned long oRx1Map, unsigned long oRx2Map){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETARMGPIOPINS(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETRADIOCTLPINMODE(uint8_t unit, unsigned long pinOptionsMask, unsigned long orxEnGpioPinSel){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETRXMANUALGAIN(uint8_t unit, unsigned long rxChannel, unsigned long gainIndex){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETOBSRXMANUALGAIN(uint8_t unit, unsigned long obsRxCh, unsigned long gainIndex){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETRXTXENABLE(uint8_t unit, unsigned long rxOrxChannel, unsigned long txChannel){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_GETRXTXENABLE(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_GETRADIOSTATE(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_GETINITCALSTATUS(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_CHECKINITCALCOMPLETE(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_ENABLEFRAMERTESTDATA(uint8_t unit, unsigned long framerSelect, unsigned long testDataSource, unsigned long injectPoint){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETUPADCSAMPLEXBAR(uint8_t unit, unsigned int framerSel, unsigned int *conv){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_GETERRCODE(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_RESCHEDULETRACKINGCAL(uint8_t unit, unsigned long trackingCal){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_ENABLEDEFRAMERPRBSCHECKER(uint8_t unit, unsigned long polyOrder, unsigned long checkerLocation){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_READDEFRAMERPRBSCOUNTERS(uint8_t unit, unsigned long lane){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_SETRFPLLLOOPFILTER(uint8_t unit, unsigned long loopBandwidth_kHz, unsigned long stability){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TALISE_GETRFPLLLOOPFILTER(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TAL_FPGA_INITIALIZE(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TAL_RFIC_INITIALIZE(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TAL_PLLIC_MCS(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TAL_ARM_START(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TAL_RFIC_CAL(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TAL_JESD_SET(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TAL_PLLIC_JESD(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }
int RFIC_TAL_SENDRCV_SET(uint8_t unit){ DPRINT("unit:0x%x \n",unit); RETURN(); }

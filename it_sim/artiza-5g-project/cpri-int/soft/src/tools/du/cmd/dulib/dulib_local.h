#ifndef __DULIB_LOCAL_H__
#define __DULIB_LOCAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <errno.h> 
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/compile.h>
#include <mruby/dump.h>
#include <mruby/variable.h>
#include "f_rrh_inc.h"
#include "BPF_COM.h"
#include "BPF_COM_END.h"
#include "BPF_COM_PLOG.h"
#include "BPF_HM_DEVC.h"
#include "BPF_RU_IPCM.h"
#include "BPF_RM_SVRM.h"
#include "f_rrh_reg_eep.h"
#include "rrhApi_Rfp.h"
#include "rrhApi_Com.h"
#include "f_du_reg_ctb.h"

void f_rrhlib_dmsg(const char* filename , const int line,  const char* format, ...);
#define rrhlib_dmsg(...) f_rrhlib_dmsg( __FILE__ , __LINE__ , __VA_ARGS__ )

void rrhlib_device_access_install(mrb_state* mrb ,struct RClass * module_class);
void rrhlib_api_install(mrb_state* mrb ,struct RClass * module_class);
extern unsigned int rrhlib_dev_assignflg;
#ifdef RRHLIB_MEM_CHK
int rrhlib_memchecker( unsigned char* chkbufp );
#define rrhlib_memchecker(addr) f_rrhlib_memchecker(addr)
#else
#define rrhlib_memchecker(addr) 0
#endif
unsigned int rrhlib_tbl_access_install(mrb_state* mrb ,struct RClass * module_class);

/********************************************************************************************************************/
/* Device access define                                                                                             */
/********************************************************************************************************************/
typedef struct
{
	char*				name;
	int					size;
	void*				shm_addr;
	void*				member;
	int					member_size;
}du_t_members;

typedef struct
{
	char*				name;
	int					id;
	int					size;
	void*				shm_addr;
	du_t_members*		members;
	int					members_size;
}du_t_shm_info;

extern T_RRH_DU_CMD_INFO* dulib_cmdshm_addr;

#endif

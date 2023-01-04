#ifndef __RRUNITLIB_H__
#define __RRUNITLIB_H__
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/compile.h>
#include <mruby/dump.h>
#include <mruby/variable.h>
#include <CUnit.h>
#include <Console.h>


typedef struct {
	CU_ErrorCode cu_error;				/*!< CUnit error code */
	char*       cu_txt;					/*!< CUnit error text */
}t_ct_CUerror;

/* struct */
typedef struct 
{
	unsigned long mode;
	unsigned long kind;
	unsigned long addr;
	unsigned long val;
	unsigned long ofs;
	unsigned long size;
	unsigned long msec;
}t_rrulib_dev;

typedef struct
{
	unsigned int top;
	unsigned int size;
	void*	     addr;
	char*	     name;
	unsigned int access;
}t_rrulib_mem;

typedef struct
{
	unsigned int addr;
	unsigned int val;
}t_rrulib_busy;


typedef struct
{
	char			name[32];
	unsigned int	size;
	void*			addr;
}t_rrulib_sym;

typedef struct
{
	unsigned int rrulib_sym_posbit;
	t_rrulib_sym symbol[32];
}t_rrulib_sym_mng;


/* Core */
void rrulib_usage(char* argv);
void rrulib_install(mrb_state* mrb);
void rrulib_uninstall(void);
void f_rrulib_dmsg(
	const char* name ,
	const char* funcname,
	const char* format,
	  ...);

mrb_value rrulib_func(char* name , mrb_state* mrb, mrb_value self,void* dl_handle);
mrb_value rrulib_addr_table(char* name , mrb_state* mrb, mrb_value self,void* dl_handle);
mrb_value rrulib_set_table(char* name , mrb_state* mrb, mrb_value self,void* dl_handle);
mrb_value rrulib_get_table(char* name , mrb_state* mrb, mrb_value self,void* dl_handle);
void* rrulib_dlopen(char* name , void** tmphandle);
void rrulib_dlclose(void* tmphandle);

/* Init */
void ct_init_install(mrb_state* mrb);
void ct_init_uninstall(void);
void ct_initmw_install(mrb_state* mrb);
void ct_initmw_uninstall(void);

/* PF */
void ct_pf_install(mrb_state* mrb);
void ct_pf_uninstall(void);

/* MP */
void ct_mp_install(mrb_state* mrb);
void ct_mp_uninstall(void);

/* MEM */
/* FPGA */
void rrulib_reg_init(unsigned int val);
unsigned int rrulib_reg_getval(unsigned int addr , char** name);
void rrulib_reg_setval(unsigned int addr , unsigned int val , char** name);

/* SPI */
void rrulib_spi_init(unsigned int val);
unsigned int rrulib_spi_getval(unsigned int kind , unsigned int subkind ,unsigned int addr, char**name);
void rrulib_spi_setval(unsigned int kind , unsigned int subkind ,unsigned int addr , unsigned int val , char** name);

/* Shared */
void* rrulib_shm_addrget(unsigned int kind , char** name);
void* rrulib_shm_getaddr(char* name);
void* rrulib_shm_set(char* name , unsigned int ofs , unsigned int size , unsigned int val);
unsigned int rrulib_shm_get(char* name , unsigned int ofs , unsigned int size );

/* Sym */
void* rrulib_sym_getaddr(char* name);
void* rrulib_sym_alloc(char* name , unsigned int size );
void* rrulib_sym_set(char* name , unsigned int ofs , unsigned int size , unsigned int val);
void* rrulib_sym_set_s(char* name , unsigned int ofs , unsigned int size , char* val);
unsigned int rrulib_sym_get(char* name , unsigned int ofs , unsigned int size );
void rrulib_sym_free(char* name);
void rrulib_sym_allclean( void );

/* Qspi */
void rrulib_qspi_get(unsigned int face , unsigned int addr , unsigned int size, unsigned char* buf , char** name);
void rrulib_qspi_set(unsigned int face , unsigned int addr , unsigned int size, unsigned char* buf , char** name);
void rrulib_qspi_init(unsigned int val);
int  rrulib_qspi_fileinit(unsigned int face , unsigned int ofs ,unsigned int size ,char* filename);

/* I2C */
void rrulib_i2c_init(unsigned int val);
unsigned int rrulib_i2c_getval(unsigned int kind , unsigned int subkind , unsigned int addr, char**name);
void rrulib_i2c_setval(unsigned int kind , unsigned int subkind , unsigned int addr , unsigned int val , char** name);

/* call result */
void rrulib_callresult_init(char* name );
void rrulib_callresult_set(char* name , long val);
long rrulib_callresult_get(char* name );

void oal_callresult_init(char* name );
void oal_callresult_set(char* name , int val);
int oal_callresult_get(char* name );

void hal_callresult_init(char* name );
void hal_callresult_set(char* name , int val);
int hal_callresult_get(char* name );

void rfic_callresult_init(char* name );
void rfic_callresult_set(char* name , int val);
int  rfic_callresult_get(char* name );


int ct_units_main(void* test_suites_array, int mode);


#endif

/*!
 *  @skip	$Id:$
 *  @file	rrulib.c
 *  @brief	rru command library routine
 *  @author	FJT)Taniguchi
 *  @date	2013/11/18 FJT)Taniguchi create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup	rru_TP_CMD_LIB	rru command library
 * @brief		rru command library
 * @note		This is rru command library.
 * @{
 */

#include "rrunitlib.h"

unsigned int rrulib_flg_dbg;
unsigned int rrulib_log_convert;
static unsigned int rrulib_dmsg_counter;
static unsigned int rrulib_log_convert_counter;
static volatile int rrulib_clear_flg[2] = {0,0};
static int rrulib_pagesize;
unsigned int rrulib_cunit_mode = 0;

/**********************************************************************************************/
/* Global routines                                                                            */
/**********************************************************************************************/
void* rrulib_dlopen(char* name , void** tmphandle)
{
	if(*tmphandle == NULL)
	{
		if ((*tmphandle = dlopen (name, RTLD_LAZY)) == NULL)
		{
			fprintf (stderr, "%s\n", dlerror());
			abort();	/* 強制終了 */
		}
	}
	return(*tmphandle);
}

void rrulib_dlclose(void* tmphandle)
{
	if(tmphandle != NULL)
	{
		dlclose(tmphandle);
	}
	return;
}

static int rrulib_name_to_level(const char* name)
{
	int level = -1;
	if(!strcmp(name,"CORE"))       { level = 0x1; }
	else if(!strcmp(name,"SYS"))   { level = 0x2; }
	else if(!strcmp(name,"OAL"))   { level = 0x4; }
	else if(!strcmp(name,"HAL"))   { level = 0x8; }
	else if(!strcmp(name,"RFIC"))  { level = 0x10; }
	else if(!strcmp(name,"CMN"))   { level = 0x20; }
	else if(!strcmp(name,"INIT"))  { level = 0x40; }
	else if(!strcmp(name,"INITMW")){ level = 0x80; }
	else if(!strcmp(name,"PF"))    { level = 0x100; }
	else if(!strcmp(name,"MP"))    { level = 0x200; }
	else if(!strcmp(name,"ALL"))   { level = 0x7FFFFFFF; }
	else{ ; }
	return(level);
}

void f_rrulib_dmsg(
	const char* name ,
	const char* funcname,
	const char* format,
	  ...)
{
	char str[1*1024*1024];
	int len;
	va_list args;
	
	if((rrulib_flg_dbg & rrulib_name_to_level(name) ) == 0)
	{
		return;
	}
	if(rrulib_log_convert != 0)
	{
		return;
	}
	rrulib_dmsg_counter++;
	len = snprintf(str,sizeof(str),"No.%6d;%-5s;%-32s;",rrulib_dmsg_counter,name,funcname );
	va_start(args, format);	/* pgr0039 */
	len +=  vsnprintf( &str[len] , sizeof(str) - len, format, args);
	va_end(args);

	fprintf(stdout, "%s", str);
	fflush(stdout);
	return;
}

#define rrulib_dmsg(...) f_rrulib_dmsg("CORE", __func__ ,__VA_ARGS__ )


void f_rrulib_dconvert_log(
	char*	device,
	char*	mode,
	const char* format,
	  ...)
{
	char str[256];
	int len;
	va_list args;
	int self = syscall(SYS_gettid);
	
	if(rrulib_log_convert == 0)
	{
		return;
	}
	rrulib_log_convert_counter++;
	len = snprintf(str,sizeof(str),"%d\t%s\t%s\ttid-%d\t",rrulib_log_convert_counter,device,mode,self);
	va_start(args, format);	/* pgr0039 */
	len +=  vsnprintf( &str[len] , sizeof(str) - len, format, args);
	va_end(args);
	fprintf(stdout, "%s",str);
	fflush(stdout);
	return;
}

void rrulib_usage(char* argv)
{
	printf("%s command usage \n",argv);
	printf(" * %s <scripts file name> \n",argv);
	return;
}

/**********************************************************************************************/
/* Local routines                                                                             */
/**********************************************************************************************/
static int rrulib_mrb_to_value_p1(mrb_value p1 , unsigned long* dev)
{
	if( sscanf( RSTRING_PTR(p1), "%08lx", dev) == EOF ){
		return(-1);
	}
	return 0;
}

static int rrulib_mrb_to_value_p2(mrb_value p1 , mrb_value p2 , unsigned long* dev)
{
	if( sscanf( RSTRING_PTR(p1), "%08lx", dev) == EOF ){
		return(-1);
	}
	if( sscanf( RSTRING_PTR(p2), "%08lx", (dev+1)) == EOF ){
		return(-1);
	}
	return 0;
}

static int rrulib_mrb_to_value_p3(mrb_value p1 , mrb_value p2 , mrb_value p3 , unsigned long* dev)
{
	if( sscanf( RSTRING_PTR(p1), "%08lx", dev) == EOF ){
		return(-1);
	}
	if( sscanf( RSTRING_PTR(p2), "%08lx", (dev+1)) == EOF ){
		return(-1);
	}
	if( sscanf( RSTRING_PTR(p3), "%08lx", (dev+2)) == EOF ){
		return(-1);
	}
	return 0;
}

static int rrulib_mrb_to_value_p4(mrb_value p1 , mrb_value p2 , mrb_value p3 ,mrb_value p4 , unsigned long* dev)
{
	if( sscanf( RSTRING_PTR(p1), "%08lx", dev) == EOF ){
		return(-1);
	}
	if( sscanf( RSTRING_PTR(p2), "%08lx", (dev+1)) == EOF ){
		return(-1);
	}
	if( sscanf( RSTRING_PTR(p3), "%08lx", (dev+2)) == EOF ){
		return(-1);
	}
	if( sscanf( RSTRING_PTR(p4), "%08lx", (dev+3)) == EOF ){
		return(-1);
	}
	return 0;
}

static void rrulib_init(mrb_state* mrb)
{
	/* 必要に応じて設定 */
	rrulib_pagesize = getpagesize();
	rrulib_flg_dbg = 0;
	rrulib_dmsg_counter = 0;
	rrulib_log_convert = 0;
	rrulib_dmsg("Init \n");
	return;
}

static void device_clear_stop(unsigned int mode)
{
	struct	timespec req;
	struct	timespec rem;

	rrulib_clear_flg[mode] = 0;
	req.tv_sec = 0;
	req.tv_nsec = 10*1000*1000;
	rem.tv_sec = 0;
	rem.tv_nsec = 0;
	nanosleep(&req, &rem);
	return;
}

static void* device_clear_start(void * arg)
{
	struct	timespec req;
	struct	timespec rem;
	t_rrulib_dev* dev = (t_rrulib_dev*)arg;
	char*	   name;

	if(dev->mode == 0){
		rrulib_dmsg("clear thread start %08x %08x %08x<-%08x %08xmsec \n",dev->mode,dev->kind,dev->addr,dev->val,dev->msec);
	}else{
		rrulib_dmsg("clear thread start %08x %08x %08x %08x<-%08x %08xmsec \n",dev->mode,dev->kind,dev->ofs,dev->addr,dev->val,dev->msec);
	}
	rrulib_clear_flg[dev->mode] = 1;
	while(1)
	{
		req.tv_sec = dev->msec/1000;
		req.tv_nsec = ((dev->msec % 1000) * 1000 * 1000);
		rem.tv_sec = 0;
		rem.tv_nsec = 0;
		nanosleep(&req, &rem);
		switch(dev->mode)
		{
			case 0:
				rrulib_reg_setval(dev->addr,dev->val,&name);
				break;
			case 1:
				rrulib_spi_setval(dev->ofs,dev->kind,dev->addr,dev->val,&name);
				break;
			default:
				return(NULL);
				break;
		}
		if(rrulib_clear_flg[dev->mode] == 0)
		{
			break;
		}
	}
	free(dev);
	rrulib_dmsg("finish\n");
	return(NULL);
}

int f_rrulib_memchecker( unsigned char* chkbufp )
{
	unsigned char wrkbufp[rrulib_pagesize];
	unsigned long	          chkaddr = (unsigned long)(chkbufp) - ((unsigned long)(chkbufp) % rrulib_pagesize);		/* page size alignment */
	return mincore((void*)chkaddr,1,wrkbufp);
}
#define rrulib_memchecker(addr) f_rrulib_memchecker(addr)

/**********************************************************************************************/
/* Script routines                                                                            */
/**********************************************************************************************/
/* ------------------------------------------------------------------------------------------ */
/* Common                                                                                     */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_set_table(char* name , mrb_state* mrb, mrb_value self,void* dl_handle)
{
	mrb_value		tblname;
	mrb_value		ofs;
	mrb_value		size;
	mrb_value		val;
	void*			table;
	t_rrulib_dev	dev;
	char			val_str[64];
	int				msize;
	mrb_value		rtn;

	mrb_get_args(mrb, "SSSS",&tblname,&ofs,&size,&val);
	if ((table = dlsym (dl_handle, RSTRING_PTR(tblname))) == NULL)
	{
		fprintf (stderr, "%s\n", dlerror());
		return mrb_false_value();
	}
	memset(&dev,0,sizeof(dev));
	if( rrulib_mrb_to_value_p3(val,ofs,size,&(dev.val)))
	{
		return mrb_false_value();
	}
	switch(dev.size)
	{
		case 1:
			*(unsigned char*)(table + dev.ofs) = (unsigned char)dev.val;
			break;
		case 2:
			*(unsigned short*)(table + dev.ofs) = (unsigned short)dev.val;
			break;
		case 4:
			*(unsigned int*)(table + dev.ofs) = (unsigned int)dev.val;
			break;
		default:
			memset( (unsigned char*)(table + dev.ofs) , (unsigned char)dev.val , dev.size);
			break;
	}
	f_rrulib_dmsg(name, __func__ ,"%s<ofs:%s><size:%s> <- %s \n",RSTRING_PTR(tblname),RSTRING_PTR(ofs),RSTRING_PTR(size),RSTRING_PTR(val));
	msize = snprintf(val_str, sizeof(val_str), "%lx", (unsigned long)(table + dev.ofs));
	rtn = mrb_str_new(mrb, val_str, msize);
	return (rtn);
}

mrb_value rrulib_get_table(char* name , mrb_state* mrb, mrb_value self,void* dl_handle)
{
	mrb_value		tblname;
	mrb_value		ofs;
	mrb_value		size;
	unsigned int	val = 0;
	void*			table;
	t_rrulib_dev	dev;
	char			val_str[64];
	int				msize;
	mrb_value		rtn;
	
	mrb_get_args(mrb, "SSS",&tblname,&ofs,&size);

	if ((table = dlsym (dl_handle, RSTRING_PTR(tblname))) == NULL)
	{
		fprintf (stderr, "%s\n", dlerror());
		return mrb_false_value();
	}
	memset(&dev,0,sizeof(dev));
	if( rrulib_mrb_to_value_p2(ofs,size,&(dev.ofs)))
	{
		return mrb_false_value();
	}
	switch(dev.size)
	{
		case 1:
			val = *(unsigned char*)(table + dev.ofs);
			break;
		case 2:
			val = *(unsigned short*)(table + dev.ofs);
			break;
		case 4:
			val = *(unsigned int*)(table + dev.ofs);
			break;
		default:
			return mrb_false_value();
			break;
	}
	f_rrulib_dmsg(name, __func__ ,"%s<ofs:%s><size:%s> -> <%08x> \n",RSTRING_PTR(tblname),RSTRING_PTR(ofs),RSTRING_PTR(size),val);
	msize = snprintf(val_str, sizeof(val_str), "%08x", val);
	rtn = mrb_str_new(mrb, val_str, msize);
	return (rtn);
}


mrb_value rrulib_func(char* name , mrb_state* mrb, mrb_value self,void* dl_handle)
{
	mrb_value		*argv;
	int				argc;
	int				type[16];
	unsigned long	val[16];
	char			str[16][256];
	int				(*func)(int ,...);
	int				rtn;
	unsigned		count;
	char			val_str[64];
	int				size;
	mrb_value		mrtn;
	int				ssrtn;

	mrb_get_args(mrb, "*", &argv, &argc);
	if(argc < 1)
	{
		return mrb_false_value();
	}
	memset(val,0,sizeof(val));
	memset(type,0,sizeof(type));
	
	for( count = 0 ; count < argc ; count++ )
	{
		type[count] = mrb_type(argv[count]);
	}
	if(type[0] != MRB_TT_STRING )
	{
		return mrb_false_value();
	}
	for(count=0; count < argc - 1 ; count++)
	{
		switch ( type[ count + 1 ] )
		{
			case MRB_TT_FIXNUM:
				val[count] = mrb_fixnum( argv[ count + 1 ] );
				break;
			case MRB_TT_STRING:
				if(( ssrtn = sscanf( RSTRING_PTR(argv[ count + 1 ]), "%lx", &val[count])) == EOF ){
					return mrb_false_value();
				}
				if ( ssrtn == 0)
				{
					strncpy(&str[count][0],RSTRING_PTR(argv[ count + 1 ]),strlen(RSTRING_PTR(argv[ count + 1 ])));
					val[count] = (unsigned long)&str[count][0];
				}
				break;
			default:
				printf ("what is type ? %x \n",type[ count + 1 ]);
				return mrb_false_value();
				break;
		}
	}
	if ((func = dlsym (dl_handle, RSTRING_PTR(argv[0]))) == NULL)
	{
		fprintf (stderr, "%s\n", dlerror());
		return mrb_false_value();
	}
	int len = 0;
	char input[256];
	memset(input,0,sizeof(input));
	for(count=0; count < argc - 1 ; count++)
	{
		len += snprintf(&input[len],sizeof(input) - len ,"0x%lx,",val[count]);
	}
	if((argc - 1) > 0)
	{
		input[len - 1] = '\0';
	}
	
	f_rrulib_dmsg(name, __func__ ,"%s(%s)\n",RSTRING_PTR(argv[0]),input);
	rtn = (func)(val[0],val[1],val[2],val[3],val[4],val[5],val[6],val[7],val[8],val[9],val[10],val[11],val[12],val[13],val[14],val[15] );
	f_rrulib_dmsg(name,  __func__ ,"return(0x%08x);\n",rtn);
	size = snprintf(val_str, sizeof(val_str), "%08x", rtn);
	mrtn = mrb_str_new(mrb, val_str, size);
	return (mrtn);
}

/* ------------------------------------------------------------------------------------------ */
/* Debug display ONOFF                                                                        */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_dmsg_onoff(mrb_state* mrb, mrb_value self)
{
	mrb_value onoff;
	int     level;

	mrb_get_args(mrb, "S", &onoff );
	if((level = rrulib_name_to_level(RSTRING_PTR(onoff))) == -1)
	{
		rrulib_flg_dbg = 0;
	}
	else
	{
		rrulib_flg_dbg |= level;
	}
	return (mrb_fixnum_value(level));
}
/* ------------------------------------------------------------------------------------------ */
/* Debug convert ONOFF                                                                        */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_dconvert(mrb_state* mrb, mrb_value self)
{
	mrb_value onoff;

	mrb_get_args(mrb, "S", &onoff );
	if(!strcmp( RSTRING_PTR(onoff),"ON"))
	{
		rrulib_log_convert = 1;
	}
	else if(!strcmp( RSTRING_PTR(onoff),"OFF"))
	{
		rrulib_log_convert = 0;
	}
	else
	{
		;
	}
	return (mrb_fixnum_value(rrulib_log_convert));
}

/* ------------------------------------------------------------------------------------------ */
/* Require                                                                                    */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_require(mrb_state* mrb, mrb_value self)
{
	mrb_value  str;
	mrbc_context *ctxt;
	FILE   *fp;
	mrb_value value;

	mrb_get_args(mrb, "S", &str );

	rrulib_dmsg("Require \"%s\"\n",RSTRING_PTR(str));

	if((fp = fopen(RSTRING_PTR(str),"r")) == NULL)
	{
		rrulib_dmsg("Require file opem error \n");
		return mrb_false_value();
	}
	if((ctxt = mrbc_context_new(mrb)) == NULL)
	{
		rrulib_dmsg("Require ctx ng \n");
		fclose(fp);
		return mrb_false_value();
	}
	mrbc_filename(mrb , ctxt , RSTRING_PTR(str));
	value = mrb_load_file_cxt(mrb, fp, ctxt);
	if(mrb_undef_p(value) != 0)
	{
		rrulib_dmsg("Require result %X \n",mrb_undef_p(value));
	}
	mrbc_context_free(mrb,ctxt);
	fclose(fp);
	return mrb_true_value();
}

/* ------------------------------------------------------------------------------------------ */
/* Debug display ONOFF                                                                        */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_addr_table(char* name , mrb_state* mrb, mrb_value self,void* dl_handle)
{
	mrb_value		tblname;
	void*			table;
	char			val_str[64];
	int				msize;
	mrb_value		rtn;

	mrb_get_args(mrb, "S",&tblname);
	if ((table = dlsym (dl_handle, RSTRING_PTR(tblname))) == NULL)
	{
		fprintf (stderr, "%s\n", dlerror());
		return mrb_false_value();
	}
	f_rrulib_dmsg(name, __func__ ,"%s -> %lx \n",RSTRING_PTR(tblname),(unsigned long)table);
	msize = snprintf(val_str, sizeof(val_str), "%lx", (unsigned long)(table));
	rtn = mrb_str_new(mrb, val_str, msize);
	return (rtn);
}

/* ------------------------------------------------------------------------------------------ */
/* FPGA                                                                                       */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_reginit(mrb_state* mrb, mrb_value self)
{
	mrb_value    val;
	unsigned long rval;
	mrb_get_args(mrb, "S", &val );

	if( rrulib_mrb_to_value_p1(val,&rval) )
	{
		return(mrb_false_value());
	}
	rrulib_reg_init(rval);
	rrulib_dmsg("all %08x\n",rval);
	return (mrb_true_value());
}

mrb_value rrulib_regread(mrb_state* mrb, mrb_value self)
{
	mrb_value    addr;
	int        val;
	char*	   name;
	char val_str[64];
	int size;
	mrb_value rtn;
	t_rrulib_dev  dev;

	memset(&dev,0,sizeof(t_rrulib_dev));
	mrb_get_args(mrb, "S", &addr );
	if( rrulib_mrb_to_value_p1(addr,&(dev.addr)) )
	{
		return(mrb_false_value());
	}
	val = rrulib_reg_getval(dev.addr,&name);
	rrulib_dmsg("%08x->%08x[%s]\n",dev.addr,val,name);
	size = snprintf(val_str, sizeof(val_str), "%08x", val);
	rtn = mrb_str_new(mrb, val_str, size);
	return (rtn);
}


mrb_value rrulib_regwrite(mrb_state* mrb, mrb_value self)
{
	mrb_value    addr;
	mrb_value    val;
	char*	   name;
	t_rrulib_dev  dev;

	memset(&dev,0,sizeof(t_rrulib_dev));
	mrb_get_args(mrb, "SS", &addr , &val);
	if( rrulib_mrb_to_value_p2(addr,val,&(dev.addr)) ){
		return(mrb_false_value());
	}
	rrulib_reg_setval(dev.addr,dev.val,&name);
	rrulib_dmsg("%08x<-%08x[%s]\n",dev.addr,dev.val,name);
	return (mrb_true_value());
}

mrb_value rrulib_regclear(mrb_state* mrb, mrb_value self)
{
	mrb_value    addr;
	mrb_value    val;
	mrb_value    msec;
	pthread_t  thread;
	t_rrulib_dev* dev = NULL;
	mrb_value    rtn;
	char val_str[64];
	int size;
	
	mrb_get_args(mrb, "SS|S", &addr,&val,&msec);
	if((dev = (t_rrulib_dev*)malloc(sizeof(t_rrulib_dev))) == NULL)
	{
		return(mrb_false_value());
	}
	memset(dev,0,sizeof(t_rrulib_dev));
	if( rrulib_mrb_to_value_p2(addr,val,&(dev->addr)) ){
		free(dev);
		return(mrb_false_value());
	}
	dev->msec = 1;
	if( mrb_type(msec) == MRB_TT_STRING )
	{
		if( rrulib_mrb_to_value_p1(msec,&(dev->msec)) ){
			free(dev);
			return(mrb_false_value());
		}
	}
	dev->mode = 0;
	rrulib_dmsg("%08x,%08x,%08x %08x\n",dev->mode,dev->addr,dev->val,dev->msec);
	pthread_create(&thread, NULL, device_clear_start, (void*)dev);
	size = snprintf(val_str, sizeof(val_str), "%lx", thread);
	rtn = mrb_str_new(mrb, val_str, size);
	return (rtn);
}

mrb_value rrulib_regclearstop(mrb_state* mrb, mrb_value self)
{
	rrulib_dmsg("%x \n" , 0);
	device_clear_stop(0);
	return (mrb_true_value());
}

/* ------------------------------------------------------------------------------------------ */
/* SPI                                                                                        */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_spiinit(mrb_state* mrb, mrb_value self)
{
	mrb_value    val;
	unsigned long rval;
	mrb_get_args(mrb, "S", &val );

	if( rrulib_mrb_to_value_p1(val,&rval) )
	{
		return(mrb_false_value());
	}
	rrulib_spi_init(rval);
	rrulib_dmsg("all %08x\n",rval);
	return (mrb_true_value());
}

mrb_value rrulib_spiread(unsigned int spikind,mrb_state* mrb, mrb_value self)
{
	mrb_value    kind;
	mrb_value    addr;
	int        val;
	char*	   name;
	char val_str[64];
	int size;
	mrb_value rtn;
	t_rrulib_dev  dev;

	memset(&dev,0,sizeof(t_rrulib_dev));

	mrb_get_args(mrb, "SS", &kind,&addr );
	if( rrulib_mrb_to_value_p2(kind,addr,&(dev.kind)))
	{
		return(mrb_false_value());
	}
	val = rrulib_spi_getval(spikind,dev.kind,dev.addr,&name);
	rrulib_dmsg("%08x->%08x[%s(%08x_%08x)]\n",dev.addr,val,name,spikind,dev.kind);
	size = snprintf(val_str, sizeof(val_str), "%08x", val);
	rtn = mrb_str_new(mrb, val_str, size);
	return (rtn);
}

mrb_value rrulib_spi0read(mrb_state* mrb, mrb_value self){
	return rrulib_spiread(0,mrb,self);
}

mrb_value rrulib_spi1read(mrb_state* mrb, mrb_value self){
	return rrulib_spiread(1,mrb,self);
}

mrb_value rrulib_spi2read(mrb_state* mrb, mrb_value self){
	return rrulib_spiread(2,mrb,self);
}

mrb_value rrulib_spi3read(mrb_state* mrb, mrb_value self){
	return rrulib_spiread(3,mrb,self);
}

mrb_value rrulib_spiwrite(unsigned int spikind,mrb_state* mrb, mrb_value self)
{
	mrb_value    kind;
	mrb_value    addr;
	mrb_value    val;
	t_rrulib_dev  dev;
	char*        name;

	memset(&dev,0,sizeof(t_rrulib_dev));

	mrb_get_args(mrb, "SSS", &kind,&addr,&val );
	if( rrulib_mrb_to_value_p3(kind,addr,val,&(dev.kind)) ){
		return(mrb_false_value());
	}
	rrulib_spi_setval(spikind,dev.kind,dev.addr,dev.val,&name);
	rrulib_dmsg("%08x<-%08x[%s(%08x.%08x)]\n",dev.addr,dev.val,name,spikind,dev.kind);
	return (mrb_true_value());
}

mrb_value rrulib_spi0write(mrb_state* mrb, mrb_value self){
	return rrulib_spiwrite(0,mrb,self);
}

mrb_value rrulib_spi1write(mrb_state* mrb, mrb_value self){
	return rrulib_spiwrite(1,mrb,self);
}

mrb_value rrulib_spi2write(mrb_state* mrb, mrb_value self){
	return rrulib_spiwrite(2,mrb,self);
}

mrb_value rrulib_spi3write(mrb_state* mrb, mrb_value self){
	return rrulib_spiwrite(3,mrb,self);
}


mrb_value rrulib_spiclear(unsigned int spikind,mrb_state* mrb, mrb_value self)
{
	mrb_value    kind;
	mrb_value    addr;
	mrb_value    val;
	mrb_value    msec;
	pthread_t  thread;
	t_rrulib_dev* dev = NULL;
	mrb_value    rtn;
	char val_str[64];
	int size;

	mrb_get_args(mrb, "SSS|S", &kind,&addr,&val,&msec);
	if((dev = (t_rrulib_dev*)malloc(sizeof(t_rrulib_dev))) == NULL)
	{
		return(mrb_false_value());
	}
	memset(dev,0,sizeof(t_rrulib_dev));
	if( rrulib_mrb_to_value_p3(kind,addr,val,&(dev->kind)) ){
		free(dev);
		return(mrb_false_value());
	}
	dev->msec = 1;
	if( mrb_type(msec) == MRB_TT_STRING )
	{
		if( rrulib_mrb_to_value_p1(msec,&(dev->msec)) ){
			free(dev);
			return(mrb_false_value());
		}
	}
	dev->mode = 1;
	rrulib_dmsg("%08x,%08x,%08x,%08x,%08x,%08x %08x\n",dev->mode,spikind,dev->kind,dev->addr,dev->val,dev->msec);
	dev->ofs=spikind;
	pthread_create(&thread, NULL, device_clear_start, (void*)dev);
	size = snprintf(val_str, sizeof(val_str), "%lx", thread);
	rtn = mrb_str_new(mrb, val_str, size);
	return (rtn);
}


mrb_value rrulib_spi0clear(mrb_state* mrb, mrb_value self){
	return rrulib_spiclear(0,mrb,self);
}

mrb_value rrulib_spi1clear(mrb_state* mrb, mrb_value self){
	return rrulib_spiclear(1,mrb,self);
}

mrb_value rrulib_spi2clear(mrb_state* mrb, mrb_value self){
	return rrulib_spiclear(2,mrb,self);
}

mrb_value rrulib_spi3clear(mrb_state* mrb, mrb_value self){
	return rrulib_spiclear(3,mrb,self);
}

mrb_value rrulib_spiclearstop(mrb_state* mrb, mrb_value self)
{
	rrulib_dmsg("%x \n" , 1);
	device_clear_stop(1);
	return (mrb_true_value());
}

/* ------------------------------------------------------------------------------------------ */
/* Symbol                                                                                     */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_symalloc(mrb_state* mrb, mrb_value self)
{
	mrb_value    sym;
	mrb_value    size;
	mrb_value    rtn;
	char         val_str[64];
	int          rsize;
	unsigned long val;

	mrb_get_args(mrb, "SS", &sym , &size );
	if( rrulib_mrb_to_value_p1(size,&val) ){
		return(mrb_false_value());
	}
	rrulib_dmsg("%s,%08x\n",RSTRING_PTR(sym),val);
	rsize = snprintf(val_str, sizeof(val_str), "%lx", (unsigned long)rrulib_sym_alloc(RSTRING_PTR(sym),val));
	rtn = mrb_str_new(mrb, val_str, rsize);
	return (rtn);
}

mrb_value rrulib_symset(mrb_state* mrb, mrb_value self)
{
	mrb_value    sym;
	mrb_value    ofs;
	mrb_value    size;
	mrb_value    val;
	mrb_value    rtn;
	char         val_str[64];
	int          rsize;
	unsigned long rval[3];

	mrb_get_args(mrb, "SSSS", &sym , &ofs , &size , &val);

	if( rrulib_mrb_to_value_p3(ofs,size,val,rval) ){
		return(mrb_false_value());
	}
	rrulib_dmsg("%s,%08x,%08x,%08x \n",RSTRING_PTR(sym),rval[0],rval[1],rval[2]);
	rsize = snprintf(val_str, sizeof(val_str), "%lx", (unsigned long)rrulib_sym_set( RSTRING_PTR(sym),rval[0],rval[1],rval[2]));
	rtn = mrb_str_new(mrb, val_str, rsize);
	return (rtn);
}


mrb_value rrulib_symset_string(mrb_state* mrb, mrb_value self)
{
	mrb_value    sym;
	mrb_value    ofs;
	mrb_value    size;
	mrb_value    val;
	mrb_value    rtn;
	char         val_str[64];
	int          rsize;
	unsigned long rval[3];

	mrb_get_args(mrb, "SSSS", &sym , &ofs , &size , &val);

	if( rrulib_mrb_to_value_p2(ofs,size,rval) ){
		return(mrb_false_value());
	}
	rrulib_dmsg("%s,%08x,%08x,%s\n",RSTRING_PTR(sym),rval[0],rval[1],RSTRING_PTR(val));
	rsize = snprintf(val_str, sizeof(val_str), "%lx", (unsigned long)rrulib_sym_set_s( RSTRING_PTR(sym),rval[0],rval[1],RSTRING_PTR(val)));
	rtn = mrb_str_new(mrb, val_str, rsize);
	return (rtn);
}

mrb_value rrulib_symget(mrb_state* mrb, mrb_value self)
{
	mrb_value    sym;
	mrb_value    ofs;
	mrb_value    size;
	mrb_value    rtn;
	char         val_str[64];
	int          rsize;
	unsigned long rval[2];

	mrb_get_args(mrb, "SSS", &sym , &ofs , &size );

	if( rrulib_mrb_to_value_p2(ofs,size,rval) ){
		return(mrb_false_value());
	}
	rrulib_dmsg("%s,%08x,%08x \n",RSTRING_PTR(sym),rval[0],rval[1]);
	rsize = snprintf(val_str, sizeof(val_str), "%08x", (unsigned int)rrulib_sym_get( RSTRING_PTR(sym),rval[0],rval[1]));
	rtn = mrb_str_new(mrb, val_str, rsize);
	return (rtn);
}


mrb_value rrulib_symaddr(mrb_state* mrb, mrb_value self)
{
	mrb_value    sym;
	mrb_value    rtn;
	char         val_str[64];
	int          rsize;

	mrb_get_args(mrb, "S", &sym );
	rrulib_dmsg("%s\n",RSTRING_PTR(sym));
	rsize = snprintf(val_str, sizeof(val_str), "%lx", (unsigned long)rrulib_sym_getaddr(RSTRING_PTR(sym)));
	rtn = mrb_str_new(mrb, val_str, rsize);
	return (rtn);
}

mrb_value rrulib_symfree(mrb_state* mrb, mrb_value self)
{
	mrb_value    sym;
	mrb_get_args(mrb, "S", &sym );
	rrulib_dmsg("%s\n",RSTRING_PTR(sym));
	rrulib_sym_free(RSTRING_PTR(sym));
	return(mrb_true_value());
}

mrb_value rrulib_symallfree(mrb_state* mrb, mrb_value self)
{
	rrulib_sym_allclean();
	return(mrb_true_value());
}

/* ------------------------------------------------------------------------------------------ */
/* Shared Mem                                                                                 */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_shmset(mrb_state* mrb, mrb_value self)
{
	mrb_value    sym;
	mrb_value    ofs;
	mrb_value    size;
	mrb_value    val;
	mrb_value    rtn;
	char         val_str[64];
	int          rsize;
	unsigned long rval[3];

	mrb_get_args(mrb, "SSSS", &sym , &ofs , &size , &val);

	if( rrulib_mrb_to_value_p3(ofs,size,val,rval) ){
		return(mrb_false_value());
	}
	rrulib_dmsg("%s,%08x,%08x,%08x \n",RSTRING_PTR(sym),rval[0],rval[1],rval[2]);
	rsize = snprintf(val_str, sizeof(val_str), "%lx", (unsigned long)rrulib_shm_set( RSTRING_PTR(sym),rval[0],rval[1],rval[2]));
	rtn = mrb_str_new(mrb, val_str, rsize);
	return (rtn);
}

mrb_value rrulib_shmget(mrb_state* mrb, mrb_value self)
{
	mrb_value    sym;
	mrb_value    ofs;
	mrb_value    size;
	mrb_value    rtn;
	char         val_str[64];
	int          rsize;
	unsigned long rval[2];

	mrb_get_args(mrb, "SSS", &sym , &ofs , &size );

	if( rrulib_mrb_to_value_p2(ofs,size,rval) ){
		return(mrb_false_value());
	}
	rrulib_dmsg("%s,%08x,%08x \n",RSTRING_PTR(sym),rval[0],rval[1]);
	rsize = snprintf(val_str, sizeof(val_str), "%08x", (unsigned int)rrulib_shm_get( RSTRING_PTR(sym),rval[0],rval[1]));
	rtn = mrb_str_new(mrb, val_str, rsize);
	return (rtn);
}


mrb_value rrulib_shmaddr(mrb_state* mrb, mrb_value self)
{
	mrb_value    sym;
	mrb_value    rtn;
	char         val_str[64];
	int          rsize;

	mrb_get_args(mrb, "S", &sym );
	rrulib_dmsg("%s\n",RSTRING_PTR(sym));
	rsize = snprintf(val_str, sizeof(val_str), "%lx", (unsigned long)rrulib_shm_getaddr(RSTRING_PTR(sym)));
	rtn = mrb_str_new(mrb, val_str, rsize);
	return (rtn);
}

/* ------------------------------------------------------------------------------------------ */
/* QSPI                                                                                       */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_qspiinit(mrb_state* mrb, mrb_value self)
{
	mrb_value    val;
	unsigned long rval;
	mrb_get_args(mrb, "S", &val );

	if( rrulib_mrb_to_value_p1(val,&rval) )
	{
		return(mrb_false_value());
	}
	rrulib_qspi_init(rval);
	rrulib_dmsg("all %08x\n",rval);
	return (mrb_true_value());
}

mrb_value rrulib_qspiinit_file(mrb_state* mrb, mrb_value self)
{
	mrb_value    face;
	mrb_value    ofs;
	mrb_value    size;
	mrb_value    filename;
	unsigned long val[3];
	int          fsize;
		
	mrb_get_args(mrb, "SSSS", &face, &ofs, &size, &filename );

	if ( rrulib_mrb_to_value_p3(face, ofs, size, val))
	{
		return(mrb_false_value());
	}
	fsize = rrulib_qspi_fileinit(val[0],val[1],val[2],RSTRING_PTR(filename));
	rrulib_dmsg("all %08x\n",fsize);
	return (mrb_true_value());
}

mrb_value rrulib_qspiwrite(mrb_state* mrb, mrb_value self)
{
	mrb_value    face;
	mrb_value    ofs;
	mrb_value    val;
	unsigned long rval[3];
	char*		name;

	mrb_get_args(mrb, "SSS", &face, &ofs ,&val );
	if ( rrulib_mrb_to_value_p3(face, ofs, val,rval))
	{
		return(mrb_false_value());
	}
	rrulib_qspi_set(rval[0],rval[1],sizeof(rval[2]),(unsigned char*)&rval[2],&name);
	rrulib_dmsg("%08x,%08x<-%08x [%s]\n",rval[0],rval[1],rval[2],name);
	return (mrb_true_value());
}

mrb_value rrulib_qspiread(mrb_state* mrb, mrb_value self)
{
	mrb_value    face;
	mrb_value    ofs;
	unsigned long rval[3];
	unsigned long edval;
	char*		name;
	mrb_value    rtn;
	char         val_str[64];
	int          rsize;

	mrb_get_args(mrb, "SS", &face, &ofs  );
	if ( rrulib_mrb_to_value_p2(face, ofs,rval))
	{
		return(mrb_false_value());
	}
	rrulib_qspi_get(rval[0],rval[1],sizeof(rval[2]),(unsigned char*)&rval[2],&name);
	edval = ((rval[2] & 0xFF000000) >> 24) | ((rval[2] & 0x00FF0000) >> 8) | ((rval[2] & 0x0000FF00) << 8) | ((rval[2] & 0x000000FF) << 24);
	rrulib_dmsg("%08x,%08x->%08x [%s]\n",rval[0],rval[1],edval,name);
	rsize = snprintf(val_str, sizeof(val_str), "%lx", edval);
	rtn = mrb_str_new(mrb, val_str, rsize);
	return (rtn);
}

/* ------------------------------------------------------------------------------------------ */
/* I2C                                                                                        */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_i2cinit(mrb_state* mrb, mrb_value self)
{
	mrb_value    val;
	unsigned long rval;
	mrb_get_args(mrb, "S", &val );
	if( rrulib_mrb_to_value_p1(val,&rval) )
	{
		return(mrb_false_value());
	}
	rrulib_i2c_init(rval);
	rrulib_dmsg("all %08x\n",rval);
	return (mrb_true_value());
}

static mrb_value rrulib_i2cread(unsigned int i2ckind , mrb_state* mrb, mrb_value self)
{
	mrb_value    kind;
	mrb_value    addr;
	int        val;
	char*	   name;
	char val_str[64];
	int size;
	mrb_value rtn;
	t_rrulib_dev  dev;

	memset(&dev,0,sizeof(t_rrulib_dev));
	mrb_get_args(mrb, "SS", &kind,&addr );
	if( rrulib_mrb_to_value_p2(kind,addr,&(dev.kind)))
	{
		return(mrb_false_value());
	}
	val = rrulib_i2c_getval(i2ckind,dev.kind,dev.addr,&name);
	rrulib_dmsg("i2c%x %08x->%08x[%s(%08x)]\n",i2ckind,dev.addr,val,name,dev.kind);
	size = snprintf(val_str, sizeof(val_str), "%08x", val);
	rtn = mrb_str_new(mrb, val_str, size);
	return (rtn);
}

mrb_value rrulib_i2c0read(mrb_state* mrb, mrb_value self)
{
	return (rrulib_i2cread(0,mrb,self));
}

mrb_value rrulib_i2c1read(mrb_state* mrb, mrb_value self)
{
	return (rrulib_i2cread(1,mrb,self));
}

mrb_value rrulib_i2c2read(mrb_state* mrb, mrb_value self)
{
	return (rrulib_i2cread(2,mrb,self));
}

static mrb_value rrulib_i2cwrite(unsigned int i2ckind , mrb_state* mrb, mrb_value self)
{
	mrb_value    kind;
	mrb_value    addr;
	mrb_value    val;
	t_rrulib_dev  dev;
	char*        name;

	memset(&dev,0,sizeof(t_rrulib_dev));

	mrb_get_args(mrb, "SSS", &kind,&addr,&val );
	if( rrulib_mrb_to_value_p3(kind,addr,val,&(dev.kind)) ){
		return(mrb_false_value());
	}
	rrulib_i2c_setval(i2ckind,dev.kind,dev.addr,dev.val,&name);
	rrulib_dmsg("i2c%x %08x<-%08x[%s(%08x)]\n",i2ckind,dev.addr,dev.val,name,dev.kind);
	return (mrb_true_value());
}

mrb_value rrulib_i2c0write(mrb_state* mrb, mrb_value self)
{
	return (rrulib_i2cwrite(0,mrb, self));
}

mrb_value rrulib_i2c1write(mrb_state* mrb, mrb_value self)
{
	return (rrulib_i2cwrite(1,mrb, self));
}

mrb_value rrulib_i2c2write(mrb_state* mrb, mrb_value self)
{
	return (rrulib_i2cwrite(2,mrb, self));
}

mrb_value rrulib_eepread(mrb_state* mrb, mrb_value self)
{
	mrb_value  addr;
	int        val;
	char*	   name;
	char val_str[64];
	int size;
	mrb_value rtn;
	t_rrulib_dev  dev;

	memset(&dev,0,sizeof(t_rrulib_dev));
	mrb_get_args(mrb, "S",&addr );
	if( rrulib_mrb_to_value_p1(addr,&(dev.addr)))
	{
		return(mrb_false_value());
	}
	val = rrulib_i2c_getval(0,dev.kind,dev.addr,&name);
	rrulib_dmsg("i2c%x %08x->%08x[%s(%08x)]\n",0,dev.addr,val,name,dev.kind);
	size = snprintf(val_str, sizeof(val_str), "%08x", val);
	rtn = mrb_str_new(mrb, val_str, size);
	return (rtn);
}

static mrb_value rrulib_eepwrite(mrb_state* mrb, mrb_value self)
{
	mrb_value    addr;
	mrb_value    val;
	t_rrulib_dev  dev;
	char*        name;

	memset(&dev,0,sizeof(t_rrulib_dev));

	mrb_get_args(mrb, "SS", &addr,&val );
	if( rrulib_mrb_to_value_p2(addr,val,&(dev.addr)) ){
		return(mrb_false_value());
	}
	rrulib_i2c_setval(0,dev.kind,dev.addr,dev.val,&name);
	rrulib_dmsg("i2c%x %08x<-%08x[%s(%08x)]\n",0,dev.addr,dev.val,name,dev.kind);
	return (mrb_true_value());
}

/* ------------------------------------------------------------------------------------------ */
/* Call result                                                                                */
/* ------------------------------------------------------------------------------------------ */
static mrb_value rrulib_drc_init(mrb_state* mrb, mrb_value self,int flg)
{
	mrb_value    name;
	mrb_get_args(mrb, "S", &name);
	if( flg == 0){ rrulib_callresult_init(RSTRING_PTR(name)); }
	else if(flg == 1){ oal_callresult_init(RSTRING_PTR(name)); }
	else if(flg == 2){ hal_callresult_init(RSTRING_PTR(name)); }
	else{ rfic_callresult_init(RSTRING_PTR(name)); }
	rrulib_dmsg("%s result init \n",RSTRING_PTR(name));
	return (mrb_true_value());
}

static mrb_value rrulib_drc_set(mrb_state* mrb, mrb_value self,int flg)
{
	mrb_value    name;
	mrb_value    val;
	long          rval;

	mrb_get_args(mrb, "SS", &name,&val);
	if( rrulib_mrb_to_value_p1(val,(unsigned long*)&rval) ){
		return(mrb_false_value());
	}
	if( flg == 0){ rrulib_callresult_set(RSTRING_PTR(name),rval); }
	else if( flg == 1){ oal_callresult_set(RSTRING_PTR(name),rval); }
	else if( flg == 2){ hal_callresult_set(RSTRING_PTR(name),rval); }
	else{ rfic_callresult_set(RSTRING_PTR(name),rval); }
	rrulib_dmsg("%s result set %x \n",RSTRING_PTR(name),rval);
	return (mrb_true_value());
}

static mrb_value rrulib_drc_get(mrb_state* mrb, mrb_value self,int flg)
{
	mrb_value    name;
	int          rval;
	char val_str[64];
	int size;
	mrb_value rtn;

	mrb_get_args(mrb, "S", &name);
	if( flg == 0){ rval = rrulib_callresult_get(RSTRING_PTR(name)); }
	else if( flg == 1){ rval = oal_callresult_get(RSTRING_PTR(name)); }
	else if( flg == 2){ rval = hal_callresult_get(RSTRING_PTR(name)); }
	else{ rval = rfic_callresult_get(RSTRING_PTR(name)); }
	rrulib_dmsg("%s result get %x \n",RSTRING_PTR(name),rval);
	size = snprintf(val_str, sizeof(val_str), "%08x",rval);
	rtn = mrb_str_new(mrb, val_str, size);
	return (rtn);
}

/* system call */
static mrb_value rrulib_dsrc_init(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_init( mrb , self , 0);
}

static mrb_value rrulib_dsrc_set(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_set( mrb , self , 0);
}

static mrb_value rrulib_dsrc_get(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_get( mrb , self , 0);
}

/* oal call */
static mrb_value rrulib_dbres_init_oal(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_init( mrb , self , 1);
}

static mrb_value rrulib_dbres_set_oal(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_set( mrb , self , 1);
}

static mrb_value rrulib_dbres_get_oal(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_get( mrb , self , 1);
}

/* hal call */
static mrb_value rrulib_dbres_init_hal(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_init( mrb , self , 2);
}

static mrb_value rrulib_dbres_set_hal(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_set( mrb , self , 2);
}

static mrb_value rrulib_dbres_get_hal(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_get( mrb , self , 2);
}

/* rfic call */
static mrb_value rrulib_dbres_init_rfic(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_init( mrb , self , 3);
}

static mrb_value rrulib_dbres_set_rfic(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_set( mrb , self , 3);
}

static mrb_value rrulib_dbres_get_rfic(mrb_state* mrb, mrb_value self)
{
	return rrulib_drc_get( mrb , self , 3);
}


/* ------------------------------------------------------------------------------------------ */
/* Sleep                                                                                      */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_sleep(mrb_state* mrb, mrb_value self)
{
	mrb_int sec = 0;
	mrb_int nsec = 0;
	struct timespec stime;

	mrb_get_args(mrb, "i|i", &sec , &nsec );
	rrulib_dmsg("Sleep %u.%09u sec \n",sec,nsec);
	stime.tv_sec = (int)sec;
	stime.tv_nsec = (int)nsec;
	nanosleep(&stime,NULL);
	return ( self );
}

/* ------------------------------------------------------------------------------------------ */
/* 数値モジュール                                                                             */
/* ------------------------------------------------------------------------------------------ */
mrb_value rrulib_val_to_val(mrb_state* mrb, mrb_value self)
{
	mrb_value  str;
	mrb_value  from;
	mrb_value  to;
	unsigned int val;
	unsigned int len;
	unsigned int count;
	char val_string[33];
	
	mrb_get_args(mrb, "SSS", &from,&to,&str);

	memset(val_string,0,sizeof(val_string));
	
	if ( strcmp("h",RSTRING_PTR(from)) == 0 )
	{
		sscanf(RSTRING_PTR(str),"%x",&val);
	}
	else if( strcmp("d",RSTRING_PTR(from)) == 0 ) 
	{
		sscanf(RSTRING_PTR(str),"%d",&val);
	}
	else if( strcmp("b",RSTRING_PTR(from)) == 0 )
	{
		strncpy(val_string,RSTRING_PTR(str),sizeof(val_string));
		len = strlen(val_string);
		val = 0;
		for(count=0 ; count < len ; count++)
		{
			if(val_string[count] == '1')
			{
				val |= (0x00000001 << (len - count - 1)) ;
			}
		}
		memset(val_string,0,sizeof(val_string));
	}
	else
	{
		return mrb_false_value();
	}
	if( strcmp("h",RSTRING_PTR(to)) == 0 ) 
	{
		len = snprintf(val_string, sizeof(val_string) , "%x", val);
	}
	else if( strcmp("d",RSTRING_PTR(to)) == 0 ) 
	{
		len = snprintf(val_string,sizeof(val_string),"%d", val);
	}
	else if( strcmp("b",RSTRING_PTR(to)) == 0 ) 
	{
		for( count= 0 ,len = 0 ; count <= 31 ; count++ ,len++)
		{
			if(((val >> count) & 0x00000001) != 0)
			{
				val_string[ 31 - count ] = '1';
			}
			else
			{
				val_string[ 31 - count ] = '0';
			}
		}
	}
	else
	{
		return mrb_false_value();
	}
	rrulib_dmsg("Val to Val %s -> %s  \n",RSTRING_PTR(str),val_string );
	return mrb_str_new(mrb, val_string, len);
}

mrb_value rrulib_string_clac(mrb_state* mrb, mrb_value self)
{
	mrb_value	str1;
	mrb_value	ope;
	mrb_value	str2;
	unsigned int var1,var2,ans;
	char rtn_str[64];
	int size;
	mrb_value rtn;

	mrb_get_args(mrb, "SSS", &str1, &ope, &str2);
	/* exchange string to number */
	if( sscanf( RSTRING_PTR(str1), "%08x", &var1) == EOF ){
		rrulib_dmsg("Invlid value is detected in face : %s\n", RSTRING_PTR(str1));
		return mrb_false_value();
	}
	if( sscanf( RSTRING_PTR(str2), "%08x", &var2) == EOF ){
		rrulib_dmsg("Invlid value is detected in addr : %s\n", RSTRING_PTR(str2));
		return mrb_false_value();
	}
	rrulib_dmsg("%08x %s %08x\n", var1, RSTRING_PTR(ope), var2);
	switch ( *RSTRING_PTR(ope) ) {
		case '+' :
		    ans = var1 + var2;
		    break;
		case '-' : 
		    ans = var1 - var2;
		    break;
		case '*' : 
		    ans = var1 * var2;
		    break;
		case '/' : 
		    ans = var1 / var2;
		    break;
		case '%' : 
		    ans = var1 % var2;
		    break;
		case '>' : 
		    ans = var1 >> var2;
		    break;
		case '<' : 
		    ans = var1 << var2;
		    break;
		case '&' : 
		    ans = var1 & var2;
		    break;
		case '|' : 
		    ans = var1 | var2;
		    break;
		default : 
		    return mrb_false_value();
	}
	/* exchange number to string */
	size = snprintf(rtn_str, sizeof(rtn_str), "%08x", ans);
	if(size < 0){
		rrulib_dmsg("Invlid value is detected\n");
		return mrb_false_value();
	}
	rtn = mrb_str_new(mrb, rtn_str, size);
	return rtn;
}

mrb_value rrulib_system(mrb_state* mrb, mrb_value self)
{
	mrb_value  str;
	char rtn_str[64];
	int size;
	mrb_value rtn;

	mrb_get_args(mrb, "S", &str);
	rrulib_dmsg("System \"%s\" \n",RSTRING_PTR(str));
	size = snprintf(rtn_str, sizeof(rtn_str), "%08x", system(RSTRING_PTR(str)));
	if(size < 0){
		rrulib_dmsg("Invlid value is detected\n");
		return mrb_false_value();
	}
	rtn = mrb_str_new(mrb, rtn_str, size);
	return rtn;
}

mrb_value rrulib_memcmp(mrb_state* mrb, mrb_value self)
{
	mrb_value	 src;
	mrb_value	 dst;
	mrb_value	 size;
	int rtn;
	unsigned long val[3];
	
	mrb_get_args(mrb, "SSS", &src,&dst,&size);	
	if( rrulib_mrb_to_value_p3(src,dst,size,(unsigned long*)&val[0]) ){
		return(mrb_false_value());
	}

	rrulib_dmsg("Memcmp:s %lx d %lx size %lx \n",val[0],val[1],val[2]);
	if((val[0] == val[1] ) || ( val[2] == 0 ))
	{
		rrulib_dmsg("Memcmp:OK\n");
		return mrb_true_value();
	}
	if(
	 ( rrulib_memchecker( (unsigned char*)(val[0] + val[2])) != 0 ) ||
	   ( rrulib_memchecker((unsigned char*)(val[1] + val[2])) != 0 )
	)
	{
		rrulib_dmsg("Memcmp:add error \n");
		return mrb_false_value();
	}
	if(( rtn = memcmp((const void*)val[0] , (const void*)val[1] , val[2]) ) != 0)
	{
		rrulib_dmsg("Memcmp: mismatch %x \n",rtn);
		return mrb_false_value();
	}
	rrulib_dmsg("Memcmp:OK\n");
	return mrb_true_value();
}

mrb_value rrulib_cmode_get(mrb_state* mrb, mrb_value self)
{
	rrulib_dmsg("rrulib_cunit_mode %d\n",rrulib_cunit_mode);
	return (mrb_fixnum_value(rrulib_cunit_mode));
}
mrb_value rrulib_cmode_set(mrb_state* mrb, mrb_value self)
{
	mrb_int mode;

	mrb_get_args(mrb, "i", &mode );
	rrulib_dmsg("mode %d rrulib_cunit_mode %d\n",mode,rrulib_cunit_mode);
	rrulib_cunit_mode = mode;
	return (mrb_fixnum_value(rrulib_cunit_mode));
}
/*!
 * @brief		モジュール登録処理
 * @note
<pre>
	スクリプト上で使用するクラス、モジュール、メソッドを登録する関数である.
</pre>
 * @param		mrb	[in]	mrb status
 * @return		N/A
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void rrulib_install(mrb_state* mrb)
{
	struct RClass * module_class;
	rrulib_init(mrb);
	rrulib_dmsg("install start \n");

	/* scriptで使用できるモジュールを登録する */
	module_class = mrb_define_module(mrb, "Core");

	/* デバッグ情報表示モジュール */
	mrb_define_module_function(mrb, module_class, "dmsg"        , rrulib_dmsg_onoff         , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "dconvert"    , rrulib_dconvert           , MRB_ARGS_REQ(1));

	/* デバッグシステムコール結果変更モジュール */
	mrb_define_module_function(mrb, module_class, "sys_rcinit"   , rrulib_dsrc_init          , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "sys_rcset"    , rrulib_dsrc_set           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "sys_rcget"    , rrulib_dsrc_get           , MRB_ARGS_REQ(1));

	/* デバッグBPF結果変更モジュール */
	mrb_define_module_function(mrb, module_class, "oal_rcinit"   , rrulib_dbres_init_oal          , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "oal_rcset"    , rrulib_dbres_set_oal           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "oal_rcget"    , rrulib_dbres_get_oal           , MRB_ARGS_REQ(1));

	mrb_define_module_function(mrb, module_class, "hal_rcinit"   , rrulib_dbres_init_hal          , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "hal_rcset"    , rrulib_dbres_set_hal           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "hal_rcget"    , rrulib_dbres_get_hal           , MRB_ARGS_REQ(1));

	mrb_define_module_function(mrb, module_class, "rfic_rcinit"   , rrulib_dbres_init_rfic          , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "rfic_rcset"    , rrulib_dbres_set_rfic           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "rfic_rcget"    , rrulib_dbres_get_rfic           , MRB_ARGS_REQ(1));


	/* 擬似I2Cモジュール */
	mrb_define_module_function(mrb, module_class, "i2cinit"       , rrulib_i2cinit            , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "eepread"       , rrulib_eepread            , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "eepwrite"      , rrulib_eepwrite           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "i2c0read"      , rrulib_i2c0read           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "i2c0write"     , rrulib_i2c0write          , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "i2c1read"      , rrulib_i2c1read           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "i2c1write"     , rrulib_i2c1write          , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "i2c2read"      , rrulib_i2c2read           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "i2c2write"     , rrulib_i2c2write          , MRB_ARGS_REQ(3));

	/* 擬似QSPIモジュール */
	mrb_define_module_function(mrb, module_class, "qspiinit"     , rrulib_qspiinit          , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "qspiinitfile" , rrulib_qspiinit_file     , MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, module_class, "qspiwrite"    , rrulib_qspiwrite         , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "qspiread"     , rrulib_qspiread          , MRB_ARGS_REQ(2));

	/* 擬似レジスタモジュール */
	mrb_define_module_function(mrb, module_class, "reginit"       , rrulib_reginit           , MRB_ARGS_REQ(1));	
	mrb_define_module_function(mrb, module_class, "regread"       , rrulib_regread           , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "regwrite"      , rrulib_regwrite          , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "regworker"     , rrulib_regclear          , MRB_ARGS_ARG(2,1));
	mrb_define_module_function(mrb, module_class, "regworkerstop" , rrulib_regclearstop      , MRB_ARGS_NONE());

	/* requireモジュール */
	mrb_define_module_function(mrb, module_class, "require"     , rrulib_require            , MRB_ARGS_REQ(1));

	/* 擬似共有メモリモジュール */
	mrb_define_module_function(mrb, module_class, "shmset"      , rrulib_shmset             , MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, module_class, "shmget"      , rrulib_shmget             , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "shmaddr"     , rrulib_shmaddr            , MRB_ARGS_REQ(1));

	/* sleepモジュール */
	mrb_define_module_function(mrb, module_class, "sleep"       , rrulib_sleep              , MRB_ARGS_ARG(1,1));

	/* 擬似SPIモジュール */
	mrb_define_module_function(mrb, module_class, "spiinit"       , rrulib_spiinit           , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "spi0read"      , rrulib_spi0read          , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "spi0write"     , rrulib_spi0write         , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "spi1read"      , rrulib_spi1read          , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "spi1write"     , rrulib_spi1write         , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "spi2read"      , rrulib_spi2read          , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "spi2write"     , rrulib_spi2write         , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "spi3read"      , rrulib_spi3read          , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "spi3write"     , rrulib_spi3write         , MRB_ARGS_REQ(3));


	mrb_define_module_function(mrb, module_class, "spi0worker"     , rrulib_spi0clear          , MRB_ARGS_ARG(3,1));
	mrb_define_module_function(mrb, module_class, "spi1worker"     , rrulib_spi1clear          , MRB_ARGS_ARG(3,1));
	mrb_define_module_function(mrb, module_class, "spi2worker"     , rrulib_spi2clear          , MRB_ARGS_ARG(3,1));
	mrb_define_module_function(mrb, module_class, "spi3worker"     , rrulib_spi3clear          , MRB_ARGS_ARG(3,1));

	mrb_define_module_function(mrb, module_class, "spiworkerstop" , rrulib_spiclearstop      , MRB_ARGS_NONE());

	/* 擬似シンボルモジュール */
	mrb_define_module_function(mrb, module_class, "symalloc"    , rrulib_symalloc           , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "symset"      , rrulib_symset             , MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, module_class, "symset_s"    , rrulib_symset_string      , MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, module_class, "symget"      , rrulib_symget             , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "symaddr"     , rrulib_symaddr            , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "symfree"     , rrulib_symfree            , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "symallfree"  , rrulib_symallfree         , MRB_ARGS_NONE());

	/* systemモジュール */
	mrb_define_module_function(mrb, module_class, "system"       ,rrulib_system             , MRB_ARGS_REQ(1));

	/* 数値モジュール */
	mrb_define_module_function(mrb, module_class, "v_to_v"      , rrulib_val_to_val         , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "calc"        , rrulib_string_clac        , MRB_ARGS_REQ(3));

	/* 比較モジュール */
	mrb_define_module_function(mrb, module_class, "memcmp"      , rrulib_memcmp             , MRB_ARGS_REQ(3));

	/* CUNITモードモジュール */
	mrb_define_module_function(mrb, module_class, "cmode_get"      , rrulib_cmode_get       , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "cmode_set"      , rrulib_cmode_set       , MRB_ARGS_REQ(1));

	ct_init_install(mrb);
	ct_initmw_install(mrb);
	ct_pf_install(mrb);
	ct_mp_install(mrb);
	rrulib_dmsg("install OK \n");
	return;
}

void rrulib_uninstall(void)
{
	rrulib_dmsg("uninstall start \n");
	ct_init_uninstall();
	ct_initmw_uninstall();
	ct_pf_uninstall();
	ct_mp_uninstall();
	rrulib_sym_allclean();
	rrulib_dmsg("uninstall OK \n");
	return;
}

/* @} */

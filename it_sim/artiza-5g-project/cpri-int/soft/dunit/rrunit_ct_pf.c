#include "rrunitlib.h"

#undef CT_PROC_NAME
#define CT_PROC_NAME "PF"
#undef CT_PROC_LIBNAME
#define CT_PROC_LIBNAME "libct_pf.so"

static void* handle = NULL;
static mrb_value ct_module(mrb_state* mrb, mrb_value self)
{
	return(rrulib_func(CT_PROC_NAME,mrb,self,rrulib_dlopen(CT_PROC_LIBNAME , &handle)));
}
static mrb_value ct_addr_table(mrb_state* mrb, mrb_value self)
{
	return(rrulib_addr_table(CT_PROC_NAME,mrb,self,rrulib_dlopen(CT_PROC_LIBNAME , &handle)));
}
static mrb_value ct_set_table(mrb_state* mrb, mrb_value self)
{
	return(rrulib_set_table(CT_PROC_NAME,mrb,self,rrulib_dlopen(CT_PROC_LIBNAME , &handle)));
}
static mrb_value ct_get_table(mrb_state* mrb, mrb_value self)
{
	return(rrulib_get_table(CT_PROC_NAME,mrb,self,rrulib_dlopen(CT_PROC_LIBNAME , &handle)));
}

static mrb_value ct_cunit(mrb_state* mrb, mrb_value self)
{
	mrb_value		tblname;
	mrb_int			mode = 0;
	
	mrb_get_args(mrb, "S|i",&tblname,&mode);
	(void*)rrulib_dlopen(CT_PROC_LIBNAME , &handle);
	if( ( ct_units_main(dlsym (handle, RSTRING_PTR(tblname)) ,mode )) != 0){
		return(mrb_false_value());
	}
	return(mrb_true_value());
}

void ct_pf_install(mrb_state* mrb)
{
	struct RClass * module_class;
	module_class = mrb_define_module(mrb, CT_PROC_NAME);
	mrb_define_module_function(mrb, module_class, "module"      , ct_module     , MRB_ARGS_ANY());
	mrb_define_module_function(mrb, module_class, "tbladdr"     , ct_addr_table , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "tblset"      , ct_set_table  , MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, module_class, "tblget"      , ct_get_table  , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "cunit"       , ct_cunit      , MRB_ARGS_ARG(1,1));
	return;
}

void ct_pf_uninstall(void)
{
	rrulib_dlclose(handle);
	return;
}


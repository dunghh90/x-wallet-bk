#include "rrunitlib.h"

#define DPRINT(...) f_rrulib_dmsg("MP", __func__ ,__VA_ARGS__ )

#define log_all(fstr, ... )	\
do{ \
	char str[4096];	\
	va_list args;	\
	va_start(args, fstr);	\
	vsnprintf( str , sizeof(str) , fstr, args );	\
	va_end(args);	\
	DPRINT("%s",str);	\
}while(0)


void log_error(const char *fstr,...)	{ log_all(fstr,...); }
void log_warn(const char *fstr,...)		{ log_all(fstr,...); }
void log_info(const char *fstr,...)		{ log_all(fstr,...); }
void log_debug(const char *fstr,...)	{ log_all(fstr,...); }
void log_debug2(const char *fstr,...)	{ log_all(fstr,...); }
void log_debug3(const char *fstr,...)	{ log_all(fstr,...); }
void log_debug4(const char *fstr,...)	{ log_all(fstr,...); }
void log_dev0(const char *fstr,...)		{ log_all(fstr,...); }
void log_dev1(const char *fstr,...)		{ log_all(fstr,...); }



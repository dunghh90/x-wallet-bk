
#include "f_rsv_header.h"

#ifdef OPT_RRH_SIM
void f_rsv_dbg()
{
	UINT	loop;
	
//	for(loop = 0; loop < D_RSV_SVTBL_NUM; loop++)
//	{
//		printf("tra_sv_newsta[%d] = %04x\n", loop, tra_sv_newsta[loop]);
//	}
//
//	for(loop = 0; loop < D_RSV_SVTBL_NUM; loop++)
//	{
//		printf("tra_sv_oldsta[%d] = %04x\n", loop, tra_sv_oldsta[loop]);
//	}

	for(loop = 0; loop < 3; loop++)
	{
		printf("f_rsvw_cardStaChgNtc_QID[%d] = %08x\n", loop, f_rsvw_cardStaChgNtc_QID[loop]);
		
	}

	return;
}
#endif



#include "f_rrh_inc.h"
#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_rc_header.h"			/* RE監視タスクヘッダファイル			*/
#ifdef OPT_RRH_SIM
void m_rc_dbg()
{
	UINT	loop;
	
	printf("----------- The information of cmw_tra_sv_tbl -----------\n");

	printf("cmw_tra_sv_tbl.en_flg = %4x\n", cmw_tra_sv_tbl.en_flg);
	for(loop = 0; loop < 5; loop++)
	{
		printf("cmw_tra_sv_tbl.sv_inf[%d] = %04x\n", loop, cmw_tra_sv_tbl.sv_inf[loop]);
	}
	printf("cmw_tra_sv_tbl.alm_num = %d\n", cmw_tra_sv_tbl.alm_num);

	for(loop = 0; loop < cmw_tra_sv_tbl.alm_num; loop++)
	{
		printf("cmw_tra_sv_tbl.alm_inf[%d] = %04x\n", loop, cmw_tra_sv_tbl.alm_inf[loop]);
	}

	printf("cmw_tra_sv_tbl.card_cntitem = %04x\n", cmw_tra_sv_tbl.card_cntitem);
	printf("cmw_tra_sv_tbl.card_extitem = %04x\n", cmw_tra_sv_tbl.card_extitem);
	printf("cmw_tra_sv_tbl.carr_onoff = %04x\n", cmw_tra_sv_tbl.carr_onoff);

	printf("----------- The information of cmw_cardcnt_tbl for S3G -----------\n");
	printf("cmw_cardcnt_tbl.cnt_kind = %d\n", cmw_cardcnt_tbl.cnt_kind);
	printf("cmw_cardcnt_tbl.cnt_itm_s3g = %d\n", cmw_cardcnt_tbl.cnt_itm_s3g);
	printf("cmw_cardcnt_tbl.cnt_extitem_s3g = %d\n", cmw_cardcnt_tbl.cnt_extitem_s3g);
	
	return;
}
#endif


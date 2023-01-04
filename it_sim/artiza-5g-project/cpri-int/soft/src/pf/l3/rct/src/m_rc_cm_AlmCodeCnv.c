/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_cm_AlmCodeCnv.c
 *  @brief  convert card alarm factor to alarm code
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/05/18 FFCS)Wangjuan modify for M-S3G-eNBPF-02009
 							when several alarm code exist, all need report.
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date 2012/06/20 FFCS)niemsh modify for 15.GRRE
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */

/****************************************************************************/
/* ヘッダインクルード                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_rc_header.h"
#include "rrhApi_Svp.h"
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#include "f_rrh_reg_cnt.h"
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @{ */
/**
* @brief convert card alarm factor to alarm code
* @note convert card alarm factor to alarm code.\n
* @param almInfo [in] alarm information
* 
* @return none
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2012/07/20 FFCS)niemsh modify for 15.GRRE
* @date 2015/04/21 FFCS)zhangxq modify for TDD-SRE
* @date 2015/10/26 TDI)satou リソースALMのチェックを追加
* @date 2015/12/04 TDI)satou IT3問処番号No.108
*/

VOID m_rc_cm_AlmCodeCnv( T_API_SVP_ALMINFO  almInfo )
{
//	USHORT	loopcnt;			/* use as loop counter */
//	USHORT	shiftnum;			/* use as shift operation */
//	UINT	almcode;
    UINT bit;

	/* clear old alarm info */
	cmw_tra_sv_tbl.alm_num = CMD_NUM0;
	cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_num = CMD_NUM0;
	cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_num = CMD_NUM0;
	cm_MemClr(cmw_tra_sv_tbl.alm_inf, sizeof(cmw_tra_sv_tbl.alm_inf));
	cm_MemClr(cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_inf, sizeof cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_inf);
	cm_MemClr(cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_inf, sizeof cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_inf);
	
	/* check if soft alarm exist or not */
	if(((almInfo.almFactor) & RCD_SOFTALM_BIT) != CMD_NUM0)
	{
		cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = almInfo.softAlm;
		cmw_tra_sv_tbl.alm_num++;		
		cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_inf[ cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_num ] = almInfo.softAlm;
		cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_num++;
		cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_inf[ cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_num ] = almInfo.softAlm;
		cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_num++;
	}

    /* リソースALMをチェック */
    if (((almInfo.almFactor) & RCD_SOFTALM2_BIT) != CMD_NUM0)
    {
        bit = D_RRH_REG_CNT_SV7INF_BIT_REFLASH;
        bit >>= 16;
        if (almInfo.softAlm2 & bit)
        {
            cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = D_RRH_ALMCD_REFLASHNG;
            cmw_tra_sv_tbl.alm_num++;
            cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_inf[ cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_num ] = D_RRH_ALMCD_REFLASHNG;
            cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_num++;
            cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_inf[ cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_num ] = D_RRH_ALMCD_REFLASHNG;
            cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_num++;
        }

        bit = D_RRH_REG_CNT_SV7INF_BIT_PARAM_MISMATCH;
        bit >>= 16;
        if (almInfo.softAlm2 & bit)
        {
            cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = D_RRH_ALMCD_MISMATCH_PARM;
            cmw_tra_sv_tbl.alm_num++;
        }

        bit = D_RRH_REG_CNT_SV7INF_BIT_PARAM_MISMATCH_3G;
        bit >>= 16;
        if (almInfo.softAlm2 & bit)
        {
            cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_inf[ cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_num ] = D_RRH_ALMCD_MISMATCH_PARM;
            cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_num++;
        }

        bit = D_RRH_REG_CNT_SV7INF_BIT_PARAM_MISMATCH_S3G;
        bit >>= 16;
        if (almInfo.softAlm2 & bit)
        {
            cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_inf[ cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_num ] = D_RRH_ALMCD_MISMATCH_PARM;
            cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_num++;
        }
    }

//	/* check if PLL alarm exist or not */
//	if(((almInfo.almFactor) & D_RRH_MAJALM_PLL_BIT) != CMD_NUM0)
//	{
//		for(loopcnt = CMD_NUM0; loopcnt < RCD_PLLALM_NUM; loopcnt++)
//		{
//			shiftnum = loopcnt;
//
//			/* if minor alarm factor exist, convert it to alarm code */
//			if((((almInfo.pllAlm) >> shiftnum) & CMD_NUM1) != CMD_NUM0)
//			{
//				almcode = rcr_pllalmcode[loopcnt];
//
//				cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = almcode;
//				cmw_tra_sv_tbl.alm_num++;
//			}
//		}
//	}
//
//	/* check if CPRI alarm exist or not */
//	if(((almInfo.almFactor) & D_RRH_MAJALM_CPRI_BIT) != CMD_NUM0)
//	{
//		for(loopcnt = CMD_NUM0; loopcnt < RCD_CPRIALM_NUM; loopcnt++)
//		{
//			shiftnum = loopcnt;
//
//			/* if minor alarm factor exist, convert it to alarm code */
//			if((((almInfo.cpriAlm) >> shiftnum) & CMD_NUM1) != CMD_NUM0)
//			{
//				almcode = rcr_cprialmcode[loopcnt];
//
//				cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = almcode;
//				cmw_tra_sv_tbl.alm_num++;
//			}
//		}
//	}
//
//	/* check if APD COM alarm exist or not */
//	if(((almInfo.almFactor) & D_RRH_MAJALM_APDCOM_BIT) != CMD_NUM0)
//	{
//		for(loopcnt = CMD_NUM0; loopcnt < RCD_APDALM_LOOP_NUM; loopcnt++)
//		{
//			shiftnum = loopcnt;
//
//			/* if minor alarm factor exist, convert it to alarm code */
//			if((((almInfo.apdComAlm) >> shiftnum) & CMD_NUM1) != CMD_NUM0)
//			{
//
//				almcode = rcr_apdcomalmcode[loopcnt];
//				cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = almcode;
//				cmw_tra_sv_tbl.alm_num++;
//			}
//		}
//	}
//	/* check if APD0 alarm exist or not */
//	if(((almInfo.almFactor) & D_RRH_MAJALM_APD0_BIT) != CMD_NUM0)
//	{
//		for(loopcnt = CMD_NUM0; loopcnt < RCD_APDALM_LOOP_NUM; loopcnt++)
//		{
//			shiftnum = loopcnt;
//
//			/* if minor alarm factor exist, convert it to alarm code */
//			if((((almInfo.apd0Alm) >> shiftnum) & CMD_NUM1) != CMD_NUM0)
//			{
//				almcode = rcr_apd0almcode[loopcnt];
//				cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = almcode;
//				cmw_tra_sv_tbl.alm_num++;
//			}
//		}
//	}
//	/* check if APD1 alarm exist or not */
//	if(((almInfo.almFactor) & D_RRH_MAJALM_APD1_BIT) != CMD_NUM0)
//	{
//		for(loopcnt = CMD_NUM0; loopcnt < RCD_APDALM_LOOP_NUM; loopcnt++)
//		{
//			shiftnum = loopcnt;
//
//			/* if minor alarm factor exist, convert it to alarm code */
//			if((((almInfo.apd1Alm) >> shiftnum) & CMD_NUM1) != CMD_NUM0)
//			{
//				almcode = rcr_apd1almcode[loopcnt];
//				cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = almcode;
//				cmw_tra_sv_tbl.alm_num++;
//			}
//		}
//	}
//
//	/* check if APD2 alarm exist or not */
//	if(((almInfo.almFactor) & D_RRH_MAJALM_APD2_BIT) != CMD_NUM0)
//	{
//		for(loopcnt = CMD_NUM0; loopcnt < RCD_APDALM_LOOP_NUM; loopcnt++)
//		{
//			shiftnum = loopcnt;
//
//			/* if minor alarm factor exist, convert it to alarm code */
//			if((((almInfo.apd2Alm) >> shiftnum) & CMD_NUM1) != CMD_NUM0)
//			{
//				almcode = rcr_apd2almcode[loopcnt];
//				cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = almcode;
//				cmw_tra_sv_tbl.alm_num++;
//			}
//		}
//	}
//
//	/* check if APD3 alarm exist or not */
//	if(((almInfo.almFactor) & D_RRH_MAJALM_APD3_BIT) != CMD_NUM0)
//	{
//		for(loopcnt = CMD_NUM0; loopcnt < RCD_APDALM_LOOP_NUM; loopcnt++)
//		{
//			shiftnum = loopcnt;
//
//			/* if minor alarm factor exist, convert it to alarm code */
//			if((((almInfo.apd3Alm) >> shiftnum) & CMD_NUM1) != CMD_NUM0)
//			{
//				almcode = rcr_apd3almcode[loopcnt];
//				cmw_tra_sv_tbl.alm_inf[ cmw_tra_sv_tbl.alm_num ] = almcode;
//				cmw_tra_sv_tbl.alm_num++;
//			}
//		}
//	}

    l3_com_ushortsort_asc(cmw_tra_sv_tbl.alm_num, sizeof(USHORT)/sizeof(USHORT), cmw_tra_sv_tbl.alm_inf);
    l3_com_ushortsort_asc(cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_num, sizeof(USHORT)/sizeof(USHORT), cmw_tra_sv_tbl.sys[CMD_SYS_3G ].alm_inf);
    l3_com_ushortsort_asc(cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_num, sizeof(USHORT)/sizeof(USHORT), cmw_tra_sv_tbl.sys[CMD_SYS_S3G].alm_inf);

	return ;
}
/* @} */

/* @} */


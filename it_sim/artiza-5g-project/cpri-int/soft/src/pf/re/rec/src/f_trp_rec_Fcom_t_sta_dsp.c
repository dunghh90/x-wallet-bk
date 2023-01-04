/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_sta_dsp.c
 * @brief  ログコマンド処理(ステータス)
 * @date   2007/04/25 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  ログコマンド処理(ステータス)
 * @note   ステータスログを出力する
 * @param  prm     [in]  パラメータ
 * @param  stream  [in]  出力ストリーム
 * @return None
 * @date   2007/04/25 FJT)Nagasima create.
 * @date   2010/06/02 FJT)Tokunaga TS-514項 M-S3G-eNBSYS-01559
 * @date   2015/09/03 FPT)Lay modify for FHM
 * 
*/
/****************************************************************************/
VOID f_trp_rec_Fcom_t_sta_dsp(                                    /* なし:R */
    INT*  prm,                                              /* パラメータ:I */
    FILE* stream                                        /* 出力ストリーム:I */
) {

    T_REC_LOGTBL* infp;                                     /* 情報ポインタ */
    UINT  inf_idx;                                      /* 情報インデックス */
    UINT   dmp_idx;
//    USHORT cnt;
//    UINT   resetTime;
//    struct bpf_tm tm;
#if defined(OPT_CPRI_L2HARD)
    UINT  regVal;
    UINT  linkcnt;
    UINT  slotcnt;
#else
//    USHORT slotNo;
//    USHORT cpriNo;
//    T_REC_CARDOBJ* cardObj;
//    T_REC_CPRILINKOBJ*  cpriLinkObj;
//    UINT   cpriCnt;
//    UINT   slotCnt;
//    USHORT index;
    //USHORT tablePos[D_CPRILINK_MAX][D_REC_CARD_INDEX_NUM];
//    USHORT regdat;
#endif

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* ストリームがNULLの場合 */
    if (stream == NULL) {
        /* 標準出力に変更 */
        stream = stdout;
    }

    fprintf( stream, "##########################\n");
    fprintf( stream, "## REC STATUS LOG START ##\n");
    fprintf( stream, "##########################\n");

    /* RUNHIS指定の場合 */
    if      (*prm == D_TCM_LOGOUT_KIND_RUNHIS) {
        /* RUNHIS表示 */
        f_trp_rec_Fcom_t_runhis_dsp(stream);
    }

    /* REGHIS指定の場合 */
    else if (*prm == D_TCM_LOGOUT_KIND_REGHIS) {
        /* REGHIS表示 */
        f_trp_rec_Fcom_t_reghis_dsp(stream);
    }

    /* STN指定の場合 */
    else if (*prm == D_TCM_LOGOUT_KIND_STN) {
        /* STN表示 */
        f_trp_rec_Fcom_t_stn_dsp(stream);
    }

    /* TIM指定の場合 */
    else if (*prm == D_TCM_LOGOUT_KIND_TIM) {
        /* TIM表示 */
        f_trp_rec_Fcom_t_tim_dsp(stream);
    }

    /* DPL指定の場合 */
    else if (*prm == D_TCM_LOGOUT_KIND_DPL) {
        /* ダンプリスト表示 */
        fprintf( stream, "## Dump List ##\n");
        for (inf_idx = CMD_NUM0; inf_idx < D_REC_LOGTBL_NUM; inf_idx++) {
            /* 情報ポインタ設定 */
            infp = &f_trp_rec_Rcom_logtbl_tbl.inf[inf_idx];
            fprintf( stream, "%s:%s\n", infp->nic, infp->tbl_name);
        }                        
    }

    /* DMP指定の場合 */
    else if (*prm == D_TCM_LOGOUT_KIND_DMP) {
        /* テーブルダンプ */
        for (inf_idx = CMD_NUM0; inf_idx < D_REC_LOGTBL_NUM; inf_idx++) {
            /* 情報ポインタ設定 */
            infp = &f_trp_rec_Rcom_logtbl_tbl.inf[inf_idx];
            fprintf( stream, "## Dump Table ##\n");
            fprintf( stream, "%s:%s[0x%08X-0x%08X]\n",
                infp->nic, infp->tbl_name,
                (UINT)infp->tbl_adr, infp->tbl_size);
            f_trp_com_Fhex_dmp(stream, infp->tbl_adr, infp->tbl_size);
        }

        f_trp_rec_Fcom_t_tbldump_dsp(stream);
        f_trp_rec_Fcom_t_tbldump_dsp2(stream);
    }

    else if (*prm == D_TCM_REALMWATCH_DBGON) {
//TODO        if((f_trp_com_log_kind >= 1) && (f_trp_com_log_kind <= 99))
//        {
//             RE_AlarmProtectTime = f_trp_com_log_kind * 60;    /* 指定した値×60(s) */
//        }
//        else
//        {
//             RE_AlarmProtectTime = D_TCM_ALM_PROTECT_DEBUG;     /* 10分 */
//             f_trp_com_log_kind = 10;
//        }
//        RE_AlarmProtectTime_Debug = CMD_ON;
//        fprintf( stream,"##---------------------------------------##\n");
//        fprintf( stream,"## RE ALM WATCH TIME CHANGE (%d minutes) ##\n",f_trp_com_log_kind);
//        fprintf( stream,"##---------------------------------------##\n");
    }
    else if (*prm == D_TCM_REALMWATCH_DBGOFF) {
//TODO        RE_AlarmProtectTime_Debug = CMD_OFF;
//        fprintf( stream,"##------------------------------------##\n");
//        fprintf( stream,"## RE ALM WATCH TIME DEFAULT (48hour) ##\n");
//        fprintf( stream,"##------------------------------------##\n");
    }
#if !defined(OPT_CPRI_L2HARD)
    else if( *prm == D_TCM_CMUX_REG_DSP ) {
//TODO        fprintf( stream,"##---------------##\n");
//        fprintf( stream,"## CMUX REG DISP ##\n");
//        fprintf( stream,"##---------------##\n");
//        
//        if( (f_trp_com_log_kind == D_TCM_CARDID_CPRIMUX1) || (f_trp_com_log_kind == D_TCM_CARDID_CPRIMUX2) )
//        {
//            if( f_trp_com_log_kind2 == 0 )
//            {
//                if( (f_trp_com_log_kind3 == 0x1116) || (f_trp_com_log_kind3 == 0x1117) || (f_trp_com_log_kind3 == 0x1118) )
//                {
//                    f_trp_com_r_mux_reg(f_trp_com_log_kind,f_trp_com_log_kind2,(USHORT)f_trp_com_log_kind3,(USHORT*)&regdat,D_TRP_CMUX_SEM_GIVEREQ_REC_TOFFSET);
//                    fprintf( stream,"Card ID  = 0x%x\n",f_trp_com_log_kind);
//                    fprintf( stream,"CMUX NUM = 0x%x\n",f_trp_com_log_kind2);
//                    fprintf( stream,"REG Addr = 0x%x\n",f_trp_com_log_kind3);
//                    fprintf( stream,"REG Data = 0x%x\n",regdat);
//                }
//            }
//            else if( f_trp_com_log_kind2 == 1 )
//            {
//                if( (f_trp_com_log_kind3 == 0x5116) || (f_trp_com_log_kind3 == 0x5117) || (f_trp_com_log_kind3 == 0x5118) )
//                {
//                    f_trp_com_r_mux_reg(f_trp_com_log_kind,f_trp_com_log_kind2,(USHORT)f_trp_com_log_kind3,(USHORT*)&regdat,D_TRP_CMUX_SEM_GIVEREQ_REC_TOFFSET);
//                    fprintf( stream,"Card ID  = 0x%x\n",f_trp_com_log_kind);
//                    fprintf( stream,"CMUX NUM = 0x%x\n",f_trp_com_log_kind2);
//                    fprintf( stream,"REG Addr = 0x%x\n",f_trp_com_log_kind3);
//                    fprintf( stream,"REG Data = 0x%x\n",regdat);
//                }
//            }
//            else if( f_trp_com_log_kind2 == 2 )
//            {
//                if( (f_trp_com_log_kind3 == 0x9116) || (f_trp_com_log_kind3 == 0x9117) || (f_trp_com_log_kind3 == 0x9118) )
//                {
//                    f_trp_com_r_mux_reg(f_trp_com_log_kind,f_trp_com_log_kind2,(USHORT)f_trp_com_log_kind3,(USHORT*)&regdat,D_TRP_CMUX_SEM_GIVEREQ_REC_TOFFSET);
//                    fprintf( stream,"Card ID  = 0x%x\n",f_trp_com_log_kind);
//                    fprintf( stream,"CMUX NUM = 0x%x\n",f_trp_com_log_kind2);
//                    fprintf( stream,"REG Addr = 0x%x\n",f_trp_com_log_kind3);
//                    fprintf( stream,"REG Data = 0x%x\n",regdat);
//                }
//            }
//        }
    }
#endif
    else if (*prm == D_TCM_REALMATIME_DSP) {
//TODO        fprintf( stream,"##---------------##\n");
//        fprintf( stream,"## RE Reset TIME ##\n");
//        fprintf( stream,"##---------------##\n");
//        
//#if !defined(OPT_CPRI_L2HARD)
//        for(cnt=0; cnt<D_REC_CARD_INDEX_NUM; cnt++)
//        {
//            resetTime = f_trp_com_AutoResetMngTbl->autoRstReInf[cnt].resetTime;
//            if(resetTime != D_TCM_DEFAULT_RSTTIM)
//            {
//                cpriNo = 0;
//                slotNo = 0;
//                
//				for(cpriCnt=CMD_NUM0; cpriCnt < D_CPRILINK_MAX; cpriCnt++)
//        		{
//            		cpriLinkObj = f_trp_rec_FCpriLink_Obj_GetByNo(cpriCnt+1);
//            		if(cpriLinkObj != NULL)
//            		{
//            		    cardObj = f_trp_rec_FCpriLink_AllCardList_GetFirst(cpriLinkObj);
//            		    while(cardObj != NULL)
//            		    {
//            		        index = f_trp_rec_FCard_ObjIdx_Get(cardObj);
//            		        if(cnt == index)
//            		        {
//            		        	cpriNo = cpriCnt+1;
//                	            slotNo = cardObj->slotNo;
//            	        	}
//            	        	cardObj = f_trp_rec_FCpriLink_AllCardList_GetNext(cpriLinkObj, cardObj);
//            	    	}
//            		}
//   		     	}
//                BPF_RU_UTTM_GETLOCALTIME(&(resetTime), &tm);
//                fprintf( stream,"[RE#%d] Reset Time = [%04d/%02d/%02d %02d:%02d:%02d]  CPRI=%d,SLOT=%d\n", cnt, tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,cpriNo,slotNo);
//            }
//            else
//            {
//                fprintf( stream,"[RE#%d] Reset Time = [-]\n", cnt);
//            }
//        }
//
//#else
//		for(linkcnt=0; linkcnt<CMD_LINK_NUM_T; linkcnt++)
//		{
//			for(slotcnt=0; slotcnt<CMD_SLOTINF_MAX; slotcnt++)
//			{
//		        (VOID)f_trp_com_Fread_reg((UINT)crr_reAutoCntReg[linkcnt][slotcnt], &regVal );
//		        fprintf( stream,"[RE#%dSLOT#%d] Reset Time = %08d\n", linkcnt+1,slotcnt+1,regVal);
//		    }
//		}
//#endif
    }
    
    else if(*prm == D_TCM_LOGOUT_KIND_RUNHIS_ALL) {
        dmp_idx = f_trp_com_WlogDspIndex;
        f_trp_rec_Fcom_t_runhis_dmp(stream,dmp_idx);
    }

    else if(*prm == D_TCM_LOGOUT_KIND_FBSTSDMP) {
//TODO        f_trp_rec_Fcom_t_fbsts_dsp(stream);
    }

    /* 全指定の場合 */
    else if (*prm == D_TCM_LOGOUT_KIND_ALL) {
        /* RUNHIS表示 */
#if 0
        dmp_idx = f_trp_com_WlogWriteIndex;
        f_trp_rec_Fcom_t_runhis_dmp(stream,dmp_idx);
        
        /* REGHIS表示 */
        f_trp_rec_Fcom_t_reghis_dsp(stream);
        /* STN表示 */
        f_trp_rec_Fcom_t_stn_dsp(stream);
        /* TIM表示 */
        f_trp_rec_Fcom_t_tim_dsp(stream);
        /* テーブルダンプ */
        for (inf_idx = CMD_NUM0; inf_idx < D_REC_LOGTBL_NUM; inf_idx++) {
            /* 情報ポインタ設定 */
            if( inf_idx != D_REC_LOGTBL_NUM_RUNHIS )
            {
                infp = &f_trp_rec_Rcom_logtbl_tbl.inf[inf_idx];
                fprintf( stream,"## Dump Table ##\n");
                fprintf( stream,"%s:%s[0x%08X-0x%08X]\n",
                    infp->nic, infp->tbl_name,
                    (UINT)infp->tbl_adr, infp->tbl_size);
                f_trp_com_Fhex_dmp(stream, infp->tbl_adr, infp->tbl_size);
            }
        }

        f_trp_rec_Fcom_t_tbldump_dsp(stream);
#endif
        f_trp_rec_Fcom_t_runhis_dsp(stream);
        f_trp_rec_Fcom_t_tbldump_dsp2(stream);

    }

    fprintf( stream,"##########################\n");
    fprintf( stream,"## REC STATUS LOG END   ##\n");
    fprintf( stream,"##########################\n");

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_reeqpinf_chk.c
 * @brief  RE装置構成情報報告応答パラメータチェック処理
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE装置構成情報報告応答パラメータチェック処理
 * @note   RE装置構成情報報告応答のパラメータチェックを行う
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 応答結果
 * @retval 0:正常
 * @retval 1:異常
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2011/05/23 FJT)Koshida modify for 11B-01-001(eNB-004)
 * @date   2015/09/23 FPT)Dung modify for FHM 
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_reeqpinf_chk(                               /* 変化:R */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    CMT_CPRIF_EQPSTRRES_S3G *bufp;          /* RE装置構成情報報告応答MSG*/
    CHAR errdata[CMD_NUM48];
    
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
    /* アドレス設定 */
    bufp = (CMT_CPRIF_EQPSTRRES_S3G *)l3_bufp;
    
    /************************************************************************/
    /* RE装置構成情報報告応答パラメータチェック処理                         */
    /************************************************************************/
    /********************************************/
    /* CHECK!メーカ名(範囲:65～90)                */
    /********************************************/
    if(((bufp->cpristr_inf.maker <  CMD_MAKERMIN)  ||
        (bufp->cpristr_inf.maker >  CMD_MAKERMAX)) &&
       ( bufp->cpristr_inf.maker != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][mak:0x%02x]Maker check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.maker);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
        
    /********************************************/
    /* CHECK!BTS形番(範囲:3001 )              */
    /********************************************/
    if(bufp->cpristr_inf.bts_no !=  CMD_BTS_NO)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][btsno:0x%02x]BTS No check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.bts_no);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /********************************************/
    /* CHECK!装置号番(範囲:1～ )                */
    /********************************************/
    if(bufp->cpristr_inf.eqp_no <  CMD_EQPMIN)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][eqpno:0x%02x]Eqp No check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.eqp_no);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
        
    /********************************************/
    /* CHECK!RE種別(範囲:2～3)                  */
    /********************************************/
    if((bufp->cpristr_inf.rekind != D_REC_RETYP_AMP)   &&
       (bufp->cpristr_inf.rekind != D_REC_RETYP_OFTRX) &&
       (bufp->cpristr_inf.rekind != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][rek:0x%02x]RE kind check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.rekind);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /********************************************/
    /* CHECK!3GBTS 共用方式(範囲:0～2)          */
    /********************************************/
    if((bufp->cpristr_inf.bts_share_type >  CMD_MODEL3G_MAX) &&
       (bufp->cpristr_inf.bts_share_type != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][modle_3g:0x%02x]3G Share model check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.bts_share_type); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

	/********************************************/
    /* CHECK!電源種別(範囲:0～3)                */
    /********************************************/
    if((bufp->cpristr_inf.powerkind >  CMD_POWERKIND_AC1_2) &&
       (bufp->cpristr_inf.powerkind != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][modle_3g:0x%02x]Power type check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerkind); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
	/********************************************/
    /* CHECK!受信アンテナ数(範囲:1～8)          */
    /********************************************/
    if((bufp->cpristr_inf.rcv_antena_no  <  D_REC_ANTNUM_MIN) ||
       ((bufp->cpristr_inf.rcv_antena_no >  D_REC_ANTNUM_MAX) && 
        (bufp->cpristr_inf.rcv_antena_no != D_REC_RETBLINI_EQUINF)) )
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][brcnum:0x%02x]Recving Antena number check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.rcv_antena_no); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /********************************************/
    /* CHECK!送信アンテナ数(範囲:1～8)          */
    /********************************************/
    if((bufp->cpristr_inf.snd_antena_no  <  D_REC_ANTNUM_MIN) ||
       ((bufp->cpristr_inf.snd_antena_no >  D_REC_ANTNUM_MAX) && 
        (bufp->cpristr_inf.snd_antena_no != D_REC_RETBLINI_EQUINF)) )
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][brcnum:0x%02x]Sending Antena number check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.snd_antena_no); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /********************************************/
    /* CHECK!パワークラス単位(範囲:0～2)        */
    /********************************************/
    if((bufp->cpristr_inf.powerunit >  CMD_POWERUNIT_MICROW) &&
       (bufp->cpristr_inf.powerunit != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][powu:0x%02x]Power uint check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerunit);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /********************************************/
    /* CHECK!パワークラス(0 系)(範囲:1～ )      */
    /********************************************/
    if(bufp->cpristr_inf.powerclass_0 <  CMD_PUNITMIN)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][pow_0:0x%02x]Power class_0 check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerclass_0); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /********************************************/
    /* CHECK!パワークラス(1 系)(範囲:1～ )      */
    /********************************************/
    if(bufp->cpristr_inf.powerclass_1 <  CMD_PUNITMIN)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][pow_1:0x%02x]Power class_1 check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerclass_1); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /********************************************/
    /* CHECK!パワークラス(2 系)(範囲:1～ )      */
    /********************************************/
    if(bufp->cpristr_inf.powerclass_2 <  CMD_PUNITMIN)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][pow_2:0x%02x]Power class_2 check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerclass_2); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /********************************************/
    /* CHECK!パワークラス(3 系)(範囲:1～ )      */
    /********************************************/
    if(bufp->cpristr_inf.powerclass_3 < CMD_PUNITMIN)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][pow_3:0x%02x]Power class_3 check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerclass_3); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
        
    /********************************************/
    /* CHECK!対応周波数帯(範囲: 2^0, 2^1, 2^2, 2^3, 2^4, 2^5, 2^6 )        */
    /********************************************/
    if((bufp->cpristr_inf.frqband != CMD_FRQBAND_2_LTE) &&
       (bufp->cpristr_inf.frqband != CMD_FRQBAND_15_LTE) &&
       (bufp->cpristr_inf.frqband != CMD_FRQBAND_800_LTE) &&
       (bufp->cpristr_inf.frqband != CMD_FRQBAND_700_LTE) &&
       (bufp->cpristr_inf.frqband != CMD_FRQBAND_17_LTE) &&
       (bufp->cpristr_inf.frqband != CMD_FRQBAND_17L_LTE) &&
       (bufp->cpristr_inf.frqband != CMD_FRQBAND_35_LTE) &&
       (bufp->cpristr_inf.frqband != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][frq:0x%02x]Frquence band check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.frqband); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
    /********************************************/
    /* CHECK!最大周波数帯域幅(範囲:1～20)        */
    /********************************************/
    if(((bufp->cpristr_inf.max_bandwidth < CMD_SYSBANDMIN)  ||
        (bufp->cpristr_inf.max_bandwidth > CMD_SYSBANDMAX)) &&
       (bufp->cpristr_inf.max_bandwidth != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][bnd:0x%02x]System band check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.max_bandwidth);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /**************************************/
    /* CHECK!2GHz帯キャリア数(範囲:1)     */
    /**************************************/
    if((bufp->cpristr_inf.us2g_car_no != CMD_NUM1) &&
       (bufp->cpristr_inf.us2g_car_no != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]2GHz Carrier number check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.us2g_car_no); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /**************************************/
    /* CHECK!1.5GHz帯キャリア数(範囲:1)   */
    /**************************************/
    if((bufp->cpristr_inf.us15g_car_no != CMD_NUM1) &&
       (bufp->cpristr_inf.us15g_car_no != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]1.5GHz Carrier number check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.us15g_car_no); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }

    /**************************************/
    /* CHECK!800MHz帯キャリア数(範囲:1)   */
    /**************************************/
    if((bufp->cpristr_inf.us800m_car_no != CMD_NUM1) &&
       (bufp->cpristr_inf.us800m_car_no != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]800MHz Carrier number check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.us800m_car_no); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
    /**************************************/
    /* CHECK!700MHz帯キャリア数(範囲:1)   */
    /**************************************/
    if((bufp->cpristr_inf.us700m_car_no != CMD_NUM1) &&
       (bufp->cpristr_inf.us700m_car_no != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]700MHz Carrier number check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.us700m_car_no); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
    /**************************************/
    /* CHECK!1.7GHz帯キャリア数(範囲:1)   */
    /**************************************/
    if((bufp->cpristr_inf.us17g_car_no != CMD_NUM1) &&
       (bufp->cpristr_inf.us17g_car_no != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]1.7GHz Carrier number check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.us17g_car_no); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
	/**************************************/
    /* CHECK!1.7GHz LowBand帯キャリア数(範囲:1)   */
    /**************************************/
    if((bufp->cpristr_inf.dummy3[0] != CMD_NUM1) &&
       (bufp->cpristr_inf.dummy3[0] != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]1.7GHz LowBand Carrier number check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.dummy3[0]); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
    /*******************************************/
    /* CHECK!RE側TRX-INFタイプ情報(範囲:0～3)  */
    /*******************************************/
    if((bufp->cpristr_inf.trx_inf_type >  CMD_13_21DB) &&
       (bufp->cpristr_inf.trx_inf_type != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][trxinf:0x%02x]TRX inf type check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.trx_inf_type); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
	  /**************************************/
    /* CHECK!キャリア数(3.5GHz帯)予備(範囲:1)   */
    /**************************************/
    if((bufp->cpristr_inf.us35g_car_no != CMD_35GCARLMIN) && 
       (bufp->cpristr_inf.us35g_car_no != CMD_35GCARLMAX) &&
       (bufp->cpristr_inf.us35g_car_no != D_REC_RETBLINI_EQUINF))
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][crrnum:0x%02x]3.5GHz Carrier number check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.us35g_car_no); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
	/********************************************/
    /* CHECK!パワークラス(4 系)(範囲:1～ )      */
    /********************************************/
    if(bufp->cpristr_inf.powerclass_4 <  CMD_PUNITMIN)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][pow_1:0x%02x]Power class_4 check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerclass_4); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
    
		/********************************************/
    /* CHECK!パワークラス(5 系)(範囲:1～ )      */
    /********************************************/
    if(bufp->cpristr_inf.powerclass_5 <  CMD_PUNITMIN)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][pow_1:0x%02x]Power class_5 check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerclass_5); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
		/********************************************/
    /* CHECK!パワークラス(6 系)(範囲:1～ )      */
    /********************************************/
    if(bufp->cpristr_inf.powerclass_6 <  CMD_PUNITMIN)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][pow_1:0x%02x]Power class_6 check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerclass_6); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
		/********************************************/
    /* CHECK!パワークラス(7 系)(範囲:1～ )      */
    /********************************************/
    if(bufp->cpristr_inf.powerclass_7 <  CMD_PUNITMIN)
    {
        /* パラメータ範囲外のためアサートを出力 */
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][pow_1:0x%02x]Power class_7 check NG.", D_REC_C02_CPR_NO(), bufp->cpristr_inf.powerclass_7); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

        return CMD_NG;
    }
	
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

    /* 復帰値返却 */
    return CMD_OK ;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

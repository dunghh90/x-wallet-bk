/****************************************************************************/
/*!
 * @skip    $ld:$
 * @file    f_trp_rec_Fc02_m_use_sendtypeswitch.c
 * @brief  運用中REキャリア状態報告終了処理
 * @date    2015/09/21 FPT)Dung create. 
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2010
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "f_rrh_reg_dis.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  運用中送信系を１系に切り替える処理 (S3G)
 * @note   (LTE)(0x5821)REキャリア状態報告応答
 * @return None
 * @Bug_No M-RRU-ZSYS-01966
 * @date   2015/09/11 FPT)Dung create. 
 * @date   2015/10/08 FPT)Dung update. 
 * @date   2015/11/06 TDI)satou M-RRU-ZSYS-01966 1系ALM発生中に0系ALM発生すると送信系切替が行われる問題の修正
 * @date   2015/11/18 FPT)Yen fix bug IT3 No89
 * @date   2015/12/17 TDI)enoki 4Br時に送信系切替されない
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_sendtypeswitch(                           /* なし:R */
) {
    USHORT                  re_cpri_num;
    T_RRH_CARST_S3G         ltCarstLte;
    USHORT                  lwCarCnt;
    USHORT                  lwCarSta;
    T_TRXSET_PARA_LTE       ltTrxsetParaLte[CMD_MAX_CARNUM];
    UINT                    regAdd;
    UINT                    regVal;
    UINT                    bitifno;
	T_RRH_SYSPAR_LTE		syspar_lte;
	USHORT					lte_bw;
	USHORT					car_idx;
	USHORT					car_cnt;
	UINT					swap_regAddr[2] = {D_RRH_REG_DIS_LSWAP0,D_RRH_REG_DIS_LSWAP1};

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    re_cpri_num = D_REC_C02_CPR_NO();
    bitifno     = 1 << (re_cpri_num - 1);
    f_cmn_com_carst_s3g_get(re_cpri_num, &ltCarstLte);
    f_cmn_com_trxset_para_lte_get((USHORT)CMD_CARNO_C1, &ltTrxsetParaLte[0]);
    f_cmn_com_trxset_para_lte_get((USHORT)CMD_CARNO_C2, &ltTrxsetParaLte[1]);

    /* LTEのキャリアリソースが少なくとも1つ以上「使用中」となっていること.
     * そうなっていない場合は処理を終了する. */
    lwCarSta = 0;
    for (lwCarCnt = 0; lwCarCnt < CMD_MAX_CARNUM_S3G; lwCarCnt++)
    {
        lwCarSta |= ltCarstLte.car_every[lwCarCnt].car_st;
    }

    if (0 == (CMD_RESSTBIT_USE & lwCarSta))
    {
        return;
    }

    /* 少なくとも3G/LTEの1キャリアのTRX設定がなされていて、
     * その送信系指定において、合計2系が指定されていること.
     * そうなっていない場合は処理を終了する. */
    if      ((ltTrxsetParaLte[0].wSigKind != 0) && ((ltTrxsetParaLte[0].wSend & D_REC_RGV_BRCSPEC_02) == D_REC_RGV_BRCSPEC_02)) {}
    else if ((ltTrxsetParaLte[1].wSigKind != 0) && ((ltTrxsetParaLte[1].wSend & D_REC_RGV_BRCSPEC_02) == D_REC_RGV_BRCSPEC_02)) {}
    else    { return; }

	
    /* キャリア番号をAxCに変換する */
	if( f_cmn_com_syspara_lte_get(&syspar_lte) == D_RRH_OK)
	{
		/* シスパラ変数名EのLTE帯域幅を取得 */
    	lte_bw=syspar_lte.syspar_lte[17];
	}else{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "share memory get NG : f_cmn_com_syspara_lte_get");
		return;
	}
	
    /* Loop all carrier */
    for( lwCarCnt = 0; lwCarCnt < CMD_MAX_CARNUM_S3G; lwCarCnt++ )
    {
        /* 0系送信部の障害発生時に独立で送信系切替制御を実施.
         * 3Gは、配下RE単位で判定し、配下RE毎に独立で送信系切替制御を実施.
         * LTEは、キャリア単位で判定し、キャリア毎に独立で送信系切替制御を実施.
         *
         * ※0系送信部の障害発生時の定義
         * 3Gでは、LTEのREキャリア状態において、
         * 対象キャリア#1ブランチ#0REキャリア状態(DL)がALM場合
         * LTEでは、LTEのREキャリア状態において、
         * 対象キャリア#nブランチ#0REキャリア状態(DL)がALMの場合 */
        switch (ltCarstLte.car_every[lwCarCnt].car_num)
        {
        case CMD_CARNO_C1:
            (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_DIS_LSWAP0, &regVal);
            if (0 != (regVal & bitifno))
            {   /* 送信系切替済み */
                continue;
            }

            if ((0 != (CMD_RESSTBIT_ALM & ltCarstLte.car_every[lwCarCnt].br_every[0].car_st_dl))
            &&  (0 == (CMD_RESSTBIT_ALM & ltCarstLte.car_every[lwCarCnt].br_every[1].car_st_dl)))
            {
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "occur swap. carrier#1 cpri=%d br#0:%04x br#1:%04x",
                        re_cpri_num,
                        ltCarstLte.car_every[lwCarCnt].br_every[0].car_st_dl,
                        ltCarstLte.car_every[lwCarCnt].br_every[1].car_st_dl);
                BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_DIS_LSWAP0, bitifno);
                BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_DIS_WSWAP,  bitifno);
            }
            break;
    	case CMD_CARNO_C2:
        	/* 自分より若番のキャリア数をカウントする */
        	car_cnt=0;
        	for(car_idx=0; car_idx < ltCarstLte.car_every[lwCarCnt].car_num-1; car_idx++) /* pgr0692 */
	        {
        		/* キャリアcar_idxのLTE帯域幅が0以外の場合 */
        		if( (lte_bw & (0x0007 << (car_idx*3) ) ) != 0)
        		{
        			car_cnt++;
        		}
        	}
        	regAdd=swap_regAddr[car_cnt];
        	
            (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regAdd, &regVal);
            if (0 != (regVal & bitifno))
            {   /* 送信系切替済み */
                continue;
            }

            if ((0 != (CMD_RESSTBIT_ALM & ltCarstLte.car_every[lwCarCnt].br_every[0].car_st_dl))
            &&  (0 == (CMD_RESSTBIT_ALM & ltCarstLte.car_every[lwCarCnt].br_every[1].car_st_dl)))
            {
                BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "occur swap. carrier#2 cpri=%d br#0:%04x br#1:%04x",
                        re_cpri_num,
                        ltCarstLte.car_every[lwCarCnt].br_every[0].car_st_dl,
                        ltCarstLte.car_every[lwCarCnt].br_every[1].car_st_dl);
                BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, regAdd, bitifno);
            }
            break;
        default:
            break;
        }
    }
}
/* @} */  /* group TRIF_REC */

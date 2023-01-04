/*!
 *  @skip    $Id:$
 *  @file    dulib_api.c
 *  @brief	du command API library routine
 *  @author  FJT)Taniguchi
 *  @date    2014/01/15 by FJT)Taniguchi create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup    RRH_TP_CMD_LIB
 * @{
 */
#include "dulib_local.h"
#include "rrhApi_Inv.h"
#include "duApi_Ptp.h"

#define RRHLIB_API_INPUTMAX 1+11		/* API名+最大引数 */

typedef struct 
{
	char*			name;
	E_RRHAPI_RCODE	(*func)(INT qid, ...);
	INT				num;
	INT				type[ RRHLIB_API_INPUTMAX - 1 ];
}t_rrh_lib_api_entry;


const static t_rrh_lib_api_entry rrh_lib_api_entry[ ] = 
{
/* INV api */
	{ 
	  "rrhApi_Inv_Fsm_GetDevKind"    , (E_RRHAPI_RCODE(*)(INT qid, ...))rrhApi_Inv_Fsm_GetDevKind    ,
	   2 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
	{ 
	  "rrhApi_Inv_Fsm_GetTraInvInfo"    , (E_RRHAPI_RCODE(*)(INT qid, ...))rrhApi_Inv_Fsm_GetTraInvInfo    ,
	   2 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
/* INV api(ptp) */
	{ 
	  "duApi_Ptp_Mqr_PtpStart"         , (E_RRHAPI_RCODE(*)(INT qid, ...))duApi_Ptp_Mqr_PtpStart         ,
	   3 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
	{ 
	  "duApi_Ptp_Mqr_PtpShutdown"      , (E_RRHAPI_RCODE(*)(INT qid, ...))duApi_Ptp_Mqr_PtpShutdown      ,
	   3 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
	{ 
	  "duApi_Ptp_Mqr_SetTraceLogLevel" , (E_RRHAPI_RCODE(*)(INT qid, ...))duApi_Ptp_Mqr_SetTraceLogLevel ,
	   5 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
	{ 
	  "duApi_Ptp_Mqr_SetAprLogLevel"   , (E_RRHAPI_RCODE(*)(INT qid, ...))duApi_Ptp_Mqr_SetAprLogLevel   ,
	   4 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
	{ 
	  "duApi_Ptp_Mqr_GetTrafficLog"    , (E_RRHAPI_RCODE(*)(INT qid, ...))duApi_Ptp_Mqr_GetTrafficLog    ,
	   3 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
	{ 
	  "duApi_Ptp_Mqr_GetDevState"      , (E_RRHAPI_RCODE(*)(INT qid, ...))duApi_Ptp_Mqr_GetDevState      ,
	   3 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
	{ 
	  "duApi_Ptp_Mqr_GetPtpClock"      , (E_RRHAPI_RCODE(*)(INT qid, ...))duApi_Ptp_Mqr_GetPtpClock      ,
	   3 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
	{ 
	  "duApi_Ptp_Mpr_SubscribeSyncInfo", (E_RRHAPI_RCODE(*)(INT qid, ...))duApi_Ptp_Mpr_SubscribeSyncInfo,
	   3 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
	{ 
	  "duApi_Ptp_Mqr_GetPtpPram"       , (E_RRHAPI_RCODE(*)(INT qid, ...))duApi_Ptp_Mqr_GetPtpPram       ,
	   3 , { MRB_TT_FIXNUM, MRB_TT_FIXNUM, MRB_TT_FIXNUM  }
	},
/* funciton */
	{ NULL                          , NULL                        , 0 ,{0} }
};



/*!
 * @brief		api処理
 * @note
	<pre>
	スクリプト上からapiを実行するための関数である.
	api引数はapiに対応した引数が入力される.
	</pre>
 * @tparam		api名			[in]	String
 * @tparam		api引数(可変長)	[in]	Integer
 * @param		mrb				[in]	mrb status
 * @param		self			[in]	mrb self
 * @return		API復帰値
 * @retval		0:OK
 * @retval		0以外:NG
 * @date		2013/11/18 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.api()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */

mrb_value rrhlib_api(mrb_state* mrb, mrb_value self)
{
	mrb_value		*argv;
	unsigned int	count;
	unsigned int	count2;
	int				argc;
	int				type[RRHLIB_API_INPUTMAX];
	unsigned long	val[RRHLIB_API_INPUTMAX];
	char			str[RRHLIB_API_INPUTMAX][256];
	int				num;
	E_RRHAPI_RCODE	rtn;

	mrb_get_args(mrb, "*", &argv, &argc);

	rrhlib_dmsg("API call param num(%d) \n",argc);
	if(( argc > RRHLIB_API_INPUTMAX ) || (argc < 1) )
	{
		rrhlib_dmsg("API call error paramnum \n");
		return mrb_false_value();
	}
	memset(type,0,sizeof(type));
	/* apiチェック開始 */
	for( count = 0 ; count < argc ; count++ )
	{
		type[count] = mrb_type(argv[count]);
	}
	if(type[0] != MRB_TT_STRING )
	{
		rrhlib_dmsg("API call error inpara0 is not string \n");
		return mrb_false_value();
	}
	rtn = -1;
	memset(val,0,sizeof(val));
	memset(str,0,sizeof(str));

	for(count=0;count < ( sizeof(rrh_lib_api_entry )/sizeof( rrh_lib_api_entry[0])) ; count++)
	{
		if( strncmp(rrh_lib_api_entry[count].name,RSTRING_PTR(argv[0]),strlen(RSTRING_PTR(argv[0]))) )
		{
			continue;
		}
		
		if(rrh_lib_api_entry[count].num < (argc - 1) )
		{
			rrhlib_dmsg("API call error allnum %d \n",rrh_lib_api_entry[count].num);
			break;
		}
		memset(val,0,sizeof(val));
		memset(str,0,sizeof(str));
		num = rrh_lib_api_entry[count].num;
		for(count2 = 0; count2 < num ;count2++)
		{
			if( type[ count2 + 1 ] != rrh_lib_api_entry[count].type[ count2 ] )
			{
				rrhlib_dmsg("API call error type miss %x \n",type[ count2 + 1 ]);
				break;
			}
			switch ( type[ count2 + 1 ] )
			{
			 case MRB_TT_FIXNUM:
			  val[count2] = mrb_fixnum( argv[ count2 + 1 ] );
			  break;
			 case MRB_TT_STRING:
				strncpy(&str[count2][0],RSTRING_PTR(argv[ count2 + 1 ]),strlen(RSTRING_PTR(argv[ count2 + 1 ])));
			 	val[count2] = (unsigned long)&str[count2][0];
			  break;
			  default:
				rrhlib_dmsg("API call not supported types %x \n",type[ count2 + 1 ]);
				return mrb_false_value();
			   break;
			}
		}
		if(count2 < num )
		{
			rrhlib_dmsg("API call error allnum %d \n",rrh_lib_api_entry[count].num);
			break;
		}
		/* api call */
		rtn = (rrh_lib_api_entry[count].func)(val[0],val[1],val[2],val[3],val[4],val[5],val[6],val[7],val[8],val[9],val[10] );
		break;
	}
	rrhlib_dmsg("API call finish %s(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x) , return code is %X \n",
						RSTRING_PTR(argv[0]),
						val[0],
						val[1],
						val[2],
						val[3],
						val[4],
						val[5],
						val[6],
						val[7],
						val[8],
						val[9],
						val[10],
						rtn);
	return mrb_fixnum_value((int)rtn);
}


/*!
 * @brief        txon処理
 * @note
<pre>
 	txonを実施.
</pre>
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/09/11
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.txon
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_txon(mrb_state* mrb, mrb_value self)
{
    mrb_int         uiAntNum;
    mrb_int         uiCarrier;
    mrb_int         uiAccellId;
    mrb_int         uiCcId;
    mrb_int         uiDuPortId;
    mrb_int         uiCrCentFreq;
    mrb_int         uiBw;
    mrb_int         uiTxRx;
    mrb_int         uiTestFlg;
    mrb_int         uiScs;
    int             iRtn;

    mrb_get_args(mrb, "iiiiiiiiii",&uiAntNum, &uiCarrier, &uiAccellId, &uiCcId, &uiDuPortId, &uiCrCentFreq, &uiBw, &uiTxRx, &uiTestFlg, &uiScs);
    rrhlib_dmsg("txon start %x %x %x %x %x %x %x %x %x %x \n",uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId, uiCrCentFreq, uiBw, uiTxRx, uiTestFlg, uiScs);

    iRtn = rrhApi_Rfa_Mqr_CarSet(D_RRH_PROCQUE_TP,
                                 (unsigned int)uiAntNum ,
                                 (unsigned int)uiCarrier ,
                                 (unsigned int)uiAccellId ,
                                 (unsigned int)uiCcId ,
                                 (unsigned int)uiDuPortId ,
                                 (unsigned int)uiCrCentFreq ,
                                 (unsigned int)uiBw ,
                                 (unsigned int)uiTxRx ,
                                 (unsigned int)uiTestFlg ,
                                 (unsigned int)uiScs);
    if( iRtn != E_API_RCD_OK )
    {
        rrhlib_dmsg("txon error, errcode = %d \n", iRtn);
        return mrb_false_value();
    }
    rrhlib_dmsg("txon finish\n");
    return mrb_true_value();
}


/*!
 * @brief        txoff処理
 * @note
<pre>
 	txoffを実施.
</pre>
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/09/11
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.txoff
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_txoff(mrb_state* mrb, mrb_value self)
{
    mrb_int         uiAntNum;
    mrb_int         uiCarrier;
    mrb_int         uiCrCentFreq;
    mrb_int         uiTxRx;
    mrb_int         uiTestFlg;
    int             iRtn;

    mrb_get_args(mrb, "iiiii",&uiAntNum, &uiCarrier, &uiCrCentFreq, &uiTxRx, &uiTestFlg);
    rrhlib_dmsg("txoff start %x %x %x %x %x \n",uiAntNum, uiCarrier, uiCrCentFreq, uiTxRx, uiTestFlg);

    iRtn = rrhApi_Rfa_Mnt_CarRel(D_RRH_PROCQUE_TP,
                                 (unsigned int)uiAntNum ,
                                 (unsigned int)uiCarrier ,
                                 (unsigned int)uiCrCentFreq ,
                                 (unsigned int)uiTxRx ,
                                 (unsigned int)uiTestFlg);
    if( iRtn != E_API_RCD_OK )
    {
        rrhlib_dmsg("txoff error, errcode = %d \n", iRtn);
        return mrb_false_value();
    }
    rrhlib_dmsg("txoff finish\n");
    return mrb_true_value();
}

/*!
 * @brief        txgain処理
 * @note
<pre>
 	txgainを実施.
</pre>
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2019/03/29
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.txgain
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_api_txgain(mrb_state* mrb, mrb_value self)
{
	mrb_int         AntNum;
	mrb_int         Value;
	int             iRtn;

	mrb_get_args(mrb, "ii",&AntNum, &Value);
	rrhlib_dmsg("txgain start 0x%x 0x%x \n",AntNum, Value);
	iRtn = rrhApi_Cca_Mnt_PowerAdjust(D_RRH_PROCQUE_TP, 0, NULL, (USHORT)AntNum , (USHORT)Value);
	if( iRtn != E_API_RCD_OK ){
		rrhlib_dmsg("txgain  error, errcode = %d \n", iRtn);
		return mrb_false_value();
	}
	rrhlib_dmsg("txgain finish\n");
	return mrb_true_value();
}


/*!
 * @brief	TPコマンド用API系モジュール登録処理
 * @note
<pre>
	TPコマンド用のAPI系モジュールを登録する関数である.
</pre>
 * @param		mrb				[in]	mrb status
 * @param		module_class	[in]	module_class
 * @return		N/A
 * @date		2014/01/15
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void rrhlib_api_install(mrb_state* mrb ,struct RClass * module_class)
{
	rrhlib_dmsg("rrh lib api install start \n");

	/* scriptで使用できるモジュールを登録する */
	mrb_define_module_function(mrb, module_class, "api",rrhlib_api,	MRB_ARGS_ANY());
    mrb_define_module_function(mrb, module_class, "txon"  , rrhlib_device_txon  , MRB_ARGS_REQ(10));
    mrb_define_module_function(mrb, module_class, "txoff" , rrhlib_device_txoff , MRB_ARGS_REQ(5));
    mrb_define_module_function(mrb, module_class, "txgain" , rrhlib_api_txgain , MRB_ARGS_REQ(2));

	rrhlib_dmsg("rrh lib device access install OK \n");
	return;
}
/* @} */

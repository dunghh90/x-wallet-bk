/*!
 * @skip		$ld:$
 * @file		f_dbg_thread_exec_cmd_zync.c
 * @brief		デバッグスレッドコマンド実行処理
 * @author		alpha)宮崎
 * @date		2013/11/26 alpha)宮崎 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include "f_dbg_thread_inc.h"
#include <dlfcn.h>

/*!
 * @brief		f_dbg_thread_exec_cmd
 * @note  関数処理内容.
 *        -# 受信パラメータポインタチェック.
 *        	-  NULL時：
 *        		-# Assertログ取得関数Call(BPF_COM_LOG_ASSERT()).
 *           	-# エラーとして上位関数へ返る
 *        -# 受信メッセージの宛先チェック
 *        	-  NG時
 *           	-# Assertログ取得関数Call(BPF_COM_LOG_ASSERT()).
 *           	-# エラーとして上位関数へ返る
 *        -# 処理結果出力用のファイルをオープン(BPF_COM_LOG_OUTPUT_FILE_OPEN() )
 *        	-  NG時
 *          	 -# Assertログ取得関数Call(BPF_COM_LOG_ASSERT()).
 *          	 -# エラーとして上位関数へ返る
 *        -# 受信メッセージを解析して要求に応じた処理を呼び出し
 *        	-  NG時
 *           	-# Assertログ取得関数Call(BPF_COM_LOG_ASSERT()).
 *           	-# エラーとして上位関数へ返る
 *        -# 処理結果が正常時
 *        	-  NG時
 *           	-# Assertログ取得関数Call(BPF_COM_LOG_ASSERT()).
 *           	-# エラーとして上位関数へ返る
 *        -# 処理結果出力用のファイルをクローズ(BPF_COM_LOG_OUTPUT_FILE_CLOSE() )
 *        -# 処理結果のファイル名と処理結果を実行要求元へ送信
 *        	- NG時
 *           	-# Assertログ取得関数Call(BPF_COM_LOG_ASSERT()).
 *           	-# エラーとして上位関数へ返る
 *        -# 正常終了を戻り値として上位に返却.
 * @param		ulMyProcId	[in]UINT	自プロセスID
 * @param		ulMyThreaId	[in]UINT	自スレッドID
 * @param		msgP		[in]VOID*	受信メッセージ
 * @retval		D_RRH_OK	0:正常終了
 * @retval		D_RRH_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/26 alpha)宮崎
 */
extern T_DBGCMD_EXEC_FUNC   DebugCmdExecFunc[ D_CMD_DBG_EXEC_FUNCS_CNT ];

UINT f_dbg_thread_exec_cmd(T_DBGTHREAD_EXEC_INFO *execParam, VOID *msgP)
{
	/* 変数宣言	*/
	T_DBG_EXEC_CMD_REQ*			rcvMsgP;				/* 受信メッセージ		*/
	T_DBG_EXEC_CMD_RES*			sndMsgP;				/* 送信メッセージ		*/
	unsigned char				outFpath[512];			/* 出力先ファイル名		*/
	char						addFpath[100];
	int							bpf_result	;
	FILE						*outfp		;
	int							cmdResult	=	0;
	int							execStatus	=	0;
	int							ret			=	0;
	int							cmd_ind		;
	int							errcd		;
    void    					*dlHandle		;		/* DLハンドラ*/
    char    					*dlErrorMessage	;
	int							(*execFunc)(char *, FILE *fp);

	/* メッセージ内容チェック	*/
	if( msgP == NULL ) {
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Receive Message NULL (myProcQId:%08x, myThreadQId:%08x)", execParam->uiProcQID, execParam->uiDbgThreadQID);
		return D_RRH_NG;
	}
	rcvMsgP = (T_DBG_EXEC_CMD_REQ *)msgP	;


	/********************************/
	/* デバッグコマンド実行関数実行	*/
	/********************************/

	/* 実行結果を正常と初期化	*/
	execStatus = D_RRH_OK	;

	for( cmd_ind = 0 ; cmd_ind < D_CMD_DBG_EXEC_FUNCS_CNT ; cmd_ind++ ) {	/* 実行環境定義数分のループ	*/
		if( DebugCmdExecFunc[cmd_ind].myProcessQid ==  execParam->uiProcQID ) {	/* 実行対象が自プロセス(l3/l2/pf/mt) */
			if( ( strcmp(rcvMsgP->dbg_exec_req_info.cmd_exec_thread,  DebugCmdExecFunc[cmd_ind].execThreadName ) == 0) )	{		/* 受信メッセージの実行対象スレッドが定義済のもの */

				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER, "DBGCMD-EXEC:on myProcId:%08x thread:%s dlname:%s funcname%s cmd_str[%s]",
						execParam->uiProcID,
						DebugCmdExecFunc[cmd_ind].execThreadName,
						DebugCmdExecFunc[cmd_ind].dlName,
						DebugCmdExecFunc[cmd_ind].execFuncName,
						rcvMsgP->dbg_exec_req_info.cmd_str);

				/* ダイナミックリンクライブラリのオープン */
			    if(( dlHandle = dlopen( DebugCmdExecFunc[cmd_ind].dlName, RTLD_LAZY ) ) == (void *)NULL) {
                    BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Open dynamic link library Fail. name:%s reason:[%s]",
                        DebugCmdExecFunc[cmd_ind].dlName, dlerror() ) ;
                    execStatus = D_DBG_CMD_EXEC_DLOPEN_ERR;
			    	break;
			    }

			    /* 実行関数のポインタ取得   */
			    execFunc = dlsym( dlHandle, DebugCmdExecFunc[cmd_ind].execFuncName ) ;
			    dlErrorMessage = dlerror() ;
			    if( dlErrorMessage != NULL ) {       /* 失敗有り */
			    	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Execute Function Search Error: %s", dlErrorMessage );
			    	execStatus = D_DBG_CMD_EXEC_FUNC_ERR;

					/* ダイナミックリンクファイルクローズ	*/
					dlclose( dlHandle ) ;
			    	break;
			    }

				/* デバッグ出力用ファイルオープン	*/
				strcpy( addFpath, execParam->cpDbgThreadName );
				strcat( addFpath, "-");
				strcat( addFpath, rcvMsgP->dbg_exec_req_info.cmd_exec_thread ) ;
				if((outfp = BPF_COM_LOG_OUTPUT_FILE_OPEN(
						addFpath,
						E_BPF_LOG_CMDLOG_OUTDIR_NORMAL,
						outFpath )) == NULL ) {
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Output File open Error. (FilePath:%s)", addFpath);

					execStatus = D_DBG_CMD_EXEC_OFILE_OP_ERR;

					/* ダイナミックリンクファイルクローズ	*/
					dlclose( dlHandle ) ;
			    	break;
				}

			    /* 取得した関数の実行   */
				cmdResult = (*execFunc)( rcvMsgP->dbg_exec_req_info.cmd_str, outfp ) ;

				/* ダイナミックリンクファイルクローズ	*/
				dlclose( dlHandle ) ;


				/* デバッグ出力用ファイルクローズ	*/
				if(( bpf_result = BPF_COM_LOG_OUTPUT_FILE_CLOSE( outfp )) != 0 ) {
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Output File close Error.(FilePath:%s)", outFpath );
			    	execStatus = D_DBG_CMD_EXEC_OFILE_CL_ERR;
			    	break;
				}

		    	execStatus = D_DBG_CMD_EXEC_SUCCESS ;
				break;
			}
		}
	}


	/************************/
	/* デバッグ出力結果送信	*/
	/************************/

	/* 通信用共有メモリを取得する	*/
	bpf_result = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別		*/
										sizeof(T_DBG_EXEC_CMD_RES),					/* Size				*/
										(void **)&sndMsgP,								/* msgP				*/
										&errcd );
	if( bpf_result != D_RRH_OK ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BUFFER GET NG %d \n", bpf_result );
		return D_RRH_NG;
	}

	/* 送信MSG作成(送信先は実行コマンドを含むDBGスレッド)	*/
	sndMsgP->head.uiEventNo		= CMD_TSKIF_DBG_EXEC_CMD_RES;			/* デバッグコマンド実行要求			*/
	sndMsgP->head.uiDstPQueueID	= rcvMsgP->head.uiSrcPQueueID	;		/* 送信先Process Queue ID			*/
	sndMsgP->head.uiDstTQueueID	= rcvMsgP->head.uiSrcTQueueID	;		/* 送信先Thread Queue ID			*/
	sndMsgP->head.uiSrcPQueueID	= execParam->uiProcQID			;		/* 送信元Process Queue ID			*/
	sndMsgP->head.uiSrcTQueueID	= execParam->uiDbgThreadQID		;		/* 送信元Thread Queue ID			*/
	sndMsgP->head.uiLength		= sizeof(T_DBG_EXEC_CMD_RES);			/* 送信メッセージサイズ				*/
	sndMsgP->head.uiSignalkind	= 0	;									/* 送信シグナル						*/

	sndMsgP->dbg_exec_res_info.cmd_exec_status	= execStatus;			/* コマンド実行結果					*/
	sndMsgP->dbg_exec_res_info.cmd_exec_result 	= cmdResult;			/* 実行コマンドの処理結果			*/
	strncpy( sndMsgP->dbg_exec_res_info.cmd_result_fname, (char *)outFpath, sizeof(sndMsgP->dbg_exec_res_info.cmd_result_fname) ) ;	/* 出力先ファイル名	*/

	/* コマンド実行結果を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(	rcvMsgP->head.uiSrcPQueueID,			/* 送信先Process Queue ID			*/
									(void *)sndMsgP );					/* 送信Message						*/
	if( ret != BPF_RU_IPCM_OK )	{
		fprintf(stderr, "command request send error.  code:%d\n", ret );
		return -1;
	}
	return D_RRH_OK;
}

/* @} */

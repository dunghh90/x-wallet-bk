/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_Fregister_tim.c
 *  @brief  to register timer
 *  @date   20070316  FFCS)linjunhua create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2007
 */
 
#include "f_trp_com.h"				/*TRIFプロセス共通ヘッダ			*/
#include "re_com_inc.h"

/*! @addtogroup TRIF_TCM
 * @{ */


/*!
 *  @brief to register timer
 *  @note firstly, get self thread_id, initialize the key_inf, then register timer to system
 *  @param  notify_type [IN] once or period
 *  @param  time [IN] timer value
 *  @param  key_inf [IN] the infomation that user saved
 *  @param  timerid [OUT] the timer id that return to user
 *  @param  errcd [OUT] error code that return to user
 *  @return result
 *  @retval CMD_NG failed to register timer
 *	@retval CMD_OK success
 *  @date 20070316 FFCS)linjunhua create
 */
  
INT f_trp_com_Fregister_tim_M(bpf_ru_hrtm_notify_type_t notify_type,
								UINT time, 
								bpf_ru_hrtm_key_inf_t key_inf,
								UINT timerid,
								INT* errcd,
								UCHAR* file_p, UINT line)
{
#if 0
	INT retv;
	UINT bpfid;
	UINT osid;
#endif
	UINT ofst;
	USHORT timVal;
	USHORT timValExt;
	USHORT count;
	USHORT toNum;
	UCHAR keyinf[4];
	UINT time_10m;
	UCHAR tmkName[41][13] = 
	{
		"RETIMSET_INT",  /*!< (00)RE時刻設定送信間隔タイマ                 */
		"TOFVAL_INT  ",  /*!< (01)Toffset値報告送信間隔タイマ              */
		"TOFVAL_CNF  ",  /*!< (02)Toffset値報告確認タイマ                  */
		"REEQPINF_INT",  /*!< (03)RE装置構成情報報告送信間隔タイマ         */
		"REEQPINF_CNF",  /*!< (04)RE装置構成情報報告確認タイマ             */
		"RESLTINF_CNF",  /*!< (05)REスロット情報報告確認タイマ             */
		"RESTS_INT   ",  /*!< (06)RE状態報告送信間隔タイマ                 */
		"RESTS_CNF   ",  /*!< (07)RE状態報告確認タイマ                     */
		"REPRTSTS_INT",  /*!< (08)RE PORT状態報告送信間隔タイマ            */
		"REPRTSTS_CNF",  /*!< (09)RE PORT状態報告確認タイマ                */
		"RECRDSSC_INT",  /*!< (10)REカード状態報告(セクタ部)送信間隔タイマ */
		"RECRDSSC_CNF",  /*!< (11)REカード状態報告(セクタ部)確認タイマ     */
		"RECRDSCM_INT",  /*!< (12)REカード状態報告(共通部)送信間隔タイマ   */
		"RECRDSCM_CNF",  /*!< (13)REカード状態報告(共通部)確認タイマ       */
		"RECRRSTS_INT",  /*!< (14)REキャリア状態報告送信間隔タイマ         */
		"RECRRSTS_CNF",  /*!< (15)REキャリア状態報告確認タイマ             */
		"RERST_CNF   ",  /*!< (16)REリセット確認タイマ                     */
		"REFRCRST_CNF",  /*!< (17)RE強制リセット確認タイマ                 */
		"2NDRST_WAT  ",  /*!< (18)二次リセット待ちタイマ                   */
		"ACTCHG_WAT  ",  /*!< (19)Active切替指示送信待ちタイマ             */
		"T14RTY_WAT  ",  /*!< (20)遅延補正値設定待ちタイマ                 */
		"TXS_CNF     ",  /*!< (21)TRX設定確認タイマ                        */
		"TXR_CNF     ",  /*!< (22)TRX解放確認タイマ                        */
		"NMC_CNF     ",  /*!< (23)無変調キャリアON/OFF指定確認タイマ       */
		"REVER_CNF   ",  /*!< (24)REバージョン報告確認タイマ               */
		"RECRDCTL_CNF",  /*!< (25)REカード制御確認タイマ                   */
		"RECRDDIA_CNF",  /*!< (26)REカード診断確認タイマ                   */
		"REPRTCTL_CNF",  /*!< (27)RE PORT制御確認タイマ                    */
		"RELOGGET_CNF",  /*!< (28)RE 障害ログ取得確認タイマ                */
		"RELOGSTP_CNF",  /*!< (29)RE 障害ログ中止確認タイマ                */
		"RELOGSND_CNF",  /*!< (30)RE 障害ログ送信確認タイマ                */
		"REFILUP_CNF ",  /*!< (31)REファイル更新確認タイマ                 */
		"REAUTRST_WAT",  /*!< (32)RE auto reset 送信待ちタイマ             */
		"ACTCHG_WAT2 ",  /*!< (33)Active切替指示送信待ちタイマ(M-LRE2)     */
		"ACTCHG_WAT3 ",  /*!< (34)Active切替指示送信待ちタイマ(M-LRE3)     */
		"ACTCHG_WAT4 ",  /*!< (35)Active切替指示送信待ちタイマ(M-LRE4)     */
		"ACTCHG_WAT5 ",  /*!< (36)Active切替指示送信待ちタイマ(M-LRE5)     */
		"ACTCHG_WAT6 ",  /*!< (37)Active切替指示送信待ちタイマ(M-LRE6)     */
		"ACTCHG_WAT7 ",  /*!< (38)Active切替指示送信待ちタイマ(M-LRE7)     */
		"MT_ADD_SET  ",  /*!< (39)MTアドレス設定確認タイマ					*/
		"10MSEC_INT  "	 /*!< (40)10msec周期タイマ							*/
	};
#if 0
	UINT loop;	/*	for dbg	*/
	UINT wstart, wlast, wnext, wofst, wnum;	/*	for dbg	*/
#endif

	if(errcd == NULL)
	{
		/*parameter error*/
		F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_ALERT, 
			"Invalid para: NULL",  
			(UCHAR *)NULL,
			CMD_NUM0);
		return CMD_NG;
	}
	keyinf[0] = (UCHAR)(key_inf.keyinf1 & 0xff);
	keyinf[1] = (UCHAR)((key_inf.keyinf1>>8) & 0xff);
	keyinf[2] = (UCHAR)((key_inf.keyinf1>>16) & 0xff);
	keyinf[3] = (UCHAR)((key_inf.keyinf1>>24) & 0xff);
	if(keyinf[0] > CMD_MAXSLOTNO)
	{
		*errcd = CMD_NG;
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "Ext No Err:0x%08x(%d;%d;%d;%d)tid=%d:%s(%d)",
													 key_inf.keyinf1, keyinf[0], keyinf[1], keyinf[2], keyinf[3], timerid, file_p, line);
		return CMD_NG;
	}
	else if(keyinf[0] == 0)
	{
		keyinf[0]=1;
	}
	if((keyinf[1] < 1) || (keyinf[1] > 16))
	{
		*errcd = CMD_NG+1;
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "cpri No Err:0x%08x(%d;%d;%d;%d)tid=%d:%s(%d)",
													 key_inf.keyinf1, keyinf[0], keyinf[1], keyinf[2], keyinf[3], timerid, file_p, line);
		return CMD_NG;
	}
	if(keyinf[2] > 64)
	{
		*errcd = CMD_NG+2;
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "timer kind Err:0x%08x(%d;%d;%d;%d)tid=%d:%s(%d)",
													 key_inf.keyinf1, keyinf[0], keyinf[1], keyinf[2], keyinf[3], timerid, file_p, line);
		return CMD_NG;
	}
	if((keyinf[3] == 2) || (keyinf[3] == 4))
	{
		keyinf[3] = CMD_SYS_LTE;
	}
	else if((keyinf[3] == 3) || (keyinf[3] == 5))
	{
		keyinf[3] = CMD_SYS_3G;
	}
	else
	{
		*errcd = CMD_NG+3;
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "timer SYS No Err:0x%08x(%d;%d;%d;%d)tid=%d:%s(%d)",
													 key_inf.keyinf1, keyinf[0], keyinf[1], keyinf[2], keyinf[3], timerid, file_p, line);
		return CMD_NG;
	}
	keyinf[0]--;
	keyinf[1]--;
	
	/*	既にタイマ開始している場合	*/
	if((f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].count != 0xffff) ||
		(f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].ofst != 0xffff))
	{
		count = f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].count;
		ofst = f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].ofst;
		timVal = f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].timvalSav;
		timValExt = f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].dummy;
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_WARNING, "timer start double tStop:0x%08x(slt=%d;cpr=%d;tmk=%d[%s];sys=%d)tId=%d,tVal=%d,sCnt=%d,tCnt=%d,tOfst=%d)",
													 key_inf.keyinf1, keyinf[0]+1, keyinf[1]+1, keyinf[2], tmkName[keyinf[2]], keyinf[3], timerid, timVal, timValExt, count, ofst);
		f_trp_com_Fcancel_tim(key_inf.keyinf1);
	}
	
	time_10m = time/10;
	if((time_10m == 0) || (time_10m > 720000))
	{
		*errcd = CMD_NG+4;
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "timer value Err:0x%08x(%d;%d;%d;%d)tid=%d:%s(%d)",
													 key_inf.keyinf1, keyinf[0], keyinf[1], keyinf[2], keyinf[3], timerid, file_p, line);
		return CMD_NG;
	}
	/*	Timer値が2000msec(2sec)以下の場合	*/
	if(time_10m <= 200)
	{
		/*	タイムアウト回数を1回する	*/
		toNum = 1;
		/*	タイマ値を算出	*/
		timVal = (USHORT)time_10m;
		timValExt = timVal;
	}
	/*	Timer値が2010msec~7200000msec(2hour)の場合	*/
	else
	{
		/*	タイムアウト回数を算出する	*/
		toNum = (USHORT)time_10m/200;
		/*	最後のタイムアウト値を算出する	*/
		timValExt = (USHORT)(time_10m%200);
		if(timValExt != 0)
		{
			toNum++;
		}
		else
		{
			timValExt = 200;
		}
		/*	タイマ値を2秒後とする	*/
		timVal = 200;
	}
	/*	タイムアウト位置(次のタイムアウトから)を設定する	*/
	/*	単発タイマの場合は次のタイムアウトからカウントするので、プラス1する	*/
	if(notify_type != BPF_RU_HRTM_REG_PERIODIC)
	{
		timVal++;
	}
	count = (f_trp_com_timerMngTbl.count + timVal) % 256;
	/*	タイマ数が既に384の場合	*/
	if(f_trp_com_timerMngTbl.info[count].num > 383)
	{
		*errcd = (CMD_NG+5);
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "timer num Over([%d]%d):%s(%d)", count, f_trp_com_timerMngTbl.info[count].num, file_p, line);
		return CMD_NG;
	}
	/*	同じタイムアウト位置に登録されているNEXTオフセットを取得する	*/
	ofst = f_trp_com_timerMngTbl.info[count].next;
	if(ofst == 0xffff)
	{
		*errcd = (CMD_NG+6);
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "next timer not exsit([%d]%d):%s(%d)", count, ofst, file_p, line);
		return CMD_NG;
	}
	/*	タイムアウト情報を設定する	*/
	f_trp_com_timerMngTbl.info[count].tim_typ[ofst] = notify_type;
	f_trp_com_timerMngTbl.info[count].tim_info[ofst] = key_inf.keyinf1;
	f_trp_com_timerMngTbl.info[count].tim_kind[ofst] = keyinf[2];
	f_trp_com_timerMngTbl.info[count].tim_cpri[ofst] = keyinf[1];
	f_trp_com_timerMngTbl.info[count].tim_extNo[ofst] = keyinf[0];
	f_trp_com_timerMngTbl.info[count].tim_sysNo[ofst] = keyinf[3];
	/*	登録数を更新する	*/
	f_trp_com_timerMngTbl.info[count].num++;
	f_trp_com_timerMngTbl.useNum++;
	/*	最終オフセットを更新する	*/
	f_trp_com_timerMngTbl.info[count].next = f_trp_com_timerMngTbl.info[count].tim_next[ofst];
	
	/*	タイマ情報を保存する	*/
	f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].count = count;
	f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].ofst = ofst;
	f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].timval = timValExt;
	f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].times = toNum;
	f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].timvalSav = time;
	f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].timerID = timerid;
	f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].dummy = f_trp_com_timerMngTbl.count;
	if(f_trp_com_timerMngTbl.useNum > f_trp_com_timerMngTbl.useNumMax)
	{
		f_trp_com_timerMngTbl.useNumMax = f_trp_com_timerMngTbl.useNum;
	}
	if(f_trp_com_timerMngTbl.info[count].num > f_trp_com_timerMngTbl.info[count].numMax)
	{
		f_trp_com_timerMngTbl.info[count].numMax = f_trp_com_timerMngTbl.info[count].num;
	}
#if 0
	/************	for dbg sta	************/
	if(logflg_ent == 0)
	{
		wnum = f_trp_com_timerMngTbl.info[count].num;
		wstart = wofst = f_trp_com_timerMngTbl.info[count].start;
		wlast = f_trp_com_timerMngTbl.info[count].last;
		wnext = f_trp_com_timerMngTbl.info[count].next;
		for(loop=0; loop<wnum; loop++)
		{
			wofst = f_trp_com_timerMngTbl.info[count].tim_next[wofst];
			if(wofst == 0xffff)
			{
				if(loop<383)
				{
					BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "next err[%d](c=%d, s=%d,l=%d,n=%d,[%d][%d]:id=%08x,time=%d(%d,%d,%d,%d),f=%d)", 
									loop, wnum, wstart, wlast, wnext, count, wofst, timerid, time, keyinf[2] ,keyinf[1], keyinf[0], keyinf[3], logflg_ent);
					logflg_ent++;
					break;
				}
			}
		}
		if(wnext != wofst)
		{
			BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "wstart err(c=%d, s=%d,l=%d,n=%d,[%d][%d]:id=%08x,time=%d(%d,%d,%d,%d),f=%d)", 
							wnum, wstart, wlast, wnext, count, wofst, timerid, time, keyinf[2] ,keyinf[1], keyinf[0], keyinf[3], logflg_ent);
			logflg_ent++;
		}
		wofst = wnext;
		for(loop=0; loop<wnum; loop++)
		{
			wofst = f_trp_com_timerMngTbl.info[count].tim_before[wofst];
			if(wofst == 0xffff)
			{
				if(loop<383)
				{
					BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "brfore err[%d](c=%d, s=%d,l=%d,n=%d,[%d][%d]:id=%08x,time=%d(%d,%d,%d,%d),f=%d)", 
									loop, wnum, wstart, wlast, wnext, count, wofst, timerid, time, keyinf[2] ,keyinf[1], keyinf[0], keyinf[3], logflg_ent);
					logflg_ent++;
					break;
				}
			}
		}
		if(wstart != wofst)
		{
			BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "wnext err(c=%d, s=%d,l=%d,n=%d,[%d][%d]:id=%08x,time=%d(%d,%d,%d,%d),f=%d)", 
							wnum, wstart, wlast, wnext, count, wofst, timerid, time, keyinf[2] ,keyinf[1], keyinf[0], keyinf[3], logflg_ent);
			logflg_ent++;
		}
	}
	/************	for dbg end	************/
#endif
#if 0
	/*   タイマ登録  */
	retv = BPF_RU_HRTM_REGISTER_ID(
			notify_type,					/* Periodic timer 				*/
			timerid,						/* Time Id 						*/
			time,							/* Time							*/
			CMD_TSKIF_TIMOUTNTC,			/* event 						*/
			CMD_TSKID_REC,					/* Event notification entry		*/
			errcd,							/* Error Code					*/
			&bpfid,
			&osid,
			key_inf.keyinf1);

	if(retv != BPF_RU_HRTM_COMPLETE)
	{
		/* 入力パラメータ異常 or リソースビジー or タイマ登録失敗   */
		
		F_COMF_CMM_FINVDLOG( 
			D_TCM_INVLOGLV_WARN2, 					/* Logging Level	*/
			"Timer start NG",						/* User Data 1	*/
			(UCHAR*)&retv,							/* User Data 2	*/	
			sizeof(retv));							/* User Data 2 size	*/
		return CMD_NG;
	}
#endif
	return CMD_OK;
}
/* @} */  /* group TRIF_TCM */


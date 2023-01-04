/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_Fcancel_tim.c
 *  @brief  cancel the register timer
 *  @date   20070316  FFCS)linjunhua create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2007
 */
 
#include "f_trp_com.h"				/* TRIFプロセス共通ヘッダ			*/
/*! @addtogroup TRIF_TCM
 * @{ */

/*!
 *  @brief cancel the register timer
 *  @note call BPF function
 *  @param  timerid [IN] the timer ID
 *  @return result
 *  @retval CMD_NG failed to cancel timer
 *	@retval CMD_OK success
 *  @date 20070316 FFCS)linjunhua create
 */
  
INT    f_trp_com_Fcancel_tim_M(UINT timerid, UCHAR* file_p, UINT line)
{
#if 0
	INT retv;
	INT errcd;
#endif
	UCHAR keyinf[4];
	UINT count;
	UINT ofst;
	USHORT before;
	USHORT next;
	UINT last;
#if 0
	UINT loop;	/*	for dbg	*/
	UINT wstart, wlast, wnext, wofst, wnum;	/*	for dbg	*/
#endif
	/*  タイマキャンセル処理開始  */
	keyinf[0] = (UCHAR)(timerid & 0xff);
	keyinf[1] = (UCHAR)((timerid>>8) & 0xff);
	keyinf[2] = (UCHAR)((timerid>>16) & 0xff);
	keyinf[3] = (UCHAR)((timerid>>24) & 0xff);
	
	if(keyinf[0] > CMD_MAXSLOTNO)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Ext No Err=0x%08x(%d;%d;%d;%d):%s(%d)", 
							timerid, keyinf[0], keyinf[1], keyinf[2], keyinf[3], file_p, line);
		return CMD_NG;
	}
	else if(keyinf[0] == 0)
	{
		keyinf[0]=1;
	}
	if((keyinf[1] < 1) || (keyinf[1] > 16))
	{
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "cpri No Err=0x%08x(%d;%d;%d;%d):%s(%d)", 
							timerid, keyinf[0], keyinf[1], keyinf[2], keyinf[3], file_p, line);
		return CMD_NG;
	}
	if(keyinf[2] > 64)
	{
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "timer kind Err=0x%08x(%d;%d;%d;%d):%s(%d)", 
							timerid, keyinf[0], keyinf[1], keyinf[2], keyinf[3], file_p, line);
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
		BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "timer SYS No Err=0x%08x(%d;%d;%d;%d):%s(%d)", 
							timerid, keyinf[0], keyinf[1], keyinf[2], keyinf[3], file_p, line);
		return CMD_NG;
	}
	keyinf[0]--;
	keyinf[1]--;
	
	count = f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].count;
	ofst  = f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].ofst;
	if((count > 255) || (ofst > 383))
	{
		return CMD_NG;
	}
	/*	タイマ生成中の場合	*/
	if(f_trp_com_timerMngTbl.info[count].tim_typ[ofst] != 0)
	{
		if(timerid != f_trp_com_timerMngTbl.info[count].tim_info[ofst])
		{
			BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "timer info Err=0x%08x!=0x%08x(%d;%d):%s(%d)", 
								timerid, f_trp_com_timerMngTbl.info[count].tim_info[ofst], count, ofst, file_p, line);
		}
		/*	タイムアウト情報をクリアする	*/
		f_trp_com_timerMngTbl.info[count].tim_typ[ofst] = 0;
		f_trp_com_timerMngTbl.info[count].tim_info[ofst] = 0;
		
		/*	タイマ数を減算する	*/
		if(f_trp_com_timerMngTbl.info[count].num == 0)
		{
			BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "timer no set([%d]=%d)0x%08x:%s(%d)", 
							count, f_trp_com_timerMngTbl.info[count].num, timerid, file_p, line);
			return CMD_NG;
		}
		else
		{
			f_trp_com_timerMngTbl.info[count].num--;
		}
		if(f_trp_com_timerMngTbl.useNum == 0)
		{
			BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "timer no set([%d]=%d)0x%08x:%s(%d)", 
							count, f_trp_com_timerMngTbl.useNum, timerid, file_p, line);
			return CMD_NG;
		}
		else
		{
			f_trp_com_timerMngTbl.useNum--;
		}
		next = f_trp_com_timerMngTbl.info[count].tim_next[ofst];
		
		/*	削除したタイマ情報が一番最後で無い場合		*/
		if(next < 384)
		{
			/*	削除したタイマ上をチェーンから外す	*/
			before = f_trp_com_timerMngTbl.info[count].tim_before[ofst];
			f_trp_com_timerMngTbl.info[count].tim_before[next] = before;
			/*	削除したタイマ情報が一番最初でない場合		*/
			if(before < 384)
			{
				f_trp_com_timerMngTbl.info[count].tim_next[before] = next;
			}
			/*	削除したタイマ情報が一番最初の場合		*/
			else
			{
				/*	2番目のタイマ情報を一番最初にする	*/
				f_trp_com_timerMngTbl.info[count].start = next;
			}
			/*	削除したタイマ情報を一番最後に繋げる	*/
			last = f_trp_com_timerMngTbl.info[count].last;
			f_trp_com_timerMngTbl.info[count].tim_next[last] = ofst;
			
			f_trp_com_timerMngTbl.info[count].tim_before[ofst] = last;
			f_trp_com_timerMngTbl.info[count].tim_next[ofst] = 0xffff;
			f_trp_com_timerMngTbl.info[count].last = ofst;
		}
		else
		{
			/*	最終オフセットを更新する	*/
			f_trp_com_timerMngTbl.info[count].next = ofst;
		}
	}
	else
	{
		cm_Assert(	CMD_ASL_USELOW, timerid, "delete not starting timer");
	}
	/*	タイマ情報をクリアする	*/
	f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].count = 0xffff;
	f_trp_com_timerInfo[keyinf[2]][keyinf[1]][keyinf[0]][keyinf[3]].ofst = 0xffff;
#if 0
	/************	for dbg	************/
	if(logflg == 0)
	{
		wnum = f_trp_com_timerMngTbl.info[count].num;
		wstart = wofst = f_trp_com_timerMngTbl.info[count].start;
		wlast = f_trp_com_timerMngTbl.info[count].last;
		wnext = f_trp_com_timerMngTbl.info[count].next;
		if(f_trp_com_timerMngTbl.info[count].tim_before[wofst] != 0xffff)
		{
			BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "wstart err(c=%d, s=%d,l=%d,n=%d,[%d][%d].before=%d:id=%08x,ofs=%d,f=%d)", 
							wnum, wstart, wlast, wnext, count, wofst, f_trp_com_timerMngTbl.info[count].tim_before[wofst], timerid, ofst, logflg);
			logflg++;
		}
		for(loop=0; loop<383; loop++)
		{
			next = f_trp_com_timerMngTbl.info[count].tim_next[wofst];
			before = f_trp_com_timerMngTbl.info[count].tim_before[next];
			if(wofst != before)
			{
				BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "before err[%d](c=%d, s=%d,l=%d,n=%d,[%d][%d][%d].before=%d:id=%08x,ofs=%d,f=%d)", 
								loop, wstart, wlast, wnext, count, next, wofst, before, timerid, ofst, logflg);
				logflg++;
				break;
			}
			if(loop < wnum)
			{
				if(f_trp_com_timerMngTbl.info[count].tim_typ[wofst] == 0)
				{
					BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "typ err1[%d](c=%d, s=%d,l=%d,n=%d,[%d][%d].typ=%d:id=%08x,ofs=%d,f=%d)", 
									loop, wnum, wstart, wlast, wnext, count, wofst, f_trp_com_timerMngTbl.info[count].tim_typ[wofst], timerid, ofst, logflg);
					logflg++;
				}
			}
			else
			{
				if(f_trp_com_timerMngTbl.info[count].tim_typ[wofst] != 0)
				{
					BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "typ err2[%d](c=%d, s=%d,l=%d,n=%d,[%d][%d].typ=%d:id=%08x,ofs=%d,f=%d)", 
									loop, wnum, wstart, wlast, wnext, count, wofst, f_trp_com_timerMngTbl.info[count].tim_typ[wofst], timerid, ofst, logflg);
					logflg++;
				}
			}
			wofst = next;
		}
		wofst = wlast;
		if(f_trp_com_timerMngTbl.info[count].tim_next[wofst] != 0xffff)
		{
			BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "wlast err(c=%d, s=%d,l=%d,n=%d,[%d][%d].next=%d:id=%08x,ofs=%d,f=%d)", 
							wnum, wstart, wlast, wnext, count, wofst, f_trp_com_timerMngTbl.info[count].tim_next[wofst], timerid, ofst, logflg);
			logflg++;
		}
		for(loop=0; loop<383; loop++)
		{
			before = f_trp_com_timerMngTbl.info[count].tim_before[wofst];
			next = f_trp_com_timerMngTbl.info[count].tim_next[before];
			if(wofst != next)
			{
				BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "next err[%d](c=%d, s=%d,l=%d,n=%d,[%d][%d][%d].next=%d:id=%08x,ofs=%d,f=%d)", 
								loop, wnum, wstart, wlast, wnext, count, before, wofst, next, timerid, ofst, logflg);
				logflg++;
				break;
			}
			wofst = before;
		}
		wofst = wnext;
		if(wofst != 0xffff)
		{
			if(f_trp_com_timerMngTbl.info[count].tim_typ[wofst] != 0)
			{
				BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "wnext err1(c=%d, s=%d,l=%d,n=%d,[%d][%d].typ=%d:id=%08x,ofs=%d,f=%d)", 
								 wnum, wstart, wlast, wnext, count, wofst, f_trp_com_timerMngTbl.info[count].tim_typ[wofst], timerid, ofst, logflg);
				logflg++;
			}
			before = f_trp_com_timerMngTbl.info[count].tim_before[wofst];
			if(before != 0xffff)
			{
				if(f_trp_com_timerMngTbl.info[count].tim_typ[before] == 0)
				{
					BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "wnext err2(c=%d, s=%d,l=%d,n=%d,[%d][%d].typ=%d:id=%08x,ofs=%d,f=%d)", 
									 wnum, wstart, wlast, wnext, count, before, f_trp_com_timerMngTbl.info[count].tim_typ[before], timerid, ofst, logflg);
					logflg++;
				}
			}
		}
		else
		{
			BPF_COM_LOG_ASSERT(	D_RRH_LOG_AST_LV_ERROR, "wnext err3(c=%d, s=%d,l=%d,n=%d,[%d].next=%d:id=%08x,ofs=%d,f=%d)", 
							 wnum, wstart, wlast, wnext, count, wofst, timerid, ofst, logflg);
			logflg++;
		}
	}
	/************	for dbg end	************/
#endif
#if 0
	retv = BPF_RU_HRTM_CANCEL(timerid, (INT*)&errcd);
	if(retv != BPF_RU_HRTM_COMPLETE )
	{
		/* 入力パラメータ異常 or 指定タイマ未登録 or タイマキャンセル失敗  */
		/* it is normal case, do not need to make the error trace */
		return CMD_NG;
	}
#endif
	
	return CMD_OK;
}
/* @} */  /* group TRIF_TCM */


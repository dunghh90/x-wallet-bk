/************************************************************************/
/*	RRH装置	共通部品													*/
/*																		*/
/*	・データ一括圧縮(fldc4を使用)		f_com_FA_encode					*/
/*	・データ一括復元(fldc4を使用)		f_com_FA_decode					*/
/************************************************************************/
#include <string.h>
#include "fldccom.h"
#include "fldc4.h"


/************************************************************************/
/*																		*/
/*	NAME	: f_com_FA_encode											*/
/*																		*/
/*	FUNCTION: データ一括圧縮(fldc4を使用)								*/
/*																		*/
/*	CALLING	: int	f_com_FA_encode(InBuf,OutBuf,inlen,outbsz,blsadr,	*/
/*									outlen)								*/
/*																		*/
/*	INPUT	: 															*/
/*			unsigned char *InBuf	 	被圧縮データ用バッファアドレス	*/
/* 			unsigned char *OutBuf  	 	圧縮データ用バッファアドレス 	*/
/* 			unsigned int   inlen 		被圧縮データのバイトサイズ		*/
/*										※1以上を指定のこと				*/
/* 			unsigned int   outbsz 		圧縮データ用バッファバイトサイズ*/
/*										※圧縮率により必要サイズは異なる*/
/*										※最大値はinlen+(inlen/32768)*2	*/
/* 			FLDC4_ELEM 	  *blsadr 		fldc4用圧縮辞書アドレス 		*/
/* 						 					※209Kバイトの領域が必要	*/
/*																		*/
/*	OUTPUT	: 															*/
/* 			unsigned int   *outlen 		圧縮後のデータバイトサイズ	 	*/
/*																		*/
/*	RETURN	: 					 										*/
/*	  		NORMAL(0)	   	正常	 									*/
/*			BUFFERFULL(2)	圧縮データ用バッファサイズ不足				*/
/*			ERR_FATAL(5)	致命的なエラー								*/
/*			PARA_NG(9)		パラメータNG								*/
/*																		*/
/*	ITEM No	: RRH-001-001-008											*/
/*																		*/
/*	DATE	: 2008.10.02	FJT)T.Yanagisawa							*/
/*							MBBTS 08A(2008/10/02)版から取り出し			*/
/*																		*/
/*	UPDATE	: 2008.10.23	FJT)T.Yanagisawa							*/
/*							WDTクリア追加								*/
/*																		*/
/*	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008				*/
/*																		*/
/************************************************************************/
int f_com_FA_encode(
 unsigned char *InBuf,		 	/* 被圧縮データ用バッファアドレス	 	*/
 unsigned char *OutBuf, 	 	/* 圧縮データ用バッファアドレス 		*/
 unsigned int	inlen,	 		/* 被圧縮データのデータバイトサイズ		*/
 unsigned int	outbsz, 		/* 圧縮データ用バッファバイトサイズ		*/
 FLDC4_ELEM    *blsadr, 		/* fldc4用圧縮辞書アドレス 				*/
 unsigned int  *outlen			/* 圧縮後のデータバイトサイズ 			*/
)
{
	unsigned int		in_offset;			/* 非圧縮データ取り出し位置	*/
	unsigned int		out_offset;			/* 圧縮データ格納位置		*/
	unsigned int		org_inlen;	 		/* 被圧縮データサイズ		*/
	unsigned short int 	tmp_inlen;	 		/* 非圧縮データサイズ	 	*/
	unsigned short int 	tmp_outlen; 		/* 圧縮後データサイズ	 	*/
	int					rtc;				/* fldc4戻り値				*/

											/* 圧縮後データサイズ初期化	*/
	*outlen = 0;								/* pgr0541 */
											/* パラメータチェック		*/
	if (inlen == 0) {						/* 被圧縮データサイズが0	*/
		return(PARA_NG);
	}
											/* 圧縮辞書初期化			*/
	f_com_fldc4_init(blsadr);
											/* 圧縮準備					*/
	in_offset = 0;							/* 非圧縮データ取り出し位置	*/
	out_offset = 0;							/* 圧縮データ格納位置		*/
	org_inlen = inlen;				 		/* 被圧縮データサイズ記憶	*/

											/* 32KB単位で圧縮を繰り返す	*/
	do {
											/* 一回の圧縮サイズを決定	*/
		if (inlen > FLDC4_BUF_SIZE) {
			tmp_inlen = FLDC4_BUF_SIZE;		/* 一回の最大圧縮サイズ=32KB*/
		}
		else {
											/* 32KB以下を圧縮			*/
			tmp_inlen = (unsigned short int)inlen;
		}
											/* 圧縮用バッファ不足？		*/
		if (outbsz < (tmp_inlen + 2) + out_offset) {
											/* 圧縮用バッファサイズ不足	*/
			return(BUFFERFULL);
		}
											/* fldc4で最大32KBを圧縮	*/
		rtc = f_com_fldc4_encode(&InBuf[in_offset],&OutBuf[out_offset], /* pgr0541 */
							tmp_inlen, &tmp_outlen, blsadr);
											/* 圧縮失敗？				*/
		
#if 0
		/* WDT制御レジスタへクリア設定  */
		f_com_IO_write((UINT)D_SYS_REG_WDT, D_SYS_REG_WDT_WDTC);
#endif
		
		if (rtc != NORMAL){
			return( rtc );					/* 圧縮失敗で復帰			*/
		}
											/* 未圧縮サイズを更新		*/
		inlen	-= FLDC4_BUF_SIZE;
											/* 非圧縮データ取り出し位置	*/
		in_offset += FLDC4_BUF_SIZE;
											/* 圧縮データ格納位置更新	*/
		out_offset += tmp_outlen;

/************************************************************************/
/* WDT T.O防止が必要な場合、WAITまたはWDTクリアを追加してください		*/
/************************************************************************/

											/* 未圧縮データが残っている間*/
	} while(in_offset < org_inlen);
											/* 圧縮後のデータサイズ		*/
	*outlen = out_offset;
											/*	圧縮成功				*/
	return(NORMAL);
}

/************************************************************************/
/*																		*/
/*	NAME	: f_com_FA_decode											*/
/*																		*/
/*	FUNCTION: データ一括復元(fldc4を使用)								*/
/*																		*/
/*	CALLING	: int	f_com_FA_decode(InBuf,OutBuf,inlen,outbsz,			*/
/*									TmpBuf,blsadr,outlen)				*/
/*																		*/
/*	INPUT	: 															*/
/*			unsigned char *InBuf	 	圧縮データ用バッファアドレス	*/
/* 			unsigned char *OutBuf  	 	復元データ用バッファアドレス 	*/
/* 			unsigned int   inlen 		圧縮データのバイトサイズ		*/
/*											※3以上を指定のこと			*/
/* 			unsigned int   outbsz 		復元データ用バッファバイトサイズ*/
/* 			FLDC4_ELEM 	  *blsadr 		fldc4用圧縮辞書アドレス 		*/
/* 						 					※209Kバイトの領域が必要	*/
/*	 		unsigned char *TmpBuf		復元テンポラリバッファアドレス	*/
/*											※(32KB+2)の領域が必要		*/
/*																		*/
/*	OUTPUT	: 															*/
/* 			unsigned int   *outlen 		復元後のデータバイトサイズ	 	*/
/*																		*/
/*	RETURN	: 					 										*/
/*	  		NORMAL(0)	   	正常	 									*/
/*			BADCODE(1)		エラー(復元できないデータだった)			*/
/*			BUFFERFULL(2)	復元データ用バッファサイズ不足				*/
/*			PARA_NG(9)		パラメータNG								*/
/*																		*/
/*	ITEM No	: RRH-001-001-008											*/
/*																		*/
/*	DATE	: 2008.10.02	FJT)T.Yanagisawa							*/
/*							MBBTS 08A(2008/10/02)版から取り出し			*/
/*																		*/
/*	UPDATE	: 2008.10.23	FJT)T.Yanagisawa							*/
/*							WDTクリア追加								*/
/*																		*/
/*	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008				*/
/************************************************************************/
int f_com_FA_decode(
 unsigned char *InBuf,		 	/* 圧縮データ用バッファアドレス 		*/
 unsigned char *OutBuf, 	 	/* 復元データ用バッファアドレス 		*/
 unsigned int	inlen,	 		/* 圧縮データのデータバイトサイズ	 	*/
 unsigned int	outbsz, 		/* 復元データ用バッファバイトサイズ		*/
 unsigned char *TmpBuf,			/* 復元テンポラリバッファアドレス		*/
 FLDC4_ELEM    *blsadr,			/* fldc4用圧縮辞書アドレス 				*/
 unsigned int  *outlen	 		/* 復元後のデータバイトサイズ		 	*/
 
)
{
	unsigned int		in_offset;			/* 圧縮データ取り出し位置	*/
	unsigned int		out_offset;			/* 復元データ格納位置		*/
	unsigned short int 	tmp_inlen;	 		/* 圧縮データサイズ	 		*/
	unsigned short int 	tmp_outlen; 		/* 復元後データサイズ	 	*/
	int					rtc;				/* fldc4戻り値				*/

	short int 			mode;				/* 圧縮モード				*/
	unsigned short int 	tmp_size;			/* 圧縮制御情報				*/

											/* 復元後データサイズ初期化	*/
	*outlen = 0;								/* pgr0541 */
											/* パラメータチェック		*/
	if (inlen < 3) {						/* 圧縮データサイズが不正	*/
		return(PARA_NG);
	}
											/* 圧縮辞書初期化			*/
	f_com_fldc4_init(blsadr);
											/* 復元準備					*/
	in_offset = 0;							/* 圧縮データ取り出し位置	*/
	out_offset = 0;							/* 復元データ格納位置		*/

											/* 32KB単位で復元を繰り返す	*/
	do {
											/* 圧縮制御情報取得			*/
		tmp_size = (unsigned short)
				(InBuf[in_offset] * 256 + InBuf[in_offset+1]);/* pgr0541 */
											/* 圧縮データ取り出し位置	*/
		in_offset += 2;
											/* 圧縮モード取得			*/
		mode = (short)(tmp_size & 0x8000);
											/* 一回分の圧縮データサイズ	*/
		tmp_inlen = (unsigned short int)(tmp_size & 0x7FFF);
		if (tmp_inlen == 0) {
		   tmp_inlen = FLDC4_BUF_SIZE;		/* 一回の最大サイズ=32KB	*/
		}
											/* fldc4で最大32KBを復元	*/
		rtc = f_com_fldc4_decode(&InBuf[in_offset],TmpBuf,
								tmp_inlen, &tmp_outlen, mode, blsadr);
											/* 復元失敗？				*/
#if 0
		/* WDT制御レジスタへクリア設定  */
		f_com_IO_write((UINT)D_SYS_REG_WDT, D_SYS_REG_WDT_WDTC);
#endif
		if (rtc != NORMAL){
			return( rtc );					/* 復元失敗で復帰			*/
		}
											/* 復元用バッファ不足？		*/
		if ((outbsz - out_offset) < tmp_outlen) {
											/* 復元用バッファサイズ不足	*/
			return(BUFFERFULL);
		}
/************************************************************************/
/*以下の処理はvxWorks系以外のカードでは適切な転送処理に置き換えて下さい	*/
											/* 復元データを転送			*/
/*		cm_MemCpy((char*)&OutBuf[out_offset],(char*)TmpBuf,tmp_outlen) ;*/
/*		bcopy((char*)TmpBuf,(char*)&OutBuf[out_offset],tmp_outlen);(MB-SV card)*/
		memcpy((char*)&OutBuf[out_offset],(char*)TmpBuf,tmp_outlen) ;
/************************************************************************/

											/* 圧縮データ取り出し位置	*/
		in_offset += tmp_inlen;
											/* 復元データ格納位置更新	*/
		out_offset += tmp_outlen;

/************************************************************************/
/* WDT T.O防止が必要な場合、WAITまたはWDTクリアを追加してください		*/
/************************************************************************/

											/* 圧縮データが残っている間	*/
	} while(in_offset < inlen);
											/* 復元後のデータサイズ		*/
	*outlen = out_offset;
											/* 復元成功					*/
	return(NORMAL);
}

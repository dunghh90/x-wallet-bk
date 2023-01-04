/************************************************************************/
/*																		*/
/*	FILE-NAME : fldc4.c 										 		*/
/*	FUNCTION  : universal data Compression							  	*/
/*	INCLUDE   : fldccom.h fldc4.h										*/
/*	AUTHOR	  : Noriko Satoh											*/
/*	DATE	  : 1999/02/01												*/
/*	VERSION   : 1.9 											 		*/
/*																		*/
/*	  All Rights Reserved, Copyright(C) FUJITSU LABORATORIES LTD. 1998	*/
/*																		*/
/************************************************************************/
/*	DATE	: 2008.10.02	FJT)T.Yanagisawa							*/
/*							MBBTS 08A(2008/10/02)版から取り出し			*/
/*																		*/
/*	UPDATE	: 															*/
/*																		*/
/*	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008				*/
/************************************************************************/
/************************************************************************/
/*																		*/
/* 1th-order context tree + tree coding 					  			*/
/*				& Match string length coding & scramble table			*/
/*																		*/
/************************************************************************/

/************************************************************************/
/*																		*/
/* データ圧縮ソフト。バッファ単位（～32Kbyte）に圧縮し、ネガティブ		*/
/* 圧縮の場合（元データより却って大きくなる場合。ランダムなデータや 	*/
/* 既に圧縮されたデータ等を圧縮した場合におこる）はスクランブルをか 	*/
/* けて、そのまま出力する。 											*/
/* 圧縮時のバッファ単位ごとに、何バイトに圧縮できたか、圧縮したか否 	*/
/* かの情報を２バイトのヘッダーとして付加する。 						*/
/* 圧縮方式には「固定１次スプレイ符号と一致長符号」を使用する。 		*/
/*	   固定１次スプレイ符号:											*/
/*		 短い文字列の繰返しを圧縮するのに使用する。 					*/
/*		 直前の１バイトによって、符号化に使用するスプレイ符号木を		*/
/*		 切り替える。直前１バイトは下位７ビット、128通りに縮退する。 	*/
/*		 128通り各々に対し、葉の数が256+1（キャラクタ数：８ビットの 	*/
/*		 バリエーション＋END文字）のスプレイ符号木を準備。				*/
/*	   一致長符号:														*/
/*		 長い文字列の繰返しを圧縮するのに使用する。 					*/
/*		 過去に出現した３バイトが再度出現した場合、その３バイトより 	*/
/*		 後の文字列を一致長で符号化する。一致長は、過去の出現位置		*/
/*		 以降の文字列と入力文字列との一致する長さとする。				*/
/*																		*/
/************************************************************************/

/************************************************************************/
/*																		*/
/* 使用方法例： 													  	*/
/*	【基本構成】														*/
/*	 圧縮																*/
/*	   FLDC4_ELEM *blsadr;												*/
/*	   f_com_fldc4_init(blsadr);										*/
/*	   ファイルオープン();												*/
/*	   while(ファイル終了まで) { 										*/
/*		 被圧縮データ読み込み(バッファ単位:32KB);						*/
/*		 f_com_fldc4_encode(元データ, 圧縮データ, blsadr);				*/
/*	   }																*/
/*																		*/
/*	 復元																*/
/*	   FLDC4_ELEM *blsadr;												*/
/*	   f_com_fldc4_init(blsadr);										*/
/*	   ファイルオープン();												*/
/*	   while(ファイル終了まで) { 										*/
/*		 圧縮データ読み込み(２バイト：圧縮データのヘッダー);			*/
/*		 最上位1ビットがmodeを、残りの15ビットが圧縮後のサイズを示す 	*/
/*		 圧縮データ読み込み(圧縮後のサイズ単位);						*/
/*		 f_com_fldc4_decode(圧縮データ, 元データ, blsadr);				*/
/*	   }																*/
/*																		*/
/************************************************************************/
#include "fldccom.h"
#include "fldc4.h"

/************************************************************************/
/*	fldc4 define宣言													*/
/************************************************************************/
#define EofChar 	256 		/* ファイルの最後を示すキャラクタ 		*/
#define Root		0			/* スプレイ木のrootのノード番号 		*/

#define MaxLen_1	31			/* MaxLen -1 							*/
#define NIL 		0			/* tree node terminator 				*/
#define FLAG_FULL	0x0100		/* bit for 1byte stack 					*/

								/* Used to unpack bits and bytes 		*/
static const byte fldc4_BitMask[8]={1, 2, 4, 8, 16, 32, 64, 128};

								/* 一致長符号用の変数 					*/
#define fldc4_Mask1	0x00f8		/* mask for hash（ハッシュ検索） 		*/
#define fldc4_Mask2	0x0007		/* mask for hash（ハッシュ検索） 		*/

								/* 動作モニタ				 			*/
#define fldc4_monitor(variety,context,target) \
	{ printf("       %s : context %x target %x\n", (variety), (context), (target)); }

/************************************************************************/
/*	FLDC4内部関数プロトタイプ宣言										*/
/************************************************************************/
static void fldc4_Initialize_Splay(UpIndex *up,DownIndex *left,DownIndex *right);
static void fldc4_InitializeLen_Splay(UpIndex *upL,UpIndex *leftL,UpIndex *rightL);
static void fldc4_InitializeArray(word *prev);
static int	fldc4_matchlen_coding(FLDC4_ELEM *blsadr);
static int	fldc4_matchlen_decoding(FLDC4_ELEM *blsadr);
static void fldc4_Splay(byte context,word target,FLDC4_ELEM *blsadr);
static int	fldc4_Compress(byte context,word target,FLDC4_ELEM *blsadr);
static int	fldc4_Expand(byte context,word *target,FLDC4_ELEM *blsadr);
static void fldc4_SplayL(short int len,FLDC4_ELEM *blsadr);
static int 	fldc4_CompressL(short int len,FLDC4_ELEM *blsadr);
static int 	fldc4_ExpandL(short int *len,FLDC4_ELEM *blsadr);

static void fldc4_encode_xx(unsigned char *InBuf,unsigned char *OutBuf,
							unsigned short int inlen,unsigned short int *outlen,
							FLDC4_ELEM *blsadr,byte *pp_code);/* pgr0733 */

/************************************************************************/
/* initilize for encode and decode										*/
/*																		*/
/* fldc4用の圧縮辞書の初期化											*/
/* ネガティブ圧縮データ（圧縮できないデータ）用に、スクランブルを		*/
/* かけるための変換テーブルの初期化 									*/
/*																		*/
/*	  《戻値》	なし													*/
/************************************************************************/
void f_com_fldc4_init(FLDC4_ELEM *blsadr)	/* fldc4用圧縮辞書の構造体 	*/
{
 int i;
 int t_offset,offset;

/* スクランブル用のテーブルを初期化 			*/
/* （NtoSは圧縮時に、StoNは復元時に使用する）	*/
	t_offset = 301 ;
	offset = 1 ;
	blsadr->NtoS[0] = 0 ;	/* pgr0541 */
	blsadr->StoN[0] = 0 ;	/* pgr0541 */
/************************************************************************/
/* initialize the scramble table										*/
/************************************************************************/
	for (i=1; i<256; i++) {
		offset <<= 1 ;
		if (offset >= 256) {
			offset ^= t_offset ;
		}
		blsadr->NtoS[i] = (unsigned char)offset ;
		blsadr->StoN[offset] = (unsigned char)i ;
	}
}

/************************************************************************/
#define PS (*p_src++ != *(*pp_original)++)
#define PS15 PS || PS || PS || PS || PS || PS ||PS || PS || PS || PS || PS || PS ||PS || PS || PS

/************************************************************************/
/* 圧縮データ出力処理													*/
/************************************************************************/
/*__inline__*/					/* インライン展開可能な場合				*/
static void fldc4_encode_xx
(
 unsigned char 		*InBuf,		/* 被圧縮データ用バッファのポインタ 	*/
 unsigned char 		*OutBuf, 	/* 圧縮データ用バッファのポインタ 		*/
 unsigned short int inlen,	 	/* 被圧縮データのデータサイズ（バイト） */
 unsigned short int *outlen, 	/* 圧縮データのデータサイズ（バイト） 	*/
 FLDC4_ELEM 		*blsadr,	/* fldc4用圧縮辞書の構造体 				*/
 byte 				*pp_code	/* バッファポインタ						*/
)
{
 unsigned short int codesize;  	/* 圧縮後のファイルサイズ 				*/
 unsigned int i;

 /* 圧縮後のサイズ（バイト） */
	codesize = (unsigned short int)(pp_code - OutBuf -2); 	/* pgr0541 */

 /* check negative-data */
 /* 圧縮できていれば圧縮データを出力 */
	if (codesize < inlen) { /* 圧縮できている場合 */
   		OutBuf[0] = (unsigned char)((codesize>>8) & 0xFF);
   		OutBuf[1] = (unsigned char)(codesize & 0xFF);
   		*outlen = (unsigned short int)(codesize + 2);		/* pgr0541 */
 	}
 /* 圧縮できていなければ元のデータにスクランブルをかけたものを出力 */
 	else { 					/* 圧縮できていない場合 */
   		OutBuf[0] = (unsigned char)(FLDC4_COPY_MODE | ((inlen>>8) & 0xFF));
   		OutBuf[1] = (unsigned char)(inlen & 0xFF);
   /* scramble the raw data */
   		for(i=0; i<inlen; i++) {
			OutBuf[i+2] = blsadr->NtoS[InBuf[i]];			/* pgr0541 */
		}
   		*outlen = (unsigned short int)(inlen + 2);			/* pgr0541 */
 	}
}
/************************************************************************/
/* main routine of encode(圧縮)											*/
/*																		*/
/* 配列InBufに与えられたinlenバイトのデータを圧縮します。圧縮結果を 	*/
/* 配列OutBufへ、そのデータサイズ（バイト）をoutlenへ返します。 		*/
/*																		*/
/* 直前３バイトの過去の出現位置を保持していれば、一致長で符号化し、 	*/
/* 保持していなければ、固定１次スプレイで符号化する。					*/
/* ネガティブ圧縮の場合は元データにスクランブルをかけて、そのまま		*/
/* 出力する。															*/
/*																		*/
/* InBufサイズ（inlen）は、1～32,768バイトを指定できる。				*/
/* （大きい方が圧縮率はよい）											*/
/* OutBufはFLDC4_BUF_SIZE(32Kバイト）+2の領域を準備する必要がある。		*/
/* （ネガティブ圧縮の判断をFLDC4_BUF_SIZEを超えるか否かで行っている） 	*/
/* OutBufに返す圧縮結果には、２バイトのヘッダーを付加しており、 		*/
/* 最上位１ビットが復元情報modeを、残りの15ビットが圧縮後のサイズ		*/
/* を示す。 															*/
/*																		*/
/*	  《戻値》	NORMAL	   問題無し 									*/
/*				ERR_FATAL  致命的なエラー								*/
/************************************************************************/
int f_com_fldc4_encode(
 unsigned char *InBuf,		 	/* 被圧縮データ用バッファのポインタ 	*/
 unsigned char *OutBuf, 	 	/* 圧縮データ用バッファのポインタ 		*/
 unsigned short int inlen,	 	/* 被圧縮データのデータサイズ（バイト） */
 unsigned short int *outlen, 	/* 圧縮データのデータサイズ（バイト） 	*/
 FLDC4_ELEM 	*blsadr			/* fldc4用圧縮辞書の構造体 				*/
)
{
 int RCode; 		 			/* for fldc-sub return value 			*/
 byte **pp_original;			/* InBuf中の符号化済と符号化前の境		*/
 byte **pp_original_end;		/* InBuf の最後							*/
 byte **pp_code;				/* OutBuf中の符号格納済と未格納の境		*/
 byte **pp_code_end;			/* OutBufの最後							*/

 /* initialize */
	fldc4_InitializeArray(blsadr->Prev_point);		/* pgr0541 */
	fldc4_Initialize_Splay(blsadr->Up, blsadr->Left, blsadr->Right);
 	fldc4_InitializeLen_Splay(blsadr->UpL, blsadr->LeftL, blsadr->RightL);
 	blsadr->OutByte = FLAG_FULL;

 	blsadr->P_original = InBuf;
 	blsadr->P_original_zero = InBuf;
 	blsadr->P_original_end = InBuf+inlen;			/* pgr0541 */
 	blsadr->P_code = OutBuf +2;						/* pgr0541 */
	 /* 出力データの先頭２バイトは、圧縮後のサイズを書くために空けておく */
 	blsadr->P_code_end = OutBuf + FLDC4_BUF_SIZE +2;

   /* P_original が InBuf中の符号化済と符号化前の境を指す 					*/
   /* P_original_zero が InBuf の頭を、P_original_end が InBuf の最後を指す*/
   /* P_code は OutBuf中の符号格納済と未格納の境を指す 					*/
 	pp_original = &(blsadr->P_original);
 	pp_original_end = &(blsadr->P_original_end);
 	pp_code = &(blsadr->P_code);
 	pp_code_end = &(blsadr->P_code_end);

 /* 符号化処理スタート */
 	if (inlen <= 2) {
   		(*pp_code) += inlen;
 	}
 	else {
   /* 直前３バイトを一致長符号への切替えに使用するため、				*/
   /* 頭の３バイトは、一致長符号切替なしで、全て固定１次スプレイで圧縮 	*/
   /* 固定１次スプレイ符号化（頭１バイトは文脈を０とする） 				*/
   		if ((RCode = fldc4_Compress(0, **pp_original, blsadr)) != NORMAL) {
			if (RCode == ERR_FATAL) {
				return(RCode);
			}
			/* 圧縮データ出力	*/
			fldc4_encode_xx(InBuf,OutBuf,inlen,outlen,blsadr,*pp_code);
		 	return(NORMAL);
	 	}
   		(*pp_original) ++;
   /* 固定１次スプレイ符号化 */
   		if ((RCode = fldc4_Compress((byte)((*(*pp_original-1)) & 0x7f), **pp_original, blsadr)) != NORMAL) {
			if (RCode == ERR_FATAL) {
				return(RCode);
			}
			/* 圧縮データ出力	*/
			fldc4_encode_xx(InBuf,OutBuf,inlen,outlen,blsadr,*pp_code);
		 	return(NORMAL);
	 	}
   		(*pp_original) ++;	   /* ここまでで、頭２バイトを符号化済 */

   /* InBufの3バイト目～最後まで符号化処理を繰返す */
		while(*pp_original != *pp_original_end) {
	 /* 固定１次スプレイ符号化 */
			if ((RCode = fldc4_Compress((byte)((*(*pp_original-1)) & 0x7f),**pp_original, blsadr)) != NORMAL) {
				if (RCode == ERR_FATAL) {
					return(RCode);
				}
				/* 圧縮データ出力	*/
				fldc4_encode_xx(InBuf,OutBuf,inlen,outlen,blsadr,*pp_code);
			 	return(NORMAL);
			}
			(*pp_original) ++;
	 /* 一致長符号化 */
			if ((RCode = fldc4_matchlen_coding(blsadr)) != NORMAL) {
				if (RCode == ERR_FATAL) {
					return(RCode);
				}
				/* 圧縮データ出力	*/
				fldc4_encode_xx(InBuf,OutBuf,inlen,outlen,blsadr,*pp_code);
			 	return(NORMAL);
			}
   		}

   /* encode Exit code */
   /* 復元のときに、最後の数ビットが、符号なのか単に0ビットを詰めている */
   /* だけなのか分からないので、データの終了を示す符号を埋め込む 		*/
   /* 固定１次スプレイ（EofChar：ファイルの最後を示すキャラクタ） 		*/
		if ((RCode = fldc4_Compress((byte)((*(*pp_original-1)) & 0x7f), EofChar, blsadr)) != NORMAL) {
			if (RCode == ERR_FATAL) {
				return(RCode);
			}
			/* 圧縮データ出力	*/
			fldc4_encode_xx(InBuf,OutBuf,inlen,outlen,blsadr,*pp_code);
		 	return(NORMAL);
		}

   /* flush bits */
   /* １バイトに満たずにスタックされている符号を出力する */
		if ((blsadr->OutByte != FLAG_FULL) && (*pp_code < *pp_code_end)) {
			while(blsadr->OutByte>0) {
				(blsadr->OutByte) <<= 1;
			}
			**pp_code = (unsigned char)blsadr->OutByte;
			(*pp_code)++;
		}
	}

   /* 圧縮データ出力	*/
	fldc4_encode_xx(InBuf,OutBuf,inlen,outlen,blsadr,*pp_code);

 	return(NORMAL);
}

/************************************************************************/
/* main routine of decode（復元）										*/
/*																		*/
/* 配列InBufに与えられたinlenバイトのデータを復元します。復元結果を 	*/
/* 配列OutBufへ、そのデータサイズ（バイト）をoutlenへ返します。 		*/
/*																		*/
/* 直前３バイトの過去の出現位置を保持していれば、一致長で復元し、		*/
/* 保持していなければ、固定１次スプレイで復元する。 					*/
/* modeが1 の場合は、逆スクランブルをかけて、そのまま出力する。 		*/
/*																		*/
/*	  《戻値》	NORMAL	   問題無し 									*/
/*				BADCODE エラー。復元できないデータだった				*/
/************************************************************************/
int f_com_fldc4_decode(
 unsigned char *InBuf,		 	/* 圧縮データ用バッファのポインタ 		*/
 unsigned char *OutBuf, 	 	/* 復元データ用バッファのポインタ 		*/
 unsigned short int inlen,	 	/* 圧縮データのデータサイズ（バイト） 	*/
 unsigned short int *outlen, 	/* 復元データのデータサイズ（バイト） 	*/
 short int mode, /* InBuf内のデータは、 								*/
				 /* mode = 1 のとき、元データにスクランブルをかけたのみ */
				 /* mode = 0 のとき、圧縮データ 						*/
 FLDC4_ELEM *blsadr 			/* fldc4用圧縮辞書の構造体 				*/
)
{
 word DeByte;				/* 復元後の１コード 						*/
			  				/* 0～255または、データの最後を示すEofChar 	*/
 int RCode; 				/* for fldc-sub return value 				*/
 unsigned int i;
 byte **pp_original;		/* OutBuf中の復元データ格納と未格納の境		*/
 byte **pp_original_end;	/* OutBufの最後								*/
 byte **pp_code;			/* InBuf 中の復元済と復元前の境				*/
 byte **pp_code_end;		/* InBuf の最後								*/

 /* mode = 0 のとき、復元する */
	if (mode == 0) {
  /* initialize */
		fldc4_InitializeArray(blsadr->Prev_point);	/* pgr0541 */
		fldc4_Initialize_Splay(blsadr->Up, blsadr->Left, blsadr->Right);
		fldc4_InitializeLen_Splay(blsadr->UpL, blsadr->LeftL, blsadr->RightL);
		blsadr->BitPos = 0;
		blsadr->P_code = InBuf;
		blsadr->P_code_end = InBuf + inlen;			/* pgr0541 */
		blsadr->P_original = OutBuf;
		blsadr->P_original_zero = OutBuf;
		blsadr->P_original_end = OutBuf + FLDC4_BUF_SIZE;	/* pgr0541 */

   /* P_original が OutBuf 中の復元データ格納と未格納の境を指す */
   /* P_code が InBuf 中の復元済と復元前の境を指す */
   /* P_original_zero が OutBuf の頭を、P_code_end が InBuf の最後を指す */
   		pp_original = &(blsadr->P_original);
   		pp_original_end = &(blsadr->P_original_end);
		pp_code = &(blsadr->P_code);
   		pp_code_end = &(blsadr->P_code_end);

   /* 復元処理スタート */
   /* 圧縮処理同様、頭の３バイトは、一致長符号切替えなしで、*/
   /* 全て固定１次スプレイで復元する */
   /* 固定１次スプレイで復元 */
   		if ((RCode = fldc4_Expand(0, &DeByte, blsadr)) != NORMAL) {
			return(RCode);
		}
		**pp_original = (unsigned char)DeByte;
		(*pp_original) ++;
   /* 固定１次スプレイで復元 */
   		if ((RCode = fldc4_Expand((byte)((*(*pp_original-1)) & 0x7f), &DeByte, blsadr)) != NORMAL) {
			return(RCode);
		}
		**pp_original = (unsigned char)DeByte;
		(*pp_original) ++;	   /* ここまでで、頭２バイトを復元済 */

   /* データの最後を示すEofCharを復元するまで復元処理を繰返す */
		while(1) {
		 /* 固定１次スプレイで復元 */
			if ((RCode = fldc4_Expand((byte)((*(*pp_original-1)) & 0x7f), &DeByte, blsadr)) != NORMAL) {
				return(RCode);
			}
		 	if (DeByte == EofChar) { /* データ終了の符号を復元したら終了 */
		   		if (*pp_code != *pp_code_end) {
			 		return(BADCODE);	/* 復元不能なデータ	*/
			 	}
		   		break;
		 	}
			if (*pp_original >= *pp_original_end) {
				return(BADCODE);		/* 復元不能なデータ	*/
			}
			**pp_original = (unsigned char)DeByte;
			(*pp_original)++;
		 /* 一致長符号で復元 */
		 	if ((RCode = fldc4_matchlen_decoding(blsadr)) != NORMAL) {
				return(RCode);
			}
		}

		/* 復元後のサイズ（バイト） */
		*outlen = (unsigned short int)(*pp_original - OutBuf);	/* pgr0541 */
	}
 /* recover the scrambled data */
 /* mode = 1 のとき、逆スクランブルをかけて元のデータにする */
 	else {
		for (i=0; i<inlen; i++) {
		 	OutBuf[i] = blsadr->StoN[InBuf[i]];		/* pgr0541 */
		}
   		*outlen = inlen;							/* pgr0541 */
 	}

	return(NORMAL);
}

/************************************************************************/
/* initialize the code tree - as a balanced tree						*/
/*																		*/
/* 固定１次スプレイ符号木の初期状態をセット。							*/
/* 葉の数が256＋1の２分木を128個準備する。								*/
/************************************************************************/
static void fldc4_Initialize_Splay(UpIndex *up,DownIndex *left,DownIndex *right)
{
 unsigned int I, J;
 DownIndex K;
 word L;

	for(L = 0; L < MulTree; L++) {
		up++;	/* pgr0541 */
		for (I = 1; I <= TwiceMax; I++) {
			*(up++) = (UpIndex)((I - 1) >> 1);
		}
		for (J = 0; J <= PredMax; J++) {
			K = (DownIndex)((J + 1) << 1);
			*(left++) = (DownIndex)(K - 1);	/* pgr0541 */
			*(right++) = K;					/* pgr0541 */
   		}
 	}
}

/********************************************************************/
/* initialize the code tree for RL - as a balanced tree 			*/
/*																	*/
/* 一致長用のスプレイ符号木の初期状態をセット。 					*/
/* 葉の数が32の２分木を1個準備する。								*/
/********************************************************************/
static void fldc4_InitializeLen_Splay(UpIndex *upL,UpIndex *leftL,UpIndex *rightL)
{
 unsigned int I, J;
 UpIndex K;

	for (I = 1; I < (MaxLen*2); I++) {
		upL[I] = (UpIndex)((I - 1) >> 1);	/* pgr0541 */
	}
	for (J = 0; J < MaxLen; J++) {
   		K = (UpIndex)(((byte)J + 1) << 1);
   		leftL[J] = (UpIndex)(K - 1);		/* pgr0541 */
   		rightL[J] = K;						/* pgr0541 */
 	}
}

/********************************************************************/
/*																	*/
/* 過去の出現位置を保持する配列を初期化 							*/
/*																	*/
/********************************************************************/
static void fldc4_InitializeArray(word *prev)
{
 int i;

	for(i=0; i<8192; i++) {
		prev[i] = 0;	/* pgr0541 */
 	}
}

/********************************************************************/
/* match length coding												*/
/*																	*/
/* 一致長符号で圧縮。過去に出現した３バイトが再度出現した場合、 	*/
/* その３バイト以降の文字列を一致長で符号化する。					*/
/* 一致長は、過去の出現位置以降の文字列と入力文字列との一致する長さ */
/* とする。 														*/
/********************************************************************/
static int fldc4_matchlen_coding(
 FLDC4_ELEM *blsadr				/* fldc4用圧縮辞書の構造体 			*/
)
{
 byte *p_src, *p_start;
 short int match_len; 			/* 一致長 							*/
 word hash_code;	  			/* 検索用ハッシュ値 				*/
 int RCode; 		  			/* for fldc-dll return value 		*/
 word *prev_point;	  			/* 過去の出現位置 					*/
 byte **pp_original;			/* InBuf 中の符号化済と符号化前の境	*/
 byte **pp_original_zero;		/* InBuf 中の先頭					*/
 byte **pp_original_end;		/* InBuf 中の最後					*/
 unsigned int work __attribute__((unused));;

   /* *pp_original が InBuf 中の符号化済と符号化前の境を指す	*/
   /* p_src が過去の出現位置（ポインタ）を示す 					*/
   /* prev_point[hash_code]が過去の出現位置を示す				*/
	pp_original = &(blsadr->P_original);	/* pgr0541 */
	pp_original_zero = &(blsadr->P_original_zero);
	pp_original_end = &(blsadr->P_original_end);
	prev_point = blsadr->Prev_point;

 /* 3バイトを13ビットに押し込める（縮退する） */
 /* 特徴的なビットを活かすように重ねあわせる  */
	hash_code = (unsigned short)((*(*pp_original-3)<<5)
		  ^((*(*pp_original-2)&fldc4_Mask1)<<5)
			  ^((*(*pp_original-2)&fldc4_Mask2)<<3)^(*(*pp_original-1)));

 /* 直前３バイトの過去の出現位置を検索＆過去の出現位置を更新 */
	if (prev_point[hash_code] != 1) {
		/* prev_point[hash_code] == 1 の場合、直前の３バイトは、	  */
		/* その３バイトのみの繰返しで出現する可能性が高いことを示す。 */
		/*（一致長０で符号化する可能性が高い）						  */
		/* このため、一致長符号を付加するのを抑制する。 			  */

   		p_src = *pp_original_zero + prev_point[hash_code]; /* 過去の出現位置 */

   /* 3×8ビットを13ビットに押し込め（縮退し）ているので、 */
   /* 衝突（異なる３バイトの組が同じバケットを指定する）が */
   /* 起こる。このため、一致の確認を取る。				   */
   /* 厳密にいうと、縮退した３バイト分すべて確認しないといけないが、 */
   /* *(*pp_original-2)は、圧縮率に殆ど影響しないので、確認処理を省く  */
		if ((prev_point[hash_code] == 0) ||
		   (*(*pp_original-1) != *(p_src-1)) ||
		   (*(*pp_original-2) != *(p_src-2))) {
	 /* 過去の出現位置を保持していなかった場合		   */
	 /* 一致長符号は付加せず、出現位置の更新のみを行う */
			prev_point[hash_code] = (unsigned short)(*pp_original - *pp_original_zero);
		}

   /* 過去の出現位置を保持していた場合。一致長符号を付加する */
		else {
	/* 出現位置の更新 */
	/* 一致長が0の場合は、これ以降一致長符号をつけないようにするために */
	/* 一致長付加なしを示す「1」を入れ、それ以外は出現位置（3～32K-1） */
	/* を入れる。													   */
	/* ただし、一致長0でも、直前２文字が同じコードである場合は、特別に */
	/* それ以降でも一致長符号をつけられるように出現位置を入れる。	   */
			if ((*p_src != **pp_original) && (*(*pp_original-2) != *(*pp_original-1))) {
				prev_point[hash_code] = 1;
			}
			else {
				prev_point[hash_code] = (unsigned short)(*pp_original - *pp_original_zero);
			}

	/* 一致長を求め、符号化する。						*/
	/* 一致長0～MaxLen-1に対して符号を準備している。	*/
	/* 一致長がMaxLen以上の場合、MaxLen-1ずつに分割し、 */
	/* 数回に分けて符号化する。 						*/
	/* 復元時は、一致長MaxLen-1のとき、次の符号も一致長 */
	/* と判断する。 									*/
			while(1) {
				p_start = *pp_original;
				if ((*pp_original_end - *pp_original) < MaxLen_1) {
					while(*pp_original != *pp_original_end) {
						if (PS) {	/* #define PS *p_src++ != *(*pp_original)++ */
							(*pp_original) --;
							break;
			 			}
		   			}
					match_len = (short)(*pp_original - p_start);
					if ((RCode = fldc4_CompressL(match_len, blsadr)) != NORMAL) {
		 				return(RCode);
			 		}
#ifdef FLDC4_DEBUG
		   			fprintf(stdout," length %d\n", match_len);
#endif
		 		}
		 		else {
					work = PS15 || PS15 || PS || (*pp_original)++;	/* PS15は、PS || PS || ... */
	   				(*pp_original) --;
	   				match_len = (short)(*pp_original - p_start);
		   			if ((RCode = fldc4_CompressL(match_len, blsadr)) != NORMAL) {
						return(RCode);
					}
#ifdef FLDC4_DEBUG
					fprintf(stdout," length %d\n", match_len);
#endif
					if (match_len == MaxLen_1) {
						continue;
					}
	 			}
	 			break;
	 		}
   		}
	}

#ifdef FLDC4_DEBUG
	fprintf(stdout, "prev_point[%x] = %x\n",hash_code, prev_point[hash_code]);
#endif

	return(NORMAL);
}

/************************************************************************/
/* match length decoding												*/
/*																		*/
/* 一致長符号で復元。過去に出現した３バイトが再度出現した場合、 		*/
/* 次の符号を一致長として復元する。 									*/
/* 復元された一致長により、過去の出現位置以降の文字列をコピーして、 	*/
/* 復元文字列とする。													*/
/************************************************************************/
static int fldc4_matchlen_decoding(
 FLDC4_ELEM *blsadr 			/* fldc4用圧縮辞書の構造体 				*/
)
{
 unsigned char *p_src;
 short int match_len, s_match_len; 	/* 一致長 							*/
 word hash_code;				/* 検索用ハッシュ値 					*/
 int RCode; 					/* for fldc-dll return value 			*/
 word *prev_point;				/* 過去の出現位置 						*/
 byte **pp_original;			/* OutBuf中の復元データ格納と未格納の境	*/
 byte **pp_original_zero;		/* OutBuf中の先頭						*/
 byte **pp_original_end;		/* OutBuf中の最後						*/

 /* *pp_original が OutBuf 中の復元データ格納と未格納の境を指す */
 /* p_src が過去の出現位置（ポインタ）を示す 				 	*/
 /* prev_point[hash_code]が過去の出現位置を示す				 	*/
	pp_original = &(blsadr->P_original);	/* pgr0541 */
	pp_original_zero = &(blsadr->P_original_zero);
	pp_original_end = &(blsadr->P_original_end);
	prev_point = blsadr->Prev_point;

 /* 3バイトを13ビットに押し込める（圧縮時と同じ重ね方） */
	hash_code = (unsigned short)((*(*pp_original-3)<<5)
		  ^((*(*pp_original-2)&fldc4_Mask1)<<5)
			  ^((*(*pp_original-2)&fldc4_Mask2)<<3)^(*(*pp_original-1)));

 /* 直前３バイトの過去の出現位置を検索＆過去の出現位置を更新 */
	if (prev_point[hash_code] != 1) {
		/* prev_point[hash_code] == 1 の場合、圧縮側と同様に、 */
		/* 一致長符号付加がないと判断する。 				   */

		p_src = *pp_original_zero + prev_point[hash_code]; /* 過去の出現位置 */

   /* 圧縮側と同じ一致確認を取る。 */
		if ((prev_point[hash_code] == 0) ||
		   (*(*pp_original-1) != *(p_src-1)) ||
		   (*(*pp_original-2) != *(p_src-2))) {
	 /* 一致長符号付加なしの場合。圧縮側同様、更新のみを行う */
			prev_point[hash_code] = (unsigned short)(*pp_original - *pp_original_zero);
		}

   /* 一致長符号付加ありの場合 */
		else {
	/* 出現位置の更新。圧縮側と同様に更新する。 */
	/* 一致長を求める：fldc4_ExpandL				  */
			if ((RCode = fldc4_ExpandL(&match_len, blsadr)) != NORMAL) {
				return(RCode);
			}
			if ((match_len	== 0) && (*(*pp_original-2) != *(*pp_original-1))) {
	   			prev_point[hash_code] = 1;
	   		}
	 		else {
	   			prev_point[hash_code] = (unsigned short)(*pp_original - *pp_original_zero);
	   		}

#ifdef FLDC4_DEBUG
			fprintf(stdout,"   length %d\n", match_len);
#endif

	 /* 得られた一致長分、InBufからOutBufにコピーする	   */
	 /* 一致長がMaxLen-1の場合、次の符号も一致長と判断する */
			if ((*pp_original+match_len) > *pp_original_end) {
				return(BADCODE);
			}
			s_match_len = match_len;
	 		while(s_match_len--) {
	   			*(*pp_original)++ = *p_src++;
	   		}
	 		while(match_len == MaxLen_1) {
				/* 一致長復元 */
	   			if ((RCode = fldc4_ExpandL(&match_len, blsadr)) != NORMAL) {
		 			return(RCode);
		 		}
#ifdef FLDC4_DEBUG
	   			fprintf(stdout," length %d\n", match_len);
#endif
	   			s_match_len = match_len;
	   			if ((*pp_original+match_len) > *pp_original_end) {
		 			return(BADCODE);
		 		}
	   			while(s_match_len--) {
		 			*(*pp_original)++ = *p_src++;
		 		}
	 		}
   		}
	}
#ifdef FLDC4_DEBUG
	fprintf(stdout, "prev_point[%x] = %x\n",hash_code, prev_point[hash_code]);
#endif

	return(NORMAL);
}

/************************************************************************/
/* rearrange the code tree for each succeeding character				*/
/*																		*/
/* 固定１次スプレイ木を更新する 										*/
/* ２分木を折り曲げ、出現した(context, target)の符号長を半分にする		*/
/*																		*/
/* targetは、２分木の１つの葉に対応し、根から葉までの経路がtargetの 	*/
/* 符号に対応する。更新では、葉から根までの経路にある各節点を順番に 	*/
/* ２つづつ組にし、その組ごとに経路が半分になるように木を組み替える 	*/
/************************************************************************/
static void fldc4_Splay(
 byte context,				/* 符号化対象バイトの直前バイトの下位7ビット*/
 word target,				/* 符号化対象バイト（0～256） 				*/
 FLDC4_ELEM *blsadr 		/* fldc4用圧縮辞書の構造体 					*/
)
{
 DownIndex A, B;		/* スプレイ符号木ノード番号						*/
 UpIndex C, D;			/* スプレイ符号木ノード番号						*/
 UpIndex *up;			/* スプレイ符号木(親)							*/
 DownIndex *left;		/* スプレイ符号木(葉)							*/
 DownIndex *right;		/* スプレイ符号木(葉)							*/

#ifdef FLDC4_DEBUG
	fldc4_monitor("fldc4_Splay",context,target);
#endif

 	up = blsadr->Up + context*(TwiceMax+1);			/* pgr0244 *//* pgr0541 */
 	left = blsadr->Left + context*(PredMax+1);		/* pgr0244 */
 	right = blsadr->Right + context*(PredMax+1);	/* pgr0244 */
 	A = (word)(target + MaxChar); /* 符号化対象バイトtargetに対応する節点（葉） */

	do {
  /* targetの葉から根(Root)まで２分木を辿りながら、以下の処理を繰返す */
   		C = up[A];
   		if (C != Root) {
	 /* a pair remains */
	 		D = up[C]; 	/* C,Dに、targetの葉から根までの経路に */
						/* 順に並んでいる２節点を代入		   */

	 /* Dの左右の節点のうち、targetの葉へ継がらない節点と、   */
	 /* Cの左右の節点のうち、targetの葉へ継がる節点と交換する */
	 /* このことによって根からtargetまでの経路からCが除外され */
	 /* Dが残される。（２節点が１節点へと半分になる）		  */
	 		B = left[D];
	 		if (C == B) {
	   			B = right[D];
	   			right[D] = A;
	 		}
	 		else {
	   			left[D] = A;
	   		}
	 		if (A == left[C]) {
	   			left[C] = B;
	   		}
	 		else {
	   			right[C] = B;
	   		}
	 		up[A] = D;
	 		up[B] = C;
	 		A = D;
   		}
   		else {
	 		A = C;
	 	}
	} while (A != Root);
}

/************************************************************************/
/* Compress a single char with Splay code followed the context			*/
/*																		*/
/* 固定１次スプレイ符号化												*/
/*	 直前の１バイトによって、符号化に使用するスプレイ符号木を			*/
/* 切り替える。直前１バイトは下位７ビット、128通りに縮退する。			*/
/*	 128通り各々に対し、葉の数が256+1（キャラクタ数：８ビットのバリ 	*/
/* エーション＋END文字）のスプレイ符号木を準備。						*/
/*	 スプレイ符号木は２分木の形状をしており、根から各葉までの経路		*/
/* が各葉の符号に相当する。２分木の符号は各種ある（Huffman等）が、		*/
/* 方式の違いは、符号木の更新の仕方（作成の仕方）だけである。			*/
/* このため、多少２分木の表現の仕方は異なるが、２分木を辿りながら		*/
/* 符号を生成するところは基本的に同じである。							*/
/************************************************************************/
static int fldc4_Compress(
 byte context,			/* 符号化対象バイトの直前バイトの下位7ビット	*/
 word target,			/* 符号化対象バイト（0～256） 					*/
 FLDC4_ELEM *blsadr 	/* fldc4用圧縮辞書の構造体 						*/
)
{
 word U, A;				/* 符号化対象バイトtargetに対応する節点(葉)		*/
 bool Stack[MaxChar+1]; /* 葉から根へと辿ったときの符号 				*/
						/* （本来の符号と逆向き）		 				*/
 bool *St, *St_e;		/* 符号へのポインタ								*/
 UpIndex *up;			/* スプレイ符号木(親)							*/
 DownIndex *right;		/* スプレイ符号木(葉)							*/
 short int *outbyte;	/* Bit出力エリア								*/
 byte **pp_code;		/* OutBuf 中の圧縮済と圧縮前の境				*/
 byte **pp_code_end;	/* OutBuf の最後								*/

#ifdef FLDC4_DEBUG
	fldc4_monitor("fldc4_Compress",context,target);
#endif

	up = blsadr->Up + context*(TwiceMax+1);			/* pgr0244 *//* pgr0541 */
 	right = blsadr->Right + context*(PredMax+1);	/* pgr0244 */
 	outbyte = &(blsadr->OutByte);
 	pp_code = &(blsadr->P_code);
 	pp_code_end = &(blsadr->P_code_end);

	A = (word)(target + MaxChar); /* 符号化対象バイトtargetに対応する節点（葉） */
 	St = Stack;
 	St_e = St + (MaxChar+1);

 /* walk up the tree pushing bits onto stack		 */
 /* 葉から根へ向かって辿りながら、符号をスタックする */
	do {
   		U = up[A];
   		*(St++) = (bool)(right[U] == A); /* ２分木の右を1、左を0とする */
   		A = U;
   		if (St == St_e) {	/* バグ用トリガ。無限ループに落ちないように準備 */
#ifdef FLDC4_DEBUG
	 		fprintf(stderr,"3: one code is too long!!\n");
#endif
	 		return(ERR_FATAL);
   		}
 	} while (A != Root);

 /* unstack to transmit bits in correct order */
 /* スタックに溜めた符号を逆向きに並び変えて、OutBufに格納		 */
 /* 一旦OutByteに出力して、１バイト分溜まってから、OutBufに移す  */
	do {
   		St--;
   		(*outbyte) |= (*St);
   		if (*outbyte < 0) { /* byte filled with bits, write it out */
	 		if (*pp_code >= *pp_code_end) {
	   			return(BUFFERFULL);
	   		}
	 		**pp_code = (unsigned char)*outbyte; (*pp_code)++;
	 		*outbyte = FLAG_FULL;
   		}
   		else {
			*outbyte <<= 1;
		}
 	} while (St != Stack);

 /* 符号の更新 */
 	fldc4_Splay(context, target, blsadr);

 	return(NORMAL);
}

/************************************************************************/
/* return the decoded word												*/
/*																		*/
/* 固定１次スプレイ符号の復元											*/
/*	 圧縮側同様、直前の１バイトによって、符号化に使用するスプレイ		*/
/* 符号木を切り替える。 												*/
/*	 スプレイ符号木を符号に従って、根から順に節点を辿り、辿り着いた 	*/
/* 葉に対応するバイトを復元バイトとする。								*/
/************************************************************************/
static int fldc4_Expand(
 byte context, 			/* 符号化対象バイトの直前バイトの下位7ビット	*/
 word *target,			/* 符号化対象バイト（0～256） 					*/
 FLDC4_ELEM *blsadr 	/* fldc4用圧縮辞書の構造体 						*/
)
{
 DownIndex A;			/* スプレイ符号木(rootのノード)					*/
 DownIndex *left;		/* スプレイ符号木(葉)							*/
 DownIndex *right;		/* スプレイ符号木(葉)							*/
 word *bitpos;			/* bit位置										*/
 byte *input_code;		/* 圧縮データ									*/
 byte **pp_code;		/* InBuf 中の復元済と復元前の境					*/
 byte **pp_code_end;	/* InBuf の最後									*/

	left = blsadr->Left + context*(PredMax+1);		/* pgr0244 *//* pgr0541 */
	right = blsadr->Right + context*(PredMax+1);	/* pgr0244 */
	bitpos = &(blsadr->BitPos);
	input_code = &(blsadr->Input_code);
	pp_code = &(blsadr->P_code);
	pp_code_end = &(blsadr->P_code_end);

	A = Root;
 /* scan the tree to a leaf, which determines the character */
	do {
		if (*bitpos == 0) { /* used up bits in current byte, get another */
	 		if (*pp_code >= *pp_code_end) {
								/* 圧縮データでないものを入力データとして*/
							   	/* 渡された場合のエラーチェック			 */
#ifdef FLDC4_DEBUG
	   			fprintf(stderr,"4: Can't decode the EOF character\n");
#endif
	   			return(BADCODE);
	 		}
			*input_code = **pp_code;
	 		(*pp_code)++;
	 		*bitpos = 7;
   		}
   		else {
			(*bitpos)--;
		}

		if ((*input_code & fldc4_BitMask[*bitpos]) == 0) {
			A = left[A];
		}
   		else {
	 		A = right[A];
	 	}

 	} while (A <= PredMax); /* MaxChar(PredMax+1)～MaxChar×2-1 が葉に相当 */

 	*target = (DownIndex)(A-MaxChar);	/* pgr0541 */

#ifdef FLDC4_DEBUG
 	fldc4_monitor("fldc4_Compress",context,*target);
#endif

 /* 符号の更新 */
 	fldc4_Splay(context, *target, blsadr);
 	return(NORMAL);
}

/************************************************************************/
/*																		*/
/* 一致長符号用のスプレイ木を更新する									*/
/*																		*/
/************************************************************************/
static void fldc4_SplayL(
 short int len,   			/* 一致長（0～31） 							*/
 FLDC4_ELEM *blsadr			/* fldc4用圧縮辞書の構造体 					*/
)
{
 UpIndex A, B, C, D;	/* スプレイ符号木ノード番号						*/
 UpIndex *upL;			/* スプレイ符号木(親)							*/
 UpIndex *leftL;		/* スプレイ符号木(葉)							*/
 UpIndex  *rightL;		/* スプレイ符号木(葉)							*/

#ifdef FLDC4_DEBUG
	fldc4_monitor("fldc4_SplayL",0,len);
#endif

	upL = blsadr->UpL;	/* pgr0541 */
	leftL = blsadr->LeftL;
	rightL = blsadr->RightL;

	A = (byte)(len + MaxLen_1);
	do {
  /* targetの葉から根(Root)まで２分木を辿りながら、以下の処理を繰返す */
		C = upL[A];
   		if (C != Root) {
	 /* a pair remains */
	 		D = upL[C];	/* C,Dに、targetの葉から根までの経路に */
						/* 順に並んでいる２節点を代入		   */

	 /* Dの左右の節点のうち、targetの葉へ継がらない節点と、   */
	 /* Cの左右の節点のうち、targetの葉へ継がる節点と交換する */
	 /* このことによって根からtargetまでの経路からCが除外され */
	 /* Dが残される。（２節点が１節点へと半分になる）		  */
	 		B = leftL[D];
	 		if (C == B) {
	   			B = rightL[D];
	   			rightL[D] = A;
	 		}
	 		else {
	   			leftL[D] = A;
	   		}
	 		if (A == leftL[C]) {
	   			leftL[C] = B;
	   		}
	 		else {
	   			rightL[C] = B;
	   		}
	 		upL[A] = D;
	 		upL[B] = C;
	 		A = D;
   		}
   		else {
			A = C;
		}
 	} while (A != Root);
}

/************************************************************************/
/*																		*/
/* 一致長を符号化する									  				*/
/*																		*/
/************************************************************************/
static int fldc4_CompressL(
 short int len,   			/* 一致長（0～31） 							*/
 FLDC4_ELEM *blsadr			/* fldc4用圧縮辞書の構造体 					*/
)
{
 bool Stack[MaxLen+1];	/* 葉から根へと辿ったときの符号 				*/
 bool *St, *St_e;		/* 符号へのポインタ								*/
 UpIndex U, A;			/* スプレイ符号木ノード番号						*/
 UpIndex *upL;			/* スプレイ符号木(親)							*/
 UpIndex *rightL;		/* スプレイ符号木(葉)							*/
 short int *outbyte;	/* Bit出力エリア								*/
 byte **pp_code;		/* OutBuf 中の圧縮済と圧縮前の境				*/
 byte **pp_code_end;	/* OutBuf の最後								*/

#ifdef FLDC4_DEBUG
	fldc4_monitor("fldc4_CompressL",0, len);
#endif

	upL = blsadr->UpL;		/* pgr0541 */
	rightL = blsadr->RightL;
	outbyte = &(blsadr->OutByte);
	pp_code = &(blsadr->P_code);
	pp_code_end = &(blsadr->P_code_end);

	A = (byte)(len + MaxLen_1);
	St = Stack;
	St_e = St + (MaxLen+1);
	do { /* walk up the tree pushing bits onto stack */
		U = upL[A];
   		*(St++) = (bool)(rightL[U] == A);
   		A = U;
   		if (St == St_e) { /* バグ用トリガ。無限ループに落ちないように準備 */
#ifdef FLDC4_DEBUG
	 		fprintf(stderr,"3: one code is too long!!\n");
#endif
	 		return(ERR_FATAL);
   		}
 	} while (A != Root);

	do { /* unstack to transmit bits in correct order */
		St--;
   		(*outbyte) |= (*St);
   		if (*outbyte < 0) { /* byte filled with bits, write it out */
	 		if (*pp_code >= *pp_code_end) {
	   			return(BUFFERFULL);
	   		}
	 		**pp_code = (unsigned char)*outbyte; (*pp_code)++;
	 		*outbyte = FLAG_FULL;
   		}
   		else {
			*outbyte <<= 1;
		}
	} while (St != Stack);

	fldc4_SplayL(len, blsadr);
	return(NORMAL);
}

/************************************************************************/
/*																		*/
/* 一致長を復元する													   	*/
/*																		*/
/************************************************************************/
static int fldc4_ExpandL(
 short int *len,   			/* 一致長（0～31） 							*/
 FLDC4_ELEM *blsadr			/* fldc4用圧縮辞書の構造体 					*/
)
{
 UpIndex A;				/* スプレイ符号木ノード番号						*/
 UpIndex *leftL;		/* スプレイ符号木(葉)							*/
 UpIndex *rightL;		/* スプレイ符号木(葉)							*/
 word *bitpos;			/* bit位置										*/
 byte *input_code;		/* 圧縮データ									*/
 byte **pp_code;		/* InBuf 中の復元済と復元前の境					*/
 byte **pp_code_end;	/* InBuf の最後									*/

	leftL = blsadr->LeftL;		/* pgr0541 */
	rightL = blsadr->RightL;
	bitpos = &(blsadr->BitPos);
	input_code = &(blsadr->Input_code);
	pp_code = &(blsadr->P_code);
	pp_code_end = &(blsadr->P_code_end);

	A = Root;
 /* scan the tree to a leaf, which determines the character */
	do {
   		if (*bitpos == 0) { /* used up bits in current byte, get another */
	 		if (*pp_code >= *pp_code_end) {
						/* 圧縮データでないものを入力データとして 		*/
						/* 渡された場合のエラーチェック					*/
#ifdef FLDC4_DEBUG
	   			fprintf(stderr,"4: Can't decode the EOF character\n");
#endif
	  			return(BADCODE);
	 		}
	 		*input_code = **pp_code;
	 		(*pp_code)++;
	 		*bitpos = 7;
   		}
   		else {
			(*bitpos)--;
		}

   		if ((*input_code & fldc4_BitMask[*bitpos]) == 0) {
	 		A = leftL[A];
   		}
   		else {
			A = rightL[A];
		}

	} while (A < MaxLen_1);

#ifdef FLDC4_DEBUG
	fldc4_monitor("fldc4_CompressL",0, A-MaxLen_1);
#endif

	*len = (short int)(A-MaxLen_1);		/* pgr0541 */
	fldc4_SplayL(*len, blsadr);

	return(NORMAL);
}

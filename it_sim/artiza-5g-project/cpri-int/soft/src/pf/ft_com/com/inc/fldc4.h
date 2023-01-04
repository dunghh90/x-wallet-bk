#ifndef __INCL_FLDC4_H
#define	__INCL_FLDC4_H
/************************************************************************/
/*																		*/
/*	FILE-NAME : fldc4.h 									  			*/
/*	FUNCTION  : universal data compression								*/
/*	AUTHOR	  : Noriko Satoh											*/
/*	DATE	  : 99/02/01												*/
/*	VERSION   : 1.9 											 		*/
/*																		*/
/*	  All Rights Reserved, Copyright(C) 								*/
/*	  FUJITSU LABORATORIES LTD. 1998									*/
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
/*	 header file for fldc4												*/
/*																		*/
/*		   圧縮辞書の構造体（FLDC4_ELEM）								*/
/*																		*/
/*			 固定１次スプレイ符号  192K 								*/
/*			 一致長符号　　　　　　 16K+128 							*/
/*			 スクランブルテーブル　　　 512 							*/
/*			 ------------------------------ 							*/
/*							   計　209Kバイト							*/
/*																		*/
/************************************************************************/

#define FLDC4_BUF_SIZE	32768	/* input buffer size (8192*4)			*/
#define FLDC4_COPY_MODE	0x80	/* flag that show next block is copy 	*/

#define MaxChar 	256 		/* キャラクタ数 0～255の256個  			*/
#define PredMax 	255 		/* MaxChar - 1 							*/
#define TwiceMax	512 		/* MaxChar×2 							*/
#define MulTree 	128 		/* 文脈の数 ７ビット、128通り 			*/
#define MaxLen		32			/* 一致長の最長一致長 					*/

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned char bool;
typedef byte UpIndex;			/* 0..PredMax 	*/
typedef word DownIndex; 		/* 0..TwiceMax 	*/

typedef struct{
/* 固定１次スプレイ符号 */
  UpIndex Up[MulTree*(TwiceMax+1)]; 	/* parent branches of code tree	  	*/
  DownIndex Left[MulTree*(PredMax+1)];	/* children branches of code tree 	*/
  DownIndex Right[MulTree*(PredMax+1)]; /* children branches of code tree 	*/

/* 一致長符号 */
  UpIndex UpL[MaxLen*2];				/* 符号木 							*/
  UpIndex LeftL[MaxLen];				/* 符号木 							*/
  UpIndex RightL[MaxLen];				/* 符号木 							*/
  word Prev_point[8192];				/* 過去の出現位置を保持するテーブル */

/* scramble table for uncompressed block */
  unsigned char NtoS[256]; 				/* normal to scramble 				*/
  unsigned char StoN[256]; 				/* scramble to normal 				*/

/* stack for input or output 1byte data */
  word BitPos;					   		/* current bit in byte				*/
  short int OutByte;			   		/* for encode variable output byte 	*/
  byte Input_code;			   			/* for decode variable input byte	*/

/* pointer to input/output array */
  byte *P_original;
  byte *P_original_zero;
  byte *P_original_end;
  byte *P_code;
  byte *P_code_end;
} FLDC4_ELEM;

/************************************************************************/
/*	fldc4関数プロトタイプ宣言											*/
/************************************************************************/
void f_com_fldc4_init(			/* fldc4用の圧縮辞書初期化ルーチン		*/
 FLDC4_ELEM *blsadr				/* fldc4用圧縮辞書の構造体 				*/
);

int f_com_fldc4_encode(			/* fldc4圧縮メインルーチン				*/
 unsigned char *InBuf,		 	/* 被圧縮データ用バッファのポインタ 	*/
 unsigned char *OutBuf, 	 	/* 圧縮データ用バッファのポインタ 		*/
 unsigned short int inlen,	 	/* 被圧縮データのデータサイズ（バイト） */
 unsigned short int *outlen, 	/* 圧縮データのデータサイズ（バイト） 	*/
 FLDC4_ELEM 	*blsadr			/* fldc4用圧縮辞書の構造体 				*/
);

int f_com_fldc4_decode(			/* fldc4復元メインルーチン				*/
 unsigned char *InBuf,		 	/* 圧縮データ用バッファのポインタ 		*/
 unsigned char *OutBuf, 	 	/* 復元データ用バッファのポインタ 		*/
 unsigned short int inlen,	 	/* 圧縮データのデータサイズ（バイト） 	*/
 unsigned short int *outlen, 	/* 復元データのデータサイズ（バイト） 	*/
 short int mode, /* InBuf内のデータは、 								*/
				 /* mode = 1 のとき、元データにスクランブルをかけたのみ */
				 /* mode = 0 のとき、圧縮データ 						*/
 FLDC4_ELEM *blsadr 			/* fldc4用圧縮辞書の構造体 				*/
);
#endif

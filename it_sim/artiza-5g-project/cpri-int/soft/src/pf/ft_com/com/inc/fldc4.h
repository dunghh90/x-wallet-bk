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
/*							MBBTS 08A(2008/10/02)�ł�����o��			*/
/*																		*/
/*	UPDATE	: 															*/
/*																		*/
/*	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008				*/
/************************************************************************/

/************************************************************************/
/*	 header file for fldc4												*/
/*																		*/
/*		   ���k�����̍\���́iFLDC4_ELEM�j								*/
/*																		*/
/*			 �Œ�P���X�v���C����  192K 								*/
/*			 ��v�������@�@�@�@�@�@ 16K+128 							*/
/*			 �X�N�����u���e�[�u���@�@�@ 512 							*/
/*			 ------------------------------ 							*/
/*							   �v�@209K�o�C�g							*/
/*																		*/
/************************************************************************/

#define FLDC4_BUF_SIZE	32768	/* input buffer size (8192*4)			*/
#define FLDC4_COPY_MODE	0x80	/* flag that show next block is copy 	*/

#define MaxChar 	256 		/* �L�����N�^�� 0�`255��256��  			*/
#define PredMax 	255 		/* MaxChar - 1 							*/
#define TwiceMax	512 		/* MaxChar�~2 							*/
#define MulTree 	128 		/* �����̐� �V�r�b�g�A128�ʂ� 			*/
#define MaxLen		32			/* ��v���̍Œ���v�� 					*/

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned char bool;
typedef byte UpIndex;			/* 0..PredMax 	*/
typedef word DownIndex; 		/* 0..TwiceMax 	*/

typedef struct{
/* �Œ�P���X�v���C���� */
  UpIndex Up[MulTree*(TwiceMax+1)]; 	/* parent branches of code tree	  	*/
  DownIndex Left[MulTree*(PredMax+1)];	/* children branches of code tree 	*/
  DownIndex Right[MulTree*(PredMax+1)]; /* children branches of code tree 	*/

/* ��v������ */
  UpIndex UpL[MaxLen*2];				/* ������ 							*/
  UpIndex LeftL[MaxLen];				/* ������ 							*/
  UpIndex RightL[MaxLen];				/* ������ 							*/
  word Prev_point[8192];				/* �ߋ��̏o���ʒu��ێ�����e�[�u�� */

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
/*	fldc4�֐��v���g�^�C�v�錾											*/
/************************************************************************/
void f_com_fldc4_init(			/* fldc4�p�̈��k�������������[�`��		*/
 FLDC4_ELEM *blsadr				/* fldc4�p���k�����̍\���� 				*/
);

int f_com_fldc4_encode(			/* fldc4���k���C�����[�`��				*/
 unsigned char *InBuf,		 	/* �툳�k�f�[�^�p�o�b�t�@�̃|�C���^ 	*/
 unsigned char *OutBuf, 	 	/* ���k�f�[�^�p�o�b�t�@�̃|�C���^ 		*/
 unsigned short int inlen,	 	/* �툳�k�f�[�^�̃f�[�^�T�C�Y�i�o�C�g�j */
 unsigned short int *outlen, 	/* ���k�f�[�^�̃f�[�^�T�C�Y�i�o�C�g�j 	*/
 FLDC4_ELEM 	*blsadr			/* fldc4�p���k�����̍\���� 				*/
);

int f_com_fldc4_decode(			/* fldc4�������C�����[�`��				*/
 unsigned char *InBuf,		 	/* ���k�f�[�^�p�o�b�t�@�̃|�C���^ 		*/
 unsigned char *OutBuf, 	 	/* �����f�[�^�p�o�b�t�@�̃|�C���^ 		*/
 unsigned short int inlen,	 	/* ���k�f�[�^�̃f�[�^�T�C�Y�i�o�C�g�j 	*/
 unsigned short int *outlen, 	/* �����f�[�^�̃f�[�^�T�C�Y�i�o�C�g�j 	*/
 short int mode, /* InBuf���̃f�[�^�́A 								*/
				 /* mode = 1 �̂Ƃ��A���f�[�^�ɃX�N�����u�����������̂� */
				 /* mode = 0 �̂Ƃ��A���k�f�[�^ 						*/
 FLDC4_ELEM *blsadr 			/* fldc4�p���k�����̍\���� 				*/
);
#endif

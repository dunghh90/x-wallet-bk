/********************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		m_cm_ReCardStaTbl.h
 *  @brief	RE card status report type def
 *  @date     2015/04/22 FFCS)Zhangxq create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
  */
/********************************************************************************/

#ifndef M_CM_RECARDSTATBL_H
#define M_CM_RECARDSTATBL_H

#define D_RCT_RECARDSTS_FB_NUM	8	/* For TDD SRE is 7, other reserve */ 
#define D_RCT_RECARDSTS_ALM_NUM 48	/* For TDD SRE is 38, m_rc_cm_AlmCodeCnv, other reserve */
#define D_RCT_RECARDSTS_ERR_NUM 8	/* For TDD SRE is 7, other reserve */ 

typedef struct
{
	USHORT					slot_no;
	UCHAR					card_name[CMD_NUM8];
	USHORT					year;
	USHORT					month_day;
	USHORT					hour;
	USHORT					minsec;
	USHORT					card_svitem;
	USHORT					card_extitem;
	USHORT					card_cntitem;
	USHORT					card_extcntitem;
	USHORT					card_fbnum;
}__attribute__ ((packed)) RCT_CARDSTS_INF_T;


typedef struct
{
	USHORT					signal_kind;
	USHORT					result;
	RCT_CARDSTS_INF_T		cardsts_inf;
}__attribute__ ((packed)) RCT_CPRIF_CARDSTSRES_T;


typedef struct
{
	USHORT					fbId;
	USHORT					fbAlmSta;
	USHORT					fbBaseSVItem;
	USHORT					fbExtSVItem;
	USHORT					fbCarNum;
}__attribute__ ((packed)) RCT_CARDSTS_FB_EVERY_COM_T;

typedef struct
{
	USHORT					carNo;
	USHORT					carBandwidth;
	USHORT					carBaseSVItem;
	USHORT					carExtSVItem;
	USHORT					carBrNum;
}__attribute__ ((packed)) RCT_CARDSTS_FB_CAR_EVERY_COM_T;

typedef struct
{
	USHORT					brNo;
	USHORT					brBaseSVItem;
	USHORT					brExtSVItem;
}__attribute__ ((packed)) RCT_CARDSTS_FB_CAR_BR_EVERY_T;

typedef struct
{
	RCT_CARDSTS_FB_CAR_EVERY_COM_T				com;
	RCT_CARDSTS_FB_CAR_BR_EVERY_T				br_every[CMD_MAX_BRNUM];
}__attribute__ ((packed)) RCT_CARDSTS_FB_CAR_EVERY_T;

typedef struct
{
	RCT_CARDSTS_FB_EVERY_COM_T					com;
	RCT_CARDSTS_FB_CAR_EVERY_T					car_every[CMD_MAX_CARNUM];
}__attribute__ ((packed)) RCT_CARDSTS_FB_EVERY_T;

typedef struct
{
	USHORT					almNum;
}__attribute__ ((packed)) RCT_CARDSTS_ALM_COM_T;

typedef struct
{
	USHORT					almcd;
}__attribute__ ((packed)) RCT_CARDSTS_ALM_EVERY_T;

typedef struct
{
	USHORT					errNum;
}__attribute__ ((packed)) RCT_CARDSTS_ERR_COM_T;

typedef struct
{
	USHORT					errcd;
}__attribute__ ((packed)) RCT_CARDSTS_ERR_EVERY_T;


typedef struct
{
	RCT_CPRIF_CARDSTSRES_T	res;
	RCT_CARDSTS_FB_EVERY_T	fb_every[D_RCT_RECARDSTS_FB_NUM];
	RCT_CARDSTS_ALM_COM_T	alm;
	RCT_CARDSTS_ALM_EVERY_T	alm_every[D_RCT_RECARDSTS_ALM_NUM];
	RCT_CARDSTS_ERR_COM_T	err;
	RCT_CARDSTS_ERR_EVERY_T	err_every[D_RCT_RECARDSTS_ERR_NUM];
}__attribute__ ((packed)) RCT_CARDSTS_MNG_T;

#endif

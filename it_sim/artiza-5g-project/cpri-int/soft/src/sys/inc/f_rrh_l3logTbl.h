/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_rrh_l3logTbl.h
 *  @brief	RE L3 log Table
 *  @date   2015/05/15 FFCS)wuqy create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/******************************************************************************************************************************/

#ifndef F_RRH_L3LOGTBL_HEAD
#define F_RRH_L3LOGTBL_HEAD


/* It is danger to change this strucure, except for the first edition of this re */
/* because data may succeed fail */

#define D_RRH_L3LOG_MNGIDX_CARDSTS	16
#define D_RRH_L3LOG_MNGIDX_CARSTS	17
#define D_RRH_L3LOG_MNGIDX_NUM		32
#define D_RRH_L3LOG_NUM				64
#define D_RRH_L3LOG_DIR_RCV			0x52
#define D_RRH_L3LOG_DIR_SND			0x53
#define D_RRH_L3LOG_CARDSTS_LEN		0x304
#define D_RRH_L3LOG_CARLSTS_LEN		0xF2

typedef struct
{
	UINT					start_offset;
	USHORT					next_block_no;
	USHORT					block_num;
	USHORT					block_size;
	USHORT					max_log_len;
}T_RRH_L3LOG_MNGITEM;

typedef struct
{
	T_RRH_L3LOG_MNGITEM		item[D_RRH_L3LOG_MNGIDX_NUM];
}T_RRH_L3LOG_MNG;

typedef struct
{
	T_RRH_SYSTEM_TIME		log_time;
	USHORT					log_len;
	USHORT					log_dir;
	UCHAR					log_data[D_RRH_L3LOG_CARLSTS_LEN];
}T_RRH_L3LOG_CARSTS;

typedef struct
{
	T_RRH_SYSTEM_TIME		log_time;
	USHORT					log_len;
	USHORT					log_dir;
	UCHAR					log_data[D_RRH_L3LOG_CARDSTS_LEN];
}T_RRH_L3LOG_CARDSTS;

typedef struct
{
	T_RRH_SYSTEM_TIME		log_time;
	USHORT					log_len;
	USHORT					log_dir;
	UCHAR					log_data;
}T_RRH_L3LOG_BLOCKITEM;

typedef struct
{
	T_RRH_L3LOG_MNG    		mng;
	T_RRH_L3LOG_CARDSTS  	card_sts[D_RRH_L3LOG_NUM];
	T_RRH_L3LOG_CARSTS  	car_sts[D_RRH_L3LOG_NUM];
}T_RRH_L3_SR_LOG;

#endif

/* RRH-010 OSE-SHELLD alpha add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   f_cmd_sfp_info.c
 *  @brief
 *  @date   $Date:$ $Author:$
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */
/********************************************************************************************************************/


/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_dbg_cmd_inc.h"
#include "BPF_COM_LOG.h"
#include "BPF_HM_DEVC.h"
#include "BPF_RM_SVRM.h"

/** @addtogroup SHELLD_CMD_RICR
 * @{ */

typedef struct {
	int		addr;
	int		size;
	int		disp_type;
	char*	title;
	char*	unit_str;
} T_CMD_SFP_DATA;



enum {
	D_CMD_SFP_DISP_TIT1,		/* */
	D_CMD_SFP_DISP_TIT2,		/* */
	D_CMD_SFP_DISP_NAME,	/* 表示用コメント	*/
	D_CMD_SFP_DISP_HEX,		/* 16進数表示	*/
	D_CMD_SFP_DISP_TEXT,	/* 文字列表示	*/
	D_CMD_SFP_DISP_DEC,		/* 10進数表示	*/
	D_CMD_SFP_DISP_TEMP,		/* 温度変換表示	*/
	D_CMD_SFP_DISP_VOL,		/* 電圧変換表示	*/
	D_CMD_SFP_DISP_BIAS,	/* バイアス変換表示	*/
	D_CMD_SFP_DISP_POW,		/* 出力変換表示		*/
	D_CMD_SFP_DISP_RX_PWR,	/* RX_PWR			*/
	D_CMD_SFP_DISP_SLOPE,	/* Slope			*/
	D_CMD_SFP_DISP_OFFSET	/* Offset			*/
};


/* SFP Address(A0) */
const T_CMD_SFP_DATA sfp_data_A0[] =
{
	{	0,	0, 	D_CMD_SFP_DISP_TIT1,	"BASE ID FIELDS",				""			},	/* */
	{	0,	1,	D_CMD_SFP_DISP_HEX,		"Identifier",					""			},	/* Identifier						*/
	{	1,	1,	D_CMD_SFP_DISP_HEX,		"Ext. Identifier",				""			},	/* Ext. Identifier					*/
	{	2,	1,	D_CMD_SFP_DISP_HEX,		"Connector",					""			},	/* Connector						*/
	{	3,	8,	D_CMD_SFP_DISP_HEX,		"Transceiver",					""			},	/* Transceiver						*/
	{	11, 1,	D_CMD_SFP_DISP_HEX,		"Encoding",						""			},	/* Encoding							*/
	{	12,	1,	D_CMD_SFP_DISP_DEC,		"BR. Normal",					"x100MBd"	},	/* BR. Normal						*/
	{	13,	1,	D_CMD_SFP_DISP_HEX,		"Rate Identifier",				""			},	/* Rate Identifier					*/
	{	14,	1,	D_CMD_SFP_DISP_DEC,		"Length(SMF,km)",				"km"		},	/* Length {9um} -km					*/
	{	15,	1,	D_CMD_SFP_DISP_DEC,		"Length(SMF)",					"x100m"		},	/* Length {9um}						*/
	{	16,	1,	D_CMD_SFP_DISP_DEC,		"Length(50um)",					"x10m"		},	/* Length {50um} 					*/
	{	17,	1,	D_CMD_SFP_DISP_DEC,		"Length(62.5um)",				"x10m"		},	/* Length {62.5um} 					*/
	{	18,	1,	D_CMD_SFP_DISP_DEC,		"Length(cable)",				"m"			},	/* Length {Copper} 					*/
	{	19,	1,	D_CMD_SFP_DISP_DEC,		"Length(OM3)",					"x10m"		},	/* Length (OM3) 					*/
	{	20,	16, D_CMD_SFP_DISP_TEXT,	"Vendor Name",					""			},	/* Vendor Name	 					*/
	{	36,	1, 	D_CMD_SFP_DISP_HEX,		"Transceiver",					""			},	/*	Reserved 						*/
	{	37,	3,	D_CMD_SFP_DISP_HEX,		"Vendor OUI",					""			},	/* Vendor OUI	 					*/
	{	40, 16,	D_CMD_SFP_DISP_TEXT,	"Vendor PN",					""			},	/* Vendor PN	 					*/
	{	56,	4, 	D_CMD_SFP_DISP_TEXT,	"Vendor rev",					""			},	/* Vendor REV	 					*/
	{	60,	2, 	D_CMD_SFP_DISP_HEX,		"Wavelength",					""			},	/* Wave Length	 					*/
	{	62,	1, 	D_CMD_SFP_DISP_HEX,		"Unallocated",					""			},	/*	Reserved	 					*/
	{	63,	1, 	D_CMD_SFP_DISP_HEX,		"CC_BASE",						""			},	/* CC BASE		 					*/

	{	 0, 0,  D_CMD_SFP_DISP_TIT1,	"EXTENDED ID FIELDS",			""			},
	{	64,	2, 	D_CMD_SFP_DISP_HEX,		"Options",						""			},	/* Options		 					*/
	{	66,	1, 	D_CMD_SFP_DISP_DEC,		"BR max",						"%"			},	/* BR max		 					*/
	{	67,	1, 	D_CMD_SFP_DISP_DEC,		"BR min",						"%"			},	/* BR min		 					*/
	{	68,	16, D_CMD_SFP_DISP_TEXT,	"Vendor SN",					""			},	/* Vendor SN	 					*/
	{	84,	8, 	D_CMD_SFP_DISP_TEXT,	"Date Code",					""			},	/* Date Code	 					*/
	{	92,	1, 	D_CMD_SFP_DISP_HEX,		"Diagnostic Monitor Type",		""			},	/* Diagnostic Monitor Type			*/
	{	93,	1, 	D_CMD_SFP_DISP_HEX,		"Enhanced Options",				""			},	/* Enhancecd Options				*/
	{	94,	1, 	D_CMD_SFP_DISP_HEX,		"SFF-8472 Compliance",			""			},	/* 5FF-8472 compliance				*/
	{	95,	1, 	D_CMD_SFP_DISP_HEX,		"CC_EXT",						""			},	/* CC EXT							*/

	{	 0,  0, D_CMD_SFP_DISP_TIT1,	"VENDOR SPECIFIC ID FIELDS",	""			},
	{	96, 32, D_CMD_SFP_DISP_HEX,		"Vendor Specific",				""			},	/* Vencor Specific					*/
	{	128,128, D_CMD_SFP_DISP_HEX,	"Reserved.",					""			},	/* Reserved SFF8079					*/
	{	-1, 0, 	0, 						0,								""			}	/* terminate						*/
};

/* SFP Address(A1) */
const T_CMD_SFP_DATA sfp_data_A2[] =
{
	{	0,	0, D_CMD_SFP_DISP_TIT1,		"DIAGNOSTIC AND CONTROL/STATUS FIELDS",	""		},
	{	0,	0, D_CMD_SFP_DISP_TIT2,		"A/W Thresholds",						""		},
		{	0,	2, D_CMD_SFP_DISP_TEMP,		"Temp High Alarm",			"C"		},	/* Temp High Alarm(Thresholds)		*/
		{	2,	2, D_CMD_SFP_DISP_TEMP,		"Temp Low Alarm",			"C"		},	/* Temp Low Alarm(Thresholds)		*/
		{	4,	2, D_CMD_SFP_DISP_TEMP,		"Temp High Warning",		"C"		},	/* Temp High Warning(Thresholds)	*/
		{	6,	2, D_CMD_SFP_DISP_TEMP,		"Temp Low Warning",			"C"		},	/* Temp Low Warning(Thresholds)		*/
		{	8,	2, D_CMD_SFP_DISP_VOL,		"Voltage High Alarm",		"V"		},	/* Voltage High Alarm(Thresholds)	*/
		{	10,	2, D_CMD_SFP_DISP_VOL,		"Voltage Low Alarm",		"V"		},	/* Voltage Low Alarm(Thresholds)	*/
		{	12,	2, D_CMD_SFP_DISP_VOL,		"Voltage High Warning",		"V"		},	/* Voltage High Warning(Thresholds)	*/
		{	14,	2, D_CMD_SFP_DISP_VOL,		"Voltage Low Warning",		"V"		},	/* Voltage Low Warning(Thresholds)	*/
		{	16,	2, D_CMD_SFP_DISP_BIAS,		"Bias High Alarm",			"uA"	},	/* Bias High Alarm(Thresholds)		*/
		{	18,	2, D_CMD_SFP_DISP_BIAS,		"Bias Low Alarm",			"uA"	},	/* Bias Low Alarm(Thresholds)		*/
		{	20,	2, D_CMD_SFP_DISP_BIAS,		"Bias High Warning",		"uA"	},	/* Bias High Warning(Thresholds)	*/
		{	22,	2, D_CMD_SFP_DISP_BIAS,		"Bias Low Warning",			"uA"	},	/* Bias Low Warning(Thresholds)		*/
		{	24,	2, D_CMD_SFP_DISP_POW,		"TX Power High Alarm",		"mW"	},	/* TX POWER High Alarm(Thresholds)	*/
		{	26,	2, D_CMD_SFP_DISP_POW,		"TX Power Low Alarm",		"mW"	},	/* TX POWER Low Alarm(Thresholds)	*/
		{	28,	2, D_CMD_SFP_DISP_POW,		"TX Power High Warning",	"mW"	},	/* TX POWER High Warning(Thresholds)*/
		{	30,	2, D_CMD_SFP_DISP_POW,		"TX Power Low Warning",		"mW"	},	/* TX POWER Low Warning(Thresholds)	*/
		{	32,	2, D_CMD_SFP_DISP_POW,		"RX Power High Alarm",		"mW"	},	/* TX PULSE High Alarm(Thresholds)	*/
		{	34,	2, D_CMD_SFP_DISP_POW,		"RX Power Low Alarm",		"mW"	},	/* TX PULSE Low Alarm(Thresholds)	*/
		{	36,	2, D_CMD_SFP_DISP_POW,		"RX Power High Warning",	"mW"	},	/* TX PULSE High Warning(Thresholds)*/
		{	38,	2, D_CMD_SFP_DISP_POW,		"RX Power Low Warning",		"mW"	},	/* TX PULSE Low Warning(Thresholds)	*/
		{   40, 16, D_CMD_SFP_DISP_HEX,		"Unallocated",				""		},	/*	Unallocated		 				*/

	{	0,	0, D_CMD_SFP_DISP_TIT2,		"Ext Cal Constants",		""		},	
		{	56, 4, D_CMD_SFP_DISP_RX_PWR,	"Rx_PWR(4)",				"uW"	},	/* 					 				*/
		{	60, 4, D_CMD_SFP_DISP_RX_PWR,	"Rx_PWR(3)",				"uW"	},	/* 					 				*/
		{	64, 4, D_CMD_SFP_DISP_RX_PWR,	"Rx_PWR(2)",				"uW"	},	/* 					 				*/
		{	68, 4, D_CMD_SFP_DISP_RX_PWR,	"Rx_PWR(1)",				"uW"	},	/* 					 				*/
		{	72, 4, D_CMD_SFP_DISP_RX_PWR,	"Rx_PWR(0)",				"uW"	},	/* 					 				*/
		{	76, 2, D_CMD_SFP_DISP_SLOPE,	"Tx_I(Slope)",				""		},	/* 					 				*/
		{	78, 2, D_CMD_SFP_DISP_OFFSET,	"Tx_I(Offset)",				""		},	/* 					 				*/
		{	80, 2, D_CMD_SFP_DISP_SLOPE,	"Tx_PWR(Slope)",			""		},	/* 					 				*/
		{	82, 2, D_CMD_SFP_DISP_OFFSET,	"Tx_PWR(Offset)",			""		},	/* 					 				*/
		{	84, 2, D_CMD_SFP_DISP_SLOPE,	"T (Slope)",				""		},	/* 					 				*/
		{	86, 2, D_CMD_SFP_DISP_OFFSET,	"T (Offset)",				""		},	/* 					 				*/
		{	88, 2, D_CMD_SFP_DISP_SLOPE,	"V (Slope)",				""		},	/* 					 				*/
		{	90, 2, D_CMD_SFP_DISP_OFFSET,	"V (Offset)",				""		},	/* 					 				*/
	{	92,	3, D_CMD_SFP_DISP_HEX,		"Unallocated",					""		},	/* Unallocated		 					*/
	{	95,	1, D_CMD_SFP_DISP_HEX,		"CC_DMI",						""		},	/* CC DMI		 					*/

	{	0,  0, D_CMD_SFP_DISP_TIT2,		"Diagnostics",							""		},
		{	96,	2, D_CMD_SFP_DISP_TEMP,		"Temperature",				"C"		},	/* Temperature	 					*/
		{	98,	2, D_CMD_SFP_DISP_VOL,		"Vcc",						"V"		},	/* Vcc 		 						*/
		{	100,2, D_CMD_SFP_DISP_BIAS,		"TX Bias",					"uA"	},	/* TX Bias	 						*/
		{	102,2, D_CMD_SFP_DISP_POW,		"TX Power",					"mW"	},	/* TX Power	 						*/
		{	104,2, D_CMD_SFP_DISP_POW,		"RX Power",					"mW"	},	/* RX Power	 						*/
	{	106,4, D_CMD_SFP_DISP_HEX,		"Unallocated"							""		},	/*	Reserved						*/
	{	0,  0, D_CMD_SFP_DISP_TIT2,		"Other",								""		},	
	{	110,1, D_CMD_SFP_DISP_HEX,		"Status/Control",						""		},	/* Status/Control					*/
	{	111,1, D_CMD_SFP_DISP_HEX,		"Reserved",								""		},	/*	{Reserved}	 					*/
	{	112,2, D_CMD_SFP_DISP_HEX,		"Alarm Flags",							""		},	/* Alarm Flags						*/
	{	114,2, D_CMD_SFP_DISP_HEX,		"Unallocated",								""		},	/*	Unallocated	 					*/
	{	116,2, D_CMD_SFP_DISP_HEX,		"Warning Flags",						""		},	/* Warning Flags					*/
	{	118,2, D_CMD_SFP_DISP_HEX,		"Ext Status/Control",					""		},	/* Ext Status/Control				*/
	{	0,	0, D_CMD_SFP_DISP_TIT1,		"GENERAL USE FIELDS",					""		},
		{	120,8, D_CMD_SFP_DISP_HEX,		"Vendor Specific",						""		},	/* Vendor Specific					*/
		{	128,120, D_CMD_SFP_DISP_HEX,	"User EEPROM",							""		},	/* User EEPROM						*/
		{	248,  8, D_CMD_SFP_DISP_HEX,	"Vendor Control",						""		},	/* Vendor Control */
	{	-1, 0, 	0, 						0,										""		}	/* termnator						*/
};


const SHORT	D_CMD_SFP_DATA_MASK = 0xFF00;
const CHAR *D_CMD_SFP_STR_OPT0 = "Primary SFP  ";
const CHAR *D_CMD_SFP_STR_OPT1 = "Secondary SFP";
const CHAR *D_CMD_SFP_A0h_FMT = "2-wire adress A0h(%s)\n";
const CHAR *D_CMD_SFP_A2h_FMT = "2-wire adress A2h(%s)\n";


/*
 * conv Temp
 */
void convTemp( unsigned char *top ) {
	char	msb			=	*top	;
	char	lsb			=	*(top+1);
    double  msbtmpD6 	= 0.0 ;
    double  msbtmpD5 	= 0.0 ;
    double  msbtmpD4 	= 0.0 ;
    double  msbtmpD3 	= 0.0 ;
    double  msbtmpD2 	= 0.0 ;
    double  msbtmpD1 	= 0.0 ;
    double  msbtmpD0 	= 0.0 ;
    double  msbtmp   	= 0.0 ;
    double  lsbtmpD7 	= 0.0 ;
    double  lsbtmpD6 	= 0.0 ;
    double  lsbtmpD5 	= 0.0 ;
    double  lsbtmpD4 	= 0.0 ;
    double  lsbtmpD3 	= 0.0 ;
    double  lsbtmpD2 	= 0.0 ;
    double  lsbtmpD1 	= 0.0 ;
    double  lsbtmpD0 	= 0.0 ;
    double  lsbtmp   	= 0.0 ;

    /* msb */
    msbtmpD6 = (( 0x40 & msb )== 0)? 0.0 :  64.0  ;
    msbtmpD5 = (( 0x20 & msb )== 0)? 0.0 :  32.0  ;
    msbtmpD4 = (( 0x10 & msb )== 0)? 0.0 :  16.0  ;
    msbtmpD3 = (( 0x08 & msb )== 0)? 0.0 :  8.0  ;
    msbtmpD2 = (( 0x04 & msb )== 0)? 0.0 :  4.0  ;
    msbtmpD1 = (( 0x02 & msb )== 0)? 0.0 :  2.0  ;
    msbtmpD0 = (( 0x01 & msb )== 0)? 0.0 :  1.0  ;
    msbtmp = msbtmpD6 + msbtmpD5 + msbtmpD4 + msbtmpD3 + msbtmpD2 + msbtmpD1 + msbtmpD0 ;
//printf("// msbtmp : %02x %lf\n", msb, msbtmp ) ;
    if( (msb&0x80) == 0x80 ) { /* minus */
        msbtmp = 0x80 - msbtmp ;
    }
    /* lsb */
    lsbtmpD7 = (( 0x80 & lsb )== 0)? 0.0 :  0.5         ;
    lsbtmpD6 = (( 0x40 & lsb )== 0)? 0.0 :  0.25        ;
    lsbtmpD5 = (( 0x20 & lsb )== 0)? 0.0 :  0.125       ;
    lsbtmpD4 = (( 0x10 & lsb )== 0)? 0.0 :  0.0625      ;
    lsbtmpD3 = (( 0x08 & lsb )== 0)? 0.0 :  0.03125     ;
    lsbtmpD2 = (( 0x04 & lsb )== 0)? 0.0 :  0.015625    ;
    lsbtmpD1 = (( 0x02 & lsb )== 0)? 0.0 :  0.0078125   ;
    lsbtmpD0 = (( 0x01 & lsb )== 0)? 0.0 :  0.00390625  ;

    lsbtmp  =   lsbtmpD7 + lsbtmpD6 + lsbtmpD5+ lsbtmpD4+ lsbtmpD3+ lsbtmpD2+ lsbtmpD1+ lsbtmpD0;
    if(( msb & 0x80 ) == 0x80 ) { /* minus */
        lsbtmp = 0.0 - lsbtmp ;
    }
    printf( "%s%f", (((msb&0x80)== 0)? "":"-"), msbtmp+lsbtmp ) ; /* pgr0823 */
}
void convSlopes( unsigned char *top ) {
	char	msb		=	*top	;
	char	lsb		=	*(top+1);
    double  msbtmpD7 = 0.0 ;
    double  msbtmpD6 = 0.0 ;
    double  msbtmpD5 = 0.0 ;
    double  msbtmpD4 = 0.0 ;
    double  msbtmpD3 = 0.0 ;
    double  msbtmpD2 = 0.0 ;
    double  msbtmpD1 = 0.0 ;
    double  msbtmpD0 = 0.0 ;
    double  msbtmp   = 0.0 ;
    double  lsbtmpD7 = 0.0 ;
    double  lsbtmpD6 = 0.0 ;
    double  lsbtmpD5 = 0.0 ;
    double  lsbtmpD4 = 0.0 ;
    double  lsbtmpD3 = 0.0 ;
    double  lsbtmpD2 = 0.0 ;
    double  lsbtmpD1 = 0.0 ;
    double  lsbtmpD0 = 0.0 ;
    double  lsbtmp   = 0.0 ;

    /* msb */
    msbtmpD7 = (( 0x80 & msb  )== 0)? 0.0 : 128.0  ;
    msbtmpD6 = (( 0x40 & msb  )== 0)? 0.0 : 64.0  ;
    msbtmpD5 = (( 0x20 & msb  )== 0)? 0.0 : 32.0  ;
    msbtmpD4 = (( 0x10 & msb  )== 0)? 0.0 : 16.0  ;
    msbtmpD3 = (( 0x08 & msb  )== 0)? 0.0 : 8.0  ;
    msbtmpD2 = (( 0x04 & msb  )== 0)? 0.0 : 4.0  ;
    msbtmpD1 = (( 0x02 & msb  )== 0)? 0.0 : 2.0  ;
    msbtmpD0 = (( 0x01 & msb  )== 0)? 0.0 : 1.0  ;
    msbtmp = msbtmpD7 +msbtmpD6 + msbtmpD5 + msbtmpD4 + msbtmpD3 + msbtmpD2 + msbtmpD1 + msbtmpD0 ;


    /* lsb */
    lsbtmpD7 = (( 0x80 & lsb )== 0)? 0.0 :  0.5         ;
    lsbtmpD6 = (( 0x40 & lsb )== 0)? 0.0 :  0.25        ;
    lsbtmpD5 = (( 0x20 & lsb )== 0)? 0.0 :  0.125       ;
    lsbtmpD4 = (( 0x10 & lsb )== 0)? 0.0 :  0.0625      ;
    lsbtmpD3 = (( 0x08 & lsb )== 0)? 0.0 :  0.03125     ;
    lsbtmpD2 = (( 0x04 & lsb )== 0)? 0.0 :  0.015625    ;
    lsbtmpD1 = (( 0x02 & lsb )== 0)? 0.0 :  0.0078125   ;
    lsbtmpD0 = (( 0x01 & lsb )== 0)? 0.0 :  0.00390625  ;

    lsbtmp = lsbtmpD7 + lsbtmpD6 + lsbtmpD5+ lsbtmpD4+ lsbtmpD3+ lsbtmpD2+ lsbtmpD1+ lsbtmpD0;

    printf( "%f", msbtmp+lsbtmp ) ; /* pgr0823 */

}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  SFP INFO表示コマンド
 *  @note   I2CからSFP情報を読み出し、表示する
 *  @param  argc  [in]   Standard main argc
 *  @param  argv  [in]   Standard main argv
 *  @return result code
 *  @retval D_CMD_OK
 *  @retval D_CMD_NG

 */
/********************************************************************************************************************/
int cmd_sfp_info_main(int argc, char *argv[], FILE *fp )
{
	int				read_stat;
	int				disp_line_cnt;
	int				disp_num_cnt;
	int				table_cnt;
	int 			target;
	const T_CMD_SFP_DATA*	table;
	UCHAR			buffer[256];
	int				ps;	/* primary(0) or secondary(1) */
	
	if( argc < 2 ) {
		fprintf(fp, "usage:\n");
		fprintf(fp, "  cmd_sfp_info port-type\n");
		fprintf(fp, "  port-type:  P:Primary / S:Secondary\n");
		return D_CMD_OK;
	}
	if( strcmp( "P", argv[1]) == 0 ) {	/* Primary 指定 */
		ps = 0;
	}
	else if( strcmp( "S", argv[1])  == 0 ){
		ps = 1;
	}
	else {
		fprintf(fp, "usage:\n");
		fprintf(fp, "  cmd_sfp_info port-type\n");
		fprintf(fp, "  port-type:  P:Primary / S:Secondary\n");
		return D_CMD_OK;
	}
	/* A0h/A2hを読み出し表示する */
	for(table_cnt=0; table_cnt < 2; table_cnt++)
	{
		/* */
		table = (table_cnt==0) ? sfp_data_A0 : sfp_data_A2;
		if( ps == 0 ) {	/* Primary */
			target =(table_cnt==0) ?  BPF_HM_DEVC_SFP_A0: BPF_HM_DEVC_SFP_A2	;
		}
		else {
			target =(table_cnt==0) ?  BPF_HM_DEVC_SFP_SA0: BPF_HM_DEVC_SFP_SA2	;
		}
		memset(buffer, 0, sizeof(buffer));

		/* I2C read */
		if((read_stat =  BPF_HM_DEVC_SFP_READ_BUFFER( target, 0, 255, buffer ))	!= BPF_HM_DEVC_COMPLETE ) {
			fprintf(fp, "SFP read error. kind:%d ret:%d", target, read_stat);
			return D_CMD_NG;
		}

		/* 表タイトル表示 */
		fprintf(fp, "SFP info : %s\n", (target== BPF_HM_DEVC_SFP_A0 || target == BPF_HM_DEVC_SFP_SA0 )? 
					"2-wire address A0h":	"2-wire address A2h" ) ;
		/* Display */
		for(disp_line_cnt=0; ; disp_line_cnt++)
		{
			/* テーブル上最後の行まで表示したらループを抜ける */
			if(table[disp_line_cnt].addr == -1 )
			{
				break;
			}
		
			if( table[disp_line_cnt].disp_type == D_CMD_SFP_DISP_TIT1 ) {
				fprintf( fp, " %-40s *********************************\n", table[disp_line_cnt].title );
				continue;
			}
			if( table[disp_line_cnt].disp_type == D_CMD_SFP_DISP_TIT2 ) {
				fprintf( fp, "  %-39s --------------------------------\n", table[disp_line_cnt].title );
				continue;
			}

			/* 行タイトル表示 */
			fprintf( fp,  "  %3d-%3d(%3d) %-35s   : ", 
					table[disp_line_cnt].addr, 
					table[disp_line_cnt].addr + table[disp_line_cnt].size-1,
					table[disp_line_cnt].size,
					table[disp_line_cnt].title);

			switch(table[disp_line_cnt].disp_type)	{	/* 表示種別ごと	*/
			case	D_CMD_SFP_DISP_HEX:													/* １６進数表示	*/
				/* HEXの場合16進で表示し、4バイト表示ごとにスペースを挿入する */
				for(disp_num_cnt=0; disp_num_cnt<table[disp_line_cnt].size; disp_num_cnt++)
				{
					if( disp_num_cnt != 0 && (disp_num_cnt % 0x10 ) == 0) {
						fprintf( fp, "\n                                                       ");
					}
					fprintf( fp, "%02x", buffer[table[disp_line_cnt].addr + disp_num_cnt]);
					if(((disp_num_cnt+1) %4) == 0)
					{
						fprintf( fp, " ");
					}
				}
				break;

			case	D_CMD_SFP_DISP_TEXT:													/* 文字列表示	*/
				/* TEXTの場合charで表示する */
				fprintf( fp,  "\"");
				for(disp_num_cnt=0; disp_num_cnt<table[disp_line_cnt].size; disp_num_cnt++)
				{
					fprintf( fp, "%c", buffer[table[disp_line_cnt].addr + disp_num_cnt]);
				}
				fprintf( fp,  "\"");
				break;

			case	D_CMD_SFP_DISP_DEC:		/* 10進数表示	*/
				fprintf( fp,  "%d [%02x]" , ( buffer[ table[disp_line_cnt].addr ] & 0xff ),( buffer[ table[disp_line_cnt].addr ] & 0xff ));
				break;

			case	D_CMD_SFP_DISP_TEMP:		/* 温度変換表示	*/
				convTemp( &buffer[ table[disp_line_cnt].addr ] ) ;	/* 温度変換処理	*/
				fprintf( fp, " [ %02x %02x ] ", buffer[ table[disp_line_cnt].addr ], buffer[ table[disp_line_cnt].addr + 1 ]);
				break;

			case	D_CMD_SFP_DISP_VOL:		/* 電圧変換表示	*/
				{
					unsigned short	short_val = 0;
					char	*p			;
					p = (char *)&short_val	;
					*(p+0)	=	buffer[ table[disp_line_cnt].addr + 1 ];
					*(p+1)	=	buffer[ table[disp_line_cnt].addr + 0 ];
					fprintf( fp, "%3.5f", (float)short_val / 10000.0 );
					fprintf( fp, " [ %02x %02x ]", buffer[ table[disp_line_cnt].addr ] , buffer[ table[disp_line_cnt].addr + 1 ] );
				}
				break;

			case	D_CMD_SFP_DISP_BIAS:	/* バイアス変換表示	*/
				{
					unsigned short	short_val = 0;
					char	*p			;
					p = (char *)&short_val	;
					*(p+0)	=	buffer[ table[disp_line_cnt].addr + 1];
					*(p+1)	=	buffer[ table[disp_line_cnt].addr + 0];
					fprintf( fp, "%3.5f", (float)short_val / 1000.0 );
					fprintf( fp, " [ %02x %02x ]", buffer[ table[disp_line_cnt].addr ], buffer[ table[disp_line_cnt].addr + 1 ] );
				}
				break;

			case	D_CMD_SFP_DISP_POW:		/* 出力変換表示		*/
				{
					short	short_val = 0;
					char	*p			;
					p = (char *)&short_val	;
					*(p+0)	=	buffer[ table[disp_line_cnt].addr + 1];
					*(p+1)	=	buffer[ table[disp_line_cnt].addr + 0];
					fprintf( fp, "%3.5f", (float)short_val / 10000.0 );
					fprintf( fp, " [ %02x %02x ]", buffer[ table[disp_line_cnt].addr ] , buffer[ table[disp_line_cnt].addr + 1 ] );
				}
				break;

			case	D_CMD_SFP_DISP_RX_PWR:	/* RX_PWR			*/
				{
					float retfloat ;
					char    *p ;
					p = (char *)&retfloat ;
					*p 		=	buffer[ table[disp_line_cnt].addr + 0];
					*(p+1) 	=	buffer[ table[disp_line_cnt].addr + 1];
					*(p+2) 	=	buffer[ table[disp_line_cnt].addr + 2];
					*(p+3) 	=	buffer[ table[disp_line_cnt].addr + 3];
					fprintf( fp,  "%f", retfloat ) ;
					fprintf( fp, " [ %02x %02x %02x %02x ]", 
						buffer[ table[disp_line_cnt].addr ], 
						buffer[ table[disp_line_cnt].addr + 1 ],
						buffer[ table[disp_line_cnt].addr + 2 ],
						buffer[ table[disp_line_cnt].addr + 3 ] 
					);
				}
				break;

			case	D_CMD_SFP_DISP_SLOPE:	/* Slope			*/
				convSlopes(&buffer[ table[disp_line_cnt].addr ]);
				fprintf( fp, " [ %02x %02x ]", buffer[ table[disp_line_cnt].addr ] , buffer[ table[disp_line_cnt].addr + 1 ] );
				break;

			case	D_CMD_SFP_DISP_OFFSET:/* Offset			*/
				{
					short cnvs = 0;
					char *p ;
					p = (char *)&cnvs ;
					*(p+1) = buffer[ table[disp_line_cnt].addr] ;
					*(p+0) = buffer[ table[disp_line_cnt].addr+1] ;
					fprintf( fp,  "%d", cnvs ) ;
					fprintf( fp, " [ %02x %02x ]", buffer[ table[disp_line_cnt].addr ] , buffer[ table[disp_line_cnt].addr + 1 ] );
				}
				break;

			}
			/* 単位出力	*/
			fprintf( fp, " %s", table[disp_line_cnt].unit_str ) ;
			fprintf( fp, "\n");
		}		
		
	}

	return D_CMD_OK;

}
/* @} */

/* @} */

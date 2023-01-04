/********************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		m_mk_typ.h
 *  @brief	Maker MT manage task Common structure
 *  @date   2008/07/29 FFCS)zhengmh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
  */
/********************************************************************************/




#ifndef M_MK_TYP
#define M_MK_TYP


/**
 * @brief	Client infomation	structure
 * @note	Contents:	Client infomation structure	\n
 * 
 */

typedef struct
{
	UINT 		conn;				/**<  Connection state		*/
	INT 		tcplinkhdl;			/**<  TCP link handle		*/
}MKT_CLIENTINFO;

/**
 * @brief	CPRI simulator start request	structure
 * @note	Contents:	CPRI simulator start request structure	\n
 * @date    2015/09/15 TDI)satou dummy2→linknoに変更
 */
 
typedef struct
{
	INT	 	linkhandle;		/**<  TCP link handle	*/	
	USHORT	signal_kind;	/**<  signal kind 			*/
	USHORT	dummy1;			/**<  dummy				*/		
	UINT 	msglen;			/**<  Message length	*/
	UINT 	msgevent;		/**<  Message event	*/
	USHORT 	linkno;			/**<  CPRIリンク番号(1-6) */
	USHORT 	simmode;		/**<  Simulator mode(1: common mode  2:L3 test mode*/
}MKT_CPRISIMUSTRREQ;		

/**
 * @brief	CPRI simulator data reception notification	structure
 * @note	Contents:	CPRI simulator data reception notification structure	\n
 * 
 */

typedef struct
{
	INT 	linkhandle;		/**<  TCP link handle	*/
	USHORT	signal_kind;	/**<  signal kind 			*/
	USHORT	dummy;			/**<  dummy				*/		
	UINT 	msglen;			/**<  Message length	*/
	UINT 	msgevent;		/**<  Message event	*/
	UCHAR 	buf[4];			/**<  Buffer			*/
}MKT_CPRISIMUDATRCV;

/**
 * @brief	RTC	structure
 * @note	Contents:	RTC structure	\n
 * 
 */
typedef struct
{
	USHORT 	year ;          /**< year */
	UCHAR	month;          /**< month */
	UCHAR	day;            /**< day */
	UCHAR	hour;           /**< hour */
	UCHAR	min ;           /**< minute */
	UCHAR	sec ;           /**< second */
	UCHAR	msec ;          /**< millisecond */
}MKT_RTC;

/**
 * @brief	CPRI monitor start request	structure
 * @note	Contents:	CPRI monitor start request structure	\n
 * 
 */

typedef struct
{
	INT 	linkhandle;		/**<  TCP link handle	*/		
	UINT 	msglen;			/**<  Message length	*/
	UINT 	msgevent;		/**<  Message event	*/
	USHORT 	dummy1;			/**<  Dummy			*/		
	USHORT 	dummy2;			/**<  Dummy			*/
}MKT_CPRIMONSTRREQ;

/**
 * @brief	CPRI simulator stop request	structure
 * @note	Contents:	CPRI simulator stop request structure	\n
 * 
 */

typedef struct
{
	INT 		linkhandle;	/**<  TCP link handle	*/
	UINT 		msglen;		/**<  Message length	*/
	UINT 		msgevent;	/**<  Message event	*/
	USHORT 		dummy1;		/**<  Dummy			*/
	USHORT 		dummy2;		/**<  Dummy			*/
}MKT_CPRISIMUSTPREQ;

/**
 * @brief	CPRI monitor stop request	structure
 * @note	Contents:	CPRI monitor stop request structure	\n
 * 
 */

typedef struct
{
	INT	 	linkhandle;		/**<  TCP link handle	*/
	UINT 	msglen;			/**<  Message length	*/
	UINT 	msgevent;		/**<  Message event	*/
	USHORT 	dummy1;			/**<  Dummy			*/
	USHORT 	dummy2;			/**<  Dummy			*/
}MKT_CPRIMONSTPREQ;

/**
 * @brief	RE-MT connection start request	structure
 * @note	Contents:	RE-MT connection start request structure	\n
 * 
 */

typedef struct
{
	UINT	Linkhandle;		/**<  link handle */
	UINT 	CPRIlinkno;		/**<  CPRI link number */
	UINT 	Carrier;		/**<  carrier */
	UINT 	Sector;			/**<  sector */
}MKT_REMTCONSTRREQ;

/**
 * @brief	RE-MT connection start request interface	structure
 * @note	Contents:	RE-MT connection start request interface structure	\n
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head		*/
	MKT_REMTCONSTRREQ		constrreq;			/**<  RE-MT connection start request */
}MKT_TCPIF_REMTCONSTRREQ;

/**
 * @brief	RE-MT connection start response	structure
 * @note	Contents:	RE-MT connection start response structure	\n
 * 
 */


typedef struct
{
	UINT	Result;			/**<  OK/NG	*/
	UINT 	NGCode;			/**<  NG Code	*/
}MKT_REMTCONSTRRSP;

/**
 * @brief	RE-MT connection start response	interface structure
 * @note	Contents:	RE-MT connection start response interface structure	\n
 * 
 */
 
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head		*/
	MKT_REMTCONSTRRSP		constrrsp;			/**<  RE-MT connection start response */
}MKT_TCPIF_REMTCONSTRRSP;

/**
 * @brief	RE-MT connection force release	notice structure
 * @note	Contents:	RE-MT connection force release notice structure	\n
 * 
 */


typedef struct
{
	UINT	NGCode;			/**<  Cause Code	*/
}MKT_REMTCONFRCSTPNTC;

/**
 * @brief	RE-MT connection force release	notice interface structure
 * @note	Contents:	RE-MT connection force release notice interface structure	\n
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			    head;				/**<  task common head			*/
	MKT_REMTCONFRCSTPNTC		confrcstp;			/**<  RE-MT connection force release notice */
}MKT_TCPIF_REMTCONFRCSTPNTC;

/**
 * @brief	RE-MT connection stop request	structure
 * @note	Contents:	RE-MT connection stop request structure	\n
 * 
 */

typedef struct
{
	UINT	NGCode;			/**<  Cause Code(Reserve)	*/
}MKT_REMTCONSTPREQ;

/**
 * @brief	RE-MT connection stop request	interface structure
 * @note	Contents:	RE-MT connection stop request interface structure	\n
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head		*/
	MKT_REMTCONSTPREQ		constpreq;			/**<  RE-MTconnection stop request */
}MKT_TCPIF_REMTCONSTPREQ;

/**
 * @brief	RE-MT connection stop response	structure
 * @note	Contents:	RE-MT connection stop response structure	\n
 * 
 */

typedef struct
{
	UINT	Result;			/**<  OK/NG			*/
	UINT 	NGCode;			/**<  NG Code(Reserve)	*/
}MKT_REMTCONSTPRSP;

/**
 * @brief	RE-MT connection stop response	interface structure
 * @note	Contents:	RE-MT connection stop response interface structure	\n
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head		*/
	MKT_REMTCONSTPRSP		constprsp;			/**<  RE-MT connection stop response */
}MKT_TCPIF_REMTCONSTPRSP;




/**
 *  @brief  RE-MT inventory information read request
 *  @note  Contents : read request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	RegNum;			/**<  EEPROM Number				*/
	USHORT 	RegAddr;		/**<  EEPROM Address			*/
}MKT_REMTIVTINFREQ;


/**
 *  @brief  RE-MT inventory information read  request interface structure
 *  @note  Contents : RE-MT inventory information read  request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head					*/
	MKT_REMTIVTINFREQ		IvtInfReq;			/**<  inventory information read request */
}MKT_TCPIF_REMTIVTINFREQ;

/**
 *  @brief  RE-MT inventory information read response
 *  @note  Contents : read response for RE-MT
 *  
 * 
 */

typedef struct
{
	UINT	FunctionID;			/**<  Function ID				*/
	UINT 	SeqNo;				/**<  Sequence NO				*/
	UINT 	Result;				/**<  OK/NG						*/
	UINT 	NGCode;				/**<  NG Code(reserve)			*/
	UINT 	RegNum;				/**<  EEPROM Number	*/
	USHORT 	RegAddr;			/**<  EEPROM Address	*/
	USHORT 	RegValue;			/**<  EEPROM Value		*/
}MKT_REMTIVTINFRES;

/**
 *  @brief  RE-MT inventory information read response interface structure
 *  @note  Contents : RE-MT inventory information read response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head					*/
	MKT_REMTIVTINFRES		IvtInfRes;			/**<  inventory information read response */
}MKT_TCPIF_REMTIVTINFRES;


/**
 *  @brief  RE-MT svinf read request
 *  @note  Contents : read request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
}MKT_REMTSVINFREQ;


/**
 *  @brief  RE-MT svinf read  request interface structure
 *  @note  Contents : RE-MT svinf read  request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head		*/
	MKT_REMTSVINFREQ		SvInfReq;			/**<  svinf read request */
}MKT_TCPIF_REMTSVINFREQ;

/**
 *  @brief  RE-MT svinf read response
 *  @note  Contents : read response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG						*/
	UINT 	NGCode;			/**<  NG Code(reserve)			*/
	UINT 	SvInf[5];		/**<  SvInf						*/
}MKT_REMTSVINFRES;

/**
 *  @brief  RE-MT svinf read response interface structure
 *  @note  Contents : RE-MT svinf read response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head		*/
	MKT_REMTSVINFRES		SvInfRes;			/**<  svinf read response */
}MKT_TCPIF_REMTSVINFRES;


/**
 *  @brief  RE-MT carrier information read request
 *  @note  Contents : read request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
}MKT_REMTCARRFREQREQ;


/**
 *  @brief  RE-MT carrier information read  request interface structure
 *  @note  Contents : RE-MT carrier information read  request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head					*/
	MKT_REMTCARRFREQREQ		CarrFreqReq;		/**<  carrier information read request  */
}MKT_TCPIF_REMTCARRFREQREQ;

/**
 *  @brief  RE-MT carrier information read response
 *  @note  Contents : read response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG						*/
	UINT 	NGCode;			/**<  NG Code(reserve)			*/
	UINT 	Carrier[2];		/**<  Carrier state				*/
	UINT 	Freqence[2];	/**<  Freqence					*/
	UINT 	Power[2];		/**<  Power						*/
}MKT_REMTCARRFREQRES;

/**
 *  @brief  RE-MT carrier information read response interface structure
 *  @note  Contents : RE-MT carrier information read response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head				*/
	MKT_REMTCARRFREQRES		CarrFreqRes;		/**<  carrier information read response */
}MKT_TCPIF_REMTCARRFREQRES;

/**
 *  @brief  RE-MT card block control request
 *  @note  Contents : control request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Blk;			/**<  0:unblock 1:block			*/
}MKT_REMTBLKREQ;


/**
 *  @brief  RE-MT card block control request interface structure
 *  @note  Contents : RE-MT card block control request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;			/**<  task common head			*/
	MKT_REMTBLKREQ			BlkReq;			/**<  card block control request */
}MKT_TCPIF_REMTBLKREQ;

/**
 *  @brief  RE-MT card block control response
 *  @note  Contents : read response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG						*/
	UINT 	NGCode;			/**<  NG Code(reserve)			*/
}MKT_REMTBLKRES;

/**
 *  @brief  RE-MT card block control response interface structure
 *  @note  Contents : RE-MT card block control response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;			/**<  task common head			*/
	MKT_REMTBLKRES		BlkRes;			/**<  card block control response */
}MKT_TCPIF_REMTBLKRES;

/**
 *  @brief  RE-MT card reset control request
 *  @note  Contents : control request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
}MKT_REMTRSTREQ;


/**
 *  @brief  RE-MT card reset control request interface structure
 *  @note  Contents : RE-MT card reset control request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;			/**<  task common head			*/
	MKT_REMTRSTREQ			RstReq;			/**<  card reset control request */
}MKT_TCPIF_REMTRSTREQ;

/**
 *  @brief  RE-MT card reset control response
 *  @note  Contents : read response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG					*/
	UINT 	NGCode;			/**<  NG Code(reserve)				*/
}MKT_REMTRSTRES;

/**
 *  @brief  RE-MT card reset control response interface structure
 *  @note  Contents : RE-MT card reset control response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;			/**<  task common head		*/
	MKT_REMTRSTRES		RstRes;			/**<  card reset control response */
}MKT_TCPIF_REMTRSTRES;


/**
 *  @brief  RE-MT PA ON/OFF control request
 *  @note  Contents : control request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Branch;			/**<  BranchNo:0~3				*/
	UINT 	OnOff;			/**<  0:Off 1:ON				*/
}MKT_REMTPAONOFFREQ;


/**
 *  @brief  RE-MT PA ON/OFF control request interface structure
 *  @note  Contents : RE-MT PA ON/OFF control request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;			/**<  task common head		*/
	MKT_REMTPAONOFFREQ		PaOnOffReq;		/**<  PA ON/OFF control request	*/
}MKT_TCPIF_REMTPAONOFFREQ;

/**
 *  @brief  RE-MT PA ON/OFF control response
 *  @note  Contents : read response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG					*/
	UINT 	NGCode;			/**<  NG Code(reserve)				*/
}MKT_REMTPAONOFFRES;

/**
 *  @brief  RE-MT PA ON/OFF control response interface structure
 *  @note  Contents : RE-MT PA ON/OFF control response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;			/**<  task common head		*/
	MKT_REMTPAONOFFRES	PaOnOffRes;		/**<  PA ON/OFF control response */
}MKT_TCPIF_REMTPAONOFFRES;


/**
 *  @brief  RE-MT AISG state read request
 *  @note  Contents : read request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
}MKT_REMTAISGGETREQ;


/**
 *  @brief  RE-MT AISG state read request interface structure
 *  @note  Contents : RE-MT AISG state read request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;			/**<  task common head		*/
	MKT_REMTAISGGETREQ		AisgGetReq;		/**<  AISG state read request */
}MKT_TCPIF_REMTAISGGETREQ;

/**
 *  @brief  RE-MT AISG state read response
 *  @note  Contents : read response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG					*/
	UINT 	NGCode;			/**<  NG Code(reserve)				*/
	UINT 	State;			/**<  0: not connect  1:connect			*/
}MKT_REMTAISGGETRES;

/**
 *  @brief  RE-MT AISG state read response interface structure
 *  @note  Contents : RE-MT AISG state read response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;			/**<  task common head		*/
	MKT_REMTAISGGETRES	AisgGetRes;		/**<  AISG state read response */
}MKT_TCPIF_REMTAISGGETRES;


/**
 *  @brief  RE-MT AISG state set request
 *  @note  Contents : set request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	State;			/**<  0: not connect  1:connect			*/
}MKT_REMTAISGSETREQ;


/**
 *  @brief  RE-MT AISG state set request interface structure
 *  @note  Contents : RE-MT AISG state set request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;			/**<  task common head		*/
	MKT_REMTAISGSETREQ		AisgSetReq;		/**<  AISG state set request */
}MKT_TCPIF_REMTAISGSETREQ;

/**
 *  @brief  RE-MT AISG state set response
 *  @note  Contents : set response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG					*/
	UINT 	NGCode;			/**<  NG Code(reserve)				*/
}MKT_REMTAISGSETRES;

/**
 *  @brief  RE-MT AISG state set response interface structure
 *  @note  Contents : RE-MT AISG state set response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;			/**<  task common head		*/
	MKT_REMTAISGSETRES	AisgSetRes;		/**<  AISG state set response */
}MKT_TCPIF_REMTAISGSETRES;

/**
 *  @brief  RE-MT fiber output state read request
 *  @note  Contents : read request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
}MKT_REMTFIBEROUTPUTGETREQ;


/**
 *  @brief  RE-MT fiber output state read request interface structure
 *  @note  Contents : RE-MT fiber output state read request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD				head;				/**<  task common head				*/
	MKT_REMTFIBEROUTPUTGETREQ	FiberOutputGetReq;	/**<  fiber output state read request */
}MKT_TCPIF_REMTFIBEROUTPUTGETREQ;

/**
 *  @brief  FHM-MT fiber output state read request

 *  @note  Contents : read request for FHM-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
}MKT_FHMMT_FIBEROUTPUTGET_REQ;

/**
 *  @brief  FHM-MT fiber output state read request interface structure
 *  @note  Contents : FHM-MT fiber output state read request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;		/**<  task common head				*/
	UINT	FunctionID;				/**<  Function ID				*/
	UINT 	SeqNo;					/**<  Sequence NO				*/
	UINT 	Result;					/**<  OK/NG					*/
	UINT 	NGCode;					/**<  NG Code(reserve)				*/
	UINT	FiberOutput[D_RRH_CPRINO_RE_MAX];	/**<  fiber output state read request */
}MKT_FHMMT_FIBEROUTPUTGET_RES;

/**
 *  @brief  FHM-MT fiber output state set request

 *  @note  Contents : set request for FHM-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID						*/
	UINT 	SeqNo;			/**<  Sequence NO						*/
	UINT 	FiberOutID;		/**<  Sequence NO						*/
	UINT	State;			/**<  0: output off  1: output on				*/
}MKT_FHMMT_FIBEROUTPUTSET_REQ;

/**

 *  @brief  FHM-MT fiber output state set response interface structure
 *  @note  Contents : FHM-MT fiber output state set response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;	/**<  task common head				*/
	UINT	FunctionID;			/**<  Function ID				*/
	UINT 	SeqNo;				/**<  Sequence NO				*/
	UINT 	Result;				/**<  OK/NG					*/
	UINT 	NGCode;				/**<  NG Code(reserve)				*/
}MKT_FHMMT_FIBEROUTPUTSET_RES;

/**
 *  @brief  FHM-MT FHM state read request
 *  @note  Contents : read request for FHM-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
}MKT_FHMMT_STATEGET_REQ;


/**
 *  @brief  FHM-MT FHM state read response
 *  @note  Contents : read response for FHM-MT
 *  
 * 
 */
typedef struct
{
	UINT	ActState;			/**<  Act state info			*/
	UINT	UseState;			/**<  Use state info			*/
	UINT	AlmState;			/**<  Alm state info			*/
	UINT	ErrState;			/**<  Err state info			*/
	UINT	Alm_num;			/**<  ALM情報数				*/
	UINT	Alm_info[255];			/**<  ALM情報				*/
	UINT	Err_num;			/**<  ERR情報数				*/
	UINT	Err_info[255];			/**<  ERR情報				*/
}MKM_FHMMT_FHMSTATEGET_RES;

/**
 *  @brief  FHM-MT RE state read response
 *  @note  Contents : read response for FHM-MT
 *  
 * 
 */
typedef struct
{
	UINT	ActState;			/**<  RE Act state info			*/
	UINT	UseState;			/**<  RE Use state info			*/
	UINT	AlmState;			/**<  RE Alm state info			*/
	UINT	ErrState;			/**<  RE Err state info			*/
	UINT	AmpState;			/**<  RE Amp state info			*/
	UINT	WupState;			/**<  RE W-Up state info		*/
	UINT	FanState;			/**<  RE Fan state info			*/
}MKM_FHMMT_RESTATEGET_RES;

/**
 *  @brief  FHM-MT RE state read response
 *  @note  Contents : read response for FHM-MT
 *  
 * 
 */
typedef struct
{
	UINT	ActState;			/**<  00: Off, 11: Lighting, 10: T.B.D		*/
	UINT	ErrState;			/**<  00: Off, 11: Lighting, 10: T.B.D		*/
}MKM_FHMMT_CPRILEDSTA_RES;

/**
 *  @brief  FHM-MT FHM state read response
 *  @note  Contents : read response for FHM-MT
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;		/**<  task common head				*/
	UINT	FunctionID;			/**<  Function ID				*/
	UINT 	SeqNo;				/**<  Sequence NO				*/
	UINT 	Result;				/**<  OK/NG					*/
	UINT 	NGCode;				/**<  NG Code(reserve)				*/
	UINT 	HWVer[2];			/**<  Hard ware version				*/
	UINT 	FWVer[2];			/**<  Firm ware version				*/
	UCHAR 	DeviceKind[D_RRH_CARD_NAME_LEN];		/**<  Device kind				*/
	MKM_FHMMT_FHMSTATEGET_RES 	FHMState;		/**<  [ACT, USE, ALM, ERR]	*/
	MKM_FHMMT_RESTATEGET_RES 	REState[D_RRH_CPRINO_RE_MAX];		/**<  RE state 			*/
	MKM_FHMMT_CPRILEDSTA_RES 	CPRIState[D_RRH_CPRINO_NUM];		/**<  CPRI state		*/
}MKT_FHMMT_STATEGET_RES;

typedef struct
{
	UINT	ledSta;
	UINT	setVal;
}T_RSV_LEDINFO;

/**
 *  @brief  FHM-MT RE ALM, ERR, FAN state read request
 *  @note  Contents : read request for FHM-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	REIndex;		/**<  RE index					*/
}MKT_FHMMT_REINFSTATEGET_REQ;

/**
 *  @brief  FHM-MT RE ALM, ERR, FAN state read response

 *  @note  Contents : read response for FHM-MT
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;		/**<  task common head				*/
	UINT	FunctionID;			/**<  Function ID				*/
	UINT 	SeqNo;				/**<  Sequence NO				*/
	UINT 	Result;				/**<  OK/NG					*/
	UINT 	NGCode;				/**<  NG Code(reserve)				*/
	UINT 	Alm_num;			/**<  Alm number				*/
	UINT 	Alm_inf[255];			/**<  Alm info array				*/
	UINT 	Err_num;			/**<  Error number				*/
	UINT 	Err_inf[255];			/**<  Error info array				*/
	UINT 	Fan_num;			/**<  Fan number				*/
	UINT 	Fan_inf[D_RRH_CPRINO_RE_MAX];	/**<  Fan info array				*/
}MKT_FHMMT_REINFSTATEGET_RES;

/**
 *  @brief  RE use/unuse state read request
 *  @note  Contents : read request for FHM-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
}MKT_FHMMT_REUSESTATEGET_REQ;

/**
 *  @brief  RE use/unuse state read response
 *  @note  Contents : read request for FHM-MT
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;	/**<  task common head					*/
	UINT	FunctionID;		/**<  Function ID					*/
	UINT 	SeqNo;			/**<  Sequence NO					*/
	UINT 	Result;			/**<  OK/NG						*/
	UINT 	NGCode;			/**<  NG Code(reserve)				*/
	UINT 	USEState[D_RRH_CPRINO_RE_MAX];		/**<  0: unuse  1: use		*/
}MKT_FHMMT_REUSESTATEGET_RES;

/**
 *  @brief  RE use/unuse state set request
 *  @note  Contents : Set request for FHM-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	REIndex;		/**<  Sequence NO				*/
	UINT 	State;			/**<  0: unuse  1: use				*/
}MKT_FHMMT_REUSESTATESET_REQ;

/**
 *  @brief  RE use/unuse state set response
 *  @note  Contents : set request for FHM-MT
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;	/**<  task common head					*/
	UINT	FunctionID;		/**<  Function ID					*/
	UINT 	SeqNo;			/**<  Sequence NO					*/
	UINT 	Result;			/**<  OK/NG						*/
	UINT 	NGCode;			/**<  NG Code(reserve)				*/
}MKT_FHMMT_REUSESTATESET_RES;

/**
 *	@brief	RE PORT状態取得要求
 *	@note	Contents : read request for FHM-MT
 */
typedef struct
{
	UINT FunctionID;		/**< function ID */
	UINT SeqNo;				/**< sequence NO */
}MKT_FHMMT_REPORTSTATEGET_REQ;

/**
 *	@brief	RE PORT状態取得応答
 *	@note	Contents : read request for FHM-MT
 */
typedef struct
{
	CMT_TSKIF_HEAD head;	/**< task common head */
	UINT FunctionID;		/**< function ID */
	UINT SeqNo;				/**< sequence NO */
	UINT Result;			/**< OK/NG */
	UINT NGCode;			/**< NG Code(reserve) */
	UINT CpriNo[16];
	UINT RE_Bit[16];
}MKT_FHMMT_REPORTSTATEGET_RES;

/**
 *	@brief	RE PORT状態設定要求
 *	@note	Contents : set request for FHM-MT
 */
typedef struct
{
	UINT FunctionID;		/**< function ID */
	UINT SeqNo;				/**< sequence NO */
	UINT CpriNo[16];
	UINT RE_Bit[16];
}MKT_FHMMT_REPORTSTATESET_REQ;

/**
 *	@brief	RE PORT状態設定応答
 *	@note	Contents : set request for FHM-MT
 */
typedef struct
{
	CMT_TSKIF_HEAD head;	/**< task common head */
	UINT FunctionID;		/**< function ID */
	UINT SeqNo;				/**< sequence NO */
	UINT Result;			/**< OK/NG */
	UINT NGCode;			/**< NG Code(reserve) */
}MKT_FHMMT_REPORTSTATESET_RES;

/**
 *  @brief  RE-MT fiber output state read response
 *  @note  Contents : read response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID					*/
	UINT 	SeqNo;			/**<  Sequence NO					*/
	UINT 	Result;			/**<  OK/NG							*/
	UINT 	NGCode;			/**<  NG Code(reserve)				*/
	UINT 	State;			/**<  0: output off  1: output on	*/
}MKT_REMTFIBEROUTPUTGETRES;

/**
 *  @brief  RE-MT fiber output state read response interface structure
 *  @note  Contents : RE-MT fiber output state read response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;							/**<  task common head				*/
	MKT_REMTFIBEROUTPUTGETRES	FiberOutputGetRes;		/**<  fiber output state read response */
}MKT_TCPIF_REMTFIBEROUTPUTGETRES;

/**
 *  @brief  RE-MT fiber output state set request
 *  @note  Contents : set request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID						*/
	UINT 	SeqNo;			/**<  Sequence NO						*/
	UINT 	State;			/**<  0: output off  1: output on		*/
}MKT_REMTFIBEROUTPUTSETREQ;

/**
 *  @brief  RE-MT fiber output state set request interface structure
 *  @note  Contents : RE-MT fiber output state set request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;					/**<  task common head				*/
	MKT_REMTFIBEROUTPUTSETREQ	FiberOutputSetReq;	/**<  fiber output state set request */
}MKT_TCPIF_REMTFIBEROUTPUTSETREQ;

/**
 *  @brief  RE-MT fiber output state set response
 *  @note  Contents : set response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG						*/
	UINT 	NGCode;			/**<  NG Code(reserve)			*/
}MKT_REMTFIBEROUTPUTSETRES;

/**
 *  @brief  RE-MT fiber output state set response interface structure
 *  @note  Contents : RE-MT fiber output state set response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD				head;				/**<  task common head				*/
	MKT_REMTFIBEROUTPUTSETRES	FiberOutputSetRes;	/**<  fiber output state set response */
}MKT_TCPIF_REMTFIBEROUTPUTSETRES;

/**
 *  @brief  RE-MT power adjust control request
 *  @note  Contents : control request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Branch;			/**<  0~3						*/
	UINT 	State;			/**<  0: up  1: down			*/
}MKT_REMTPOWADJREQ;


/**
 *  @brief  RE-MT power adjust control request interface structure
 *  @note  Contents : RE-MT power adjust control request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;			/**<  task common head			*/
	MKT_REMTPOWADJREQ		PowAdjReq;		/**<  power adjust control request */
}MKT_TCPIF_REMTPOWADJREQ;

/**
 *  @brief  RE-MT power adjust control response
 *  @note  Contents : read response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG						*/
	UINT 	NGCode;			/**<  NG Code(reserve)			*/
}MKT_REMTPOWADJRES;

/**
 *  @brief  RE-MT power adjust control response interface structure
 *  @note  Contents : RE-MT power adjust control response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD		head;				/**<  task common head		*/
	MKT_REMTPOWADJRES	PowAdjReqRes;		/**<  power adjust control response */
}MKT_TCPIF_REMTPOWADJRES;


/**
 *  @brief  RE-MT power save control request
 *  @note  Contents : control request for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
}MKT_REMTPOWSAVREQ;


/**
 *  @brief  RE-MT power save control request interface structure
 *  @note  Contents : RE-MT power save control request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;			/**<  task common head			*/
	MKT_REMTPOWSAVREQ		PowSaveReq;		/**<  power save control request */
}MKT_TCPIF_REMTPOWSAVREQ;

/**
 *  @brief  RE-MT power save control response
 *  @note  Contents : set response for RE-MT
 *  
 * 
 */
typedef struct
{
	UINT	FunctionID;		/**<  Function ID				*/
	UINT 	SeqNo;			/**<  Sequence NO				*/
	UINT 	Result;			/**<  OK/NG						*/
	UINT 	NGCode;			/**<  NG Code(reserve)			*/
}MKT_REMTPOWSAVRES;

/**
 *  @brief  RE-MT power save control response interface structure
 *  @note  Contents : RE-MT power save control response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;			/**<  task common head		*/
	MKT_REMTPOWSAVRES		PowSaveRes;		/**<  power save control response */
}MKT_TCPIF_REMTPOWSAVRES;






/**
 *  @brief  RE-MT file download start request structure
 *  @note  Contents : RE-MT file download start request structure
 *  
 * 
 */

typedef struct
{
	UINT	FunctionID;		/**<  Function ID		*/
	UINT 	DLTarget;		/**<  Download target 	*/
	UINT 	FileType;		/**<  File Type		*/
	UINT 	FileSize;		/**<  File Size		*/
}MKT_REMTFLDLSTRREQ;

/**
 *  @brief  RE-MT file download start request interface structure
 *  @note  Contents : RE-MT file download start request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head				*/
	MKT_REMTFLDLSTRREQ		fldlstrreq;			/**<  RE-MT file download start request */
}MKT_TCPIF_REMTFLDLSTRREQ;


/**
 *  @brief  RE-MT file download start response structure
 *  @note  Contents : RE-MT file download start response structure
 *  
 * 
 */

typedef struct
{
	UINT	FunctionID;		/**<  Function ID	*/
	UINT 	Result;			/**<  OK/NG		*/
	UINT 	NGCode;			/**<  NG Code		*/
}MKT_REMTFLDLSTRRSP;

/**
 *  @brief  RE-MT file download start response interface structure
 *  @note  Contents : RE-MT file download start response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head				*/
	MKT_REMTFLDLSTRRSP		fldlstrrsp;			/**<  RE-MT file download start response */
}MKT_TCPIF_REMTFLDLSTRRSP;

/**
 *  @brief  RE-MT TRA file block sending structure
 *  @note  Contents : RE-MT TRA file block sending structure
 *  
 * 
 */

typedef struct
{
	UINT 	SeqNo;			/**<  Sequence NO	*/
	UINT 	Length;			/**<  Data Length	*/
	UCHAR 	Buffer;			/**<  Data Buffer	*/
}MKT_REMTTRAFLBLKSND;

/**
 *  @brief  RE-MT TRA file block sending interface structure
 *  @note  Contents : RE-MT TRA file block sending interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head			*/
	MKT_REMTTRAFLBLKSND		flblksnd;			/**<  RE-MT TRA file block sending */
}MKT_TCPIF_REMTTRAFLBLKSND;

/**
 *  @brief  RE-MT  file complete request structure
 *  @note  Contents : RE-MT  file complete request structure
 *  
 * 
 */

typedef struct
{
	UINT	FunctionID;		/**<  Function ID			*/
	USHORT	FileType;		/**<  File Type			*/
	USHORT	EndType;		/**<  End Type				*/
	UINT 	CheckSum;		/**<  Check Sum			*/
	UINT 	Flag;			/**<  Reset/Reconfig Flag	*/
}MKT_REMTFLDLCMPREQ;

/**
 *  @brief  RE-MT  file complete request interface structure
 *  @note  Contents : RE-MT  file complete request interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head				*/
	MKT_REMTFLDLCMPREQ		fldlcmpreq;			/**<  RE-MT file complete request   */
}MKT_TCPIF_REMTFLDLCMPREQ;

/**
 *  @brief  RE-MT  file complete response structure
 *  @note  Contents : RE-MT  file complete response structure
 *  
 * 
 */

typedef struct
{
	UINT	FunctionID;		/**<  Function ID	*/
	UINT 	Result;			/**<  OK/NG		*/
	UINT 	NGCode;			/**<  NG Code		*/
}MKT_REMTFLDLCMPRSP;

/**
 *  @brief  RE-MT  file complete response interface structure
 *  @note  Contents : RE-MT  file complete response interface structure
 *  
 * 
 */
typedef struct
{
	CMT_TSKIF_HEAD			head;				/**<  task common head				*/
	MKT_REMTFLDLCMPRSP		fldlcmprsp;			/**<  RE-MT file complete response  */
}MKT_TCPIF_REMTFLDLCMPRSP;


/**
 *  @brief  file complete response table structure
 *  @note  Contents :  file complete response table structure
 *  
 * 
 */

typedef struct
{
	UINT		en_flg;							/**<  EN flag                   */
	UINT		file_type;						/**<  file type 				*/
	UINT		div_no;							/**<  divide number             */
	UINT		dl_target;						/**<  download target			*/
} MKT_FILEINFREP_TBL;

/**
 *  @brief  download manage table structure
 *  @note  Contents :  download manage table structure
 *  
 * 
 */

typedef struct {
	UINT		total_size;						/**<  total size                */
	UINT		rcvdata_size;					/**<  received data size        */
	UINT		div_no;							/**<  divide number             */
} MKT_DLMNG_TBL;


#endif	/**< M_MK_TYP*/

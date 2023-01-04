/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_com_plog.h
 *  @brief  BPF common packet log header
 *  @date   2014/03/28 FJT)Taniguchi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2014
 */
/********************************************************************************************************************/
#ifndef bpf_com_plog_h
#define bpf_com_plog_h

/********************************************************************************************************************/
/* Internal Definition                                                                                              */
/********************************************************************************************************************/
#define D_BPF_PLOG_AST_TEXT		180						/* assert text */
#define D_BPF_PLOG_TRACE_FILE	24						/* trace log file name length */
#define D_BPF_PLOG_TRACE_TEXT	156						/* trace log text */
#define D_BPF_PLOG_COM_DATA		184						/* com log */

/********************************************************************************************************************/
/* Internal Structure                                                                                               */
/********************************************************************************************************************/
typedef	struct{
	unsigned long	id;									/* semid addr    */
	unsigned int	val1;								/* option    */
	unsigned int	val2;								/* option    */
}
t_log_sem;

typedef	struct{
	unsigned int	id;									/* timer id    */
	unsigned int	id_bpf;								/* timer id for BPF */
	unsigned int	id_os;								/* timer id for OS */
	unsigned int	value;								/* time value */
}t_log_tim;

typedef	struct{
	unsigned long	id;									/* timer id    */
	unsigned int	latency;							/* latency */
	unsigned int	period;								/* period */
}t_log_tim2;


typedef	struct{
	unsigned int	id;									/* keyid    */
	unsigned int	shmid;								/* shm id   */
	unsigned int	addr;								/* buf addr */
	unsigned int	size;								/* buf size */
}t_log_buf;

typedef	struct{
	unsigned int addr;									/* apd addr */
	unsigned int data;									/* apd data */
	unsigned int kind;									/* apd bus or apd buf */
	unsigned int brno;									/* apd branch no */
}t_log_apd;

typedef	struct{
	unsigned int addr;									/* reg addr */
	unsigned int data;									/* reg data */
}t_log_reg;

typedef	struct{
	unsigned int addr;									/* spi addr */
	unsigned int data;									/* spi data */
	unsigned int no;									/* spi no */
	unsigned int ssno;									/* spi ss */
}t_log_spi;

typedef	struct{
	unsigned int addr;									/* i2c addr */
	unsigned int data;									/* i2c data */
	unsigned int no;									/* i2c no   */
	unsigned int ssno;									/* i2c ss   */
}t_log_i2c;


typedef	struct{
	unsigned int addr;									/* qspi addr */
	unsigned int size;									/* qspi data */
	unsigned int no;									/* qspi no   */
	unsigned int face;									/* qspi face  */
}t_log_qspi;

typedef	struct{
	unsigned int addr;									/* vupu flash addr */
	unsigned int size;									/* vupu flash size */
	unsigned int no;									/* vupu flash branch no   */
	unsigned int face;									/* vupu flash face  */
}t_log_vflsh;

typedef	struct{
	unsigned int	code;								/* code */
	char			text[D_BPF_PLOG_AST_TEXT];			/* text */
}t_log_ast;

typedef	struct{
	char			file[D_BPF_PLOG_TRACE_FILE];		/* filename */
	int				line;								/* line */
	char			text[D_BPF_PLOG_TRACE_TEXT];		/* text */
}t_log_trace;

typedef	struct{
	char			data[D_BPF_PLOG_COM_DATA];			/* simple data		 */
}t_log_com;

typedef struct {
	char				magicno[4];		/* magicno */
	int					len;			/* info length */
	struct timespec		tcount;			/* bootup time */
	union {
		t_log_vflsh		vflsh;			/* flash log */
		t_log_sem		sem;			/* sem log    */
		t_log_tim		tim;			/* timer log  */
		t_log_buf		buf;			/* buffer log */
		t_log_apd		apd;			/* apd log    */
		t_log_ast		ast;			/* assert log */
		t_log_reg		reg;			/* reg log */
		t_log_spi		spi;			/* spi log */
		t_log_i2c		i2c;			/* i2c log */
		t_log_qspi		qspi;			/* qspi log */
		t_log_trace		trace;			/* trace log */
		t_log_com		com;			/* com data log */
		t_log_tim2		tim2;			/* tim2         */
	}info;
} t_bpf_com_plog_data;

typedef struct {
								/* pcap headr   16byte */
								/* ether headr  12byte */
	struct	ip			head;	/* ip heade r   20byte */
	char				dummy[4];
	t_bpf_com_plog_data	data;	/* user data   208byte */
} t_bpf_com_plog_msg;			/* total       256byte */

#endif

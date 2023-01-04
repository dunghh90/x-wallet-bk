/*!
 *  @skip	$Id:$
 *  @file	saa_main.c
 *  @brief	SAA inter-thread communication with buffers.
 *  @author	KCN) Nakai Hiroyuki
 *  @date	2018/11/22 KCN)Nakai create
 *  @date	2019/02/04 KCN)Nakai change
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */


/*!
 * @addtogroup	SAA
 * @brief		NETCONF sub-agent.
 * @note		Works with netconfd-pro
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <errno.h>

#include "procdefs.h"
#include "log.h"
#include "sil_sa.h"
#include "status_enum.h"
#include "ycontrol.h"
#include "db_api.h"

#include "f_mpmw_srv.h"
#include "f_mpsw_msg_ready_def.h"
#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_msg_controller.h"
#include "f_saa_ntf_lib.h"
#include "f_saa_ntf_send.h"

#define CONFIGFILE "/opt/agent/etc/DU_agent.conf"
#define MAX_STR_LENGTH 256
#define MAX_FILENAME 128
#define MAX_HOSTNAME 64
#define MAX_ADDRESS 64
#define MAX_DEVICE_TYPE 16
#define VENDOR_PARAM_NUM 2

#define THREAD_NUM 2
#define DELAY_MSEC 100

typedef struct {
	char data_file[MAX_FILENAME];
	char validation_file[MAX_FILENAME];
} T_SAA_INIT_CONFIG;


typedef struct {
	char servername[MAX_HOSTNAME];
	char subsysname[MAX_HOSTNAME];
	char addr_str[MAX_ADDRESS]; /* Yuma needs this as string */
	in_port_t port;
} T_SAA_YUMA_CONFIG;

typedef struct {
	int  delay_msec;
} T_SAA_MISC_CONFIG;

typedef struct {
	T_SAA_INIT_CONFIG init_conf;
	T_SAA_YUMA_CONFIG yuma_conf;
	T_SAA_MSG_CTRL_THREAD_ARG msg_conf;
	T_SAA_MISC_CONFIG misc_conf;
} T_SAA_SYS_CONFIG;

T_SAA_SYS_CONFIG g_sys_conf;

pthread_t saa_threads[THREAD_NUM];


static int check_parameters(int argc, char **argv, char *device_type, char *data_file2, int *argc_yuma, char ***argv_yuma);
static void print_usage(void);
static int get_config(const char *filename, T_SAA_SYS_CONFIG *sys_conf);
static int initialize(T_SAA_INIT_CONFIG *init_conf, char *data_type, char *data_file2);
static char * get_value(char *str);
static int create_threads(T_SAA_MSG_CTRL_THREAD_ARG *msg_conf);
static void cleanup_threads(void);
static int setup_yuma(int argc_yuma, char **argv_yuma, T_SAA_YUMA_CONFIG *yuma_conf);
static status_t event_loop(void);
static int send_ready(void);

/*!
 * @brief	main
 * @note	argc, argvはycontrol_initに引き渡される。
 * @param	argc 起動パラメータ数
 * @param	argv 起動パラメータリスト
 * @return	0:正常終了、0以外:以上終了
 * @date	2018/11/16 KCN)Nakai create
 * @date	2019/02/04 KCN)Nakai change
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
int main (int argc, char **argv){
	int rc = 0;
	char device_type[MAX_DEVICE_TYPE];
        char data_file2[MAX_FILENAME];
	int argc_yuma = 0;
	char **argv_yuma = NULL;

	memset(device_type, 0, sizeof(device_type));
	memset(data_file2, 0, sizeof(data_file2));


	rc = check_parameters(argc, argv, device_type, data_file2, &argc_yuma, &argv_yuma);
	if(rc != 0){
		print_usage();
		return 2;
	}

	printf("DU_agent: started.\n");

	rc = get_config(CONFIGFILE, &g_sys_conf);
	if(rc != 0){
		return 1;
	}
	
	printf("DU_agent: got configuration.\n");

	rc = initialize(&g_sys_conf.init_conf, device_type, data_file2);
	if(rc != 0){
		return rc;
	}

	printf("DU_agent: loaded initialization data.\n");

	rc = setup_yuma(argc_yuma, argv_yuma, &g_sys_conf.yuma_conf);
	if(rc != 0){
		return rc;
	}
	
	log_info("\nDU_agent: setup Yuma-API done.");

	rc = create_threads(&g_sys_conf.msg_conf);
	if(rc != 0){
		return rc;
	}

	log_info("\nDU_agent: started all threads.");

	rc = send_ready();
	if(rc != 0){
		return rc;
	}

	log_info("\nDU_agent: sent 'READY' to device.");

	event_loop();
	cleanup_threads();

	if(argv_yuma != NULL){
		free(argv_yuma);
	}

	log_info("\nDU_agent: exits.");
	
	return rc;
}


/*!
 * @brief	起動パラメータを検査する。
 * @note	第一パラメータは機種識別子。以降はycontrol_init()に渡す。
 * @param	argc パラメータ数
 * @param	argv パラメータのリスト
 * @param	device_type  機種識別子用のバッファ
 * @param    	data_file2 追加設定ファイル
 * @param	argc_yuma Yuma APIに渡す引数の数
 * @param	argv_yuma Yuma APIに渡す引数のリスト
 * @return	0: 正常, -1:異常
 * @date	2018/11/29 KCN)Nakai create
 * @date	2019/02/04 KCN)Nakai change
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static int check_parameters(int argc, char **argv, char *device_type, char *data_file2, int *argc_yuma, char ***argv_yuma){

	int rc = 0;
	int i = 0;
	int j = 0;
	char **argv_new;

	if (argc < (VENDOR_PARAM_NUM + 1)){
		rc = -1;
	}
	else{
		strncpy(device_type, argv[1], MAX_DEVICE_TYPE);
		printf("DU_agent: devie type = %s\n", device_type);
		strncpy(data_file2, argv[2], MAX_FILENAME);
		printf("DU_agent: data file 2 = %s\n", data_file2);
		argv_new = malloc(sizeof(char *) * (argc - VENDOR_PARAM_NUM)); /* pgr0524 */
		if(argv_new == NULL){
			rc = -1;
		}
		else{
			*argc_yuma = argc - VENDOR_PARAM_NUM;
			argv_new[0] = argv[0];
			for(i=VENDOR_PARAM_NUM + 1; i< argc; i++){
				j = i - VENDOR_PARAM_NUM;
				argv_new[j] = argv[i];
				printf("DU_agent: Yuma param: %s\n", argv_new[j]);
			}
			*argv_yuma = argv_new;
			rc =  0;
		}
	}

	return rc; /* pgr0524 */
}

/*!
 * @brief	起動パラメータの説明を出力する。
 * @note	パラメータが不正な場合に使用する。
 * @param	なし
 * @return	なし
 * @date	2018/11/29 KCN)Nakai create
 * @date	2019/02/04 KCN)Nakai change
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static void print_usage(void){
	fprintf(stderr, "Usage: DU_agent device_type data_file2 [yuma options ...]\n");
}

/*!
 * @brief	本プロセスのスレッドを全て生成する。
 * @note	現在notification用のスレッドは生成しない。必要になったときに追加すること。
 * @param	msg_conf メッセージ送信および受信スレッドで使用する設定情報
 * @return	0: 正常, -1:異常
 * @date	2018/11/22 KCN)Nakai create
 * @date	2019/02/04 KCN)Nakai change
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static int create_threads(T_SAA_MSG_CTRL_THREAD_ARG *msg_conf){
	int rc = 0;
	int irc = 0;
	int i = 0;
	
	void *(*func[])(void *) = {f_saa_msg_receiver, f_saa_msg_sender};

	saa_threads[0] = saa_threads[1] = (pthread_t)0;

	for(i=0;i<THREAD_NUM;i++){
		irc = pthread_create(&saa_threads[i], NULL, func[i], msg_conf);
		if(irc != 0){
			log_dev0("\nDU_agent: pthread_create failed. i=%d, errno=%d", i, irc);
			rc = -1;
			break;
		}
	}
	
	return rc;
}

/*!
 * @brief	スレッド終了処理を行う。
 * @note	特になし
 * @param	なし
 * @return	0: 正常, -1:異常
 * @date	2018/11/22 KCN)Nakai create
 * @date	2019/02/04 KCN)Nakai change
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static void cleanup_threads(void){
	int rc = 0;
	int i = 0;
	
	for(i=0;i<THREAD_NUM;i++){
		if(saa_threads[i] != 0){
			rc = pthread_cancel(saa_threads[i]);
			if(rc != 0){
				log_dev0("\nDU_agent:pthread_cancel. i=%d, errno=%d", i, rc);
			}
			rc = pthread_join(saa_threads[i], NULL);
			if(rc != 0){
				log_dev0("\nDU_agent:pthread_join. i=%d, errno=%d", i, rc);
			}
		}
	}
}
		
/*!
 * @brief	YumaWorksのプログラムを利用するための設定を行う。
 * @note	netconfd-proとの接続等を行う。
 * @param	argc パラメータ数
 * @param	argv パラメータのリスト
 * @param	yuma_conf 設定情報
 * @return	0: 正常, -1:異常
 * @date	2018/11/22 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static int setup_yuma(int argc_yuma, char **argv_yuma, T_SAA_YUMA_CONFIG *yuma_conf){
	int rc = 0;

	/* 1) setup yumapro messaging service profile */
	status_t res = ycontrol_init(argc_yuma, argv_yuma,(const xmlChar *)yuma_conf->subsysname);
	if (res != NO_ERR) {
		fprintf(stderr, "DU_agent:ycontrol_init failed. res=%d\n", res);
		return -1;
	}

	/* 2) register vendor log */
	MPMW_SRV_LOG_INIT();

	/* 3) register services with the control layer */
	res = sil_sa_register_service();
	if (res != NO_ERR) {
		log_dev0("\nDU_agent:sil_sa_register_service failed. res=%d", res);
		return -1;
	}

	/* 4) do 2nd stage init of the control manager (connect to server) */
	if (yuma_conf->addr_str[0] != 0) {
		if (yuma_conf->port == 0) {
			yuma_conf->port = htons(2023);
		}
		log_info("\nDU_agent: use ycontrol_init2_ha(%s, %s, %d)",
			yuma_conf->servername, yuma_conf->addr_str, ntohs(yuma_conf->port));
		res = ycontrol_init2_ha(yuma_conf->servername, (const char *)yuma_conf->addr_str, yuma_conf->port);
	}
	else {
		log_info("\nDU_agent: use ycontrol_init2");
		res = ycontrol_init2();
	}

	if(res == NO_ERR){
		rc = 0;
	}
	else{
		log_dev0("\nDU_agent:ycontrol_init2 or ycontrol_init2_ha failed. res=%d", res);
		rc = -1;
	}

	return rc;
}

/*!
 * @brief	netconfd-proからのイベントを待つ。
 * @note	shutdownが実行されたら終了する。
 * @param	なし
 * @return	0: 正常, -1:異常
 * @date	2018/11/22 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static status_t event_loop(void){
	
	boolean done = FALSE;
	status_t res = NO_ERR;
	struct timespec req;

	req.tv_sec = 0;
	req.tv_nsec = (time_t)(DELAY_MSEC * 1000);
	
	/* 4) call ycontrol_check_io periodically from the main program
	 * control loop
	 */

	log_info("\nDU_agent: start event-loop.");
	while (!done && res == NO_ERR) {
	    res = ycontrol_check_io();

	    if (ycontrol_shutdown_now()) {
		log_info("\nDU_agent: ycontrol shutdown.");
	        done = TRUE;
	    }
            else{
                f_saa_ntf_send();
            }

	    // Using sleep to represent other program work; remove for real <- do I need this now?
	    if (!done && res == NO_ERR) {
	        nanosleep(&req, 0);
	    }

	}

	/* 5) cleanup the control layer before exit */
	ycontrol_cleanup();

	return (int)res;

}

/*!
 * @brief	プラットフォームに対して準備完了を通知する。
 * @note	応答は待たない。
 * @param	なし
 * @return	0: 正常, -1:異常
 * @date	2018/11/22 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static int send_ready(void){
	int rc = 0;
	int request_id = 0;

	request_id = f_saa_com_add_request(D_MSG_MSI_READY_IND, 0, 0);
	if(request_id == 0){
		log_dev0("\nDU_agent: send 'READY' failed.");
		rc = -1;
	}

	return rc;
}

/*!
 * @brief	本プロセスの初期化処理を行う。
 * @note	SIL-SAで使用する初期値、検査条件を読み込む。
 * @param	init_conf 初期値、検査条件設定情報
 * @param	device_type  機種識別子
 * @param	data_file2  追加設定ファイル
 * @return	0: 正常, -1:異常
 * @date	2018/11/22 KCN)Nakai create
 * @date	2019/02/04 KCN)Nakai change
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static int initialize(T_SAA_INIT_CONFIG *init_conf, char *data_type, char *data_file2){
	int rc = 0;

	char data_file[MAX_FILENAME + MAX_DEVICE_TYPE + 1];
	char validation_file[MAX_FILENAME + MAX_DEVICE_TYPE + 1];

	memset(data_file, 0, sizeof(data_file));
	memset(validation_file, 0, sizeof(validation_file));

        f_saa_com_init_notification_table();

	sprintf(data_file, "%s.%s", init_conf->data_file, data_type);
	sprintf(validation_file, "%s.%s", init_conf->validation_file, data_type);

	fprintf(stderr, "DU_agent: initialize data_file=%s\n", data_file);
	fprintf(stderr, "DU_agent: initialize data_file2=%s\n", data_file2);
	fprintf(stderr, "DU_agent: initialize validation_file=%s\n", validation_file);
	rc = f_saa_com_load_init_data(data_file, data_file2, validation_file);
	if(rc != 0){
		fprintf(stderr, "DU_agent:cannot load initialization data.\n");
	}

	return rc;
}

/*!
 * @brief	本プロセスの設定情報を読み込む。
 * @note	特になし
 * @param	sys_conf 設定情報書き込み先
 * @return	0: 正常, -1:異常
 * @date	2018/11/22 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static int get_config(const char *filename, T_SAA_SYS_CONFIG *sys_conf){
	int rc = 0;
	FILE *fd;
	const char *key;
	char buffer[MAX_STR_LENGTH];
	in_port_t host_order_port = 0;
	int tmpi = 0;
	

	printf("DU_agent: start with config [%s].\n", filename);

	memset(buffer, 0, sizeof(buffer));
	memset(sys_conf, 0, sizeof(T_SAA_SYS_CONFIG ));

	sys_conf->misc_conf.delay_msec = DELAY_MSEC;

	fd = fopen(filename, "r");
	if(fd == NULL){
		fprintf(stderr, "DU_agent: cannot open [%s]. errno=%d\n", filename, errno);
		return -1;
	}

	while(1){
		if(fgets(buffer, MAX_STR_LENGTH, fd) == NULL){
			break;
		}

		key = "misc.delay_msec";
		if(strncmp(buffer, key, strlen(key)) == 0){
			tmpi = atoi(buffer);
			if(tmpi != 0){
				sys_conf->misc_conf.delay_msec = tmpi;
			}
			continue;
		}

		key = "init.data_file";
		if(strncmp(buffer, key, strlen(key)) == 0){
			strcpy(sys_conf->init_conf.data_file, get_value(buffer));
			continue;
		}

		key = "init.validation_file";
		if(strncmp(buffer, key, strlen(key)) == 0){
			strcpy(sys_conf->init_conf.validation_file, get_value(buffer));
			continue;
		}

		key = "yuma.addr";
		if(strncmp(buffer, key, strlen(key)) == 0){
			//must be string.
			strcpy(sys_conf->yuma_conf.addr_str, get_value(buffer));
			continue;
		}

		key = "yuma.port";
		if(strncmp(buffer, key, strlen(key)) == 0){
			host_order_port = atoi(get_value(buffer));
			sys_conf->yuma_conf.port = htons(host_order_port);
			continue;
		}
			
		key = "yuma.servername";
		if(strncmp(buffer, key, strlen(key)) == 0){
			strcpy(sys_conf->yuma_conf.servername, get_value(buffer));
			continue;
		}
			
		key = "yuma.subsysname";
		if(strncmp(buffer, key, strlen(key)) == 0){
			strcpy(sys_conf->yuma_conf.subsysname, get_value(buffer));
			continue;
		}

		key = "msgcontroll.src.addr";
		if(strncmp(buffer, key, strlen(key)) == 0){
			sys_conf->msg_conf.src_addr = inet_addr(get_value(buffer));
			continue;
		}

		key = "msgcontroll.src.port";
		if(strncmp(buffer, key, strlen(key)) == 0){
			host_order_port = atoi(get_value(buffer));
			sys_conf->msg_conf.src_port = htons(host_order_port); 
			continue;
		}
			
		key = "msgcontroll.dst.addr";
		if(strncmp(buffer, key, strlen(key)) == 0){
			sys_conf->msg_conf.dst_addr = inet_addr(get_value(buffer));
			continue;
		}

		key = "msgcontroll.dst.port";
		if(strncmp(buffer, key, strlen(key)) == 0){
			host_order_port = atoi(get_value(buffer));
			sys_conf->msg_conf.dst_port = htons(host_order_port);
			continue;
		}

	}

	fclose(fd);

	return rc;
}

/*!
 * @brief	"key=value"形式の文字列から"value"のみを取り出す。
 * @note	returnで返されるのは関数内のstatic変数のポインタである。
 * @param	str "key=value"形式の文字列
 * @return	"value"部分のアドレス
 * @date	2018/11/22 KCN)Nakai create
 * @warning	N/A
 * @FeatureID	N/A
 * @Bug_No	N/A
 * @CR_No	N/A
 * @TBD_No	N/A
 */
static char * get_value(char *str){
	static char buffer[MAX_STR_LENGTH];
	int i = 0;
	int j = 0;

	while (str[i++] != '='){
		continue;
	}
	while(str[i] != '\n' && i < MAX_STR_LENGTH){
		buffer[j++] = str[i++];
	}
	buffer[j] = '\0';

	return buffer;
}


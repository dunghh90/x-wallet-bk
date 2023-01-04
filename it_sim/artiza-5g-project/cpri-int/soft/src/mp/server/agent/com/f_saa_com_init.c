/*!
 * @skip        $ld:$
 * @file        f_saa_com_init.c
 * @brief       SAA_COM SIL-callback parameter get
 * @date        2018/11/19 KCN)Wakamizu Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup    SAA_COM
 * @brief
 * @note
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/mman.h>

#include "f_saa_com_init.h"

#define MAX_MODULES   40             /* モジュール最大数 */
#define MAX_RECORDS 5120             /* モジュール内のレコード最大数 */

/* 以下の文字列長は終端の'\0'を含まない */
#define MAX_MODULE_NAME_LENGTH 30              /* モジュール名最大長 */
#define MAX_KEY_LENGTH        192              /* キー最大長 */
#define MAX_VALUE_LENGTH (D_PARAM_NUM_MAX - 1) /* 値最大長 */

typedef struct {
    char *key;
    char *value;
} RECORD;

typedef struct {
    char *module;
    RECORD record[MAX_RECORDS];
} MODULE_DATA;

MODULE_DATA sys_init_table[MAX_MODULES];    /* 初期値テーブル */
MODULE_DATA sys_init_table2[MAX_MODULES];   /* 初期値テーブル2 */
MODULE_DATA sys_valid_table[MAX_MODULES];   /* 検査値テーブル */


static int load_file(const char*, MODULE_DATA *table);
static char *get_parameter(MODULE_DATA *tbl, char *module_name, char *parameter_name);
static int str_to_list(char *src, const char *delim, int max, char **list);

/****************************************************************************/
/*!
 * @brief       f_saa_com_load_init_data
 * @note        init用ファイルとvalidate用ファイルを開き、メモリに保存する
 * @param       init_file           char * init初期設定ファイルフルパス
 * @param       init_file2          char * init初期設定ファイル2フルパス
 * @param       valid_file          char * validate初期設定ファイルフルパス
 * @retval      D_SYS_OK                 0:正常終了
 * @retval      D_SYS_NG                -1:異常終了
 * @return      INT                     実行結果
 * @warning     N/A
 * @FeatureID   -
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date        2018/11/19 KCN)Wakamizu Create
 * @date        2019/02/04 KCN)Nakai change
 */
/****************************************************************************/
int f_saa_com_load_init_data(char* init_file, char *init_file2, char* valid_file)
{
    printf("DU_agent: Load file: start.\n");

    int rc = D_SYS_OK;
    int irc = 0;

    irc = load_file(init_file, sys_init_table);
    if(irc != 0){
        rc = D_SYS_NG;
    }
    else {
        irc = load_file(init_file2, sys_init_table2);
        if(irc != 0){
            rc = D_SYS_NG;
        }
        else {
            irc = load_file(valid_file, sys_valid_table);
            if(irc != 0){
                rc = D_SYS_NG;
            }
        }
    }

    printf("DU_agent: Load file: end.\n");
    return rc;

}


/****************************************************************************/
/*!
 * @brief       load_file
 * @note        指定されたファイルを開き、メモリに保存する(内部関数)
 * @param       data_file char *  init初期設定ファイルフルパス
 * @param       table     T_SAA_COM_INIT_VALIDATE_TBL*    保存先テーブル
 * @retval      0:正常終了
 * @retval     -1:異常終了
 * @return      INT          実行結果
 * @warning     N/A
 * @FeatureID   -
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date        2018/11/19 KCN)Wakamizu Create
 */
/****************************************************************************/
static int load_file(const char *data_file, MODULE_DATA *table) {
    int rc = 0;
    char buffer[MAX_KEY_LENGTH + 1 + MAX_VALUE_LENGTH + 1]; /* key=value\0 */
    char module_wk[MAX_MODULE_NAME_LENGTH + 1];
    char key_wk[MAX_KEY_LENGTH + 1];
    char value_wk[MAX_VALUE_LENGTH + 1];
    char *list[2];
    char *module_p = NULL;
    char *key_p = NULL;
    char *value_p = NULL;
    int line_len = 0;
    int m_len = 0;
    int k_len = 0;
    int v_len = 0;
    FILE* fp = NULL;
    int module_idx = -1;
    int record_idx = -1;
    int num = 0;

    printf("DU_agent: Load file: file = %s\n start.",data_file);

    memset(buffer, 0, sizeof(buffer));
    memset(module_wk, 0, sizeof(module_wk));
    memset(key_wk, 0, sizeof(key_wk));
    memset(value_wk, 0, sizeof(value_wk));
    memset(table, 0, sizeof(MODULE_DATA) * MAX_MODULES); 

    list[0] = key_wk;
    list[1] = value_wk;

    fp = fopen(data_file,"r");
    if(fp == NULL) {
        fprintf(stderr, "DU_agent: Load file: cannot open file = %s\n",data_file);
        return -1; 
    }

    while(fgets(buffer, sizeof(buffer), fp) !=NULL) {
        line_len = strlen(buffer);

        if(buffer[line_len -1] == '\n'){
            buffer[line_len -1] = 0;
        }

        /* ------------ コメント行をスキップ ----------- */
        if(buffer[0] == 0){
            continue;
        }
        if(buffer[0] == '#'){
            continue;
        }

        /* ------------ モジュール名の取り出し ----------- */
        line_len = strlen(buffer);
        if (buffer[0] == '[' && buffer[line_len -1] == ']'){

            module_idx += 1;
            record_idx  = -1;

            if( module_idx > (MAX_MODULES - 1)) {
                fprintf(stderr, "DU_agent: Load file: too many modules.\n");
                rc = -1;
                break;
            }

            memset(module_wk, 0, sizeof(module_wk));
            strncpy(module_wk, buffer + 1, line_len - 2);
            if(strlen(module_wk) > MAX_MODULE_NAME_LENGTH){
                fprintf(stderr, "DU_agent: Load file: module name too long. (%s)\n", module_wk);
                rc = -1;
                break;
            } 

            m_len = strlen(module_wk) + 1;
            module_p = malloc(m_len);
            if(module_p == NULL){
                fprintf(stderr, "DU_agent: Load file: malloc() failed. (module_name)\n");
                rc = -1;
                break;
            }

            memset(module_p, 0, m_len);
            strcpy(module_p, module_wk);
            table[module_idx].module = module_p;

            continue;
        }
        else if(module_idx < 0){
            fprintf(stderr, "DU_agent: Load file: need module name first. (%s)\n", buffer);
            rc = -1;
            break;
        }

        /* ------------ モジュール毎定義の取り出し ----------- */

        record_idx++;

        if( record_idx > (MAX_RECORDS - 1)) {
            fprintf(stderr, "DU_agent: Load file: too many records to module(%s).\n", module_p);
            rc = -1;
            break;
        }

        num = str_to_list(buffer, "=", 2, list);
        if( num != 2){
            if(num == 1){
                strcpy(list[1],"");
            }
            else{
                fprintf(stderr, "DU_agent: Load file: wrong key-value format. (%s)\n", buffer);
                rc = -1;
                break;
            }
        }

        if(strlen(key_wk) > MAX_KEY_LENGTH){
            fprintf(stderr, "DU_agent: Load file: key is too long. (%s)\n", key_wk);
            rc = -1;
            break;
        }
        if(strlen(value_wk) > MAX_VALUE_LENGTH){
            fprintf(stderr, "DU_agent: Load file: value is too long. (%s)\n", value_wk);
            rc = -1;
            break;
        }

        k_len = strlen(key_wk) + 1;
        v_len = strlen(value_wk) + 1;
        key_p = malloc(k_len);
        value_p = malloc(v_len);
        if(key_p == NULL || value_p == NULL){
            fprintf(stderr, "DU_agent: Load file: malloc failed. (key, value)\n");
            rc = -1;
            break;
        }

        memset(key_p, 0, k_len);
        memset(value_p, 0, v_len);
        strcpy(key_p, key_wk);
        strcpy(value_p, value_wk);
        table[module_idx].record[record_idx].key = key_p;
        table[module_idx].record[record_idx].value = value_p;

    } /* end of while */

    fclose(fp);
    printf("DU_agent: Load file: file = %s end.\n", data_file);

    return rc; /* pgr0524 */

}

/****************************************************************************/
/*!
 * @brief       f_saa_com_dump_init_data
 * @note        読み込んだデータをダンプする。
 * @param       なし
 * @return      なし
 * @warning     N/A
 * @FeatureID   -
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date        2018/12/10 Nakai Create
 */
/****************************************************************************/
void f_saa_com_dump_init_data(void){
    MODULE_DATA *tbl_p[] = {sys_init_table, sys_init_table2, sys_valid_table};
    int t = 0;
    int m = 0;
    int r = 0;

    for(t=0; t<3; t++){
        printf("TABLE:%d\n", t);
        for(m=0; m<MAX_MODULES ;m++){
            if(tbl_p[t][m].module != 0){
                printf("[%s]\n", tbl_p[t][m].module);
                for(r=0; r<MAX_RECORDS ;r++){
                    if(tbl_p[t][m].record[r].key != 0){
                        printf("%s=%s\n", tbl_p[t][m].record[r].key, tbl_p[t][m].record[r].value);
                    }
                    else{
                        break;
                    }
                }
            }
            else{
                break;
            }
        }
    }
}
/****************************************************************************/
/*!
 * @brief       f_saa_com_get_init_data
 * @note        メモリ上からinitに利用する初期値を文字列で返す
 * @param       module_name        char *  モジュール名
 * @param       parameter_name     char *  パラメータ名
 * @retval      NULL               char *  異常終了
 * @return      char *             実行結果
 * @warning     N/A
 * @FeatureID   -
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date        2018/11/19 KCN)Wakamizu Create
 * @date        2018/12/10 KCN)Nakai
 */
/****************************************************************************/
char * f_saa_com_get_init_data(char *module_name, char *parameter_name)
{
    char *p = NULL;

    p = get_parameter(sys_init_table, module_name, parameter_name);
    if(p == NULL){
        p = get_parameter(sys_init_table2, module_name, parameter_name);
    }

    return p;
}

/****************************************************************************/
/*!
 * @brief       f_saa_com_get_validate_data
 * @note        メモリ上からinitに利用する初期値を文字列で返す
 * @param       module_name        char *  モジュール名
 * @param       parameter_name     char *  パラメータ名
 * @retval      NULL               char *  異常終了
 * @return      char *             実行結果
 * @warning     N/A
 * @FeatureID   -
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date        2018/11/19 KCN)Wakamizu Create
 * @date        2018/12/10 KCN)Nakai change
 * @date        2019/02/04 KCN)Nakai change
 */
/****************************************************************************/
char * f_saa_com_get_validate_data(char *module_name, char *parameter_name)
{
    return get_parameter(sys_valid_table, module_name, parameter_name);
}

/****************************************************************************/
/*!
 * @brief       get_parameter
 * @note        内部テーブルからパラメータの値を取りだす。
 * @param       table           T_SAA_COM_INIT_VALIDATE_TBL * 対象テーブル
 * @param       module_name     char *  モジュール名
 * @param       parameter_name  char *  パラメータ名
 * @retval      ret             char *  取得した文字列のアドレス
 * @retval      NULL            char *  異常終了
 * @return      char *          実行結果
 * @warning     N/A
 * @FeatureID   -
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date        2018/11/19 KCN)Wakamizu Create
 * @date        2018/12/10 KCN)Nakai
 */
/****************************************************************************/
static char * get_parameter(MODULE_DATA *tbl, char *module_name, char *parameter_name)
{
    int module_idx  = 0;
    int record_idx = 0;
    char *param = NULL;
    int found = 0;

    /* モジュール名を検索 */
    for(module_idx=0; tbl[module_idx].module != NULL && module_idx <= MAX_MODULES; module_idx++){
        if(strcmp(tbl[module_idx].module, module_name) ==0){
            found = 1;
            break;
        }
    }

    if(found == 0){
        return NULL;
    }

    /* 対象のモジュール名とパラメータ名から要求されているパラメータを特定 */
    for(record_idx=0; tbl[module_idx].record[record_idx].key != NULL && record_idx < MAX_RECORDS; record_idx++){
        if(strcmp(tbl[module_idx].record[record_idx].key, parameter_name) == 0){
            param = tbl[module_idx].record[record_idx].value;
            break;
        }

    }


    return param;

}

/****************************************************************************/
/*!
 * @brief       f_saa_com_conversion_validate_data_max_min
 * @note        文字列を最小値・最大値に変換して返す(int max & int min)
 * @param       src       char *  変換元文字列アドレス
 * @param       min       int64_t バッファポインタ(min)
 * @param       max       int64_t バッファポインタ(max)
 * @retval      D_SYS_OK  0:正常終了
 * @retval      D_SYS_NG -1:異常終了
 * @return      INT       実行結果
 * @warning     N/A
 * @FeatureID   -
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date        2018/11/19 KCN)Wakamizu Create
 * @date        2018/12/10 KCN)Nakai
 */
/****************************************************************************/
int f_saa_com_conversion_validate_data_max_min(char* src, int64_t *min, int64_t *max)
{
    char min_str[D_PARAM_NUM_MAX];
    char max_str[D_PARAM_NUM_MAX];
    char *list[2];
    int num = 0;
    int rc = 0;

    memset(min_str, 0, D_PARAM_NUM_MAX);
    memset(max_str, 0, D_PARAM_NUM_MAX);

    list[0] = min_str;
    list[1] = max_str;

    num = str_to_list(src, ":", 2, list);
    if( num == 2){
        *min = atoll(min_str);
        *max = atoll(max_str);
        rc = D_SYS_OK;
    }
    else{
        rc = D_SYS_NG;
    }

    return rc;

}

/****************************************************************************/
/*!
 * @brief       f_saa_com_conversion_validate_data_list
 * @note        文字列を','で分割したリストを返す(param_list)
 * @param       src            char *  変換元文字列アドレス
 * @param       list        T_SAA_COM_PARAM_LIST_TBL    値格納先バッファ
 * @retval      D_SYS_NG    -1                          異常終了
 * @return      int         ret   変換された要素数もしくはNG
 * @warning     N/A
 * @FeatureID    -
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date        2018/11/19 KCN)Wakamizu Create
 * @date        2018/12/10 KCN)Nakai
 */
/****************************************************************************/
int f_saa_com_conversion_validate_data_list(char* src, T_SAA_COM_PARAM_LIST_TBL* param_list)
{
    int rc = 0;
    int i = 0;
    char *list[D_SAA_COM_VALIDATE_MAX];

    for(i=0; i<D_SAA_COM_VALIDATE_MAX;i++){
        list[i] = param_list->list[i];
    }

    rc = str_to_list(src, ",", D_SAA_COM_VALIDATE_MAX, list);

    return rc;
}

/****************************************************************************/
/*!
 * @brief       devide_to_string
 * @note        文字列を指定した区切り文字で分割したリストを返す。
 * @param       src    char * 入力文字列
 * @param       delim  char * 区切り文字列
 * @param       max    int    最大数
 * @param       list char *** char[][]のリストへのポインタ
 * @return      int  リストの要素数
 * @warning     N/A
 * @FeatureID    -
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date        2018/12/10 KCN)Nakai Create
 */
/****************************************************************************/
static int str_to_list(char *src, const char *delim, int max, char **list){
    int i = 0;
    char *p = NULL;
    char buffer[D_PARAM_NUM_MAX];

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, src);

    p = strtok(buffer, delim);
    while(p != NULL){
        strcpy(list[i], p);
        p = strtok(NULL, delim);
        i++;
        if(i >= max){
            break;
        }
    }

    return i;
}

/*!
 * @brief		メモリアドレス検証
 * @note		指定されたアドレスが自プロセス内のメモリかどうかをチェックする
 * @param		chkbufp	[in]	チェック対象のメモリアドレス
 * @return		結果
 * @retval		0:プロセス内メモリ(正常時)
 * @retval		-1:プロセス外メモリ(異常時)
 * @date		2019/03/28 FJT)Taniguchi Create
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
int f_saa_com_validate_memaddr( unsigned char* chkbufp )
{
	long my_page_size = sysconf(_SC_PAGESIZE);
	unsigned char wrkbufp[my_page_size];
	unsigned long  chkaddr = (unsigned long)(chkbufp) - ((unsigned long)(chkbufp) % my_page_size);
	return mincore((void*)chkaddr,1,wrkbufp);
}

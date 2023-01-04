/*!
 *  @skip    $Id:$
 *  @file    f_saa_utc_lib.c
 *  @brief    SAA UTC library
 *  @author    KCN) Nakai Hiroyuki
 *  @date    2019/6/11 KCN)Nakai create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/*!
 * @addtogroup    SAA
 * @brief        NETCONF sub-agent.
 * @note        Works with netconfd-pro
 * @{
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "f_saa_utc_lib.h"


#define LOWEST_OFFSET (-720)
#define HIGHEST_OFFSET 840
#define MAX_STRING_LEN 32
#define MIN_PER_HOUR 60

static int16_t lib_offset = 0;

/* 本ライブラリ作成時点ではsingle threadでの使用を予定しているが、将来的にmulti threadsで使用されることを想定してmutexを使用してリソースへのアクセスの排他制御を行う。*/
static pthread_mutex_t lib_mutex = PTHREAD_MUTEX_INITIALIZER;

/*!
 * @brief   UTCオフセット値を設定する。
 * @note    設定可能範囲は -720から840まで。
 * @param   offset       オフセット値(分)
 * @return  0: 正常, -1: 異常
 * @date    2019/6/11 KCN)Nakai create
 */
int f_saa_com_set_utc_offset(int16_t offset){
    int rc = 0;

    if(offset >= LOWEST_OFFSET && offset <= HIGHEST_OFFSET){
        pthread_mutex_lock(&lib_mutex);
        lib_offset = offset;
        pthread_mutex_unlock(&lib_mutex);
        rc = 0;
    }
    else{
        rc = -1;
    }

    return rc;
}


/*!
 * @brief   現在のUTCオフセット値を取得する。
 * @param   なし
 * @return  UTCオフセット値
 * @date    2019/6/11 KCN)Nakai create
 */
int16_t f_saa_com_get_utc_offset(void){
    int16_t offset = 0;

    pthread_mutex_lock(&lib_mutex);
    offset = lib_offset;
    pthread_mutex_unlock(&lib_mutex);

    return offset;
}


/*!
 * @brief   指定されたUTC時間をUTCオフセット値で指定されたdate-and-time形式の文字列に変換する。
 * @note    UTC値オフセット値が00:00の場合、必ず "+00:00"と表現する。
 * @note    monthに13を指定するなどしてもそれなりの値を返すので注意。(strftime()の仕様に従う。)
 * @param   year  年
 * @param   month 月
 * @param   day	  日
 * @param   hour  時
 * @param   min   分
 * @param   sec   秒
 * @param   out_buffer	変換結果を格納するためのバッファ
 * @return  0: 正常, -1: 異常
 * @date    2019/6/11 KCN)Nakai create
 */
int f_saa_com_utc_to_offset_string(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, char *out_buffer){
    int rc = 0;
    struct tm in;
    struct tm tm_buffer;
    time_t time_value = 0;
    size_t size = 0;
    char work_buffer[MAX_STRING_LEN + 1];
    int16_t offset = 0;
    

    if(out_buffer == NULL){
        return -1;
    }

    pthread_mutex_lock(&lib_mutex);
    offset = lib_offset;
    pthread_mutex_unlock(&lib_mutex);

    memset(work_buffer, 0, sizeof(work_buffer));

    /* 入力された値を個々に評価しない。
       変換可能かどうかで評価する。 */

    /* 入力された日時はUTCであるが、localtimeだと思って計算している。
       結果には影響しないので、これでよい。*/

    in.tm_sec   = sec;
    in.tm_min   = min;
    in.tm_hour  = hour;
    in.tm_mday  = day;
    in.tm_mon   = month - 1;
    in.tm_year  = year - 1900;
    in.tm_wday  = 0;
    in.tm_yday  = 0;
    in.tm_isdst = 0;

    time_value = mktime(&in);
    if(time_value != -1){
        time_value += offset * MIN_PER_HOUR;
        localtime_r(&time_value, &tm_buffer);
        size = strftime(work_buffer, MAX_STRING_LEN, "%FT%H:%M:%S", &tm_buffer);
        if(size > 0){
            sprintf(out_buffer, "%s%+03d:%02d",
                work_buffer, offset / MIN_PER_HOUR, abs(offset) % MIN_PER_HOUR);
        }
        else{
            rc = -1;
        }
    }
    else{
        rc = -1;
    }

    return rc;
}



/* 以下はテスト用。使う場合はテスト用にビルドして動かすこと。

 > cc -DTEST <this file>

*/

#ifdef TEST
void main(int argc, char *argv[]){
    int16_t offset = 0;
    int16_t offset_got = 0;
    time_t now = 0;
    struct tm gmt;
    int rc = 0;
    char buffer[MAX_STRING_LEN + 1];

    memset(buffer, 0, sizeof(buffer));

    if(argc == 2){
        offset = atoi(argv[1]);
    }

    printf("[オフセット値の設定と取得]\n");
    printf("parameter      : offset = %d\n", offset);
    f_saa_com_set_utc_offset(offset);
    offset_got = f_saa_com_get_utc_offset();
    printf("global variable: offset = %d\n", offset_got);

    printf("\n[現在時刻]\n");
    now = time(NULL);
    gmtime_r(&now, &gmt);
    printf("year:%d, month:%d, day%d, hour:%d, min:%d, sec:%d, wday:%d, yday:%d, isdst:%d\n",
        gmt.tm_year, gmt.tm_mon, gmt.tm_mday,
        gmt.tm_hour, gmt.tm_min, gmt.tm_sec,
        gmt.tm_wday, gmt.tm_yday, gmt.tm_isdst);

    rc = f_saa_com_utc_to_offset_string(
        (uint16_t)(gmt.tm_year + 1900), (uint16_t)(gmt.tm_mon + 1),
        (uint8_t)gmt.tm_mday, (uint8_t)gmt.tm_hour, (uint8_t)gmt.tm_min,
        (uint8_t)gmt.tm_sec,
        buffer);

    printf("rc = %d\n", rc);
    printf("buffer = %s\n", buffer);

    printf("\n[バリエーション:オフセット直指定]\n");
    memset(buffer, 0, sizeof(buffer));
    lib_offset = offset; /* 検査なしで代入 */
    rc = f_saa_com_utc_to_offset_string(
        (uint16_t)(gmt.tm_year + 1900), (uint16_t)(gmt.tm_mon + 1),
        (uint8_t)gmt.tm_mday, (uint8_t)gmt.tm_hour, (uint8_t)gmt.tm_min,
        (uint8_t)gmt.tm_sec,
        buffer);
    printf("parameter      : offset = %d\n", offset);
    printf("rc = %d\n", rc);
    printf("buffer = %s\n", buffer);


    printf("\n[バリエーション:日時異常]\n");
    memset(buffer, 0, sizeof(buffer));
    rc = f_saa_com_utc_to_offset_string( 2019, 15, 20, 5, 4, 3, buffer);
    printf("rc = %d\n", rc);
    printf("buffer = %s\n", buffer);

}

#endif

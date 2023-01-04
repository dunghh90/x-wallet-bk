/*!
 *  @skip    $Id:$
 *  @file    rrhlib_device_access.c
 *  @brief   rrh command file library of device_access for tp
 *  @author  FJT)Harada
 *  @date    2013/11/29 by FJT)Harada create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup    RRH_TP_CMD_LIB
 * @{
 */
#include "rrhlib_local.h"
#include "f_dpda_inc.h"
mrb_value rrhlib_bpfshm_read(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_bpfshm_write(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_ptpconf_set(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_device_pgc_write(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_device_pgc_write_reg(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_device_pgc_read_reg(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_shm_dpda_flg_write(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_device_calphase(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_device_getrtc(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_device_setrtc(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_device_mutexfree(mrb_state* mrb, mrb_value self);
mrb_value rrhlib_device_pgc_read(mrb_state* mrb, mrb_value self);

UINT rrhlib_device_svtx_sub6 ( mrb_int ant_s )
{
	/* 変数宣言 */
	UINT result = 0 ;

	/* プロセス */
	/* RFICから情報を読み出す */

	/* アンテナ情報ごとのEEP領域にデータを保存する */

	return (result) ;
}


UINT rrhlib_device_svtx_mmw ( mrb_int ant_s , mrb_int para_s , mrb_int mode_s )
{
	/* 変数宣言 */
	UINT result = 0 ;

	/* プロセス */
	/* RFICから情報を読み出す */

	/* アンテナ情報ごとのEEP領域にデータを保存する */

	return (result) ;
}


UINT rrhlib_device_svrx_sub6 ( mrb_int ant_s )
{
	/* 変数宣言 */
	UINT result = 0 ;

	/* プロセス */
	/* RFICから情報を読み出す */

	/* アンテナ情報ごとのEEP領域にデータを保存する */

	return (result) ;
}


UINT rrhlib_device_svrx_mmw ( mrb_int ant_s , mrb_int para_s , mrb_int mode_s )
{
	/* 変数宣言 */
	UINT result = 0 ;

	/* プロセス */
	/* RFICから情報を読み出す */

	/* アンテナ情報ごとのEEP領域にデータを保存する */

	return (result) ;
}


UINT rrhlib_device_svpa_sub6 ( mrb_int ant_s )
{
	/* 変数宣言 */
	UINT result = 0 ;

	/* プロセス */
	/* RFICから情報を読み出す */

	/* アンテナ情報ごとのEEP領域にデータを保存する */

	return (result) ;
}


UINT rrhlib_device_svpa_mmw ( mrb_int ant_s , mrb_int mode_s )
{
	/* 変数宣言 */
	UINT result = 0 ;

	/* プロセス */
	/* RFICから情報を読み出す */

	/* アンテナ情報ごとのEEP領域にデータを保存する */

	return (result) ;
}


/*!
 * @brief        レジスタ読み取り処理
 * @note
<pre>
     指定されたレジスタアドレスからデータを読み取る処理である.
</pre>
 * @tparam		レジスタアドレス	[in]	String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2013/11/29
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.regread
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_register_read(mrb_state* mrb, mrb_value self)
{
    mrb_value addr_str;
    unsigned int addr;
    unsigned int data;
    char rtn_str[64];
    int size;
    int iRtn;
    mrb_value rtn;

    mrb_get_args(mrb, "S", &addr_str );


    /* exchange string to number */
    if( sscanf( RSTRING_PTR(addr_str), "%8x", &addr) == EOF ){
        rrhlib_dmsg("Invlid value is detected in addr\n");
        return mrb_false_value();
    }

   iRtn = BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, addr, &data );
   if (iRtn != BPF_HM_DEVC_COMPLETE)
   {
            rrhlib_dmsg("register read error errcode = %d , addr = 0x%08x \n",iRtn,addr);
            /* データ取得失敗は0とする */
            data = 0;
    }

    /* exchange number to string */
    size = snprintf(rtn_str, sizeof(rtn_str), "%08x", data);
    if(size < 0){
        rrhlib_dmsg("Invlid value is detected in data\n");
        return mrb_false_value();
    }

    rrhlib_dmsg("register read finish : addr = 0x%08x, data = 0x%08x\n", addr, data);

    rtn = mrb_str_new(mrb, rtn_str, size);
    return rtn;
}


/*!
 * @brief        レジスタ書き込み処理
 * @note
<pre>
     指定されたレジスタアドレスにデータを書き込む処理である.
</pre>
 * @tparam		レジスタアドレス	[in]	String
 * @tparam		データ				[in]	String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/02
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.regwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_register_write(mrb_state* mrb, mrb_value self)
{
    mrb_value addr_str;
    mrb_value data_str;
    unsigned int addr;
    unsigned int data;
    int iRtn;

    mrb_get_args(mrb, "SS", &addr_str, &data_str );

    /* exchange string to number */
    if( sscanf( RSTRING_PTR(addr_str), "%8x", &addr) == EOF ){
        rrhlib_dmsg("Invlid value is detected in addr\n");
        return mrb_false_value();
    }
    if( sscanf( RSTRING_PTR(data_str), "%8x", &data) == EOF ){
        rrhlib_dmsg("Invlid value is detected in data\n");
        return mrb_false_value();
    }

    iRtn = BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, addr, &data);
    if (iRtn != BPF_HM_DEVC_COMPLETE){
        rrhlib_dmsg("register write error, errcode = %d , addr = 0x%08x \n",iRtn,addr);
	        /* データ書込み失敗 */
	        return mrb_false_value();
    }

    rrhlib_dmsg("register write finish : addr = 0x%08x, data = 0x%08x\n", addr, data);
    return mrb_true_value();
}

/*!
 * @brief        BIT ON処理
 * @note
<pre>
     指定されたレジスタアドレスにデータを書き込む処理である.
</pre>
 * @tparam		レジスタアドレス	[in]	String
 * @tparam		データ				[in]	String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/02
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.regbiton
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_register_biton(mrb_state* mrb, mrb_value self)
{
    mrb_value addr_str;
    mrb_value data_str;
    unsigned int addr;
    unsigned int data;
    int iRtn;

    mrb_get_args(mrb, "SS", &addr_str, &data_str );

    /* exchange string to number */
    if( sscanf( RSTRING_PTR(addr_str), "%8x", &addr) == EOF ){
        rrhlib_dmsg("Invlid value is detected in addr\n");
        return mrb_false_value();
    }
    if( sscanf( RSTRING_PTR(data_str), "%8x", &data) == EOF ){
        rrhlib_dmsg("Invlid value is detected in data\n");
        return mrb_false_value();
    }
    iRtn = BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, addr, data);
    if (iRtn != BPF_HM_DEVC_COMPLETE){
        rrhlib_dmsg("register bit on error, errcode = %d , addr = 0x%08x \n",iRtn,addr);
        /* データ書込み失敗 */
        return mrb_false_value();
    }
    rrhlib_dmsg("register bit on finish : addr = 0x%08x, data = 0x%08x\n", addr, data);
    return mrb_true_value();
}

/*!
 * @brief        BIT OFF処理
 * @note
<pre>
     指定されたレジスタアドレスにデータを書き込む処理である.
</pre>
 * @tparam		レジスタアドレス	[in]	String
 * @tparam		データ				[in]	String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/02
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.regbitoff
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_register_bitoff(mrb_state* mrb, mrb_value self)
{
    mrb_value addr_str;
    mrb_value data_str;
    unsigned int addr;
    unsigned int data;
    int iRtn;

    mrb_get_args(mrb, "SS", &addr_str, &data_str );

    /* exchange string to number */
    if( sscanf( RSTRING_PTR(addr_str), "%8x", &addr) == EOF ){
        rrhlib_dmsg("Invlid value is detected in addr\n");
        return mrb_false_value();
    }
    if( sscanf( RSTRING_PTR(data_str), "%8x", &data) == EOF ){
        rrhlib_dmsg("Invlid value is detected in data\n");
        return mrb_false_value();
    }
    iRtn = BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, addr, data);
    if (iRtn != BPF_HM_DEVC_COMPLETE){
        rrhlib_dmsg("register bit off error, errcode = %d , addr = 0x%08x \n",iRtn,addr);
        /* データ書込み失敗 */
        return mrb_false_value();
    }
    rrhlib_dmsg("register bit off finish : addr = 0x%08x, data = 0x%08x\n", addr, data);
    return mrb_true_value();
}


/*!
 * @brief        EEPROM読み取り処理
 * @note
<pre>
     指定されたEEPROMのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		EEPROMアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2013/12/03
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.eepread
 * @warning     EEPROMアクセスのみ引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_eeprom_read(mrb_state* mrb, mrb_value self)
{
    mrb_int addr;
    unsigned char data;
    char rtn_str[64];
    int size;
    int iRtn;
    mrb_value rtn;

    mrb_get_args(mrb, "i", &addr );

	/* アドレス範囲判定 */
	if( (unsigned short)addr > D_DBG_TP_EEPROM_SIZE_MAX ){
		rrhlib_dmsg("eeprom address error : <addr> = 0~7FF \n");
        return mrb_false_value();
	}

    /* Call EEPROM read function of BPF */
    iRtn = BPF_HM_DEVC_EEPROM_READ( (unsigned short)addr, &data);
    if (iRtn != BPF_HM_DEVC_COMPLETE)
    {
        rrhlib_dmsg("eeprom read error, errcode = %d , addr = 0x%04hx \n", iRtn, (unsigned short)addr);
        /* データ取得失敗 */
        return mrb_false_value();
    }

    /* exchange number to string */
    size = snprintf(rtn_str, sizeof(rtn_str), "%02hhx", data);
    if(size < 0){
        rrhlib_dmsg("Invlid value is detected in data\n");
        return mrb_false_value();
    }

    rrhlib_dmsg("eeprom read finish : addr = 0x%04hx, data = 0x%02hhx\n", (unsigned short)addr, data);

    rtn = mrb_str_new(mrb, rtn_str, size);
    return rtn;
}

/*!
 * @brief        EEPROM読み取り処理
 * @note
<pre>
     指定されたEEPROMのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		EEPROMアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2013/12/03
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.eepread
 * @warning     EEPROMアクセスのみ引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_eeprom_read_txt(mrb_state* mrb, mrb_value self)
{
	mrb_int addr;
	mrb_int	len;
	unsigned char data[D_RRH_I2C_EEP_DATANUM];
	char rtn_str[(D_RRH_I2C_EEP_DATANUM*2) + 2];
	int size;
	int iRtn;
	mrb_value rtn;
	mrb_int			count;

	mrb_get_args(mrb, "ii", &addr , &len);
	if( (unsigned short)addr >= D_RRH_I2C_EEP_DATANUM ){
		rrhlib_dmsg("eeprom address error : <addr> = 0~7FF \n");
		return mrb_false_value();
	}
	if((unsigned short)(addr + len) >= D_RRH_I2C_EEP_DATANUM ){
		rrhlib_dmsg("eeprom address error : <addr> = 0~7FF \n");
		return mrb_false_value();
	}
	memset(data,0,sizeof(data));
	for(count=0 ; count < len ; count++){
		iRtn = BPF_HM_DEVC_EEPROM_READ( (unsigned short)(addr + count) , &data[count]);
		if (iRtn != BPF_HM_DEVC_COMPLETE){
			rrhlib_dmsg("eeprom read error, errcode = %d , addr = 0x%04hx \n", iRtn, (unsigned short)(addr + count));
			return mrb_false_value();
		}
	}

	size = 0;
	memset(rtn_str,0,sizeof(rtn_str));
	for(count=0 ; count < len ; count++){
		if( (data[count] >= 0x21) && (data[count] <= 0x7e) ){
			size += snprintf(&rtn_str[size], sizeof(rtn_str) - size ,"%c", (unsigned char)data[count] );
		}
		else{
			size += snprintf(&rtn_str[size], sizeof(rtn_str) - size ," ");
		}
	}
	rrhlib_dmsg("eeprom read finish : addr = 0x%04hx,len = 0x%d %s \n", (unsigned short)addr, len,rtn_str);
	rtn = mrb_str_new(mrb, rtn_str, size);
	return rtn;
}

/*!
 * @brief       EEPROM書き込み処理
 * @note
<pre>
     指定されたEEPROMのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		EEPROMアドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/04
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.eepwrite
 * @warning     EEPROMアクセスのみ引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_eeprom_write(mrb_state* mrb, mrb_value self)
{
    mrb_int addr;
    mrb_int data;
    unsigned char c_data;
    int iRtn;

    mrb_get_args(mrb, "ii", &addr, &data );

	/* アドレス範囲判定 */
	if( (unsigned short)addr > D_DBG_TP_EEPROM_SIZE_MAX ){
		rrhlib_dmsg("eeprom address error : <addr> = 0~7FF \n");
        return mrb_false_value();
	}
	/* データ範囲判定 */
	if( (unsigned int)data > 0xFF ){
		rrhlib_dmsg("eeprom write data error : <data> = 0~FF \n");
        return mrb_false_value();
	}

	c_data = (unsigned char)data;
    /* Call EPPROM write function of BPF */
    iRtn = BPF_HM_DEVC_EEPROM_WRITE( (unsigned short)addr, &c_data);
    if (iRtn != BPF_HM_DEVC_COMPLETE)
    {
        rrhlib_dmsg("eeprom write error, errcode = %d , addr = 0x%04hx \n", iRtn, (unsigned short)addr);
        /* データ書込み失敗 */
        return mrb_false_value();
    }

    rrhlib_dmsg("eeprom write finish : addr = 0x%04hx, data = 0x%02hhx\n", (unsigned short)addr, (unsigned char)c_data);
    return mrb_true_value();
}

/*!
 * @brief        QSPI-FLASH読み取り処理(boot ver)
 * @note
<pre>
     指定オフセット位置のQSPI-FLASHのデータを読み取る処理である.
</pre>
 * @tparam		Flashリード情報	[in]	Object
 * @param       mrb				[in]	mrb status
 * @param       self			[in]	mrb self
 * @return      false
 * @return      バッファポインタ
 * @date        2016/4/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.flashread_btver
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_qspi_flash_read_btver(mrb_state* mrb, mrb_value self)
{
	int						 index;
	unsigned int			 face;
	unsigned int			 ofs;
	unsigned char			 str_p[16];
	int						 iRtn=BPF_HM_DEVC_NG;

	mrb_get_args(mrb, "i", &index);
	switch(index)
	{
		case 0:
			face = 0;
			ofs = D_RRH_QSPI_FLASH_OFFSET_BOOT;
			break;
		case 1:
			face = 0;
			ofs = D_RRH_QSPI_FLASH_OFFSET_BOOT_2;
			break;
		case 2:
			face = 1;
			ofs = D_RRH_QSPI_FLASH_OFFSET_BOOT;
			break;
		case 3:
			face = 1;
			ofs = D_RRH_QSPI_FLASH_OFFSET_BOOT_2;
			break;
		default:
			return mrb_false_value();
			break;
	}
	memset(str_p,0,sizeof(str_p));
	if(( iRtn = BPF_HM_DEVC_QSPI_FLASH_READ_BOOT_VER(face,ofs,str_p) ) == BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("boot ver read finish %d %08x %s\n",face,ofs,str_p);
	}
	else{
		rrhlib_dmsg("boot ver read error %d %08x %s\n",face,ofs,str_p);
		return mrb_false_value();
	}
	return mrb_str_new(mrb, (char*)str_p, strlen((char*)str_p));
}

/*!
 * @brief        QSPI-FLASH読み取り処理
 * @note
<pre>
     指定オフセット位置のQSPI-FLASHのデータを読み取る処理である.
</pre>
 * @tparam		Flashリード情報	[in]	Object
 * @param       mrb				[in]	mrb status
 * @param       self			[in]	mrb self
 * @return      false
 * @return      バッファポインタ
 * @date        2013/12/13
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.flashread
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_qspi_flash_read(mrb_state* mrb, mrb_value self)
{
	int			 face;
	int			 addr;
	int			 rsize;
	unsigned char*		buf_p;
	int			 iRtn=BPF_HM_DEVC_NG;
	int count;
	char		 str_p[64];
	unsigned int ssize;
	mrb_value 	 rtn;


    mrb_get_args(mrb, "iii", &face, &addr, &rsize);

	if (rsize > 0x10) {
		rrhlib_dmsg("read_size_over:%d\n",rsize);
		return mrb_false_value();
	}

	buf_p = mrb_malloc(mrb , (size_t)rsize );		/* pgr0821 */
	if(buf_p == NULL)
	{
		rrhlib_dmsg("malloc error \n");
		return mrb_false_value();
	}

    memset(buf_p,0,rsize);

	rrhlib_dmsg("flash read start %x %x %x \n",face,rsize,addr);
	/* Call flash write function of BPF */
	switch(face)
	{
		case BPF_HM_DEVC_QSPI_FLASH_FACE_0:
		case BPF_HM_DEVC_QSPI_FLASH_FACE_1:
			iRtn = BPF_HM_DEVC_QSPI_FLASH_READ( face, addr, rsize, (unsigned char*)buf_p);
			break;
		case 2:
			iRtn = BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ(addr, rsize, (unsigned char*)buf_p);
			break;
		default:
			rrhlib_dmsg("Invlid value is detected in face\n");
			break;
	}
	if (iRtn != BPF_HM_DEVC_COMPLETE)
	{
		rrhlib_dmsg("flash read error, errcode = %d\n",iRtn);
		mrb_free(mrb,(void*)buf_p);
		/* データ取得失敗 */
		return mrb_false_value();
	}

	ssize = 0;
	for(count=0; count<rsize; count++)
	{
		ssize += snprintf(str_p + ssize, (unsigned int)rsize*2 ,"%02x", (unsigned char)*(buf_p + count) );
	}
	str_p[ssize] = '\0';
	ssize++;

	rtn = mrb_str_new(mrb, str_p, ssize);
	mrb_free(mrb,(void*)buf_p);

	rrhlib_dmsg("flash read finish %d\n",rsize);
	return rtn;
}

/*!
 * @brief        QSPI-FLASH読み取り処理2
 * @note
<pre>
     指定オフセット位置のQSPI-FLASHのデータを読み取る処理である.
</pre>
 * @tparam		Flashリード情報	[in]	Object
 * @param       mrb				[in]	mrb status
 * @param       self			[in]	mrb self
 * @return      false
 * @return      バッファポインタ
 * @date        2013/12/13
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.flashread
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_qspi_flash_read_b(mrb_state* mrb, mrb_value self)
{

	mrb_value 	 obj;
	mrb_value 	 *ptr;
	mrb_int 	 num;
	int			 face;
	int			 addr;
	int			 rsize;
	unsigned long			 buf_p;
	int			 iRtn;
	unsigned char* val_p;
	int			 len = 0;

	mrb_get_args(mrb, "o", &obj);

	ptr = RARRAY_PTR(obj);
	num = RARRAY_LEN(obj);

	if(num != 5)
	{
		rrhlib_dmsg("flash read num over %d \n",num);
		return mrb_false_value();
	}
	face  = mrb_fixnum(ptr[0]);
	rsize = mrb_fixnum(ptr[1]);
	addr  = mrb_fixnum(ptr[2]);
	buf_p = mrb_fixnum(ptr[3]);

	rrhlib_dmsg("flash read start %x %x %x \n",face,addr,rsize);
	/* Call flash write function of BPF */
	iRtn = BPF_HM_DEVC_QSPI_FLASH_READ( face, addr, rsize, (unsigned char*)buf_p);
	if (iRtn != BPF_HM_DEVC_COMPLETE)
	{
		rrhlib_dmsg("flash read error, errcode = %d\n",iRtn);
		/* データ取得失敗 */
		return mrb_false_value();
	}
	if(rsize >= 4)
	{
		val_p = (unsigned char*)buf_p;
		memcpy(&len , val_p , sizeof(len));
		ptr[4] = mrb_fixnum_value(ntohl(len));
	}
	rrhlib_dmsg("flash read finish %d\n",len);
	return mrb_true_value();
}


/*!
 * @brief        QSPI-FLASH書込み処理
 * @note
<pre>
     バッファの内容を指定サイズ分QSPI-FLASHへ書き込む処理である.
</pre>
 * @tparam		面情報			[in]	String
 * @tparam		flashアドレス	[in]	String
 * @tparam		書き込みサイズ	[in]	String
 * @tparam		バッファー		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/04
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.flashwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_qspi_flash_write(mrb_state* mrb, mrb_value self)
{
    mrb_value face_str;
    mrb_value addr_str;
    mrb_value wsize_str;
    mrb_int buffer;
    unsigned int face;
    unsigned int addr;
    unsigned int wsize;
    int iRtn = -1;

    mrb_get_args(mrb, "SSSi", &face_str, &addr_str, &wsize_str, &buffer );

    /* exchange string to number */
    if( sscanf( RSTRING_PTR(face_str), "%x", &face) == EOF ){
        rrhlib_dmsg("Invlid value is detected in face\n");
        return mrb_false_value();
    }
    if( sscanf( RSTRING_PTR(addr_str), "%x", &addr) == EOF ){
        rrhlib_dmsg("Invlid value is detected in addr\n");
        return mrb_false_value();
    }
    if( sscanf( RSTRING_PTR(wsize_str), "%x", &wsize) == EOF ){
        rrhlib_dmsg("Invlid value is detected in wsize\n");
        return mrb_false_value();
    }
	rrhlib_dmsg("flash write %x %x %x \n", face , addr , wsize);
	switch(face)
	{
		case BPF_HM_DEVC_QSPI_FLASH_FACE_0:
		case BPF_HM_DEVC_QSPI_FLASH_FACE_1:
#if 0		/* コンパイルエラー対処 start	*/
		case BPF_HM_DEVC_QSPI_FLASH_FACE_2:
		case BPF_HM_DEVC_QSPI_FLASH_FACE_3:
#endif		/* コンパイルエラー対処 end	*/
			/* Call flash write function of BPF */
			iRtn = BPF_HM_DEVC_QSPI_FLASH_WRITE( face, addr, wsize, (unsigned char*)buffer);
			break;
		case 2:
			iRtn = BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(addr,wsize,(unsigned char*)buffer);
			break;
		default:
			rrhlib_dmsg("Invlid value is detected in face\n");
			break;
	}
	if (iRtn != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("flash write error, errcode = %d\n",iRtn);
		/* データ取得失敗 */
		return mrb_false_value();
	}
	rrhlib_dmsg("flash write finish\n");
	return mrb_true_value();
}



/*!
 * @brief        QSPI-FLASHイレース処理
 * @note
<pre>
     指定されたFLASHの領域をイレースする処理である
</pre>
 * @tparam		イレース領域	[in]	String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/04
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.flasherase
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_qspi_flash_erase(mrb_state* mrb, mrb_value self)
{
	mrb_value face_str;
	mrb_value addr_str;
	mrb_value esize_str;
	unsigned int face;
	unsigned int addr;
	unsigned int esize;
	int iRtn = -1;

	mrb_get_args(mrb, "SSS", &face_str, &addr_str, &esize_str);

	/* exchange string to number */
	if( sscanf( RSTRING_PTR(face_str), "%x", &face) == EOF ){
		rrhlib_dmsg("Invlid value is detected in face\n");
		return mrb_false_value();
	}
	if( sscanf( RSTRING_PTR(addr_str), "%x", &addr) == EOF ){
		rrhlib_dmsg("Invlid value is detected in addr\n");
		return mrb_false_value();
	}
	if( sscanf( RSTRING_PTR(esize_str), "%x", &esize ) == EOF ){
		rrhlib_dmsg("Invlid value is detected in wsize\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("flash erase %x %x %x \n", face , addr , esize);
	switch(face)
	{
		case BPF_HM_DEVC_QSPI_FLASH_FACE_0:
		case BPF_HM_DEVC_QSPI_FLASH_FACE_1:
#if 0		/* コンパイルエラー対処	start	*/
		case BPF_HM_DEVC_QSPI_FLASH_FACE_2:
		case BPF_HM_DEVC_QSPI_FLASH_FACE_3:
#endif		/* コンパイルエラー対処 end	*/
			/* Call flash erace function of BPF */
			iRtn = BPF_HM_DEVC_QSPI_FLASH_ERASE( face, addr, esize );
			break;
		case 2:
			iRtn = BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE(addr,esize);
			break;
		default:
			break;
	}
	if (iRtn != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("flash erase error, errcode = %d\n",iRtn);
		/* データ取得失敗 */
		return mrb_false_value();
	}
	rrhlib_dmsg("flash erase finish\n");
	return mrb_true_value();
}

/*!
 * @brief        SFP読み取り処理
 * @note
<pre>
 	I2C(SFP)からデータを読み取る処理である.
</pre>
 * @tparam		デバイス種別	[in]	String
 * @tparam		アドレス		[in]	String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2013/12/12
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.sfpread
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_sfp_read(mrb_state* mrb, mrb_value self)
{
	mrb_int 		kind;
	mrb_int			len;
	mrb_int			addr;
	mrb_value		dtype;

	char			data[384];
	mrb_int			count;
	char			rtn_str[1024];
	unsigned int	size;
	char			type;

	mrb_get_args(mrb, "iiiS",&kind,&addr,&len,&dtype);
	rrhlib_dmsg("sfp read %x %x %x \n", kind , addr , len);

	if(
	   ( kind != BPF_HM_DEVC_SFP_A0 ) &&
	   ( kind != BPF_HM_DEVC_SFP_A2 )
	)
	{
		rrhlib_dmsg("sfp read kind error \n");
		return mrb_false_value();
	}
	if(sizeof(data) < len )
	{
		rrhlib_dmsg("sfp read len error \n");
		return mrb_false_value();
	}
	if((addr + len) > 0x100 )
	{
		rrhlib_dmsg("sfp read addr+len error \n");
		return mrb_false_value();
	}
	if( !strncmp(RSTRING_PTR(dtype),"bin",sizeof("bin")) )
	{
		type = 0;
	}
	else if( !strncmp(RSTRING_PTR(dtype),"txt",sizeof("txt")) )
	{
		type = 1;
	}
	else
	{
		rrhlib_dmsg("sfp read type error \n");
		return mrb_false_value();
	}
	memset(data,0,sizeof(data));
	if(( BPF_HM_DEVC_SFP_READ_BUFFER( (unsigned int)kind, (unsigned short)addr,len,(unsigned char*)data) ) != BPF_HM_DEVC_COMPLETE )
	{
		rrhlib_dmsg("sfp read bpf error \n");
		return mrb_false_value();
	}
	size = 0;
	memset(rtn_str,0,sizeof(rtn_str));
	for(count=0 ; count < len ; count++)
	{
		if(type == 0)
		{
			size += snprintf(&rtn_str[size], sizeof(rtn_str) - size ,"%02x", (unsigned char)data[count] );
		}
		else
		{
			if( (data[count] >= 0x21) && (data[count] <= 0x7e) )
			{
				size += snprintf(&rtn_str[size], sizeof(rtn_str) - size ,"%c", (unsigned char)data[count] );
			}
			else
			{
				size += snprintf(&rtn_str[size], sizeof(rtn_str) - size ,".");
			}
		}
	}
	rrhlib_dmsg("sfp read result %d %s\n",size,rtn_str);
	return mrb_str_new(mrb, rtn_str, size);
}

/*!
 * @brief        SFP書き込み処理
 * @note
<pre>
 	I2C(SFP)にデータを書き込む処理である.
</pre>
 * @tparam		デバイス種別	[in]	Integer
 * @tparam		アドレス		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/12
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.sfpwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_sfp_write(mrb_state* mrb, mrb_value self)
{
	mrb_int 		kind;
	mrb_int			addr;
	mrb_int			data;

	mrb_get_args(mrb, "iii",&kind,&addr,&data);
	rrhlib_dmsg("sfp write %x %x %x \n", kind , addr , data);

	if(
	   ( kind != BPF_HM_DEVC_SFP_A0 ) &&
	   ( kind != BPF_HM_DEVC_SFP_A2 )
	)
	{
		rrhlib_dmsg("sfp write kind error \n");
		return mrb_false_value();
	}
	if( data > 0xFF )
	{
		rrhlib_dmsg("sfp write data error \n");
		return mrb_false_value();
	}
	if( addr > 0xFF )
	{
		rrhlib_dmsg("sfp write addr error \n");
		return mrb_false_value();
	}

	if(( BPF_HM_DEVC_SFP_WRITE( (unsigned int)kind, (unsigned short)addr, (unsigned char*)&data) ) != BPF_HM_DEVC_COMPLETE )
	{
		rrhlib_dmsg("sfp write bpf error \n");
		return mrb_false_value();
	}
	rrhlib_dmsg("sfp write OK\n");
	return mrb_true_value();
}

/*!
 * @brief        温度センサー読み取り処理
 * @note
<pre>
     温度センサーから温度を読み出す
</pre>
 * @tparam		N/A
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/21 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.tempread
 * @warning     TEMPアクセスのみ引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_temp_read(mrb_state* mrb, mrb_value self)
{
	char rtn_str[64];
	int temp;
	int iRtn;
	int size;

	if(( iRtn = BPF_HM_DEVC_TEMP_READ(&temp) ) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("temp read error, errcode = %d \n", iRtn);
		return mrb_false_value();
	}
	if(( size = snprintf(rtn_str, sizeof(rtn_str), "%d", temp)) < 0){
		rrhlib_dmsg("Invlid value is detected in data 0x%08x \n",temp);
		return mrb_false_value();
	}
	rrhlib_dmsg("temp read finish : %d(0x%08x)\n", temp,temp);
	return mrb_str_new(mrb, rtn_str, size);
}

/*!
 * @brief        温度センサー読み取り処理
 * @note
<pre>
     温度センサーから温度を読み出す
</pre>
 * @tparam		N/A
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/21 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.tempread
 * @warning     TEMPアクセスのみ引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_temp_read2(mrb_state* mrb, mrb_value self)
{
	char rtn_str[64];
	int temp;
	int iRtn;
	int size;

	if(( iRtn = BPF_HM_DEVC_TEMP2_READ(&temp) ) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("temp read error, errcode = %d \n", iRtn);
		return mrb_false_value();
	}
	if(( size = snprintf(rtn_str, sizeof(rtn_str), "%d", temp)) < 0){
		rrhlib_dmsg("Invlid value is detected in data 0x%08x \n",temp);
		return mrb_false_value();
	}
	rrhlib_dmsg("temp read finish : %d(0x%08x)\n", temp,temp);
	return mrb_str_new(mrb, rtn_str, size);
}

/*!
 * @brief        MMICPLD読み取り処理
 * @note
<pre>
     指定されたMMICPLDのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		MMIPLDアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.mmiread
 * @warning     MMICPLDアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_mmipld_read(mrb_state* mrb, mrb_value self)
{
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;

	mrb_get_args(mrb, "i", &addr );
	if((iRtn = BPF_HM_DEVC_MMI_CPLD_READ(addr, &data)) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("mmi pld read error, errcode = %d , addr = 0x%04hx \n", iRtn, (unsigned short)addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", (unsigned short)data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("mmi pld read finish : addr = 0x%04hx, data = 0x%04x\n", (unsigned short)addr, (unsigned short)data);
	return mrb_str_new(mrb, rtn_str, size);
}


/*!
 * @brief       MMICPLD書き込み処理
 * @note
<pre>
     指定されたMMICPLDのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		MMICPLDアドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.mmiwrite
 * @warning     MMICPLDアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_mmipld_write(mrb_state* mrb, mrb_value self)
{
	mrb_int addr;
	mrb_int data;
	unsigned int c_data = 0;
	int iRtn;

	mrb_get_args(mrb, "ii", &addr, &data );
	c_data = (unsigned short)data;
	if ((iRtn = BPF_HM_DEVC_MMI_CPLD_WRITE( (unsigned short)addr, &c_data)) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("mmi pld write error, errcode = %d , addr = 0x%04x \n", iRtn, (unsigned short)addr);
		return mrb_false_value();
	}
	rrhlib_dmsg("mmi pld write finish : addr = 0x%04hx, data = 0x%04x \n", (unsigned short)addr, c_data);
	return mrb_true_value();
}

/*!
 * @brief        LCCPLD0読み取り処理
 * @note
<pre>
     指定されたLCCPLD0のアドレスからデータを読み取る処理である.
</pre>
 * @tparam		MMIPLDアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.cpld0read
 * @warning     LCCPLD0アクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_cpld0_read(mrb_state* mrb, mrb_value self)
{
#if 0		/* コンパイルエラー対処 start	*/
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;

	mrb_get_args(mrb, "i", &addr );
	if ((iRtn = BPF_HM_DEVC_LC_CPLD_READ(BPF_HM_DEVC_SPI_CPLD0,addr, &data)) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("lccpld0 read error, errcode = %d , addr = 0x%04hx \n", iRtn, (unsigned short)addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", (unsigned short)data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("lccpld0 read finish : addr = 0x%04hx, data = 0x%04x\n", (unsigned short)addr, (unsigned short)data);
	return mrb_str_new(mrb, rtn_str, size);
#endif		/* コンパイルエラー対処 end	*/
	return mrb_true_value();		/* 返り値は設定する	*/
}


/*!
 * @brief       LCCPLD0書き込み処理
 * @note
<pre>
     指定されたLCCPLD0のアドレスにデータを書き込む処理である.
</pre>
 * @tparam		LCCPLD0アドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.cpld0write
 * @warning     LCCPLD0アクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_cpld0_write(mrb_state* mrb, mrb_value self)
{
#if 0		/* コンパイルエラー対処 start	*/
	mrb_int addr;
	mrb_int data;
	unsigned int c_data = 0;
	int iRtn;

	mrb_get_args(mrb, "ii", &addr, &data );
	c_data = (unsigned short)data;
	if((iRtn = BPF_HM_DEVC_LC_CPLD_WRITE(BPF_HM_DEVC_SPI_CPLD0, (unsigned short)addr, c_data)) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("lccpld0 write error, errcode = %d , addr = 0x%04x \n", iRtn, (unsigned short)addr);
		return mrb_false_value();
	}
	rrhlib_dmsg("lccpld0 write finish : addr = 0x%04hx, data = 0x%04x \n", (unsigned short)addr, c_data);
#endif		/* コンパイルエラー対処 end	*/
	return mrb_true_value();
}

/*!
 * @brief        LCCPLD1読み取り処理
 * @note
<pre>
     指定されたLCCPLD1のアドレスからデータを読み取る処理である.
</pre>
 * @tparam		MMIPLDアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.cpld1read
 * @warning     LCCPLD0アクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_cpld1_read(mrb_state* mrb, mrb_value self)
{
#if 0		/* コンパイルエラー対処 start	*/
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;

	mrb_get_args(mrb, "i", &addr );
	if((iRtn = BPF_HM_DEVC_LC_CPLD_READ(BPF_HM_DEVC_SPI_CPLD1,addr, &data)) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("lccpld1 read error, errcode = %d , addr = 0x%04hx \n", iRtn, (unsigned short)addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", (unsigned short)data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("lccpld1 read finish : addr = 0x%04hx, data = 0x%04x\n", (unsigned short)addr, (unsigned short)data);
	return mrb_str_new(mrb, rtn_str, size);
#endif		/* コンパイルエラー対処 end	*/
	return mrb_true_value();		/* 返り値は設定する	*/
}


/*!
 * @brief       LCCPLD1書き込み処理
 * @note
<pre>
     指定されたLCCPLD1のアドレスにデータを書き込む処理である.
</pre>
 * @tparam		LCCPLD1アドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.cpld1write
 * @warning     LCCPLD1アクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_cpld1_write(mrb_state* mrb, mrb_value self)
{
#if 0		/* コンパイルエラー対処 start	*/
	mrb_int addr;
	mrb_int data;
	unsigned int c_data = 0;
	int iRtn;
	mrb_get_args(mrb, "ii", &addr, &data );
	c_data = (unsigned short)data;
	if ((iRtn = BPF_HM_DEVC_LC_CPLD_WRITE(BPF_HM_DEVC_SPI_CPLD1, (unsigned short)addr, c_data)) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("lccpld1 write error, errcode = %d , addr = 0x%04x \n", iRtn, (unsigned short)addr);
		return mrb_false_value();
	}
	rrhlib_dmsg("lccpld1 write finish : addr = 0x%04hx, data = 0x%04x \n", (unsigned short)addr, c_data);
#endif		/* コンパイルエラー対処 end	*/
	return mrb_true_value();
}


/*!
 * @brief        UNITX 読み取り処理
 * @note
<pre>
     指定されたUNITXのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		UNITXアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.txrfread
 * @warning     UNITXアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_txrf_read(mrb_state* mrb, mrb_value self)
{
	mrb_int unino;
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;

	mrb_get_args(mrb, "ii", &unino,&addr );
	if((iRtn = BPF_HM_DEVC_RFIC_READ(BPF_HM_DEVC_RFIC_TXRX_KIND_TX,unino, addr,&data)) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("unitx read error, errcode = %d , unino= 0x%04hx addr = 0x%04hx \n", iRtn, (unsigned short)unino,(unsigned short)addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", (unsigned short)data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("unitx read finish : unino= 0x%04hx, addr = 0x%04hx, data = 0x%04x\n", (unsigned short)unino,(unsigned short)addr, (unsigned short)data);
	return mrb_str_new(mrb, rtn_str, size);
}

/*!
 * @brief        UNITX 書き込み処理
 * @note
<pre>
     指定されたUNITXのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		UNITXアドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.txrfwrite
 * @warning     UNITXアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_txrf_write(mrb_state* mrb, mrb_value self)
{
	mrb_int unino;
	mrb_int addr;
	mrb_int data;
	unsigned int c_data = 0;
	int iRtn;

	mrb_get_args(mrb, "iii", &unino , &addr, &data );
	c_data = (unsigned short)data;
	if((iRtn = BPF_HM_DEVC_RFIC_WRITE(BPF_HM_DEVC_RFIC_TXRX_KIND_TX,(unsigned short)unino, (unsigned short)addr, c_data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("unitx write error, errcode = %d , unino= 0x%04hx,addr = 0x%04x \n", iRtn, (unsigned short)unino,(unsigned short)addr);
		return mrb_false_value();
	}
	rrhlib_dmsg("unitx write finish : unino= 0x%04hx ,addr = 0x%04hx, data = 0x%04x \n", (unsigned short)unino,(unsigned short)addr, c_data);
	return mrb_true_value();
}

/*!
 * @brief        UNIRX 読み取り処理
 * @note
<pre>
     指定されたUNIRXのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		UNIRXアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rxrfread
 * @warning     UNIRXアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rxrf_read(mrb_state* mrb, mrb_value self)
{
	mrb_int unino;
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;

	mrb_get_args(mrb, "ii", &unino , &addr );
	if((iRtn = BPF_HM_DEVC_RFIC_READ(BPF_HM_DEVC_RFIC_TXRX_KIND_RX,unino, addr,&data)) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("unirx read error, errcode = %d , unino= 0x%04hx addr = 0x%04hx \n", iRtn, (unsigned short)unino,(unsigned short)addr);
		return mrb_false_value();
	}
	if(( size = snprintf(rtn_str, sizeof(rtn_str), "%04x", (unsigned short)data) ) < 0 ){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("unirx read finish : unino= 0x%04hx, addr = 0x%04hx, data = 0x%04x\n", (unsigned short)unino,(unsigned short)addr, (unsigned short)data);
	return mrb_str_new(mrb, rtn_str, size);
}

/*!
 * @brief        UNIRX 書き込み処理
 * @note
<pre>
     指定されたUNIRXのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		UNIRXアドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rxrfwrite
 * @warning     UNITRXアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rxrf_write(mrb_state* mrb, mrb_value self)
{
	mrb_int unino;
	mrb_int addr;
	mrb_int data;
	unsigned int c_data = 0;
	int iRtn;

	mrb_get_args(mrb, "iii", &unino , &addr, &data );
	c_data = (unsigned short)data;
	if((iRtn = BPF_HM_DEVC_RFIC_WRITE(BPF_HM_DEVC_RFIC_TXRX_KIND_RX,(unsigned short)unino, (unsigned short)addr, c_data) ) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("unirx write error, errcode = %d , unino= 0x%04hx,addr = 0x%04x, data = 0x%04x \n", iRtn, (unsigned short)unino,(unsigned short)addr , c_data);
		return mrb_false_value();
	}
	rrhlib_dmsg("unirx write finish : unino= 0x%04hx ,addr = 0x%04hx, data = 0x%04x \n", (unsigned short)unino,(unsigned short)addr, c_data);
	return mrb_true_value();
}

/*!
 * @brief        RFIC 読み取り処理
 * @note
<pre>
     指定されたRFICのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		RFICアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rficread
 * @warning     RFICアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rfic_read(mrb_state* mrb, mrb_value self)
{
	mrb_int unino;
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;

	mrb_get_args(mrb, "ii", &unino,&addr );
	if((iRtn = BPF_HM_DEVC_RFIC_READ(BPF_HM_DEVC_RFIC_TXRX_KIND_TX,unino, addr,&data)) != BPF_HM_DEVC_COMPLETE){
		rrhlib_dmsg("rfic read error, errcode = %d , unino= 0x%04hx addr = 0x%04hx \n", iRtn, (unsigned short)unino,(unsigned short)addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", (unsigned short)data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("rfic read finish : unino= 0x%04hx, addr = 0x%04hx, data = 0x%04x\n", (unsigned short)unino,(unsigned short)addr, (unsigned short)data);
	return mrb_str_new(mrb, rtn_str, size);
}

/*!
 * @brief        RFIC 書き込み処理
 * @note
<pre>
     指定されたRFICのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		UNITXアドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rficwrite
 * @warning     RFICアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rfic_write(mrb_state* mrb, mrb_value self)
{
	mrb_int unino;
	mrb_int addr;
	mrb_int data;
	unsigned int c_data = 0;
	int iRtn;

	mrb_get_args(mrb, "iii", &unino , &addr, &data );
	c_data = (unsigned short)data;
	if((iRtn = BPF_HM_DEVC_RFIC_WRITE(BPF_HM_DEVC_RFIC_TXRX_KIND_TX,(unsigned short)unino, (unsigned short)addr, c_data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("rfic write error, errcode = %d , unino= 0x%04hx,addr = 0x%04x \n", iRtn, (unsigned short)unino,(unsigned short)addr);
		return mrb_false_value();
	}
	rrhlib_dmsg("rfic write finish : unino= 0x%04hx ,addr = 0x%04hx, data = 0x%04x \n", (unsigned short)unino,(unsigned short)addr, c_data);
	return mrb_true_value();
}


/*!
 * @brief        PLL読み取り処理
 * @note
<pre>
     指定されたPLLのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		PLLアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.pllread
 * @warning     PLLアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_pll_read(mrb_state* mrb, mrb_value self)
{
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;

	mrb_get_args(mrb, "i", &addr );
	if(( iRtn = BPF_HM_DEVC_EXT_PLL_READ(addr,&data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("pll read error, errcode = %d  addr = 0x%04hx \n", iRtn, (unsigned short)addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", (unsigned short)data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("pll read finish : addr = 0x%04x, data = 0x%04x\n",(unsigned short)addr, (unsigned short)data);
	return mrb_str_new(mrb, rtn_str, size);
}

/*!
 * @brief        PLL書き込み処理
 * @note
<pre>
     指定されたPLLのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		PLLアドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.pllwrite
 * @warning     UNITRXアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_pll_write(mrb_state* mrb, mrb_value self)
{
	mrb_int addr;
	mrb_int data;
	unsigned int c_data = 0;
	int iRtn;
	mrb_get_args(mrb, "ii",&addr, &data );
	c_data = (unsigned short)data;
	if(( iRtn = BPF_HM_DEVC_EXT_PLL_WRITE((unsigned short)addr, c_data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("pll write error, errcode = %d , unino= 0x%04hx,addr = 0x%04x, data = 0x%04x \n", iRtn, (unsigned short)addr , (unsigned short)c_data);
		return mrb_false_value();
	}
	rrhlib_dmsg("pll write finish :addr = 0x%04x, data = 0x%04x\n", (unsigned short)addr, (unsigned short)c_data);
	return mrb_true_value();
}



/*!
 * @brief        PTP読み取り処理
 * @note
<pre>
     指定されたPTPのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		PTPアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.ptpread
 * @warning     PTPアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_ptp_read(mrb_state* mrb, mrb_value self)
{
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;

	mrb_get_args(mrb, "i", &addr );
	if(( iRtn = BPF_HM_DEVC_PTP_READ(addr,&data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("ptp read error, errcode = %d  addr = 0x%04hx \n", iRtn, (unsigned short)addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", (unsigned short)data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("ptp read finish : addr = 0x%04x, data = 0x%04x\n",(unsigned short)addr, (unsigned short)data);
	return mrb_str_new(mrb, rtn_str, size);
}

/*!
 * @brief        PTP書き込み処理
 * @note
<pre>
     指定されたPTPのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		PTPアドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2015/04/22 taniguchi
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.ptpwrite
 * @warning     PTPアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_ptp_write(mrb_state* mrb, mrb_value self)
{
	mrb_int addr;
	mrb_int data;
	unsigned int c_data = 0;
	int iRtn;
	mrb_get_args(mrb, "ii",&addr, &data );
	c_data = (unsigned short)data;
	if(( iRtn = BPF_HM_DEVC_PTP_WRITE((unsigned short)addr, c_data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("ptp write error, errcode = %d , unino= 0x%04hx,addr = 0x%04x, data = 0x%04x \n", iRtn, (unsigned short)addr , (unsigned short)c_data);
		return mrb_false_value();
	}
	rrhlib_dmsg("ptp write finish :addr = 0x%04x, data = 0x%04x\n", (unsigned short)addr, (unsigned short)c_data);
	return mrb_true_value();
}


/*!
 * @brief        QMOD(ADMV1013)読み取り処理
 * @note
<pre>
     指定されたQMODのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		セレクター		[in]	Integer
 * @tparam		QMODアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/07/27 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.qmodread
 * @warning     QMODアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_qmod_read(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	mrb_int sel;
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;

	mrb_get_args(mrb, "iii",&ant, &sel, &addr );



if(( iRtn = BPF_HM_DEVC_QMOD_READ(ant, sel, addr,  &data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("qmod read error, errcode = %d Ant = %d, sel = %d, addr = 0x%04hx  \n", iRtn, ant, sel, addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}

	rrhlib_dmsg("qmod read finish : Ant = %d, sel = %d, addr = 0x%04x,  data = 0x%04x\n",ant, sel, addr,  data);
	return mrb_str_new(mrb, rtn_str, size);

}


/*!
 * @brief        QMOD(ADMV1013)レジスタ書き込み処理
 * @note
<pre>
     指定されたQMODのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		セレクター		[in]	Integer
 * @tparam		QMODアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/07/27 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.qmodwrite
 * @warning     QMODアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_qmod_write(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	mrb_int sel;
	mrb_int addr;
	mrb_int data;
	int iRtn;
	mrb_get_args(mrb, "iiii",&ant, &sel, &addr,  &data );



if(( iRtn = BPF_HM_DEVC_QMOD_WRITE(ant, sel, addr,  data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("qmod write error, errcode = %d , unino= 0x%04hx, Ant = %d, sel = %d, addr = 0x%04x,  data = 0x%04x \n", iRtn, ant, sel, addr,  (unsigned int)data);
		return mrb_false_value();
	}
	rrhlib_dmsg("qmod write finish :Ant = %d, sel = %d, addr = 0x%04x,  data = 0x%04x\n",ant, sel, addr,  data);
	return mrb_true_value();
}



/*!
 * @brief        QMOD(ADMV1014)読み取り処理
 * @note
<pre>
     指定されたQMODのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		QMODアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.qdemread
 * @warning     QMODアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_qdem_read(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	mrb_int sel;
	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;
	mrb_get_args(mrb, "iii", &ant, &sel, &addr);



if(( iRtn = BPF_HM_DEVC_QDEM_READ(ant,sel,addr,&data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("qdem read error, errcode = %d Ant = %d, sel = %d, addr = 0x%04hx \n", iRtn, ant, sel, addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}

	rrhlib_dmsg("qdem read finish : Ant = %d, sel = %d, addr = 0x%04x,  data = 0x%04x\n",ant, sel, addr, data);
	return mrb_str_new(mrb, rtn_str, size);


}


/*!
 * @brief        QMOD(ADMV1014)レジスタ書き込み処理
 * @note
<pre>
     指定されたQMODのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		QMODアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.qdemwrite
 * @warning     QMODアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_qdem_write(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	mrb_int sel;
	mrb_int addr;
	mrb_int data;
	int iRtn;

	mrb_get_args(mrb, "iiii",&ant, &sel, &addr, &data );



if(( iRtn = BPF_HM_DEVC_QDEM_WRITE(ant,sel,addr,data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("qdem write error, errcode = %d , unino= 0x%04hx, Ant = %d, sel = %d, addr = 0x%04x, data = 0x%04x \n", iRtn, ant, sel, addr, data);
		return mrb_false_value();
	}
	rrhlib_dmsg("qdem write finish :Ant = %d, sel = %d, addr = 0x%04x, data = 0x%04x\n",ant, sel, addr, data);
	return mrb_true_value();
}



/*!
 * @brief        RFPLL(LMX2594)読み取り処理
 * @note
<pre>
     指定されたRFPLLのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		RFPLLアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rfpllread
 * @warning     RFPLLアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rfpll_read(mrb_state* mrb, mrb_value self)
{

	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;
	mrb_get_args(mrb, "i", &addr);


if(( iRtn = BPF_HM_DEVC_RFPLL_READ(addr,&data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("rfpll read error, errcode = %d addr = 0x%04hx  \n", iRtn, addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}

	rrhlib_dmsg("rfpll read finish : addr = 0x%04x, data = 0x%04x \n", addr, data);
	return mrb_str_new(mrb, rtn_str, size);


}


/*!
 * @brief        RFPLL(LMX2594)書き込み処理
 * @note
<pre>
     指定されたRFPLLのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		RFPLLアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rfpllwrite
 * @warning     RFPLLアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rfpll_write(mrb_state* mrb, mrb_value self)
{

	mrb_int addr;
	mrb_int data;
	int iRtn;
	mrb_get_args(mrb, "ii", &addr, &data );


if(( iRtn = BPF_HM_DEVC_RFPLL_WRITE(addr, data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("rfpll write error, errcode = %d , unino= 0x%04hx, addr = 0x%04x,  data = 0x%04x \n", iRtn, addr , data);
		return mrb_false_value();
	}
	rrhlib_dmsg("rfpll write finish : addr = 0x%04x, data = 0x%04x\n",addr, data);
	return mrb_true_value();
}



/*!
 * @brief        RFEEP(S-24C16CI-I8T1U3)読み取り処理
 * @note
<pre>
     指定されたRFEEPのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		EPPROMページ数	[in]	Integer
 * @tparam		データサイズ	[in]	Integer
 * @tparam		RFEEPアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rferead
 * @warning     RFEEPアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rfe_read(mrb_state* mrb, mrb_value self)
{


	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;
	mrb_get_args(mrb, "i", &addr );


if(( iRtn = BPF_HM_DEVC_RFEEP_READ(addr,&data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("rfeep read error, errcode = %d, addr = 0x%04hx, \n", iRtn, addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}

	rrhlib_dmsg("rfeep read finish : addr = 0x%04x, data = 0x%04x\n", addr, data);
	return mrb_str_new(mrb, rtn_str, size);


}


/*!
 * @brief        RFEEP(S-24C16CI-I8T1U3)書き込み処理
 * @note
<pre>
     指定されたRFEEPのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		EPPROMページ数	[in]	Integer
 * @tparam		データサイズ	[in]	Integer
 * @tparam		RFEEPアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rfewrite
 * @warning     RFEEPアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rfe_write(mrb_state* mrb, mrb_value self)
{


	mrb_int addr;
	mrb_int data;
	int iRtn;
	mrb_get_args(mrb, "ii",&addr, &data );


if(( iRtn = BPF_HM_DEVC_RFEEP_WRITE(addr,data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("rfeep write error, errcode = %d , unino= 0x%04hx, addr = 0x%04x, data = 0x%04x \n", iRtn, addr, data);
		return mrb_false_value();
	}
	rrhlib_dmsg("rfeep write finish : addr = 0x%04x, data = 0x%04x\n",addr, data);
	return mrb_true_value();
}

/*!
 * @brief        RXADC (AD9689)読み取り処理
 * @note
<pre>
     指定されたRXADCのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		RXADCアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rxadcread
 * @warning     RXADCアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rxadc_read(mrb_state* mrb, mrb_value self)
{

	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;
	mrb_get_args(mrb, "i", &addr );


if(( iRtn = BPF_HM_DEVC_RXADC_READ(addr,&data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("rxadc read error, errcode = %d addr = 0x%04hx  \n", iRtn, addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}

	rrhlib_dmsg("rxadc read finish : addr = 0x%04x, data = 0x%04x \n", addr, data);
	return mrb_str_new(mrb, rtn_str, size);


}


/*!
 * @brief        RXADC(LMX2594)書き込み処理
 * @note
<pre>
     指定されたRXADCのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		RXADCアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.rxadcwrite
 * @warning     RXADCアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_rxadc_write(mrb_state* mrb, mrb_value self)
{

	mrb_int addr;
	mrb_int data;
	int iRtn;
	mrb_get_args(mrb, "ii", &addr, &data );


if(( iRtn = BPF_HM_DEVC_RXADC_WRITE(addr, data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("rxadc write error, errcode = %d , unino= 0x%04hx, addr = 0x%04x, data = 0x%04x \n", iRtn, addr , data);
		return mrb_false_value();
	}
	rrhlib_dmsg("rxadc write finish : addr = 0x%04x,  data = 0x%04x\n",addr, data);
	return mrb_true_value();
}

/*!
 * @brief        TXDAC (AD9172)読み取り処理
 * @note
<pre>
     指定されたTXDACのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		TXDACアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.txdacread
 * @warning     TXDACアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_txdac_read(mrb_state* mrb, mrb_value self)
{

	mrb_int addr;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;
	mrb_get_args(mrb, "i", &addr );


if(( iRtn = BPF_HM_DEVC_TXDAC_READ(addr,&data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("txdac read error, errcode = %d addr = 0x%04hx  \n", iRtn, addr);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}

	rrhlib_dmsg("txdac read finish : addr = 0x%04x, data = 0x%04x\n", addr, data);
	return mrb_str_new(mrb, rtn_str, size);


}


/*!
 * @brief        TXDAC(AD9172)書き込み処理
 * @note
<pre>
     指定されたTXDACのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		TXDACアドレス	[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/07/30 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.txdacwrite
 * @warning     TXDACアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_txdac_write(mrb_state* mrb, mrb_value self)
{

	mrb_int addr;
	mrb_int data;
	int iRtn;
	mrb_get_args(mrb, "ii", &addr, &data );


if(( iRtn = BPF_HM_DEVC_TXDAC_WRITE(addr, data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("txdac write error, errcode = %d , unino= 0x%04hx, addr = 0x%04x, data = 0x%04x \n", iRtn, addr , data);
		return mrb_false_value();
	}
	rrhlib_dmsg("txdac write finish : addr = 0x%04x, data = 0x%04x\n",addr, data);
	return mrb_true_value();
}



/*!
 * @brief        RXVATT設定読み取り処理
 * @note
<pre>
     指定されたRXVATTのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/08/06 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.srxvread
 * @warning     RXVATTアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_srxv_read(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;
	mrb_get_args(mrb, "i", &ant );



if(( iRtn = BPF_HM_DEVC_RXVATTDAC_READ(ant,&data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("srxv read error, errcode = %d Ant = %d,  \n", iRtn, ant);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}

	rrhlib_dmsg("srxv read finish : Ant = %d, data = 0x%04x\n",ant, data);
	return mrb_str_new(mrb, rtn_str, size);


}


/*!
 * @brief        RXVATT設定レジスタ書き込み処理
 * @note
<pre>
     指定されたRXVATTのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/06 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.srxvwrite
 * @warning     RXVATTアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_srxv_write(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	mrb_int data;
	int iRtn;

	mrb_get_args(mrb, "ii",&ant, &data );



if(( iRtn = BPF_HM_DEVC_RXVATTDAC_WRITE(ant,data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("srxv write error, errcode = %d , unino= 0x%04hx, Ant = %d,  data = 0x%04x \n", iRtn, ant, data);
		return mrb_false_value();
	}
	rrhlib_dmsg("srxv write finish :Ant = %d,  data = 0x%04x\n",ant, data);
	return mrb_true_value();
}


/*!
 * @brief        TXVATT設定読み取り処理
 * @note
<pre>
     指定されたTXVATTのアドレスからデータを読み取る処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/08/06 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.stxvread
 * @warning     TXVATTアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_stxv_read(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	unsigned int data = 0;
	char rtn_str[64];
	int size;
	int iRtn;
	mrb_get_args(mrb, "i", &ant );



if(( iRtn = BPF_HM_DEVC_TXVATTDAC_READ(ant,&data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("stxv read error, errcode = %d Ant = %d,  \n", iRtn, ant);
		return mrb_false_value();
	}
	if((size = snprintf(rtn_str, sizeof(rtn_str), "%04x", data)) < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}

	rrhlib_dmsg("stxv read finish : Ant = %d, data = 0x%04x\n",ant, data);
	return mrb_str_new(mrb, rtn_str, size);


}


/*!
 * @brief        TXVATT設定レジスタ書き込み処理
 * @note
<pre>
     指定されたTXVATTのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/06 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.stxvwrite
 * @warning     TXVATTアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_stxv_write(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	mrb_int data;
	int iRtn;

	mrb_get_args(mrb, "ii",&ant, &data );



if(( iRtn = BPF_HM_DEVC_TXVATTDAC_WRITE(ant,data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("stxv write error, errcode = %d , unino= 0x%04hx, Ant = %d,  data = 0x%04x \n", iRtn, ant, data);
		return mrb_false_value();
	}
	rrhlib_dmsg("stxv write finish :Ant = %d,  data = 0x%04x\n",ant, data);
	return mrb_true_value();
}



/*!
 * @brief        PAbias設定処理
 * @note
<pre>
     指定されたPAVDDACのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		装置種別		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/06 wakamizu
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.setbias
 * @warning     PAVDDACアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_set_bias(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	mrb_int data;
    T_RRH_TRAINING_DATA		*traData;		/* 共有メモリアドレス(tra data) */
	unsigned char board =0;
    int			endcd;						/* 終了コード                   */
    int			errcd;						/* 詳細NGコード                 */
	int iRtn;
	unsigned int data_s = 0;
	unsigned int addr_a = 0;
	unsigned int addr_b = 0;
	unsigned char data_a =0;
	unsigned char data_b =0;

	mrb_get_args(mrb, "ii",&ant, &data );

	if(data == 0){
		if(( iRtn = BPF_HM_DEVC_PAVGDAC_WRITE(ant,data_s)) != BPF_HM_DEVC_COMPLETE ){
			rrhlib_dmsg("pavgdac write error, errcode = %d , unino= 0x%04hx, Ant = %d, data = 0x%04x \n", iRtn, ant , data);
			return mrb_false_value();
		}
	}

	else {
		if(( iRtn =BPF_HM_DEVC_EEPROM_READ((unsigned short)D_DU_EEP_DEVICE_KIND0, &board)) != BPF_HM_DEVC_COMPLETE ){
			rrhlib_dmsg("eeprom read error, errcode = %d addr = 0x%04hx \n", iRtn, 0x40);
			return mrb_false_value();
		}

		/* 仮想レジスタ(E_RRH_SHMID_APL_TRAINING_DATA_VT)の読出し */
		if (board == D_DU_EEP_DEV_KIND_5GDU_SUB6){       /* 装置種別 = SUB6なら */
			addr_a = 0x12 + (0x80 * ant);
			addr_b = addr_a + 1;
		}
		else if (board == D_DU_EEP_DEV_KIND_5GDU_MMW){  /* 装置種別 = mmWなら */
			addr_a = 0x34 + (0x2 * ant);
		}
		addr_b = addr_a + 1;

		/* 共有メモリの読出し */
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
		        E_RRH_SHMID_APL_TRAINING_DATA,      /* 共有メモリ番号               */
		        (VOID **)&traData,                  /* 共有メモリアドレス           */
		        &errcd                              /* 詳細NGコード                 */
	    );
		/* 共有メモリの取得失敗なら */
		if( endcd != BPF_RU_IPCM_OK)
		{
			rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_TRAINING_DATA_VT  NG = %d ,errcd = %d \n",endcd,errcd);
			return mrb_false_value();
		}
		data_a = traData->traVal_vt[(int)addr_a];
		data_b = traData->traVal_vt[(int)addr_b];
		data_s = (((data_a << 8) & 0x300) | (data_b & 0x0FF));

		/* PAVGDACの書込み */
		if(( iRtn = BPF_HM_DEVC_PAVGDAC_WRITE(ant,data_s)) != BPF_HM_DEVC_COMPLETE ){
			rrhlib_dmsg("pavgdac write error, errcode = %d , unino= 0x%04hx, Ant = %d, data = 0x%04x \n", iRtn, ant,  data);
			return mrb_false_value();
		}
	}
	rrhlib_dmsg("pavgdac write finish :Ant = %d, addr = 0x%04x~0x%04x, data = 0x%04x\n",ant, addr_a, addr_b, data_s);
	return mrb_true_value();
}


/*!
 * @brief        PADAC設定レジスタ書き込み処理
 * @note
<pre>
     指定されたアンテナ番号のPADACにデータを書き込む処理である.
</pre>
 * @tparam		アンテナ		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/15  KCN)Kamimoto
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.adjpawrt
 * @warning     TXVATTアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_adjpawrt(mrb_state* mrb, mrb_value self)
{
	mrb_int ant;
	mrb_int dev;
	mrb_int data;
	int iRtn;

	mrb_get_args(mrb, "iii",&ant, &dev, &data );

if(( iRtn = BPF_HM_DEVC_PAVGDAC_WRITE(ant,data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("stxv write error, errcode = %d , unino= 0x%04hx, Ant = %d,  data = 0x%04x \n", iRtn, ant, data);
		return mrb_false_value();
	}
	rrhlib_dmsg("stxv write finish :Ant = %d,  data = 0x%04x\n",ant, data);
	return mrb_true_value();
}


/*!
 * @brief        PTP読み取り処理
 * @note
<pre>
     指定されたPTPのアドレスからデータを読み取る処理である.
     アクセスするページを選択するレジスタ(0x7f)でページを選択した上で値をリードを実施する.
</pre>
 * @tparam		PTPアドレス	[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      読み取り値(string)
 * @date        2018/08/22 wang
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.ptpread2
 * @warning     PTPアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_ptp_read2(mrb_state* mrb, mrb_value self)
{
	mrb_int addr;
	mrb_int len;
	unsigned int data = 0;
	char rtn_str[64];
	int iRtn;

	mrb_get_args(mrb, "ii", &addr, &len );
	int size[len];

	unsigned int		temp;
	unsigned short		regaddr;

	temp = ((addr & 0x00000F80) >> 7);

	if(( iRtn = BPF_HM_DEVC_PTP_WRITE( 0x007F, temp)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("ptp write error, errcode = %d , addr = 0x007F, data = 0x%04 \n", iRtn,  (unsigned short)temp);
	}

	regaddr = ((unsigned short)addr) & 0x007F;

	for(int i=0; i<len; i++){
		if(( iRtn = BPF_HM_DEVC_PTP_READ(regaddr+i,&data)) != BPF_HM_DEVC_COMPLETE ){
			rrhlib_dmsg("ptp read error, errcode = %d  addr = 0x%04hx \n", iRtn, regaddr+len);
			return mrb_false_value();
		}else{
			rtn_str[i] = (unsigned short)data;
		}
		if((size[len] = snprintf(rtn_str, sizeof(rtn_str), "%04x", (unsigned short)data)) < 0){
			rrhlib_dmsg("Invlid value is detected in data\n");
			return mrb_false_value();
		}
	}

	rrhlib_dmsg("ptp read finish : addr = 0x%04x, data = 0x%04x\n",regaddr, (unsigned short)data);
	return mrb_str_new(mrb, rtn_str, size[len]);
}

/*!
 * @brief        PTP書き込み処理
 * @note
<pre>
     指定されたPTPのアドレスにデータを書き込む処理である.
     アクセスするページを選択するレジスタ(0x7f)でページを選択した上で値をライトを実施する.
</pre>
 * @tparam		PTPアドレス	[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/22 wang
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.ptpwrite2
 * @warning     PTPアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_ptp_write2(mrb_state* mrb, mrb_value self)
{
	mrb_int addr;
	mrb_int data;
	unsigned int c_data = 0;
	int iRtn;
	mrb_get_args(mrb, "ii",&addr, &data );
	c_data = (unsigned short)data;
	unsigned int		temp;
	unsigned short		regaddr;

	temp = ((addr & 0x00000F80) >> 7);

	if(( iRtn = BPF_HM_DEVC_PTP_WRITE( 0x007F, temp)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("ptp write error, errcode = %d , addr = 0x007F, data = 0x%04 \n", iRtn,  (unsigned short)temp);
	}

	regaddr = ((unsigned short)addr) & 0x007F;

	if(( iRtn = BPF_HM_DEVC_PTP_WRITE(regaddr, c_data)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("ptp write error, errcode = %d , addr = 0x%04x, data = 0x%04 \n", iRtn, regaddr , (unsigned short)c_data);
		return mrb_false_value();
	}
	rrhlib_dmsg("ptp write finish :addr = 0x%04x, data = 0x%04x\n", regaddr, (unsigned short)c_data);
	return mrb_true_value();
}


/*************************************************************************************/
/*                                   API呼び出し                                     */
/*************************************************************************************/


/*!
 * @brief		RE種別取得処理
 * @note
<pre>
	RE種別を取得する処理である.
</pre>
 * @tparam		N/A
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.getkind()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_kind_get(mrb_state* mrb, mrb_value self)
{
	E_RRHAPI_RCODE api_rcode __attribute__((unused));
	unsigned int		kind;
    int 				size;
	char 		rtn_str[64];

	api_rcode = rrhApi_Inv_Fsm_GetDevKind(&kind,sizeof(unsigned int) );

	/* exchange number to string */
    size = snprintf(rtn_str, sizeof(rtn_str), "%04x", kind);
    if(size <= 0){
        rrhlib_dmsg("Invlid value is detected in data\n");
        return mrb_false_value();
    }

    rrhlib_dmsg("kind get  finish , dev = 0x%04x\n", kind);
    return mrb_fixnum_value(kind);

}

/*!
 * @brief		RE種別設定処理
 * @note
<pre>
	RE種別を設定する処理である.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.setkind()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_kind_set(mrb_state* mrb, mrb_value self)
{
	E_RRHAPI_RCODE		api_rcode;
	mrb_int				re_kind = (int)D_RRH_RRHKIND_NUM;
	unsigned int*		re_kind_p = NULL;
	int					errcd;

	mrb_get_args(mrb, "i", &re_kind);
	if(re_kind < 0) {
		rrhlib_dmsg("Error set re_kind(%d)\n",re_kind);
		return mrb_false_value();
	}
	if(( api_rcode = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_RRH_KIND,(VOID**)&re_kind_p,&errcd)) != E_API_RCD_OK ){
		rrhlib_dmsg("Shared memory attach Error(%d)\n",errcd );
		return mrb_false_value();
	}
	if(re_kind_p == NULL){
		rrhlib_dmsg("re_kind_p NULL \n");
		return mrb_false_value();
	}
	rrhlib_dmsg("Set Re kind:0x%x \n", re_kind );
	*re_kind_p = ( unsigned int )re_kind;
	return mrb_true_value();
}


/*!
 * @brief		IRQ受信処理
 * @note
<pre>
	IRQ受信処理である.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.irqwait()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_irqwait(mrb_state* mrb, mrb_value self)
{
	mrb_int				irqno = 0;
	int					errcd;

	mrb_get_args(mrb, "i", &irqno);
	rrhlib_dmsg("irq%d wait \n", irqno );
	if(( errcd = BPF_HM_DEVC_RESERVE(irqno)) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("BPF_HM_DEVC_RESERVE Error(%d)\n",errcd );
		return mrb_false_value();
	}
	rrhlib_dmsg("irq%d detect \n", irqno );
	return mrb_true_value();
}


/*!
 * @brief		TXパラメータセーブ処理
 * @note
<pre>
	SUB6/mmWのTXパラメータセーブ処理である.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.svtxwrite()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_svtx(mrb_state* mrb, mrb_value self)
{
	mrb_int				ant_s = 0;
	mrb_int				para_s = 0;
	mrb_int				mode_s = 0;
	INT					rtn_bpf;
	int					errcd;
	UINT				*shm_rrhKind;
	UINT				count = 0;

	mrb_get_args(mrb, "iii", &ant_s , &para_s , &mode_s);

	/* 装置種別の読出し */
	rtn_bpf =  BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_DBG_TP_OK )
	{
		return mrb_false_value();
	}

	switch( M_BPF_HM_DEVC_KIND_GET(*shm_rrhKind) )
	{
		case	BPF_HM_DEVC_SUB6:		/*	5G-DU治具(sub6)	*/
			if(ant_s == 0xFF)
			{
				/* アンテナ番号がすべてなら */
				for ( count = 0 ; count < 8 ; count++)
				{
					errcd = rrhlib_device_svtx_sub6( count ) ;
				}
			}
			else
			{
				errcd = rrhlib_device_svtx_sub6( ant_s ) ;
			}
			break ;
		case	BPF_HM_DEVC_MMW	:		/*	5G-DU治具(mmW)	*/
			if(ant_s == 0xFF)
			{
				for ( count = 0 ; count < 8 ; count++)
				{
					errcd = rrhlib_device_svtx_mmw( count , para_s , mode_s ) ;
				}
			}
			else
			{
				errcd = rrhlib_device_svtx_mmw( ant_s , para_s , mode_s ) ;
			}
			break ;
		default :									/* その他           */
			rrhlib_dmsg("E_RRH_SHMID_APL_RRH_KIND Error(%d)\n",*shm_rrhKind );
			return mrb_false_value();
			break ;
	}
	return mrb_true_value();
}


/*!
 * @brief		RXパラメータセーブ処理
 * @note
<pre>
	SUB6/mmWのRXパラメータセーブ処理である.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.svrxwrite()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_svrx(mrb_state* mrb, mrb_value self)
{
	mrb_int				ant_s = 0;
	mrb_int				para_s = 0;
	mrb_int				mode_s = 0;
	INT					rtn_bpf;
	int					errcd;
	UINT				*shm_rrhKind;
	UINT				count = 0;

	mrb_get_args(mrb, "iii", &ant_s , &para_s , &mode_s);

	/* 装置種別の読出し */
	rtn_bpf =  BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_DBG_TP_OK )
	{
		return mrb_false_value();
	}

	switch( M_BPF_HM_DEVC_KIND_GET(*shm_rrhKind) )
	{
		case	BPF_HM_DEVC_SUB6:		/*	5G-DU治具(sub6)	*/
			if (ant_s == 0xFF)
			{
				/* アンテナ番号がすべてなら */
				for ( count = 0 ; count < 8 ; count++)
				{
					errcd = rrhlib_device_svrx_sub6( count ) ;
				}
			}
			else
			{
				errcd = rrhlib_device_svrx_sub6( ant_s ) ;
			}
			break ;
		case	BPF_HM_DEVC_MMW	:		/*	5G-DU治具(mmW)	*/
			if(ant_s == 0xFF)
			{
				for ( count = 0 ; count < 8 ; count++)
				{
					errcd = rrhlib_device_svrx_mmw( count , para_s , mode_s ) ;
				}
			}
			else
			{
				errcd = rrhlib_device_svrx_mmw( ant_s , para_s , mode_s ) ;
			}
			break ;
		default :										/* その他           */
			rrhlib_dmsg("E_RRH_SHMID_APL_RRH_KIND Error(%d)\n",*shm_rrhKind );
			return mrb_false_value();
			break ;
	}
	return mrb_true_value();
}


/*!
 * @brief		パラメータセーブ処理
 * @note
<pre>
	SUB6/mmWのパラメータセーブ処理である.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.svpawrite()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_svpa(mrb_state* mrb, mrb_value self)
{
	mrb_int				ant_s = 0;
	mrb_int				mode_s = 0;
	INT					rtn_bpf;
	int					errcd;
	UINT				*shm_rrhKind;
	UINT				count = 0;

	mrb_get_args(mrb, "ii", &ant_s , &mode_s);

	/* 装置種別の読出し */
	rtn_bpf =  BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_DBG_TP_OK )
	{
		return mrb_false_value();
	}

	switch( M_BPF_HM_DEVC_KIND_GET(*shm_rrhKind) )
	{
		case	BPF_HM_DEVC_SUB6:		/*	5G-DU治具(sub6)	*/
			if (ant_s == 0xFF)
			{
				/* アンテナ番号がすべてなら */
				for ( count = 0 ; count < 8 ; count++)
				{
					errcd = rrhlib_device_svpa_sub6( count ) ;
				}
			}
			else
			{
				errcd = rrhlib_device_svpa_sub6( ant_s ) ;
			}
			break ;
		case	BPF_HM_DEVC_MMW	:		/*	5G-DU治具(mmW)	*/
			if(ant_s == 0xFF)
			{
				for ( count = 0 ; count < 8 ; count++)
				{
					errcd = rrhlib_device_svpa_mmw( count , mode_s ) ;
				}
			}
			else
			{
				errcd = rrhlib_device_svpa_mmw( ant_s , mode_s ) ;
			}
			break ;
		default :									/* その他           */
			rrhlib_dmsg("E_RRH_SHMID_APL_RRH_KIND Error(%d)\n",*shm_rrhKind );
			return mrb_false_value();
			break ;
	}
	return mrb_true_value();
}


/*!
 * @brief		APDデバッグ ON処理
 * @note
<pre>
	APDのデバッグモードをON処理する.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.apddbgon()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_apddbgon(mrb_state* mrb, mrb_value self)
{
	INT					rtn_bpf;
	int					errcd;
	UINT				*shm_rrhKind;
	unsigned long		fpga_base_addr_BX;
	unsigned int		data ;

	/* 装置種別の読出し */
	rtn_bpf =  BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_DBG_TP_OK )
	{
		return mrb_false_value();
	}

	switch( M_BPF_HM_DEVC_KIND_GET(*shm_rrhKind) ) {
		case	BPF_HM_DEVC_SUB6 :		/*	5G-DU治具(sub6)	*/
			fpga_base_addr_BX = 0xB1200000;		/* FB_SWCNTのベースアドレス */
			data = 0x00000000 ;   /*デバッグモード　ON状態 */
			/* FB_SWCNTに"0"を設定 */
			if (BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr_BX + 0x00002000, &data))
			{
				rrhlib_dmsg("BPF_HM_DEVC_REG_WRITE Error(%08x)\n",fpga_base_addr_BX + 0x00002000 );
				return mrb_false_value();
			}
			if (BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr_BX + 0x02002000, &data))
			{
				rrhlib_dmsg("BPF_HM_DEVC_REG_WRITE Error(%08x)\n",fpga_base_addr_BX + 0x02002000 );
				return mrb_false_value();
			}
			break ;
		default :									/* その他           */
			rrhlib_dmsg("E_RRH_SHMID_APL_RRH_KIND Error(%d)\n",*shm_rrhKind );
			return mrb_false_value();
			break ;
	}
	return mrb_true_value();
}


/*!
 * @brief		APDデバッグ OFF処理
 * @note
<pre>
	APDのデバッグモードをOFF処理する.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.apddbgof()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_apddbgof(mrb_state* mrb, mrb_value self)
{
	INT					rtn_bpf;
	int					errcd;
	UINT				*shm_rrhKind;
	unsigned long		fpga_base_addr_BX;
	unsigned int		data ;

	/* 装置種別の読出し */
	rtn_bpf =  BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_DBG_TP_OK )
	{
		return mrb_false_value();
	}

	switch( M_BPF_HM_DEVC_KIND_GET(*shm_rrhKind) )
	{
		case	BPF_HM_DEVC_SUB6:		/*	5G-DU治具(sub6)	*/
			fpga_base_addr_BX = 0xB1200000;		/* FB_SWCNTのベースアドレス */
			data = 0x000000F1 ;   /*デバッグモード　OFF状態 */
			/* FB_SWCNTに"x000000F1"を設定 */
			if (BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr_BX + 0x00002000, &data))
			{
				rrhlib_dmsg("BPF_HM_DEVC_REG_WRITE Error(%08x)\n",fpga_base_addr_BX + 0x00002000 );
				return mrb_false_value();
			}
			if (BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr_BX + 0x02002000, &data))
			{
				rrhlib_dmsg("BPF_HM_DEVC_REG_WRITE Error(%08x)\n",fpga_base_addr_BX + 0x02002000 );
				return mrb_false_value();
			}
			break ;
		default:									/* その他           */
			rrhlib_dmsg("E_RRH_SHMID_APL_RRH_KIND Error(%d)\n",*shm_rrhKind );
			return mrb_false_value();
			break ;
	}
	return mrb_true_value();
}


/*!
 * @brief		送信電力表示処理
 * @note
<pre>
	送信電力表示を行う.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.trnspower()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_power(mrb_state* mrb, mrb_value self)
{
	INT					rtn_bpf;
	int					errcd;
	UINT				*shm_rrhKind;

	/* 装置種別の読出し */
	rtn_bpf =  BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_DBG_TP_OK )
	{
		return mrb_false_value();
	}

	/* 送信電力量の表示 */
	return mrb_true_value();
}


/*!
 * @brief		Carrior ID パラメータ設定処理
 * @note
<pre>
	Carrior IDごとに設定できるパラメータの設定処理.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.carrsetparam()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_carrsetparam(mrb_state* mrb, mrb_value self)
{
	mrb_int				carrid_s ;
	mrb_int				DLUL_s ;
	mrb_int				ant_s ;
	mrb_int				Carr_s ;
	mrb_int				NCO_s ;
	mrb_int				ACELLID_s ;
	mrb_int				CCID_s ;
	mrb_int				PORTID_s ;
	mrb_int				CAGain_s ;
	mrb_int				CADly_s ;
	INT					rtn_bpf;
	int					errcd;
	UINT				*shm_rrhKind;

	mrb_get_args(mrb, "iiiiiiiiii", &carrid_s , &DLUL_s , &ant_s ,
	             &Carr_s , &NCO_s , &ACELLID_s , &CCID_s , &PORTID_s ,
	             &CAGain_s , &CADly_s);

	/* 装置種別の読出し */
	rtn_bpf =  BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_DBG_TP_OK )
	{
		return mrb_false_value();
	}

	/* Carrior IDごとに各種パラメータを設定する */
	return mrb_true_value();
}


/*!
 * @brief		Carrior ID パラメータ読出し処理
 * @note
<pre>
	Carrior IDごとに設定したパラメータの読出し処理.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.carrparaminf()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_carrparaminf(mrb_state* mrb, mrb_value self)
{
	mrb_int				carrid_s ;
	INT					rtn_bpf;
	int					errcd;
	UINT				*shm_rrhKind;

	mrb_get_args( mrb, "i", &carrid_s );

	/* 装置種別の読出し */
	rtn_bpf =  BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_DBG_TP_OK )
	{
		return mrb_false_value();
	}

	/* Carrior IDごとに各種パラメータを読出し表示する */
	return mrb_true_value();
}


/*!
 * @brief		Carrior ID パラメータ読出し処理
 * @note
<pre>
	Carrior IDごとに設定したパラメータの読出し処理.
</pre>
 * @tparam		RE種別				[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @warning		本関数はスクリプト上でRrh.DataTxGen()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_DataTxGen(mrb_state* mrb, mrb_value self)
{
	mrb_int				GenSW_s;
	mrb_int				Rate_s;
	INT					rtn_bpf;
	int					errcd;
	UINT				*shm_rrhKind;
	UINT				regw_data;
	int					iRtn;

	mrb_get_args( mrb, "ii", &GenSW_s , &Rate_s );

	/* 装置種別の読出し */
	rtn_bpf =  BPF_RU_IPCM_PROCSHM_ADDRGET(	E_RRH_SHMID_APL_RRH_KIND,
											(void**)&shm_rrhKind,
											&errcd );
	if( rtn_bpf != D_DBG_TP_OK )
	{
		return mrb_false_value();
	}

	/* 第一引数が0の時はOFF処理のみ */
	if (GenSW_s == 0)
	{
		regw_data = 0x00000000;
		iRtn = BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, 0xB1000788, &regw_data );
		if (iRtn != BPF_HM_DEVC_COMPLETE){
			rrhlib_dmsg("register write error, errcode = %d , addr = 0x%08x \n",iRtn,0xB1000788);
    		/* データ書込み失敗 */
    		return mrb_false_value();
		}
	}
	else
	{
		switch( M_BPF_HM_DEVC_KIND_GET(*shm_rrhKind) )
		{
			case	BPF_HM_DEVC_SUB6:		/*	5G-DU治具(sub6)	*/
				switch(Rate_s){
					case 1:							/* 100M */
						regw_data = 0x00000011;
						break;
					default:						/* SUB6は0-1のみ */
						rrhlib_dmsg("parameter error, Rate_s = %d \n",Rate_s);
						return mrb_false_value();
						break ;
				}

				iRtn = BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, 0xB1000784, &regw_data );
    			if (iRtn != BPF_HM_DEVC_COMPLETE){
        			rrhlib_dmsg("register write error, errcode = %d , addr = 0x%08x \n",iRtn,0xB1000784);
	        		/* データ書込み失敗 */
	        		return mrb_false_value();
    			}
				break;

			case	BPF_HM_DEVC_MMW	:		/*	5G-DU治具(mmW)	*/

				switch(Rate_s){
					case 1:							/* 100M */
					case 2:							/* 200M */
						regw_data = Rate_s;
						break;
					default:
						rrhlib_dmsg("parameter error, Rate_s = %d \n",Rate_s);
						return mrb_false_value();
						break ;
				}

				iRtn = BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, 0xB1000784, &regw_data );
				if (iRtn != BPF_HM_DEVC_COMPLETE){
					rrhlib_dmsg("register write error, errcode = %d , addr = 0x%08x \n",iRtn,0xB1000784);
    				/* データ書込み失敗 */
    				return mrb_false_value();
				}
				break;
			default :									/* その他           */
				rrhlib_dmsg("E_RRH_SHMID_APL_RRH_KIND Error(%d)\n",*shm_rrhKind );
				return mrb_false_value();
				break ;
		}

		/* 以下共通処理 */
		regw_data = 0x00000180;
		iRtn = BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, 0xB1000788, &regw_data );
		if (iRtn != BPF_HM_DEVC_COMPLETE){
			rrhlib_dmsg("register write error, errcode = %d , addr = 0x%08x \n",iRtn,0xB1000788);
			/* データ書込み失敗 */
			return mrb_false_value();
		}
		regw_data = 0x00000000;
		iRtn = BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, 0xB100078C, &regw_data );
		if (iRtn != BPF_HM_DEVC_COMPLETE){
			rrhlib_dmsg("register write error, errcode = %d , addr = 0x%08x \n",iRtn,0xB100078C);
			/* データ書込み失敗 */
			return mrb_false_value();
		}
		regw_data = 0x00000004;
		iRtn = BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, 0xB10007FC, &regw_data );
		if (iRtn != BPF_HM_DEVC_COMPLETE){
			rrhlib_dmsg("register write error, errcode = %d , addr = 0x%08x \n",iRtn,0xB10007FC);
			/* データ書込み失敗 */
			return mrb_false_value();
		}
		regw_data = 0x00000001;
		iRtn = BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, 0xB1000788, &regw_data );
		if (iRtn != BPF_HM_DEVC_COMPLETE){
			rrhlib_dmsg("register write error, errcode = %d , addr = 0x%08x \n",iRtn,0xB1000788);
			/* データ書込み失敗 */
			return mrb_false_value();
		}
	}
	return mrb_true_value();
}


/*!
 * @brief        FAN DAC設定処理(5G-商用DU)
 * @note
<pre>
     指定されたFAN DAC値を設定する処理である.
</pre>
 * @tparam		Writeデータ	[in]	String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2019/03/20
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.fandac_w
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_fandac_write(mrb_state* mrb, mrb_value self)
{
    mrb_value data_str;
    unsigned int data;
    int iRtn;

    mrb_get_args(mrb, "S", &data_str );

    if( sscanf( RSTRING_PTR(data_str), "%8x", &data) == EOF ){
        rrhlib_dmsg("Invlid value is detected in data\n");
        return mrb_false_value();
    }

    iRtn = BPF_HM_DEVC_FANDAC_WRITE( data );
    if (iRtn != BPF_HM_DEVC_COMPLETE){
        rrhlib_dmsg("fandac write error, errcode = %d \n",iRtn);
	        /* データ書込み失敗 */
	        return mrb_false_value();
    }

    rrhlib_dmsg("fandac write finish : data = 0x%08x \n", data);
    return mrb_true_value();
}


typedef struct {
	const char*	name;
}T_TPCMD_APR_PARM;

static const T_TPCMD_APR_PARM  TPCMD_APR_PARM[] = {
	{"default1HzRealignmentType"                        },
	{"defaultPerPacketAdjType"                          },
	{"default1HzRealignmentInterval"                    },
	{"defaultStaticOffset"                              },
	{"default1HzFilterBandwidth"                        },
	{"default1HzFilterWriteInterval"                    },
	{"defaultDeviceOptMode"                             },
	{"defaultDeviceHybridAdjMode"                       },
	{"zl303xx_AprDcs"                                   },
	{"defaultHwDcoResolutionInPpt"                      },
	{"defaultEnterPhaseLockStatusThreshold"             },
	{"defaultExitPhaseLockStatusThreshold"              },
	{"defaultWarmStartFlag"                             },
	{"defaultWsInitFreqOffset"                          },
	{"defaultAPRFrequencyLockedPhaseSlopeLimit"         },
	{"defaultAPRFrequencyNotLockedPhaseSlopeLimit"      },
	{"defaultAPRFrequencyFastPhaseSlopeLimit"           },
	{"defaultAPRFrequencyLockedFrequencyChangeLimit"    },
	{"defaultAPRFrequencyNotLockedFrequencyChangeLimit" },
	{"defaultAPRFrequencyFastFrequencyChangeLimit"      },
	{"defaultAdjSize1HzPSL[0]"                          },
	{"defaultAdjSize1HzPSL[1]"                          },
	{"defaultAdjSize1HzPSL[2]"                          },
	{"defaultAdjSize1HzPSL[3]"                          },
	{"defaultAdjSize1HzPSL[4]"                          },
	{"defaultPSL_1Hz[0]"                                },
	{"defaultPSL_1Hz[1]"                                },
	{"defaultPSL_1Hz[2]"                                },
	{"defaultPSL_1Hz[3]"                                },
	{"defaultPSL_1Hz[4]"                                },
	{"defaultAdjSize1HzFCL[0]"                          },
	{"defaultAdjSize1HzFCL[1]"                          },
	{"defaultAdjSize1HzFCL[2]"                          },
	{"defaultAdjSize1HzFCL[3]"                          },
	{"defaultAdjSize1HzFCL[4]"                          },
	{"defaultFCL_1Hz[0]"                                },
	{"defaultFCL_1Hz[1]"                                },
	{"defaultFCL_1Hz[2]"                                },
	{"defaultFCL_1Hz[3]"                                },
	{"defaultFCL_1Hz[4]"                                },
	{"defaultAdjSize1HzAdjScaling[0]"                   },
	{"defaultAdjSize1HzAdjScaling[1]"                   },
	{"defaultAdjSize1HzAdjScaling[2]"                   },
	{"defaultAdjSize1HzAdjScaling[3]"                   },
	{"defaultAdjSize1HzAdjScaling[4]"                   },
	{"defaultAdjScaling_1Hz[0]"                         },
	{"defaultAdjScaling_1Hz[1]"                         },
	{"defaultAdjScaling_1Hz[2]"                         },
	{"defaultAdjScaling_1Hz[3]"                         },
	{"defaultAdjScaling_1Hz[4]"                         },
	{"defaultPFLockInThreshold"                         },
	{"defaultPFLockInCount"                             },
	{"defaultPFLockOutThreshold"                        },
	{"defaultMaxAdjFreqTime"                            },
	{"defaultPFAdjustFreqMinPhase"                      },
	{"defaultThresholdForFlagV"                         },
	{"defaultSetTimePacketTreatment"                    },
	{"defaultStepTimePacketTreatment"                   },
	{"defaultAdjustTimePacketTreatment"                 },
	{"defaultLegacyTreatment"                           },
	{"exampleUseLegacyStreamStartUp"                    },
	{"exampleAllow1HzAdjustmentsInHoldover"             },
	{"defaultPFStepTimeResolution"                      },
	{"defaultPFLockedPhaseOutlierThreshold"             },
	{"defaultPFInitialFrequencyOffset"                  },
	{"exampleOnlyAllowSteps"                            },
	{"defaultAlgTypeMode"                               },
	{"defaultbUseT2PLL"                                 },
	{"defaultenableXOComp"                              },
	{"defaultbT2PLLFLEnb"                               },
	{"T2FLMinPhaseNs"                                   },
	{"defaultOscillatorFilter"                          },
	{"defaultOscHoldoverStability"                      },
	{"defaultXOsModeTimeout"                            },
	{"defaultSModeAgeOut"                               },
	{"defaultReRouteHoldover2"                          },
	{"defaultFilter"                                    },
	{"defaultXdslHpFlag"                                },
	{"defaultFwdPPS"                                    },
	{"defaultRevPPS"                                    },
	{"defaultTsFormat"                                  },
	{"defaultTs32BitFlag"                               },
	{"zl303xx_AprUseReversePath"                        },
	{"defaultHybridServerFlag"                          },
	{"defaultPacketDiscardDurationInSecFlag"            },
	{"defaultPullInRange"                               },
	{"defaultEnterHoldeverGST"                          },
	{"defaultExitValidGST"                              },
	{"defaultExitLockGST"                               },
	{"defaultExitPhaseAlignGST"                         },
	{"defaultLockMasks"                                 },
	{"FastLockTotalTimeInSecs"                          },
	{"HoldoverFreezeValue"                              },
	{"AprTaskBasePeriodMs"                              },
	{"PslFclTaskBasePeriodMs"                           },
	{"DFSeedValue"                                      },
	{"Type2BFastlockStartupIt"                          },
	{"Type2BFastlockThreshold"                          },
	{"OutlierTimerValue"                                },
	{"ClkInvalidCntrValue"                              },
	{"FastLockPktSelWindowSize"                         },
	{"L2phase_varLimitValue"                            },
	{"L4Threshold"                                      },
	{"DefaultUseOFM"                                    },
	{"FastLockBW"                                       },
	{"exampleAprFCL"                                    },
	{"fastLock1HzEnable"                                },
	{"fastLock1HzInterval"                              },
	{"fastLock1HzTotalPeriod"                           },
	{"fastLock1HzDelay"                                 },
	{"stepTimeAdjustTimeThreshold"                      },
	{"stepTimeAdjustFreqThreshold"                      },
	{"defaultAdjustTimeMinThreshold"                    },
	{"defaultUseAdjustTimeHybrid"                       },
	{"defaultUseAdjustTimePacket"                       },
	{"defaultStepTimeDetectableThr"                     },
	{"defaultAprDynamicPhaseCorrectionEnabled"          },
	{"defaultAdjFreqDynamicAdjustmentEnable"            },
	{"defaultAprDynamicPhaseCorrectionThr"              },
	{"DefaultUseNCOAssistDpll"                          },
	{"defaultHybridLockTimeTarget"                      },
	{"clkStabDelayIter"                                 },
	{"default1HzEnabledFlag"                            },
	{"startupTimeInit"                                  }
};


mrb_value rrhlib_ptpapr_get(mrb_state* mrb, mrb_value self)
{
	mrb_value	name;
	UINT		count;
	UINT		id = 0xFFFFFFFF;
	T_RRH_PTP_APR_PARM*	aprparm;
	int endcd;
	int errcd;
	char		 rtn_str[64];
	int 		 size = 0;
	UINT		idmax = (sizeof(TPCMD_APR_PARM)/sizeof(TPCMD_APR_PARM[0]));

	mrb_get_args(mrb, "S", &name);

	for(count=0;count < (sizeof(TPCMD_APR_PARM)/sizeof(TPCMD_APR_PARM[0]));count++){
		if(!strncmp(RSTRING_PTR(name),TPCMD_APR_PARM[count].name,strlen(TPCMD_APR_PARM[count].name))){
			id = count;
			break;
		}
	}
	if(id == 0xFFFFFFFF){
		if(!strncmp(RSTRING_PTR(name),"saveflg",strlen("saveflg"))){
			id = idmax;
		}
		else{
			rrhlib_dmsg("%s NOT FOUND \n",RSTRING_PTR(name));
			return mrb_false_value();
		}
	}

	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PTP_TIME_ADJ,  		/* 共有メモリ番号               */
		(void **)&aprparm,    			   	/* 共有メモリアドレス           */
		&errcd                              /* 詳細NGコード                 */
	);
	if( endcd != BPF_RU_IPCM_OK ){
		rrhlib_dmsg("%s:%d BPF ERROR \n",RSTRING_PTR(name),errcd );
		return mrb_false_value();
	}

	if(id < idmax){
		if(aprparm->info[id].valid == D_RRH_OFF){
			rrhlib_dmsg("%s:%d NOT SET \n",RSTRING_PTR(name),errcd );
			return mrb_false_value();
		}
		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", aprparm->info[id].val);
		rrhlib_dmsg("%s id = %d data = 0x%08x\n", RSTRING_PTR(name),id, aprparm->info[id].val);
	}else{
		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", aprparm->saveflg);
		rrhlib_dmsg("%s id = %d data = 0x%08x\n", RSTRING_PTR(name),id, aprparm->saveflg);
	}

	if(size < 0){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	return mrb_str_new(mrb, rtn_str, size);
}


mrb_value rrhlib_ptpapr_set(mrb_state* mrb, mrb_value self)
{
	mrb_value	name;
	mrb_int		 val;

	UINT		count;
	UINT		id = 0xFFFFFFFF;
	T_RRH_PTP_APR_PARM*	aprparm;
	int endcd;
	int errcd;
	UINT		idmax = (sizeof(TPCMD_APR_PARM)/sizeof(TPCMD_APR_PARM[0]));

	mrb_get_args(mrb, "Si", &name,&val);

	for(count=0;count < idmax ;count++){
		if(!strncmp(RSTRING_PTR(name),TPCMD_APR_PARM[count].name,strlen(TPCMD_APR_PARM[count].name))){
			id = count;
			break;
		}
	}
	if(id == 0xFFFFFFFF){
		if(!strncmp(RSTRING_PTR(name),"saveflg",strlen("saveflg"))){
			id = idmax;
		}
		else{
			rrhlib_dmsg("%s NOT FOUND \n",RSTRING_PTR(name));
			return mrb_false_value();
		}
	}

	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
		E_RRH_SHMID_APL_PTP_TIME_ADJ,  		/* 共有メモリ番号               */
		(void **)&aprparm,    			   	/* 共有メモリアドレス           */
		&errcd                              /* 詳細NGコード                 */
	);
	if( endcd != BPF_RU_IPCM_OK ){
		rrhlib_dmsg("%s:%d BPF ERROR \n",RSTRING_PTR(name),errcd );
		return mrb_false_value();
	}
	if(id < idmax){
		aprparm->info[id].val   = (UINT)val;
		aprparm->info[id].len  = 4;
		aprparm->info[id].index  = id;
		aprparm->info[id].valid = D_RRH_ON;
		aprparm->length = sizeof(T_RRH_PTP_APR_PARM);
		aprparm->saveflg = D_RRH_ON;
		rrhlib_dmsg("%s id = %d data = 0x%08x saveflg = %d \n", RSTRING_PTR(name),id, aprparm->info[id].val,aprparm->saveflg);
	}else{
		memset(aprparm,0,sizeof(T_RRH_PTP_APR_PARM));
		rrhlib_dmsg("%s id = %d saveflg = %d \n", RSTRING_PTR(name),id,aprparm->saveflg);
	}
	return mrb_true_value();
}

/*!
 * @brief        FAN情報取得処理(5G-商用DU)
 * @note
<pre>
     指定されたキーワードから情報取得する処理である.
</pre>
 * @tparam		キーワード	[in]	String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      String(取得結果)
 * @date        2019/03/27
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.getFanInfo
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_fanInfo_get(mrb_state* mrb, mrb_value self)
{
	mrb_value					inputStr;
	INT							errcd;
	T_RRH_DU_EIO_SVCTRL_MNG		*pShmTbl;
	INT							size = 0;
	char		 				rtn_str[16];

	mrb_get_args(mrb, "S", &inputStr );

	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_EIO_SVCTRL_MNG_TBL, (VOID **)&pShmTbl, &errcd);

	/*	FANデバッグモード	*/
	if(!strncmp(RSTRING_PTR(inputStr), "fandbgf", strlen("fandbgf")))
	{
		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanCtrl.fanDbgFlg);
		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanCtrl.fanDbgFlg);
	}
#if 0
//	/*	FAN停止状態のDAC値	*/
//	else if(!strncmp(RSTRING_PTR(inputStr), "fandacS", strlen("fandacS")))
//	{
//		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanCtrl.fanDacParam[0].fanDacVal);
//		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanCtrl.fanDacParam[0].fanDacVal);
//	}
//	/*	FAN低電圧状態のDAC値	*/
//	else if(!strncmp(RSTRING_PTR(inputStr), "fandacL", strlen("fandacL")))
//	{
//		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanCtrl.fanDacParam[1].fanDacVal);
//		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanCtrl.fanDacParam[1].fanDacVal);
//	}
//	/*	FAN高電圧状態のDAC値	*/
//	else if(!strncmp(RSTRING_PTR(inputStr), "fandacH", strlen("fandacH")))
//	{
//		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanCtrl.fanDacParam[2].fanDacVal);
//		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanCtrl.fanDacParam[2].fanDacVal);
//	}
//	/*	FAN MAX電圧状態のDAC値	*/
//	else if(!strncmp(RSTRING_PTR(inputStr), "fandacM", strlen("fandacM")))
//	{
//		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanCtrl.fanDacParam[3].fanDacVal);
//		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanCtrl.fanDacParam[3].fanDacVal);
//	}
#endif
	/*	FANユニット状態	*/
	else if(!strncmp(RSTRING_PTR(inputStr), "unitSts", strlen("unitSts")))
	{
		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanSv.fanUnitFault);
		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanSv.fanUnitFault);
	}
	/*	FAN#1-3回転異常状態	*/
	else if(!strncmp(RSTRING_PTR(inputStr), "fansFlt", strlen("fansFlt")))
	{
		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanSv.fanSpeedInfo.fanSpeedFault);
		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanSv.fanSpeedInfo.fanSpeedFault);
	}
	else if(!strncmp(RSTRING_PTR(inputStr), "f1Speed", strlen("f1Speed")))
	{
		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanSv.fanSpeedInfo.fanSpeed[0]);
		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanSv.fanSpeedInfo.fanSpeed[0]);
	}
	else if(!strncmp(RSTRING_PTR(inputStr), "f2Speed", strlen("f2Speed")))
	{
		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanSv.fanSpeedInfo.fanSpeed[1]);
		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanSv.fanSpeedInfo.fanSpeed[1]);
	}
	else if(!strncmp(RSTRING_PTR(inputStr), "f3Speed", strlen("f3Speed")))
	{
		size = snprintf(rtn_str, sizeof(rtn_str), "%08x", pShmTbl->fanSv.fanSpeedInfo.fanSpeed[2]);
		rrhlib_dmsg("%s = %08x\n", RSTRING_PTR(inputStr), pShmTbl->fanSv.fanSpeedInfo.fanSpeed[2]);
	}
	else
	{
		size = 0;
	}
	return mrb_str_new(mrb, rtn_str, size);
}

#ifndef OPT_RRH_CT

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/*!
 * @brief		レジスタドライバ処理
 * @note
<pre>
	レジスタドライバを直接起動する処理である.
</pre>
 * @tparam		アクセス種別		[in]	String
 * @tparam		アドレス			[in]	String
 * @tparam		サイズ or 設定値	[in]	String
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @date		2015/3/6 Taniguchi create
 * @warning		本関数はスクリプト上でRrh.regdrv()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_regdrv(mrb_state* mrb, mrb_value self)
{
	mrb_value	 kind_str;
	mrb_value	 addr_str;
	mrb_value	 data_str;
	int			 fd;
	void		 *ptr;
	unsigned	 page_addr;
	unsigned	 page_addr_l;
	unsigned	 page_offset;
	unsigned	 page_size   = sysconf(_SC_PAGESIZE);
	unsigned int addr = 0;
	unsigned int data = 0;
	mrb_value	 rtn;
	char		 rtn_str[64];
	int 		 size = 0;

	mrb_get_args(mrb, "SSS", &kind_str, &addr_str , &data_str );
	if( sscanf( RSTRING_PTR(addr_str), "%8x", &addr) == EOF ){
		rrhlib_dmsg("Invlid value is detected in addr\n");
		return mrb_false_value();
	}
	if( sscanf( RSTRING_PTR(data_str), "%8x", &data) == EOF ){
		rrhlib_dmsg("Invlid value is detected in data\n");
		return mrb_false_value();
	}
	if (( fd = open("/dev/mem",O_RDWR) ) == 0 )
	{
		rrhlib_dmsg("regdrv open error\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("regdrv kind:%s ofs:0x%08x data:0x%08x Fd:%d \n", RSTRING_PTR(kind_str) , addr , data, fd);
	page_addr   = (addr & ~(page_size-1));
	page_addr_l = ((addr + size) & ~(page_size-1));
	if(page_addr != page_addr_l){
		page_size += page_addr_l - page_addr;
	}
	page_offset = addr - page_addr;
	if((ptr = mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,page_addr)) != (void*)-1 ){	/* pgr0203 */
		msync( ptr , page_size , MS_SYNC );
		if( strncmp("r",RSTRING_PTR(kind_str),1) == 0){
			data = (unsigned int)(*(unsigned int*)(ptr + page_offset));
			rrhlib_dmsg("read %08x \n",data);
		}
		else if( strncmp("w",RSTRING_PTR(kind_str),1) == 0){
			*((unsigned int*)(ptr + page_offset)) = data;
			rrhlib_dmsg("write %08x \n",data);
		}
		else if( strlen(RSTRING_PTR(kind_str)) == strlen("half_x") ){
			if( strcmp("half_r",RSTRING_PTR(kind_str)) == 0 ){
				data = (unsigned short)(*(unsigned short*)(ptr + page_offset));
				data &= 0x0000ffff;
				rrhlib_dmsg("half read %08x \n",data);
			}
			else if( strcmp("half_w",RSTRING_PTR(kind_str)) == 0 ){
				data &= 0x0000FFFF;
				*((unsigned short*)(ptr + page_offset)) = (unsigned short)data;
				rrhlib_dmsg("half write %08x \n",data);
			}
		}
		else{
			rrhlib_dmsg("regdrv not supported %s \n",RSTRING_PTR(kind_str));
		}
		msync( ptr , page_size , MS_SYNC );
		munmap(ptr,page_size);
	}
	close(fd);
	size = snprintf(rtn_str, sizeof(rtn_str), "%08x", data);
	if(size < 0){
		return mrb_false_value();
	}
	rtn = mrb_str_new(mrb, rtn_str, size);
	return rtn;
}

/*!
 * @brief		EEPROMドライバ処理
 * @note
<pre>
	EEPROMドライバを直接起動する処理である.
</pre>
 * @tparam		アクセス種別		[in]	String
 * @tparam		アドレス			[in]	Integer
 * @tparam		サイズ or 設定値	[in]	Integer
 * @param		mrb					[in]	mrb status
 * @param		self				[in]	mrb self
 * @return		結果(String)
 * @date		2013/20 Taniguchi create
 * @warning		本関数はスクリプト上でRrh.eepdrv()でコールできる
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_device_eepdrv(mrb_state* mrb, mrb_value self)
{
	mrb_value	 kind_str;
	mrb_int		 addr;
	mrb_int		 val;
	int			 fd;
	char		 buf_p[2];
	int			 ret;
	int			 page;
	int			 getval = -1;

	mrb_get_args(mrb, "Sii", &kind_str, &addr , &val );

	rrhlib_dmsg("Eepdrv kind:%s ofs:0x%08x Size:%d \n", RSTRING_PTR(kind_str) , addr , val);
	if (( fd = open("/dev/i2c-0",O_RDWR) ) == 0 )
	{
		rrhlib_dmsg("Eepdrv open error\n");
		return self;
	}
	rrhlib_dmsg("Eepdrv Fd:%d \n", fd);
	page = ((addr >> 8) & 0x07) | 0x00000050 ;
	if( ( ret = ioctl(fd, I2C_SLAVE, page ) ) != 0 )
	{
		rrhlib_dmsg("Eepdrv ioctl NG :%d \n", ret);
		close(fd);
		return mrb_fixnum_value(getval);
	}
	memset(buf_p,0,sizeof(buf_p));
	if( strncmp("r",RSTRING_PTR(kind_str),1) == 0)
	{
		do {
			buf_p[0] = (char)(addr & 0xFF);
			if(( ret = write(fd,buf_p,1) ) <= 0 )
			{
				rrhlib_dmsg("Eepdrv write NG :%d \n", ret);
				break;
			}
			buf_p[0] = 0;
			if(( ret = read(fd,buf_p,1)) <= 0 )
			{
				rrhlib_dmsg("Eepdrv read NG :%d \n", ret);
				break;
			}
			getval = buf_p[0];
		}while( 0 );
	}
	else if( strncmp("w",RSTRING_PTR(kind_str),1) == 0)
	{
		struct timespec stime;
		stime.tv_sec = 0;
		stime.tv_nsec = 10000000;	/* 10msec */
		do {
			buf_p[0] = (char)(addr & 0xFF);
			buf_p[1] = (char)(val & 0xFF);
			if(( ret = write(fd,buf_p,sizeof(buf_p)) ) <= 0 )
			{
				rrhlib_dmsg("Eepdrv write NG :%d \n", ret);
				getval = ret;
				break;
			}
			else{
				getval = 0;
			}
			nanosleep(&stime,NULL);
		}while( 0 );
	}
	else
	{
		rrhlib_dmsg("Eepdrv not supported \n");
	}
	close(fd);
	rrhlib_dmsg("Eepdrv finish %02x%02x \n",buf_p[0],buf_p[1]);
	return mrb_fixnum_value(getval);
}

/*!
 * @brief        プロセス管理機能割付け
 * @note
<pre>
     プロセス管理機能割付けを行う.
 	 BPFを用いるのに必要
</pre>
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/17
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.assign
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
extern	const	T_RRH_SYS_CREATE_SHMMEM		f_comr_shmmemCreMng[E_RRH_SHMID_APL_MAX];
mrb_value rrhlib_device_init(mrb_state* mrb, mrb_value self)
{
	int rtn_bpf;

	rtn_bpf = BPF_RM_SVRM_INIT(D_DBG_TP_PROCESS_DESC);
	if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
	{
		rrhlib_dmsg("device_init init error = %d\n", rtn_bpf );
		return mrb_false_value();
	}
	rtn_bpf = BPF_RM_SVRM_SHM_CREATE(	D_DBG_TP_PROCESS_DESC,
										E_RRH_SHMID_APL_MAX,
										(VOID *)&f_comr_shmmemCreMng );

	if(BPF_RM_SVRM_COMPLETE != rtn_bpf)
	{
		rrhlib_dmsg("device_init create error = %d\n", rtn_bpf );
		return mrb_false_value();
	}
	rrhlib_dmsg("device_init OK \n");
	return mrb_true_value();
}


/*!
 * @brief        デバイス状態変更
 * @note
<pre>
	デバイス状態を変更する
</pre>
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/17
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.devcfg
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_config(mrb_state* mrb, mrb_value self)
{
	mrb_value	 dev_str;
	mrb_int		 val;

	mrb_get_args(mrb, "Si", &dev_str, &val );

	if( strcmp("plconfig",RSTRING_PTR(dev_str)) == 0){
		BPF_HM_DEVC_PL_CONFIG_STATE_SET( val );
	}
	else if( strcmp("fpga_access",RSTRING_PTR(dev_str)) == 0){
		BPF_HM_DEVC_FPGA_ACCESS(val);
	}
	else
	{
		rrhlib_dmsg("device_config NG unknown %s<-%x \n",RSTRING_PTR(dev_str),val);
		return mrb_false_value();
	}
	rrhlib_dmsg("device_config OK %s<-%x \n",RSTRING_PTR(dev_str),val);
	return mrb_true_value();
}



/*!
 * @brief        共有メモリ読み取り処理
 * @note
<pre>
 	共有メモリからデータを読み取る処理である.
</pre>
 * @tparam      読み取り種別     [in]    String
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/13
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.bpfshmread
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_bpfshm_read(mrb_state* mrb, mrb_value self)
{
	mrb_int					addr;		/* トレーニングデータアドレス	*/
	int						endcd;		/* 終了コード					*/
	int						errcd;		/* 詳細NGコード					*/
	mrb_value				shmstr;
	mrb_value				addrstr;
	mrb_value				memstr;
	unsigned int			data;
	char					rtn_str[64];
	int size;

	T_RRH_TRAINING_DATA		*traData;	/* 共有メモリアドレス(tra data) */
	T_RRH_DU_PROCESS_INFO	*tDuProcInfo;
	T_RRH_CAL_COEF_TBL		*cal_coef_tbl;
	T_RRH_BF_MODE_TBL		*bf_mode_tbl;

    mrb_get_args(mrb, "SSS", &shmstr, &memstr, &addrstr);

	 if( strcmp("E_RRH_SHMID_APL_TRAINING_DATA",RSTRING_PTR(shmstr)) == 0){
	    endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
	            E_RRH_SHMID_APL_TRAINING_DATA,      /* 共有メモリ番号               */
	            (VOID **)&traData,                  /* 共有メモリアドレス           */
	            &errcd                              /* 詳細NGコード                 */
	    );
	    /* 共有メモリの取得失敗時 */
	    if( endcd != BPF_RU_IPCM_OK )
	    {
	        rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_TRAINING_DATA  NG = %d ,errcd = %d \n",endcd,errcd);
	        return mrb_false_value();
	    }
	    if( sscanf( RSTRING_PTR(addrstr), "%lx", &addr) == EOF ){
	        rrhlib_dmsg("Invlid value is detected in addstr : %s\n", RSTRING_PTR(addrstr));
	        return mrb_false_value();
	    }
		if( ((unsigned int)addr) > D_RRH_TRAING_DATA_SIZE ){
			rrhlib_dmsg("parameter error training addr %x \n",((unsigned int)addr));
        	return mrb_false_value();
		}
		data = traData->traVal[(int)addr];
		rrhlib_dmsg("shared memory read complete 0x%x Data 0x%x: DPDA INFO training\n",addr ,data);
	}
	else if( strcmp("E_RRH_SHMID_APL_TRAINING_DATA_VT",RSTRING_PTR(shmstr)) == 0){
	    endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
	            E_RRH_SHMID_APL_TRAINING_DATA,      /* 共有メモリ番号               */
	            (VOID **)&traData,                  /* 共有メモリアドレス           */
	            &errcd                              /* 詳細NGコード                 */
	    );
	    /* 共有メモリの取得失敗時 */
	    if( endcd != BPF_RU_IPCM_OK )
	    {
	        rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_TRAINING_DATA_VT  NG = %d ,errcd = %d \n",endcd,errcd);
	        return mrb_false_value();
	    }
	    if( sscanf( RSTRING_PTR(addrstr), "%lx", &addr) == EOF ){
	        rrhlib_dmsg("Invlid value is detected in addstr : %s\n", RSTRING_PTR(addrstr));
	        return mrb_false_value();
	    }
		if( ((unsigned int)addr) > D_RRH_TRAING_DATA_SIZE ){
			rrhlib_dmsg("parameter error training addr %x \n",((unsigned int)addr));
        	return mrb_false_value();
		}
		data = traData->traVal_vt[(int)addr];
		rrhlib_dmsg("shared memory read complete 0x%x Data 0x%x: VT INFO training\n",addr ,data);
	}
	else if( strcmp("E_RRH_SHMID_APL_DU_PROCESS_INFO",RSTRING_PTR(shmstr)) == 0){
		/* 共有メモリの取得 */
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
			E_RRH_SHMID_APL_DU_PROCESS_INFO,       /* 共有メモリ番号               */
			(VOID **)&tDuProcInfo,              /* 共有メモリアドレス           */
			&errcd                              /* 詳細NGコード                 */
		);
	    /* 共有メモリの取得失敗時 */
	    if( endcd != BPF_RU_IPCM_OK )
	    {
	        rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_DU_PROCESS_INFO  NG = %d ,errcd = %d \n",endcd,errcd);
	        return mrb_false_value();
	    }

		if( strcmp("dbgmode",RSTRING_PTR(addrstr)) == 0){
			data = tDuProcInfo->dbgmode;
		}
		else{
	        rrhlib_dmsg("Memory Get addr:%s NG = %d\n",RSTRING_PTR(addrstr));
	        return mrb_false_value();
		}
		rrhlib_dmsg("shared memory read complete : %s\n",RSTRING_PTR(addrstr));
	}
	else if(strcmp("E_RRH_SHMID_APL_TXCAL_COEF", RSTRING_PTR(shmstr)) == 0) {
		/* 共有メモリの取得 */
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
					E_RRH_SHMID_APL_TXCAL_COEF,			/* 共有メモリ番号				*/
					(VOID **)&cal_coef_tbl,				/* 共有メモリアドレス			*/
					&errcd);							/* 詳細NGコード					*/

		/* 共有メモリの取得失敗時 */
		if( endcd != BPF_RU_IPCM_OK ) {
			rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_TXCAL_COEF NG = %d, errcd = %d \n",endcd, errcd);
			return mrb_false_value();
		}

		if( sscanf( RSTRING_PTR(addrstr), "%lx", &addr) == EOF ){
			rrhlib_dmsg("Invlid value is detected in addrstr : %s\n", RSTRING_PTR(addrstr));
			return mrb_false_value();
		}

		/* 引数判定 */
		if( ((unsigned int)addr) > D_RRH_ANT_MAX){
			rrhlib_dmsg("parameter error : addr %x \n",(unsigned int)addr);
			return mrb_false_value();
		}
		data = cal_coef_tbl->cal_coef[addr];
		rrhlib_dmsg("tx cal coef get OK! ant = %d, coef = %08x \n", addr, data);
	}
	else if(strcmp("E_RRH_SHMID_APL_RXCAL_COEF", RSTRING_PTR(shmstr)) == 0) {
		/* 共有メモリの取得 */
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
					E_RRH_SHMID_APL_RXCAL_COEF,			/* 共有メモリ番号				*/
					(VOID **)&cal_coef_tbl,				/* 共有メモリアドレス			*/
					&errcd);							/* 詳細NGコード					*/

		/* 共有メモリの取得失敗時 */
		if( endcd != BPF_RU_IPCM_OK ) {
			rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_RXCAL_COEF NG = %d, errcd = %d \n",endcd, errcd);
			return mrb_false_value();
		}

		if( sscanf( RSTRING_PTR(addrstr), "%lx", &addr) == EOF ){
			rrhlib_dmsg("Invlid value is detected in addrstr : %s\n", RSTRING_PTR(addrstr));
			return mrb_false_value();
		}

		/* 引数判定 */
		if( ((unsigned int)addr) > D_RRH_ANT_MAX){
			rrhlib_dmsg("parameter error : addr %x \n",(unsigned int)addr);
			return mrb_false_value();
		}
		data = cal_coef_tbl->cal_coef[addr];
		rrhlib_dmsg("rx cal coef get OK! ant = %d, coef = %08x \n", addr, data);
	}
	else if(strcmp("E_RRH_SHMID_APL_BF_MODE_TBL", RSTRING_PTR(shmstr)) == 0) {
		/* 共有メモリの取得 */
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
					E_RRH_SHMID_APL_BF_MODE_TBL,		/* 共有メモリ番号				*/
					(VOID **)&bf_mode_tbl,				/* 共有メモリアドレス			*/
					&errcd);							/* 詳細NGコード					*/

		/* 共有メモリの取得失敗時 */
		if( endcd != BPF_RU_IPCM_OK ) {
			rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_BF_MODE_TBL NG = %d, errcd = %d \n",endcd, errcd);
			return mrb_false_value();
		}
		data = bf_mode_tbl->flag;
		rrhlib_dmsg("bf_mode_tbl->flag get OK! mode = %08x \n", data);
	}
	else{
		rrhlib_dmsg("read kind error\n");
		return mrb_false_value();
	}

	size = snprintf(rtn_str, sizeof(rtn_str), "%08x", data);
	if(size < 0){
		return mrb_false_value();
	}

	return mrb_str_new(mrb, rtn_str, size);
}


/*!
 * @brief        共有メモリトレーニングデータ書き込み処理
 * @note
<pre>
 	共有メモリにトレーニングデータを書き込む処理である.
</pre>
 * @tparam      書き込みアドレス   [in]    Integer
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.bpfshmwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_bpfshm_write(mrb_state* mrb, mrb_value self)
{
	int						endcd;		/* 終了コード                   */
	int						errcd;		/* 詳細NGコード                 */
	mrb_value				shmstr;
	mrb_value				addrstr;
	mrb_value				memstr;
	mrb_value				datastr;
	unsigned int			data;
	unsigned int			addr;

	T_RRH_TRAINING_DATA		*traData;	/* 共有メモリアドレス(tra data) */
	T_RRH_DU_PROCESS_INFO	*tDuProcInfo;
	T_RRH_CAL_COEF_TBL		*cal_coef_tbl;


    mrb_get_args(mrb, "SSSS", &shmstr, &memstr, &addrstr, &datastr );
    if( sscanf( RSTRING_PTR(datastr), "%x", &data) == EOF ){
        rrhlib_dmsg("Invlid value is detected in datastr : %s\n", RSTRING_PTR(datastr));
        return mrb_false_value();
    }

	if( strcmp("E_RRH_SHMID_APL_TRAINING_DATA",RSTRING_PTR(shmstr)) == 0){
		/* 共有メモリの取得 */
	    endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
	            E_RRH_SHMID_APL_TRAINING_DATA,      /* 共有メモリ番号               */
	            (VOID **)&traData,                  /* 共有メモリアドレス           */
	            &errcd                              /* 詳細NGコード                 */
	    );
	    /* 共有メモリの取得失敗時 */
	    if( endcd != BPF_RU_IPCM_OK )
	    {
	        rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_TRAINING_DATA  NG = %d ,errcd = %d \n",endcd,errcd);
	        return mrb_false_value();
	    }

	    if( sscanf( RSTRING_PTR(addrstr), "%x", &addr) == EOF ){
	        rrhlib_dmsg("Invlid value is detected in addrstr : %s\n", RSTRING_PTR(addrstr));
	        return mrb_false_value();
	    }

		/* 引数判定 */
		if( ((unsigned int)addr) > D_RRH_TRAING_DATA_SIZE){
			rrhlib_dmsg("parameter error : addr %x \n",(unsigned int)addr);
			return mrb_false_value();
		}

		/* データの書き込み */
		traData->traVal[addr] = (unsigned char)data;

		rrhlib_dmsg("shared memory write complete addr 0x%x data 0x%x: training\n",addr, data);
	}
	else if( strcmp("E_RRH_SHMID_APL_TRAINING_DATA_VT",RSTRING_PTR(shmstr)) == 0){
		/* 共有メモリの取得 */
	    endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
	            E_RRH_SHMID_APL_TRAINING_DATA,      /* 共有メモリ番号               */
	            (VOID **)&traData,                  /* 共有メモリアドレス           */
	            &errcd                              /* 詳細NGコード                 */
	    );
	    /* 共有メモリの取得失敗時 */
	    if( endcd != BPF_RU_IPCM_OK )
	    {
	        rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_TRAINING_DATA_VT  NG = %d ,errcd = %d \n",endcd,errcd);
	        return mrb_false_value();
	    }

	    if( sscanf( RSTRING_PTR(addrstr), "%x", &addr) == EOF ){
	        rrhlib_dmsg("Invlid value is detected in addrstr : %s\n", RSTRING_PTR(addrstr));
	        return mrb_false_value();
	    }

		/* 引数判定 */
		if( ((unsigned int)addr) > D_RRH_TRAING_DATA_SIZE){
			rrhlib_dmsg("parameter error : addr %x \n",(unsigned int)addr);
			return mrb_false_value();
		}

		/* データの書き込み */
		traData->traVal_vt[addr] = (unsigned char)data;

		rrhlib_dmsg("shared memory write complete addr 0x%x data 0x%x: VT INFO training\n",addr, data);
	}
	else if( strcmp("E_RRH_SHMID_APL_DU_PROCESS_INFO",RSTRING_PTR(shmstr)) == 0){
		/* 共有メモリの取得 */
	    endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
	            E_RRH_SHMID_APL_DU_PROCESS_INFO,      /* 共有メモリ番号               */
	            (VOID **)&tDuProcInfo,                  /* 共有メモリアドレス           */
	            &errcd                              /* 詳細NGコード                 */
	    );
	    /* 共有メモリの取得失敗時 */
	    if( endcd != BPF_RU_IPCM_OK )
	    {
	        rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_DU_PROCESS_INFO  NG = %d ,errcd = %d \n",endcd,errcd);
	        return mrb_false_value();
	    }

		if( strcmp("dbgmode",RSTRING_PTR(addrstr)) == 0){
			tDuProcInfo->dbgmode = data;
		}
		else{
		    rrhlib_dmsg("Memory Get addr:%s NG = %d\n",RSTRING_PTR(addrstr));
		    return mrb_false_value();
		}
		rrhlib_dmsg("shared memory write complete : %s 0x%x\n",RSTRING_PTR(addrstr),data);
	}
	else if(strcmp("E_RRH_SHMID_APL_TXCAL_COEF", RSTRING_PTR(shmstr)) == 0) {
		/* 共有メモリの取得 */
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
					E_RRH_SHMID_APL_TXCAL_COEF,			/* 共有メモリ番号				*/
					(VOID **)&cal_coef_tbl,				/* 共有メモリアドレス			*/
					&errcd);							/* 詳細NGコード					*/

		/* 共有メモリの取得失敗時 */
		if( endcd != BPF_RU_IPCM_OK ) {
			rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_TXCAL_COEF NG = %d, errcd = %d \n",endcd, errcd);
			return mrb_false_value();
		}

		if( sscanf( RSTRING_PTR(addrstr), "%x", &addr) == EOF ){
			rrhlib_dmsg("Invlid value is detected in addrstr : %s\n", RSTRING_PTR(addrstr));
			return mrb_false_value();
		}

		/* 引数判定 */
		if( ((unsigned int)addr) > D_RRH_ANT_MAX){
			rrhlib_dmsg("parameter error : addr %x \n",(unsigned int)addr);
			return mrb_false_value();
		}
		cal_coef_tbl->cal_coef[addr] = data;
		rrhlib_dmsg("tx cal coef set OK! ant = %d, coef = %08x set_val=%08x \n",
					addr, cal_coef_tbl->cal_coef[addr], data);
	}
	else if(strcmp("E_RRH_SHMID_APL_RXCAL_COEF", RSTRING_PTR(shmstr)) == 0) {
		/* 共有メモリの取得 */
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
					E_RRH_SHMID_APL_RXCAL_COEF,			/* 共有メモリ番号				*/
					(VOID **)&cal_coef_tbl,				/* 共有メモリアドレス			*/
					&errcd);							/* 詳細NGコード					*/

		/* 共有メモリの取得失敗時 */
		if( endcd != BPF_RU_IPCM_OK ) {
			rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_RXCAL_COEF NG = %d, errcd = %d \n",endcd, errcd);
			return mrb_false_value();
		}

		if( sscanf( RSTRING_PTR(addrstr), "%x", &addr) == EOF ){
			rrhlib_dmsg("Invlid value is detected in addrstr : %s\n", RSTRING_PTR(addrstr));
			return mrb_false_value();
		}

		/* 引数判定 */
		if( ((unsigned int)addr) > D_RRH_ANT_MAX){
			rrhlib_dmsg("parameter error : addr %x \n",(unsigned int)addr);
			return mrb_false_value();
		}
		cal_coef_tbl->cal_coef[addr] = data;
		rrhlib_dmsg("rx cal coef set OK! ant = %d, coef = %08x set_val=%08x \n",
					addr, cal_coef_tbl->cal_coef[addr], data);
	}
	else{
		rrhlib_dmsg("write kind error\n");
		return mrb_false_value();
	}

    return mrb_true_value();
}


/*!
 * @brief        PTP-PLL config設定処理
 * @note
<pre>
 	PTP-PLL configファイルを読出し、初期設定を行う処理である。
</pre>
 * @tparam      configファイルパス   [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/09
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.ptpconf_set
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_ptpconf_set(mrb_state* mrb, mrb_value self)
{
	mrb_value			file_name;

	char				flag = 0;

	FILE				*fp;
	char				read_data[128];
	char				kind;
	unsigned int		addr;
	unsigned int		data;
	unsigned int		time;

	struct timespec		stime;

	unsigned int		temp;
	unsigned char		page = 0xFF;
	unsigned short		regaddr;


	mrb_get_args(mrb, "S", &file_name );

	fp = fopen( RSTRING_PTR(file_name), "r" );

	if( fp == NULL ) {
		rrhlib_dmsg("File open error : %s \n", RSTRING_PTR(file_name));
		return mrb_false_value();
	}

	while(1) {
		memset(read_data, 0, sizeof(read_data));

		/* 1行分読出し*/
		if( fgets(read_data, 127, fp) == NULL ) {
			rrhlib_dmsg("File fgets error : %s \n", RSTRING_PTR(file_name));
			break;
		}
#if 0
		rrhlib_dmsg("File fgets data     : %s", read_data);
#endif
		if( strstr(read_data, "Register Configuration End" ) != 0 ) {
			rrhlib_dmsg("File fgets end \n");
			break;
		}

		if( strstr(read_data, "Register Configuration Start" ) != 0 ) {
			flag++;
			if( flag == 2 ) {
				rrhlib_dmsg("PTP Config Start    : flag = %x \n", flag);
			}
		}
		if( flag < 2 ) {
			continue;
		}

		/* register set	*/
		if( strstr(read_data, "X") != 0 ) {
			sscanf( read_data, "%s , %x , %x", &kind, &addr, &data );
			rrhlib_dmsg("File fgets reg set  : kind = X, addr = %04x, data = %02x \n", addr, data);

			temp = ((addr & 0x00000F80) >> 7);

			if( page != (unsigned char)temp ) {
				page = (unsigned char)temp;

				BPF_HM_DEVC_PTP_WRITE( 0x007F, page);
				rrhlib_dmsg("           page set : addr = 007F, page = %02x \n", page);
			}
			regaddr = ((unsigned short)addr) & 0x007F;

			BPF_HM_DEVC_PTP_WRITE( regaddr, (unsigned char)data);
			rrhlib_dmsg("           reg set  : addr = %04x, data = %02x \n", regaddr, data);
		}
		/* usec wait	*/
		if( strstr(read_data, "W") != 0 ) {
			sscanf( read_data, "%s , %d", &kind, &time );

			stime.tv_sec  = (time - (time % 1000000)) / 1000000;
			stime.tv_nsec = (time - (stime.tv_sec * 1000000)) * 1000;
			nanosleep( &stime, NULL );

			rrhlib_dmsg("File fgets wait     : kind = W, time = %d \n", time);
		}
	}
	return mrb_true_value();
}

/*!
 * @brief        PGC通常書込み
 * @note
<pre>
     指定されたPGCのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		グループ		[in]	Integer
 * @tparam		PGCアドレス	[in]	Integer
 * @tparam		SRN		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/09 wang
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.pgcwrite
 * @warning     RFPLLアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_pgc_write(mrb_state* mrb, mrb_value self)
{

	mrb_int grp;
	mrb_int txrx;
	mrb_int gain1;
	mrb_int phase1;
	mrb_int gain2;
	mrb_int phase2;
	mrb_int gain3;
	mrb_int phase3;
	mrb_int gain4;
	mrb_int phase4;


	int iRtn;
	mrb_get_args(mrb, "iiiiiiiiii", &grp, &txrx, &gain1, &phase1, &gain2, &phase2, &gain3, &phase3, &gain4, &phase4 );


	if(( iRtn = BPF_HM_DEVC_PGC_GAIN_PHASE_WRITE((UCHAR)grp, (UCHAR)txrx, gain1, phase1, gain2, phase2, gain3, phase3, gain4, phase4))
	 != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("pgcw write error, errcode = %d , grp = %d,txrx = %d 1:0x%x,0x%x 2:0x%x,0x%x 3:0x%x.0x%x 4:0x%x.0x%x  \n", iRtn,grp, txrx, gain1, phase1, gain2, phase2, gain3, phase3, gain4, phase4);
		return mrb_false_value();
	}
	rrhlib_dmsg("pgcw write grp = %d,txrx = %d 1:0x%x,0x%x 2:0x%x,0x%x 3:0x%x.0x%x 4:0x%x.0x%x  \n", grp, txrx, gain1, phase1, gain2, phase2, gain3, phase3, gain4, phase4);
	return mrb_true_value();
}


/*!
 * @brief        PGC通常読み出し処理
 * @note
<pre>
     指定されたPGCのアドレスからデータを読み出す処理である.
</pre>
 * @tparam		グループ		[in]	Integer
 * @tparam		PGCアドレス	[in]	Integer
 * @tparam		SRN		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/15 wang
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.pgcread
 * @warning     RFPLLアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_pgc_read(mrb_state* mrb, mrb_value self)
{

	mrb_int grp;
	mrb_int txrx;
	int iRtn;

	unsigned int gain1,phase1,reg1;
	unsigned int gain2,phase2,reg2;
	unsigned int gain3,phase3,reg3;
	unsigned int gain4,phase4,reg4;

	mrb_get_args(mrb, "ii", &grp, &txrx);

	if(( iRtn = BPF_HM_DEVC_PGC_GAIN_PHASE_READ((UCHAR)grp, (UCHAR)txrx,&gain1, &phase1,&reg1,
																		&gain2, &phase2,&reg2,
																		&gain3, &phase3,&reg3,
																		&gain4, &phase4,&reg4) ) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("pgcw read error, errcode = %d , grp = %d,txrx = %d 1:0x%x,0x%x,0x%x 2:0x%x,0x%x,0x%x 3:0x%x,0x%x,0x%x 4:0x%x,0x%x,0x%x \n",
		 iRtn,grp, txrx, gain1, phase1,reg1, gain2, phase2,reg2, gain3, phase3,reg3, gain4, phase4,reg4);
		return mrb_false_value();
	}

	printf("\n");
	printf("1/4@Chain\n");
	printf("Ch3-0 Gain       : 0x%08x\n",gain1);
	printf("Ch3-0 Phase      : 0x%08x\n",phase1);
	printf("ReadBack Command : 0x%08x\n",reg1);
	printf("\n");
	printf("2/4@Chain\n");
	printf("Ch3-0 Gain       : 0x%08x\n",gain2);
	printf("Ch3-0 Phase      : 0x%08x\n",phase2);
	printf("ReadBack Command : 0x%08x\n",reg2);
	printf("\n");
	printf("3/4@Chain\n");
	printf("Ch3-0 Gain       : 0x%08x\n",gain3);
	printf("Ch3-0 Phase      : 0x%08x\n",phase3);
	printf("ReadBack Command : 0x%08x\n",reg3);
	printf("\n");
	printf("4/4@Chain\n");
	printf("Ch3-0 Gain       : 0x%08x\n",gain4);
	printf("Ch3-0 Phase      : 0x%08x\n",phase4);
	printf("ReadBack Command : 0x%08x\n",reg4);
	printf("\n");

	return mrb_true_value();
}


/*!
 * @brief        PGC通常書込み
 * @note
<pre>
     指定されたPGCのアドレスにデータを書き込む処理である.
</pre>
 * @tparam		グループ		[in]	Integer
 * @tparam		PGCアドレス	[in]	Integer
 * @tparam		SRN		[in]	Integer
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/09 wang
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.pgcwrite
 * @warning     RFPLLアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_pgc_write_reg(mrb_state* mrb, mrb_value self)
{

	mrb_int grp;
	mrb_int gain1;
	mrb_int gain2;
	mrb_int gain3;
	mrb_int gain4;

	int iRtn;
	mrb_get_args(mrb, "iiiii", &grp, &gain1, &gain2, &gain3,&gain4);

	if(( iRtn = BPF_HM_DEVC_PGC_WRITE2((UCHAR)grp, gain1, gain2, gain3,gain4))
	 != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("pgcw write error, errcode = %d , grp = %d,1:0x%x 2:0x%x 3:0x%x 4:0x%x\n", iRtn,grp, gain1, gain2, gain3,gain4);
		return mrb_false_value();
	}
	rrhlib_dmsg("pgcw write  grp = %d,1:0x%x 2:0x%x 3:0x%x 4:0x%x\n", iRtn, gain1, gain2, gain3,gain4);
	return mrb_true_value();
}


/*!
 * @brief        PGC通常読み出し処理
 * @note
<pre>
     指定されたPGCのアドレスからデータを読み出す処理である.
</pre>
 * @tparam		グループ		[in]	Integer
 * @tparam		PGCアドレス	[in]	Integer
 * @tparam		SRN		[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2018/08/15 wang
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.pgcread
 * @warning     RFPLLアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_pgc_read_reg(mrb_state* mrb, mrb_value self)
{

	mrb_int grp;
	mrb_int bank;
	int iRtn;

	unsigned int gain1,phase1,reg1;
	unsigned int gain2,phase2,reg2;
	unsigned int gain3,phase3,reg3;
	unsigned int gain4,phase4,reg4;

	mrb_get_args(mrb, "ii", &grp, &bank);

	if(( iRtn = BPF_HM_DEVC_PGC_READ2((UCHAR)grp, (UCHAR)bank,	&gain1, &phase1,&reg1,
	                                                            &gain2, &phase2,&reg2,
	                                                            &gain3, &phase3,&reg3,
	                                                            &gain4, &phase4,&reg4 )) != BPF_HM_DEVC_COMPLETE ){
		rrhlib_dmsg("pgcw read error, errcode = %d , grp = %d,bank = %d 1:0x%x,0x%x,0x%x 2:0x%x,0x%x,0x%x 3:0x%x,0x%x,0x%x 4:0x%x,0x%x,0x%x \n",
						 iRtn,grp, bank, gain1, phase1,reg1, gain2, phase2,reg2, gain3, phase3,reg3, gain4, phase4,reg4);
		return mrb_false_value();
	}

	printf("\n");
	printf("1/4@Chain\n");
	printf("DO1,DO0 (0xA200_0A50) = 0x%08x\n",gain1);
	printf("DO3,DO2 (0xA200_0A54) = 0x%08x\n",phase1);
	printf("DO4(0xA200_0A58)      = 0x%08x\n",reg1);
	printf("\n");
	printf("2/4@Chain\n");
	printf("DO1,DO0 (0xA200_0A60) = 0x%08x\n",gain2);
	printf("DO3,DO2(0xA200_0A64)  = 0x%08x\n",phase2);
	printf("DO4(0xA200_0A68)      = 0x%08x\n",reg2);
	printf("\n");
	printf("3/4@Chain\n");
	printf("DO1,DO0 (0xA200_0A70) = 0x%08x\n",gain3);
	printf("DO3,DO2(0xA200_0A74)  = 0x%08x\n",phase3);
	printf("DO4(0xA200_0A78)      = 0x%08x\n",reg3);
	printf("\n");
	printf("4/4@Chain\n");
	printf("DO1,DO0 (0xA200_0A80) = 0x%08x\n",gain4);
	printf("DO3,DO2(0xA200_0A84)  = 0x%08x\n",phase4);
	printf("DO4(0xA200_0A88)      = 0x%08x\n",reg4);
	printf("\n");
	return mrb_true_value();
}

/*!
 * @brief        共有メモリフラグ書き込み処理
 * @note
<pre>
 	共有メモリに指定されたフラグの情報を書き込む処理である.
</pre>
 * @tparam      書き込み種別       [in]    String
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmflgwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_shm_dpda_flg_write(mrb_state* mrb, mrb_value self)
{
	mrb_value 		kind_str;			/* 書き込み種別 */
	mrb_int			data;				/* 書き込みデータ */
	T_DPDA_INFO_ALL	*buffer;			/* 共有メモリアドレス           */
	int				endcd;				/* 終了コード                   */
	int				errcd;				/* 詳細NGコード                 */

	mrb_get_args(mrb, "Si", &kind_str, &data );

	/* 共有メモリの取得 */
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
	        E_RRH_SHMID_APL_DPDA_MNG_TBL,       /* 共有メモリ番号               */
	        (VOID **)&buffer,                   /* 共有メモリアドレス           */
	        &errcd                              /* 詳細NGコード                 */
	);
	/* 共有メモリの取得失敗時 */
	if( endcd != BPF_RU_IPCM_OK ){
		rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_DPDA_MNG_TBL  NG = %d ,errcd = %d \n",endcd,errcd);
		return mrb_false_value();
	}
	/* データ判定 */
	if( (D_SYS_ON != (unsigned short)data) && (D_SYS_OFF != (unsigned short)data)  ){
		rrhlib_dmsg("parameter error : data \n");
		return mrb_false_value();
	}
	/* データの書き込み */
	if( strcmp("dbgmode",RSTRING_PTR(kind_str)) == 0){
		buffer->taskInfo.flg.dbgmode = (unsigned short)data;
		rrhlib_dmsg("shared memory write complete : flg.dbgmode %d \n",data);
	}
	else if( strcmp("temperature",RSTRING_PTR(kind_str)) == 0){
		buffer->taskInfo.flg.temperature = (unsigned short)data;
		rrhlib_dmsg("shared memory write complete : flg.temperature %d \n",data);
	}
	else if( strcmp("almmsk",RSTRING_PTR(kind_str)) == 0){
		buffer->almmsk = (unsigned int)data;
		rrhlib_dmsg("shared memory write complete : almmsk %d\n",data);
	}
	else{
		rrhlib_dmsg("parameter error : kind %s\n",RSTRING_PTR(kind_str));
		return mrb_false_value();
	}
	return mrb_true_value();
}

/*!
 * @brief        共有メモリフラグ書き込み処理
 * @note
<pre>
 	共有メモリに指定されたフラグの情報を書き込む処理である.
</pre>
 * @tparam      書き込み種別       [in]    String
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmflgwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_calphase(mrb_state* mrb, mrb_value self)
{
	mrb_int 			kind;				/* 書き込み種別 */
	mrb_int 			antofs;				/* 書き込み種別 */
	mrb_value			data;				/* 書き込みデータ */
	T_RRH_CAL_COEF_TBL	*coef_tbl;			/* 共有メモリアドレス           */
	int					endcd;				/* 終了コード                   */
	int					errcd;				/* 詳細NGコード                 */
	DL					dldata;

	mrb_get_args(mrb, "iiS", &kind,&antofs, &data );

	rrhlib_dmsg("start %d %d %s\n",kind, antofs,RSTRING_PTR(data));

	if(kind == 0){
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_TXCAL_COEF,						/* 共有メモリ番号		*/
						(VOID **)&coef_tbl				,				/* 共有メモリアドレス	*/
						&errcd);
	}else if(kind == 1){
		endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_RXCAL_COEF,						/* 共有メモリ番号		*/
						(VOID **)&coef_tbl				,				/* 共有メモリアドレス	*/
						&errcd);
	}else{
		rrhlib_dmsg("Invlid kind %d\n", kind);
		return mrb_false_value();
	}
	/* 共有メモリの取得失敗時 */
	if( endcd != BPF_RU_IPCM_OK ){
		rrhlib_dmsg("Memory Get key:E_RRH_SHMID_APL_TXCAL_COEF or E_RRH_SHMID_APL_RXCAL_COEF  NG = %d ,errcd = %d %d %d\n",endcd,errcd,kind,antofs);
		return mrb_false_value();
	}

	if(antofs >=  D_RRH_ANT_MAX){
		rrhlib_dmsg("Invlid antofs %d\n", antofs);
		return mrb_false_value();
	}


	if( sscanf( RSTRING_PTR(data), "%lf", &dldata) == EOF ){
		rrhlib_dmsg("Invlid value %s\n", RSTRING_PTR(data));
		return mrb_false_value();
	}

	coef_tbl->phase[antofs] = dldata;
	return mrb_true_value();
}


/*!
 * @brief        共有メモリフラグ書き込み処理
 * @note
<pre>
 	共有メモリに指定されたフラグの情報を書き込む処理である.
</pre>
 * @tparam      書き込み種別       [in]    String
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmflgwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_getrtc(mrb_state* mrb, mrb_value self)
{

	char					rtn_str[64];
	int						size;
	T_RRH_SYSTEM_TIME		sysTime;

	memset(rtn_str,0,sizeof(rtn_str));
	memset(&sysTime,0,sizeof(sysTime));

	BPF_HM_DEVC_GETTIME( &sysTime );

	size = snprintf( rtn_str, sizeof(rtn_str),
				"%04d/%02d/%02d %02d:%02d:%02d:%02d0",
					sysTime.year,
					sysTime.month,
					sysTime.day,
					sysTime.hour,
					sysTime.min,
					sysTime.sec,
					sysTime.msec) ;
	if(size < 0){
		return mrb_false_value();
	}
	return mrb_str_new(mrb, rtn_str, size);
}

/*!
 * @brief        共有メモリフラグ書き込み処理
 * @note
<pre>
 	共有メモリに指定されたフラグの情報を書き込む処理である.
</pre>
 * @tparam      書き込み種別       [in]    String
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.shmflgwrite
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_setrtc(mrb_state* mrb, mrb_value self)
{
	mrb_value 		time_str;			/* 書き込み時刻 */
	T_RRH_SYSTEM_TIME		sysTime;
	INT					val[6];

	memset(&sysTime,0,sizeof(sysTime));
	memset(val,0,sizeof(val));

	mrb_get_args(mrb, "S", &time_str );
	sscanf( RSTRING_PTR(time_str), "%04d-%02d-%02d %02d:%02d:%02d", &val[0], &val[1],&val[2],&val[3],&val[4],&val[5] );

	sysTime.year  = (USHORT)val[0];
	sysTime.month =  (UCHAR)val[1];
	sysTime.day   =  (UCHAR)val[2];
	sysTime.hour  =  (UCHAR)val[3];
	sysTime.min   =  (UCHAR)val[4];
	sysTime.sec   =  (UCHAR)val[5];

	rrhlib_dmsg("Set:%s %04d-%02d-%02d %02d:%02d:%02d\n", RSTRING_PTR(time_str),sysTime.year, sysTime.month,sysTime.day,sysTime.hour,sysTime.min,sysTime.sec);
	BPF_HM_DEVC_SETTIME(sysTime);
	return mrb_true_value();
}


/*!
 * @brief        mutex強制的に開放処理
 * @note
<pre>
 	mutexを強制的に開放する処理である
</pre>
 * @tparam      書き込み種別       [in]    String
 * @tparam      書き込みデータ     [in]    Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2013/12/16
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.mutexfree
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_mutexfree(mrb_state* mrb, mrb_value self)
{
	rrhlib_dmsg("mutex free\n");
	BPF_HM_DEVC_MUTEX_LOCK_FREE();
	return mrb_true_value();
}


#endif

/*!
 * @brief        FH Fail Safe(共有メモリ)書き換え処理
 * @note
<pre>
     指定されたデータ値でFH Fail Safe(共有メモリ)に書き込む処理である.
</pre>
 * @tparam		データ			[in]	Integer
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2020/05/22 koshida
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.setFHfsOffFlg
 * @warning     RFICアクセスの引数が数値であることに注意
 * @FeatureID   N/A
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 */
mrb_value rrhlib_device_setFHfsOffFlg(mrb_state* mrb, mrb_value self)
{
	mrb_int			data;				/* 書き込みデータ		*/
	UINT			*fhfsOffFlg;		/* 共有メモリアドレス	*/
	int				errcd;				/* 詳細NGコード			*/
	int				endcd;				/* 終了コード			*/

    mrb_get_args(mrb, "i", &data );

	rrhlib_dmsg("start FH fail safe off flg=%d\n", data);

	/*	共有メモリのアドレスを取得する	*/
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SFPFS_FLG, (VOID **)&fhfsOffFlg, &errcd);
	if( endcd != BPF_RU_IPCM_OK ){
		rrhlib_dmsg("shareMemory Get NG = %d ,errcd = %d\n",endcd,errcd);
		return mrb_false_value();
	}
	/*	共有メモリにデータ値を設定する	*/
	*fhfsOffFlg = data;

	rrhlib_dmsg("complete FH fail safe off flg=%d\n", data);
	return mrb_true_value();
}

/*!
 * @brief        TPコマンド用デバイスアクセス系モジュール登録処理
 * @note
<pre>
    TPコマンド用のデバイスアクセス系モジュールを登録する関数である.
</pre>
 * @param        mrb                [in]    mrb status
 * @param        module_class    [in]    module_class
 * @return        N/A
 * @date        2013/11/26
 * @warning        N/A
 * @FeatureID    N/A
 * @Bug_No        N/A
 * @CR_No        N/A
 * @TBD_No        N/A
 */
void rrhlib_device_access_install(mrb_state* mrb ,struct RClass * module_class)
{
    rrhlib_dmsg("rrh lib device access install start \n");

    /* scriptで使用できるモジュールを登録する */
    mrb_define_module_function(mrb, module_class, "regread"     ,  rrhlib_device_register_read   , MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb, module_class, "regwrite"    ,  rrhlib_device_register_write  , MRB_ARGS_REQ(2));
    mrb_define_module_function(mrb, module_class, "regbiton"    ,  rrhlib_device_register_biton  , MRB_ARGS_REQ(2));
    mrb_define_module_function(mrb, module_class, "regbitoff"   ,  rrhlib_device_register_bitoff , MRB_ARGS_REQ(2));

    /* qspi */
    mrb_define_module_function(mrb, module_class, "flashread_b"   ,rrhlib_device_qspi_flash_read_b ,MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb, module_class, "flashread"   ,  rrhlib_device_qspi_flash_read , MRB_ARGS_REQ(3));
    mrb_define_module_function(mrb, module_class, "flashwrite"  ,  rrhlib_device_qspi_flash_write, MRB_ARGS_REQ(4));
    mrb_define_module_function(mrb, module_class, "flasherase"  ,  rrhlib_device_qspi_flash_erase, MRB_ARGS_REQ(3));
    mrb_define_module_function(mrb, module_class, "flashread_btver"   ,rrhlib_device_qspi_flash_read_btver ,MRB_ARGS_REQ(1));

    /* i2c */
    mrb_define_module_function(mrb, module_class, "sfpread"     ,  rrhlib_device_sfp_read        , MRB_ARGS_REQ(4));
    mrb_define_module_function(mrb, module_class, "sfpwrite"    ,  rrhlib_device_sfp_write       , MRB_ARGS_REQ(3));
    mrb_define_module_function(mrb, module_class, "eepread"     ,  rrhlib_device_eeprom_read     , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "eepread_txt" ,  rrhlib_device_eeprom_read_txt , MRB_ARGS_REQ(2));
    mrb_define_module_function(mrb, module_class, "eepwrite"    ,  rrhlib_device_eeprom_write    , MRB_ARGS_REQ(2));
    mrb_define_module_function(mrb, module_class, "tempread"    ,  rrhlib_device_temp_read       , MRB_ARGS_NONE());
    mrb_define_module_function(mrb, module_class, "tempread2"    ,  rrhlib_device_temp_read2       , MRB_ARGS_NONE());
    mrb_define_module_function(mrb, module_class, "mmiread"     ,  rrhlib_device_mmipld_read     , MRB_ARGS_REQ(1));
    mrb_define_module_function(mrb, module_class, "mmiwrite"    ,  rrhlib_device_mmipld_write    , MRB_ARGS_REQ(2));

	/* spi */
	mrb_define_module_function(mrb, module_class, "cpld0read"  ,  rrhlib_device_cpld0_read    , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "cpld0write" ,  rrhlib_device_cpld0_write   , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "cpld1read"  ,  rrhlib_device_cpld1_read    , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "cpld1write" ,  rrhlib_device_cpld1_write   , MRB_ARGS_REQ(2));

	mrb_define_module_function(mrb, module_class, "txrfread"   ,  rrhlib_device_txrf_read     , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "txrfwrite"  ,  rrhlib_device_txrf_write    , MRB_ARGS_REQ(3));

	mrb_define_module_function(mrb, module_class, "rxrfread"   ,  rrhlib_device_rxrf_read      , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "rxrfwrite"  ,  rrhlib_device_rxrf_write     , MRB_ARGS_REQ(3));


	mrb_define_module_function(mrb, module_class, "rficread"   ,  rrhlib_device_rfic_read     , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "rficwrite"  ,  rrhlib_device_rfic_write    , MRB_ARGS_REQ(3));

	mrb_define_module_function(mrb, module_class, "pllread"      ,  rrhlib_device_pll_read      , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "pllwrite"     ,  rrhlib_device_pll_write     , MRB_ARGS_REQ(2));

	mrb_define_module_function(mrb, module_class, "ptpread"      ,  rrhlib_device_ptp_read      , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "ptpwrite"     ,  rrhlib_device_ptp_write     , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "ptpread2"      ,  rrhlib_device_ptp_read2      , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "ptpwrite2"     ,  rrhlib_device_ptp_write2     , MRB_ARGS_REQ(2));

//C9     mrb_define_module_function(mrb, module_class, "bpfshmread",    rrhlib_bpfshm_read, MRB_ARGS_REQ(3));
//     mrb_define_module_function(mrb, module_class, "bpfshmwrite",   rrhlib_bpfshm_write, MRB_ARGS_REQ(4));

//     mrb_define_module_function(mrb, module_class, "ptpconf_set",   rrhlib_ptpconf_set, MRB_ARGS_REQ(1));

// 	mrb_define_module_function(mrb, module_class, "qmodread"      ,  rrhlib_device_qmod_read      , MRB_ARGS_REQ(3));
// 	mrb_define_module_function(mrb, module_class, "qmodwrite"     ,  rrhlib_device_qmod_write     , MRB_ARGS_REQ(4));
// 	mrb_define_module_function(mrb, module_class, "qdemread"      ,  rrhlib_device_qdem_read      , MRB_ARGS_REQ(3));
// 	mrb_define_module_function(mrb, module_class, "qdemwrite"     ,  rrhlib_device_qdem_write     , MRB_ARGS_REQ(4));
// 	mrb_define_module_function(mrb, module_class, "rfpllread"      ,  rrhlib_device_rfpll_read      , MRB_ARGS_REQ(1));
// 	mrb_define_module_function(mrb, module_class, "rfpllwrite"     ,  rrhlib_device_rfpll_write     , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "rferead"      ,  rrhlib_device_rfe_read      , MRB_ARGS_REQ(1));
// 	mrb_define_module_function(mrb, module_class, "rfewrite"     ,  rrhlib_device_rfe_write     , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "rxadcread"      ,  rrhlib_device_rxadc_read      , MRB_ARGS_REQ(1));
// 	mrb_define_module_function(mrb, module_class, "rxadcwrite"     ,  rrhlib_device_rxadc_write     , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "txdacread"      ,  rrhlib_device_txdac_read      , MRB_ARGS_REQ(1));
// 	mrb_define_module_function(mrb, module_class, "txdacwrite"     ,  rrhlib_device_txdac_write     , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "srxvread"      ,  rrhlib_device_srxv_read      , MRB_ARGS_REQ(1));
// 	mrb_define_module_function(mrb, module_class, "srxvwrite"     ,  rrhlib_device_srxv_write     , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "stxvread"      ,  rrhlib_device_stxv_read      , MRB_ARGS_REQ(1));
// 	mrb_define_module_function(mrb, module_class, "stxvwrite"     ,  rrhlib_device_stxv_write     , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "setbias"     ,  rrhlib_device_set_bias     , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "pgcwrite"     ,  rrhlib_device_pgc_write     , MRB_ARGS_REQ(10));
// 	mrb_define_module_function(mrb, module_class, "pgcread"     ,  rrhlib_device_pgc_read     , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "pgcwrite_reg"     ,  rrhlib_device_pgc_write_reg     , MRB_ARGS_REQ(5));
// 	mrb_define_module_function(mrb, module_class, "pgcread_reg"     ,  rrhlib_device_pgc_read_reg     , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "fandac_w"    ,  rrhlib_device_fandac_write	 , MRB_ARGS_REQ(1));	/*	5G-商用DU	*/

// 	/* etc */
// 	mrb_define_module_function(mrb, module_class, "getkind"     ,  rrhlib_kind_get      		 , MRB_ARGS_NONE());
// 	mrb_define_module_function(mrb, module_class, "setkind"     ,  rrhlib_kind_set      		 , MRB_ARGS_REQ(1));

// 	mrb_define_module_function(mrb, module_class, "irqwait"     ,  rrhlib_irqwait        		 , MRB_ARGS_REQ(1));

// 	mrb_define_module_function(mrb, module_class, "adjpawrt"    ,  rrhlib_device_adjpawrt		, MRB_ARGS_REQ(3));
// 	mrb_define_module_function(mrb, module_class, "getFanInfo"    ,  rrhlib_fanInfo_get			 , MRB_ARGS_REQ(1));
// 	/* 2ndリリース TPコマンド */
// 	mrb_define_module_function(mrb, module_class, "svtxwrite"     ,  rrhlib_device_svtx    		 , MRB_ARGS_REQ(3));
// 	mrb_define_module_function(mrb, module_class, "svrxwrite"     ,  rrhlib_device_svrx    		 , MRB_ARGS_REQ(3));
// 	mrb_define_module_function(mrb, module_class, "svpawrite"     ,  rrhlib_device_svpa    		 , MRB_ARGS_REQ(2));
// 	mrb_define_module_function(mrb, module_class, "apddbgon"      ,  rrhlib_device_apddbgon		 , MRB_ARGS_NONE());
// 	mrb_define_module_function(mrb, module_class, "apddbgof"      ,  rrhlib_device_apddbgof		 , MRB_ARGS_NONE());
// 	mrb_define_module_function(mrb, module_class, "trnspower"     ,  rrhlib_device_power   		 , MRB_ARGS_NONE());
// 	mrb_define_module_function(mrb, module_class, "carrsetparam"  ,  rrhlib_device_carrsetparam	 , MRB_ARGS_REQ(10));
// 	mrb_define_module_function(mrb, module_class, "carrparaminf"  ,  rrhlib_device_carrparaminf	 , MRB_ARGS_REQ(1));
// 	mrb_define_module_function(mrb, module_class, "DataTxGen"     ,  rrhlib_device_DataTxGen	 , MRB_ARGS_REQ(2));

//     mrb_define_module_function(mrb, module_class, "shmflgwrite"  , rrhlib_shm_dpda_flg_write     , MRB_ARGS_REQ(2));

// #ifndef OPT_RRH_CT
// 	/* 実機のみ実行可能 */
// 	mrb_define_module_function(mrb, module_class, "devinit"     ,  rrhlib_device_init            , MRB_ARGS_NONE());
// 	mrb_define_module_function(mrb, module_class, "devcfg"      ,  rrhlib_device_config          , MRB_ARGS_REQ(1));
// 	mrb_define_module_function(mrb, module_class, "eepdrv"      ,  rrhlib_device_eepdrv          , MRB_ARGS_REQ(3));
// 	mrb_define_module_function(mrb, module_class, "regdrv"      ,  rrhlib_device_regdrv          , MRB_ARGS_REQ(3));
// #endif

// 	mrb_define_module_function(mrb, module_class, "get_ptpapr"  ,  rrhlib_ptpapr_get      		 , MRB_ARGS_REQ(1));
// 	mrb_define_module_function(mrb, module_class, "set_ptpapr"  ,  rrhlib_ptpapr_set      		 , MRB_ARGS_REQ(2));


// 	mrb_define_module_function(mrb, module_class, "set_calphase"  ,  rrhlib_device_calphase        , MRB_ARGS_REQ(3));

// 	mrb_define_module_function(mrb, module_class, "getrtc"     ,  rrhlib_device_getrtc   		 , MRB_ARGS_NONE());
// 	mrb_define_module_function(mrb, module_class, "setrtc"     ,  rrhlib_device_setrtc   		 , MRB_ARGS_REQ(1));

// 	mrb_define_module_function(mrb, module_class, "mutexfree"    ,  rrhlib_device_mutexfree   		 , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "setFHfsOffFlg",  rrhlib_device_setFHfsOffFlg	 , MRB_ARGS_REQ(1));

	rrhlib_dmsg("rrh lib device access install OK \n");
	return;
}

/* @} */

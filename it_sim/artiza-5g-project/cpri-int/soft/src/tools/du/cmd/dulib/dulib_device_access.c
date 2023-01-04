/*!
 *  @skip    $Id:$
 *  @file    dulib_device_access.c
 *  @brief   du command file library of device_access for tp
 *  @author  FJT)Harada
 *  @date    2013/11/29 by FJT)Harada create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup    RRH_TP_CMD_LIB
 * @{
 */
#include "dulib_local.h"

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
	if( (unsigned short)addr >= D_RRH_I2C_EEP_DATANUM ){
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
	if( (unsigned short)addr >= D_RRH_I2C_EEP_DATANUM ){
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
 * @brief       EEPROM書き込み処理(txt)
 * @note
<pre>
     指定されたEEPROMのアドレスにtxtデータを書き込む処理である.
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
mrb_value rrhlib_device_eeprom_write_txt(mrb_state* mrb, mrb_value self)
{
    mrb_int addr;
    mrb_int len;
    mrb_value data_str;
    unsigned char c_data;
    int iRtn;
	mrb_int			count;
	char*		txt_str;

	mrb_get_args(mrb, "iiS", &addr, &len ,&data_str);
	
	/* アドレス範囲判定 */
	if( (unsigned short)addr >= D_RRH_I2C_EEP_DATANUM ){
		rrhlib_dmsg("eeprom address error : <addr> = 0~7FF 0x%x\n",addr);
		return mrb_false_value();
	}
	/* データ範囲判定 */
	if( (unsigned short)(addr + len) >= D_RRH_I2C_EEP_DATANUM ){
		rrhlib_dmsg("eeprom len error 0x%x \n",len);
		return mrb_false_value();
	}

	txt_str = RSTRING_PTR(data_str);
	if( strlen(txt_str) != len){
		rrhlib_dmsg("len mismatch error 0x%x : %s \n",len,txt_str);
		return mrb_false_value();
	}

	for(count=0 ; count < len ; count++ , txt_str++){
		if( (*txt_str < 0x20) || (*txt_str > 0x7e) ){
			rrhlib_dmsg("Not ascii detected 0x%x \n",*txt_str);
			return mrb_false_value();
		}
	}

	txt_str = RSTRING_PTR(data_str);
	for(count=0 ; count < len ; count++ , txt_str++){
		c_data = (unsigned char)(*txt_str);
		iRtn = BPF_HM_DEVC_EEPROM_WRITE( (unsigned short)(addr + count) , &c_data);
		if (iRtn != BPF_HM_DEVC_COMPLETE){
			rrhlib_dmsg("eeprom write error, errcode = %d , addr = 0x%04hx \n", iRtn, (unsigned short)(addr + count),c_data);
			return mrb_false_value();
		}
	}
	rrhlib_dmsg("eeprom write finish : addr = 0x%04hx, len = 0x%04hx txt:%s \n", (unsigned short)addr, (unsigned short)len, RSTRING_PTR(data_str));
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
	   ( kind != BPF_HM_DEVC_SFP_A2 ) &&
	   ( kind != BPF_HM_DEVC_SFP_SA0 ) && 
	   ( kind != BPF_HM_DEVC_SFP_SA2 )
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
 * @brief		Ether Event処理
 * @note		eth0向けにEventメッセージを送信する
 * @tparam		イベントメッセージ	[in]	String
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2019/11/29 FJT)Taniguchi Create
 * @warning		本関数はスクリプト上でRrh.event()として使用できる.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_event(mrb_state* mrb, mrb_value self)
{
	mrb_value	eventMsg;
	UINT		readdat;
	UCHAR		hwSrcAddr[6];
	UCHAR		hwDstAddr[6];
	CHAR		dp_header[32];
	int			sd;
	INT			length;
	CHAR		value[1024];

	mrb_get_args(mrb, "S", &eventMsg);

	rrhlib_dmsg("Event message:%s \n",RSTRING_PTR(eventMsg));

	BPF_HM_DEVC_REG_READ( 0, D_DU_REG_CTB_SWWORK_DP_CUMAC_H, &readdat );
	hwDstAddr[0] = (UCHAR)((readdat & 0x0000FF00) >> 8);
	hwDstAddr[1] = (UCHAR)(readdat & 0x000000FF);

	BPF_HM_DEVC_REG_READ( 0, D_DU_REG_CTB_SWWORK_DP_CUMAC_L, &readdat );
	hwDstAddr[2] = (UCHAR)((readdat & 0xFF000000) >> 24);
	hwDstAddr[3] = (UCHAR)((readdat & 0x00FF0000) >> 16);
	hwDstAddr[4] = (UCHAR)((readdat & 0x0000FF00) >> 8);
	hwDstAddr[5] = (UCHAR)( readdat & 0x000000FF);

	BPF_HM_DEVC_REG_READ( 0, D_DU_REG_CTB_SWWORK_DP_RUMAC_H, &readdat );
	hwSrcAddr[0] = (UCHAR)((readdat & 0x0000FF00) >> 8);
	hwSrcAddr[1] = (UCHAR)(readdat & 0x000000FF);

	BPF_HM_DEVC_REG_READ( 0, D_DU_REG_CTB_SWWORK_DP_RUMAC_L, &readdat );
	hwSrcAddr[2] = (UCHAR)((readdat & 0xFF000000) >> 24);
	hwSrcAddr[3] = (UCHAR)((readdat & 0x00FF0000) >> 16);
	hwSrcAddr[4] = (UCHAR)((readdat & 0x0000FF00) >> 8);
	hwSrcAddr[5] = (UCHAR)( readdat & 0x000000FF);

	memset(dp_header,0,sizeof(dp_header));
	if((sd = BPF_COM_DPLOG_DBG_INT(hwSrcAddr,hwDstAddr,dp_header)) >= 0 ){
		memset(value,0,sizeof(value));
		length = snprintf(value,sizeof(value),"%s",RSTRING_PTR(eventMsg));
		BPF_COM_DPLOG_DBG_TXT(sd,dp_header,length,value);
	}else{
		rrhlib_dmsg("Event message error:%s \n",RSTRING_PTR(eventMsg));
		close(sd);
		return mrb_false_value();
	}
	close(sd);
	return mrb_true_value();
}

/*!
 * @brief        RTC取得処理
 * @note        RTC取得処理
 * @tparam      N/A
 * @param       mrb     [in]    mrb status
 * @param       self    [in]    mrb self
 * @return      false
 * @return      true
 * @date        2019/11/29
 * @warning     本関数はスクリプトからのコール関数である.
 * @warning     スクリプト上でのモジュール呼び出し名 : Rrh.getrtc
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
				"%04d/%02d/%02d %02d:%02d:%02d",
					sysTime.year,
					sysTime.month,
					sysTime.day,
					sysTime.hour,
					sysTime.min,
					sysTime.sec) ;
	if(size < 0){
		return mrb_false_value();
	}
	return mrb_str_new(mrb, rtn_str, size);
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
	rrhlib_dmsg("du lib device access install start \n");
	/* scriptで使用できるモジュールを登録する */
	/* 顧客コマンド向けは最低限とする */
	mrb_define_module_function(mrb, module_class, "regread"     ,  rrhlib_device_register_read   , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "regwrite"    ,  rrhlib_device_register_write  , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "regbiton"    ,  rrhlib_device_register_biton  , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "regbitoff"   ,  rrhlib_device_register_bitoff , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "sfpread"     ,  rrhlib_device_sfp_read        , MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, module_class, "eepread"     ,  rrhlib_device_eeprom_read     , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "eepread_txt" ,  rrhlib_device_eeprom_read_txt , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "eepwrite"    ,  rrhlib_device_eeprom_write    , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "eepwrite_txt",  rrhlib_device_eeprom_write_txt , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "tempread"    ,  rrhlib_device_temp_read       , MRB_ARGS_NONE());
	mrb_define_module_function(mrb, module_class, "bpfshmwrite" ,  rrhlib_bpfshm_write           , MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, module_class, "event"       ,  rrhlib_event                  , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "getrtc"      ,  rrhlib_device_getrtc   		 , MRB_ARGS_NONE());

	rrhlib_dmsg("du lib device access install OK \n");
	return;
}

/* @} */

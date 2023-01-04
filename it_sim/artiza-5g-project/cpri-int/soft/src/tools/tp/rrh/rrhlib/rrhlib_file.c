/*!
 *  @skip	$Id:$
 *  @file	rrhlib_file.c
 *  @brief	rrh command file library routine
 *  @author	FJT)Harada
 *  @date	2013/11/26
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup	RRH_TP_CMD_LIB
 * @{
 */
#include "rrhlib_local.h"

/*!
 * @brief		ファイルオープン処理
 * @note
<pre>
	ファイルをオープンする処理である.
 	ファイル名とオープンモードを指定する.
 	オープンモードはfopen関数を踏襲する.
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ファイルディスクリプタ
 * @date		2013/11/26 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_open(mrb_state* mrb, mrb_value self)
{
	mrb_value file_name;
	mrb_value mode;
	mrb_value rtn;
	FILE* fp;
	
	mrb_get_args(mrb, "SS", &file_name, &mode );
	
	fp = fopen( RSTRING_PTR(file_name), RSTRING_PTR(mode) );	/* pgr0524 */
	if( fp == NULL ){
		rrhlib_dmsg("File open error : %s \n", RSTRING_PTR(file_name));
		return mrb_false_value();
	}
	
	rrhlib_dmsg("File open finish : %s \n", RSTRING_PTR(file_name));
	rtn = mrb_fixnum_value( (unsigned long)fp );
	return rtn; /* pgr0524 */
}
/* @} */


/*!
 * @brief		ファイルクローズ処理
 * @note
<pre>
	ファイルをクローズする処理である.
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/26 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_close(mrb_state* mrb, mrb_value self)
{
	mrb_int fp;
	int c_rtn;
	
	mrb_get_args(mrb, "i", &fp );
	c_rtn = fclose( (FILE*)fp );
	if( c_rtn == EOF ){
		rrhlib_dmsg("File close error \n");
		return mrb_false_value();
	}
	rrhlib_dmsg("File close finish \n");
	return mrb_true_value();
}
/* @} */

/*!
* @brief		ファイルシーク処理(サイズ指定)
 * @note
<pre>
	ファイルディスクリプタの位置を先頭から指定サイズシークさせる処理である.
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		シーク結果
 * @date		2013/11/26 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_seek(mrb_state* mrb, mrb_value self)
{
	mrb_int fp;
	mrb_int size;
	int c_rtn;
	
	mrb_get_args(mrb, "ii", &fp, &size);
	c_rtn = fseek( (FILE*)fp, (int)size , SEEK_SET);
	if( c_rtn == -1){
		rrhlib_dmsg("File seek error\n");
		return mrb_false_value();
	}
	rrhlib_dmsg("File seek finish\n");
	
	return mrb_true_value();
}
/* @} */



/*!
* @brief		ファイルシーク処理(シーク行数指定)
 * @note
<pre>
	ファイルディスクリプタの位置を指定行数シークさせる処理である.
	行端の特定には改行コードを用いる.
	ex) 1行シーク = 2行目の先頭を指す
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		シーク処理結果
 * @date		2013/11/27 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_seek_line(mrb_state* mrb, mrb_value self)
{
	mrb_int fp;
	mrb_int lnum;
	int cnt;
	size_t length;
	int c_rtn;
	char* buffer = NULL;
	
	mrb_get_args(mrb, "ii", &fp, &lnum);
	
	/* fpを先頭に移動 */
	c_rtn = fseek( (FILE*)fp, 0 ,SEEK_SET);
	if( (int)c_rtn == -1){
		rrhlib_dmsg("File seek error\n");
		return mrb_false_value();
	}
	
	/* シーク行数分getline */
	for( cnt = 0; cnt < (int)lnum; cnt++){
		c_rtn = getline( &buffer, &length , (FILE*)fp );
		if(c_rtn == -1 ) {
			rrhlib_dmsg("File seekline error \n");
			if( buffer != NULL ) free(buffer);
			return mrb_false_value();
		}
	}

	free(buffer);
	
	rrhlib_dmsg("File seekline finish\n");
	
	return mrb_true_value();
}
/* @} */



/*!
 * @brief		ファイル書き込み処理
 * @note
<pre>
	指定バッファの内容をファイルに書き込む処理である．
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/26 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_write(mrb_state* mrb, mrb_value self)
{
	mrb_int fp;
	mrb_int buffer;
	mrb_int size;
	int c_rtn;
	
	mrb_get_args(mrb, "iii", &buffer, &size, &fp );
	c_rtn = fwrite( (void*)buffer, 1, (int)size , (FILE*)fp );
	if( c_rtn == EOF){
		rrhlib_dmsg("File write error \n");
		return mrb_false_value();
	}
	
	rrhlib_dmsg("File write finish\n");
	return mrb_fixnum_value( c_rtn );
}
/* @} */


/*!
 * @brief		ファイル読み取り処理
 * @note
<pre>
	ファイルから指定サイズ分読み取る処理である．
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
* @return		読み取りデータ量 (byte)
 * @date		2013/11/26 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_read(mrb_state* mrb, mrb_value self)
{
	mrb_int fp;
	mrb_int size;
	int c_rtn;
	mrb_int buffer;
	
	mrb_get_args(mrb, "iii", &buffer, &size, &fp);
	c_rtn = fread( (void*)buffer, 1, (int)size , (FILE*)fp );
	if( c_rtn == EOF){
		rrhlib_dmsg("File read error \n");
		return mrb_false_value();
	}
	rrhlib_dmsg("File read finish\n");
	
	return mrb_fixnum_value( c_rtn );
}
/* @} */


/*!
 * @brief		ファイル一行読み取り処理
 * @note
<pre>
	ファイルディスクリプタ位置から一行分読み取る処理である．
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		読み取りデータ
 * @date		2013/11/26 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_get_line(mrb_state* mrb, mrb_value self)
{
	mrb_int fp;
	size_t length;
	mrb_value rtn;
	int c_rtn;
	char* buffer = NULL;
	
	mrb_get_args(mrb, "i", &fp);
	
	c_rtn = getline( (void*)&buffer, &length , (FILE*)fp );
	if(c_rtn == -1 ) {
		rrhlib_dmsg("File getline error : 0x%x \n", (int)fp);
		if( buffer != NULL ) free(buffer);
		return mrb_false_value();
	}
	
	rrhlib_dmsg("File getline finish\n");
	rtn = mrb_str_new(mrb, buffer, c_rtn);
	if( buffer != NULL ) free(buffer);
	return rtn;
	
}
/* @} */

/*!
 * @brief		ファイルディスクリプタの位置参照
 * @note
<pre>
	ファイルディスクリプタの位置を参照する処理である.
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ファイルディスクリプタの位置
 * @date		2013/11/27 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_tell(mrb_state* mrb, mrb_value self)
{
	mrb_int fp;
	long c_rtn;
	
	mrb_get_args(mrb, "i", &fp);
	
	c_rtn = ftell( (FILE*)fp );
	if(c_rtn == -1 ) {
		rrhlib_dmsg("File tell error \n");
		return mrb_false_value();
	}
	
	rrhlib_dmsg("File tell finish\n");
	return  mrb_fixnum_value( c_rtn );
	
}
/* @} */


/*!
 * @brief		ファイルサイズ取得
 * @note
<pre>
	指定されたファイル名のサイズを取得する処理である.
	内部的にはfopen fseek ftell fclose を行う.
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		ファイルサイズ
 * @date		2013/11/26 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_size_get(mrb_state* mrb, mrb_value self)
{
	mrb_value file_name;
	mrb_value rtn;
	int c_rtn;
	FILE* fp;
	
	mrb_get_args(mrb, "S", &file_name );
	
	/* ファイルオープン */
	fp = fopen( RSTRING_PTR(file_name), "r" );	/* pgr0524 */
	if( fp == NULL ){
		rrhlib_dmsg("File open error :%s \n", RSTRING_PTR(file_name));
		return mrb_false_value();
	}
	
	/* ファイルディスクリプタをファイル末尾へ移動 */
	c_rtn = fseek( (FILE*)fp, 0 , SEEK_END);
	if( c_rtn == -1){
		rrhlib_dmsg("File seek error \n");
		fclose( (FILE*)fp );
		return mrb_false_value();
	}
	
	/* ファイルサイズ取得 */
	c_rtn = ftell( (FILE*)fp );
	if(c_rtn == -1 ) {
		rrhlib_dmsg("File tell error \n");
		fclose( (FILE*)fp );
		return mrb_false_value();
	}
	rtn = mrb_fixnum_value( c_rtn );
	
	/* ファイルクローズ */
	c_rtn = fclose( (FILE*)fp );
	if( c_rtn == EOF ){
		rrhlib_dmsg("File close error \n");
		return mrb_false_value();
	}
	
	rrhlib_dmsg("File size get finish\n");
	return  rtn;
	
}
/* @} */


/*!
* @brief		ファイル書き込み処理(文字列)
 * @note
<pre>
	指定文字列をファイルに書き込む処理である．
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		self
 * @date		2013/11/26 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_write_string(mrb_state* mrb, mrb_value self)
{
	mrb_int fp;
	mrb_value str;
	mrb_int size;
	int c_rtn;
	
	mrb_get_args(mrb, "Sii", &str, &size, &fp );
	c_rtn = fwrite( RSTRING_PTR(str), sizeof(char), (int)size , (FILE*)fp );
	if( c_rtn == EOF){
		rrhlib_dmsg("File write error \n");
		return mrb_false_value();
	}
	
	rrhlib_dmsg("File write : %s \n",RSTRING_PTR(str));
	return mrb_true_value();
}
/* @} */


/*!
* @brief		ファイル読み取り処理(文字列)
 * @note
<pre>
	ファイルから指定サイズ分文字列を読み取る処理である．
</pre>
 * @param		mrb		[in]	mrb status
 * @param		self	[in]	mrb self
 * @return		読み取った文字列
 * @date		2013/11/26 
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
mrb_value rrhlib_file_read_string(mrb_state* mrb, mrb_value self)
{
	mrb_int fp;
	mrb_int size;
	int c_rtn;
	char str[256];
	
	mrb_get_args(mrb, "ii", &size, &fp);
	c_rtn = fread( str, sizeof(char), (int)size , (FILE*)fp );
	if( c_rtn == EOF){
		rrhlib_dmsg("File read error \n");
		return mrb_false_value();
	}
	rrhlib_dmsg("File read finish\n");
	
	return mrb_str_new(mrb, str, size);
}
/* @} */


/*!
 * @brief		ファイル処理用モジュール登録処理
 * @note
<pre>
	スクリプト上で使用するファイル操作モジュールを登録する関数である.
	スクリプトで使用できるモジュールは以下の通りである
		- Rrh.fopen
</pre>
 * @param		mrb				[in]	mrb status
 * @param		module_class	[in]	module_class
 * @return		N/A
 * @date		2013/11/26
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 */
void rrhlib_file_install(mrb_state* mrb ,struct RClass * module_class)
{
	rrhlib_dmsg("rrh lib file install start \n");
	
	/* scriptで使用できるモジュールを登録する */
	/* file */
	mrb_define_module_function(mrb, module_class, "fopen"    ,  rrhlib_file_open     , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "fwrite"   ,  rrhlib_file_write    , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "fread"    ,  rrhlib_file_read     , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "fseek"    ,  rrhlib_file_seek     , MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "ftell"    ,  rrhlib_file_tell     , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "fclose"   ,  rrhlib_file_close    , MRB_ARGS_REQ(1));
	
	mrb_define_module_function(mrb, module_class, "fgetline" ,  rrhlib_file_get_line , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "fseekline",  rrhlib_file_seek_line, MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, module_class, "fsizeget" ,  rrhlib_file_size_get , MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, module_class, "fwritestr",  rrhlib_file_write_string  , MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, module_class, "freadstr" ,  rrhlib_file_read_string   , MRB_ARGS_REQ(2));

	rrhlib_dmsg("rrh lib file install OK \n");
	return;
}
/* @} */

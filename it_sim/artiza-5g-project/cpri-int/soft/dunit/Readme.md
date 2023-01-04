- 0.はじめに  
 * 本環境は5G LLs-DU向けのテストスイートであり、関数単体評価(CT)を目的としています


- 1.できること
 * 擬似レジスタ、ビジークリアによりハードアクセスも含めた評価が可能です
 * OAL/HALの復帰値を変更し異常系を評価が可能です
 * システムコール復帰値変更による異常系の評価が可能です


- 2.テストの構造  
  * テストは実行ファイル(dunit)とテストシナリオ(xx.rb)の構成となります
  * テストシナリオはすべてrubyのスクリプト形式となります  
  * テストする関数はすべて共有ライブラリ(.so)として生成します  
  * テストシナリオを実行すると、dunitでシナリオの構文解析を実施します
  * シナリオ内で実行するmoduleに応じて対応するlib内の関数を参照し実行します  
  * OAL/HAL/RFICのライブラリAPIはすべて擬似となります


- 3.テストに使用するOSSパッケージ
 * linux環境(gccコンパイラ)
 * mruby  v1.0.0
 * CUnit  v2.1-2
 * ruby(\*)  
 * bison(\*)  
 * (\*)：テスト時は不要、mrubyのライブラリ(libmruby.so)を作成する場合に必要　　


- 4.テストを行うためのフォルダ構成
  * /soft   
　       +-- /src         --------> 5G-lls-DUソースファイル    
　       +-- /dunit       --------> dunit環境    


- 5.テスト環境構築手順  
　　$ cd dunit  
　　$ ./setup.sh　


- 6.テスト実施手順  
　　$ cd bin  
　　$ ./dunit xxxxx.rb < v >
    * xxxxx.rbはテストシナリオ名
    * < v >は./bin/verboseを参照ください


- 7.テストスクリプトで使用可能なモジュール  
 * ./bin/Readme.md  を参照してください


- 8.テストモード
 * 2種類の方法をテストモードをサポート  
  * 1. テストシナリオ内ですべて実現する方法
     * 各プロセスのXXXX.module()を使って関数を実行し、XXX.moduleの結果をシナリオ内で判定  
      ` 例)`    
         `if ( result = INIT.module("f_init_xxx","0") == true) then `   
          　`puts "OK" `  
        ` else `   
        　  `puts "NG"  `  
        ` end  `

  * 2. テストシナリオからCUnitのテストスイートをコールして実現する方法
     * 各プロセスのXXXX.cunit()を使ってCUnitのテストスイートを実行する  
      ` 例)`    
        `if ( result = INIT.cunit("test_suites_array_ft_init",0) == true) then `   
         　`puts "OK" `  
        ` else `   
         　`puts "NG"  `  
        ` end  `

- 9.独自モジュールの作成方法
  * rrulib_installを参照ください
  * 例)Coreモジュールを追加する場合
    * モジュール名の定義  
	   * module_class = mrb_define_module(mrb, "Core");		// -> ここではCoreというモジュールを作成
    * モジュール名にfunctionを登録
     * mrb_define_module_function(mrb, module_class, "dmsg", rrulib_dmsg_onoff,MRB_ARGS_REQ(1));　// ->Coreモジュールにdmsgというfunctionを登録
    * mrb_define_module_function引数の説明  
     *  mrb　=>　お決まり　　
     *  module_class　=>　mrb_define_moduleの復帰値  　　
     *  "dmsg"　=>　function名
     *  rrulib_dmsg_onoff　=>　モジュールが呼ばれたときに呼ばれるCの関数　　
     *  MRB_ARGS_REQ(1)　=>　モジュールが受けた引数　　
       * //MRB_ARGS_REQ(n)　=> n個の引数　　
       * //MRB_ARGS_NONE()　=> 引数なし　　　
       * //MRB_ARGS_ARG(n,m)　=> n個の引数、m個はオプション(あってもなくても問題ない)　　
       * //MRB_ARGS_ANY()　　　=> 可変長の引数を要求する  
    *  モジュール実行イメージ  
       * 例)スクリプトからCore.dmsg("PF")をコールする場合  
       * ret = Core.dmsg("PF")　# -> rrulib_dmsg_onoffがコールがされる

1.事前準備

※mrubyをコンパイルするため、コンパイルPC上では以下が実行可能である必要があります。

(1)gccコマンドがつかえること ---------------> mrubyをコンパイルするために必要
　fedoraの場合のインストール方法は
　　yum install gcc
(2)rubyがつかえること        ---------------> mrubyをコンパイルするために必要
　fedoraの場合のインストール方法は
　　yum install ruby
(3)bisonがつかえること        ---------------> mrubyをコンパイルするために必要
　fedoraの場合のインストール方法は
　　yum install bison

※mrubyを実行する環境下では不要です

2.フォルダ構成

 $
 +--/lib                              : mrubyライブラリ格納用
 +--/lib_pc                           : linux PC用mrubyライブラリ格納用
 +--/rrh                              : rrhコマンド作成用フォルダ
 |    +-- /bin                        ; rrhコマンド格納フォルダ、スクリプトも格納する
 |    +-- /example                    ; スクリプトのサンプル
 |    +-- /rrhlib                     ; rrhライブラリ格納フォルダ
 |    |     +-- Makefile              ; rrhライブラリ用makefile
 |    |     +-- rrhlib.c              ; rrhライブラリソース
 |    |     +-- rrhlib.h              ; rrhライブラリ公開ヘッダ
 |    |     +-- ...                   ; rrhライブラリ各ソース類
 |    +-- auto_test.sh                ; rrh,rrhライブラリのみをコンパイルするsh
 |    +-- Makefile                    : CTツール作成用makefile
 |    +-- MITL                        : MIT License ファイル
 |    +-- rrh.c                       : rrhコマンドメインファイル
 |    +-- reg.tar.gz                  ; CT時の擬似レジスタファイル類
 +--Doxyfile                          : Doxygen出力用のconfigファイル
 +--Makefile                          : 全コンパイル用makefie
 +--Makefile.in                       : 全コンパイル用makefie.in
 +--/mruby                            : Mruby本体
 +--/config                           : Mruby configファイル
 +--Readme.txt

3.make方法

(1)カレントフォルダに移動し、makeコマンドを実行するだけ

-bash-4.2$ ls -l

total 108
-rwxr--r--  1 cpri cpri 79837 Nov 18 15:17 Doxyfile
-rwxr--r--  1 cpri cpri    72 Nov 18 16:48 git.txt
drwxr-xr-x  2 cpri cpri  4096 Nov 18 17:12 lib
-rwxr--r--  1 cpri cpri   452 Nov 18 17:11 Makefile
-rwxr--r--  1 cpri cpri   328 Nov 17 00:03 Makefile.in
drwxr-xr-x 15 cpri cpri  4096 Nov 18 09:02 mruby
-rwxr--r--  1 cpri cpri  2596 Nov 18 17:12 Readme.txt
drwxr-xr-x  4 cpri cpri  4096 Nov 18 17:12 rrh

(2-1)コンパイルする場合

#make clean
#make

4.実行形式

(1)コンソール上から、以下を実施することで動作する

 ./rrh -<option> <p1> <p2> <...> <pN>


-bash-4.2$ ls -l
total 572
-rwxr--r-- 1 cpri cpri     93 Nov 18 09:32 -         ─┐
-rwxr--r-- 1 cpri cpri    529 Nov 18 00:29 help        │
-rwxr--r-- 1 cpri cpri    842 Nov 18 09:59 online      │
-rwxr--r-- 1 cpri cpri    212 Nov 18 14:31 sleep       ├─> こいつらがoptionの部分です。すべてruby scriptsで作成しています。
-rwxr--r-- 1 cpri cpri   1106 Nov 17 19:48 test        │
-rwxr--r-- 1 cpri cpri    183 Nov 17 19:19 time        │
-rwxr--r-- 1 cpri cpri     62 Nov 17 16:58 ver         │
-rwxr--r-- 1 cpri cpri     91 Nov 17 16:58 verbose   ─┘
-rwxrwxr-x 1 cpri cpri 551464 Nov 18 17:13 rrh          ---> これがrrhコマンドの実行ファイルです。こいつはC言語で作成しています。

e.g)helpを出す場合
 ./rrh -help

となります

5.ruby scriptの記述方法
・rubyで書けます。でもいくつか制限があります。

6.動作実績

現在、動作確認できているのは以下です。

　・Fedora 16,19
　・Windows(cygwin)
　・B25 rrh
　・DCM Zynq RE

7. 現在使用している版数
　mruby-mruby-4a12d35.zip

8. 最新のmrubyは以下から入手できる

master
https://github.com/mruby/mruby/zipball/master

trunk
git clone https://github.com/mruby/mruby.git


以上

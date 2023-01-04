#
# mp プロセス用テストシナリオ
#
Core.require("./verbose")
mode = Core.cmode_get()


puts "MP functions test Start"

# bufferの獲得
Core.symalloc("arg","16")
Core.symset("arg","0","16","0")
addr = Core.symaddr("arg")

# 関数呼び出し
MP.module("f_saa_msg_receiver",addr)

puts "MP test END"



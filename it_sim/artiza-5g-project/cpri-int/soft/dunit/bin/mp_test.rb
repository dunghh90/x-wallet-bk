#
# mp �v���Z�X�p�e�X�g�V�i���I
#
Core.require("./verbose")
mode = Core.cmode_get()


puts "MP functions test Start"

# buffer�̊l��
Core.symalloc("arg","16")
Core.symset("arg","0","16","0")
addr = Core.symaddr("arg")

# �֐��Ăяo��
MP.module("f_saa_msg_receiver",addr)

puts "MP test END"



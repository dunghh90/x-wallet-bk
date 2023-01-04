#
#  Makefile
include ../../Makefile.in
include ./Makefile.in

RRH_PATH = ./rrh
RRH_INSTALL_PATH = ../../../ramdisk/target/opt/tp
RRH_BIN_PATH = $(RRH_PATH)/bin

all:
#	rm -rf ./lib/libmruby.a
#	rm -rf ./lib_pc/libmruby.a
#	rm -rf  $(MRUBY_PATH)
	tar zxvf $(MRUBY_PATH).tar.gz
	cp -rf ./config/build_config.rb $(MRUBY_PATH)/build_config.rb
	cp -rf ./config/mrbconf.h $(MRUBY_PATH)/include/mrbconf.h

ifdef CROSS_COMPILE
	cd $(MRUBY_PATH); make
	mv $(MRUBY_PATH)/build/host/lib/libmruby.a lib_pc/
	cd $(RRH_PATH)/rrhlib/CT ; make
	tar zxvf $(RRH_PATH)/reg.tar.gz -C $(RRH_BIN_PATH)/
else
#	cd $(MRUBY_PATH); make
#	mv $(MRUBY_PATH)/build/arm64/lib/libmruby.a lib/
endif
	cd $(RRH_PATH)/rrhlib; make
	cd $(RRH_PATH); make
	mv $(RRH_PATH)/rrh $(RRH_PATH)/bin

doxygen:
	doxygen Doxyfile

pgr:
	cd ./rrh;make pgr;

clean:
	rm -rf  $(MRUBY_PATH)
#	rm -rf ./lib/libmruby.a
#	rm -rf ./lib_pc/libmruby.a
	cd $(RRH_PATH)/rrhlib; make clean
	cd $(RRH_PATH); make clean
	rm -rf ./html ./*.d

install:
	cp -rf $(RRH_BIN_PATH)/*         $(RRH_INSTALL_PATH)/


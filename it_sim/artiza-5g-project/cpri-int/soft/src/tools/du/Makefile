#
#  Makefile
include ../../Makefile.in
include ./Makefile.in

RRH_PATH = ./cmd
RRH_INSTALL_PATH = ../../../ramdisk/target/opt/du
RRH_BIN_PATH = $(RRH_PATH)/bin

all:
#	rm -rf ./lib/libmruby.a
#	rm -rf  $(MRUBY_PATH)
	tar zxvf $(MRUBY_PATH).tar.gz
	cp -rf ./config/build_config.rb $(MRUBY_PATH)/build_config.rb
	cp -rf ./config/mrbconf.h $(MRUBY_PATH)/include/mrbconf.h

#	cd $(MRUBY_PATH); make
#	mv $(MRUBY_PATH)/build/arm64/lib/libmruby.a lib/
	cd $(RRH_PATH)/dulib; make
	cd $(RRH_PATH); make
	mv $(RRH_PATH)/du $(RRH_PATH)/bin/

doxygen:
	doxygen Doxyfile

pgr:
	cd ./du;make pgr;git 

clean:
	rm -rf  $(MRUBY_PATH)
#	rm -rf ./lib/libmruby.a
	cd $(RRH_PATH)/dulib; make clean
	cd $(RRH_PATH); make clean
	rm -rf ./html ./*.d

install:
	cp -rf $(RRH_BIN_PATH)/*         $(RRH_INSTALL_PATH)/


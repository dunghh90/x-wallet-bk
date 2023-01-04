#
#	M-planeSW Server all Make file
#

SAA_INSTL_DIR		=	opt
SAA_AGENT_DIR		=	agent
MPMW_DIR			=	mw
IF_DIR				=	interface
SAA_PRG_NAME		=	DU_agent
SAA_PRG_DBAPI_NAME	=	DU_dbapi
SAA_PRG_DBGET_NAME	=	DU_dbget
NSA_DIR				=	etc
MPSW_LIB_DIR		=	lib
TOP_DIR				=	../../..

SAA_AGENT_SRCDIR	=	$(TOP_DIR)/$(SAA_AGENT_DIR)/saa
SAA_DBAPI_SRCDIR	=	$(TOP_DIR)/$(SAA_AGENT_DIR)/dbapi
SAA_DBGET_SRCDIR	=	$(TOP_DIR)/$(SAA_AGENT_DIR)/dbget
SAA_COM_SRCDIR		=	$(TOP_DIR)/$(SAA_AGENT_DIR)/com
CALLBACK_SRCDIR		=	$(TOP_DIR)/$(SAA_AGENT_DIR)/callbacks
SAA_CNF_DIR			=	$(TOP_DIR)/build/llsRU/agent
NSA_CNF_DIR			=	$(TOP_DIR)/build/llsRU/netconfd-pro
MPMW_SRCDIR			=	$(TOP_DIR)/$(MPMW_DIR)/libmpmw_srv
YPMW_SRCDIR			=	$(TOP_DIR)/$(MPMW_DIR)/libsystem
FUNCIF_SRCDIR		=	$(TOP_DIR)/$(IF_DIR)/function
SAA_AGENT_INSTL_DIR	=	$(SAA_INSTL_DIR)/$(SAA_AGENT_DIR)
SAA_AGENT_PRG_DIR	=	$(SAA_AGENT_INSTL_DIR)/bin
SAA_AGENT_LIB_DIR	=	$(SAA_AGENT_INSTL_DIR)/lib
SAA_AGENT_ETC_DIR	=	$(SAA_AGENT_INSTL_DIR)/etc
NSA_ETC_DIR			=	$(NSA_DIR)/yumapro


all: alllib 
clean: allclean

alllib:
	mkdir -p $(NSA_DIR)
	mkdir -p $(NSA_ETC_DIR)
	mkdir -p $(SAA_INSTL_DIR)
	mkdir -p $(SAA_AGENT_INSTL_DIR)
	mkdir -p $(SAA_AGENT_PRG_DIR)
	mkdir -p $(SAA_AGENT_LIB_DIR)
	mkdir -p $(SAA_AGENT_ETC_DIR)
	mkdir -p $(MPSW_LIB_DIR)

	make function
	make libmpmw
	make agent
	make callbacks

	find $(FUNCIF_SRCDIR) -name *.so		| xargs -i cp {} $(PWD)/$(MPSW_LIB_DIR)/
	find $(MPMW_SRCDIR) -name *.so			| xargs -i cp {} $(PWD)/$(MPSW_LIB_DIR)/
	find $(SAA_COM_SRCDIR) -name *.so		| xargs -i cp {} $(PWD)/$(MPSW_LIB_DIR)/
	find $(YPMW_SRCDIR) -name *.so			| xargs -i cp {} $(PWD)/$(SAA_AGENT_LIB_DIR)/
	find $(CALLBACK_SRCDIR) -name *.so		| xargs -i cp {} $(PWD)/$(SAA_AGENT_LIB_DIR)/
	cp $(SAA_AGENT_SRCDIR)/$(SAA_PRG_NAME)					 $(PWD)/$(SAA_AGENT_PRG_DIR)/
	cp $(SAA_DBAPI_SRCDIR)/$(SAA_PRG_DBAPI_NAME)			 $(PWD)/$(SAA_AGENT_PRG_DIR)/
	cp $(SAA_DBGET_SRCDIR)/$(SAA_PRG_DBGET_NAME)			 $(PWD)/$(SAA_AGENT_PRG_DIR)/
	cp $(SAA_CNF_DIR)/*										 $(PWD)/$(SAA_AGENT_ETC_DIR)/
	cp $(NSA_CNF_DIR)/*										 $(PWD)/$(NSA_ETC_DIR)/
	cd $(PWD); du -a ./

function:
	-rm $(FUNCIF_SRCDIR)/*.o $(FUNCIF_SRCDIR)/*.so
	make -C $(FUNCIF_SRCDIR)

libmpmw:
	-rm $(MPMW_SRCDIR)/*.o $(MPMW_SRCDIR)/*.so
	-rm $(YPMW_SRCDIR)/*.o $(YPMW_SRCDIR)/*.so

	make -C $(MPMW_SRCDIR)
	make -C $(YPMW_SRCDIR)

agent:
	-rm $(SAA_COM_SRCDIR)/*.o $(SAA_COM_SRCDIR)/*.so
	-rm $(SAA_AGENT_SRCDIR)/*.o $(SAA_AGENT_SRCDIR)/$(SAA_PRG_NAME)
	-rm $(SAA_DBAPI_SRCDIR)/*.o $(SAA_DBAPI_SRCDIR)/$(SAA_PRG_DBAPI_NAME)
	-rm $(SAA_DBGET_SRCDIR)/*.o $(SAA_DBGET_SRCDIR)/$(SAA_PRG_DBGET_NAME)
	
	make -C $(SAA_COM_SRCDIR)
	make -C $(SAA_AGENT_SRCDIR)
	make -C $(SAA_DBAPI_SRCDIR)
	make -C $(SAA_DBGET_SRCDIR)

callbacks:
	make clean -C $(CALLBACK_SRCDIR)
	make -C $(CALLBACK_SRCDIR)

allclean:
	rm -rf $(SAA_INSTL_DIR)
	rm -rf $(MPSW_LIB_INSTL_DIR)
	rm -rf $(NSA_DIR)

	-rm $(FUNCIF_SRCDIR)/*.o $(FUNCIF_SRCDIR)/*.so
	-rm $(MPMW_SRCDIR)/*.o $(MPMW_SRCDIR)/*.so
	-rm $(YPMW_SRCDIR)/*.o $(YPMW_SRCDIR)/*.so
	-rm $(SAA_COM_SRCDIR)/*.o $(SAA_COM_SRCDIR)/*.so
	-rm $(SAA_AGENT_SRCDIR)/*.o $(SAA_AGENT_SRCDIR)/$(SAA_PRG_NAME)
	-rm $(SAA_DBAPI_SRCDIR)/*.o $(SAA_DBAPI_SRCDIR)/$(SAA_PRG_DBAPI_NAME)
	-rm $(SAA_DBGET_SRCDIR)/*.o $(SAA_DBGET_SRCDIR)/$(SAA_PRG_DBGET_NAME)
	make clean -C $(CALLBACK_SRCDIR)


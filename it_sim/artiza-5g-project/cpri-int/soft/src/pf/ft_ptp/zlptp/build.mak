
SRC_DIR = ../src

SRC = $(foreach src_dir, $(SRC_DIR), $(wildcard $(src_dir)/*.c))

OBJS = $(notdir $(SRC:.c=.o))

VPATH = $(SRC_DIR)

INCLUDE_DIRS = $(SRC_DIR) $(wildcard ../../zl*/include) $(INCLUDE_DIRS_DEF)

CC_DIRS = $(foreach X, $(INCLUDE_DIRS), -I$(X))

$(TARGET): $(OBJS)
	@echo -----------------------------------
	@echo Making target $@
	@echo CFLAGS=$(CFLAGS) > $(dir $@)/FLAGS.txt
	@echo LDFLAGS=$(LDFLAGS) >> $(dir $@)/FLAGS.txt
	$(LD) $(LDFLAGS) $^ -o $@

%.o : %.c
	@echo compiling $<
	$(CC) $(CFLAGS) $(CC_DIRS) -c $< -o $@

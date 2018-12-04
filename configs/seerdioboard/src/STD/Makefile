-include $(TOPDIR)/Make.defs

DIRS := $(shell find .  -maxdepth 3 -type d)
CFLAGS += -I ./ -I FWlib/inc -I CMSIS/inc
src = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
obj = $(patsubst %.c, %.o, $(src))

all : stdlib.a
	@echo "lalalaAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"


%.o : %.c
	@echo $(Q) $(MKDEP) $(CC) -c $(CFLAGS) $<
	$(Q) $(MKDEP) $(CC) -c $(CFLAGS) $<

stdlib.a : $(obj)
	@echo "stdlib BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
	$(Q) $(AR) $< obj

-include Make.dep
	 
	

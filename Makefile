


CFLAGS    = -mcpu=arm926ej-s -nostartfiles -fno-common -g   -Iinclude -Wall 
#-DDEBUG 
CROSS    ?=  arm-none-linux-gnueabi-

LINKER_DIR = linker

objdir ?= .obj

vpath %.c arch kernel lib
vpath %.S arch kernel lib
vpath %.h include

csources    = $(wildcard arch/*.c kernel/*.c lib/*.c)
asmsources  = $(wildcard arch/*.S lib/*.S)

asmobjs	:= $(patsubst %.S,$(objdir)/%.o,$(notdir $(asmsources)))
cobjs   := $(patsubst %.c,$(objdir)/%.o,$(notdir $(csources))) 

objs :=   $(cobjs) $(asmobjs)
dummy	= $(objdir)/.dummy

tgt := final

$(tgt): $(dummy) $(objs)
	$(CROSS)gcc $(objs) -o $@ -Wl,-T $(LINKER_DIR)/ld.script -Wl,-M -nostdlib  $(CFLAGS) > final.map
	$(CROSS)objcopy -O binary $@ $@.bin

$(objdir)/%.o:%.c
	$(CROSS)gcc -c $^ $(CFLAGS) -o $@ 

$(objdir)/%.o:%.S
	$(CROSS)gcc -c $^ $(CFLAGS) -o $@ 


print: 
	echo $(objs)

$(objdir)/.dummy :  
	if [ ! -d $(objdir) ] ; then \
		mkdir -p $(objdir); \
	fi
	touch $(objdir)/.dummy

cscope: 
	find $(shell pwd) -name "*.[chsS]" > cscope.files
	cscope -b -k -q
clean:
	rm -rf $(objdir) final final.bin final.map
	

clean_cscope:
	rm -rf cscope.*

mrproper: clean clean_cscope
	
run:
	qemu-system-arm -M versatilepb

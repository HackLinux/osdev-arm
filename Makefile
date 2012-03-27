
CFLAGS    = -mcpu=arm926ej-s -nostartfiles -fno-common
CROSS    ?=  arm-none-linux-gnueabi-

objdir ?= .obj


csources    = $(wildcard *.c)
asmsources  = $(wildcard *.S)

asmobjs	:= $(patsubst %.S,$(objdir)/%.o,$(notdir $(asmsources)))
cobjs   := $(patsubst %.c,$(objdir)/%.o,$(notdir $(csources))) 

objs :=   $(cobjs) $(asmobjs)
dummy	= $(objdir)/.dummy

tgt := final

$(tgt): $(dummy) $(objs)
	$(CROSS)gcc $(objs) -o $@ -Wl,-T ld.script -Wl,-M -nostdlib  $(CFLAGS) > final.map
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
	find $(shell pwd) -name "*.[chs]" > $(objdir)/cscope.files
	cd $(objdir) && cscope -b -k -q
clean:
	rm -rf $(objdir) final

run:
	qemu-system-arm -M versatilepb

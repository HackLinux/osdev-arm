


includes 		= include lib
CFLAGS    		= -mcpu=arm926ej-s -nostartfiles -fno-common -nostdinc -g $(addprefix -I, $(includes)) -Wall #-DDEBUG 
CROSS_COMPILE   ?=  arm-none-linux-gnueabi-

LINKER_DIR = linker

objdir ?= .obj
depend ?= .obj/depend
logfile?= /tmp/.final

vpath %.c arch kernel lib
vpath %.S arch kernel lib

csources    = $(wildcard arch/*.c kernel/*.c lib/*.c)
asmsources  = $(wildcard arch/*.S lib/*.S)

asmobjs		:= $(patsubst %.S,$(objdir)/%.o,$(notdir $(asmsources)))
cobjs   	:= $(patsubst %.c,$(objdir)/%.o,$(notdir $(csources))) 

objs 		:=   $(cobjs) $(asmobjs)

dependencies = $(patsubst %.c,$(depend)/%.d,$(notdir $(csources))) 
dependencies += $(patsubst %.c,$(depend)/%.d,$(notdir $(asmcsources))) 



#automatic directory creation
$(shell mkdir -p $(objdir); mkdir -p $(depend);echo > $(logfile))



tgt := final

$(tgt): $(objs)
	@echo ; 
	echo "Generating kernel image" ; 
	$(CROSS_COMPILE)gcc $(objs) -o $@ -Wl,-T $(LINKER_DIR)/ld.script -Wl,-M -nostdlib  $(CFLAGS) > final.map ;\
	$(CROSS_COMPILE)objcopy -O binary $@ $@.bin





#dependency generation
$(call make-depend,source-file,object-file,depend-file,logfile)
define make-depend
$(CROSS_COMPILE)gcc -MM -MF $3 -MP -MT $2 $(CFLAGS) $1 > $4 || (cat $4 ; exit 1)
endef
 
$(call compile,source-file,object-file,depend-file,logfile)
define compile
@echo "Compilging $1"; \
$(call make-depend,$1,$2,$3,$4); \
$(CROSS_COMPILE)gcc -c $1 $(CFLAGS) -o $2 > $4 || (cat $4; exit 1)
endef

 #dependency inclusion
ifneq "$(MAKECMDGOALS)" "clean"
	-include $(dependencies)
endif
 


$(objdir)/%.o:%.c
	$(call compile,$<,$@,$(depend)/$(subst .o,.d,$(@F)),$(logfile)) 


$(objdir)/%.o:%.S
	$(call compile,$<,$@,$(depend)/$(subst .o,.d,$(@F)),$(logfile)) 


print: 
	echo $(objs)

cscope: 
	find $(shell pwd) -name "*.[chsS]" > cscope.files
	cscope -b -k -q

clean:
	rm -rf $(objdir) final final.bin final.map
	

clean_cscope:
	rm -rf cscope.*

mrproper: clean clean_cscope
	
run:
	qemu-system-arm -kernel $(tgt).bin -M versatilepb -serial stdio 

run-debug:
	qemu-system-arm -kernel $(tgt).bin -M versatilepb -serial stdio -s -S

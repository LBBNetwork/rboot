include common.mk
OBJECTS = entry.o start.o
LDFLAGS = -Ttext=0x42000000 -Larm -Lboard -Lcrt -Lsystem -Lhw -Lfw
LIBRARIES = -larm -lboard -lsystem -lhw -lfw \
	-lboard -lcrt -lm -lc -lgcc		# STUPID LINKER
SUBDIRS = arm board crt system hw fw

all: rBoot.image

rBoot.image: $(OBJECTS)
	for i in $(SUBDIRS); \
		do make -C $$i; \
	done
	@echo LD $(@).elf
	@$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) $(LIBRARIES) -o $(@).elf
	@echo OBJCOPY $(@)
	@$(OBJCOPY) -O binary $(@).elf $(@)

clean:
	for i in $(SUBDIRS); \
		do make -C $$i clean; \
	done
	rm -rf $(OBJECTS) *.a *.elf rBoot.image

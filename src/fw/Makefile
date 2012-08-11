include ../common.mk

OBJECTS = memory.o serial.o video.o osloader.o

all:	libfw.a

libfw.a:	$(OBJECTS)
	@echo AR $(@)
	@$(AR) r $(@) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) libfw.a

include ../common.mk

OBJECTS = vsnprintf.o

all:	libcrt.a

libcrt.a:	$(OBJECTS)
	@echo AR $(@)
	@$(AR) r $(@) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) libcrt.a

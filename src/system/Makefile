include ../common.mk

OBJECTS = nvram.o core.o cmdline.o

all:	libsystem.a

libsystem.a:	$(OBJECTS)
	@echo AR $(@)
	@$(AR) r $(@) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) libsystem.a

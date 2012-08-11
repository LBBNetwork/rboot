include ../common.mk

OBJECTS = font.o framebuffer.o

all:	libhw.a

libhw.a:	$(OBJECTS)
	@echo AR $(@)
	@$(AR) r $(@) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) libhw.a

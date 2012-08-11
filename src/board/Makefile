include ../common.mk

OBJECTS = gpio.o time.o h3.o chipid.o hwinit.o

all:	libboard.a

libboard.a:	$(OBJECTS)
	@echo AR $(@)
	@$(AR) r $(@) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) libboard.a

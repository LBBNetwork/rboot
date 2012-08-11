include ../common.mk

OBJECTS = arm_flush_dcache.o arm_stall_processor.o arm_sweep_dcache.o arm_sweep_icache.o arm_vectors.o arm_registers.o arm_initialize.o arm_fpu.o

all:	libarm.a

libarm.a:	$(OBJECTS)
	@echo AR $(@)
	@$(AR) r $(@) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) libarm.a

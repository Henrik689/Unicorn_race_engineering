##### Flags ####

# HEXFORMAT -- format for .hex file output
HEXFORMAT=ihex

# compiler
CFLAGS=-I. $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) $(DEFINES) \
	-fpack-struct -fshort-enums -mrelax  -flto \
	-Wl,-u,vfprintf -lprintf_flt -lm \
	-funsigned-bitfields -funsigned-char \
	-Wall -Wstrict-prototypes -Werror \
	-Wa,-ahlms=$(firstword \
	$(filter %.lst, $(<:.c=.lst)))

# c++ specific flags
CPPFLAGS=-fno-exceptions \
	-Wa,-ahlms=$(firstword \
	$(filter %.lst, $(<:.cpp=.lst)) \
	$(filter %.lst, $(<:.cc=.lst)) \
	$(filter %.lst, $(<:.C=.lst)))

# assembler
ASMFLAGS =-I. $(INC) -mmcu=$(MCU) \
	-x assembler-with-cpp \
	-Wl,-u,vfprintf -lprintf_flt -lm \
	-Wa,-gstabs,-ahlms=$(firstword \
		$(<:.S=.lst) $(<.s=.lst))


# linker
LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU) \
	-Wl,-u,vfprintf -lprintf_flt -lm -mrelax -flto -O$(OPTLEVEL) \
	-lm $(LIBS)

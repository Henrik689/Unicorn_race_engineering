.SUFFIXES : .c .cc .cpp .C .o .elf .s .S \
	.hex .ee.hex .h .hh .hpp


.PHONY: writeflash clean stats gdbinit stats readfuse writefuse

# Make targets:
# all, disasm, stats, hex, writeflash/install
all: $(TRG)
	@echo "$(PROJECTNAME) Done!"

disasm: $(DUMPTRG) stats

stats: $(TRG)
	$(OBJDUMP) -h $(TRG)
	$(SIZE) $(TRG)

hex: $(HEXTRG)


writeflash: hex
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID) \
	 -p $(PROGRAMMER_MCU) -P usb -B 8 \
	 -U flash:w:$(HEXROMTRG)

readfuse:
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID) \
	-p $(PROGRAMMER_MCU) -P usb \
	-U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h -U lock:r:-:h

writefuse:
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID) \
	-p $(PROGRAMMER_MCU) -P usb -e \
	-u -U lfuse:w:0xff:m \
	-u -U hfuse:w:0xf9:m \
	-u -U efuse:w:0xdf:m # same as l:0xff h:0xff e:d9


install: writeflash

$(DUMPTRG): $(TRG)
	$(OBJDUMP) -S  $< > $@


$(TRG): $(OBJDEPS)
	$(CC) $(LDFLAGS) -o $(TRG) $(OBJDEPS)


#### Generating assembly ####
# asm from C
%.s: %.c
	$(CC) -S $(CFLAGS) $< -o $@

# asm from (hand coded) asm
%.s: %.S
	$(CC) -S $(ASMFLAGS) $< > $@


# asm from C++
.cpp.s .cc.s .C.s :
	$(CC) -S $(CFLAGS) $(CPPFLAGS) $< -o $@



#### Generating object files ####
# object from C
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@


# object from C++ (.cc, .cpp, .C files)
.cc.o .cpp.o .C.o :
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# object from asm
.S.o :
	$(CC) $(ASMFLAGS) -c $< -o $@


#### Generating hex files ####
# hex files from elf
#####  Generating a gdb initialisation file    #####
.elf.hex:
	$(OBJCOPY) -j .text \
		-j .data \
		-O $(HEXFORMAT) $< $@

.elf.ee.hex:
	$(OBJCOPY) -j .eeprom \
		--change-section-lma .eeprom=0 \
		-O $(HEXFORMAT) $< $@


#####  Generating a gdb initialisation file    #####
##### Use by launching simulavr and avr-gdb:   #####
#####   avr-gdb -x gdbinit-myproject           #####
gdbinit: $(GDBINITFILE)

$(GDBINITFILE): $(TRG)
	@echo "file $(TRG)" > $(GDBINITFILE)

	@echo "target remote localhost:1212" \
		                >> $(GDBINITFILE)

	@echo "load"        >> $(GDBINITFILE)
	@echo "break main"  >> $(GDBINITFILE)
	@echo "continue"    >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"


#### Cleanup ####
clean:
	$(REMOVE) $(TRG) $(TRG).map $(DUMPTRG)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST) $(GDBINITFILE)
	$(REMOVE) $(GENASMFILES)
	$(REMOVE) $(HEXTRG)

clean_minushex:
	$(REMOVE) $(TRG) $(TRG).map $(DUMPTRG)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST) $(GDBINITFILE)
	$(REMOVE) $(GENASMFILES)

SHELL=/bin/sh
MAKE=make

SUBDIRS=can_simple_rx_tx io_read_write

all:
	@for i in $(SUBDIRS); do \
	echo "make all in $$i..."; \
	(cd $$i; $(MAKE) all); done

install:
	@for i in $(SUBDIRS); do \
	echo "make installing in $$i..."; \
	(cd $$i; $(MAKE) install); done

clean:
	@for i in $(SUBDIRS); do \
	echo "make cleaning in $$i..."; \
	(cd $$i; $(MAKE) clean); done

.PHONY: all install clean


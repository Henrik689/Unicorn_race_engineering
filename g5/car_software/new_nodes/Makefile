SHELL=/bin/sh
MAKE=make

NODES=TemplateNode
SUBDIRS=$(NODES) tests

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

##### Customize the values as indicated below and :
##### make
##### make disasm
##### make stats
##### make hex
##### make writeflash
##### make gdbinit
##### or make clean

# Name of our project
# (use a single word, e.g. 'myproject')
PROJECTNAME=GPSNode
SRC=$(wildcard *.c)

TOP=..
include $(TOP)/mk/config.mk
include $(TOP)/mk/flags.mk
include $(TOP)/mk/executables.mk

DEFINES += -DNO_USART0_SUPPORT -DNO_USART1_BUFFERED_OUTPUT

include $(TOP)/mk/rules.mk


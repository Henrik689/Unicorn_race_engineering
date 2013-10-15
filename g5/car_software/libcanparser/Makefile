CC = gcc
SRC  := $(filter-out main.c, $(wildcard *.c))

CFLAGS = -Wall -O2
LDFLAGS = -Wall -I$(CURDIR) -L$(CURDIR) -lcanparser
BIN = parser

LIBNAME = canparser
WIN_NAME = $(LIBNAME).dll
MAC_NAME = lib$(LIBNAME).dylib
LINUX_NAME = lib$(LIBNAME).so

all: $(BIN)

$(BIN): shared
	$(CC) $(LDFLAGS) main.c -o $(BIN)

.PHONEY: shared
shared: shared-windows shared-linux shared-mac

.PHONEY: shared-windows
shared-windows: $(SRC)
	$(CC) $(CFLAGS) -o $(WIN_NAME) -shared $(SRC)

.PHONEY: shared-mac
shared-mac: $(SRC)
	$(CC) $(CFLAGS) -o $(MAC_NAME) -shared $(SRC)

.PHONEY: shared-linux
shared-linux: $(SRC)
	$(CC) $(CFLAGS) -o $(LINUX_NAME) -shared $(SRC)


.PHONEY: clean
clean:
	rm -f $(WIN_NAME) $(MAC_NAME) $(LINUX_NAME) $(LIBNAME) $(BIN)

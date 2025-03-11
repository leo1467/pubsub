#
# @(#)Makefile
#
# Modification History:
#
#
# Makefile for Linux platform
TMPDIR=../tmp/
BINDIR_DYNAMIC=../bin/
BINDIR_STATIC=../bin/
BINDIR=../bin/
HEADERDIR=

USR_LOCAL_LIB=${HOME}/.local
INI=../inih

MYLIB=$(INI)

LINK_TYPE=static

CC=g++

AR=$(CC)
AFLAG=$(CCFLAG)

SRCFILES=$(wildcard *.c *.cc *.cpp)
HEADERS=$(wildcard *.h *.hpp)
INCLUDEDIRFILES=$(addprefix $(HEADERDIR), $(HEADERS))

OBJS=$(patsubst %, $(TMPDIR)%, $(patsubst %.c, %.o, $(patsubst %.cc, %.o, $(SRCFILES))))

TARGET_STATIC=main
TARGET_DYNAMIC=main
LINK1=main
LINK2=main

TARGET?=$(TARGET_DYNAMIC)

BUILD?=debug
BUILD_TYPE?=dynamic

DEBUG_CCFLAG=-O0 -DDEBUG

RELEASE_CCFLAG=-Ofast

CCFLAG=-Wall -Wextra -Wno-parentheses \
		 -std=c++17 -g -finline-functions -finline-limit=3600 -m64 -fno-builtin-malloc -ldl -rdynamic -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -D_GNU_SOURCE

INCLUDEDIR=\
		-I. \
		-I$(INI)/include \
		# -I$(USR_LOCAL_LIB)/include \
		-I$(TKBUS)/include \
		-I$(NULIB)/include \
		-I$(NUEV)/include \
		-I$(EMGGWCLNT)/include/EmgGwClnt \
		-I$(AOUTIL)/include/static \
		-I$(AOMGRCLNT)/include/static \
		-I$(EMGTOOLS)/include \
		-I$(EMGDATA)/include \
		-I$(EMGTKBUS)/include \
		-I$(EMGOTCGW)/include \
		-I$(STRATEGY_RUSHORDER)/include \
		-I$(STRATEGY_TOOLS)/include \
		-I$(EMGCORE)/include \
		-I$(EMGRISK)/include \
		-I$(MSGDISPATCHER)/include

LIBDIR1=\
		-L$(INI)/bin \
		# -L$(EMGTOOLS)/bin \
		-L$(EMGDATA)/bin \
		-L$(EMGTKBUS)/bin \
		-L$(EMGOTCGW)/bin \
		-L$(STRATEGY_RUSHORDER)/bin \
		-L$(STRATEGY_TOOLS)/bin \
		-L$(EMGCORE)/bin \
		-L$(EMGRISK)/bin \
		-L$(MSGDISPATCHER)/bin

LIBDIR2=\
		# -L$(USR_LOCAL_LIB)/lib \
		-L$(TKBUS)/bin \
		-L$(NULIB)/bin/PIC \
		-L$(NUEV)/bin \
		-L$(EMGGWCLNT)/bin \
		-L$(AOUTIL)/bin/static \
		-L$(AOMGRCLNT)/bin/static

LIBDIR=$(addsuffix /$(BUILD_TYPE), $(LIBDIR1)) $(LIBDIR2)

# LIBS=-lMsgDispatcher -lEmgOtcGw -lEmgTkBus -lEmgGwClnt -lEmgData -lStrategyTools -lRushOrder -lEmgCore -lEmgRisk -lEmgTools -lAOMgrClnt -lTukanBusClnt -lNuFixUtil -lNuEv -lNuEv_pic -lNuUtil -lpthread -lAOUtil

LIBS=-lIni -lpthread

# LDFLAGS_DYNAMIC=-Wl,-rpath,`pwd`/$(EMGTKBUS)/bin/dynamic:`pwd`/$(EMGOTCGW)/bin/dynamic:`pwd`/$(EMGTOOLS)/bin/dynamic:`pwd`/$(MSGDISPATCHER)/bin/dynamic:`pwd`/$(EMGDATA)/bin/dynamic:`pwd`/$(EMGCORE)/bin/dynamic:`pwd`/$(STRATEGY_RUSHORDER)/bin/dynamic:`pwd`/$(STRATEGY_TOOLS)/bin/dynamic:`pwd`/$(EMGRISK)/bin/dynamic
LDFLAGS_DYNAMIC=

#LDFLAGS_STATIC=-static
LDFLAG=

ifeq ($(BUILD), debug)
	CCFLAG += $(DEBUG_CCFLAG)
else ifeq ($(BUILD), release)
	CCFLAG += $(RELEASE_CCFLAG)
endif

ifeq ($(BUILD_TYPE), dynamic)
	TARGET=$(TARGET_DYNAMIC)
	LDFLAG+=$(LDFLAGS_DYNAMIC)
	BINDIR=$(BINDIR_DYNAMIC)
	LINK=$(CC) $(CCFLAG) $(LDFLAGS_DYNAMIC)
else ifeq ($(BUILD_TYPE), static)
	TARGET=$(TARGET_STATIC)
	LDFLAG+=$(LDFLAGS_STATIC)
	BINDIR=$(BINDIR_STATIC)
	LINK=$(CC) $(CCFLAG) $(LDFLAGS_STATIC)
endif

COMPILE+=$(CC) $(CCFLAG) $(LDFLAG)

all: init $(addprefix $(TMPDIR), $(TARGET)) 
	@echo "---- end of build ----"

init:
	@mkdir -p $(TMPDIR) $(BINDIR) $(HEADERDIR)

$(TMPDIR)$(TARGET): $(OBJS)
	@echo "---- build $@ ----"
	$(LINK) -o $@ $^ $(LIBDIR) $(LIBS)
	@echo .

$(TMPDIR)%.o: %.cc 
	@echo "---- Compile $< ----"
	$(COMPILE) $(INCLUDEDIR) -o $@ -c $<
	@echo .

clean:
	@echo "Clean up..."
	@if [ -n "$(HEADERDIR)" ]; then \
		rm -f $(HEADERDIR)*; \
	fi
	rm -f $(TMPDIR)*
	rm -f $(BINDIR)*
	@if [ "${BUILD_TYPE}x" = "dynamicx" ]; then \
		rm -f $(BINDIR)$(LINK1) $(BINDIR)$(LINK2); \
	fi

install: all
	@if [ -n "$(HEADERDIR)" ]; then \
		if [ -n "$(HEADERS)" ]; then \
			echo "--- install headers ... $(HEADERS)"; \
			echo "cp -p $(HEADERS) $(HEADERDIR)"; \
			cp -p $(HEADERS) $(HEADERDIR); \
		fi \
	fi
	@if [ -f $(TMPDIR)$(TARGET) ]; then \
		echo "--- install binaries ... $(TARGET)"; \
		echo "cp -p $(TMPDIR)$(TARGET) $(BINDIR)"; \
		cp -p $(TMPDIR)$(TARGET) $(BINDIR); \
	fi

debug:
	$(MAKE) BUILD=debug

release:
	$(MAKE) BUILD=release

.PHONY: all init clean install

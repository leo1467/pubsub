#
# @(#)Makefile
#
# Modification History:
#
#
# Makefile for Linux platform
TMPDIR=../tmp/
BINDIR_DYNAMIC=../bin/dynamic/
BINDIR_STATIC=../bin/static/
BINDIR=$(BINDIR_DYNAMIC)
HEADERDIR=../include/

LINK_TYPE=static

CC=g++

AR=ar
#AFLAG=ruvs
AFLAG=rvs

SRCFILES=$(wildcard *.c *.cc *.cpp)
HEADERS=$(wildcard *.h *.hpp)
INCLUDEDIRFILES=$(addprefix $(HEADERDIR), $(HEADERS))

OBJS=$(patsubst %, $(TMPDIR)%, $(patsubst %.c, %.o, $(patsubst %.cc, %.o, $(patsubst %.cpp, %.o, $(SRCFILES)))))

TARGET_STATIC=libIni.a
TARGET_DYNAMIC=libIni.so.1.0.0
LINK1=libIni.so.1
LINK2=libIni.so

TARGET?=$(TARGET_DYNAMIC)

BUILD?=debug
BUILD_TYPE?=dynamic

DEBUG_CCFLAG=-O0 -DDEBUG

RELEASE_CCFLAG=-Ofast

CCFLAG=-Wall -Wextra -Wno-parentheses \
		 -std=c++17 -g -finline-functions -finline-limit=3600 -m64 -fno-builtin-malloc -ldl -rdynamic -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -D_GNU_SOURCE

INCLUDEDIR=\
		-I. \
		-I../src \

LIBDIR1=\

LIBDIR2=\

LIBDIR=$(addsuffix /$(BUILD_TYPE), $(LIBDIR1)) $(LIBDIR2)

LIBS=-lpthread

LDFLAGS_DYNAMIC=-fpic -shared -Wl,-soname,$(LINK1)
LDFLAGS_STATIC=-static
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
	LINK=$(AR) $(AFLAG)
endif

COMPILE+=$(CC) $(CCFLAG) $(LDFLAG)

all: init $(addprefix $(TMPDIR), $(TARGET)) 
	@echo "---- end of build ----"

init:
	@mkdir -p $(TMPDIR) $(BINDIR) $(HEADERDIR)

$(TMPDIR)$(TARGET): $(OBJS)
	@echo "---- build $@ ----"
	$(LINK) -o $@ $^
	@echo .

$(TMPDIR)%.o: %.cc
	@echo "---- Compile $< ----"
	$(COMPILE) $(INCLUDEDIR) -o $@ -c $<
	@echo .

$(TMPDIR)%.o: %.cpp
	@echo "---- Compile $< ----"
	$(COMPILE) $(INCLUDEDIR) -o $@ -c $<
	@echo .

$(TMPDIR)%.o: %.c 
	@echo "---- Compile $< ----"
	$(COMPILE) $(INCLUDEDIR) -o $@ -c $<
	@echo .

clean:
	@echo "Clean up..."
# rm -f $(HEADERDIR)*
	rm -f $(TMPDIR)*
# rm -f $(BINDIR)*
# rm -rf ../bin/*
# @if [ "${BUILD_TYPE}x" = "dynamicx" ]; then \
# 	rm -f $(BINDIR)$(LINK1) $(BINDIR)$(LINK2); \
# fi

install: all LINK1 LINK2
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


LINK1:
	@if [ "${BUILD_TYPE}x" = "dynamicx" ]; then \
		cd $(BINDIR); \
		ln -sf $(TARGET) $(LINK1); \
	fi

LINK2:
	@if [ "${BUILD_TYPE}x" = "dynamicx" ]; then\
		cd $(BINDIR); \
		ln -sf $(LINK1) $(LINK2); \
	fi

debug:
	$(MAKE) BUILD=debug

release:
	$(MAKE) BUILD=release

.PHONY: all init clean install

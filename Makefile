CWD=$(shell pwd)

SRC_PATH=$(CWD)/src
INCLUDE_PATH=$(CWD)/include
DEPS_PATH=$(CWD)/deps

UV_PATH=$(DEPS_PATH)/libuv
LOGH_PATH=$(DEPS_PATH)/log.h

UV_LIB=$(UV_PATH)/out/Debug/libuv.a

CC=cc
CFLAGS=-c -O2 -Wall -std=c99
INCLUDES=-I$(UV_PATH)/include/ -I/$(LOGH_PATH)/ -I/$(INCLUDE_PATH)/
LIBS=

uname_S=$(shell uname -s)

ifeq (Darwin, $(uname_S))
CFLAGS+=
#-framework CoreServices
endif

ifeq (Linux, $(uname_S))
LIBS=-lrt -ldl -lm -pthread
endif

TBL_PARSER_SRC=$(SRC_PATH)/tbl-parser.c
TBL_PARSER_OBJECTS=$(TBL_PARSER_SRC:.c=.o)
TBL_PARSER=tbl-parser

all: clean $(TBL_PARSER)

run: all
	@echo "\n\033[1;33m>>>\033[0m"
	./$(TBL_PARSER)
	@echo "\n\033[1;33m<<<\033[0m\n"
	make clean

$(UV_PATH): 
	git submodule update --init

$(UV_LIB): $(UV_PATH)
	cd $(UV_PATH) &&                                                                                                \
	test -d ./build/gyp || (mkdir -p ./build && git clone https://git.chromium.org/external/gyp.git ./build/gyp) && \
	./gyp_uv.py -f make &&                                                                                          \
	$(MAKE) -C ./out


$(TBL_PARSER): $(UV_LIB) $(TBL_PARSER_OBJECTS)
	$(CC) $(LIBS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f $(TBL_PARSER) $(TBL_PARSER_OBJECTS) 

.PHONY: all clean

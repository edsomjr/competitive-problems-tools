# Compiler setup
export CC=g++
export CFLAGS=-W -Wall -Werror -std=c++17

RELEASE_CFLAGS=-O2
DEBUG_CFLAGS=-g -O0

ifeq ($(MODE),debug)
    CFLAGS+=$(DEBUG_CFLAGS)
else
    CFLAGS+=$(RELEASE_CFLAGS)
endif

LDFLAGS=
INCLUDES=-Iinclude

# Tools
AR=ar
AR_FLAGS=rcs
LINKER=g++

# Environment variables
INCLUDES_FLAG=-I
LIBPATH_FLAG=-L
EXT_LIBPATH_FLAG=-L
LIBS_FLAG=-l
EXT_LIBS_FLAG=-l
OBJ_OUTPUT_FLAG=-o 
AR_OUTPUT_FLAG=
OUTPUT_FLAG=-o 
GEN_OBJECT_FLAG=-c
STATIC_LIB_SUFFIX=.a
STATIC_LIB_PREFIX=lib
OBJ_EXTENSION=.o

# Directories
SRC_DIR=src
SCRIPTS_DIR=scripts

INSTALL_BIN_DIR=/usr/local/bin
INSTALL_COMPLETION_DIR=/etc/bash_completion.d

# Project targets
PROJECT=cp-tools
LIBRARY=$(STATIC_LIB_PREFIX)$(PROJECT)$(STATIC_LIB_SUFFIX)

.PHONY: all clean

# Project source files
SOURCES=${wildcard $(SRC_DIR)/*.cpp}
OBJECTS=$(SOURCES:.cpp=$(OBJ_EXTENSION))
COMPLETION_SCRIPT=$(PROJECT)-completion.sh

.SUFFIXES: .cpp .$(OBJ_EXTENSION) 

# Rules
.cpp$(OBJ_EXTENSION):
	$(CC) $(GEN_OBJECT_FLAG) $< $(OBJ_OUTPUT_FLAG) $@ $(CFLAGS) $(INCLUDES)

all: $(LIBRARY) $(PROJECT) 

$(LIBRARY): $(OBJECTS)
	$(AR) $(AR_FLAGS) $(AR_OUTPUT_FLAG) $@ $(OBJECTS) 

$(PROJECT): $(COMPONENTS) $(OBJECTS)
	$(LINKER) $(OUTPUT_FLAG)$@ $(LDFLAGS) $(LIBRARY) $(LIBS) $(EXTRA_LIBS)

install: $(PROJECT)
	@cp $(PROJECT) $(INSTALL_BIN_DIR)
	@cp $(SCRIPTS_DIR)/$(COMPLETION_SCRIPT) $(INSTALL_COMPLETION_DIR)

uninstall:
	@rm -f $(INSTALL_COMPLETION_DIR)/$(COMPLETION_SCRIPT)
	@rm -f $(INSTALL_BIN_DIR)/$(PROJECT)

clean:
	@rm -f *~ $(LIBRARY) $(PROJECT)
	@find . -name *.o -exec rm -f {}  \;

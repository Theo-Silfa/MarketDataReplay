#
#  Makefile
#  market_data_replay
#
#  Created by Fedor Lisochenko on 14.01.2018.
#  Copyright © 2018 Fedor Lisochenko. All rights reserved.
#

# HIL: No spaces or comments after otherwise it captures them!
# Determine the platform
UNAME_S := $(shell uname -s)

# CC
ifeq ($(UNAME_S),Darwin)
  CC := clang++ -arch x86_64
else
  CC := g++
endif

# Folders
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin
TESTDIR := tests

# Targets
EXECUTABLE := md_replay
TARGET := $(TARGETDIR)/$(EXECUTABLE)

TESTEXECUTABLE := $(EXECUTABLE)_test
TESTTARGET := $(TARGETDIR)/$(TESTEXECUTABLE)

# Final Paths
INSTALLBINDIR := /usr/local/bin

# Code Lists
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

TESTSOURCES := $(shell find $(TESTDIR) -type f -name *.$(SRCEXT))
TESTOBJECTS := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o)) $(filter-out $(BUILDDIR)/main.o,$(OBJECTS))

# Shared Compiler Flags
CFLAGS := -c -Wall -Wextra

LIB := -L /usr/local/lib

TESTLIB := -L /usr/local/lib -lpthread -L ../gtestdist/lib -lgtest -lgtest_main
TESTINC := -I ../gtestdist/include -I $(SRCDIR)

# Platform Specific Compiler Flags
ifeq ($(UNAME_S),Linux)
  CFLAGS += -std=gnu++14 -O2 # -fPIC
else
  CFLAGS += -std=c++14 -stdlib=libc++ -O2
endif

.DEFAULT_GOAL := $(TARGET)

all : $(TARGET) $(TESTTARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "  Linking $(TARGET)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "Compiling $<..."; $(CC) $(CFLAGS) -c -o $@ $<

$(TESTTARGET): $(TESTOBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "  Linking $(TESTTARGET)"; $(CC) $^ -o $(TESTTARGET) $(TESTLIB)

$(BUILDDIR)/%.o: $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "Compiling $<..."; $(CC) $(CFLAGS) $(TESTINC) -c -o $@ $<

clean:
	@echo "Cleaning $(TARGET) $(TESTTARGET)..."; $(RM) -r $(BUILDDIR) $(TARGET) $(TESTTARGET) $(TARGETDIR)

install:
	@echo "Installing $(EXECUTABLE)..."; cp $(TARGET) $(INSTALLBINDIR)

distclean:
	@echo "Removing $(EXECUTABLE)"; rm $(INSTALLBINDIR)/$(EXECUTABLE)

.PHONY: clean

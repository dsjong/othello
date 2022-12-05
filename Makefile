CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/runner
SUBDIR := submission
SUBMISSION := $(SUBDIR)/submission.cpp

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++17 -Wall -O3 -pthread
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/engines
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

submission:
	@mkdir -p $(SUBDIR)
	@rm -f $(SUBMISSION)
	@cat src/edgetable.cpp >> $(SUBMISSION)
	@cat include/common.hpp >> $(SUBMISSION)
	@cat include/move.hpp >> $(SUBMISSION)
	@cat include/board.hpp >> $(SUBMISSION)
	@cat include/heuristics.hpp >> $(SUBMISSION)
	@cat include/engine.hpp >> $(SUBMISSION)
	@cat include/engines/ab_engine.hpp >> $(SUBMISSION)

	@cat src/board.cpp >> $(SUBMISSION)
	@cat src/engine.cpp >> $(SUBMISSION)
	@cat src/engines/ab_engine.cpp >> $(SUBMISSION)

	@cat $(SUBDIR)/main.cpp >> $(SUBMISSION)
	@sed '/^#include/d' $(SUBMISSION) > tmp
	@sed '/^#pragma/d' tmp > $(SUBMISSION)
	@sed -i '1i #include <bits/stdc++.h>' $(SUBMISSION)
	@sed -i '2i #pragma GCC optimize ("O3")' $(SUBMISSION)
	@rm -f tmp

# Tests
frontier: $(BUILDDIR)/board.o
	$(CC) $(CFLAGS) tests/frontier.cpp $(BUILDDIR)/board.o $(INC) $(LIB) -o bin/frontier

mobility: $(BUILDDIR)/board.o
	$(CC) $(CFLAGS) tests/mobility.cpp $(BUILDDIR)/board.o $(INC) $(LIB) -o bin/mobility

move: $(BUILDDIR)/board.o
	$(CC) $(CFLAGS) tests/move.cpp $(BUILDDIR)/board.o $(INC) $(LIB) -o bin/move

stability: $(BUILDDIR)/edgetable.o
	$(CC) $(CFLAGS) tests/stability.cpp $(BUILDDIR)/edgetable.o $(INC) $(LIB) -o bin/stability

.PHONY: clean submission

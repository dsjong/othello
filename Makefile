CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/runner
SUBDIR := submission
SUBMISSION := $(SUBDIR)/submission.cpp

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
override CFLAGS += -std=c++17 -Wall -O3 -pthread
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET)"; $(CC) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/engines
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET) $(SUBMISSION)"; $(RM) -r $(BUILDDIR) $(TARGET) $(SUBMISSION)

submission:
	@mkdir -p $(SUBDIR)
	@rm -f $(SUBMISSION)
	@cat src/edgetable.cpp >> $(SUBMISSION)
	@cat include/common.hpp >> $(SUBMISSION)
	@cat include/move.hpp >> $(SUBMISSION)
	@cat include/board.hpp >> $(SUBMISSION)
	@cat include/heuristics.hpp >> $(SUBMISSION)

	@cat src/board.cpp >> $(SUBMISSION)
	@cat src/heuristics.cpp >> $(SUBMISSION)

	@cat $(SUBDIR)/mtdf_engine.cpp >> $(SUBMISSION)
	@cat $(SUBDIR)/main.cpp >> $(SUBMISSION)
	@sed -i '/^#include/d' $(SUBMISSION)
	@sed -i '/^#pragma/d' $(SUBMISSION)
	@sed -i '/^ \*/d' $(SUBMISSION)
	@sed -i '/^\/\*\*/d' $(SUBMISSION)
	@sed -i '/^\/\//d' $(SUBMISSION)
	@sed -i '1i #include <bits/stdc++.h>' $(SUBMISSION)
	@sed -i '2i #pragma GCC optimize ("O3")' $(SUBMISSION)
	@awk -r 'NF' $(SUBMISSION) > tmp
	mv tmp $(SUBMISSION)
	@wc -c $(SUBMISSION)

# Tests
frontier: $(BUILDDIR)/board.o
	$(CC) $(CFLAGS) tests/frontier.cpp $(BUILDDIR)/board.o $(INC) -o bin/frontier

mobility: $(BUILDDIR)/board.o
	$(CC) $(CFLAGS) tests/mobility.cpp $(BUILDDIR)/board.o $(INC) -o bin/mobility

move: $(BUILDDIR)/board.o
	$(CC) $(CFLAGS) tests/move.cpp $(BUILDDIR)/board.o $(INC) -o bin/move

stability: $(BUILDDIR)/edgetable.o
	$(CC) $(CFLAGS) tests/stability.cpp $(BUILDDIR)/edgetable.o $(INC) -o bin/stability

search: $(OBJECTS)
	@rm -f build/main.o
	$(CC) $(CFLAGS) tests/search.cpp $(filter-out build/main.o,$(OBJECTS)) $(INC) -o bin/search

.PHONY: clean submission

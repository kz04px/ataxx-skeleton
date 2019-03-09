CC         = g++
CFLAGS     = -std=c++17 -Wall -Wextra

LINKER     = g++ -o
LFLAGS     = -pthread

TARGET     = main
SRCDIR     = src
OBJDIR     = obj
BINDIR     = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(BINDIR) $(OBJDIR) $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

bin:
	mkdir -p $(BINDIR)

obj:
	mkdir -p $(OBJDIR)

debug:
	$(MAKE) CFLAGS="$(CFLAGS) -fsanitize=address" LFLAGS="$(LFLAGS) -g -fsanitize=address"

release:
	$(MAKE) CFLAGS="$(CFLAGS) -O3 -flto -DNDEBUG"

clean:
	rm -r $(OBJDIR)

.PHONY: clean
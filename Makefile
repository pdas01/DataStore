# Project name 
TARGET   = data-store

# Command for compile
CC = g++ -Wall -O2
# Compile include reference
CFLAGS = -std=c++14 -Wall -I.
# Command for link
LINK = g++
# Command for link liberaries
LFLAGS = -Wall -I. -lsqlite3

# Directory names
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Scan file from src
SOURCES  := $(wildcard $(SRCDIR)/*.cc) $(wildcard $(SRCDIR)/**/*.cc)
INCLUDES := $(wildcard $(SRCDIR)/*.h) $(wildcard $(SRCDIR)/**/*.h)

# Get object file names
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)

# Command to remove file 
rm        = rm -rf

# Build this project
all: init $(BINDIR)/$(TARGET)
	@echo "Build $(BINDIR)/$(TARGET) successfully!"

# Init before start building
init:
# Echo related file names
	@echo SOURCES:\ \ $(SOURCES)
	@echo INCLUDES:\ $(INCLUDES)
	@echo OBJECTS:\ \ $(OBJECTS)
# Make sure bin dir exist
	@mkdir -p bin
# Make sure obj dir has same struct with src dir
	@cd $(SRCDIR) && find . -type d -exec mkdir -p -- ../$(OBJDIR)/{} \;

# Execute target
exec: all
	$(BINDIR)/$(TARGET)

# Link files
$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINK) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

# Compile files
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

# Clean
.PHONY: clean
clean:
	@$(rm) $(OBJDIR)
	@echo "Cleanup complete!"

# Remove
.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)
	@echo "Executable removed!"

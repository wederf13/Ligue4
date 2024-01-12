# compiler
CC = gcc

# compilation options
CFLAGS = -Wall -I./include

# directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# compiler
CC = gcc

# compilation options
CFLAGS = -Wall -I./include

# directories
SRCDIR = src
OBJDIR = obj

# executable name
EXECUTABLE = ligue4

# source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# default target
all: $(EXECUTABLE)

# target for the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

# target for objects
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# target to clean generated files
clean:
	rm -rf $(OBJDIR) $(EXECUTABLE)

# creates the object directory, if it does not exist
$(shell mkdir -p $(OBJDIR))

.PHONY: all clean
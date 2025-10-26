# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pthread

# Target executable
TARGET = os_sim

# Source files
SRCS = mainFunc.c pcbInitialize.c decodeExec.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean files
clean:
	rm -f $(TARGET) $(OBJS) variablesNew.txt variables.txt.bak logs_rr_with_thread logs_rr_without_thread

.PHONY: all clean


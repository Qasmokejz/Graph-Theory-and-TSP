# Makefile for CSE 13S, Spring 2023, Assignment 4

# Usage:
#
#    $ make             Make the sorting program (ready for LLDB debugging).
#    $ make all
#
#    $ make clean       Type this command after editing this Makefile or
#                       when switching between all and release targets.
#
#    $ make format      Run clang-format on *.c and *.h
#
#    $ make release     Make the sorting program (without debugging info).
#                       We use this for the reference design.


CC=clang
debug:   CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic -g -gdwarf-4
release: CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic
LDFLAGS=-lm
EXECBIN=tsp
OBJS=\
    graph.o \
    path.o \
    stack.o \
    tsp.o \

.PHONY: all debug release clean

all: debug

debug: $(EXECBIN)

release: $(EXECBIN)

$(EXECBIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c vertices.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECBIN) $(OBJS)

format:
	clang-format -i --style=file *.[ch]


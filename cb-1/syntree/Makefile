#!/usr/bin/make
.SUFFIXES:
.PHONY: all run pack clean

TAR = testsyntree
SRC = syntree.c testsyntree.c
OBJ = $(SRC:%.c=%.o)
DEP = $(OBJ:%.o=%.d)
-include $(DEP)

CFLAGS = -std=c11 -Wall -pedantic -MMD -MP

%.o: %.c
	$(CC) $(CFLAGS) $< -c

$(TAR): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

all: $(TAR)

run: all
	./$(TAR)

pack:
	@$(MAKE) -C .. pack

clean:
	$(RM) $(RMFILES) $(TAR) $(OBJ) $(DEP)

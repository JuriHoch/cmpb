#!/usr/bin/make
.SUFFIXES:
.PHONY: all run pack clean

PCK = lab-1.zip

all:
	@$(MAKE) -C stack all
	@$(MAKE) -C syntree all

run:
	@$(MAKE) -C stack run
	@$(MAKE) -C syntree run

pack:
	zip -vj $(PCK) stack/stack.[ch] syntree/syntree.[ch]

clean:
	@$(MAKE) -C stack clean
	@$(MAKE) -C syntree clean
	$(RM) $(RMFLAGS) $(PCK)

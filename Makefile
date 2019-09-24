.SILENT:
.PHONY: bsh install uninstall line 
.DEFAULT_GOAL = bsh

EXEC = bsh
CC = gcc
FLAGS = -Wall -Wextra -Werror -O3 -fPIC
CFILES = $(wildcard *.c)
ODIR = ObjectFiles
OFILES = $(wildcard o $(ODIR)/*.o)
GREP_LINE = $(shell wc -l Banana_shell.h $(CFILES) | grep -E ".[0-9]+ total")
DIR = $(shell pwd)

$(EXEC): Dir Main Banana_shell Banana_loop Banana_shell_functions Banana_print_functions

	echo "Compiling.."
	$(CC) $(FLAGS) -include Banana_shell.h $(OFILES) -lreadline -o $@
	#Create Binary Stripped, for also MacOS
	strip $@
	echo "Compilation finished successfully"

Dir:
	[ ! -d "$(ODIR)" ] && mkdir $(ODIR) || true

Main:
	$(CC) -c Main.c -o $(ODIR)/Main.o

Banana_shell:
	$(CC) -c Banana_shell.c -o $(ODIR)/Banana_shell.o

Banana_loop:
	$(CC) -c Banana_loop.c -o $(ODIR)/Banana_loop.o

Banana_shell_functions:
	$(CC) -c Banana_shell_functions.c -o $(ODIR)/Banana_shell_functions.o

Banana_print_functions:
	$(CC) -c Banana_print_functions.c -o $(ODIR)/Banana_print_functions.o

clean:
	rm -rf $(ODIR)

install: $(EXEC) clean
	sudo mv bsh /usr/bin/bsh
	echo "Installation Finished !"

uninstall:
	if [ -f "/bin/bsh" ];\
	then\
		echo "Removing...";\
		sudo rm -f /bin/bsh && echo "Remove successfully";\
	else\
		echo "Banana SHell is not installed";\
	fi;

update:
	git clone https://github.com/Cobraxo/Banana_SHell
	cd Banana_SHell; make install
	cd ../..; rm -rf $(DIR)

line: 
	echo "Banana SHell contains:\n$(GREP_LINE) lines !"

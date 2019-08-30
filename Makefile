.SILENT:
.PHONY: bsh install uninstall line 

EXEC = bsh
CC = gcc
CFILES = $(wildcard *.c)
OFILES = $(wildcard o ObjectFiles/*.o)

GREP_LINE = $(shell wc -l Banana_shell.h $(CFILES) | grep -E ".[0-9]+ total")

.DEFAULT_GOAL = $(EXEC)

$(EXEC): Main Banana_shell Banana_loop Banana_shell_functions Banana_print_functions
	echo "Compiling.."
	$(CC) -Wall -Wextra -Werror Banana_shell.h $(OFILES) -o $@
	echo "Compilation finished successfully"

Main:
	$(CC) -c Main.c -o ObjectFiles/Main.o

Banana_shell:
	$(CC) -c Banana_shell.c -o ObjectFiles/Banana_shell.o

Banana_loop:
	$(CC) -c Banana_loop.c -o ObjectFiles/Banana_loop.o

Banana_shell_functions:
	$(CC) -c Banana_shell_functions.c -o ObjectFiles/Banana_shell_functions.o

Banana_print_functions:
	$(CC) -c Banana_print_functions.c -o ObjectFiles/Banana_print_functions.o

clean:
	rm -f $(OFILES)

install: $(EXEC)
	sudo mv bsh /bin/bsh
	echo "Installation Finished !"

uninstall:
	if [ -f "/bin/bsh" ];\
	then\
		echo "Removing...";\
		sudo rm -f /bin/bsh && echo "Remove successfully";\
	else\
		echo "Banana SHell is not installed";\
	fi;

line: 
	echo "Banana SHell contains:\n$(GREP_LINE) lines !"

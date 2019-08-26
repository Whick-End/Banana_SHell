.SILENT:
.PHONY: bsh install uninstall line 

.DEFAULT_GOAL = bsh

EXEC = bsh
CC = gcc
SRC = $(wildcard *.c)

GREP_LINE = $(shell wc -l Banana_shell.h $(SRC) | grep -E ".[0-9]+ total")

$(EXEC): Main.c Banana_shell.c Banana_loop.c Banana_shell_functions.c Banana_print_functions.c
	echo "Compiling.."
	$(CC) -Wall -Wextra -Werror Banana_shell.h  $(SRC) -o $@
	echo "Compilation finished successfully"

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

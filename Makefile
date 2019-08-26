.SILENT:
.PHONY: compile install uninstall line 

.DEFAULT_GOAL = compile

compile:
		echo "Compiling.."
		$(CC) -Wall -Wextra -Werror Banana_shell.h Main.c Banana_shell.c Banana_loop.c
		Banana_shell_functions.c Banana_print_functions.c -o bsh
		echo "Compile successfully"

install: compile
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

line: line_out
	echo "Banan SHell contains:\n$(GREP_LINE) lines !"

line_out:
	GREP_LINE := $(shell wc -l Banana_loop.c Banana_shell.c Banana_shell_functions.c Banana_shell.h Banana_print_functions.c Main.c | grep -E ".[0-9]+ total")

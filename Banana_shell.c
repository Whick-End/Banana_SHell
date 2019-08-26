#include "Banana_shell.h"


int banana_shell(void) {

    int status_banana_loop = 1;

    /***
    * 
    * By default status_banana_loop = 1
    * If user want to quit the shell, status_banana_loop = 0
    * 
    * Else if any errors :
    * status_banana_loop = -1, and banana_shell print the error, without crash
    * 
    ***/

   CLEAR_SHELL

    while (status_banana_loop) {

        errno = 0;

        status_banana_loop = banana_loop();

        // If any error
        if (status_banana_loop == -1 || errno)
            perror("");

    }

    // Return 0
    return status_banana_loop;

}

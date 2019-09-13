#include "Banana_shell.h"


int print_banana(void) {

    // If user input is banana

    CLEAR_SHELL

    (void)printf("\n\
                  %s(#\n\
               @ #@#\n\
              @&@###\n\
     # ###*   (@@###      .(###\n\
  ########@@###@(@#@   ##@##&&@\n\
    .@@@@@#####@#@(@ (@(#&\n\
          &@##@###@@@@&@@@@@@@@@#@@\n\
              %s,***,*%s#%s,/*(,*%s&@##@@@#@@@ \n\
           @#%s/(,/*//,/,/(*/*%s###&#@@&&&@\n\
         @###%s***/**/,/*//*//%s##&###\n\
         (#&(#%s**/**/***//*/*  %s.#&##\n\
        @#((#@&*%s(/*/(#**(*/       %s#\n\
        ###&&      %s((@(@#\n\
                   (##(#@              %sBanana Shell%s\n\
                   ((#(#@                 %s-Author: %sCobraxo%s\n\
                   ((#(@#                 %s-Date: %s09/13/2019%s\n\
                  #(##((##.               %s-Github: %shttps://github.com/Cobraxo/%s\n\
                  ##@(##(#                %sHave a fun ! ^^%s\n\
                  #####(@\n\
                ###@##@@##\n\
                ######@@@%s\n\
    \n",

    GREEN, YELLOW, GREEN, YELLOW, GREEN, YELLOW, GREEN, YELLOW, GREEN,
    YELLOW, GREEN, YELLOW, GREEN, BROWN, YELLOW, BROWN, RESET, BLUE, 
    BROWN, RESET, BLUE, BROWN, RESET, BLUE, BROWN, DARK_BLUE, BROWN, RESET);

    return 0;

}

int print_banana_help(void) {

    // If user input is help

    CLEAR_SHELL

    (void)printf("\n\
                      %s(@\n\
                      (#\n\
                      /#(\n\
                    ***/##/       %sBanna Shell Command%s\n\
                   /***/(##         %s-exit    %s// To exit the shell%s\n\
                   ****/(##(        %s-help    %s// To get help like that%s\n\
                   ****/(###        %s-banana  %s// To see the Author and his github%s\n\
                   *****(###           %sEnjoy !%s\n\
                   ****#(###       &\n\
                  ****/((###     /*(*\n\
                  ****/*(((#   **//(\n\
                  ****/((#(##///((((\n\
                 *****//(((#######*\n\
 &/(           *******////###\n\
  ,****/%%**********  ****/(##\n\
     /**********/*     *****/(#\n\
          ..            ******//(\n\
                         *****/\n\
                           **/(%s\n\n\n\
    \n",

    LIGHT_YELLOW, YELLOW, LIGHT_YELLOW, RESET,
    BLUE, LIGHT_YELLOW, RESET, BLUE, LIGHT_YELLOW,
    RESET, BLUE, LIGHT_YELLOW, DARK_BLUE, LIGHT_YELLOW, RESET);

    return 0;

}

int print_version(void) {

    puts("Banana SHell 1.3");
    return 0;

}

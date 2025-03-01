#include "file.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_stdin() {
    char c;
    while ((c=getchar() != '\n' && c != EOF));
    return;
}

void format_input(char *input) {
    unsigned first_alpha_index;
    unsigned last_char_index;
    unsigned char first_alpha_found;
    unsigned char spaces_only;

    first_alpha_found = 0;
    spaces_only = 1;

    if (strchr(input, '\n') == NULL) {
        clear_stdin();
    } else {
        input[strlen(input) - 1] = '\0';
    }

    for (unsigned i = 0; input[i] != '\0'; i++) {
        if (input[i] != ' ') {
            last_char_index = i;
            spaces_only = 0;

            if (first_alpha_found == 0) {
                first_alpha_index = i;
                first_alpha_found = 1;
            }
        }
    }

    if (spaces_only == 1) {
        input[0] = '\0';

        return;
    }

    for (unsigned i = first_alpha_index; i <= last_char_index; i++) {
        input[i - first_alpha_index] = input[i];
    }

    input[last_char_index - first_alpha_index + 1] = '\0';

    return;
}

void user_input(char *input, int length, char *prompt, char break_line) {
    printf("%s", prompt);

    if (break_line) {
        printf("\n");
    }

    fgets(input, length, stdin);

    format_input(input);

    return;
}

int validate_input(char *input) {
    char invalid_input = 0;

    if (input == NULL || input[0] == '\0' || input[0] == ' ') {
        return 1;
    }

    return invalid_input;
}

int validate_int_input(char *input, int min, int max) {
    char invalid_input = 0;

    invalid_input = validate_input(input);

    for (unsigned i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            invalid_input = 1;
            break;
        }
    }

    if (atoi(input) > max || atoi(input) < min) {
        invalid_input = 1;
    }

    return invalid_input;
}

int there_is_space(char *str){
    for (int i = 0; str[i] != '\0'; i++){
        if(str[i] == ' '){
            return 1;
        }
    }
    return 0;
}

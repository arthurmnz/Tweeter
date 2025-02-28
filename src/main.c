#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio_ext.h>
#include "user.h"
#include "post.h"
#include "file.h"
#include "trending.h"
#include "timeline.h"
#include "input.h"

user *current_user = NULL;

int main() {
    char choice_buffer[100];
    int user_choice;
    unsigned char first_login;

    first_login = 1;

    create_file(USER_FILE_NAME);
    create_file(POST_FILE_NAME);

    printf("Bem vindo(a) ao\n");
    printf(" _____                   _            \n"
        "|_   _|_      _____  ___| |_ ___ _ __ \n"
        "  | | \\ \\ /\\ / / _ \\/ _ \\ __/ _ \\ '__|\n"
        "  | |  \\ V  V /  __/  __/ ||  __/ |   \n"
        "  |_|   \\_/\\_/ \\___|\\___|\\__\\___|_|   \n\n"
    );

    while (current_user == NULL) {
        printf("-------------------\n  [ 1 ] Entrar\n  [ 2 ] Cadastrar\n  [ 3 ] Sair\n-------------------\n\n");
        user_input(choice_buffer, sizeof(choice_buffer), "Opção: ", 0);
        if (validate_int_input(choice_buffer, 1, 3) == 1) {
            printf("\nEscolha inválida!\n\n");
            continue;
        }
        user_choice = atoi(choice_buffer);
        switch (user_choice) {
        case 1:
            current_user = sing_in(); 
            break;
        case 2:
            sing_up();
            break;
        case 3:
            return 1;
            break;
        default:
            printf("\nEscolha inválida!\n\n");
            break;
        }
    while (current_user != NULL) {
        if (first_login == 1) {
            printf("\nBem vindo(a), %s\n", current_user->username);
            first_login = 0;
        }

            printf("\n-------------------------\n  [ 1 ] Novo post\n  [ 2 ] Trending Topics\n  [ 3 ] Timeline\n  [ 4 ] logout\n-------------------------\n\n");
            user_input(choice_buffer, sizeof(choice_buffer), "Opção: ", 0);
            if (validate_int_input(choice_buffer, 1, 4) == 1) {
                printf("\nEscolha inválida!\n\n");
                continue;
            }
            user_choice = atoi(choice_buffer);
            switch (user_choice)
            {
            case 1:
                post_up(current_user);
                break;
            case 2:
                show_trending_topics(INITIAL_TRENDING_TOPICS_CAPACITY);
                break;
            case 3:
                show_timeline(TIMELINE_LENGTH);
                break;
            case 4:
                log_out(current_user);
                current_user = NULL;
                first_login = 1;
                break;
            default:

                break;
            }
        }
    }
    return 0;
}

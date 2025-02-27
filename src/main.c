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

    create_file(USER_FILE_NAME);
    create_file(POST_FILE_NAME);

    while (current_user == NULL){
        printf("---\n[ 1 ] Sing-in\n[ 2 ] Sing-up\n[ 3 ] Sair\n---\n\n");
        user_input(choice_buffer, sizeof(choice_buffer), "Opção: ", 0);
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
            printf("Escolha invalida!");
            break;
        }
    }
    while (1){
        printf("---\n[ 1 ] Post\n[ 2 ] Trending Topics\n[ 3 ] Timeline\n[ 4 ] Sair\n---\n\n");
        user_input(choice_buffer, sizeof(choice_buffer), "Opção: ", 0);
        user_choice = atoi(choice_buffer);
        switch (user_choice){
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
            return 1;
            break;
        default:

            break;
        }
    }
    return 0;

} 

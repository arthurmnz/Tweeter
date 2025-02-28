#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <time.h>
#include "post.h"
#include "user.h"
#include "input.h"


void post_up(user *current_user){
    struct tm *timeinfo;
    time_t seconds;

    post *p = (post *) malloc(sizeof(post));
    if(p == NULL){
        printf("Memory allocation failed\n");
        return;
    }

    //autor do post
    strcpy(p->username, current_user->username);

    //criacao do post
    time(&seconds);
    timeinfo = localtime(&seconds);
    sprintf(p->create_at, "%02d:%02d:%02d %02d/%02d/%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1 ,timeinfo->tm_year + 1900);
    p->create_at[strcspn(p->create_at, "\n")] = 0;
    
    //texto do post
    char buffer[MAX_TAM_TEXT_POST];
    user_input(buffer, MAX_TAM_TEXT_POST, "Digite o conteúdo: ", 0);
    printf("\n");
    if (validate_input(buffer) == 1) {
        printf("O post não pode ser vazio\n\n");
        return;
    }
    strcpy(p->text, buffer);

    FILE *user_add = fopen(POST_FILE_NAME, "a");

    fprintf(user_add, "%s|%s|%s\n", p->username, p->create_at, p->text);
    fclose(user_add);

    printf("Post: %s\n\n", p->text);
}

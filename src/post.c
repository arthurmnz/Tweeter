#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <time.h>
#include "post.h"
#include "user.h"
#include "input.h"
#include "trending.h"


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

    printf("Post enviado com sucesso!\n\n");
}

int show_posts_in_topic(char *topic_name, unsigned max_posts) {
    FILE *posts_ptr;
    char buffer[500];
    char *username, *time, *text;
    char formatted_time[6];
    char topic[50];
    unsigned topic_exist;
    unsigned topic_found;
    unsigned topic_start;
    unsigned posts_counter;
    long line_end;
    long pos;

    posts_counter = 0;

    posts_ptr = fopen(POST_FILE_NAME, "r");

    if (posts_ptr == NULL){
        printf("Error while reading %s\n", POST_FILE_NAME);
        return 1;
    }

    if (max_posts == 0) {
        max_posts = 10;
    }


    if (fseek(posts_ptr, 0, SEEK_END) != 0) {
        perror("Error seeking in file");
        fclose(posts_ptr);
        return 1;
    }

    pos = ftell(posts_ptr);

    if (pos <= 0) {
        fclose(posts_ptr);
        return 1;
    }

    line_end = pos;

    while (pos >= 0) {
        fseek(posts_ptr, pos, SEEK_SET);
        int ch = fgetc(posts_ptr);

        if (ch == '\n' || pos == 0) {
            long line_start = (pos == 0) ? pos : pos + 1;
            size_t line_length = line_end - line_start;

            if (line_length > 0) {
                fseek(posts_ptr, line_start, SEEK_SET);
                fread(buffer, 1, line_length, posts_ptr);
                buffer[line_length] = '\0';

                username = strtok(buffer, "|");
                time = strtok(NULL, "|");
                text = strtok(NULL, "\n");

                topic_exist = 0;
                topic_found = 0;

                if (text != NULL) {
                    for (unsigned i = 0; text[i] != '\0'; i++) {
                        if (text[i] == '#') {
                            topic[0] = '\0';
                            topic_found = 1;
                            topic_start = i + 1;
                        } else if (topic_found == 1 && is_hashtag_end(text[i]) == 0 && i - topic_start + 1 < sizeof(topic)) {
                            topic[i - topic_start] = text[i];
                            topic[i - topic_start + 1] = '\0';

                            if (topic_found == 1 && is_hashtag_end(text[i + 1]) == 1 && topic[0] != '\0' && strcmp(topic, topic_name) == 0) {
                                topic_exist = 1;
                                break;
                            }
                        }
                    }
                }

                if (text != NULL && topic_exist == 1) {
                    strncpy(formatted_time, time, 5);
                    printf("@%s às %s - \"%s\"\n", username, formatted_time, text);
                    posts_counter++;
                }

                if (posts_counter == max_posts) {
                    break;
                }
            }

            line_end = pos;
        }

        pos--;
    }

    if (posts_counter == 0) {
        printf("Nenhum post encontrado neste tópico\n");
    }

    printf("\n");

    fclose(posts_ptr);

    return 0;
}

int search_topic(unsigned max_posts) {
    char buffer[100];
    char topic_name[50];

    user_input(buffer, sizeof(buffer), "Digite o tópico: ", 0);

    if (validate_input(buffer) == 1) {
        return 1;
    }

    printf("\n");

    strncpy(topic_name, buffer, sizeof(topic_name));

    return show_posts_in_topic(topic_name, max_posts);
}

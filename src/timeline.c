#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <ctype.h>
#include <time.h>
#include "timeline.h"
#include "post.h"
#include "trending.h"

int show_timeline(unsigned max_posts) {
    FILE *posts_ptr;
    char buffer[500];
    char *username, *time, *text;
    char formatted_time[6];
    char topic[50];
    unsigned posts_counter;
    unsigned topic_found;
    unsigned topic_start;
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

                strncpy(formatted_time, time, 5);

                printf("@%s às %s - \"", username, formatted_time);

                topic_found = 0;

                for (unsigned i = 0; text[i] != '\0'; i++) {
                    if (text[i] == '#') {
                        topic[0] = '\0';
                        topic_found = 1;
                        topic_start = i + 1;

                        if (is_hashtag_end(text[i + 1]) == 1) {
                            printf("%c", text[i]);
                        }
                    } else if (topic_found == 1 && is_hashtag_end(text[i]) == 0 && i - topic_start + 1 < sizeof(topic)) {
                        topic[i - topic_start] = text[i];
                        topic[i - topic_start + 1] = '\0';

                        if (topic_found == 1 && is_hashtag_end(text[i + 1]) == 1 && topic[0] != '\0') {
                            printf("\033[34m");
                            printf("#%s", topic);
                            printf("\033[0m");
                            topic_found = 0;
                        }
                    } else if (topic_found == 0) {
                        printf("%c", text[i]);
                    }
                }

                printf("\"\n");

                posts_counter++;

                if (posts_counter == max_posts) {
                    break;
                }
            }

            line_end = pos;
        }

        pos--;
    }

    printf("\n");

    fclose(posts_ptr);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <ctype.h>
#include <time.h>
#include "timeline.h"
#include "post.h"

int show_timeline(unsigned max_posts) {
    FILE *posts_ptr;
    char buffer[500];
    char *username, *time, *text;
    char formatted_time[6];
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

                strncpy(formatted_time, time, 5);

                printf("@%s às %s - \"%s\"\n", username, formatted_time, text);

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

#ifndef POST_H
#define POST_H

#define MAX_TAM_USERNAME  50
#define MAX_TAM_TEXT_POST 500
#define POST_FILE_NAME "../files/posts.txt"


#include "user.h"

typedef struct Post{
    char username[MAX_TAM_USERNAME];
    char create_at[50];
    char text[MAX_TAM_TEXT_POST];
}post;

void post_up(user *);

#endif //POST_H

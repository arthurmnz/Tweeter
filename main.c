#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct User{
    int id; 
    char user_name[20];
    char password[50];
}user;

typedef struct Post{
    char user_name[50];
    char create_at[50];
    char text[500];
    struct Post *next;
}post;

typedef struct Topic {
    char name[50];
    unsigned posts_count;
} topic;

typedef struct TrendingTopics {
    topic *topics;
    unsigned size;
    unsigned max_topics;
} trending_topics;

user *current_user = NULL;
post *start_post = NULL;
post *end_post = NULL;
struct tm *timeinfo;
time_t seconds;

int str_is_valid(char *str){
    for (int i = 0; str[i] != '\0'; i++){
        if(str[i] == ' '){
            return 0;
        }
    }
    return 1;
}

int get_last_id(){
    FILE *f = fopen("users.txt", "r");
    if(f == NULL){
        printf("Error opening file\n");
        return -1;
    }

    int id = 0;
    char buffer[500];
    while(fgets(buffer, 500, f) != NULL){
        id = atoi(strtok(buffer, "\n"));
    }

    fclose(f);
    return id;
}

void add_user(user *u){
    FILE *f = fopen("users.txt", "a");
    if(f == NULL){
        printf("Error opening file\n");
        return;
    }

    u->id = get_last_id() + 1;
    fprintf(f, "%d|%s|%s\n", u->id, u->user_name, u->password);
    fclose(f);
}

void sing_up(){
    char buffer_name[50];
    char buffer_password[50];
    user *u = (user *) malloc(sizeof(user));
    if(u == NULL){
        printf("Memory allocation failed\n");
        return;
    }    
    do{
        printf("Username não pode conter espaços\n");
        printf("Enter username: ");
        fgets(buffer_name, 50, stdin);
        strcpy(u->user_name, buffer_name);
        u->user_name[strcspn(buffer_name, "\n")] = 0;
        __fpurge(stdin);
        if (!str_is_valid(u->user_name))
            printf("Username Inválido!!\n");
    } while (!str_is_valid(u->user_name));

    
    do{
        printf("Password não pode conter espaços\n");
        printf("Enter password: ");
        fgets(buffer_password, 50, stdin);
        strcpy(u->password, buffer_password);
        u->password[strcspn(buffer_password, "\n")] = 0;
        __fpurge(stdin);
        if(!str_is_valid(u->password))
            printf("Password Inválida!!\n");
    } while (!str_is_valid(u->password));

    add_user(u);
    free(u);
}

void sing_in(){
    char buffer_name[50];
    char buffer_password[50];
    user *u = (user *) malloc(sizeof(user));
    if(u == NULL){
        printf("Memory allocation failed\n");
        return;
    }    

    printf("Enter user name: ");
    fgets(buffer_name, 50, stdin);
    strcpy(u->user_name, buffer_name);
    u->user_name[strcspn(buffer_name, "\n")] = 0;
    __fpurge(stdin);

    printf("Enter password: ");
    fgets(buffer_password, 50, stdin);
    strcpy(u->password, buffer_password);
    u->password[strcspn(buffer_password, "\n")] = 0;
    __fpurge(stdin);

    FILE *f = fopen("users.txt", "r");
    if(f == NULL){
        printf("Error opening file\n");
        return;
    }

    char buffer[500];
    while(fgets(buffer, 500, f) != NULL){
        int id = atoi(strtok(buffer, "|"));
        char *user_name = strtok(NULL, "|");
        char *password = strtok(NULL, "\n");
        u->id = id;

        if(strcmp(u->user_name, user_name) == 0 && strcmp(u->password, password) == 0){
            current_user = u;
            fclose(f);
            return;
        }
    }
    printf("User not found\n");
    fclose(f);
    free(u);
}   

void add_post(post *p){
    if(start_post == NULL){
        start_post = p;
        end_post = p;
        return;
    }else{
        end_post->next = p;
        end_post = p;
    }
}

void post_up(){

    if(current_user == NULL){
        printf("You need to sing in first\n");
        return;
    }
    post *p = (post *) malloc(sizeof(post));
    if(p == NULL){
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(p->user_name, current_user->user_name);

    time(&seconds);
    timeinfo = localtime(&seconds);
    sprintf(p->create_at, "%d:%d:%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    p->create_at[strcspn(p->create_at, "\n")] = 0;
    
    char buffer[500];
    printf("Enter your post: ");
    fgets(buffer,500,stdin);
    strcpy(p->text, buffer);
    p->text[strcspn(buffer, "\n")] = 0;
    __fpurge(stdin);

    FILE *f = fopen("posts.txt", "a");
    if(f == NULL){
        printf("Error opening file\n");
        return;
    }
    fprintf(f, "%s|%s|%s\n", p->user_name, p->create_at, p->text);
    fclose(f);

    add_post(p);

    printf("Post: %s\n", p->text);
}

void init_trending_topics(trending_topics *ttopics, unsigned max_topics) {
    ttopics->topics = calloc(max_topics, sizeof(topic));
    ttopics->size = 0;
    ttopics->max_topics = max_topics;
}

int add_new_topic(trending_topics *ttopics, char *new_topic_name) {
    topic new_topic;

    if (ttopics == NULL || strlen(new_topic_name) == 0 || ttopics->size == ttopics->max_topics) {
        return 1;
    }

    strncpy(new_topic.name, new_topic_name, sizeof(new_topic.name));
    new_topic.posts_count = 1;

    if (ttopics->size == 0) {
        ttopics->topics[0] = new_topic;
        ttopics->size++;
        return 0;
    }
    for (unsigned i = 0; i < ttopics->size; i++) {
        if (strcmp(ttopics->topics[i].name, new_topic_name) == 0) {
            return 1;
        }

        if (ttopics->topics[i].posts_count == 1) {
            for (unsigned j = ttopics->size; j > i; j--) {
                ttopics->topics[j] = ttopics->topics[j - 1];
            }

            ttopics->topics[i] = new_topic;
            ttopics->size++;
            return 0;
        }
    }

    ttopics->topics[ttopics->size] = new_topic;
    ttopics->size++;

    return 0;
}

int increment_topic(trending_topics *ttopics, char *topic_name) {
    if (ttopics == NULL || strlen(topic_name) == 0 || ttopics->size == 0) {
        return 1;
    }

    for (unsigned i = 0; i < ttopics->size; i++) {
        if (strcmp(ttopics->topics[i].name, topic_name) == 0 && ttopics->topics[i].posts_count > 0) {
            ttopics->topics[i].posts_count++;
            return 0;
        }
    }

    return 1;
}

int is_hashtag_end(char c) {
    if (isalnum(c) == 0 && c != '_') {
        return 1;
    }

    return 0;
}

int main() {
    sing_in();
    post_up();
    post_up();
    post_up();

    return 0;

} 

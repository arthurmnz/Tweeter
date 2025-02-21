#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct User{
    int id; 
    char user_name[20];
    char password[50];
}user;

user *current_user = NULL;

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

    printf("Enter user name: ");
    scanf("%s", buffer_name);
    strcpy(u->user_name, buffer_name);
    __fpurge(stdin);

    printf("Enter password: ");
    scanf("%s", buffer_password);
    strcpy(u->password, buffer_password);
    __fpurge(stdin);

    add_user(u);
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
    scanf("%s", buffer_name);
    strcpy(u->user_name, buffer_name);


    printf("Enter password: ");
    scanf("%s", buffer_password);
    strcpy(u->password, buffer_password);


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
 
        if(strcmp(u->user_name, user_name) == 0 && strcmp(u->password, password) == 0){
            current_user = u;
            printf("User found\n");
            fclose(f);
            return;
        }
    }
    printf("User not found\n");
    fclose(f);
}   

int main() {

    return 0;
} 
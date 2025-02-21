#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct User{
    int id; 
    char user_name[20];
    char password[50];
}user;

user *current_user = NULL;

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
 
        if(strcmp(u->user_name, user_name) == 0 && strcmp(u->password, password) == 0){
            u->id = id;
            current_user = u;
            fclose(f);
            free(u);
            return;
        }
    }
    printf("User not found\n");
    fclose(f);
    free(u);
}   

int main() {

    return 0;
} 
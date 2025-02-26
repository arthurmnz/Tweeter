#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <time.h>
#include "user.h"
#include "file.h"

int str_is_valid(char *str){
    for (int i = 0; str[i] != '\0'; i++){
        if(str[i] == ' '){
            return 0;
        }
    }
    return 1;
}

int username_is_valid(char *username_entry){
    
    
    FILE *users_read = fopen(USER_FILE_NAME, "r");
    if(users_read == NULL){
        //nenhum usuario registrado
        fclose(users_read);
        return 1;
    }
    
    char buffer[500];
    while(fgets(buffer, 500, users_read) != NULL){
        char *username = strtok(buffer, "|");

        if(strcmp(username_entry, username) == 0){

            fclose(users_read);
            return 0;
        }
    }
    fclose(users_read);
    return 1;
}

void add_user(user *u){
    FILE *user_append;
    user_append = fopen(USER_FILE_NAME, "a");
    if(user_append == NULL){
        printf("Error opening file\n");
        return;
    }

    fprintf(user_append, "%s|%s\n", u->username, u->password);
    fclose(user_append);
}

void sing_up(){
    char buffer_username[MAX_TAM_USERNAME];
    char buffer_password[MAX_TAM_PASSWORD];
    user *u = (user *) malloc(sizeof(user));
    if(u == NULL){
        printf("Memory allocation failed\n");
        return;
    }
    

    do{
        printf("Username não pode conter espaços\n");
        printf("Enter username: ");
        
        fgets(buffer_username, MAX_TAM_USERNAME, stdin);
        strcpy(u->username, buffer_username);
        u->username[strcspn(buffer_username, "\n")] = 0;
        __fpurge(stdin);

        if (!str_is_valid(u->username))
            printf("\nUsername Inválido!!\n\n");
        else if(!username_is_valid(u->username))
            printf("\nUsername em uso!!\n\n");
    } while (!str_is_valid(u->username) || !username_is_valid(u->username));

    do{
        printf("Password não pode conter espaços\n");
        printf("Enter password: ");

        fgets(buffer_password, MAX_TAM_PASSWORD, stdin);
        strcpy(u->password, buffer_password);
        u->password[strcspn(buffer_password, "\n")] = 0;
        __fpurge(stdin);

        if(!str_is_valid(u->password))
            printf("\nPassword Inválida!!\n\n");
            
    } while (!str_is_valid(u->password));

    add_user(u);
    free(u);
}

user* sing_in(){   
    FILE *user_read = fopen(USER_FILE_NAME, "r");
    if(user_read == NULL){
        printf("Nenhum usuario cadastrado\n");
        fclose(user_read);
        return NULL;
    }

    char buffer_name[MAX_TAM_USERNAME];
    char buffer_password[MAX_TAM_PASSWORD];
    user *u = (user *) malloc(sizeof(user));
    if(u == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }    

    printf("Enter user name: ");

    fgets(buffer_name, MAX_TAM_USERNAME, stdin);
    strcpy(u->username, buffer_name);
    u->username[strcspn(buffer_name, "\n")] = 0;
    __fpurge(stdin);

    printf("Enter password: ");

    fgets(buffer_password, MAX_TAM_PASSWORD, stdin);
    strcpy(u->password, buffer_password);
    u->password[strcspn(buffer_password, "\n")] = 0;
    __fpurge(stdin);

    char buffer[500];
    while(fgets(buffer, 500, user_read) != NULL){
        char *username = strtok(buffer, "|");
        char *password = strtok(NULL, "\n");

        if(strcmp(u->username, username) == 0 && strcmp(u->password, password) == 0){
            fclose(user_read);
            return u;
        }
    }
    printf("Usuario ou senha nao encontrado!!\n");
    fclose(user_read);
    free(u);
    return NULL;
}   

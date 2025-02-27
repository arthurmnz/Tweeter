#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "user.h"
#include "file.h"
#include "input.h"

int username_in_use(char *username_entry){    
    FILE *users_read;
    char buffer[500];
    
    users_read= fopen(USER_FILE_NAME, "r");
    while(fgets(buffer, 500, users_read) != NULL){
        char *username = strtok(buffer, "|");
        strtok(NULL, "\n");

        if(strcmp(username_entry, username) == 0){

            fclose(users_read);
            return 1;
        }
    }
    fclose(users_read);
    return 0;
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
        user_input(buffer_username, MAX_TAM_USERNAME, "Digite username: ", 0);

        if (there_is_space(buffer_username))
            printf("\nUsername não pode ter espaços!!\n\n");

        else if(username_in_use(buffer_username))
            printf("\nUsername em uso!!\n\n");

    } while (there_is_space(buffer_username) || username_in_use(buffer_username));
    
    do{
        user_input(buffer_password, MAX_TAM_PASSWORD, "Digite a senha: ", 0);

        if(there_is_space(u->password))
            printf("\nPassword não pode ter espaços!!\n\n");
            
    } while (there_is_space(u->password));

    strcpy(u->username, buffer_username);
    strcpy(u->password, buffer_password);

    add_user(u);
    free(u);

    printf("\nUsuario cadastrado com sucesso!!\n\n");
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
    clear_stdin();

    printf("Enter password: ");

    fgets(buffer_password, MAX_TAM_PASSWORD, stdin);
    strcpy(u->password, buffer_password);
    u->password[strcspn(buffer_password, "\n")] = 0;
    clear_stdin();

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

void sing_out(user *current_user){
    free(current_user);
    current_user = NULL;
}
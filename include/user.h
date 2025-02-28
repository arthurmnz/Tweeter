#ifndef USER_H
#define USER_H

#define MAX_TAM_USERNAME  50
#define MAX_TAM_PASSWORD 50
#define USER_FILE_NAME "../files/users.txt"


typedef struct User{
    char username[MAX_TAM_USERNAME];
    char password[MAX_TAM_PASSWORD];
}user;

int username_in_use(char *);
void add_user(user *);
void sing_up();
user* sing_in();
void log_out(user *);

#endif //USER_H
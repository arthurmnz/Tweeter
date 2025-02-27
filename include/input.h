#ifndef INPUT_H
#define INPUT_H

void clear_stdin();
void format_input(char*);
void user_input(char*, int, char*, char);
int validate_input(char*);
int validate_int_input(char*, int, int);
int there_is_space(char *);

#endif // INPUT_H

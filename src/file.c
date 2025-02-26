#include <stdio.h>
#include "file.h"
#include <linux/limits.h>

void create_file(char *file_name){
    FILE *f = fopen(file_name,"a");
    fclose(f);
}



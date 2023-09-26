#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct File{
    int id;
    int size;
    struct Directory* directory;
    char name[63];
    char data[1024];
};

struct Directory{
    unsigned char nf;
    unsigned char nd;
    char path[2048];
    struct Directory* directories[256];
    struct File* files[256];
    char name[63];
};



void overwrite_to_file(struct File* file, const char* str){
    if (file != NULL){
        strncpy(file->data, str, 1024);
        file->size = strlen(file->data);
    }
};

void append_to_file(struct File* file, const char* str){
    if (file != NULL && (file->size + strlen(str)) < 1023){
        strcat(file->data, str);
        file->size = file->size + strlen(str);
    }
};

void printp_file(struct File* file){
    if (file != NULL){
        for (int i = 0; i < 2048; i++){
            if (file->directory->path[i] == '\0'){
                break;
            }
            printf("%c", file->directory->path[i]);
        }
        printf("/%s\n", file->name);
    }
};

void add_file(struct File* file, struct Directory* dir){
    if (file != NULL && dir != NULL && dir->nf < 62){
        dir->files[dir->nf] = file;
        dir->nf++;
        file->directory = dir;
    }
};

void add_dir(struct Directory* dir1, struct Directory* dir2)
{
    if (dir1 != NULL && dir2 != NULL && dir2->nd < 255)
    {
        dir2->directories[dir2->nd] = dir1;
        dir2->nd++;
        char temp_path[2048];
        
        strcpy(temp_path, dir2->path);
        strcat(temp_path, "/");
        strcat(temp_path, dir1->name);
        strcpy(dir1->path, temp_path);
        
    }
};

int main()
{
    struct Directory root = {0, 0, "/", {NULL}, {NULL}, "root"};
    struct Directory home = {0, 0, "/home", {NULL}, {NULL}, "home"};
    struct Directory bin = {0, 0, "/bin", {NULL}, {NULL}, "bin"};
    
    add_dir(&home, &root);
    add_dir(&bin, &root);
    
    struct File bash = {1, 0, NULL, "bash", ""};
    add_file(&bash, &bin);
    
    
    struct File ex3_1 = {2, 0, NULL, "ex3_1.c", ""};
    struct File ex3_2 = {2, 0, NULL, "ex3_2.c", ""};

    
    add_file(&ex3_1, &home);
    add_file(&ex3_2, &home);
    
    overwrite_to_file(&bash, "Bourne Again Shell!!");
    overwrite_to_file(&ex3_1, "int printf(const char * format, ...);\n");
    overwrite_to_file(&ex3_2, "//This is a comment in C language\n");
    
    append_to_file(&ex3_1, "int main(){printf(”Hello World!”)}\n");
    
    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);
    
    return 0;
};

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* liste(int argc, char *argv[]){
    char *sonuc = malloc(1024);
    const char *path = ".";
    if (argc > 1) path = argv[1];

    DIR *dir = opendir(path);
    if (!dir) {
        return "hata";
    }

    int capacity = 100;
    int count = 0;
    char **file_list = malloc(capacity * sizeof(char *));

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (count >= capacity) {
            capacity *= 2;
            file_list = realloc(file_list, capacity * sizeof(char *));
        }

        file_list[count] = strdup(entry->d_name);
        count++;
    }

    closedir(dir);

    for (int i = 0; i < count; i++) {
        strcat(sonuc, file_list[i]);
        strcat(sonuc, " ");
        free(file_list[i]);
    }
    free(file_list);
    return sonuc;
}

int main(int argc, char *argv[]) {
    char *result = liste(argc, argv);
    free(result);
    return 0;
}
#include "duplicates.h"
#include <stdio.h>
#include <string.h>

#if     defined(__linux__)
extern         char *strdup(const char *s);
#endif

// f-flag functionality
// searches the generated hashtable for files whose
// SHA2 has matches that of the given filename
void find_file_paths(char *filename) {
    if (is_file(filename)){
        char *targetfile = strdup(filename);
        CHECK_ALLOC(targetfile);
        char *targethash = strSHA2(filename);
        CHECK_ALLOC(targethash);
        
        if(hashtable_find(ht, targethash)) {
            exit(EXIT_SUCCESS);
        }
    }
    else {
        usage();
        exit(EXIT_FAILURE);
    }
}

// Checks if given path is a file
bool is_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

// l-flag functionality
// lists all duplicate files found in the generated hashtable
// presented as one unique file per line, separated with tabs
void list_duplicates(void) {
    for (int i = 0; i < HASHTABLE_SIZE; ++i){
        if(ht[i] != NULL) {
            if(ht[i]->next != NULL) {
                while(ht[i] != NULL){
                printf("%s \t", ht[i]->filepath);
                ht[i] = ht[i]->next;
            }
            printf("\n");
            }
        }
    }   
}

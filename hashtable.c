#include  "duplicates.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/param.h>

#if     defined(__linux__)
extern         char *strdup(const char *s);
extern         char *realpath(const char *restrict path,
                      char *restrict resolved_path);
#endif

// Create a new, empty FILE structure 
FILESTRUCT *filestruct_new(void)
{
    return NULL;
}

// Creates a new FILE structure, holding a given filepath-name and its SHA2hash
FILESTRUCT *filestruct_new_item(char *newfilename, char *newSHA2hash) 
{
    FILESTRUCT *new         = malloc( sizeof(FILESTRUCT) );
    CHECK_ALLOC(new);
    new->filepath = strdup(newfilename);
    CHECK_ALLOC(new->filepath);
    new->SHA2hash = strdup(newSHA2hash);
    CHECK_ALLOC(new->SHA2hash);
    new->next = NULL;
    return new;
}

// Adds a new FILE structure to the head of a list
FILESTRUCT *filestruct_add(FILESTRUCT *filestruct, char *newfilename, char *newSHA2hash){
    FILESTRUCT *new = filestruct_new_item(newfilename, newSHA2hash);
    new->next = filestruct;
    return new;
}

// Finds and returns true if a given FILE structure contains a given SHA2hash
bool filestruct_find(FILESTRUCT *filestruct, char *wantedSHA2hash){
    bool found = false;
    while(filestruct != NULL) {
        // if SHA2hashes match --> they are the same file
        if(strcmp(filestruct->SHA2hash, wantedSHA2hash) == 0 ) {
            // f-flag: returns matching file path names, except the indicated file
            if (fflag != NULL && ht_initialised) {
                char buffer[PATH_MAX];
                char buffer2[PATH_MAX];
                // realpath is used to convert to an absolute pathname
                char *real = realpath(fflag, buffer);
                char *real2 = realpath(filestruct->filepath, buffer2);
                // compare absolute paths of the given filename and the ones stored in hashtble
                if(strcmp(real, real2) != 0) {
                    printf("%s\n", filestruct->filepath);
                }
            } 
            else if (ht_initialised) {  
                printf("%s\n", filestruct->filepath);
            }
            found = true;
        }
        filestruct = filestruct->next;
    }
    return found;
}

// hashing function used on SHA2hash for allocation into hashtable
// an implementation of the Fowler–Noll–Vo hash function
uint32_t hash_string(char *filehash) 
{
    uint32_t prime = 16777619;
    uint32_t hash = 0;  
    uint32_t i = 0;       
    int length = strlen(filehash); 
    
    for(i = 0 ; i < length ; i++) {
        hash *= prime;
        hash ^= (*filehash);
        filehash++;
    }
    return hash;
}

// Creates a new, empty hashtable
HASHTABLE *hashtable_new(void)
{
    HASHTABLE       *new = malloc(HASHTABLE_SIZE*sizeof(FILESTRUCT *));
    CHECK_ALLOC(new);
    return new; 
}

// adds a given filepath and SHA2hash combination into hashtable
// function calles hash_string() and filestruct_find() to find correct 
// placement in given hashtable
void hashtable_add(HASHTABLE *hashtable, char *filepath, char *SHA2hash) 
{
    uint32_t h = hash_string(SHA2hash) % HASHTABLE_SIZE;
    if (hashtable[h] != NULL) {
        if (strcmp(hashtable[h]->SHA2hash, SHA2hash) != 0) {
            while ((hashtable[h] != NULL) && (strcmp(hashtable[h]->SHA2hash, SHA2hash) != 0)) {
                h = (h+1) % HASHTABLE_SIZE;
            }
        }
    }
    hashtable[h] = filestruct_add(hashtable[h], filepath, SHA2hash);
}

// returns true if a given SHA2has is present in the hashtable 
// hash_string() is called to find relevant position in hashtable
bool hashtable_find(HASHTABLE *hashtable, char *wantedSHA2hash) 
{
    uint32_t h = hash_string(wantedSHA2hash) % HASHTABLE_SIZE;
    return filestruct_find(hashtable[h], wantedSHA2hash);
}

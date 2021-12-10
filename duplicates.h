//      CITS2002 Project 2 2021
//      Name(s): Cathy Ngo,             Student-name: Glen Nguyen
//      Student number(s): 22712411,    Student-number: 22575354 

#ifndef DUPLICATES_H
#define DUPLICATES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <inttypes.h>

// DECLARE GLOBAL PREPROCESSOR CONSTANTS
#define DT_DIR           4
#define DT_REG           8
#define HASHTABLE_SIZE   7919
#define PATH_MAX         1024
#define SHA_SIZE         65

//  A HELPFUL PREPROCESSOR MACRO TO CHECK IF ALLOCATIONS WERE SUCCESSFUL
#define CHECK_ALLOC(p) if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }

// A FILE DATATYPE - STORES FILEPATH, FILE HASH, AND A POINTER TO THE NEXT FILE
typedef struct _filestruct {
    char                 *filepath;
    char                 *SHA2hash;
    struct _filestruct   *next;
} FILESTRUCT;

// DECLARE GLOBAL VARIABLES
char *progname;
extern  int             filecount; 
extern  int             uniquefiles; 
extern  long long       totalsize; 
extern  long long       minimumsize; 

bool          aflag;
bool          lflag;  
bool          qflag;

char          *fflag;
char          *hflag;

char          *targethash;
bool          ht_initialised;

// DECLARE GLOBAL FUNCTIONS
extern void         usage(void);
extern	char        *strSHA2(char *filename);
extern  void        read_directory(char *dirname);
extern  void        processfile(char *filename, struct stat buffer);
extern  void        find_file_paths(char *filename);

// ------------------------------------------------------------------------------------------------//
// FUNCTIONS RELATING TO FILESTRUCT CREATION AND INTERACTION WITH HASHTABLE
// CREATE A NEW EMPTY FILE STRUCTURE
extern  FILESTRUCT *filestruct_new(void);
// ADD A NEW FILE STRUCTURE TO AN EXISTING LIST IN HASHTABLE
extern  FILESTRUCT *filestruct_add(FILESTRUCT *filestruct, char *newfilename, char *newSHA2hash);
// DETERMINE IF A REQUIRED ITEM (FILE) IS STORED IN A GIVEN LIST IN HASHTABLE
extern  bool list_find(FILESTRUCT *filestruct, char *wanted);
// ------------------------------------------------------------------------------------------------//

// ------------------------------------------------------------------------------------------------//
// FUNCTIONS RELATING TO HASHTABLE IMPLEMENTATION
// DEFINE A HASHTABLE AS A DYNAMICALLY ALLOCATED ARRAY OF FILESTRUCTS
typedef         FILESTRUCT * HASHTABLE; 
HASHTABLE       *ht;
// ALLOCATE SPACE FOR A NEW HASHTABLE
extern  HASHTABLE *hashtable_new(void);
// ADD A NEW PROCESSED FILE TO A GIVEN HASHTABLE
extern  void hashtable_add(HASHTABLE *, char *newfilepath, char *newSHA2hash);
// DETERMINE IF A REQUIRED PROCESSED FILE IS ALREADY IN A GIVEN HASH TABLE
extern  bool hashtable_find(HASHTABLE *, char *wantedSHA2hash);
extern  uint32_t hash_string(char *filehash);
// ------------------------------------------------------------------------------------------------//

// FIND AND LIST DUPLICATE FILES FOR L-FLAG IMPLEMENTATION
extern  void list_duplicates(void);
// CHECKS IF PATH IS A FILE FOR F-FLAG IMPLEMENTATION
bool is_file(const char *path);

#endif 


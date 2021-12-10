#include  "duplicates.h"    
#include  <stdio.h>
#include  <stdbool.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <dirent.h>
#include  <sys/param.h>
#include  <string.h>
#include  <stddef.h>

#if     defined(__linux__)
extern         char *strdup(const char *s);
#endif

// Reads directories by providing an open directory stream
// File data is stored in a stat buffer
// If read_directory encounters another directory, the function is recursively called
void read_directory(char *dirname)
{
    DIR *dirp = opendir(dirname);
    struct dirent *dp;  

    // ENSURE THAT WE CAN OPEN THE DIRECTORY
    if(dirp == NULL) {
        perror(dirname);
        exit(EXIT_FAILURE);
    }

    while ( (dp = readdir(dirp) ) != NULL) {
        char filepath[MAXPATHLEN]; 
        // returns formatted string filepath
        sprintf(filepath, "%s/%s", dirname, dp->d_name); 

        // statinfo contains file data 
        struct stat     statinfo;                                 
        stat(dp->d_name, &statinfo);

        // if read_directory encounters a file
        if (dp->d_type == DT_REG) {
            if (strstr(dp->d_name, ".") == dp->d_name) {     
                // aflag indicates if the program should consider hidden files       
                if (aflag) {
                    processfile(filepath, statinfo);
                }
                continue;    
            }         
            processfile(filepath, statinfo);
        }
        // if read_directory encounters a sub-directory
        else if (dp->d_type == DT_DIR) {
            // ignore directories . and .. 
            if ((strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0)) {
                continue;
            }
            // if aflag is enabled, consider and recursively call for hidden directories
            if (strstr(dp->d_name, ".") == dp->d_name) {            
                if (aflag) {
                    processfile(filepath, statinfo);
                }
                continue; 
                }
            // read and process all other directories as normal
            read_directory(filepath);
        }
    }
    // close directory when done 
    closedir(dirp);
}

// processfile takes in the filepath from sprintf and the statinfo buffer from readdir
void processfile(char *filepath, struct stat buffer) {

    // Metrics for use in hashtable
    char* tempname = strdup(filepath);
    char* temphash = strSHA2(filepath);
    
    // increment filecount to record total files processed
    // increment total file size processed
    ++filecount;
    totalsize += buffer.st_size;  

    // if the file being processed isn't in the hashtable 
    // increment unique files and minimum total file size
    if (!(hashtable_find(ht, temphash))) {
        ++uniquefiles;            
        minimumsize += buffer.st_size;
    }

    // else if a duplicate has been found, program exits
    else {
        if (qflag){
        exit(EXIT_SUCCESS);
        }
    }
    // Adds all processed files into hashtable, even the duplicate ones!
    hashtable_add(ht, tempname, temphash);
}

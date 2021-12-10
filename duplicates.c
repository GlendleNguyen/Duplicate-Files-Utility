#include  "duplicates.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>

#if     defined(__linux__)
extern         char *strdup(const char *s);
#endif

// Options list for standard project usage
#define OPTLIST     "aAf:h:lq"

// Report the program's synopsis, should command-line processing detect any errors.
void usage(void)
{
    fprintf(stderr, 
            "\nUsage : %s [options] directoryname\n\n", progname);
    fprintf(stderr,    
            "options: \n");
    fprintf(stderr, 
            " -a\t check all files in directory\n");
    fprintf(stderr, 
            " -A\t indicates advanced project attempt\n");
    fprintf(stderr, 
            " -l\t list all duplicates found\n");
    fprintf(stderr, 
            " -q\t quietly test if any duplicates are found\n\n");
    fprintf(stderr, 
            " -f\t [filename]\t find relative pathnames of files that\n");
    fprintf(stderr, 
            "\t\t\t match the SHA2hash of the given filename\n\n");
    fprintf(stderr, 
            " -h\t [SHA2hash]\t find relative pathnames of files that\n");
    fprintf(stderr, 
            "\t\t\t match the given SHA2hash\n\n");
    
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    progname = argv[0];
    int opt;

    lflag = false; 
    qflag = false;  
    aflag = false; 
    
    fflag = NULL;
    hflag = NULL;

    opterr = 0;
    
    while ((opt = getopt(argc, argv, OPTLIST)) != -1) {
        switch (opt) {
            
            // Advanced version of project
            case 'A' : 
            exit(EXIT_FAILURE);
            break;

            // Check all files in directory
            case 'a' : aflag = !aflag;               
            break;

            // List duplicate files found
            case 'l' : lflag = !lflag;
            break; 

            // Program quietly tests if duplicates exist
            case 'q' : qflag = !qflag;
            break; 

            // find relative pathnames of files which match the SHA2hash of 
            // the given file 
            case 'f' : fflag = strdup(optarg);        
                       CHECK_ALLOC(fflag);
            break;

            // find relative pathnames of files whose SHA2hash matches
            // that of the given SHA2hash
            case 'h' : hflag = strdup(optarg);
                       CHECK_ALLOC(hflag);
            break;            

            // if arguments are unrecognised
            default: fprintf(stderr, "%s : Unknown argument -%c\n", 
                     progname, optopt);
                     argc = -1;
            break;
                     
        }
    }
    
// Check if number of directories is valid
    if(argc < 2) {
        usage();
    } 
    else {
        // Generate a new, empty hashtable 
        ht = hashtable_new();

        if (fflag != NULL) {
            read_directory(".");
            ht_initialised = true;
            find_file_paths(fflag);
        }

        if (hflag != NULL) {
            read_directory(".");
            ht_initialised = true;
            if(hashtable_find(ht, hflag)){
                exit(EXIT_SUCCESS);
            }
            else {
                exit(EXIT_FAILURE);}
        }

        if (lflag) {
            if(argc == 2) {
                read_directory(".");
                ht_initialised = true;
                list_duplicates();
                exit(EXIT_SUCCESS);
            }
            usage();
            exit(EXIT_FAILURE);
        }

        if (qflag) {
            if(argc == 2) {
                read_directory(".");
            }
            usage();
            exit(EXIT_FAILURE);
        }

        else {
            read_directory(argv[argc-1]);
        }     

        // Print out statistics
        printf("%d\n", filecount);
        printf("%lli\n", totalsize);
        printf("%d\n", uniquefiles);
        printf("%lli\n", minimumsize);

        if (qflag) {
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }
    return 0; 
} 

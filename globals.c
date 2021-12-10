#include  "duplicates.h"             
#include <stdbool.h>
#include <string.h>

// initialise global variables that track metrics
int filecount = 0;
int uniquefiles = 0;
long long totalsize = 0;
long long minimumsize = 0;

// true if hash table initialised
bool ht_initialised = false;

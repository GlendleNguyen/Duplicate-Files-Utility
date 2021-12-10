# A Makefile to build our 'duplicates' project

PROJECT = duplicates
HEADERS = $(PROJECT).h
C11	= cc -std=c11
CFLAGS = -Wall -Werror -pedantic
OBJ = duplicates.o globals.o readdir.o strSHA2.o hashtable.o findduplicates.o

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $< $(HEADERS)

clean:
	rm -f $(PROJECT) $(OBJ)
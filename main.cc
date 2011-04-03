#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "main.h"

using namespace std ;

// Usage of the program
void usage(){
	printf("Usage:\n\t ./hw5 tablecheck -t=tablefile\n") ;
	printf("\t ./hw4 encrypt -k=keyfile -t=tablefile [file]\n") ;
	printf("\t ./hw4 decrypt -k=key -t=tablefile [file]\n") ;
	printf("\t ./hw4 encrypt3 -k=key3 -t=tablecheck [file]\n") ;
	printf("\t ./hw4 decrypt3 -k=key3 -t=tablecheck\n") ;
	exit(0) ;
}


// Main function
int main(int argc, char **argv){


	int choice ;
	char *tablefile, *key ;
	FILE *inpF ;


	// Parse the command line 
	if (argc < 3){
		usage() ;
	}
	else {
		argv++ ;
		if (strcmp(*argv, "tablecheck") == 0) {
			choice = 1 ;
		} else if (strcmp(*argv, "encrypt") == 0) {
			choice = 2 ;
		} else if (strcmp(*argv, "decrypt") == 0) {
			choice = 3 ;
		} else if (strcmp(*argv, "encrypt3") == 0) {
			choice = 4 ;
		} else if (strcmp(*argv, "decrypt3") == 0) {
			choice = 5 ;
		} else {
			usage() ;
		}

		argv++ ;

		// keygen chosen
		if (choice == 1){
			if (argc < 3){
				usage() ;
			}
			for (int i = 0 ; i < 1 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 't') {
						/* read pphrase from *argv */
						tablefile = strtok(*argv, "=") ;
						tablefile = strtok(NULL, "=") ;
						if (tablefile == NULL)
							usage() ;
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
		}
		else if(choice == 2){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 2 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'k') {
						/* read pphrase from *argv */
						key = strtok(*argv, "=") ;
						key = strtok(NULL, "=") ;
						if (key == NULL)
							usage() ;
					} 
					else if (argv[0][1] == 't') {
						/* read pphrase from *argv */
						tablefile = strtok(*argv, "=") ;
						tablefile = strtok(NULL, "=") ;
						if (tablefile == NULL)
							usage() ;
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
			if (argc == 5){
				inpF = fopen(*argv, "r") ;
			       if (inpF == NULL){
				       fprintf(stderr, "File could not be opened\n") ;
				       exit(0) ;
			       }	       
			}
			else{
				inpF = stdin ;
			}
		}
		// Invkey command line option
		else if(choice == 3){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 2 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'k') {
						/* read pphrase from *argv */
						key = strtok(*argv, "=") ;
						key = strtok(NULL, "=") ;
						if (key == NULL)
							usage() ;
					} 
					else if (argv[0][1] == 't') {
						/* read pphrase from *argv */
						tablefile = strtok(*argv, "=") ;
						tablefile = strtok(NULL, "=") ;
						if (tablefile == NULL)
							usage() ;
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
			if (argc == 5){
				inpF = fopen(*argv, "r") ;
			       if (inpF == NULL){
				       fprintf(stderr, "File could not be opened\n") ;
				       exit(0) ;
			       }	       
			}
			else{
				inpF = stdin ;
			}
		}
		// Histo command line option
		else if(choice == 4){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 2 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'k') {
						/* read pphrase from *argv */
						key = strtok(*argv, "=") ;
						key = strtok(NULL, "=") ;
						if (key == NULL)
							usage() ;
					} 
					else if (argv[0][1] == 't') {
						/* read pphrase from *argv */
						tablefile = strtok(*argv, "=") ;
						tablefile = strtok(NULL, "=") ;
						if (tablefile == NULL)
							usage() ;
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
			if (argc == 5){
				inpF = fopen(*argv, "r") ;
			       if (inpF == NULL){
				       fprintf(stderr, "File could not be opened\n") ;
				       exit(0) ;
			       }	       
			}
			else{
				inpF = stdin ;
			}
		}
		else if(choice == 5){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 2 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'k') {
						/* read pphrase from *argv */
						key = strtok(*argv, "=") ;
						key = strtok(NULL, "=") ;
						if (key == NULL)
							usage() ;
					} 
					else if (argv[0][1] == 't') {
						/* read pphrase from *argv */
						tablefile = strtok(*argv, "=") ;
						tablefile = strtok(NULL, "=") ;
						if (tablefile == NULL)
							usage() ;
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
			if (argc == 5){
				inpF = fopen(*argv, "r") ;
			       if (inpF == NULL){
				       fprintf(stderr, "File could not be opened\n") ;
				       exit(0) ;
			       }	       
			}
			else{
				inpF = stdin ;
			}
		}

	}

	// Command line parsing done, now call respective methods
	
	if (choice == 1){
		tablecheck(tablefile) ;
	}
	else if (choice == 2){
		tablecheck(tablefile) ;
		encrypt(key, tablefile, inpF) ;
	}
	else if (choice == 3){
		tablecheck(tablefile) ;
		decrypt(key, tablefile, inpF) ;
	}
	else if (choice == 4){
		tablecheck(tablefile) ;
		encrypt3(key, tablefile, inpF) ;
	}
	else if (choice == 5){
		tablecheck(tablefile) ;
		decrypt3(key, tablefile, inpF) ;
	}


} // end of main function




#include <stdio.h>
#include <iostream>
#include "main.h"

int IP[64] ;
int E[48] ;
int P[32] ;
int V[16] ;
int PC1[56] ;
int PC2[48] ;
int S[9][64] ;

using namespace std ;

void validateString(char *str){
	int i = 0 ; 
	while (str[i] != '\0'){
		if(str[i] < 48 || str[i] > 57){
			if (str[i] != '\n'){
				printf ("%d\n", str[i]) ;
				fprintf(stderr, "Not a valid number\n") ;
				exit(0) ;
			}
		}
		++i ;
	}

}


void populateIP(char *value){

	int checkIP[65] ;

	for (int i = 0 ; i < 65 ; ++i)
		checkIP[i] = 0 ;


	char *c = strtok(value, ",") ;
	validateString(c) ;
	if (c == NULL) {
		fprintf(stderr, "IP: No value beween ,\n") ;
		exit(0) ;
	}
	IP[0] = atoi(c) ;
	if (IP[0] <= 0 || IP[0] > 64){
		fprintf(stderr, "IP: Value out of range\n") ;
		exit(0) ;
	}
	++checkIP[IP[0]] ;


	for(int i = 1 ; i < 64 ; ++i){
		c = strtok(NULL, ",") ;
		if (c == NULL) {
			fprintf(stderr, "IP: no value between , or few values than expected\n") ;
			exit(0) ;
		}
		validateString(c) ;
		IP[i] = atoi(c) ;

		if (IP[i] <= 0 || IP[i] > 64){
			fprintf(stderr, "IP: Value out of range\n") ;
			exit(0) ;
		}

		checkIP[IP[i]] = checkIP[IP[i]] + 1 ;
	}

	c = strtok(NULL, ",") ;
	if (c != NULL){
		fprintf(stderr, "IP: more value than expected\n") ;
		exit(0) ;
	}

	for (int i = 1 ; i < 65; ++i){ 
		if (checkIP[i] > 1 || checkIP[i] == 0){
			fprintf(stderr, "IP: %d missing or present more than once\n", i) ;
			exit(0) ;
		}
	}


}

void populateE(char *value){
	int checkIP[33] ;
	for (int i = 0 ; i < 33 ; ++i)
		checkIP[i] = 0 ;

	char *c = strtok(value, ",") ;
	if (c == NULL) {
		fprintf(stderr, "E: few arguments or no value between ,\n") ;
		exit(0) ;
	}
	validateString(c) ;
	E[0] = atoi(c) ;

	if (E[0] <= 0 || E[0] > 32){
		fprintf(stderr, "E: value out of range\n") ;
		exit(0) ;
	}
	++checkIP[E[0]] ;


	for(int i = 1 ; i < 48 ; ++i){
		char *c = strtok(NULL, ",") ;
		if (c == NULL) {
			fprintf(stderr, "E: few arguments or no value between ,\n") ;
			exit(0) ;
		}
		validateString(c) ;
		E[i] = atoi(c) ;
		if (E[i] <= 0 || E[i] > 32){
			fprintf(stderr, "E: value of out of range\n") ;
			exit(0) ;
		}
		checkIP[E[i]] = checkIP[E[i]] + 1 ;
	}
	c = strtok(NULL, ",") ;
	if (c != NULL){
		fprintf(stderr, "E: values more than expected\n") ;
		exit(0) ;
	}

	for (int i = 1 ; i < 32; ++i){ 
		if (checkIP[i] > 2 || checkIP[i] == 0){
			fprintf(stderr, "E: %d value missing or appearing more than once\n", i) ;
			exit(0) ;
		}
	}


}


void populateP(char *value){
	int checkIP[33] ;
	for (int i = 0 ; i < 33 ; ++i)
		checkIP[i] = 0 ;

	char *c = strtok(value, ",") ;
	if (c == NULL) {
		fprintf(stderr, "P: few values than expected\n") ;
		exit(0) ;
	}
	validateString(c) ;
	P[0] = atoi(c) ;

	if (P[0] <= 0 || P[0] > 32){
		fprintf(stderr, "P: value out of range\n") ;
		exit(0) ;
	}
	++checkIP[P[0]] ;


	for(int i = 1 ; i < 32 ; ++i){
		char *c = strtok(NULL, ",") ;
		if (c == NULL) {
			fprintf(stderr, "P: few values than expected or no value between comma\n") ;
			exit(0) ;
		}
		validateString(c) ;
		P[i] = atoi(c) ;
		if (P[i] <= 0 || P[i] > 32){
			fprintf(stderr, "P: value out of range\n") ;
			exit(0) ;
		}
		checkIP[P[i]] = checkIP[P[i]] + 1 ;
	}
	c = strtok(NULL, ",") ;
	if (c != NULL){
		fprintf(stderr, "P: few values than expected\n") ;
		exit(0) ;
	}

	for (int i = 1 ; i < 32; ++i){ 
		if (checkIP[i] > 1 || checkIP[i] == 0){
			fprintf(stderr, "P: %d missing or appearing more than once\n", i) ;
			exit(0) ;
		}
	}


}



void populateV(char *value){
	int checkIP[3] ;
	for (int i = 0 ; i < 3 ; ++i)
		checkIP[i] = 0 ;

	char *c = strtok(value, ",") ;
	if (c == NULL) {
		fprintf(stderr, "V: few values than expected\n") ;
		exit(0) ;
	}
	validateString(c) ;
	V[0] = atoi(c) ;

	if (V[0] <= 0 || V[0] > 2){
		fprintf(stderr, "V: value out range\n") ;
		exit(0) ;
	}

	++checkIP[V[0]] ;


	for(int i = 1 ; i < 16 ; ++i){
		char *c = strtok(NULL, ",") ;
		if (c == NULL) {
			fprintf(stderr, "V: few values than expected\n") ;
			exit(0) ;
		}
		validateString(c) ;
		V[i] = atoi(c) ;
		if (V[i] <= 0 || V[i] > 2){
			fprintf(stderr, "V: values out of range\n") ;
			exit(0) ;
		}
		checkIP[V[i]] = checkIP[V[i]] + 1 ;
	}
	c = strtok(NULL, ",") ;
	if (c != NULL){
		fprintf(stderr, "V table incorrect\n") ;
		exit(0) ;
	}

	if (checkIP[1] != 4 || checkIP[2] != 12){
		fprintf(stderr, "V: invalid number of 1 or number of 2\n") ;
		exit(0) ;
	}

}



void populatePC1(char *value){
	int checkIP[65] ;
	for (int i = 0 ; i < 65 ; ++i)
		checkIP[i] = 0 ;

	char *c = strtok(value, ",") ;
	if (c == NULL) {
		fprintf(stderr, "PC1: few arguments than expected\n") ;
		exit(0) ;
	}
	validateString(c) ;
	PC1[0] = atoi(c) ;

	if (PC1[0] <= 0 || PC1[0] > 64){
		fprintf(stderr, "PC1: values out of range\n") ;
		exit(0) ;
	}

	++checkIP[PC1[0]] ;


	for(int i = 1 ; i < 56 ; ++i){
		char *c = strtok(NULL, ",") ;
		if (c == NULL) {
			fprintf(stderr, "PC1: few values than expected\n") ;
			exit(0) ;
		}
		PC1[i] = atoi(c) ;
		if (PC1[i] <= 0 || PC1[i] > 64){
			fprintf(stderr, "PC1: values out of range\n") ;
			exit(0) ;
		}
		validateString(c) ;
		checkIP[PC1[i]] = checkIP[PC1[i]] + 1 ;
	}
	c = strtok(NULL, ",") ;
	if (c != NULL){
		fprintf(stderr, "PC1 table incorrect\n") ;
		exit(0) ;
	}

	for (int i = 1 ; i < 65; ++i){ 
		if ((checkIP[i] > 1 )  ){

			fprintf(stderr, "PC1: %d appearing more than once\n", i) ;
			exit(0) ;
		}
		if (i % 8 == 0 && checkIP[i] != 0){
			fprintf(stderr, "PC1: %d should be missing\n", i) ;
			exit(0) ;
		}
	}


}

void populatePC2(char *value){
	int checkIP[57] ;
	for (int i = 0 ; i < 57 ; ++i)
		checkIP[i] = 0 ;

	char *c = strtok(value, ",") ;
	if (c == NULL) {
		fprintf(stderr, "PC2: few values than expected\n") ;
		exit(0) ;
	}
	validateString(c) ;
	PC2[0] = atoi(c) ;

	if (PC2[0] <= 0 || PC2[0] > 57){
		fprintf(stderr, "PC2: Values out of range\n") ;
		exit(0) ;
	}

	++checkIP[PC2[0]] ;


	for(int i = 1 ; i < 48 ; ++i){
		char *c = strtok(NULL, ",") ;
		if (c == NULL) {
			fprintf(stderr, "PC2: fw values than expected\n") ;
			exit(0) ;
		}
		validateString(c) ;
		PC2[i] = atoi(c) ;
		if (PC2[i] <= 0 || PC2[i] > 57){
			fprintf(stderr, "PC2: values out of range\n") ;
			exit(0) ;
		}
		checkIP[PC2[i]] = checkIP[PC2[i]] + 1 ;
	}
	c = strtok(NULL, ",") ;
	if (c != NULL){
		fprintf(stderr, "PC2: more values than expected\n") ;
		exit(0) ;
	}
	int count = 0 ;
	for (int i = 1 ; i < 57; ++i){ 
		if ((checkIP[i] > 1 )  ){

			fprintf(stderr, "PC2: %d appearing more than once\n", i) ;
			exit(0) ;
		}
		if ( checkIP[i] == 0){
			++count ;
		}
	}
	if (count != 8){
		fprintf(stderr, "PC2: 8 values are not missing out\n") ;
		exit(0) ;
	}


}

void populateS(char *value, char id){

	if ( id < 49 || id > 56){
		fprintf(stderr, "S%c: invlaid id for S table\n", id) ;
		exit(0) ;
	}
	int checkIP[16] ;
	for (int i = 0 ; i < 16 ; ++i)
		checkIP[i] = 0 ;

	char *c = strtok(value, ",") ;
	if (c == NULL) {
		fprintf(stderr, "S%c: few values than expected\n", id) ;
		exit(0) ;
	}
	validateString(c) ;
	S[id-48][0] = atoi(c) ;

	if (S[id-48][0] < 0 || S[id-48][0] > 15){
		fprintf(stderr, "S%c: values out of range\n", id) ;
		exit(0) ;
	}

	++checkIP[S[id-48][0]] ;


	for(int i = 1 ; i < 64 ; ++i){
		char *c = strtok(NULL, ",") ;
		if (c == NULL) {
			fprintf(stderr, "S%c: few values than expected\n", id) ;
			exit(0) ;
		}
		validateString(c) ;
		S[id-48][i] = atoi(c) ;
		if (S[id-48][i] < 0 || S[id-48][i] > 15){
			fprintf(stderr, "S%c: values out of range\n", id) ;
			exit(0) ;
		}
		checkIP[S[id-48][i]] = checkIP[S[id-48][i]] + 1 ;
		if ((i+1)%16 == 0){
			for (int j = 0 ; j < 16; ++j){ 
				if (checkIP[j] > 1 || checkIP[j] == 0){
					fprintf(stderr, "S%c: %d missing or present more than once\n", id, i) ;
					exit(0) ;
				}
			}
			for (int k = 0 ; k < 16 ; ++k)
				checkIP[k] = 0 ;

		}

	}
	c = strtok(NULL, ",") ;
	if (c != NULL){
		fprintf(stderr, "S%c: more values than expected\n", id) ;
		exit(0) ;
	}

}


void tablecheck( char *tablefile){

	FILE *fp ;

	fp = fopen(tablefile, "r") ;
	if (fp == NULL){
		fprintf(stderr, "File could not be opened\n") ;
		exit(0) ;
	}

	char linebuf[256] ;
	char *value ;
	while(!feof(fp)){
		memset(linebuf, '\0', 256) ;
		fgets(linebuf, 256, fp) ;
		if(linebuf[0] == '\0')
			break ;
		value = strchr(linebuf, '=') ;
		if (value == NULL){
			fprintf(stderr, "Invalid tablecheck file\n") ;
			exit(0) ;
		}

		*value = '\0' ;
		++value ;

		if (!strcmp(linebuf, "IP")){
			populateIP(value) ;
		}
		else if(!strcmp(linebuf, "E")){
			populateE(value) ;
		}
		else if(!strcmp(linebuf, "P")){
			populateP(value) ;
		}
		else if(!strcmp(linebuf, "V")){
			populateV(value) ;
		}
		else if(!strcmp(linebuf, "PC1")){
			populatePC1(value) ;
		}
		else if(!strcmp(linebuf, "PC2")){
			populatePC2(value) ;
		}
		else if(linebuf[0] == 'S'){
			populateS(value, linebuf[1]) ;
		}
		else{
			fprintf(stderr, "Invalid tables file\n") ;
			//	exit(0) ;
		}



	}

}

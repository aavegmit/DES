#include "main.h"

using namespace std ;

unsigned char SKey[16][6] ;

void validateIfHex(char c){
	if ((c < 48 || c > 57) && ( c < 97 || c > 102)    )     {
		fprintf(stderr, "Invalid key, not a hex\n") ;
		exit(0) ;
	}


}

// Indexed from 0
unsigned char readBit(unsigned char *str, int location){
	int byte_loc = location / 8  ;
	int bit_loc = location % 8 ;

	unsigned char temp = str[byte_loc] ;

	temp = temp << bit_loc ;
	temp = temp >> 7 ;
	return temp ;

}

void writeBit(unsigned char *str, int location, unsigned char value){
	int byte_loc = location / 8  ;
	int bit_loc = location % 8 ;

	unsigned char temp = 0x80 ;
	temp = temp >> bit_loc ;

	if(value == 0x01){
		str[byte_loc] = str[byte_loc] | temp ;
	}
	else{
		str[byte_loc] = str[byte_loc] & ~temp ;
	}


}

void leftCircShift(unsigned char *str){
	unsigned char c ;
	c = str[0] >> 7 ;
	writeBit(str, 28, c) ;
	for (int l = 0 ; l < 3 ; ++l){
		str[l] = str[l] << 1 ;
		writeBit( str, l*8 + 7,  readBit(str, 8*(l+1)) ) ;
	}
	str[3] = str[3] << 1 ;
}

void concatCD(unsigned char *CD, unsigned char *C, unsigned char *D){
	for (int i = 0 ; i < 28 ; ++i){
		writeBit(CD, i, readBit(C, i)) ;
		writeBit(CD, i + 28, readBit(D, i)) ;
	}
}


void keySchedule(char *key){

	unsigned char key_64[8] ;
	int j = 0 ;
	unsigned char temp[3] ;
	unsigned int tempHex = 0x00 ;

	// Convert string represention of hex into HEX 
	for (int i = 0 ; i < 8 ; ++i){
		key_64[i] = 0x00 ;
		j = i*2 ;
		validateIfHex(key[j]) ;
		temp[0] = key[j] ;
		++j ;
		validateIfHex(key[j]) ;
		temp[1] = key[j] ;
		temp[2] = '\0' ;
		sscanf((const char *)temp, "%02x", &tempHex) ;
		key_64[i] = tempHex ;

	}


	unsigned char key_56[7] ;

	unsigned char C[4], D[4] ;


	for(int i = 0 ; i < 56 ; ++i){
		writeBit(key_56, i, readBit(key_64, PC1[i]-1)) ;
	}
	for (int j = 0 ; j < 28 ; ++j){
		writeBit(C, j,  readBit(key_56, j)) ;
		writeBit(D, j,  readBit(key_56, j+28 )) ;
	}

	printf("(C0,D0)=") ;
	for (int k1 = 0 ; k1 < 7 ; ++k1)
		printf("%02x", key_56[k1]) ;
	printf("\n") ;

	for (int l = 1 ; l <= 16 ; ++l){
		leftCircShift(C) ;
		leftCircShift(D) ;
		if (V[l-1] == 2){
			leftCircShift(C) ;
			leftCircShift(D) ;
		}
		printf("(C%d,D%d)=", l, l);
		unsigned char CD[7] ;
		concatCD(CD, C, D) ;
		for(int k1 = 0 ; k1 < 7 ; ++k1)
			printf("%02x", CD[k1]) ;
		printf("\n") ;

		for(int i = 0 ; i < 48 ; ++i){
			writeBit(SKey[l-1], i, readBit(CD, PC2[i]-1)) ;
		}
		
		printf("k%d=", l) ;
		for(int k1 = 0 ; k1 < 6 ; ++k1)
			printf("%02x", SKey[l-1][k1]) ;
		printf("\n") ;


	}




	return;



}


void encrypt(char *key, char *tablefile){
	printf("Key: %s, tablefile:%s\n", key, tablefile) ;

	if(strlen(key) != 16){
		fprintf(stderr, "Invalid length of key\n") ;
		exit(0) ;
	}

	keySchedule(key) ;
}

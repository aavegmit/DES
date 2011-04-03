#include "main.h"

using namespace std ;

unsigned char SKey[16][6] ;
unsigned char SKeyDec[16][6] ;

unsigned char SKey1[16][6] ;
unsigned char SKeyDec1[16][6] ;

unsigned char SKey2[16][6] ;
unsigned char SKeyDec2[16][6] ;

void validateIfHex(char c){
	if ((c < 48 || c > 57) && ( c < 97 || c > 102)    )     {
		fprintf(stderr, "Invalid key, not a hex\n") ;
		exit(0) ;
	}

}

// Indexed from 0
// Read a bit from the array
unsigned char readBit(unsigned char *str, int location){
	int byte_loc = location / 8  ;
	int bit_loc = location % 8 ;

	unsigned char temp = str[byte_loc] ;

	temp = temp << bit_loc ;
	temp = temp >> 7 ;
	return temp ;

}

// Write a bit at the specified memory location
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

// Method to Circular left shift the string
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

// MEthod to concatinate C and D, into one array
void concatCD(unsigned char *CD, unsigned char *C, unsigned char *D){
	for (int i = 0 ; i < 28 ; ++i){
		writeBit(CD, i, readBit(C, i)) ;
		writeBit(CD, i + 28, readBit(D, i)) ;
	}
}


// The key scheduler
// Takes in input a hex phrase, and returns
//set of 16 sub keys
void keySchedule(char *key, unsigned char SKey[][6], unsigned char SKeyDec[][6]){

	unsigned char key_64[8] ;
	int j = 0 ;
	unsigned char temp[3] ;
	unsigned int tempHex = 0x00 ;

	// Convert string represention of hex into HEX 
	for (int i = 0 ; i < 8 ; ++i){
		key_64[i] = 0x00 ;
		j = i*2 ;
		// Validate if the char is a string or not
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


	// Apply PC1 
	for(int i = 0 ; i < 56 ; ++i){
		writeBit(key_56, i, readBit(key_64, PC1[i]-1)) ;
	}

	// find the initial value of C and D
	for (int j = 0 ; j < 28 ; ++j){
		writeBit(C, j,  readBit(key_56, j)) ;
		writeBit(D, j,  readBit(key_56, j+28 )) ;
	}

	// Output on stderr
	fprintf(stderr, "(C0,D0)=") ;
	for (int k1 = 0 ; k1 < 7 ; ++k1)
		fprintf(stderr, "%02x", key_56[k1]) ;
	fprintf(stderr, "\n") ;

	// Iterate 16 times to find different sub keys
	for (int l = 1 ; l <= 16 ; ++l){
		leftCircShift(C) ;
		leftCircShift(D) ;
		if (V[l-1] == 2){
			leftCircShift(C) ;
			leftCircShift(D) ;
		}
		fprintf(stderr,"(C%d,D%d)=", l, l);
		unsigned char CD[7] ;
		concatCD(CD, C, D) ;
		for(int k1 = 0 ; k1 < 7 ; ++k1)
			fprintf(stderr,"%02x", CD[k1]) ;
		fprintf(stderr,"\n") ;

		for(int i = 0 ; i < 48 ; ++i){
			writeBit(SKey[l-1], i, readBit(CD, PC2[i]-1)) ;
		}

		fprintf(stderr,"k%d=", l) ;
		for(int k1 = 0 ; k1 < 6 ; ++k1)
			fprintf(stderr,"%02x", SKey[l-1][k1]) ;
		fprintf(stderr, "\n") ;

	}

	// Populating the inverse of the subkeys
	for (int i = 0 ; i < 16 ; ++i){
		for (int j = 0 ; j < 6 ; ++j)
			SKeyDec[15-i][j] = SKey[i][j] ;
	}


	return;
}


// This method is called for both encryption and decryption
void crypt(unsigned char *input_8, unsigned char subK[][6], unsigned char *out_8, int printF){

	unsigned char left[4], right[4], temprl[4] ;

	for(int i = 0; i < 32 ; i++){
		writeBit(left, i,   readBit(input_8, IP[i]-1 ) ) ;
		writeBit(right, i ,   readBit(input_8, IP[i+32] - 1 )) ;
	}

	if (printF){
		fprintf(stderr,"(L0,R0)=") ;
		for(int i = 0; i < 4; ++i)
			fprintf(stderr,"%02x", left[i]) ;
		for(int i = 0; i < 4; ++i)
			fprintf(stderr,"%02x", right[i]) ;
		fprintf(stderr,"\n") ;
	}

	unsigned char re[6] ;
	unsigned char sub[8] ;
	for(int it = 0 ; it < 16; ++it){

		// Expansion
		for (int ex = 0 ; ex < 48 ; ++ex){
			writeBit(re, ex, readBit(right, E[ex]-1  )) ;
		}

		// XOR with the key
		for (int ex = 0 ; ex < 6 ; ++ex)
			re[ex] = re[ex] ^ subK[it][ex] ;

		// Substitution
		memset(sub, 0, 8) ;
		int j = 0 ;
		for (int i = 0 ; i < 64; ++i){
			if (i % 8 == 0 || (i-1)%8 == 0){
			}
			else{
				writeBit(sub, i ,readBit(re, j)) ;
				++j ;
			}
		}

		unsigned char sub1[4] ;
		for (int i = 0 ; i < 8 ; ++i){
			unsigned int r = 2 * (int)readBit(sub, i*8 + 2) + (int)readBit(sub, i*8 + 7) ;
			unsigned char c = sub[i]  ;
			c = c << 3 ;
			c = c >> 4 ;
			unsigned char c1 = S[i+1][r*16+ (int)c] ;
			if (i%2 == 0)
				sub1[i/2] = c1 << 4 ;
			else
				sub1[i/2] = sub1[i/2] | c1 ;
		}

		// Permutation
		unsigned char sub1_p[4] ;
		for (int i = 0 ; i < 32 ; ++i){
			writeBit(sub1_p, i, readBit(sub1, P[i]-1)) ;
		}

		for (int i = 0 ; i < 4 ; ++i)
			temprl[i] = left[i] ;

		for (int i = 0; i < 4 ; ++i)
			left[i] = right[i] ;

		for (int i = 0; i < 4 ; ++i){
			right[i] = temprl[i] ^ sub1_p[i] ;
		}


		// Output on stderr
		if(printF){
			fprintf(stderr, "(L%d,R%d)=",it+1, it+1 ) ;
			for(int i = 0; i < 4; ++i)
				fprintf(stderr, "%02x", left[i]) ;
			for(int i = 0; i < 4; ++i)
				fprintf(stderr, "%02x", right[i]) ;
			fprintf(stderr,"\n") ;
		}


	}

	// Exchange left and right
	for (int i = 0 ; i < 4 ; ++i)
		temprl[i] = left[i] ;

	for (int i = 0; i < 4 ; ++i)
		left[i] = right[i] ;

	for (int i = 0; i < 4 ; ++i){
		right[i] = temprl[i]  ;
	}

	unsigned char tempOut[8] ;
	for (int i = 0; i < 4 ; ++i){
		tempOut[i] = left[i]  ;
		tempOut[i+4] = right[i] ;
	}
	for (int i = 0; i < 64 ; ++i){
		writeBit( out_8, i, readBit(tempOut, InvIP[i]-1 )   ) ;
	}


}

// call different functions based on encryption technique
void encrypt(char *key, char *tablefile, FILE *fp){

	if(strlen(key) != 16){
		fprintf(stderr, "Invalid length of key\n") ;
		exit(0) ;
	}

	keySchedule(key, SKey, SKeyDec) ;
	unsigned char byte_8[8] ;
	unsigned char out_8[8] ;


	int printF = 1 ;
	while(!feof(fp)){
		memset(byte_8, 0, 8) ;
		int bytes_read = fread(byte_8,1, 8, fp) ;
		if (bytes_read > 0){
			crypt(byte_8, SKey, out_8, printF) ;
			if(printF)
				printF = 0 ;
			for (int i=0 ; i < 8 ; ++i)
				printf("%c", out_8[i]) ;
		}
	}
}



void decrypt(char *key, char *tablefile, FILE *fp){

	if(strlen(key) != 16){
		fprintf(stderr, "Invalid length of key\n") ;
		exit(0) ;
	}

	keySchedule(key, SKey, SKeyDec) ;
	unsigned char byte_8[8] ;
	unsigned char out_8[8] ;


	int printF = 1 ;
	while(!feof(fp)){
		memset(byte_8, 0, 8) ;
		int bytes_read = fread(byte_8,1, 8, fp) ;
		if(feof(fp))
			break ;
		if (bytes_read > 0){
			crypt(byte_8, SKeyDec, out_8, printF) ;
			if(printF)
				printF = 0 ;
			for (int i=0 ; i < 8 ; ++i)
				printf("%c", out_8[i]) ;
		}
	}
}

void encrypt3(char *key, char *tablefile, FILE *fp){

	if(strlen(key) != 48){
		fprintf(stderr, "Invalid length of key\n") ;
		exit(0) ;
	}
	char key1[16], key2[16], key3[16] ;
	for(int i = 0 ; i < 16 ; ++i){
		key1[i] = key[i] ;
		key2[i] = key[i+16] ;
		key3[i] = key[i+32] ;
	}

	keySchedule(key1, SKey, SKeyDec) ;
	keySchedule(key2, SKey1, SKeyDec1) ;
	keySchedule(key3, SKey2, SKeyDec2) ;
	unsigned char byte_8[8] ;
	unsigned char out_8[8], out1_8[8], out2_8[8] ;


	int printF = 1 ;
	while(!feof(fp)){
		memset(byte_8, 0, 8) ;
		int bytes_read = fread(byte_8,1, 8, fp) ;
		if (bytes_read > 0){
			crypt(byte_8, SKey, out_8, printF) ;
			crypt(out_8, SKeyDec1, out1_8, printF) ;
			crypt(out1_8, SKey2, out2_8, printF) ;
			if(printF)
				printF = 0 ;
			for (int i=0 ; i < 8 ; ++i)
				printf("%c", out2_8[i]) ;
		}
	}
}

void decrypt3(char *key, char *tablefile, FILE *fp){

	if(strlen(key) != 48){
		fprintf(stderr, "Invalid length of key\n") ;
		exit(0) ;
	}
	char key1[16], key2[16], key3[16] ;
	for(int i = 0 ; i < 16 ; ++i){
		key1[i] = key[i] ;
		key2[i] = key[i+16] ;
		key3[i] = key[i+32] ;
	}

	keySchedule(key1, SKey, SKeyDec) ;
	keySchedule(key2, SKey1, SKeyDec1) ;
	keySchedule(key3, SKey2, SKeyDec2) ;
	unsigned char byte_8[8] ;
	unsigned char out_8[8], out1_8[8], out2_8[8] ;


	int printF = 1 ;
	while(!feof(fp)){
		memset(byte_8, 0, 8) ;
		int bytes_read = fread(byte_8,1, 8, fp) ;
		if (bytes_read > 0){
			crypt(byte_8, SKeyDec2, out_8, printF) ;
			crypt(out_8, SKey1, out1_8, printF) ;
			crypt(out1_8, SKeyDec, out2_8, printF) ;
			if(printF)
				printF = 0 ;
			for (int i=0 ; i < 8 ; ++i)
				printf("%c", out2_8[i]) ;
		}
	}
}


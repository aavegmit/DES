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

	fprintf(stderr, "(C0,D0)=") ;
	for (int k1 = 0 ; k1 < 7 ; ++k1)
		fprintf(stderr, "%02x", key_56[k1]) ;
	fprintf(stderr, "\n") ;

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


	return;
}

void crypt(unsigned char *input_8, unsigned char subK[][6], unsigned char *out_8){

	unsigned char left[4], right[4], temprl[4] ;

	for(int i = 0; i < 32 ; i++){
		writeBit(left, i,   readBit(input_8, IP[i]-1 ) ) ;
		writeBit(right, i ,   readBit(input_8, IP[i+32] - 1 )) ;
	}

	printf("(L0,R0)=") ;
	for(int i = 0; i < 4; ++i)
		printf("%02x", left[i]) ;
	for(int i = 0; i < 4; ++i)
		printf("%02x", right[i]) ;
	printf("\n") ;

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
//			printf("r=%d, c=%d\n", r, c) ;
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


		printf("(L%d,R%d)=",it+1, it+1 ) ;
		for(int i = 0; i < 4; ++i)
			printf("%02x", left[i]) ;
		for(int i = 0; i < 4; ++i)
			printf("%02x", right[i]) ;
		printf("\n") ;


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


void encrypt(char *key, char *tablefile, FILE *fp){

	if(strlen(key) != 16){
		fprintf(stderr, "Invalid length of key\n") ;
		exit(0) ;
	}

	keySchedule(key) ;
	unsigned char byte_8[8] ;
	unsigned char out_8[8] ;


	while(!feof(fp)){
		memset(byte_8, 0, 8) ;
		int bytes_read = fread(byte_8,1, 8, fp) ;
		if (bytes_read > 0){
			crypt(byte_8, SKey, out_8) ;
			for (int i=0 ; i < 8 ; ++i)
				printf("%02x", out_8[i]) ;
		}
	}
	printf("\n") ;
}

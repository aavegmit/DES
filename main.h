#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tablecheck( char *tablefile ) ;
void encrypt(char *key, char *tablefile, FILE *) ;
void encrypt3(char *key, char *tablefile, FILE *) ;
void decrypt(char *key, char *tablefile, FILE *) ;
void decrypt3(char *key, char *tablefile, FILE *) ;
extern unsigned char readBit(unsigned char *, int ) ;
extern void writeBit(unsigned char *, int , unsigned char) ;


extern int IP[64] ;
extern int InvIP[64] ;
extern int E[48] ;
extern int P[32] ;

extern int V[16] ;
extern int PC1[56] ;
extern int PC2[48] ;
extern int S[9][64] ;
extern unsigned char SKey[16][6] ;
extern unsigned char SKeyDec[16][6] ;

extern unsigned char SKey1[16][6] ;
extern unsigned char SKeyDec1[16][6] ;

extern unsigned char SKey2[16][6] ;
extern unsigned char SKeyDec2[16][6] ;

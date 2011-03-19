# Macros

CC = g++
CFLAGS = -g -Wall
OBJ = main.o tablecheck.o keyschedule.o
LIBS = -lcrypto
#LIBS = -L/home.scf-22/csci551b/openssl/lib -lcrypto -lnsl -lsocket -lresolv
#INC = -I/home/scf-22/csci551b/openssl/include
INC = 

# Explicit rule
all: hw5 

hw5: $(OBJ)
	$(CC) $(CFLAGS) -o hw5 $(OBJ) $(INC) $(LIBS) 

clean:
	rm -rf *.o hw5

main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc $(INC)
tablecheck.o: tablecheck.cc
	$(CC) $(CFLAGS) -c tablecheck.cc $(INC)

keyschedule.o: keyschedule.cc
	$(CC) $(CFLAGS) -c keyschedule.cc $(INC)

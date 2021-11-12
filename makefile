CC=gcc
CFLAG=-Wall
RM=rm


all:main

main: tools.o serveur.o gameTools.o client.o main.o
		$(CC) $(CFLAG) $^ -o main
main.o: main.c
		$(CC) $(CFLAG) -c $<
client.o: client.c client.h
		$(CC) $(CFLAG) -c $<
gameTools.o: gameTools.c gameTools.h
		$(CC) $(CFLAG) -c $<
serveur.o: serveur.c serveur.h
		$(CC) $(CFLAG) -c $<
tools.o: tools.c tools.h
		$(CC) $(CFLAG) -c $<
clean:
		$(RM) *.o
doc:
		doxygen
doc_clean:
		$(RM) -rf ./html

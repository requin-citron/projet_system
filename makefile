CC=gcc
CFLAG=-g3 -ggdb -Wall -Wextra
LIBS=-lpthread -lcurl
RM=rm
SERVER_DIRECTORY=./serveur

all:server_kleman

server_kleman: $(SERVER_DIRECTORY)/serveur_kleman



$(SERVER_DIRECTORY)/serveur_kleman: $(SERVER_DIRECTORY)/client.o $(SERVER_DIRECTORY)/main.o $(SERVER_DIRECTORY)/serveur.o
		$(CC) $(CFLAG) $(LIBS) $^ -o $@

$(SERVER_DIRECTORY)/main.o: $(SERVER_DIRECTORY)/main.c
		$(CC) $(CFLAG) $(LIBS) $< -c -o $@

$(SERVER_DIRECTORY)/serveur.o: $(SERVER_DIRECTORY)/serveur.c $(SERVER_DIRECTORY)/serveur.h
		$(CC) $(CFLAG) $(LIBS) $< -c -o $@

$(SERVER_DIRECTORY)/client.o: $(SERVER_DIRECTORY)/client.c $(SERVER_DIRECTORY)/client.h
		$(CC) $(CFLAG) $(LIBS) $< -c -o $@

clean:
		$(RM) ./serveur/*.o
doc:
		doxygen
doc_clean:
		$(RM) -rf ./html

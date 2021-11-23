CC=gcc
CFLAG=-g3 -ggdb -Wall -Wextra
LIBS=-lpthread
RM=rm
CLIENT_DIRECTORY=./client
SERVER_DIRECTORY=./serveur
SERVER_KLEMOU_DIRECTORY=./serveur_kleman

all:client server

client: $(CLIENT_DIRECTORY)/client

server: $(SERVER_DIRECTORY)/serveur

server_kleman: $(SERVER_KLEMOU_DIRECTORY)/serveur_kleman

$(CLIENT_DIRECTORY)/client:$(CLIENT_DIRECTORY)/client.o $(CLIENT_DIRECTORY)/main.o
		$(CC) $(CFLAG) $^ -o $@

$(CLIENT_DIRECTORY)/client.o: $(CLIENT_DIRECTORY)/client.c $(CLIENT_DIRECTORY)/client.h
		$(CC) $(CFLAG) $< -c -o $@

$(CLIENT_DIRECTORY)/main.o: $(CLIENT_DIRECTORY)/main.c
		$(CC) $(CFLAG) $< -c -o $@

$(SERVER_DIRECTORY)/serveur:$(SERVER_DIRECTORY)/serveur.o $(SERVER_DIRECTORY)/main.o
		$(CC) $(CFLAG) $^ -o $@

$(SERVER_DIRECTORY)/serveur.o: $(SERVER_DIRECTORY)/serveur.c $(SERVER_DIRECTORY)/serveur.h
		$(CC) $(CFLAG) $< -c -o $@

$(SERVER_DIRECTORY)/main.o: $(SERVER_DIRECTORY)/main.c
		$(CC) $(CFLAG) $< -c -o $@

$(SERVER_KLEMOU_DIRECTORY)/serveur_kleman: $(SERVER_KLEMOU_DIRECTORY)/main.o $(SERVER_KLEMOU_DIRECTORY)/serveur.o
		$(CC) $(CFLAG) $(LIBS) $^ -o $@

$(SERVER_KLEMOU_DIRECTORY)/main.o: $(SERVER_KLEMOU_DIRECTORY)/main.c
		$(CC) $(CFLAG) $(LIBS) $< -c -o $@

$(SERVER_KLEMOU_DIRECTORY)/serveur.o: $(SERVER_KLEMOU_DIRECTORY)/serveur.c $(SERVER_KLEMOU_DIRECTORY)/serveur.h
		$(CC) $(CFLAG) $(LIBS) $< -c -o $@


clean:
		$(RM) ./client/*.o
		$(RM) ./serveur/*.o
		$(RM) ./serveur_kleman/*.o
doc:
		doxygen
doc_clean:
		$(RM) -rf ./html

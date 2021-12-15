CC=gcc
CFLAG=-g3 -ggdb -Wall -Wextra
LIBS=-lpthread -lcurl
LIBS_CLIENT=-lpthread
RM=rm
SERVER_DIRECTORY=./serveur
CLIENT_DIRECTORY=./client
WRAPPER_DIRECTORY=./bot

all:server_kleman client wrapper

server_kleman: $(SERVER_DIRECTORY)/serveur_kleman

client: $(CLIENT_DIRECTORY)/client

wrapper: $(WRAPPER_DIRECTORY)/wrapper

$(WRAPPER_DIRECTORY)/wrapper:$(WRAPPER_DIRECTORY)/wrapper.c
		$(CC) $(CFLAG) $< -o $@

$(CLIENT_DIRECTORY)/client: $(CLIENT_DIRECTORY)/client.c
		$(CC) $(CFLAG) $(LIBS_CLIENT) $< -o $@

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
		$(RM) ./client/*.o
doc:
		doxygen
doc_clean:
		$(RM) -rf ./html

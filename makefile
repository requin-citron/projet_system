CC=gcc
CFLAG=-Wall
RM=rm
CLIENT_DIRECTORY=./client
SERVER_DIRECTORY=./serveur

all:client server

client: $(CLIENT_DIRECTORY)/client

server: $(SERVER_DIRECTORY)/serveur

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

clean:
		$(RM) ./client/*.o
		$(RM) ./serveur/*.o
doc:
		doxygen
doc_clean:
		$(RM) -rf ./html

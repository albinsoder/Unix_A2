# Compile server + client executables:
# $ make all
# Cleanup executables:
# $ make clean

DIR_SRC += ./src
DIR_OBJ += ./objects
DIR_INC += ./include

all: client server

server:
	gcc -Werror -pthread $(DIR_SRC)/TCPserver.c $(DIR_SRC)/ConnectServer.c $(DIR_SRC)/kmeans.c $(DIR_SRC)/matrix_inverse.c -o $(DIR_OBJ)/server

client:
	gcc -Werror $(DIR_SRC)/TCPclient.c $(DIR_SRC)/ConnectClient.c -o $(DIR_OBJ)/client

clean:
	rm $(DIR_OBJ)/server $(DIR_OBJ)/client

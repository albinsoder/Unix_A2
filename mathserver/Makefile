# all: server client

# server: TCPserver.c
#     gcc -o server TCPserver.c ConnectServer.C kmeans.c matrix_inverse.c 

# client: TCPclient.c
#     gcc -o client TCPclient.c ConnectClient.c

PROJECT := main.exe

DIR_SRC += .
DIR_SRC += ./src

DIR_INC += -lpthread 
DIR_INC += -I./inc 
DIR_INC += $(addprefix -I, $(DIR_SRC))

SRC_C += $(wildcard $(addsuffix /*.c, $(DIR_SRC)))
#OBJ := $(filter %.o, $(SRC_C:.c=.o))
OBJ := $(patsubst %.c, %.o, $(SRC_C))
EXE := $(PROJECT)

CC_PREFIX := 
CC := $(CC_PREFIX)gcc
CFLAG = 
CLIB = -L .

.PHONY:all

all:$(OBJ) $(EXE)

%.o: %.c
    $(CC) $(CFLAG) $(DIR_INC) -c $< -o $@ 

$(EXE): $(OBJ)
    $(CC) $(CFLAG) $(CLIB) $(OBJ) -o $@ 

clean:
    rm -r $(EXE) $(OBJ) 
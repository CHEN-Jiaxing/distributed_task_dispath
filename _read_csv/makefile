PATH_NOW = ${shell pwd}
PATH_TENDIS = ../_tendis_server
#PATH_INC = ./include
PATH_REDIS = ./hiredis
PATH_REDIS_OP = ${PATH_TENDIS}
PATH_REDIS_VIP = ${PATH_TENDIS}/hiredis-vip
#PATH_SRC = ./source/*
PATH_REDIS_OP_SOURCE = ${PATH_REDIS_OP}/redis_op.cpp
PATH_ZKEPER = ../_zk_server
#SO_REDIS = ./hiredis/libhiredis.so
SO_REDIS_VIP = ${PATH_REDIS_VIP}/libhiredis_vip.so
SO_ZKEPER = ${PATH_ZKEPER}/libclust_comm.so


CC      := g++
DEBUG   := -gdwarf-3 -gstrict-dwarf
#CC      += ${DEBUG}
#CFLAGS  += -I $(PATH_INC) -I $(PATH_REDIS) -I ${PATH_REDIS_VIP} -lpthread
#CFLAGS  += -I $(PATH_INC) -I $(PATH_REDIS) -I ${PATH_REDIS_OP} -lpthread
#CFLAGS  += -I $(PATH_INC) -I ${PATH_REDIS_VIP} -I ${PATH_REDIS_OP} -lpthread
CFLAGS  += -I ${PATH_REDIS_VIP} -I ${PATH_REDIS_OP} -I ${PATH_ZKEPER} -lpthread

SRC     := main.cpp read_csv.cpp
OBJ     := main.o read_csv.o
TARGET  := run.out

#$(TARGET): $(OBJ) $(SO_REDIS)\
	$(CC) -o $(TARGET) $(SO_REDIS) $(SRC) $(PATH_SRC) $(CFLAGS)

#$(TARGET): $(OBJ) $(SO_REDIS_VIP) ${PATH_REDIS_OP_SOURCE}\
	$(CC) -o $(TARGET) $(SO_REDIS_VIP) $(SRC) ${PATH_REDIS_OP_SOURCE} $(PATH_SRC) $(CFLAGS)

$(TARGET): $(OBJ) $(SO_REDIS_VIP) ${PATH_REDIS_OP_SOURCE}
	$(CC) -o $(TARGET) ${OBJ} -lzookeeper_mt $(SO_REDIS_VIP) ${PATH_REDIS_OP_SOURCE} $(CFLAGS)

#$(OBJ): $(SRC) $(SO_REDIS_VIP) ${PATH_SRC}\
	$(CC) -c $(SRC) $(CFLAGS)

$(OBJ): $(SRC) $(SO_REDIS_VIP)
	$(CC) -c ${SRC} $(CFLAGS)


#$(SO_REDIS): $(PATH_REDIS)/* \
	cd $(PATH_REDIS) && make \
	cd ..


$(SO_REDIS_VIP): ${PATH_REDIS_VIP}/*
	cd ${PATH_REDIS_VIP} && make
	cd ${PATH_NOW}

.PHONY: clean
clean:
	rm -f *.o *.out
	cd $(PATH_REDIS_VIP) && make clean
	cd ${PATH_NOW}
















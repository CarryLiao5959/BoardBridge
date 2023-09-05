# 编译器
CC = g++

# 源文件
SRCS := $(shell find ./* -type f|grep '\.cpp' | grep -v 'server\.cpp'| grep -v 'client\.cpp')
$(warning SRCS is ${SRCS})

# 目标文件
COMMON_OBJS := $(patsubst %.cpp, %.o, $(filter-out client.cpp server.cpp, $(SRCS)))
$(warning COMMON_OBJS is ${COMMON_OBJS})
OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is ${OBJS})
ALL_OBJS := $(shell find . -name "*.o")

# 头文件
INCLUDE_TEMP = $(shell find ./* -type d | grep -v '\./config')
INCLUDE = $(patsubst %,-I %, $(INCLUDE_TEMP))
$(warning INCLUDE is ${INCLUDE})

# 编译选项
CFLAGS = -g -O0 -Wall -Werror -Wno-unused -fPIC
$(warning CFLAGS is ${CFLAGS})

# 链接选项
LDFLAG = -lpthread -std=c++11 -ldl
$(warning LDFLAG is ${LDFLAG})

# 库
LIBS = -lpqxx -lpq

SRC_SERVER = server/server.cpp
OBJ_SERVER = ${SRC_SERVER:%.cpp=%.o}
EXE_SERVER = server/server

SRC_CLIENT = client/client.cpp
OBJ_CLIENT = ${SRC_CLIENT:%.cpp=%.o}
EXE_CLIENT = client/client

target: clean ${EXE_SERVER} ${EXE_CLIENT}

# server
$(EXE_SERVER): server/server.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(LDFLAG) $(CFLAGS) $(INCLUDE) $(LIBS)

# client
$(EXE_CLIENT): client/client.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(LDFLAG) $(CFLAGS) $(INCLUDE) $(LIBS)

%.o: %.cpp
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@ $(LIBS)

clean:
	rm -f ${ALL_OBJS} ${OBJ_SERVER} ${EXE_SERVER} ${OBJ_CLIENT} ${EXE_CLIENT}
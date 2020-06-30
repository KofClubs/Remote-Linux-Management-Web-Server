CC:=g++
CCFLAGS:=
LDFLAGS:=

TARGETS_DIR:=./bin
LIB_DIR:=./lib
OBJ_DIR:=./obj
SRC_DIR:=./src

LIB:=$(LIB_DIR)/libmemfree.so $(LIB_DIR)/libprocess.so $(LIB_DIR)/libtime.so $(LIB_DIR)/libversion.so # 服务端功能对应的动态链接库
OBJ:=$(OBJ_DIR)/thread_pool.o $(OBJ_DIR)/web_page.o $(OBJ_DIR)/server.o # 服务端对应的对象文件

.PHONY:all clean
all:lib server client

lib:$(LIB) # 动态链接库
	@echo "Libraries constructed!"
$(LIB_DIR)/libmemfree.so:$(SRC_DIR)/sys_memfree.cpp $(SRC_DIR)/sys_memfree.h $(SRC_DIR)/exit_code.h # 空闲内存
	$(CC) -shared -fPIC -o $(LIB_DIR)/libmemfree.so $(SRC_DIR)/sys_memfree.cpp
$(LIB_DIR)/libprocess.so:$(SRC_DIR)/sys_process.cpp $(SRC_DIR)/sys_process.h $(SRC_DIR)/exit_code.h # 进程
	$(CC) -shared -fPIC -o $(LIB_DIR)/libprocess.so $(SRC_DIR)/sys_process.cpp
$(LIB_DIR)/libtime.so:$(SRC_DIR)/sys_time.cpp $(SRC_DIR)/sys_time.h $(SRC_DIR)/exit_code.h # 时间
	$(CC) -shared -fPIC -o $(LIB_DIR)/libtime.so $(SRC_DIR)/sys_time.cpp
$(LIB_DIR)/libversion.so:$(SRC_DIR)/sys_version.cpp $(SRC_DIR)/sys_version.h $(SRC_DIR)/exit_code.h # 版本
	$(CC) -shared -fPIC -o $(LIB_DIR)/libversion.so $(SRC_DIR)/sys_version.cpp

server:$(TARGETS_DIR)/server # 服务端
$(TARGETS_DIR)/server:$(OBJ)
	$(CC) -o $(TARGETS_DIR)/server $(OBJ) -L. -lpthread -ldl
	@echo "Server constructed!"
$(OBJ_DIR)/thread_pool.o:$(SRC_DIR)/thread_pool.cpp $(SRC_DIR)/thread_pool.h
	$(CC) -c -o $(OBJ_DIR)/thread_pool.o $(SRC_DIR)/thread_pool.cpp
$(OBJ_DIR)/web_page.o:$(SRC_DIR)/web_page.cpp $(SRC_DIR)/web_page.h $(SRC_DIR)/exit_code.h
	$(CC) -c -o $(OBJ_DIR)/web_page.o $(SRC_DIR)/web_page.cpp
$(OBJ_DIR)/server.o:$(SRC_DIR)/server.cpp $(SRC_DIR)/thread_pool.h $(SRC_DIR)/web_page.h $(SRC_DIR)/exit_code.h
	$(CC) -c -o $(OBJ_DIR)/server.o $(SRC_DIR)/server.cpp

client:$(TARGETS_DIR)/client # 客户端
$(TARGETS_DIR)/client:
	$(CC) -o $(TARGETS_DIR)/client $(SRC_DIR)/client.cpp
	@echo "Client constructed!"

clean:
	rm -f $(TARGETS_DIR)/* $(LIB_DIR)/* $(OBJ_DIR)/*
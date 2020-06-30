/* 基于Linux的C++   课程作业15
 * 作者：张展鹏 2016011975
 * 名称：
 * 实现操作系统管理的Web服务器
 * 功能：
 * 1. 支持本地或远程用户通过HTTP协议获得操作系统部分信息；
 * 2. 使用模块化架构，根据Web请求动态生成HTML网页；
 *  注：使用共享目标库实现每个模块，支持模块的动态装载、增删查改。
 * 3. 在子进程或线程处理Web请求，设计进程池或线程池管理它们。
 * 说明：
 * 1. 服务器没有超级用户
 * 2. 面向对象架构
 */

#include <arpa/inet.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "exit_code.h"
#include "thread_pool.h"
#include "web_page.h"

// 动态链接库存储路径缺省值
#define TIME_PATH "../lib/libtime.so"
#define VERSION_PATH "../lib/libversion.so"
#define MEMFREE_PATH "../lib/libmemfree.so"
#define PROCESS_PATH "../lib/libprocess.so"

enum SERVER_TYPE { LOCAL, REMOTE }; /* 本地或远程 */
SERVER_TYPE type;           /* 服务器类型，在Welcome函数被初始化 */
sockaddr_un sun;            /* 本地套接字 */
sockaddr_in sin;            /* 远程套接字 */
int sockfd;                 /* 服务器端socket文件描述符 */
const int clientNumMax = 8; /* 允许的最大客户连接数 */
ThreadPool *tp;             /* 本例的线程池对象 */

// 函数：服务器功能
void Welcome();                           /* 欢迎界面 */
void Init();                              /* 服务器初始化 */
void MakeWebPage(void (*)(const char *)); /* 调用不同的库函数制作、打开网页 */
void Response(int); /* 对服务器的每一条指令映射的关键码应答 */
int Serve(int);     /* 服务某个服务器的消息 */

int main() {
  tp = new ThreadPool(clientNumMax << 1); /* 线程池装填因子不能太高 */
  tp->turnOn();                           /* 启动线程池 */
  printf("%s\n", "Thread pool opened!");
  Welcome();
  Init();
  listen(sockfd, clientNumMax); /* 侦听客户端连接 */
  int tmp;
  do {
    sockaddr_un client_s;
    socklen_t client_s_len;
    int client_sockfd = accept(sockfd, (sockaddr *)&client_s,
                               &client_s_len); /* 接受客户端连接请求 */
    tmp = Serve(client_sockfd);
    close(
        client_sockfd); /* 关闭客户端socket文件描述符，结束这次应答，这个操作在客户端完成
                         */
  } while (tmp == -1);  /* 结束条件是收到某个客户的quit指令 */
  close(sockfd);
  printf("%s\n", "Server OFF!");
  // 删除相关文件
  if (type == LOCAL)
    unlink("/tmp/socket");
  else
    unlink("127.0.0.1:1024");
  printf("%s\n", "Temporary files cleared!");
  delete tp;
  tp = NULL;
  return 0;
}

void Welcome() {
  // 欢迎
  printf("%s\n", "I shall help you initialize this server...");
  // 初始化
  printf("%s\n", "2 types are available:");
  printf("%s\t%s\n", "0. Local", "1. Remote"); /* 服务器在本地或者远程连接 */
  int tmp;
  do {
    printf("%s\n", "Please input 0 or 1!");
    scanf("%d", &tmp);
  } while (tmp != 0 && tmp != 1); /* 保证选择合法 */
  if (tmp == 0)
    type = LOCAL; /* 本地服务器 */
  else
    type = REMOTE; /* 远程服务器 */
}

void Init() {
  if (type == LOCAL) {
    sockfd = socket(PF_LOCAL, SOCK_STREAM, 0); /* 套接字 */
    // 初始化套接字
    sun.sun_family = AF_LOCAL;
    strcpy(sun.sun_path, "/tmp/socket"); /* 缺省地存储在本地tmp目录下 */
    bind(sockfd, (sockaddr *)&sun, sizeof(sun)); /* 绑定套接字 */
  } else {
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(1024); /* 使用1024~5000，即临时端口 */
    sin.sin_addr.s_addr =
        inet_addr("127.0.0.1"); /* IP地址，这个提供本地测试使用 */
    bind(sockfd, (sockaddr *)&sin, sizeof(sin));
  }
}

void MakeWebPage(void (*func)(const char *)) {
  OpenHTMLFile("/tmp/WebPage");
  WriteHTMLFile("/tmp/WebPage", func);
  CloseHTMLFile("/tmp/WebPage");
  printf(
      "%s\n",
      "If Firefox is installed, you shall preview the Web page by default...");
  system("firefox /tmp/WebPage");
  int ifDeleteWebPage;
  printf("%s\n", "Path of Web page is /tmp/WebPage, do you want to DELETE it?");
  printf("%s\t%s\n", "0. NO", "1. YES");
  do {
    printf("%s\n", "Please input 0 or 1!");
    scanf("%d", &ifDeleteWebPage);
  } while (ifDeleteWebPage != 0 && ifDeleteWebPage != 1);
  if (ifDeleteWebPage)
    system("rm -f /tmp/WebPage");
}

void Response(int n) {
  void *handle = NULL;        /* 被链接.so的句柄 */
  void (*func)(const char *); /* 被调用的函数指针 */
  switch (n) {
  case 1:
    handle = dlopen(TIME_PATH, RTLD_NOW | RTLD_GLOBAL);
    if (handle == NULL) {
      exit(DLOPEN_ERROR);
      dlclose(handle);
    }
    func = (void (*)(const char *))dlsym(handle, "GetTime");
    if (func == NULL) {
      exit(DLSYM_ERROR);
      dlclose(handle);
    }
    break;
  case 2:
    handle = dlopen(VERSION_PATH, RTLD_NOW | RTLD_GLOBAL);
    if (handle == NULL) {
      exit(DLOPEN_ERROR);
      dlclose(handle);
    }
    func = (void (*)(const char *))dlsym(handle, "GetVersion");
    if (func == NULL) {
      exit(DLSYM_ERROR);
      dlclose(handle);
    }
    break;
  case 3:
    handle = dlopen(MEMFREE_PATH, RTLD_NOW | RTLD_GLOBAL);
    if (handle == NULL) {
      exit(DLOPEN_ERROR);
      dlclose(handle);
    }
    func = (void (*)(const char *))dlsym(handle, "GetMemFree");
    if (func == NULL) {
      exit(DLSYM_ERROR);
      dlclose(handle);
    }
    break;
  case 4:
    handle = dlopen(PROCESS_PATH, RTLD_NOW | RTLD_GLOBAL);
    if (handle == NULL) {
      exit(DLOPEN_ERROR);
      dlclose(handle);
    }
    func = (void (*)(const char *))dlsym(handle, "GetProcesses");
    if (func == NULL) {
      exit(DLSYM_ERROR);
      dlclose(handle);
    }
    break;
  default:
    exit(LOGICAL_ERROR);
    break;
  }
  MakeWebPage(func);
  dlclose(handle);
}

int Serve(int client_sockfd) {
  while (true) {
    int len;
    char *msg;
    if (read(client_sockfd, &len, sizeof(len)) ==
        0) /* 消息文本长度是0，连接断开 */
    {
      printf("%s\n", "Connection OFF!");
      return -1;
    }
    msg = new char[len];
    read(client_sockfd, msg, len);
    if (!strcmp(msg, "quit")) /* 收到quit消息 */
    {
      printf("%s\n", "The client shall quit this connection...");
      delete[] msg;
      msg = NULL;
      return 0;
    }
    if (!strcmp(msg, "time"))
      tp->addTask(std::bind(Response, int(1))); /* 添加任务 */
    if (!strcmp(msg, "version"))
      tp->addTask(std::bind(Response, int(2)));
    if (!strcmp(msg, "memfree"))
      tp->addTask(std::bind(Response, int(3)));
    if (!strcmp(msg, "process"))
      tp->addTask(std::bind(Response, int(4)));
    else
      printf("%s\n", "Invalid command!");
    delete[] msg;
    msg = NULL;
  }
}

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std;

#define COMMAND_NUMBER 5 /* 合法消息数目 */
#define MSG_LENGTH 8     /* 消息长度限制 */

int sockfd;
std::string command[COMMAND_NUMBER] = {"time", "version", "memfree", "process",
                                       "quit"}; /* 命令选项 */
char msg[MSG_LENGTH];                           /* 消息 */

void SendMsg(); /* 发送消息 */

int main() {
  // 欢迎
  printf("%s\n", "I shall help you initialize this client...");
  // 初始化
  printf("%s\n", "Please input the type of this client.");
  printf("%s\t%s\n", "0. Local", "1. Remote"); /* 服务器在本地或者远程连接 */
  int tmp;
  do {
    printf("%s\n", "Please input 0 or 1");
    scanf("%d", &tmp);
  } while (tmp != 0 && tmp != 1); /* 保证选择合法 */
  // 本地套接字构造、连接
  if (tmp == 0) {
    sockfd = socket(PF_LOCAL, SOCK_STREAM, 0);
    sockaddr_un sun;
    sun.sun_family = AF_LOCAL;
    strcpy(sun.sun_path, "/tmp/socket");
    connect(sockfd, (sockaddr *)&sun, sizeof(sun));
  }
  // 远程套接字构造、连接
  else {
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(1024);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sockfd, (sockaddr *)&sin, sizeof(sin)); /* 连接服务器 */
  }
  // 命令选择、发送
  printf("%s\n", "Please input your command.");
  for (int i = 0; i < COMMAND_NUMBER; i++)
    printf("%s\t", command[i].c_str());
  printf("\n");
  memset(msg, '\0', sizeof(msg));
  cin >> msg;
  SendMsg();
  printf("%s\n", "Message sent!");
  // close(sockfd); /* 单独测试代码块，服务器关闭套接字 */
  printf("%s\n", "Client OFF!");
  /*
  if(tmp==0)
      unlink("/tmp/socket");
  else
      unlink("127.0.0.1:1024");
   */ /* 单独测试代码块，服务器删除相关文件 */
  return 0;
}

void SendMsg() {
  int len = strlen(msg) + 1;
  write(sockfd, &len, sizeof(len));
  write(sockfd, msg, len);
}

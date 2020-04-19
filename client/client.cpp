
/**
 *@description: Client class source file
 * 
 *@author zsq
 *@date 2020-4-18
 *@copyright：GPL 2.0
 **/

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>

#include "client.h"

#define SERV_IP "127.0.0.1"
#define SERV_PORT 10088

namespace client_ns
{

Client::Client()
{
    std::cout << "Client class constructed" << std::endl;
}

Client::~Client()
{
    std::cout << "Client class deconstructed" << std::endl;
}

// 信号处理函数
void handle_siganl(int sig)
{
    std::cout << "capture signal:" << sig << std::endl;
    exit(-1);
}

// 捕捉信号
void capture_signal()
{
    struct sigaction act, ocat;
    act.sa_handler = handle_siganl;
    sigfillset(&act.sa_mask);
    act.sa_flags = 0;               // 设置第一个处理函数, /0:sa_handler / 1:sa_sigaction
    sigaction(SIGINT, &act, &ocat); // 捕捉Ctrl+C中断信号s
}

void Client::run()
{

    int cfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); // 初始化

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);
    // 建立与服务器端的链接
    connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char buf[BUFSIZ];
    capture_signal();
    while (true)
    {

        fgets(buf, sizeof(buf), stdin);
        write(cfd, buf, strlen(buf));
        int len = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);
    }
    close(cfd);
}
} // namespace client_ns
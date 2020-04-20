
/**
 *@description: Server class source file
 * 
 *@author zsq
 *@date 2020-4-20
 *@copyright：GPL 2.0
 **/

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "server.h"

#define PORT 10088

#define MAX_EVENTS 20
#define TIMEOUT 500
#define MAX_PROCESS 4

namespace server_ns
{

Server::Server()
{
    std::cout << "Server class constructed" << std::endl;
}

Server::~Server()
{
    std::cout << "Server class deconstructed" << std::endl;
}

void Server::run()
{

    // 服务端后台运行
    if (daemon(0, 0) == -1)
    {
        std::cout << "error" << std::endl;
        exit(-1);
    }

    int s_fd = socket(AF_INET, SOCK_STREAM, 0); // 建立服务端套接字，返回服务器端的套接字描述符
                                                // AF_INET：IPv4通信协议， SOCK_STREAM：TCP连接
    int flags = fcntl(s_fd, F_GETFL, 0);
    fcntl(s_fd, F_SETFL, flags | O_NONBLOCK); // 设置非阻塞

    struct sockaddr_in s_addr;                  // 描述ip地址信息的结构体
    s_addr.sin_family = AF_INET;                // 采用的协议族
    s_addr.sin_port = htons(PORT);              // 端口号
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip地址；htonl将本地的小端存储格式更改为网络字节序（大端存储格式）。 INADDR_ANY:选用本机可用地址

    bind(s_fd, (struct sockaddr *)&s_addr, sizeof(s_addr)); // 绑定套接字、IP地址和端口
    listen(s_fd, 128);                                      // 最大等待连接数

    sockaddr_in c_addr;
    char buf[BUFSIZ]; // 内容缓冲区

    socklen_t len = 0;

    // 创建进程
    int pid = -1;
    for (auto i = 0; i < MAX_PROCESS; i++)
    {
        if (pid != 0)
            pid = fork();
    }
    if (pid == 0)
    {

        // epoll 方式
        int epoll_fd = epoll_create(256);
        struct epoll_event ev, events[MAX_EVENTS];
        ev.events = EPOLLIN; // 输入事件
        ev.data.fd = s_fd;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, s_fd, &ev); // 添加侦听的socket fd
        int event_number;
        while (true)
        {
            event_number = epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT); // 返回事件变化的文件描述符数量
            for (auto i = 0; i < event_number; i++)
            {
                if (events[i].data.fd == s_fd)
                {
                    // 建立与客户端的链接，返回客户端的套接字描述符，阻塞等待连接
                    int c_fd = accept(s_fd, (struct sockaddr *)&c_addr, &len);
                    int flags = fcntl(c_fd, F_GETFL, 0);
                    fcntl(c_fd, F_SETFL, flags | O_NONBLOCK); // 设置非阻塞

                    ev.events = EPOLLIN | EPOLLET; // 设置边缘触发
                    ev.data.fd = c_fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, c_fd, &ev);
                }
                else if (events[i].events & EPOLLIN)
                {
                    int n;
                    do
                    {
                        char ip_buf[33];
                        char ip_addr[BUFSIZ];

                        // 打印连接的客户端的ip地址与端口
                        inet_ntop(AF_INET, &c_addr.sin_addr.s_addr, ip_buf, sizeof(ip_buf));
                        std::cout << "client ip:" << ip_buf << std::endl; //ntohl(c_addr.sin_addr.s_addr) << endl;
                        std::cout << "client port: " << ntohs(c_addr.sin_port) << std::endl;

                        n = read(events[i].data.fd, buf, sizeof(buf)); // 读取客户端的请求内容,返回字节大小
                        std::cout << "receive:" << buf << std::endl;
                        if (n == 0)
                            close(events[i].data.fd);
                    } while (n < -1 && errno == EINTR);
                    if (n > 0)
                    { // 对字节进行小写->大写的转换
                        for (int i = 0; i < n; i++)
                            buf[i] = toupper(buf[i]);
                        write(events[i].data.fd, buf, n); // 通过客户端套接字写回客户端
                    }
                }
            }
        }

        close(s_fd);
    }
    else
    { // pid != 0 父进程， 等待所有子进程结束
        do
        {
            waitpid(-1, NULL, 0);
        } while (pid != -1);
    }
}

} // namespace server_ns
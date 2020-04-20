
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

#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"

#include "server.h"

#define PORT 10088

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

//
void on_read_cb(struct bufferevent *bev, void *ctx)
{
    struct evbuffer *input = nullptr;
    struct evbuffer *output = nullptr;
    input = bufferevent_get_input(bev);
    output = bufferevent_get_output(bev);

    // size_t len;
    // char *buf = nullptr;
    // buf = evbuffer_readln(output, &len, EVBUFFER_EOL_LF);
    // if (!buf)
    //     std::cout << "Warning: data has not arrived" << std::endl;
    // else
    // {
    //     for (auto i = 0; i < len; i++)
    //         buf[i] = toupper(buf[i]);
    // }
    // struct evbuffer *out = nullptr;
    // evbuffer_add(out, buf, len);
    evbuffer_add_buffer(output, input);
}

// 回调函数
void on_accept_cb(struct evconnlistener *listener,
                  evutil_socket_t fd,
                  struct sockaddr *addr,
                  int socklen,
                  void *ctx)
{
    struct bufferevent *bev = nullptr;
    struct event_base *base = nullptr;
    base = evconnlistener_get_base(listener);
    bev = bufferevent_socket_new(base, fd, 0);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
    bufferevent_setcb(bev, on_read_cb, NULL, NULL, NULL);
}

void Server::run()
{

    if (daemon(0, 0) == -1)
    {
        std::cout << "error" << std::endl;
        exit(-1);
    }

    struct event_base *base = nullptr;
    base = event_base_new();

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    struct evconnlistener *listener = nullptr;
    listener = evconnlistener_new_bind(base,
                                       on_accept_cb, // 回调函数
                                       NULL,         // 上下文参数
                                       LEV_OPT_REUSEABLE,
                                       10,
                                       (struct sockaddr *)&serveraddr,
                                       sizeof(serveraddr));
    event_base_dispatch(base);
}

} // namespace server_ns
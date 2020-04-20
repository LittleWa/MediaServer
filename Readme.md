## 基础服务器编程-TCP  /Server
#### 1、服务器程序后台运行 daemon
#### 2、客户端程序捕捉中断信号 sigaction
#### 3、以多进程的方式实现多客户端连接 fork
###### 运行方式：
###### 服务器端编译运行：
###### cd server
###### g++ -std=c++11 -g -o Server main.cpp server.cpp
###### ./Server
###### 客户端编译运行:
###### cd client
###### g++ -std=c++11 -g -o Client main.cpp client.cpp
###### ./Client

## 基础服务器编程-TCP  /Server_epoll
#### 1、服务器程序后台运行 daemon
#### 2、客户端程序捕捉中断信号 sigaction
#### 3、Fork+Epoll的方式实现多客户端服务器连接 fork, epoll
#### 会存在惊群问题，一个事件会触发多个进程竞争，最终只有一个创建epoll,可以通过线程池或者加锁来解决
###### cd server_epoll
###### g++ -std=c++11 -g -o Server main.cpp server.cpp
###### cd server
###### g++ -std=c++11 -g -o Server main.cpp server.cpp
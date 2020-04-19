#include <iostream>

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
/*
1、 fork子进程，父进程退出，子进程成为孤儿进程，并init进程接管
2、调用setid建立新的进程回话
3、将当前工作目录切换到根目录
4、将标准输入、输出、出错重定向到/dev/null
*/

void daemonize()
{
    int fd;
    pid_t pid;
    if (pid = fork() < 0)
    {
        std::cout << "cant create suporcess" << std::endl;
    }
    else
    {
        if (pid != 0)
            exit(0);
    }
    setsid();
    if (chdir("/") < 0)
    {
        std::cout << "can't change dir!" << std::endl;
        exit(-1);
    }
    fd = open("/dev/null", O_RDWR);

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    return;
}
int main()
{
    daemonize();
    while (true)
    {
        std::cout << "running... " << std::endl;
    }

    return 0;
}
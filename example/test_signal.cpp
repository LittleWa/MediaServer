#include <iostream>
#include <unistd.h>
#include <signal.h>

void handle_signal(int sig)
{
    std::cout << "捕捉到信号:" << sig << std::endl;
}

int main()
{
    // signal(SIGINT, handle_signal);
    /*signalaction api*/
    struct sigaction act, oact;
    act.sa_handler = handle_signal; // 设置处理函数
    sigfillset(&act.sa_mask);       // 当函数处理过程中再触发事件，不予响应
    act.sa_flags = 0;               // 设置第一个处理函数, /sa_handler/sa_sigaction
    sigaction(SIGINT, &act, &oact);
    pause();
    return 0;
}
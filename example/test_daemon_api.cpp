#include <iostream>

#include <stdlib.h>
#include <unistd.h>

int main()
{
    // 重定向到根目录, 标准输入输出到/dev/null
    if (daemon(0, 0) == -1)
    {
        std::cout << "error" << std::endl;
        exit(-1);
    }
    while (true)
    {
        sleep(1);
    }
    return 0;
}
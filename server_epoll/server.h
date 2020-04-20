#ifndef __SERVER_H_
#define __SERVER_H_

/**
 *@description: Server class header
 * 
 *@author zsq
 *@date 2020-4-16
 *@copyright：GPL 2.0
 **/

namespace server_ns
{
class Server
{
public:
    Server();
    ~Server();

public:
    void run();
};
} // namespace server_ns

#endif //__SERVER_H_
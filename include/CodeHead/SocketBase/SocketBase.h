#pragma once
#include <ros/ros.h>
#include "CodeHead/CommonHead.h"
#include "SocketConfig.h"
enum RosSocketResult
{        
       RESULT_OK            =    0,
       SOCKET_ERROR   =   -1,
       BIND_ERROR          =   -2, 
       LISTEN_ERROR      =   -3,
       ACCEPT_ERROR     =   -4,
       RESULT_FAIL_INIT  =  -5,
       EPOLL_CREATE_ERROR =-6,
       EPOLL_CTRL_ERROR =-7,
       EPOLL_WAIT_ERROR = -8,
       SET_NOBLK_ERROR =-9
};

class RosSocketBase
{
private:
    int  ssock ;
    int  csock;
    socklen_t  socketLen;
    struct  sockaddr_in   clientAddr;
    struct  sockaddr_in   serverAddr;
    struct epoll_event  events[MAX_EPOLL_EVENTS ];
    std::map<int , std::string >ClientMap;
public:
    RosSocketBase();
    RosSocketResult InitServer();  
    RosSocketResult Start();
     RosSocketResult SetNoBlocking(int sockfd);
    bool  Read(int fd, void *buf, size_t nbytes, unsigned int timout);
    bool Write(char *buffer,const int itimeout=0);
    //void CloseListen();
    //void CleanClient(int fd ,int *maxfd,fd_set &tempfds);
    void DealCall();
    void CheckDiscon(int socketfd);
    ~RosSocketBase();
};














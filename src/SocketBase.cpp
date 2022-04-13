#include "CodeHead/CommonHead.h"
#include  "SocketBase/SocketBase.h"
#include "SocketConfig.h"

void RosSocketBase::DealCall()
{
     std::cout << "CallBack "<<std::endl;
}

 RosSocketResult  RosSocketBase::InitServer()
{
    ssock = socket(AF_INET,SOCK_STREAM,0);
    if(ssock<=0){
        return SOCKET_ERROR;
     }
    #if USING_ADDRESS_REUSE 
    int reuse = 1 ;
    setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &(reuse), sizeof(reuse));
    #endif
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
   // BIND
    if(((bind(ssock,(struct sockaddr *)&serverAddr,sizeof(serverAddr)))<0)){
       // CloseListen();
        return BIND_ERROR;
    }
    //LISTEN
    if((listen(ssock,LISTEN_WAIT_MAX))<0){
        return LISTEN_ERROR;
    }
    return RESULT_OK ;
}

RosSocketResult  RosSocketBase::Start()
{
   if(ssock<0){
        return  RESULT_FAIL_INIT;
   }
   socketLen= sizeof(clientAddr);
   int epollfd=epoll_create(1);
   if(epollfd == -1){
        return EPOLL_CREATE_ERROR;
   }
   struct epoll_event  ev;
   ev.data.fd = ssock;
   ev.events =EPOLLIN;
   if (epoll_ctl(epollfd, EPOLL_CTL_ADD, ssock, &ev) == -1) {
        return    EPOLL_CTRL_ERROR;
   }
   while(true)
   {
        int nfds = epoll_wait(epollfd,events,MAX_EPOLL_EVENTS,-1); 
        if(nfds == -1){
            return  EPOLL_WAIT_ERROR;
        }
        for (int n = 0; n < nfds; ++n) 
        {
            if(events[n].data.fd == ssock)  
            {
                socketLen = sizeof(clientAddr);
                csock = accept(ssock,(struct sockaddr *)&clientAddr,&socketLen);
                if(csock<0){
                    return ACCEPT_ERROR;
                    continue;
                } 
                std::string clientIP  =  inet_ntoa (clientAddr.sin_addr);
                //Client IP
                #ifdef USING_PRINT_LOG
                std::cout <<  clientIP <<std::endl;
                #endif
                memset(&ev, 0,sizeof(struct epoll_event));
                ev.data.fd = csock;
                ev.events =EPOLLIN;
                if(epoll_ctl(epollfd,EPOLL_CTL_ADD,csock,&ev)==-1){
                     return    EPOLL_CTRL_ERROR;
                }
                // add   socketfd&&clientip  to map
                 ClientMap.insert(make_pair(csock,clientIP));
                //Connect
                #ifdef USING_PRINT_LOG
                std::cout <<"[" <<csock << "]" << " " << "connect ok"<<std::endl;
                #endif
                continue;
            }
            else if(events[n].events &EPOLLIN) //对应fd可以读(包括对端的SOCKET正常关闭)
            {
                // std::map<int , std::string >::iterator it;
                // for(it=ClientMap.begin();it!=ClientMap.end();++it)
                // std::cout<<"key: "<<it->first <<" value: "<<it->second<<std::endl;
                char buffer[RING_BUFFER_SIZE];
                bzero(buffer,sizeof(buffer));
                ssize_t  Read = read(events[n].data.fd,&buffer,sizeof(buffer));
                //Recv Msg
                #ifdef USING_PRINT_LOG
                std::cout  <<"[" <<events[n].data.fd << "]" << buffer << std::endl;
                #endif
                 //判断下是否客户端连接断开  如果连接断开就关闭相应的fd
                CheckDiscon(events[n].data.fd);
            }
        }
   }
    close(epollfd);
    return  RESULT_OK;
}

RosSocketResult   RosSocketBase::SetNoBlocking(int sockfd)
{
    if(fcntl(sockfd,F_SETFL,fcntl(sockfd,F_GETFD,0)|O_NONBLOCK)== -1 ){
        return SET_NOBLK_ERROR;
    }
}

void  RosSocketBase::CheckDiscon(int socketfd)
{
    struct tcp_info info;
    int len = sizeof(info);
    getsockopt(socketfd,IPPROTO_TCP,TCP_INFO,&info,(socklen_t *)&len);
    if(info.tcpi_state!=TCP_ESTABLISHED){
        #ifdef USING_PRINT_LOG
        std::cout << "[" << socketfd << "]" << "disconnect " <<std::endl;
        #endif
        ClientMap.erase(socketfd);
        close(socketfd);
    }
}
RosSocketBase::RosSocketBase(/* args */){
}

RosSocketBase::~RosSocketBase(){
    ClientMap.clear();
}


// char *RosSocketBase::GetIP()
// {
//   return(inet_ntoa(clientAddr.sin_addr));
// }
// Read Usiing Select 


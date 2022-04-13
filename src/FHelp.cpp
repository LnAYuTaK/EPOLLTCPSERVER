#include "CodeHead/CommonHead.h"
#include"SocketBase/SocketBase.h"
void  ErrorCheck( RosSocketResult FResult,const char* Test)
{
assert (Test);
switch (FResult)
{
case RESULT_OK:
    std::cout  <<"[" <<Test <<"]"<< "RESULT_OK"<<std::endl;
    break;
case SOCKET_ERROR:
    std::cerr <<"[" <<Test <<"]"<< "SOCKET_ERROR "<<std::endl;
    break;
case BIND_ERROR:
    std::cerr <<"[" <<Test <<"]"<< "BIND_ERROR"<<std::endl;
    break;
case LISTEN_ERROR:
    std::cerr <<"[" <<Test <<"]"<< " LISTEN_ERROR "<<std::endl;
     break;
case  ACCEPT_ERROR:
std::cerr <<"[" <<Test <<"]"<< " ACCEPT_ERROR "<<std::endl;
    break;
case  RESULT_FAIL_INIT:
std::cerr <<"[" <<Test <<"]"<< "SERVER_INIT_FAIL "<<std::endl;
    break;
default:
    break;
}
}





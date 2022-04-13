#include "CodeHead/FHelp.h"
#include "CodeHead/CommonHead.h"
#include  "SocketBase/SocketBase.h"
int main(int argc, char const *argv[])
{
    RosSocketBase  TestSock;
    ErrorCheck(TestSock.InitServer(),"InitServer");
    ErrorCheck(TestSock.Start(),"Start");
    return 0;
}


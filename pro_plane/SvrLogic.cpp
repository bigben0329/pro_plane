//
//  SvrLogic.cpp
//  pro_plane
//
//  Created by momo@ben on 13-9-8.
//
//

#include "SvrLogic.h"
#include "CCSprite.h"
#include "CThreadManager.h"

#include <stdio.h>
#include <netinet/in.h>  //定义数据结构sockaddr_in
#include <sys/socket.h>  //提供socket函数及数据结构
#include <sys/types.h> //数据类型定义
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

using namespace std;


#define FREG_FUNC(scmd, bfunc)\
{\
if( 0 == strcmp(cmd.c_str(),scmd) )\
{\
CCLOG("do %s", scmd);\
if( helloworld )\
{\
helloworld->bfunc = true;\
}\
else\
{\
CCLOG("_helloworld is null");\
}\
CCLOG("end %s", scmd);\
}\
}


int Process(int fd, HelloWorld* helloworld )
{
    CCLOG("Process begin fd:%d",fd);
    
    //子进程用于接收信息
    char buf[1024] = {0};
    memset(buf,0,100);
    if(recv(fd,buf,100,0) <= 0)
    {
        CCLOG("recv:");
        close(fd);
        raise(SIGSTOP);
        exit(1);
    }
    CCLOG("server cmd response:%s",buf);
    
    
    std::string cmd, reps;
    std::string rbody = std::string(buf);
    int r = CSvrLogic::splitResp(rbody, cmd, reps);
    CCLOG("r:%d cmd:%s reps:%s", r, cmd.c_str(), reps.c_str());
    if( 0 == r )
    {
        helloworld->_cmd = cmd;
        helloworld->_response = reps;
        
        FREG_FUNC("onlineinfo", bsetOnlineLable)
        FREG_FUNC("hit",bsetBoomPos)
        FREG_FUNC("hitstat",bsetBoomStatPos)
        {
            CCLOG("can not found function for cmd %s", cmd.c_str());
        }
    }
    
	return 0;
}


CSvrLogic::CSvrLogic()
{
    _bready = false;
}


CSvrLogic::~CSvrLogic()
{
    closeSvr();
}


int CSvrLogic::initSvr(const std::string ip, const int port )
{
    //build svr ip
    struct hostent *host;
    host = gethostbyname(ip.c_str());
    if((_clientfd = socket(AF_INET,SOCK_STREAM,0)) == -1)  //创建客户端套接字
    {
        CCLOG("socket error!\n");
        return -1;
    }
    
    //绑定客户端套接字
    struct sockaddr_in clientaddr;
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(port);
    clientaddr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(clientaddr.sin_zero),0);
    if(connect(_clientfd,(struct sockaddr *)&clientaddr,sizeof(struct sockaddr)) == -1) //连接服务端
    {
        CCLOG("connect error!\n");
        return -1;
    }
    
    //接收服务器第一次发包
    _rev_buf=(char *)malloc(1024);
    memset(_rev_buf,0,1024);
    
    //int recvbytes = 0;
    if( recv(_clientfd,_rev_buf,1024,0) == -1)
    {
        CCLOG("recv error!");
        return -3;
    }
    CCLOG("protocol head\n%s\n",_rev_buf);
    
    // 创建线程管理类的实例，把要执行的线程函数和最大线程数传进去
    CThreadManager* pManager = new CThreadManager(Process, 1);
    pManager->regHelloWorld(_helloworld);
	pManager->regFd(_clientfd);
	pManager->PostSem();
    
    _bready = true;    
    return 0;
}

int CSvrLogic::splitResp(const std::string resp, std::string& cmd, std::string& body)
{
    std::string::size_type pos1, pos2;
    pos2 = resp.find('@');
    pos1 = 0;
    if(std::string::npos == pos2)
    {
        CCLOG("not found cmd! resp:%s\n", resp.c_str());
        return -1;
    }
    
    cmd = resp.substr(pos1, pos2 - pos1);
    pos1 = pos2 + 1;
    body = resp.substr(pos1);
    CCLOG("cmd:%s\nbody:%s\n", cmd.c_str(), body.c_str());
    
    return 0;
}

int CSvrLogic::doSvrCmd(std::string cmd, std::string request)
{
    std::string tmp;
    return doSvrCmd(cmd, request, tmp, false);
}


int CSvrLogic::doSvrCmd(std::string cmd, std::string request, std::string& reps, bool bNeedResp)
{
    if( !_bready )
    {
        CCLOG("client fd is not ready!");
        return -1;
    }
    
    //send request buffer
    char buf[1024] = {0};
    memset(buf,0,1024);
    strcpy(buf,cmd.c_str());
    strcat(buf,"@");
    strncat(buf,request.c_str(), request.length());
    CCLOG("doSvrCmd send buf:%s\n", buf);
    
    int sendbytes;//定义客户端套接字
    if((sendbytes = send(_clientfd, buf, strlen(buf), 0)) == -1)
    {
        CCLOG("send error!\n");
        return -1;
    }
    
    
    if( !bNeedResp )
        return 0;
    
    reps.clear();
    memset(buf,0,1024);
    
    //recive response buffer
    if(recv(_clientfd,buf,1024,0) <= 0)
    {
        CCLOG("recv error!");
        close(_clientfd);
        raise(SIGSTOP);
        return -4;
    }
    CCLOG("doSvrCmd recive buf:%s",buf);
    
    std::string rbody = std::string(buf);
    int r = splitResp(rbody, cmd, reps);
    if( 0 == r )
    {
        
    }
    
    return r;
}

int CSvrLogic::closeSvr()
{
    close(_clientfd);
    return 0;
}

int CSvrLogic::regScene(HelloWorld* hl)
{
    _helloworld = hl;
    return 0;
}


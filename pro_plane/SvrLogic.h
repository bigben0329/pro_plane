//
//  SvrLogic.h
//  pro_plane
//
//  Created by momo@ben on 13-9-8.
//
//

#ifndef __pro_plane__SvrLogic__
#define __pro_plane__SvrLogic__

#include <iostream>
#include "HelloWorldScene.h"

class CSvrLogic {
public:
    
    CSvrLogic();
    ~CSvrLogic();
    
    int initSvr(const std::string ip, const int port);
    int doSvrCmd(std::string cmd, std::string request);
    int doSvrCmd(std::string cmd, std::string request, std::string& reps, bool bNeedResp = true);
    int splitResp(const std::string resp, std::string& cmd, std::string& body);
    
    int regScene(HelloWorld* hl);
    
    int closeSvr();
    
    
    
private:
    
    int _clientfd;//定义客户端套接字
    HelloWorld* _helloworld;
    
};


#endif /* defined(__pro_plane__SvrLogic__) */

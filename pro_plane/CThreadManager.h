/*
 * CThreadManager.h
 *
 *  Created on: 2010/05/08
 *      Author: zh2000g
 */

#ifndef CTHREADMANAGER_H_
#define CTHREADMANAGER_H_

#include <stdio.h>
#include <list>
#include <queue>
#include <semaphore.h>

#include "CThread.h"
#include "HelloWorldScene.h"

using namespace std;

class CThreadManager {
	friend void* ManageFuction(void*);
private:
	sem_t m_sem;	// 信号量
	pthread_mutex_t m_mutex; // 互斥锁
    
	queue<int> m_queWork; // 工作队列
	list<CThread*> m_lstThread; // 线程list
    
	int (*m_threadFuction)(int, HelloWorld*); //函数指针，指向main函数传过来的线程执行函数
    
    HelloWorld* _hl;
    int _fd;
    
    
public:
	CThreadManager(int (*threadFuction)(int, HelloWorld*), int nMaxThreadCnt);
	virtual ~CThreadManager();
    
	int WaitSem();
    
	int PostSem();
    
	int LockMutex();
    
	int UnlockMutex();
    
    
	int RunThreadFunction(int fd, HelloWorld* hl);
    
    void regHelloWorld(HelloWorld* hl);
    void regFd(int fd);
};

#endif /* CTHREADMANAGER_H_ */

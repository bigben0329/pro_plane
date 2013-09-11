/*
 * CThreadManager.cpp
 *
 *  Created on: 2010/05/08
 *      Author: zh2000g
 */

#include "CThreadManager.h"


// 线程执行函数，它只是个壳子，处理信号量和互斥锁等，
// 最后调用main函数传过来的线程执行函数来实现业务处理
void* ManageFuction(void* argv)
{
	CThreadManager* pManager = (CThreadManager*)argv;
    
	// 进行无限循环（意味着线程是不销毁的，重复利用）
	while(true)
	{
		// 线程开启后，就在这里阻塞着，直到main函数设置了信号量
		pManager->WaitSem();
		printf("thread wakeup.\n");
		pManager->RunThreadFunction(pManager->_fd, pManager->_hl);
	}
    
	return 0;
}


CThreadManager::CThreadManager(int (*threadFuction)(int, HelloWorld*), int nMaxThreadCnt) {
    
	sem_init(&m_sem, 0, 0);
	pthread_mutex_init(&m_mutex, NULL);
    
	m_threadFuction = threadFuction;
    
	for(int i=0; i<nMaxThreadCnt; i++)
	{
		CThread* pThread = new CThread(ManageFuction, this);
		printf("thread started.\n");
		m_lstThread.push_back(pThread);
	}
}

CThreadManager::~CThreadManager()
{
	sem_destroy(&m_sem);
	pthread_mutex_destroy(&m_mutex);
    
	list<CThread*>::iterator it;
	for(it=m_lstThread.begin(); it!=m_lstThread.end();it++)
	{
		(*it)->JoinThread();
	}
}

// 等待信号量
int CThreadManager::WaitSem()
{
	return sem_wait(&m_sem);
}

// 设置信号量
int CThreadManager::PostSem()
{
	return sem_post(&m_sem);
}

// 取得锁
int CThreadManager::LockMutex()
{
	int n= pthread_mutex_lock(&m_mutex);
	return n;
}

// 释放锁
int CThreadManager::UnlockMutex()
{
	return pthread_mutex_unlock(&m_mutex);
}

// 执行main函数传过来的线程执行函数
int CThreadManager::RunThreadFunction(int fd, HelloWorld* hl)
{
	return (*m_threadFuction)(fd, hl);
}

void CThreadManager::regHelloWorld(HelloWorld* hl)
{
    _hl = hl;
}

void CThreadManager::regFd(int fd)
{
    _fd = fd;
}

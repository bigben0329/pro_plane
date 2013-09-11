/*
 * CThread.cpp
 *
 *  Created on: 2010/05/08
 *      Author: zh2000g
 */

#include "CThread.h"

CThread::CThread(void* (*threadFuction)(void*),void* threadArgv) {

	// 初始化线程属性
	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);

	pthread_create(&m_thread, &threadAttr, threadFuction, threadArgv);
}

CThread::~CThread() {
	// TODO Auto-generated destructor stub
}


void CThread::JoinThread()
{
	// join
	pthread_join(m_thread, NULL);
}

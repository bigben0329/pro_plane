/*
 * CThread.h
 *
 *  Created on: 2010/05/08
 *      Author: zh2000g
 */

#ifndef CTHREAD_H_
#define CTHREAD_H_

#include <pthread.h>

class CThread {
private:
	pthread_t m_thread;  //保持线程句柄
public:
	CThread(void* (*threadFuction)(void*),void* threadArgv);
	virtual ~CThread();

	void JoinThread();
};

#endif /* CTHREAD_H_ */

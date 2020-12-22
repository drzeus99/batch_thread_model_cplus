//
//  thread_pool.h
//
//
//  Created by Zachary Kraus on 2/18/16.
//
//

#ifndef ____thread_pool__
#define ____thread_pool__

#include <atomic>
#include "kmap.h"
#include <thread>
#include <vector>
#include <memory>

//do we need to pause threads when lack of processes exists
class thread_pool
{
public:
	thread_pool(unsigned long long, unsigned int, bool = true);
	void insert(std::shared_ptr<void>&&, void* (*method) (void*));
	void begin();
	void end();
private:
	std::vector<std::thread> pool;
	std::atomic<unsigned long long> num {1};
	bool threaded_write;
	struct helper
	{
		helper() {};
		helper(std::shared_ptr<void>&& in, void* (*method) (void*)) : method(method) {this->in = move(in);}
		std::shared_ptr<void> in;
		void* (*method) (void*);
	};
	kmap<unsigned long long, helper> storage;
	bool pool_exit = false;
	void batch_run(int, int);
	unsigned int thread_count;
};

#endif /* defined(____thread_pool__) */

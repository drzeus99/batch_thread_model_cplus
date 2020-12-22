//
//  thread_pool.cpp
//
//
//  Created by Zachary Kraus on 2/18/16.
//
//

#include "thread_pool.h"
using namespace std;

thread_pool::thread_pool(unsigned long long init_pool_size, unsigned int thread_count, bool threaded_write)
{
	storage.resize(init_pool_size);
	//pool.resize(thread_count);
	this->thread_count = thread_count;
    storage.begin_read_write(threaded_write); //this step ensures multi threaded inserting does not cause data races
	this->threaded_write = threaded_write;
}

void thread_pool::insert(shared_ptr<void>&& input, void* (*method) (void*))
{
	int process_num = num.fetch_add(1);
	storage.insert(process_num, helper(move(input), method));
}

void thread_pool::begin()
{
	storage.end_read_write(threaded_write);
	storage.begin_read_write(threaded_write);
	for (unsigned int i = 0; i < thread_count; i++)
		pool.push_back(thread(&thread_pool::batch_run, this, i, thread_count));
}

void thread_pool::end()
{
	pool_exit = true;
	for (auto& val: pool)
		val.join();
}

void thread_pool::batch_run(int start_index, int thread_num)
{
	while (!pool_exit) {
		for (unsigned int i = start_index; i < storage.hash_size(); i += thread_num) {
			for (auto& item: storage.batch(i))
				item.second.method(item.second.in.get());
			storage.move_write_batch(i);
		}
	}
	//code to guarantee correct exiting
	for (int j = 0; j < 2; j++) {
		for (unsigned int i = start_index; i < storage.hash_size(); i += thread_num) {
			for (auto& item: storage.batch(i))
				item.second.method(item.second.in.get());
			storage.move_write_batch(i);
		}
	}
}

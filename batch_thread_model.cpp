#include <unordered_map>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <cmath>
#include "thread_pool.h"
#include "batch_thread_probability.h"
using namespace std;

ostream& operator<<(ostream& in, vector<vector<int> > result);

//need to initialize vector<double> results. I am now writing out of range

struct input
{
    #ifdef LOW_MEM
        input(int threads, const vector<int>& batches, vector<double>& results, int index) :
            threads(threads), batches(batches), results(results), index(index)
            {}
    #else
        input(int threads, const vector<int>& batches, vector<double>& results, shared_ptr<vector<vector<int>>> combinations, int index) :
            combinations(combinations), threads(threads), batches(batches), results(results), index(index)
            {}
        shared_ptr<vector<vector<int>>> combinations;
    #endif // LOW_MEM
	int threads;
	const vector<int>& batches;
	vector<double>& results;
	int index;
};

void* calc (void* in)
{
	input* tup = (input*) in;
	/*printf("before testing thread %d and batches %d\n", tup->threads, tup->batches[tup->index]);
	for (auto val: tup->results)
		printf("%f ", val);
	printf("\n");
	printf("the index is %d\n", tup->index);*/
	/*printf("the combinations are: ");
	cout << tup->combinations; */
	#ifdef LOW_MEM
        tup->results.at(tup->index) = multinomial_experiment(tup->threads, tup->batches[tup->index]);
	#else
        tup->results[tup->index] = multinomial_experiment(tup->threads, tup->batches[tup->index], *tup->combinations);
	#endif // LOW_MEM
	printf("finished testing thread %d and batches %d\n", tup->threads, tup->batches[tup->index]);
	return nullptr;
}

thread_pool tp(3027, 24, true); //4 took 1228.5s ns 1838.104s os,
//4 threads took 976.8s pbo test
//changed from 2000 to 2300 size with 6 threads took 810.9s ns 1656.352s os uma test
//6 threads took 806.5s ns numa
//6 threads took 803.0s ns numactl cpunode=0 memnode=0
//6 threads took 644.8s pbo test
//changed from 2300 to 2000 size with 8 threads took 636.9s ns
//8 threads took 504.3 pbo test
//changed from 2000 to 2300 size with 12 threads took 341.1 ns uma test
//12 threads took 441.4s ns numa test
//12 threads took 430.3s ns numa --interleave=0-1 --all
//12 threads took 440.7s ns numactl --all --localalloc equivalent to just running normal process
//12 threads took 340.0s pbo test --
//changed from 2300 to 3027 size with 24 threads took 327.2s ns uma test
//24 threads took 351.3s ns numa test
//24 threads took 337.1s ns numactl --all --interleave=0-1
//24 threads took 315.6s pbo test

//!HIGH MEM
//4/4/2/1 run took 1136.3s ns used 72.8 GiB memory
//4/4/2/1 run took 889.0s pbo test
//6/4/2/1 changed from 2000 to 2300 size run took 694.4s ns used 52 GiB memory uma test
//6/4/2/1 took 711.7s ns numa test
//6/4/2/1 took 553.8s pbo test
//8/4/4/2 changed from 2300 to 2000 size run took 543.1s ns used 66 Gib memory
//8/4/4/2 took 428.3s pbo test
//9/3/3 changed from 2000 to 2300 size took 478.4s ns used 62.8 GiB memory uma test
//9/3/3 took 495.0s ns numa test
//9/3/3 took 399.4s pbo test
//12/4/4/3 run took 406.1s ns used 43.3 GiB memory
//12/4/4/3 run took 396.4s ns used 50.8 GiB memory uma test
//12/4/4/3 run took 395.4s ns numa test
//12/4/4/3 run took 334.1s pbo test
//18/6/6/6 run took 449.1s ns used 25 GiB memory **strange behavior fan drops to 1600 temp drops from 94.5 to 80 tctrl yet near 80% cpu utilization
//18/6/6/6 run took 400.0s pbo test
//20/4/4/4 changed from 2300 to 2560 size run took 433.1s ns used 25.3 GiB memory **strange behavior fan drops to 800 temp drops from 94.5 to 80 tctrl yet near 100% cpu utilization
//20/4/4/4 run took 338.0s pbo test
//20/4/4/4 change from 2560 to 5020 size run took 449.5s ns UMA test
//20/4/4/4 run took 446.3s ns numa test

//!Potentially could build a double threadpool model but I think it would only be helpful for 18 and 20 thread cases
void build_model(const int threads, const vector<int> &batches, vector<double> &results)
{
	int previous = batches[0];
	#ifdef LOW_MEM
        tp.insert(shared_ptr<void>(new input(threads, batches, results, 0)), &calc);
	#else
        shared_ptr<vector<vector<int> > > combinations(new vector<vector<int> >(outcome_combinations(threads, batches[0])));
        tp.insert(shared_ptr<void>(new input(threads, batches, results, combinations, 0)), &calc);
	#endif // LOW_MEM
	printf("finished storing thread %d and batches %d\n", threads, batches[0]);
	for (unsigned int i = 1; i < batches.size(); i++)
	{
	    #ifdef LOW_MEM
            tp.insert(shared_ptr<void>(new input(threads, batches, results, i)), &calc);
	    #else
            if (previous <= threads)
                combinations = shared_ptr<vector<vector<int> > >(new vector<vector<int> >(outcome_combinations(threads, batches[i]))) ;
            previous = batches[i];
            tp.insert(shared_ptr<void>(new input(threads, batches, results, combinations, i)), &calc);
	    #endif // LOW_MEM
		printf("finished storing thread %d and batches %d\n", threads, batches[i]);
	}
}

void model(int start, int finish, int delta, int thread_count, vector<int>& batches, unordered_map<int, vector<double> >& data)
{
    int j = start;
	//fill data using multi threading
	while (j <= finish) {
		vector<thread> threads;
		for (int i = 0; i < thread_count; i++, j += delta) {
			if (j <= finish) {
                data[j].resize(batches.size());
				threads.push_back(thread(build_model, j, ref(batches), ref(data[j])));
			}
			else
				break;
		}
		for (auto& val: threads)
			val.join();
	}
}

int main()
{
	unordered_map<int, vector<double> > data;
	vector<int> batches;

	for (int i = 2; i < 50; i++)
		batches.push_back(i);

	for (int i = 50; i <= 160; i++)
		batches.push_back(i);
	//fill data using multi threading
	//need to change threads.push_back to threads[i] and presize threads
	//can try to change threads to 3, 2, or 1 for final part to see if it solves memory problem
	//in worse case we can add outcome_combinations into multinomial_experiment, thereby relieving the memory issue

	//fill data using multi threading
	model(2, 16, 1, 4, batches, data);
	tp.begin();
    model(18, 32, 2, 2, batches, data);
    //model(36, 52, 4, 4, batches, data);
    //model(56, 64, 4, 3, batches, data);
    model(36, 64, 4, 1, batches, data);
    tp.end();

	ofstream file("data.txt");

	for (int val: batches)
		file << val << " ";
	file << endl;

	for (auto& item: data) {
		file << item.first << " " ;
		for (double val: item.second)
			file << val <<  " ";
		file << endl;
	}

	int result = system("python3 batch_thread_graph_driver.py");

	return result;
	//return 0;
}

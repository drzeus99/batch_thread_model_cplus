#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
using namespace std; //Have an error in this code somewhere need to find it

ostream& operator<<(ostream& in, vector<int> result)
{
	for (auto val: result)
		in << val << " ";
	in << endl;
	return in;
}

ostream& operator<<(ostream& in, vector<vector<int> > result)
{
	for (auto& vect: result)
		in << vect;
	return in;
}

int sumate(vector<int> &in)
{
	int result = 0;
	for (int val: in)
		result += val;
	return result;
}

class Count
{
public:
	Count();
	void add(unsigned);
	void subtract(unsigned);
	bool duplicate();
	bool unique();
	int get();
private:
	unsigned count;
};

Count::Count()
{
	count = 0;
}

void Count::add(unsigned number)
{
	count += number;
}

void Count::subtract(unsigned number)
{
	count = (number > count)? 0 : count - number;
}

int Count::get()
{
	return count;
}

bool Count::duplicate()
{
	return count > 1;
}

bool Count::unique()
{
	return count == 1;
}


void next(int val, int threads, int max_len, vector<vector<int> > &ans, int sum)
{
	if (sum >= threads)
		return;
	if (int(ans.back().size()) >= max_len) {
		ans.pop_back();
		return;
	}
	int diff = threads - sum;
	if (diff > val && ans.back().size() >= 2)
		diff = val;
	ans.back().push_back(diff);
	vector<int> temp = ans.back();
	if ( sum != threads)
		next(diff, threads, max_len, ans, sum + diff);
	for (int i = diff - 1; i > 0; i--) {
		ans.push_back(temp);
		ans.back().back() = i;
		next(i, threads, max_len, ans, sum + i);
	}
}


vector<vector<int> > outcome_combinations(int threads, int batches)
{
	int max_len;
	if (threads > batches)
		max_len = batches;
	else
		max_len = threads - 1;
	vector<vector<int> > ans;
	for (int i =  threads; i >= 2; i--) {
		ans.push_back(vector<int>(1, i));
		next(i, threads, max_len, ans, i);
	}
	return ans;
}

#ifdef LOW_MEM
    double multinomial_experiment(int threads, int batches)
    {
        vector<vector<int> > combinations = outcome_combinations(threads, batches);
#else
    double multinomial_experiment(int threads, int batches, vector<vector<int> > &combinations)
    {
#endif //LOW_MEM
	    double ans = 0.0;
	    double individual_prob = 1 / float(batches);
	    double starting_prob = tgamma(threads + 1) * pow(individual_prob, threads);
	    unordered_map<int, Count> accounting_table;
	    for (auto& vect: combinations) {
		    accounting_table.clear();
		    double experiment_prob = starting_prob;
		    for (auto num: vect) {
			    experiment_prob /=  tgamma(num + 1);
			    accounting_table[num].add(1);
		    }
		    //above code gives the experimental probability of a specific combination of outcomes
		    //below code multiples the experimental probability by the permuation of experiments
		    //that use those outcomes to find the total probability of experiments that have that combination
		    //of outcomes
		    experiment_prob *= tgamma(batches) / tgamma(batches - vect.size() + 1);
		    accounting_table[vect.front()].subtract(1);
		    for (auto& item: accounting_table) {
			    experiment_prob /= tgamma(item.second.get() + 1);
		    }
		    ans += experiment_prob;
	    }
	    return ans;
    }

/*
int main() {
    #ifdef LOW_MEM
        cout << "the multinomial experiment result is " << multinomial_experiment(3, 4) << endl;
        cout << "the multinomial experiment result is " << multinomial_experiment(4, 100) << endl;
    #else
        vector<vector<int> > a = outcome_combinations(3,4);
	    cout << "the answer is " << a <<endl;
        cout << "the multinomial experiment result is " << multinomial_experiment(3, 4, a) << endl;
	    a = outcome_combinations(4, 100);
	    cout << "the answer is " << a<< endl;
	    cout << "the multinomial experiment result is " << multinomial_experiment(4, 100, a) << endl;
    #endif //LOW_MEM
	return 0;
}*/

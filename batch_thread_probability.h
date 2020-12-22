#ifndef BATCH_THREAD_PROBABILITY_H_INCLUDED
#define BATCH_THREAD_PROBABILITY_H_INCLUDED

#ifdef LOW_MEM
    double multinomial_experiment(int, int);
#else
    std::vector<std::vector<int> > outcome_combinations(int, int);
    double multinomial_experiment(int, int, std::vector<std::vector<int> > &);
#endif //LOW_MEM

#endif // BATCH_THREAD_PROBABILITY_H_INCLUDED

# ifndef GA_H
# define GA_H

# include <vector>
# include <random>

class GA {
private:
    int pop_size, ch_size, max_gen, mutation_rate, c;
    std::vector<std::vector<bool>> population;
    std::vector<int> &weights, &values, fitnesses;

    std::random_device rd;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random_value;
    std::uniform_int_distribution<int> random_gene;

public:
    GA(std::vector<int> &weights, std::vector<int> &values, int c,
        int pop_size = 50, int max_gen = 500, int mutation_rate = 1);

    int fitness(int ch);
    void select(int &parent1, int &parent2);
    void mutation(std::vector<bool> &offspring);
    void crossover(int p1, int p2, std::vector<bool> &c1, std::vector<bool> &c2);
    void breed(int parent1, int parent2);
    void start();
};

# endif /* end of include guard: GA_H */

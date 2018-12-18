# include "ga.h"
# include <iostream>
# include <map>

using namespace std;

GA::GA(vector<int> &weights, vector<int> &values, int c, int pop_size,
int max_gen, int mutation_rate): weights(weights), values(values), c(c),
pop_size(pop_size), max_gen(max_gen), mutation_rate(mutation_rate) {
    ch_size = weights.size();
    generator = default_random_engine(rd());
    random_value = uniform_int_distribution<int>(0, 1);
    random_gene = uniform_int_distribution<int>(0, ch_size - 1);
    population = vector<vector<bool>>(pop_size, vector<bool>(ch_size));
    fitnesses.resize(pop_size);

    for (int i = 0; i < pop_size; ++i)
        for (int j = 0; j < ch_size; ++j)
            population[i][j] = random_value(generator);
}

int GA::fitness(int ch) {
    int total_value, total_weight;

    do {
        total_value = total_weight = 0;
        for (int i = 0; i < ch_size; ++i) {
            total_value += values[i] * population[ch][i];
            total_weight += weights[i] * population[ch][i];
        }

        if (total_weight > c) {
            int gene;
            do gene = random_gene(generator); while (!population[ch][gene]);
            population[ch][gene] = 0;
            total_weight -= weights[gene];
        }

    } while (total_weight > c);

    return total_value;
}

void GA::select(int &parent1, int &parent2) {
    int total = 0;
    for (int i = 0; i < pop_size; ++i) total += fitnesses[i];

    uniform_int_distribution<int> v(0, total);
    int chosen = v(generator);

    for (int i = 0; chosen > 0; ++i) {
        chosen -= fitnesses[i];
        if (chosen <= 0) { parent1 = i; break; }
    }

    chosen = v(generator);
    for (int i = 0; chosen > 0; ++i) {
        chosen -= fitnesses[i];
        if (chosen <= 0) { parent2 = i; break; }
    }
}

void GA::mutation(vector<bool> &offspring) {
    uniform_int_distribution<int> m(1, 100);

    for (int i = 0; i < ch_size; ++i)
        if (m(generator) == 1) offspring[i] = !offspring[i];
}

void GA::crossover(int p1, int p2, vector<bool> &c1, vector<bool> &c2) {
    int pivot = random_gene(generator);

    for (int i = 0; i <= pivot; ++i) {
        c1[i] = population[p1][i];
        c2[i] = population[p2][i];
    }

    for (int i = pivot; i < ch_size; ++i) {
        c1[i] = population[p2][i];
        c2[i] = population[p1][i];
    }

    mutation(c1); mutation(c2);
}

void GA::breed(int parent1, int parent2) {
    vector<vector<bool>> new_population(pop_size, vector<bool>(ch_size));
    int survivors_qtd = pop_size - int(pop_size * 0.85);

    // Select the survivors using elitism
    multimap<int, int> best;
    for (int i = 0; i < pop_size; ++i) best.insert({ fitnesses[i], i });

    int i = 0;
    for (auto it = best.rbegin(); it != best.rend(), i < survivors_qtd; ++it)
        new_population[i] = population[it->second], ++i;

    // Generate the rest of the new population through crossover
    vector<bool> c1(ch_size), c2(ch_size);
    for (int i = survivors_qtd; i + 1 < pop_size; ++i) {
        crossover(parent1, parent2, c1, c2);
        new_population[i] = c1; new_population[i + 1] = c2;
    }

    population = new_population;
}

void GA::start() {
    cout << '\n';
    for (int i = 0; i < max_gen; ++i) {
        cout << "\rProcessing generation " << i + 1 << "/" << max_gen << "...";

        // Calculate the fitness of all chromossomes
        for (int j = 0; j < pop_size; ++j) fitnesses[j] = fitness(j);

        // Probabilistically chooses the parents
        int parent1, parent2;
        select(parent1, parent2);

        // Breed and generate a new population
        breed(parent1, parent2);

        for (int j = 0; j < pop_size; ++j) fitnesses[j] = fitness(j);
    }

    int best_solution = 0;
    for (int i = 0; i < pop_size; ++i) {
        if (fitnesses[i] > fitnesses[best_solution]) best_solution = i;

        int total_weight = 0;
        for (int j = 0; j < ch_size; ++j)
            total_weight += weights[j] * population[i][j];
    }

    cout << "\n\nBest solution found:\n";
    int total_weight = 0;
    for (int j = 0; j < ch_size; ++j) {
        total_weight += population[best_solution][j] * weights[j];
        cout << population[best_solution][j] << ' ';
    }
    cout << "\ntotal value: " << fitnesses[best_solution] << "\n";
    cout << "total weight: " << total_weight << '\n';
}

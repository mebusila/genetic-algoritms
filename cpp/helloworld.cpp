#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <math.h>

#define POPULATION_SIZE      2048
#define MAX_GENERATIONS      16384
#define CROSSOVER            RAND_MAX * 0.80f
#define ELITISM_RATE         0.10f
#define MUATION_RATE         0.30f
#define MUTATION             RAND_MAX * MUATION_RATE
#define TARGET               "hello world!"

using namespace std;

struct chromosome {
    char gene[100];
    unsigned int fitness;
};

void init_population(vector<chromosome> &population, vector<chromosome> &buffer) {
    int tsize = strlen(TARGET);

    for (int i = 0; i < POPULATION_SIZE; i++) {
        chromosome citizen;
        citizen.fitness = 0;
        for (int j = 0; j < tsize; j++)
            citizen.gene[j] = (rand() % 90) + 32;
        citizen.gene[tsize] = '\0';
        population.push_back(citizen);
    }

    buffer.resize(POPULATION_SIZE);
}

void calc_fitness(vector<chromosome> &population) {
    string target = TARGET;
    int tsize = target.size();
    unsigned int fitness;

    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness = 0;
        for (int j = 0; j < tsize; j++) {
            fitness += abs(int(population[i].gene[j] - target[j]));
        }
        population[i].fitness = fitness;
    }
}

bool fitness_sort(chromosome x, chromosome y) {
    return (x.fitness < y.fitness);
}

inline void sort_by_fitness(vector<chromosome> &population) {
    sort(population.begin(), population.end(), fitness_sort);
}

void elitism(vector<chromosome> &population, vector<chromosome> &buffer, int esize) {
    for (int i = 0; i < esize; i++) {
        for (int j = 0; j < strlen(buffer[i].gene); j ++) {
            buffer[i].gene[j] = population[i].gene[j];
        }
        buffer[i].fitness = population[i].fitness;
    }
}

void mutate(chromosome &citizen) {
    int tsize = strlen(TARGET);
    int ipos = rand() % tsize;
    int delta = (rand() % 90) + 32;
    citizen.gene[ipos] = ((citizen.gene[ipos] + delta) % 122);
}

void mate(vector<chromosome> &population, vector<chromosome> &buffer) {
    int esize = POPULATION_SIZE * ELITISM_RATE;
    int tsize = strlen(TARGET), spos, i1, i2;
    
    elitism(population, buffer, esize);

    for (int i = esize; i < POPULATION_SIZE; i++) {
        if (rand() < CROSSOVER) {
            i1   = rand() % (POPULATION_SIZE / 2);
            i2   = rand() % (POPULATION_SIZE / 2);
            spos = rand() % tsize;

            for (int j = 0; j < spos; j ++) {
                buffer[i].gene[j] = population[i1].gene[j];
            }
            
            for (int j = spos; j < tsize - spos; j ++) {
                buffer[i].gene[j] = population[i2].gene[j];
            }
            if (rand() < MUTATION) mutate(buffer[i]);
        } else {
            if (rand() < MUTATION) mutate(buffer[i]);
        }
    }
}

inline void swap(vector<chromosome> *&population, vector<chromosome> *&buffer) {
    vector<chromosome> *temp = population;
    population = buffer;
    buffer = temp;
}

int main() {
    srand(unsigned(time(NULL)));

    vector<chromosome> pop_alpha, pop_beta;
    vector<chromosome> *population, *buffer;

    init_population(pop_alpha, pop_beta);
    population = &pop_alpha;
    buffer = &pop_beta;

    for (int i = 0; i < MAX_GENERATIONS; i++) {
        calc_fitness(*population);
        sort_by_fitness(*population);
        cout << "generation: " << i << " : " << (*population)[0].gene << " (" << (*population)[0].fitness << ")" << endl;
        if ((*population)[0].fitness == 0) break;

        mate(*population, *buffer);
        swap(population, buffer);
    }
    return 0;
}
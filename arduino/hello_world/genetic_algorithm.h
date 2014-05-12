#include <vector>
#include <string>

#define POPULATION_SIZE      20
#define MAX_GENERATIONS      16384
#define CROSSOVER            RAND_MAX * 0.80f
#define ELITISM_RATE         0.10f
#define MUATION_RATE         0.90f
#define MUTATION             RAND_MAX * MUATION_RATE
#define TARGET               std::string("hello world")

using namespace std;

struct chromosome {
    string gene;
    unsigned short fitness;
};

vector<chromosome> pop_alpha, pop_beta;
vector<chromosome> *population, *buffer;

void init_population(vector<chromosome> &population, vector<chromosome> &buffer) {
    int tsize = TARGET.size();
    for (int i = 0; i < POPULATION_SIZE; i++) {
        chromosome citizen;
        citizen.fitness = 0;
        citizen.gene.erase();
        for (int j = 0; j < tsize; j++)
            citizen.gene += (rand() % 90) + 32;
        population.push_back(citizen);
    }
    buffer.resize(POPULATION_SIZE);
}

void calc_fitness(vector<chromosome> &population) {
    string target = TARGET;
    int tsize = target.size();
    unsigned short fitness;
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
        buffer[i].gene = population[i].gene;
        buffer[i].fitness = population[i].fitness;
    }
}

void mutate(chromosome &citizen)
{
    int tsize = TARGET.size();
    int ipos  = rand() % tsize;
    int delta = (rand() % 90) + 32; 
    citizen.gene[ipos] = ((citizen.gene[ipos] + delta) % 122);
}

void mate(vector<chromosome> &population, vector<chromosome> &buffer) {
    int esize = POPULATION_SIZE * ELITISM_RATE;
    int tsize = TARGET.size(), spos, i1, i2;
    
    elitism(population, buffer, esize);

    for (int i = esize; i < POPULATION_SIZE; i++) {
        if (rand() < CROSSOVER) {
            i1   = rand() % (POPULATION_SIZE / 2);
            i2   = rand() % (POPULATION_SIZE / 2);
            spos = rand() % tsize;

            buffer[i].gene = population[i1].gene.substr(0, spos) + 
                             population[i2].gene.substr(spos, tsize - spos);
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

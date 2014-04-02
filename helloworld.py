from random import (choice, random, randint)

class Chromosome:

    _target = "Hello World"

    def __init__(self, gene):
        self.gene = gene
        self.fitness = Chromosome._set_fitness(gene)

    @staticmethod
    def _set_fitness(gene):
        fitness = 0
        for a, b in zip(gene, Chromosome._target):
            fitness += abs(ord(a) - ord(b))

        return fitness

    @staticmethod
    def init_random():
        gene = []
        for i in range(len(Chromosome._target)):
            gene.append(chr(randint(32, 121)))

        return Chromosome(''.join(gene))

    def mate(self, mate):
        p = randint(0, len(self.gene) - 1)
        g1 = self.gene[:p] + mate.gene[p:]
        g2 = mate.gene[:p] + self.gene[p:]

        return Chromosome(g1), Chromosome(g2)

    def mutate(self):
        gene = list(self.gene)
        delta = randint(32, 121)
        idx = randint(0, len(gene) - 1)
        gene[idx] = chr((ord(gene[idx]) + delta) % 122 )

        return Chromosome(''.join(gene))


class Population:

    _size = 3

    def __init__(self, size=1024, crossover=0.8, elitism=0.1, mutation=0.03):
        self.elitism = elitism
        self.mutation = mutation
        self.crossover = crossover

        buf = []
        for i in range(size):
            buf.append(Chromosome.init_random())

        self.population = list(sorted(buf, key=lambda x: x.fitness))

    def selection(self):
        best = choice(self.population)
        for i in range(Population._size):
            cont = choice(self.population)
            if (cont.fitness < best.fitness):
                best = cont
        return best


    def _get_parents(self):
        return (self.selection(), self.selection())

    def evolve(self):
        size = len(self.population)
        idx = int(round(size * self.elitism))
        buf = self.population[:idx]

        while (idx < size):
            if random() <= self.crossover:
                (p1, p2) = self._get_parents()
                children = p1.mate(p2)
                for c in children:
                    if random() <= self.mutation:
                        buf.append(c.mutate())
                    else:
                        buf.append(c)
                idx += 2
            else:
                if random() <= self.mutation:
                    buf.append(self.population[idx].mutate())
                else:
                    buf.append(self.population[idx])
                idx += 1

        self.population = list(sorted(buf[:size], key=lambda x: x.fitness))


if __name__ == "__main__":
    itinerations = 16384
    p = Population(size=2048, crossover=0.8, elitism=0.1, mutation=0.3)

    for i in range(1, itinerations + 1):
        print("generation: %d %s" % (i, p.population[0].gene))
        if p.population[0].fitness == 0:
            break
        else:
            p.evolve()


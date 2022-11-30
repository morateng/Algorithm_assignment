
#pragma once

#include "ga.h"

struct chromosome pop1[POPULATION_SIZE], pop2[POPULATION_SIZE];
struct chromosome* pop = pop1, * pop_next = pop2;
struct chromosome best;

void init_pop();
void ga_impl();
void evaluate(struct chromosome* f);
int select(int f_sum);
void crossover(int i1, int i2);
void mutation();
void print_result(int f_sum);
bool new_best();

int generations;

void print_chromosome(int idx) {
	int i;
	printf("x%d:", idx);
	for (i = 0; i < GENES; i++)
		printf("%d ", pop[idx].genes[i]);
	printf("\n");
}
void print_population() {
	int i;

	printf("solution population\n");
	for (i = 0; i < POPULATION_SIZE; i++)
		print_chromosome(i);
}
void copy_genes(int dest[], int src[]) {
	for (int i = 0; i < GENES; i++)
		dest[i] = src[i];
}
void init_pop() {
	int i, j;

	for (i = 0; i < POPULATION_SIZE; i+=2) {
		for (j = 0; j < GENES; j++) {
			if (rand() % 2) {
				pop[i].genes[j] = 1;
				pop[i + 1].genes[j] = 0;
			}
			else {
				pop[i].genes[j] = 0;
				pop[i + 1].genes[j] = 1;
			}
		}
	}
	print_population();

	for (int i = 0; i < POPULATION_SIZE; i++)
		evaluate(pop + i);

	best.fitness = 0;
	new_best();

	int fitness_sum = 0;
	for (i = 0; i < POPULATION_SIZE; i++)
		fitness_sum += pop[i].fitness;
	print_result(fitness_sum);
}

void ga_impl() {
	int i;
	int count = 0;
	int fitness_sum;
	struct chromosome* pop_temp;

	generations = 0;
	while (count < GA_END) {
		generations++;

		fitness_sum = 0;
		for (i = 0; i < POPULATION_SIZE; i++)
			fitness_sum += pop[i].fitness;

		if (generations%10==0)
			print_result(fitness_sum);

		for (i = 0; i < POPULATION_SIZE; i++)
			pop_next[i] = pop[select(fitness_sum)];

		SWAP(pop, pop_next, pop_temp);

		for (i = 0; i < POPULATION_SIZE; i += 2) {
			if (drand() <= PCROSS)
				crossover(i, i + 1);
		}

		mutation();

		for (i = 0; i < POPULATION_SIZE; i++)
			evaluate(pop + i);

		if (new_best())
			count = 0;
		else {
			count++;
			pop[0] = best;
		}
	}

	print_result(fitness_sum);
}

bool new_best() {
	bool flag = false;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (best.fitness < pop[i].fitness) {
			best = pop[i];
			flag = true;
		}
	}
	return flag;
}

void evaluate(chromosome* f) {
	int w = 1;
	f->fitness = 0;
	f->x = 0;
	for (int i = GENES - 1; i >= 0; i--) {
		f->x += f->genes[i] * w;
		w *= 2;
	}
	f->fitness = 15 * f->x - f->x * f->x;
}
void print_result(int f_sum) {
	int f_x, f_fitness;

	printf("generation: %d, fitness avg: %.1f\n", generations, (double)f_sum / POPULATION_SIZE);
	f_fitness = 0;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (f_fitness < pop[i].fitness) {
			f_x = pop[i].x;
			f_fitness = pop[i].fitness;
		}
	}
	printf("pop-max : x:%d, f(x):%d\n", f_x, f_fitness);
	printf("best-max: x:%d, f(x):%d\n", best.x, best.fitness);
}

void mutation() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		for (int j = 0; j < GENES; j++) {
			if (drand() <= PMUT) {
				if (pop[i].genes[j] == 0)
					pop[i].genes[j] = 1;
				else
					pop[i].genes[j] = 0;
			}
		}
	}
}
void crossover(int i1, int i2) {
	int tmp;
	int point = random(1, GENES);

	//for (int i = point; i < GENES; i++)
	//	SWAP(pop[i1].genes[i], pop[i2].genes[i], tmp);	
	//�Ʒ��� �ϼ��Ͻÿ�.
	for (int i = 0; i <= point; i++)
		SWAP(pop[i1].genes[i], pop[i2].genes[i], tmp);
}

int select(int f_sum) {
	int i;
	double r, cdf_sum;

	r = drand() * f_sum;
	cdf_sum = 0.0;

	for (i = 0; i < POPULATION_SIZE; i++) {
		cdf_sum += pop[i].fitness;
		if (r <= cdf_sum)
			return i;
	}
	return POPULATION_SIZE - 1;
}

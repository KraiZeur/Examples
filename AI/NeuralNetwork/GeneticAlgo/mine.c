#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/**
 * 
 * 1 - on trie les meilleurs individu par bestfit le meilleur sera le premier
 * 2 - on garde l'élite (25%) les 25 premier par ex
 * 3 - on créée des crossOver en cherchant 2 meilleurs parmi 8 individus random
 * 4 - on trouve le pivot du crossOver
 * 5 - on CrossOver
 * 6 - on fait une effectue une mutation d'un gene en fonction de la probabilité (25%)
 * 7 - on calcul le bestfit du premier element
 * 
 * tant que bestfit !=0
 * 
 * @brief [brief description]
 * @details [long description]
 * 
 * @return [description]
 */

#define CHARMAP "abcdefghijklmnopqrstuvwxyz"\
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
		"0123456789"\
		"!@#$%%^&*(_-)+=[]{}<>|\\;:'\",./?~` "

#define RANDBETWEEN(A,B) A + rand()/(RAND_MAX/(B - A))

char * target;
size_t target_length;
size_t total_size;
size_t population_size;

/** RANDOM **/
static char rndchr(char* map) {
	return *(map+RANDBETWEEN(0, strlen(map)));
}

static char randchar() {
	return rndchr(CHARMAP);
}

void rndstr(char * dest, char* map, size_t strsize) {
	int i;

	for (i = 0; i < strsize; i++) {
		*(dest+i) = rndchr(map);
	};
}

/** FITNESS **/
static int fitness(char* str, char* gauge, size_t n) {
	int i;
	int result = 0;

	for (i = n-1; i >= 0; i--) {
		result += abs(str[i]-gauge[i]);
	}

	return result;
}

static int _fitness(char * str) {
	return fitness(str, target, target_length);
}

static int fit_cmp(const void * el1, const void * el2) {
	int a = _fitness((char *)el1);
	int b = _fitness((char *)el2);

	if (a > b) return 1;
	if (a < b) return -1;
	return 0;
}

/** MUTATING **/
#define CHANCE(A) rand() < A * RAND_MAX
static unsigned int challengers = 3; // fight between 3 for crossover (finding best father phenotype)
static float elitism = .1; // keep 10% best
static float mutating_factor = .25;

static void mutate(char *p) {
	*(p + RANDBETWEEN(0, target_length)) = randchar();
}

static char* rnd_el(char *p) {
	unsigned int top = population_size;

/*	if ((options & O_SPARTA) == O_SPARTA) {
		top = population_size * elitism;
	}*/

	return p + target_length * (int)(RANDBETWEEN(0, top));
}

static char* trnmnt(char *p) {
	size_t i;
	char* winner = rnd_el(p); // look for a random indivu
	char* challenger;
	int f1 = _fitness(winner); // compute score
	int f2;

	for (i = challengers; i > 0; i--) { // take the best of the 3 challengers and the random winner
		challenger = rnd_el(p);
		f2 = _fitness(challenger);
		if (f2 < f1) {
			f1 = f2;
			winner = challenger;
		}
	}

	return winner;
}

static void mutation(char * p, char * buffer) {
	char * a, * b;
	size_t i, pivot;
	size_t skip = (size_t)(elitism * population_size) * target_length;
	memcpy(buffer, p, total_size);

	//on skip les elites
	//et on itere sur chaque individu
	for (i = skip; i <= total_size-target_length; i += target_length) {
		a = trnmnt(p); // random individu winner amongst four father
		b = trnmnt(p); // randomer individu winner amongst four mother
		pivot = RANDBETWEEN(0, target_length); // look for a pivot for cross over

		// cross over between the winners
		strncpy(buffer + i, a, target_length);
		strncpy(buffer + i, b, pivot);

		if (CHANCE(mutating_factor)) { mutate(buffer + i); } // test if a gene can be mutated (i.e a char)

		// Create two children
		if (i < total_size - target_length) {
			i += target_length;
			strncpy(buffer + i, b, target_length);
			strncpy(buffer + i, a, pivot);
			if (CHANCE(mutating_factor)) { mutate(buffer + i); }
		}
	}

	memcpy(p, buffer, total_size);
}

int main(int argc, char * argv[]) {
	
	srand((unsigned int)time(NULL));

	population_size=2048;
	if (argc<2) {
		fprintf(stderr, "%s\n", "Not enough args");
		return -1;
	}
	target = argv[1];
	target_length=strlen(target);

	total_size=target_length*population_size;
	char * current_population=malloc(total_size);
	char * tmp=malloc(total_size);
	rndstr(current_population, CHARMAP, total_size);

	fprintf(stderr, "%.*s\n", (int)total_size, current_population);

	int bestfit=0;
	int count=0;

	do {
		qsort(current_population, population_size, target_length, fit_cmp);

		bestfit=_fitness(current_population);
		fprintf(stderr, "bestfit : %d\n", bestfit);

		mutation(current_population, tmp);
		count++;
	} while (bestfit);

	fprintf(stderr, "\n\n%.*s\n", (int)total_size, current_population);
	fprintf(stderr, "%d\n", count);

	free(current_population);
	free(tmp);

	return 0;
}
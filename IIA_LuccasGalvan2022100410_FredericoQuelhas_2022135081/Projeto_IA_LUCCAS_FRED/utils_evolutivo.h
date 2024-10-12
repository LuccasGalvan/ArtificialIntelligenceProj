

#ifndef PROJETO2IIA_UTILS_EVOLUTIVO_H
#define PROJETO2IIA_UTILS_EVOLUTIVO_H

int init_data(char *s, int *mat, struct info *d);

int flip();

pchrom init_pop(struct info d);

chrom get_best(pchrom pop, struct info d, chrom best);

void write_best(chrom x, struct info d);

int random_l_h_evolutivo(int min, int max);

float rand_01_evolutivo();

#endif //PROJETO2IIA_UTILS_EVOLUTIVO_H

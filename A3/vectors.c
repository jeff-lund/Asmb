#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vectors.h"

struct doubleVector * vector_alloc(int length) {
  struct doubleVector *dv;
  dv = (struct doubleVector *) malloc(sizeof(struct doubleVector));
  dv->length = length;
  dv->data = malloc(length * sizeof(double));

  return dv;
}

struct doubleVector * random_vector(int length) {
  struct doubleVector *dv;
  dv = vector_alloc(length);

  for(int i = 0; i < length; ++i) {
    int r1 = rand();
    int r2 = rand();
    dv->data[i] = (double) r1 / (double) r2;
  }

  return dv;
}

struct doubleVector * vector_clone(const struct doubleVector * original) {
  struct doubleVector *dv = vector_alloc(original->length);
  vectorized_copy(original->data, dv->data, original->length);

  return dv;
}

void vector_display(const struct doubleVector * vec) {
  int length = vec->length;

  printf("[");
  if(length > 0) {
    for(int i = 0, j = length - 1; i < j; ++i) {
      printf("%f, ", vec->data[i]);
    }
    printf("%f", vec->data[length - 1]);
  }
  printf("]");
}

bool scalar_fma(struct doubleVector * a,
                const struct doubleVector * b,
                const struct doubleVector * c) {
  int length = a->length;
  if (b->length != length || c->length != length) {
    return false;
  }

  scalar_fma_asm(a->data, b->data, c->data, length);

  return true;
}

bool vector_compare(const struct doubleVector * v1, const struct doubleVector * v2) {
  int length = v1->length;
  if (v2->length != length) {
    return false;
  }

  return vectorized_compare(v1->data, v2->data, length);
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "vectors.h"

int main(int argc, char ** argv) {
  srand(time(NULL));

  int length = 5;
  if (argc > 1) {
    int newLength = atoi(argv[1]);
    if (newLength > 0) {
      length = newLength;
    }
  }

  struct doubleVector *a = vector_alloc(length);
  struct doubleVector *b = vector_alloc(length);
  struct doubleVector *c = vector_alloc(length);

  for(int i = 0; i < length; ++i) {
    a->data[i] = 1.0;
    b->data[i] = 2.0;
    c->data[i] = 3.0;
  }
  a->data[0] = 2.1303856227531655;
  b->data[0] = 2.4580704836593399;
  c->data[0] = 1.1085901486833514;

  struct doubleVector *a2 = vector_clone(a);
  struct doubleVector *b2 = vector_clone(b);
  struct doubleVector *c2 = vector_clone(c);

  scalar_fma(a, b, c);
  printf("Scalar result: ");
  vector_display(a);

  vector_fma(a2, b2, c2);
  printf("\nVector result: ");
  vector_display(a2);
  printf("\n");

  printf("%s\n", vector_compare(a, a2) ? "MATCH" : "NO MATCH");

  free(a->data);
  free(a);
  free(b->data);
  free(b);
  free(c->data);
  free(c);
  free(a2->data);
  free(a2);
  free(b2->data);
  free(b2);
  free(c2->data);
  free(c2);
}

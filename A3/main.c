#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "vectors.h"

bool testWithLength(int length);

// This function prototype is the worst thing I have ever written
int perfTest(struct doubleVector *a,
              const struct doubleVector *b,
              const struct doubleVector *c,
              bool(*fma)(struct doubleVector *,
                         const struct doubleVector *,
                         const struct doubleVector *));

int main(void) {
  srand(time(NULL));

  for(int i = 2; i <= 10; ++i) {
    testWithLength(i);
  }

  testWithLength(10000000);
}

/*
 * bool testWithLength(int length)
 *
 * Creates three random vectors with the given length and times the performance of scalar_fma() and
 * vectorized_fma() with these vectors as input.
 *
 * Returns true if the results match.
 */
bool testWithLength(int length) {
  printf("Test length: %d\n", length);

  // Create (and clone) three vectors with the given length
  struct doubleVector * a = random_vector(length);
  struct doubleVector * b = random_vector(length);
  struct doubleVector * c = random_vector(length);

  struct doubleVector * a2 = vector_clone(a);
  struct doubleVector * b2 = vector_clone(b);
  struct doubleVector * c2 = vector_clone(c);


  // Test scalar performance
  printf("\tScalar cycle count: %d\n", perfTest(a, b, c, scalar_fma));

  // Test vector performance
  printf("\tVector cycle count: %d\n", perfTest(a2, b2, c2, vector_fma));

  // Compare results
  bool correct = memcmp(a->data,a2->data,sizeof(a->data));
  printf("\t%s\n", correct ? "NO MATCH" : "MATCH");
  //correct = vector_compare(a, a2);
  //printf("\t%s\n", correct ? "MATCH" : "NO MATCH");

  // Free dynamic resources
  free(a->data);
  free(b->data);
  free(c->data);
  free(a2->data);
  free(b2->data);
  free(c2->data);
  free(a);
  free(b);
  free(c);
  free(a2);
  free(b2);
  free(c2);

  return correct;
}

int perfTest(struct doubleVector *a,
             const struct doubleVector *b,
             const struct doubleVector *c,
             bool(*fma)(struct doubleVector *,
                        const struct doubleVector *,
                        const struct doubleVector *)) {

  int startCycles = rdtsc();
  fma(a, b, c);
  int endCycles = rdtsc();
  return endCycles - startCycles;
}

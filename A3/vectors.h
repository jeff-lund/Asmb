#ifndef _RYAN_VEC
#define _RYAN_VEC

#include <stdlib.h>

struct doubleVector {
  size_t length;
  double * data;
};

struct doubleVector * vector_alloc(int length);
struct doubleVector * random_vector(int length);
struct doubleVector * vector_clone(const struct doubleVector *original);
void vector_display(const struct doubleVector * vec);
bool vector_compare(const struct doubleVector * v1, const struct doubleVector * v2);

// We could use memcpy(), but here's a good opportunity to see what vector
// instructions look like
void vectorized_copy(double * src, double * dst, int length);
bool vectorized_compare(double * src, double * dst, int length);

// A naive scalar implementation to benchmark ourselves against
bool scalar_fma(struct doubleVector * a,
                const struct doubleVector * b,
                const struct doubleVector * c);

void scalar_fma_asm(double * a,
                    const double * b,
                    const double * c,
                    int length);

// A prototype for your code
bool vector_fma(struct doubleVector * a,
                const struct doubleVector * b,
                const struct doubleVector * c);

// Helper function to get cycle count
int rdtsc(void);

#endif

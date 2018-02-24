#include <stdbool.h>

#include "vectors.h"


bool vector_fma(struct doubleVector * a,
                const struct doubleVector * b,
                const struct doubleVector * c) {
  int length = a->length;
  double * a_data = a->data;
  double * b_data = b->data;
  double * c_data = c->data;

  if (b->length != length || c->length != length) {
    return false;
  }

  __asm__ volatile (
    "1:\n"
      "movsd (%0), %%xmm0\n"
      "movsd (%1), %%xmm1\n"
      "movsd (%2), %%xmm2\n"
      "vfmadd231sd %%xmm1, %%xmm2,  %%xmm0\n"
      "movsd %%xmm0,  (%0)\n"
      "addq  $8, %0\n"
      "addq  $8, %1\n"
      "addq  $8, %2\n"
      "loop 1b\n"
    : "+r" (a_data), "+r" (b_data), "+r" (c_data), "+c" (length)
    :
    : "memory"
  );

  return true;
}

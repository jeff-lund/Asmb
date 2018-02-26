#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vectors.h"

bool vector_fma(struct doubleVector * a, 
        const struct doubleVector * b, 
            const struct doubleVector * c) {
        
    int length = a->length;     
    double * a_data = a->data;
    double * b_data = b->data;
    double * c_data = c->data; 
    int v, s, i;
    
    s = length % 4; //scalared runs
    v = (length - s) / 4; //vectored runs

    if(b->length != length || c->length != length)
        return false;
    
    // Vectored instructions for FMA
    __asm__ volatile (
      "1:\n"
          "cmp $1, %3\n"
          "jl Prep2\n"            
      "vector_packed:\n"
          "vmovupd (%0), %%ymm0\n"
          "vmovupd (%1), %%ymm1\n"
          "vmovupd (%2), %%ymm2\n"
          "vfmadd231pd %%ymm1, %%ymm2, %%ymm0\n"
          "vmovupd %%ymm0, (%0)\n"
          "addq $32, %0\n"
          "addq $32, %1\n"
          "addq $32, %2\n"
          "loop vector_packed\n"
          "cmp $0, %4\n"
          "jne Prep\n" 
          "jmp Done\n"
      "Prep:\n"
          "movl %4, %3\n"
      "vector_scalar:\n"
          "movsd (%0), %%xmm0\n"
          "movsd (%1), %%xmm1\n"
          "movsd (%2), %%xmm2\n"
          "vfmadd231sd %%xmm1, %%xmm2,  %%xmm0\n"
          "movsd %%xmm0, (%0)\n" 
          "addq $8, %0\n"
          "addq $8, %1\n"
          "addq $8, %2\n"
          "loop vector_scalar\n"
      "Done:\n" 
          
          : "+r" (a_data), "+r" (b_data), "+r" (c_data), "+c" (v), "+r" (s)
          : 
          : "memory"
       );

    return true;
}

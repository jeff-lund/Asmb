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
    int v20, v4, s, i;
    
    // Calculating necessary runs at each level  
    s = length % 4; //scalared runs
    v20 = length / 20; //fully packed vectored runs
    v4 = (length % 20) / 4;

    if(b->length != length || c->length != length)
        return false;
    
    // Vectored instructions for FMA
    // (%0) a_data | (%1) b_data | (%2) c_data
    // (%3) v20 | (%4) v4 | (%5) s
    __asm__ volatile (
      "1:\n"
          "cmp $0, %3\n" // jump if v20 is 0 i.e. length < 20
          "je prep_four\n"
      "full_vector_packed:\n" // fully loaded packed vectors {0,1,2} {3,4,5} {6,7,8} {9,10,11} {12,13,14}
          // un
          "vmovupd (%0), %%ymm0\n"
          "vmovupd (%1), %%ymm1\n"
          "vmovupd (%2), %%ymm2\n"
          "vfmadd231pd %%ymm1, %%ymm2, %%ymm0\n"
          "vmovupd %%ymm0, (%0)\n"
          // deux
          "vmovupd 0x20(%0), %%ymm3\n"
          "vmovupd 0x20(%1), %%ymm4\n"
          "vmovupd 0x20(%2), %%ymm5\n"
          "vfmadd231pd %%ymm4, %%ymm5, %%ymm3\n"
          "vmovupd %%ymm3, 0x20(%0)\n"
          // trois
          "vmovupd 0x40(%0), %%ymm6\n"
          "vmovupd 0x40(%1), %%ymm7\n"
          "vmovupd 0x40(%2), %%ymm8\n"
          "vfmadd231pd %%ymm7, %%ymm8, %%ymm6\n"
          "vmovupd %%ymm6, 0x40(%0)\n"
          // quatre
          "vmovupd 0x60(%0), %%ymm9\n"
          "vmovupd 0x60(%1), %%ymm10\n"
          "vmovupd 0x60(%2), %%ymm11\n"
          "vfmadd231pd %%ymm10, %%ymm11, %%ymm9\n"
          "vmovupd %%ymm9, 0x60(%0)\n"
          //cinq
          //"vmovupd 0x80(%0), %%ymm12\n"
          //"vmovupd 0x80(%1), %%ymm13\n"
          //"vmovupd 0x80(%2), %%ymm14\n"
          //"vfmadd231pd %%ymm13, %%ymm14, %%ymm12\n"
          //"vmovupd %%ymm12, 0x80(%0)\n"
	      //moving pointers
          "addq $0xA0, %0\n"
          "addq $0xA0, %1\n"
          "addq $0xA0, %2\n"
          "loop full_vector_packed\n"
          "cmp $0, %4\n"  
          "je prep_scalar\n"
      "prep_four:\n"
          "movl %4, %3\n"
          "cmp $0, %3\n"
          "je prep_scalar\n"
      "vector_packed:\n" // single packed vector 
          "vmovupd (%0), %%ymm0\n"
          "vmovupd (%1), %%ymm1\n"
          "vmovupd (%2), %%ymm2\n"
          "vfmadd231pd %%ymm1, %%ymm2, %%ymm0\n"
          "vmovupd %%ymm0, (%0)\n"
          "addq $32, %0\n"
          "addq $32, %1\n"
          "addq $32, %2\n"
          "loop vector_packed\n"
      "prep_scalar:\n"
          "movl %5, %3\n"
          "cmp $0, %3\n"
          "je Done\n"
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
          
          : "+r" (a_data), "+r" (b_data), "+r" (c_data), "+c" (v20), "+r" (v4), "+r" (s)
          : 
          : "memory"
       );

    return true;
}

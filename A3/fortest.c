#include <stdio.h>

void main() {
    int i, m, r;
    for(int i = 0; i < 10; i++) {
        m = i - i%4;
        r = i%4;
        printf("Initial: %d Full loops: %d Scalar Loops: %d\n", i, m , r);
    }
}

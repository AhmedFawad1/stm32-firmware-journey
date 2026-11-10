#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define SHOW(t) printf("%-10s %zu\n", #t, sizeof(t))
int main(){
    SHOW(char);
    SHOW(short);
    SHOW(int);
    SHOW(long);
    SHOW(long long);
    SHOW(float);
    SHOW(double);
    SHOW(void *);
    SHOW(uint8_t);
    SHOW(uint16_t);
    SHOW(uint32_t);
    SHOW(uint64_t);
    SHOW(size_t);
    return 0;
}
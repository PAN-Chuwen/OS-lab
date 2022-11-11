#include "print.h"
#include "sbi.h"

void puts(char *s) {
    for (; *s != '\0'; s++) {
        sbi_ecall(0x01, 0x0, *s, 0, 0, 0, 0, 0);
    }
}

void puti(int x) {
    const int base = 10;
    int power = 1;
    // avoid using array 
    do
    {
        power *= 10;
    } while (x >= power);
    /*
        x = 10 -> power = 100
    */
    
    /*
        power /= 10 -> power = 10
        curDigit = '1'
        x = 10 % 10 = 0
        ...
    */

    while (power != 1)
    {
        power /= base;
        char curDigitASCII = (x / power) + '0';
        sbi_ecall(0x1, 0x0, curDigitASCII, 0, 0, 0, 0, 0);
        x = x % power;
    }
    

    sbi_ecall(0x1, 0x0, x, 0, 0, 0, 0, 0);
}

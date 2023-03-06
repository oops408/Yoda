/**
//////////////////////////////////////////////////////////////////////////////////
// Engineer: Emmanuel Francis
// Create Date: 28.05.2020 18:48:38
// Module Name: lpf
// Project Name: YODA
// Description: Low Pass Filter
//////////////////////////////////////////////////////////////////////////////////

 */

#include <iostream>

typedef unsigned char BYTE;

const BYTE y_test[] = {
    100, 125, 148, 168, 184, 185, 200,
    198, 190, 177, 159, 148, 113, 87,
    63, 41, 23, 17, 2, 0, 5,
    16, 32, 52, 75
};

const int MAX_DATA_SIZE = 100;

BYTE y_lpf[MAX_DATA_SIZE];

BYTE avg4(BYTE a, BYTE b, BYTE c, BYTE d)
{
    BYTE sum = (a + b + c + d) / 4;
    return sum;
}

void lpf(int reset, int loadmem, int readmem, int dolpf, unsigned i, BYTE xin, BYTE& xout)
{
    static BYTE mem_raw[MAX_DATA_SIZE];
    static BYTE mem_lpf[MAX_DATA_SIZE];
    static unsigned int n = 0;

    if (reset) {
        n = 0;
    }
    else if (loadmem) {
        mem_raw[i] = xin;
        if (i >= n) {
            n = i + 1;
        }
    }
    else if (readmem) {
        xout = mem_lpf[i];
    }

    if (dolpf) {
        int j = (i > 0) ? (i - 1) : (n - 1);
        mem_lpf[i] = avg4(mem_raw[j], mem_raw[i], mem_raw[(i + 1) % n], mem_raw[(i + 2) % n]);
    }
}

int main()
{
    std::cout << "LPF using wrapping moving average of 4!" << std::endl;

    lpf(1, 0, 0, 0, 0, 0, y_lpf[0]);

    for (unsigned int i = 0; i < sizeof(y_test) / sizeof(y_test[0]); i++) {
        lpf(0, 1, 0, 0, i, y_test[i], y_lpf[i]);
    }

    for (unsigned int i = 0; i < sizeof(y_test) / sizeof(y_test[0]); i++) {
        lpf(0, 0, 0, 1, i, y_test[i], y_lpf[i]);
    }

    std::cout << "LPF4(Y) = " <<

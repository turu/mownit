/**
*  Piotr Turek, zad 1
*  x{n+1} = x{n} + 3.0 * x{n} * (1 - x{n})
*  x{0} = 0.01
*/

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "../commons/point.h"
#include "../commons/timemeas.h"

void testFloat(float x, int n, FILE * out) {
    int i = 0;
    while (n--) {
        fprintf(out, "%d %f\n", i, x);
        float tmp1 = 1. - x;
        float tmp2 = x * tmp1;
        float tmp3 = 3. * tmp2;
        x = x + tmp3;
        i++;
    }
}

void testDouble(double x, int n, FILE * out) {
    int i = 0;
    while (n--) {
        fprintf(out, "%d %.10f\n", i, x);
        double tmp1 = 1. - x;
        double tmp2 = x * tmp1;
        double tmp3 = 3. * tmp2;
        x = x + tmp3;
        i++;
    }
}

int main(int argc, char ** argv) {
    int c, n = 100;
    char * optvalue;
    resetTimer();

    while ((c = getopt(argc, argv, "n:")) != -1) {
        switch(c) {
            case 'n':
                optvalue = optarg;
                if (optvalue != 0) {
                    n = atoi(optvalue);
                }
            break;
        }
    }

    FILE * fout1 = fopen("zad1_1", "w");
    FILE * fout2 = fopen("zad1_2", "w");

    //Test float
    float xf = 0.01;
    testFloat(xf, n, fout1);

    //Test double
    double xd = 0.01;
    testDouble(xd, n, fout2);

    fclose(fout1);
    fclose(fout2);

    printf("%.9fms elapsed", getMillisElapsed());

    return 0;
}

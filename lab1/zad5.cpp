#include <cstdio>
#include <unistd.h>
#include <cstdlib>

//Unstable euler method tried on a stiff equation: y' = -15y, y(0) = 1
void euler(FILE * out, double step, double m) {

    double t = 0;
    double y = 1;
    while (t <= m) {
        fprintf(out, "%0.10f %0.20f\n", t, y);
        y = y + step * (-15. * y);
        t = t + step;
    }

}

int main(int argc, char ** argv) {

    double max = 1;
    double h1 = 1./4., h2 = 1./8., h3 = 1./16.;
    FILE * out1 = fopen("zad5_1", "w");
    FILE * out2 = fopen("zad5_2", "w");
    FILE * out3 = fopen("zad5_3", "w");

    int c;
    char * optval;
    while ((c = getopt(argc, argv, "a:b:c:m:")) != -1) {
        switch(c) {
            case 'a':
                optval = optarg;
                h1 = atof(optval);
            break;
            case 'b':
                optval = optarg;
                h2 = atof(optval);
            break;
            case 'c':
                optval = optarg;
                h3 = atof(optval);
            break;
            case 'm':
                optval = optarg;
                max = atof(optval);
            break;
        }
    }

    euler(out1, h1, max);
    euler(out2, h2, max);
    euler(out3, h3, max);

    fclose(out1);
    fclose(out2);
    fclose(out3);
    return 0;

}

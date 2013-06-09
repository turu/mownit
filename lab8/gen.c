#include <stdlib.h>
#include <stdio.h>

#define SIZE 70
#define MAX_Z 10
#define ERR .0001
#define STOP_ON 1
#define STOP_OFF 0

#define ABS(x) ((x) < 0 ? -(x) : (x))


double h[SIZE][SIZE];

double f1(double x) {
    return MAX_Z * x*x / (SIZE * SIZE);
}

double f2(double x) {
    return MAX_Z - f1(x);
}

int update(int i, int j) {
    if (i == 0 || i == SIZE-1 || j == 0 || j == SIZE-1) {
        return STOP_ON;
    }

    double prev = h[i][j];
    h[i][j] = (h[i][j+1] + h[i][j-1] + h[i+1][j] + h[i-1][j]) / 4;

    if (ABS(prev - h[i][j]) < ERR) {
        return STOP_ON;
    } else {
        return STOP_OFF;
    }
}

void output_data(char* name){
    int i,j;
    FILE * f = fopen(name, "w");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            fprintf(f, "%d\t%d\t%f\n", i, j, h[i][j]);
        }
        fprintf(f,"\n");
    }

    fclose(f);
}

void impose_conditions(){
    int i,j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            h[i][j] = 0;
        }
    }
    for (i = 0; i < SIZE; i++) {
        h[0][i] = f1((double)i);
        h[SIZE-1][i] = f2((double)i);
    }
}

int iterate(){
    int i,j;
    int stop = STOP_ON;
    for (i = 0; i < SIZE; i++) {
        for (j=0;j < SIZE; j++) {
            stop *= update(i, j);
        }
    }

    return stop;
}

void solve() {
    while (1) {
        if (iterate() == STOP_ON) break;
    }
}

int main(int argc, char** argv){
    FILE * f = fopen("plot.p", "w");
    fprintf(f, "set iso 30\nset samp 30\nunset key\n");
    fprintf(f, "set xrange[0:%d]\n",SIZE-1);
    fprintf(f, "set yrange[0:%d]\n",SIZE-1);
    fprintf(f, "unset surf\n");
    fprintf(f, "set style line 1 lt 4 lw .5\n");
    fprintf(f, "set pm3d at s hidden3d 1\n");
    fprintf(f,"splot \"output.dat\" with lines\n");

    impose_conditions();
    solve();
    output_data("output.dat");
    fclose(f);

    system("gnuplot -p plot.p");
    return 0;
}

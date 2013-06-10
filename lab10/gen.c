#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MODE_EXPLICIT 0
#define MODE_IMPLICIT 1

#define SIZE 90
#define MAX_Z 1
#define ERR .0001
#define STOP_ON 1
#define STOP_OFF 0
#define K .01
#define H 1.
#define ALPHA 1.

#define ABS(x) ((x) < 0 ? -(x) : (x))

double gaussian(double x, double y, double x0, double y0, double A, double a, double b, double c) {
    return A * exp(-1*(a*(x - x0)*(x - x0) + 2*b*(x - x0)*(y - y0) + c*(y - y0)*(y - y0)));
}

typedef struct moisture_pattern {
    int bell_cnt;
    double * x;
    double * y;
    double * A;
    double * a;
    double * b;
    double * c;
} moisture_pattern;

moisture_pattern * create_random_moisture_pattern(int bell_cnt, double x_min, double x_max,
                                                         double y_min, double y_max, double A_max) {
    srand(time(NULL));
    double a_max = 0.1;
    double b_max = 0;
    double c_max = 0.1;
    moisture_pattern * pattern = (moisture_pattern *) malloc(sizeof(moisture_pattern));

    pattern->bell_cnt = bell_cnt;
    pattern->x = (double*) malloc(sizeof(double)*bell_cnt);
    pattern->y = (double*) malloc(sizeof(double)*bell_cnt);
    pattern->A = (double*) malloc(sizeof(double)*bell_cnt);
    pattern->a = (double*) malloc(sizeof(double)*bell_cnt);
    pattern->b = (double*) malloc(sizeof(double)*bell_cnt);
    pattern->c = (double*) malloc(sizeof(double)*bell_cnt);
    while (bell_cnt--) {
        double ratio = rand() % 100;
        double x = x_min + ratio * (x_max - x_min) / 100.;
        ratio = rand() % 100;
        double y = y_min + ratio * (y_max - y_min) / 100.;
        ratio = rand() % 100;
        double A = ratio * A_max / 100.;
        ratio = rand() % 100;
        double a = ratio * a_max / 100.;
        ratio = rand() % 100;
        double b = ratio * b_max / 100.;
        ratio = rand() % 100;
        double c = ratio * c_max / 100.;
        pattern->x[bell_cnt] = x;
        pattern->y[bell_cnt] = y;
        pattern->A[bell_cnt] = A;
        pattern->a[bell_cnt] = a;
        pattern->b[bell_cnt] = b;
        pattern->c[bell_cnt] = c;
    }

    return pattern;
}

void delete_moisture_pattern(moisture_pattern * pattern) {
    free(pattern->x);
    free(pattern->y);
    free(pattern->A);
    free(pattern->a);
    free(pattern->b);
    free(pattern->c);
    free(pattern);
}

double eval_moisture_pattern(double x, double y, moisture_pattern * pattern) {
    double a = 0.1, b = 0, c = 0.1;
    double res = 0;
    int i;
    for (i = 0; i < pattern->bell_cnt; i++) {
        res += gaussian(x, y, pattern->x[i], pattern->y[i], pattern->A[i],
                        pattern->a[i], pattern->b[i], pattern->c[i]);
    }
    return res;
}

double h[SIZE][SIZE];

void init_value(moisture_pattern * pattern) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            h[i][j] = eval_moisture_pattern(i, j, pattern);
            //printf("%f ", h[i][j]);
        }
    }
}

void impose_conditions(){
    int i,j;
    for (i = 0; i < SIZE; i++) {
        h[0][i] = 0;
        h[SIZE-1][i] = 0;
        h[i][0] = 0;
        h[i][SIZE-1] = 0;
    }
}

void init_sim(moisture_pattern * pattern) {
    init_value(pattern);
    impose_conditions();
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

void save_prev(double store[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            store[i][j] = h[i][j];
        }
    }
}

void explicit_step() {
    static double h_prev[SIZE][SIZE];
    static double lambda = K / (ALPHA * H * H);
    save_prev(h_prev);

    int i, j;
    for (i = 1; i < SIZE - 1; i++) {
        for (j = 1; j < SIZE - 1; j++) {
            h[i][j] = (1. - 4.*lambda)*h[i][j] + lambda*
                    (h[i+1][j] + h[i-1][j] + h[i][j+1] + h[i][j-1]);
        }
    }
}

void implicit_step() {
}

void step(int mode) {
    if (mode == MODE_EXPLICIT) {
        explicit_step();
    } else if (mode == MODE_IMPLICIT) {
        implicit_step();
    }
}

void play(FILE * plot_file, double max_time, int mode) {
    double time = 0;
    char tmp_name[10];
    int iter_ctr = 0;
    while (time <= max_time) {
        step(mode);
        sprintf(tmp_name, "data/%d.dat", iter_ctr);
        output_data(tmp_name);
        fprintf(plot_file, "splot \"data/%d.dat\" with lines\n", iter_ctr);

        time += K;
        iter_ctr++;
    }
}

int main(int argc, char** argv){
    FILE * f = fopen("init.p", "w");
    fprintf(f, "set iso 30\nset samp 30\nunset key\n");
    fprintf(f, "set xrange[0:%d]\n",SIZE-1);
    fprintf(f, "set yrange[0:%d]\n",SIZE-1);
    fprintf(f, "unset surf\n");
    fprintf(f, "set style line 1 lt 4 lw .5\n");
    fprintf(f, "set pm3d at s hidden3d 1\n");
    fprintf(f, "splot \"data/init.dat\" with lines\n");
    fclose(f);
    f = fopen("animate.p", "w");
    fprintf(f, "set iso 30\nset samp 30\nunset key\n");
    fprintf(f, "set xrange[0:%d]\n",SIZE-1);
    fprintf(f, "set yrange[0:%d]\n",SIZE-1);
    fprintf(f, "unset surf\n");
    fprintf(f, "set style line 1 lt 4 lw .5\n");
    fprintf(f, "set pm3d at s hidden3d 1\n");

    moisture_pattern * pattern = create_random_moisture_pattern(200, 0, SIZE-1, 0, SIZE-1, MAX_Z);
    init_sim(pattern);
    output_data("data/init.dat");
    play(f, 1, MODE_EXPLICIT);
    delete_moisture_pattern(pattern);
    fclose(f);

    system("gnuplot -p init.p");
    return 0;
}

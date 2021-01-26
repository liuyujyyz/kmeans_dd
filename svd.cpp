#include <cstdio>
#include <cstdlib>
#include "math.h"

FILE * fin = fopen ("YPMSD.in", "r");
FILE * fout1 = fopen ("YPMSD1.in", "w");
FILE * fout2 = fopen ("YPMSD2.in", "w");
FILE * fout3 = fopen ("YPMSD3.in", "w");

int PTS = 100000, DIM = 90;

int main (){
    for (int i = 0; i < PTS; i ++){
        double t;
        for (int j = 0; j < 30; j ++){
            fscanf(fin, "%lf", &t);
            fprintf (fout1, "%lf ", t);
        }
        fprintf (fout1, "\n");
        for (int j = 0; j < 30; j ++){
            fscanf(fin, "%lf", &t);
            fprintf (fout2, "%lf ", t);
        }
        fprintf (fout2, "\n");
        for (int j = 0; j < 30; j ++){
            fscanf(fin, "%lf", &t);
            fprintf (fout3, "%lf ", t);
        }
        fprintf (fout3, "\n");
    }
    return 0;
}

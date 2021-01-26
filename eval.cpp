#include <cstdio>
#include <cstdlib>

FILE * fin;

int PTS = 100000, DIM = 90, K, d;

double ** v;
double ** c;
int * sc;
int * g;

void calc_cent(){
     for (int i =0; i < K; i ++){
         for (int j = 0; j<DIM; j ++){
             c[i][j] = 0;
         }
     }
     for (int i = 0; i < PTS; i ++){
         if (g[i] != -1)
         for (int j = 0; j < DIM; j ++){
             c[g[i]][j] += v[i][j];
         }
     }
     for (int i =0; i < K; i ++){
         if (sc[i] != 0)
         for (int j =0 ; j < DIM; j ++){
             c[i][j] /= sc[i];
         }
     }
     return;
}

void print_err(){
     double re = 0.0;
     for (int i = 0; i < PTS; i ++){
         if (g[i] != -1)
         for (int j = 0; j < DIM; j ++){
             double delta = v[i][j] - c[g[i]][j];
             re += delta * delta;
         }
     }
     fprintf (fin, "K = %d lowrank = %d cost = %lf\n", K, d, re);
}

int main(int argc, char * argv[]){
    fin = fopen(argv[1], "r");
    fscanf (fin, "%d%d", &d, &K);
    v = (double **)malloc(sizeof(double*) * PTS);
    c = (double **)malloc(sizeof(double*) * K);
    g = (int*)malloc(sizeof(int)*PTS);
    sc = (int*)malloc(sizeof(int)*K);
    for (int i = 0; i < PTS; i ++){
        v[i] = (double*)malloc(sizeof(double) * DIM);
        g[i] = 0;
    }
    for (int i = 0; i < K; i ++){
        c[i] = (double *)malloc(sizeof(double) * DIM);
        sc[i] = 0;
    }
    double tmp;
    for (int i = 0; i < K; i ++){
        for (int j =0; j < d; j ++){
            fscanf (fin, "%lf", &tmp);
        }
    }
    for (int i = 0; i < PTS; i ++){
        fscanf (fin, "%d", &(g[i]));
        if (g[i] != -1)
        sc[g[i]] ++;
    }
    fin = fopen ("YPMSD.in", "r");
    for (int i =0; i < PTS; i ++){
        for (int j = 0; j < DIM; j++){
            fscanf (fin, "%lf", &(v[i][j]));
        }
    }
    calc_cent ();
    fin = fopen (argv[2], "w");
    print_err ();    
    return 0;
}

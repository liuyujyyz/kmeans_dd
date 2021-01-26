#include <cstdio>
#include <cstdlib>
#include "math.h"

FILE * fin = fopen ("YPMSD.in", "r");
FILE * fout = fopen ("YPMSDCSoutlier.out", "w");

double ** v;
double ** c;
double ** cs;
double ** csc;
int * g;
int * sc;
int K, DIM = 90, PTS = 100000, CSD, Z = 500;
int outlier[500];
double outdist[500];
int min_out = 0;
int n_out = 0;

void column_selection(){
     cs = (double **)malloc (sizeof (double *) * PTS);
     for (int i = 0; i < PTS; i ++){
         cs[i] = (double *)malloc(sizeof(double) * CSD);
     }
     csc = (double **)malloc(sizeof(double *) * K);
     for (int i = 0; i < K; i++){
         csc[i] = (double *)malloc(sizeof(double) * CSD);
     }
     for (int i = 0; i < CSD; i ++){
         int tmp = rand () % DIM;
         for (int j = 0; j < PTS; j ++){
             cs[j][i] = v[j][tmp];
         }
     }
     return;
}

int nearest (int ID){
    int re = g[ID];
    double tmp = HUGE_VAL;
    for (int j = 0; j < K;  j ++){
        double dist = 0.0;
        for (int k =0; k < CSD; k ++){
            double delta = csc[j][k] - cs[ID][k];
            dist += delta * delta;
        }
        if (dist < tmp){
           tmp = dist;
           re = j;
        }
    }
    if (n_out < Z){
       outlier[n_out] = ID;
       outdist[n_out] = tmp;
       if (outdist[min_out] > tmp) min_out = n_out;
       n_out ++;
    }
    else{
         if (tmp > outdist[min_out]){
            outlier[min_out] = ID;
            outdist[min_out] = tmp;
            for (int i = 0; i < Z; i ++){
                if (outdist[i] < tmp){
                   min_out = i;
                   tmp = outdist[i];}
            }
         }
    }
    return re;
}

void Lloyd(){
     for (int i =0; i < K; i ++){
         int tmp = rand () % PTS;
         for (int j = 0; j < CSD; j ++){
             csc[i][j] = cs[tmp][j];
         }
     }
     min_out = n_out = 0;
     for (int i =0; i < PTS; i ++){
         g[i] = nearest(i);
     }
     for (int i = 0; i < Z; i ++){
         g[outlier[i]] = -1;
     }
     int changed = 0;
     do{
         for (int i = 0; i < K; i++){
             sc[i] = 0;
             for (int j = 0; j<CSD; j ++){
                 csc[i][j] = 0.0;
             }
         }
         for (int i = 0; i < PTS; i++){
             if (g[i] == -1) continue;
             sc[g[i]] ++;
             for (int j = 0; j<CSD; j ++){
                 csc[g[i]][j] += cs[i][j];
             }
         }
         for (int i = 0; i < K; i ++){
             if (sc[i] != 0){
                for (int j = 0; j < CSD; j++){
                    csc[i][j] /= sc[i];
                }
             }
         }
         changed = 0;
         min_out = n_out = 0;
         for (int i = 0; i < PTS; i ++){
             int tmp = nearest (i);
             if (g[i] == -1){
                g[i] = tmp;
             }
             else
             if (tmp != g[i]){
                g[i] = tmp;
                changed ++;
             }
         }
         for (int i = 0; i < Z; i++){
             g[outlier[i]] = -1;
         }
         printf ("%d ", changed);
     }while (changed > PTS / 1000);
     return;
}

void calc_cent(){
     for (int i = 0; i < K; i++){
         sc[i] = 0;
         for (int j = 0; j < DIM; j ++){
             //printf ("%d %d ", i, j);
             c[i][j] = 0.0;
         }
     }
     for (int i = 0; i < PTS; i++){
         if (g[i] != -1){
         sc[g[i]] ++;
         for (int j = 0; j<DIM; j ++){
             c[g[i]][j] += v[i][j];
         }
         }
     }
     for (int i = 0; i < K; i ++){
         if (sc[i] != 0){
            for (int j = 0; j < DIM; j++){
                c[i][j] /= sc[i];
            }
         }
     }
     return;
}

void print_err(){
     double re = 0.0;
     for (int i = 0; i < PTS; i++){
         if (g[i] != -1)
         for (int j = 0; j < DIM; j++){
             double delta = v[i][j] - c[g[i]][j];
             re += delta * delta;
         }
     }
     fprintf (fout, "%lf\n", re);
     fflush (fout);
     return;
}

int main(){
    int conK[5] = {5, 10, 15, 50, 100};
    int conCSD[3] = {1, 5, 10};
    
     v = (double **)malloc (sizeof (double *) * PTS);
     g = (int *)malloc(sizeof(int) * PTS);
     for (int i = 0; i < PTS; i ++){
         v[i] = (double *)malloc(sizeof(double) * DIM);
         for (int j = 0; j < DIM; j++){
             fscanf (fin, "%lf", &(v[i][j]));
         }
         g[i] = 0;
     }
    
    for (int i = 0; i < 5; i++){
        K = conK[i];
         c = (double **)malloc(sizeof(double *) * K);
         sc = (int *)malloc (sizeof(int) * K);
         for (int i = 0; i < K; i++){
             c[i] = (double *)malloc(sizeof(double) * DIM);
             sc[i] = 0;
         }
        for (int j = 0; j < 3; j ++){
            CSD = conCSD[j];
            column_selection();
            Lloyd ();
            calc_cent();
            fprintf (fout, "Z = 500 K = %d CSD = %d\n", K, CSD);
            fflush (fout);
            print_err ();
            free (cs);
            free (csc);
        }
        free (c);
        free (sc);
    }
    free (v);
    free (g);
    return 0;
}

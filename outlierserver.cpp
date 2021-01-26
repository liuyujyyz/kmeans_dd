
#include <cstdlib>
#include <cstdio>
#include "math.h"
using namespace std;

#define PTS 100000

FILE * fin;
FILE * fout;
int DIM, K, NSUB;
double *** COS;//cent_of_sub;
int * DOS;//dim_of_sub;
int ** grid; // grid representation of nodes
int * group;// group of nodes
double ** cent;// final center
int * sc;//size_of_cluster;
int * outlier;

inline double G_C_dist (int GID, int CID){// Grid to Center
       double re = 0.0;
       int st = 0;
       for (int i = 0; i < NSUB; i ++){
           for (int j = 0; j < DOS[i]; j ++){
               double delta = COS[i][grid[GID][i]][j] - cent[CID][st + j];
               re += delta * delta;
           }
           st += DOS[i];
       }
       return re;
}

inline int nearest (int GID, int n_cluster, double * d){
    int min_i, i;
    double min_d, d2;
    double * c;
        min_d = HUGE_VAL;
        min_i = rand() % n_cluster; 
        for (int i = 0; i < n_cluster; i ++){
            if (min_d > (d2 = G_C_dist(GID, i))){
                      min_d = d2;
                      min_i = i;
            }
        }
    
    if (d) *d = min_d;
    return min_i;
}

void opeq (int CID, int GID){
     int st = 0;
     for (int i = 0; i < NSUB; i ++){
         for (int j = 0; j < DOS[i]; j ++){
             cent[CID][st + j] = COS[i][grid[GID][i]][j];
         }
         st += DOS[i];
     }
     return;
}

void search (int len, int n_cluster, int n_outlier){
     double r = 
     return;
}

int main (int argc, char * argv[]){
    char s[50];
    NSUB = atoi (argv[1]);
    K = atoi(argv[2]);
    Z = atoi (arg[3]);
    
    grid = (int **) malloc (sizeof (int) * PTS);
    for (int i = 0; i < PTS; i ++){
        grid[i] = (int *)malloc (sizeof (int) * NSUB);
    }
    group = (int *)malloc (sizeof (int) * PTS);
    DOS = (int *)malloc (sizeof (int) * NSUB);
    outlier = (int *)malloc (sizeof (int) * Z);
    COS = (double ***)malloc (sizeof(double **) * NSUB);
    DIM = 0;
    for (int i = 0; i < NSUB; i ++){
        
        int pk;
        char s[50];
        sprintf (s, "%d_%d_%d%s", K, i, NSUB, argv[4]);
        fin = fopen (s, "r");
        fscanf (fin, "%d%d", &(DOS[i]), &pk);
        
        DIM += DOS[i];
        COS[i] = (double **)malloc (sizeof (double *) * pk);
        for (int j = 0; j < pk; j ++){
            COS[i][j] = (double *)malloc (sizeof (double) * DOS[i]);
            for (int l = 0; l < DOS[i]; l ++){
                fscanf (fin, "%lf", &(COS[i][j][l]));
            }
        }
        for (int j = 0; j < PTS; j ++){
            fscanf (fin, "%d", &(grid[j][i]));
        }
        fclose (fin);
    }
    
    cent = (double **)malloc (sizeof (double *) * K);
    for (int i = 0; i < K; i ++){
        cent[i] = (double *) malloc (sizeof (double) * DIM);
    }
    sc = (int *) malloc (sizeof (int) * K);
    
    //lloyd (PTS, K);
    search (PTS, K, Z);
    
    sprintf (s, "%d_%d_%s", K, NSUB, argv[4]);
    fout = fopen (s, "w");
    fprintf (fout, "%d\n", K);
    for (int i = 0; i < PTS; i ++){
        fprintf (fout, "%d ", group[i]);
    }
    fclose (fout);
    free (COS);
    free (DOS);
    free (group);
    free (cent);
    free (grid);
    free (sc);
    free (outlier);
    return 0;
}

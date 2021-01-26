#include <cstdlib>
#include <cstdio>
#include "math.h"
#include <vector>
using namespace std;

FILE * fin;
FILE * fout;

int DIM, K, Z;

double ** v;
int * g;
double ** cent;
int * sc;

#define PTS 100000

double dist (double * a, double * b){
	double re = 0.0;
	for (int i = 0; i < DIM; i ++){
		re += (a[i] - b[i])*(a[i] - b[i]);
	}
	return re;
}
 
double randf(double m)
{
	return m * rand() / (RAND_MAX - 1.0);
}
 
int nearest(int ID, int n_cluster, double *d2)
{
	int i, min_i;
	double d, min_d;
	double * c;
		min_d = HUGE_VAL;
		min_i = g[ID];
		for (i = 0; i < n_cluster; i ++) {
			if (min_d > (d = dist(cent[i], v[ID]))) {
				min_d = d; min_i = i;
			}
		}
	if (d2) *d2 = min_d;
	return min_i;
}
 
void kpp(int len, int n_cent)
{
	int i, j;
	int n_cluster;
	double sum, *d = (double *)malloc(sizeof(double) * (len + 1));
 
	double * c;
	int tmp = rand () % len;
    for (i = 0; i < DIM; i ++)	cent[0][i] = v[tmp][i];
	for (n_cluster = 1; n_cluster < n_cent; n_cluster++) {
 
		sum = 0;
		for (j = 0; j < len; j++) {
			nearest(j, n_cluster, d + j);
			sum += d[j];
		}
		sum = randf(sum);
		for (j = 0; j < len; j++) {
			if ((sum -= d[j]) > 0) continue;
			for (i = 0; i < DIM; i ++) cent[n_cluster][i] = v[j][i];
			break;
		}
	}
	for (j = 0; j < len; j++) 
        g[j] = nearest(j, n_cent, 0);
	free(d);
}
 
void lloyd(int len, int n_cluster)
{
	int i, j, min_i;
	int changed;
	kpp(len, n_cluster);
		 
	do {
		for (i = 0; i < n_cluster; i++) { 
            sc[i] = 0; 
            for(int l = 0; l < DIM; l ++) 
                    cent[i][l] = 0.0; 
        }
		for (j = 0; j < len; j++) {
			sc[g[j]] ++;
			for (int l = 0; l < DIM; l ++){
			    cent[g[j]][l] += v[j][l];
             }
		}
		for (i = 0; i < n_cluster; i++) 
        {                
            if (sc[i] != 0)
            for(int l = 0; l < DIM;l ++){
                     cent[i][l] /= sc[i];
            }
        }
		changed = 0;
		
		for (j = 0; j < len; j++) {
			min_i = nearest(j, n_cluster, 0);
			if (min_i != g[j]) {
				changed ++;
				g[j] = min_i;
			}
		}
		
	} while (changed > (len >> 10)); 
}

int main (int argc, char * argv[]){
    char s[50];
    fin = fopen (argv[1], "r");
    //fscanf (fin, "%d", &DIM);
    DIM = 30;
    v = (double **) malloc (sizeof (double *) * PTS);
    for (int i = 0; i < PTS; i ++)
        v[i] = (double *)malloc (sizeof (double) * DIM);

    g = (int *)malloc (sizeof (int) * PTS);
	for (int j = 0; j < PTS; j ++) {
        for (int i = 0; i < DIM; i ++){
            fscanf (fin, "%lf", &(v[j][i]));
        }
	}
	
    K = atoi (argv[2]);
    Z = 500;
    cent = (double **) malloc (sizeof (double *) * (K + Z));
    for (int i = 0 ; i < K + Z; i ++){
        cent[i] = (double *) malloc (sizeof (double) * DIM);
    }
    sc = (int *)malloc (sizeof (int) * (K + Z));
    
    sprintf (s, "%d_%d_%s", K, Z, argv[1]);
    fout = fopen (s, "w");
    
    fprintf (fout, "%d %d %d\n", DIM, K, Z);
    fflush (fout);
    
    lloyd (PTS, K + Z);
    for (int i = 0; i < K + Z; i ++){
        for (int j = 0; j < DIM; j ++){
            fprintf (fout, "%lf ", cent[i][j]);
            fflush (fout);
        }
        fprintf (fout, "\n");
            fflush (fout);
    }
    for (int i = 0; i < PTS; i ++){
        fprintf (fout, "%d ", g[i]);
            fflush (fout);
    }
    free (v);
    free (g);
    free (cent);
    free (sc);
    return 0;
}

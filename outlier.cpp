#include <cstdlib>
#include <cstdio>
#include "math.h"
#include <vector>
using namespace std;

FILE * fin;
FILE * fout;

int DIM = 90, K, Z = 500, pd;

double ** v;
double ** prd;
int * g;
double ** cent;
int * sc;
int outlier[500];
double outdist[500];
int min_out = 0;
int n_out = 0;

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
 
int nearest(int ID)
{
	int i, min_i;
	double d, min_d;
	{
		min_d = HUGE_VAL;
		min_i = g[ID];
		for (i = 0; i < K; i++)  {
			if (min_d > (d = dist(cent[i], v[ID]))) {
				min_d = d; min_i = i;
			}
		}
	}
    if (n_out < Z){
       outlier[n_out] = ID;
       outdist[n_out] = min_d;
       if (outdist[min_out] > min_d) min_out = n_out;
       n_out ++;
    }
    else{
         if (min_d > outdist[min_out]){
            outlier[min_out] = ID;
            outdist[min_out] = min_d;
            for (int i = 0; i < Z; i ++){
                if (outdist[i] < min_d){
                   min_out = i;
                   min_d = outdist[i];}
            }
         }
    }
	return min_i;
}

void lloyd(int len, int n_cluster)
{
	int i, j, min_i;
	int changed;
	/* assign init grouping randomly */
	//for_len p->group = j % n_cluster;
	/* or call k++ init */
	//kpp(len, n_cluster);
	
	for (int i = 0; i < n_cluster; i++){
        int tmp = rand() % len;
        for (int j = 0; j < DIM; j ++)
            cent[i][j] = v[tmp][j];
    }
    min_out = n_out = 0;
    for (int i = 0; i<len; i ++)
        g[i] = nearest(i);
    for (int i = 0; i < Z; i ++){
         g[outlier[i]] = -1;
    }
	do {
		/* group element for centroids are used as counters */
		for (i = 0; i < n_cluster; i++) { 
            sc[i] = 0; 
            for(int l = 0; l < DIM; l ++) 
                    cent[i][l] = 0.0; 
        }
		for (j = 0; j < len; j++) {
            if (g[j] == -1) continue;
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
		min_out = n_out = 0;
		/* find closest centroid of each point */
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
         //printf ("%d ", changed);
	} while (changed > (len >> 10)); /* stop when 99.9% of points are good */
}

double randG (double mu, double sigma){
       double U1, U2, W, mult;
       static double X1, X2;
       static int call = 0;
       
       if (call == 1){
                call = 0;
                return (mu + sigma * X2);
       }
       
       do {
           U1 = -1 + rand () * 2.0/ RAND_MAX;
           U2 = -1 + rand () * 2.0/ RAND_MAX;
           W = U1 * U1 + U2 * U2;
           //printf ("%lf %lf\n", U1, U2);
       }while (W > 1 || W == 0);
       
       mult = sqrt (-2 * log (W) / W);
       X1 = U1 * mult;
       X2 = U2 * mult;
       call = !call;
       return (mu + sigma * X1);
}

void RP (int ld, int d){
     double nf = 1 / sqrt (ld);
     double ** RM = (double **) malloc (sizeof (double *) * (ld));
     for (int i = 0; i < ld; i ++){
         RM[i] = (double *)malloc (sizeof (double) * (d));
     }
     for (int i = 0; i < ld; i ++){
         for (int j = 0; j < d; j ++){
             RM[i][j] = randG (0, 1) * nf;
         }
     }
     //printf ("Romdom complete\n");
     for (int i = 0; i < PTS; i ++){
         for (int j = 0; j < ld; j ++){
             for (int k = 0; k < d; k ++){
                 v[i][j] += prd[i][k] * RM[j][k];
             }
         }
     }
     free (RM);
     return;
}

int main (int argc, char * argv[]){
    char s[50];
    fin = fopen ("YPMSD.in", "r");
    //fscanf (fin, "%d", &DIM);
    pd = atoi(argv[2]);
    K = atoi (argv[1]);
    
    v = (double **) malloc (sizeof (double *) * PTS);
    prd = (double **) malloc (sizeof (double *) * PTS);
    g = (int *)malloc (sizeof (int) * PTS);
    for (int i = 0; i < PTS; i ++){
        v[i] = (double *)malloc (sizeof (double) * pd);
        prd[i] = (double *)malloc (sizeof (double) * DIM);
        g[i] = 0;
    }
	for (int j = 0; j < PTS; j ++) {
        for (int i = 0; i < DIM; i ++){
            fscanf (fin, "%lf", &(prd[j][i]));
        }
	}
	RP(pd, DIM);
	DIM = pd;	
    cent = (double **) malloc (sizeof (double *) * K);
    for (int i = 0 ; i < K; i ++){
        cent[i] = (double *) malloc (sizeof (double) * DIM);
    }
    sc = (int *)malloc (sizeof (int) * K);
    
    sprintf (s, "%d_%d_rp.in", K, pd);
    fout = fopen (s, "w");
    
    fprintf (fout, "%d %d\n", pd, K);
    fflush (fout);
    
    lloyd (PTS, K);
    for (int i = 0; i < K; i ++){
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

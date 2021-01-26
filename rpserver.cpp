#include <cstdio>
#include <cstdlib>
#include "math.h"

using namespace std;
#define PTS 100000
FILE * fin;
FILE * fout;

int K, NSUB, pd;

double ** v;
int * group;
double ** cent;
int * sc;

inline double dist (double * a, double * b){
	double re = 0.0;
	for (int i = 0; i < pd; i ++){
		re += (a[i] - b[i])*(a[i] - b[i]);
	}
	return re;
}
 
double randf(double m)
{
	return m * rand() / (RAND_MAX - 1.0);
}
 
inline int nearest(int ID, int n_cluster, double *d2)
{
	int i, min_i;
	double d, min_d;
	double * c;
#	define for_n for (c = cent[0], i = 0; i < n_cluster; i++, c++)
	for_n{
		min_d = HUGE_VAL;
		min_i = group[ID];
		for_n  {
			if (min_d > (d = dist(c, v[ID]))) {
				min_d = d; min_i = i;
			}
		}
	}
	if (d2) *d2 = min_d;
	return min_i;
}
 
void kpp(int len, int n_cent)
{
#	define for_len for (j = 0, p = pts; j < len; j++, p++)
	int i, j;
	int n_cluster;
	double sum, *d = (double *)malloc(sizeof(double) * (len + 1));
 
	double * c;
	int tmp = rand () % len;
	//printf ("%d\n", tmp);
	cent[0] = v[tmp];
	for (n_cluster = 1; n_cluster < n_cent; n_cluster++) {
        printf ("%d ", n_cluster);
		sum = 0;
		for (j = 0; j < len; j++) {
			nearest(j, n_cluster, d + j);
			sum += d[j];
		}
		sum = randf(sum);
		for (j = 0; j < len; j++) {
			if ((sum -= d[j]) > 0) continue;
			cent[n_cluster] = v[j];
			break;
		}
	}
	for (j = 0; j < len; j++) 
        group[j] = nearest(j, n_cent, 0);
	free(d);
}
 
void lloyd(int len, int n_cluster)
{
	int i, j, min_i;
	int changed;
	/* assign init grouping randomly */
	//for_len p->group = j % n_cluster;
	/* or call k++ init */
	kpp(len, n_cluster);
		
	printf ("kpp finished\n");
 
	do {
        //printf("fuck ");
		/* group element for centroids are used as counters */
		for (i = 0; i < n_cluster; i++) { 
            sc[i] = 0; 
            for(int l = 0; l < pd; l ++) 
                    cent[i][l] = 0.0; 
        }
		for (j = 0; j < len; j++) {
			sc[group[j]] ++;
			for (int l = 0; l < pd; l ++){
			    //if (pts[j].attr[l] > 0) printf ("%lf %d ", pts[j].attr[l], pts[j].group);
			    cent[group[j]][l] += v[j][l];
             }
		}
		for (i = 0; i < n_cluster; i++) 
        {
            //printf ("%d ", c->group);                    
            if (sc[i] != 0)
            for(int l = 0; l < pd;l ++){
                     cent[i][l] /= sc[i];
            }
        }
		changed = 0;
		/* find closest centroid of each point */
		for (j = 0; j < len; j++) {
			min_i = nearest(j, n_cluster, 0);
			if (min_i != group[j]) {
				changed ++;
				group[j] = min_i;
			}
		}
		printf ("%d\n", changed);
	} while (changed > (len >> 10)); /* stop when 99.9% of points are good */ 
}

int main (int argc, char * argv[]){
    NSUB = atoi (argv[1]);
    pd = atoi (argv[2]);
    K = atoi (argv[4]);
    cent = (double **) malloc (sizeof (double *) * K);
    sc = (int *) malloc (sizeof (int) * K);
    for (int i = 0; i < K; i ++){
        sc[i] = 0;
        cent[i] = (double *)malloc (sizeof (double) * pd);
    }
    v = (double **) malloc (sizeof (double *) * PTS);
    group = (int *) malloc (sizeof (int) * PTS);
    for (int i = 0; i < PTS; i ++){
        v[i] = (double *) malloc (sizeof (double) * pd);
        group[i] = 0;
        for (int j = 0; j < pd; j ++)
            v[i][j] = 0.0;
    }
    char s[50];
    for (int i = 0; i < NSUB; i ++){
        sprintf (s, "%d_%d_%d%s", pd, i, NSUB, argv[3]);
        fin = fopen (s, "r");
        int tmp;
        fscanf (fin, "%d", &tmp);
        if (tmp != pd){
                printf ("%d dirty data\n", i);
                continue;
        }
        for (int j = 0; j < PTS; j ++){
            for (int k = 0; k < pd; k ++){
                double t;
                fscanf (fin, "%lf", &t);
                v[j][k] += t;
            }
        }
        fclose (fin);
    }
    lloyd (PTS, K);
    sprintf (s, "rp%d_%d_%d_%s", K, NSUB, pd, argv[3]);
    fout = fopen (s, "w");
    for (int i = 0; i < PTS; i ++){
        fprintf (fout, "%d ", group[i]);
    }
    fclose (fout);
    return 0;
}

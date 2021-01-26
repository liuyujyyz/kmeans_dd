
typedef struct {vector<double> attr; int group;} point_t, *point;

#define PTS 10000

inline int pow (int a, int b){
       int re = 1;
       while (b-- > 0){
             re *= a;
       }
       return re;
}

inline double dist (point a, point b){
	if (a->attr.size() != b->attr.size()) return -1;
	double re = 0.0;
	for (int i = 0; i < a->attr.size(); i ++){
		re += (a->attr[i] - b->attr[i])*(a->attr[i] - b->attr[i]);
	}
	return re;
}
 
double randf(double m)
{
	return m * rand() / (RAND_MAX - 1.);
}
 
inline int nearest(point pt, point cent, int n_cluster, double *d2)
{
	int i, min_i;
	point c;
	double d, min_d;
 
#	define for_n for (c = cent, i = 0; i < n_cluster; i++, c++)
	for (c = cent, i = 0; i < n_cluster; i++, c++) {
		min_d = HUGE_VAL;
		min_i = pt->group;
		for_n {
			if (min_d > (d = dist(c, pt))) {
				min_d = d; min_i = i;
			}
		}
	}
	if (d2) *d2 = min_d;
	return min_i;
}
 
void kpp(point pts, int len, point cent, int n_cent)
{
#	define for_len for (j = 0, p = pts; j < len; j++, p++)
	int i, j;
	int n_cluster;
	double sum, *d = (double *)malloc(sizeof(double) * (len + 1));
 
	point p, c;
	int tmp = rand () % len;
	//printf ("%d\n", tmp);
	cent[0] = pts[tmp];
	printf ("h3\n");
	for (n_cluster = 1; n_cluster < n_cent; n_cluster++) {
        printf ("%d ", n_cluster);
		sum = 0;
		for (j = 0, p = pts; j < len; j++, p++) {
			nearest(p, cent, n_cluster, d + j);
			sum += d[j];
		}
		sum = randf(sum);
		for (j = 0, p = pts; j < len; j++, p++) {
			if ((sum -= d[j]) > 0) continue;
			cent[n_cluster] = pts[j];
			break;
		}
	}
	for (j = 0, p = pts; j < len; j++, p++) 
        p->group = nearest(p, cent, n_cent, 0);
	free(d);
}
 
point lloyd(point pts, int len, int n_cluster)
{
	int i, j, min_i;
	int changed;
 
	point cent = (point_t *)malloc(sizeof(point_t) * (n_cluster + 1)), p, c;
 
	/* assign init grouping randomly */
	//for_len p->group = j % n_cluster;
	//printf ("%d ", cent);
	if (cent == NULL) printf ("NULL");
	//cent[0] = pts[0];
    //printf ("h1\n");
	/* or call k++ init */
	kpp(pts, len, cent, n_cluster);
		
	printf ("kpp finished\n");
 
	do {
        //printf("fuck ");
		/* group element for centroids are used as counters */
		for (c = cent, i = 0; i < n_cluster; i++, c++) { 
            c->group = 0; 
            for(int l = 0; l < c->attr.size(); l ++) 
                    c->attr[l] = 0.0; 
        }
		for (j = 0; j < len; j++) {
			c = cent + pts[j].group;
			c->group ++;
			for (int l = 0; l < c->attr.size(); l ++){
			    //if (pts[j].attr[l] > 0) printf ("%lf %d ", pts[j].attr[l], pts[j].group);
			    c->attr[l] += pts[j].attr[l];
             }
		}
		for (c = cent, i = 0; i < n_cluster; i++, c++) 
        {
            //printf ("%d ", c->group);
            for(int l = 0; l < c->attr.size();l ++){
                    //if (c->attr[l] > 0) printf ("%lf ", c->attr[l]);
                    if (c->group != 0) c->attr[l] = (c->attr[l] + 0.0) / (c->group + 0.0);
            }
        }
 //printf("cent complete ");
		changed = 0;
		/* find closest centroid of each point */
		for (j = 0, p = pts; j < len; j++, p++) {
			min_i = nearest(p, cent, n_cluster, 0);
			if (min_i != p->group) {
				changed ++;
				p->group = min_i;
			}
		}
		printf ("%d\n", changed);
	} while (changed > (len >> 10)); /* stop when 99.9% of points are good */
 
 
    printf ("FUCKOUT\n");
	//for_n { c->group = i; }
 
	return cent;
}

double print_err(point v, int len, point c, int n_cluster){
     double re = 0.0;
     for (int i = 0; i < len; i ++){
         re += dist(&v[i], &c[v[i].group]);
     }
     /*for (int i = 0; i < n_cluster; i ++){
         for (int j = 0; j < DIM; j ++){
             printf ("%lf ", c[i].attr[j]);
         }
         printf ("\n");
     }*/
     return re;
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

void RP (point v, int ld, int d){
     double nf = 1 / sqrt (ld);
     double ** RM = (double **) malloc (sizeof (double *) * (ld + 1));
     for (int i = 0; i < ld; i ++){
         RM[i] = (double *)malloc (sizeof (double) * (d + 1));
     }
     for (int i = 0; i < ld; i ++){
         for (int j = 0; j < d; j ++){
             RM[i][j] = randG (0, 1) * nf;
         }
     }
     //printf ("Romdom complete\n");
     for (int i = 0; i < PTS; i ++){
         point_t tmp;
         tmp.attr.resize (ld);
         for (int j = 0; j < ld; j ++){
             tmp.attr[j] = 0;
             for (int k = 0; k < d; k ++){
                 tmp.attr[j] += v[i].attr[k] * RM[j][k];
             }
         }
         v[i].attr.resize (ld);
         v[i].attr = tmp.attr;
     }
     free (RM);
     return;
}
































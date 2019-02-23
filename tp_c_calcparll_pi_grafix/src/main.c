#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#include "gnuplot_i.h"

#define total 100000000

#define SLEEP_LGTH  100000000
#define NPOINTS     50
#define times 50
#define max_exp_ther 2

typedef struct {
	char *message1;
	int number;
	int a, b;
	long int time;
} seg;
double t[3000];
pthread_mutex_t lock;
int NThread = 1;
int inside = 0;

double elements[times];

int main() {

	gnuplot_ctrl * h;
	double x[max_exp_ther];
	double y[max_exp_ther];

	printf("init \n");


	/* init time */
	for (int i = 0; i < 3000; i++) {
		t[i] = i;
	}

	/*
	 * Initialize the gnuplot handle
	 */

	h = gnuplot_init();
	void plot_gnu(gnuplot_ctrl * h1, double x[NThread], double y[NThread],
			char* message);
	void *montecarlo_pi(void* ptr);

	gnuplot_cmd(h, "set multiplot ");
	gnuplot_cmd(h, "unset key          ");
	gnuplot_cmd(h, "replot                 ");
	gnuplot_cmd(h,
			"set title 'time montecarlo algorthm to find pi 50 thread in µs'");
	gnuplot_cmd(h, "set xlabel 'Number of threads'");
	gnuplot_cmd(h, "set ylabel 'time (micSec)'", "set grid");

	for (int sb = 1; sb <= 1; sb++) {
		for (int exp = 1; exp <= times; exp++) {
			printf("PI with %i Threads ------------\n", exp);
			pthread_t thread[NThread];
			seg Tarray[NThread];

			int chunkSize = total / NThread;

			int iret;
			// int bonus = total - chunk_size * NThread;  // i.e. remainder

			for (int j = 0; j < NThread; j++) {

				Tarray[j].message1 = (char*) "thread";

				Tarray[j].number = j;

				int start = j * chunkSize;
				int end = start + chunkSize - 1;
				if (j == NPOINTS - 1) {
					end = total - 1;
				}
				Tarray[j].a = start;
				Tarray[j].b = end;

				iret = pthread_create(&thread[j], NULL, montecarlo_pi,
						&Tarray[j]);

			}
			for (int i = 0; i < NThread; i++) {

				pthread_join(thread[i], NULL);

			}
			int totaltime = 0;
			for (int i = 0; i < NThread; i++) {
				totaltime = totaltime + Tarray[i].time;
			}
			elements[exp] = totaltime;

			NThread = exp;
			printf("total time %i \n", totaltime);

			printf(" pi = %f \n", ((float) inside / total) * 4);

			pthread_mutex_lock(&lock);
			inside = 0;
			pthread_mutex_unlock(&lock);

		}
		char * str[80];
		sprintf(str, "experiance :%d", sb);

		plot_gnu(h, t, elements, str);
	}

//for(int i=0;i<times;i++)
//	printf("%d \n",elements[i]);
	sleep(SLEEP_LGTH);
	return 0;

}

void plot_gnu(gnuplot_ctrl * h1, double x[NThread], double y[NThread],
		char* message) {
	/*
	 * Slopes
	 */

	gnuplot_resetplot(h1);
	gnuplot_setstyle(h1, "lines");
	gnuplot_plot_xy(h1, x, y, NThread, message);

}

void *montecarlo_pi(void* ptr) {
	struct timeval stop, start;

	float y2, x2, val;
	gettimeofday(&start, NULL);

	seg *fn;
	fn = (seg*) ptr;

	// inizialize
	for (int i = fn->a; i <= fn->b; i++) {
		// Generate random x, y in [0, 1].
		x2 = (float) (rand() / (float) (RAND_MAX));
		y2 = (float) (rand() / (float) (RAND_MAX));
		val = (x2 * x2) + (y2 * y2);
		//printf("%f\n",val );
		if (val < 1.0) {
			pthread_mutex_lock(&lock);
			inside++;
			pthread_mutex_unlock(&lock);
		}
	}
	gettimeofday(&stop, NULL);
	printf("%s[%i] %d %d time= %lu \n", fn->message1, fn->number, fn->a, fn->b,
			stop.tv_usec - start.tv_usec);
	fn->time = abs((stop.tv_usec - start.tv_usec) / 1000);
}

void *montecarlo_pi_iterative(void* ptr) {
	struct timeval stop, start;
	float y2, x2, val;
	gettimeofday(&start, NULL);
	seg *fn;
	fn = (seg*) ptr;
	// inizialize
	for (int i = fn->a; i <= fn->b; i++) {
		// Generate random x, y in [0, 1].
		x2 = (float) (rand() / (float) (RAND_MAX));
		y2 = (float) (rand() / (float) (RAND_MAX));
		val = (x2 * x2) + (y2 * y2);
		if (val < 1.0) {
			pthread_mutex_lock(&lock);
			inside++;
			pthread_mutex_unlock(&lock);
		}
	}
	gettimeofday(&stop, NULL);
	printf("%s[%i] %d %d time= %lu \n", fn->message1, fn->number, fn->a, fn->b,
			stop.tv_usec - start.tv_usec);
	fn->time = abs((stop.tv_usec - start.tv_usec) / 1000);
}


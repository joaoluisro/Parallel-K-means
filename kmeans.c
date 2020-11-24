#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#define DIM 3
int main(void) {
	int i, j, k, n, c;
	double dmin, dx;
	double *x, *mean, *sum;
	int *cluster, *count, color;
	int flips;
	omp_lock_t *locks;

	scanf("%d", &k);
	scanf("%d", &n);

	x = (double *)malloc(sizeof(double)*DIM*n);
	mean = (double *)malloc(sizeof(double)*DIM*k);
	sum= (double *)malloc(sizeof(double)*DIM*k);
	cluster = (int *)malloc(sizeof(int)*n);
	count = (int *)malloc(sizeof(int)*k);
	locks = (omp_lock_t*)malloc(sizeof(omp_lock_t)*n);

	for (i = 0; i<n; i++)
		cluster[i] = 0;
	for (i = 0; i<k; i++)
		scanf("%lf %lf %lf", mean+i*DIM, mean+i*DIM+1, mean+i*DIM+2);
	for (i = 0; i<n; i++)
		scanf("%lf %lf %lf", x+i*DIM, x+i*DIM+1, x+i*DIM+2);
	flips = n;

	#pragma omp parallel for
	for(int i =0; i < n; i++) omp_init_lock(&locks[i]);

	// condição de parada: há uma mudança nos centróides
	while (flips > 0) {

		flips = 0;

		// inicializa a quantidade de elementos em cada cluster e vetor sum
		// *facil paralelizar

		#pragma omp parallel for
		for (j = 0; j < k; j++) count[j] = 0;

		#pragma omp parallel for
		for(j = 0; j < k * DIM; j++)sum[j] = 0.0;

		// itera sobre todos os pontos
		#pragma omp parallel for shared(cluster, locks) private(color)
		for (i = 0; i < n; i++) {

			// color : cluster do ponto xi
			// dmin  : menor distância encontrada
			dmin = -1; color = cluster[i];

			// itera sobre todos os centróides
			for (c = 0; c < k; c++) {
				dx = 0.0;

				// calcula a distância entre o ponto xi e o centróide do cluster j
				for (j = 0; j < DIM; j++)
					dx +=  (x[i*DIM+j] - mean[c*DIM+j])*(x[i*DIM+j] - mean[c*DIM+j]);

				// se encontrou uma distância menor atualiza color
				if (dx < dmin || dmin == -1) {
					color = c;
					dmin = dx;
				}
			}

			// se o cluster é diferente do atual, atualiza o cluster de xi
			// conta uma alteração (flip)
			if (cluster[i] != color) {
				flips++;
				cluster[i] = color;
		  }
		}

		// count armazena o número de elementos em cada cluster j
		for (i = 0; i < n; i++) {
			count[cluster[i]]++;

			// sum armazena a soma de cada componente de x em um dado cluster
			for (j = 0; j < DIM; j++)
				sum[cluster[i]*DIM+j] += x[i*DIM+j];
			}

		// calcula a média de cada cluster
		for (i = 0; i < k; i++) {
			for (j = 0; j < DIM; j++) {
				mean[i*DIM+j] = sum[i*DIM+j]/count[i];
	  	}
		}
	}

	for (i = 0; i < k; i++) {
		for (j = 0; j < DIM; j++)
			printf("%5.2f ", mean[i*DIM+j]);
		printf("\n");
	}

	#ifdef DEBUG
	for (i = 0; i < n; i++) {
		for (j = 0; j < DIM; j++)
			printf("%5.2f ", x[i*DIM+j]);
		printf("%d\n", cluster[i]);
	}
	#endif
	return(0);
}

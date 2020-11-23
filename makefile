all:
	@gcc -fopenmp -lm kmeans_p.c	-o	kmeans_p
	@gcc -fopenmp -lm kmeans.c	-o	kmeans

clean:
	@rm	kmeans_p
	@rm	kmeans

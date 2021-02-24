#include "epidemic.h"

#include <stdio.h>
#include <float.h>
#include <stdlib.h>

#define PREC 1.0e-10

int main(int argc, char const *argv[])
{
	//create transition matrix
	transition_m* mat = create_transition_m();
	
	//try different damping factors
	/*for(double alpha = 0.1; alpha < 0.99; alpha += 0.1) {
		
		//create vectors
		vector* vect = create_vector();
		vector* res = create_vector();

		//distance between the 2 vectors
		double dist = 1.0;

		//counter of iterations
		int cpt = 0;

		//initiate the vector to (1;0;0;0;....)
		vect->val[0] = 1.0;

		//repeat pageRank while the distance between the vectors is greater than PREC
		while(dist > PREC) {
			product_matrix_vector_pageRank(mat, vect, res);
			improved_vector(vect, res, alpha);
			normalize(res);
			dist = distance(vect, res);
			copy_result(vect, res);
			cpt++;
		}
		
		printf("alpha = %f, cpt = %d\n", alpha, cpt);

		//array to identify the nodes after the sort
		int* tab_ind = malloc(NB_NODE * sizeof(int));
		int* tab_ind_tmp = malloc(NB_NODE * sizeof(int));

		//initialize index
		for(int i = 0; i < NB_NODE; i++) {
			tab_ind[i] = i;
		}

		//sort by decreasing order
		sort_merge(0, vect->nb_val - 1, vect->val, res->val, tab_ind, tab_ind_tmp);

		//output
		write_result(vect, tab_ind, alpha);
		
		

		//free objects
		free_vector(vect);
		free_vector(res);
		free(tab_ind);
		free(tab_ind_tmp);
	}*/
	
	//epidemicWithoutVaccination(mat,0.2,0.24,0.2);
	//epidemicWithRandomVaccination(mat,0.2,0.24,0.2);
	epidemicWithPageRankVaccination(mat,0.2,0.24,0.2);

	//free transition matrix
	free_transition_m(mat);
	
	return 0;
}

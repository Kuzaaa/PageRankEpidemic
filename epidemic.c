#include "epidemic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>

//creates a transition matrix dynamically by reading the graph file
transition_m* create_transition_m() {
	//allocate the matrix and the arrays
	transition_m* mat = malloc(sizeof(transition_m));
	mat->nb_val = NB_EDGE;
	mat->row = malloc(NB_EDGE * sizeof(int));
	mat->col = malloc(NB_EDGE * sizeof(int));
	mat->val = malloc(NB_EDGE * sizeof(double));

	//open data file
	FILE *fp;
	fp = fopen(DATA_PATH, "r");

	//pointer to read the line
	char* line = NULL;
	//length of the line
	size_t len = 0;
	//returned value of the getline() function
	ssize_t read;

	//delimitation character to tokenize : tabulation
	char delim[] = "\t";
	//returned value of the strtok() function
	char* p_token = NULL;
	
	//number of lines read
	int cpt = 0;
	//number of outgoing edges of a node
	int nb_link = 1;

	//read the file line by line
	while((read = getline(&line, &len, fp)) != -1) {

		//not reading the comments
		if(line[0] != '#') {
			//tokenize the line : column_number		row_number
			//and fill col and row of the transition matrix
			p_token = strtok(line, delim);
			mat->col[cpt] = atoi(p_token);
			p_token = strtok(NULL, delim);
			mat->row[cpt] = atoi(p_token);

			//if it's not the first line
			if (cpt != 0) {

				//if the edges start with the same node as the previous line
				if (mat->col[cpt] == mat->col[cpt - 1]) {

					//increment the number of outgoing edge for this node
					nb_link ++;
				}

				//the node has changed
				else {

					//fill the values of the transition matrix with the probability : 1/#outgoing_edges
					for(int i = 1; i <= nb_link; i++) {
						mat->val[cpt - i] = 1.0/nb_link;
					}

					//reset
					nb_link = 1;
				}
			}

			//increment the number of read lines
			cpt++;
		}
	}
	
	//fill the values of the transition matrix for the last node
	for(int i = 1; i <= nb_link; i++) {
		mat->val[cpt - i] = 1.0/nb_link;
	}

	//close the file
	fclose(fp);
	if (line) {
		free(line);
	}

	return mat;
}

//free the transition matrix
void free_transition_m(transition_m* mat) {
	free(mat->row);
	free(mat->col);
	free(mat->val);
	free(mat);
}

//creates a vector dynamically initialised to 0.0
vector* create_vector() {
	vector* vect = malloc(sizeof(vector));
	vect->nb_val = NB_NODE;
	vect->val = malloc(NB_NODE * sizeof(double));

	for(int i = 0; i < 	NB_NODE ; i++) {
		vect->val[i] = 0.0;
	}

	return vect;
}

//free a vector
void free_vector(vector* vect) {
	free(vect->val);
	free(vect);
}

//product between a sparse matrix (mat) and a vector (vect), stocked in a vector (res)
void product_matrix_vector_pageRank(transition_m* mat, vector* vect, vector* res) {
	for(int i = 0; i < mat->nb_val; i++) {
		res->val[mat->row[i]] += mat->val[i] * vect->val[mat->col[i]];
	}
}

//product between a sparse matrix (mat) and a vector (vect), stocked in a vector (res) for adjency matrix
void product_matrix_vector(transition_m* mat, vector* vect, vector* res) {
	for(int i = 0; i < mat->nb_val; i++) {
		if(mat->col[i] != -1) {
			res->val[mat->row[i]] += 1.0 * vect->val[mat->col[i]];
		}
	}
}

//applies the formula of the improved pageRank on the vector res : alpha * P + (1 - alpha) * G
void improved_vector_pageRank(vector* vect, vector* res, double alpha) {
	//sum the elements of vect
	double sum = 0.0;
	for(int i = 0; i < vect->nb_val; i++) {
		sum += vect->val[i];
	}

	//calculate the coefficient
	double coef = ((1.0 - alpha) / vect->nb_val) * sum;

	//applies the formula on each element of res
	for(int i = 0; i < res->nb_val ; i++) {
		res->val[i] = res->val[i] * alpha + coef;
	}
}


void improved_vector(vector* vect, vector* res, double infectionRate, double curringRate) {
	
	double coeffAdd = 1 - curringRate;
	double coeffMult = infectionRate * ( 1.0 - ( 0.5 * curringRate));
	
	for(int i = 0; i < res->nb_val ; i++) {
		res->val[i] = res->val[i] * coeffMult + coeffAdd * vect->val[i];
	}
	
}

//normalize the vector res with the max value of the vector
void normalize(vector* res) {
	double max = res->val[0];

	//find the max
	for(int i = 1; i < res->nb_val ; i++) {
		if(res->val[i] > max) {
			max = res->val[i];
		}
	}

	//divide each element by the max
	for(int i = 0 ; i < res->nb_val; i++) {
		res->val[i] /=  max;
	}
}

//calculate the euclide distance between 2 vectors
double distance(vector* vect, vector* res) {
	double dist = 0.0;

	//sum each (xi - yi)²
	for(int i = 0; i < vect->nb_val; i++) {
		dist += ((vect->val[i] - res->val[i]) * (vect->val[i] - res->val[i]));
	}

	//applies the square root on the distance
	return sqrt(dist);
}

//copy the vector res in the vector vect and set res to 0.0
void copy_result(vector* vect, vector* res) {
	for(int i = 0; i < vect->nb_val; i++) {
		vect->val[i] = res->val[i];
		res->val[i] = 0.0;
	}
}

//write the node number and the probability associed in the result file (format result_alpha.txt)
void write_result(vector* vect, int* tab_ind, double alpha) {
	char path[20];
	snprintf(path, 20, RES_PATH, alpha);
	FILE* fp;
	fp = fopen(path, "w");
	for(int i = 0; i < vect->nb_val; i++) {
		fprintf(fp, "%d, %.*f\n", tab_ind[i], DBL_DIG - 1, vect->val[i]);
	}
	fclose(fp);
}

//sort in decreasing order an array (tab) and its associated index (tab_ind) by merging method
void sort_merge(int i, int j, double* tab, double* tmp, int* tab_ind, int* tab_ind_tmp) {
	
	//stop condition
	if(j <= i) {
		return;
	}

	//index of the middle element
	int m = (i + j) / 2;

	//recursive sort of the left part
	sort_merge(i, m, tab, tmp, tab_ind, tab_ind_tmp);
	//recursive sort of the right part
	sort_merge(m + 1, j, tab,  tmp, tab_ind, tab_ind_tmp);

	//index of the left part
	int left = i;
	//index of the right part
	int right = m + 1;

	//counter
	int cpt;

	//for each element of the merged array
	for(cpt = i; cpt <= j; cpt++) {

		//end of left part
		if(left == (m + 1)) {
			tmp[cpt] = tab[right];
			tab_ind_tmp[cpt] = tab_ind[right];
			right++;
		}

		//end of right part
		else if(right == (j + 1)) {
			tmp[cpt] = tab[left];
			tab_ind_tmp[cpt] = tab_ind[left];
			left++;
		}

		//if the left element is greater than the right element
		else if(tab[left] > tab[right]) {
			tmp[cpt] = tab[left];
			tab_ind_tmp[cpt] = tab_ind[left];
			left++;
		}

		//the right element is greater than the left element
		else {
			tmp[cpt] = tab[right];
			tab_ind_tmp[cpt] = tab_ind[right];
			right++;
		}
	}

	//copy elements of tmp to tab and associated index
	for(cpt = i; cpt <= j; cpt++) {
		tab[cpt] = tmp[cpt];
		tab_ind[cpt] = tab_ind_tmp[cpt];
	}
}

//Simulate an epidemic without vaccination
void epidemicWithoutVaccination(transition_m* mat, double infectionRate, double curringRate, double infectedAtStartRate){
	
	srand(time(NULL));
	int i, j, nbInfected=0;
	
	FILE* file = NULL;
	file = fopen("infectedIndivualsWithoutVaccination.txt", "w+");
	
	if (file==NULL){
		fprintf(stderr, "Erreur : Probleme à l'ouverture du fichier.\n");
		exit(EXIT_FAILURE);
	}
		
	//create vectors
	vector* vect = create_vector();
	vector* res = create_vector();
	
	
	for(i=0 ; i<vect->nb_val ; i++){
		vect->val[i] = infectedAtStartRate;

		if(rand()/(RAND_MAX+1.0) < infectedAtStartRate){
			nbInfected++;
		}
	}
		
	fprintf(file,"   0 %d\n",nbInfected);
		
	for(i=1 ; i<=100 ; i++){
		nbInfected = 0;
		
		product_matrix_vector(mat, vect, res);
		improved_vector(vect, res, infectionRate, curringRate);
		
		for(j=0 ; j<res->nb_val ; j++){
			if(rand()/(RAND_MAX+1.0) < res->val[j]){
				nbInfected++;
			}
		}
		
		fprintf(file,"%4d %d\n",i,nbInfected);
	
	}
	
	fclose(file);
	free_vector(vect);
	free_vector(res);
	
}

//Simulate an epidemic with random vaccination
void epidemicWithRandomVaccination(transition_m* mat, double infectionRate, double curringRate, double infectedAtStartRate){
	srand(time(NULL));
	int i, j, nbInfected=0;
	
	FILE* file = NULL;
	file = fopen("infectedIndivualsWithRandomVaccination.txt", "w+");
	
	if (file==NULL){
		fprintf(stderr, "Erreur : Probleme à l'ouverture du fichier.\n");
		exit(EXIT_FAILURE);
	}
		
	//create vectors
	vector* vect = create_vector();
	vector* res = create_vector();
	
	
	for(i=0 ; i<vect->nb_val ; i++){
		vect->val[i] = infectedAtStartRate;

		//infected at start
		if(rand()/(RAND_MAX+1.0) < infectedAtStartRate){
			nbInfected++;
		}

		//vaccination at start
		if(rand()/(RAND_MAX+1.0) < infectedAtStartRate){
			for(j=0; j<mat->nb_val; j++) {
				if(mat->col[j] == i) {
					mat->col[j] = -1;
				}
			}
		}
	}
		
	fprintf(file,"   0 %d\n",nbInfected);
		
	for(i=1 ; i<=100 ; i++){
		printf("%d\n",i);
		nbInfected = 0;
		
		product_matrix_vector(mat, vect, res);
		improved_vector(vect, res, infectionRate, curringRate);
		
		for(j=0 ; j<res->nb_val ; j++){
			if(rand()/(RAND_MAX+1.0) < res->val[j]){
				nbInfected++;
			}

			if(rand()/(RAND_MAX+1.0) < infectedAtStartRate){
				for(int k=0; k<mat->nb_val; k++) {
					if(mat->col[k] == j) {
						mat->col[k] = -1;
					}
				}
			}
		}
		
		fprintf(file,"%4d %d\n",i,nbInfected);
	
	}
	
	fclose(file);
	free_vector(vect);
	free_vector(res);
}

//Simulate an epidemic with pageRank vaccination
void epidemicWithPageRankVaccination(transition_m* mat, double infectionRate, double curringRate, double infectedAtStartRate){
}


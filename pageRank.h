#ifndef PAGE_RANK_H
#define PAGE_RANK_H

//path of the graph data file
#define DATA_PATH "soc-Epinions1.txt"
//number of nodes in the graph (/!\ in this file, some nodes_id were deleted, there's 75879 nodes but the max is 75887)
#define NB_NODE 75888
//number of edges in the graph
#define NB_EDGE 508837
//path for the result file
#define RES_PATH "result_%.1f.txt"

/* 
transition matrix (sparse matrix) with :
	nb_val the number of values different of zero
	row the row index of the values
	col the column index of the values
	val the values different of zero
*/
typedef struct transition_m
{
	int nb_val;
	int* row;
	int* col;
	double* val;
} transition_m;

/*
vector with :
	nb_val the number of values in the vector
	val the values of the vector
*/
typedef struct vector
{
	int nb_val;
	double* val;
} vector;

//creates a transition matrix dynamically by reading the graph file
transition_m* create_transition_m();

//free the transition matrix
void free_transition_m(transition_m* mat);

//creates a vector dynamically initialised to 0.0
vector* create_vector();

//free a vector
void free_vector(vector* vect);

//product between a sparse matrix (mat) and a vector (vect), stocked in a vector (res)
void product_matrix_vector(transition_m* mat, vector* vect, vector* res);

//applies the formula of the improved pageRank on the vector res : alpha * P + (1 - alpha) * G
void improved_vector(vector* vect, vector* res, double alpha);

//normalize the vector res with the max value of the vector
void normalize(vector* res);

//calculate the euclide distance between 2 vectors
double distance(vector* vect, vector* res);

//copy the vector res in the vector vect and set res to 0.0
void copy_result(vector* vect, vector* res);

//write the node number and the probability associed in the result file (format result_alpha.txt)
void write_result(vector* vect, int* tab_index, double alpha);

//sort in decreasing order an array (tab) and its associated index (tab_ind) by merging method
void sort_merge(int i, int j, double* tab, double* tmp, int* tab_ind, int* tab_ind_tmp);

#endif
#include "epidemic.h"

int main(int argc, char const *argv[])
{
	//create transition matrix
	transition_m* mat = create_transition_m();
	
	epidemicWithoutVaccination(mat,0.2,0.24,0.2);
	epidemicWithRandomVaccination(mat,0.2,0.24,0.2);
	epidemicWithPageRankVaccination(mat,0.2,0.24,0.2);

	//free transition matrix
	free_transition_m(mat);
	
	return 0;
}

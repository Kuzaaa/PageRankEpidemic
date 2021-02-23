#include <stdio.h>
#include <stdlib.h>
 
#include "graph.h"
#include "pageRank.h"
#include "exploitResult.h"
 
int main() {
  
	int startPage;
	float dampingFactor;
	Graph* g = initGraphFromFile();

	printf("Sur quel page démarrer ? (entre 0 et %d)\n",g->nbPages);
	scanf(" %d",&startPage);
	if(startPage < 0 || startPage >= g->nbPages){
		printf("Erreur, mauvais nombre d'itérations, choisissez une page entre 0 et %d\n",g->nbPages);
		return 0;
	}
	
	printf("Quel facteur d'amortissement ? (entre 0 et 1)\n");
	scanf(" %f",&dampingFactor);
	if(dampingFactor < 0 || dampingFactor > 1){
		printf("Erreur, mauvais facteur d'amortissement, choisissez un facteur d'amortissement entre 0 et 1\n");
		return 0;
	}

	pageRank(g,startPage,dampingFactor);
	printValue(g);
	sort_and_print(g);
  
	free(g);
  
  return 0;
}

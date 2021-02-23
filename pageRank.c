#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pageRank.h"

//Algorithme PageRank amelioré
void pageRank(Graph* g, int startPage, float dampingFactor){
	
	int i,it = 0;
	double max = 1;
	double tmp,scalar;
	double norm = 0, oldNorm;
	double tol = 0.0000000001;
	
	g->value[startPage] = 1;
	scalar = 1.0;
	
	//On calcul tmp ici afin d'éviter de le calculer plusieurs fois par clics
	tmp = (1-dampingFactor) / g->nbPages;
	
	//Jusqu'à ce que les différences des normes du vecteur de l'itération k et k-1 soient inférieurs ou égale à notre tolérance
	do{ 	
		it++;
		//Pour tous les liens
		for(i=0;i<g->nbLinks;i++){
			
			//Si la chance d'être à la page source à ce clic différent de 0
			if(g->value[g->raw[i]]!=0){
			
				//On met à jour la chance d'être à la page de destinaton pour le clic suivant
				g->newValue[g->columns[i]] += g->value[g->raw[i]] * g->probaLinks[i];
				
			}
		}
		
		max = 0;
		//Pour toutes les pages
		for(i=0;i<g->nbPages;i++){
		
			//On met de nouveau à jour la chance d'être à la page de destinaton pour le clic suivant
			//Grâce au facteur d'amortissement
			g->newValue[i] = dampingFactor * g->newValue[i] + (tmp * scalar);
			
			//On calcul le max
			if(max < g->newValue[i]){
				max = g->newValue[i];
			}
			
		}
		
		oldNorm = norm;
		norm = 0;
		scalar = 0;
		
		//On évite de faire la division plusieurs fois
		max = 1/max;
		
		//Pour toutes les pages
		for(i=0;i<g->nbPages;i++){
			
			//On normalise en divisant par le max
			g->newValue[i] = g->newValue[i] * max;
			
			//On calcul la norme du vecteur
			norm += fabs(g->newValue[i] - g->value[i]);
			
			//On passe au clic suivant
			g->value[i] = g->newValue[i];
			g->newValue[i] = 0;
			scalar += g->value[i];
			
		}
	}while(fabs((norm - oldNorm)) >= tol);
	
	printf("itération nécessaire pour la tolérance %.10f: %d\n",tol,it);
	
}

//Remplit un fichier avec les résultats
void printValue(Graph* g){
	int i;
	FILE* file = NULL;
	file = fopen("Result.txt", "w+");
	
	if (file==NULL){
		fprintf(stderr, "Erreur : Probleme à l'ouverture du fichier.\n");
		exit(EXIT_FAILURE);
    }
	
	//Pour toutes les pages
	for(i=0;i<g->nbPages;i++){
		
		//On écrit l'identifiant de la page et la chance de se trouver sur cette dernière
		fprintf(file,"%d %.15f\n",i,g->value[i]);
	}
	fclose(file);
	
	printf("Fichier Result.txt mis à jour.\n");

}

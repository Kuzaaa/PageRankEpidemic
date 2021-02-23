#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#define PATH "Test.txt"

//Initialise un graphe à partir d'un fichier
Graph* initGraphFromFile()
{
	int i;
	int srcPage, destPage;
	FILE* file = NULL;
	char raw[100];
	Graph* graph;
	
	graph = malloc(sizeof(Graph));
	graph->nbLinks = 0;
	
	//Initialisation de nbLinks & nbPages à travers un parcours dans le fichier
    file = fopen(PATH, "r+");
	
	if (file==NULL){
		fprintf(stderr, "Erreur : Probleme à l'ouverture du fichier.\n");
		exit(EXIT_FAILURE);
    }
    
    i = 0;
    while ( fgets( raw, 100, file) != NULL ){
		sscanf(raw,"%d %d", &srcPage, &destPage);
		if(i<srcPage){
			i = srcPage;
		}
		if(i<destPage){
			i = destPage;
		}
		
		//Le nombre de lignes du fichier correspondra à notre nombre de liens
        graph->nbLinks++;
    }
    
    //La valeur la plus grande de notre fichier correspondra à notre nombre de pages
    graph->nbPages = i+1;
    
    fclose(file); 
	
    //Allocation des différents tableaux    
    graph->value=(double*)malloc(graph->nbPages * sizeof(double));
    
    graph->newValue=(double*)malloc(graph->nbPages * sizeof(double));
    
    graph->probaPages=(double*)malloc(graph->nbPages * sizeof(double));
    
    graph->probaLinks=(double*)malloc(graph->nbLinks * sizeof(double));
    
    graph->raw=(int*)malloc(graph->nbLinks * sizeof(int));
    
    graph->columns=(int*)malloc(graph->nbLinks * sizeof(int));
    
    for(i=0;i<graph->nbPages;i++){
		graph->value[i] = 0;
		graph->newValue[i] = 0;
		graph->probaPages[i] = 0;
	}
	
    //Remplissage des tableaux
    file = fopen(PATH, "r+");
	
	if (file==NULL){
		fprintf(stderr, "Erreur : Probleme à l'ouverture du fichier.\n");
		exit(EXIT_FAILURE);
    }
    
    i = 0;
    while ( fgets( raw, 100, file) != NULL ){
		sscanf(raw,"%d %d", &srcPage, &destPage);
		
		graph->raw[i] = srcPage;
		graph->columns[i] = destPage;
		graph->probaPages[srcPage] += 1;
			
		i++;
    }
    
    for(i=0;i<graph->nbPages;i++){
		graph->probaPages[i] = 1/graph->probaPages[i];
	}
    
    for(i=0;i<graph->nbLinks;i++){
		graph->probaLinks[i]=graph->probaPages[graph->raw[i]];
	}
    
    fclose(file); 
    
	return graph;
}

//Libère l'espace mémoire pris par un graphe
void freeGraph(Graph* graph){
	   
	free(graph->value);
	free(graph->newValue);
	free(graph->probaPages);
	free(graph->probaLinks);
	free(graph->raw);
	free(graph->columns);

}

//Affiche sur la console tous les liens d'un graphe
void printGraph(Graph* graph){

	int i;
	for(i=0;i<graph->nbLinks;i++){
		printf("%d %d %f\n",graph->raw[i],graph->columns[i],graph->probaLinks[i]);
	}
}

#ifndef __GRAPH__H__
#define __GRAPH__H__
	
	typedef struct Graph Graph;
	struct Graph
	{
		int nbPages;				//Nombre de pages
		int nbLinks;				//Nombre de liens
		double* value;				//Valeur Xn-1
		double* newValue;			//Valeur Xn
		double* probaLinks;			//Probalité de passer à la page suivante par lien
		double* probaPages;			//Probabilité de passer à la page suivante par page
		int* raw;					//Ligne de la matrice de transition
		int* columns;				//Colonne de la matrice de transition
	};

	Graph* initGraphFromFile();		//Initialise un graphe à partir d'un fichier
	void freeGraph(Graph* g);		//Libère l'espace mémoire pris par un graphe
	void printGraph(Graph* g);		//Affiche sur la console tous les liens d'un graphe

#endif

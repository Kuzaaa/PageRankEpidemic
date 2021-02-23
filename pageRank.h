#ifndef __PAGERANK__H__
#define __PAGERANK__H__

#include "graph.h"

		void pageRank(Graph* g, int startPage, float dampingFactor);		//Algorithme PageRank amelioré
		void printValue(Graph* g);											//Remplit un fichier avec les résultats

#endif


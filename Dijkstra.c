/* 
 * Implementação Dijkstra - Teoria de Grafos - Projeto de Programação - PARTE 1
 * Arthur Ricardo de Paula Oliveira – TIA: 31824617
 * Pedro Savoy Prieto - TIA: 31831583
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <stdbool.h>

#define MAX 100
#define INF (~(0x1<<31)) 

/* 
 * Estrutura de dados para representar grafos
 */
typedef struct a{ /* Celula de uma lista de arestas */
	int    nome;
	int distancia; /* variável para guardar distância da aresta */
	struct a *prox;
}Aresta;

typedef struct vert{  /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele */
	int nome;
	Aresta *prim;
}Vertice;

/*
 * Declaracoes das funcoes para manipulacao de grafos 
 */
void criaGrafo(Vertice **G, int ordem);
void destroiGrafo(Vertice **G, int ordem);
int  acrescentaAresta(Vertice G[], int ordem, int v1, int v2, int distancia);
void imprimeGrafo(Vertice G[], int ordem);
void dijkstra(Vertice G[], int ordemG, int casa);
int getMinimaDistancia(int dist[], bool visitados[], int ordemG);
int getDistancia(Vertice G[], int start, int end);
int imprimeSolucao(int dist[], int ordemG, int path[]);
void imprimeCaminho(int path[], int vertice);

/*
* Função para achar a distância entre dois Vertices do Grafo
*/
int getDistancia(Vertice G[], int start, int end){

	Aresta *a;
	int distancia;
		
	a = G[start].prim; 
	
	int i;
	for(i = 0; a != NULL; a = a->prox, i++){ /* esse for percorre lista 'a' até achar o a = end; e retorna distancia de a */
		if(a->nome == end){
			return(a->distancia);
		}
	}
}

/*
* Função para achar a menor distância entre dois Vertices do Grafo
*/
int getMinimaDistancia(int dist[], bool visitados[], int ordemG){

	int min = INT_MAX, min_index;
	int v;
	for(v = 0; v < ordemG; v++)
	if(visitados[v] == false && dist[v] <= min)
		min = dist[v], min_index = v;
	return min_index;

}

/*
* Funcao para imprimir o caminho do Vertice casa (15) para o Vertice escolhido
*/
void imprimeCaminho(int path[], int vertice){
	
	if(path[vertice]==-1)
		return;
	
	imprimeCaminho(path, path[vertice]);
	
	printf("%d ", vertice);
	
}

/*
* Funcao para imprimir a menor distância do Vertice casa (15) para o Vertice escolhido
*/
int imprimeSolucao(int dist[], int ordemG, int path[]){

	printf("Escolha um vertice entre 1 e 15 para ver a menor distancia desde a casa (1):\n\n");
	int vertice = -1;
	
	while(vertice != 0){
		
		printf("\nEscolha Vertice entre 1 e 15 (digite 0 para encerrar o programa): ");
		scanf("%d", &vertice);
		if(vertice >= 1 && vertice <= 32){
			printf("Vertice \t Distancia \t Caminho\n");
			printf("%d \t\t %d \t\t", vertice, dist[vertice]);
			imprimeCaminho(path, vertice);
			printf("\n");
		}
		else if(vertice == 0){
			break;
		}
		else{
			printf("\nNumero do vertice invalido, tente novamente.\n");
		}
		
	}
}

/*
* IMPLEMENTAÇÃO DO DJIKSTRA
*/
void dijkstra(Vertice G[], int ordemG, int casa){
	
	int dist[ordemG]; /* vetor para guardar a menor distancia do vertice casa (15) para o vertice [i] */
	bool visitados[ordemG]; /* vetor para guardar se vertice já foi visitado (bool) */
	int path[ordemG]; /* vetor para guardar caminhos */
	
	int i, count, v;
	
	for(i = 0; i < ordemG; i++){
		dist[i] = INT_MAX, visitados[i] = false; path[0] = -1;
	}
	
	dist[casa] = 0;
	
	for(count = 0; count < ordemG - 1; count++){
		int u = getMinimaDistancia(dist, visitados, ordemG);
		visitados[u] = true;
		for(v = 0; v < ordemG; v++){
			/* if para atualizar a minima distancia (dist[v]) se a nova distancia achada é menor */
			if(!visitados[v] && getDistancia(G, u, v) && dist[u] != INT_MAX && dist[u] + getDistancia(G, u, v) < dist[v]){
				path[v] = u;
				dist[v] = dist[u] + getDistancia(G, u, v); 
			}
		}
	}
	
	printf("\n");
	
	int g;
	for(g = 1; g < ordemG-1; g++){
		printf("%d, ", g);
		printf("%d, ", dist[g]);
		printf("%d", path[g]);
		printf("\n");
	}
	
	printf("\n");

	imprimeSolucao(dist, ordemG, path);

}

 
/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 * e, inicilamente, sem nenhuma aresta 
 */
 void criaGrafo(Vertice **G, int ordem){
	int i;
	*G= (Vertice*) malloc(sizeof(Vertice)*ordem); /* Alcacao dinamica de um vetor de vertices */
	
	for(i=0; i<ordem; i++){
		(*G)[i].nome= i;
		(*G)[i].prim= NULL;    /* Cada vertice sem nenua aresta incidente */
	}
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vertice **G, int ordem){
	int i;
    Aresta *a, *n;
    
	for(i=0; i<ordem; i++){ /* Remove lista de adjacencia de cada vertice */
	    a= (*G)[i].prim;
        while (a!= NULL){
              n= a->prox;
              free(a);
              a= n;
        }
	}
    free(*G);
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j
 *   serao criadas, na estrutura de dados, arestas (i,j) e (j,i)
 */
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2, int distancia){
    Aresta * A1, *A2;
    
	if (v1<0 || v1 >= ordem) /* Testo se vertices sao validos */
	   return 0;
	if (v2<0 || v2 >= ordem)
	   return 0;
	
	/* Acrescento aresta na lista do vertice v1 */
	A1= (Aresta *) malloc(sizeof(Aresta));
	A1->nome= v2;
	A1->prox= G[v1].prim;
	A1->distancia = distancia; /* aqui adicionei a distancia */
	G[v1].prim= A1;

	/* Acrescento aresta na lista do vertice v2 se v2 != v1 */
	if (v1 == v2) return 1;
	
	A2= (Aresta *) malloc(sizeof(Aresta));
	A2->nome= v1;
	A2->prox= G[v2].prim;
	A2->distancia = distancia; /* aqui eu adicionei a distancia */
	G[v2].prim= A2;
		
	return 1;
}


/*  
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vertice G[], int ordem){
	int i;
	Aresta *aux;

	printf("\nOrdem:   %d",ordem - 2);
	printf("\nLista de Adjacencia:\n");


	for (i=1; i<ordem-1; i++){
		if(i<10){
			printf("\n    V%d:  ", i);
			aux= G[i].prim;
			for( ; aux != NULL; aux= aux->prox)
				printf("%3d", aux->nome);
		}
		else{
			printf("\n    V%d: ", i);
			aux= G[i].prim;
			for( ; aux != NULL; aux= aux->prox)
				printf("%3d", aux->nome);
		}
	}
	printf("\n\n");
}


/*
 * Programinha simples para testar a representacao de grafo
 */
int main(int argc, char *argv[]) {

	Vertice *G;
	int ordemG = 17;
		
	criaGrafo(&G, ordemG);

	acrescentaAresta(G,ordemG,1,2,170);
	acrescentaAresta(G,ordemG,1,3,16);
	acrescentaAresta(G,ordemG,1,4,25);
	acrescentaAresta(G,ordemG,2,8,350);
	acrescentaAresta(G,ordemG,3,8,80);
	acrescentaAresta(G,ordemG,3,7,60);
	acrescentaAresta(G,ordemG,3,6,40);
	acrescentaAresta(G,ordemG,4,5,40);
	acrescentaAresta(G,ordemG,8,15,20);
	acrescentaAresta(G,ordemG,7,9,80);
	acrescentaAresta(G,ordemG,6,10,60);
	acrescentaAresta(G,ordemG,5,10,75);
	acrescentaAresta(G,ordemG,5,12,20);
	acrescentaAresta(G,ordemG,10,9,80);
	acrescentaAresta(G,ordemG,10,11,120);
	acrescentaAresta(G,ordemG,10,13,100);
	acrescentaAresta(G,ordemG,11,8,50);
	acrescentaAresta(G,ordemG,12,11,30);
	acrescentaAresta(G,ordemG,11,14,120);
	acrescentaAresta(G,ordemG,9,13,70);
	acrescentaAresta(G,ordemG,13,14,80);
	acrescentaAresta(G,ordemG,14,15,70);
	acrescentaAresta(G,ordemG,13,15,70);
	acrescentaAresta(G,ordemG,1,16,10000000);


	imprimeGrafo(G, ordemG);
	
	int casa = 1;

	dijkstra(G, ordemG, casa);
    
	destroiGrafo(&G, ordemG);

    system("PAUSE");

	return 0;
}

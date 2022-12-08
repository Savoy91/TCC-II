/* 
 * Projeto de Programaï¿½ï¿½o - Comï¿½rcio no Meu Bairro - PARTE 2
 * Arthur Ricardo de Paula Oliveira ï¿½ TIA: 31824617
 * Pedro Savoy Prieto - TIA: 31831583
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <stdbool.h>

#define MAX 100
#define INF 99999
#define ORDEMMG 16
/* 
 * Estrutura de dados para representar grafos
 */
typedef struct a{ /* Celula de uma lista de arestas */
	int    nome;
	int distancia; /* variï¿½vel para guardar distï¿½ncia da aresta */
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
int getMinimaDistancia(int dist[], bool visitados[], int ordemG);
int getDistancia(Vertice G[], int start, int end);

/*
 * Novas funï¿½ï¿½es para a parte 2
 */
void floyd_warshall(Vertice G[], int ordemG, int length, int conjunto_locais[], int fact, int casa);
void printFloydWarshallArray(int dist[][ORDEMMG]);
void printPathArray(int v, int u);
void acha_permutacao(int *arr, int start, int end, int fact, int casa, int dist[][ORDEMMG]);
void troca(int* arr, int a, int b); 
void calcula_permutacao( int* a, int len, int casa, int dist[][ORDEMMG], int fact);
void print_resultados(int casa, int length, int fact);
void printPath(int v, int u);

/*
 * Declaraï¿½ï¿½o de algumas variï¿½veis globais para a parte 2
 */
extern int dist_total[10000] = {0};
extern int dist_cada[10000] = {0};
extern int sequencia_locais[10000] = {0};
extern int path[1000][1000] = {0};

/*
* Funï¿½ï¿½o para achar a distï¿½ncia entre dois Vertices do Grafo
*/
int getDistancia(Vertice G[], int start, int end){

	Aresta *a;
	int distancia;
		
	a = G[start].prim; 
	
	int i;
	for(i = 0; a != NULL; a = a->prox, i++){ /* esse for percorre lista 'a' atï¿½ achar o a = end; e retorna distancia de a */
		if(a->nome == end){
			return(a->distancia);
		}
	}
	
	return(0);
}

/*
* Funï¿½ï¿½o para achar a menor distï¿½ncia entre dois Vertices do Grafo
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
* Funï¿½ï¿½o para achar a menor distï¿½ncia entre todo par de Vertices do Grafo
* Nesta funï¿½ï¿½o utilizaremos dois vetores 2D, um para a menor distancia entre dois pares de vertice (dist[][])
* E o outro vetor serï¿½ para guardar o caminho entre dois pares de vertice (path[][])
*/
void floyd_warshall(Vertice G[], int ordemG, int length, int conjunto_locais[], int fact, int casa){

    int dist[ordemG][ordemG], i, j, k;
 
    for (i = 1; i < ordemG; i++){
        for (j = 1; j < ordemG; j++){
        	if(i == j){
        		dist[i][j] = 0;
        		path[i][j] = 0;
			}
        	else if(getDistancia(G,i,j) != '\0'){
			
            	dist[i][j] = getDistancia(G, i, j);
            	path[i][j] = i;
        	}
        	else{
        		dist[i][j] = INF;
        		path[i][j] = -1;
			}
		}
	}
 
    for (k = 1; k < ordemG; k++)
    {
        
        for (i = 1; i < ordemG; i++)
        {
            for (j = 1; j < ordemG; j++)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j]){
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[k][j];
            	}
            }
        }
    }
    
    acha_permutacao(conjunto_locais, 0, length, fact, casa, dist);

    printFloydWarshallArray(dist);
    
    printPathArray(1,1);
    
}

/*
* Funï¿½ï¿½o auxiliar para as permutacoes
*/
void troca(int* arr, int a, int b) { 

  int tmp = arr[a]; 
  arr[a] = arr[b]; 
  arr[b] = tmp; 
  
} 

/*
* Funï¿½ï¿½o principal para achar as permutaï¿½ï¿½es do conjunto de locais que devem ser visitados
*/
void acha_permutacao(int *arr, int start, int end, int fact, int casa, int dist[][ORDEMMG]) { 
	
	int i;
	
	/*toda vez que o programa acha uma nova permutacao ele envia essa permutacao para o calcula_permutacao();*/
    if(start == end){ 
    	calcula_permutacao(arr, end, casa, dist, fact);
    	return; 
    } 
 
    acha_permutacao(arr, start + 1, end, fact, casa, dist); /* inicia uma nova permutacao no proximo elemento */ 
    
    /* permuta os proximos elementos recursivamente*/ 
    for(i = start + 1; i < end; i++)  
    { 
        if( arr[start] == arr[i] ) continue; 
  
	    troca(arr, start, i); 
	 
	    acha_permutacao(arr, start + 1, end, fact, casa, dist); 
 
	    troca(arr, start, i);
 
    } 
    
} 

/*
* Funï¿½ï¿½o principal para calcular a distancia da nova permutacao.
* Tambem nesta funcao sera atualizadas as variaveis globais,
* caso a distancia total achada ï¿½ menor do que as permutacoes anteriores.
*/
void calcula_permutacao( int* a, int len, int casa, int dist[][ORDEMMG], int fact) {
	
	/*variï¿½veis auxiliares para o calculo*/	
	int dist_total_aux = 0;
	int dist_cada_aux[len];
	int aux, aux2, aux3;
	
	/*Calcularemos primerio a distancia total e a distancia vertice a vertice da nova permutaï¿½ï¿½o (a[])*/
	aux2 = a[0];	
	aux = dist[casa][aux2];
	dist_total_aux += aux;
	dist_cada_aux[0] = aux;
	
	int i;
	for(i = 0; i < len - 1; i++){
		aux2 = a[i];
		aux3 = a[i+1];
		aux = dist[aux2][aux3];
		dist_total_aux += aux;
		dist_cada_aux[i+1] = aux;
	}
	
	aux2 = a[len-1];
	aux = dist[aux2][casa];
	dist_total_aux += aux;
	dist_cada_aux[len] = aux;
	
	/* Se essa for a primeira permutacao sendo calculada, iremos atualizar as variaveis globais*/
	if(dist_total[0] == 0){
		int y;
		for(y = 0; y <= len; y++){
			dist_cada[y] = dist_cada_aux[y];
			if(y < len){
				sequencia_locais[y] = a[y];
			}
		}
	}
	
	/*Caso nï¿½o for a primeira permutacao, o programa compara se a distancia total da nova permuatacao
	* ï¿½ menor do que as outras (dist_total_aux < dist_total[l]), e se for atualiza as variaveis globais */
	else{
		int l;
		for(l = 0; l < fact; l++){
			if(dist_total[l] != 0 && dist_total_aux < dist_total[l]){
				int k;
				for(k = 0; k <= len; k++){
					dist_cada[k] = dist_cada_aux[k];
					if(k < len){
						sequencia_locais[k] = a[k];
					}
				}
			}
		}
	}
	
	/*Inclui distancia total achada no proximo indice livre (dist_total[i] = 0)*/
	int x;
	for(x = 0; x < fact; x++){
		if(dist_total[x] == 0)
			break;
	}
	dist_total[x] = dist_total_aux; 
} 

/*
* Funï¿½ï¿½o para imprimir os resultados achados.
*/
void print_resultados(int casa, int length, int fact){
	
	printf("Resultado final para o caminho mais curto, tendo como inicio e fim vertice = %d, a partir do conjunto:\n[", casa);
	int x;
	for(x = 0; x < length - 1; x++){
		printf("%d, ", sequencia_locais[x]);
	}
	printf("%d]\n", sequencia_locais[length - 1]);

	/*Imprime a sequï¿½ncia de locais que deve ser visitada de forma a 
	* obter o passeio mais curto. */
	printf("\nSequencia de Locais para obter o passeio mais curto: ");
	int i;
	printf("[%d,", casa);
	for(i = 0; i <= length - 1; i++){
		printf(" %d, ", sequencia_locais[i]);
	}
	printf("%d]\n", casa);

	/*Imprime a distï¿½ncia percorrida de um local da sequï¿½ncia 
	* obtida como resultado atï¿½ o local seguinte da sequï¿½ncia. */
	printf("\nDistancia de um local da sequencia ate o outro:\n");
	int j;
	printf("%d -> %d = %d\n", casa, sequencia_locais[0], dist_cada[0]);
	for(j = 0; j <= length - 2; j++){
		printf("%d -> %d = %d\n", sequencia_locais[j], sequencia_locais[j+1], dist_cada[j]);
	}
	printf("%d -> %d = %d\n", sequencia_locais[length-1], casa, dist_cada[length]);

	/* a sequï¿½ncia de vï¿½rtices que deve ser visitada no 
	* passeio obtido como resultado; a partir da funcao printPath(); */
	int aux2, aux3;
	aux2 = sequencia_locais[0];
	printf("\nSequencia de vertices que devem ser visitados:\n[%d, ", casa);
	printPath(casa, aux2);
	int z;
	for(z = 0; z < length - 1; z++){
		aux2 = sequencia_locais[z];
		printf("%d, ", aux2);
		aux3 = sequencia_locais[z+1];
		printPath(aux2, aux3);
	}
	aux2 = sequencia_locais[length-1];
	printf("%d, ", aux2);
	printPath(aux2, casa);
	printf("%d]\n", casa);
	
	/* Imprime a distï¿½ncia total percorrida */
	int k;
	int aux = 0;
	printf("\nDistancia Total: ");
	for(k = 0; k < fact; k++){
		if(k == 0){
			aux = dist_total[k];
		}
		else if(dist_total[k] < aux){
			aux = dist_total[k];
		}
	}
	printf("%d\n\n", aux);
	
	printf("\n\n");

}

/* 
* Funcao recursiva para imprimir a sequencia o menor caminho.
*/
void printPath(int v, int u){
	
	if(path[v][u] == v){
		return;
	}
	printPath(v, path[v][u]);
	printf("%d, ", path[v][u]);
}

/*
* Funï¿½ï¿½o auxiliar para imprimir a matriz gerada pelo algoritmo Floyd-Warshall, no caso ela nï¿½o estï¿½ sendo chamada.
*/
void printFloydWarshallArray(int dist[][ORDEMMG])
{
    printf ("Matriz para menor distância entre todos os pares de vertice:\n\n");
    int i, j;
    for (i = 1; i < ORDEMMG; i++)
    {
        for (j = 1; j < ORDEMMG; j++)
        {
            if (dist[i][j] == INF)
                printf("%2s", "INF");
            else
                printf ("%5d", dist[i][j]);
        }
        printf("\n");
        
    }
    printf("\n");
}

void printPathArray(int v, int u){
	printf ("Matriz para menor caminho entre todos os pares de vertice:\n\n");
    int i, j;
    for (i = 1; i < ORDEMMG; i++)
    {
        for (j = 1; j < ORDEMMG; j++)
        {
            if (path[i][j] == INF)
                printf("%2s", "INF");
            else
                printf ("%5d", path[i][j]);
        }
        printf("\n");
        
    }
    printf("\n");

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

	printf("\nOrdem:   %d",ordem - 1);
	printf("\nLista de Adjacencia:\n");


	for (i=1; i<ordem; i++){
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

int main(int argc, char *argv[]) {

	Vertice *G;
	int ordemG = 16;
		
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

	imprimeGrafo(G, ordemG);
	
	int casa = 1; /*pode trocar o vertice que o caminho irï¿½ iniciar e acabar, o valor pode ser entre 1 - 15*/

	int conjunto_locais [] = {15}; /*aqui o senhor pode trocar os vertices que devem ser visitados, o valor pode ser entre 1 - 15*/
	
	int length = sizeof(conjunto_locais)/sizeof(conjunto_locais[0]); /*variï¿½vel para guardar o tamanho do vetor de conjunto de locais*/
	
	unsigned long long fact = 1; /*calcular a quantidade de permutacoes possiveis, length fatorial (length!)*/
	int i;
	for (i = 1; i <= length; ++i) {
        fact *= i;
    }
	
	floyd_warshall(G, ordemG, length, conjunto_locais, fact, casa); /*chama funcao para executar o algoritmo Floyd-Warshall*/
	
	print_resultados(casa, length, fact); /*chama funcao para imprimir os resultados finais*/
	
	destroiGrafo(&G, ordemG);

    system("PAUSE");

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct vertice{
    char estado[2];
    char nome[50];

};
typedef struct vertice Vertice;
typedef struct grafo Grafo;
struct grafo{
    int Nvertices;
    int nArestas;
    //Ponteiro para a lista de adjacencias
};
struct no{
    int peso;//
    Vertice v;
    link prox;
};
typedef struct no *link;//Ponteiro para o proximo no da lista de adj
typedef link *ListaDeAdj;//Vetor com a lista de Adj
void insereVerticeOrdenado(Vertice v,Grafo *grafo,ListaDeAdj lista){//Supoe que a lista ja esteja alocada na memoria depois de ler o cabecalho do arquivo
    link novo;
    int i = 0;
    novo->v = v;
    novo->prox = NULL;
    novo->peso = -1;
    if(grafo->Nvertices == 0){
        lista[0] = novo;
        grafo->Nvertices++;
        return; 
    }
    while(strcmp(v.nome,lista[i]->v.nome) > 0){
        i++;
    }
    //estou na posição que eu devo ser colocado preciso me colocar e shiftar a lista
    

}
link addNo(Vertice v,link prox,int peso){//Adiciona um novo no em um vetor da Lista, recebe o vertice que eu sou e pra quem eu aponto,Me retorna
    link a = malloc(sizeof(struct no));
    a->prox = prox;
    a->v = v;
    a->peso = peso;
    return a;   
}
struct teste{
    int a;
    int b;
};
int main(int argc, char const *argv[])
{
    struct teste teste1;
    teste1.a = 0;
    struct teste teste2;
    teste2.a = 1;
    struct teste *lista;
    lista[0] = teste1;
    lista[1] = teste2;
    for(int i = 0;i < 2;i++){
        printf("%d",lista[i].a);
    }
}

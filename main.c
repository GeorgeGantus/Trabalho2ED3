#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct vertice{
    char estado[2];
    char nome[50];

};
typedef struct no **ListaDeAdj;//Vetor com a lista de Adj
typedef struct vertice Vertice;
typedef struct grafo Grafo;
struct grafo{
    int Nvertices;
    int nArestas;
    ListaDeAdj adj;
};
typedef struct no *link;
struct no{
    int peso;//
    Vertice v;
    link prox;
};//Ponteiro para o proximo no da lista de adj
link addNo(Vertice v,link prox,int peso){//Cria um novo no em um vetor da Lista, recebe o vertice que eu sou e pra quem eu aponto,Me retorna
    link a = malloc(sizeof(struct no));
    a->prox = prox;
    a->v = v;
    a->peso = peso;
    return a;   
}
void insereVerticeOrdenado(Vertice v,Grafo *grafo,ListaDeAdj lista){//Supoe que a lista ja esteja alocada na memoria depois de ler o cabecalho do arquivo
    link novo = addNo(v,NULL,-1);
    link aux1;
    link aux2;
    int i = 0;
    if(grafo->Nvertices == 0){
        lista[0] = novo;
        grafo->Nvertices++;
        return; 
    }
    while(i < grafo->Nvertices && strcmp(v.nome,lista[i]->v.nome) > 0){
        i++;
    }
       //estou na posição que eu devo ser colocado preciso me colocar e shiftar a lista
    grafo->Nvertices++;
    if(i == grafo->Nvertices-1){
        lista[i] = novo;
        return;
    }
    aux1 = lista[i];
    lista[i] = novo;
    i++;
    for(i;i < grafo->Nvertices;i++){//faz o shift no vetor
        aux2 = lista[i];
        lista[i] = aux1;
        aux1 = aux2;
    }
}
int buscaRetIndice(ListaDeAdj lista,Vertice v,int nVertices){//nVertives = nAtual de vertices Busca Binária
  int inf = 0;
     int sup = nVertices-1;
     int meio;
     int aux;
     while (inf <= sup)
     {
          meio = (inf + sup)/2;
          aux = strcmp(v.nome,lista[meio]->v.nome);
          if (aux == 0){
               return meio;
          }if (aux < 0){
               sup = meio-1;
          }else
               inf = meio+1;
     }
     return -1; 
}

int main(int argc, char const *argv[])
{
   Vertice v1;
   strcpy(v1.nome,"acidade");
   Vertice v2;
   strcpy(v2.nome,"bcidade");
   Vertice v3;
   strcpy(v3.nome,"dcidade");
   Vertice v4;
   strcpy(v4.nome,"ccidade");
   Grafo grafo;
   grafo.Nvertices = 0;
   grafo.adj = malloc(4*sizeof(link));
   insereVerticeOrdenado(v1,&grafo,grafo.adj);
   insereVerticeOrdenado(v2,&grafo,grafo.adj);
   insereVerticeOrdenado(v3,&grafo,grafo.adj);
   insereVerticeOrdenado(v4,&grafo,grafo.adj);
   int d = buscaRetIndice(grafo.adj,v3,grafo.Nvertices);
   printf("%d",d);
}

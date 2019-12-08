#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct vertice{
    char estado[2];
    char nome[50];

};
struct Cabecalho{
    char status; // consistência do arquivo de dados, '0' => arquivo de dados está inconsistente ou ‘1’=> arquivo de dados está consistente.
    int numeroVertices; // indica o número de cidades diferentes que estão armazenadas no arquivo de dados, 0 => nenhum registro.
    int numeroArestas; // indica o número de registros que estão armazenados no arquivo de dados, 0 => nenhum registro.
    char dataUltimaCompactacao[11]; // formato DD/MM/AAAA, ‘##/##/####’ foi carregado com dados pela primeira vez
};
typedef struct Cabecalho Cabecalho;
/* Struct que define o registro de dados*/
/* Tamanho fixo de registro de 85 bytes.*/
struct Dados{
    int RRN; // numero relativo de registro.
    char estadoOrigem[3]; // tamanho fixo 2 bytes, o 3 é o "\0".
    char estadoDestino[3]; // tamanho fixo 2 bytes, o 3 é o "\0".
    int distancia; // tamanho fixo 4 bytes.
    char cidadeOrigem[100]; // tamanho variavel, delimitador entre campos pipe "|", lixo = "#"
    char cidadeDestino[100]; // tamanho variavel, delimitador entre campos pipe "|".
    char tempoViagem[100]; // tamanho variavel, delimitador entre campos pipe "|".
};
typedef struct Dados Dados;
void lerAtePipe(char *campo,FILE *file){
    char c;
    int contador = 0;
    fread(&c,1,1,file);
    while(c != '|'){ /* lê chars até encontrar um | */
        campo[contador] = c; /*escrevemos o valor do campo ate ser encontrado o |*/
        contador ++;
        fread(&c,1,1,file);
    }
    campo[contador] = 0;

}
struct Cabecalho leCabecalho(FILE *file){ /*Função que retorna uma struct cabeçalho depois de lida*/
    struct Cabecalho r;
    r.dataUltimaCompactacao[10] = '\0';
    rewind(file); // volta a posicao do cursor para o começo do arquivo
    fread(&r.status,1,1,file);
    fread(&r.numeroVertices,4,1,file);
    fread(&r.numeroArestas,4,1,file);
    fread(&r.dataUltimaCompactacao,10,1,file);
    rewind(file); /*Volta a posição do cursor para o inicio do arquivo.*/
    return r;
}
void lerRegistro(FILE *file,struct Dados *r){ /*Função responsável por ler um registro e retorna um ponteiro para seu endereço*/
    struct Dados registro;
    fread(registro.estadoOrigem,2,1,file);
    fread(registro.estadoDestino,2,1,file);
    registro.estadoDestino[2] = registro.estadoOrigem[2] = '\0';
    fread(&registro.distancia,4,1,file);
    //Campos de tamanhoVariavel ------>Ler ate o delimitador char a char
    lerAtePipe(registro.cidadeOrigem,file);
    lerAtePipe(registro.cidadeDestino,file);
    lerAtePipe(registro.tempoViagem,file);
    *r = registro;
}
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
    for(i = i;i < grafo->Nvertices;i++){//faz o shift no vetor
        aux2 = lista[i];
        lista[i] = aux1;
        aux1 = aux2;
    }
}
void insereArestaOrdenado(ListaDeAdj lista,int indice,Vertice v,int peso){//Precisa Testar
    link vetor = lista[indice];
    link aux;
    if(vetor->prox == NULL){
        vetor->prox = addNo(v,NULL,peso);
        return;
    }
    while (vetor->prox != NULL && strcmp(v.nome,vetor->prox->v.nome) > 0){
        vetor = vetor->prox;
    }//O proximo cara é maior ou igual a mim ou null
    if(vetor->prox == NULL){
        vetor->prox = addNo(v,NULL,peso);
        return;
    }
    //Se chegou ate aqui o proximo cara é o cara com igual maior ou igual a mim
    aux = vetor->prox;
    vetor->prox = addNo(v,aux,peso);
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
link MenorAresta(ListaDeAdj lista,int indice){
    link aux = lista[indice];
    link ret = lista[indice];
    int pesoMinimo;
    if(aux->prox == NULL){
        return ret;
    }
    aux = aux->prox;
    pesoMinimo = aux->peso;
    while(aux != NULL){
        if(aux->peso < pesoMinimo){
            ret = aux;
        }
        aux=aux->prox;
    }
    return ret;
}
void GerarGrafo(Grafo *grafo,char *nomeArquivo){
    Dados aux;
    Vertice v1;
    Vertice v2;
    int indice;
    int rrn = 0;
    int contador = 0;
    FILE *file = fopen(nomeArquivo,"rb");
    Cabecalho c = leCabecalho(file);
    grafo->nArestas = 0;
    grafo->Nvertices = 0;
    if(c.status == '0'){
        printf("Falha no carregamento do arquivo.");//ARQUIVO A ZOADO PRINTAR ERRO
        return;
    }
    grafo->adj = calloc(c.numeroVertices,sizeof(link));
    fseek(file,19,SEEK_SET);
    while (rrn < c.numeroArestas){
        lerRegistro(file,&aux);
        if(aux.estadoOrigem[0] == '*'){
            contador++;
            fseek(file,(contador*85)+19,SEEK_SET);
            continue;
        }
        strcpy(v1.nome,aux.cidadeOrigem);
        strncpy(v1.estado,aux.estadoOrigem,2);
        strcpy(v2.nome,aux.cidadeDestino);
        strncpy(v2.estado,aux.estadoDestino,2);
        if(buscaRetIndice(grafo->adj,v1,grafo->Nvertices) == -1){
            insereVerticeOrdenado(v1,grafo,grafo->adj);
        }
        if(buscaRetIndice(grafo->adj,v2,grafo->Nvertices) == -1){
            insereVerticeOrdenado(v2,grafo,grafo->adj);
        }
        indice = buscaRetIndice(grafo->adj,v1,grafo->Nvertices);
        insereArestaOrdenado(grafo->adj,indice,v2,aux.distancia);//insere no vertice 1 a aresta que vai ate a 2 com oseu respectivo peso
        indice = buscaRetIndice(grafo->adj,v2,grafo->Nvertices);
        insereArestaOrdenado(grafo->adj,indice,v1,aux.distancia);
        rrn++;
        contador++;
        fseek(file,(contador*85)+19,SEEK_SET);
    }
    fclose(file);

}
int main(int argc, char const *argv[])
{
    /*
   Vertice v1;
   strcpy(v1.nome,"acidade");
   Vertice v2;
   strcpy(v2.nome,"bcidade");
   Vertice v3;
   strcpy(v3.nome,"dcidade");
   Vertice v4;
   strcpy(v4.nome,"ccidade");
    Vertice v5;
   strcpy(v5.nome,"TESTE");
   Grafo grafo;
   grafo.Nvertices = 0;
   grafo.adj = malloc(4*sizeof(link));
   insereVerticeOrdenado(v1,&grafo,grafo.adj);
   insereVerticeOrdenado(v2,&grafo,grafo.adj);
   insereVerticeOrdenado(v3,&grafo,grafo.adj);
   insereVerticeOrdenado(v4,&grafo,grafo.adj);
   int d = buscaRetIndice(grafo.adj,v5,grafo.Nvertices);
   //Ate aqui ta fino
   printf("%d",d);
    */
   Grafo grafo;
   char nome[50] =  "caso03.bin";
   GerarGrafo(&grafo,nome);
}

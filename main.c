#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define INFINITO 999999
struct vertice{
    char estado[3];
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
int menorValor(int a,int b){
    if(b < 0){
        return a;
    }
    if(a < b){
        return a;
    }
    return b;

}
struct Aresta{
    int indiceOrigem;
    int indiceDestino;
    int peso;
    char tempoViagem[100];
};
typedef struct Aresta pseudoAresta;
void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

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
    char tempoviagem[50];
    Vertice v;
    link prox;
};//Ponteiro para o proximo no da lista de adj
link addNo(Vertice v,link prox,int peso,char *tempoviagem){//Cria um novo no em um vetor da Lista, recebe o vertice que eu sou e pra quem eu aponto,Me retorna
    link a = malloc(sizeof(struct no));
    a->prox = prox;
    a->v = v;
    a->peso = peso;
    strncpy(a->tempoviagem,tempoviagem,50);
    return a;
}
void insereVerticeOrdenado(Vertice v,Grafo *grafo,ListaDeAdj lista){//Supoe que a lista ja esteja alocada na memoria depois de ler o cabecalho do arquivo
    link novo = addNo(v,NULL,-1,"0");
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
void insereArestaOrdenado(ListaDeAdj lista,int indice,Vertice v,int peso,char *tempoviagem){//Precisa Testar
    link vetor = lista[indice];
    link aux;
    if(vetor->prox == NULL){
        vetor->prox = addNo(v,NULL,peso,tempoviagem);
        return;
    }
    while (vetor->prox != NULL && strcmp(v.nome,vetor->prox->v.nome) > 0){
        vetor = vetor->prox;
    }//O proximo cara é maior ou igual a mim ou null
    if(vetor->prox == NULL){
        vetor->prox = addNo(v,NULL,peso,tempoviagem);
        return;
    }
    //Se chegou ate aqui o proximo cara é o cara com igual maior ou igual a mim
    aux = vetor->prox;
    vetor->prox = addNo(v,aux,peso,tempoviagem);
}
int buscaRetIndice(ListaDeAdj lista,Vertice vert,int nVertices){//nVertives = nAtual de vertices Busca Binária
  int inf = 0;
     int sup = nVertices-1;
     int meio;
     int aux;
     while (inf <= sup)
     {
          meio = (inf + sup)/2;
          aux = strcmp(vert.nome,lista[meio]->v.nome);
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
int GerarGrafo(Grafo *grafo,char *nomeArquivo){
    Dados aux;
    Vertice v1;
    Vertice v2;
    int indice;
    int rrn = 0;
    int contador = 0;
    FILE *file = fopen(nomeArquivo,"rb");
    if(!file){
        printf("Falha no carregamento do arquivo.\n");
        return 0; // caso tenha ocorrido algum erro com o arquivo, retorna 0
    }
    Cabecalho c = leCabecalho(file);
    grafo->nArestas = 0;
    grafo->Nvertices = 0;
    if(c.status == '0'){
        printf("Falha no carregamento do arquivo.\n");//ARQUIVO A ZOADO PRINTAR ERRO
        return 0;
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
        strncpy(v1.estado,aux.estadoOrigem,3);
        strcpy(v2.nome,aux.cidadeDestino);
        strncpy(v2.estado,aux.estadoDestino,3);
        if(buscaRetIndice(grafo->adj,v1,grafo->Nvertices) == -1){
            insereVerticeOrdenado(v1,grafo,grafo->adj);
        }
        if(buscaRetIndice(grafo->adj,v2,grafo->Nvertices) == -1){
            insereVerticeOrdenado(v2,grafo,grafo->adj);
        }
        indice = buscaRetIndice(grafo->adj,v1,grafo->Nvertices);
        insereArestaOrdenado(grafo->adj,indice,v2,aux.distancia,aux.tempoViagem);//insere no vertice 1 a aresta que vai ate a 2 com oseu respectivo peso
        indice = buscaRetIndice(grafo->adj,v2,grafo->Nvertices);
        insereArestaOrdenado(grafo->adj,indice,v1,aux.distancia,aux.tempoViagem);
        rrn++;
        contador++;
        fseek(file,(contador*85)+19,SEEK_SET);
    }
    fclose(file);
    return 1;
}

void imprimeGrafo(Grafo grafo){
   link aux;
   int i=0;
   for (i = 0; i < grafo.Nvertices; i++){
    aux = grafo.adj[i];
    while(aux!=NULL){
        if(aux->peso == -1){
            printf("%s %s",aux->v.nome,aux->v.estado);
            aux=aux->prox;
            continue;
        }
        if(!(strncmp(aux->tempoviagem,"\0",1))){
            printf(" %s %s %d",aux->v.nome,aux->v.estado,aux->peso);
        }else{
            printf(" %s %s %d %s",aux->v.nome,aux->v.estado,aux->peso,aux->tempoviagem);
        }
        aux=aux->prox;
    }
    printf("\n");
   }
}
int verificaAbertos(int *abertos,int tamanho){
    for(int i = 0;i<tamanho;i++){
        if(abertos[i] == 1){
            return 1;
        }
    }
    return 0;
}
int **djkstra(char* nomeCidadeOrigem,Grafo grafo){
    Vertice aux;
    int menor = INFINITO;
    int indiceaux;
    int indiceatual;
    link arestaAux;
    strcpy(aux.nome,nomeCidadeOrigem);
    int* antecessores = (int*) calloc(grafo.Nvertices, sizeof(int));
    int* distancias = (int*) calloc(grafo.Nvertices, sizeof(int));
    int abertos[grafo.Nvertices];
    for(int i = 0;i < grafo.Nvertices;i++){
        distancias[i] = INFINITO;
        antecessores[i] = -1;
        abertos[i] = 1;
    }
    int posicao = buscaRetIndice(grafo.adj,aux,grafo.Nvertices);
    if(posicao == -1){
        printf("Cidade inexistente.");
    }
    antecessores[posicao] = posicao;
    distancias[posicao] = 0;
    while(verificaAbertos(abertos,grafo.Nvertices)){
        menor = INFINITO;
        for(int i = 0;i<grafo.Nvertices;i++){
            if(distancias[i] < menor && abertos[i] == 1){
                menor = distancias[i];
                indiceatual = i;
            }
        }
            abertos[indiceatual] = 0;
            arestaAux = grafo.adj[indiceatual];
            if(arestaAux->prox == NULL){//Se tiver um no isolado entra em loop infinito
                continue;
            }
            arestaAux = arestaAux->prox;

            while(arestaAux != NULL){
                indiceaux = buscaRetIndice(grafo.adj,arestaAux->v,grafo.Nvertices);
                if(distancias[indiceatual]+arestaAux->peso < distancias[indiceaux]){
                    distancias[indiceaux] = distancias[indiceatual]+arestaAux->peso;
                    antecessores[indiceaux] = indiceatual;
                }
            arestaAux = arestaAux->prox;
            }
    }
    //retorna o vetor distancias e o vetor antecessores
    int **retorno = calloc(2,sizeof(int*));
    retorno[0] = distancias;
    retorno[1] = antecessores;
    return retorno;
}

void exibirDijkstra(char* valorCampo,Grafo grafo){
    Vertice aux;
    Vertice auxAnt;
    strcpy(aux.nome,valorCampo);
    int Origem = buscaRetIndice(grafo.adj,aux,grafo.Nvertices);
    strcpy(aux.estado, grafo.adj[Origem]->v.estado);
    int **resultado = djkstra(valorCampo,grafo);
    int *distancias = resultado[0]; // ditancias 1, distancia da origem ate a posicao 1
    int *posAnterior = resultado[1]; // posicao 1 tem o cara anterior a posicao 1
    for (int i = 0; i < grafo.Nvertices; i++){
        if(i == Origem){
            continue;
        }
        strcpy(auxAnt.nome, grafo.adj[posAnterior[i]]->v.nome);
        strcpy(auxAnt.estado, grafo.adj[posAnterior[i]]->v.estado);
//        printf("%d\n", distancias[i]);
        printf("%s %s %s %s %d %s %s\n",aux.nome,aux.estado,grafo.adj[i]->v.nome,grafo.adj[i]->v.estado,distancias[i],auxAnt.nome,auxAnt.estado);

    }
    //printf("%s %s ",aux.nome,aux.estado);

}


int verificaVertices(int *arvore,int tamanho){
    for(int i = 0;i < tamanho;i++){
        if(arvore[i] == -1){
            return 0;
        }
    }
    return 1;
}
pseudoAresta *prim(char* nomeCidadeOrigem,Grafo grafo){
    Vertice aux;
    strcpy(aux.nome,nomeCidadeOrigem);
    link arestas = NULL;
    int arvore[grafo.Nvertices];
    int j =0;
    int indice = buscaRetIndice(grafo.adj,aux,grafo.Nvertices);
    if(indice == -1){
        printf("Cidade inexistente.");
        return NULL;
    }
    pseudoAresta auxiliar;
    pseudoAresta *retorno = calloc(grafo.Nvertices,sizeof(pseudoAresta));
    for(int i =0;i < grafo.Nvertices;i++){
        arvore[i] = -1;
    }
    arvore[indice] = 1;
    while(!verificaVertices(arvore,grafo.Nvertices)){
        auxiliar.peso = INFINITO;
        for(int i =0;i<= grafo.Nvertices;i++){
            if(arvore[i] == 1){//estou num vertice que eu tenho que analizar
                arestas = grafo.adj[i]->prox;
                while(arestas != NULL){
                    if(arestas->peso < auxiliar.peso && arvore[buscaRetIndice(grafo.adj,arestas->v,grafo.Nvertices)] != 1){
                        auxiliar.peso = arestas->peso;
                        auxiliar.indiceDestino = buscaRetIndice(grafo.adj,arestas->v,grafo.Nvertices);
                        auxiliar.indiceOrigem = i;
                        strcpy(auxiliar.tempoViagem,arestas->tempoviagem);
                    }
                    arestas=arestas->prox;
                }
            }
        }
        retorno[j].indiceOrigem = auxiliar.indiceOrigem;
        retorno[j].indiceDestino = auxiliar.indiceDestino;
        retorno[j].peso = auxiliar.peso;
        strcpy(retorno[j].tempoViagem,auxiliar.tempoViagem);
        arvore[retorno[j].indiceDestino] = 1;
        j++;

    }
    retorno[j].peso = -1;
    return retorno;
}

void exibirPrim(char* valorCampo,Grafo grafo){
    pseudoAresta *retorno = prim(valorCampo,grafo);
    int i = 0;
    while(retorno[i].peso != -1){
        i++;
    }
    Grafo prim;
    prim.Nvertices = 0;
    prim.nArestas = 0;
    prim.adj = calloc(i,sizeof(link));//talvez colocar -1
    int j = 0;
    while(retorno[j].peso != -1){
        Vertice v1;
        Vertice v2;
        strcpy(v1.nome,grafo.adj[retorno[j].indiceOrigem]->v.nome);
        strcpy(v2.nome,grafo.adj[retorno[j].indiceDestino]->v.nome);
        strcpy(v1.estado,grafo.adj[retorno[j].indiceOrigem]->v.estado);
        strcpy(v2.estado,grafo.adj[retorno[j].indiceDestino]->v.estado);
        if(buscaRetIndice(prim.adj,v1,prim.Nvertices) == -1){

            insereVerticeOrdenado(v1,&prim,prim.adj);
        }
        if(buscaRetIndice(prim.adj,v2,prim.Nvertices) == -1){
            insereVerticeOrdenado(v2,&prim,prim.adj);
        }
        insereArestaOrdenado(prim.adj,buscaRetIndice(prim.adj,v1,prim.Nvertices),v2,retorno[j].peso,retorno[j].tempoViagem);
        j++;
    }

    imprimeGrafo(prim);
}

int main(int argc, char const *argv[]){
  Grafo grafo;
  char nomeArq[50];
  char cidadeOrigm[50];
  char valorCampo[50];
  int opt; /* Opção do switch case.*/
  scanf("%d", &opt);  /* Digita o número da opção a ser considerada */
  switch (opt){
    case 9:
        scanf("%s",nomeArq); /* Digita o nome do arquivo csv.*/
        GerarGrafo(&grafo,nomeArq);
        imprimeGrafo(grafo);
        break;
    case 10:
        scanf("%s",nomeArq); /* Digita o nome do arquivo csv.*/
        scanf("%s",cidadeOrigm);
        scan_quote_string(valorCampo);
        if(GerarGrafo(&grafo,nomeArq)){
            exibirDijkstra(valorCampo,grafo);
        }
    break;
    case 11:
        scanf("%s",nomeArq); /* Digita o nome do arquivo csv.*/
        scanf("%s",cidadeOrigm);
        scan_quote_string(valorCampo);
        GerarGrafo(&grafo,nomeArq);
        exibirPrim(valorCampo,grafo);

        //exibirPrim(valorCampo,grafo);
    break;

  }
   return 1;
}

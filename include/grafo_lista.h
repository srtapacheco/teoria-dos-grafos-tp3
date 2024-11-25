#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

typedef struct No {
    int vertice;
    double capacidade;
    double fluxo;
    struct No *prox;
} No;

typedef struct {
    No **listaAdj;
    int numVertices;
} GrafoLista;

// Funções para manipulação do grafo com lista de adjacência
GrafoLista *criarGrafoLista(int numVertices);
void liberarGrafoLista(GrafoLista *grafoLista);
void adicionarArestaLista(GrafoLista *grafoLista, int u, int v, double capacidade, int direcionado);

// Funções adicionais para o grafo residual
void adicionarArestaListaResidual(GrafoLista *grafoLista, int u, int v, double capacidade, int original);

#endif // GRAFO_LISTA_H

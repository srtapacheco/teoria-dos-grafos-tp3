#include "../include/grafo_lista.h"
#include <stdlib.h>

GrafoLista *criarGrafoLista(int numVertices) {
    GrafoLista *grafo = (GrafoLista *)malloc(sizeof(GrafoLista));
    grafo->numVertices = numVertices;
    grafo->listaAdj = (No **)malloc(numVertices * sizeof(No *));
    for (int i = 0; i < numVertices; i++) {
        grafo->listaAdj[i] = NULL;
    }
    return grafo;
}

void liberarGrafoLista(GrafoLista *grafoLista) {
    for (int i = 0; i < grafoLista->numVertices; i++) {
        No *atual = grafoLista->listaAdj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(grafoLista->listaAdj);
    free(grafoLista);
}

// Função para adicionar uma aresta na lista de adjacência
void adicionarArestaLista(GrafoLista *grafoLista, int u, int v, double peso, int direcionado) {
    // Adiciona a aresta u -> v
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->vertice = v;
    novoNo->peso = peso;
    novoNo->prox = grafoLista->listaAdj[u];
    grafoLista->listaAdj[u] = novoNo;

    // Se o grafo não for direcionado, adiciona a aresta v -> u
    if (!direcionado) {
        novoNo = (No *)malloc(sizeof(No));
        novoNo->vertice = u;
        novoNo->peso = peso;
        novoNo->prox = grafoLista->listaAdj[v];
        grafoLista->listaAdj[v] = novoNo;
    }
}
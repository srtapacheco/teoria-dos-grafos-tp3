#include "../include/grafo_matriz.h"
#include <stdlib.h>

GrafoMatriz *criarGrafoMatriz(int numVertices) {
    GrafoMatriz *grafo = (GrafoMatriz *)malloc(sizeof(GrafoMatriz));
    grafo->numVertices = numVertices;

    // Alocar a matriz de capacidade
    grafo->capacidade = (double **)malloc(numVertices * sizeof(double *));
    for (int i = 0; i < numVertices; i++) {
        grafo->capacidade[i] = (double *)calloc(numVertices, sizeof(double));
    }

    // Alocar a matriz de fluxo
    grafo->fluxo = (double **)malloc(numVertices * sizeof(double *));
    for (int i = 0; i < numVertices; i++) {
        grafo->fluxo[i] = (double *)calloc(numVertices, sizeof(double));
    }

    return grafo;
}

void liberarGrafoMatriz(GrafoMatriz *grafoMatriz) {
    for (int i = 0; i < grafoMatriz->numVertices; i++) {
        free(grafoMatriz->capacidade[i]);
        free(grafoMatriz->fluxo[i]);
    }
    free(grafoMatriz->capacidade);
    free(grafoMatriz->fluxo);
    free(grafoMatriz);
}

// Função para adicionar uma aresta na matriz de adjacência
void adicionarArestaMatriz(GrafoMatriz *grafoMatriz, int u, int v, double capacidade, int direcionado) {
    grafoMatriz->capacidade[u][v] = capacidade;
    grafoMatriz->fluxo[u][v] = 0.0;

    if (!direcionado) {
        grafoMatriz->capacidade[v][u] = capacidade;
        grafoMatriz->fluxo[v][u] = 0.0;
    }
}

// Função para adicionar uma aresta no grafo residual
void adicionarArestaMatrizResidual(GrafoMatriz *grafoMatriz, int u, int v, double capacidade, int original) {
    grafoMatriz->capacidade[u][v] = capacidade;
    grafoMatriz->fluxo[u][v] = (double)original; // 1 para aresta original, 0 para reversa
}

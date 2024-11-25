#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

typedef struct {
    double **capacidade;
    double **fluxo;
    int numVertices;
} GrafoMatriz;

// Funções para manipulação do grafo com matriz de adjacência
GrafoMatriz *criarGrafoMatriz(int numVertices);
void liberarGrafoMatriz(GrafoMatriz *grafoMatriz);
void adicionarArestaMatriz(GrafoMatriz *grafoMatriz, int u, int v, double capacidade, int direcionado);

#endif // GRAFO_MATRIZ_H

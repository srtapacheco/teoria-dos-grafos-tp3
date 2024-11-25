#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/grafo.h"
#include "include/grafo.h"

#define MAX_NOME_ARQUIVO_SAIDA 256
#define SUFIXO_ESTUDO_CASO "-estudoCaso4.txt"
#define TAMANHO_SUFIXO_ESTUDO_CASO (sizeof(SUFIXO_ESTUDO_CASO) - 1) // Exclui o terminador nulo

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <arquivo_entrada> <tipo_representacao>\n", argv[0]);
        printf("Tipos de representação:\n");
        printf("1 - Lista de Adjacência\n");
        printf("2 - Matriz de Adjacência\n");
        return 1;
    }

    const char *nomeArquivoEntrada = argv[1];
    int tipoRepresentacao = atoi(argv[2]);

    TipoRepresentacao tipo;
    if (tipoRepresentacao == 1) {
        tipo = LISTA_ADJACENCIA;
    } else if (tipoRepresentacao == 2) {
        tipo = MATRIZ_ADJACENCIA;
    } else {
        printf("Tipo de representação inválido. Use 1 para Lista de Adjacência ou 2 para Matriz de Adjacência.\n");
        return 1;
    }

    // Calcula o tamanho máximo para o nome base do arquivo
    const size_t max_base_nome_arquivo = MAX_NOME_ARQUIVO_SAIDA - TAMANHO_SUFIXO_ESTUDO_CASO - 1; // -1 para o terminador nulo

    // Declara baseNomeArquivo com o tamanho adequado
    char baseNomeArquivo[max_base_nome_arquivo + 1]; // +1 para o terminador nulo
    // Copia o nome do arquivo de entrada para baseNomeArquivo, garantindo que não exceda o tamanho máximo
    strncpy(baseNomeArquivo, nomeArquivoEntrada, max_base_nome_arquivo);
    baseNomeArquivo[max_base_nome_arquivo] = '\0';  // Garante terminação nula

    // Remove a extensão, se presente
    char *ponto = strrchr(baseNomeArquivo, '.');
    if (ponto != NULL) {
        *ponto = '\0';
    }

    // Lê o grafo do arquivo
    FILE *arquivo = fopen(nomeArquivoEntrada, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivoEntrada);
        return 1;
    }

    int numVertices;
    if (fscanf(arquivo, "%d", &numVertices) != 1) {
        printf("Erro ao ler o número de vértices do arquivo %s.\n", nomeArquivoEntrada);
        fclose(arquivo);
        return 1;
    }

    if (numVertices <= 0) {
        printf("Erro: Número de vértices inválido (%d) lido do arquivo %s.\n", numVertices, nomeArquivoEntrada);
        fclose(arquivo);
        return 1;
    }

    // Cria o grafo direcionado
    Grafo *grafo = criarGrafo(numVertices, tipo, 1); // '1' indica grafo direcionado
    if (!grafo) {
        printf("Erro ao criar o grafo com %d vértices.\n", numVertices);
        fclose(arquivo);
        return 1;
    }

    // Lê as arestas do arquivo
    lerArestas(grafo, nomeArquivoEntrada, 1); // '1' indica grafo direcionado

    fclose(arquivo);

    // Executa o estudo de caso
    int numExecucoes = 10;
    double tempoTotal = 0.0;
    double fluxoMaximo = 0.0;

    for (int i = 0; i < numExecucoes; i++) {
        // Faz uma cópia do grafo original para cada execução
        Grafo *grafoCopia = criarGrafo(numVertices, tipo, 1);
        if (tipo == LISTA_ADJACENCIA) {
            for (int u = 0; u < grafo->numVertices; u++) {
                No *atual = grafo->grafoLista->listaAdj[u];
                while (atual != NULL) {
                    adicionarArestaGrafo(grafoCopia, u, atual->vertice, atual->capacidade, 1);
                    atual = atual->prox;
                }
            }
        } else if (tipo == MATRIZ_ADJACENCIA) {
            for (int u = 0; u < grafo->numVertices; u++) {
                for (int v = 0; v < grafo->numVertices; v++) {
                    double capacidade = grafo->grafoMatriz->capacidade[u][v];
                    if (capacidade > 0) {
                        adicionarArestaGrafo(grafoCopia, u, v, capacidade, 1);
                    }
                }
            }
        }

        clock_t inicio = clock();
        double fluxo = fordFulkerson(grafoCopia, 0, 1); // Vértices 1 e 2 (índices 0 e 1)
        clock_t fim = clock();

        double tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0; // Tempo em ms
        tempoTotal += tempoExecucao;
        fluxoMaximo = fluxo; // O fluxo máximo será o mesmo em todas as execuções

        liberarGrafo(grafoCopia);
    }

    double tempoMedio = tempoTotal / numExecucoes;

    printf("Fluxo máximo entre os vértices 1 e 2: %.2f\n", fluxoMaximo);
    printf("Tempo médio de execução do algoritmo: %.6f ms\n", tempoMedio);

    // Gera o nome do arquivo de saída, seguindo o padrão dos outros estudos de caso
    char nomeArquivoSaida[MAX_NOME_ARQUIVO_SAIDA];
    snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s%s", baseNomeArquivo, SUFIXO_ESTUDO_CASO);

    FILE *arquivoSaida = fopen(nomeArquivoSaida, "w");
    if (!arquivoSaida) {
        printf("Erro ao criar o arquivo %s.\n", nomeArquivoSaida);
    } else {
        fprintf(arquivoSaida, "Fluxo máximo entre os vértices 1 e 2: %.2f\n", fluxoMaximo);
        fprintf(arquivoSaida, "Tempo médio de execução do algoritmo: %.6f ms\n", tempoMedio);
        fclose(arquivoSaida);
        printf("Resultados do estudo de caso salvos em %s\n", nomeArquivoSaida);
    }

    liberarGrafo(grafo);

    return 0;
}

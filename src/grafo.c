#include "../include/grafo.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

Grafo *criarGrafo(int numVertices, TipoRepresentacao tipo, int direcionado) {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->numArestas = 0;
    grafo->tipo = tipo;
    grafo->direcionado = direcionado;

    if (tipo == MATRIZ_ADJACENCIA) {
        grafo->grafoMatriz = criarGrafoMatriz(numVertices);
        grafo->grafoLista = NULL;
    } else if (tipo == LISTA_ADJACENCIA) {
        grafo->grafoLista = criarGrafoLista(numVertices);
        grafo->grafoMatriz = NULL;
    }
    return grafo;
}

// Função para criar um grafo vazio (usado para a árvore DFS)
Grafo *criarGrafoVazio(int numVertices, TipoRepresentacao tipo) {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->numArestas = 0;
    grafo->tipo = tipo;
    grafo->direcionado = 0; // Por padrão, grafos vazios são não direcionados

    if (tipo == MATRIZ_ADJACENCIA) {
        grafo->grafoMatriz = criarGrafoMatriz(numVertices);
        grafo->grafoLista = NULL;
    } else if (tipo == LISTA_ADJACENCIA) {
        grafo->grafoLista = criarGrafoLista(numVertices);
        grafo->grafoMatriz = NULL;
    }
    return grafo;
}


// Função para ler as arestas do arquivo e preencher o grafo
void lerArestas(Grafo *grafo, const char *nomeArquivo, int direcionado) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int numVertices;
    fscanf(arquivo, "%d", &numVertices); // Ignora o número de vértices, pois já foi lido

    int u, v;
    double peso;
    while (fscanf(arquivo, "%d %d %lf", &u, &v, &peso) == 3) {
        // Ajusta os índices para 0-based
        u -= 1;
        v -= 1;

        // Adiciona a aresta
        adicionarArestaGrafo(grafo, u, v, peso, direcionado);
    }

    fclose(arquivo);
}



void liberarGrafo(Grafo *grafo) {
    if (grafo->tipo == MATRIZ_ADJACENCIA) {
        liberarGrafoMatriz(grafo->grafoMatriz);
    } else if (grafo->tipo == LISTA_ADJACENCIA) {
        liberarGrafoLista(grafo->grafoLista);
    }
    free(grafo);
}

// Função auxiliar para calcular estatísticas dos graus
EstatisticasGrafo *calcularEstatisticasGrafo(Grafo *grafo) {
    EstatisticasGrafo *stats = (EstatisticasGrafo *)malloc(sizeof(EstatisticasGrafo));
    int *graus = (int *)malloc(grafo->numVertices * sizeof(int));
    int totalGrau = 0;

    // Inicializar valores
    stats->grauMinimo = grafo->numVertices;
    stats->grauMaximo = 0;
    stats->numArestas = 0;

    // Calcular o grau de cada vértice
    for (int i = 0; i < grafo->numVertices; i++) {
        int grau = 0;
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int j = 0; j < grafo->numVertices; j++) {
                grau += grafo->grafoMatriz->matriz[i][j];
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *atual = grafo->grafoLista->listaAdj[i];
            while (atual != NULL) {
                grau++;
                atual = atual->prox;
            }
        }

        graus[i] = grau;
        stats->numArestas += grau;

        // Atualizar grau mínimo e máximo
        if (grau < stats->grauMinimo) stats->grauMinimo = grau;
        if (grau > stats->grauMaximo) stats->grauMaximo = grau;
        totalGrau += grau;
    }

    // Calcular grau médio
    stats->grauMedio = (float)totalGrau / grafo->numVertices;

    // Calcular mediana
    qsort(graus, grafo->numVertices, sizeof(int), compararInteiros);
    if (grafo->numVertices % 2 == 0) {
        stats->medianaGrau = (graus[grafo->numVertices / 2 - 1] + graus[grafo->numVertices / 2]) / 2;
    } else {
        stats->medianaGrau = graus[grafo->numVertices / 2];
    }

    // Dividir por 2 pois cada aresta foi contada duas vezes
    stats->numArestas /= 2;
    stats->numVertices = grafo->numVertices;

    // Limpar
    free(graus);

    return stats;
}

// Função auxiliar para comparação
int compararInteiros(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Cria uma pilha com capacidade específica
Pilha *criarPilha(int capacidade) {
    Pilha *pilha = (Pilha *)malloc(sizeof(Pilha));
    if (!pilha) {
        fprintf(stderr, "Erro ao alocar memória para a pilha\n");
        exit(EXIT_FAILURE);
    }
    pilha->dados = (int *)malloc(capacidade * sizeof(int));
    if (!pilha->dados) {
        fprintf(stderr, "Erro ao alocar memória para os dados da pilha\n");
        exit(EXIT_FAILURE);
    }
    pilha->topo = -1;
    pilha->capacidade = capacidade;
    return pilha;
}

// Empilha um valor na pilha
void push(Pilha *pilha, int valor) {
    if (pilha->topo < pilha->capacidade - 1) {
        pilha->dados[++(pilha->topo)] = valor;
    } else {
        // Pilha cheia - redimensionar ou tratar o erro
        fprintf(stderr, "Erro: Pilha cheia ao tentar empilhar %d\n", valor);
        exit(EXIT_FAILURE);
    }
}

// Desempilha um valor da pilha
int pop(Pilha *pilha) {
    if (pilha->topo >= 0) {
        return pilha->dados[(pilha->topo)--];
    } else {
        // Tratamento de erro: pilha vazia
        fprintf(stderr, "Erro: Tentativa de desempilhar de uma pilha vazia\n");
        exit(EXIT_FAILURE);
    }
}

// Verifica se a pilha está vazia
int estaVaziaPilha(Pilha *pilha) {
    return pilha->topo == -1;
}

// Libera a memória da pilha
void liberarPilha(Pilha *pilha) {
    free(pilha->dados);
    free(pilha);
}

// Função para adicionar uma aresta ao grafo (usada para construir a árvore DFS)
void adicionarArestaGrafo(Grafo *grafo, int u, int v, double peso, int direcionado) {
    if (grafo->tipo == MATRIZ_ADJACENCIA) {
        adicionarArestaMatriz(grafo->grafoMatriz, u, v, peso, direcionado);
    } else if (grafo->tipo == LISTA_ADJACENCIA) {
        adicionarArestaLista(grafo->grafoLista, u, v, peso, direcionado);
    }
    grafo->numArestas++;
}



// Função principal para DFS com pilha que constrói a árvore DFS
void dfsComPilhaArvore(Grafo *grafo, int verticeInicial, int *visitados,
                       Grafo *arvoreDFS, int *pais, int *niveis, FILE *arquivoSaida) {
    Pilha *pilha = criarPilha(grafo->numVertices);
    push(pilha, verticeInicial);
    visitados[verticeInicial] = 1;  // Marca como visitado no momento em que é empilhado

    pais[verticeInicial] = verticeInicial;
    niveis[verticeInicial] = 0;

    while (!estaVaziaPilha(pilha)) {
        int verticeAtual = pop(pilha);

        // Escreve no arquivo de saída se ele não for NULL
        if (arquivoSaida != NULL) {
            fprintf(arquivoSaida, "Vértice: %d, Pai: %d, Nível: %d\n",
                    verticeAtual + 1, pais[verticeAtual] + 1, niveis[verticeAtual]);
        }

        // Adiciona aresta ao grafo árvore, se não for o vértice inicial e arvoreDFS não for NULL
        if (verticeAtual != verticeInicial && arvoreDFS != NULL) {
            adicionarArestaGrafo(arvoreDFS, verticeAtual, pais[verticeAtual], 1.0, 0);        }

        // Explora os vértices adjacentes
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int j = grafo->numVertices - 1; j >= 0; j--) {
                if (grafo->grafoMatriz->matriz[verticeAtual][j] == 1 && !visitados[j]) {
                    push(pilha, j);
                    visitados[j] = 1;
                    pais[j] = verticeAtual;
                    niveis[j] = niveis[verticeAtual] + 1;
                }
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *atual = grafo->grafoLista->listaAdj[verticeAtual];
            while (atual != NULL) {
                int v = atual->vertice;
                if (!visitados[v]) {
                    push(pilha, v);
                    visitados[v] = 1;  // Marca como visitado ao empilhar
                    pais[v] = verticeAtual;
                    niveis[v] = niveis[verticeAtual] + 1;
                }
                atual = atual->prox;
            }
        }
    }

    liberarPilha(pilha);
}

// Função para criar uma fila com capacidade específica
Fila *criarFila(int capacidade) {
    Fila *fila = (Fila *)malloc(sizeof(Fila));
    fila->dados = (int *)malloc(capacidade * sizeof(int));
    fila->frente = -1;  // Inicialize com -1 para indicar que a fila está vazia
    fila->tras = -1;    // Inicialize com -1 para indicar que a fila está vazia
    fila->capacidade = capacidade;
    return fila;
}

// Enfileira um valor na fila
void enqueue(Fila *fila, int valor) {
    // Verifica se a fila está cheia
    if ((fila->tras + 1) % fila->capacidade == fila->frente) {
        printf("Erro: Fila cheia ao tentar enfileirar %d\n", valor + 1);
        return;
    }

    if (fila->frente == -1) {
        // Caso especial para o primeiro elemento
        fila->frente = 0;
    }

    fila->tras = (fila->tras + 1) % fila->capacidade;
    fila->dados[fila->tras] = valor;
}

// Desenfileira um valor da fila
int dequeue(Fila *fila) {
    if (fila->frente == -1) {
        // Fila vazia
        printf("Erro: Fila vazia ao tentar desenfileirar\n");
        return -1;
    }

    int valor = fila->dados[fila->frente];
    if (fila->frente == fila->tras) {
        // Fila ficará vazia após esta operação
        fila->frente = -1;
        fila->tras = -1;
    } else {
        fila->frente = (fila->frente + 1) % fila->capacidade;
    }

    return valor;
}

// Verifica se a fila está vazia
int estaVaziaFila(Fila *fila) {
    int vazia = (fila->frente == -1 && fila->tras == -1);
    return vazia;
}

// Libera a memória da fila
void liberarFila(Fila *fila) {
    free(fila->dados);
    free(fila);
}

// Função principal para BFS
void bfsComFilaArvore(Grafo *grafo, int verticeInicial, int *visitados, Grafo *arvoreBFS, int *pais, int *niveis, FILE *arquivoSaida) {
    Fila *fila = criarFila(grafo->numVertices);
    enqueue(fila, verticeInicial);
    visitados[verticeInicial] = 1;
    pais[verticeInicial] = verticeInicial; // O pai do vértice inicial é ele mesmo
    niveis[verticeInicial] = 0;

    while (!estaVaziaFila(fila)) {
        int verticeAtual = dequeue(fila);

        // Escreve no arquivo de saída se ele não for NULL
        if (arquivoSaida != NULL) {
            fprintf(arquivoSaida, "Vértice: %d, Pai: %d, Nível: %d\n", verticeAtual + 1, pais[verticeAtual] + 1, niveis[verticeAtual]);
        }

        // Adiciona aresta à árvore BFS se não for o vértice inicial e se arvoreBFS não for NULL
        if (verticeAtual != verticeInicial && arvoreBFS != NULL) {
            adicionarArestaGrafo(arvoreBFS, verticeAtual, pais[verticeAtual], 1.0, 0);
        }

        // Explora os vértices adjacentes
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int j = 0; j < grafo->numVertices; j++) {
                if (grafo->grafoMatriz->matriz[verticeAtual][j] == 1 && !visitados[j]) {
                    visitados[j] = 1;  // Marca como visitado
                    enqueue(fila, j);   // Enfileira o vértice adjacente
                    pais[j] = verticeAtual;
                    niveis[j] = niveis[verticeAtual] + 1;
                }
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *atual = grafo->grafoLista->listaAdj[verticeAtual];
            while (atual != NULL) {
                int v = atual->vertice;
                if (!visitados[v]) {
                    visitados[v] = 1;  // Marca como visitado
                    enqueue(fila, v);   // Enfileira o vértice adjacente
                    pais[v] = verticeAtual;
                    niveis[v] = niveis[verticeAtual] + 1;
                }
                atual = atual->prox;
            }
        }
    }

    liberarFila(fila);
}

void descobrirComponentes(Grafo *grafo, int *componentes, int *numComponentes) {
    int *visitados = (int *)calloc(grafo->numVertices, sizeof(int));
    int *pais = (int *)malloc(grafo->numVertices * sizeof(int));
    int *niveis = (int *)malloc(grafo->numVertices * sizeof(int));
    *numComponentes = 0;

    for (int v = 0; v < grafo->numVertices; v++) {
        if (!visitados[v]) {
            (*numComponentes)++;
            // Inicializa os arrays 'pais' e 'niveis' para esta BFS
            for (int i = 0; i < grafo->numVertices; i++) {
                pais[i] = -1;
                niveis[i] = -1;
            }

            // Chama a BFS a partir do vértice 'v' sem gerar saída e sem construir a árvore BFS
            bfsComFilaArvore(grafo, v, visitados, NULL, pais, niveis, NULL);

            // Após a BFS, todos os vértices visitados pertencem à mesma componente
            for (int u = 0; u < grafo->numVertices; u++) {
                if (visitados[u] && componentes[u] == 0 && (pais[u] != -1 || u == v)) {
                    componentes[u] = *numComponentes;
                }
            }
        }
    }

    free(visitados);
    free(pais);
    free(niveis);
}

// Função para calcular a distância mínima entre dois vértices usando BFS
int calcularDistancia(Grafo *grafo, int origem, int destino) {
    if (origem < 0 || origem >= grafo->numVertices || destino < 0 || destino >= grafo->numVertices) {
        printf("Vértices inválidos.\n");
        return -1;
    }

    int *visitados = (int *)calloc(grafo->numVertices, sizeof(int));
    int *distancias = (int *)malloc(grafo->numVertices * sizeof(int));
    for (int i = 0; i < grafo->numVertices; i++) {
        distancias[i] = -1; // Inicializa todas as distâncias como -1 (não alcançado)
    }

    Fila *fila = criarFila(grafo->numVertices);
    enqueue(fila, origem);
    visitados[origem] = 1;
    distancias[origem] = 0;

    while (!estaVaziaFila(fila)) {
        int verticeAtual = dequeue(fila);

        // Se encontramos o destino, podemos retornar a distância
        if (verticeAtual == destino) {
            int distancia = distancias[destino];
            liberarFila(fila);
            free(visitados);
            free(distancias);
            return distancia;
        }

        // Explora os vértices adjacentes
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int j = 0; j < grafo->numVertices; j++) {
                if (grafo->grafoMatriz->matriz[verticeAtual][j] == 1 && !visitados[j]) {
                    visitados[j] = 1;
                    distancias[j] = distancias[verticeAtual] + 1;
                    enqueue(fila, j);
                }
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *atual = grafo->grafoLista->listaAdj[verticeAtual];
            while (atual != NULL) {
                int v = atual->vertice;
                if (!visitados[v]) {
                    visitados[v] = 1;
                    distancias[v] = distancias[verticeAtual] + 1;
                    enqueue(fila, v);
                }
                atual = atual->prox;
            }
        }
    }

    // Se o destino não foi alcançado, os vértices não estão conectados
    liberarFila(fila);
    free(visitados);
    free(distancias);
    return -1; // Indica que não há caminho entre origem e destino
}

// Função de Dijkstra utilizando vetor
void dijkstraVetor(Grafo *grafo, int origem, int imprimir, double *distancia, int *pais) {
    int numVertices = grafo->numVertices;
    int *visitados = (int *)calloc(numVertices, sizeof(int));

    if (!distancia || !visitados || !pais) {
        printf("Erro ao alocar memória para arrays de Dijkstra.\n");
        return;
    }

    // Inicializa o array de distâncias com infinito e os pais com -1
    for (int i = 0; i < numVertices; i++) {
        distancia[i] = DBL_MAX;
        pais[i] = -1;
    }

    distancia[origem] = 0.0;

    // Algoritmo de Dijkstra
    for (int i = 0; i < numVertices - 1; i++) {
        // Encontrar o vértice com a menor distância que ainda não foi visitado
        double minDistancia = DBL_MAX;
        int u = -1;

        for (int v = 0; v < numVertices; v++) {
            if (!visitados[v] && distancia[v] < minDistancia) {
                minDistancia = distancia[v];
                u = v;
            }
        }

        // Marca o vértice como visitado
        if (u != -1) {
            visitados[u] = 1;

            // Atualizar as distâncias dos vértices adjacentes
            if (grafo->tipo == MATRIZ_ADJACENCIA) {
                for (int v = 0; v < numVertices; v++) {
                    if (grafo->grafoMatriz->matriz[u][v] > 0 && !visitados[v]) {
                        double peso = grafo->grafoMatriz->matriz[u][v];
                        if (distancia[u] + peso < distancia[v]) {
                            distancia[v] = distancia[u] + peso;
                            pais[v] = u;
                        }
                    }
                }
            } else if (grafo->tipo == LISTA_ADJACENCIA) {
                No *adjacente = grafo->grafoLista->listaAdj[u];
                while (adjacente != NULL) {
                    int v = adjacente->vertice;
                    double peso = adjacente->peso;
                    if (!visitados[v] && distancia[u] + peso < distancia[v]) {
                        distancia[v] = distancia[u] + peso;
                        pais[v] = u;
                    }
                    adjacente = adjacente->prox;
                }
            }
        }
    }

    // Exibir as distâncias mínimas e os caminhos se 'imprimir' for verdadeiro
    if (imprimir) {
        printf("Distâncias mínimas a partir do vértice %d:\n", origem + 1);
        for (int i = 0; i < numVertices; i++) {
            if (distancia[i] == DBL_MAX) {
                printf("Vértice %d: Inacessível\n", i + 1);
            } else {
                printf("Vértice %d: %.2f (Caminho: ", i + 1, distancia[i]);
                // Reconstruir o caminho
                int v = i;
                int caminho[numVertices];
                int tamanhoCaminho = 0;
                while (v != -1) {
                    caminho[tamanhoCaminho++] = v;
                    v = pais[v];
                }
                // Imprimir o caminho na ordem correta
                for (int j = tamanhoCaminho - 1; j >= 0; j--) {
                    printf("%d", caminho[j] + 1);
                    if (j > 0) {
                        printf(" -> ");
                    }
                }
                printf(")\n");
            }
        }
    }

    free(visitados);
}

MinHeap* criarMinHeap(int capacidade) {
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->capacidade = capacidade;
    minHeap->tamanho = 0;
    minHeap->posicoes = (int*) malloc(capacidade * sizeof(int));
    minHeap->array = (HeapNode**) malloc(capacidade * sizeof(HeapNode*));
    return minHeap;
}

void trocarHeapNode(HeapNode** a, HeapNode** b) {
    HeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int menor = idx;
    int esquerda = 2 * idx + 1;
    int direita = 2 * idx + 2;

    if (esquerda < minHeap->tamanho &&
        minHeap->array[esquerda]->distancia < minHeap->array[menor]->distancia)
        menor = esquerda;

    if (direita < minHeap->tamanho &&
        minHeap->array[direita]->distancia < minHeap->array[menor]->distancia)
        menor = direita;

    if (menor != idx) {
        // Atualiza as posições
        minHeap->posicoes[minHeap->array[menor]->vertice] = idx;
        minHeap->posicoes[minHeap->array[idx]->vertice] = menor;

        // Troca os nós
        trocarHeapNode(&minHeap->array[menor], &minHeap->array[idx]);

        minHeapify(minHeap, menor);
    }
}

HeapNode* extrairMinimo(MinHeap* minHeap) {
    if (minHeap->tamanho == 0)
        return NULL;

    // Armazena o nó raiz
    HeapNode* raiz = minHeap->array[0];

    // Substitui a raiz pelo último nó
    HeapNode* ultimoNode = minHeap->array[minHeap->tamanho - 1];
    minHeap->array[0] = ultimoNode;

    // Atualiza a posição
    minHeap->posicoes[raiz->vertice] = minHeap->tamanho - 1;
    minHeap->posicoes[ultimoNode->vertice] = 0;

    minHeap->array[minHeap->tamanho - 1] = NULL;

    // Reduz o tamanho do heap e heapify na raiz
    minHeap->tamanho--;
    minHeapify(minHeap, 0);

    return raiz;
}

void diminuirChave(MinHeap* minHeap, int vertice, double distancia) {
    int i = minHeap->posicoes[vertice];
    minHeap->array[i]->distancia = distancia;

    // Move o nó para cima enquanto a propriedade do heap não for restabelecida
    while (i && minHeap->array[i]->distancia < minHeap->array[(i - 1) / 2]->distancia) {
        // Atualiza as posições
        minHeap->posicoes[minHeap->array[i]->vertice] = (i - 1) / 2;
        minHeap->posicoes[minHeap->array[(i - 1) / 2]->vertice] = i;

        // Troca os nós
        trocarHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

int estaVazioHeap(MinHeap *minHeap) {
    return minHeap->tamanho == 0;
}

int estaNoHeap(MinHeap *minHeap, int vertice) {
    if (minHeap->posicoes[vertice] < minHeap->tamanho)
        return 1;
    return 0;
}

void liberarMinHeap(MinHeap *minHeap) {
    for (int i = 0; i < minHeap->capacidade; i++) {
        if (minHeap->array[i]) {
            free(minHeap->array[i]);
            minHeap->array[i] = NULL;
        }
    }
    free(minHeap->array);
    free(minHeap->posicoes);
    free(minHeap);
}

void dijkstraHeap(Grafo *grafo, int origem, int imprimir, double *distancia, int *pais) {
    int numVertices = grafo->numVertices;

    // Cria o Min Heap e inicializa
    MinHeap* minHeap = criarMinHeap(numVertices);

    for (int v = 0; v < numVertices; v++) {
        distancia[v] = DBL_MAX;
        pais[v] = -1;
        minHeap->array[v] = (HeapNode*) malloc(sizeof(HeapNode));
        minHeap->array[v]->vertice = v;
        minHeap->array[v]->distancia = distancia[v];
        minHeap->posicoes[v] = v;
    }

    // Define a distância do vértice de origem como zero e atualiza no heap
    distancia[origem] = 0.0;
    diminuirChave(minHeap, origem, distancia[origem]);

    minHeap->tamanho = numVertices;

    // Enquanto o heap não estiver vazio
    while (!estaVazioHeap(minHeap)) {
        // Extrai o vértice com a menor distância
        HeapNode* minNode = extrairMinimo(minHeap);
        int u = minNode->vertice;
        free(minNode); // Libera o nó extraído

        // Para cada vizinho de u
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int v = 0; v < numVertices; v++) {
                double peso = grafo->grafoMatriz->matriz[u][v];
                if (peso > 0 && estaNoHeap(minHeap, v)) {
                    if (distancia[u] + peso < distancia[v]) {
                        distancia[v] = distancia[u] + peso;
                        pais[v] = u;
                        diminuirChave(minHeap, v, distancia[v]);
                    }
                }
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *adjacente = grafo->grafoLista->listaAdj[u];
            while (adjacente != NULL) {
                int v = adjacente->vertice;
                double peso = adjacente->peso;
                if (estaNoHeap(minHeap, v)) {
                    if (distancia[u] + peso < distancia[v]) {
                        distancia[v] = distancia[u] + peso;
                        pais[v] = u;
                        diminuirChave(minHeap, v, distancia[v]);
                    }
                }
                adjacente = adjacente->prox;
            }
        }
    }

    // Exibir as distâncias mínimas e os caminhos se 'imprimir' for verdadeiro
    if (imprimir) {
        printf("Distâncias mínimas a partir do vértice %d:\n", origem + 1);
        for (int i = 0; i < numVertices; i++) {
            if (distancia[i] == DBL_MAX) {
                printf("Vértice %d: Inacessível\n", i + 1);
            } else {
                printf("Vértice %d: %.2f (Caminho: ", i + 1, distancia[i]);
                // Reconstruir o caminho
                int v = i;
                int caminho[numVertices];
                int tamanhoCaminho = 0;
                while (v != -1) {
                    caminho[tamanhoCaminho++] = v;
                    v = pais[v];
                }
                // Imprimir o caminho na ordem correta
                for (int j = tamanhoCaminho - 1; j >= 0; j--) {
                    printf("%d", caminho[j] + 1);
                    if (j > 0) {
                        printf(" -> ");
                    }
                }
                printf(")\n");
            }
        }
    }

    liberarMinHeap(minHeap);
}
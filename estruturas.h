// estruturas.h
#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

typedef struct {
    float x, y, z;
} Vertice;

typedef struct {
    int v1, v2, v3;
} Face;

// Define a estrutura de uma caixa
typedef struct {
    float x, z;                // Coordenadas de posição
    float largura, altura, profundidade; // Dimensões da caixa
    float direcao;             // Direção do movimento (1 ou -1)
    float tipoMovimento;       // Tipo de movimento (1 = horizontal, 2 = vertical, 3 = aleatorio)
} Caixa;

// Define a estrutura de uma moeda
typedef struct {
    float x, y, z;             // Posição da moeda no mundo
    int coletada;              // Estado: 0 = não coletada, 1 = coletada
    float largura;             // Largura (extent em X)
    float altura;              // Altura (extent em Y)
    float profundidade;        // Profundidade (extent em Z)
} Moeda;

// Define a estrutura de um AABB (Axis-Aligned Bounding Box)
typedef struct {
    float x;                   // Coordenada X do centro
    float y;                   // Coordenada Y do centro
    float z;                   // Coordenada Z do centro
    float largura;             // Largura (extent em X)
    float altura;              // Altura (extent em Y)
    float profundidade;        // Profundidade (extent em Z)
} AABB;

// Estrutura para representar uma bolinha
struct Bolinha {
    float x, y, z;       // Posicao
    float dx, dy, dz;    // Direcao
};

#endif // ESTRUTURAS_H

#ifndef VARIAVEIS_H
#define VARIAVEIS_H

#include <GL/gl.h>
#include "estruturas.h"
#include <vector>

// Definição de constantes
#define FRENTE 1
#define TRAS 2
#define DIREITA 3
#define ESQUERDA 4
#define NUM_MOEDAS 5
#define NUM_CAIXAS 4

// Declarações das variáveis (extern evita múltipla definição)
extern float angle, fAspect;
extern float targetX, targetY, targetZ;
extern float passo;
extern float vetorX, vetorZ, magnitude, nX, nZ;
extern char texto[30];

extern AABB barreiras[5];
extern Vertice *vertices;
extern Face *faces;
extern int numVertices, numFaces;
extern AABB jogador, jogadorSimulado;
extern std::vector<Bolinha> bolinhas;
extern int moedasColetadas;
extern Moeda moedas[NUM_MOEDAS];
extern Caixa caixas[NUM_CAIXAS];
extern GLfloat velocidadeCaixa;

void FinalizaJogo();

#endif

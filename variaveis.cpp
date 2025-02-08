#include "variaveis.h"
#include <cstdlib>
#include <GL/freeglut.h>

// Definição das variáveis
float angle = 45, fAspect;
float targetX = 0.0f, targetY = 5.0f, targetZ = -10.0f;
float passo = 10.0f;
float vetorX = 0.0f, vetorZ = 0.0f;
float magnitude = 0.0f, nX = 0.0f, nZ = 0.0f;
char texto[30];

AABB barreiras[5] = {
    {0.0f, 0.0f, -400.0f, 800.0f, 200.0f, 10.0f},
    {0.0f, 0.0f,  400.0f, 800.0f, 200.0f, 10.0f},
    {400.0f, 0.0f,  0.0f, 10.0f, 200.0f, 800.0f},
    {-400.0f, 0.0f,  0.0f, 10.0f, 200.0f, 800.0f},
};

Vertice *vertices = NULL;
Face *faces = NULL;
int numVertices = 0, numFaces = 0;

AABB jogador = {0.0f, 5.0f, 15.0f, 1.0f, 2.0f, 2.0f};
AABB jogadorSimulado = jogador;
std::vector<Bolinha> bolinhas;

int moedasColetadas = 0;
Moeda moedas[NUM_MOEDAS];

Caixa caixas[NUM_CAIXAS] = {
    {-300.0, 0.0, 65.0, 65.0, 65.0, 1, 1},
    {300.0, 0.0, 65.0, 65.0, 65.0, -1, 2},
    {0.0, -300.0, 65.0, 65.0, 65.0, -1, 1},
    {100.0, 100.0, 65.0, 65.0, 65.0, 2, 2}
};

GLfloat velocidadeCaixa = 2.0;

// Função para liberar memória e encerrar o jogo
void FinalizaJogo() {
    if (vertices) {
        free(vertices);
        vertices = NULL;
    }
    if (faces) {
        free(faces);
        faces = NULL;
    }
    glutLeaveMainLoop();
}

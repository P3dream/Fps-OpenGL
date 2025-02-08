#include <stdlib.h>
#include "colisoes.h"
#include <math.h>
#include <fstream>
#include <sstream>

int verificaColisao(AABB a, AABB b) {
    // Checa se as caixas se intersectam em cada eixo
    return (fabs(a.x - b.x) * 2 < (a.largura + b.largura)) &&
           (fabs(a.y - b.y) * 2 < (a.altura + b.altura)) &&
           (fabs(a.z - b.z) * 2 < (a.profundidade + b.profundidade));
}

void verificaColisoesComMoedas(Moeda moedas[], int numMoedas, AABB jogador, int * moedasColetadas, char * texto){
    for (int i = 0; i < numMoedas; i++) {
        if (!moedas[i].coletada) { // Apenas verifica moedas que ainda nao foram coletadas
            AABB moedaAABB = moedaParaAABB(moedas[i]);

            // Verifica colisao com o jogador
            if (verificaColisao(moedaAABB, jogador)) {
                printf("Colisao detectada entre o jogador e a moeda %d!\n", i);
                moedas[i].coletada = 1; // Marca a moeda como coletada
                *moedasColetadas = *moedasColetadas + 1;
                sprintf(texto, "%d Moedas Coletadas", *moedasColetadas);
                continue;
            }
        }
    }
}

AABB moedaParaAABB(Moeda moeda) {
    AABB aabb;
    aabb.x = moeda.x;
    aabb.y = moeda.y;
    aabb.z = moeda.z;
    aabb.largura = moeda.largura;
    aabb.altura = moeda.altura;
    aabb.profundidade = moeda.profundidade;
    return aabb;
}

bool checaColisoesBarreiras(AABB *jogador, AABB barreiras[], int numBarreiras) {
    for (int i = 0; i < numBarreiras; i++) {
        if (verificaColisao(*jogador, barreiras[i])) {
            return true;
        }
 	}
    return false;
}

AABB caixaParaAABB(Caixa caixa) {
    AABB aabb;
    aabb.x = caixa.x;
    aabb.y = 0.0;
    aabb.z = caixa.z;
    aabb.largura = caixa.largura;
    aabb.altura = caixa.altura;
    aabb.profundidade = caixa.profundidade;
    return aabb;
}

bool verificaColisoesComCaixas(Caixa caixas[], int numCaixas, AABB jogador) {
    for (int i = 0; i < numCaixas; i++) {
        AABB caixaAABB = caixaParaAABB(caixas[i]); // Converte a caixa para AABB
        if (verificaColisao(caixaAABB, jogador)) {
            return true;
        }
    }
    return false;
}

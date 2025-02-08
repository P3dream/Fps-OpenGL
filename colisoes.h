#ifndef COLISOES_H
#define COLISOES_H
#include "estruturas.h"

int verificaColisao(AABB a, AABB b);
void verificaColisoesComMoedas(Moeda moedas[], int numMoedas, AABB jogador, int * moedasColetadas, char * texto);
bool checaColisoesBarreiras(AABB *jogador, AABB barreiras[], int numBarreiras);
AABB moedaParaAABB(Moeda moeda);
AABB caixaParaAABB(Caixa caixa);
bool verificaColisoesComCaixas(Caixa caixas[], int numCaixas, AABB jogador);
#endif

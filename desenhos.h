#ifndef DESENHOS_H
#define DESENHOS_H
#include "estruturas.h"

void DesenhaCaixa(Caixa caixa);
void DesenhaBolinhas(Bolinha* bolinhas, size_t tamanho);
void DesenhaEixos();
void DesenhaParedes();
void DesenhaTeapots();
void DesenhaCena(bool cameraMapa, AABB jogador, Caixa* caixas,int numCaixas, std::vector<Bolinha>& bolinhas, int numMoedas, int numFaces, Vertice* vertices, Face* faces, Moeda* moedas);
void DesenhaTexto(char *string);
void DesenhaMoedas(int numMoedas, int numFaces, Vertice* vertices, Face* faces, Moeda* moedas);
void DesenhaMira();

#endif
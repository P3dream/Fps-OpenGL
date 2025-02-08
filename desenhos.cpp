#include <GL/glut.h>
#include <stdlib.h>
#include "estruturas.h"
#include <vector>

// Funcao para desenhar uma caixa
void DesenhaCaixa(Caixa caixa) {
    glPushMatrix();
    glTranslatef(caixa.x, 0.0, caixa.z);
    glColor3f(1.0, 0.0, 0.0); // Vermelho
    glutSolidCube(caixa.largura);
    glPopMatrix();
}

// Desenha todas as bolinhas
void DesenhaBolinhas(std::vector<Bolinha>& bolinhas, size_t tamanho) {
    for (size_t i = 0; i < tamanho; ++i) { 
        glPushMatrix();
        glTranslatef(bolinhas[i].x, bolinhas[i].y, bolinhas[i].z);
        glColor3f(1.0f, 0.0f, 0.0f); // Cor vermelha
        glutSolidSphere(2.0f, 20, 20); // Bolinha com raio 2
        glPopMatrix();
    }
}

void DesenhaEixos(){
	glBegin(GL_LINES);
    // Eixo X (Vermelho)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-500.0f, 0.0f, 0.0f);
    glVertex3f(500.0f, 0.0f, 0.0f);

    // Eixo Y (Verde)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -500.0f, 0.0f);
    glVertex3f(0.0f, 500.0f, 0.0f);

    // Eixo Z (Azul)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -500.0f);
    glVertex3f(0.0f, 0.0f, 500.0f);

    glEnd();
}

void DesenhaParedes(){
	// Desenha as paredes

    // Parede frente
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.9f);
    glTranslatef(0.0f, 0.0f, -400.0f); // Desloca para frente
    glScalef(80.0f, 20.0f, 1.0f);  // Dimensoes da parede frente
    glutSolidCube(10.0f);
    glPopMatrix();    

    //Parede tr�s
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.9f);
    glTranslatef(0.0f, 0.0f, 400.0f); // Desloca para frente
    glScalef(80.0f, 20.0f, 1.0f);  // Dimensoes da parede frente
    glutSolidCube(10.0f);
    glPopMatrix();

    // Parede direita
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.9f);
    glTranslatef(400.0f, 0.0f, 0.0f); // Desloca para a direita
    glScalef(1.0f, 20.0f, 80.0f);  // Dimens�es da parede direita
    glutSolidCube(10.0f);
    glPopMatrix();

    // Parede esquerda
    
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.9f);
    glTranslatef(-400.0f, 0.0f, 0.0f); // Desloca para a esquerda
    glScalef(1.0f, 20.0f, 80.0f);  // Dimensoes da parede esquerda
    glutSolidCube(10.0f);
    glPopMatrix();

    // Piso
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);
    glScalef(80.0f, 1.0f, 80.0f);
    glutSolidCube(10.0f);
    glPopMatrix();
    glEnd();
}

void DesenhaTeapots(){
	// Primeiro teapot
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(-100.0f, 10.0f, 0.0f);
    glutSolidTeapot(25.0f);
    glPopMatrix();  
    
    // Segundo teapot
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 10.0f, -100.0f);
    glutSolidTeapot(25.0f);
    glPopMatrix();

    // Terceiro teapot
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 1.0f);
    glTranslatef(100.0f, 10.0f, 0.0f);
    glutSolidTeapot(25.0f);
    glPopMatrix();
}

void DesenhaMoedas(int numMoedas, int numFaces, Vertice* vertices, Face* faces, Moeda* moedas) {
    for (int i = 0; i < numMoedas; i++) {
        if (!moedas[i].coletada) { // Desenha somente as moedas nao coletadas
            glPushMatrix(); // Salva a matriz atual
            glTranslatef(moedas[i].x, moedas[i].y, moedas[i].z); // Posiciona a moeda
            
            // Desenha o objeto usando os vertices e faces carregados
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < numFaces; j++) {
                glColor3f(1.0, 1.0, 0.0); // Cor amarela
                glVertex3f(vertices[faces[j].v1 - 1].x, vertices[faces[j].v1 - 1].y, vertices[faces[j].v1 - 1].z);
                glVertex3f(vertices[faces[j].v2 - 1].x, vertices[faces[j].v2 - 1].y, vertices[faces[j].v2 - 1].z);
                glVertex3f(vertices[faces[j].v3 - 1].x, vertices[faces[j].v3 - 1].y, vertices[faces[j].v3 - 1].z);
            }
            glEnd();

            glPopMatrix(); // Restaura a matriz
        }
    }
}

void DesenhaTexto(char *string) 
{  
  	glPushMatrix();
        glDisable(GL_LIGHTING);  // Desativa iluminação para o texto
        glColor3f(1.0f, 1.0f, 1.0f); // Define a cor para branco puro

        glRasterPos3f(-300, 910, 0); 
        while (*string)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++); 

        glEnable(GL_LIGHTING);  // Reativa iluminação
	glPopMatrix();
}

void DesenhaMira() {
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1); // Projeção ortográfica normalizada

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);  // Garante que a mira fique sempre visível
    glDisable(GL_LIGHTING);    // Evita efeitos de luz na mira
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho

    glBegin(GL_LINES);
        // Linha horizontal
        glVertex2f(-0.05f, 0.0f);
        glVertex2f(0.05f, 0.0f);

        // Linha vertical
        glVertex2f(0.0f, -0.05f);
        glVertex2f(0.0f, 0.05f);
    glEnd();

    glEnable(GL_DEPTH_TEST);   // Reativa teste de profundidade
    glEnable(GL_LIGHTING);     // Reativa iluminação

    glPopMatrix();
    glPopMatrix();
}



void DesenhaCena(bool cameraMapa, AABB jogador, Caixa* caixas, int numCaixas, std::vector<Bolinha>& bolinhas, int numMoedas, int numFaces, Vertice* vertices, Face* faces, Moeda* moedas) {
    if(cameraMapa){
        glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(jogador.x, jogador.y, jogador.z); // Desloca para frente
        glScalef(2.0f, 1.0f, 2.0f);  // Dimensoes da parede frente
        glutSolidCube(10.0f);
        glPopMatrix();
    }
    
	DesenhaBolinhas(bolinhas, bolinhas.size());
    DesenhaMoedas(numMoedas, numFaces, vertices, faces, moedas);
	//DesenhaEixos();
	DesenhaParedes();
    
    // Desenha as caixas
    for(int i=0; i<numCaixas; i++){
        DesenhaCaixa(caixas[i]);
    }
    
    //DesenhaTeapots();
    if(!cameraMapa){
        DesenhaMira();
    }
}
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>
#include "colisoes.h"
#include "desenhos.h"
#include <cstdlib>  // Para usar rand()
#include "variaveis.h"

// Funcao para carregar o arquivo OBJ
void CarregaOBJ(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (linha[0] == 'v' && linha[1] == ' ') {
            // Le os vertices
            Vertice v;
            sscanf(linha, "v %f %f %f", &v.x, &v.y, &v.z);
            vertices = (Vertice*) realloc(vertices, sizeof(Vertice) * (numVertices + 1));
            vertices[numVertices++] = v;
        } else if (linha[0] == 'f') {
            // Le as faces
            Face f;
            sscanf(linha, "f %d %d %d", &f.v1, &f.v2, &f.v3);
            faces = (Face*) realloc(faces, sizeof(Face) * (numFaces + 1));
            faces[numFaces++] = f;
        }
    }

    fclose(arquivo);
}

void InicializaMoedas() {
    srand(time(NULL));
    for (int i = 0; i < NUM_MOEDAS; i++) {
        moedas[i].x = (rand() % 761) - 380;
        moedas[i].y = 4.0f;          // Altura fixa
        moedas[i].z = (rand() % 761) - 380;
        moedas[i].coletada = 0;       // Inicialmente, nenhuma moeda foi coletada
        moedas[i].largura = 12.0f;
        moedas[i].altura = 0.6f;
        moedas[i].profundidade = 12.0f; 
    }
}

void AtualizaCaixas(Caixa caixas[], int numCaixas, AABB jogador, AABB barreiras[], int numBarreiras) {
    for (int i = 0; i < numCaixas; i++) {
        // Simula a nova posição da caixa
        Caixa caixaSimulada = caixas[i];

        // Se o tipo de movimento for 1 (horizontal), move a caixa horizontalmente
        if (caixas[i].tipoMovimento == 1) {
            caixaSimulada.x += velocidadeCaixa * caixas[i].direcao;  // Direção 1 ou -1
        }
        // Se o tipo de movimento for 2 (vertical), move a caixa verticalmente
        else if (caixas[i].tipoMovimento == 2) {
            caixaSimulada.z += velocidadeCaixa * caixas[i].direcao;  // Direção 1 ou -1
        }
        // Se o tipo de movimento for 3 (aleatorio), muda a direção aleatoriamente
        else if (caixas[i].tipoMovimento == 3) {
            caixas[i].direcao = (rand() % 2 == 0) ? 1 : -1;  // Atribui 1 ou -1 aleatoriamente
            caixaSimulada.x += velocidadeCaixa * caixas[i].direcao;
            caixaSimulada.z += velocidadeCaixa * caixas[i].direcao;
        }

        // Converte a caixa simulada para AABB
        AABB caixaAABB = caixaParaAABB(caixaSimulada);

        // Verifica colisão entre as caixas
        for (int j = 0; j < numCaixas; j++) {
            if (i != j) {
                AABB caixaComparar = caixaParaAABB(caixas[j]);
                if (verificaColisao(caixaAABB, caixaComparar)) {
                    // Se houver colisão entre as caixas, inverte a direção das duas
                    caixas[i].direcao = -caixas[i].direcao;
                    caixas[j].direcao = -caixas[j].direcao;

                    // Ajusta a posição da caixa para evitar sobreposição
                    if (caixaSimulada.x > caixas[j].x) {
                        caixaSimulada.x = caixas[j].x + caixas[j].largura / 2 + caixaAABB.largura / 2;
                    } else {
                        caixaSimulada.x = caixas[j].x - caixas[j].largura / 2 - caixaAABB.largura / 2;
                    }

                    if (caixaSimulada.z > caixas[j].z) {
                        caixaSimulada.z = caixas[j].z + caixas[j].altura / 2 + caixaAABB.altura / 2;
                    } else {
                        caixaSimulada.z = caixas[j].z - caixas[j].altura / 2 - caixaAABB.altura / 2;
                    }
                }
            }
        }

        // Verifica colisão com o jogador
        if (verificaColisao(caixaAABB, jogador)) {
            // Se houver colisão com o jogador, ajusta a posição da caixa
            float deltaX = caixaSimulada.x - jogador.x;
            float deltaZ = caixaSimulada.z - jogador.z;

            // Determina qual direção a caixa deve mover para evitar sobreposição
            if (fabs(deltaX) > fabs(deltaZ)) {
                // Move a caixa horizontalmente
                if (deltaX > 0) {
                    caixaSimulada.x = jogador.x + (jogador.largura / 2) + (caixaAABB.largura / 2);
                } else {
                    caixaSimulada.x = jogador.x - (jogador.largura / 2) - (caixaAABB.largura / 2);
                }
            } else {
                // Move a caixa verticalmente
                if (deltaZ > 0) {
                    caixaSimulada.z = jogador.z + (jogador.altura / 2) + (caixaAABB.altura / 2);
                } else {
                    caixaSimulada.z = jogador.z - (jogador.altura / 2) - (caixaAABB.altura / 2);
                }
            }

            // Inverte a direção da caixa após a colisão com o jogador
            caixas[i].direcao = -caixas[i].direcao;
        }

        // Ajuste para garantir que a caixa não ultrapasse os limites do mapa
        if (caixaSimulada.x + caixaAABB.largura / 2 > 400.0f) {
            caixaSimulada.x = 400.0f - caixaAABB.largura / 2;  // Limite direito
            caixas[i].direcao = -caixas[i].direcao;  // Inverte a direção
        }
        if (caixaSimulada.x - caixaAABB.largura / 2 < -400.0f) {
            caixaSimulada.x = -400.0f + caixaAABB.largura / 2;  // Limite esquerdo
            caixas[i].direcao = -caixas[i].direcao;  // Inverte a direção
        }
        if (caixaSimulada.z + caixaAABB.altura / 2 > 400.0f) {
            caixaSimulada.z = 400.0f - caixaAABB.altura / 2;  // Limite superior
            caixas[i].direcao = -caixas[i].direcao;  // Inverte a direção
        }
        if (caixaSimulada.z - caixaAABB.altura / 2 < -400.0f) {
            caixaSimulada.z = -400.0f + caixaAABB.altura / 2;  // Limite inferior
            caixas[i].direcao = -caixas[i].direcao;  // Inverte a direção
        }

        // Atualiza a posição da caixa após a simulação de movimento
        caixas[i].x = caixaSimulada.x;
        caixas[i].z = caixaSimulada.z;

    }

    // Redesenha a cena
    glutPostRedisplay();
}

void LancaBolinha() {
    // Cria uma nova bolinha na posicao da camera
    Bolinha novaBolinha;
    novaBolinha.x = jogador.x;
    novaBolinha.y = jogador.y;
    novaBolinha.z = jogador.z;

    // Define a direcao baseada no vetor direcao da camera
    float vetorX = targetX - jogador.x;
    float vetorY = targetY - jogador.y;
    float vetorZ = targetZ - jogador.z;

    float magnitude = sqrt(vetorX * vetorX + vetorY * vetorY + vetorZ * vetorZ);
    novaBolinha.dx = vetorX / magnitude;
    novaBolinha.dy = vetorY / magnitude;
    novaBolinha.dz = vetorZ / magnitude;

    // Adiciona ao vetor de bolinhas
    bolinhas.push_back(novaBolinha);
}

// Funcao callback chamada para fazer o desenho
void Desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Primeira camera - Principal 
    glViewport(0,0,800,800);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.4, 4000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(jogador.x, jogador.y, jogador.z, 
            targetX, targetY, targetZ, 
            0, 1, 0);
    DesenhaCena(false, jogador, caixas, NUM_CAIXAS, bolinhas, NUM_MOEDAS, numFaces, vertices, faces, moedas);

    
	// Segunda camera - MAPA 
    glViewport(800,0,500,500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();	
    glOrtho(-800,800,-800,800,1,1600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 800, 0, 
            0,0,0, 
            0, 0, -1);
    DesenhaCena(true, jogador, caixas, NUM_CAIXAS, bolinhas, NUM_MOEDAS, numFaces, vertices, faces, moedas);
    
    // Terceira camera - TESTE
    glViewport(800, 400, 500, 500);  // Ajuste a area do viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-800, 800, -800, 800, 1, 1600); // Mesma projecao
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 950, 0, // Posicao da camera no mapa
              0, 0, 0,   // Ponto de interesse (centro do mapa)
              0, 0, -1); // Vetor up da camera
    
    
    // Teto
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(0.0f, 900.0f, 0.0f);
    glScalef(80.0f, 1.0f, 80.0f);
    glutSolidCube(10.0f);
    glPopMatrix();
    glEnd();
    DesenhaTexto(texto);
    glutSwapBuffers();
}

// Callback para capturar cliques do mouse
void MouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        LancaBolinha(); // Lanca uma bolinha ao clicar com o botao esquerdo
    }
}

void AtualizaBolinhas(Moeda moedas[], int numMoedas, AABB barreiras[], int numBarreiras, Caixa caixas[], int numCaixas) {
    float velocidade = 2.0f; // Velocidade da bolinha

    for (size_t i = 0; i < bolinhas.size(); ++i) {
        // Atualiza a posicao da bolinha
        bolinhas[i].x += bolinhas[i].dx * velocidade;
        bolinhas[i].y += bolinhas[i].dy * velocidade;
        bolinhas[i].z += bolinhas[i].dz * velocidade;

        // Verifica colisao com barreiras (caixas ou parede)
        AABB bolinhaAABB;
        bolinhaAABB.x = bolinhas[i].x;
        bolinhaAABB.y = bolinhas[i].y;
        bolinhaAABB.z = bolinhas[i].z;
        bolinhaAABB.largura = bolinhaAABB.altura = bolinhaAABB.profundidade = 4.0f; // Define o tamanho da bolinha

        // Colisao com barreiras
        if (checaColisoesBarreiras(&bolinhaAABB, barreiras, numBarreiras)) {
            printf("Bolinha %zu colidiu com uma barreira e foi removida!\n", i);
            bolinhas.erase(bolinhas.begin() + i); // Remove a bolinha do vetor
            --i; // Ajusta o indice devido a remocao
            continue; // Pula para a proxima iteracao do loop
        }

        // Colisao com caixas
        bool colisaoComCaixa = false;
        for (int j = 0; j < numCaixas; ++j) {
            AABB caixaAABB = caixaParaAABB(caixas[j]); // Converte a caixa para AABB
            if (verificaColisao(caixaAABB, bolinhaAABB)) {
                printf("Bolinha %zu colidiu com a caixa %d e foi removida!\n", i, j);
                bolinhas.erase(bolinhas.begin() + i); // Remove a bolinha do vetor
                --i; // Ajusta o indice devido a remocao
                colisaoComCaixa = true;
                break; // Sai do loop de caixas apos detectar colisao
            }
        }
        if (colisaoComCaixa) {
            continue; // Pula para a proxima bolinha
        }

        // Verifica colisao com as moedas
        for (int j = 0; j < numMoedas; ++j) {
            if (!moedas[j].coletada) { // Apenas verifica moedas que ainda nao foram coletadas
                AABB moedaAABB = moedaParaAABB(moedas[j]); // Converte a moeda para AABB

                if (verificaColisao(moedaAABB, bolinhaAABB)) { // Colisao detectada
                    printf("Bolinha %zu colidiu com a moeda %d!\n", i, j);
                    moedas[j].coletada = 1; // Marca a moeda como coletada
                    moedasColetadas ++;
                    sprintf(texto, "%d Moedas Coletadas", moedasColetadas);
                }
            }
        }
    }
}   

// Atualiza a cena continuamente
void AtualizaCena(int value) {
    AtualizaBolinhas(moedas,NUM_MOEDAS,barreiras,4,caixas,NUM_CAIXAS); // Atualiza a posicao das bolinhas
    AtualizaCaixas(caixas, NUM_CAIXAS, jogador, barreiras, 4);
    glutPostRedisplay();

    glutTimerFunc(16, AtualizaCena, 0); // Atualiza a cada ~16ms (60 FPS)
}

// Inicializa parametros de rendering
void Inicializa (void) { 
    GLfloat luzAmbiente[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat luzDifusa[4] = {0.0, 1.0, 1.0, 1.0};   // "cor" 
    GLfloat luzEspecular[4] = {1.0, 1.0, 1.0, 1.0}; // "brilho" 
    GLfloat posicaoLuz[4] = {0.0, 700.0, 0.0, 1.0};
    GLfloat posicaoLuz2[4] = {0.0, 950.0, 0.0, 1.0};

    GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0}; 
    GLint especMaterial = 60;

    glClearColor(0.0f,0.0f, 0.0f, 1.0f); // Fundo branco
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente); 
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz2 );

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);  
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    
    InicializaMoedas();
    CarregaOBJ("moeda.obj");  // Carrega o arquivo OBJ da moeda
    sprintf(texto, "%d Moedas Coletadas", moedasColetadas);
}

// Especifica o volume de visualizacao
void EspecificaParametrosVisualizacao(void) {
 	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.4, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Funcao callback chamada quando o tamanho da janela e alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    EspecificaParametrosVisualizacao();
}

// Funcao para gerenciar teclas especiais (setas)
void movimenta(int direcao) {
    switch (direcao) {
    	case FRENTE:
    	case TRAS:
        	vetorX = targetX - jogador.x;
            vetorZ = targetZ - jogador.z;

            magnitude = sqrt(vetorX * vetorX + vetorZ * vetorZ);
            
            nX = vetorX / magnitude;
            nZ = vetorZ / magnitude;

            if (direcao == TRAS) {
                nX = -nX;
                nZ = -nZ;
            }
            jogadorSimulado.x += nX * passo;
            jogadorSimulado.z += nZ * passo;

            // Verifica se ha colisao na posicao simulada
            if (!checaColisoesBarreiras(&jogadorSimulado, barreiras, 5) && !verificaColisoesComCaixas(caixas, NUM_CAIXAS, jogadorSimulado)) {
                jogador.x = jogadorSimulado.x;
                jogador.z = jogadorSimulado.z;

                // Atualiza a posicao do alvo para manter o foco na direcao da movimentacao
                targetX += nX * passo;
                targetZ += nZ * passo;
            }
            else{
            	jogadorSimulado.x = jogador.x;
            	jogadorSimulado.z = jogador.z;
            	break;
			}
			verificaColisoesComMoedas(moedas, NUM_MOEDAS, jogador, &moedasColetadas, texto);
            break;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}


void giraCamera(int direcao) {
    // angulo de rotacao em radianos (10 graus como exemplo)
    float angulo = (direcao == DIREITA ? -10.0f : 10.0f) * (M_PI / 180.0f);

    // Vetor direcao do alvo relativo a camera
    float dx = targetX - jogador.x;
    float dz = targetZ - jogador.z;

    // Rotacao do vetor direcao
    float novoX = cos(angulo) * dx - sin(angulo) * dz;
    float novoZ = sin(angulo) * dx + cos(angulo) * dz;

    // Atualiza as posicoes do alvo
    targetX = jogador.x + novoX;
    targetZ = jogador.z + novoZ;

    // Re-renderiza a cena
    glutPostRedisplay();
}

void Teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // Frente
            movimenta(FRENTE);
            break;
        case 's': // Tras
            movimenta(TRAS);
            break;
        case 'd': // Direita
            movimenta(DIREITA);
            break;
        case 'a': // Esquerda
            movimenta(ESQUERDA);
            break;
        case 'q': // Vira 10 graus para esquerda
            giraCamera(ESQUERDA);
            break;
        case 'e':
            giraCamera(DIREITA);
            break;
        case '=': // Zoom-in	
            if (angle > 10) angle -= 5;
            break;
        case '-': // Zoom-out
            if (angle < 130) angle += 5;
            break;
        case 27: //Esc
            FinalizaJogo();
            break;
    }
    glutPostRedisplay();
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Fps - Pedro Pizzi");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(MouseClick); // Registra callback do mouse
    glutTimerFunc(16, AtualizaCena, 0); // Inicia o loop de atualizacao
    glutKeyboardFunc(Teclado);
    Inicializa();
    glutMainLoop();
}

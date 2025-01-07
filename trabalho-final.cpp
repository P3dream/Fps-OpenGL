#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

// Variáveis globais
GLfloat angle, fAspect;
float targetX = 0.0f, targetY = 0.0f, targetZ = 0.0f;
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 0.0f; // Posição da câmera
float dirX = 0.0f, dirY = 0.0f, dirZ = -1.0f;         // Direção de visão
float speed = 3.0f;                                   // Velocidade de movimento

// Função para normalizar o vetor de direção
void NormalizaDirecao() {
    float magnitude = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    dirX /= magnitude;
    dirY /= magnitude;
    dirZ /= magnitude;
}

// Função callback chamada para fazer o desenho
void Desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Atualiza a posição da câmera
    gluLookAt(cameraX, cameraY, cameraZ, 
              cameraX + dirX, cameraY + dirY, cameraZ + dirZ, 
              0, 1, 0);

    // Desenha os eixos
    glDisable(GL_LIGHTING); // Desativa a iluminação para os eixos
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
    glEnable(GL_LIGHTING); // Reativa a iluminação

    // Primeiro teapot
    glColor3f(0.0f, 0.0f, 1.0f);
    glEnable(GL_LIGHT2);
    glPushMatrix();
    glTranslatef(-100.0f, 0.0f, 0.0f);
    glutSolidTeapot(25.0f);
    glPopMatrix();

    // Segundo teapot
    glColor3f(1.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(100.0f, 0.0f, 0.0f);
    glutSolidTeapot(25.0f);
    glPopMatrix();

    glutSwapBuffers();
}


// Inicializa parâmetros de rendering
void Inicializa (void) { 
    GLfloat luzAmbiente[4] = {0.2, 0.2, 0.2, 1.0}; 
    GLfloat luzDifusa[4] = {0.0, 1.0, 1.0, 1.0};   // "cor" 
    GLfloat luzEspecular[4] = {0.0, 1.0, 0.0, 1.0}; // "brilho" 
    GLfloat posicaoLuz[4] = {-100.0, 200.0, 200.0, 1.0};

    GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0}; 
    GLint especMaterial = 60;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fundo branco
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);  
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    angle = 45;
}


// Especifica o volume de visualização
void EspecificaParametrosVisualizacao(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.4, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, targetX, targetY, targetZ, 0, 1, 0);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    EspecificaParametrosVisualizacao();
}

// Função para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && angle > 10) 
        angle -= 5; // Zoom-in
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && angle < 130) 
        angle += 5; // Zoom-out

    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

// Função para gerenciar teclas especiais (setas)
void GerenciaTeclado(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            targetY += 1.0f;
            break;
        case GLUT_KEY_DOWN:
            targetY -= 1.0f;
            break;
        case GLUT_KEY_LEFT:
            targetX -= 1.0f;
            break;
        case GLUT_KEY_RIGHT:
            targetX += 1.0f;
            break;
    }
    glutPostRedisplay();
}

void RotacionaCamera(float angulo, char eixo) {
    float rad = angulo * M_PI / 180.0f; // Converte o ângulo para radianos
    float novoDirX, novoDirZ;

    if (eixo == 'y') { // Rotação em torno do eixo Y (horizontal)
        novoDirX = dirX * cos(rad) - dirZ * sin(rad);
        novoDirZ = dirX * sin(rad) + dirZ * cos(rad);
        dirX = novoDirX;
        dirZ = novoDirZ;
    } 
    else if (eixo == 'x') { // Rotação em torno do eixo X (vertical)
        float novoDirY = dirY * cos(rad) - dirZ * sin(rad);
        novoDirZ = dirY * sin(rad) + dirZ * cos(rad);
        dirY = novoDirY;
        dirZ = novoDirZ;
    }

    NormalizaDirecao(); // Normaliza o vetor de direção
}
void CalculaVetorRight(float* rightX, float* rightY, float* rightZ) {
    // Produto vetorial para calcular o vetor à direita
    float upX = 0.0f, upY = 1.0f, upZ = 0.0f; // Vetor "up"
    *rightX = upY * dirZ - upZ * dirY;
    *rightY = upZ * dirX - upX * dirZ;
    *rightZ = upX * dirY - upY * dirX;

    // Normaliza o vetor "right"
    float magnitude = sqrt((*rightX) * (*rightX) + (*rightY) * (*rightY) + (*rightZ) * (*rightZ));
    *rightX /= magnitude;
    *rightY /= magnitude;
    *rightZ /= magnitude;
}

// Função para gerenciar teclas normais (WASD e rotação)
void Teclado(unsigned char key, int x, int y) {
    // Calcula o vetor "right" com base no vetor de direção e "up"
    float rightX, rightY, rightZ;
    CalculaVetorRight(&rightX, &rightY, &rightZ);

    switch (key) {
        case 'a': // Move para frente
            cameraX += dirX * speed;
            cameraY += dirY * speed;
            cameraZ += dirZ * speed;
            break;
        case 'd': // Move para trás
            cameraX -= dirX * speed;
            cameraY -= dirY * speed;
            cameraZ -= dirZ * speed;
            break;
        case 'w': // Move para a esquerda (strafe left)
            cameraX -= rightX * speed;
            cameraY -= rightY * speed;
            cameraZ -= rightZ * speed;
            break;
        case 's': // Move para a direita (strafe right)
            cameraX += rightX * speed;
            cameraY += rightY * speed;
            cameraZ += rightZ * speed;
            break;
        case 'q': // Rotaciona a câmera para cima
            RotacionaCamera(-5.0f, 'x');
            break;
        case 'e': // Rotaciona a câmera para baixo
            RotacionaCamera(5.0f, 'x');
            break;
        case '=': // Zoom-in
            if (angle > 10) angle -= 5;
            break;
        case '-': // Zoom-out
            if (angle < 130) angle += 5;
            break;
    }

    // Atualiza os parâmetros de visualização
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}





// Programa Principal
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Fps - Pedro Pizzi");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(Teclado);
    glutSpecialFunc(GerenciaTeclado);
    Inicializa();
    glutMainLoop();
    return 0;
}

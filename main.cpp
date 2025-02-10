#include <GL/freeglut.h>  
#include <GL/gl.h>        
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <iostream>

float dragonSize = 1.0f;  
std::vector<std::pair<float, float>> dragonBody;  
std::set<char> eatenFruits;  
float fruitPosX = 0.0f, fruitPosY = 0.0f; 
char currentFruit = 'A'; 
bool fruitEaten = false;  

void initOpenGL();
void display();
void keyboard(unsigned char key, int x, int y);
void timer(int);
void createRandomFruit();
void drawFruit(float x, float y);
void checkForFruitCollision();
void moveDragon();

void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);  
    srand(static_cast<unsigned>(time(0)));  
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); 

    if (!fruitEaten) {
        drawFruit(fruitPosX, fruitPosY);
    }

    glColor3f(0.0f, 1.0f, 0.0f);  
    for (size_t i = 0; i < dragonBody.size(); ++i) {
        glBegin(GL_QUADS);
        glVertex2f(dragonBody[i].first - dragonSize / 2, dragonBody[i].second - dragonSize / 2);
        glVertex2f(dragonBody[i].first + dragonSize / 2, dragonBody[i].second - dragonSize / 2);
        glVertex2f(dragonBody[i].first + dragonSize / 2, dragonBody[i].second + dragonSize / 2);
        glVertex2f(dragonBody[i].first - dragonSize / 2, dragonBody[i].second + dragonSize / 2);
        glEnd();
    }

    glutSwapBuffers();  
}

void createRandomFruit() {
    fruitPosX = rand() % 20 - 10;  
    fruitPosY = rand() % 20 - 10;
    currentFruit = 'A' + rand() % 26;
    fruitEaten = false;  
}

void drawFruit(float x, float y) {
    glColor3f(1.0f, 0.0f, 0.0f); 
    glBegin(GL_QUADS);
    glVertex2f(x - 0.5f, y - 0.5f);
    glVertex2f(x + 0.5f, y - 0.5f);
    glVertex2f(x + 0.5f, y + 0.5f);
    glVertex2f(x - 0.5f, y + 0.5f);
    glEnd();
}

void checkForFruitCollision() {
    if (dragonBody.empty()) return;

    float headX = dragonBody[0].first;
    float headY = dragonBody[0].second;

    if (abs(headX - fruitPosX) < 1.0f && abs(headY - fruitPosY) < 1.0f) {
        if (eatenFruits.find(currentFruit) != eatenFruits.end()) {
            
            std::cout << "Game Over! Du åt samma frukt två gånger." << std::endl;
            exit(0);
        } else {
            eatenFruits.insert(currentFruit);
            dragonBody.push_back(dragonBody.back());  
            fruitEaten = true;  
            createRandomFruit(); 
        }
    }
}

void moveDragon() {
    for (size_t i = dragonBody.size() - 1; i > 0; --i) {
        dragonBody[i] = dragonBody[i - 1];
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }

    if (key == 'w') {
        moveDragon();
        dragonBody[0].second += 1.0f;  
    } else if (key == 's') {
        moveDragon();
        dragonBody[0].second -= 1.0f; 
    } else if (key == 'a') {
        moveDragon();
        dragonBody[0].first -= 1.0f;  
    } else if (key == 'd') {
        moveDragon();
        dragonBody[0].first += 1.0f;  
    }

    checkForFruitCollision(); 

    glutPostRedisplay(); 
}

void timer(int value) {
    glutPostRedisplay();  
    glutTimerFunc(16, timer, 0);  
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);  
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);  
    glutInitWindowSize(800, 600);  
    glutCreateWindow("Dragon Game 2D");  
    dragonBody.push_back({0.0f, 0.0f});  

    initOpenGL();  

    glutDisplayFunc(display);  
    glutKeyboardFunc(keyboard);  
    glutTimerFunc(16, timer, 0);  

    createRandomFruit();  

    glutMainLoop();  
    return 0;
}
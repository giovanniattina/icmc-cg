#ifndef __CG_WINDOW_H__
#define __CG_WINDOW_H__
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/// Controla o comportamento da aplicacao quando uma tecla eh pressionada
/// ou solta
/// \param window

typedef struct {
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;

}color_background;

typedef struct{
    GLfloat horizontal, vertical, rotate;
    GLfloat origin_x, origin_y;
}move;

typedef struct{

    GLfloat *matrix;
    int qnt;
}figuras;

void processInput(GLFWwindow *window, color_background *background, move *m);

void initializeGLFW();
GLFWwindow *createWindow(int width, int height);
void move_fig(figuras figs, move *m);
int shader_program();

figuras define_fig();

void run(GLFWwindow *window);



#endif
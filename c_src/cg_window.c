#include <cg_window.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


void processInput(GLFWwindow *window, color_background *background, move *m) {
  /**
    Process user input
  --Input
      windows -> utiliza para pegar a entrada do usuario
      background -> caso queira modificar o fundo da janela
      m -> variavél com dados auxiliar para realizar a transformações
  */
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){//translação para direita
    printf("asa\n");
    m->horizontal = 0.01;
    m->origin_x += 0.01;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){ //translação para esquerda
    m->horizontal = -0.01;
    m->origin_x -= 0.01;

  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){//translação para cima
    m->vertical = 0.01;
    m->origin_y += 0.01;

  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){//translação para baixo
    m->vertical = -0.01;
    m->origin_y -= 0.01;

  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){//rotaciona para esquerda
    m->rotate += 0.001;
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){//rotaciona para direita
    m->rotate -= 0.001;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){//para de rotacionar
    m->rotate = 0;
  }
}

void initializeGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Se estiver em ambiente MAC, descomente a seguinte linha:
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

GLFWwindow *createWindow(int width, int height) {
  // Cria janela
  GLFWwindow *window = glfwCreateWindow(width, height, "CG 2019", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    exit(-2);
  }

  glViewport(0, 0, width, height);
  return window;
}

int shader_program(){
   // vertex shader
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success){
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
  }
  // fragment shader
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success){
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
  }
  // link shaders
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success){
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  
  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  return shaderProgram;
}

figuras define_fig(){
  figuras triangulos;
  triangulos.matrix = malloc(sizeof(GLfloat)*36);
  //left
  triangulos.matrix[0] = -0.2f;
  triangulos.matrix[1] = 0.0f;
  triangulos.matrix[2] = 1.0f;

  triangulos.matrix[3] = -0.7f;
  triangulos.matrix[4] = 0.5f;
  triangulos.matrix[5] = 1.0f;

  triangulos.matrix[6] = -0.7f;
  triangulos.matrix[7] = -0.5f;
  triangulos.matrix[8] = 1.0f;
  //button
  triangulos.matrix[9] = 0.0f;
  triangulos.matrix[10] = -0.2f;
  triangulos.matrix[11] = 1.0f;

  triangulos.matrix[12] = -0.5f;
  triangulos.matrix[13] = -0.7f;
  triangulos.matrix[14] = 1.0f;

  triangulos.matrix[15] = 0.5f;
  triangulos.matrix[16] = -0.7f;
  triangulos.matrix[17] = 1.0f;
  //right
  triangulos.matrix[18] = 0.2f;
  triangulos.matrix[19] = 0.0f;
  triangulos.matrix[20] = 1.0f;

  triangulos.matrix[21] = 0.7f;
  triangulos.matrix[22] = 0.5f;
  triangulos.matrix[23] = 1.0f;

  triangulos.matrix[24] = 0.7f;
  triangulos.matrix[25] = -0.5f;
  triangulos.matrix[26] = 1.0f;
  //up
  triangulos.matrix[27] = 0.0f;
  triangulos.matrix[28] = 0.2f;
  triangulos.matrix[29] = 1.0f;

  triangulos.matrix[30] = 0.5f;
  triangulos.matrix[31] = 0.7f;
  triangulos.matrix[32] = 1.0f;

  triangulos.matrix[33] = -0.5f;
  triangulos.matrix[34] = 0.7f;
  triangulos.matrix[35] = 1.0f;
  

  triangulos.qnt = 4;
  return triangulos;
}

void move_fig(figuras figs, move *m){


  //variaveis auxiliares
  GLfloat x_aux = 0;
  GLfloat y_aux = 0;
  GLfloat z_aux = 0;

  for(int i = 0; i < figs.qnt*9; i+=3){
    //realiza as transformação primeiro para não fica confuso
    x_aux = figs.matrix[i]+m->horizontal;
    y_aux = figs.matrix[i+1]+m->vertical;
    z_aux = figs.matrix[i+2];
    //realiza a rotação translano para o ponto de origem e voltando para a coordenada depois
    figs.matrix[i] = cos(m->rotate)*(x_aux) - sin(m->rotate)*(y_aux) + (m->origin_x*(1.0-cos(m->rotate)) + m->origin_y*sin(m->rotate))*z_aux;
    figs.matrix[i+1] = sin(m->rotate)*(x_aux) + cos(m->rotate)*(y_aux) + (m->origin_y*(1.0-cos(m->rotate)) - m->origin_x*sin(m->rotate))*z_aux;
    printf("%f %f %f \n", figs.matrix[i], figs.matrix[i+1], figs.matrix[i+2]);
  }
  m->horizontal = 0;
  m->vertical = 0;
}

move start_moves(){
  //define todas as variaveis auxiliares como zero
  move m;
  m.horizontal = 0;
  m.vertical = 0;
  m.rotate = 0;
  m.origin_x = 0;
  m.origin_y = 0;
  return m;
}
void draw(const GLfloat* renderArray, size_t nFlots) {
    /**
      Recebe vetor de figuras e quantidade de pontos e desenha no vertex shader
    **/
    unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,  // first Triangle
      1, 2, 3
    };
    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, nFlots * sizeof(GL_FLOAT), renderArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(VAO); 

}

void run(GLFWwindow *window) {
  //defina uma cor para o fundo
  color_background  background;
  background.red = 0.2f;
  background.green = 0.3f;
  background.blue = 0.3f;
  background.alpha = 1.0f;
  //inicializa a figura e as variaveis auxiliares
  figuras trigs = define_fig();
  move m = start_moves();
  //recebe o vertex shader
  int  shaderProgram = shader_program();

  while (!glfwWindowShouldClose(window)) {
    // Comandos de entrada

    processInput(window, &background, &m);
    printf("horizontal %f, vertical %f, rota %f\n", m.horizontal, m.vertical, m.rotate);
    //define cor de fundo
    glClearColor(background.red, background.green, background.blue, background.alpha);
    glClear(GL_COLOR_BUFFER_BIT);

    // Comandos de renderizacao vao aqui
    // Move as figuras dada o input do usuarios
    move_fig(trigs, &m);

    // desenha os trianguls na trela
    draw(trigs.matrix, 9*trigs.qnt);
    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 9*trigs.qnt);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Controla eventos e troca os buffers para renderizacao
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
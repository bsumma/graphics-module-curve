#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common/Angel.h"
#include "shader.h"
#include "Curve.h"


#include <stdlib.h>
#include <stdio.h>

using namespace Angel;

Curve *curve;
int control_point_selected = -1;
int win_width, win_height;


static void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    double click_x = -2.0 + 4.0*(xpos/(double)win_width);
    double click_y = 1.0 - 2.0*(ypos/(double)win_height);
    control_point_selected = curve->test_click(vec2(click_x, click_y));
  }
  if(action == GLFW_RELEASE){
    control_point_selected = -1;
  }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
  if(control_point_selected != -1){
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    double click_x = -2.0 + 4.0*(xpos/(double)win_width);
    double click_y = 1.0 - 2.0*(ypos/(double)win_height);
    curve->updateControlPoint(control_point_selected, vec2(click_x, click_y));
  }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void init(){
  GLuint vertex_shader, fragment_shader, program;
  GLint vpos_location;

  std::string vshader = shader_path + "vshader.glsl";
  std::string fshader = shader_path + "fshader.glsl";
  
  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  check_shader_compilation(vshader, vertex_shader);
  
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  check_shader_compilation(fshader, fragment_shader);
  
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  glLinkProgram(program);
  check_program_link(program);
  
  glUseProgram(program);

  glBindFragDataLocation(program, 0, "fragColor");
  

  curve = new Curve(100, program);
  
}

int main(void)
{
  GLFWwindow* window;
  
  glfwSetErrorCallback(error_callback);
  
  if (!glfwInit())
    exit(EXIT_FAILURE);
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  window = glfwCreateWindow(1024, 512, "Simple example", NULL, NULL);
  if (!window)
    {
    glfwTerminate();
    exit(EXIT_FAILURE);
    }
  
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  glfwSwapInterval(1);

  init();
  
  
  glClearColor(1.0, 1.0, 1.0, 1.0);
  
  //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  mat4 P = Ortho2D(-2.0, 2.0, -1.0, 1.0);
  
  while (!glfwWindowShouldClose(window))
    {
    glfwGetFramebufferSize(window, &win_width, &win_height);
    
    glViewport(0, 0, win_width, win_height);
    glClear(GL_COLOR_BUFFER_BIT);
    
    curve->draw(P);
          
    glfwSwapBuffers(window);
    glfwPollEvents();
    }
  
  glfwDestroyWindow(window);
  
  glfwTerminate();
  exit(EXIT_SUCCESS);
}


// hello_world.cpp
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include "vec.hpp"
#include "mat.hpp"
#include "operator.hpp"
#include "transform.hpp"
#include <stdlib.h>
#include <time.h>
#include <queue>

void init();
void mydisplay();
void myidle();
GLuint create_shader_from_file(const std::string& filename, GLuint shader_type);
void myMenu(int value);
void hilbert_curves(int level);
int random_number(int size);

GLuint program; // 쉐이더 프로그램 객체의 레퍼런스 값
GLint  loc_a_position;
GLint  loc_a_color;
GLint  loc_u_Model;
float  dx = 0.0f, dy = 0.0f;

void myidle()
{
  dx += 0.1f;
  dy += 0.1f;

  if (dx > 1)
	dx = 0.0f;
  if (dy > 1)
	dy = 0.0f;

  glutPostRedisplay();
}

void createContextMenu()
{
  GLint SubMenu_offseting = glutCreateMenu(myMenu);
  glutAddMenuEntry("7", 1);
  glutAddMenuEntry("5", 2);
  glutAddMenuEntry("3", 3);

  GLint SubMenu_minmax = glutCreateMenu(myMenu); 
  glutAddMenuEntry("1/5", 4);
  glutAddMenuEntry("5/15", 5);
  glutAddMenuEntry("15/30", 6);

  GLint SubMenu_ratio = glutCreateMenu(myMenu);
  glutAddMenuEntry("2", 7);
  glutAddMenuEntry("3", 8);
  glutAddMenuEntry("5", 9);

  glutCreateMenu(myMenu);
  glutAddSubMenu("Offsetting", SubMenu_offseting);
  glutAddSubMenu("Min/Max numbers of square gruops in each level", SubMenu_minmax);
  glutAddSubMenu("Ratio of square gruop numbers between consecutive level", SubMenu_ratio);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}


std::queue<float> positions;
float draw_recs[500] = {0,};
int cnt = 0;

//레벨 별로 랜덤 좌표 뽑기
void each_level_random(int level, int how_many)
{
  int size = pow(2, level);
  float filling[size] = { 0, };

  for(int j = 0; j<size; j++)
  {
    filling[j] = positions.front();
    positions.pop();
  }

  for(int i = 0; i<how_many; i++)
  {
    int cx = random_number(size);
    int cy = random_number(size);  

    draw_recs[cnt] = filling[cx];
    cnt++;
    draw_recs[cnt] = filling[cy];
    cnt++;
  }
}

int level = 0;
int how_many = 0;
float offsetting = 0.05f;
int ratio = 2;

int main(int argc, char* argv[])
{

  srand((int)time(0));

  //H1이면 2^1-1 = 1개 (양수만)
  level = 7;
  int size = pow(2, level);

  hilbert_curves(level);

  cnt = 0;
  how_many = 10;
  for(int i = 1; i<=level; i++)
  {
    each_level_random(i, how_many);
  }

  glutInit(&argc, argv);
  glutInitWindowSize(600, 600);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Hello Hilbert^^");

  init();
  glutDisplayFunc(mydisplay);
  createContextMenu();
  glutIdleFunc(myidle);
  glutMainLoop();

  return 0;
}

void myMenu(int value)
{

  switch(value)
  {
    case 1:
        offsetting = 0.07f;
        break;
    case 2:
        offsetting = 0.05f;
        break;
    case 3:
        offsetting = 0.03f;
        break;

    case 4:
        how_many = rand()%5+1;
        break;
    case 5:
        how_many = rand()%15+5;
        break;
    case 6:
        how_many = rand()%30+15;
        break;

    case 7:
        ratio = 2;
        break;
    case 8:
        ratio = 3;
        break;
    case 9:
        ratio = 5;
        break;
  }

}
// GLSL 파일을 읽어서 컴파일한 후 쉐이더 객체를 생성하는 함수
GLuint create_shader_from_file(const std::string& filename, GLuint shader_type)
{
  GLuint shader = 0;
  shader = glCreateShader(shader_type);
  std::ifstream shader_file(filename.c_str());
  std::string shader_string;
  shader_string.assign(
    (std::istreambuf_iterator<char>(shader_file)),
    std::istreambuf_iterator<char>());

  const GLchar* shader_src = shader_string.c_str();

  glShaderSource(shader, 1, (const GLchar**)&shader_src, NULL);
  glCompileShader(shader);

  return shader;
}


void init()
{
  glewInit();

  // 정점 쉐이더 객체를 파일로부터 생성
  GLuint vertex_shader
    = create_shader_from_file("./shader/vertex.glsl", GL_VERTEX_SHADER);

  // 프래그먼트 쉐이더 객체를 파일로부터 생성
  GLuint fragment_shader
    = create_shader_from_file("./shader/fragment.glsl", GL_FRAGMENT_SHADER);

  // 쉐이더 프로그램 생성 및 컴파일
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  loc_a_position = glGetAttribLocation(program, "a_position");
  loc_a_color    = glGetAttribLocation(program, "a_color");
  loc_u_Model    = glGetUniformLocation(program, "u_Model");

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

float colorArray[]={
  222.0f/255.0f,   223.0f/255.0f,   224.0f/255.0f,
  255.0f/255.0f,   143.0f/255.0f,   147.0f/255.0f,
  255.0f/255.0f,   196.0f/255.0f,   205.0f/255.0f,
  201.0f/255.0f,   255.0f/255.0f,   252.0f/255.0f,
  155.0f/255.0f,   242.0f/255.0f,   240.0f/255.0f,
  253.0f/255.0f,   177.0f/255.0f,   187.0f/255.0f,
  238.0f/255.0f,   239.0f/255.0f,   244.0f/255.0f,
  208.0f/255.0f,   206.0f/255.0f,   210.0f/255.0f,
  236.0f/255.0f,   214.0f/255.0f,   216.0f/255.0f,
  226.0f/255.0f,   195.0f/255.0f,   201.0f/255.0f,
};


void draw_a_rectangle(float cx, float cy, float size, float r, float g, float b)
{
  kmuvcl::math::mat4x4f  model_matrix;
  kmuvcl::math::mat4x4f  S, T;

  float position[] = {
     // right-down triangle
     0.5f,  0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f,
     // left-bottom triangle
     0.5f,  0.5f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f,
  };

  float color[] = {
    0.0f, 0.7f, 0.0f, 1.0f,
    0.0f, 0.7f, 0.0f, 1.0f,
    0.0f, 0.7f, 0.0f, 1.0f,
    0.0f, 0.7f, 0.0f, 1.0f,
    0.0f, 0.7f, 0.0f, 1.0f,
    0.0f, 0.7f, 0.0f, 1.0f,
  }; 

  for (unsigned int i=0; i<6; ++i)
  {
    color[4*i + 0] = r;
    color[4*i + 1] = g;
    color[4*i + 2] = b;
    color[4*i + 3] = 1.0f;
  }

  T = kmuvcl::math::translate(cx, cy, 0.0f);
  S = kmuvcl::math::scale(size, size, 1.0f);
  model_matrix = T*S;

  glUniformMatrix4fv(loc_u_Model, 1, GL_FALSE, model_matrix);

  glVertexAttribPointer(loc_a_position, 4, GL_FLOAT, GL_FALSE, 0, position);
  glVertexAttribPointer(loc_a_color, 4, GL_FLOAT, GL_FALSE, 0, color);

  glEnableVertexAttribArray(loc_a_position);
  glEnableVertexAttribArray(loc_a_color);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(loc_a_position);
  glDisableVertexAttribArray(loc_a_color);
}

//겹쳐지는 네모 그리기
void draw_a_rectangle_group(
  float cx, float cy,
  float group_size,
  unsigned int num_rects, 
  float r, float g, float b)
{
  //const float offset = 0.05f;
  int idx = 0;
  float size = group_size;
  for (unsigned int i=0; i<num_rects; ++i)
  {
    draw_a_rectangle(cx, cy, size, r, g, b);
    if (size > offsetting) size -= offsetting;
    //r*=0.9f;
    //g*=0.9f;
    //b*=0.9f;
    if (idx > 27) idx = 0;
    r = colorArray[idx];
    g = colorArray[idx+1];
    b = colorArray[idx+2];
    idx += 3;
  }

}

int random_number(int size)
{
  //srand(time(NULL));
  int temp = rand()%size;

  return temp;
}

void hilbert_curves(int level)
{
  int size = pow(2, level-1);
  int full_size = pow(2, level);
  int cnt = 0;  

  static float val[300] = { 0, };

  if (level == 1) {
    float first_num = 0.5;
    val[0] = first_num;
    val[1] = -first_num;
  }

  else {
    hilbert_curves(level-1);

    float first_num = val[0];
    val[0] = first_num/2;

    for (int i = 0; i < size-1; i++) {
      val[i+1] = val[i] + first_num;
    }
  }

  int scope = size;
  if(level!=1)
  {
    for (int j = 0; j < size; j++) {
      val[scope] = val[j] * (-1);
      scope++;
    }
  }
  
  for (int n = 0; n < full_size; n++) 
  {
    positions.push(val[n]);   
  }
}


void mydisplay()
{
  glLoadIdentity();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,600,600);

  glUseProgram(program);
 
  float group_size = 0.6f;
  int cnt = 0;
  int idx = 0;
  for(int i=0; i<level; i++)
  {
    if(i != 0) group_size /= ratio;
    for(int j=0; j<how_many; j++)
    {
      if(draw_recs[i] !=0)
      {
        if (idx > 27) idx = 0;
        draw_a_rectangle_group(draw_recs[cnt], draw_recs[cnt+1],group_size, 10, colorArray[idx], colorArray[idx+1], colorArray[idx+2]);
        cnt += 2;
	idx += 3;
      }
    }
  }

  glUseProgram(0);

  glutSwapBuffers();
}

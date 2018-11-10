// Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>

#include "Object.h"
#include "Camera.h"
#include "Shader.h"

#include "transform.hpp"
#include "SOIL.h"

void init();
void display();
void reshape(int, int);
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);

GLuint		program;
GLuint		text_program;

GLint		loc_a_vertex;
GLint	  	tex_a_normal;
GLint		tex_a_vertex;
GLint           tex_a_texcoord;

GLint		loc_u_pvm_matrix;
GLint		tex_u_pvm_matrix;
GLint	  	tex_u_view_matrix;
GLint	  	tex_u_model_matrix;
GLint	  	tex_u_normal_matrix;

GLint	  	tex_u_light_vector;

GLint	  	tex_u_light_ambient;
GLint	  	tex_u_light_diffuse;
GLint	  	tex_u_light_specular;

GLint	  	tex_u_material_ambient;
GLint	  	tex_u_material_diffuse;
GLint	  	tex_u_material_specular;
GLint	  	tex_u_material_shininess;

GLint     	tex_u_texid;
GLint     	tex_u_texid_frame;

GLuint    	texid;
GLuint    	texid_frame;

kmuvcl::math::mat4x4f mat_PVM;
kmuvcl::math::mat3x3f mat_Normal;

kmuvcl::math::vec4f light_vector      = kmuvcl::math::vec4f(0.0f, 19.0f, 0.0f);
kmuvcl::math::vec4f light_ambient     = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_diffuse     = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_specular    = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);

kmuvcl::math::vec4f material_ambient  = kmuvcl::math::vec4f(0.0886f, 0.0792f, 0.0671f, 1.0f);
kmuvcl::math::vec4f material_diffuse  = kmuvcl::math::vec4f(0.0886f, 0.0792f, 0.0671f, 1.0f);
kmuvcl::math::vec4f material_specular = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
float     material_shininess= 50.0f;

Object		g_desk, g_fan, g_sofa, g_tv;	// furniture
Camera		g_camera;			// viewer (you)

float angle = 0.0f;

GLuint elephant, mba1, porsche, radar, teddy;
float elephantrot, mba1rot, porscherot, radarrot, teddyrot;
char ch='1';

void loadObj(const char *fname)
{
FILE *fp;
int read;
GLfloat x, y, z;
char ch;
elephant=glGenLists(1);
fp=fopen(fname,"r");
if (!fp) 
    {
        printf("can't open file %s\n", fname);
	  exit(1);
    }
glPointSize(2.0);
glNewList(elephant, GL_COMPILE);
{
glPushMatrix();
glBegin(GL_POINTS);
while(!(feof(fp)))
 {
  read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
  if(read==4&&ch=='v')
  {
   glVertex3f(x,y,z);
  }
 }
glEnd();
}
glPopMatrix();
glEndList();
fclose(fp);
}

void other_loadObj(const char *fname)
{
FILE *fp;
int read;
GLfloat x, y, z;
char ch;
teddy=glGenLists(2);
fp=fopen(fname,"r");
if (!fp) 
    {
        printf("can't open file %s\n", fname);
	  exit(1);
    }
glPointSize(2.0);
glNewList(teddy, GL_COMPILE);
{
glPushMatrix();
glBegin(GL_POINTS);
while(!(feof(fp)))
 {
  read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
  if(read==4&&ch=='v')
  {
   glVertex3f(x,y,z);
  }
 }
glEnd();
}
glPopMatrix();
glEndList();
fclose(fp);
}

void drawCar()
{
 	glPushMatrix();
 	glTranslatef(0,-40.00,-105);
 	glColor3f(1.0,0.23,0.27);
 	glScalef(0.1,0.1,0.1);
 	glRotatef(elephantrot,0,1,0);
 	glCallList(elephant);
 	glPopMatrix();
 	elephantrot=elephantrot+0.6;
 	if(elephantrot>360)elephantrot=elephantrot-360;
}

GLfloat vertices[] = {
  // front
  -1, 1, 1,   1, 1, 1,   1,-1, 1,  -1,-1, 1,
  // back
  1, 1, -1,  -1, 1, -1,  -1,-1, -1,   1,-1, -1,
  // top
  -1, 1, -1,   1, 1, -1,   1, 1, 1,  -1, 1, 1,
  // bottom
  -1,-1, 1,   1,-1, 1,   1,-1, -1,  -1,-1, -1,
  // right
  1, 1, 1,   1, 1, -1,   1,-1, -1,   1,-1, 1,
  // left
  -1, 1, -1,  -1, 1, 1,  -1,-1, 1,  -1,-1, -1,
};


GLfloat normals[] = {
  //front
  0.0, 0.0, -4.0,    0.0, 0.0, -4.0, 
  //back
  0.0, 0.0, 4.0,    0.0, 0.0, 4.0, 
  //top
  0.0, -4.0, 0.0,    0.0, -4.0, 0.0,
  //bottom
  -0.0, 4.0, 0.0,    0.0, 4.0, 0.0,  
  //right
  -4.0, 0.0, -0.0,    -4.0, 0.0, 0.0,  
  //left
  4.0, 0.0, -0.0,    4.0, 0.0, 0.0, 
};


GLfloat texcoords[] = {
  // front
  0,1,  1,1,  1,0,  0,0,
  // back
  0,1,  1,1,  1,0,  0,0,
  // top
  0,1,  1,1,  1,0,  0,0,
  // bottom
  0,1,  1,1,  1,0,  0,0,
  // right
  0,1,  1,1,  1,0,  0,0,
  // left
  0,1,  1,1,  1,0,  0,0,
};

GLushort indices[] = {
  //front
  0, 3, 2,   2, 1, 0,
  //back
  4, 7, 6,   6, 5, 4,
  // top
  8,11,10,  10, 9, 8,
  // bottom
  12,15,14,  14,13,12,
  //right
  16,19,18,  18,17,16,
  //left
  20,23,22,  22,21,20,
};


int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);  
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 640);  
  glutCreateWindow("Strange Museum");
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(idle);

  if (glewInit() != GLEW_OK) 
    {
    std::cerr << "failed to initialize glew" << std::endl;
	return -1;
    }
  	
  init();
  
  glutMainLoop();

  return 0;
}

void init()
{
  g_desk.load_simple_obj("./data/desk.obj");
  g_fan.load_simple_obj("./data/fan.obj");
  g_sofa.load_simple_obj("./data/sofa.obj");
  g_tv.load_simple_obj("./data/tv.obj");
  loadObj("./data/elepham.obj");//replace porsche.obj with radar.obj or any other .obj to display it
  other_loadObj("./data/teddy.obj");

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);

  program = Shader::create_program("./shader/simple.vert", "./shader/simple.frag");
  text_program = Shader::create_program("./shader/texture.vert", "./shader/texture.frag");
  
	
  loc_u_pvm_matrix 	   = glGetUniformLocation(program, "u_pvm_matrix");
  tex_u_pvm_matrix 	   = glGetUniformLocation(text_program, "u_pvm_matrix");
  tex_u_view_matrix        = glGetUniformLocation(text_program, "u_view_matrix");
  tex_u_model_matrix       = glGetUniformLocation(text_program, "u_model_matrix");
  tex_u_normal_matrix      = glGetUniformLocation(text_program, "u_normal_matrix");

  tex_u_light_vector       = glGetUniformLocation(text_program, "u_light_vector");
  tex_u_light_ambient      = glGetUniformLocation(text_program, "u_light_ambient");
  tex_u_light_diffuse      = glGetUniformLocation(text_program, "u_light_diffuse");
  tex_u_light_specular     = glGetUniformLocation(text_program, "u_light_specular");
  
  tex_u_material_ambient   = glGetUniformLocation(text_program, "u_material_ambient");
  tex_u_material_diffuse   = glGetUniformLocation(text_program, "u_material_diffuse");
  tex_u_material_specular  = glGetUniformLocation(text_program, "u_material_specular");
  tex_u_material_shininess = glGetUniformLocation(text_program, "u_material_shininess");

  tex_u_texid      = glGetUniformLocation(text_program, "u_texid");

  loc_a_vertex 	   = glGetAttribLocation(program, "a_vertex");
  tex_a_vertex 	   = glGetAttribLocation(text_program, "a_vertex");
  tex_a_normal     = glGetAttribLocation(text_program, "a_normal");
  tex_a_texcoord   = glGetAttribLocation(text_program, "a_texcoord");

  int width = 0, height = 0, channels;
  unsigned char* image = SOIL_load_image("./data/wall.jpg", 
    &width, &height, &channels, SOIL_LOAD_RGB);

  // TODO: generate texture
  glGenTextures(1, &texid);
  glBindTexture(GL_TEXTURE_2D, texid);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);

  if(image)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    
  }
  else
  {
    std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
  }

  SOIL_free_image_data(image);

}

void draw_furniture(float x, float y, float z, float size)
{
  kmuvcl::math::mat4x4f	model_matrix, T, R, S;

  T = kmuvcl::math::translate(x, y, z);
  S = kmuvcl::math::scale(size, size, size);
  model_matrix = T*S;

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, GL_FALSE, model_matrix);
}

void draw_wall()
{
  glVertexAttribPointer(tex_a_vertex,   3, GL_FLOAT, GL_FALSE, 0, vertices);
  glVertexAttribPointer(tex_a_normal,   3, GL_FLOAT, GL_FALSE, 0, normals);
  glVertexAttribPointer(tex_a_texcoord, 2, GL_FLOAT, GL_FALSE, 0, texcoords);

  glEnableVertexAttribArray(tex_a_vertex);
  glEnableVertexAttribArray(tex_a_normal);
  glEnableVertexAttribArray(tex_a_texcoord);

  glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, indices);

  glDisableVertexAttribArray(tex_a_vertex);
  glDisableVertexAttribArray(tex_a_normal);
  glDisableVertexAttribArray(tex_a_texcoord);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  // Camera setting
  kmuvcl::math::mat4x4f   mat_Proj, mat_View, mat_Model;

  // camera extrinsic param
  mat_View = kmuvcl::math::lookAt(
		// eye position
		g_camera.position()(0), g_camera.position()(1), g_camera.position()(2),
		// center position
		g_camera.center_position()(0), g_camera.center_position()(1), g_camera.center_position()(2),
		// up direction 
		g_camera.up_direction()(0), g_camera.up_direction()(1), g_camera.up_direction()(2)
		);

  // camera intrinsic param
  mat_Proj = kmuvcl::math::perspective(g_camera.fovy(), 1.0f, 0.001f, 10000.0f);

  mat_Model = kmuvcl::math::mat4x4f(1.0f);

  mat_PVM = mat_Proj*mat_View*mat_Model;
  
  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);
	 	
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);

  kmuvcl::math::mat4x4f mat_View_inv = kmuvcl::math::inverse(mat_View);

  glUseProgram(0);
  glUseProgram(text_program);

  glUniformMatrix4fv(tex_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(tex_u_model_matrix, 1, false, mat_Model);   
  glUniformMatrix4fv(tex_u_view_matrix, 1, false, mat_View_inv);   
  glUniformMatrix3fv(tex_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(tex_u_light_vector, 1, light_vector);
  glUniform4fv(tex_u_light_ambient, 1, light_ambient);
  glUniform4fv(tex_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(tex_u_light_specular, 1, light_specular);

  glUniform4fv(tex_u_material_ambient, 1, material_ambient);
  glUniform4fv(tex_u_material_diffuse, 1, material_diffuse);
  glUniform4fv(tex_u_material_specular, 1, material_specular);
  glUniform1f(tex_u_material_shininess, material_shininess);
 
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for wireframe rendering  

  // TODO: using texture 
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texid);
  glUniform1i(tex_u_texid, 0);

  kmuvcl::math::mat4x4f	model_matrix, T, R, S, pvm;

  //right wall
  T = kmuvcl::math::translate(20.0f, 10.0f, -10.0f);
  R = kmuvcl::math::rotate(90.0f, 0.0f, 1.0f, 0.0f);
  S = kmuvcl::math::scale(10.0f, 10.0f, 0.0f);
  model_matrix = T*R*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();

  T = kmuvcl::math::translate(20.0f, 10.0f, 10.0f);
  R = kmuvcl::math::rotate(90.0f, 0.0f, 1.0f, 0.0f);
  S = kmuvcl::math::scale(10.0f, 10.0f, 0.0f);
  model_matrix = T*R*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();


  //left wall
  T = kmuvcl::math::translate(-20.0f, 10.0f, -10.0f);
  R = kmuvcl::math::rotate(90.0f, 0.0f, 1.0f, 0.0f);
  S = kmuvcl::math::scale(10.0f, 10.0f, 0.0f);
  model_matrix = T*R*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();

  T = kmuvcl::math::translate(-20.0f, 10.0f, 10.0f);
  R = kmuvcl::math::rotate(90.0f, 0.0f, 1.0f, 0.0f);
  S = kmuvcl::math::scale(10.0f, 10.0f, 0.0f);
  model_matrix = T*R*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();



  //center wall
  T = kmuvcl::math::translate(-10.0f, 10.0f, -20.0f);
  S = kmuvcl::math::scale(10.0f, 10.0f, 0.0f);
  model_matrix = T*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();


  T = kmuvcl::math::translate(10.0f, 10.0f, -20.0f);
  S = kmuvcl::math::scale(10.0f, 10.0f, 0.0f);
  model_matrix = T*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();

  //ceiling
  T = kmuvcl::math::translate(0.0f, 20.0f, 0.0f);
  R = kmuvcl::math::rotate(90.0f, 1.0f, 0.0f, 0.0f);
  S = kmuvcl::math::scale(20.0f, 20.0f, 0.0f);
  model_matrix = T*R*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();

  //bottom
  T = kmuvcl::math::translate(0.0f, 0.0f, 0.0f);
  R = kmuvcl::math::rotate(90.0f, 1.0f, 0.0f, 0.0f);
  S = kmuvcl::math::scale(20.0f, 20.0f, 0.0f);
  model_matrix = T*R*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();


  //front wall
  T = kmuvcl::math::translate(-10.0f, 10.0f, 20.0f);
  S = kmuvcl::math::scale(10.0f, 10.0f, 0.0f);
  model_matrix = T*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();


  T = kmuvcl::math::translate(10.0f, 10.0f, 20.0f);
  S = kmuvcl::math::scale(10.0f, 10.0f, 0.0f);
  model_matrix = T*S;
  pvm = mat_Proj*mat_View*model_matrix; 
  
  glUniformMatrix4fv(tex_u_pvm_matrix, 1, GL_FALSE, pvm);
  draw_wall();

  glUseProgram(0);

  // TODO: draw furniture by properly transforming each object

  glUseProgram(program);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

  //desk
  T = kmuvcl::math::translate(-5.0f, 0.0f, 0.0f);
  S = kmuvcl::math::scale(1.5f, 1.5f, 1.5f);
  model_matrix = T*S;  
  pvm = mat_Proj*mat_View*model_matrix;  

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, GL_FALSE, pvm);
  g_desk.draw(loc_a_vertex);

  //fan
  T = kmuvcl::math::translate(0.0f, 18.0f, 0.0f);
  R = kmuvcl::math::rotate(angle, 0.0f, 1.0f, 0.0f);
  S = kmuvcl::math::scale(1.5f, 1.5f, 1.5f);
  model_matrix = T*R*S;  
  pvm = mat_Proj*mat_View*model_matrix;  
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, GL_FALSE, pvm);
  g_fan.draw(loc_a_vertex);

  //sofa
  T = kmuvcl::math::translate(0.0f, 0.0f, 5.0f);
  R = kmuvcl::math::rotate(180.0f, 0.0f, 1.0f, 0.0f);
  S = kmuvcl::math::scale(1.5f, 1.5f, 1.5f);
  model_matrix = T*R*S;  
  pvm = mat_Proj*mat_View*model_matrix;  
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, GL_FALSE, pvm);
  g_sofa.draw(loc_a_vertex);

  //tv
  T = kmuvcl::math::translate(0.0f, 0.0f, -5.0f);
  S = kmuvcl::math::scale(2.0f, 2.0f, 2.0f);
  model_matrix = T*S;  
  pvm = mat_Proj*mat_View*model_matrix;  
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, GL_FALSE, pvm);
  g_tv.draw(loc_a_vertex);

  glLoadIdentity();

  //elephant
  T = kmuvcl::math::translate(10.0f, 0.0f, -7.0f);
  S = kmuvcl::math::scale(0.01f, 0.01f, 0.01f);
  model_matrix = T*S;  
  pvm = mat_Proj*mat_View*model_matrix;  
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, GL_FALSE, pvm);
  glCallList(elephant);

  //teddy
  T = kmuvcl::math::translate(-15.0f, 4.0f, -15.0f);
  R = kmuvcl::math::rotate(45.0f, 0.0f, 1.0f, 0.0f);
  S = kmuvcl::math::scale(0.2f, 0.2f, 0.2f);
  model_matrix = T*R*S;  
  pvm = mat_Proj*mat_View*model_matrix;  
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, GL_FALSE, pvm);
  glCallList(teddy);

  glUseProgram(0);
  Shader::check_gl_error("draw");

  glutSwapBuffers();
}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
}

float cnt = 0.0f;
void keyboard(unsigned char key, int x, int y)
{
  if(key == 'a' || key == 'A')
  {
    g_camera.rotate_left(1.0f);
    glutPostRedisplay();
  }
  else if(key == 'd' || key == 'D')
  {
    g_camera.rotate_right(1.0f);
    glutPostRedisplay();
  }
  else if(key == 'w' || key == 'W')
  {
    if(cnt < 44.0f)
    {
      cnt += 1.0f;
      g_camera.rotate_up(1.0f);
      glutPostRedisplay();
    }
  }
  else if(key == 's' || key == 'S')
  {
    if(cnt > 0)
    {
      cnt -= 1.0f;
      g_camera.rotate_down(1.0f);
      glutPostRedisplay();
    }
  }
  else if(key == 27) exit(1);
  else if(key == 'z' || key == 'Z')
  {
    g_camera.reset();
    glutPostRedisplay();
  }
}

float front = 0.0f, back = 0.0f;
void special(int key, int x, int y)
{
  // TODO: properly handle special keyboard event
  switch(key)
  {
    case GLUT_KEY_UP:
	g_camera.move_forward(0.2);
	break;
    case GLUT_KEY_DOWN:
	g_camera.move_backward(0.2);
	break;
    case GLUT_KEY_LEFT:
	g_camera.move_left(0.2);
	break;
    case GLUT_KEY_RIGHT:
	g_camera.move_right(0.2);
	break;
  }
  glutPostRedisplay();
}

void idle()
{
  // TODO: update your variables in idle time
  angle += 0.5f;

  if(angle > 360.0f)
	angle = 0.0f;

  //draw to update data
  //message to OS
  //mydisplay(); 
  glutPostRedisplay();
}

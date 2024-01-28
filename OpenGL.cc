/*********************************************************/
/*                    openGL.cc                          */
/*********************************************************/
/*                                                       */
/*********************************************************/ 

/* inclusion des fichiers d'en-tete Glut */
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jerror.h>

#include <vector>
#include "point.h"
#include "color.h"
#include "graham.h"

#define Pi 3.141592654

double Scal=1.5;

double trX=0.0,trY=0.0,dist=0.;//,trZ=0.0
char presse;
int anglex,angley,x,y,xold,yold;

/* Prototype des fonctions */
void affichage();//  procedure a modifier en fonction de la scene
void clavier(unsigned char touche,int x,int y); 
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);

//-************************************************************
//
//  Procedure avec mise en file des sommets des primitives
//
//-***********************************************************
void init();

int main(int argc,char **argv)
{
  /* initialisation de glut et creation de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(1000,1000);
  glutCreateWindow("Projet 3A envelope convexe.");

  /* Initialisation d'OpenGL */
  glClearColor(1.0,1.0,1.0,0.0);
  glColor3f(0.0,0.0,0.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);
  glColor3f(0.0,0.0,0.0);

  //glEdgeFlag(GL_FALSE);
  glEdgeFlag(GL_TRUE);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
  
  // glEnable(GL_LIGHTING);
  glDisable(GL_LIGHTING);


  /* enregistrement des fonctions de rappel */
  init();
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);


  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}


void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    case 'q' : /*la touche 'q' permet de quitter le programme */
        exit(0);
    case '+' :
      dist+=0.5;
      Scal=Scal+0.5;
      glutPostRedisplay();
      break;
    case '-' :
      dist-=0.5;
      Scal=Scal-0.5;
      glutPostRedisplay();
      break;
    case '6' : trX+=0.25;  glutPostRedisplay();  break;
    case '4' : trX-=0.25;  glutPostRedisplay();  break;
    case '8' : trY+=0.25;  glutPostRedisplay();  break;
    case '2' : trY-=0.25;  glutPostRedisplay();  break;
    }
}



void reshape(int x,int y)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //taille de la scene
  double  Ortho=-150;
  glOrtho(-Ortho,Ortho,-Ortho,Ortho,-Ortho,Ortho);// fenetre
  glMatrixMode(GL_MODELVIEW);
  glViewport(0,0,x,y);
}

void mouse(int button, int state,int x,int y)
{
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
  {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
    presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
  {
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
	 en fonction de la position actuelle de la souris et de la derniere
	 position sauvegardee */
      anglex=anglex+(x-xold); 
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }
    
    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
  }



/****************************************************************
 **                                                            **
 **                    Affichage de  la scene                  **
 **                                                            **
 ****************************************************************/

void openGL(const Point& point, const Color& color, double size)
{
  glColor3f(color.red, color.green, color.blue);	//initialisation de la couleur
	glPointSize(size);	// initialisation de la taille
	glBegin(GL_POINTS);	// on trace un point
	glVertex2f(point.x, point.y);	// coordonnees du point
	glEnd();	// fin de glBegin
}

void trace_segment(const Point& point1, const Point& point2, const Color& color, double size)
{
  glColor3f(color.red, color.green, color.blue);//initialisation de la couleur
	glLineWidth(size); // initialisation de la taille
	glBegin(GL_LINES); // on trace un segment
	glVertex2f(point1.x, point1.y); // coordonnees du premier point
  glVertex2f(point2.x, point2.y); // coordonnees du dernier point 
	glEnd(); // fin de glBegin
}

	
//fonction ou les objets sont a definir
void init()
{
  vector<Point> points = {
    {14, 7}, {53, 29}, {92, 43}, {68, 18}, {41, 56}, {7, 83}, {22, 39}, {61, 74},
    {87, 32}, {49, 92}, {3, 15}, {36, 48}, {79, 64}, {12, 28}, {94, 52}, {58, 9},
    {25, 71}, {69, 36}, {43, 83}, {9, 25}, {61, 47}, {85, 18}, {34, 63}, {72, 41},
    {18, 74}, {49, 13}, {95, 27}, {28, 57}, {67, 31}, {40, 86}, {8, 52}, {57, 14},
    {23, 68}, {78, 43}, {46, 96}, {4, 20}, {33, 58}, {71, 84}, {15, 37}, {92, 63},
    {56, 29}, {27, 92}, {83, 18}, {38, 74}, {11, 46}, {62, 81}, {89, 37}, {48, 64},
    {21, 53}, {73, 27}, {44, 78}, {2, 34}, {37, 59}, {81, 23}, {14, 68}, {52, 42},
    {96, 12}, {69, 87}, {32, 56}, {66, 9}, {47, 34}, {89, 51}, {23, 65}, {75, 48},
    {19, 93}, {59, 26}, {84, 37}, {29, 78}, {11, 49}, {46, 70}, {92, 18}, {57, 34},
    {25, 68}, {70, 11}, {42, 96}, {6, 32}, {35, 76}, {81, 19}, {15, 57}, {66, 42},
    {38, 89}, {3, 27}, {77, 13}, {50, 64}, {94, 31}, {19, 73}, {54, 36}, {29, 84},
    {86, 24}, {63, 47}, {17, 92}, {45, 18}, {78, 41}, {13, 54}, {91, 29}, {55, 68},
    {24, 35}, {72, 9}, {8, 42}, {37, 89}, {68, 22}, {43, 56}, {9, 84}, {61, 38}
  };

  Graham graham(points); graham.convexHull();
  vector<Point> convex_points = graham.getConvexHullPoints();

  glNewList(1,GL_COMPILE_AND_EXECUTE); //liste numero 1
  for(Point& point : points){
    openGL(point, {1.,0.,0.}, 5.);
  }
  glEndList();

  glNewList(2,GL_COMPILE_AND_EXECUTE);  //liste numero 2
  double centerX = 0.0, centerY = 0.0;
  for(int i=0; i < convex_points.size()-1; i++){
    trace_segment(convex_points[i], convex_points[i+1], {1.0,0.0,1.0}, 2.0);
    // centerX += convex_points[i].x;
    // centerY += convex_points[i].y;
  }
  trace_segment(convex_points.front(), convex_points.back(), {1.0,0.0,1.0}, 2.0);
  // centerX /= points.size();
  // centerY /= points.size();
  // trX = centerX;
  // trY = centerY;
  glEndList();

  cout<<"\n Voila, c'est fini"<<endl;
}   
   
// fonction permettant d'afficher les objets en utilisant des listes   
void affichage() 
{
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef(0.0,0.0,dist);
  // Pour la 3D
  // glRotatef(-angley,1.0,0.0,0.0);
  //  glRotatef(-anglex,0.0,1.0,0.0);
  // Pour la 2D
  glRotatef(-anglex+angley,0.0,0.0,1.0);
  glScalef(Scal,Scal,Scal); // diminution de la vue de la scene
  glRotatef(180,0.0,1.0,0.0);
  glRotatef(180,1.0,0.0,0.0);
  glTranslatef(trX,trY,0.);
  glCallList(1); // appel de la liste numero 1
  glCallList(2);   // appel de la liste numero 2
  glFlush(); 
  // On echange les buffers
  glutSwapBuffers();
}


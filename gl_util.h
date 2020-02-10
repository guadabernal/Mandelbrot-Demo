#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#ifndef _GL_UTIL_H
#define _GL_UTIL_H

void set3DMode( size_t Width, size_t Height ) {
  glEnable( GL_DEPTH_TEST );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 60.0, (float)Width / Height, 0.01f, 1000 );
  gluLookAt( 0, 0, 40, 0, 0, 0, 0.0, 1.0, 0.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

void set2DMode( size_t Width, size_t Height ) {
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0, Width, 0, Height );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

#endif

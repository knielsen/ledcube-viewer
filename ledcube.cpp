#include <stdlib.h>

#include <QGLWidget>
#include <QVector3D>

#include <qmath.h>
#include <QObject>
#include <QColor>

#include "io.h"


static QVector<QVector3D> vertices;
static QVector<QVector3D> normals;
static QVector<GLushort> faces;

static float colourVec[16][4] = {
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 },
  { 1.0, 1.0, 0.55, 1.0 }
};

static float col_green[4];
static float col_red[4] = { 0.55, 0.025, 0.025};

static void
add_face(QVector3D a, QVector3D b, QVector3D c)
{
  QVector3D normal = QVector3D::normal(a, b, c);
  faces.append(vertices.count());
  vertices.append(a);
  normals.append(normal);
  faces.append(vertices.count());
  vertices.append(b);
  normals.append(normal);
  faces.append(vertices.count());
  vertices.append(c);
  normals.append(normal);
}

void
build_geometry()
{
  static const float D = 0.15;
  static const float B= 150;
  QVector3D a0(2, -1, -1);
  QVector3D b0(0, 3, -1);
  QVector3D c0(-2, -1, -1);
  QVector3D d0(0, 0, 3);
  a0*= 1/B;
  b0*= 1/B;
  c0*= 1/B;
  d0*= 1/B;

  float x = -2*D;
  for (int i = 0; i < 5; ++i, x += D)
  {
    float y = -2*D;
    for (int j = 0; j < 5; ++j, y += D)
    {
      float z = -2*D;
      for (int j = 0; j < 5; ++j, z += D)
      {
        QVector3D tr(x, y, z);
        QVector3D a = a0;
        a += tr;
        QVector3D b = b0;
        b += tr;
        QVector3D c = c0;
        c += tr;
        QVector3D d = d0;
        d += tr;
        add_face(a,c,b);
        add_face(a,d,c);
        add_face(c,d,b);
        add_face(a,b,d);
      }
    }
  }

  QVector3D t1(-2.7*D, -2.4*D, -2.7*D);
  QVector3D t2(-2.7*D, -2.4*D, 2.7*D);
  QVector3D t3(2.7*D, -2.4*D, -2.7*D);
  QVector3D t4(2.7*D, -2.4*D, 2.7*D);
  QVector3D b1(-2.7*D, -3.3*D, -2.7*D);
  QVector3D b2(-2.7*D, -3.3*D, 2.7*D);
  QVector3D b3(2.7*D, -3.3*D, -2.7*D);
  QVector3D b4(2.7*D, -3.3*D, 2.7*D);
  add_face(t1,t2,t3);
  add_face(t3,t2,t4);
  add_face(t2,t1,b1);
  add_face(b2,t2,b1);
  add_face(t1,b3,b1);
  add_face(t1,t3,b3);
  add_face(t3,t4,b4);
  add_face(t3,b4,b3);
  add_face(t4,t2,b2);
  add_face(b2,b4,t4);
  add_face(b1,b3,b2);
  add_face(b3,b4,b2);

  for (int i= 0; i < 16; i++)
  {
    for (int j= 0; j < 3; j++)
    {
      colourVec[i][j] *= (i+2)*(i+2)/(17.0*17.0);
    }
  }
  QColor tmp_green= QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
  col_green[0]= tmp_green.redF();
  col_green[1]= tmp_green.greenF();
  col_green[2]= tmp_green.blueF();
  col_green[3]= tmp_green.alphaF();
}

void
draw_ledcube()
{
  glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
  glNormalPointer(GL_FLOAT, 0, normals.constData());

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  const uint8_t *frame= get_current_frame();

  const GLushort *indices = faces.constData();
  int i= 0;
  for (int z= 0; z < 5; ++z)
    for (int y= 0; y < 5; ++y)
      for (int x= 0; x < 5; ++x)
      {
        uint8_t col= frame[y * (11*11) + z * 11 + x];
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colourVec[col]);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, indices + 12*i);
        ++i;
      }

  release_frame();

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col_red);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices + 12*i);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

#include <stdlib.h>

#include <QGLWidget>
#include <QVector3D>

#include <qmath.h>
#include <QObject>
#include <QColor>

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

  for (int i= 0; i < 16; i++)
  {
    for (int j= 0; j < 3; j++)
    {
      colourVec[i][j] *= ((15-i)*(15-i))/225.0;
    }
  }
}

void
draw_ledcube()
{
  glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
  glNormalPointer(GL_FLOAT, 0, normals.constData());

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  const GLushort *indices = faces.constData();
  for (int i= 0; i < 5*5*5; i++)
  {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 colourVec[i % 16]);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, indices + 12*i);
  }

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

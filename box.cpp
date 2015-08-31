#include "stdafx.h"
#include "box.h"


//box::box()
//{
//}


box::~box()
{
}

void box::createGeometry(){
	//Vec3f startpoint = Vec3f(0.0-width/2,0.0-length/2,0.0-height/2);
	//if (length_seg == 1 && width_seg == 1 && height_seg==1){
	//	//points
	//	m_verts.resize(8);
	//	m_verts[0] = startpoint;
	//	m_verts[1] = startpoint+Vec3f(width,0.0,0.0);
	//	m_verts[2] = startpoint + Vec3f(0.0, length, 0.0);
	//	m_verts[3] = startpoint + Vec3f(width, length, 0.0);
	//	m_verts[4] = startpoint + Vec3f(0.0, 0.0, height);
	//	m_verts[5] = startpoint + Vec3f(width, 0.0, height);
	//	m_verts[6] = startpoint + Vec3f(0.0, length, height);
	//	m_verts[7] = startpoint + Vec3f(width, length, height);
	//	//polygons
	//	m_polys.resize(6);
	//	int poly1[6][4] = {
	//		0,1,3,2,
	//		0, 4, 5,1,
	//		0,2,4,4,
	//		1,5,7,3,
	//		2,3,7,6,
	//		4,6,7,5
	//	};

	//	m_polys[0].assign(poly1,poly1+4);
	//	m_polys[1].assign(poly1 + 4, poly1 + 7);
	//	m_polys[2].assign(poly1 + 8, poly1 + 11);
	//	m_polys[3].assign(poly1 + 12, poly1 + 11);
	//	m_polys[4].assign(poly1 + 16, poly1 + 19);
	//	m_polys[5].assign(poly1 + 20, poly1 + 23);
	//	//normal
	//	m_normal.resize(6);
	//	m_normal[0] = Vec3f(0.0,0.0,1.0);
	//	m_normal[2] = Vec3f(0.0,1.0,0.0);
	//	m_normal[3] = Vec3f(1.0, 0.0, 0.0);
	//	m_normal[4] = Vec3f(-1.0, 0.0, 0.0);
	//	m_normal[5] = Vec3f(0.0, -1.0, 0.0);
	//	m_normal[6] = Vec3f(0.0, 0.0, -1.0);
	//	//texture coordinate
	//	Vec3i tiling = Vec3i(1, 1, 1);
	//	m_tex_coord.resize(6);
	//	Vec2f face_coord[4] = { Vec2f(0.0, 0.0), Vec2f(0.0, 1.0), Vec2f(1.0,1.0), Vec2f(0.0, 0.0) };
	//	for (int i = 0; i < 6;i++)
	//	{
	//		m_tex_coord[i].assign(face_coord, face_coord + 4);
	//	}

	//}
	/*int Vertexnumber = 2 * (4+2*(length_seg-1)+)
	int PolygonNumber= 
	for (unsigned int i = PolygonNumber; i < ;i++)
	{
		for (unsigned int j = 0; j < 4;j++)
		{
		}
	}*/
}
void box::draw(){
	glBegin(GL_QUADS);
	for (int i = 0; i < m_polys.size(); i++)
	{
		glNormal3fv(m_normal[i].getPtr());
		for (int j = 0; j < 4; j++)
		{
			glTexCoord2fv(m_tex_coord[i][j].getPtr());
			glVertex3fv(m_verts[m_polys[i][j]].getPtr());
		}
	}
	glEnd();
}
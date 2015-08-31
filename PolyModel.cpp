/*
 *  PolyModel.cpp
 *
 *  Created by Robert Falk
 *
 */

#include "PolyModel.h"

#include "stdafx.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>


#include <GL/glut.h>
#include <cmath>

using namespace std;


//protected funcitons
void PolyModel::SetRotationIndentity(){
	Rotation_M = Mat4f(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
		);
}

void PolyModel::SetTransformIndentity(){
	TransformMatrix = Mat4f(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}


//public function definitions
void PolyModel::computeFaceNormals()
{
	m_face_norms.resize(m_polys.size());
	m_face_con.resize(m_verts.size());
	//traverse polygons
	for (int i = 0; i < m_polys.size(); i++){
		for (int j = 0; j < m_polys[i].size(); j++){
			m_face_con[m_polys[i][j]].push_back(i);
		}
		//get 2 vectors from one polygon
		Vec3f vector1 = m_verts[m_polys[i][1]] - m_verts[m_polys[i][0]];
		Vec3f vector2 = m_verts[m_polys[i][m_polys[i].size()-1]] - m_verts[m_polys[i][0]];
		//get cross product
		Vec3f normal = vector1.cross(vector2);

		normal.normalize();
		m_face_norms[i] = normal;
	}
}

void PolyModel::computeVertexNormals()
{
	m_vert_norms.resize(m_verts.size());
	//traverse verteces
	for (int iVer = 0; iVer < m_verts.size(); iVer++){
		//restore face normals temperaly
		vector<Vec3f> face_norms;
		face_norms.resize(m_face_con[iVer].size());
		for (int i = 0; i < m_face_con[iVer].size(); i++){
			face_norms[i] = m_face_norms[m_face_con[iVer][i]];
		}
		Vec3f ver_norm = Vec3f(0, 0, 0);
		for (int n = 0; n < face_norms.size(); n++){
			ver_norm += face_norms[n];
		}
		ver_norm.normalize();
		m_vert_norms[iVer] = ver_norm;
	}
}

void PolyModel::translate(const Vec3f& o)
{
	//real translate vector
	Vec3f offset=o;
	//translate Matrix
	Mat4f Mat_translate = Mat4f(
		1.0f, 0.0f, 0.0f, offset.x(),
		0.0f, 1.0f, 0.0f, offset.y(),
		0.0f, 0.0f, 1.0f, offset.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	//inverse rotation matrix
	Mat4f IN_Rotation = Rotation_M.Inverse();
	TranslateMatrix = Mat_translate*TranslateMatrix;

	//preform translation
	//TransformMatrix = Parent_M.Inverse()*Rotation_M*(Mat_translate*(IN_Rotation*Parent_M*TransformMatrix));
	TransformMatrix = Parent_M.Inverse()*(Mat_translate*(Parent_M*TransformMatrix));
	/*glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(TransformMatrix.ToArray());*/

	//translate center
	//Vec4f h_center = Vec4f(m_center.x(), m_center.y(), m_center.z(), 1);
	//m_center = HomoGenTransformation(Parent_M.Inverse()*Rotation_M*Mat_translate*IN_Rotation*Parent_M, m_center);
	m_center = HomoGenTransformation(Parent_M.Inverse()*Mat_translate*Parent_M, m_center);
	//m_center = Vec3f(h_center.x(), h_center.y(), h_center.z());

	

}

void PolyModel::rotate(float z)
{
	//SetTransformIndentity();
	//build current rotation matrix
	Mat4f Mat_Rotate = Mat4f(
		cos(z), -sin(z), 0.0f, 0.0f,
		sin(z), cos(z), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	//change rotation matrix
	//Rotation_M = Mat_Rotate*Rotation_M;
	Rotation_M = Mat_Rotate*Rotation_M;
	
	/*preform rotation*/

	//translate matrix
	Mat4f Translate = Mat4f(
		1.0f, 0.0f, 0.0f, -m_center.x(),
		0.0f, 1.0f, 0.0f, -m_center.y(),
		0.0f, 0.0f, 1.0f, -m_center.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	//inverse translate matrix
	Mat4f In_Translate = Mat4f(
		1.0f, 0.0f, 0.0f, m_center.x(),
		0.0f, 1.0f, 0.0f, m_center.y(),
		0.0f, 0.0f, 1.0f, m_center.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);

	/*preform rotation*/
	TransformMatrix = In_Translate*(Mat_Rotate*(Translate*TransformMatrix));

	//rotate direction//
	Vec4f h_move_dir = Vec4f(move_dir.x(), move_dir.y(), move_dir.z(), 1);
	h_move_dir = Mat_Rotate*h_move_dir;
	move_dir = Vec3f(h_move_dir.x(), h_move_dir.y(), h_move_dir.z());
}

void PolyModel::rotate(int degree, Vec3f axis){
	float r_radius = (float)degree / 180 * PI;
	Mat4f Mat_Rotate = Mat4f(
		((1 - cos(r_radius))*axis.x()*axis.x()) + cos(r_radius), ((1 - cos(r_radius))*axis.x()*axis.y()) - axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.x()*axis.z()) + axis.y()*sin(r_radius), 0.0f,
		((1 - cos(r_radius))*axis.x()*axis.y()) + axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.y()*axis.y()) + cos(r_radius), ((1 - cos(r_radius))*axis.y()*axis.z()) - axis.x()*sin(r_radius), 0.0f,
		((1 - cos(r_radius))*axis.x()*axis.z()) - axis.y()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.y()) + axis.x()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.z()) + cos(r_radius), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Mat4f Translate = Mat4f(
		1.0f, 0.0f, 0.0f, -m_center.x(),
		0.0f, 1.0f, 0.0f, -m_center.y(),
		0.0f, 0.0f, 1.0f, -m_center.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Rotation_M = Parent_M.Inverse()*Mat_Rotate*Parent_M*Rotation_M;
	TransformMatrix = Translate.Inverse()*Parent_M.Inverse()*Mat_Rotate*Parent_M*Translate*TransformMatrix;

}

void PolyModel::rotate(float degree, Vec3f axis){
	float r_radius = degree / 180 * PI;
	Mat4f Mat_Rotate = Mat4f(
		((1 - cos(r_radius))*axis.x()*axis.x()) + cos(r_radius), ((1 - cos(r_radius))*axis.x()*axis.y()) - axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.x()*axis.z()) + axis.y()*sin(r_radius), 0.0f,
		((1 - cos(r_radius))*axis.x()*axis.y()) + axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.y()*axis.y()) + cos(r_radius), ((1 - cos(r_radius))*axis.y()*axis.z()) - axis.x()*sin(r_radius), 0.0f,
		((1 - cos(r_radius))*axis.x()*axis.z()) - axis.y()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.y()) + axis.x()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.z()) + cos(r_radius), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Mat4f Translate = Mat4f(
		1.0f, 0.0f, 0.0f, -m_center.x(),
		0.0f, 1.0f, 0.0f, -m_center.y(),
		0.0f, 0.0f, 1.0f, -m_center.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
		//Rotation_M = Parent_M.Inverse()*Mat_Rotate*Parent_M*Temp_RMatrix;
		Rotation_M = Parent_M.Inverse()*Mat_Rotate*Parent_M*Rotation_M;
	TransformMatrix = Translate.Inverse()*Parent_M.Inverse()*Mat_Rotate*Parent_M*Translate*TransformMatrix;

}

void PolyModel::intuitiveRotation(float degree, Vec3f axis){
	float r_radius = degree / 180 * PI;
	Mat4f Mat_Rotate = Mat4f(
		((1 - cos(r_radius))*axis.x()*axis.x()) + cos(r_radius), ((1 - cos(r_radius))*axis.x()*axis.y()) - axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.x()*axis.z()) + axis.y()*sin(r_radius), 0.0f,
		((1 - cos(r_radius))*axis.x()*axis.y()) + axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.y()*axis.y()) + cos(r_radius), ((1 - cos(r_radius))*axis.y()*axis.z()) - axis.x()*sin(r_radius), 0.0f,
		((1 - cos(r_radius))*axis.x()*axis.z()) - axis.y()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.y()) + axis.x()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.z()) + cos(r_radius), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Mat4f Translate = Mat4f(
		1.0f, 0.0f, 0.0f, -m_center.x(),
		0.0f, 1.0f, 0.0f, -m_center.y(),
		0.0f, 0.0f, 1.0f, -m_center.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Rotation_M = Parent_M.Inverse()*Mat_Rotate*Parent_M*Rotation_M;
	TransformMatrix = Translate.Inverse()*Parent_M.Inverse()*Mat_Rotate*Parent_M*Translate*TransformMatrix;
}

void PolyModel::debugdraw(){
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glBegin(GL_TRIANGLES);
	//	for (unsigned int i = 0; i < m_polys.size();i++){
	//		for (unsigned int j = 0; j < m_polys[i].size(); j++){
	//			glVertex3f(m_verts[m_polys[i][j] - 1].x(), m_verts[m_polys[i][j] - 1].y(), m_verts[m_polys[i][j] - 1].z());
	//		}
	//		
	//	}
	//glEnd();
}

void PolyModel::drawPivot(){
	glColor3f(0.0f, 0.0f, 1.0f);
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glVertex3f(m_center.x(), m_center.y(), m_center.z());
	glEnd();

	////draw direction
	glColor3f(0.0f, 1.0f, 1.0f);
	glPointSize(3.0f);

	////draw axis/////////


	//x
	Vec3f dir_point_x = Vec3f(5.0f, 0.0f, 0.0f);
	dir_point_x = HomoGenTransformation(Rotation_M, dir_point_x);
	//
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex3f(m_center.x(), m_center.y(), m_center.z());
	glVertex3f(m_center.x() + dir_point_x.x(), m_center.y() + dir_point_x.y(), m_center.z() + dir_point_x.z());
	glEnd();

	//y
	Vec3f dir_point_y = Vec3f(0.0f, 5.0f, 0.0f);
	dir_point_y = HomoGenTransformation(Rotation_M, dir_point_y);
	//
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(m_center.x(), m_center.y(), m_center.z());
	glVertex3f(m_center.x() + dir_point_y.x(), m_center.y() + dir_point_y.y(), m_center.z() + dir_point_y.z());
	glEnd();

	//z
	Vec3f dir_point_z = Vec3f(0.0f, 0.0f, 5.0f);
	dir_point_z = HomoGenTransformation(Rotation_M, dir_point_z);
	//
	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_LINES);
	glVertex3f(m_center.x(), m_center.y(), m_center.z());
	glVertex3f(m_center.x() + dir_point_z.x(), m_center.y() + dir_point_z.y(), m_center.z() + dir_point_z.z());
	glEnd();
}

void PolyModel::drawBoundingbox(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	Mat4f Translate = Mat4f(
		1.0f, 0.0f, 0.0f, -m_center.x(),
		0.0f, 1.0f, 0.0f, -m_center.y(),
		0.0f, 0.0f, 1.0f, -m_center.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	glLoadMatrixf((Scale_M*Rotation_M*Translate).ToArray());
	//glMultiplyMatrixf(TranslateMatrix.ToArray());
	//glTranslatef(-m_center.x(), -m_center.y(), -m_center.z());
	
	//draw
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);

	//bottom
	glVertex3f(m_min_bounding.x(), m_min_bounding.y(), m_min_bounding.z());
	glVertex3f(m_max_bounding.x(), m_min_bounding.y(), m_min_bounding.z());

	glVertex3f(m_min_bounding.x(), m_min_bounding.y(), m_min_bounding.z());
	glVertex3f(m_min_bounding.x(), m_max_bounding.y(), m_min_bounding.z());

	glVertex3f(m_max_bounding.x(), m_min_bounding.y(), m_min_bounding.z());
	glVertex3f(m_max_bounding.x(), m_max_bounding.y(), m_min_bounding.z());

	glVertex3f(m_min_bounding.x(), m_max_bounding.y(), m_min_bounding.z());
	glVertex3f(m_max_bounding.x(), m_max_bounding.y(), m_min_bounding.z());

	//vertical
	glVertex3f(m_min_bounding.x(), m_min_bounding.y(), m_min_bounding.z());
	glVertex3f(m_min_bounding.x(), m_min_bounding.y(), m_max_bounding.z());

	glVertex3f(m_max_bounding.x(), m_min_bounding.y(), m_min_bounding.z());
	glVertex3f(m_max_bounding.x(), m_min_bounding.y(), m_max_bounding.z());

	glVertex3f(m_max_bounding.x(), m_max_bounding.y(), m_min_bounding.z());
	glVertex3f(m_max_bounding.x(), m_max_bounding.y(), m_max_bounding.z());

	glVertex3f(m_min_bounding.x(), m_max_bounding.y(), m_min_bounding.z());
	glVertex3f(m_min_bounding.x(), m_max_bounding.y(), m_max_bounding.z());

	//top
	glVertex3f(m_min_bounding.x(), m_min_bounding.y(), m_max_bounding.z());
	glVertex3f(m_max_bounding.x(), m_min_bounding.y(), m_max_bounding.z());

	glVertex3f(m_min_bounding.x(), m_min_bounding.y(), m_max_bounding.z());
	glVertex3f(m_min_bounding.x(), m_max_bounding.y(), m_max_bounding.z());

	glVertex3f(m_max_bounding.x(), m_min_bounding.y(), m_max_bounding.z());
	glVertex3f(m_max_bounding.x(), m_max_bounding.y(), m_max_bounding.z());

	glVertex3f(m_min_bounding.x(), m_max_bounding.y(), m_max_bounding.z());
	glVertex3f(m_max_bounding.x(), m_max_bounding.y(), m_max_bounding.z());

	glEnd();

	glPopMatrix();
}

void PolyModel::drawNormal(){
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < m_verts.size(); i++){
		glVertex3fv(m_verts[i].getPtr());
	}
	glEnd();
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < m_verts.size(); i++){
		glVertex3fv(m_verts[i].getPtr());
		glVertex3fv(((m_verts[i] + m_vert_norms[i])).getPtr());
	}
	glEnd();
}

float* PolyModel::dataArray(){
	float *fResult;
	vector<float> VBO;
	for (int i = 0; i < m_polys.size(); i++)
	{
		//check back facing
		if (!m_face_dir[i]){
			continue;
		}
		//traverse lines in each polygon
		for (int j = 0; j < m_polys[i].size(); j++){
			if (j == (m_polys[i].size() - 1)){
				VBO.push_back(m_verts[m_polys[i][j] - 1].x());
				VBO.push_back(m_verts[m_polys[i][j] - 1].y());
				VBO.push_back(m_verts[m_polys[i][j] - 1].z());

				VBO.push_back(m_verts[m_polys[i][0] - 1].x());
				VBO.push_back(m_verts[m_polys[i][0] - 1].y());
				VBO.push_back(m_verts[m_polys[i][0] - 1].z());
				continue;
			}
			VBO.push_back(m_verts[m_polys[i][j] - 1].x());
			VBO.push_back(m_verts[m_polys[i][j] - 1].y());
			VBO.push_back(m_verts[m_polys[i][j] - 1].z());

			VBO.push_back(m_verts[m_polys[i][j+1] - 1].x());
			VBO.push_back(m_verts[m_polys[i][j+1] - 1].y());
			VBO.push_back(m_verts[m_polys[i][j + 1] - 1].z());
		}
	}
	fResult = &VBO[0];
	return fResult;
}

void PolyModel::draw(Vec3f color)
{
	//preform transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	glLineWidth(1.5f);
	//glMultMatrixf(TransformMatrix.ToArray());
	glPushMatrix();
	glLoadMatrixf(TransformMatrix.ToArray());
	//glTranslatef(20.0f, 0.0f, 0.0f);

	// Draw each triangle... //GL_LINE_LOOP
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glBegin(GL_LINE_LOOP);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLES);

		//draw object
		glColor3f(color.r(), color.g(), color.b());
		for (int i = 0; i < m_polys.size(); i++)
		{
			//glNormal3fv(m_face_norms[i].getPtr());
		for (int j = 0; j < m_polys[i].size(); j++){
			glNormal3fv(m_vert_norms[m_polys[i][j]].getPtr());
			glVertex3f(m_verts[m_polys[i][j]].x(), m_verts[m_polys[i][j]].y(), m_verts[m_polys[i][j]].z());
		}
		}
	glEnd();
	glPopMatrix();


	if (false){//debug mode
		////draw center
		glColor3f(0.0f, 0.0f, 1.0f);
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		glVertex3f(m_center.x(), m_center.y(), m_center.z());
		glEnd();

		////draw direction
		glColor3f(0.0f, 1.0f, 1.0f);
		glPointSize(3.0f);

		////draw axis/////////


		//x
		Vec3f dir_point_x = Vec3f(5.0f, 0.0f, 0.0f);
		dir_point_x = HomoGenTransformation(Rotation_M, dir_point_x);
		//
		glBegin(GL_LINES);
		glVertex3f(m_center.x(), m_center.y(), m_center.z());
		glVertex3f(m_center.x() + dir_point_x.x(), m_center.y() + dir_point_x.y(), m_center.z() + dir_point_x.z());
		glEnd();

		//y
		Vec3f dir_point_y = Vec3f(0.0f, 5.0f, 0.0f);
		dir_point_y = HomoGenTransformation(Rotation_M, dir_point_y);
		//
		glBegin(GL_LINES);
		glVertex3f(m_center.x(), m_center.y(), m_center.z());
		glVertex3f(m_center.x() + dir_point_y.x(), m_center.y() + dir_point_y.y(), m_center.z() + dir_point_y.z());
		glEnd();

		Vec3f dir_point_z = Vec3f(5.0f, 0.0f, 0.0f);
		dir_point_z = HomoGenTransformation(Rotation_M, dir_point_z);
		//
		glBegin(GL_LINES);
		glVertex3f(m_center.x(), m_center.y(), m_center.z());
		glVertex3f(m_center.x() + dir_point_z.x(), m_center.y() + dir_point_z.y(), m_center.z() + dir_point_z.z());
		glEnd();

	} //end 


	glFlush();
}


bool PolyModel::loadModel(std::istream& istr, bool reverse)
{
	int vertex_count;
	int face_count;
	string data;

	if (!istr.good())
		return false;

	char line[1024];
	istr.getline(line, 1024);

	std::istringstream sstr(line);
	sstr >> data >> vertex_count >> face_count;

	//store readbuffer
	std::streambuf* backup;
	backup = sstr.rdbuf();
	//get position
	sstr.seekg(0, sstr.cur);
	int cur = sstr.tellg();

	std::string attrib_type;
	bool has_texcoords = false;
	while (!sstr.eof() && sstr.good()) {
		sstr >> attrib_type;
		if (attrib_type == "tex-coords1")
			has_texcoords = true;
	}
	sstr.seekg(cur-1, sstr.beg);
	string test;
	sstr >> test;
	
	m_verts.resize(vertex_count);
	m_polys.resize(face_count);

	
	
    
    // Read in all the points
	//sstr << backup;
	
	float x, y, z;
	for (int i=0; i < vertex_count;i++){
		char new_line[1024];
		istr.getline(new_line, 1024);
		std::istringstream new_sstr(new_line);
		new_sstr >> x >> y >> z;
		m_verts[i] = Vec3f(x, y, z);
	}
    // Read in all the polygons (triangles)
	int vertex_p_poly;
	for (int i=0; i < face_count; i++){
		char new_line[1024];
		istr.getline(new_line, 1024);
		std::istringstream new_sstr(new_line);
		
		try{
			new_sstr >> vertex_p_poly;
		}
		catch (exception){
			vertex_p_poly < 3;
			std::cout << "Invalid Model" << std::endl;
		}
		vector<int> vTemp(vertex_p_poly);
		for (int j=0; j < vertex_p_poly; j++){
			new_sstr >> vTemp[j];
		}
		m_polys[i]=vTemp;
	}
	// Read in all texture coordinates
	if (has_texcoords){
		int Num_cord;
		while (!istr.eof() && istr.good()){
			char new_line[1024];
			istr.getline(new_line, 1024);
			std::istringstream new_sstr(new_line);
			new_sstr >> Num_cord;
			vector<float> fTemp(Num_cord);
			for (int j = 0; j < Num_cord; j++){
				new_sstr >> fTemp[j];
			}
			m_tex_cord.insert(m_tex_cord.end(), Vec2f(fTemp.data()));
		}
	}
	

	/*calculate center point*/

	//check xmin
	//auto x_min_elem = min_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	//{
	//	return s1.x() < s2.x();
	//});
	//float x_min = x_min_elem->x();

	//auto x_max_elem = max_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	//{
	//	return s1.x() < s2.x();
	//});
	//float x_max = x_max_elem->x();
	////check ymin
	//auto y_min_elem = min_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	//{
	//	return s1.y() < s2.y();
	//});
	//float y_min = y_min_elem->y();

	//auto y_max_elem = max_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	//{
	//	return s1.y() < s2.y();
	//});
	//float y_max = y_max_elem->y();

	//auto z_min_elem = min_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	//{
	//	return s1.z() < s2.z();
	//});
	//float z_min = z_min_elem->z();

	//auto z_max_elem = max_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	//{
	//	return s1.z() < s2.z();
	//});
	//float z_max = z_max_elem->z();

	////define bouding box
	//m_max_bounding = Vec3f(x_max,y_max,z_max);
	//m_min_bounding = Vec3f(x_min, y_min, z_min);
	////get center
	//m_center = (m_max_bounding + m_min_bounding) / 2;
	////calculate bounding circle radius
	//radius = sqrtf((x_max - x_min)*(x_max - x_min) + (y_max - y_min)*(y_max - y_min)) / 2;

	//calculate normals
	computeFaceNormals();
	computeVertexNormals();

	//set rotation to identity
	SetRotationIndentity();
	SetTransformIndentity();
	TranslateMatrix = Mat4f(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,  0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	//set move direction  --direction for "w"
	move_dir = Vec3f(0.0f,1.0f,0.0f);
	Scale_M = Mat4f(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Parent_M = Mat4f(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	//
	Temp_RMatrix = Mat4f(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	//getExteriorEdge();
	return true;
}



//bool PolyModel::detectCollisionCircle(Vec3f point1, Vec3f point2){
//	return (PointLineDistance(m_center, point1, point2)=radius);
//}
//
//bool PolyModel::detectCollision(Vec3f point1, Vec3f point2){
//	return true;
//}

//point line distance
float PolyModel::PointLineDistance(Vec3f target, Vec3f point1, Vec3f point2){
	//initate vectors
	Vec3f vector1 = target - point1;
	Vec3f vector2 = point2 - point1;

	//distance 
	Vec3f vector3 = vector1.cross(vector2);
	float distance = vector3.length() / vector2.length();
	return distance;
}

//if consider point- edge distance
bool PolyModel::withinEdgeRange(Vec3f target, Vec3f point1, Vec3f point2){
	bool outsidepoint1 = true;
	bool outsidepoint2 = true;
	//if outside point1
	Vec3f vector1 = target - point1;
	Vec3f vector2 = point2 - point1;
	float dis1 = vector1.dot(vector2) / vector2.length();
	if (dis1>0 && dis1<vector2.length()){
		outsidepoint1 = false;
	}
	//if outside point2
	Vec3f vector3 = target - point2;
	Vec3f vector4 = point1 - point2;
	float dis2 = vector3.dot(vector4) / vector4.length();
	if (dis2>0 && dis2<vector2.length()){
		outsidepoint2 = false;
	}
	return !(outsidepoint1 | outsidepoint2);
}

//collision detection against model

//void PolyModel::getExteriorEdge(){
//	vector<tuple<Vec3f, Vec3f>> EdgeContainer;
//	///////////

Vec3f PolyModel::HomoGenTransformation(Mat4f TransMat, Vec3f Vec){
	Vec4f h_Vec(Vec.x(), Vec.y(), Vec.z(), 1.0f);
	h_Vec = TransMat*h_Vec;
	return Vec3f(h_Vec.x(), h_Vec.y(), h_Vec.z());
}


//scale
void PolyModel::scale(const Vec3f& scale){
	Mat4f scale_Mat(
		scale.x(), 0.0f, 0.0f, 0.0f,
		0.0f, scale.y(), 0.0f, 0.0f,
		0.0f, 0.0f, scale.z(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

void PolyModel::scale(const float& scale_ratio){
	Mat4f scale_Mat1(  //current scale matrix
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1/(1+scale_ratio)
		);
	Mat4f Translate = Mat4f(  //translate to origin
		1.0f, 0.0f, 0.0f, -m_center.x(),
		0.0f, 1.0f, 0.0f, -m_center.y(),
		0.0f, 0.0f, 1.0f, -m_center.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Scale_M = scale_Mat1*Scale_M;

	TransformMatrix = Translate.Inverse()*Rotation_M*scale_Mat1*Rotation_M.Inverse()*Translate*TransformMatrix;


}

void PolyModel::setParentMatrix(Mat4f &Matrix){
	Parent_M = Matrix;
}
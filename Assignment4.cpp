// Assignment4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//define basic parameters  //////////////////////////////
//window related

const int WINDOW_X = 30;       //initate x position
const int WINDOW_Y = 30;       //initate y posiiton
const int WINDOW_WIDTH = 800;  //initate window width
const int WINDOW_HEIGHT = 600; //initate window height

#define ESCAPE 27

//virtual trackball
int Window_width = WINDOW_WIDTH;
int Window_height = WINDOW_HEIGHT;
int curx, cury;
int startX, startY;
Vec3f axis, position;
float angle = 0.0;

static bool trackingMouse = false;
static bool forceRedraw = false;
static bool trackballMoved = false;

Vec3f LastPos = Vec3f(0.0, 0.0, 0.0);

//models
#define DIRECTION "..//Assignment4//Resources//"

#define SHADERDIRECTION "..//Assignment4//"

//model
PolyModel Model1;
PolyModel Model2;
PolyModel Model3;
fbx_loader fbxModel1;
Model testModel;
Model testModel1;
Model testModel2;
Model testModel3;

//parameters
static float move_step = 0.8f;
static float step = 0.2f;
static float rotate_step = 3.14159f / 20.0f;

//test
const unsigned int ProgramNumber = 10;
GLuint ProgramList[ProgramNumber];


//Camera & Light
Camera Camera1;

Light testlight;

//Scene
Scene MainScene;
std::list<GLuint*> ShaderList;

Model *selectedOne;

//texture
const unsigned TextureNum = 4;

image_t texture1;
image_t texture2;
image_t texture3;

GLuint TextureList[TextureNum];

//selection
#define BUF_SIZE 1
GLuint buffer[BUF_SIZE];

///function declaration /////////////
void InitShader();


//// funcitons ////////////////////////////

void Init(){ //init nessary parameters

	//load a polymodel //////////////////
	std::string Model1Name = DIRECTION + std::string("cow.d2");
	std::fstream ifs(Model1Name.c_str());
	Model1.loadModel(ifs, true);
	testModel = convert2Model(Model1,MODEL_UV);
	testModel.Translate(Vec3f(-5.0, -9.0, 4.0));
	testModel.Rotation(90.0f, Vec3f(1.0, 0.0, 0.0));

	std::string Model3Name = DIRECTION + std::string("queen.d2");
	std::fstream ifs2(Model3Name.c_str());
	Model2.loadModel(ifs2, true);
	testModel2 = convert2Model(Model2,MODEL_BASIC);
	testModel2.Translate(Vec3f(-5.0, -3.0, 0.2));
	//testModel2.Rotation(190.0f, Vec3f(0.0, 1.0, 0.0));


	std::string Model4Name = DIRECTION + std::string("car.d2");
	std::fstream ifs4(Model4Name.c_str());
	Model3.loadModel(ifs4, true);
	testModel3 = convert2Model(Model3, MODEL_BASIC);
	testModel3.Translate(Vec3f(2.0,6.0,8.0));
	testModel3.Scale(4.0);
	testModel3.Rotation(90.0f, Vec3f(0.0, 1.0, 0.0));

	//load fbx model
	std::string Model2Name = DIRECTION + std::string("house.fbx");
	//std::fstream ifs1(Model2Name.c_str());
	fbxModel1.LoadModel(Model2Name);
	testModel1 = convert2Model(fbxModel1,MODEL_UV);
	testModel1.Scale(-0.3);
	testModel1.Translate(Vec3f(0.0,0.0,-9.0));
	//testModel2 = convert2Model(Model2);

	//MainScene.AddModel(CowModel);

	///set up camera ////////////////
	Camera1.setPosition(Vec3f(3.0, 5.0, 3.0));
	Camera1.setTarget(Vec3f(0.0, 0.0, 3.0));
	Camera1.setUpVector(Vec3f(0.0, 0.0, 1.0));

	Camera1.SetFrustum(0.2, 120, 0.05, (float)WINDOW_WIDTH / WINDOW_HEIGHT);

	Camera1.CalculateObjSpace();
	//light
	testlight = Point_Light(Vec3f(10.0, 10.0, 10.0), Vec3f(1.0, 1.0, 1.0), 3.0);

	
	//TextureList[1] = tgaLoadAndBind(cstr1, TGA_LOW_QUALITY,2);
	//tgaLoad(cstr1, &texture2, TGA_LOW_QUALITY);
	//delete[] cstr1;

	////////// lighting /////////////
	

	//bind parameters
	InitShader();
	glUseProgram(ProgramList[0]);

	GLuint texLoc;

	texLoc = glGetUniformLocation(ProgramList[0], "BaseMap");
	glUniform1i(texLoc, 0);

	texLoc = glGetUniformLocation(ProgramList[0], "ReflectMap");
	glUniform1i(texLoc, 1);

	texLoc = glGetUniformLocation(ProgramList[0], "AOMap");
	glUniform1i(texLoc, 2);

	//texture map
	glGenTextures(3, TextureList);
	glBindTexture(GL_TEXTURE_2D, TextureList[0]);

	//diffuse map
	const std::string texture1name = DIRECTION + std::string("TilesOrnate0130_5_DIFFUSE.tga");
	char *cstr = new char[texture1name.size() + 1];
	strcpy(cstr, texture1name.c_str());
	glActiveTexture(GL_TEXTURE0);
	//TextureList[0] = tgaLoadAndBind(cstr, TGA_LOW_QUALITY,2);
	tgaLoad(cstr, &texture1, TGA_LOW_QUALITY);
	std::cout << "Length of array = " << (sizeof(texture1.data) / sizeof(unsigned char)) << std::endl;
	tgaUploadImage(&texture1, TGA_LOW_QUALITY);
	tgaFree(&texture1);
	delete[] cstr;

	//specualr level
	glBindTexture(GL_TEXTURE_2D, TextureList[1]);
	const std::string texture2name = DIRECTION + std::string("TilesOrnate0130_5_height_SPEC.tga");
	char *cstr1 = new char[texture2name.size() + 1];
	strcpy(cstr1, texture2name.c_str());
	glActiveTexture(GL_TEXTURE1);
	tgaLoad(cstr1, &texture2, TGA_LOW_QUALITY);
	tgaUploadImage(&texture2, TGA_LOW_QUALITY);
	tgaFree(&texture2);
	delete cstr1;

	//ambient occlusions
	glBindTexture(GL_TEXTURE_2D, TextureList[2]);
	const std::string texture3name = DIRECTION + std::string("TilesOrnate0130_5_height_OCC_No_tiling.tga");
	char *cstr2 = new char[texture3name.size() + 1];
	strcpy(cstr2, texture3name.c_str());
	glActiveTexture(GL_TEXTURE2);
	tgaLoad(cstr2, &texture3, TGA_LOW_QUALITY);
	tgaUploadImage(&texture3, TGA_LOW_QUALITY);
	tgaFree(&texture3);
	delete cstr2;


	float color[] = { 1.0, 0.1, 0.0, 1.0 };
	GLuint ColorID = glGetUniformLocation(ProgramList[0], "color");
	glUniform3fv(ColorID, 4 * sizeof(float), color);

	float ka = 0.4;
	GLuint AmbientStrength = glGetUniformLocation(ProgramList[0], "Ka");
	glUniform1f(AmbientStrength, ka);

	float kd = 0.4;
	GLuint DiffuseStrength = glGetUniformLocation(ProgramList[0], "Kd");
	glUniform1f(DiffuseStrength, kd);

	float ks = 0.6;
	GLuint SpecularStrength = glGetUniformLocation(ProgramList[0], "Ks");
	glUniform1f(SpecularStrength, ks);

	/*int shiness = 4;
	GLuint Shinness = glGetUniformLocation(Program1, "Shinness");
	glUniform1i(Shinness, shiness);*/

	float tiling1[] = { 10.0, 10.0 };
	GLuint Tiling1 = glGetUniformLocation(ProgramList[0], "tiling");
	glUniform2fv(Tiling1, 2 * sizeof(float), tiling1);

	glUseProgram(ProgramList[1]);

	float color1[] = { 1.0, 0.1, 0.0, 1.0 };
	GLuint ColorID1 = glGetUniformLocation(ProgramList[1], "color");
	glUniform3fv(ColorID1, 4 * sizeof(float), color1);

	float ka1 = 0.4;
	GLuint AmbientStrength1 = glGetUniformLocation(ProgramList[1], "Ka");
	glUniform1f(AmbientStrength1, ka1);

	float kd1 = 0.4;
	GLuint DiffuseStrength1 = glGetUniformLocation(ProgramList[1], "Kd");
	glUniform1f(DiffuseStrength1, kd1);

	float ks1 = 0.6;
	GLuint SpecularStrength1 = glGetUniformLocation(ProgramList[1], "Ks");
	glUniform1f(SpecularStrength1, ks1);

	//ward's shader
	glUseProgram(ProgramList[3]);

	float Alpha[] = { 0.2, 0.3 };
	GLuint WardAlpha = glGetUniformLocation(ProgramList[3], "Alpha");
	glUniform2fv(WardAlpha,2*sizeof(float),Alpha);

	float Tangent[] = { 0.2, 0.3,0.4 };
	GLuint WardTangent = glGetUniformLocation(ProgramList[3], "Tangent");
	glUniform3fv(WardAlpha, 3 * sizeof(float), Tangent);

	float binormalDirection[] = { 0.5, 0.6,-0.2 };
	GLuint Wardbinormal = glGetUniformLocation(ProgramList[3], "binormalDirection");
	glUniform3fv(WardAlpha, 3 * sizeof(float), binormalDirection);

}

/////////////// draw scene///////////////////////////////////////////////////////////
void processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint names, *ptr;

	printf("hits = %d\n", hits);
	ptr = (GLuint *)buffer;
	for (i = 0; i < hits; i++) { /*  for each hit  */
		names = *ptr;
		printf(" number of names for hit = %d\n", names); ptr++;
		printf("  z1 is %g;", (float)*ptr / 0x7fffffff); ptr++;
		printf(" z2 is %g\n", (float)*ptr / 0x7fffffff); ptr++;
		printf("   the name is ");
		for (j = 0; j < names; j++) {     /*  for each name */
			printf("%d ", *ptr); ptr++;
		}
		printf("\n");
	}
}

void draw_Objects(){

}

void select(){
	GLuint selectBuf[BUF_SIZE];
	GLint hits;

	glInitNames();
	glPushName(0);
	//glPushMatrix();
	glLoadName(1);
	testModel.ClassicDraw(SMOOTH, UV_LACK);
	glLoadName(2);
	testModel2.ClassicDraw(SMOOTH, UV_LACK);
	glLoadName(3);
	testModel3.ClassicDraw(SMOOTH, UV_LACK);
	glLoadName(4);
	testModel1.ClassicDraw(FLAT, UV_RAW);
	//glPopMatrix();

	glSelectBuffer(BUF_SIZE, selectBuf);
	(void)glRenderMode(GL_SELECT);
}
	
void draw_Scene(){

	//glRenderMode(GL_SELECT);
	// ...render as usual...
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);

	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	int fov = 2 * atan(Camera1.h()*Camera1.Aspect() / Camera1.n()) / PI * 180;
	gluPerspective(fov, Camera1.Aspect(), Camera1.n(), Camera1.f());

	gluLookAt(Camera1.getPosx(), Camera1.getPosy(), Camera1.getPosz(),
		Camera1.getPosx() + Camera1.getDircetx(), Camera1.getPosy() + Camera1.getDircety(), Camera1.getPosz() + Camera1.getDircetz(),
		Camera1.getUpVectorx(), Camera1.getUpVectory(), Camera1.getUpVectorz());

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glMatrixMode(GL_MODELVIEW);
	testModel2.setParent(Camera1.getCameraSpace());

	if (trackballMoved)
	{
		testModel2.RelativeRotation(angle, axis);
	}

	

	//using shader
	/*GLuint lightIntensity = glGetUniformLocation(Program1, "LightIntensity");
	glUniform1f(lightIntensity,testlight.getIntensity());

	GLuint lightPosition = glGetUniformLocation(Program1, "LightPosition");
	glUniform3fv(lightPosition, 3 * sizeof(float), testlight.getPosition().getPtr());

	GLuint lightColor = glGetUniformLocation(Program1, "LightColor");
	glUniform3fv(lightColor, 3 * sizeof(float), testlight.getColor().getPtr());*/
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	

	//texture
	// Bind our diffuse texture in Texture Unit 0
	//const std::string texture1name = DIRECTION + std::string("TilesOrnate0130_5_height_COLOR.tga");
	//const char* cName = texture1name.c_str();
	//char *cstr = new char[texture1name.size() + 1];
	//strcpy(cstr, texture1name.c_str());
	////tgaLoad(cstr, &texture1, TGA_LOW_QUALITY);
	//tgaLoadAndBind(cstr, TGA_DEFAULT);
	//delete[] cstr;
	//glActiveTexture(GL_TEXTURE0);
	//glActiveTexture(GL_TEXTURE0);
	//GLuint texture_id;
	//glGenTextures(1, &texture_id);
	//glBindTexture(GL_TEXTURE_2D, texture_id);
	//tgaUploadImage(&texture1, TGA_DEFAULT);

	//GLuint DiffuseTextureID = glGetUniformLocation(Program1, "DiffuseTexture");
	//glProgramUniform1i(Program1, DiffuseTextureID, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, DiffuseTextureID);
	//glBindSampler(0,DiffuseTextureID);
	////parameters
	
	//////////////////////////////////////////////////////////////////////////

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = { 5.0, 3.0, 7.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	GLfloat lightpos1[] = { 8.0, 9.0, 10.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);

	GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat cyan[] = { 0.f, .8f, .8f, 1.f };
	GLfloat WarmRed[] = {1.0f,0.2f,0.03f,1.0f};
	GLfloat yellow[] = {0.8,0.8,0.1,1.0f};
	//material
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, cyan);
	//light0 color
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	//light1 color
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow);
	glLightfv(GL_LIGHT1, GL_SPECULAR, yellow);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white);

	//draw 
	
	glUseProgram(ProgramList[1]);

	//glUseProgram(0);
	//draw 1st object
	testModel.Reset();

	testModel.ClassicDraw(SMOOTH, UV_LACK);

	testModel.Rotation(30.0, Vec3f(0.0, 1.0, 0.0));

	testModel.ClassicDraw(SMOOTH, UV_LACK);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, WarmRed);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, WarmRed);

	//draw 2nd object
	glUseProgram(ProgramList[3]);
	
	testModel2.ClassicDraw(SMOOTH, UV_LACK);
	
	

	//draw 3nd Object
	glUseProgram(ProgramList[1]);
	testModel3.ClassicDraw(SMOOTH, UV_LACK);

	//draw background
	glUseProgram(ProgramList[0]);

	//glGenTextures(2, TextureList);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureList[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureList[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureList[2]);
	//new material
	//GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat red[4] = { 1.0f, 0.2f, 0.1f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, white);

	testModel1.ClassicDraw(FLAT,UV_RAW);

	

	//glUseProgram(Program2);

	
	/*Model1.draw(Vec3f(1.0,0.2,0.5));
	Model1.rotate(angle, axis);*/
	
	glutSwapBuffers();
	//select();
}



void Reshape(GLint width, GLint height){  //reshape function
	Window_width = width;
	Window_height = height;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Camera1.setAspect(WINDOW_WIDTH / WINDOW_HEIGHT);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	float _new_aspect = (float)width / height;
	//glMatrixMode(GL_PROJECTION);             //Matrix mode set as projection matrix 
	//glLoadIdentity();
	// Define perspective projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	

	Camera1.setAspect(width / height);
	int fov = 2 * atan(Camera1.h() / Camera1.n()) / PI * 180;
	gluPerspective(fov, Camera1.Aspect(), Camera1.n(), Camera1.f());
}

///control //////////////////////////
void Press_Key(unsigned char key, int x, int y){
	switch (key) {
	case ' ': /* press spacebar to start */
		//update(1);
		break;
	//case 'y'://move away from camera
	//	object.translate(Vec3f(0.0f, 0.0f, move_step));
	//	break;
	//case 'b'://move to camera
	//	object.translate(Vec3f(0.0f, 0.0f, -move_step));
	//	break;
	//case 'g':  //move left
	//	object.translate(Vec3f(-move_step, 0.0f, 0.0f));
	//	break;
	//case 'h':  //move right
	//	object.translate(Vec3f(move_step, 0.0f, 0.0f));
	//	break;
	//case 'i':  //Rotate up
	//	object.rotate(rotate_degree_step, Vec3f(-1.0f, 0.0f, 0.0f));
	//	break;
	//case 'j': //Rotate left
	//	object.rotate(rotate_degree_step, Vec3f(0.0f, 1.0f, 0.0f));
	//	break;
	//case 'k':  //Rotate right
	//	object.rotate(rotate_degree_step, Vec3f(0.0f, -1.0f, 0.0f));
	//	break;
	//case 'm':  //rotate down
	//	object.rotate(rotate_degree_step, Vec3f(1.0f, 0.0f, 0.0f));
	//	break;
		//camera control
	case 'w':
		//move camera forward
		Camera1.ConstrainedTranslate(Vec3f(0.0f, 0.0f, move_step));

		break;
	case 'a':
		//move camera backward
		Camera1.ConstrainedTranslate(Vec3f(-move_step, 0.0f, 0.0f));
		break;
	case 's':
		//move camera left
		Camera1.ConstrainedTranslate(Vec3f(0.0f, 0.0f, -move_step));
		break;
	case 'd':
		//move camera right
		Camera1.ConstrainedTranslate(Vec3f(move_step, 0.0f, 0.0f));
		break;
		////additional
	case 'r':
		//reset
		InitShader();
		break;
		//Camera1.Reset();
		break;
	case ESCAPE: /* exit the program...normal termination. */
		exit(0);
	default:
		break;
	}
	glutPostRedisplay();
}

////////  virtual trackball functions /////////////////////
void trackball_ptov(int x, int y, int width, int height, Vec3f &v);

void StartRotation(int x, int y){
	trackingMouse = true;
	forceRedraw = false;
	startX = x;
	startY = y;
	curx = x;
	cury = y;
	trackball_ptov(x, y, Window_width, Window_height, LastPos);
	trackballMoved = true;
}

void StopRotation(int x, int y){
	trackingMouse = false;
	/* check if position has changed */
	if (startX != x || startY != y)
		forceRedraw = true;
	else
	{
		angle = 0.0;
		forceRedraw = false;
		trackballMoved = false;
	}
}

void trackball_ptov(int x, int y, int width, int height, Vec3f &v)
{
	float d, a;
	/* project x,y onto a hemisphere centered within width, height ,
	note z is up here*/
	float v0 = (2.0*x - width) / width;
	float v1 = (height - 2.0F*y) / height;
	d = sqrt(v0 * v0 + v1 * v1);
	v = Vec3f(v0, v1, cos((PI / 2.0) * ((d < 40.0) ? d : 40.0)));

	a = 1.0 / v.length();
	v *= a;
	v.normalize();
}

//mouse funciton control
void Mouse_Func(int button, int state, int x, int y){
	GLuint selectBuf[BUF_SIZE];
	GLint hits;
	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)  //start rotation
		{
			/*glSelectBuffer(BUF_SIZE, selectBuf);
			(void)glRenderMode(GL_SELECT);

			glInitNames();
			glPushName(0);*/
			/*int x1 = Window_width - x;*/
			StartRotation(x, y);
			/*mouse_x = x;
			mouse_y = y;
			std::cout << "mouse x: " << mouse_x << "mouse y: " << mouse_y << std::endl;
			mouse_press = true;*/
			//glutIdleFunc(MouseRotation);
			break;
		}
		if (state == GLUT_UP)   //stop rotation
		{
			/*int x1 = Window_width - x;*/
			StopRotation(x, y);
			//mouse_press = false;
		}
		//glutIdleFunc(NULL);
		break;
	default: //do nothing
		break;
	}
	glutPostRedisplay();
}

void Mouse_Motion(int x, int y){
	Vec3f curPos;
	Vec3f dpos;
	//rotate model using virtual track ball
	if (trackingMouse){
		/* compute the change in position
		on the hemisphere */
		trackball_ptov(x, y, Window_width, Window_height, curPos);
		dpos = curPos - LastPos;
	}
	if (dpos.x() || dpos.y() || dpos.z())
	{
		/* compute theta and cross product */
		angle = 90.0*dpos.length();
		//angle = 90.0 * acos(LastPos.dot(curPos))/PI;
		axis = LastPos.cross(curPos);
		axis.normalize();
		axis = Vec3f(axis.x(), -axis.y(), axis.z());
		/*axis[0] = lastPos[1] * curPos[2] –
		lastPos[2] * curPos[1];
		axis[1] = lastPos[2] * curPos[0] –
		lastPos[0] * curPos[2];
		axis[2] = lastPos[0] * curPos[1] –
		lastPos[1] * curPos[0];*/
		/* update position */
		LastPos = curPos;
	}

	glutPostRedisplay();
}

void spinCube()
{
	if (forceRedraw) glutPostRedisplay();
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){
	//create shader
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


void InitShader(){
	std::string vertexshadername = SHADERDIRECTION + std::string("vertexshader1.glsl");
	std::string fragmentshadername = SHADERDIRECTION + std::string("fragshader.glsl");

	std::string vertexshaderPure = SHADERDIRECTION + std::string("Text.VertexShader");
	std::string fragmentshaderPure = SHADERDIRECTION + std::string("Text.FragmentShader");

	std::string vertexshaderPhong = SHADERDIRECTION + std::string("vp0.vert");
	std::string fragmentshaderPhong = SHADERDIRECTION + std::string("fp0.frag");

	std::string Wardvertex = SHADERDIRECTION + std::string("Ward.verShader");
	std::string Wardfrag = SHADERDIRECTION + std::string("Ward.fragShader");

	ProgramList[0] = LoadShaders(vertexshaderPhong.c_str(), fragmentshaderPhong.c_str());
	ProgramList[1] = LoadShaders(vertexshadername.c_str(), fragmentshadername.c_str());

	ProgramList[2] = LoadShaders(vertexshaderPure.c_str(), fragmentshaderPure.c_str());

	ProgramList[3] = LoadShaders(Wardvertex.c_str(), Wardfrag.c_str());
}

void Special(int key, int x, int y){
	switch (key) {
	case GLUT_KEY_RIGHT: //right arrow
		//camera rotate left
		Camera1.Rotate(-rotate_step);
		break;
	case GLUT_KEY_LEFT: //left arrow
		//camera rotate right
		Camera1.Rotate(rotate_step);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

//main function
int _tmain(int argc, char* argv[])
{
	//initate glut functions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Assigment4: Shader");

	//glew init check
	GLenum err = glewInit();

	if (err != GLEW_OK){
		std::cout << "Glew load failure!"<<std::endl;
	}

	/*if (LoadShaders("PhongVertexShader.glsl", "PhongFragmentShader.glsl") == -1){
	exit(1);
	}*/
	/*if (err == GLEW_OK) {
		glutMainLoop();
	}*/
	Init();

	//glut draw funcitons
	glutDisplayFunc(draw_Scene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Press_Key);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse_Func);
	glutMotionFunc(Mouse_Motion);

	

	glutMainLoop();

	return 0;
}


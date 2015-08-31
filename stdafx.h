// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here

//c++
#include <string>
#include <iostream>
#include <fstream>
#include <list>
//opengl
#include <GL/glew.h>
#include <GL/glut.h>



//relative headers
#include "Vec3.h"
#include "Vec2.h"
#include "Mat4.h"
#include "Transformation.h"
#include "Camera.h"
#include "PolyModel.h"
#include "fbx_loader.h"

//material 
#include "Material.h"

//lights
#include "Light.h"
#include "Point_Light.h"
//model
#include "Model.h"
//texture
#include "Texture_Map.h"
#include "TgaTexture.h"

//scene
#include "Scene.h"


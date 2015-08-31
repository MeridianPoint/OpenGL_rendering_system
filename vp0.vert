varying vec3 v;
varying vec3 N;
uniform float Ka;
uniform float Kd;
uniform float Ks;

uniform vec2 tiling;

void main(void){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_TexCoord[2]  = gl_MultiTexCoord2;
	v=vec3(gl_ModelViewMatrix*gl_Vertex);
	N=normalize(gl_NormalMatrix*gl_Normal);
	gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;
	
}

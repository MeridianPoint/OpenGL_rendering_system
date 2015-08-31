#version 120

uniform float Ka;

uniform float Kd;

uniform float Ks;
uniform float Shinness;

varying vec3 normal;

varying vec3 vertex_eye;

varying vec4 color;

varying vec3 v;

varying vec3 N;


void main(void){

	v = vec3(gl_ModelViewMatrix*gl_Vertex);
	N = normalize(gl_NormalMatrix*gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;

}
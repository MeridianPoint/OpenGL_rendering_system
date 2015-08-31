varying vec3 v;
varying vec3 N;
uniform float Ka;
uniform float Kd;
uniform float Ks;
uniform sampler2D TextureUnit0;
uniform sampler2D TextureUnit1;
uniform sampler2D TextureUnit2;

uniform vec2 tiling;

float GreyScaleValue(in vec4 color){
	return float(color.r+color.g+color.b)/3.0;
}

vec4 mixcolor(in vec4 color1,in vec4 color2){
	return vec4(color1.r*color2.r,color1.g*color1.g,color1.b*color1.b,1.0);
}

void main(void){
	vec2 xy =gl_TexCoord[0].xy;
	vec2 phase = xy *30.0;

	vec4 value1 = texture2D(TextureUnit0, phase);

	vec4 value2 =texture2D(TextureUnit1,phase);

	vec4 ao=vec4(1.0,1.0,1.0,1.0)-(vec4(1.0,1.0,1.0,1.0)-texture2D(TextureUnit2,gl_TexCoord[0].xy))*0.2;

	vec3 L= normalize(gl_LightSource[0].position.xyz-v);

	vec3 E= normalize(-v);

	vec3 R= normalize(-reflect(L,N));

	vec4 ambient, diffuse, specular;

	ambient= (gl_FrontLightProduct[0].ambient*value1+gl_FrontLightProduct[1].ambient*value1)*ao/2.0;

	diffuse= max(dot(N,L),0.0)*(mixcolor(value1,gl_FrontLightProduct[0].diffuse)+mixcolor(value1,gl_FrontLightProduct[1].diffuse))*ao/2.0;

	

	if(dot(N,L)<=0.0){
	specular=vec4(0.0,0.0,0.0,1.0);
	}
	else{
	specular=(gl_FrontLightProduct[0].specular*pow(max(dot(R,E),0.0),GreyScaleValue(value2)*9.0)+gl_FrontLightProduct[1].specular*pow(max(dot(R,E),0.0),GreyScaleValue(value2)*9.0));
	}
	
	
	gl_FragColor=(ambient*Ka+diffuse*Kd+specular*Ks);
	//gl_FragColor=texture2D(TextureUnit0, gl_TexCoord[0].xy);
}

#version 120

varying vec2 f_texcoord;
varying vec4 shadowCoord;

uniform sampler2D inTexture;
uniform sampler2DShadow shadowMap;

void main(){
	/*vec3 n = normalize(normalCam);
	vec3 l = normalize(lightDir);
	float cosTheta = clamp(dot(n,l),0, 0.05);
	float bias = 0.005*tan(acos(cosTheta));
	bias = clamp(bias,0, 0.005);
	float visibility = 1.0f;
	for(int i=0;i<4;i++){
		int index = i; //Banded anti-aliasing
		//int index = int(16.0*random(gl_FragCoord.xyy, i))%16; //Noise anti-aliasing
		visibility -= 0.2*(1.0-(shadow2D(shadowMap,vec3(shadowCoord.xy + poissonDisk[index]/700.0,shadowCoord.z-bias))).z );
	}*/
	float visibility = 1.2-shadow2DProj(shadowMap,shadowCoord.xyzw).r;
	vec3 texColour = texture2D(inTexture,f_texcoord).rgb;
	gl_FragColor = vec4(visibility*texColour,1.0);
}

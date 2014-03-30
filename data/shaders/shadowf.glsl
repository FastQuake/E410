#version 120

varying vec4 lightCoord;

uniform int lastpass;
uniform sampler2DShadow shadowTexture;
uniform sampler2DShadow depthTexture;

void main(){
	if(shadow2D(shadowTexture,vec3(gl_FragCoord.xy,1.0)).x == 1){
		gl_FragColor = vec4(0.4,0.0,0.0,0.0);
	}
	else{
		gl_FragColor = 1.0-vec4(shadow2DProj(depthTexture,vec4(lightCoord.xy,lightCoord.z-0.005,lightCoord.w)).r);
		gl_FragColor *= shadow2DProj(depthTexture,vec4(gl_FragCoord.xyzw)).r;
	}
	if(lastpass == 1){
		gl_FragColor = vec4(0.5);
	}
}

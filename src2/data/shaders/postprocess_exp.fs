varying vec2 texcoord;
uniform float expfactor;
uniform sampler2D tex0;

void main(void)
{
	vec4 texel = texture2D(tex0, texcoord);
	//calculate luminosity
	float lum = texel.r * 0.3 + texel.g * 0.59 + texel.b * 0.11;
	
	float factor = pow(lum, expfactor);
	texel.rgb *= factor;
	
	gl_FragColor = texel; 
}
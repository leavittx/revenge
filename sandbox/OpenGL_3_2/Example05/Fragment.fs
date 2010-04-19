#version 150

uniform sampler2D firstTexture; 

in vec3 fragmentNormal;
in vec2 fragmentTexCoord;

out vec4 fragColor;

void main(void)
{
	float intensity;
	
	intensity = max(dot(fragmentNormal, vec3(0.0, 0.0, 1.0)), 0.0);
	
	fragColor = texture2D(firstTexture, fragmentTexCoord)*intensity;
}

#version 150

uniform samplerCube cubemap;

in vec3 fragmentNormal;
in vec3 fragmentEye;

out vec4 fragColor;

void main(void)
{
	float intensity;
	
	vec3 reflection = reflect(fragmentEye, fragmentNormal);
	
	vec4 environmentColor = texture(cubemap, reflection);
	
	intensity = max(dot(fragmentNormal, vec3(0.0, 0.0, 1.0)), 0.0);
	
	fragColor = environmentColor * intensity;
}

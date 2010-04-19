#version 150

uniform samplerCube cubemap;

in vec3 fragmentReflection;

out vec4 fragColor;

void main(void)
{
	vec4 environmentColor = texture(cubemap, fragmentReflection);
	
	fragColor = environmentColor;
}

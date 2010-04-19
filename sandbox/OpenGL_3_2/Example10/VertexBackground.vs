#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

in vec4 vertex;
in vec3 normal;

out vec3 fragmentReflection;

void main(void)
{
	vec3 normalModelView = (modelViewMatrix*vec4(normal, 0.0)).xyz;
	
	vec3 eyeModelView = normalize((modelViewMatrix*vertex).xyz);
	
	fragmentReflection = -(modelViewMatrix*vec4(reflect(eyeModelView, normalModelView), 0.0)).xyz;
	
	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}

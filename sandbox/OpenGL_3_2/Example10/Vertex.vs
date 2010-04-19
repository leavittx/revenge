#version 150

const float Eta = 0.21; // Glass

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform float offset;

in vec4 vertex;
in vec3 normal;

out vec3 fragmentReflection;
out vec3 fragmentRefraction;
out float fresnel;

void main(void)
{
	vec3 normalModelView = (modelViewMatrix*vec4(normal, 0.0)).xyz;
	
	vec3 eyeModelView = normalize((modelViewMatrix*vertex).xyz);
		
	fragmentReflection = (modelViewMatrix*vec4(reflect(eyeModelView, normalModelView), 0.0)).xyz;
	
	fragmentRefraction = (modelViewMatrix*vec4(refract(eyeModelView, vec3(-normalModelView.x, -normalModelView.y, normalModelView.z*offset), Eta), 0.0)).xyz;
	
	fresnel = Eta + (1.0 - Eta) * pow((1.0 - dot(-eyeModelView, normalModelView)), 5.0);
		
	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}

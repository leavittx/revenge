//textured flat shading
//
//input:
//
//	gl_LightSource[0].position = light direction
//  gl_LightSource[0].diffuse = light color
//	gl_LightSource[0].ambient = ambient color
//  gl_FrontMaterial.diffuse = material color
//

//lighted up pixel color that goes to the fragment shader
varying vec4 diffuse;
varying vec4 ambient;
//texture coordinate that goes to the fragment shader
varying vec2 texcoord;

//different vectors needed for lighting in the fragment shader
varying vec3 halfvector;
varying vec3 lightdirection;
varying vec3 normal;

void main(void)
{
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightdirection = normalize(vec3(gl_LightSource[0].position));
	halfvector = normalize(gl_LightSource[0].halfVector.xyz);	

	//calculate colors from both the light and the material
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	
	texcoord = gl_MultiTexCoord0.st;
	gl_Position = ftransform();
}
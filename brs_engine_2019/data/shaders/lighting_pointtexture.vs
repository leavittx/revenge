// Point light with texture
//
//	gl_LightSource[0].position = light position
//  gl_LightSource[0].diffuse = light color
//	gl_LightSource[0].ambient = ambient color
//  gl_FrontMaterial.diffuse = material color
//

//lighted up pixel color that goes to the fragment shader
varying vec4 diffuse;
varying vec4 ambient;

//different vectors needed for lighting in the fragment shader
varying vec3 halfVector;
varying vec3 lightdirection;
varying vec3 normal;
varying float distanceToLight;

varying vec2 texcoord;

void main(void)
{
	normal = normalize(gl_NormalMatrix * gl_Normal);
	vec4 transformedPosition = gl_ModelViewMatrix * gl_Vertex;
	vec3 directiontemp = vec3(gl_LightSource[0].position - transformedPosition);

	lightdirection = normalize(directiontemp);
	distanceToLight = length(directiontemp);
	
	halfVector = normalize(gl_LightSource[0].halfVector.xyz);	

	//calculate colors from both the light and the material
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;

	texcoord = gl_MultiTexCoord0.st;	
	gl_Position = ftransform();
}
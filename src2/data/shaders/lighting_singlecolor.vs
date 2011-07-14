// Just a flat color
//
//  gl_FrontMaterial.diffuse = color color
//

//lighted up pixel color that goes to the fragment shader
uniform vec4 diffuse;

void main(void)
{
	gl_Position = ftransform();
}
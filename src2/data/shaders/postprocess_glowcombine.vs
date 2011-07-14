varying vec2 texturecoordinate;

void main(void)
{
	texturecoordinate = gl_MultiTexCoord0.st;
	gl_Position = ftransform();
}

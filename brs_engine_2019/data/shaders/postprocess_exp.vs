varying vec2 texcoord;

void main(void)
{
	texcoord = gl_MultiTexCoord0.st;
	gl_Position = ftransform();
}
varying vec2 offs0;
varying vec2 offs1;
varying vec2 offs2;
varying vec2 offs3;
varying vec2 offs4;
uniform float blurscale;

void main(void)
{
	//take 5 texels
	vec2 dx = vec2(blurscale, 0.0);
	vec2 dx2 = vec2(2.0 * blurscale, 0.0);
	offs0 = gl_MultiTexCoord0.st - dx2;
	offs1 = gl_MultiTexCoord0.st - dx;
	offs2 = gl_MultiTexCoord0.st;
	offs3 = gl_MultiTexCoord0.st + dx;
	offs4 = gl_MultiTexCoord0.st + dx2;
	
	gl_Position = ftransform();
}

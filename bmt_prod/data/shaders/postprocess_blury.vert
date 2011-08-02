varying vec2 offs0;
varying vec2 offs1;
varying vec2 offs2;
varying vec2 offs3;
varying vec2 offs4;
uniform float blurscale;

void main(void)
{
	//take 5 texels
	vec2 dy = vec2(0.0, blurscale);
	vec2 dy2 = vec2(0.0, 2.0 * blurscale);
	offs0 = gl_MultiTexCoord0.st - dy2;
	offs1 = gl_MultiTexCoord0.st - dy;
	offs2 = gl_MultiTexCoord0.st;
	offs3 = gl_MultiTexCoord0.st + dy;
	offs4 = gl_MultiTexCoord0.st + dy2;
	
	gl_Position = ftransform();
}

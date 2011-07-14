uniform sampler2D tex;
varying vec2 offs0;
varying vec2 offs1;
varying vec2 offs2;
varying vec2 offs3;
varying vec2 offs4;
uniform float bluralpha;

void main()
{
	vec4 texel = vec4(0.0, 0.0, 0.0, 0.0);
	texel += texture2D(tex, offs0) * 0.1;
	texel += texture2D(tex, offs1) * 0.25;
	texel += texture2D(tex, offs2) * 0.5;
	texel += texture2D(tex, offs3) * 0.25;
	texel += texture2D(tex, offs4) * 0.1;
	gl_FragColor = texel * bluralpha;
}

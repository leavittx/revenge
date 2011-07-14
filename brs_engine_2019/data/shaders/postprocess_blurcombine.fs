uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float bluralpha;
varying vec2 texturecoordinate;

void main(void)
{
	vec4 normal_texel = texture2D(tex0, texturecoordinate);
	vec4 blur_texel = texture2D(tex1, texturecoordinate);
	gl_FragColor = normal_texel + (blur_texel - normal_texel) * bluralpha;

}
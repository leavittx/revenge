#version 150

uniform sampler2D firstTexture; 

in float alpha;

out vec4 fragColor;

void main(void)
{
	fragColor = texture(firstTexture, gl_PointCoord)*vec4(1.0, 0.0, 0.0, alpha);
}

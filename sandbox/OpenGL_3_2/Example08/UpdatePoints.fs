#version 150

in vec4 newParticle;

out vec4 fragParticle;

void main(void)
{
	fragParticle = newParticle;
}

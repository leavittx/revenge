#version 410 core

#define NUMBERWAVES 4

const float Eta = 0.15; // Water
const float PI = 3.141592654;
const float G = 9.81;

uniform float waterPlaneLength;

uniform float passedTime;

uniform vec4 waveParameters[NUMBERWAVES];
uniform vec2 waveDirections[NUMBERWAVES];

in vec2 fragmentTexCoord;

layout(location = 0, index = 0) out vec4 fragColor;

vec3 normalToTexture(vec3 orgNormal)
{
	return vec3(clamp(orgNormal.x*0.5 + 0.5, 0.0, 1.0), clamp(orgNormal.y*0.5 + 0.5, 0.0, 1.0), clamp(orgNormal.z*0.5 + 0.5, 0.0, 1.0));
}

void main(void)
{
	vec3 vertex = vec3(fragmentTexCoord.s*waterPlaneLength-waterPlaneLength/2.0 + 0.5, 0.0, -fragmentTexCoord.t*waterPlaneLength+waterPlaneLength/2.0 + 0.5);

	vec4 finalVertex;

	finalVertex.x = vertex.x;
	finalVertex.y = vertex.y;
	finalVertex.z = vertex.z;
	finalVertex.w = 1.0;

	vec3 finalNormal;
	
	finalNormal.x = 0.0;
	finalNormal.y = 0.0;
	finalNormal.z = 0.0;

	for (int i = 0; i < NUMBERWAVES; i++)
	{
		vec2 direction = normalize(waveDirections[i]);
		float speed = waveParameters[i].x;
		float amplitude = waveParameters[i].y;
		float wavelength = waveParameters[i].z;
		float steepness = waveParameters[i].w;

		float frequency = sqrt(G*2.0*PI/wavelength);
		float phase = speed*frequency;
		float alpha = frequency*dot(direction, vertex.xz)+phase*passedTime;
		
		finalVertex.x += steepness*amplitude*direction.x*cos(alpha);
		finalVertex.y += amplitude*sin(alpha);
		finalVertex.z += steepness*amplitude*direction.y*cos(alpha);
	}

	for (int i = 0; i < NUMBERWAVES; i++)
	{
		vec2 direction = normalize(waveDirections[i]);
		float speed = waveParameters[i].x;
		float amplitude = waveParameters[i].y;
		float wavelength = waveParameters[i].z;
		float steepness = waveParameters[i].w;

		float frequency = sqrt(G*2.0*PI/wavelength);
		float phase = speed*frequency;
		float alpha = frequency * dot(direction, finalVertex.xz) + phase*passedTime;
				
		finalNormal.x += direction.x * wavelength * amplitude * cos(alpha);
		finalNormal.y += steepness * wavelength * amplitude * sin(alpha);
		finalNormal.z += direction.y * wavelength * amplitude * cos(alpha);
	}

	finalNormal.x = -finalNormal.x;
	finalNormal.y = 1.0 - finalNormal.y;
	finalNormal.z = -finalNormal.z;
	finalNormal = normalize(finalNormal);

	fragColor = vec4(normalToTexture(finalNormal), 1.0);
}

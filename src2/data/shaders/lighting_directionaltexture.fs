//textured flat shading
//
//input:
//
//  tex - texture unit
//

uniform sampler2D tex;

varying vec2 texcoord;
varying vec4 diffuse;
varying vec4 ambient;
varying vec3 halfvector;
varying vec3 lightdirection;
varying vec3 normal;

void main(void)
{
	vec3 interpolatedNormal;

	float angleDot;
	float angleDot2;

	//take ambient as base color
	vec4 color = ambient;
	vec4 specular;

	interpolatedNormal = normalize(normal);
	angleDot = max(dot(interpolatedNormal, lightdirection), 0.0);
	
	if (angleDot > 0.0)
	{
		//add diffuse component
		color += angleDot * diffuse;
		vec3 halfn = normalize(halfvector);

		float angledot2 = max(dot(interpolatedNormal, halfn), 0.0);
		
		//shade the texel
		
		//calculate specular component
		specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(angledot2, gl_FrontMaterial.shininess);
	}
//	gl_FragColor = color + specular;
	
	gl_FragColor = color * texture2D(tex, texcoord) + specular;
}

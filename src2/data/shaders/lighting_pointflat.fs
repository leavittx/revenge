varying vec4 diffuse;
varying vec4 ambient;
varying vec3 halfVector;
varying vec3 lightdirection;
varying vec3 normal;

varying float distanceToLight;

void main(void)
{
	vec3 interpolatedNormal;
	float angledot;
	float angledot2;

	//take ambient as base color
	vec4 color = ambient;
	vec4 specular;

	interpolatedNormal = normalize(normal);
	angledot = max(dot(interpolatedNormal, lightdirection), 0.0);
	
	if (angledot > 0.0)
	{
		float att = 1.0 / (gl_LightSource[0].constantAttenuation +
				gl_LightSource[0].linearAttenuation * distanceToLight +
				gl_LightSource[0].quadraticAttenuation * distanceToLight * distanceToLight);
		//add diffuse component
		color += att * angledot * diffuse;

		vec3 halfn = normalize(halfVector);
		angledot2 = max(dot(interpolatedNormal, halfn), 0.0);
		
		//calculate specular component
		specular = att * gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(angledot2, gl_FrontMaterial.shininess);
	}
	gl_FragColor = color + specular;
}

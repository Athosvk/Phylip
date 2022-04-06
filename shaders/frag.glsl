#version 450 core

in vec3 norm;
in vec3 fragPos;

struct Light{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform vec3 viewPos;

out vec4 fColor;

void main(void){
	Light lights[3] = Light[3](
		Light(
			vec4(0.0, 3.0, 3.0, 0.0),
			vec3(0.05, 0.05, 0.05),
			vec3(0.5, 0.5, 0.5),
			vec3(1.0f, 1.0f, 1.0f))
		,Light(
			vec4(-5.0, 3.0, -5.0, 0.0),
			vec3(0.05, 0.05, 0.05),
			vec3(0.5, 0.5, 0.5),
			vec3(1.0f, 1.0f, 1.0f))
		,Light(
			vec4(4.0, 3.0, -5.0, 0.0),
			vec3(0.05, 0.05, 0.05),
			vec3(0.5, 0.5, 0.5),
			vec3(1.0f, 1.0f, 1.0f))
			);

	vec3 diffuse = vec3(0.0);
	vec3 ambient = vec3(0.0);
	vec3 specular = vec3(0.0);
	vec3 viewDir = normalize(viewPos - fragPos);
	for(int i=0;i<3;++i)
	{
		vec3 lightDir = normalize(lights[i].position.xyz - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		float diff = max(dot(norm, lightDir), 0.0);

		diffuse += diff * lights[i].diffuse * material.diffuse;
		ambient += material.diffuse * lights[i].ambient;
		specular += lights[i].specular * spec * material.specular;
	}
	
	vec3 result = (ambient + diffuse + specular);

	fColor = vec4(result, 1.0);
}

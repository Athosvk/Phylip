#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 projView;
uniform mat4 model;

out vec3 norm;
out vec3 fragPos;

void main(void){
	gl_Position = projView * model * vec4(aPos, 1.0);
	fragPos = vec3(model * vec4(aPos, 1.0));
	norm = mat3(transpose(inverse(model))) * aNorm;
}

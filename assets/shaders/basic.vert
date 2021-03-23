#version 330 core

layout(location = 0) in vec3 _normal;
layout(location = 1) in vec3 _position;
layout(location = 3) in vec2 _texCoord0;

out vec3 normal;
out vec2 texCoord0;

uniform mat4 model;
uniform mat4 vp;

void main() {
	gl_Position = vp * model * vec4(_position, 1.0);
	texCoord0 = _texCoord0;

	normal = normalize((model * vec4(_normal, 0.0)).xyz);
}
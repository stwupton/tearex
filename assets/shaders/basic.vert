#version 330 core

layout(location = 1) in vec4 position;

void main() {
	vec4 newPosition = vec4(position.x, position.y, 0, 1) * 0.01;
	gl_Position = newPosition;
}
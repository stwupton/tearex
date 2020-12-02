#version 330 core

layout(location = 1) in vec4 position;

void main() {
	gl_Position = position;
}
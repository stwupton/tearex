#version 330 core

in vec3 light;
in vec2 texCoord0;

out vec4 color;

uniform sampler2D tex0;

void main() {
	vec3 ambientLight = vec3(0.5, 0.8, 1.0) * 0.8;
	color = texture(tex0, texCoord0) * (vec4(ambientLight, 1.0) + vec4(light, 1.0));
}
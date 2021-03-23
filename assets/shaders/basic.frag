#version 330 core

in vec3 normal;
in vec2 texCoord0;

out vec4 color;

uniform vec3 lightColour;
uniform vec3 lightDirection;
uniform sampler2D tex0;

void main() {
	vec3 ambientLight = vec3(0.5, 0.8, 1.0) * 0.8;
	float directionLightDiff = max(dot(normal, -lightDirection), 0.0);
	vec3 light = directionLightDiff * lightColour + ambientLight;

	vec4 textureColour = texture(tex0, texCoord0);
	if (length(textureColour.xyz) <= 0) {
		textureColour = vec4(1.0, 0.0, 1.0, 1.0);
	}

	color = textureColour * vec4(light, 1.0);
}
#version 330 core
out vec4 FragColor;

in vec3 oPosition;
in vec3 oNormal;
in vec2 oTexCoord;

uniform sampler2D uTextureIndex;

void main() {
	FragColor = texture(uTextureIndex, oTexCoord) * vec4(1.0);
}
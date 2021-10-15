#version 330 core
out vec4 FragColor;

in vec3 oPosition;
in vec3 oNormal;
in vec2 oTexCoord;

uniform sampler2D uTextureIndex;
uniform bool uHovering;

void main() {
	if(!uHovering) {
		FragColor = texture(uTextureIndex, oTexCoord) * vec4(1.0);
	}
	else {
		FragColor = texture(uTextureIndex, oTexCoord) * vec4(0.9, 0.9, 0.9, 1.0);
		FragColor.w = max(0.2, FragColor.w);
	}
}
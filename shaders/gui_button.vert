#version 330 core
layout (location = 0)in vec3 iPosition;
layout (location = 1)in vec3 iNormal;
layout (location = 2)in vec2 iTexCoord;

out vec3 oPosition;
out vec3 oNormal;
out vec2 oTexCoord;

uniform mat4 uProjection;
uniform mat4 uModel;


void main() {
	oPosition = iPosition;
	oNormal = iNormal;
	oTexCoord = iTexCoord;
	gl_Position = uProjection * uModel * vec4(iPosition, 1.0);
}
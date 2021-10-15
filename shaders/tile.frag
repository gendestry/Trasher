#version 330 core
out vec4 FragColor;

in vec3 oPosition;
in vec3 oNormal;
in vec2 oTexCoord;

uniform sampler2D uTextureIndex;
uniform vec3 uPlayerPos;
uniform float uTime;
uniform float uFogOfWar;

void main() {
	/*float size = 32.0;
	float d = distance(oPosition, uPlayerPos);
	vec3 result = vec3(texture(uTextureIndex, oTexCoord) * min((uFogOfWar/d)-1.0, 1.0)); // * abs(sin(uTime))
	FragColor = vec4(result, 1.0);*/
	
	float size = 32.0;
	float d = distance(oPosition, uPlayerPos);
	vec4 texcolor = texture(uTextureIndex, oTexCoord);
	texcolor.xyz *= min((uFogOfWar/d)-1.0, 1.0); // *abs(sin(uTime))
	FragColor = texcolor;//vec4(result, 1.0);
}
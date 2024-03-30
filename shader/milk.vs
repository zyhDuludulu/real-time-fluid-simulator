#version 330

vec2 position[4] = {
	vec2(-1.0, -1.0),
	vec2( 1.0, -1.0),
	vec2(-1.0,  1.0),
	vec2( 1.0,  1.0)
};

vec2 texCoords[4] = {
	vec2(0.0, 0.0),
	vec2(1.0, 0.0),
	vec2(0.0, 1.0),
	vec2(1.0, 1.0)
};

out vec2 texCoord;

void main() {
	gl_Position = vec4(position[gl_VertexID], 0.0, 1.0);
	texCoord = texCoords[gl_VertexID];
}
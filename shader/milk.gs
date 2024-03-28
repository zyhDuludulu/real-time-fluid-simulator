#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 center;

float size = 0.02;

void main() {
	center = gl_in[0].gl_Position.xy;

	gl_Position = gl_in[0].gl_Position + vec4(-size, 0, 0, 0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0, -size, 0, 0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0,  size, 0, 0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4( size, 0, 0, 0);
	EmitVertex();
	EndPrimitive();
}
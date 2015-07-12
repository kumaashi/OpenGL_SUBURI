#version 450

layout(triangles) in;
//layout(triangle_strip, max_vertices=3)  out;
layout(line_strip, max_vertices=2)  out;

void main() {
	for(int i = 0; i < gl_in.length() - 1; i++) {
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}

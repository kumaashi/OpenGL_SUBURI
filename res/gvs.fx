#version 150

#define MAX_VERTEX      3
#define MAX_LINE_VERTEX 2

layout(triangles) in;
layout(triangle_strip, max_vertices=MAX_VERTEX)  out;
varying in   vec4 out_color[];
varying out  vec4 fs_color[];

void main() {
	for(int i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		fs_color[i] = out_color[i];
		EmitVertex();
	}
	EndPrimitive();
}


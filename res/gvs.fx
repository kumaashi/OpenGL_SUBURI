#version 450

#define MAX_VERTEX      3

layout(triangles) in;
layout(triangle_strip, max_vertices=MAX_VERTEX) out;

in  vec3  out_nor[MAX_VERTEX];
in  vec3  out_pos[MAX_VERTEX];
in  vec4  out_color[MAX_VERTEX];
out vec3  fs_nor[MAX_VERTEX];
out vec3  fs_pos[MAX_VERTEX];
out vec4  fs_color[MAX_VERTEX];

void main() {
	for(int i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		fs_color[i] = out_color[i];
		fs_pos[i]   = out_pos[i];
		fs_nor[i]   = out_nor[i];
		EmitVertex();
	}
	EndPrimitive();
}


//#version 400
#version 410

#define MAX_VERTEX      30

layout(triangles) in;
layout(triangle_strip, max_vertices=MAX_VERTEX) out;

in         vec3  out_nor[];
in         vec4  out_pos[];
in         vec4  out_color[];

out        vec4  fs_color;
out        vec3  fs_nor;
out        vec4  fs_pos;

uniform    mat4  view;
uniform    mat4  proj;
uniform    mat4  world;

void main() {
	for(int i = 0; i < gl_in.length(); i++) {
		fs_color = out_color[i];
		fs_nor   = out_nor[i];
		fs_pos   = out_pos[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}




#define MAX_VERTEX      3
#define MAX_LINE_VERTEX 2

layout(triangles) in;
layout(triangle_strip, max_vertices=MAX_VERTEX)  out;
varying in   vec3 out_pos[MAX_VERTEX];
varying out  vec3 fs_pos[MAX_VERTEX];

varying in   vec4 out_color[MAX_VERTEX];
varying out  vec4 fs_color[MAX_VERTEX];

void main() {
	for(int i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		fs_color[i] = out_color[i];
		fs_pos[i]   = fs_pos[i];
		EmitVertex();
	}
	EndPrimitive();
}


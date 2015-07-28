#version 150

#define MAX_VERTEX 3
#define MAX_LINE_VERTEX 3
layout(triangles) in;
layout(triangle_strip, max_vertices=MAX_VERTEX)  out;
in  vec4 IncColor[MAX_VERTEX];
in  vec4 IngColor[MAX_VERTEX];
out vec4 cColor[MAX_VERTEX];
out vec4 gColor[MAX_VERTEX];

void main() {
	for(int i = 0; i < gl_in.length() - 1; i++) {
		gl_Position = gl_in[i].gl_Position;
		cColor[i]   = IncColor[i];
		gColor[i]   = IngColor[i];
		EmitVertex();
	}
	EndPrimitive();
}


#version 450

#define MAX_VERTEX 3
layout(triangles) in;
layout(triangle_strip, max_vertices=MAX_VERTEX)  out;
//layout(line_strip, max_vertices=2)  out;
in  vec4 IncColor[MAX_VERTEX];
in  vec4 IngColor[MAX_VERTEX];
out vec4 cColor[MAX_VERTEX];
out vec4 gColor[MAX_VERTEX];

void main()
{
	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = gl_in[i].gl_Position;
		cColor[i] = IncColor[i];
		gColor[i] = IngColor[i];
		EmitVertex();
	}
	EndPrimitive();
}


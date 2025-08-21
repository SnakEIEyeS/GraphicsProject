#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

void main()
{
	const vec4 DebugOffset = vec4(0.f, 0.f, -0.00001f, 0.f);
	
	gl_Position = gl_in[0].gl_Position + DebugOffset;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position + DebugOffset;
	EmitVertex();

	gl_Position = gl_in[2].gl_Position + DebugOffset;
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + DebugOffset;
	EmitVertex();

	EndPrimitive();
}

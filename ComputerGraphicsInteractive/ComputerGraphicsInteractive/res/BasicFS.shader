#version 330 core

layout(location=0) out vec4 o_ColorFS;

in vec4 o_ColorVS;

void main()
{
	o_ColorFS = o_ColorVS;
}
#version 330 core

layout(location=0) out vec4 o_ColorFS;

in vec4 o_ColorVS;

void main()
{
	o_ColorFS = vec4(1.0f, 1.0f , 0.0f, 1.0f);
}
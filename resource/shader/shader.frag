#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

in vec2 textureCoord;

in vec4 p_color;
uniform sampler2D outTexture;

void main()
{
	color = p_color;
}

#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 colormap[256];
uniform float minDepth;
uniform float maxDepth;

out vec2 textureCoord;
out vec4 p_color;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0);
	textureCoord = texCoord;
	
	float normalizedZ = ((position.z - minDepth) / (maxDepth - minDepth)) * 255.0;
	int index = int(floor(normalizedZ));  
	if (index < 0) index = 0;
	else if (index > 255) index = 255;
	p_color = colormap[index];
}

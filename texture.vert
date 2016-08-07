#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTextureCoord;

out vec2 textureCoord;

uniform mat4 mvpMatrix;

void main()
{
	textureCoord = vertexTextureCoord;
	gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
}

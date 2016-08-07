#version 400

in vec2 textureCoord;

uniform sampler2D tex;

layout (location = 0) out vec4 fragColor;

void main()
{
	fragColor = texture(tex, textureCoord);
}


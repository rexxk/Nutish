#shadertype vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

void main()
{
	gl_Position = vec4(a_Position, 1.0);
}


#shadertype pixel
#version 450 core

layout(location = 0) out vec4 o_Color;

uniform vec4 u_Color;
uniform vec3 u_LightDirection;
uniform sampler2D m_Texture;

void main()
{
	o_Color = u_Color;

//	o_Color = vec4(0.2, 0.35, 0.75, 1.0);
}

#shadertype vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
//	gl_Position = vec4(a_Position, 1.0);

	v_TexCoord = a_TexCoord;
}


#shadertype pixel
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform vec3 u_LightDirection;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;


void main()
{
//	o_Color = u_Color;

	o_Color = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.5);

//	o_Color = vec4(0.2, 0.35, 0.75, 1.0);
}

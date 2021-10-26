#shadertype vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in mat4 a_InstanceMatrix;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec3 v_Normal;

void main()
{
//	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
	gl_Position = u_ViewProjection * a_InstanceMatrix * vec4(a_Position, 1.0);
//	gl_Position = vec4(a_Position, 1.0);

	v_Normal = mat3(u_ViewProjection) * a_Normal;

	v_TexCoord = a_TexCoord;
}


#shadertype pixel
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
in vec3 v_Normal;

uniform vec4 u_Color;
uniform vec3 u_LightDirection;
//uniform sampler2D u_Texture1;
//uniform sampler2D u_Texture2;


void main()
{
//	o_Color = u_Color;

	vec3 ambient = vec3(0.2, 0.2, 0.2);

	float diff = max(dot(v_Normal, u_LightDirection), 0.0);
	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

	vec3 ibl = ambient + diffuse;

//	o_Color = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.5);
//	o_Color = texture(u_Texture1, v_TexCoord);

	o_Color = vec4(ibl, 1.0);

//	o_Color = vec4(0.2, 0.35, 0.75, 1.0);
}

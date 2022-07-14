#shader vertex
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Tex_Coord;
layout(location = 3) in float a_Tex_Index;

uniform mat4 u_View_Proj;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_Tex_Coord;
out float v_Tex_Index;

void main()
{
	v_Color = a_Color;
	v_Tex_Coord = a_Tex_Coord;
	v_Tex_Index = a_Tex_Index;
	gl_Position = u_View_Proj * u_Transform * vec4(a_Position, 1.0, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) = out vec4 o_Color;

int vec4 v_Color;
in vec2 v_Tex_Coord;
in float v_Tex_Index;

uniform sampler_2D u_Textures[32];

void main()
{
	int index = int(v_Tex_Index);
	o_Color = texture(u_Textures[index], v_Tex_Coord) * v_Color;
}
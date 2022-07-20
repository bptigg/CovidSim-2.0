#shader vertex
#version 460 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in float a_tex_index;
layout(location = 3) in vec4 a_text_color;

out vec2 v_Tex_Coord;
out vec4 v_Text_Colour;
out float v_Tex_Index;

uniform mat4 u_MVP;
uniform vec4 u_Zoom;

void main()
{
	gl_Position = u_MVP * (u_Zoom * a_position);
	v_Tex_Coord = a_tex_coord;
	v_Text_Colour = a_text_color;
	v_Tex_Index = a_tex_index;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 o_color;

in vec2 v_Tex_Coord;
in vec4 v_Text_Colour;
in float v_Tex_Index;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(v_Tex_Index);
	o_color = vec4(v_Text_Colour.r, v_Text_Colour.g, v_Text_Colour.b, v_Text_Colour.a * texture(u_Textures[index], v_Tex_Coord).r);
}
